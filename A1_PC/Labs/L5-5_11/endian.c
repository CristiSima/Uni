#include <stdio.h>

int main()
{
	int p=1;
	if((char)p==1)
		printf("Littl-endian\n");
	else	
		printf("Big-endian\n");
}
