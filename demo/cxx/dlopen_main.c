#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

// 	gcc dlopen_main.c -ldl

int main(int argc, char **argv){
	void *handle = dlopen("./libso.so", RTLD_LAZY);
	if(!handle){
		fprintf(stderr, "%s\n", dlerror());
		exit(1);
	}
	dlerror();

	int (*f)(int, int);
	f = (int(*)(int, int))dlsym(handle, "add");
	if(f){
		fprintf(stdout, "%d\n", f(1, 999));
	}

	void *fx = dlsym(handle, "add");
	if(fx){
		fprintf(stdout, "%d\n", ((int(*)())fx)(1, 2));
		fprintf(stdout, "%d\n", ((int(*)(int, int))fx)(1, 2));
	}

	dlclose(handle);
	return 0;
}

