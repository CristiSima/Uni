#define SET_SIZE 100
typedef unsigned char SET[SET_SIZE];

void insert_in_set(SET s,unsigned int n);
void delete_from_set(SET s, unsigned int n);
int is_in_set(SET s, unsigned int n);
void delete_all_from_set(SET s);
int card_set(SET s);
int is_empty_set(SET s);
void print_set(SET s);
