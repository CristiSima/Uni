typedef unsigned char *bitset;

bitset bitset_gen(int size);
int bitset_get(bitset S,int pos);
void bitset_set(bitset S,int pos);
void bitset_reset(bitset S,int pos);
void bitset_flip(bitset S,int pos);
