#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


// send the length before the data
void MPI_Send_Variable(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator)
{
	MPI_Send(&count, 1, MPI_INT, destination, tag, communicator);
	MPI_Send(data, count, datatype, destination, tag, communicator);
}

// read the length before the data
void MPI_Recv_Variable(void* data, int* count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)
{
	MPI_Recv(count, 1, MPI_INT, source, tag, communicator, status);
	MPI_Recv(data, *count, datatype, source, tag, communicator, status);
}

// read the length before the data and allocate a buffer with the apropriate size
void MPI_Recv_Variable_Init(void** data_p, int* count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)
{
	MPI_Recv(count, 1, MPI_INT, source, tag, communicator, status);
	int type_size;
	MPI_Type_size(datatype, &type_size);
	*data_p=malloc(type_size* count[0]);
	MPI_Recv(*data_p, *count, datatype, source, tag, communicator, status);
}

void print_topology(int rank, int *topology_size, int **topology)
{
	printf("%d -> ", rank);

	for (size_t i = 0; i < 4; i++) {
		printf("%d:", i);

		for(int j=1;j<topology_size[i];j++)
			printf("%d,", topology[i][j-1]);
		if(topology_size[i]>0)
			printf("%d", topology[i][topology_size[i]-1]);
		printf(" ");
	}
	printf("\n");
}

// macro for logging communication
#define log_msg(src, dst) printf("M(%d,%d)\n", src, dst)
// macro wrapper for logging communication
#define MPI_Send_W(data, count, datatype, destination, tag, communicator) {log_msg(rank, destination); MPI_Send(data, count, datatype, destination, tag, communicator);}

// macros for geting the neighbours of a coordinator node
#define FORWARD_ID ((rank==3)?0:((rank!=0)?(rank+1):-1))
#define BACKWARD_ID ((rank==0)?3:((rank!=1)?(rank-1):-1))

