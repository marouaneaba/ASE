/**
 * inode.c - gestion des inodes
 *
 */
#include "constantes.h"
#include "mbr.h"
#include "vol.h"
#include "inode.h"
#include <assert.h>
#include <stdlib.h>

void read_inode(unsigned int inumber, struct inode_s *inode){
	assert(sizeof(struct inode_s) <= tailleSec);
    read_bloc_n(current_vol, inumber, (char *) inode, sizeof(struct inode_s));
}

void 
write_inode(unsigned int inumber, struct inode_s *inode){
	assert(sizeof(struct inode_s) <= tailleSec);
    write_bloc_n(current_vol, inumber, (char *) inode, sizeof(struct inode_s));
}

int 
create_inode(enum file_type_e type){
	struct inode_s * inode;
	
	inode = malloc(sizeof(struct inode_s));
	
	/* Init
	Memset : ne fais pas le malloc mais init tout à 0 */
	
	memset(inode->direct, BLOC_NULL, sizeof(inode->direct));
	inode->indirect = BLOC_NULL;
	inode->double_indirect = BLOC_NULL;
	inode->taille = 0;
	inode->type = type;
	
	/* Alouer bloc */
	int bloc = new_bloc();
	
	/* Ecrire */
	write_inode(bloc, inode);
	
	/* On a écrit l'inode sur le disque on peut free en mémoire */
	free(inode);
    return bloc;
}


void 
delete_indirect(int number_indirect) {
    int i;
    int *indirects;
    indirects = malloc(NB_INDIRECT);
    read_bloc(current_vol, number_indirect, (char *) indirects);
    for (i = 0; i < sizeof(indirects); i++) {
        if (indirects[i] != BLOC_NULL) {
            free_bloc(indirects[i]);
        }
    }
    free_bloc(number_indirect);
    free(indirects);
}

void 
delete_double_indirect(int number_double_indirect){
	int i;
    int *double_indirects;
    double_indirects = malloc(NB_INDIRECT);
    read_bloc(current_vol, number_double_indirect, (char *) double_indirects);
    for (i = 0; i < sizeof(double_indirects); i++) {
        if (double_indirects[i] != BLOC_NULL) {
            delete_indirect(double_indirects[i]);
        }
    }
    delete_indirect(number_double_indirect);
    free(double_indirects);
}


int 
delete_inode(unsigned int inumber){
	int i;
    struct inode_s *inode;
    
	inode = malloc(sizeof(struct inode_s));
    read_inode(inumber, inode);
     
	/* Libérer les blocs données (parcourir bloc avec un read
	 et faire un free_bloc si entrée != 0)
	 directs */
	for (i = 0; i < NB_DIRECT; i++) {
        free_bloc(inode->direct[i]);
    }
	/* indirects */
	if (inode->indirect != BLOC_NULL) {
        delete_indirect(inode->indirect);
    }
	/* double indirects */
	if (inode->double_indirect != BLOC_NULL) {
        delete_double_indirect(inode->double_indirect);
    }
	
    
    /* libérer bloc de l'inode */
    free_bloc(inumber);
    free(inode);
    
    return RETURN_SUCCESS;
}

int make_indirect(int *indirects) {
    int i;
    int bloc = new_bloc();
      
    if (bloc == BLOC_NULL) {
        return BLOC_NULL;
    }
    
    read_bloc(current_vol, bloc, (char *) indirects);
    for (i = 0; i < NB_INDIRECT; i++) {
        indirects[i] = BLOC_NULL;
    }
    
    return bloc;
}


unsigned int 
vbloc_of_fbloc_direct(unsigned int inumber, 
						struct inode_s *inode, 
						unsigned int fbloc, 
						int do_allocate) {
	int bloc;
    
    if (!do_allocate) {
        return inode->direct[fbloc];
    }
    
    if (inode->direct[fbloc] == BLOC_NULL) {
        bloc = new_bloc();
        if (bloc != BLOC_NULL) {
            inode->direct[fbloc] = bloc;
            write_inode(inumber, inode);
        }
    }
    
    return inode->direct[fbloc];
}

