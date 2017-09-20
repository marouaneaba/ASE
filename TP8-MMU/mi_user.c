#include <stdio.h>
#include <string.h>

#include "mi_syscall.h"
#include "mi_user.h"
#include "hw_config.h"
#include "hardware.h"

/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

static int 
sum(void *ptr) 
{
    int i;
    int sum = 0;
    	
    for(i = 0; i < PAGE_SIZE * N; i++){
		sum += ((char*)ptr)[i];
	}
        
    return sum;
}

/**
 * Initialize the memory. Two threads set full 1 and
 * full 3 in their memory pages.
 * Then, read the memory and sum it up to see if the
 * write (and read) operation is successfull
 */
void init() {
    void *ptr;
    int res;
    ptr = virtual_memory;
    
    /* Write the memory */
    _int(SYSCALL_SWTCH_0);
    memset(ptr, 1, PAGE_SIZE * N);
    _int(SYSCALL_SWTCH_1);
    memset(ptr, 3, PAGE_SIZE * N);
    
    /* Read the memory */
    _int(SYSCALL_SWTCH_0);
    res = sum(ptr);
    printf("Resultat du processus 0 : %d\n",res);
    _int(17);
    res = sum(ptr);
    printf("Resultat processus 1 (processus 0 devrait être égal à %d) ): %d\n",res/3, res);
}
