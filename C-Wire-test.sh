#!/bin/bash

#Fonction pour aider l'utilisateur
aide(){
	echo "Le script premet de filtrer les donneés d'un fichier, crée un fichier qui contient une liste avec toutes les stations et d'autres inforations (somme des consommateurs etc)"
	echo " $0 <chemin_csv> <type_station> <type_conso> [<id_centrale>][-h]" # les crochets sont utilisés pour dire que l'argument est optionnel
	echo "Paramètres :"
    	echo "  <chemin_csv>       : Chemin vers le fichier de données CSV (obligatoire)"
    	echo "  <type_station>     : Type de station à traiter : hvb, hva ou lv (obligatoire)"
    	echo "  <type_conso>       : Type de consommateur à analyser : comp, indiv, all (obligatoire)"
    	echo "Type de consommateurs :"
    	echo "   hvb  		   : uniquement comp (entreprises)"
    	echo "   hva 		   : uniquement comp (entreprises)"
    	echo "   lv  		   : comp, indiv (particuliers) ou all (tous)"
    	echo "  <id_centrale>      : Identifiant de la centrale (option)"
    	echo "Options interdites :"
    	echo " hvb all, hvb indiv, hva all, hva indiv"
    	echo "  -h                 : Affiche cette aide et ignore tous les autres paramètres (optionnel)."
     	exit 0
} 

#Verification des arguments si le nombre d'argument est insuiffisant ou faut affichier la fonction d'aide

if [[ "$#" -lt 3 || "$*" == *"-h"* ]]; then #si l'option -h est demandé quelque soit l'endroit ou si le nombre d'argument est inferieur a trois 
	aide
	echo "temps : 0.0sec"
	exit 1
fi

#Initialisation des paramètres en fonction des arguments fournis

chemin_fichier=$1
station=$2
conso=$3
id_centrale=$4

# Vérification si le fichier existe

if [ ! -f "$chemin_fichier" ]; then #verifier si le fichier existe
    echo "Erreur : fichier introuvable"
    aide
    echo "temps : 0.0sec"
    exit 2
fi

#verifier si le type de station et le consommateur ecrit est valide

if [ $station != "hvb" ] && [ $station != "hva" ] && [ $station != "lv" ]; then
    echo "Erreur : La station n'existe pas. Valeurs possibles : hvb, hva, lv."
    aide
    echo "temps : 0.0sec"
    exit 3
fi

# Vérification de la validité du type de consommateur
if [ $conso != "comp" ] && [ $conso != "indiv" ] && [ $conso != "all" ]; then
    echo "Erreur : Le consommateur n'existe pas. Valeurs possibles : comp, indiv, all."
    aide
    exit 4
fi

# Vérifications des restrictions spécifiques
if [[ "$station" == "hvb" && ( "$conso" == "all" || "$conso" == "indiv" ) ]]; then
    echo "Erreur : Les options 'hvb all' et 'hvb indiv' sont interdites."
    aide
    echo "temps : 0.0sec"
    exit 5
fi

# Vérifications des restrictions spécifiques
if [[ "$station" == "hva" && ( "$conso" == "all" || "$conso" == "indiv" ) ]]; then
    echo "Erreur : Les options 'hva all' et 'hva indiv' sont interdites."
    aide
    echo "temps : 0.0sec"
    exit 6
fi

# Si aucun identifiant de centrale n'est fourni, on attribue une valeur par défaut (-1)
if [ -z "$id_centrale" ]; then
    id_centrale=-1
fi

# Validation de l'identifiant de la centrale
if ! [[ $id_centrale =~ ^[1-5]$ ]] && [[ $id_centrale != -1 ]]; then
  echo " L'identifiant de la centrale doit être un nombre positif entre 1 et 5"
  exit 8
fi

# on vérifie que le fichier tmp n'existe pas et on le créer sinon ont le vide
if [ ! -d "tmp" ]; then
    echo "Le dossier 'tmp' n'existe pas. Création."
    mkdir -p tmp
