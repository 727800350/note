# 参数传递
Things are passed by value unless you specify otherwise using the &-operator 
(note that this operator is also used as the 'address-of' operator, but in a different context).
```
void foo(vector<int> bar); // by value
void foo(vector<int> &bar); // by reference (non-const, so modifyable inside foo
void foo(vector<int> const &bar); // by const-reference
```
You can also choose to pass a pointer to a vector void `foo(vector<int> *bar)`, 
but unless you know what you know exactly what are doing, do not do this.

````
void foo1(int *arr) { cout << sizeof(arr) << '\n'; }
void foo2(int arr[]) { cout << sizeof(arr) << '\n'; }
void foo3(int arr[10]) { cout << sizeof(arr) << '\n'; }
void foo4(int (&arr)[10]) { cout << sizeof(arr) << '\n'; }

int main(){
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    foo1(arr);
    foo2(arr);
    foo3(arr);
    foo4(arr);
}
````

# IO
- `std::cin extern std::istream`
- `std::cout extern std::ostream`
- `std::cerr extern std::ostream`
- ios_base <--- ios <--- istream(ostream) <--- ifstream(ofstream)

ifstream: input file stream


```
std::cin >> in;
std::cout << out;
```
But when reading a string, `std::cin >> in` stops reading as soon as it encounters any black space character(a space or new line etc.)
You may want to use getline to get a entire line of input from the console, `std::getline(std::cin, in)`
```
istream& getline (istream& is, string& str, char delim);
istream& getline (istream& is, string& str);
```
Extracts characters from is and stores them into str until the delimitation character delim is found (or the newline character '\n' for the second usage).
The **encountered delimiter is extracted and discarded**, i.e. it is not stored and the next input operation will begin after it.

You use the same methods with a file (when dealing with non binary data).
std::ofstream ofs('myfile.txt');

[标准输入, 输出](../demo/cpp/stdio_demo.cpp)

- `istream& read (char* s, streamsize n);`  
Extracts n characters from the stream and stores them in the array pointed to by s.
This function simply copies a block of data, without checking its contents nor appending a null character at the end.
- `ostream& write (const char* s, streamsize n);`  
Inserts the first n characters of the array pointed by s into the stream.
This function simply copies a block of data, without checking its contents: 
The array may contain null characters, which are also copied without stopping the copying process.

[二进制标准输入, 输出](../demo/cpp/stdio_binary_demo.cpp)

## File IO
- fwrite is still (usually) buffered I/O(and that is a good thing)
- ofstream is a perfectly good interface for doing formatted output to a file
and ostream (and thus all of its derived classes) have the write member which serves the same purpose as fwrite.

```
#include <fstream>      // std::ofstream
int main (){
	std::ofstream ofs ("output.txt", std::ofstream::out);
	ofs << "lorem ipsum";
	ofs.close();
	return 0;
}
```

