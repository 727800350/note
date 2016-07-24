#include <stdio.h>
#include <unistd.h>
#include "omp.h"

#define N 100
#define NUM_THREADS 4

int main(){
	int x[N];
	int y[N];
	double sum=0.0, sum_local[NUM_THREADS];

	for(int i = 0; i < N; i++){
		x[i] = i + 1;
		y[i] = i * i;
	}

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int me = omp_get_thread_num();
		sum_local[me] = 0.0;
		
		#pragma omp for
		for (int i = 0; i < N; i++){
			sum_local[me] += x[i] * y[i];
		}
		
		#pragma omp atomic
		sum += sum_local[me];
	}

	fprintf(stderr, "sum = %f\n", sum);

	return 0;
}

