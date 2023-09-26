// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>

char *strcpy(char *destination, const char *source)
{
	/* TODO: Implement strcpy(). */
	size_t i;
	for(i=0;source[i];i++)
		destination[i]=source[i];
	destination[i]=0;

	return destination;
}

char *strncpy(char *destination, const char *source, size_t len)
{
	/* TODO: Implement strncpy(). */
	size_t i;
	for(i=0;source[i] && i<len;i++)
		destination[i]=source[i];
	if(i<len)
		destination[i]=0;

	return destination;
}

char *strcat(char *destination, const char *source)
{
	/* TODO: Implement strcat(). */
	size_t i;
	for(i=0;destination[i];i++);

	for(size_t j=0;source[j];i++, j++)
		destination[i]=source[j];

	destination[i]=0;

	return destination;
}

char *strncat(char *destination, const char *source, size_t len)
{
	/* TODO: Implement strncat(). */
	size_t i;
	for(i=0;destination[i];i++);

	size_t j=0;
	for(j=0;source[j] && j<len;i++, j++)
		destination[i]=source[j];

	destination[i]=0;

	return destination;
}

int strcmp(const char *str1, const char *str2)
{
	/* TODO: Implement strcmp(). */
	size_t l1=strlen(str1);
	size_t l2=strlen(str2);
	if(l1 != l2)
		return l1 < l2? -1 : 1;

	return memcmp(str1, str2, l1);
}

int strncmp(const char *str1, const char *str2, size_t len)
{
	/* TODO: Implement strncmp(). */
	size_t l1=strlen(str1);
	if(l1>len)
		l1=len;

	size_t l2=strlen(str2);
	if(l2>len)
		l2=len;

	if(l1 != l2)
		return l1 < l2? -1 : 1;

	return memcmp(str1, str2, l1);
}

size_t strlen(const char *str)
{
	size_t i = 0;

	for (; *str != '\0'; str++, i++)
		;

	return i;
}

char *strchr(const char *str, int c)
{
	/* TODO: Implement strchr(). */
	for(size_t i=0;str[i];i++)
		if(str[i]==c)
			return (char*)str+i;

	return NULL;
}

char *strrchr(const char *str, int c)
{
	/* TODO: Implement strrchr(). */
	char *last_occurrence=NULL;
	for(size_t i=0;str[i];i++)
		if(str[i]==c)
			last_occurrence=(char*)str+i;

	return last_occurrence;
}

char *strstr(const char *haystack, const char *needle)
{
	/* TODO: Implement strstr(). */
	for(size_t i=0;haystack[i];i++)
		for(size_t j=0;haystack[i+j];j++)
			if(!needle[j])
				return (char*)haystack+i;
			else if(haystack[i+j] != needle[j])
				break;

	return NULL;
}

char *strrstr(const char *haystack, const char *needle)
{
	/* TODO: Implement strrstr(). */
	char *last_occurrence=NULL;
	for(size_t i=0;haystack[i];i++)
		for(size_t j=0;haystack[i+j];j++)
			if(!needle[j])
				last_occurrence=(char*)haystack+i;
			else if(haystack[i+j] != needle[j])
				break;

	return last_occurrence;
}

void *memcpy(void *destination, const void *source, size_t num)
{
	/* TODO: Implement memcpy(). */
	for(size_t i=0;i<num;i++)
		*(char*)(destination+i)=*(char*)(source+i);

	return destination;
}

void *memrcpy(void *destination, const void *source, size_t num)
{
	/* TODO: Implement memcpy(). */
	for(size_t i=num;i;i--)
		*(char*)(destination+i-1)=*(char*)(source+i-1);

	return destination;
}

void *memmove(void *destination, const void *source, size_t num)
{
	/* TODO: Implement memmove(). */
	if(source<destination && source+num>=destination)
		memrcpy(destination, source, num);
	else
		memcpy(destination, source, num);

	return destination;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
	/* TODO: Implement memcmp(). */
	for(size_t i=0;i<num;i++)
		if( *(char*)(ptr1+i) != *(char*)(ptr2+i))
			return *(char*)(ptr1+i) < *(char*)(ptr2+i)? -1 : 1;

	return 0;
}

void *memset(void *source, int value, size_t num)
{
	/* TODO: Implement memset(). */
	for(size_t i=0;i<num;i++)
		*(char*)(source+i)=value;

	return source;
}
