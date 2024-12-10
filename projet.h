#include <stdio.h>
#include<stdlib.h>


typedef struct station {
    int identifiant;
    long capacite;
    long som_conso;
}Station; 

typedef struct AVL{
    Station * station;
    struct AVL * fg;
    struct AVL * fd;
    int eq;
}AVL;


typedef AVL* pavl;

int hateur(Pavl n);
Pavl CreerAvl(Station * station);pavl  rotationGauche(pavl a);
pavl  rotationGauche(pavl a);
pavl rotationDroite(pavl a);
pavl doubleRotationGauche(pavl a);
pavl doubleRotationDroite(pavl a);
pavl insertionAVL(pavl a, Station* e, int * h);


