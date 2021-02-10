#include <stdio.h>
int min_FV(int v[])
{
	int n=0;
	for(int i=0;i<=9;i++)
		for(int j=0;j<v[i];j++)
			n=n*10+i;
	return n;
}

int max_FV(int v[])
{
	int n=0;
	for(int i=9;i>=0;i--)
		for(int j=0;j<v[i];j++)
			n=n*10+i;
	return n;
}
void build_V(int nr,int v[])
{
	for(int i=0;i<=9;i++)	v[i]=0;
	while(nr)
	{
		v[nr%10]++;
		nr/=10;
	}
}
int in_H(int nr,int H[],int h)
{
	for(int i=0;i<h;i++)
		if(nr==H[i]) return i;
	return -1;
}
int main()
{
	int H[101],h=0;
	int F[10];
	int n;
	scanf("%d",&n);
	while(in_H(n,H,h)==-1)
	{
		//printf("%d\n",n);
		H[h]=n;h++;
		build_V(n,F);
		n=max_FV(F)-min_FV(F);
	}
	printf("%d\n",in_H(n,H,h)-1);
	for(int i=in_H(n,H,h);i<h;i++)
		printf("%d ",H[i]);
	printf("\n");
}
