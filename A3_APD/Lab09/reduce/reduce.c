#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main (int argc, char *argv[])
{
    int procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("%d/%d\n", rank, procs);

    int value = rank;
    int temp;
    MPI_Status status;

    // int max

    for (int i = 2; i <= procs; i *= 2) {
        printf("%d\n", i);
        if (rank % i == 0 && rank + (i / 2)<procs)
        {
            printf("R %d <- %d\n", rank, rank + (i / 2));

            // primește de la procesul cu rank-ul [rank + (pas / 2)]
            MPI_Recv(&temp, 1, MPI_INT, rank + (i / 2), 0, MPI_COMM_WORLD, &status);
            // adună
            printf("R %d\t%d\n", rank, temp);

            value+=temp;

        }
        else if (rank % (i / 2) == 0)
        {
            printf("S %d -> %d\n", rank, rank - (i / 2));

            // trimite la procesul cu rank-ul [rank - (pas / 2)]
            MPI_Send(&value, 1, MPI_INT, rank - (i / 2), 0, MPI_COMM_WORLD);

        }
    }

    if (rank == MASTER) {
        printf("Result = %d\n", value);
    }

    MPI_Finalize();

}
