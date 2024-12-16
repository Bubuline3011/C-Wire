#include "projet.h"

// Écriture des résultats dans un fichier de sortie
void ecrireFichierSortie(const char *nomFichier, pavl a) {
    	FILE *f = fopen(nomFichier, "w");
    	if (f == NULL) {
        	perror("Erreur d'ouverture du fichier de sortie");
        	exit(3);
    	}
    	fprintf(f, "ID;Capacité;Consommation\n");
    	if (a != NULL) {
        	ecrireFichierSortieRec(a, f);
    	}
    	fclose(f);
}

void ecrireFichierSortieRec(pavl a, FILE *f) {
    	if (a == NULL){
		return;
	}
    	ecrireFichierSortieRec(a->fg, f);
    	fprintf(f, "%d:%d:%d\n", a->station->id, a->station->capacite, a->station->som_conso);
    	ecrireFichierSortieRec(a->fd, f);
}


// Lecture des données CSV
pavl lireFichierCSV(const char *nomFichier, pavl racine) {
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) {
        perror("Erreur d'ouverture du fichier CSV");
        exit(4);
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        Station * s = malloc(sizeof(Station));
        if(s == NULL){
        	exit(5);
        }
        if (sscanf(ligne, "%d;%d;%d", &s->id, &s->capacite, &s->load) == 3) {
            int h = 0;
            racine = insertionAVL(racine, s, &h);
        }
        else {
            free(s);
        }
    }
    fclose(f);
    return racine;
}
