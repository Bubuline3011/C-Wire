#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

// Définition de la structure 'Station' qui représente une station
typedef struct {
    unsigned long id; //identifiant d’une centrale électrique 
   // int load; //quantité d’énergie consommée par le consommateur final entreprise ou particulier
    long capacite;//quantité d’énergie produite par une centrale
    int som_conso; // sommes de consomation
}Station; 

// Enumération des types de stations
typedef enum {
    STATION_DEFAUT, // Défaut, équivalent à nul ou non défini
    STATION_HVB,    // Station HVB
    STATION_HVA,    // Station HVA
    STATION_LV      // Station LV
} StationType;


// Définition de la structure 'AVL' pour l'arbre AVL
typedef struct AVL{
    Station * station;
    struct AVL * fg;
    struct AVL * fd;
    int equilibre;
}AVL;

// Définition d'un type pavl pour un pointeur vers un arbre AVL
typedef AVL* pavl;
// Définition d'un type pStation pour un pointeur vers un station
typedef Station* pStation


//recuperer les donner de fichier et cree les station
pStation creerStation(){
pStation new=malloc(sizeof(Station));
if(new==NULL){
exit(3);
}
new->id=id;
new->capacite=capacite;
new->load=load;
new->centrale_id=centrale_id;
new->som_conso=0;
return new;
}








