#include "try.h"

/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

ctx_s * buf;

int main() 
{
    int esp, ebp, result;
    

    buf = malloc(sizeof(ctx_s));
    
    buf->magic = CIX_MAGIC;

	result = try(buf,mul,1);

    printf("Result : %d \n",result);

    return 0;
}

