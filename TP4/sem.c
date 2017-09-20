#include "sem.h"

/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

void sem_init(struct sem_s * sem, unsigned int val) {
	sem->count = val;
	sem->blocked = (struct ctx_s *) 0;
}

void sem_up(struct sem_s * sem) {
	
	irq_disable();
	
	sem->count++;
	
	// No ressource available
	if (sem->count <= 0) {
				
		struct ctx_s * tmp = sem->blocked;
		sem->blocked = tmp->next;
		
		// Insertion in the scheduler listl
		tmp->next = current_ctx->next;
		current_ctx->next = tmp;
		
		tmp->state = CTX_ACTIVABLE;
	}	

	irq_enable(); 
}
 
void sem_down(struct sem_s * sem) {
	// Be sure the current context is not the last in the scheduler list, 
	// else deadlock
	//assert(current_ctx->next != current_ctx);	
	
	irq_disable();
	
	sem->count--;
	
	// If the context must be blocked
	if (sem->count < 0) {
		
		// Save the context to extract of the scheduler m
		struct ctx_s * tmp = current_ctx;
		
		// Find the last item of the list
		while (tmp->next != current_ctx) {
			tmp = tmp->next;
		}
		
		
		tmp->next = current_ctx->next;
		current_ctx->next = sem->blocked;	
		sem->blocked = current_ctx;	
		sem->blocked->state = CTX_BLOCKED;
        
		switch_to_ctx(tmp->next);
	}
	
	irq_enable();
}
