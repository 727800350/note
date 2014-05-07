# IO
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

# Class
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
