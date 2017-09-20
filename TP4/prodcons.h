#ifndef _prodcons_H
#define _prodcons_H


#ifndef SIZE_STACK
#define SIZE_STACK 16384
#endif

#ifndef N
#define N 1000  /* nombre de places dans le tampon */
#endif

#include <stdio.h>
#include "sem.h"

struct sem_s mutex, vide, plein;

typedef struct objet_t
{
	int number;
	struct objet_t * suivant;

} objet_t;

objet_t* objects[N];
int indice_obj_placement;
int indice_obj_prendre;

void producteur (void);
void consommateur (void);
void produire_objet(objet_t * objet);
void retirer_objet(objet_t * objet);
void mettre_objet(objet_t * objet);
void utiliser_objet(objet_t * objet);



#endif
