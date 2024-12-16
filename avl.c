#include "projet.h"

pavl creerAvl(Station * a){
	if(a == NULL){
		printf("Erreur");
		exit(1);
	}
	pavl noeud = malloc(sizeof(pavl)); // Allocation dynamique pour un nouvel arbre AVL
	if(arbre==NULL){
   		exit(2); //Allocation échouée
   	}
	 // Attribution de tous les champ a notre nouveau noeud de l'AVL
	noeud->station = a;
	noeud->fg = NULL; // Le sous-arbre gauche est initialement NULL
	noeud->fd = NULL; // Le sous-arbre droit est initialement NULL
	noeud->equilibre = 0; //L'équilibre initial de l'arbre est 0 (arbre vide)
    	return noeud;
}

// Fonction d'insertion d'une station dans l'arbre AVL avec rééquilibrage
pavl insertionAVL(pavl a, Station * e, int * h) {
    	if (a == NULL) {
       		*h = 1;                      // Si l'arbre est vide, on crée un nouvel arbre
        	return creerAvl(e);           // Créer un nouvel arbre avec la station
    	} 
    	else if (e->id < a->station->id) {         // Si la station est inférieure à la station actuelle
        	a->fg = insertionAVL(a->fg, e, h); // Insertion récursive dans le sous-arbre gauche
        	*h = -(*h);                  // Inverser l'état de hauteur
    	} 
    	else if (e->id > a->station->id) {         // Si la station est supérieure à la station actuelle
        	a->fd = insertionAVL(a->fd, e, h); // Insertion récursive dans le sous-arbre droit
    	} 
    	else {                            // Si la station est déjà présente, on ne l'insère pas
    		a->station->load = a->station>load + a->load;
        	*h = 0;
        	return a;
    	}
	if (*h != 0) {                     // Si l'équilibre n'est pas nul
        	a->equilibre = a->equilibre + *h;            // Ajuster l'équilibre de l'arbre
        	a = equilibrerAVL(a);          // Équilibrer l'arbre après insertion
        	if (a->equilibre == 0) {              // Si l'arbre est équilibré après insertion
            		*h = 0;
        	}
        	else {
            		*h = 1;                   // Sinon, l'arbre reste déséquilibré
        	}
    	}
    	return a;
}

pavl recherche(pavl noeud, int id) {
    	if (noeud == NULL)
    		return 0;
	}
	else if(a->id == e){
        	return 1;
    	}
    	if (id < noeud->id) {
        	return recherche(noeud->fg, id);
    	} 
    	else if (id > root->id) {
        	return recherche(noeud->fd, id);
    	} 
}

void parcoursInfixe(pavl a){
	if(a != NULL)
		parcoursInfixe(a->fg);
		printf("(Id : %d, Capacite : %d, Load : %d, Equilibre : %d, Somme : %d)", a->station->id, a->station->capacite, a->station->load, a->equilibre, a->station->som_conso);
		parcoursInfixe(a->fd);
	}
}


// Fonction de rotation à gauche pour rééquilibrer l'arbre AVL
pavl rotationGauche(pavl a) {  	
    	AVL * pivot;
    	int eq_a, aq_p;
    	pivot = a->fd; // Le fils droit devient le pivot
    	a->fd = pivot->fg; // Le sous-arbre gauche du pivot devient le fils droit de `a`
    	pivot->fg = a; //`a` devient le fils gauche du pivot
    	eq_a = a->equilibre;
    	eq_p = pivot->equilibre;
    	// Mise à jour des facteurs d'équilibre
    	a->equilbre = eq_a - max(eq_p, 0) - 1;
    	pivot->equilibre = min(eq_a -2, eq_a + eq_p -2, eq_p -1);
    	a = pivot; // Le pivot devient la nouvelle racine
    	return a;
}

// Fonction de rotation à droite pour rééquilibrer l'arbre AVL
pavl rotationDroite(pavl a) {
    	AVL * pivot;
    	int eq_a, aq_p;
    	pivot = a->fg; // Le fils gauche devient le pivot
    	a->fg = pivot->fd; // Le sous-arbre droit du pivot devient le fils gauche de `a`
    	pivot->fd = a; //`a` devient le fils droit du pivot
    	eq_a = a->equilibre;
    	eq_p = pivot->equilibre;
    	// Mise à jour des facteurs d'équilibre
    	a->equilbre = eq_a - min(eq_p, 0) + 1;
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

pavl equilibreAVL(pavl * a){
    if(a->equilibre >= 2){ // sous-arbre droit plus profond
        if(a->fd->equilibre>= 0){
            return rotationGauche(a);
        }
        else {
            return doublerotationGauche(a);
        }
    }
    else if(a->equilibre <= -2){ // sous-arbre gauche plus profond
        if(a->fg->equilibre<= 0){
            return rotationDroite(a);
        }
        else {
            return doublerotationDroite(a);
        }
    }
    return a;
}

pavl suppMinAVL(pavl* a, int * h, int * pe){
    	AVL * tmp;
    	if(a->fg == NULL){
        	*pe = a->station->id;
        	*h = -1;
        	tmp = a;
        	a = a->fd;
        	free(tmp);
        	return a;
    	}
    	else {
        	a->fg = suppMinAVL(a->fg, h, pe):
   	}
    	if(*h != 0){
        	a->equilibre = a->equilibre + *h;
        	a = equilibreAVL(a);
        	if(a->equilibre == 0){
            		*h = 0;
        	}
        	else {
           	 	*h = 1;
        	}
    	}
    	return a;
}

pavl suppressionAVL(pavl * a, int * h, int e){
	pavl * tmp;
    	if(a == NULL){
        	*h = 0;
        	return a;
    	}
    	else if(e->id < a->station->id){
        	a->fg = suppressionAVL(a->fg, e);
        	*h = -*h;
    	}
    	else if(e->id > a->station->id){
        	a->fd = suppressionAVL(a->fd, e);
    	}
    	else if(a->fd != NULL){
        	a->fd = suppMinAVL(a->fd, h, &a->station->id);
    	}
    	else {
        	tmp = a;
        	a = a->fg;
        	*h = -1;
        	free(tmp);
        	return a;
    	}
    	if(*h != 0){
        	a->equilibre = a->equilibre + *h;
        	a = equilibreAVL(a);
        	if(a->equilibre == 0){
            		*h = 0;
        	}
        	else {
            		*h = 1;
        	}
    	}
    	return a;
}

// Libération de la mémoire de l'AVL
void libérer_noeud_station(StationNode *noeud) {
    	if (noeud == NULL){// Si le nœud est NULL, on ne fait rien
    		return;
	}
    	libérer_noeud_station(noeud->gauche); // Libérer le sous-arbre gauche
    	libérer_noeud_station(noeud->droit);// Libérer le sous-arbre droit
    	free(a->station);
	free(noeud);	// Libérer le nœud courant
}

void sommeConso(pavl a) {
    	if (a == NULL){ 
    		return;
    	}
    	sommeConso(a->fg);
    	sommeConso(a->fd);
    	a->station->som_conso = a->station->load;
    	if (a->fg != NULL){
    		a->station->som_conso += a->fg->station->som_conso;
    	}
    	if (a->fd != NULL){ 
    		a->station->som_conso += a->fd->station->som_conso;
    	}
}

