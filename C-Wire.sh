#!/bin/bash

aide(){
	echo "Le script premet de filtrer les donneés d'un fichier, crée un fichier qui contient une liste avec toutes les stations et d'autres inforations (somme des consommateurs etc)"
	echo " $0 <chemin_csv> <type_station> <type_conso> [<id_centrale>][-h]" # les crochets sont utilisés pour dire que largument est optionnel
	echo "Paramètres :"
    	echo "  <chemin_csv>       :  Chemin vers le fichier de données CSV (obligatoire)"
    	echo "  <type_station>     : Type de station à traiter : hvb, hva ou lv (obligatoire)"
    	echo "  <type_conso>       : Type de consommateur à analyser : comp, indiv, all (obligatoire)"
    	echo "Restrictions :"
    	echo "   hvb  		   : seulement comp (entreprises)"
    	echo "   hva 		   : seulement comp (entreprises)"
    	echo "   lv  		   : comp, indiv (particuliers) ou all (tous)"
    	echo "  <id_centrale>      : Identifiant de la centrale (optionnel)"
    	echo "Options interdites :"
    	echo " hvb all, hvb indiv, hva all, hva indiv"
    	echo "  -h                 : Affiche cette aide (prioritaire)"
} 

if [[ "$#" -lt 3 ] || [[ "$*" == *"-h"* ]]; then #si l'option -h est demandé quelque soit l'endroit ou si le nombre d'argument est inferieur a trois 
	aide
	exit 1
}

chemin_fichier=$1
station=$2
conso=$3
id_centrale=$4

if [ ! -f "$chemin_fichier" ]; then
    echo "Erreur : Le fichier CSV '$chemin_fichier' est n'existe pas."
    aide
    echo "temps : 0.0sec"
    exit 2
fi

if [[ "$station" != "hvb" && "$station" != "hva" && "$station" != "lv" ]]; then
    echo "Erreur : La station saisie n'existe pas. Valeurs possibles : hvb, hva, lv."
    aide
    echo "temps : 0.0sec"
    exit 3
fi

if [[ "$conso" != "comp" && "$conso" != "indiv" && "$conso" != "all" ]]; then
    echo "Erreur : Le consommateur saisie n'existe pas. Valeurs possibles : comp, indiv, all."
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

if [ ! -d "tmp" ]; then
    echo "Le dossier 'tmp' n'existe pas. Création du dossier."
    mkdir -p tmp
else
    echo "Le dossier 'tmp' existe déjà. Il sera vidé."
    rm -rf tmp/* #surprime tout ce qui est a l'interieur  
fi

# Vérification et création du dossier graphs
if [ ! -d "graphs" ]; then
    echo "Le dossier 'graphs' n'existe pas. Création du dossier."
    mkdir -p graphs
else
    echo "Le dossier 'graphs' existe déjà."
fi

debut = $(date + %s)

#faire le scrpit avec le filtrage


fin = $(date + %s)


duree = $(debut - fin)
echo "temps : $dureesec"




