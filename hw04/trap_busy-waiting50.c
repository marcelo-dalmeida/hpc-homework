/**
 * University of Pittsburgh
 * Department of Computer Science
 * CS1645: Introduction to HPC Systems
 * Instructor Bryan Mills, PhD
 * Student: 
 * Implement Pthreads version of trapezoidal approximation.
 */

#include <pthread.h>
#include <stdio.h>
#include "timer.h"
#define NUM_THREADS	50

// Global variables to make coverting to pthreads easier :)
double a;
double b;
int n;
double approx;
double total;
int flag;

// Actual areas under the f(x) = x^2 curves, for you to check your
// values against.
double static NEG_1_TO_POS_1 = 0.66666666666667;
double static ZERO_TO_POS_10 = 333.333;

// f function is defined a x^2
double f(double a) {
  return a * a;
}

void* calculate_trap(void* rank){
	double local_a;
	double local_b;
	double my_int;
	long my_rank = (long) rank;
	
	local_a = a + my_rank*local_n*h;
	local_b = local_a + local_n*h;
	
	my_int = calculate_local_trap(local_a, local_b, local_n, h);
	while (flag != my_rank);
	total += my_int;
	flag = (flag+1) % NUM_THREADS;
	
	return NULL;
}


// Serial implementation of trapezoidal approximation. You should
// refactor the loop in this function to be parallized using pthread.
void calculate_local_trap(double local_a, double local_b, int local_n, double h) {
  double x_i;
  approx = ( f(a) + f(b) ) / 2.0;
  for(int i = 1; i < local_n-1; i++) {
    x_i = local_a + i*h;
    approx += f(x_i);
  }
  approx = h*approx;
}

void trap(){
	pthread_t threads[NUM_THREADS];  
	int rc;
	long t;
	total = 0.0;
	flag = 0;
	h = (b-a)/n;
	local_n = n/NUM_THREADS;
	
	for(t = 0; t < NUM_THREADS; t++){
		printf("In main: creating thread %ld\n", t);
		rc = pthread_create(&threads[t], NULL, calculate_trap, (void *)t);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	
	for (t = 0; t = NUM_THREADS; t++){
		pthread_join(threads[i], NULL);
	}
	
	free(threads);
	
	return 0;
}

int main() {
  
  // Example 1 [-1,1]
  a = -1.0;
  b = 1.0;
  n = 1000000000;
  timerStart();
  trap();
  printf("Took %ld ms\n", timerStop());
  printf("a:%f\t b:%f\t n:%d\t actual:%f\t approximation:%f\n", a, b, n, NEG_1_TO_POS_1, approx);

  // Example 2 [0,10]
  a = 0.0;
  b = 10.0;
  n = 1000000000;
  timerStart();
  trap();
  printf("Took %ld ms\n", timerStop());
  printf("a:%f\t b:%f\t n:%d\t actual:%f\t approximation:%f\n", a, b, n, ZERO_TO_POS_10, approx);

  pthread_exit(NULL);
  return 0;
}
