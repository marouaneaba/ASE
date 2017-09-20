#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "context.h"
/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

static ctx_s * current_ctx = (ctx_s *) 0;
static ctx_s * ring_ctx = (ctx_s *) 0;

int init_ctx(struct ctx_s *ctx, int stack_size, 
				func_t f, void *args)
{
	ctx->stack = malloc(sizeof(char) * stack_size);
	assert(ctx->stack);
	
	ctx->esp = &ctx->stack[stack_size - sizeof(int)];
	ctx->ebp = &ctx->stack[stack_size -  sizeof(int)];
	
	ctx->fct = f;
	ctx->args = args;
	
	ctx->magic = CTX_MAGIC;
	ctx->state = CTX_READY;
	
		
	return 1;
}

int create_ctx(int stack_size, func_t f, void *args)
{	
	irq_disable();
	
	ctx_s * new_ctx = malloc(sizeof(ctx_s));
	init_ctx(new_ctx, stack_size, f, args);

    if (current_ctx) {
        new_ctx->next = current_ctx->next;
        current_ctx->next = new_ctx;
    }
    else {
        current_ctx = new_ctx;
        current_ctx->next = new_ctx;
    }
	
	irq_enable();
	return 1;
}


void switch_to_ctx(struct ctx_s *ctx)
{	
	assert(ctx->magic == CTX_MAGIC);
	
	irq_disable();
	
	while(ctx->state == CTX_TERMINATED){
		if(ctx->next == current_ctx){
			exit(0);
		}	
			
		free(ctx->stack);
		if(ring_ctx == ctx){
			ring_ctx = ctx->next;
		}
			
		current_ctx->next = ctx->next;
		free(ctx);
		ctx = ctx->next;
		
	}
	
	
	if(current_ctx && (current_ctx->state != CTX_READY)){ 
		/* On doit save dans le contexte courant si il existe */
		
		asm ("movl %%esp, %0" "\n\t" "movl %%ebp, %1"
			: "=r" (current_ctx->esp), "=r" (current_ctx->ebp)
			: 
			:
			);	 
					
	}
	
	current_ctx = ctx;
	
	asm ("movl %0, %%esp" "\n\t" "movl %1, %%ebp"
			: 
			: "r" (current_ctx->esp), "r" (current_ctx->ebp)
			:
			);
	
	irq_enable();
	
	if(current_ctx->state == CTX_READY ) { 
		start_current_ctx();		
	} 
	
	
}

static void start_current_ctx() {
    current_ctx->state = CTX_ACTIVABLE;
    current_ctx->fct(current_ctx->args);
    current_ctx->state = CTX_TERMINATED;    
    if (current_ctx->next == current_ctx) {
        printf("\nNo more process\n");
        exit(EXIT_SUCCESS);
    }
    irq_disable();
    struct ctx_s * tmp = current_ctx;
    while (tmp->next != current_ctx) {
        tmp = tmp->next;
    }
    tmp->next = current_ctx->next;
    free(current_ctx->stack);
    free(current_ctx);
    current_ctx = (void *) 0;
    irq_enable();
    switch_to_ctx(tmp->next);
}


void yield() {	
    assert(current_ctx);
    _out(TIMER_ALARM,0xFFFFFFFE);
    switch_to_ctx(current_ctx->next);
}

void irq_enable()
{
    _mask(1);
}

void irq_disable()
{
    _mask(3);
}

void doNothing()
{
    return;
}

void start_sched() 
{
	
    unsigned int i;
    
    /* init hardware */
    if (init_hardware(HARDWARE_INI) == 0) {
        fprintf(stderr, "Error in hardware initialization\n");
        exit(EXIT_FAILURE);
    }
    
    /* dummy interrupt handlers */
    for (i=0; i<16; i++)
        IRQVECTOR[i] = doNothing;
    
    /* program timer */
    IRQVECTOR[TIMER_IRQ] = yield;
    _out(TIMER_PARAM,128+64+32+1); /* reset + alarm on + 1 tick / alarm */
    _out(TIMER_ALARM,0xFFFFFFFE);  /* alarm at next tick (at 0xFFFFFFFF) */
    
    /* allows all IT */
    irq_enable();
    
    /* count for a while... */
    while (1) {
        pause();
    }
    
    /* and exit! */
    exit(0);
    

}

