#include "projet.h"

// Programme principal
int main(){
	pavl a = NULL;
	recuperer_info_csv(&a);
	ecrire_ds_fichier_result_tmp(a);
	supprimer_AVL(a);
	return 0;
}

