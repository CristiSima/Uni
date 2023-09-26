#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>

typedef struct {
    int size;
    int arr[1000];
} queue;

void add_queue(queue *q, int nr)
{
    q->arr[q->size]=nr;
    q->size++;
}

int main (int argc, char *argv[]) {
    int numtasks, rank;

    queue q;
    // TODO: declare the types and arrays for offsets and block counts

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype customtype;

    MPI_Datatype oldtypes[2]={MPI_INT, MPI_INT};
    int blockcounts[2]={1, 1000};
    MPI_Aint offsets[2]={offsetof(queue, size), offsetof(queue, arr)};

    MPI_Type_create_struct(2, blockcounts, offsets, oldtypes, &customtype);
    MPI_Type_commit(&customtype);


    // TODO: create the MPI data type, using offsets and block counts, and commit the data type

    srand(time(NULL));

    // First process starts the circle.
    if (rank == 0) {
        // First process starts the circle.
        // Generate a random number and add it in queue.
        add_queue(&q, rank);
        // Sends the queue to the next process.
        MPI_Send(&q, 1, customtype, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&q, 1, customtype, numtasks-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == numtasks - 1) {
        // Last process close the circle.
        // Receives the queue from the previous process.
        MPI_Recv(&q, 1, customtype, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Generate a random number and add it in queue.
        add_queue(&q, rank);
        // Sends the queue to the first process.
        MPI_Send(&q, 1, customtype, 0, 0, MPI_COMM_WORLD);
    } else {
        // Middle process.
        // Receives the queue from the previous process.
        MPI_Recv(&q, 1, customtype, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Generate a random number and add it in queue.
        add_queue(&q, rank);
        // Sends the queue to the next process.
        MPI_Send(&q, 1, customtype, rank+1, 0, MPI_COMM_WORLD);

    }

    // TODO: Process 0 prints the elements from queue
    if(rank==0)
    {
        for(int i=0;i<q.size;i++)
            printf("%d ", q.arr[i]);
        printf("\n");
    }

    // TODO: free the newly created MPI data type
    MPI_Type_free(&customtype);

    MPI_Finalize();
}
