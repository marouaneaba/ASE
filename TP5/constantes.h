#ifndef _CONSTANTES_H_
#define _CONSTANTES_H_




#define MAGIC 0xB16B00B5 
#define NB_VOL_MAX 8

#define BLOC_SIZE  tailleSec
#define DATA_BLOC_SIZE  tailleSec
#define NB_DIRECT 112 /* ((tailleSec - sizeof(int) * 4) / sizeof(int)) */
#define NB_INDIRECT 128 /* (tailleSec / sizeof(int)) */
#define BLOC_NULL 0
#define READ_EOF		-2
#define RETURN_FAILURE	-1
#define RETURN_SUCCESS	 0
#define TRUE 1
#define FALSE 0


#define HDA_CMDREG      0x3F6    /* registre de commande du disque maitre */
#define HDA_DATAREGS    0x110    /* registres de données (r,r+1,...r+15) */
#define HDA_IRQ         14       /* Interruption du disque */
#define NB_HW_IRQ       16       /* nombre d'iRQ */
#define TIMER_IRQ       2        /* Niveau dinterruption de l'horloge */
#define HDA_SECTORSIZE  512      /* Nombres d'octets constituant un bloc */
#define HDA_MAXCYLINDER 1024     /* Nombre de pistes du disque maître */
#define HDA_MAXSECTOR   32       /* Nombre de secteurs du disque maître */

#define xHDA_STPS       2        /* nombre de SYSTICK pour changer de secteur */
#define HDA_STPC        1        /* nombre de SYSTICK pour changer de piste */
#define HDA_PON_DELAY   30       /* nombre de SYSTICK avant amorce du disque */
#define HDA_POFF_DELAY  30       /* nombre de SYSTICK avant arret du disque */



unsigned int nbCyl; /* Nombre de cylindres du disque maître */
unsigned int nbSec; /* Nombre de secteurs du disque maître */
unsigned int tailleSec; /* Taille (en octet) un secteur du disque maitre */


/* Type de volume */
enum vol_type_e {
    vol_base,
    vol_annexe,
    vol_misc
};

/* Volume */
struct volume_s {
    unsigned int secteur_base;
    unsigned int cyl_base;
    unsigned int nb_secteurs;
    enum vol_type_e vol_type;
};

/* Mbr */
struct mbr_s {
    unsigned int magic;
    struct volume_s volumes[NB_VOL_MAX];
    unsigned int nb_volumes;
};

struct mbr_s mbr;

struct superbloc_s {
    unsigned int magic;
    unsigned int num_serie;
    unsigned char name[32];
    int first_free_bloc;
    int number_free_blocs;
};

struct superbloc_s superbloc;
int current_vol;

struct free_block_s {
    unsigned int size;
    unsigned int next; /* Convention si 0, dernier bloc libre */
};

enum file_type_e {
	directory, 
	file, 
	special
};

struct inode_s {
	enum file_type_e type;
	int direct[NB_DIRECT];
	int indirect;
	int double_indirect;
	int taille;
};

typedef int indir_t[NB_INDIRECT];
typedef int dbindir_t[NB_INDIRECT];


struct file_desc_s {
    unsigned int fds_inumber;   /* le numéro de l’inœud qui décrit le fichier visé  */
    unsigned int fds_pos;       /* la position en octet du curseur dans le fichier. Cette position donne l’octet à lire/écrire dans le fichier ouvert, et elle est incrémentée après chaque opération d’accès  */
    unsigned int fds_size;      /* la taille totale du fichier (utilisée lors des accès en lecture) */
    unsigned char fds_buf[256]; /* un tableau d’octets qui sert de tampon entre le bloc du disque et le programme  */
    char fds_dirty;             /* un drapeau qui indique si le tampon courant à été modifié ou pas. */
};

typedef struct file_desc_s file_desc_t; 


#endif
