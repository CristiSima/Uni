#include <stdio.h>
int main()
{
	int *a=0x00;
	int *b=a;
	++a;
	printf("%p\n",(void *)(a-b));
	int c;
	int d;
	printf("%p\n",(void *)((void *)d-(void *)c));
	unsigned long *p;	
	printf("\n\np=&p;\n");
	p=&p;
	while(1)	p=*p;
}
