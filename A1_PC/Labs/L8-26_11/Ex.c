#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	char *nume;
	char grupa[6];
	double medie;
} student;

void print_student(student s)
{
	printf("%s\t\t%s\t\t%2.2f\n",s.nume,s.grupa,s.medie);
}
student *get_student()
{
	student *s=(student *)malloc(sizeof(student));
	s->nume=(char *)malloc(29);
	return s;
}
student *read_student()
{
	student *s=get_student();
	scanf(" %s %s %lf",s->nume,s->grupa,&(s->medie));
	return s;
}
void free_student(student *s)
{
	free(s->nume);
	free(s);
}
student *clone_student(student *og)
{
	student *s=get_student();

	strcpy(s->nume,og->nume);
	strcpy(s->grupa,og->grupa);
	s->medie=og->medie;
	return s;
}
int main()
{
	int n;
	scanf("%d",&n);
	student *temp;
	student **v=(student **)malloc(sizeof(student *));
	for(int i=0;i<n;i++)
		v[i]=read_student();

	for(int i=0;i<n;i++)
		for(int j=i+1;j<n;j++)
			if(strcmp(v[i]->nume,v[j]->nume)>0)
			{
				temp=v[i];
				v[i]=v[j];
				v[j]=temp;
			}
	printf("Nume\t\tGrupa\t\tMedie\n----------------------------------------\n");
	for(int i=0;i<n;i++)
		print_student(v[i][0]);
	printf("\n\n");

	student *new=clone_student(v[0]);
	v[0]->nume[0]='X';

	print_student(*v[0]);
	print_student(*new);
	
	for(int i=0;i<n;i++)
		free_student(v[i]);
	free(v);
	return 0;
}

