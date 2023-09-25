cargs= -g -Wall -Iinclude



build: router

router: router.o queue.o skel.o list.o
	gcc $(cargs) -o router router.o queue.o skel.o list.o

router.o: router.c
	gcc $(cargs) -c router.c

queue.o: queue.c
	gcc $(cargs) -c queue.c

list.o: list.c
	gcc $(cargs) -c list.c

skel.o: skel.c
	gcc $(cargs) -c skel.c

clean:
	rm *.o router *.zip

tests: build close_port
	echo "it takes 3m to run all tests\nNot it's: " && date '+%T'
	(echo "FAILED?"&&sudo ./topo.py tests) | grep --color --context=1000 "FAILED" # NO OUTPUT UNTIL THE END

.PHONY: close_port
close_port:
	sudo fuser -k 6653/tcp || echo -n ''

manual_test: build close_port
	sudo ./topo.py
	sudo pkill -f "xterm -title"

router0: build
	./router rtable0.txt rr-0-1 r-0 r-1

router1: build
	./router rtable1.txt rr-0-1 r-0 r-1

pack:
	zip -r 324CA_Simache_Cristian_Ioan_Tema1.zip *.c include README Makefile
