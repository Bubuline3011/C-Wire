#include "projet.h"

// Programme principal
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <fichier_csv_entree> <fichier_csv_sortie>\n", argv[0]);
        exit(6);
    }

    const char *fichierEntree = argv[1];
    const char *fichierSortie = argv[2];

    pavl racine = NULL;
    racine = lireFichierCSV(fichierEntree, racine);

    sommeConso(racine);
    ecrireFichierSortie(fichierSortie, racine);

    libérer_noeud_station(racine);

    return 0;
}

