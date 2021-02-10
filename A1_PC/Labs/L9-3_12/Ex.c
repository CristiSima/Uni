#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void E1a()
{
	int age;
	char name[31];
	scanf("%s %d",name,&age);
	FILE *out,*in;
	out=fopen("1-gen.txt","w");
	fprintf(out,"%s %d\n",name,age);
	fclose(out);

	in=fopen("1-gen.txt","r");
	fscanf(in,"%s %d",name,&age);
	printf("saved_name  = \"%s\"\nsaved_age   = %d\n",name,age);
	fclose(in);
}
void E1b()
{
	int age;
	char name[31];
	scanf("%s %d",name,&age);
	FILE *out,*in;
	out=fopen("1-gen.bin","wb");
	int len=strlen(name);
	fwrite(&len,sizeof(int),1,out);
	fwrite(name,1,len+1,out);
	fwrite(&age,sizeof(int),1,out);
	fclose(out);

	in=fopen("1-gen.bin","rb");
	fread(&len,sizeof(int),1,in);
	fread(name,1,len+1,in);
	fread(&age,sizeof(int),1,in);
	printf("saved_name  = \"%s\"\nsaved_age   = %d\n",name,age);
	fclose(in);
}
void E2()
{
	int n;
	int *v;
	scanf("%d",&n);
	v=(int *)malloc(sizeof(int)*n);
	for(int i=0;i<n;i++)
		scanf("%d",v+i);

	FILE *out;
	out=fopen("2-gen.txt","w");
	for(int i=0;i<n;i++)
		fprintf(out,"%d ",v[i]);
	fclose(out);

	out=fopen("2-gen.bin","wb");
	for(int i=0;i<n;i++)
		fwrite(v+i,sizeof(int),1,out);
	fclose(out);
}
void E3a()
{
	FILE *in;
	fpos_t cpos;
	in=fopen("2-gen.txt","r");
	fgetpos(in,&cpos);
	int nr;
	int n=0;
	while(fscanf(in,"%d",&nr)==1)
		n++;
	int *v=(int *)malloc(sizeof(int)*n);
	printf("n = %d\n",n);
	fsetpos(in,&cpos);
	for(int i=0;i<n;i++)
	{
		fscanf(in,"%d",v+i);
		printf("%d ",v[i]);
	}
	fclose(in);
	printf("\n");
}
void E3b()
{
	FILE *in;
	fpos_t cpos;
	in=fopen("2-gen.bin","rb");
	fgetpos(in,&cpos);
	int nr;
	int n=0;
	while(fread(&nr,sizeof(int),1,in))
		n++;
	int *v=(int *)malloc(sizeof(int)*n);
	printf("n = %d\n",n);
	fsetpos(in,&cpos);
	for(int i=0;i<n;i++)
	{
		fread(v+i,sizeof(int),1,in);
		printf("%d ",v[i]);
	}
	fclose(in);
	printf("\n");
}
int main()
{
	E3b();
}
