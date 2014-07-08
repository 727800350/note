# IO
## File IO
### [ofstream](http://www.cplusplus.com/reference/fstream/ofstream/)
Output file stream  
ios_base <--- ios <--- ostream <--- ofstream

Objects of this class maintain a filebuf object as their internal stream buffer, which performs input/output operations on the file they are associated with.

File streams are associated with files either on construction, or by calling member open.

	explicit ofstream (const char* filename, ios_base::openmode mode = ios_base::out);

demo

	// ofstream constructor.
	#include <fstream>      // std::ofstream
	int main () {
	std::ofstream ofs ("test.txt", std::ofstream::out);
	ofs << "lorem ipsum";
	ofs.close();
	return 0;
	}


`cin >> mystring;`  
However, as it has been said, `cin` extraction stops reading as soon as if finds any blank space character, so in this
case we will be able to get just one word for each extraction.

In order to get entire lines, we can use the function `getline`

we can initialize the array of char elements called myword with a null-terminated sequence of characters
by either one of these two methods:

	char myword [] = { 'H', 'e', 'l', 'l', 'o', '\0' };
	char myword [] = "Hello";
In both cases the array of characters myword is declared with a size of 6 elements of type char: the 5 characters
that compose the word "Hello" plus a final null character ('\0') which specifies the end of the sequence and that,
in the second case, when using double quotes (") it is appended automatically.

**Pointers to functions**  

	// pointer to functions
	#include <iostream>
	using namespace std;

	int addition (int a, int b){ 
		return (a+b);
	}
	int subtraction (int a, int b){ 
		return (a-b);
	}
	int operation (int x, int y, int (*functocall)(int,int)){
		int g;
		g = (*functocall)(x,y);
		return (g);
	}
	
	int main (){
		int m,n;
		int (*minus)(int,int) = subtraction;
		m = operation (7, 5, addition);
		n = operation (20, m, minus);
		cout <<n;
		return 0;
	}

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

- *private* members of a class are accessible only from within other members of the same class or from their friends.
- *protected* members are accessible from members of their same class and from their friends, but also from members of their derived classes.
- *public* members are accessible from anywhere where the object is visible.

**By default, all members of a class are private.**

	// classes example
	#include <iostream>
	using namespace std;
	class CRectangle {
		int x, y;
	public:
		CRectangle (int,int);
		~CRectangle ();
		void set_values (int,int);
		int area () {return (x*y);}
	};
	
	void CRectangle::set_values (int a, int b) {
		x = a;
		y = b;
	}

	int main () {
		CRectangle rect;
		rect.set_values (3,4);
		cout << "area: " << rect.area();
		return 0;
	}

**Constructors**  
Objects generally need to initialize variables or assign dynamic memory during their process of creation to become
operative and to avoid returning unexpected values during their execution.

Like any other function, a constructor can also be overloaded with more than one function that have the same
name but different types or number of parameters.

The use of destructors is especially suitable when an object assigns dynamic memory during its lifetime and at the
moment of being destroyed we want to release the memory that the object was allocated.

	// example on constructors and destructors
	#include <iostream>
	using namespace std;
	
	class CRectangle {
		int *width, *height;
	public:
		CRectangle (int,int);
		~CRectangle ();
		int area () {return (*width * *height);}
	};

	CRectangle::CRectangle (int a, int b) {
		width = new int;
		height = new int;
		*width = a;
		*height = b;
	}

	CRectangle::~CRectangle () {
		delete width;
		delete height;
	}

	int main () {
		CRectangle rect (3,4), rectb (5,6);
		cout << "rect area: " << rect.area() << endl;
		cout << "rectb area: " << rectb.area() << endl;
		return 0;
	}

## Overloading operators
	// vectors: overloading operators example
	#include <iostream>
	using namespace std;
	
	class CVector {
	public:
		int x,y;
		CVector () {};
		CVector (int,int);
		CVector operator + (CVector);
	};

	CVector::CVector (int a, int b) {
		x = a;
		y = b;
	}

	CVector CVector::operator+ (CVector param) {
		CVector temp;
		temp.x = x + param.x;
		temp.y = y + param.y;
		return (temp);
	}

	int main () {
		CVector a (3,1);
		CVector b (1,2);
		CVector c;
		c = a + b;
		cout << c.x << "," << c.y;
		return 0;
	}

Usage

	c = a + b;
	c = a.operator+ (b);
Both expressions are equivalent.

**keyword this**  
The keyword this represents a pointer to the object whose member function is being executed. It is a pointer to
the object itself.

**Static members**  
A class can contain static members, either data or functions.

Static data members of a class are also known as "class variables", because there is **only one unique value for all
the objects of that same class**. Their content is not different from one object of this class to another.

For example, it may be used for a variable within a class that can contain a counter with the number of objects of
that class that are currently allocated.

## Friendship and inheritance
### Friend functions
In principle, private and protected members of a class cannot be accessed from outside the same class in which
they are declared. However, this rule does not affect friends.

	// friend functions
	#include <iostream>
	using namespace std;

	class CRectangle {
		int width, height;
	public:
		void set_values (int, int);
		int area () {return (width * height);}
		friend CRectangle duplicate (CRectangle);
	};

	void CRectangle::set_values (int a, int b) {
		width = a;
		height = b;
	}

	CRectangle duplicate (CRectangle rectparam)	{
		CRectangle rectres;
		rectres.width = rectparam.width*2;
		rectres.height = rectparam.height*2;
		return (rectres);
	}

	int main () {
		CRectangle rect, rectb;
		rect.set_values (2,3);
		rectb = duplicate (rect);
		cout << rectb.area();
		return 0;
	}

### Friend classes
Just as we have the possibility to define a friend function, we can also define a class as friend of another one,
granting that first class access to the protected and private members of the second one.

	// friend class
	#include <iostream>
	using namespace std;
	
	// make class csquare visible to crectangle
	class CSquare;
	
	class CRectangle {
		int width, height;
	public:
		int area ()	{return (width * height);}
		void convert (CSquare a);
	};
	
	class CSquare {
	private:
		int side;
	public:
		void set_side (int a) {side=a;}
		friend class CRectangle;
	};
	
	void CRectangle::convert (CSquare a) {
		width = a.side;
		height = a.side;
	}

	int main () {
		CSquare sqr;
		CRectangle rect;
		sqr.set_side(4);
		rect.convert(sqr);
		cout << rect.area();
		return 0;
	}

In this example, we have declared CRectangle as a friend of `CSquare` so that CRectangle member functions could
have access to the protected and private members of `CSquare`, more concretely to `CSquare::side`, which describes the side width of the square.

### Inheritance between classes
Classes that are derived from others inherit all the accessible members of the base class. That means that if a base
class includes a member A and we derive it to another class with another member called B, the derived class will
contain both members A and B.

`class derived_class_name: public base_class_name`

The public access specifier may be replaced by any one of the other access specifiers protected
and private.

权限的分配

In principle, a derived class inherits every member of a base class except:

- its constructor and its destructor
- its operator=() members
- its friends

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

# boost
安装

	tar -xzvf boost_1_54_0.tar.gz
	cd boost_1_54_0
	./bootstrap.sh --prefix=/usr/local
	sudo ./b2 install --with=all

## program_options
可以`parse_command_line, parse_config_file, parse_environment`

### Basic option configuration
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
