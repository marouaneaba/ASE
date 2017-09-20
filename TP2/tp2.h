#ifndef TP2_H
#define TP2_H

#include "context.h"

#define SIZE_STACK 16384


static ctx_s ctx_ping; 
static ctx_s ctx_pong; 


void f_ping(void *args);
void f_pong(void *args);


#endif
