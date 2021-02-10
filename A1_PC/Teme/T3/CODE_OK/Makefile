

build: main.c internalimg.c ioimg.c internalimg.h ioimg.h
	gcc main.c internalimg.c ioimg.c -Wall -Wextra -o image_editor

exec: build
	./image_editor

pack:
	zip -FSr 314CA_SimcaheCristian_Tema3.zip README Makefile *.c *.h

clean:
	rm -f image_editor

tests: build
	# cat tests/FULL			| ./image_editor	| less
	cat tests/save			| ./image_editor	| less
	# cat tests/filters		| ./image_editor	| less
	# cat tests/L			| ./image_editor	| less
	# cat tests/F			| ./image_editor	| less
	# cat tests/RGB_ascii	| ./image_editor	| less
	# cat tests/T			| ./image_editor	| less
	# cat tests/P5		| ./image_editor	| less
	# cat tests/P6		| ./image_editor	| less

.PHONY: exec pack clean tests
