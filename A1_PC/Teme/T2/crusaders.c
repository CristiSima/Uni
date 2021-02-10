#include <stdio.h>
#include <stdlib.h>
#define B0 0x000000FF
#define B1 0x0000FF00
#define B2 0x00FF0000
#define B3 0xFF000000

// Simache Cristian

int switch_endian_int(int val)
{
	return ((val&B0)<<24) | ((val&B1)<<8) | ((val&B2)>>8) | ((val&B3)>>24);
}

int switch_endian_short(int val)
{
	val&=0xFFFF;
	return (val<<8 | val>>8)&0xFFFF;
}

// wrapper la realloc + initializare
void *expand(void *p,int old_size,int size)
{
	void *q=NULL;
	while(!q)
		q=realloc(p,size);

	for(int i=old_size;i<size;i++)
		((char *)q)[i]=0;

	return q;
}

void set_I(int **map,int i,int j,int val)
{
	if(map[i][0]<j)
	{
		map[i]=expand(map[i],map[i][0]*4+4,sizeof(int)*(j+1));
		map[i][0]=j;
	}

	map[i][j]=switch_endian_int(val);
}

void set_S(int **map,int i,int j,int val)
{
	// pozitia in vectorul de tip int
	int real=((j-1)>>1)+1;
	if(map[i][0]<real)
	{
		map[i]=expand(map[i],map[i][0]*4+4,sizeof(int)*(real+1));
		map[i][0]=real;
	}

	if(j&1)
	{
		// AASS reseteaza partea din stanga si o suprascrie cu val
		map[i][real]&=0x0000FFFF;
		map[i][real]|=(switch_endian_short(val)<<16);
	}
	else
	{
		// SSAA reseteaza partea din stanga si o suprascrie cu val
		map[i][real]&=0xFFFF0000;
		map[i][real]|=switch_endian_short(val);
	}
}

void set_C(int **map,int i,int j,int val)
{
	// pozitia in vectorul de tip int
	int real=((j-1)>>2)+1;
	if(map[i][0]<real)
	{
		map[i]=expand(map[i],map[i][0]*4+4,sizeof(int)*(real+1));
		map[i][0]=real;
	}

	if((j&0b11)==0b01)
	{
		//CXXX
		map[i][real]&=0x00FFFFFF;
		map[i][real]|=val<<24;
	}
	else if((j&0b11)==0b10)
	{
		//XCXX
		map[i][real]&=0xFF00FFFF;
		map[i][real]|=val<<16;
	}
	else if((j&0b11)==0b11)
	{
		//XXCX
		map[i][real]&=0xFFFF00FF;
		map[i][real]|=val<<8;
	}
	else
	{
		//XXXC
		map[i][real]&=0xFFFFFF00;
		map[i][real]|=val;
	}
}

int get_I(int **map,int i,int j)
{
	return map[i][j];
}

int get_S(int **map,int i,int j)
{
	// pozitia in vectorul de tip int
	int real=((j-1)>>1)+1;
	if(j&1)
		// AASS reseteaza partea din stanga si o suprascrie cu val
		return (map[i][real]&0xFFFF0000)>>16;

	// SSAA reseteaza partea din stanga si o suprascrie cu val
	return (map[i][real]&0x0000FFFF)>>0;
}

int get_C(int **map,int i,int j)
{
	// pozitia in vectorul de tip int
	int real=((j-1)>>2)+1;
	if((j&0b11)==0b01)
		//CXXX
		return (map[i][real]&0xFF000000)>>24;
	else if((j&0b11)==0b10)
		//XCXX
		return (map[i][real]&0x00FF0000)>>16;
	else if((j&0b11)==0b11)
		//XXCX
		return (map[i][real]&0x0000FF00)>>8;

	//XXXC
	return (map[i][real]&0x000000FF)>>0;
}

