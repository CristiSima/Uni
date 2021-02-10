CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS=ninel codeinvim vectsecv nomogram

build: $(TARGETS)

ninel: ninel.c
	$(CC) $(CFLAGS) $(@).c -o $(@)

codeinvim: codeinvim.c
	$(CC) $(CFLAGS) $(@).c -o $(@)

vectsecv: vectsecv.c
	$(CC) $(CFLAGS) $(@).c -o $(@)

nomogram: nomogram.c
	$(CC) $(CFLAGS) $(@).c -o $(@)

pack:
	zip -FSr 314CA_SimacheCristian_Tema1.zip README Makefile *.c

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