二进制io [ex](http://www.cplusplus.com/reference/ostream/ostream/write/)
```
std::ifstream infile("input.txt", std::ifstream::binary);
infile.read()
std::ofstream outfile("output.txt", std::ofstream::binary);
outfile.write()
```

we can initialize the array of char elements called myword with a null-terminated sequence of characters
by either one of these two methods:

	char myword [] = { 'H', 'e', 'l', 'l', 'o', '\0' };
	char myword [] = "Hello";
In both cases the array of characters myword is declared with a size of 6 elements of type char: the 5 characters
that compose the word "Hello" plus a final null character ('\0') which specifies the end of the sequence and that,
in the second case, when using double quotes, '\0' is appended automatically.

[**Pointers to functions**](../demo/c++/pointer_function.cpp)  

# [string](http://www.cplusplus.com/reference/string/string/)
- `std::string::c_str`: 
Returns a pointer to an internal array that contains a null-terminated sequence of characters (i.e., a C-string).
A program shall **not alter** any of the characters in this sequence.

scan in values from a string

使用C语言的方式, sscanf
```
string str("2.5,6.7");
double val1, val2;
if(sscanf(str.c_str(),"%lf,%lf", &val1, &val2) == 2){
    // got them!
}
```

使用boost
```
#include <spirit.hpp>
using namespace boost::spirit;
boost::fusion::vector < double, double > value_;

std::string string_ = "10.5,10.6 ";
bool result_ = qi::parse(string_.begin(), string_.end(), qi::double_ >> ',' >> qi::double_, value_);
```

# Memory
	pointer = new type
	pointer = new type [number_of_elements]
The first expression is used to allocate memory to contain one single element of type type. The second one is used
to assign a block (an array) of elements of type type

	delete pointer;
	delete [] pointer;

## 二维数组
[C++二维数组new小结(zz)](http://www.cnblogs.com/beyondstorm/archive/2008/08/26/1276278.html)

# Class
定义class的结尾}后需要一个分号,和结构体的定义一样.

对象作为参数传递时是传值, 而不是传递对象的地址,如果需要传递地址,需要明确指出.

private, public or protected.  
These specifiers modify the access rights that the members following
them acquire:

- *private* members of a class are accessible only from within other members of **the same class** or from their **friends**.
- *protected* members are accessible from members of their **same class** and from their **friends**, but also from members of their **derived classes**.
- *public* members are accessible from **anywhere** where the object is visible.

**By default, all members of a class are private.**

[class demo](../demo/c++/class_demo.cpp)

**Constructors**  
Objects generally need to initialize variables or assign dynamic memory during their process of creation to become
operative and to avoid returning unexpected values during their execution.

Like any other function, a constructor can also be overloaded with more than one function that have the same
name but different types or number of parameters.

The use of destructors is especially suitable when an object assigns dynamic memory during its lifetime and at the
moment of being destroyed we want to release the memory that the object was allocated.

[class construnctor and destructor](../demo/c++/class_constructor_destructor.cpp)

## Overloading operators
[class overload operator](../demo/c++/class_overload_operator.cpp)

**keyword this**  
The keyword this represents a pointer to the object whose member function is being executed. It is a pointer to
the object itself.

**Static members**  
A class can contain static members, either data or functions.

Static data members of a class are also known as "class variables", because there is **only one unique value for all
the objects of that same class**. Their content is not different from one object of this class to another.

For example, it may be used for a variable within a class that can contain a counter with the number of objects of
that class that are currently allocated.

**类成员函数中const的使用**  
一般放在函数体后,形如:
	
	void fun() const;
如果一个成员函数的不会修改数据成员,那么最好将其声明为const,因为const成员函数中不允许对数据成员进行修改,如果修改,编译器将报错,这大大提高了程序的健壮性  
(const 的其他用法见[const in c](./c.md))

**const object**  
const对象不能调用非const函数

上面关于const的示例[const demo](../demo/c++/const.cpp)

## Friendship and inheritance
### Friend functions
In principle, private and protected members of a class cannot be accessed from outside the same class in which
they are declared. However, this rule does not affect friends.

[friend function](../demo/c++/friend_function.cpp)

### Friend classes
Just as we have the possibility to define a friend function, we can also define a class as friend of another one,
granting that first class access to the protected and private members of the second one.

[friend class](../demo/c++/friend_class.cpp)

In this example, we have declared CRectangle as a friend of `CSquare` so that CRectangle member functions could
have access to the protected and private members of `CSquare`, more concretely to `CSquare::side`, which describes the side width of the square.

### [Inheritance between classes](http://www.cnblogs.com/fzhe/archive/2012/12/25/2832250.html)
派生类的声明:
```
class 派生类名:继承方式 基类名1, 继承方式 基类名2,...,继承方式 基类名n
{
    派生类成员声明;
};
```
继承方式规定了如何访问基类继承的成员.继承方式有public, private, protected.
如果不显示给出继承方式,默认为private继承.
继承方式指定了派生类成员以及类外对象对于从基类继承来的成员的访问权限.  
**不管是哪种继承方式, 基类的私有成员都不能访问**

- 公有继承  
**基类的公有和保护成员的访问属性在派生类中不变**,
即基类的公有成员和保护成员被继承到派生类中仍作为派生类的公有成员和保护成员.派生类的其他成员可以直接访问它们.
无论派生类的成员还是派生类的对象都无法访问基类的私有成员.
 
- 私有继承  
**基类中的公有成员和保护成员都以私有成员身份出现在派生类中**,
基类的公有成员和保护成员被继承后作为派生类的私有成员,派生类的其他成员可以直接访问它们,但是在类外部通过派生类的对象无法访问.
无论是派生类的成员还是通过派生类的对象,都无法访问从基类继承的私有成员.
通过多次私有继承后,对于基类的成员都会成为不可访问.因此私有继承比较少用.
 
- 保护继承  
**基类的公有成员和私有成员都以保护成员的身份出现在派生类中**,
派生类的其他成员可以直接访问从基类继承来的公有和保护成员,但是类外部通过派生类的对象无法访问它们.
无论派生类的成员还是派生类的对象,都无法访问基类的私有成员.

派生类构造函数的语法:
```
派生类名::派生类名(参数总表):基类名1(参数表1),....基类名n(参数名n),内嵌子对象1(参数表1),...内嵌子对象n(参数表n)
{
    派生类新增成员的初始化语句;
}
```
**注:构造函数的初始化顺序并不以上面的顺序进行,而是根据声明的顺序初始化.**

派生类构造函数执行的次序:

1. 调用基类构造函数,调用顺序按照它们被继承时声明的顺序(**从左到右**),
1. 调用内嵌成员对象的构造函数,调用顺序按照它们在类中声明的顺序,
1. 派生类的构造函数体中的内容.

[inheritance construnctor demo](../demo/c++/inheritance_constructor.cpp)

派生类的析构函数的功能是在该对象消亡之前进行一些必要的清理工作,析构函数没有类型,也没有参数.
析构函数的执行顺序与构造函数相反.

1. 派生类的构造函数体中的内容
1. 调用内嵌成员对象的构造函数,调用顺序按照它们在类中声明的逆顺序
1. 调用基类构造函数,调用顺序按照它们被继承时声明的逆顺序

[inheritance destrunctor demo](../demo/c++/inheritance_destructor.cpp)

如果某派生类的多个基类拥有同名的成员,同时,派生类又新增这样的同名成员,在这种情况下,派生类成员将覆盖所有基类的同名成员.这就需要显示的指定基类, 才能调用基类的同名成员.

[scope class](../demo/c++/scope_class.cpp)

如果某个派生类的部分或全部直接基类是从另一个共同的基类派生而来,在这些直接基类中,从上一级基类继承来的成员就拥有相同的名称,因此派生类中也就会产生同名现象,
对这种类型的同名成员也要使用作用域分辨符来唯一标识,而且必须用直接基类进行限定

[scope class2](../demo/c++/scope_class2.cpp)

为了解决前面提到的多重拷贝的问题,可以将共同基类设置为**虚基类**,这时从**不同的路径继承过来的同名数据成员在内存中就只有一个拷贝,同一个函数也只有一个映射**.
虚基类的声明是在派生类的声明过程,其语法形式为:
```
class 派生类名::virtual 继承方式 基类名,
```
[scope virtual class](../demo/c++/scope_class_virtual.cpp)

虚基类及其派生类的构造函数
一般而言,派生类只对其直接基类的构造函数传递参数,但是在虚基类中,不管是直接或间接虚基类的所有派生类,都必须在构造函数的成员初始化列表中列出对虚基类的初始化.

[virtual class constructor](../demo/c++/scope_class_virtual_constructor.cpp)

以上例子看上去B0的构造函数好像被调用了三次,但是实际上只有D1类中的D1(int a):B0(a), B1(a), B2(a)
才是真正的调用了B0构造函数.
 
赋值兼容规则是指在需要基类对象的任何地方都可以使用公有派生类的对象来替代(派生类的对象可以赋值给基类对象)
在替代之后,派生类对象就可以作为基类的对象使用,但只能使用从基类继承的成员.

[class assignment](../demo/c++/class_assignment.cpp)

### Virtual members

### Polymorphism

### Abstract base classes

# Templates
## Function templates

	// function template
	#include <iostream>
	using namespace std;

	template <class T>
	T GetMax (T a, T b) {
		T result;
		result = (a>b)? a : b;
		return (result);
	}

	int main () {
		int i=5, j=6, k;
		long l=10, m=5, n;
		k=GetMax<int>(i,j);
		n=GetMax<long>(l,m);
		cout << k << endl;
		cout << n << endl;
		return 0;
	}

## Class templates

# Namespace
	// using
	#include <iostream>
	using namespace std;
	namespace first	{
		int x = 5;
		int y = 10;
	}
	namespace second {
		double x = 3.1416;
		double y = 2.7183;
	}
	int main () {
		using namespace first;
		cout << x << endl;
		cout << y << endl;
		cout << second::x << endl;
		cout << second::y << endl;
		return 0;
	}

result

	5
	10
	3.1416
	2.7183
	
# Cast
## dynamic_cast
dynamic_cast can be used only with pointers and references to objects. Its purpose is to ensure that the result of
the type conversion is a valid complete object of the requested class.
Therefore, `dynamic_cast` is **always successful when we cast a class to one of its base classes:**

	class CBase { };
	class CDerived: public CBase { };
	CBase b; CBase* pb;
	CDerived d; CDerived* pd;
	pb = dynamic_cast<CBase*>(&d);
	pd = dynamic_cast<CDerived*>(&b);
	// ok: derived-to-base
	// wrong: base-to-derived

## static_cast
`static_cast` can perform conversions between pointers to related classes, not only from the derived class to its
base, but also from a base class to its derived. This ensures that at least the classes are compatible if the proper
object is converted, but no safety check is performed during runtime to check if the object being converted is in fact a full object of the destination type. 

	class CBase {};
	class CDerived: public CBase {};
	CBase * a = new CBase;
	CDerived * b = static_cast<CDerived*>(a);
This would be valid, although b would point to **an incomplete object** of the class and could lead to runtime errors if
dereferenced.

## typeid
typeid allows to check the type of an expression:  

string 和 string & 的typeid 是一样的
	
	typeid (expression)
This operator returns a reference to a constant object of type `type_info` that is defined in the standard header file
`<typeinfo>`. 

This returned value can be compared with another one using operators `== and !=` or can serve to
obtain a null-terminated character sequence representing the data type or class name by using its `name()` member.

	// typeid, polymorphic class
	#include <iostream>
	#include <typeinfo>
	#include <exception>
	using namespace std;

	class CBase { virtual void f(){} };
	class CDerived : public CBase {};
	
	int main () {
		try {
			CBase* a = new CBase;
			CBase* b = new CDerived;
			cout << "a is: " << typeid(a).name() << '\n';
			cout << "b is: " << typeid(b).name() << '\n';
			cout << "*a is: " << typeid(*a).name() << '\n';
			cout << "*b is: " << typeid(*b).name() << '\n';
		} 
		catch (exception& e) {
			cout << "Exception: " << e.what() << endl;
		}
		return 0;
	}

result

	a is: class CBase *
	b is: class CBase *
	*a is: class CBase
	*b is: class CDerived

Notice how the type that typeid considers for pointers is the pointer type itself (both a and b are of type class `CBase *`). However, when typeid is applied to objects (like `*a` and `*b`) typeid yields their dynamic type (i.e. the type of their most derived complete object).

# [C++的头文件和实现文件分别写什么](http://www.cnblogs.com/ider/archive/2011/06/30/what_is_in_cpp_header_and_implementation_file.html)

一个Package就是由同名的.h和.cpp文件组成.当然可以少其中任意一个文件:只有.h文件的Package可以是接口或模板(template)的定义,只有.cpp文件的Package可以是一个程序的入口.

我们在项目中以Package为编辑对象来扩展和修正我们的程序. 申明写在.h文件,定义实现写在.cpp文件.

<table border="1" cellspacing="0" cellpadding="5">
<thead>
<tr>
<td align="center"> &nbsp; </td>
<td align="center"> 非模板类型(none-template) </a> </td>
<td align="center"> 模板类型(template) </a> </td>
</tr>
</thead>
<tbody>
<tr>

<td rowspan="2" valign="top"> <a href="#header_file">头文件(.h)</a> </td>

<td valign="top">
<ul>
<li>全局变量申明(带extern限定符)</li>
<li>全局函数的申明</li>
<li>带<a href="#inline_qualifier">inline限定符</a>的全局函数的定义</li>
</ul>
</td>

<td>
<ul>
<li>带<a href="#inline_qualifier">inline限定符</a>的全局模板函数的申明和定义</li>
</ul>
</td>

</tr>

<tr>
<td valign="top">
<ul>
<li>类的定义</li>
<li>类函数成员和数据成员的申明(在类内部)</li>
<li>类定义内的函数定义(相当于inline)</li>
<li>带<a href="#static_const_qualifier">static const限定符</a>的数据成员在<strong><span class="accentuation">类内部</span></strong>的初始化</li>
<li>带<a href="#inline_qualifier">inline限定符</a>的类定义外的函数定义</li>
</ul>

</td>
<td valign="top">
<ul>
<li>模板类的定义</li>
<li>模板类成员的申明和定义(定义可以放在类内或者类外,类外不需要写inline)</li>
</ul>
</td>

</tr>

<tr>
<td rowspan="2" valign="top"> 实现文件(.cpp) </td>
<td valign="top">
<ul>
<li>全局变量的定义(及初始化)</li>
<li>全局函数的定义</li>
</ul>
</td>

<td rowspan="2"> (无) </td>
</tr>

<tr>
<td valign="top">
<ul>
<li>类函数成员的定义</li>
<li>类带static限定符的数据成员的初始化</li>
</ul>
</td>
</tr>
</tbody>
</table>

**头文件**

头文件的所有内容,都必须包含在

	#ifndef {Filename}
	#define {Filename} 
	
	//{Content of head file} 
	
	#endif
这样才能保证头文件被多个其他文件引用(include)时,内部的数据不会被多次定义而造成错误

**inline限定符**

在头文件中,可以对函数用inline限定符来告知编译器,这段函数非常的简单,可以直接嵌入到调用定义之处.

当然inline的函数并不一定会被编译器作为inline来实现,如果函数过于复杂,编译器也会拒绝inline.

因此简单说来,代码最好短到只有3-5行的才作为inline.有循环,分支,递归的函数都不要用做inline.

对于**在类定义内定义实现的函数,编译器自动当做有inline请求(也是不一定inline的)**.因此在下边,我把带有inline限定符的函数成员和写在类定义体内的函数成员统称为"要inline的函数成员"


## 非模板类型
### 全局类型
就像前面笼统的话讲的:申明写在.h文件.
**
对于函数来讲,没有实现体的函数,就相当于是申明,而对于数据类型(包括基本类型和自定义类型)来说,其申明就需要用extern来修饰.**

然后在.cpp文件里定义,实现或初始化这些全局函数和全局变量.

不过导师一直反复强调:不许使用全局函数和全局变量.当然不能用自有不能用的理由以及解决方案,不过不在目前的讨论范围内.

### 自定义类型
对于自定义类型,包括类(class)和结构体(struct),它们的定义都是放在.h文件中.其成员的申明和定义就比较复杂了,不过看上边的表格,还是比较清晰的.

#### 函数成员
函数成员无论是否带有static限定符,其申明都放在.h文件的类定义内部.

对于要inline的函数成员其定义放在.h文件,其他函数的实现都放在.cpp文件中.

#### 数据成员
数据成员的申明与定义都是放在.h文件的类定义内部.对于数据类型,关键问题是其初始化要放在什么地方进行.

- 对于**只含有static**限定符的数据成员,它的**初始化要放在.cpp文件**中.因为它是所有类对象共有的,因此必须对它做合适的初始化.

- 对于**只含有const**限定符的数据成员,它的初始化只能在构造函数的初始化列表中完成.因为它是一经初始化就不能重新赋值,因此它也必须进行合适的初始化.

- 对于**既含有static限定符,又含有const限定符**的数据成员,它的初始化和定义同时进行.它也是必须进行合适的初始化

- 对于**既没有static限定符,又没有const限定符**的数据成员,它的值只针对本对象可以随意修改,因此我们并不在意它的初始化什么时候进行.

## 模板类型
对于模板,最重要的一点,就是在定义它的时候,编译器并不会对它进行编译,因为它没有一个实体可用.

只有模板被具体化(specialization)之后(用在特定的类型上),编译器才会根据具体的类型对模板进行编译.

所以才定义模板的时候,会发现编译器基本不会报错(我当时还很开心的:我写代码尽然会没有错误,一气呵成),也做不出智能提示.但是当它被具体用在一个类上之后,错误就会大片大片的出现,却往往无法准确定位.

因此设计模板就有设计模板的一套思路和方式,但是这跟本文的主题也有偏.

因为模板的这种特殊性,它并没有自己的准确定义,因此我们不能把它放在.cpp文件中,而要把他们全部放在.h文件中进行书写.这也是为了在模板具体化的时候,能够让编译器可以找到模板的所有定义在哪里,以便真正的定义方法.

至于模板类函数成员的定义放在哪里,导师的意见是放在类定义之外,因为这样当你看类的时候,一目了然地知道有那些方法和数据,我在用Visual Studio的时候查看到其标准库的实现,都是放在类内部的.

可能是我习惯了C#的风格,我比较喜欢把它们都写在类内部,也因为在开发过程中,所使用的编辑器都有一个强大的功能:代码折叠.

当然还有其他原因就是写在类外部,对于每一个函数成员的实现都需要把模板类型作为限定符写一遍,把类名限定符也要写一遍.

# STL 标准模板库
- list封装了链表, 以链表形式实现的
- vector封装了数组, vector使用连续内存存储的,支持[]运算符; 对于新添加的元素,从尾部追加

## [vector](http://www.cplusplus.com/reference/vector/vector/)
在vector中,如果元素是对象的指针,当该vector用erase删除元素时, 元素本身在该vector种会被删除, 但是指针所指向的对象不会被删除

在调用push_back时,都要重新分配一个大一个元素的存储(古国不考虑处于性能考虑而预分配的内存空间),将要push_back的元素拷贝到新分配的内存中.

对于string 等object, 即使push_back中传入的参数是reference(别名) 类型, push到vector中的是一个完整的拷贝, 而不是一直指向原来的object 的指针, 
所以即使原来的object被删除了, vector中的仍然可以正常访问.

`std::vector::back`:  Returns a reference to the last element in the vector.

[vector push_back operation demo](../demo/c++/stl/vector-push_back.cpp)

### 内存的释放
由于vector的内存占用空间只增不减,比如你首先分配了10,000个字节,然后erase掉后面9,999个,留下一个有效元素,但是内存占用仍为10,000个(capacity 仍然很大).
所有内存空间是在vector析构时候才能被系统回收.  
empty()用来检测容器是否为空的.  
clear()可以清空所有元素.但是即使clear(),vector所占用的内存空间依然如故,无法保证内存的回收

如果需要空间动态缩小,可以考虑使用deque.  
如果vector,可以用swap()来帮助你释放内存.具体方法如下:

	vector<Point>().swap(pointVec); //或者pointVec.swap(vector<Point> ())

## [list](http://www.cplusplus.com/reference/list/list)

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
[basic demo](../demo/c++/boost/program_options.cpp)

## boost::any
Storing any value in a container/variable

	boost::any variable(std::string("hello world"));
	string s = boost::any_cast<std::string>(variable);

[boost::any demo](../demo/c++/boost/container-any.cpp)

[db_wrapper using boost::any](../demo/c++/boost/db-wrapper_any.cpp)

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

[boost::variant demo](../demo/c++/boost/variant.cpp)

[db_wrapper using boost::variant](../demo/c++/boost/db-wrapper_variant.cpp)

## boost::optional
Returning a value or flag where there is no value

[boost::variant demo](../demo/c++/boost/optional.cpp)

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

[boost::array demo](../demo/c++/boost/array.cpp)

## boost::tuple
[boost::tuple demo](../demo/c++/boost/tuple.cpp)

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
