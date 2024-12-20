#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#define MAX_LIGNE 256 // Taille maximale d'une ligne

// Définition de la structure 'Station' qui représente une station
typedef struct { //on cree cette structure pour essayer d'optimiser 
    int id; //identifiant d’une centrale électrique 
    long conso; //quantité d’énergie consommée par le consommateur final entreprise ou particulier
    long capacite;//quantité d’énergie produite par une centrale
}Station; 

// Définition de la structure 'AVL' pour l'arbre AVL
typedef struct AVL{
    Station * station;
    struct AVL * fg; //pointeur vers fils gauche
    struct AVL * fd; // pointeur vers fils droit
    int equilibre; // facteur d'equilibre
}AVL;

// Définition d'un type pavl pour un pointeur vers un arbre AVL
typedef AVL* pavl;

int max(int a, int b);
int min(int a, int b);
int min3(int a, int b, int c);
int max3(int a, int b, int c);
pavl creerAvl(int id, long capacite, long conso);
pavl rotationGauche(pavl a);
pavl rotationDroite(pavl a);
pavl doubleRotationDroite(pavl a);
pavl doubleRotationGauche(pavl a);
pavl equilibreAVL(pavl a);
pavl insertionAVL(pavl a, int id, long capacite, long conso, int * h);
int recherche(pavl noeud, int id);
void supprimer_AVL(pavl a);
void recuperer_info_csv(pavl * a);
void parcoursInfixe(pavl a, FILE* f);
void ecrire_ds_fichier_result_tmp(pavl a);
