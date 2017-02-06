#ifndef _UTIL_H_
#define _UTIL_H_

#include <vector>
#include <string>
#include <string.h>

namespace util{

/**
 * function: split the const c string, return a vector of cpp string
 **/
std::vector<std::string> split(const char *str, const char *delim);

std::string num2string(int n);
std::string num2string(double n);

};

#endif

