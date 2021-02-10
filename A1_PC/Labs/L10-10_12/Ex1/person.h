#define NAME_LEN 31
// #pragma once
#include <stdio.h>

typedef struct __attribute__((__packed__))
{
	char name[NAME_LEN];
	int age;
} person_t;

person_t *person_read_std();

void person_write_std(person_t *person);


void person_write_file(FILE *out,person_t *person);

person_t *person_read_file(FILE *in);
