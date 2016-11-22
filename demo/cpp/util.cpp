#include <stdio.h>
#include <string.h>
#include <vector.h>

std::vector<std::string> split(char *str, char *delim){
	std::vector<std::string> toks;
	char *p = NULL;

	p = strtok(str, delim);
	toks.push_back(std::string(p));
	while((p = strtok(NULL, delim)) != NULL){
		toks.push_back(std::string(p));
	}

	return toks;
}

int main(){
	char s[] = "- This, a sample string.";
	char *delim = " ,.-";
	std::vector<std::string> toks = split(s, delim);

	for(unsigned int i = 0; i < toks.size(); i++){
		fprintf(stderr, "%dth token: %s\n", i, toks[i].c_str());
	}

	return 0;
}

