CC=gcc
CFLAGS=-Wall -Wextra -std=c99

build: star_dust

star_dust: star_dust.c
	$(CC) $(CFLAGS) star_dust.c -o star_dust

pack:
	zip -FSr 314CA_SimacheCristian_Tema2.zip README Makefile star_dust.c

clean:
	rm -f star_dust

.PHONY: pack clean
