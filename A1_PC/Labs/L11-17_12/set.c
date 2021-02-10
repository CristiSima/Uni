#include <stdio.h>
#include "set.h"

void print_set(SET s)
{
	printf("In set: ");
	for(int i=0;i<SET_SIZE<<3;i++)
	{
		if(is_in_set(s,i))
			printf("%d ",i);
	}
	printf("\n");
}

void insert_in_set(SET s,unsigned int n)
{
	s[n>>3]|=(1<<(n&7));
}
void delete_from_set(SET s, unsigned int n)
{
	s[n>>3]&=~(1<<(n&7));
}
int is_in_set(SET s, unsigned int n)
{
	return (s[n>>3]&(1<<(n&7)))>0;
}
void delete_all_from_set(SET s)
{
	for(int i=0;i<SET_SIZE;i++)
	{
		s[i]=0;
	}
}
int card_set(SET s)
{
	int tot=0;
	for(int i=0;i<SET_SIZE<<3;i++)
		if(is_in_set(s,i))
			tot+=1;
	return tot;
}
int is_empty_set(SET s)
{
	for(int i=0;i<SET_SIZE;i++)
		if(S[i]!=0)
			return 0;
	return 1;
}