int sum_line(int **map,int i)
{
	int sum=0;
	for(int j=1;j<=map[i][0]*4;j++)
		sum+=(char)get_C(map,i,j);
	return sum;
}

int sum_collons(int **map,int n)
{
	int sum=0;
	for(int i=2;i<n;i++)
		sum+=((char)get_C(map,i,1))+((char)get_C(map,i,map[i][0]*4));
	return sum;
}

void task1(int n,int **map)
{
	int sum=0,nr=0;
	sum+=sum_line(map,1);
	nr+=4*map[1][0];

	sum+=sum_line(map,n);
	nr+=4*map[n][0];

	sum+=sum_collons(map,n);
	nr+=2*(n-2);
	double ma=(double)sum/nr;
	printf("task 1\n%.8f\n",ma);
}

void proces_command(int **map)
{
	char operation,type;
	int i,j;
	int val;
	scanf("\n%c %c %d %d",&operation,&type,&i,&j);
	// align
	i++;
	if(operation=='M')
	{
		scanf("%x",&val);

		if(type=='I')
			set_I(map,i,j,val);
		else if(type=='S')
			set_S(map,i,j,val);
		else// C
			set_C(map,i,j,val);
	}
	else if(operation=='S')
	{
		// align
		j++;
		if(type=='I')
			set_I(map,i,j,get_I(map,i,j));
		else if(type=='S')
			set_S(map,i,j,get_S(map,i,j));
		else// C
			set_C(map,i,j,get_C(map,i,j));
	}
	else
	{
		// D
		if(type=='I')
			set_I(map,i,j,0);
		else if(type=='S')
			set_S(map,i,j,0);
		else// C
			set_C(map,i,j,0);
	}
}

void print_map(int n,int **map)
{
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=map[i][0];j++)
			printf("%08X ",switch_endian_int(map[i][j]));
		printf("\n");
	}
}

void task2(int n,int **map)
{
	int N;
	scanf("%d",&N);
	while(N)
	{
		proces_command(map);
		N--;
	}
	printf("task 2\n");
	print_map(n,map);
}

int get_black_hole_size(int n,int **map,int x,int y)
{
	int sum=1;
	if(get_C(map,x,y)!=0)
		return 0;
	set_C(map,x,y,1);

	//stanga
	if(y>1)
		sum+=get_black_hole_size(n,map,x,y-1);
	// dreapta
	if(y<map[x][0]*4)
		sum+=get_black_hole_size(n,map,x,y+1);
	// sus
	if(1<x && y<=map[x-1][0]*4)
		sum+=get_black_hole_size(n,map,x-1,y);
	// jos
	if(x<n && y<=map[x+1][0]*4)
		sum+=get_black_hole_size(n,map,x+1,y);

	return sum;
}

void task3(int n,int **map)
{
	int max_size=0,x=0,y=0,size=0;
	for(int i=1;i<=n;i++)
		for(int j=1;j<=map[i][0]*4;j++)
		{
			size=get_black_hole_size(n,map,i,j);
			if(max_size<size)
			{
				max_size=size;
				x=i;
				y=j;
			}
		}
	printf("task 3\n%d %d %d\n",x-1,y-1,max_size);
}

int **read_map(int n)
{
	int **map=(int **)malloc(sizeof(int *)*(n+1));
	int aux;
	for(int i=1;i<=n;i++)
	{
			scanf("%d",&aux);
			map[i]=(int *)malloc(sizeof(int)*(aux+1));
			map[i][0]=aux;
			for(int j=1;j<=aux;j++)
			{
				scanf("%x",map[i]+j);
				map[i][j]=switch_endian_int(map[i][j]);
			}
	}
	return map;
}

int main(void)
{
	int n;
	scanf("%d",&n);
	int **map=read_map(n);

	task1(n,map);
	task2(n,map);
	task3(n,map);

	for(int i=1;i<=n;i++)
		free(map[i]);
	free(map);

	return 0;
}
