#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 1000
#define MASTER 0

void compareVectors(int * a, int * b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		if(a[i]!=b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	int displayWidth = 2 + log10(v[N-1]);
	for(i = 0; i < N; i++) {
		printf(" %i", v[i]);
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}

int main(int argc, char * argv[]) {
	int rank, i, j;
	int nProcesses;
	MPI_Init(&argc, &argv);
	int pos[N];
	int sorted = 0;
	int *v = (int*)malloc(sizeof(int)*N);
	int *v2 = (int*)malloc(sizeof(int)*N);
	int *vQSort = (int*)malloc(sizeof(int)*N);

	for (i = 0; i < N; i++)
		pos[i] = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);
	MPI_Status status;

    if (rank == MASTER) {
		srandom(42);
		for(i = 0; i < nProcesses - 1; i++)
			v[i] = random() % 200;
    }

    // send the vector to all processes
	if(rank==0)
	for(int i=1;i<nProcesses;i++)
		MPI_Send(v, nProcesses, MPI_INT, i, 0, MPI_COMM_WORLD);
	else
		MPI_Recv(v, nProcesses, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);



	if(rank == 0) {
		// DO NOT MODIFY
		displayVector(v);

		// make copy to check it against qsort
		// DO NOT MODIFY
		for(i = 0; i < N; i++)
			vQSort[i] = v[i];
		qsort(vQSort, N, sizeof(int), cmp);

		int count=0;
		for(int i=0;i<nProcesses;i++) if(i!=rank)
			if(v[i]<v[rank])
				count++;
		int val_at_0=v[count];
		// sort the vector v
		for(i = 0; i < N; i++)
			v2[i]=-1;

		v2[0]=val_at_0;
		int temp;
        // recv the new pozitions
		for(int i=1;i<nProcesses;i++)
		{
			MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			int j=i;
			while(v[j]==-1)	j++;
			v2[j]=v[temp];
		}
		v=v2;
		displayVector(v);
		compareVectors(v, vQSort);
	} else {

        // compute the positions
		int count=0;
		for(int i=0;i<nProcesses;i++) if(i!=rank)
			if(v[i]<v[rank])
				count++;
        // send the new positions to process MASTER
		MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

	}

	MPI_Finalize();
	return 0;
}
