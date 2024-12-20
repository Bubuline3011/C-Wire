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

#Verification des arguments

if [[ "$#" -lt 3 || "$*" == *"-h"* ]]; then #si l'option -h est demandé quelque soit l'endroit ou si le nombre d'argument est inferieur a trois 
	aide
	echo "temps : 0.0sec"
	exit 1
fi

chemin_fichier=$1
station=$2
conso=$3
id_centrale=$4

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
if [[ "$station" == "hva" && ( "$conso" == "all" || "$conso" == "indiv" ) ]]; then
    echo "Erreur : Les options 'hva all' et 'hva indiv' sont interdites."
    aide
    echo "temps : 0.0sec"
    exit 6
fi

if [ -z "$id_centrale" ]; then
    id_centrale=-1
fi

if ! [[ $id_centrale =~ ^[1-5]$ ]] && [[ $id_centrale != -1 ]]; then
  echo " L'identifiant de la centrale doit être un nombre positif entre 1 et 5"
  exit 8
fi

# on verfie que le fichier tmp n'existe pas et on le creer sinon ont le vide
if [ ! -d "tmp" ]; then
    echo "Le dossier 'tmp' n'existe pas. Création."
    mkdir -p tmp
else
    echo "Le dossier 'tmp' existe déjà. On le vide"
    rm -rf tmp/* #surprime tout ce qui est a l'interieur  
fi

# on verfie que le fichier graph n'existe pas et on le creer 
if [ ! -d "graphs" ]; then
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

echo "Station $station;Capacité;Consommation" > "$fichier_sortie"

#filtrage et autres

temp_debut=$(date +%s.%N) # prend l'heure du début

echo "Filtrage des données en cours...."

if [ ! -w /tmp ]; then
    echo "Erreur : Pas de permission pour écrire dans /tmp."
    exit 10
fi

echo "Station est : $station"
echo "Station est : $conso"
echo "fichier est : $chemin_fichier"


# Filtrage des données en fonction des paramètres 
#Filtrage par numéro de centrale si il y'en a une 
if [[ "$id_centrale" -ne -1 ]]; then
    grep -E "^$id_centrale;" "$chemin_fichier" > "tmp/centrale_filtrees.csv"

    if [[ ! -s "tmp/centrale_filtrees.csv" ]]; then
        echo "Erreur : Aucune donnée trouvée pour la centrale $id_centrale."
        exit 13
    fi
else 
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

grep -E "$filtre" "tmp/centrale_filtrees.csv" | cut -d ';' -f $colonnes | tr '-' '0' > "tmp/donnees_filtrees.csv"

# Vérification après écriture
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
#touch tmp/fichier_tmp_result.csv # Crée un fichier vide nommé fichier_tmp_result dans tmp
executable="./codeC/exec" # L'exécutable est dans le dossier codeC
if [ ! -x "$executable" ]; then # Si le fichier est executable
    echo "Compilation du programme C..."
    make -C codeC clean # Nettoie les fichiers précédents
    make -C codeC all # Exécute le Makefile dans le dossier codeC
    if [ $? -ne 0 ]; then # Si la compilation échoue, on affiche une erreur
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

if [ -e lv_all.csv ]; then
	# Calculer la différence (Capacité - Consommation) et trier
	awk -F';' -v OFS=';' \
    	'NR > 1 {diff = $7 - $8; print $1, $7, diff}' lv_all.csv | sort -t';' -k3,3n > tmp/lv_all_sorted.csv

	# Extraire les 10 postes avec la plus petite et la plus grande différence
	head -n 10 tmp/lv_all_sorted.csv > tmp/lv_all_min.csv
	tail -n 10 tmp/lv_all_sorted.csv > tmp/lv_all_max.csv

	# Fusionner les résultats et créer le fichier final
	touch lv_all_minmax.csv
	echo "Station LV;Capacité;Différence" > lv_all_minmax.csv
	cat tmp/lv_all_min.csv tmp/lv_all_max.csv | sort -t';' -k1,1n >> lv_all_minmax.csv

	# Nettoyer les fichiers temporaires
	rm -f tmp/lv_all_sorted.csv tmp/lv_all_min.csv tmp/lv_all_max.csv
fi

if [ -e "lv_all_$id_centrale.csv" ]; then
	# Calculer la différence (Capacité - Consommation) et trier
	awk -F';' -v OFS=';' \
    	'NR > 1 {diff = $6 - $7; print $1, $6, diff}' "lv_all_$id_centrale.csv" | sort -t';' -k3,3n > "tmp/lv_all_sorted_$id_centrale.csv"

	# Extraire les 10 postes avec la plus petite et la plus grande différence
	head -n 10 "tmp/lv_all_sorted_$id_centrale.csv" > "tmp/lv_all_min_$id_centrale.csv"
	tail -n 10 "tmp/lv_all_sorted_$id_centrale.csv" > "tmp/lv_all_max_$id_centrale.csv"

	# Fusionner les résultats et créer le fichier final
	touch "lv_all_minmax_$id_centrale.csv"
	echo "Station LV;Capacité;Différence" > "lv_all_minmax_$id_centrale.csv"
	cat "tmp/lv_all_min_$id_centrale.csv" "tmp/lv_all_max_$id_centrale.csv" | sort -t';' -k1,1n >> "lv_all_minmax_$id_centrale.csv"

	# Nettoyer les fichiers temporaires
	rm -f "tmp/lv_all_sorted_$id_centrale.csv" "tmp/lv_all_min_$id_centrale.csv" "tmp/lv_all_max_$id_centrale.csv"
fi

#Partie bonus :
# Vérifier si Gnuplot est installé
if ! command -v gnuplot &> /dev/null; then
    echo "Erreur : Gnuplot n'est pas installé."
    exit 1
fi

# Génération du graphique
gnuplot << gnuplot
	set terminal png size 800,600
	set output "graphs/bonus.png"
	set title "Postes LV - Consommation et Capacité"
	set xlabel "Station"
	set ylabel "Différence (Capacité - Consommation)"
	set grid
	set style data histogram
	set style histogram cluster gap 1
	set style fill solid 1.00 border -1
	set boxwidth 0.75
	set xtics rotate by -45
	set key outside

# Définir les couleurs pour les barres
	set palette model RGB defined ( 0 "green", 1 "red" )

plot "$fichier_sortie" using 2:xtic(1) linecolor palette frac (column(2)<0?1:0) with boxes
gnuplot
#ecrire : si il existe lv_all_minmax.csv on fait le trux en dessous et pareill pour lv_all_minmax_$id_centrale.csv
echo "Graphique généré : graphs/bonus.png"

make clean

echo "FIN du script"

