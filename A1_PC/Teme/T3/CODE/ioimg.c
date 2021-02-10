#include "ioimg.h"
#include <stdio.h>
#include <ctype.h>

void img_print(image img)
{
	printf("P%d\n%d %d\n%d\n",img.P,img.n,img.m,img.max);
	for(int i=0;i<img.n;i++)
	{
		for(int j=0;j<img.m;j++)
			printf("%d %d %d  ",img.M[i][j].R,img.M[i][j].G,img.M[i][j].B);
		printf("\n");
	}
}

void img_print_HEX(image img)
{
	printf("P%d\n%d %d\n%d\n",img.P,img.n,img.m,img.max);
	if(img.P==-1)	return;
	for(int i=0;i<img.n;i++)
	{
		for(int j=0;j<img.m;j++)
			printf("%2X %2X %2X  ",img.M[i][j].R,img.M[i][j].G,img.M[i][j].B);
		printf("\n");
	}
}

image img_read(char *file_name)
{
	int P=img_get_type(file_name);
	image img_err={0x0,0,0,-1,0};
	switch (P)
	{
		case 1:
			return img_read_P1(file_name);
		case 2:
			return img_read_P2(file_name);
		case 3:
			return img_read_P3(file_name);
		case 4:
			return img_read_P4(file_name);
		case 5:
			return img_read_P5(file_name);
		case 6:
			return img_read_P6(file_name);
		default:
			return img_err;
	}
	return img_err;
}

int img_get_type(char *file_name)
{
	FILE *IN=fopen(file_name,"r");
	if(!IN)
		return -1;
	char a,b;
	fscanf(IN,"%c%c",&a,&b);
	fclose(IN);
	return b-'0';
}

image img_get_header(char *file_name,int *header_offset)
{
	image img;
	img.P=img_get_type(file_name);
	int tmp;
	FILE *IN=fopen(file_name,"r");
	char S[101];

	fgets(S,101,IN);
	fgets(S,101,IN);
	while (S[0]=='#')
		fgets(S,101,IN);
	// citire m n pentru ca datele sunt date latime inaltime
	if(sscanf(S,"%d %d",&img.m,&img.n)<2)
		return img;
	if(img.P!=1 && img.P!=4)
	{
		fgets(S,101,IN);
		while (S[0]=='#')
			fgets(S,101,IN);
		if(sscanf(S,"%d",&tmp)<1)
			return img;
		img.max=tmp;
	}

	*header_offset=ftell(IN);
	fclose(IN);
	return img;
}

image img_read_P1(char *file_name)
{
	int header_offset;
	image img=img_gen_M(img_get_header(file_name,&header_offset));
	FILE *IN=fopen(file_name,"r");
	fseek(IN,header_offset,SEEK_SET);
	char S[101];
	int nr=0;
	int i=0,j=0;
	char c;
	// citirea matricei ce permite comentari in matrice
	while(i!=img.n)
	{
		if(j==img.m)
		{
			i+=1;
			j=0;
		}
		if(i==img.n)
			break;

		c=fgetc(IN);
		// salvare numar
		if(isdigit(c))
		{
			nr=(c-'0');
			nr=nr==0;
			img.M[i][j].R=nr;
			img.M[i][j].G=nr;
			img.M[i][j].B=nr;
			j++;
		}
		// eliminarea comentariilor
		if(c=='#')
			fgets(S,101,IN);
	}
	img.max=1;
	fclose(IN);
	return img;
}

image img_read_P2(char *file_name)
{
	int header_offset;
	image img=img_gen_M(img_get_header(file_name,&header_offset));
	FILE *IN=fopen(file_name,"r");
	fseek(IN,header_offset,SEEK_SET);
	char S[101];
	int nr=0,digits=0;
	int i=0,j=0;
	char c;
	// citirea matricei ce permite comentari in matrice
	while(i!=img.n)
	{
		if(j==img.m)
		{
			i+=1;
			j=0;
		}
		if(i==img.n)
			break;
		c=fgetc(IN);
		// asamblare numar
		if(isdigit(c))
		{
			nr=nr*10+(c-'0');
			digits++;
		}
		else
		{
			// salvarea numar
			if(digits)
			{
				nr=min(nr,img.max);
				img.M[i][j].R=nr;
				img.M[i][j].G=nr;
				img.M[i][j].B=nr;
				j++;
				digits=0;nr=0;
			}
			// eliminarea comentariilor
			if(c=='#')
				fgets(S,101,IN);
		}
	}
	fclose(IN);
	return img;
}

image img_read_P3(char *file_name)
{
	int header_offset;
	image img=img_gen_M(img_get_header(file_name,&header_offset));
	FILE *IN=fopen(file_name,"r");
	fseek(IN,header_offset,SEEK_SET);
	char S[101];
	int nr=0,digits=0;
	int i=0,j=0,k=0;
	char c;
	// citirea matricei ce permite comentari in matrice
	while(i!=img.n)
	{
		if(j==img.m)
		{
			i+=1;
			j=0;
		}
		if(i==img.n)
			break;

		c=fgetc(IN);
		// asamblare numar
		if(isdigit(c))
		{
			nr=nr*10+(c-'0');
			digits++;
		}
		else
		{
			// salvare numar
			if(digits)
			{
				switch (k) {
					case 0:
						img.M[i][j].R=min(nr,img.max);
						break;
					case 1:
						img.M[i][j].G=min(nr,img.max);
						break;
					case 2:
						img.M[i][j].B=min(nr,img.max);
						break;
				}
				k=(k+1)%3;
				j+=k==0;
				digits=0;nr=0;
			}
			// eliminare comentarii
			if(c=='#')
				fgets(S,101,IN);
		}
	}
	fclose(IN);
	return img;
}

