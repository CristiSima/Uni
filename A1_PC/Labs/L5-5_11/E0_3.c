#include <stdio.h>
#define NMAX 10
double get(double *p,int x,int y)
{
	return *(p+NMAX*x+y);
}
int main()
{
	double M[NMAX][NMAX]={0};
	double *p=(double *)&M;
	M[4][2]=999;
	printf("%f\n",get(p,4,2));
	return 0;
}
