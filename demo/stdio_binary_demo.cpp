#include <iostream>
#include <string>

#define max 100
int main(){
	char buffer[max]
	memset(buffer, 0, max);
	std::cin.read(buffer, max);

	std::cout.write(buffer, strlen(buffer) + 1);

	return 0;
}

