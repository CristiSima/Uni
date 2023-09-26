// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <errno.h>

int ftruncate(int fd, off_t length)
{
	int rez=syscall(__NR_ftruncate, fd, length);

	if(rez<0)
	{
		errno=-rez;
		rez=-1;
	}

	return rez;
}
