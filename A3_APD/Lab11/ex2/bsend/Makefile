build:
	mpicc deadlock.c -o deadlock
run: build
	mpirun -np 2 deadlock
clean:
	rm deadlock
