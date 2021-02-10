#include <bitset.h>
#include <stdlib.h>

bitset bitset_gen(int size)
{
	return (bitset) malloc((size>>3)+1);
}
void bitset_free(bitset S)
{
	free(S);
}
int bitset_get(bitset S,int pos)
{
	return (S[pos>>3]&(1<<(pos&7)))>0;
}
void bitset_set(bitset S,int pos)
{
	S[pos>>3]|=(1<<(pos&7));
}
void bitset_reset(bitset S,int pos)
{
	S[pos>>3]&=~(1<<(pos&7));
}
void bitset_flip(bitset S,int pos)
{
	if(bitset_get(S,pos))
		bitset_reset(S,pos);
	else
		bitset_set(S,pos);
}
