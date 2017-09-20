/* vol.c - Gestionnaires des blocs et volumes 
 */
/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/
#include "vol.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "drive.h"
#include "constantes.h"

void 
read_bloc(unsigned int vol, unsigned int bloc, unsigned char * buf) {
	read_sector(convert_bloc_to_cyl(vol, bloc), convert_bloc_to_sec(vol, bloc), buf);
}

void 
read_bloc_n(unsigned int vol, unsigned int bloc, unsigned char * buf, unsigned int size) {
    read_sector_n(convert_bloc_to_cyl(vol, bloc), convert_bloc_to_sec(vol, bloc), buf, size);
}

void 
write_bloc(unsigned int vol, unsigned int bloc, const unsigned char * buf) {
	write_sector(convert_bloc_to_cyl(vol, bloc), convert_bloc_to_sec(vol, bloc), buf);
}

void 
write_bloc_n(unsigned int vol, unsigned int bloc, unsigned char * buf, unsigned int size) {
    write_sector_n(convert_bloc_to_cyl(vol, bloc), convert_bloc_to_sec(vol, bloc), buf, size);
}

void 
format_vol(unsigned int vol) {
	assert(vol <= mbr.nb_volumes && vol >= 0);
	
	int i;
	int old_super = current_vol;
	int size_free =  mbr.volumes[vol].nb_secteurs;
		
		
	load_super(vol);
	
	superbloc.magic = MAGIC;
	
	superbloc.number_free_blocs = size_free - 1;
	superbloc.first_free_bloc = 1;
	superbloc.num_serie = 1234; /* Pas important pour le moment */
	strcpy(superbloc.name,"no_name"); /* Pas important pour le moment */
	
	
    struct free_block_s *free_blc;
    
	write_bloc_n(vol, 0, (unsigned char*)&superbloc, sizeof(struct superbloc_s));
	
	for (i = 1; i <= superbloc.number_free_blocs; i++) {
        free_blc->next = (i == superbloc.number_free_blocs) ? BLOC_NULL : i + 1;
        free_blc->size =  1; 
        write_bloc_n(vol, i, &free_blc, sizeof(struct free_block_s));
    }
    
    superbloc.magic = MAGIC;
    save_super();
    
    
    load_super(old_super);
   
}


int 
convert_bloc_to_sec(unsigned int vol, unsigned int bloc) {
	assert(bloc >= 0 && vol >= 0);
	assert(mbr.magic == MAGIC && vol <= mbr.nb_volumes);
	return (mbr.volumes[vol].secteur_base + bloc) % nbSec;
}

int 
convert_bloc_to_cyl(unsigned int vol, unsigned int bloc) {
	assert(bloc >= 0 && vol >= 0);
	assert(mbr.magic == MAGIC && vol <= mbr.nb_volumes);
	return (mbr.volumes[vol].secteur_base + bloc) / nbSec + mbr.volumes[vol].cyl_base;
}


void init_super(unsigned int vol){
	struct superbloc_s super;
	struct free_block_s free_blc;
	int i;
	int size_free =  mbr.volumes[vol].nb_secteurs;
	
	/* printf("Vol : %d // size_free : %d \n",vol , size_free);
	printf("Mbr in init_super , Nb volumes : %d  // Volume 0 , secteurs : %d \n", mbr.nb_volumes ,mbr.volumes[vol].nb_secteurs); */
	
	super.magic = MAGIC;
	
	super.number_free_blocs = size_free - 1;
	super.first_free_bloc = 1;
	super.num_serie = 1234; /* Pas important pour le moment */
	strcpy(super.name,"no_name"); /* Pas important pour le moment */
	
	
	write_bloc_n(vol, 0, (unsigned char*)&super, sizeof(struct superbloc_s));
	
	for (i = 1; i <= super.number_free_blocs; i++) {
        free_blc.next = (i == super.number_free_blocs) ? BLOC_NULL : i + 1;
        free_blc.size =  1; 
        write_bloc_n(vol, i, &free_blc, sizeof(struct free_block_s));
    }
	
	superbloc = super;
	current_vol = vol;
}


int 
load_super(unsigned int vol){	
	assert(sizeof(superbloc) <= tailleSec);
    assert(vol <= mbr.nb_volumes);
    
	current_vol = vol;
    read_bloc_n(vol, 0, &superbloc, sizeof(struct superbloc_s));
    return superbloc.magic == MAGIC;
}

void 
save_super(){	
	assert(superbloc.magic == MAGIC);
    
    write_bloc_n(current_vol, 0, &superbloc, sizeof(struct superbloc_s));
}

unsigned int 
new_bloc(){
	assert(current_vol >= 0 && current_vol <= mbr.nb_volumes);
	
	int new = superbloc.first_free_bloc;
	
    if (superbloc.first_free_bloc != BLOC_NULL) {
        struct free_block_s bl;
        read_bloc_n(current_vol, new, &bl, sizeof(struct free_block_s));
        superbloc.first_free_bloc = bl.next;
        superbloc.number_free_blocs--;
   
        save_super();
    }
    return new;
}


void 
free_bloc(unsigned int bloc){
	assert(current_vol >= 0 && current_vol <= mbr.nb_volumes);
	assert(bloc >= 0 && bloc <= mbr.volumes[current_vol].nb_secteurs);
	
    if (bloc == BLOC_NULL){
		return;
	}	
		
    struct free_block_s bl;
    bl.next = superbloc.first_free_bloc;
    write_bloc_n(current_vol, bloc, &bl, sizeof(struct free_block_s));
    superbloc.number_free_blocs++;
    superbloc.first_free_bloc = bloc;
    save_super();
	
}
