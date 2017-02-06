# boost
安装

	tar -xzvf boost_1_54_0.tar.gz
	cd boost_1_54_0
	./bootstrap.sh --prefix=/usr/local
	sudo ./b2 install --with=all
最后两个步骤的所需时间都很长

## program_options
可以`parse_command_line, parse_config_file, parse_environment`

### Basic option configuration
`--option value or --option=value`, 等于号前后不能有空格等字符

All of the below options should be added as additional lines in the `desc.add_options()`

	--option
	("option", "Info message about option")

	--option or -o
	("option,o", "Info message about option") // can use -o

	-o
	(",o", "Info message about option") // must use -o

Add an option that has an associated value

	--option <value>
	("option", po::value<arg_type>(), "Info message about option")

Specify that an option is required, The call to po::notify(vm) will throw if the option is not specified

	("option", po::value<arg_type>()->required(), "required option")

Specify an option that can be specified multiple times

	--option <value1> --option <value2> --option <value3>
	("option", po::value<std::vector<arg_type> >(), "a list of values")

### Accessing option values
Have the option set directly into an existing variable

	("option", po::value<arg_type>(&existingVariable), "info message")

Check if an option was passed in the command line

	if(vm.count("option"))
To extract the value for an option manually

	vm["option"].as<arg_type>()

### demo
[basic demo](../demo/cpp/boost/program_options.cpp)

## boost::any
Storing any value in a container/variable

	boost::any variable(std::string("hello world"));
	string s = boost::any_cast<std::string>(variable);

[boost::any demo](../demo/cpp/boost/container-any.cpp)

[db_wrapper using boost::any](../demo/cpp/boost/db-wrapper_any.cpp)

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

[boost::variant demo](../demo/cpp/boost/variant.cpp)

[db_wrapper using boost::variant](../demo/cpp/boost/db-wrapper_variant.cpp)

## boost::optional
Returning a value or flag where there is no value

[boost::variant demo](../demo/cpp/boost/optional.cpp)

## boost::array
Returning an array from a function

	#include <boost/array.hpp>
	typedef boost::array<char, 4> array4_t; 
	array4_t& vector_advance(array4_t& val);

The first template parameter of boost::array is the element type, and the second one is the size of an array.   
boost::array is a fixed-size array; if you need to change the array size at runtime, use std::vector or boost::container::vector instead.  
The boost::array<> class has no handwritten constructors and all of its members are public, so the compiler will think of it as a POD type.

One of the biggest advantages of boost::array is that it provides exactly the same 
performance as a normal C array. People from the C++ standard committee also liked it, so 
it was accepted to the C++11 standard. There is a chance that your STL library already has it 
(you may try to include the <array> header and check for the availability of std::array<>)

[boost::array demo](../demo/cpp/boost/array.cpp)

## boost::tuple
[boost::tuple demo](../demo/cpp/boost/tuple.cpp)

	boost::tuple<int, std::string> almost_a_pair(10, "Hello");
	boost::tuple<int, float, double, int> quad(10, 1.0f, 10.0, 1);

Some readers may wonder why we need a tuple when we can always write our own structures 
with better names, for example, instead of writing `boost::tuple<int, std::string>`, 
we can create a structure:

	struct id_name_pair {
    	int id;
    	std::string name;
	};
Well, this structure is definitely more clear than `boost::tuple<int, std::string>`.  
But what if this structure is used only twice in the code?

