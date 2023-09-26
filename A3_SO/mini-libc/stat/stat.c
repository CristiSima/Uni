// SPDX-License-Identifier: BSD-3-Clause

#include <sys/stat.h>
#include <fcntl.h>
#include <internal/syscall.h>
#include <errno.h>

int stat(const char *restrict path, struct stat *restrict buf)
{
	int rez=syscall(__NR_stat, path, buf);

	if(rez<0)
	{
		errno=-rez;
		rez=-1;
	}

	return rez;
}
