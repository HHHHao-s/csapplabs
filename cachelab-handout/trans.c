/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

static int bsize=8;

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{

	int jj,kk,k,j;
	int v1,v2,v3,v4,v5,v6,v7,v8;
	for(kk = 0;kk<=N-bsize;kk+=bsize){ 
		for(jj = 0; jj<=M-bsize;jj+=bsize){
			for(k=kk;k<kk+bsize;k++){
				j=jj;
				v1 = A[k][j];	
				v2 = A[k][j+1];				
				v3 = A[k][j+2];	
				v4 = A[k][j+3];	
				v5 = A[k][j+4];	
				v6 = A[k][j+5];	
				v7 = A[k][j+6];	
				v8 = A[k][j+7];
				B[j][k] = v1;
				B[j+1][k] = v2;
				B[j+2][k] = v3;
				B[j+3][k] = v4;
				B[j+4][k] = v5;
				B[j+5][k] = v6;
				B[j+6][k] = v7;
				B[j+7][k] = v8;
				
			}
		}
	}
	

	
	if(kk<N){
		for(jj = 0; jj<M-bsize;jj+=bsize){ // copy the bottom in A
			for(k=kk;k<N;k++){
				for(j=jj;j<jj+bsize;j++){
					B[j][k] = A[k][j];
				}
			}
		}
	}
	
	
	if(jj<M){
		for(kk=0;kk<N-bsize;kk+=bsize){ // copy the right sight in A
			for(k=kk;k<kk+bsize;k++){
				for(j=jj;j<M;j++){
					B[j][k] = A[k][j];
				}
			}		
		}
	}
	

	if(jj<N || kk < M){
		for(k=kk;k<N;k++){ // fill the surplus
			for(j=jj;j<M;j++){
				B[j][k] = A[k][j];
			}
		}
	}
//	show(M,N,B);
	
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    if(M == 61)
	{
		int i, j, v1, v2, v3, v4, v5, v6, v7, v8;
		int n = N / 8 * 8;
		int m = M / 8 * 8;
		for (j = 0; j < m; j += 8)
			for (i = 0; i < n; ++i)
			{
				v1 = A[i][j];
				v2 = A[i][j+1];
				v3 = A[i][j+2];
				v4 = A[i][j+3];
				v5 = A[i][j+4];
				v6 = A[i][j+5];
				v7 = A[i][j+6];
				v8 = A[i][j+7];
				
				B[j][i] = v1;
				B[j+1][i] = v2;
				B[j+2][i] = v3;
				B[j+3][i] = v4;
				B[j+4][i] = v5;
				B[j+5][i] = v6;
				B[j+6][i] = v7;
				B[j+7][i] = v8;
			}
		for (i = n; i < N; ++i)
			for (j = m; j < M; ++j)
			{
				v1 = A[i][j];
				B[j][i] = v1;
			}
		for (i = 0; i < N; ++i)
			for (j = m; j < M; ++j)
			{
				v1 = A[i][j];
				B[j][i] = v1;
			}
		for (i = n; i < N; ++i)
			for (j = 0; j < M; ++j)
			{
				v1 = A[i][j];
				B[j][i] = v1;
			}
	}  

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
	registerTransFunction(trans, trans_desc); 
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

