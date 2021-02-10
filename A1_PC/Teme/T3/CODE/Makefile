#Simache Cristian Ioan

build: main.c internalimg.c ioimg.c internalimg.h ioimg.h
	gcc main.c internalimg.c ioimg.c -Wall -Wextra -o image_editor -lm

exec: build
	./image_editor

pack:
	zip -FSr 314CA_SimacheCristian_Tema3.zip README Makefile *.c *.h

clean:
	rm -f image_editor

tests: build
	cat tests/FULL		| valgrind -s --track-origins=yes ./image_editor	2>&1 | less
	cat tests/rounding		| valgrind -s --track-origins=yes ./image_editor	2>&1 | less
	cat tests/save		| valgrind -s --track-origins=yes ./image_editor	2>&1 | less
	cat tests/filters	| valgrind -s --track-origins=yes ./image_editor	2>&1 | less
	# cat tests/L			| valgrind -s --track-origins=yes ./image_editor	2>&1 | less
	# cat tests/F			| valgrind -s --track-origins=yes ./image_editor	2>&1 | less
	# cat tests/RGB_ascii	| valgrind -s --track-origins=yes ./image_editor	2>&1 | less
	# cat tests/T			| valgrind -s --track-origins=yes ./image_editor	2>&1 | less
	# cat tests/P5		| valgrind -s --track-origins=yes ./image_editor	2>&1 | less
	# cat tests/P6		| valgrind -s --track-origins=yes ./image_editor	2>&1 | less

.PHONY: exec pack clean tests
