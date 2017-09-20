#include "try.h"
/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/
int mul(int depth)
{		
    int i;

    printf("Read : ");

    switch(scanf("%d", &i))
    {
        case EOF:
            return 1; /* neutral element*/
        case 0:
            return mul(depth+1); /*error*/
        case 1:
            if(i==0)
                throw(buf, 0);
            else
                return i * mul(depth+1);


    }
}
