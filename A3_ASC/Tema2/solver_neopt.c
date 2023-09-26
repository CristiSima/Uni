/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include "utils.h"

/*
 * Add your unoptimized implementation here
 */
void mult_add(int N, double *A, double* B, double* res)
{
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			for(int k=0; k<N; k++)
				res[i*N+j]+=A[i*N+k]*B[k*N+j];
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
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			for(int k=i; k<N; k++)
				res[i*N+j]+=A[i*N+k]*B[k*N+j];
}

void mult_add_generic_inf(int N, double *A, double* B, double* res)
{
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			for(int k=j; k<N; k++)
				res[i*N+j]+=A[i*N+k]*B[k*N+j];
}

double* my_solver(int N, double *A, double* B) {
	printf("NEOPT SOLVER\n");

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
