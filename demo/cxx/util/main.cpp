#include <stdio.h>
#include "util.h"

int test_split_any(){
	char s[] = "- This,. is a  sample string.";
	char delim[] = " ,.-";

	std::vector<std::string> toks = util::split_any(s, delim);

	fprintf(stderr, "str: %s\n", s);
	fprintf(stderr, "token size: %lu\n", toks.size());
	for(unsigned int i = 0; i < toks.size(); i++){
		fprintf(stderr, "%dth token: \"%s\" of size: %lu\n", i, toks[i].c_str(), toks[i].size());
	}

	return 0;
}

int test_split_all(){
	char s[] = "\tobjurl\tfromurl\t\t";
	char delim[] = "\t";

	std::vector<std::string> toks = util::split_all(s, delim);

	fprintf(stderr, "str: %s\n", s);
	fprintf(stderr, "token size: %lu\n", toks.size());
	for(unsigned int i = 0; i < toks.size(); i++){
		fprintf(stderr, "%dth token: \"%s\" of size: %lu\n", i, toks[i].c_str(), toks[i].size());
	}

	return 0;
}

int main(){
	test_split_any();
	test_split_all();

	return 0;
}

