build:
	mpicc queue.c -o queue
run: build
	mpirun -np 8 --oversubscribe queue
clean:
	rm queue
