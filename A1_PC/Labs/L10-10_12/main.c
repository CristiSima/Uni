#include <stdio.h>
#include <stdlib.h>
#include <person.h>

void Ex2()
{
	FILE *out=fopen("2-gigel.bin","wb");
	person_t *person=person_read_std();
	person_write_file(out,person);

	fclose(out);
	person_free(person);
	free(person);

	FILE *in=fopen("2-gigel.bin","rb");
	person=person_read_file(in);
	person_write_std(person);

	fclose(in);
	person_free(person);
	free(person);
}

void Ex3()
{
	int n;
	scanf("%d",&n);
	FILE *out=fopen("3-gigel.bin","wb");
	person_t *person;
	fwrite(&n,1,sizeof(int),out);
	for(;n;n--)
	{
		person=person_read_std();
		person_write_file(out,person);
		person_free(person);
		free(person);
	}
	fclose(out);

	FILE *in=fopen("3-gigel.bin","rb");
	fread(&n,1,sizeof(int),in);
	for(;n;n--)
	{
		person=person_read_file(in);
		person_write_std(person);
		person_free(person);
		free(person);
	}
	fclose(in);
}
int main()
{
	// Ex2();
	Ex3();
	return 0;

}
