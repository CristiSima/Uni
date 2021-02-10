// #pragma once
#include <person.h>
#include <stdlib.h>
#include <string.h>

person_t *person_read_std()
{
	person_t *person=(person_t *)malloc(sizeof(person_t));
	char temp[100];
	scanf("%s %d",temp,&person->age);
	person->name=(char *)malloc(sizeof(char)*(strlen(temp)+1));
	strcpy(person->name,temp);
	return person;
}
void person_write_file(FILE *out,person_t *person)
{
	int len=strlen(person->name)+1;
	fwrite(&len,1,sizeof(int),out);
	fwrite(person->name,len,sizeof(char),out);
	fwrite(&person->age,1,sizeof(int),out);
}

void person_write_std(person_t *person)
{
	printf("%s %d\n",person->name,person->age);
}

person_t *person_read_file(FILE *in)
{
	int len;
	fread(&len,1,sizeof(int),in);
	person_t *person=(person_t *)malloc(sizeof(person_t));
	person->name=(char *) malloc(sizeof(char)*len);
	fread(person->name,sizeof(char),len,in);
	fread(&person->age,sizeof(int),1,in);
	return person;
}

void person_free(person_t *person)
{
	free(person->name);
	person->age=0;
}
