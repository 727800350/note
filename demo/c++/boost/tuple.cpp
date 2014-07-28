#include <boost/tuple/tuple.hpp>
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

int main(){
	boost::tuple<int, std::string> almost_a_pair(10, "Hello");
	int i = boost::get<0>(almost_a_pair);
	const std::string& str = boost::get<1>(almost_a_pair);
	cout << i << "," <<str <<endl;

	boost::tuple<int, float, double, int> quad(10, 1.0f, 10.0, 1);
	double d = boost::get<2>(quad);
	cout << d <<endl;

	auto t = boost::make_tuple(0, -1.0, 2);
	assert(2 == boost::get<2>(t));
	return 0;
}
