#include <stdio.h>
#include <bitset.h>
#include <set.h>

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
void demo_cmmdc()
{
	int a,b;
	scanf("%d %d",&a,&b);
	printf("cmmdc: %d\n",cmmdc(a,b));
}
void demo_bitset()
{
	int nr=101;
	bitset F=bitset_gen(nr);
	printf("%d\n",bitset_get(F,16));
	bitset_set(F,16);
	printf("%d\n",bitset_get(F,16));
}
void demo_set()
{
	SET s={0};
	print_set(s);
	insert_in_set(s,4);
	insert_in_set(s,8);
	print_set(s);
	delete_from_set(s,4);
	print_set(s);
}
int main()
{
	// demo_cmmdc();
	// demo_bitset()
	demo_set();
}
