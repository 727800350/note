#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    char *s = strndup(str, strlen(str));

    p = strtok(s, delim);
    toks.push_back(std::string(p));
    while ((p = strtok(NULL, delim)) != NULL) {
        toks.push_back(std::string(p));
    }

    free(s);
    return toks;
}

/**
 * split the str using delim as a whole, the returned vector size will always be num(delim) + 1
 * ex: "a delim gap delim b", the result is "a" "gap" "b"
 * if gap is empty, the result is "a" "" "b"
 * if b is empty, the result is "a" "gap" ""
 **/
std::vector<std::string> split_all(const char *str, const char *delim){
    std::vector<std::string> toks;

    size_t start = 0;
    size_t len = strlen(delim);

    std::string s(str);
    size_t pos = s.find(delim);
    while (pos != std::string::npos) {
        toks.push_back(s.substr(start, pos - start));
        start = pos + len;
        if (start >= s.size()) {
            break;
        }
        pos = s.find(delim, start);
    }

    if (start < s.size()) {
        toks.push_back(s.substr(start));
    }

    // the end of str is delim, so start will be str.size()
    if (start == s.size()) {
        toks.push_back("");
    }

    return toks;
}

/* Return the UNIX time in microseconds */
uint64_t ustime(void) {
    struct timeval tv;
    uint64_t ust = 0;

    gettimeofday(&tv, NULL);
    ust = ((uint64_t)tv.tv_sec) * 1000000;
    ust += tv.tv_usec;
    return ust;
}

/* Return the UNIX time in milliseconds */
uint64_t mstime(void) {
    return ustime() / 1000;
}

};

