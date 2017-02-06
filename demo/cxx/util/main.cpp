#include <stdio.h>
#include "util.h"

int test_split(){
	char s[] = "- This, is a sample string.";
	char delim[] = " ,.-";
	std::vector<std::string> toks = util::split(s, delim);
	for(unsigned int i = 0; i < toks.size(); i++){
		fprintf(stderr, "%dth token: %s\n", i, toks[i].c_str());
	}

	return 0;
}

int test_num2string(){
	fprintf(stderr, "%s\n", util::num2string(2).c_str());
	fprintf(stderr, "%s\n", util::num2string(-2).c_str());
	fprintf(stderr, "%s\n", util::num2string(2.5).c_str());
	fprintf(stderr, "%s\n", util::num2string(-2.5).c_str());

	return 0;
}

int main(){
	test_num2string();

	return 0;
}

