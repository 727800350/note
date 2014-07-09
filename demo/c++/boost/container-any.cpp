/******************************
vector, boost:any, string, string &
******************************/
#include <boost/any.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
using namespace std;

int main(){
	std::vector<boost::any> some_values;
	some_values.push_back(10);
	const char* c_str = "Hello there!";
	some_values.push_back(c_str);
	some_values.push_back(std::string("string!"));
	some_values.push_back(std::string("Wow!"));
// 	back()返回的是一个引用类型的数据
	std::string& s = boost::any_cast<std::string&>(some_values.back());

	cout << s <<endl;
	s += " That is great!";
	std::cout << s <<endl;
// 	由于上面更改的s vector 中一个元素的引用, 所以s更改之后, vector中的元素也改变了
	cout<< boost::any_cast<std::string&>(some_values.back())<<endl;

	std::string str = boost::any_cast<std::string>(some_values[2]);
	cout << str <<endl;
	str += " the second element!";
	cout << str <<endl;
// 	由于上面操作的str是从vector 中拷贝出来的, 所以对str的更改, 不会反映到vector 中
	cout<< boost::any_cast<std::string>(some_values[2])<<endl;
	return 0;
}

/**************************************
result: 
[eric@iis boost]$ ./a.out
Wow!
Wow! That is great!
Wow! That is great!
string!
string! the second element!
string!
**************************************/
