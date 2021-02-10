#include <stdarg.h>
#include <stdio.h>

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

int gcd(int nr,...)
{
	va_list va;
	nr+=100;
	va_start(va,nr);
	nr-=100;
	int arg;
	arg=va_arg(va,int);
	while(arg>0)
	{
		nr=cmmdc(nr,arg);
		arg=va_arg(va,int);
	}
	return nr;
}
int main(void)
{
	printf("GCD: %d\n",gcd(5,10,-1));
	printf("GCD: %d\n",gcd(5,10,2,100,-1));
	printf("GCD: %d\n",gcd(500,10,25,75,-1));
	printf("GCD: %d\n",gcd(1024,48,64,256,2048,-1));
}
