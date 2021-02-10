#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void printf_injection()
{
	char s[11];
	int N=123;
	scanf("%s",s);
	printf(s);
	printf("\n");
}
int is_palindrome_a(char *p)
{
	char *q=p;
	while(q[1]!=0)	q++;
	while(p<q)
	{

		if(tolower(p[0])!=tolower(q[0]))
			return 0;
		p++;q--;
	}
	return 1;
}
int is_palindrome_b(char *p)
{
	char *q=p;
	while(q[1]!=0)	q++;
	while(p<q)
	{

		if(tolower(p[0])!=tolower(q[0]))
			return 0;
		p++;q--;
	}
	return 1;
}
int is_palindrome_c(char *p)
{
	char *q=p;
	while(q[1]!=0)	q++;
	while(p<q)
	{

		if(tolower(p[0])!=tolower(q[0]))
			return 0;
		p++;while(p[0]==' ')	p++;
		q--;while(q[0]==' ')	q--;
	}
	return 1;
}
void E1()
{
	char s[101];
	//scanf("%s",s);
	fgets(s,100,stdin);
	s[strlen(s)-1]=0;
	//printf("%d\n",strlen(s));
	printf("\"%s\".palindrom:%d\n",s,is_palindrome_c(s));
}
void sort_strings(int n,char **v)
{
	char temp[51];
	char *p;
	for(int i=0;i<n;i++)
		for(int j=i+1;j<n;j++)
			if(strlen(v[i])>strlen(v[j]) || (strlen(v[i])==strlen(v[j]) && strcmp(v[i],v[j])>0))
			{
				p=v[i];
				v[i]=v[j];
				v[j]=p;
			}
}
void print_strings(int n,char **v)
{
	for(int i=0;i<n;i++)
		printf("%s\n",v[i]);
}
void E2()
{
	int n;
	char **v;
	scanf("%d\n",&n);
	v=(char **)malloc(sizeof(char *)*n);
	for(int i=0;i<n;i++)
	{
		v[i]=(char *)malloc(sizeof(char)*51);
		fgets(v[i],50,stdin);
		v[i][strlen(v[i])-1]=0;
	}
	sort_strings(n,v);
	print_strings(n,v);
	for(int i=0;i<n;i++)
		free(v[i]);
		//free(*(v+i));
	free(v);
}
int is_word(char *p)
{
	for(;p[0]!=0;p++)
		if(!isalpha(p[0]))
			return 0;
	return 1;
}
void E3()
{
	char s[101];

	fgets(s,100,stdin);
	s[strlen(s)-1]=0;
	char *p;
	p=strtok(s," .,?!\n\t");
	int words=0;
	while(p!=NULL)
	{
		printf("%s\n",p);
		words+=is_word(p);
		p=strtok(NULL," .,?!\n\t");
	}
	printf("words:%d\n",words);
}
int count_occurrences(char word[],char phrase[])
{
	int oc=0;
	int lim=strlen(phrase)-strlen(word)+1;
	int lim2=strlen(word);
	printf("%d\n",lim);
	for(int i=0;i<lim;i++)
		for(int j=0;j<lim2;j++)
			if(word[j]!=phrase[i+j])
				break;
			else if(j==lim2-1)
				oc++;
	return oc;
}
void E4()
{
	printf("%d\n",count_occurrences("Gigel","Gigel is Gigel all Gige when isn't Gigel,..Gigel"));
}
int main()
{
	E4();
}
