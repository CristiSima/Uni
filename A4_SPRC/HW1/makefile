#/**
#	Sisteme de programe pentru retele de calculatoare
#
#	Copyright (C) 2008 Ciprian Dobre & Florin Pop
#	Univerity Politehnica of Bucharest, Romania
#
#	this program is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.

#   this program is distributed in the hope that it will be useful,
#   but WItHOUt ANY WARRANtY; without even the implied warranty of
#   MERCHANtABILItY or FItNESS FOR A PARtICULAR PURPOSE.  See the
#   GNU General Public License for more details.
# */

CARGS=-g

.PHONY: clean run_server pack run_client

build: T1_xdr.c rpc_server.c rpc_client.c
	gcc $(CARGS) -o server rpc_server.c T1_svc.c T1_xdr.c -lnsl -Wall
	gcc $(CARGS) -o client rpc_client.c T1_clnt.c T1_xdr.c -lnsl -Wall

T1_xdr.c: T1.x
	rpcgen -C T1.x
	rpcgen -Cm T1.x > T1_svc.c

clean:
	rm -f T1_*.c T1.h server client

run_client: build
	./client localhost tests/test$(t)/client.in

run_server: build
	./server tests/test$(t)/userIDs.db tests/test$(t)/resources.db tests/test$(t)/approvals.db $(l)

pack: clean
	zip -r sprc_T1.zip *.c *.h *.x check.sh tests makefile README
