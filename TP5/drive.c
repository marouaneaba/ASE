/* drive.c - Accès aux périphériques
 */

/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

#include <stdlib.h>
#include <stdio.h>
#include "hardware.h"
#include "constantes.h"
#include "drive.h"


/**
 * Fonction vide pour initialiser le vecteur d'interruptions
 */
static void empty_it() {
    return;
}

/**
 * Initialise les disques, interruptions...
 */
void init_master() {
    unsigned int i;
	unsigned char *buffer;
    
    /* Initialise le hardware */
    if (init_hardware("hardware.ini") == 0) {
		fprintf(stderr, "Error in hardware initialization\n");
		exit(EXIT_FAILURE);
    }
    
    /* Récupère la géométrie du disque */
    init_disk_infos();
    
    /* Init buffer */
    buffer = malloc(tailleSec * sizeof(char));
    
    /* Interrupt handlers */
    for (i=0; i<16; i++)
		IRQVECTOR[i] = empty_it;
    
    /* Allows all IT */
    _mask(1);
    free(buffer);
}


/**
 * Initialise les infos concernant la géométrie du disque
 */
void init_disk_infos() {
    _out(HDA_CMDREG, CMD_DSKINFO);
    
     /* On va prendre les 8 premiers bits , puis les déplacer , 
      * puis prendre les 8 bits de poids faible
      * et faire un ou logique avec |= */
    
    nbCyl = _in(HDA_DATAREGS);
    nbCyl = nbCyl << 8;
    nbCyl |= _in(HDA_DATAREGS + 1);
      
    nbSec = _in(HDA_DATAREGS + 2);
    nbSec = nbSec << 8;
    nbSec |= _in(HDA_DATAREGS + 3);
    
    tailleSec = _in(HDA_DATAREGS + 4);
    tailleSec = tailleSec << 8;
    tailleSec |= _in(HDA_DATAREGS + 5);
}

/**
 * Place les données du MASTERBUFFER dans le buffer donné
 */
void dump(unsigned char *buffer, unsigned int size) {
    int i;
    for (i = 0; i < size; i++) {
        buffer[i] = MASTERBUFFER[i];
    }
}

/* 
 * Placement des données à écrire dans le MASTERBUFFER 
 */
void fill(const unsigned char *buffer, unsigned int size) {
    int i;
    for (i = 0; i < size; i++) {
        MASTERBUFFER[i] = buffer[i];
    }
}

/**
 * Placer la tête de lecture du disque sur le cylindre et le secteur donné
 */
void seek_sector(unsigned int cylinder, unsigned int sector) {
    /* Place les infos pour le seek
    >> 8 pour décaler et récupérer les 8 bits de poids fort
    & 0xFF pour masquer tous les bits de gauche sauf les 8 premiers */
    _out(HDA_DATAREGS, (cylinder >> 8) & 0xFF);
    _out(HDA_DATAREGS + 1, cylinder & 0xFF);
    _out(HDA_DATAREGS + 2, (sector >> 8) & 0xFF);
    _out(HDA_DATAREGS + 3, sector & 0xFF);
    /* Lance le seek */
    _out(HDA_CMDREG, CMD_SEEK);
    /* Attend la fin du seek */
    _sleep(HDA_IRQ);
}

/**
 * Lecture d'un secteur en mettant le résultat dans un buffer
 */
void read_sector(unsigned int cylinder, unsigned int sector, unsigned char *buffer) {
	read_sector_n(cylinder, sector, buffer, tailleSec);
}

/**
 * Lecture de plusieurs secteurs en mettant le résultat dans un buffer
 */
void read_sector_n(unsigned int cylinder, unsigned int sector, unsigned char *buffer, unsigned int size) {
    if (cylinder < 0 || cylinder > nbCyl) {
		exit(EXIT_FAILURE);
	}
	if (sector < 0 || sector > nbSec) {
		exit(EXIT_FAILURE);
	}
	if (buffer == NULL) {
		exit(EXIT_FAILURE);
	}
	
	/* Placement de la tête de lecture */
	seek_sector(cylinder, sector);
	
	/* Donne le nombre de secteur à lire (1) */
	_out(HDA_DATAREGS, 0);
	_out(HDA_DATAREGS + 1, 1);
    
	/* Exécute la méthode READ */
	_out(HDA_CMDREG, CMD_READ);
    
	/* Attente du signal d'interruption */
	_sleep(HDA_IRQ);
	
    /* Placement des données dans le buffer */
    dump(buffer, size);
}

/**
 * Ecriture d'un secteur dans à partir du buffer
 */
void write_sector(unsigned int cylinder, unsigned int sector, const unsigned char *buffer) {
	write_sector_n(cylinder, sector, buffer, tailleSec);
}

/**
 * Ecriture de "size" secteurs à partir du buffer
 */
void write_sector_n(unsigned int cylinder, unsigned int sector, const unsigned char *buffer, unsigned int size) {
    if (cylinder < 0 || cylinder > nbCyl) {
		exit(EXIT_FAILURE);
    }
    if (sector < 0 || sector > nbSec) {
		exit(EXIT_FAILURE);
    }
    if (buffer == NULL) {
        exit(EXIT_FAILURE);
    }
    
    /* Placement des données à écrire dans le MASTERBUFFER */
    fill(buffer, size);
    
	/* Placement de la tête de lecture */
	seek_sector(cylinder, sector);
    
	/* Donne le nombre de secteur à écrire (1) */
	_out(HDA_DATAREGS, 0);
	_out(HDA_DATAREGS + 1, 1);
    
	/* Exécute la méthode WRITE */
	_out(HDA_CMDREG, CMD_WRITE);
    
	/* Attente du signal d'interruption */
	_sleep(HDA_IRQ);
}

/**
 * Formattage d'un secteur en écrivant une valeur précise
 */
void format_sector(unsigned int cylinder, unsigned int sector, unsigned int value) {
	if (cylinder < 0 || cylinder > nbCyl) {
		exit(EXIT_FAILURE);
    }
    if (sector < 0 || sector > nbSec) {
		exit(EXIT_FAILURE);
    }
    if (value < 0) {
		exit(EXIT_FAILURE);
    }

	/* Placement de la tête de lecture */
	seek_sector(cylinder, sector);

	/* Donne le nombre de secteur à formater (1) */
	_out(HDA_DATAREGS, 0);
	_out(HDA_DATAREGS + 1, 1);

	/* Donne les valeurs pour le formattage */
    _out(HDA_DATAREGS + 2, (value >> 24) & 0xFF);
    _out(HDA_DATAREGS + 3, (value >> 16) & 0xFF);
    _out(HDA_DATAREGS + 4, (value >> 8) & 0xFF);
    _out(HDA_DATAREGS + 5, value & 0xFF);

	/* Exécute la méthode FORMAT */
	_out(HDA_CMDREG, CMD_FORMAT);

	/* Attente du signal d'interruption */
	_sleep(HDA_IRQ);
}
