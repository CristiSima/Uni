#include <stdio.h>
#include <stdlib.h>

void read_array(int *n,int **v)
{
	scanf("%d",n);
	*v=(int *) malloc(sizeof(int)*(*n));
	if(*v==NULL)
	{
		printf("Faild to get memory for size:%d\n",*n);
		exit(-1);

	}
	for(int i=0;i<*n;i++)
		scanf("%d",(*v)+i);
}
void print_array(int n,int *v)
{
	printf("%d\n",n);
	for(int i=0;i<n;i++)
		printf("%d ",v[i]);
	printf("\n");
}
void E1()
{
	int n;
	int *v;
	int M[2][2];
	v=*M;
	read_array(&n,&v);
	print_array(n,v);
	free(v);
}
void t(int n[][5]);
int **alloc_matrix(int n,int m)
{
	//return (int **)malloc(sizeof(int)*n*m);
	int **M=(int **)malloc(sizeof(int*)*n);
	if(M==NULL)
		return NULL;
	for(int i=0;i<n;i++)
	{
		M[i]=(int *)malloc(sizeof(int)*m);
		if(M[i]==NULL)
		{
			for(int j=0;j<i;j++)
				free(M[j]);
			return NULL;
		}
	}
	return M;
}
void read_matrix(int n,int m,int **a)
{
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			scanf("%d",a[i]+j);
}
void print_matrix(int n,int m,int **a)
{
	printf("%dx%d\n",n,m);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
			printf("%d ",a[i][j]);
		printf("\n");
	}
}
void E2()
{
	int **M;
	int n,m;
	scanf("%d %d",&n,&m);
	M=alloc_matrix(n,m);
	if(M==NULL)	return;
	read_matrix(n,m,M);
	print_matrix(n,m,M);

}
int *alloc_Vmatrix(int n,int m)
{
	return (int *)malloc(sizeof(int)*n*m);
}
void read_Vmatrix(int n,int m,int *a)
{
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			scanf("%d",a+(i*n)+j);
}
void print_Vmatrix(int n,int m,int *a)
{
	printf("%dx%d\n",n,m);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
			printf("%d ",(a+i*n)[j]);
		printf("\n");
	}
}
void E2_5()
{
	int *M;
	int n,m;
	scanf("%d %d",&n,&m);
	M=alloc_Vmatrix(n,m);
	if(M==NULL)	return;
	read_Vmatrix(n,m,M);
	print_Vmatrix(n,m,M);

}
int main()
{
	//E2();
	E2_5();
	return 0;
}
