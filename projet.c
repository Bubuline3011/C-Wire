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

int hateur(Pavl n){
  if(n==NULL){
      return 0; // l'arbre est vide son hauteur = 0
  }else{
      return->eq; // renvoyer la valeur d'hauter
         }
  }

Pavl CreerAvl(Station * station){
Pavl arbre =malloc(sizeof(Pavl):
if(arbre==NULL){
   exit(1); //Allocation echouée
   }
   
arbre->station->station;
arbre->fg=NULL;
arbre->fd=NULL;
arbre->eq=;
}

pavl  rotationGauche(pavl a){
    pavl pivot;
    int eq_a, eq_p;
    pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg = a;
    eq_a = a->equilibre;
    eq_p = pivot->equilibre;
    a->equilibre = eq_a - max(eq_p, 0) -1;
    pivot->equilibre = min(eq_a-2, eq_a + eq_p -2, eq_p -1);
    a = pivot;
    return a;
}

pavl rotationDroite(pavl a){
    pavl pivot;
    int eq_a, eq_p;
    pivot = a->fg;
    a->fg = pivot->fd;
    pivot->fd = a;
    eq_a = a->equilibre;
    eq_p = pivot->equilibre;
    a->equilibre = eq_a - min2(eq_p, 0) +1;
    pivot->equilibre = max2(eq_a+2, eq_a + eq_p -2, eq_p +1);
    a = pivot;
    return a;
}

pavl doubleRotationGauche(pavl a){
    a->fd = rotationDroite(a->fd);
    return rotationGauche(a);
}

pavl doubleRotationDroite(pavl a){
    a->fg = rotationGauche(a->fg);
    return rotationDroite(a);
}

pavl insertionAVL(pavl a, int e, int * h){
    if(a== NULL){
        *h = 1;
        return creerArbre(e);
    }
    else if(e< a->elmt){
        a->fg = insertionAVL(a->fg, e, h);
        *h = -*h;
    }
    else if(e>a->elmt){
        a->fd = insertionAVL(a->fd, e, h);
    }
    else {
        *h = 0;
        return a;
    }
    if(*h != 0){
        a->equilibre = a->equilibre + *h;
        a = equilibrerAVL(a);
        if(a->equilibre == 0){
            *h = 0;
        }
        else {
            *h = 0;
        }
    }
    return a;
}






























