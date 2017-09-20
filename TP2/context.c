#include "context.h"
/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

int init_ctx(struct ctx_s *ctx, int stack_size, 
				func_t f, void *args)
{
	ctx->stack = malloc(sizeof(char) * stack_size);
	assert(ctx->stack);
	
	ctx->esp = &ctx->stack[stack_size - sizeof(int)];
	ctx->ebp = &ctx->stack[stack_size - 2 * sizeof(int)];
	
	ctx->fct = f;
	ctx->args = args;
	
	ctx->magic = CTX_MAGIC;
	ctx->state = CTX_READY;
	
		
	return EXIT_SUCCESS;
}

void switch_to_ctx(struct ctx_s *ctx)
{
	
	assert(ctx->magic == CTX_MAGIC);
	assert(ctx->state != CTX_TERMINATED);
	
	if(current_ctx){ 
		// On doit save dans le contexte courant si il existe
		
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
	
	
	if(current_ctx->state == CTX_READY ) { 
		//Premier appel sur ce context				
		current_ctx->state = CTX_ACTIVABLE;			
		current_ctx->fct(current_ctx->args);
			
		//Si on revient ici , c'est que la fonction est terminée
		current_ctx->state = CTX_TERMINATED;
		free(current_ctx->stack);
					
	} else { 
				
		return;
	}
	
}

