#ifndef _DRIVE_H_
#define _DRIVE_H_


void init_master();

void seek_sector(unsigned int cylinder, unsigned int sector);

void read_sector(unsigned int cylinder, unsigned int sector, unsigned char *buffer);

void read_sector_n(unsigned int cylinder, unsigned int sector, unsigned char *buffer, unsigned int size);

void write_sector(unsigned int cylinder, unsigned int sector, const unsigned char *buffer);

void write_sector_n(unsigned int cylinder, unsigned int sector, const unsigned char *buffer, unsigned int size);

void format_sector(unsigned int cylinder, unsigned int sector, unsigned int value);

#endif
