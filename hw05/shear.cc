#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef _OPENMP
#	include <omp.h>
#endif 

#define thread_count 10

 
void sortRowsAlternateDirection(int *A, int M){
	int direction;
	# pragma omp parallel num_threads(thread_count) \
			default(none) shared(A, M) private(direction, i, j,  phase)
	#	pragma omp for		
	for (int i = 0; i < M; i++) {
		direction = i % 2;
		for (int phase = 0; phase < M; phase++){
			if (phase % 2 == 0){
	#			pragma omp for
				for (int j = 1; j < M; j += 2){
					if (direction == 0){
						if (A[i][j-1] > A[i][j]){
							swap(&A[i][j-1], &A[i][j]);
						}
					}
					else{
						if (A[i][j-1] < A[i][j]){
							swap(&A[i][j-1], &A[i][j]);
						}
					}
				}
			}
			else{
	#			pragma omp for
				for (int j = 1; j < M - 1; j += 2){
					if (direction == 0){
						if (A[i][j] > A[i][j+1]){
							swap(&A[i][j], &A[i][j+1]);
						}
					}
					else{
						if (A[i][j] < A[i][j+1]){
							swap(&A[i][j], &A[i][j+1]);
						}
					}
				}
			}
				
		}
	}
}
 
void sortColumns(int *A, int M){
	# pragma omp parallel num_threads(thread_count) \
			default(none) shared(A, M) private(i, j, phase)
	#	pragma omp for
	for (int j = 0; j < M; j++) {
		for (int phase = 0; phase < M; phase++){
			if (phase % 2 == 0){
	#			pragma omp for
				for (int i = 1; i < M; i += 2){
					if (A[i-1][j] > A[i][j]){
						swap(&A[i-1][j], &A[i][j]);
					}
				}
			}
			else{
	#			pragma omp for
				for (int i = 1; i < M - 1; i += 2){
					if (A[i][j] > A[i+1][j]){
						swap(&A[i][j], &A[i+1][j]);
					}
				}
			}
				
		}
	
	}
}
 
void shear_sort (int *A, int M) {
	int endloop = log2(M*M);
	for(int i = 0; i < endloop; i++)
	{
		 sortRowsAlternateDirection(A, M);
		 sortColumns(A, M)
	}
}

	
// Allocate square matrix.
int **allocMatrix(int size) {
  int **matrix;
  matrix = (int **)malloc(size * sizeof(int *));
  for (int row = 0; row < size; row++) {
    matrix[row] = (int *)malloc(size * sizeof(int));
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

// Allocate memory to square matrix
void initSquareMatrix(int N) {
  A = allocMatrix(sqrt(N));

}
 
// $ gcc -g -Wall -fopenmp -o shear shear.cc
// $ ./shear.cc
 
int main (int argc, char* argv[]) {
	
	// checking parameters
	if (argc != 1 && argc != 2) {
		return 1;
	}
	
	int N = atoi(argv[1]);
	initSquareMatrix(N);
	int M = sqrt(N);

	// reading files (optional)
	if(argc == 2){
		readMatrixFile(A, M, argv[2]);
	} else {
	// Otherwise, generate a random matrix.
	# pragma omp parallel num_threads(thread_count) \
			default(none) shared(A, M) private(i, j)
	#	pragma omp for
		for (int i = 0; i < M; i++) {
	#		pragma omp for
		  for (int j = 0; j < M; j++) {
				A[i][j] = rand() % 100;
		  }
		}
	}
	
		printMatrix(A, M);
		
		shear_sort(A, M);

		printMatrix(A, M);

    return 0;
}