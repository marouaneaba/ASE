#ifndef TRY_H
#define TRY_H
#include <stdio.h>
#include <stdlib.h>
#define CIX_MAGIC 0x42 

typedef int (func_t)(int);
typedef struct ctx_s ctx_s;


struct ctx_s
{
    int esp;
    int ebp;
    unsigned magic;
};

int mul(int);
int try(ctx_s *, func_t *, int);
int throw(ctx_s *, int);


extern ctx_s * buf;

#endif
