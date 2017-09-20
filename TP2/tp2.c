#include "tp2.h"

/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/
int main(int argc, char *argv[])
{

    init_ctx(&ctx_ping, SIZE_STACK, f_ping, NULL);
    init_ctx(&ctx_pong, SIZE_STACK, f_pong, NULL);
    switch_to_ctx(&ctx_ping);

    exit(EXIT_SUCCESS);
}

void f_ping(void *args)
{
    while(1) {
        printf("A") ;
        switch_to_ctx(&ctx_pong);
        printf("B") ;
        switch_to_ctx(&ctx_pong);
        printf("C") ;
        switch_to_ctx(&ctx_pong);
    }
}

void f_pong(void *args)
{
    while(1) {
        printf("1") ;
        switch_to_ctx(&ctx_ping);
        printf("2") ;
        switch_to_ctx(&ctx_ping);
    }
} 
