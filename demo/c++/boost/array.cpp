#include <boost/array.hpp>
#include <algorithm>
#include <iostream>
#include <stdio.h>
using namespace std;

/************************************************
unary_function has been deprecated in C++11.
unary_function is a base class for creating function objects with one argument.
unary_function does not define operator(); it is expected that derived classes will define this. 
unary_function provides only two types - argument_type and result_type - defined by the template parameters.

template <class Arg, class Result>
  struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
  };
************************************************/
// Functional object to increment value by one
struct add_1 : public std::unary_function<char, void> {
    void operator()(char& c) const {
        ++ c;
    }
    // If you're not in a mood to write functional objects,
    // but don't know what does 'boost::bind(std::plus<char>(),_1, 1)' do, 
	// then read recipe 'Binding a value as a function parameter'.
};

typedef boost::array<char, 4> array4_t;

// 传值
array4_t vector_advance1(array4_t val) {
    std::for_each(val.begin(), val.end(), add_1());
    return val;
}

// 引用类型
array4_t& vector_advance2(array4_t& val) {
    // boost::array has begin(), cbegin(), end(), cend(), rbegin(), size(), empty() and other functions that are common for STL containers.
    std::for_each(val.begin(), val.end(), add_1());
    return val;
}

void print(array4_t &val){
	std::copy(val.begin(), val.end(), std::ostream_iterator<int>(std::cout, ", "));
	cout << endl;
}

int main() {
    // We can initialize boost::array just like an array in C++11: array4_t val = {0, 1, 2, 3};
    // but in C++03 additional pair of curly brackets is required.
    array4_t val = {{0, 1, 2, 3}};

	print(val);

    // boost::array works like a usual array:
    array4_t val_res;       // it can be default constructible and
    val_res = vector_advance2(val);  // assignable

	print(val);
	print(val_res);

	cout << array4_t::static_size << endl;
	cout << val.size() << endl;
    return 0;
}

