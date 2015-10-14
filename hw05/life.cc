#include "io.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#	include <omp.h>
#endif 

#define thread_count 10

void copy(int *array1, int *array2, int N)
{
  for(int i = 0; i < N; i++)
  {
		for(int j = 0; j < N; j++) 			
			array2[i][j] = array1[i][j]; 
	}
}

void life(int *world, int N)
{
  int temp[N][N];
  copy(array, temp, N);
	for(int i = 0; i < N; i++)
 	{
 		for(int j = 0; j < N; j++) 			
		{	
			int count = 0;
			
			if (i == 0){
				if (j == 0){
					count = array[i][j+1] +
						array[i+1][j+1] +
						array[i+1][j];
				}
				else{
					if (j == N-1){
						count = array[i][j-1] + 		
							array[i+1][j] +
							array[i+1][j-1];
					}
					else{
						count = array[i][j-1] + 
							array[i][j+1] +
							array[i+1][j+1] +
							array[i+1][j] +
							array[i+1][j-1];
					}
				}
			}
			else{
				if (i == N-1){
					if (j == 0){
						count = array[i-1][j+1] +
							array[i-1][j] +
							array[i][j+1];
					}
					else{
						if (j == N-1){
							count = array[i][j-1] + 
								array[i-1][j-1] +
								array[i-1][j];
						}
						else{
							count = array[i][j-1] + 
								array[i-1][j-1] +
								array[i-1][j+1] +
								array[i-1][j] +
								array[i][j+1] +
						}
					}
				}
				else{
					if (j == 0){
						count = array[i-1][j+1] +
							array[i-1][j] +
							array[i][j+1] +
							array[i+1][j+1] +
							array[i+1][j];
					}
					else{
						if (j == N-1){
							count = array[i][j-1] + 
								array[i-1][j-1] +
								array[i-1][j] +	
								array[i+1][j] +
								array[i+1][j-1];
						}
						else{
							count = array[i][j-1] + 
								array[i-1][j-1] +
								array[i-1][j+1] +
								array[i-1][j] +
								array[i][j+1] +
								array[i+1][j+1] +
								array[i+1][j] +
								array[i+1][j-1];
						}
					}
				}
			}
				
			if(count < 2 || count > 3)
				temp[j][i] = 0;

			if(count == 2)
				temp[j][i] = array[j][i];

			if(count == 3)
				temp[j][i] = 1;				
		}
 	}
  
  copy(temp, world);
}

int main(int argc, char* argv[])
{
	// checking parameters
	if (argc != 2 && argc != 3) {
		return 1;
	}
	
	int N = atoi(argv[1]);
	int M = atoi(argv[2]);
	int world[N][N];
	
	// reading files (optional)
	if(argc == 3){
		readMatrixFile(world, N, argv[3]);
	} else {
		srand(time(NULL));
	// Otherwise, generate a random matrix.
	# pragma omp parallel num_threads(thread_count) \
			default(none) shared(world, N) private(i, j)
	#	pragma omp for
		for (int i = 0; i < N; i++) {
	#		pragma omp for
		  for (int j = 0; j < N; j++) {
				world[i][j] = rand() % 2;
		  }
		}
	}
	
	for (int g = 0; g < M; g++)
	{
		life(world);
	}
	
                
	return 0;
}