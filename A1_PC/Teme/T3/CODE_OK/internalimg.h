typedef unsigned char BYTE;
typedef struct
{
	BYTE R,G,B;
} pixel;

typedef struct
{
	pixel **M;
	int n,m;
	int P;
	BYTE max;
} image;
typedef image selection;

BYTE min(BYTE a,BYTE b);
image img_gen_M(image img);
image img_copy(image img);
image img_rotate(image img);
image img_select(image img,int x1,int y1,int x2,int y2);
void sel_rotate(selection sel);
void sel_grayscale(selection sel);
void sel_sepia(selection sel);

void img_free(image img);
void sel_free(selection sel);
