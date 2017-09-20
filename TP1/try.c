#include <assert.h>
#include "try.h"
/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/
int try(ctx_s *pctx, func_t *f, int arg)
{
    asm("movl %%esp, %0" "\n\t" "movl %%ebp, %1"
            :"=r"(pctx->esp),"=r"(pctx->ebp)
            :
            :);

    return (*f)(arg);
}
int throw(ctx_s *pctx, int r)
{ 	 
    static int p;
    p = r;
    
    assert( pctx->magic == CIX_MAGIC );
    
    asm("movl %0,%%esp" "\n\t" "movl %1,%%ebp"
            :
            :"r"(pctx->esp),"r"(pctx->ebp)
            :);

    return p;
}
