#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status status;

    int rand_num;

    // Root process generates a random number.
    // Broadcasts to all processes.
    if (rank == 0) {
        // First process starts the circle.
        // Generate a random number.
        rand_num=rand();
        printf("Start with %d\n", rand_num);
        // Send the number to the next process.
        //int MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator),

    }

        // Middle process.
        MPI_Bcast(&rand_num, 1, MPI_INT, 0, MPI_COMM_WORLD);

        printf("%d got %d\n", rank, rand_num);




    printf("Process [%d], after broadcast %d.\n", rank, rand_num);

    MPI_Finalize();

}
