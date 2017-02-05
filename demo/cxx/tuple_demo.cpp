#include <tuple>
#include <stdio.h>
#include <string>
using namespace std;

tuple<int, string, char> foo(){
	return make_tuple(2014, "tuple", 'x');
}

int main(){
	int a = 0;
	string b;
	char c = 0;

	tie(a, b, c) = foo();
	printf("elems: %d %s %c\n", a, b.c_str(), c);

	tuple<int, string, char, float> x = make_tuple(2014, "tupule", 'x', 5.30);
	auto y = make_tuple(2014, "tupule", 'x', 5.30);
	printf("%d, %s, %c, %.2f\n", get<0>(x), get<1>(x).c_str(), get<2>(x), get<3>(x));
	printf("%d, %s, %c, %.2f\n", get<0>(y), get<1>(y), get<2>(y), get<3>(y));
	
	return 0;
}

/*
 * g++ -std=c++11 -g demo.cpp
 * 通过auto 得到的y, 第二个成员的类型是 const char*, 而不是string
 *
 **/

