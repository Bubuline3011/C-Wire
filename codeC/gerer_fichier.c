#include "projet.h"

void recuperer_info_csv(pavl *a) {
    if (*a != NULL) {
        printf("Erreur : L'arbre n'est pas vide\n");
        exit(12);
    }

    FILE *f = fopen("tmp/donnees_filtrees.csv", "r");
    if (f == NULL) {
        printf("Ouverture du fichier impossible\n");
        printf("code d'erreur = %d\n", errno);
        printf("Message d'erreur = %s\n", strerror(errno));
        exit(15);
    }

    int h = 0;
    char ligne[MAX_LIGNE];
    while (fgets(ligne, sizeof(ligne), f)) {
        // Supprimer le caractère de fin de ligne
        ligne[strcspn(ligne, "\n")] = 0;

        // Ignorer les lignes vides
        if (strlen(ligne) == 0) {
            continue;
        }

        // Découper la ligne en colonnes
        char *colonne1 = strtok(ligne, ";");
        char *colonne2 = strtok(NULL, ";");
        char *colonne3 = strtok(NULL, ";");

        // Vérification des colonnes
        if (colonne1 == NULL || colonne2 == NULL || colonne3 == NULL) {
            printf("Erreur : Ligne mal formatée dans le fichier: '%s'\n", ligne);
            continue; // Ignorer la ligne mal formatée
        }

        // Conversion de l'ID avec atoi (sûr que c'est un entier)
        int id = atoi(colonne1);
        if (id <= 0) {
            printf("Erreur : ID invalide dans la ligne : '%s'\n", ligne);
            continue;
        }

        // Conversion de la capacité avec strtol
        char *endptr;
        long capacite = strtol(colonne2, &endptr, 10);
        if (*endptr != '\0' || capacite < 0) {
            printf("Erreur : Capacité invalide dans la ligne : '%s'\n", ligne);
            continue;
        }

        // Conversion de la consommation avec strtol
        long conso = strtol(colonne3, &endptr, 10);
        if (*endptr != '\0' || conso < 0) {
            printf("Erreur : Consommation invalide dans la ligne : '%s'\n", ligne);
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

