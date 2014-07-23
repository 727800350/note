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
//	construct a boost::any type variable from std::string
	boost::any variable(std::string("hello world"));
	if(typeid(variable) == typeid(boost::any))
		cout <<"equal"<<endl;
	else
		cout << "not equal"<<endl;

	cout << boost::any_cast<std::string>(variable)<<endl;
	cout <<endl;

	string *sp = boost::any_cast<std::string>(&variable);
	if(typeid(sp) == typeid(string *))
		cout <<"equal"<<endl;
	else
		cout << "not equal"<<endl;
	*sp = "change from string pointer";
	cout << boost::any_cast<std::string>(variable)<<endl;

	std::vector<boost::any> some_values;
	
	cout << "demo int" <<endl;
	some_values.push_back(10);
	cout<< boost::any_cast<int>(some_values[0])<<endl;
	cout << endl;

	cout << "demo const char" <<endl;
	const char* c_str = "const char here!";
	some_values.push_back(c_str);
//	cout<< boost::any_cast<char *>(some_values[1])<<endl;
	printf("%s\n", boost::any_cast<const char *>(some_values[1]));
	cout << endl;

//	char 的demo 有一个warning: conversion from string literal to 'char *' is deprecated
	cout << "demo char" <<endl;
//	any_cast 中的类型要与变量的类型一致
	char* ch = "char here!";
	some_values.push_back(ch);
	printf("%s\n", boost::any_cast<char *>(some_values[2]));
	cout << endl;

	cout << "demo string" <<endl;
	some_values.push_back(std::string("string!"));

// 	由于str是从vector 中拷贝出来的, 所以对str的更改, 不会反映到vector 中(尽管back返回的是一个引用但是目标类型是string, 所以str是back返回的结果的拷贝)
	std::string str = boost::any_cast<std::string>(some_values.back());
	cout << str <<endl; // output "string"
	str += " add new string!";
	cout << str <<endl; // output "string add new string"
	cout<< boost::any_cast<std::string>(some_values.back())<<endl; //still "string"

// 	back()返回的是一个引用类型的数据
	std::string& s = boost::any_cast<std::string&>(some_values.back());
	cout << s <<endl; // output "string"
	s += " That is great!";
	std::cout << s <<endl; // output "sting That is great"
// 	由于上面更改的s是 vector 中一个元素的引用, 所以s更改之后, vector中的元素也改变了
	cout<< boost::any_cast<std::string&>(some_values.back())<<endl; // output "sting That is great"

	return 0;
}
