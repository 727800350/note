# boost
安装

	tar -xzvf boost_1_54_0.tar.gz
	cd boost_1_54_0
	./bootstrap.sh --prefix=/usr/local
	sudo ./b2 install --with=all
最后两个步骤的所需时间都很长

## boost::any
Storing any value in a container/variable

	boost::any variable(std::string("hello world"));
	string s = boost::any_cast<std::string>(variable);

## boost::variant
C++03 unions can only hold extremely simple types of data called POD (plain old data).
So in C++03, you cannot, for example, store `std::string` or `std::vector` in a union.
C++11 relaxes this requirement, but you'll have to manage the construction and destruction
of such types by yourself, call in-place construction/destruction, and remember what type is stored in a union.

Boost.Variant library can store any of the types specified at compile time;  
it also manages in-place construction/destruction and doesn't even require the C++11 standard.

	boost::variant< int, std::string > v;
	v = "hello";
	std::cout << v << std::endl;



