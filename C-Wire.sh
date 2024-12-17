#!/bin/bash

chmod 777 C-Wire.sh

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
  exit 7
fi


# Vérification de l'exécutable C

executable="./exec" #explique le chemin et pour trouver l'executable qui se trouve dans le meme repertoire que le script shell
if [ ! -f "$executable" ]; then #si l'executable n'existe pas
    echo "Compilation du programme C..."
    make all # penser a appeler le makefile C-Wire 
    if [ $? -ne 0 ]; then # si le make a échouer on affiche une erreur 
        echo "Erreur : Échec de la compilation du programme C."
        echo "temps : 0.0sec"
        exit 7
    fi
    echo "Compilation reussie"
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



#filtrage et autres

mesurer_temps() {  #sert a connaitre le temps d'execution
    local temp_debut=$(date +%s.%N) # prend l'heure du début
    "$@"
    local temp_fin=$(date +%s.%N) # prend l'heure de fin
    local temp_ecoule=$(awk "BEGIN {print $temp_fin - $temp_debut}") # on fait la différence pour avoir le temps total d'execution
    printf "Temps d'exécution : %.3f sec\n" "$temp_ecoule"
}

# Fonction de filtrage
filtrer_copier() {
    local fichier_entrer="$1"
    local station_type="$2"
    local consommateur_type="$3"
    local central_id="$4"
    local fichier_sortie="tmp/donne_filtrer.dat"

    echo "Filtrage des données en fonction du type de station, du type de consommateur et de l'ID de la centrale..."
    
# Déterminer les indices de colonnes en fonction du type de station
    local station_index consommateur_index
    case "$station_type" in
        hvb) station_index=2 ;;
        hva) station_index=3 ;;
        lv) station_index=4 ;;
    esac
# Indices des colonnes en fonction du type de consommateur
    case "$consommateur_type" in
        comp) consommateur_index=5 ;;
        indiv) consommateur_index=6 ;;
        all) consommateur_index="all" ;;
    esac

    # En fonction du type de consommateur, définir les colonnes pour les filtrages
    if [[ "$consommateur_index" == "all" ]]; then
        # Si le type est "all", on ne filtre pas par consommateur
        awk -v station_idx=$station_index -v central_id=$central_id 'BEGIN { FS=";"; OFS=";" } NR==1 || ($station_idx == central_id) { print $0 }' "$fichier_entrer" > "$fichier_sortie"
    else
        # Si un type spécifique est donné, on filtre également sur la colonne du consommateur
        awk -v station_idx=$station_index -v cons_idx=$consommateur_index -v central_id=$central_id 'BEGIN { FS=";"; OFS=";" } NR==1 || ($station_idx == central_id && $cons_idx != "") { print $0 }' "$fichier_entrer" > "$fichier_sortie"
    fi


    echo "Données filtrées copiées dans : $fichier_sortie"
}

# Fonction pour calculer la différence de consommation
calculer_diff_consommation() {
    local fichier_entrer="$1"
    local fichier_sortie="tmp/lv_all_minmax.csv"

    echo "Calcul de la différence de consommation (capacité - consommation) et tri des 10 postes les plus et les moins chargés..."

    awk -F';' 'NR > 1 { 
        consommation = $5;
        capacite = $7;
        difference = capacite - consommation;
        print $0 ";" difference
    }' "$fichier_entrer" | sort -t';' -k8n | head -n 10 > "$fichier_sortie"

    awk -F';' 'NR > 1 { 
        consommation = $5;
        capacite = $7;
        difference = capacite - consommation;
        print $0 ";" difference
    }' "$fichier_entrer" | sort -t';' -k8n | tail -n 10 >> "$fichier_sortie"

    echo "Les 10 postes les plus et les moins chargés ont été enregistrés dans : $fichier_sortie"
}

# Mesurer et exécuter le filtrage
mesurer_temps filtrer_copier "$chemin_fichier" "$station" "$conso" "$id_centrale"

# Vérification si lv_all.csv existe avant de tenter de le trier
if [ ! -f "tmp/donne_filtrer.dat" ]; then
    echo "Erreur : Le fichier filtré n'a pas été généré. Vérifiez le filtrage."
    exit 8
fi

# Si 'lv all', calculer la différence de consommation
if [[ "$station" == "lv" && "$conso" == "all" ]]; then
    calculer_diff_consommation "tmp/donne_filtrer.dat"
fi

# Vérification si lv_all_minmax.csv existe avant de tenter de le trier
if [ ! -f "tmp/lv_all_minmax.csv" ]; then
    echo "Erreur : Le fichier lv_all_minmax.csv n'a pas été généré. Vérifiez le traitement."
    exit 9
fi

# Trier les résultats pour les 10 postes les plus et les moins chargés
# Pour les 10 plus chargés (max)
sort -t';' -k5n tmp/lv_all.csv | tail -n 10 > graphs/lv_all_max.csv #tail permet d'afficher les dernières lignes du fichier lv_all_max car c'est trier dans l'ordre croissant 

# Pour les 10 moins chargés (min)
sort -t';' -k5n tmp/lv_all.csv | head -n 10 > graphs/lv_all_min.csv #head permet d'afficher les premières lignes du fichier lv_all_max car c'est trier dans l'ordre croissant 

echo "FIN du script"



