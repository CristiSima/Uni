#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status status;

    int recv_num;

    // First process starts the circle.
    if (rank == 0) {
        // First process starts the circle.
        // Generate a random number.
        recv_num=rand();
        printf("Start with %d\n", recv_num);
        // Send the number to the next process.
        //int MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator),
        MPI_Send(&recv_num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_num, 1, MPI_INT, numtasks - 1, 0, MPI_COMM_WORLD, &status);

    } else if (rank == numtasks - 1) {
        // Last process close the circle.
        // Receives the number from the previous process.
        MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        // Increments the number.
        recv_num++;
        printf("%d forwardsn %d\n", rank, recv_num);
        // Sends the number to the first process.
        MPI_Send(&recv_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    } else {
        // Middle process.
        MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        // Increments the number.
        recv_num++;
        printf("%d forwardsn %d\n", rank, recv_num);
        // Sends the number to the first process.
        MPI_Send(&recv_num, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();

}
