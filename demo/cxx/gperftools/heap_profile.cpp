/*
g++ heap_profile.cpp -ltcmalloc
pprof --text ./a.out heap.prof
**/

#include <gperftools/heap-profiler.h>
#include <iostream>
#include <unistd.h>
#include <time.h>

using namespace std;

void test1(){
	int i = 0;
	while(i < 1000){
		i++;
	}
}

void test2() {
	int i = 0;
	while (i < 200){
		i++;
		char *p = new char[100];
	}
}

void test3() {
	for (int i = 0; i < 100; ++i) {
		test1();
		test2();
	}
}

int main() {
	HeapProfilerStart("heap");

	time_t tm[2];
	time(tm);
	for(int i = 0; i < 12; i++){
		test3();
		time(tm + 1);
		if(uint32_t(tm[1]) - uint32_t(tm[0]) >= 5){
			HeapProfilerDump("dump now");
			time(tm);
		}
		sleep(1);
	}
	fprintf(stdout, "Finish!\n");

	HeapProfilerStop();

	return 0;
}

