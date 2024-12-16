#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Définition de la structure 'Station' qui représente une station
typedef struct {
    int id; //identifiant d’une centrale électrique 
    int load; //quantité d’énergie consommée par le consommateur final entreprise ou particulier
    int capacite;//quantité d’énergie produite par une centrale
    int som_conso; // sommes de consomation
}Station; 

// Définition de la structure 'AVL' pour l'arbre AVL
typedef struct AVL{
    Station * station;
    struct AVL * fg;
    struct AVL * fd;
    int equilibre;
}AVL;

// Définition d'un type pavl pour un pointeur vers un arbre AVL
typedef AVL* pavl;

int max(int a, int b);
int min(int a, int b);
int min3(int a, int b, int c);
int max3(int a, int b, int c);
pavl creerAvl(Station * a);
pavl rotationGauche(pavl a);
pavl rotationDroite(pavl a);
pavl doubleRotationDroite(pavl a);
pavl doubleRotationGauche(pavl a);
pavl equilibreAVL(pavl * a);
pavl insertionAVL(pavl a, Station * e, int * h);
pavl suppMinAVL(pavl* a, int * h, int * pe);
pavl suppressionAVL(pavl * a, int * h, int e);
void sommeConso(pavl a);
void ecrireFichierSortie(const char *nomFichier, pavl a);
void libererAVL(pavl a);
pavl lireFichierCSV(const char *nomFichier, pavl racine);
