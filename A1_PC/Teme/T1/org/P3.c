#include <stdio.h>
int main()
{
	int n;
	int V[101];
	int temp;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
		scanf("%d",V+i);
	int P=0;
	for(int i=0;i<n;i++)
	{
		scanf("%d",&temp);
		P+=V[i]*temp;
		V[i]=(10-V[i])*temp;
	}
	int p_min;	
	scanf("%d",&p_min);
	for(int i=0;i<n-1;i++)
	{
		for(int j=i+1;j<n;j++)
			if(V[i]>V[j])
			{
				temp=V[i];
				V[i]=V[j];
				V[j]=temp;
			}
		//printf("%d\n",V[i]);
	}
	for(int i=n-1;i>0;i--)
	{
		//printf("%d|%d\n",P,V[i]);
		if(P>=p_min)
		{
			printf("%d\n",n-i-1);
			return 0;
		}
		P+=V[i];
	}
	if(P>=p_min)
		printf("%d\n",n-1);
	else
		if(P+V[0]>=p_min)
			printf("%d\n",n);
		else
			printf("-1\n");
	return 0;
}
