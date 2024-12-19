#include "projet.h"

void recuperer_info_csv(pavl * a){
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

