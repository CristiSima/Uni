#include <stdio.h>
#include <stdlib.h>

#define SWAP(a,b) b^=a;a^=b;b^=a;

int main(int argc,char *argv[])
{

	int ia=1,ib=2;
	printf("%d %d\n",ia,ib);
	SWAP(ia,ib)
	printf("%d %d\n",ia,ib);

	printf("\n");

	char ca='1',cb='2';
	printf("%c %c\n",ca,cb);
	SWAP(ca,cb)
	printf("%c %c\n",ca,cb);
	return 0;
}