int vbloc_of_fbloc_indirect(unsigned int inumber, 
							struct inode_s *inode, 
							unsigned int fbloc, 
							int *indirects,
							int do_allocate) {
	int ret = BLOC_NULL;
    if(!do_allocate){
		/* Les blocs indirects n'ont pas été inits, donc le bloc n'existe pas */
		if (inode->indirect != BLOC_NULL) {
			read_bloc(current_vol, inode->indirect, (char *) indirects);
			ret = indirects[fbloc - NB_DIRECT];
		}
		
		return ret;
	} else{	
		/* Lire les indirects si non nul
		Sinon tenter de les créer */
		if (inode->indirect == BLOC_NULL) {
			ret = make_indirect(indirects);
			if (ret == BLOC_NULL) {
				return BLOC_NULL;
			}
			inode->indirect = ret;
			write_inode(inumber, inode);
		} else {
			read_bloc(current_vol, inode->indirect, (char *) indirects);
		} /* Fin de la récupération ou création des indirects */
		
		
		if (indirects[fbloc - NB_DIRECT] == BLOC_NULL) {
			ret = new_bloc();
			if (ret == BLOC_NULL) {
				return BLOC_NULL;
			}
			indirects[fbloc - NB_DIRECT] = ret;
			write_bloc(current_vol, inode->indirect, (char *) indirects);
		} /* Fin de la récupération ou création du bloc */
		
		return indirects[fbloc - NB_DIRECT];  
	} /* Fin du do allocate */
    
}

int vbloc_of_fbloc_indirect_2(unsigned int inumber, 
								struct inode_s *inode, 
								unsigned int fbloc, 
								int *indirects_2, 
								int *indirects) {
    
    if (inode->double_indirect == BLOC_NULL) {
        return BLOC_NULL;
    }
    read_bloc(current_vol, inode->double_indirect, (char *) indirects_2);
    
    fbloc -= (NB_DIRECT + NB_INDIRECT);
    
    if (indirects_2[fbloc / NB_INDIRECT] == BLOC_NULL) {
        return BLOC_NULL;
    }
    read_bloc(current_vol, indirects_2[fbloc / NB_INDIRECT], (char *) indirects);
    
    return indirects[fbloc % NB_INDIRECT];
}

int vbloc_of_indirect_2_fbloc_allocate(unsigned int inumber, 
											struct inode_s *inode, 
											unsigned int fbloc, 
											int *indirects_2, 
											int *indirects) {
    
    int bloc;
    
    /*Lire les indirects si non nul
	Sinon tenter de les créer */
    if (inode->double_indirect == BLOC_NULL) {
        bloc = make_indirect(indirects_2);
        if (bloc == BLOC_NULL) {
            return BLOC_NULL;
        }
        inode->double_indirect = bloc;
        write_inode(inumber, inode);
    }
    else {
        read_bloc(current_vol, inode->double_indirect, (char *) indirects_2);
    }
    
    fbloc -= (NB_DIRECT + NB_INDIRECT);
    
    /* Lire les double indirects si non nul
	Sinon tenter de les créer */
    if (indirects_2[fbloc / NB_INDIRECT] == BLOC_NULL) {
        bloc = make_indirect(indirects);
        if (bloc == BLOC_NULL) {
            return BLOC_NULL;
        }
        indirects_2[fbloc / NB_INDIRECT] = bloc;
        write_bloc(current_vol, inode->double_indirect, (char *) indirects_2);
    }
    else {
        read_bloc(current_vol, indirects_2[fbloc / NB_INDIRECT], (char *) indirects);
    }
    
    if (indirects[fbloc % NB_INDIRECT] == BLOC_NULL) {
        bloc = new_bloc();
        if (bloc == BLOC_NULL) {
            return BLOC_NULL;
        }
        indirects[fbloc % NB_INDIRECT] = bloc;
        write_bloc(current_vol, indirects_2[fbloc / NB_INDIRECT], (char *) indirects);
    }
    
    return indirects[fbloc % NB_INDIRECT];
}

unsigned int 
vbloc_of_fbloc(unsigned int inumber, 
                            unsigned int fbloc,
                            int do_allocate){	
	assert(fbloc >= 0);
    
    int *indirects, *indirects_2;
    int ret = BLOC_NULL;
    struct inode_s *inode;
    
    inode = malloc(sizeof(struct inode_s));
    read_inode(inumber, inode);
    
    if (fbloc < NB_DIRECT) {
        ret = vbloc_of_fbloc_direct(inumber, inode, fbloc, do_allocate);
    } else {
		indirects = malloc(NB_INDIRECT * sizeof(int));
		
		
		if (fbloc < NB_DIRECT + NB_INDIRECT) {
            
            ret = vbloc_of_fbloc_indirect(inumber, inode, fbloc, indirects, do_allocate);
        }
        
        if (fbloc < NB_DIRECT + NB_INDIRECT + NB_INDIRECT * NB_INDIRECT) {
            indirects_2 = malloc(NB_INDIRECT * sizeof(int));
            if (do_allocate) {
                ret = vbloc_of_fbloc_indirect_2_allocate(inumber, inode, fbloc, indirects_2, indirects);
            }
            else {
                ret = vbloc_of_fbloc_indirect_2(inumber, inode, fbloc, indirects_2, indirects);
            }
            free(indirects_2);
        }
		free(indirects);
	}
	free(inode);
	return ret;
    
}
