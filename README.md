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
Ci-après le sujet du projet : (Rapport de projet C-Wire.pdf)

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
1. Télécharger tous les fichiers en cliquant sur chacun, excepté le rapport de projet et le README.
2. Mettre tous les fichiers dans un dossier si vous le souhaitez.
3. Si vous l’avez mis dans un dossier, écrire : **cd nom_du_fichier**.
4. Exécuter le script shell. Syntaxe générale : **./C-Wire.sh <fichier_csv> <type_station> <type_consommateur> [id_centrale]**
5. Si vous avez besoin d'aide vous pouvez ecrire : **./C-Wire.sh -h**. Une page d'aide s'affichera alors.

## Auteurs

- Bulard Yona - Initial Work - Bubuline3011
- Kallel Ala - Initial Work - zquaddy
- Attia Karin - Initial Work - attiakatin

