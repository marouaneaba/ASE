#include <stdlib.h>
#include "constantes.h"
#include "drive.h"
#include "mbr.h"
#include "vol.h"

/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

int main(){
	int bloc = 0;
	int nvol = 0;
	init_master();
	load_mbr();
	
	
	/* int sec = convert_bloc_to_sec(nvol, -1);
	int cyl = convert_bloc_to_cyl(nvol, -1); */
	
	
	for(; nvol < 8; nvol++){
		printf("*****\n*          VOLUME %d       *\n*******************\n", nvol);
		
		
		int nbblocs = mbr.volumes[mbr.nb_volumes].nb_secteurs;
		
		for(bloc = 0; bloc < nbblocs; bloc++){
			int sec = convert_bloc_to_sec(nvol, bloc);
			int cyl = convert_bloc_to_cyl(nvol, bloc);
			
			printf("Sec %d , Cyl %d \n", sec, cyl);
		}
		
			
		printf("\n\n");
		
	}
	return 0;
}
