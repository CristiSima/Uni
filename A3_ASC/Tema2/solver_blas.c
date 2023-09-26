/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include "utils.h"
#include <string.h>
/*
 * Add your BLAS implementation here
 */
#include "cblas.h"

double* my_solver(int N, double *A, double *B) {
	printf("BLAS SOLVER\n");
	// C= ((A×B) × At) + Bt×Bt

	// res initialized to 0
	double *res=malloc(sizeof(double)*N*N);

	// res = B
	memcpy((void*)res, (void*)B, sizeof(double)*N*N);

	// AxB
	// res := A * res		using A as upper
	cblas_dtrmm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit,
				N, N, 1.d, A, N, res, N);

	// (AxB) x At
	// res := res * A**T 	using A as upper
	cblas_dtrmm(CblasRowMajor, CblasRight, CblasUpper, CblasTrans, CblasNonUnit,
				N, N, 1.d, A, N, res, N);

	// Bt×Bt + ((AxB) x At)
	// res += B**T * B**T	using B as generic matrix 
	cblas_dgemm(CblasRowMajor, CblasTrans, CblasTrans,
				N, N, N, 1, B, N, B, N, 1, res, N);

	return res;
}
