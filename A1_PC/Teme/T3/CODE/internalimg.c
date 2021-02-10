#include "internalimg.h"
#include <stdlib.h>
#include <math.h>
void img_free(image img)
{
	if(img.P==-1)
		return;
	for(int i=0;i<img.n;i++)
		free(img.M[i]);
	free(img.M);
}

image img_gen_M(image img)
{
	img.M=(pixel**)malloc(sizeof(pixel*)*(img.n+1));
	for(int i=0;i<img.n;i++)
		img.M[i]=malloc(sizeof(pixel)*(img.m+1));
	return img;
}

BYTE min(int a,BYTE b)
{
	if(a<b)
		return a;
	return b;
}

image img_copy(image img)
{
	image copy=img_gen_M(img);
	for(int i=0;i<img.n;i++)
		for(int j=0;j<img.m;j++)
			copy.M[i][j]=img.M[i][j];
	return copy;
}

image img_rotate(image img)
{
	image rot=img;
	rot.n=img.m;
	rot.m=img.n;
	rot=img_gen_M(rot);
	for(int i=0;i<img.n;i++)
		for(int j=0;j<img.m;j++)
			rot.M[j][img.n-i-1]=img.M[i][j];
	return rot;
}

void sel_rotate(selection sel)
{
	image temp=img_gen_M(sel);
	for(int i=0;i<sel.n;i++)
		for(int j=0;j<sel.m;j++)
			temp.M[j][sel.n-i-1]=sel.M[i][j];
	for(int i=0;i<sel.n;i++)
		for(int j=0;j<sel.m;j++)
			sel.M[i][j]=temp.M[i][j];
	img_free(temp);
}

selection img_select(image img,int x1,int y1,int x2,int y2)
{
	selection sel=img;
	sel.n=x2-x1;
	sel.m=y2-y1;
	sel.M=(pixel**)malloc(sizeof(pixel*)*(sel.n));
	// utilizarea sectiunilor alocate pentru matricea originala
	for(int i=0;i<sel.n;i++)
		sel.M[i]=img.M[i+x1]+y1;
	return sel;
}

void sel_free(selection sel)
{
	if(sel.P!=-1)
		free(sel.M);
}

// aplicare filtrului grayscale pe un pixel
pixel pixel_grayscale(pixel px)
{
	double S=px.R+px.G+px.B;
	S/=3;
	S=round(S);
	px.R=S;
	px.G=S;
	px.B=S;
	return px;
}

void sel_grayscale(selection sel)
{
	for(int i=0;i<sel.n;i++)
		for(int j=0;j<sel.m;j++)
			sel.M[i][j]=pixel_grayscale(sel.M[i][j]);
}

// aplicare filtrului sepia pe un pixel
pixel pixel_sepia(pixel px,int max)
{
	double newR = 0.393*px.R + 0.769*px.G + 0.189*px.B;
	double newG = 0.349*px.R + 0.686*px.G + 0.168*px.B;
	double newB = 0.272*px.R + 0.534*px.G + 0.131*px.B;
	px.R=min(round(newR),max);
	px.G=min(round(newG),max);
	px.B=min(round(newB),max);
	return px;
}

void sel_sepia(selection sel)
{
	for(int i=0;i<sel.n;i++)
		for(int j=0;j<sel.m;j++)
			sel.M[i][j]=pixel_sepia(sel.M[i][j],sel.max);
}
