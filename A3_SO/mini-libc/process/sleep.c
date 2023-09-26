// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>

int nanosleep(const struct timespec *req, struct timespec *rem)
{
	int rez=syscall(__NR_nanosleep, req, rem);

	if(rez<0)
	{
		errno=-rez;
		rez=-1;
	}

	return rez;
}

unsigned int sleep(unsigned int seconds)
{
	/* TODO: Implement close(). */
	struct timespec req={
		.tv_sec=seconds,
		.tv_nsec=0,
	};
	struct timespec rem;

	int rez=nanosleep(&req, &rem);

	if(rez<0)
	{
		errno=-rez;
		rez=rem.tv_sec;
	}

	return rez;
}