#define min(a,b) ((a<b)?(a):(b))

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

	int topology_size[4]={0};
	int *topology[4]={0};

	int manager;

	char file_name[101];

	int vector_size=atoi(argv[1]);

	if(rank<4)
	{
		// read topology
		sprintf(file_name, "../checker/tests/test1/cluster%d.txt", rank);
		sprintf(file_name, "cluster%d.txt", rank);
		FILE *f=fopen(file_name, "r");

		fscanf(f, "%d", topology_size+rank);
		topology[rank]=malloc(sizeof(int)*topology_size[rank]);

		// read the ids of all the managed workers
		for(int i=0;i<topology_size[rank];i++)
			fscanf(f, "%d", topology[rank]+i);

		fclose(f);

		// distribute topology betwen coordinators

		// printf("%d <- %d -> %d\n", BACKWARD_ID, rank, FORWARD_ID);
		// send forward
		if(FORWARD_ID !=-1)
		{
			MPI_Send_W(&rank, 1, MPI_INT, FORWARD_ID, 0, MPI_COMM_WORLD);
			log_msg(rank, FORWARD_ID); log_msg(rank, FORWARD_ID);
			MPI_Send_Variable(topology[rank], topology_size[rank], MPI_INT, FORWARD_ID, 0, MPI_COMM_WORLD);
		}
		// send backwards
		if(BACKWARD_ID!=-1)
		{
			MPI_Send_W(&rank, 1, MPI_INT, BACKWARD_ID, 0, MPI_COMM_WORLD);
			log_msg(rank, BACKWARD_ID); log_msg(rank, BACKWARD_ID);
			MPI_Send_Variable(topology[rank], topology_size[rank], MPI_INT, BACKWARD_ID, 0, MPI_COMM_WORLD);
		}

		// forward topology betwen coordinators
		for(int k=1;k<4;k++)
		{
			int org_sender;
			// get the id of coordinator for the fallowing workers
			MPI_Recv(&org_sender, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			// printf("%d --> %d -> %d\n", org_sender, status.MPI_SOURCE, rank);

			MPI_Recv_Variable_Init(topology+org_sender, topology_size+org_sender, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &status);

			// get the next id for forwarding
			int next_id=(status.MPI_SOURCE==BACKWARD_ID)?FORWARD_ID:BACKWARD_ID;
			// printf("%d --> %d -> %d -> %d\n", org_sender, status.MPI_SOURCE, rank, next_id);
			if(next_id!=-1)
			{
				MPI_Send_W(&org_sender, 1, MPI_INT, next_id, 0, MPI_COMM_WORLD);
				log_msg(rank, next_id); log_msg(rank, next_id);
				MPI_Send_Variable(topology[org_sender], topology_size[org_sender], MPI_INT, next_id, 0, MPI_COMM_WORLD);
			}
		}

		// send manager id to workers
		for(int worker_idx=0;worker_idx<topology_size[rank];worker_idx++)
			MPI_Send_W(&rank, 1, MPI_INT, topology[rank][worker_idx], 0, MPI_COMM_WORLD);

		// distribute topology to managed workers
		for(int worker_idx=0;worker_idx<topology_size[rank];worker_idx++)
			for(int i=0;i<4;i++)
			{
				log_msg(rank, topology[rank][worker_idx]); log_msg(rank, topology[rank][worker_idx]);
				MPI_Send_Variable(topology[i], topology_size[i], MPI_INT, topology[rank][worker_idx], 0, MPI_COMM_WORLD);
			}
	}
	else
	{
		// worker

		// get manager id
		MPI_Recv(&manager, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

		// get the full topology
		for(int i=0;i<4;i++)
			MPI_Recv_Variable_Init(topology+i, topology_size+i, MPI_INT, manager, 0, MPI_COMM_WORLD, &status);
	}


	print_topology(rank, topology_size, topology);

	// calculate the total number of workers for the next coordinators
	int workers_next=0;
	for(int i=BACKWARD_ID;i>0;i--)
		workers_next+=topology_size[i];

	int *vector;
	if(rank==0)
	{
		// genearate and send around
		vector=malloc(sizeof(int)*vector_size);
		for(int i=0;i<vector_size;i++)
			vector[i]=vector_size-i-1;
	}

	int sub_vector_size;
	if(rank<4)
	{
		// get from the previous coordinator
		if(rank!=0)
			MPI_Recv_Variable_Init(&vector, &vector_size, MPI_INT, FORWARD_ID, 0, MPI_COMM_WORLD, &status);

		sub_vector_size=round((topology_size[rank])*((double)vector_size/(topology_size[rank]+workers_next)));
		// printf("%d %d %d/%d\n", rank, topology_size[rank], sub_vector_size, vector_size);

		// send the reast to the next coordinator
		if(BACKWARD_ID!=-1)
		{
			log_msg(rank, BACKWARD_ID); log_msg(rank, BACKWARD_ID);
			MPI_Send_Variable(vector+sub_vector_size, vector_size-sub_vector_size, MPI_INT, BACKWARD_ID, 0, MPI_COMM_WORLD);
		}

		// split to workers
		for(int worker_idx=0;worker_idx<topology_size[rank];worker_idx++)
		{
			int start = worker_idx * (double)sub_vector_size / topology_size[rank];
			int end = min((worker_idx + 1) * (double)sub_vector_size / topology_size[rank], sub_vector_size);
			log_msg(rank, topology[rank][worker_idx]); log_msg(rank, topology[rank][worker_idx]);
			MPI_Send_Variable(vector+start, end-start, MPI_INT, topology[rank][worker_idx], 0, MPI_COMM_WORLD);
		}

		// reasemble from workers
		for(int worker_idx=0;worker_idx<topology_size[rank];worker_idx++)
		{
			int start = worker_idx * (double)sub_vector_size / topology_size[rank];
			MPI_Recv_Variable(vector+start, &start, MPI_INT, topology[rank][worker_idx], 0, MPI_COMM_WORLD, &status);
		}

		// combine with the result from the next coordinators
		if(BACKWARD_ID!=-1)
			MPI_Recv(vector+sub_vector_size, vector_size-sub_vector_size, MPI_INT, BACKWARD_ID, 0, MPI_COMM_WORLD, &status);

		// send back
		if(rank!=0)
			MPI_Send_W(vector, vector_size, MPI_INT, FORWARD_ID, 0, MPI_COMM_WORLD);
	}
	else
	{
		// get worker share
		MPI_Recv_Variable_Init(&vector, &sub_vector_size, MPI_INT, manager, 0, MPI_COMM_WORLD, &status);
		// process
		for(int i=0;i<sub_vector_size;i++)
			vector[i]*=5;
		// send back
		log_msg(rank, manager); log_msg(rank, manager);
		MPI_Send_Variable(vector, sub_vector_size, MPI_INT, manager, 0, MPI_COMM_WORLD);
	}

	if(rank==0)
	{
		// print result
		printf("Rezultat: ");
		for(int i=0;i<vector_size;i++)
			printf("%d ", vector[i]);
		printf("\n");
	}

    MPI_Finalize();

}
