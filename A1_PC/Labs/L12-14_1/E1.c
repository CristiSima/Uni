#include <stdarg.h>
#include <stdio.h>

void spam(int argc,...)
{
	va_list ap;
	va_start(ap,argc);
	int args[argc];
	for(int i=0;i<argc;i++)
		args[i]=va_arg(ap,int);
	printf("Spaming %d elements\n", argc);
	for(int i=0;i<argc;i++)
	{
		for(int j=i;j<argc;j++)
			printf("%d ",args[j]);
		printf("\n");
	}
	va_end(ap);
}

int cmmdc(int a,int b)
{
	// printf("%d %d\n",a,b);
	if(a==b)
		return a;
	if((a&1)==0 && (b&1)==0)
		return cmmdc(a>>1,b>>1)<<1;
	if((a&1)==0)
		return cmmdc(a>>1,b);
	if((b&1)==0)
		return cmmdc(a,b>>1);
	// de aici am trisat
	if(a>b)
		return cmmdc((a-b)>>1,b);
	return cmmdc(a,(b-a)>>1);
}

int main(void)
{
	// spam(4,1,2,3,4);

}
