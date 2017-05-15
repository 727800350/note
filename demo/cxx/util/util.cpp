#include <stdio.h>
#include <sstream>
#include "util.h"

namespace util{

/**
 * any charactor of delim will be useded as a delimiter
 * ex: str = "- This, is a sample string." and delim = " ,.-", the result is "This" "is" "a" "sample" "string"
 **/
std::vector<std::string> split_any(const char *str, const char *delim){
	std::vector<std::string> toks;
	char *p = NULL;

	// strtok will modify the first parameter, so here make a copy
	int len = strlen(str);
	char *s = new char[len + 1];
	memcpy(s, str, len + 1);
	s[len] = '\0';

	p = strtok(s, delim);
	toks.push_back(std::string(p));
	while((p = strtok(NULL, delim)) != NULL){
		toks.push_back(std::string(p));
	}

	delete []s;
	return toks;
}

/**
 * split the str using delim as a whole, the returned vector size will always be num(delim) + 1
 * ex: "a delim gap delim b", the result is "a" "gap" "b"
 * if gap is empty, the result is "a" "" "b"
 * if b is empty, the result is "a" "gap" ""
 **/
std::vector<std::string> split_all(std::string &str, const char *delim){
	std::vector<std::string> toks;

	size_t start = 0;
	size_t len = strlen(delim);

	size_t pos = str.find(delim);
	while(pos != std::string::npos){
		toks.push_back(str.substr(start, pos - start));
		start = pos + len;
		if(start >= str.size()){
			break;
		}
		pos = str.find(delim, start);
	}

	if(start < str.size()){
		toks.push_back(str.substr(start));
	}

	// the end of str is delim, so start will be str.size()
	if(start == str.size()){
		toks.push_back("");
	}

	return toks;
}

std::string num2string(int num){
	std::ostringstream stream;
	stream << num;
	return stream.str();
}

std::string num2string(double num){
	std::ostringstream stream;
	stream << num;
	return stream.str();
}

};

