#ifndef _MBR_H_
#define _MBR_H_

/*MBR MAGIC de la structure mbr*/
#define MBR_MAGIC 0xDD

 /*Différent type de volume*/
typedef enum {BASE_VOL,ANNEX_VOL,OTHER} E_VOLUME_TYPE;
typedef struct S_VOLUME S_VOLUME;
typedef struct S_MBR S_MBR;

void save_mbr();
void load_mbr();
void dump_mbr();

#endif
