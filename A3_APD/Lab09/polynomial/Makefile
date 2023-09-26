build:
	mpicc polynomial.c -o polynomial -lm
run1: build
	mpirun -np 2 polynomial a1.txt
run2: build
	mpirun -np 14 --oversubscribe polynomial a2.txt
clear:
	rm polynomial
