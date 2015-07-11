#include <iostream>
#include <string>

int main(){
	std::string line;

	while(std::getline(std::cin, line)){
		std::cout << line << std::endl;
	}

	std::cout << "end of read" << std::endl;

	return 0;
}

