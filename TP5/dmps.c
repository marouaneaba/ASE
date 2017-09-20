#include <stdlib.h>
#include <stdio.h>
#include "hardware.h"
#include "constantes.h"
#include "drive.h"

/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/
void afficher_secteurs(unsigned int ascii, unsigned int octal) {
    unsigned int i, j;
    unsigned char * buffer;
    buffer = malloc(tailleSec * sizeof(char));
    for (i = 0; i < nbCyl; i++) {
        for (j = 0; j < nbSec; j++) {
            read_sector(i, j, buffer);
            printf("%d\t%d\t", i, j);
            print_buffer(buffer, sizeof(buffer), ascii, octal);
        }
    }
    free(buffer);
}

void afficher_secteur(unsigned int cylinder, unsigned int sector, unsigned int ascii, unsigned int octal){

	unsigned int i, j;
    unsigned char * buffer;
    buffer = malloc(tailleSec * sizeof(char));
	
	read_sector(cylinder, sector, buffer);
	printf("%d\t%d\t", cylinder, sector);
	print_buffer(buffer, sizeof(buffer), ascii, octal);    
    
    free(buffer);
}

void format_all(unsigned int val) {
    unsigned int i, j;
    unsigned char * buffer;
    printf("\n********* Format **********\n");
    buffer = malloc(tailleSec * sizeof(char));
    for (i = 0; i < nbCyl; i++) {
        for (j = 0; j < nbSec; j++) {
            format_sector(i, j, val);
        }
    }
    free(buffer);

}

/* dump buffer to stdout */
void print_buffer(unsigned char *buffer, unsigned int buffer_size, int ascii_dump, int octal_dump) {
    int i,j;
    
    for (i=0; i<buffer_size; i+=16) {
		/* offset */
		printf("%.8o",i);

		/* octal dump */
		if (octal_dump) {
			for(j=0; j<8; j++)
			printf(" %.2x", buffer[i+j]);
			printf(" - ");
			
			for( ; j<16; j++)
			printf(" %.2x", buffer[i+j]);
			
			printf("\n");
		}
		/* ascii dump */
		if (ascii_dump) {
			printf("%8c", ' ');
			
			for(j=0; j<8; j++)
			printf(" %1c ", isprint(buffer[i+j])?buffer[i+j]:' ');
			printf(" - ");
			
			for( ; j<16; j++)
			printf(" %1c ", isprint(buffer[i+j])?buffer[i+j]:' ');
			
			printf("\n");
		}
    }
}

void usage(){
	printf("Gestion des secteurs du disque.\n");
	printf("Si plusieurs option d'affichage sont passé en paramêtres, seul la dernière est prise en compte.\n");
	printf("Liste des options :\n");
	printf("\t-a affichage des secteurs en ASCII.\n");
	printf("\t-x affichage des secteurs en hexadécimale, par défaut.\n");
	printf("\t-c spécifie le cylindre à afficher, 0 par défaut.\n");
	printf("\t-s spécifie le secteur à afficher, 0 par défaut.\n");
	printf("\t-t afficher tous les secteurs.\n");
	printf("\t-f formater tous les secteurs.\n");
	printf("\t-h affiche cette aide.\n");
	exit(EXIT_SUCCESS);
}



int main(int argc, char **argv) {
    init_master();
    
    int ascii_flag = 0;   /* Si égale à 1 affichage ascii sinon affichage hex */
    int all_flag   = 0;   /* Si égale à 1 , afficher tous les secteurs */
    int format_flag= 0;   /* Si égale à 1 , formater tous les secteurs */
	int cyl        = 0;
	int sec        = 0;
	int i          = 0;
	int opt;
    extern char *optarg;
    
    if(argc == 1)
		usage();

	while((opt = getopt(argc, argv, "ftaxhc:s:")) != EOF){
		switch(opt){
			case 'a':
				ascii_flag = 1;
				break;
			case 'x':
				ascii_flag = 0;
				break;
			case 'h':
				usage();
				break;
			case 'c':
				cyl = atoi(optarg);
				break;
			case 's':
				sec = atoi(optarg);
				break;
			case 't':
				all_flag = 1;
				break;
			case 'f':
				format_flag = 1;
				break;
			default:
				fprintf(stderr, "dmps : option invalide -- '%c'\n", opt);
				fprintf(stderr, "Saisissez « dmps -h » pour plus d'informations.");
				exit(EXIT_FAILURE);
		}
	}
		
	

	if(all_flag){
		afficher_secteurs(ascii_flag, !ascii_flag);
		exit(EXIT_SUCCESS);
	}
	
	if(format_flag){
		afficher_secteurs(ascii_flag, !ascii_flag);
		format_all('X');
		afficher_secteurs(ascii_flag, !ascii_flag);
		exit(EXIT_SUCCESS);
	}
	
	/* Si on arrive ici c'est qu'on veut afficher un secteur */
	if(sec < 0 || cyl < 0){
		fprintf(stderr, "Les adresses des secteurs et cylindre doivent êtres supérieur ou égales à 0.\n");
		exit(EXIT_FAILURE);
	}
		
	afficher_secteur(cyl, sec, ascii_flag, !ascii_flag);

    exit(EXIT_SUCCESS);
}
