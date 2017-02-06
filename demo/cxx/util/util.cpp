#include <stdio.h>
#include <sstream>
#include "util.h"

namespace util{

std::vector<std::string> split(const char *str, const char *delim){
	std::vector<std::string> toks;
	char *p = NULL;

	// strtok will modify the first parameter, so here make a copy
	char *s = new char[strlen(str) + 1];
	memcpy(s, str, strlen(str) + 1);

	p = strtok(s, delim);
	toks.push_back(std::string(p));
	while((p = strtok(NULL, delim)) != NULL){
		toks.push_back(std::string(p));
	}

	delete []s;
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

