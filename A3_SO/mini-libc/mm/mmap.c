// SPDX-License-Identifier: BSD-3-Clause

#include <sys/mman.h>
#include <errno.h>
#include <internal/syscall.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	/* TODO: Implement mmap(). */
	int64_t ret=syscall(9, addr, length, prot, flags, fd, offset);
	if(ret<0)
	{
		errno=-ret;
		ret=(int64_t)MAP_FAILED;
	}

	return (void*)ret;
}

void *mremap(void *old_address, size_t old_size, size_t new_size, int flags)
{
	int64_t ret=syscall(25, old_address, old_size, new_size, flags);
	if(ret<0)
	{
		errno=-ret;
		ret=(int64_t)MAP_FAILED;
	}

	return (void*)ret;
}

int munmap(void *addr, size_t length)
{
	/* TODO: Implement munmap(). */
	return syscall(11, addr, length);
}
