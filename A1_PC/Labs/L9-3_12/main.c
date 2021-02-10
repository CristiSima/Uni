#include <stdio.h>
#include <stdlib.h>


int main()
{
		FILE *in;
		in=fopen("a","r");
		if(!in)
		{
			fprintf(stderr,"file not found\n");
			return -1;
		}
		char s[10];
		// printf("Read:%ld\n",fread(s,sizeof(char),1,in));
		// fgets(s,10,in);
		fscanf(in,"%s",s);
		printf("%s->",s);
		int n,p,l;
		fscanf(in,"%d",&n);
		fpos_t cpos;
		for(int i=0;i<n;i++)
		{
			fscanf(in,"%d %d",&p,&l);
			fgetpos(in,&cpos);
			printf("\n%d",cpos);
		}
		cpos=0;
		fsetpos(in,&cpos);
		fscanf(in,"%s",s);
		printf("\n%s->",s);
		printf("\n");
		return 0;
}
