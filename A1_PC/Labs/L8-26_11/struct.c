#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUME 100
#define MAX_GRUPA 7

typedef struct 
{
	char nume[MAX_NUME];
	char grupa[MAX_GRUPA];
	float medie;
} student;

void print_student(student s)
{
	printf("nume: %s\ngrupa: %s\nmedie: %2.2f\n\n",s.nume,s.grupa,s.medie);
}

int main()
{
	student s1= {"Ninel","314CAm",4.49};
	print_student(s1);

	student s2;
	strcpy(s2.nume,"Nicu");
	strcpy(s2.grupa,"ing");
	s2.medie=99.99;
	print_student(s2);
	
	student *s3=(student *)malloc(sizeof(student));
	strcpy(s3->nume,"Gicu");
	strcpy(s3->grupa,"pica");
	s3->medie=0.00002;
	print_student(*s3);
	return 0;
}


