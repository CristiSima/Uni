#include <stdio.h>
#include <math.h>
#include <time.h>

int prim(int n)
{
	if(n%2==0)	
		if(n==2)
			return 1;
		else
			return 0;
	for(int i=3;i*i<n;i+=2)
		if(n%i==0)
			return 0;
	return 1;
}
int main()
{
	int n;
	printf("Size of int:%d\n",sizeof(int));
	clock_t t_start,t_stop,t_sum;
	time_t t,t_tot;
	t=time(0);
	printf("ON\n");
	//scanf("%d",&n);
	//printf("%d\n",prim(n));
	int a1,a2;
	printf("a1=");
	t_start=clock();
	scanf("%d",&a1);
	t_stop=clock();
	t_sum=t_stop-t_start;
	printf("a2=");
	t_start=clock();
	scanf("%d",&a2);
	t_stop=clock();
	t_sum+=t_stop-t_start;
	t_tot=time(0);
	t_tot-=t;
	printf("a1+a2=%d\na1-a2=%d\ndelta=%d\n",a1+a2,a1-a2,(int)abs(a1-a2));
	/// IDK ar trebui ca t_tot sa aiba timpul d eexecutie( cu tot cu asteptarea dupa user input)
	printf("I've wasted %d clock with you\nI vould/ve done that in %f secconds\n",t_sum,t_tot);
	return 0;
}
