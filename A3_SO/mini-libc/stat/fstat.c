// SPDX-License-Identifier: BSD-3-Clause

#include <internal/syscall.h>
#include <sys/stat.h>
#include <errno.h>

int fstat(int fd, struct stat *st)
{
	int rez=syscall(__NR_fstat, fd, st);

	if(rez<0)
	{
		errno=-rez;
		rez=-1;
	}

	return rez;
}
