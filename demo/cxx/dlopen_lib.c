/*
 * gcc -fPIC -shared -o libso.so dlopen_lib.c
 * if in C++, need extern "C" for prevent name mangling
 */
int add(int a, int b){
	return a + b;
}

