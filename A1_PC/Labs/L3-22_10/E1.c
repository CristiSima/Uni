#include <stdio.h>
int power(int b,int p)
{
	int R=1;
	while(p)
	{
		if(p&1)	R*=b;
		b*=b;
		p>>=1;
	}
	return R;
}
int count_digits(int n)
{
	int nr=0;
	while(n)	{nr++;n/=10;}
	return nr;
}
int reverse_number(int n)
{
	int nr=0;
	while(n)
	{
		nr*=10;
		nr+=n%10;
		n/=10;
	}
	return nr;
}
int reverse_number_R(int n)
{
	if(n<10)	return n;
	return (n%10)*power(10,count_digits(n)-1)+(reverse_number_R(n/10));
}
int is_palindrom(int n)
{
	return n==reverse_number(n);
}
int next_palindrome(int n)
{
	while(!is_palindrom(n))	n++;
	return n;
}
int is_prime(int n)
{
	if(n<=1) return 0;
	if(n<=3) return 1;
	if(n&1==0)	return 0;
	for(int i=5;i*i<=n;i++)
		if(n%i==0)
			return 0;
	return 1;
}
int next_prime(int n)
{
	if(n<=2)	return 2;
	if(n&1==0)	n++;
	while(!is_prime(n))	n+=2;
	return n;
}
int gcd_a(int a,int b)
{
	int c;
	while(b)
	{
		c=a%b;
		a=b;
		b=c;
	}
	return a;
}
int gcd_b(int a,int b)
{
	while(a!=b)
		if(a>b)	a-=b;
		else	b-=a;
	return a;
}
int gcd(int a,int b)
{
	return gcd_a(a,b);
}
int scm(int a,int b)
{
	int c=gcd(a,b);
	return a*b/c;
}
int main()
{
	printf("%d\n%d\n",power(2,7),power(10,2));
	printf("%d\n",count_digits(123));
	printf("%d\n",reverse_number_R(123));
	printf("  123.palindrim:%d\n 1221.palindrom:%d\n12321.palindrom:%d\n",is_palindrom(123),is_palindrom(1221),is_palindrom(12321));
	printf("123 nu e palindrom dar %d este\n",next_palindrome(123));
	printf("  -3.prime:%d\n 128.prime:%d\n 101.prime:%d\n%d.prime:%d\n",is_prime(-3),is_prime(128),is_prime(101),101*97,is_prime(101*97));
	printf("-100->%d\n   4->%d\n 999->%d\n",next_prime(-100),next_prime(4),next_prime(999));
	printf("cmmdc(12,18)=%d\ncmmdc(99,64)=%d\n",gcd(12,18),gcd(99,64));
	printf("cmmmc(12,18)=%d\n",scm(12,18));
}
