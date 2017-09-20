#include <stdio.h>
#include <stdlib.h>

#include "hw_config.h"
#include "hardware.h"
#include "swap.h"
#include "mmu_manager.h"



/** Utilisation d'une union pour caster la struct en int
*
* http://stackoverflow.com/questions/11903820/casting-struct-into-int
*
* Structure d'une entrée TLB */
union tlb_entry_u {
  struct tlb_entry_s {
    unsigned int rfu : 8;
    unsigned int vpage : 12;
    unsigned int ppage : 8;
    unsigned int execute : 1;
    unsigned int write : 1;
    unsigned int read : 1;
    unsigned int used : 1;
  } entry;
  int sum;
};

struct vm_mapping_s {
  unsigned int ppage : 8;
  unsigned int loaded : 1;
};

struct pm_mapping_s {
  unsigned int vpage : 12;
  unsigned int loaded : 1;
};

/* Page virtuelle vers page physique */
static struct vm_mapping_s vm_mapping[VM_PAGES];
/* Page physique vers page virtuelle */
static struct pm_mapping_s pm_mapping[PM_PAGES];

extern void user_process();

/* Pour le round robin */
static int current_page = 1;


static void empty_it() {
  return;
}

/**
* Gère les erreur d'adresse de la MMU
*/
void mmu_handler() {

  union tlb_entry_u tlb;
  unsigned int vpage;
  unsigned int ppage;

	/* printf("Handler, current_page : %d\n", current_page); */

  /* récupération de la page fautive */
  vpage = _in(MMU_FAULT_ADDR);
  if (vpage < (unsigned int) virtual_memory || vpage > (unsigned int) virtual_memory + VM_SIZE) {
    perror("SEG FAULT\n");
    exit(EXIT_FAILURE);
  }

  /* récupération du n° de la page */
  vpage = (vpage - (unsigned int) virtual_memory) / PAGE_SIZE;

  /* test si la page est déjà chargée */
  if(vm_mapping[vpage].loaded){
    ppage = vm_mapping[vpage].ppage;
  } else { /* si la page n'est pas chargée */

    /* Round robin */
    ppage = (current_page++);

    if(current_page >= PM_PAGES){
      ppage = 1;
    }

    /* Il y avait une pagé chargé à cette emplacement, faut la save*/
    if(pm_mapping[ppage].loaded){
      store_to_swap(pm_mapping[ppage].vpage, ppage);
      vm_mapping[pm_mapping[ppage].vpage].loaded = 0;
    }
    tlb.entry.ppage = ppage;
    _out(TLB_DEL_ENTRY, tlb.sum);

    /* On peut maintenant aller récuperer la page */
    fetch_from_swap(vpage, ppage);

    /* et actualiser les tables de mapping */
    vm_mapping[vpage].ppage = ppage;
    vm_mapping[vpage].loaded = 1;
    pm_mapping[ppage].vpage = vpage;
    pm_mapping[ppage].loaded = 1;
  }

  /* Actualiser le tlb */
  tlb.entry.ppage = ppage;
  tlb.entry.vpage = vpage;
  tlb.entry.execute = 1 ;
  tlb.entry.read = 1;
  tlb.entry.write = 1;

  _out(TLB_ADD_ENTRY, tlb.sum);
}




/**
* Initialise le hardware et les gestionnaires d'interruptions
* Démarre le programme utilisateur
*/
int main(int argc, char **argv) {
  unsigned int i;

  /* init hardware */
  if (init_hardware("hardware.ini") == 0) {
    fprintf(stderr, "Error in hardware initialization\n");
    exit(-1);
  }

  /* Interrupt handlers */
  for (i=0; i<16; i++) {
    IRQVECTOR[i] = empty_it;
  }

  IRQVECTOR[MMU_IRQ] = mmu_handler;

  /* Initialises the swap file */
  if (init_swap("swap_file") < 0) {
    perror("Error in swap file creation");
  }

  /* Allows all IT and switch to user mode */
  _mask(0x1001);

  /* Starts user program execution */
  user_process();

  return 0;
}
