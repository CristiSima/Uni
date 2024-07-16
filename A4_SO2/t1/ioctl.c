// SPDX-License-Identifier: GPL-2.0+

// testing program

#include "./tracer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char **argv)
{

	if(argc != 3)
	{
		printf("./ioctl <cmd> <arg>\n");
		return -1;
	}
	int fd;

	fd=open("/dev/tracer", O_RDWR);
	printf("open: %d\n", fd);

	int cmd = atoi(argv[1]);
	int arg = atoi(argv[2]);

	if(cmd == 1)
		printf("ioctl add: %d\n", ioctl(fd, TRACER_ADD_PROCESS, arg));
	else
		printf("ioctl rem: %d\n", ioctl(fd, TRACER_REMOVE_PROCESS, arg));
	close(fd);
	return 0;
}
