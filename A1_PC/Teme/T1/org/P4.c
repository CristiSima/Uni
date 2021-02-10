#include <stdio.h>
int main()
{
	int M[101][101];
	int T;
	scanf("%d",&T);
	int n,m;
	int L[101][52];
	int C[101][52];
	int F=1;
	while(T)
	{
		T--;
		scanf("%d %d",&n,&m);
		for(int i=1;i<=n;i++)
		{
			scanf("%d",L[i]);
			for(int j=1;j<=L[i][0];j++)
				scanf("%d",L[i]+j);
		}
		for(int i=1;i<=m;i++)
		{
			scanf("%d",C[i]);
			for(int j=1;j<=C[i][0];j++)
				scanf("%d",C[i]+j);
		}
		for(int i=1;i<=n;i++)
			for(int j=1;j<=m;j++)
				scanf("%d",M[i]+j);
		F=1;
		for(int i=1;i<=n && F;i++)
		{
			//printf("%d\n",i);
			int j=1;
			for(int k=1;k<=L[i][0] && F;k++)
			{
				while(!M[i][j] && j<=m)j++;
				if(j>m)
				{
					F=0;
					break;
				}
				for(int l=1;l<=L[i][k];l++)
				{
					if(!M[i][j])	{F=0;break;}
					j++;
				}
				if(M[i][j] && j<=m)
					F=0;
			}
			if(!F)
				break;
			for(;j<=m;j++)
				if(M[i][j])
					F=0;
		}
		for(int j=1;j<=m && F;j++)
		{
			int i=1;
			for(int k=1;k<=C[j][0] && F;k++)
			{
				while(!M[i][j] && i<=n)i++;
				if(i>n)
				{
					F=0;
					break;
				}
				for(int l=1;l<=C[j][k];l++)
				{
					if(!M[i][j])	{F=0;break;}
					i++;
				}
				if(M[i][j] && i<=n)
					F=0;
			}
			if(!F)
				break;
			for(;i<=n;i++)
				if(M[i][j])
					F=0;
		}
		if(!F)
			printf("Eroare\n");
		else
			printf("Corect\n");
	}
	return 0;
}
