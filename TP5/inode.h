/**
 * inode.h - gestion des inodes
 *
 */

#ifndef _INODE_H_
#define _INODE_H_


void 
read_inode(unsigned int inumber, struct inode_s *inode);

void 
write_inode(unsigned int inumber, struct inode_s *inode);

int 
create_inode(enum file_type_e type);

int 
delete_inode(unsigned int inumber);
	
/* Retourne le numéro de bloc sur le volume qui correspond au fbloc-ième bloc de l’inœud. 
 * Dans un premier temps, cette fonction retourne une valeur nulle si le bloc n’a pas été alloué. 
 * Si ce bloc n’est pas alloué et do_allocate, la fonction se charge de l’allocation du bloc et de le connecter à la structure de l’inoœud. */
unsigned int 
vbloc_of_fbloc(unsigned int inumber, 
				unsigned int fbloc,
				int do_allocate);


#endif
