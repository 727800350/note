#include <string.h>
#include <stdio.h>

int main(){
	char s[] = "ab-cd : ef;gh :i-jkl;mnop;qrs-tu: vwx-y;z";
	char *delim = "-: ";
	char *p = NULL;
	p = strtok(s, delim);
	fprintf(stdout, "first:%s\n", p);
	while((p = strtok(NULL, delim)) != NULL){
		fprintf(stdout, "%s\n", p);
	}
	fprintf(stderr, "s:%s\n", s);
	return 0;
}

