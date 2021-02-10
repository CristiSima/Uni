#include <stdio.h>
#include <stdlib.h>
#include "E2_h.h"
int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		printf("YOU FAILD\nAKA\nNuma 2 argumente nenea\n");
		return -1;
	}

	int a,b;
	a=atoi(argv[1]);
	b=atoi(argv[2]);
	
	printf("%d+%d=%d\nDa stiu, sunt destept\nSi folosind o functie chiar\n",a,b,sum(a,b));
}
