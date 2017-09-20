#include <stdio.h>
#include <stdint.h>

#include "hardware.h"
#include "hw_config.h"
#include "swap.h"

static FILE *swap_file = NULL;

/** Ouvre le fichier qui sera utilisé comme swap */
char init_swap(const char *path) {
    swap_file = fopen(path, "w+"); /* w+: create, read, write*/
    return swap_file == NULL ? -1 : 0;
}
/** Stocke dans le fichier de swap la page virtuelle de numéro vpage
 *  dont le contenu est actuellement stocké dans la page physique ppage
 * */
 char store_to_swap(int vpage, int ppage) {
	 /* vpage << 12 parce que vpage est un numéro de page, donc on doit multipler par 4096, d'ou le <<12) */
    if (fseek(swap_file, vpage << 12, SEEK_SET) == -1)
        return -1;
        //     (str[],1:debut de position,sizeof(str),fichier)
    if (fwrite((void*)((ppage << 12) | (uintptr_t)physical_memory), 1, PAGE_SIZE, swap_file) == -1)
        return -1;
  return 0;
}
/** La fonction fetch_from_swap() lit dans le fichier de swap la page
 * virtuelle numéro vpage et en écrit le contenu dans la page physique
 * ppage
 * */
char fetch_from_swap(int vpage, int ppage) {
    if (fseek(swap_file, vpage << 12, SEEK_SET) == -1)
        return -1;
        //(buffer,nbr bit a lire,1:debut de position ,fichier)
    if (fread((void*)((ppage << 12) | (uintptr_t)physical_memory),1, PAGE_SIZE, swap_file) == -1)
        return -1;
    return 0;
}
