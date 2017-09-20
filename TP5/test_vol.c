#include <stdlib.h>
#include "constantes.h"
#include "drive.h"
#include "mbr.h"
#include "vol.h"

/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

int main(int argc, char **argv) {
	
	printf("*****\n*     Test du module volume  *\n*******************\n");
		
	init_master();
	load_mbr();
	
	printf("Mbr loaded ! Nb volumes : %d  // Volume 0 , secteurs : %d \n", mbr.nb_volumes , mbr.volumes[0].nb_secteurs);
	
	init_super(0);
	
	
	int i;
	int max = superbloc.number_free_blocs;
	for(i = 0; i < max + 1  ; i++){
		int result = new_bloc();
		printf("Numéro de bloc allouer : %d \n" , result); 
	}
	
	printf("Number of free bloc : %d  \n", superbloc.number_free_blocs);
	free_bloc(4);
	printf("Free bloc 4 , number of free bloc : %d \n", superbloc.number_free_blocs);
	
	
	free_bloc(49);
	printf("Free bloc 49, number of free bloc : %d \n", superbloc.number_free_blocs);
	
	
	/* free_bloc(999);
	printf("Free bloc 999 , number of free bloc : %d \n", superbloc.number_free_blocs); */
	
	
	free_bloc(1);
	printf("Free bloc 1 , number of free bloc : %d \n", superbloc.number_free_blocs);
	
	int result = new_bloc();
	printf("Numéro de bloc allouer : %d \n" , result); 
	
	return 0;
	
}
