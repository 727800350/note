// pointer to functions

#include <iostream>

using namespace std;

int addition(int a, int b){
	return (a + b);
}

int subtraction(int a, int b){
	return (a - b);
}

int operation(int x, int y, int (*func)(int, int)){
	return (*func)(x, y);
}

int main(){
	int m = 0;
	int n = 0;

	m = operation(7, 5, addition);
	cout << m << endl; // 12

	int (*minus)(int, int) = subtraction;
	n = operation(20, m, minus);
	cout << n << endl; // 8

	return 0;
}

