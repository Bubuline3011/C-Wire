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
    	echo "  -h                 : Affiche cette aide (prioritaire)"
} 

if [[ "$#" -lt 3 ] || [[ "$*" == *"-h"* ]]; then #si l'option -h est demandé quelque soit l'endroit ou si le nombre d'argument est inferieur a trois 
	aide
	exit 1
fi

chemin_fichier=$1
station=$2
conso=$3
id_centrale=$4

if [ ! -f "$chemin_fichier" ]; then
    echo "Erreur : fichier introuvable"
    aide
    echo "temps : 0.0sec"
    exit 2
fi

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

if [ ! -d "tmp" ]; then
    echo "Le dossier 'tmp' n'existe pas. Création."
    mkdir -p tmp
else
    echo "Le dossier 'tmp' existe déjà. On le vide"
    rm -rf tmp/* #surprime tout ce qui est a l'interieur  
fi

# Vérification et création du dossier graphs
if [ ! -d "graphs" ]; then
    echo "Le dossier 'graphs' n'existe pas. Création."
    mkdir -p graphs
else
    echo "Le dossier 'graphs' existe déjà."
fi

debut=$(date + %s) #sert a connaitre le temps d'execution : ici ça prend l'heure du début

#filtrage
gcc -o projet projet.c

if [[ "$2" == "hvb" ]] && [[ "$3" == "comp" ]]; then # si l'utilisateur rentre hvb comp
	

fi 

if [[ "$2" == "hva" ]] && [[ "$3" == "comp" ]]; then #si l'utilisateur ecrit hva comp
	
fi 

if [[ "$2" == "lv" ]] && [[ "$3" == "comp" ]]; then # si l'utilisateur rentre lv comp
	

fi 

if [[ "$2" == "lv" ]] && [[ "$3" == "indiv" ]]; then # si l'utilisateur rentre lv indiv
	

fi 

if [[ "$2" == "lv" ]] && [[ "$3" == "all" ]]; then # si l'utilisateur rentre lv all
	

fi 

# il manque le cas ou l'utilsateur met une option avec l'identifiant de la central

fin=$(date + %s) #prend l'heure a la fin du filtrage


duree=$(debut - fin) # on fait la différence pour avoir le temps total d'execution
echo "temps : $duree sec" # on affiche ce temps





