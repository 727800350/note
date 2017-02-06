#include <boost/variant.hpp>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
int main()
{
	typedef boost::variant<int, const char*, std::string> my_var_t;
	std::vector<my_var_t> some_values;

	some_values.push_back(10);
	some_values.push_back("Hello there!");
	some_values.push_back(std::string("Wow!"));

	cout << "size: "<<some_values.size()<<endl;

	std::string& s = boost::get<std::string>(some_values.back());
	s += " That is great!\n";
	std::cout << s;

	int i = boost::get<int>(some_values[0]);
	cout << i <<endl;

	return 0;
}
