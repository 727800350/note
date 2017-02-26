/*
g++ cpu_profile.cpp -lprofiler -lunwind
pprof --text ./a.out cpu.prof
**/

#include <gperftools/profiler.h>
#include <iostream>

using namespace std;

void test1() {
    int i = 0;
    while (i < 1000) {
        i++;
    }  
}

void test2() {
    int i = 0;
    while (i < 2000)  {
        i++;
    }  
}

void test3() {
    for (int i = 0; i < 100000; ++i) {
        test1();
        test2();
    }  
}

int main() {
    ProfilerStart("cpu.prof");

    test3();
    printf("Finish!");

    ProfilerStop(); 

    return 0;
}

