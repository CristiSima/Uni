#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int bprintf(FILE *file,char *format,...)
{
	char *buffer=malloc(strlen(format)+1);
	memcpy(buffer,format,strlen(format));
	va_list va;
	va_start(va,format);
	char *part=strtok(buffer,"%");
	int temp_int;
	char temp_char;
	char *temp_charp;
	while (part) {
		switch(part[0])
		{
			case 'd':
				temp_int=va_arg(va,int);
				fprintf(file,"%d",temp_int);
				// fwrite(&temp_int,)
				break;
			case 'c':
				temp_char=(char)va_arg(va,int);
				// fprintf(file,"%c",temp_char);
				fwrite(&temp_char,1,1,file);
				break;
			case 's':
				temp_charp=va_arg(va,char*);
				// fprintf(file,"%s",temp_charp);
				fwrite(temp_charp,strlen(temp_charp),1,file);
				break;
		}
		part++;
		// fprintf(file, "%s", part);
		fwrite(part,strlen(part),1,file);
		part=strtok(0,"%");
	}
	free(buffer);
	return 1;
}

int main(void)
{
	bprintf(stdout,"%d c:%c %s\n",1,'A',"AAAA");
}
