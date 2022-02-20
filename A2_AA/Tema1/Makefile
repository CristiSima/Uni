CC=g++
CFLAGS= -Wall -Wextra -O2 -g

build: abi tab_rare SFH abi_set

run-best: SFH
	./SFH

run-p1: abi
	./abi

run-p2: tab_rare
	./tab_rare

run-p3: SFH
	./SFH

run-set1: abi_set
	./abi_set

abi_set:abi_set.cpp
	$(CC) $(CFLAGS) abi_set.cpp -o abi_set

abi: abi.cpp
	$(CC) $(CFLAGS) abi.cpp -o abi

tab_rare: tab_rare.cpp
	$(CC) $(CFLAGS) tab_rare.cpp -o tab_rare

SFH: SFH.cpp
	$(CC) $(CFLAGS) SFH.cpp -o SFH

.PHONY: clean pack

pack:
	zip -r cristian.simache.zip Makefile *.cpp in out set_in set_out times README generate.bash EtapaFinala.pdf

clean:
	rm abi tab_rare SFH
