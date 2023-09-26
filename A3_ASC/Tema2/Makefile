CC=gcc
CFLAGS=-Wall -Werror -O0 -g
LIBDIRS= -L/usr/lib/x86_64-linux-gnu/atlas -L/usr/lib/x86_64-linux-gnu/blas
LIBS= -latlas -lblas

all: compare tema2_blas tema2_neopt tema2_opt_m

tema2_blas: solver_blas.c main.c utils.h
	$(CC) $(CFLAGS) $^ $(LIBDIRS) $(LIBS) -o $@

tema2_neopt: solver_neopt.c main.c utils.h
	$(CC) $(CFLAGS) $^ -o $@

tema2_opt_m: solver_opt.c main.c utils.h
	$(CC) $(CFLAGS) $^ -o $@

compare: compare.c utils.h
	$(CC) $(OPT_CFLAGS) $^ -lm -o $@

memory: all
	valgrind --tool=memcheck --leak-check=full ./tema2_blas  test/input_valgrind > blas.memory  2>&1 &
	valgrind --tool=memcheck --leak-check=full ./tema2_opt_m test/input_valgrind > opt_m.memory 2>&1 &
	valgrind --tool=memcheck --leak-check=full ./tema2_neopt test/input_valgrind > neopt.memory 2>&1

cache: all
	valgrind --tool=cachegrind --branch-sim=yes ./tema2_blas  test/input_valgrind > blas.cache  2>&1 &
	valgrind --tool=cachegrind --branch-sim=yes ./tema2_opt_m test/input_valgrind > opt_m.cache 2>&1 &
	valgrind --tool=cachegrind --branch-sim=yes ./tema2_neopt test/input_valgrind > neopt.cache 2>&1

clean:
	rm -rf compare tema2_blas tema2_neopt tema2_opt_m
