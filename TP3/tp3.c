#include "tp3.h"
#include <unistd.h>
/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

int main(int argc, char *argv[])
{

    //init_ctx(&ctx_ping, SIZE_STACK, f_ping, NULL);
    //init_ctx(&ctx_pong, SIZE_STACK, f_pong, NULL);
    create_ctx(SIZE_STACK, f_ping, NULL);
    create_ctx(SIZE_STACK, f_pong, NULL);
    
    start_sched();

    exit(0);
}

void f_ping(void *args)
{
    while(1) {
        printf("A") ;
        printf("B") ;
        printf("C") ;
		
    }
}

void f_pong(void *args)
{
	 while(1) {
        printf("1") ;
        printf("2") ;
		
     }
    
} 
 
