#ifndef _UTIL_H_
#define _UTIL_H_

#include <vector>
#include <string>
#include <string.h>
#include <sys/time.h>

namespace util{

std::vector<std::string> split_any(const char *str, const char *delim);
std::vector<std::string> split_all(const char *str, const char *delim);

uint64_t ustime(void);
uint64_t mstime(void);
uint64_t stime(void);

};

#endif

