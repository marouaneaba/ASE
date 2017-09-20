/**
  vm.c - Interaction avec l'utilisateur pour la gestion de volumes

 */
 /*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mbr.h"
#include "constantes.h"
#include "drive.h"


/* ------------------------------
   command list
   ------------------------------------------------------------*/
struct _cmd {
    char *name;
    void (*fun) (struct _cmd *c);
    char *comment;
};

static void list(struct _cmd *c);
static void new(struct _cmd *c);
static void del(struct _cmd *c);
static void help(struct _cmd *c) ;
static void save(struct _cmd *c);
static void quit(struct _cmd *c);
static void xit(struct _cmd *c);
static void none(struct _cmd *c) ;

static struct _cmd commands [] = {
    {"list", list, 	"display the partition table"},
    {"new",  new,	"create a new partition"},
    {"del",  del,	"delete a partition"},
    {"save", save,	"save the MBR"},
    {"quit", quit,	"save the MBR and quit"},
    {"exit", xit,	"exit (without saving)"},
    {"help", help,	"display this help"},
    {0, none, 		"unknown command, try help"}
} ;

/* ------------------------------
   dialog and execute 
   ------------------------------------------------------------*/

static void 
execute(const char *name)
{
    struct _cmd *c = commands; 
  
    while (c->name && strcmp (name, c->name))
        c++;
    (*c->fun)(c);
}

static void 
loop(void)
{
    char name[64];
    
    while (printf("> "), scanf("%62s", name) == 1)
        execute(name);
}

int 
estAvant(struct volume_s *vol1, struct volume_s *vol2) {
    return vol1->cyl_base * nbSec + vol1->secteur_base < vol2->cyl_base * nbSec + vol2->secteur_base;
}

/*
 * Fonction qui affiche la liste des partitions du disque 
*/
static void 
list(struct _cmd *c)
{
    unsigned int i;
    printf("%d partitions sur ce disque\n", mbr.nb_volumes);
    for (i = 0; i < mbr.nb_volumes; i++) {
        printf("\n\tVolume %d :\n\t\t%d sectors\n\t\tBase cylinder : %d\n\t\tBase sector : %d\n", i, mbr.volumes[i].nb_secteurs, mbr.volumes[i].cyl_base, mbr.volumes[i].secteur_base);
    }
}

/*
 * Fonction qui crée un nouveau volume 
 */
static void 
new(struct _cmd *c)
{
    int nb_blocs;
    int type;
    
    struct volume_s * volumes[mbr.nb_volumes];
    struct volume_s *vol;
    int i, j;  
    int posVol;
    
    /* nombre de secteur libre avant le volume existant */
    int free_spaces[mbr.nb_volumes + 1];
    
    if (mbr.nb_volumes >= NB_VOL_MAX) {
        printf("Nombre maximum de volumes atteint. Maximum : %d \n", NB_VOL_MAX);
        return;
    }
    
    printf("Entrez le type de volume souhaité (%d base - %d annexe - %d divers) : ", vol_base, vol_annexe, vol_misc);
    scanf("%d", &type);
    if (type != vol_base && type != vol_annexe && type != vol_misc) {
        printf("Entrée invalide\n");
        return;
    }
    
    printf("Entrez le nombre de blocs désirés : ");
    scanf("%d", &nb_blocs);
    if (nb_blocs < 1) {
        printf("Le nombre de bloc doit être positif\n");
        return;
    }
    
    /* Copie du mbr */
    for (i = 0; i < mbr.nb_volumes; i++) {
        volumes[i] = &mbr.volumes[i];
    }
    
    /* Tri */
    for (i = 0; i < (int) mbr.nb_volumes - 1; i++) {
        vol = volumes[i];
        posVol = i;
        for (j = i + 1; j < mbr.nb_volumes; j++) {
            if (estAvant(volumes[j], vol)) {
                vol = volumes[j];
                posVol = j;
            }
        }
        volumes[posVol] = volumes[i];
        volumes[i] = vol;
    }
    
    /* Table des espaces libres */
    int tmp, lastPos = 1;
    for (i = 0; i < mbr.nb_volumes; i++) {
        tmp = volumes[i]->cyl_base * nbSec + volumes[i]->secteur_base;
        free_spaces[i] = tmp - lastPos;
        lastPos = tmp + volumes[i]->nb_secteurs;
    }
    free_spaces[i] = nbCyl * nbSec - lastPos;
    
    /* Mettre le volume dans le premier espace assez grand */
    lastPos = 1;
    for (i = 0; i <= mbr.nb_volumes; i++) {
        if (free_spaces[i] >= nb_blocs) {
            mbr.volumes[mbr.nb_volumes].cyl_base = lastPos / nbSec;
            mbr.volumes[mbr.nb_volumes].secteur_base = lastPos % nbSec;
            mbr.volumes[mbr.nb_volumes].nb_secteurs = nb_blocs;
            mbr.volumes[mbr.nb_volumes].vol_type = type;
            mbr.nb_volumes++;
            return;
        }
        if (i >= mbr.nb_volumes) {
            break;
        }
        tmp = volumes[i]->cyl_base * nbSec + volumes[i]->secteur_base;
        lastPos = tmp + volumes[i]->nb_secteurs;
    }
    printf("Pas assez de secteurs contigus disponibles.\n");
    return;
}

static 
void del(struct _cmd *c)
{
    int num, i;
    printf("Entrez l'id du volume que vous voulez supprimer: ");
    scanf("%d", &num);
    if (num >= mbr.nb_volumes) {
        printf("Id invalide\n");
        return;
    }
    
    for (i = num; i < mbr.nb_volumes - 1; i++) {
        mbr.volumes[i] = mbr.volumes[i + 1];
    }
    mbr.nb_volumes--;
}

static void
save(struct _cmd *c)
{
    save_mbr();
}

static void
do_xit()
{
    exit(EXIT_SUCCESS);
}

static void
quit(struct _cmd *c)
{
    save(c);
    do_xit();
}

static void
xit(struct _cmd *dummy)
{
    do_xit(); 
}

static void 
help(struct _cmd *dummy)
{
    struct _cmd *c = commands;
  
    for (; c->name; c++) 
		printf ("%s\t-- %s\n", c->name, c->comment);
}

static void 
none(struct _cmd *c)
{
    printf ("%s\n", c->comment) ;
}

int
main(int argc, char **argv)
{
    init_master();
    load_mbr();
    
    printf ("\nGestionnaire de volumes : taper help pour obtenir la liste des commandes\n");
   
    /* dialog with user */ 
    loop();

    /* abnormal end of dialog (cause EOF for xample) */
    do_xit();

    /* make gcc -W happy */
    exit(EXIT_SUCCESS);
}
