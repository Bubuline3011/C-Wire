#!/bin/bash

#Fonction pour aider l'utilisateur
aide(){
	echo "Le script premet de filtrer les donneés d'un fichier, crée un fichier qui contient une liste avec toutes les stations et d'autres inforations (somme des consommateurs etc)"
	echo " $0 <chemin_csv> <type_station> <type_conso> [<id_centrale>][-h]" # les crochets sont utilisés pour dire que largument est optionnel
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

if [[ "$#" -lt 3 ]] || [[ "$*" == *"-h"* ]]; then #si l'option -h est demandé quelque soit l'endroit ou si le nombre d'argument est inferieur a trois 
	aide
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

#verifier si le type de station  et le consommateur ecrit est valide

if [[ "$station" != "hvb" && "$station" != "hva" && "$station" != "lv" ]]; then
    echo "Erreur : La station n'existe pas. Valeurs possibles : hvb, hva, lv."
    aide
    echo "temps : 0.0sec"
    exit 3
fi

if [[ "$conso" != "comp" && "$conso" != "indiv" && "$conso" != "all" ]]; then
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

# Vérification de l'exécutable C

executable="./C-Wire_pg" #explique le chemin et pour trouver l'executable qui se trouve dans le meme repertoire que le script shell
if [ ! -f "$executable" ]; then #si l'executable n'existe pas
    echo "Compilation du programme C..."
    make -C C-Wire # penser a appeler le makefile C-Wire 
    if [ $? -ne 0 ]; then # si le make a échouer on affiche une erreur 
        echo "Erreur : Échec de la compilation du programme C."
        echo "temps : 0.0sec"
        exit 7
    fi
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

debut=$(date +%s) #sert a connaitre le temps d'execution : ici ça prend l'heure du début

#filtrage

filtre_centrale=""
if [[ ! -z "$id_centrale" ]]; then
    filtre_centrale="&& \$1 == \"$id_centrale\""
fi

# Filtrage et traitement selon les paramètres
if [[ "$station" == "hvb" ]] && [[ "$conso" == "comp" ]]; then # Filtrer les données pour les stations hvb avec des entreprises, passer au programme C, et enregistrer dans hvb_comp.csv.
    awk -F ';' "\$2 != \"-\" && \$4 == \"-\" && \$7 == \"-\" $filtre_centrale" "$chemin_fichier" |
        cut -d';' -f2,5,7,8 | tr '-' '0' | $executable > hvb_comp.csv
    sort -t ':' -k2 -n hvb_comp.csv -o hvb_comp.csv #trier par ordre croissant
fi

if [[ "$station" == "hva" ]] && [[ "$conso" == "comp" ]]; then # Filtrer les données pour les stations HVA avec entreprises, passer au programme C, et enregistrer dans hva_comp.csv.
	awk -F ';' "\$3 != \"-\" && \$5 == \"-\" && \$7 == \"-\" $filtre_centrale" "$chemin_fichier" |cut -d';' -f3,5,7,8 | tr '-' '0' | $executable > hva_comp.csv
        sort -t ':' -k2 -n hva_comp.csv -o hva_comp.csv 
fi

if [[ "$station" == "lv" ]] && [[ "$conso" == "comp" ]]; then # Filtrer les données pour les stations LV avec des entreprises, passer au programme C, et enregistrer dans lv_comp.csv.
	awk -F ';' "\$4 != \"-\" && \$5 == \"-\" && \$7 == \"-\" $filtre_centrale" "$chemin_fichier" |cut -d';' -f4,5,7,8 | tr '-' '0' | $executable > lv_comp.csv
	sort -t ':' -k2 -n lv_comp.csv -o lv_comp.csv
fi

if [[ "$station" == "lv" ]] && [[ "$conso" == "indiv" ]]; then # Filtrer les données pour les stations LV avec des consommateurs individuels, passer au programme C, et enregistrer dans lv_indiv.csv.
	awk -F ';' "\$4 != \"-\" && \$6 != \"-\" && \$7 == \"-\" $filtre_centrale" "$chemin_fichier" |cut -d';' -f4,6,7,8 | tr '-' '0' | $executable > lv_indiv.csv
	sort -t ':' -k2 -n lv_indiv.csv -o lv_indiv.csv
fi

if [[ "$station" == "lv" ]] && [[ "$conso" == "all" ]]; then # Filtrer les données pour les stations LV avec tous les types de consommateurs (entreprises et particuliers), passer au programme C, et enregistrer dans tmp/lv_all.csv.
	awk -F ';' "\$4 != \"-\" && (\$5 != \"-\" || \$6 != \"-\") $filtre_centrale" "$chemin_fichier" |cut -d';' -f4,5,6,7 | tr '-' '0' | $executable > tmp/lv_all.csv
	sort -t ':' -k2 -n tmp/lv_all.csv -o tmp/lv_all.csv
fi

# Trier les résultats pour les 10 postes les plus et les moins chargés
# Pour les 10 plus chargés (max)
sort -t';' -k5n tmp/lv_all.csv | tail -n 10 > lv_all_max.csv #tail permet d'afficher les dernières lignes du fichier lv_all_max car c'est trier dans l'ordre croissant 

# Pour les 10 moins chargés (min)
sort -t';' -k5n tmp/lv_all.csv | head -n 10 > lv_all_min.csv #head permet d'afficher les premières lignes du fichier lv_all_max car c'est trier dans l'ordre croissant 

fin=$(date +%s) #prend l'heure a la fin du filtrage
duree=$((fin - debut)) # on fait la différence pour avoir le temps total d'execution
echo "temps : $duree sec" # on affiche ce temps
echo "FIN du script"




