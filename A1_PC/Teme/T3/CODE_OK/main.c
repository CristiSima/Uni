#include "ioimg.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
	char S[101];
	image img;
	selection sel;
	img.P=sel.P=-1;
	while(1)
	{
		scanf("%s",S);
		if(!strcmp(S,"EXIT"))
		{
			img_free(img);
			return 0;
		}
		else if(!strcmp(S,"LOAD"))
		{
			// scanf("%s",S);
			fgets(S,101,stdin);
			strchr(S,'\n')[0]=0;
			// printf("|%s|\n",S);
			img_free(img);
			sel_free(sel);
			sel.P=-1;
			img=img_read(S+1);
			if(img.P!=-1)
			{
				sel=img_select(img,0,0,img.n-1,img.m-1);
				printf("Loaded %s\n",S+1);
			}
			else
				printf("Faild to load %s\n",S+1);
		}
		else if(!strcmp(S,"SELECT"))
		{
			if(img.P==-1)
			{
				fgets(S,101,stdin);
				printf("No image loaded\n");
				continue;
			}
			fgets(S,101,stdin);
			if(!strcmp(S+1,"ALL\n"))
			{
				sel_free(sel);
				sel=img_select(img,0,0,img.n-1,img.m-1);
			}
			else
			{
				int x1,x2,y1,y2;
				sscanf(S,"%d %d %d %d",&x1,&y1,&x2,&y2);
				if(x1<0 || x2<0 || y1<0 || y2<0 || x1<=img.n || x2<=img.n || y1<=img.m || y2<=img.m)
				{
					sel_free(sel);
					sel=img_select(img,x1,y1,x2,y2);
					printf("Selected %d %d %d %d\n",x1,y1,x2,y2);
				}
				else
					printf("Incalid set of coordinates\n");
			}
		}
		else if(!strcmp(S,"PRINT"))
		{
			img_print_HEX(img);
		}
		else if(!strcmp(S,"ROTATE"))
		{
			int deg;
			scanf("%d",&deg);
			if(sel.m!=sel.n)
			{
				printf("The selectioniom must be square\n");
				continue;
			}
			if(deg%90!=0)
			{
				printf("Unsupported rotation angle\n");
				continue;
			}
			printf("Rotated %d\n",deg);
			deg/=90;
			if(deg<0)	deg+=4;
			for(int i=0;i<deg;i++)
				sel_rotate(sel);
		}
		else if(!strcmp(S,"CROP"))
		{
			if(img.P==-1)
			{
				fgets(S,101,stdin);
				printf("No image loaded\n");
				continue;
			}
			printf("Image cropped");
			image crop=img_copy(sel);
			img_free(img);
			sel_free(sel);
			img=crop;
			sel=img_select(img,0,0,img.n-1,img.m-1);
		}
		else if(!strcmp(S,"GRAYSCALE"))
		{
			if(img.P==-1 || img.P==1 || img.P==4)
				printf("Grayscale filter not available\n");
			else
			{
				printf("Grayscale filter applied\n");
				sel_grayscale(sel);
			}
		}
		else if(!strcmp(S,"SEPIA"))
		{
			if(img.P==-1 || img.P==1 || img.P==4)
				printf("Sepia filter not available\n");
			else
			{
				printf("Sepia filter applied\n");
				sel_sepia(sel);
			}
		}
		else if(!strcmp(S,"SAVE"))
		{
			char nume[100];
			scanf("%s",nume);
			fgets(S,101,stdin);
			strchr(S,'\n')[0]=0;
			if(img.P==-1)
			{
				fgets(S,101,stdin);
				printf("No image loaded\n");
				continue;
			}
			image temp=img;
			temp.P=(temp.P-1)%3+1;
			if(strcmp(S+1,"ascii")!=0)
				temp.P+=3;
			img_save(temp,nume);
			printf("Saved %s\n",nume);
		}
		else
		{
			fgets(S,101,stdin);
			printf("Invalid command\n");
		}
	}
}
