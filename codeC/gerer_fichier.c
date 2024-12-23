#include "projet.h"

// Procédure pour récupérer les informations d'un fichier CSV et les insérer dans l'arbre AVL
void recuperer_info_csv(pavl *a) {
    // Vérifie si l'arbre est vide avant de continuer.
    if (*a != NULL) {
        printf("Erreur : L'arbre n'est pas vide\n");
        exit(12);
    }

    // Ouvre le fichier CSV en mode lecture.
    FILE *f = fopen("/home/cytech/C-Wire-main/tmp/donnees_filtrees.csv", "r");
    if (f == NULL) {
        // Affiche un message d'erreur en cas d'échec d'ouverture.
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
        long capacite = strtol(colonne2, &endptr, 10); //la fonction strtol sert a convertir des chaine de caractere en long
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
	
// Procédure de parcours infixe d'un arbre AVL et écriture des données dans un fichier.
void parcoursInfixe(pavl a, FILE* f) {
    // Vérifie si le fichier est valide.
    if (f == NULL) {
        printf("Le fichier n'existe pas\n");
        exit(18);
    }

    // Si le noeud actuel n'est pas NULL, continue le parcours.
    if (a != NULL) {
        // Parcourt récursivement le sous-arbre gauche.
        parcoursInfixe(a->fg, f);

        // Écrit les données du noeud courant dans le fichier.
        if (fprintf(f, "%d:%ld:%ld\n", a->station->id, a->station->capacite, a->station->conso) < 0) {
            printf("Erreur : Échec d'écriture dans le fichier temporaire.\n");
            exit(17);
        }

        // Parcourt récursivement le sous-arbre droit.
        parcoursInfixe(a->fd, f);
    }
}

// Procédure pour écrire les données de l'arbre AVL dans un fichier temporaire au format CSV.
void ecrire_ds_fichier_result_tmp(pavl a) { 
    // Vérifie si l'arbre est vide.
    if (a == NULL) {
        printf("L'arbre n'a pas été crée\n");
        exit(8);
    }

    // Message indiquant une tentative d'ouverture ou de création du fichier.
    printf("Tentative d'ouverture ou de création du fichier dans 'tmp'.\n");

    // Ouvre le fichier temporaire en mode écriture ('w' écrase les données existantes).
    FILE *f = fopen("tmp/fichier_tmp_result.csv", "w");
    if (f == NULL) {
        // Gestion des erreurs d'ouverture avec affichage du code et message d'erreur.
        printf("Ouverture du fichier impossible\n");
        printf("code d'erreur = %d \n", errno);
        printf("Message d'erreur = %s \n", strerror(errno));
        exit(16);
    }

    // Message indiquant que le fichier a été ouvert avec succès.
    printf("Fichier temporaire ouvert avec succès pour écriture.\n");

    // Parcourt l'arbre et écrit les données dans le fichier.
    parcoursInfixe(a, f);

    // Message indiquant la fin de l'écriture dans le fichier.
    printf("Écriture dans le fichier temporaire terminée.\n");

    // Ferme le fichier après écriture.
    fclose(f);
}
