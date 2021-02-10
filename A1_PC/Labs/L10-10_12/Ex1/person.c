// #pragma once
#include <person.h>
#include <stdlib.h>

person_t *person_read_std()
{
	person_t *person=(person_t *)malloc(sizeof(person_t));
	scanf("%s %d",person->name,&person->age);
	return person;
}
void person_write_file(FILE *out,person_t *person)
{
	fwrite(person,1,sizeof(person_t),out);
}

void person_write_std(person_t *person)
{
	printf("%s %d\n",person->name,person->age);
}

person_t *person_read_file(FILE *in)
{
	person_t *person=(person_t *)malloc(sizeof(person_t));
	fread(person,sizeof(person_t),1,in);
	return person;
}
