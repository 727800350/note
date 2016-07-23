#include <stdio.h>
#include "omp.h"

int main(){
	int id = -100;
	int threads = 100;
	printf("id: %d[%p] threads: %d[%p]\n", id, &id, threads, &threads);
// 	#pragma omp parallel
// 	#pragma omp parallel shared(id, threads)
	#pragma omp parallel private(id, threads)
	{
		printf("before: id: %d[%p] threads: %d[%p]\n", id, &id, threads, &threads);
		threads = omp_get_num_threads();
		id = omp_get_thread_num();
		printf(" after: id: %d[%p] threads: %d[%p]\n", id, &id, threads, &threads);
	}

	printf("id: %d[%p] threads: %d[%p]\n", id, &id, threads, &threads);
	return 0;
}

/**
case 1:
#pragma omp parallel
$ ./a.out 
id: -100[0x7ffeb280e89c] threads: 100[0x7ffeb280e898]
before: id: -100[0x7ffeb280e89c] threads: 100[0x7ffeb280e898]
 after: id: 0[0x7ffeb280e89c] threads: 4[0x7ffeb280e898]
before: id: 0[0x7ffeb280e89c] threads: 4[0x7ffeb280e898]
 after: id: 3[0x7ffeb280e89c] threads: 4[0x7ffeb280e898]
before: id: 3[0x7ffeb280e89c] threads: 4[0x7ffeb280e898]
 after: id: 2[0x7ffeb280e89c] threads: 4[0x7ffeb280e898]
before: id: 2[0x7ffeb280e89c] threads: 4[0x7ffeb280e898]
 after: id: 1[0x7ffeb280e89c] threads: 4[0x7ffeb280e898]
id: 1[0x7ffeb280e89c] threads: 4[0x7ffeb280e898]

case 2:
#pragma omp parallel shared(id, threads)
$ ./a.out 
id: -100[0x7fff372706ac] threads: 100[0x7fff372706a8]
before: id: -100[0x7fff372706ac] threads: 100[0x7fff372706a8]
 after: id: 2[0x7fff372706ac] threads: 4[0x7fff372706a8]
before: id: 2[0x7fff372706ac] threads: 4[0x7fff372706a8]
 after: id: 1[0x7fff372706ac] threads: 4[0x7fff372706a8]
before: id: 1[0x7fff372706ac] threads: 4[0x7fff372706a8]
 after: id: 3[0x7fff372706ac] threads: 4[0x7fff372706a8]
before: id: 1[0x7fff372706ac] threads: 4[0x7fff372706a8]
 after: id: 0[0x7fff372706ac] threads: 4[0x7fff372706a8]
id: 0[0x7fff372706ac] threads: 4[0x7fff372706a8]

case 3:
#pragma omp parallel private(id, threads)
$ ./a.out 
id: -100[0x7fffcb4c1a5c] threads: 100[0x7fffcb4c1a58]
before: id: 0[0x7ff06bdd2e4c] threads: 0[0x7ff06bdd2e48]
 after: id: 3[0x7ff06bdd2e4c] threads: 4[0x7ff06bdd2e48]
before: id: 0[0x7ff06c5d3e4c] threads: 0[0x7ff06c5d3e48]
 after: id: 2[0x7ff06c5d3e4c] threads: 4[0x7ff06c5d3e48]
before: id: 32752[0x7ff06cdd4e4c] threads: 1843307237[0x7ff06cdd4e48]
 after: id: 1[0x7ff06cdd4e4c] threads: 4[0x7ff06cdd4e48]
before: id: 0[0x7fffcb4c1a0c] threads: 0[0x7fffcb4c1a08]
 after: id: 0[0x7fffcb4c1a0c] threads: 4[0x7fffcb4c1a08]
id: -100[0x7fffcb4c1a5c] threads: 100[0x7fffcb4c1a58]
**/

