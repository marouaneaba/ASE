/* ifile.c - gestion de fichiers */
 
/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "ifile.h"


int position_to_fbloc(unsigned int position){
	return position / HDA_SECTORSIZE;
}
	
unsigned int 
create_ifile(enum file_type_e type){
	unsigned int inumber;
    
    inumber = create_inode(type);
    
    return inumber;
}


int 
delete_ifile(unsigned int inumber){
	int status;
    
    status = delete_inode(inumber);

    return status;
}

int 
open_ifile(file_desc_t *fd, unsigned int inumber){	
	struct inode_s inode;
	unsigned int first_bloc;
	
		
	read_inode(inumber, &inode);
	
    fd->fds_inumber = inumber;
    fd->fds_size = inode.taille;
    fd->fds_pos = 0;
    fd->fds_dirty = FALSE;
       
 
	/* On va maintenant remplir le buffer du fd
	 * Avec des 0 si le premier bloc est un bloc libre
	 * ou avec celui-ci si c'est un bloc rempli */
	first_bloc = vbloc_of_fbloc(inumber, 0, FALSE);
	if (!first_bloc){
		memset(fd->fds_buf, 0, DATA_BLOC_SIZE);
	} else{
		read_bloc(current_vol, first_bloc, fd->fds_buf);  
	}
	
    return RETURN_SUCCESS;
}

void 
close_ifile(file_desc_t *fd){
	struct inode_s inode;

	if (fd-> fds_dirty) {
		flush_ifile(fd);
	}
	
	read_inode(fd->fds_inumber, &inode);
	inode.taille = fd->fds_size;
	write_inode(fd->fds_inumber, &inode);
}

void 
flush_ifile(file_desc_t *fd){
	unsigned int fbloc; /* index du bloc dans le fichier */
	unsigned int vbloc; /* index du bloc dans le volume */

	/* le fd n'a pas été modifié, ca ne sert a rien de le flush */
	if (fd-> fds_dirty) {	
		fbloc = bloc_of_pos(fd->fds_pos);
		vbloc = vbloc_of_fbloc(fd-> fds_inumber, fbloc, TRUE);
		
		write_bloc(current_vol, vbloc, fd->fds_buf);
		
		fd-> fds_dirty = FALSE ;
	}
}

/* relatif */
void 
seek_ifile(file_desc_t *fd, int r_offset){ 
	unsigned int new_pos, old_pos;
    unsigned int fbloc, vbloc;
    
    new_pos = fd->fds_pos + r_offset;
    fd->fds_pos = new_pos;
    
    /* faut-il jump sur un autre bloc ? */
    if(position_to_fbloc(new_pos) != position_to_fbloc(fd->fds_pos)){
       
        flush_ifile(fd);
        
        fbloc = new_pos / HDA_SECTORSIZE;
        vbloc = vbloc_of_fbloc(fd->fds_inumber, fbloc, FALSE);
        
        /* Le bloc 0 n'est pas init
         * Sinon charger le bloc */
        if (! vbloc){
			memset(fd->fds_buf, 0, BLOC_SIZE);
		}else {
			read_bloc(current_vol, vbloc, fd->fds_buf);
		}         
    }
}  

/* absolu */
void 
seek2_ifile(file_desc_t *fd, int a_offset){ 
	seek_ifile(fd, a_offset - fd->fds_pos);
} 

int 
readc_ifile(file_desc_t *fd){
	int value;
    
    if (fd->fds_pos > fd->fds_size){
		return READ_EOF;
	}
        
    /* retourner les données qui sont dans le buffer */
    value = fd->fds_buf[(fd->fds_pos % HDA_SECTORSIZE)];
    
    /* seek + 1 */
    seek_ifile(fd, 1);
    
    return value;
}



int 
writec_ifile(file_desc_t *fd, char c){
	unsigned int ibloc;
	
	fd->fds_buf[(fd->fds_pos % HDA_SECTORSIZE)] = c;
	
    if (fd->fds_size < fd->fds_pos){
		fd->fds_size = fd->fds_pos;
	}
		
	/* si c'est la 1ère écriture on doit vérifier l'allocation */
    if (!fd->fds_dirty) {
        ibloc = vbloc_of_fbloc(fd->fds_inumber, position_to_fbloc(fd->fds_pos), TRUE);
        if (ibloc == BLOC_NULL){
			return RETURN_FAILURE;
		}	         
        fd->fds_dirty = TRUE;
    }
    
    /* si le buffer est full, on doit l'écrire */
    if ((fd->fds_pos % DATA_BLOC_SIZE) == BLOC_SIZE - 1) {
		
		ibloc = vbloc_of_fbloc(fd->fds_inumber, position_to_fbloc(fd->fds_pos), FALSE);
		write_bloc(current_vol, ibloc, fd->fds_buf);
		
		seek_ifile(fd, 1);
		
		fd->fds_dirty = FALSE;
    } else {
		fd->fds_pos++;
	}
       
	return (fd->fds_pos - 1);
}

/* 
int 
read_ifile(file_desc_t *fd, void *buf, unsigned int nbyte){
	
	return 0;
}

int 
write_ifile(file_desc_t *fd, const void *buf, unsigned int nbyte){
	
	return 0;
} */
