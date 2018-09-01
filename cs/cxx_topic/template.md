# Intro
[C++ TUTORIAL - TEMPLATES - 2018](http://www.bogotobogo.com/cplusplus/templates.php)

The process of generating types from a given template arguments is called **specialization** or **template instantiation**.
For example, `std::vector<int>` is said to be a specialization of `std::vector`.

Templates are the foundation of generic programming. Generic programming relies on polymorphism.
Though there are several differences between OOP (class hierarchies and virtual functions) and generic programming (templates), the major difference is:

- Generic (templates) : compile time resolution. The choice of function invoked when we use is determined by the compiler at compile time.
- OOP (virtual functions) : run time resolution.

When we parameterize a class, we get a class template, and when we parameterize a function, we get a function template.

So, what do people actually use template for?

1. When performance is essential.
1. When flexibility in combining information from several types is essential.

But note that, the flexibility and performance come at the cost of poor error diagnostics and poor error messages.

```C++
template <typename T>
int compare(const T &v1, const T &v2){
	if(v1 < v2) return -1;
	if(v2 < v1) return 1;
	return 0;
}

Sales_data data1, data2;
cout << compare(data1, data2) << endl;
```
If class Sales does not overload `operator<`, compilation would fail.
Errors such as this one cannot be detected until the compiler instantiates the definition of compare on type `Sales_data`.

## Terminology of Templates
- Instantiation
	This is when the compiler generates a regular class, method, or function by substituting each of the template's parameters with a concrete type.
	This can happen implicitly when we create an object based on a template or explicitly if we want to control when the code generation happens.
	For instance, the following code creates two specific stack instances and will normally cause the compiler to generate code for these two different types:

	```
	Stack<int> myIntStack;
	Stack<std::string> myStringStack;
	```

- Implicit Instantiation
	This is when the compiler decides when to generate code for our template instances.
	Leaving the decision to the compiler means that it must find an appropriate place to insert the code,
	and it must also make sure that only one instance of the code exists to avoid duplicate symbol errors.
	This is non-trivial problem and can cause extra bloat in our object files or longer compile time.
	Most importantly, **implicit instantiation means that we have to include the template definitions in our header files
	so that the compiler has an access to the definitions whenever it needs to generate the instantiation code**.

- Explicit Instantiation
	This is when the programmer determine when the compiler should generate the code for a specific specialization.
	This can make for much more efficient compilation and link times because the compiler no longer needs to maintain bookkeeping information for all of its implicit instantiations.
	The onus, however, is then placed on the programmer to ensure that a particular specialization is explicitly instantiated only once.
	So, explicit instantiation allows us to move the template implementation into the .cpp file, and so hide from the user.

- Lazy Instantiation
	This describes **the standard implicit instantation behavior of a C++ compiler wherein it will only generate code for the parts of a template that are actually used**.
	Given the previous two instantiations, for example, if we never called `isEmpty()` on the myStringStack object,
	then the compiler would not generate code for the std::string specialization of that method.
	This means that we can instantiate a template with a type that can be used by some, but not all methods of a class template.
	If one method uses the >= operator, but the type we want to instantiate does not define this operator.
	This is fine as long as we don't call the particular method that attempts to use the `>=` operator.

从解释上来看, 我们的GCC 编译器采用的是 Lazy Instantiation

# Function Templates
```C++
template <typename T>
const T& min(const T& a, const T& b) {
	return a < b ? a : b;
}

LOG(INFO) << min<int>(100, 200);
LOG(INFO) << min(100, 200); // compiler will get the type information from the arguments
```

A function template can be declared inline in the same way as a nontemplate function.
```C++
template <typename T>
inline const T& min(const T& a, const T& b) {
```

# Class Templates
Implementation and the declaration of a class template should be [in the same header file](http://www.bogotobogo.com/cplusplus/template_declaration_definition_header_implementation_file.php).

```C++
// template declaration
template <typename T>
class Complx{
public:
	Complx(T&, T&);
	const T& getReal() const;
	const T& getImag() const;
private:
	T real;
	T imag;
};

// template definition
template <typename T>
Complx<T>::Complx(T& a, T& b){
	real = a;
	imag = b;
}

template <class T>
const T& Complx<T>::getReal() const{
	return real;
}

template <class T>
const T& Complx<T>::getImag() const{
	return imag;
}

Complx<double> myComplx(1.0, 2.5);
```

# Nontype Template Parameters
A template nontype parameter is a constant value inside the template definition.
A nontype parameter can be used when constant expressions are required, for example, to specify the size of an array.

For the standard class `bitset<>`, we can pass the number of bits as the template argument.
```C++
bitset<32> b32;		// bitset with 32 bits
bitset<64> b64;		// bitset with 64 bits
```
The bitsets have different types because they use different template arguments. So, we can't assign or compare them.

```C++
template <class T, unsigned int N>
void arrayInit(T (&a)[N]){
	for(unsigned int i = 0; i < N; ++i){
		a[i] = 0;
	}
}

int main(){
	int i[10];
	double d[20];
	arrayInit(i);	// arrayInit(int(&)[10])
	arrayInit(d);	// arrayInit(double (&)[20])
}
```
`arrayInit` is a function template which takes a single parameter, which is a reference to an array.

# [Template Specialization](http://www.bogotobogo.com/cplusplus/template_specialization_function_class.php)
It is not always possible to write a single template that is for every possible template argument with which the template might be instantiated.
Because general definition might not compile or might do the wrong thing.
We may be able to take advantage of some specific knowledge about a type to write a more efficient function than the one that is instantiated from the template.

## Function Specialization
In the example below, we have `add()` function which takes two parameter and returns the same type of data after adding the two args.
```C++
template <typename T>
T add(T x, T y){
	cout << "Normal template\n";
	return x + y;
}

// specialized function
template <>
char add<char>(char x, char y){
	cout << "Specialized template\n";
	int i = x - '0';
	int j = y - '0';
	return i + j;
}

LOG(INFO) << add(1, 2);
LOG(INFO) << add('a', 'b');
```
When we designed the function `add(T x, T y)`, the meaning was clear: add the two numbers.
But when the user feeds characters into the parameter, the meaning is not obvious.
So, if the intention of the design is different from the initial one, we may want to redefine the operation in a separate template. In other words, we do specialize the function.

## Class Specialization
```C++
// Normal class
template <typename T>
class A{
public:
	A(){ cout << "A()\n"; }
	static T add(T x, T y);
};

template <typename T>
T A<T>::add(T x, T y){
	return x + y;
}

// Specialized class
template <>
class A <char>{
public:
	A() { cout << "Special A()\n"; }
	static char add(char x, char y);
};

// template <>   <= this is not needed if defined outside of class, do not know why
char A<char>::add(char x, char y){
	int i = x - '0';
	int j = y - '0';
	return i + j;
}

LOG(INFO) << A<int>::add(1, 2);
LOG(INFO) << A<double>::add(1.2, 3.0);
LOG(INFO) << A<char>::add('a', 'b');
```

Notice the differences between the generic class template and the specialization:

- `template <typename T> class A { ... };`: generic template
- `template <> class A <char> { ... };`: specialization

When we declare specializations for a template class, we must also define all its members, even those exactly equal to the generic template class,
because there is no inheritance of members from the generic template to the specialization.

