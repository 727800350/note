/************************************************************
g++ program_options.cpp -L/usr/local/lib -lboost_program_options

results:
run:
	./a.out --apple 20 --orange 30
	./a.out -a 20 -o 30 --name eric
	./a.out --help

If using parse config file
	## comment: apples_oranges.cfg
	name=eric
	apple=10
	orange=20

	./a.out
************************************************************/

#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp> // 'reading_file' exception class is declared in errors.hpp
#include <iostream>

namespace opt = boost::program_options;

int main(int argc, char *argv[]){
	opt::options_description desc("All options");
	desc.add_options()
		("apple,a", opt::value<int>()->default_value(10), "apples that you have")
		("orange,o", opt::value<int>(), "oranges that you have")
		("name", opt::value<std::string>(), "your name")
		("help,h", "produce help message")
	;

	opt::variables_map vm;
	opt::store(opt::parse_command_line(argc, argv, desc), vm);
	opt::notify(vm);

	if(vm.count("help")) {
		std::cout << desc << "\n";
		return 0;
	}

	try {
		opt::store(opt::parse_config_file<char>("apples_oranges.conf", desc), vm);
		opt::notify(vm);
	} catch (const opt::reading_file& e){
		std::cout << e.what() << std::endl;
	}

	if (vm.count("name")){
		std::cout << "Hi," << vm["name"].as<std::string>() << "!\n";
	}
	std::cout << "Fruits count: " << vm["apple"].as<int>() + vm["orange"].as<int>() << std::endl;

	return 0;
}

