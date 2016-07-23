#include <stdio.h>
#include <sys/types.h>
#include "omp.h"

#define size 100
int a[size];

int main(){
	for(int i = 0; i < size; i++){
		a[i] = i;
	}

	int ave = 0;
	omp_set_num_threads(4);
	#pragma omp parallel for reduction(+:ave)
	for(int i = 0; i < size; i++){
		ave += a[i];
	}
	fprintf(stderr, "ave: %f\n", ave * 1.0 / size);

	return 0;
}

