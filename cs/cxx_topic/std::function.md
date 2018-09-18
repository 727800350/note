# function pointer
ref: [深入理解C语言函数指针](http://www.cnblogs.com/windlaughing/archive/2013/04/10/3012012.html)

```C++
int myFun(int x){
	fprintf(stdout, "myFun: %d\n", x);
	return x;
}
```
一个数据变量的内存地址可以存储在相应的指针变量中,函数的首地址也可以存储在某个函数指针变量中. 这样,我就可以通过这个函数指针变量来调用所指向的函数了.

在C系列语言中,任何一个变量,总是要先声明,之后才能使用的.函数指针变量也应该要先声明.
```C++
// 声明一个指向一个接受int 参数, 返回值为int的函数指针变量.
int (*funP)(int);
funP = myFun;
```
实际上和 `int a` 差不多, 也就是类型是`int (*)(int)`, 名字是funP, myFun 也是这个类型.
```C++
typedef int(*FUN)(int); // 将FUN 定义为一个类型, 和 typedef int a 一样
```

```C++
void (*funP)(int); // 也可写成void(*funP)(int x),但习惯上一般不这样.
void (*funA)(int);
void myFun(int x);

//一般的函数调用
myFun(100);

// myFun与funP的类型关系类似于int 与int *的关系.
funP = &myFun; // 将myFun函数的地址赋给funP变量
(*funP)(200); // 通过函数指针变量来调用函数

//myFun与funA的类型关系类似于int 与int 的关系.
funA = myFun;
funA(300);

//三个貌似错乱的调用
funP(400);
(*funA)(600);
(*myFun)(1000);
```

总结:

- 其实, myFun的函数名与funP, funA函数指针都是一样的, 即都是函数指针. myFun函数名是一个函数指针常量,而funP,funA是函数数指针变量,这是它们的关系.
- 但函数名调用如果都得如`(*myFun)(10)`这样, 那书写与读起来都是不方便和不习惯的. 所以C语言的设计者们才会设计成又可允许myFun(10)这种形式地调用(这样方便多了,并与数学中的函数形式一样).
- 为了统一调用方式, funP函数指针变量也可以funP(10)的形式来调用.
- 赋值时,可以写成`funP = &myFun`形式, 也可以写成`funP = myFun`.
- 但是在声明时, `void myFun(int)` 不能写成`void (*myFun)(int )`. `void (*funP)(int)`不能写成`void funP(int)`.
- 函数指针变量也可以存入一个数组内.数组的声明方法:`int (*fArray[10])(int)`;

- `printf("myFun\t0x%p, 0x%p\n", &myFun, myFun);`: 输出两个地址是一样的
- `typedef void(*FunType)(int);`: 前加一个typedef关键字, 这样就定义一个名为FunType函数指针类型, 而不是一个FunType变量

关于参数

- 编译期检查返回值类型是否匹配
- 如果声明时不指定参数类型,则编译和运行时都不会检查参数列表,可以多传参数,也可以少传参数.输入结果未知.
- 如果声明是指定参数列表,则会在编译期检查赋值函数的参数列表是否匹配

# lambda
`[ capture ] ( params ) { body }`

- capture 指定了在可见域范围内 lambda 表达式的代码内可见得外部变量的列表，具体解释如下
	- `[]`: 未定义变量.试图在Lambda内使用任何外部变量都是错误的.
	- `[x, &y]`: x 按值捕获, y 按引用捕获.
	- `[&]`: 用到的任何外部变量都隐式按引用捕获
	- `[=]`: 用到的任何外部变量都隐式按值捕获
	- `[&, x]`: x显式地按值捕获. 其它变量按引用捕获
	- `[=, &z]`: z按引用捕获. 其它变量按值捕获
- params 指定 lambda 表达式的参数

```C++
[] () { return 1; } // compiler knows this returns an integer
[] () -> int { return 1; } // now we're telling the compiler what we want
```

```C++
#include <vector>
#include <algorithm>
std::vector<int> vec{0, 1, 2, 3, 4};
std::for_each(std::begin(vec), std::end(vec), [](int &x){x++;}); // 修改vec 里面的元素, 因此需要用ref

int sum = 0;
std::for_each(std::begin(vec), std::end(vec), [&sum](int x){sum += x;}); // 不修改vec 的元素, 又是基本类型, 可以直接用value
```

## Why Lambdas Rock
[Lambda Functions in C++11 - the Definitive Guide](https://www.cprogramming.com/c++11/c++11-lambda-closures.html)

Imagine that you had an address book class, and you want to be able to provide a search function.
You might provide a simple search function, taking a string and returning all addresses that match the string. Sometimes that's what users of the class will want.
But what if they want to search only in the domain name or, more likely, only in the username and ignore results in the domain name? There are a lot of potentially interesting things to search for.
Instead of building all of these options into the class, wouldn't it be nice to provide a generic "find" method that takes a procedure for deciding if an email address is interesting?
Let's call the method findMatchingAddresses, and have it take a "function" or "function-like" object.

```C++
#include <string>
#include <vector>

class AddressBook{
public:
	// using a template allows us to ignore the differences between functors, function pointers and lambda
	template<typename Func>
	std::vector<std::string> findMatchingAddresses (Func func){
		std::vector<std::string> results;
		for(auto itr = _addresses.begin(), end = _addresses.end(); itr != end; ++itr){
			// call the function passed into findMatchingAddresses and see if it matches
			if(func(*itr)){
				results.push_back(*itr);
			}
		}
		return results;
	}

	private:
	std::vector<std::string> _addresses;
};

AddressBook global_address_book;

std::vector<std::string> findAddressesFromOrgs(){
	return global_address_book.findMatchingAddresses(
		// we're declaring a lambda here; the [] signals the start
		[] (const string& addr){
			return addr.find(".org") != std::string::npos;
		}
	);
}
```

## How are Lambda Closures Implemented?
So how does the magic of variable capture really work?
It turns out that the way **lambdas are implemented is by creating a small class; this class overloads the `operator()`, so that it acts just like a function.**
A lambda function is an instance of this class;
when the class is constructed, any variables in the surrounding enviroment are passed into the constructor of the lambda function class and saved as member variables.
This is, in fact, quite a bit like the idea of a functor that is already possible.
The benefit of C++11 is that doing this becomes almost trivially easy--so you can use it all the time, rather than only in very rare circumstances where writing a whole new class makes sense.

## What type is a Lambda?
The main reason that you'd want to create a lambda function is that someone has created a function that expects to receive a lambda function.
We've already seen that we can use templates to take a lambda function as an argument, and auto to hold onto a lambda function as a local variable.
But how do you name a specific lambda? Because each lambda function is implemented by creating a separate class, as you saw earlier,
even single lambda function is really a different type--even if the two functions have the same arguments and the same return value!
But C++11 does include **a convenient wrapper for storing any kind of function--lambda function, functor, or function pointer: std::function.**

The new std::function is a great way of passing around lambda functions both as parameters and as return values.
It **allows you to specify the exact types for the argument list and the return value in the template.**
Here's out AddressBook example, this time using std::function instead of templates.
```C++
#include <functional>
#include <string>
#include <vector>

class AddressBook{
public:
	std::vector<string> findMatchingAddresses (std::function<bool (const string&)> func){
		std::vector<string> results;
		for(auto itr = _addresses.begin(), end = _addresses.end(); itr != end; ++itr){
			// call the function passed into findMatchingAddresses and see if it matches
			if(func(*itr)){
				results.push_back(*itr);
			}
		}
		return results;
	}

	private:
	std::vector<string> _addresses;
};
```
One big advantage of std::function over templates is that if you write a template, you need to put the whole function in the header file, whereas std::function does not.

closure implementation using lambda:
```C++
#include <functional>
std::function<int(void)> func(int x){
	int state = x;
	return [&](){
		return state++;
	};
}

auto f = func(100);
fprintf(stdout, "%d\n", f()); // 100
fprintf(stdout, "%d\n", f()); // 101
fprintf(stdout, "%d\n", f()); // 102
```

# Functors: Function Objects in C++
```C++
#include <iostream>

class myFunctorClass{
public:
	myFunctorClass(int x) : _x( x ) {}
	// C++ allows you to overload operator(), the "function call" operator
	int operator()(int y){
		return _x + y;
	}

private:
	int _x;
};

int main(){
	myFunctorClass addFive(5);
	std::cout << addFive(6);

	return 0;
}
```
the act of constructing an object lets you give the functor information that it can use inside the implementation of its function-like behavior(when the functor is called through operator()).

[Do we have closures in C++?](https://stackoverflow.com/questions/12635184/do-we-have-closures-in-c)

If you understand **closure as a reference to a function that has an embedded, persistent, hidden and unseparable context(memory, state).**

```C++
class summer{
public:
	summer() : sum(0) {}
	int operator() (int x){
		return sum += x;
	}
private:
	int sum; // sum 保存求和的状态
};

// make a closure
summer adder;
// use closure
adder(3);
adder(4);
std::cout << adder(0) << std::endl;
```
