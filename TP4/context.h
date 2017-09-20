#ifndef CONTEXT_H
#define CONTEXT_H

#ifndef CTX_MAGIC
#define CTX_MAGIC 0xDEADBEEF
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hardware.h"
#include "hw_config.h"

enum ctx_state
{
		CTX_READY, /* Never actived */
		CTX_ACTIVABLE, /* currently active */
		CTX_TERMINATED, /* terminate */
		CTX_BLOCKED /* waiting sem */
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
    
    struct ctx_s * next;
    
} ctx_s;

static ctx_s * current_ctx = (ctx_s *) 0;
static ctx_s * ring_ctx = (ctx_s *) 0;

int init_ctx(struct ctx_s *, int , func_t *, void *);
int create_ctx(int , func_t , void *);
void switch_to_ctx(ctx_s *);
void yield();
static void start_current_ctx();
void start_sched();
void doNothing();
void irq_enable();
void irq_disable();

#endif

