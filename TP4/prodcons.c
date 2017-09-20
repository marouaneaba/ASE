#include "prodcons.h"
/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

void producteur (void)
{
	objet_t * objet = malloc(sizeof(objet_t));
	
	while (1) {
		printf("Debut prod \n");
		produire_objet(objet);           /* produire l'objet suivant */	
		sem_down(&vide);                  /* dec. nb places libres */
		sem_down(&mutex);                 /* entree en section critique */
		mettre_objet(objet);              /* mettre l'objet dans le tampon */		
		sem_up(&mutex);                   /* sortie de section critique */
		sem_up(&plein);                   /* inc. nb place occupees */
		
		
	}
}

void consommateur (void)
{
	objet_t * objet = malloc(sizeof(objet_t));

	while (1) {
		printf("Debut cons \n");
		sem_down(&plein);                 /* dec. nb emplacements occupes */
		sem_down(&mutex);                 /* entree section critique */
		retirer_objet (objet);           /* retire un objet du tampon */
		sem_up(&mutex);                   /* sortie de la section critique */
		sem_up(&vide);                    /* inc. nb emplacements libres */		
		utiliser_objet(objet);            /* utiliser l'objet */
	}
}

void produire_objet(objet_t * objet)
{
	objet->number = 5;
	printf("Produire objet %d \n" , objet->number);
}

void utiliser_objet(objet_t * objet)
{
	printf("Utilisation de l'objet %d \n" , objet->number);
}

void mettre_objet(objet_t *objet)
{
	if(indice_obj_placement >= N){
		indice_obj_placement = 0;
	}
	
	objects[indice_obj_placement] = objet;
	
	printf("Objet placé : %d à l'indice %d \n", objects[indice_obj_placement]->number, indice_obj_placement);
	
	indice_obj_placement++;
	
}

void retirer_objet(objet_t * objet)
{
	if(indice_obj_prendre >= N){
		indice_obj_prendre = 0;
	}

	objet->number = objects[indice_obj_prendre]->number;

	printf("Objet retiré : %d à l'indice %d \n", objects[indice_obj_prendre]->number, indice_obj_prendre);
	
	indice_obj_prendre++;
}


int main(int argc, char *argv[])
{
	
	indice_obj_placement = 0;
	indice_obj_prendre = 0;
	
	sem_init(&mutex, 1);                /* controle d'acces au tampon */
	sem_init(&vide, N);                 /* nb de places libres */
	sem_init(&plein, 0);                /* nb de places occupees */

	create_ctx(SIZE_STACK, consommateur, NULL);
	create_ctx(SIZE_STACK, producteur, NULL);
		 
    start_sched();
    
    return 0;
}