else
# vider le dossier tmp s'il existe
    echo "Le dossier 'tmp' existe déjà. On le vide"
    rm -rf tmp/* #surprime tout ce qui est a l'interieur  
fi

# on vérifie que le fichier graph n'existe pas et on le créer 
if [ ! -d "graphs" ]; then
# Création du dossier graphs s'il n'existe pas
    echo "Le dossier 'graphs' n'existe pas. Création."
    mkdir -p graphs
else
    echo "Le dossier 'graphs' existe déjà."
fi

# Création des fichiers de sortie en fonction des paramètres
fichier_sortie=""
if [[ "$id_centrale" -eq -1 ]]; then
    fichier_sortie="${station}_${conso}.csv"
else
    fichier_sortie="${station}_${conso}_${id_centrale}.csv"
fi

# Ajout de l'en-tête au fichier de sortie
echo "Station $station;Capacité;Consommation ($conso)" > "$fichier_sortie"

#filtrage et autres
temp_debut=$(date +%s.%N) # prend l'heure du début

echo "Filtrage des données en cours...."

# Vérification de l'accès en écriture dans le dossier temporaire
if [ ! -w /tmp ]; then
    echo "Erreur : Pas de permission pour écrire dans /tmp."
    exit 10
fi

echo "Station est : $station"
echo "Station est : $conso"
echo "fichier est : $chemin_fichier"


#Filtrage par numéro de centrale si applicable
if [[ "$id_centrale" -ne -1 ]]; then
    grep -E "^$id_centrale;" "$chemin_fichier" > "tmp/centrale_filtrees.csv"

    if [[ ! -s "tmp/centrale_filtrees.csv" ]]; then
        echo "Erreur : Aucune donnée trouvée pour la centrale $id_centrale."
        exit 13
    fi
else 
#Si aucun identifiant de centrale n'est spécifié
    cp "$chemin_fichier" "tmp/centrale_filtrees.csv"
fi
#Filtrage par station et consommateur
case "$station-$conso" in
    "hvb-comp")
        filtre="^[0-9]+;[0-9]+;-;-;[^;]+;-;[^;]+;[^;]+$"
        colonnes="2,7,8" ;;
    "hva-comp")
        filtre="^[0-9]+;[^;]+;[0-9]+;-;[^;]+;-;[^;]+;[^;]+$"
        colonnes="3,7,8" ;;
    "lv-comp")
        filtre="^[0-9]+;[^;]+;[^;]+;[0-9]+;[^;]+;-;[^;]+;[^;]+$"
        colonnes="4,7,8" ;;
    "lv-indiv")
        filtre="^[0-9]+;[^;]+;[^;]+;[0-9]+;-;[^;]+;[^;]+;[^;]+$"
        colonnes="4,7,8" ;;
    "lv-all")
        filtre="^[0-9]+;[^;]+;[^;]+;[0-9]+;[^;]+;[^;]+;[^;]+;[^;]+$"
        colonnes="4,7,8" ;;
    *)
        echo "Erreur : Combinaison station-consommateur non reconnue."
        exit 7 ;;
esac

# Application du filtre et extraction des colonnes pertinentes
grep -E "$filtre" "tmp/centrale_filtrees.csv" | cut -d ';' -f $colonnes | tr '-' '0' > "tmp/donnees_filtrees.csv"

# Vérification de la réussite du filtrage
if [ ! -f "tmp/donnees_filtrees.csv" ]; then
    echo "Erreur : Le fichier tmp/donnees_filtrees.csv n'a pas été généré. Arrêt."
    exit 1
fi


if [ ! -s "tmp/donnees_filtrees.csv" ]; then
    echo "Erreur : Aucune donnée filtrée."
    exit 13
fi

# Mesurer le temps

temp_fin=$(date +%s.%N) # prend l'heure de fin
temp_ecoule=$(awk "BEGIN {print $temp_fin - $temp_debut}") # on fait la différence pour avoir le temps total d'execution
printf "Temps d'exécution : %.3f sec\n" "$temp_ecoule"

# Vérification de l'exécutable C
executable="./codeC/exec" # Chemin vers l'exécutable du programme C

# Vérification si l'exécutable existe et est fonctionnel
if [ ! -x "$executable" ]; then # Si le fichier est executable
    echo "Compilation du programme C..."
    make -C codeC clean # Nettoyage des anciennes compilations
    make -C codeC all  # Compilation des fichiers avec Makefile

     # Si la compilation échoue, on affiche une erreur
    if [ $? -ne 0 ]; then 
        echo "Erreur : Échec de la compilation du programme C."
        echo "temps : 0.0sec"
        exit 7
    fi
    echo "Compilation réussie"
fi

# Exécution de l'exécutable
echo "Exécution de l'exécutable C..."
"$executable"
if [ $? -ne 0 ]; then
    echo "Erreur : L'exécution de l'exécutable a échoué."
    exit 8
fi

# Vérification du fichier de sortie temporaire généré par le programme C
if [ ! -f "tmp/fichier_tmp_result.csv" ]; then
    echo "Erreur : Le fichier 'tmp/fichier_tmp_result.csv' n'a pas été créé."
    exit 1
fi

fichier_result="tmp/fichier_tmp_result.csv"

if [ -s "$fichier_result" ]; then
    cat "$fichier_result" >> "$fichier_sortie" # Ajoute au fichier de sortie
    #rm "$fichier_result"                      # Supprime le fichier temporaire
    echo "Fichier temporaire traité avec succès et ajouté a $fichier_sortie."
else
    echo "Erreur : Le fichier temporaire '$fichier_result' est introuvable ou vide."
    exit 9
fi


echo "Fichier temporaire copié vers le fichier final : $fichier_sortie"

# On procede au trie par ordre croissant :
{
	head -n 1 "$fichier_sortie" #on conserve la permiere ligne 
	tail -n +2 "$fichier_sortie" | sort -t':' -k2,2n # on trie les lignes restantes par capacité
} > "tmp/fichier_croissant.csv"

mv "tmp/fichier_croissant.csv" "$fichier_sortie" # on remplace le fichier de sortie qui n'etait pas trié par ordre croissant de capacité par celui qui est trié par ordre croissant

echo "Le fichier a été trié par ordre croissant"

if [ -e "lv_all.csv" ]; then
    # 1 : Trier les postes par consommation uniquement
    sort -t':' -k3,3n "lv_all.csv" > "tmp/lv_all_trie_consommation.csv"

    # Extraire les 10 postes avec la consommation la plus faible
    head -n 10 "tmp/lv_all_trie_consommation.csv" > "tmp/lv_all_min_consommation.csv"

    # Extraire les 10 postes avec la consommation la plus élevée
    tail -n 10 "tmp/lv_all_trie_consommation.csv" > "tmp/lv_all_max_consommation.csv"

    # 2 : Réunir les 20 postes dans un fichier temporaire
    cat "tmp/lv_all_min_consommation.csv" "tmp/lv_all_max_consommation.csv" > "tmp/lv_all_20_selectionnes.csv"

    # 3 : Calculer la différence absolue pour ces 20 postes et trier
    awk -F':' -v OFS=':' -v OFMT="%.f" '
    NR > 1 {
        diff = $2 - $3;
        diff_abs = (diff < 0) ? -diff : diff;
        print $1, $2, $3, diff_abs
    }' "tmp/lv_all_20_selectionnes.csv" | sort -t':' -k4,4nr > "tmp/lv_all_trie_diff_abs.csv"

    # 4 : Créer le fichier final sans la colonne de différence
    {
        echo "Station LV:Capacité:Consommation (tous)"
        cut -d':' -f1-3 "tmp/lv_all_trie_diff_abs.csv"
    } > "lv_all_minmax.csv"

    echo "La création du fichier lv_all_minmax.csv a bien été réalisée"
elif [ -e "lv_all_$id_centrale.csv" ]; then
    # 1 : Trier les postes par consommation uniquement
    sort -t':' -k3,3n "lv_all_$id_centrale.csv" > "tmp/lv_all_trie_consommation.csv"

    # Extraire les 10 postes avec la consommation la plus faible
    head -n 10 "tmp/lv_all_trie_consommation.csv" > "tmp/lv_all_min_consommation.csv"

    # Extraire les 10 postes avec la consommation la plus élevée
    tail -n 10 "tmp/lv_all_trie_consommation.csv" > "tmp/lv_all_max_consommation.csv"

    # 2 : Réunir les 20 postes dans un fichier temporaire
    cat "tmp/lv_all_min_consommation.csv" "tmp/lv_all_max_consommation.csv" > "tmp/lv_all_20_selectionnes.csv"

    # 3 : Calculer la différence absolue pour ces 20 postes et trier
    awk -F':' -v OFS=':' -v OFMT="%.f" '
    NR > 1 {
        diff = $2 - $3;
        diff_abs = (diff < 0) ? -diff : diff;
        print $1, $2, $3, diff_abs
    }' "tmp/lv_all_20_selectionnes.csv" | sort -t':' -k4,4nr > "tmp/lv_all_trie_diff_abs.csv"

    # 4 : Créer le fichier final sans la colonne de différence
    {
        echo "Station LV:Capacité:Consommation (tous)"
        cut -d':' -f1-3 "tmp/lv_all_trie_diff_abs.csv"
    } > "lv_all_minmax.csv"

    echo "La création du fichier lv_all_minmax.csv a bien été réalisée"
fi

make -C codeC clean-all

#Partie bonus :
# Vérifier si Gnuplot est installé
if ! command -v gnuplot &> /dev/null; then
	echo "Erreur : Gnuplot n'est pas installé."
	exit 1
fi

# Génération du graphique
gnuplot << script_gnuplot
	set terminal pngcairo size 1200,700 enhanced font "Comic Sans MS,12"
	set output 'graphs/bonus.png'
	set title "Postes LV - Capacité et Consommation" font "Comic Sans MS,16"
	set xlabel "Poste LV" font "Comic Sans MS,12"
	set ylabel "Valeurs (kWh)" font "Comic Sans MS,12"
	set grid ytics
	set style fill solid border -1
	set boxwidth 0.4
	set key outside top center font "Comic Sans MS,10"
	set xtics rotate by -45 font "Comic Sans MS,10"
	#specifier le separateur
	set datafile separator ":"
	# Tracer les barres pour Capacité et Consommation
	plot 'lv_all_minmax.csv' using 0:2:xtic(1) with boxes lc rgb "green" title "Capacité", \
     '' using 0:3:xtic(1) with boxes lc rgb "red" title "Consommation"
script_gnuplot

echo "Graphique généré : graphs/bonus.png"

echo "FIN du script"



