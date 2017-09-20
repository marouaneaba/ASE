#ifndef CONTEXT_H
#define CONTEXT_H

#define CTX_MAGIC 0x42 

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


enum ctx_state
{
		CTX_READY, //Never actived
		CTX_ACTIVABLE, //currently active
		CTX_TERMINATED //terminate
};

typedef void (func_t)(void *);

typedef struct ctx_s
{
    void * esp;
    void * ebp;
    unsigned int magic;
    func_t * fct;
    void * args;
    unsigned char * stack;
    enum ctx_state state;
    
} ctx_s;

static ctx_s * current_ctx = (ctx_s *) 0;

int init_ctx(struct ctx_s *, int , func_t *, void *);
void switch_to_ctx(ctx_s *) ;


#endif

