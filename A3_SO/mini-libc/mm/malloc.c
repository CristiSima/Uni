// SPDX-License-Identifier: BSD-3-Clause

#include <internal/mm/mem_list.h>
#include <internal/types.h>
#include <internal/essentials.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

void *malloc(size_t size)
{
	/* TODO: Implement malloc(). */
	void *data=mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if(!data)
		return NULL;

	if(mem_list_add(data, size))
	{
		munmap(data, size);
		return NULL;
	}

	return data;
}

void *calloc(size_t nmemb, size_t size)
{
	return malloc(nmemb*size);
}

void free(void *ptr)
{
	struct mem_list *item;

	item = mem_list_find(ptr);
	if (item == NULL)
		return;

	munmap(item->start, item->len);
	mem_list_del(ptr);
}

void *realloc(void *ptr, size_t size)
{
	struct mem_list *item;

	item = mem_list_find(ptr);
	if (item == NULL)
		return MAP_FAILED;

	void *new_loc=mremap(item->start, item->len, size, MREMAP_MAYMOVE);

	if(new_loc==MAP_FAILED)
		return MAP_FAILED;

	item->start=new_loc;
	item->len=size;

	return new_loc;
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
	return realloc(ptr, nmemb*size);
}
