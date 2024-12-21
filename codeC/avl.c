#include "projet.h"

// Fonction pour créer un arbre AVL avec une station donnée
pavl creerAvl(int id, long capacite, long conso){
	if(id < 0 || capacite < 0 || conso < 0){
		printf("Erreur : les parametres fournies sont incorrecte");
		exit(1);
	}
	pavl noeud = malloc(sizeof(AVL)); // Allocation dynamique pour un nouvel arbre AVL
	if(noeud==NULL){
		printf("Erreur d'allocation memoire lors de la creation de l'AVL");
   		exit(2); //Allocation échouée
   	}
   	noeud->station = malloc(sizeof(Station)); // allocation memoire pour la station
    	if (noeud->station == NULL) {
        	printf("Erreur d'allocation mémoire pour la Station\n");
        	free(noeud); // on libère la memoire allouée
        	exit(2);
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
pavl insertionAVL(pavl a, int id, long capacite, long conso, int * h) {
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

// Fonction de recherche dans l'AVL:
int recherche(pavl noeud, int id) {
	// Si le noeud est NULL, l'identifiant n'est pas trouvé.
  	if (noeud == NULL){
    		return 0;
	}
		// Si l'identifiant correspond, retourne 1.
	else if(noeud->station->id == id){
        	return 1;
    	}
    // Recherche dans le sous-arbre gauche si l'identifiant est plus petit.
    	if (id < noeud->station->id) {
        	return recherche(noeud->fg, id);
    	} 
		// Recherche dans le sous-arbre droit si l'identifiant est plus grand.
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
	if(a != NULL){      // Vérifie si le noeud n'est pas NULL.

	if(a->fg != NULL){
         	supprimer_AVL(a->fg);       // Supprime récursivement le sous-arbre gauche.

		}
	     // Supprime récursivement le sous-arbre droit.
             if(a->fd != NULL){
              		supprimer_AVL(a->fd);
		}
		free(a->station);         // Libère la mémoire de la station associée au noeud.
		free(a);          // Libère la mémoire du noeud.

	}
}


