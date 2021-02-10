#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef int(*cmp_t)(const void*,const void*);

void sort(void *v,size_t num, size_t size, cmp_t cmp)
{
	void *temp=malloc(size);
	for(size_t i=0;i<num;i++)
		for(size_t j=i+1;j<num;j++)
			if(cmp(v+size*i,v+size*j)>0)
			{
				memcpy(temp		,v+size*i	,size);
				memcpy(v+size*i	,v+size*j	,size);
				memcpy(v+size*j	,temp		,size);
			}
	free(temp);
}
int cmp_int(const void *first, const void *second)
{
	int f=*(int*)first;
	int s=*(int*)second;
	if(f<s)
		return -1;
	if(f>s)
		return  1;
	return 0;
}
int cmp_char(const void *first, const void *second)
{
	char f=*(char*)first;
	char s=*(char*)second;
	if(f<s)
		return -1;
	if(f>s)
		return  1;
	return 0;
}
void vect_print(int n,int *V)
{
	for(int i=0;i<n;i++)
		printf("%d ",V[i]);
	printf("\n");
}
int main(void)
{
	int n=7;
	int V[]={1,5,7,3,2,33,5};
	char C[]="1253728";
	vect_print(n,V);
	printf("%s\n",C);
	sort(V,n,sizeof(int),cmp_int);
	sort(C,n,sizeof(char),cmp_char);
	vect_print(n,V);
	printf("%s\n",C);
}
