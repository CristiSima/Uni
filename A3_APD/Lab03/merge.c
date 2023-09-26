#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define min(a,b) (((a)<(b))?(a):(b))
int N;
int P;
int *v;
int *vQSort;
int *vNew;
int *vTemp;
pthread_barrier_t f_barrier;

void merge(int *source, int start, int mid, int end, int *destination) {
	int iA = start;
	int iB = mid;
	int i;

	for (i = start; i < end; i++) {
		if (end == iB || (iA < mid && source[iA] <= source[iB])) {
			destination[i] = source[iA];
			iA++;
		} else {
			destination[i] = source[iB];
			iB++;
		}
	}
}

void compare_vectors(int *a, int *b) {
	int i;

	for (i = 0; i < N; i++) {
		if (a[i] != b[i]) {
			printf("Sortare incorecta\n");
			return;
		}
	}

	printf("Sortare corecta\n");
}

void display_vector(int *v) {
	int i;
	int display_width = 2 + log10(N);

	for (i = 0; i < N; i++) {
		printf("%*i", display_width, v[i]);
	}

	printf("\n");
}

int cmp(const void *a, const void *b) {
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

int is_power_of_two(int n)
{
	if (n == 0) {
		return 0;
	}

	return (ceil(log2(n)) == floor(log2(n)));
}

void get_args(int argc, char **argv)
{
	if(argc < 3) {
		printf("Numar insuficient de parametri: ./merge N P (N trebuie sa fie putere a lui 2)\n");
		exit(1);
	}

	N = atoi(argv[1]);
	if (!is_power_of_two(N)) {
		printf("N trebuie sa fie putere a lui 2\n");
		exit(1);
	}

	P = atoi(argv[2]);
}

void init()
{
	int i;
	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);
	vNew = malloc(sizeof(int) * N);
	vTemp = malloc(sizeof(int) * N);
	pthread_barrier_init(&f_barrier, NULL, P);

	if (v == NULL || vQSort == NULL || vNew == NULL) {
		printf("Eroare la malloc!");
		exit(1);
	}

	srand(42);

	for (i = 0; i < N; i++)
		v[i] = rand() % N;
}


void print()
{
	printf("v:\n");
	display_vector(v);
	printf("vQSort:\n");
	display_vector(vQSort);
	compare_vectors(v, vQSort);
}

void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;
	int step_size=1;
	int * aux;

	// implementati aici merge sort paralel
	int start = thread_id * (double)N / P;
	int end = min((thread_id + 1) * (double)N / P, N);
	// printf("%d %d\n", start, (~(~start |((step_size<<1) -1))));
	for(;step_size<N;step_size<<=1)
	{
		for(int i=(~(~start |((step_size<<1) -1)))+2*step_size*((start&((step_size<<1) -1))>0); i<end;i+=2*step_size)
		{
			// printf("%d->%d step=%d\n", thread_id, i, step_size);
			// break;

			merge(v, i, i + step_size, i + 2 * step_size, vNew);


		}

		pthread_barrier_wait(&f_barrier);
		if(thread_id==0)
		{
			aux = v;
			v = vNew;
			vNew = aux;
			// display_vector(vNew);
			// display_vector(v);
		}
		pthread_barrier_wait(&f_barrier);

	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	get_args(argc, argv);
	init();

	int i;
	int thread_id[P];
	pthread_t tid[P];

	// se sorteaza vectorul etalon
	for (i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	// se creeaza thread-urile
	for (i = 0; i < P; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	// se asteapta thread-urile
	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	// merge sort clasic - trebuie paralelizat
	// int width, *aux;
	// for (width = 1; width < N; width = 2 * width) {
	// 	for (i = 0; i < N; i = i + 2 * width) {
	// 		merge(v, i, i + width, i + 2 * width, vNew);
	// 	}
	//
	// 	aux = v;
	// 	v = vNew;
	// 	vNew = aux;
	// }

	print();

	free(v);
	free(vQSort);
	free(vNew);

	return 0;
}
