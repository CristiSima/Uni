#include <stdio.h>

void swap_int(int *a,int *b)
{
	int c=*a;
	*a=*b;
	*b=c;
}
void swap_ptr(int *a,int *b)
{
	int *c=a;
	a=b;
	b=c;
}
void my_memcpy(void *dst, const void *src,int num)
{
	#define BYTE unsigned char
	for(int i=0;i<num;i++)
		*((BYTE *)dst+num)=*((BYTE *)src+num);
}
int main()
{
	int a=1,b=2;
	printf("%d %d\n",a,b);
	swap_int(&a,&b);
	printf("%d %d\n",a,b);
	
	printf("\n");

	int *c=0x1,*d=0x2;
	printf("%p %p\n",c,d);
	swap_int(&c,&d);
	printf("%p %p\n",c,d);

	printf("\n");

	//4.1: ca sa mearga pe toate tipurile de date
	//4.2 :pt a nu modifica sursa
	int o[4]={1,2,3,4};
	const int *t=&o;
	int *l=t+1;
	printf("%d\n",*t);
	//t[0]=9;
	printf("%d\n",*t);
	
	printf("\n");

	my_memcpy(&a,&b,4);
	printf("%d %d\n",a,b);
}
