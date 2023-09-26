/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include "utils.h"
#define BLOCK_SIZE 40
/*
 * Add your optimized implementation here
 */
void mult_addOLD(int N, double *A, double* B, double* res)
{
	for(int i=0; i<N; i++)
		for(int k=0; k<N; k++)
 			for(int j=0; j<N; j++)
				res[i*N+j]+=A[i*N+k]*B[k*N+j];
}

#define I (bi+i)
#define J (bj+j)
#define K (bk+k)

void mult_add(int N, double *A, double* B, double* res)
{
   for(int bi=0; bi<N; bi+=BLOCK_SIZE)
	   for(int bk=0; bk<N; bk+=BLOCK_SIZE)
		   for(int bj=0; bj<N; bj+=BLOCK_SIZE)

			   for(int i=0; i<BLOCK_SIZE; i++)
			   		for(int k=0; k<BLOCK_SIZE; k++)
		    			for(int j=0; j<BLOCK_SIZE; j++)

						   res[I*N+J]+=A[I*N+K]*B[K*N+J];
}

void self_transpose(int N, double *target)
{
	double temp;
	for(int i=0; i<N; i++)
		for(int j=i+1; j<N; j++)
		{
			temp=target[N*i+j];
			target[N*i+j]=target[N*j+i];
			target[N*j+i]=temp;
		}
}

void mult_add_sup_generic(int N, double *A, double* B, double* res)
{
	// optimisation: swap  k and j
	for(int i=0; i<N; i++)
		for(int k=i; k<N; k++)
	 		for(int j=0; j<N; j++)
				res[i*N+j]+=A[i*N+k]*B[k*N+j];
}

void mult_add_generic_inf(int N, double *A, double* B, double* res)
{
	// optimisation:
	// 		swap  k and j loops and update starting points
	//		break in blocks
	for(int bi=0; bi<N; bi+=BLOCK_SIZE)
		for(int bk=0; bk<N; bk+=BLOCK_SIZE)

			for(int i=0; i<BLOCK_SIZE; i++)
				 for(int k=0; k<BLOCK_SIZE; k++)

					for(int j=0; j<=k; j++)
						res[I*N+j]+=A[I*N+K]*B[K*N+j];
}

double* my_solver(int N, double *A, double* B) {
	printf("OPT SOLVER\n");

	// initialized to 0
	double *AxB=calloc(sizeof(double), N*N);
	double *res=calloc(sizeof(double), N*N);

	// AxB = A * B
	mult_add_sup_generic(N, A, B, AxB);

	// B = B**T
	self_transpose(N, B);
	// A = A**T
	self_transpose(N, A);

	// res = B**T * B**T
	mult_add(N, B, B, res);

	// res += (A*B) * A**T
	mult_add_generic_inf(N, AxB, A, res);

	free(AxB);
	return res;
}
