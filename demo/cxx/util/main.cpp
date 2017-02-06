#include <stdio.h>
#include "util.h"

int main(){
	char s[] = "- This, is a sample string.";
	char delim[] = " ,.-";
	std::vector<std::string> toks = util::split(s, delim);
	for(unsigned int i = 0; i < toks.size(); i++){
		fprintf(stderr, "%dth token: %s\n", i, toks[i].c_str());
	}

	return 0;
}

