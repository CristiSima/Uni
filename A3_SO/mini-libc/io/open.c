// SPDX-License-Identifier: BSD-3-Clause

#include <fcntl.h>
#include <internal/syscall.h>
#include <stdarg.h>
#include <errno.h>

int open(const char *filename, int flags, ...)
{
	va_list valist;

	va_start(valist, flags);
	mode_t mode = va_arg(valist, mode_t);
	va_end(valist);

	int rez=syscall(__NR_open, filename, flags, mode);

	if(rez<0)
	{
		errno=-rez;
		rez=-1;
	}

	return rez;
}
