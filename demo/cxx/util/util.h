#ifndef _UTIL_H_
#define _UTIL_H_

#include <vector>
#include <string>
#include <string.h>

namespace util{

std::vector<std::string> split_any(const char *str, const char *delim);
std::vector<std::string> split_all(const char *str, const char *delim);

};

#endif

