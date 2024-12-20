#include "projet.h"

void recuperer_info_csv(pavl * a){ 
	if(*a != NULL){
		printf("Erreur : L'arbre n'est pas vide\n");
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
		if (strlen(ligne) == 0) {
        		continue; // Ignorer les lignes vides
    		} 
		char * colonne1 = strtok(ligne, ";"); //strtok sert a couper la ligne en 3
		char * colonne2 = strtok(NULL, ";");  
        	char * colonne3 = strtok(NULL, ";");
        	if (colonne1 == NULL || colonne2 == NULL || colonne3 == NULL) {
            		printf("Erreur : Ligne mal formatée dans le fichier: '%s'\n", ligne);
            		continue; // Ignorer la ligne mal formatée
        	}
        	// Convertir les colonnes en entiers
        	int id = atoi(colonne1);
        	long capacite = atoi(colonne2);
        	long conso = atoi(colonne3);
        	if (id <= 0 || capacite < 0 || conso < 0) {
            		printf("Erreur : Données invalides dans la ligne : %s\n", ligne);
            		continue;
        	}
        	// Insérer les données dans l'arbre AVL
        	*a = insertionAVL(*a, id, capacite, conso, &h);
        	if (*a == NULL) {
    			printf("Erreur : Impossible d'insérer les données dans l'AVL.\n");
    			exit(14);
		}
        }
	fclose(f);
}
	
void parcoursInfixe(pavl a, FILE* f){
	if(f == NULL){
		printf("Le fichier n'existe pas\n");
		exit(18);
	}
	if(a != NULL){ // si a est NULL pas besoin d'ecrire dans le fichier
		parcoursInfixe(a->fg, f);
		if (fprintf(f, "%d:%ld:%ld\n", a->station->id, a->station->capacite, a->station->conso) < 0) {
    			printf("Erreur : Échec d'écriture dans le fichier temporaire.\n");
    			exit(17);
		}
		parcoursInfixe(a->fd, f);
	}
}

void ecrire_ds_fichier_result_tmp(pavl a){ 
	if(a == NULL){
		printf("L'arbre n'a pas été crée\n");
		exit(8);
	}
	FILE * f = fopen("tmp/fichier_tmp_result.csv", "w");// Le mode 'w' permet d'écraser les données précédentes dans le fichier temporaire de resultat
	if(f == NULL){
		printf("Ouverture du fichier impossible\n");
		printf("code d'erreur = %d \n", errno );
		printf("Message d'erreur = %s \n", strerror(errno));
		exit(16);
	}
	parcoursInfixe(a, f);
	printf("Écriture dans le fichier temporaire terminée.\n");
	fclose(f);
}

