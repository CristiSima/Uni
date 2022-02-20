#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "circles.h"

int distanteP2(circle c1,circle c2)
{
	int x=c1.x-c2.x;
	int y=c1.y-c2.y;
	return square(x)+square(y);
}

int max(int a,int b)
{
	if(a>b)
		return a;
	return b;
}

int main()
{
	int n;
	int i,j;
	circle* circles;
	int perechi=0;
	scanf("%d",&n);
	circles=malloc(n*sizeof(circle));
	DIE(circles==NULL,"circles malloc fail");
	for(i=0;i<n;i++)
		scanf("%d %d %d",&circles[i].x,&circles[i].y,&circles[i].r);
	for(i=0;i<n;i++)
		for(j=i+1;j<n;j++)
			if(distanteP2(circles[i],circles[j])<=square(circles[i].r+circles[j].r))
				perechi++;
	printf("%d\n",perechi );
	free(circles);
    return 0;
}
