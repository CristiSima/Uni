#include <stdio.h>
#include <stdlib.h>
#include <person.h>

int main()
{
	person_t *person=person_read_std();
	FILE *out=fopen("1-gigel.bin","wb");
	person_write_file(out,person);
	fclose(out);
	free(person);
	FILE *in=fopen("1-gigel.bin","rb");
	person=person_read_file(in);
	person_write_std(person);
	free(person);
	fclose(in);

	return 0;

}
