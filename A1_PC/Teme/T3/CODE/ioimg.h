#include "internalimg.h"

void img_print(image img);
void img_print_HEX(image img);

// wrapper pt citirea oricarui tip de imaginii
image img_read(char *file_name);

// obtinerea tipuilui imaginii
int img_get_type(char *file_name);
// obtinerea header-ului imaginii
image img_get_header(char *file_name,int *header_offset);

// citirea matricei specifica tipului
image img_read_P1(char *file_name);
image img_read_P2(char *file_name);
image img_read_P3(char *file_name);
image img_read_P4(char *file_name);
image img_read_P5(char *file_name);
image img_read_P6(char *file_name);

// wrapper pt salvarea oricarui tip de imaginii
void img_save(image img,char *file_name);
// salvarea header-ului imaginii
void img_save_header(image img,char *file_name);

// salvarea matricei specifica tipului
void img_save_P1(image img,char *file_name);
void img_save_P2(image img,char *file_name);
void img_save_P3(image img,char *file_name);
void img_save_P4(image img,char *file_name);
void img_save_P5(image img,char *file_name);
void img_save_P6(image img,char *file_name);
