#include <stdio.h>
#include<stdlib.h>

// Définition de la structure 'Station' qui représente une station
typedef struct station {
    char type_station[10]; // Type de la station (hvb, hva, lv, etc.)
    int id; //identifiant d’une centrale électrique 
   /* int id_B; //identifiant d’une station HV-B
    int id_A; //identifiant d’une station HV-A
    int id_LV; //identifiant d’un poste LV
    int id_e; //identifiant d’une entreprise (consommateur)
    int id_p; //identifiant d’un particulier (consommateur) */
    int load; //quantité d’énergie consommée par le consommateur final entreprise ou particulier
    int capacite;//quantité d’énergie produite par une centrale
    int som_conso; // sommes de consomation
    
}Station; 

// Définition de la structure 'AVL' pour l'arbre AVL
typedef struct AVL{
    Station * station;
    struct AVL * fg;
    struct AVL * fd;
    int eq;
}AVL;

// Définition d'un type pavl pour un pointeur vers un arbre AVL
typedef AVL* pavl;

// Fonction pour renvoyer le maximum entre deux valeurs
int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Fonction pour renvoyer le minimum entre deux valeurs
int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

// Fonction pour renvoyer le minimum parmi trois valeurs
int min3(int a, int b, int c) {
    int min_ab = min(a, b);  // Trouver le minimum entre `a` et `b`
    return min(min_ab, c);   // Comparer ce minimum avec `c`
}

// Fonction pour renvoyer le maximum parmi trois valeurs
int max3(int a, int b, int c) {
    int max_ab = max(a, b);  // Trouver le maximum entre `a` et `b`
    return max(max_ab, c);   // Comparer ce maximum avec `c`
}

// Fonction pour créer un arbre AVL avec une station donnée
pavl CreerAvl(Station * station){
pavl arbre =malloc(sizeof(Pavl); // Allocation dynamique pour un nouvel arbre AVL
if(arbre==NULL){
   exit(1); //Allocation échouée
   }
arbre->station->station; // Associe la station à cet arbre
arbre->fg=NULL; // Le sous-arbre gauche est initialement NULL
arbre->fd=NULL; // Le sous-arbre droit est initialement NULL
arbre->eq=0; L'équilibre initial de l'arbre est 0 (arbre vide)
    return arbre;
}

// Fonction de rotation à gauche pour rééquilibrer l'arbre AVL
pavl rotationGauche(pavl a) {
   pavl pivot = a->fd; // Le fils droit devient le pivot
    int eq_a = a->eq
    eq_p = pivot->eq;
    a->fd = pivot->fg; // Le sous-arbre gauche du pivot devient le fils droit de `a`
    pivot->fg = a;     // `a` devient le fils gauche du pivot

    // Mise à jour des facteurs d'équilibre
    a->eq = eq_a - max(eq_p, 0) - 1;
    pivot->eq = min3(eq_a - 2, eq_a + eq_p - 2, eq_p - 1);

    return pivot; // Le pivot devient la nouvelle racine
}

// Fonction de rotation à droite pour rééquilibrer l'arbre AVL
pavl rotationDroite(pavl a) {
  pavl pivot = a->fg; // Le fils gauche devient le pivot
    int eq_a = a->eq;
    eq_p = pivot->eq;
    a->fg = pivot->fd; // Le sous-arbre droit du pivot devient le fils gauche de `a`
    pivot->fd = a;     // `a` devient le fils droit du pivot

    // Mise à jour des facteurs d'équilibre
    a->eq = eq_a - min(eq_p, 0) + 1;
    pivot->eq = max3(eq_a + 2, eq_a + eq_p + 2, eq_p + 1);

    return pivot; // Le pivot devient la nouvelle racine
}

// Fonction de double rotation à droite pour rééquilibrer l'arbre AVL
pavl doubleRotationDroite(pavl a) {
    a->fg = rotationGauche(a->fg);  // Effectuer une rotation à gauche sur le sous-arbre gauche
    return rotationDroite(a);       // Ensuite, effectuer une rotation à droite
}

// Fonction de double rotation à gauche pour rééquilibrer l'arbre AVL
pavl doubleRotationGauche(pavl a) {
    a->fd = rotationDroite(a->fd);  // Effectuer une rotation à droite sur le sous-arbre droit
    return rotationGauche(a);       // Ensuite, effectuer une rotation à gauche
}

// Fonction d'insertion d'une station dans l'arbre AVL avec rééquilibrage
pavl insertionAVL(pavl a, Station* e, int *h) {
    if (a == NULL) {
        *h = 1;                      // Si l'arbre est vide, on crée un nouvel arbre
        return CreerAvl(e);           // Créer un nouvel arbre avec la station
    } 
    else if (e < a->station) {         // Si la station est inférieure à la station actuelle
        a->fg = insertionAVL(a->fg, e, h); // Insertion récursive dans le sous-arbre gauche
        *h = -(*h);                  // Inverser l'état de hauteur
    } 
    else if (e > a->station) {         // Si la station est supérieure à la station actuelle
        a->fd = insertionAVL(a->fd, e, h); // Insertion récursive dans le sous-arbre droit
    } 
    else {                            // Si la station est déjà présente, on ne l'insère pas
        *h = 0;
        return a;
    }

    if (*h != 0) {                     // Si l'équilibre n'est pas nul
        a->eq = a->eq + *h;            // Ajuster l'équilibre de l'arbre
        a = equilibrerAVL(a);          // Équilibrer l'arbre après insertion
        if (a->eq == 0) {              // Si l'arbre est équilibré après insertion
            *h = 0;
        }
        else {
            *h = 0;                   // Sinon, l'arbre reste déséquilibré
        }
    }
    return a;
}





























