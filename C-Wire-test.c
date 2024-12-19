#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#define MAX_LIGNE 256 // Taille maximale d'une ligne

// Définition de la structure 'Station' qui représente une station
typedef struct { //on cree cette structure pour essayer d'optimiser 
    int id; //identifiant d’une centrale électrique 
    int conso; //quantité d’énergie consommée par le consommateur final entreprise ou particulier
    int capacite;//quantité d’énergie produite par une centrale
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
pavl creerAvl(int id, int capacite, int conso);
pavl rotationGauche(pavl a);
pavl rotationDroite(pavl a);
pavl doubleRotationDroite(pavl a);
pavl doubleRotationGauche(pavl a);
pavl equilibreAVL(pavl a);
pavl insertionAVL(pavl a, int id, int capacite, int conso, int * h);
int recherche(pavl noeud, int id);
void supprimer_AVL(pavl a);
void recuperer_info_csv();
void parcoursInfixe(pavl a, FILE* f);
void ecrire_ds_fichier_result_tmp(pavl a);

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
pavl creerAvl(int id, int capacite, int conso){
	if(id < 0 || capacite < 0 || conso < 0){
		printf("Erreur : les parametres fournies sont incorrecte");
		exit(1);
	}
	pavl noeud = malloc(sizeof(AVL)); // Allocation dynamique pour un nouvel arbre AVL
	if(noeud==NULL){
		printf("Erreur d'allocation memoire lors de la creation de l'AVL");
   		exit(2); //Allocation échouée
   	}
	 // Attribution de tous les champ a notre nouveau noeud de l'AVL
	noeud->station->id = id;
	noeud->station->conso = conso;
	noeud->station->capacite = capacite;
	noeud->fg = NULL; // Le sous-arbre gauche est initialement NULL
	noeud->fd = NULL; // Le sous-arbre droit est initialement NULL
	noeud->equilibre = 0; //L'équilibre initial de l'arbre est 0 (arbre vide)
    	return noeud;
}



// Fonction d'insertion d'une station dans l'arbre AVL avec rééquilibrage
pavl insertionAVL(pavl a, int id, int capacite, int conso, int * h) {
    	if (a == NULL) {
       		*h = 1;                      // Si l'arbre est vide, on crée un nouvel arbre
        	return creerAvl(id, capacite, conso);           // Créer un nouvel arbre avec la station
    	} 
    	else if (id < a->station->id) {         // Si la station est inférieure à la station actuelle
        	a->fg = insertionAVL(a->fg, id, capacite, conso, h); // Insertion récursive dans le sous-arbre gauche
        	*h = -(*h);                  // Inverser l'état de hauteur
    	} 
    	else if (id > a->station->id) {         // Si la station est supérieure à la station actuelle
        	a->fd = insertionAVL(a->fd, id, capacite, conso, h); // Insertion récursive dans le sous-arbre droit
    	} 
    	else {                            // si c'est le meme identifiant alors on somme la consommation et on met a jour la capacite
    		if(a->station->capacite==0 && capacite !=0){ // 
    			a->station->capacite = a->station->capacite + capacite;
    		}
    		a->station->conso = a->station->conso + conso; 
        	*h = 0;
        	return a;
    	}
	if (*h != 0) {                     // Si l'équilibre n'est pas nul
        	a->equilibre = a->equilibre + *h;            // Ajuster l'équilibre de l'arbre
        	a = equilibreAVL(a);          // Équilibrer l'arbre après insertion
        	if (a->equilibre == 0) {              // Si l'arbre est équilibré après insertion
            		*h = 0;
        	}
        	else {
            		*h = 1;                   // Sinon, l'arbre reste déséquilibré
        	}
    	}
    	return a;
}

int recherche(pavl noeud, int id) {
    	if (noeud == NULL){
    		return 0;
	}
	else if(noeud->station->id == id){
        	return 1;
    	}
    	if (id < noeud->station->id) {
        	return recherche(noeud->fg, id);
    	} 
    	else if (id > noeud->station->id) {
        	return recherche(noeud->fd, id);
    	} 
}

// Fonction de rotation à gauche pour rééquilibrer l'arbre AVL
pavl rotationGauche(pavl a) {  	
    	pavl pivot;
    	int eq_a, eq_p;
    	pivot = a->fd; // Le fils droit devient le pivot
    	a->fd = pivot->fg; // Le sous-arbre gauche du pivot devient le fils droit de `a`
    	pivot->fg = a; //`a` devient le fils gauche du pivot
    	eq_a = a->equilibre;
    	eq_p = pivot->equilibre;
    	// Mise à jour des facteurs d'équilibre
    	a->equilibre = eq_a - max(eq_p, 0) - 1;
    	pivot->equilibre = min3(eq_a -2, eq_a + eq_p -2, eq_p -1);
    	a = pivot; // Le pivot devient la nouvelle racine
    	return a;
}

// Fonction de rotation à droite pour rééquilibrer l'arbre AVL
pavl rotationDroite(pavl a) {
    	pavl  pivot;
    	int eq_a, eq_p;
    	pivot = a->fg; // Le fils gauche devient le pivot
    	a->fg = pivot->fd; // Le sous-arbre droit du pivot devient le fils gauche de `a`
    	pivot->fd = a; //`a` devient le fils droit du pivot
    	eq_a = a->equilibre;
    	eq_p = pivot->equilibre;
    	// Mise à jour des facteurs d'équilibre
    	a->equilibre = eq_a - min(eq_p, 0) + 1;
    	pivot->equilibre = min3(eq_a +2, eq_a + eq_p +2, eq_p +1);
    	a = pivot; // Le pivot devient la nouvelle racine
    	return a;
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

pavl equilibreAVL(pavl a){
    if(a->equilibre >= 2){ // sous-arbre droit plus profond
        if(a->fd->equilibre >= 0){
            return rotationGauche(a);
        }
        else {
            return doubleRotationGauche(a);
        }
    }
    else if(a->equilibre <= -2){ // sous-arbre gauche plus profond
        if(a->fg->equilibre<= 0){
            return rotationDroite(a);
        }
        else {
            return doubleRotationDroite(a);
        }
    }
    return a;
}

void supprimer_AVL(pavl a){
	if(a != NULL){
		if(a->fg != NULL){
			supprimer_AVL(a->fg);
		}
		if(a->fd != NULL){
			supprimer_AVL(a->fd);
		}
		free(a->station);
		free(a);
	}
}

void recuperer_info_csv(pavl * a){ // certaine fonction utilisée ici ont été trouvé sur internet mais compris par l'ensemble des membres du groupe
	if(a != NULL){
		printf("Erreur : L'arbre n'est pas vide");
		exit(12);
	}
	FILE * f = NULL;
	f = fopen("tmp/donnees_filtrees.csv", "r"); // ouvir le fichier temporaire avec les données filtrées
	if(f == NULL){
		printf("Ouverture du fichier impossible\n");
		printf("code d'erreur = %d \n", errno );
		printf("Message d'erreur = %s \n", strerror(errno));
		exit(15);
	}
	int h = 0;
	char ligne[MAX_LIGNE];
	while(fgets(ligne, sizeof(ligne), f)){
		ligne[strcspn(ligne, "\n")] =0; 
		char * colonne1 = strtok(ligne, ";"); //strtok sert a couper la ligne en 3
		char * colonne2 = strtok(NULL, ";");  
        	char * colonne3 = strtok(NULL, ";");
        	if (colonne1 == NULL || colonne2 == NULL || colonne3 == NULL) {
            		printf("Erreur : Ligne mal formatée dans le fichier\n");
            		continue; // Ignorer la ligne mal formatée
        	}
        	// Convertir les colonnes en entiers
        	int id = atoi(colonne1);
        	int capacite = atoi(colonne2);
        	int conso = atoi(colonne3);
        	// Insérer les données dans l'arbre AVL
        	*a = insertionAVL(*a, id, capacite, conso, &h);
        }
	fclose(f);
}
	
void parcoursInfixe(pavl a, FILE* f){
	if(f == NULL){
		printf("Le fichier n'existe pas");
		exit(18);
	}
	if(a != NULL){ // si a est NULL pas besoin d'ecrire dans le fichier
		parcoursInfixe(a->fg, f);
		fprintf(f, "%d:", a->station->id);
		fprintf(f, "%d:", a->station->capacite);
		fprintf(f, "%d\n", a->station->conso);
		parcoursInfixe(a->fd, f);
	}
}

void ecrire_ds_fichier_result_tmp(pavl a){ 
	if(a == NULL){
		printf("L'arbre n'a pas été crée");
		exit(8);
	}
	FILE * f = fopen("tmp/fichier_tmp_result", "w");// Le mode 'w' permet d'écraser les données précédentes dans le fichier temporaire de resultat
	if(f == NULL){
		printf("Ouverture du fichier impossible\n");
		printf("code d'erreur = %d \n", errno );
		printf("Message d'erreur = %s \n", strerror(errno));
		exit(16);
	}
	parcoursInfixe(a, f);
	fclose(f);
}

int main(){
	pavl a = NULL;
	recuperer_info_csv(&a);
	ecrire_ds_fichier_result_tmp(a);
	supprimer_AVL(a);
	return 0;
}






















































