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

// diferentierea intre selectii si imaginii se face prin tipul de data
typedef image selection;

BYTE min(int a,BYTE b);

// alocarea matricei folosind header-ul imaginii
image img_gen_M(image img);
image img_copy(image img);
// generarea unei imaginii rotite in sensul acelor
image img_rotate(image img);

// crearea selelectiei
selection img_select(image img,int x1,int y1,int x2,int y2);

// rotirea selectiei in sensul acelor
void sel_rotate(selection sel);

// aplicare filtrelor pe selectii
void sel_grayscale(selection sel);
void sel_sepia(selection sel);

// dealocarae memoriei
void img_free(image img);
void sel_free(selection sel);
