#include <stdio.h>
void TASK_1()
{
	int n;
	scanf("%d",&n);
	printf("test1:%d\n",n%2==0);
	printf("test2:%d\n",n&1^1);
}
void TASK_4()
{
	int n;
	scanf("%d",&n);
	while(n)
	{
		for(int i=1;i<=n;i++)	printf("#");
		printf("\n");
		n--;
	}
}
void TASK_5()
{
	int n,f=1;
	scanf("%d",&n);
	for(int i=2;i<n;i++)
	{
		if(n%i==0)
		{
			printf("%d ",i);
			f=0;
		}
	}
	if(f)
		printf("PRIM\n");
	else
		printf("\n");
}
int main()
{
	//TASK_1();
	//TASK_4();
	TASK_5();
}
