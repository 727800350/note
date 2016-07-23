#include <stdio.h>
#include <sys/types.h>
#include "omp.h"

#define size 100000000
u_int64_t a[size];

int main(){
	double time = 0.0;

	for(int i = 0; i < size; i++){
		a[i] = i;
	}
	time = omp_get_wtime();
	for(int i = 0; i < size; i++){
		a[i] = a[i] + 1;
	}
	fprintf(stderr, "time: %f\n", omp_get_wtime() - time);

	for(int i = 0; i < size; i++){
		a[i] = i;
	}
	time = omp_get_wtime();
	omp_set_num_threads(4);
	#pragma omp parallel for
	for(int i = 0; i < size; i++){
		a[i] = a[i] + 1;
	}
	fprintf(stderr, "time: %f\n", omp_get_wtime() - time);

	return 0;
}

