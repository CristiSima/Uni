#include <stdio.h>
#include <stdlib.h>
#define NMAX 10
void read_array(int n,int v[])
{
	for(int i=0;i<n;i++)	
		scanf("%d",v+i);
}
void print_array(int n,int v[])
{
	for(int i=0;i<n;i++)
		printf("%d ",v[i]);
	printf("\n");
}
int sum_array(int n,int v[])
{
	int S=0;
	for(int i=0;i<n;i++)	
		S+=v[i];
	return S;
}
int min_array(int n,int v[])
{
	int min=v[0];
	for(int i=1;i<n;i++)
		if(v[i]<min)
			min=v[i];
	return min;
}
int posmax_array(int n,int v[])
{
	int max=v[0],pmax=0;
	for(int i=1;i<n;i++)
		if(max<v[i])
		{
			max=v[i];
			pmax=i;
		}
	return pmax;
}
int posmin_array(int n,int v[])
{
	int min=v[0],pmin=0;
	for(int i=1;i<n;i++)
		if(v[i]<min)
		{
			min=v[i];
			pmin=i;
		}
	return pmin;
}
double avg_array(int n,int v[])
{
	return sum_array(n,v)/n;
}
void sort_array_selection(int n,int v[])
{
	int pos,swap;
	for(int i=0;i<n;i++)
	{
		pos=posmin_array(n-i,v+i)+i;
		swap=v[i];
		v[i]=v[pos];
		v[pos]=swap;
	}
}
void sort_array_bubble(int n,int v[])
{
	int swap;
	for(int i=0;i<n;i++)
		for(int j=i+1;j<n-1;j++)
			if(v[i]>v[j])
			{
				swap=v[i];
				v[i]=v[j];
				v[j]=swap;
			}
}
void sort_array(int n,int v[])
{
	sort_array_selection(n,v);
	sort_array_bubble(n,v);
}
void multiplay_matrix(int n,int m,int p,int A[NMAX][NMAX],int B[NMAX][NMAX],int C[NMAX][NMAX])
{
	for(int i=0;i<n;i++)
		for(int k=0;k<p;k++)
			for(int j=0;j<m;j++)
				C[i][k]+=A[i][j]*B[j][k];
}
void clean_matrix(int n,int M[NMAX][NMAX])
{
	for(int i=0;i<n;i++)	for(int j=0;j<n;j++)	M[i][j]=0;
}
void neutral_matrix(int n,int M[NMAX][NMAX])
{
	clean_matrix(n,M);
	for(int i=0;i<n;i++)	M[i][i]=1;
}
void copy_matrix(int n,int m,int A[NMAX][NMAX],int B[NMAX][NMAX])
{
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			B[i][j]=A[i][j];
}
void power_matrix(int n,int A[NMAX][NMAX],int k,int B[NMAX][NMAX])
{
	int R[NMAX][NMAX];
	int P[NMAX][NMAX];
	int temp[NMAX][NMAX];
	neutral_matrix(n,B);
	copy_matrix(n,n,A,P);
	while(k)
	{
		if(k&1)	
		{
			multiplay_matrix(n,n,n,B,P,temp);
			copy_matrix(n,n,temp,B);
		}
		multiplay_matrix(n,n,n,P,P,temp);
		copy_matrix(n,n,temp,P);
		k>>=1;
	}
}

int binarysearch(int n,int v[],int nr)
{
	int st=0,dr=n-1;
	int mid=(st+dr)>>1;
	while(st!=dr)
	{
		mid=(st+dr)>>1;
		if(v[mid]==nr)	return mid;
		else if(v[mid]<nr)
			st=mid+1;
		else 
			dr=mid-1;
	}
	if(v[st]==nr)
		return st;
	else
		return -2;
}
int diff_array(int n,int v[],int m,int w[],int diff[])
{
	int d=0;
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			if(v[i]==w[j])
			{
				diff[d]=v[i];
				d++;
			}
	return d;
}
int main()
{
	int n=4,nr;
	//int *v;
	//v=malloc(4*NMAX);
	int v[NMAX]={0};
	read_array(n,v);
	print_array(n,v);
	printf("sum:%d\n",sum_array(n,v));
	printf("min:%d\n",min_array(n,v));
	printf("posmax:%d\n",posmax_array(n,v)+1);
	printf("avg:%f\n",avg_array(n,v));
	sort_array(n,v);
	print_array(n,v);
	scanf("%d",&nr);
	printf("%d e pe poz:%d\n",nr,binarysearch(n,v,nr)+1);
}
