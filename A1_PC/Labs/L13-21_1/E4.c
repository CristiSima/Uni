#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;

int cmp_int_a(const void* a,const void* b)
{
	return *(int*)a-*(int*)b;
}
int cmp_int_d(const void* a,const void* b)
{
	return *(int*)b-*(int*)a;
}
#if defined(SORT_ASC) && defined(SORT_DESC)

#endif

#ifdef SORT_ASC
	#define cmp_int cmp_int_a
#elif SORT_DESC
	#define cmp_int cmp_int_d
#else
	#error "Defineste bre SORT_ASC sau SORT_DESC"
	#define cmp_int cmp_int_d
#endif
int main(int argc,char *argv[])
{
	if(argc<=2)
	{
		fprintf(stderr, "Trebe 2 bre\n");
		return 2;
	}
	uint *V=malloc(sizeof(uint));
	for(int i=1;i<argc;i++)
	{
		int nr;
		if(sscanf(argv[i],"%d",&nr)!=1 || nr<0)
		{
			fprintf(stderr, "Numere naturale bre\n" );
			return -1;
		}
		V[i-1]=nr;
	}
	qsort(V,argc-1,4,cmp_int);
	for(int i=0;i<argc-1;i++)
		printf("%d ",V[i]);
	printf("\n");
	return 0;
}