image img_read_P4(char *file_name)
{
	int header_offset;
	image img=img_gen_M(img_get_header(file_name,&header_offset));
	FILE *IN=fopen(file_name,"rb");
	fseek(IN,header_offset,SEEK_SET);
	BYTE temp;
	for(int i=0;i<img.n;i++)
		for(int j=0;j<img.m;j+=8)
		{
			fread(&temp,1,1,IN);
			// impartirea valorilor din byte in pixeli
			for(int k=7;k>=0;k--)
			{
				if(j+k<img.m)
				{
					img.M[i][j+k].G=(temp&1)==0;
					img.M[i][j+k].B=(temp&1)==0;
					img.M[i][j+k].R=(temp&1)==0;
				}
				temp>>=1;
			}
		}
	img.max=1;
	fclose(IN);
	return img;
}

image img_read_P5(char *file_name)
{
	int header_offset;
	image img=img_gen_M(img_get_header(file_name,&header_offset));
	FILE *IN=fopen(file_name,"rb");
	fseek(IN,header_offset,SEEK_SET);
	BYTE c;
	for(int i=0;i<img.n;i++)
		for(int j=0;j<img.m;j++)
		{
			fread(&c,1,1,IN);
			c=min(c,img.max);
			img.M[i][j].G=c;
			img.M[i][j].B=c;
			img.M[i][j].R=min(c,img.max);
		}
	fclose(IN);
	return img;
}

image img_read_P6(char *file_name)
{
	int header_offset;
	image img=img_gen_M(img_get_header(file_name,&header_offset));
	FILE *IN=fopen(file_name,"rb");
	fseek(IN,header_offset,SEEK_SET);
	BYTE c;
	for(int i=0;i<img.n;i++)
		for(int j=0;j<img.m;j++)
		{
			fread(&c,1,1,IN);
			img.M[i][j].R=min(c,img.max);

			fread(&c,1,1,IN);
			img.M[i][j].G=min(c,img.max);

			fread(&c,1,1,IN);
			img.M[i][j].B=min(c,img.max);
		}
	fclose(IN);
	return img;
}

void img_save(image img,char *file_name)
{
	img_save_header(img,file_name);
	switch (img.P)
	{
		case 1:
			img_save_P1(img,file_name);
			break;
		case 2:
			img_save_P2(img,file_name);
			break;
		case 3:
			img_save_P3(img,file_name);
			break;
		case 4:
			img_save_P4(img,file_name);
			break;
		case 5:
			img_save_P5(img,file_name);
			break;
		case 6:
			img_save_P6(img,file_name);
			break;
	}
}

void img_save_header(image img,char *file_name)
{
	FILE *OUT=fopen(file_name,"w");
	fprintf(OUT,"P%d\n%d %d\n",img.P,img.m,img.n);
	if(img.P!=1 && img.P!=4)
		fprintf(OUT,"%d\n",img.max);
	fclose(OUT);
}

void img_save_P1(image img,char *file_name)
{
	FILE *OUT=fopen(file_name,"a");
	for(int i=0;i<img.n;i++)
		for(int j=0;j<img.m;j++)
			fprintf(OUT,"%d",img.M[i][j].R==0);
	fprintf(OUT,"\n");
	fclose(OUT);
}

void img_save_P2(image img,char *file_name)
{
	FILE *OUT=fopen(file_name,"a");
	for(int i=0;i<img.n;i++)
	{
		for(int j=0;j<img.m;j++)
			fprintf(OUT,"%d ",img.M[i][j].R);
		fprintf(OUT,"\n");
	}
	fclose(OUT);
}

void img_save_P3(image img,char *file_name)
{
	FILE *OUT=fopen(file_name,"a");
	for(int i=0;i<img.n;i++)
	{
		for(int j=0;j<img.m;j++)
			fprintf(OUT,"%d %d %d ",img.M[i][j].R,img.M[i][j].G,img.M[i][j].B);
		fprintf(OUT,"\n");
	}
	fclose(OUT);
}

void img_save_P4(image img,char *file_name)
{
	BYTE temp;
	FILE *OUT=fopen(file_name,"ab");
	for(int i=0;i<img.n;i++)
		for(int j=0;j<img.m;j+=8)
		{
			temp=0;
			// asamblare pixeli in byte
			for(int k=0;k<8;k++)
			{
				temp<<=1;
				if(j+k<img.m)
					temp|=img.M[i][j+k].R==0;
			}
			fwrite(&temp,1,1,OUT);
		}
	fclose(OUT);
}

void img_save_P5(image img,char *file_name)
{
	FILE *OUT=fopen(file_name,"ab");
	for(int i=0;i<img.n;i++)
		for(int j=0;j<img.m;j++)
			fwrite(&img.M[i][j].R,1,1,OUT);
	fclose(OUT);
}

void img_save_P6(image img,char *file_name)
{
	FILE *OUT=fopen(file_name,"ab");
	for(int i=0;i<img.n;i++)
		for(int j=0;j<img.m;j++)
		{
			fwrite(&img.M[i][j].R,1,1,OUT);
			fwrite(&img.M[i][j].G,1,1,OUT);
			fwrite(&img.M[i][j].B,1,1,OUT);
		}
	fclose(OUT);
}
