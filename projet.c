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
}AVL;
