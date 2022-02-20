#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int concat_line(int *line,int m)
{
	int S=0;
	int i;
	for(i=0;i<m;i++)
		S=(S*10)+line[i];
	return S;
}

int main()
{
	int n,m;
	int i,j;
	int **matrix;
	int S=0;

	scanf("%d %d",&n,&m);

	matrix=malloc(sizeof(int*)*n);
	DIE(matrix==NULL,"matrix malloc fail");

	for(i=0;i<n;i++)
	{
		matrix[i]=malloc(sizeof(int)*m);
		if(matrix[i]==NULL)
		{
			for(j=0;j<i;j++)
				free(matrix[i]);
			free(matrix);
			DIE(1,"matrix[] malloc fail");
		}
		for(j=0;j<m;j++)
			scanf("%d",matrix[i]+j);
	}

	for(i=0;i<n;i++)
		S+=concat_line(matrix[i],m);

	printf("%d\n",S);

	for(i=0;i<n;i++)
		free(matrix[i]);
	free(matrix);

	return 0;
}
