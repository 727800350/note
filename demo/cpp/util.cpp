#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

std::vector<std::string> split(const char *str, const char *delim){
	std::vector<std::string> toks;
	char *p = NULL;

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

int main(){
	char s[] = "- This, a sample string.";
	char *delim = " ,.-";
	std::vector<std::string> toks = split(s, delim);
	for(unsigned int i = 0; i < toks.size(); i++){
		fprintf(stderr, "%dth token: %s\n", i, toks[i].c_str());
	}

	return 0;
}

