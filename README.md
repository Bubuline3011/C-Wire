# Projet-INFO2 : C-Wire
## Table des Matières
1. [Description](#description)
2. [Structure](#structure)
3. [Installation](#installation)
4. [Utilisation](#utilisation)
6. [Auteurs](#auteurs)

## Description 

Ce projet a pour objectif de développer une application qui analyse les données d'un système de distribution d'énergie en France, modélisé sous forme d'un réseau hiérarchique (arbre enraciné). Le projet est divisé en deux parties principales :
Le programme permet de :
   - Filtrer les données en fonction du type de station (HVB, HVA, LV) et du type de consommateur (entreprise, particulier, ou tous).
   - Calculer la somme des consommations des stations et détecter les surcharges ou sous-utilisations grâce à un arbre AVL (arbre binaire équilibré).
   - Exporter les résultats dans des fichiers CSV structurés..

Pour plus de détails sur le projet, consultez la documentation du projet :

[📄 Projet C-Wire](Projet_C-Wire_preIng2_2024_2025-v1.4-1.pdf)


## Structure 

- **`main.c`** : contient la fonction principale
- **`projet.h`** : contient la déclaration de toutes les fonctions, les structures, les constantes et les inclusions de bibliothèques.
- **`avl.c`** : contient toutes les fonctions relatives a l'avl, comme la creation, l'insertion, les différents types de rotations etc..
- **`outil.c`** : contient les fonctions "outil" comme le calcule du minimum entre deux entier.
- **`gerer_fichier.c`** : contient les fonctions qui créent les fichier de sortie et qui ecrivent dedans.
- **Makefile** : permet d'automatiser la compilation.
- **Rapport de projet C-Wire.pdf** : contient un compte rendu sur l'organisation de l'équipe concernant le projet, les difficultés rencontrées et une brève description du projet.

## Installation 

### Prérequis 

- Un terminal C

## Utilisation

Consignes avant de pouvoir utiliser le projet  : 
1. Télécharger le dossier en cliquant sur "code", puis "Download Zip".
2. Extraire le dossier.
3. Ecrire dans le terminal : **cd nom_du_dossier**.
4. Pour rendre le fichier executable, ecrire : **chmod 777 C-Wire.sh**
5. Exécuter le script shell. Syntaxe générale : **./C-Wire.sh input/<fichier_csv> <type_station> <type_consommateur> [id_centrale]**
6. Si vous avez besoin d'aide vous pouvez ecrire : **./C-Wire.sh -h**. Une page d'aide s'affichera alors.

## Auteurs

- Bulard Yona - Initial Work - Bubuline3011
- Kallel Ala - Initial Work - zquaddy
- Attia Karin - Initial Work - attiakarin

