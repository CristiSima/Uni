CC= gcc
CFLAGS=-Wall -Wextra -std=c99

build:
	$(CC) $(CFLAGS) ex.c -o ex
	./ex

E0_1: E0_1.c
	$(CC) $(CFLAGS) $@.c -o $@
	./$@

E0_1: E0_3.c
	$(CC) $(CFLAGS) $@.c -o $@
	./$@

E0_1: endian.c
	$(CC) $(CFLAGS) $@.c -o $@
	./$@

