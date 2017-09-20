#include <stdio.h>
#include <stdlib.h>

#include "mi_syscall.h"
#include "mi_user.h"
#include "hw_config.h"
#include "hardware.h"
#include "mi_kernel.h"

/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

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

/** Utilisé pour stocker le processus en cours d'execution utilisant la mémoire. */
static unsigned int current_process;

/** Méthode qui ne fait rien */
static void empty_it() {
    return;
}

/** Remise à zéro de la MMU pour éviter les mauvaises TLB translations */
static void switch_to_process0(void) {
    current_process = 0;
    _out(MMU_CMD, MMU_RESET);
}

/* Remise à zéro de la MMU pour éviter les mauvaises TLB translations */
static void switch_to_process1(void) {
    current_process = 1;
    _out(MMU_CMD, MMU_RESET);
}

/**
 *  retourne le numéro de la page physique associée à 
 *  l’adresse virtuelle vaddr du process process 
 * (0 pour le premier processus, 1 pour le second). 
 *  La fonction retourne -1 si l’adresse virtuelle 
 *  est en dehors de l’espace alloué au processus.
 */
static int ppage_of_vaddr(unsigned int process, unsigned int vaddr) {
    int offset = 0;
    if (process != 0) {
        offset = 127;
    }
    if (vaddr >= (unsigned int) virtual_memory && vaddr < (unsigned int) virtual_memory + 127 * PAGE_SIZE) {
        return ((vaddr - (unsigned int) virtual_memory) / PAGE_SIZE) + 1 + offset;
    }
    return -1;
}

/**
 * Gère les erreur d'adresse de la MMU
 */
void mmu_handler() {

	int vaddr = _in(MMU_FAULT_ADDR);
    int ppage;
    union tlb_entry_u tlb_entry;
    
    vaddr = _in(MMU_FAULT_ADDR);
    ppage = ppage_of_vaddr(current_process, vaddr);
    if (ppage < 0) {
        printf("Erreur de segmentation\n");
        exit(-1);
    }
    
    tlb_entry.entry.ppage = ppage;
    tlb_entry.entry.vpage = (vaddr - (unsigned int) virtual_memory) / PAGE_SIZE;
    tlb_entry.entry.execute = 0;
    tlb_entry.entry.write = 1;
    tlb_entry.entry.read = 1;


    /*int sum = 0;
    sum = (tlb_entry.rfu << 31) | (tlb_entry.vpage << 23) | (tlb_entry.ppage << 11) | (tlb_entry.execute << 3) | (tlb_entry.write << 2) | (tlb_entry.read << 1) | tlb_entry.used ;
	*/
	
    _out(TLB_ADD_ENTRY, tlb_entry.sum);

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
    IRQVECTOR[SYSCALL_SWTCH_0] = switch_to_process0;
    IRQVECTOR[SYSCALL_SWTCH_1] = switch_to_process1;
    
    /* Allows all IT and switch to user mode */
    _mask(0x1001);
    
    /* Starts user program execution */
    init();
    
	return 0;
}


