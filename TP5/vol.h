/**
 * vol.h - Gestionnaires des blocs et volumes
 *
 */

#ifndef _VOL_H_
#define _VOL_H_

void read_bloc(unsigned int vol, unsigned int nbloc, unsigned char *buffer);
void write_bloc(unsigned int vol, unsigned int nbloc, const unsigned char *buffer);
void format_vol(unsigned int vol);
int convert_bloc_to_sec(unsigned int vol, unsigned int bloc);
int convert_bloc_to_cyl(unsigned int vol, unsigned int bloc);


void init_super(unsigned int vol);
int load_super(unsigned int vol);
void save_super();
unsigned int new_bloc();
void free_bloc(unsigned int bloc);


#endif
