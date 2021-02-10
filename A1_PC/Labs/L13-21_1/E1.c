#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("Got %d args\n",argc-1 );
	for(int i=1;i<argc;i++)
		printf("argv[%d]=%s\n",i,argv[i]);
	return 0;
}
