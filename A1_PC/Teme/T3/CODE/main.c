#include "ioimg.h"
#include <stdio.h>
#include <string.h>
void SAVE(image img)
{
	char S[101];
	char nume[100];
	scanf("%s",nume);
	fgets(S,101,stdin);
	strchr(S,'\n')[0]=0;
	// reducere la versiunea text
	img.P=(img.P-1)%3+1;
	// aducerea la versiunea binara
	if(strcmp(S+1,"ascii")!=0)
		img.P+=3;
	img_save(img,nume);
	printf("Saved %s\n",nume);
}

void SEPIA(selection sel)
{
	if(sel.P==3 || sel.P==6)
	{
		printf("Sepia filter applied\n");
		sel_sepia(sel);
	}
	else
	{
		printf("Sepia filter not available\n");
	}
}

void GRAYSCALE(selection sel)
{
	if(sel.P==3 || sel.P==6)
	{
		printf("Grayscale filter applied\n");
		sel_grayscale(sel);
	}
	else
	{
		printf("Grayscale filter not available\n");
	}
}

void ROTATE(image *img,selection *sel)
{
	int deg;
	scanf("%d",&deg);
	if(deg%90!=0)
	{
		printf("Unsupported rotation angle\n");
		return;
	}
	// rotarea intregi imaginii
	if(sel->m==img->m && sel->n==img->n)
	{
		printf("Rotated %d\n",deg);
		deg/=90;
		if(deg<0)	deg+=4;
		image rot;
		sel_free(*sel);
		for(int i=0;i<deg;i++)
		{
			rot=img_rotate(*img);
			img_free(*img);
			*img=rot;
		}
		*sel=img_select(*img,0,0,img->n,img->m);
		return;
	}
	if(sel->m!=sel->n)
	{
		printf("The selection must be square\n");
		return;
	}
	int old_deg=deg;
	deg/=90;
	if(deg<0)	deg+=4;
	if(deg<0 || deg >4)
	{
		printf("Unsupported rotation angle\n");
		return;
	}
	printf("Rotated %d\n",old_deg);
	for(int i=0;i<deg;i++)
		sel_rotate(*sel);
}

void arrange(int *a,int *b)
{
	if(*a>*b)
	{
		int temp=*a;
		*a=*b;
		*b=temp;
	}
}

int main(void)
{
	char S[101];
	image img;
	selection sel;
	img.P=-1;sel.P=-1;
	while(1)
	{
		scanf("%s",S);
		if(!strcmp(S,"LOAD"))
		{
			fgets(S,101,stdin);
			strchr(S,'\n')[0]=0;
			img_free(img);sel_free(sel);
			sel.P=-1;
			img=img_read(S+1);
			if(img.P!=-1)
			{
				sel=img_select(img,0,0,img.n,img.m);
				printf("Loaded %s\n",S+1);
			}
			else
			{
				printf("Failed to load %s\n",S+1);
			}
		}
		else if(!strcmp(S,"EXIT"))
		{
			if(img.P!=-1)
			{
				img_free(img);sel_free(sel);
			}
			else
			{
				printf("No image loaded\n");
			}
			return 0;
		}
		else if(img.P==-1)
		{
			fgets(S,101,stdin);
			printf("No image loaded\n");
		}
		else if(!strstr("EXIT LOAD SELECT ROTATE CROP GRAYSCALE SEPIASAVE",S))
		{
			fgets(S,101,stdin);
			printf("Invalid command\n");
		}
		else if(!strcmp(S,"SELECT"))
		{
			fgets(S,101,stdin);
			if(S[1]=='A' && S[2]=='L' && S[3]=='L' && (S[4]==' ' || S[4]=='\n'))
			{
				sel_free(sel);
				printf("Selected ALL\n");
				sel=img_select(img,0,0,img.n,img.m);
			}
			else
			{
				int x1,x2,y1,y2;
				// citire y x pentru ca datele sunt date latime inaltime
				if(sscanf(S,"%d %d %d %d",&y1,&x1,&y2,&x2)<4)
				{
					printf("Invalid command\n");
				}
				else if(x1<0 || x2<0 || y1<0 || y2<0 || img.n<x1 || img.n<x2 || img.m<y1 || img.m<y2 || x1==x2 || y1==y2)
				{
					printf("Invalid set of coordinates\n");
				}
				else
				{
					sel_free(sel);
					arrange(&x1,&x2);arrange(&y1,&y2);
					sel=img_select(img,x1,y1,x2,y2);
					printf("Selected %d %d %d %d\n",y1,x1,y2,x2);
				}
			}
		}
		else if(!strcmp(S,"PRINT"))
		{
			img_print_HEX(img);
		}
		else if(!strcmp(S,"ROTATE"))
		{
			ROTATE(&img,&sel);
		}
		else if(!strcmp(S,"CROP"))
		{
			printf("Image cropped\n");
			image crop=img_copy(sel);
			img_free(img);sel_free(sel);
			img=crop;
			sel=img_select(img,0,0,img.n,img.m);
		}
		else if(!strcmp(S,"GRAYSCALE"))
		{
			GRAYSCALE(sel);
		}
		else if(!strcmp(S,"SEPIA"))
		{
			SEPIA(sel);
		}
		else if(!strcmp(S,"SAVE"))
		{
			SAVE(img);
		}
	}
}
