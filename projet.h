#include <stdio.h>
#include<stdlib.h>


typedef struct station {
    char type_station[10];
    int id; //identifiant d’une centrale électrique
   /* int id_B; //identifiant d’une station HV-B
    int id_A; //identifiant d’une station HV-A
    int id_LV; //identifiant d’un poste LV
    int id_e; //identifiant d’une entreprise (consommateur)
    int id_p; //identifiant d’un particulier (consommateur) */
    int load; //quantité d’énergie consommée par le consommateur final entreprise ou particulier
    int capacite;//quantité d’énergie produite par une centrale
    int som_conso; // sommes de consomation
    
typedef struct AVL{
    Station * station;
    struct AVL * fg;
    struct AVL * fd;
    int eq;
}AVL;


typedef AVL* pavl;

int hateur(Pavl n);
pavl CreerAvl(Station * station);pavl  rotationGauche(pavl a);
pavl  rotationGauche(pavl a);
pavl rotationDroite(pavl a);
pavl doubleRotationGauche(pavl a);
pavl doubleRotationDroite(pavl a);
pavl insertionAVL(pavl a, Station* e, int * h);


