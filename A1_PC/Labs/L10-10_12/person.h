#include <stdio.h>

typedef struct
{
	char *name;
	int age;
} person_t;


person_t *person_read_std();

void person_write_std(person_t *person);

void person_write_file(FILE *out,person_t *person);

person_t *person_read_file(FILE *in);

void person_free(person_t *person);
