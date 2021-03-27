- [Introduction](#introduction)
- [deduce types](#deduce-types)
  - [template type deduction](#template-type-deduction)
    - [case 1: ParamType is a reference or pointer, but not a universal reference](#case-1-paramtype-is-a-reference-or-pointer-but-not-a-universal-reference)
    - [case 2: ParamType is a universal reference](#case-2-paramtype-is-a-universal-reference)
    - [case 3: ParamType is neither a pointer nor a reference](#case-3-paramtype-is-neither-a-pointer-nor-a-reference)
    - [array arguments](#array-arguments)
    - [function arguments](#function-arguments)
  - [auto](#auto)
  - [decltype](#decltype)
  - [know how to view deduced types](#know-how-to-view-deduced-types)
    - [compiler diagnostics](#compiler-diagnostics)
    - [runtime output](#runtime-output)
- [moving to modern C++](#moving-to-modern-c)
  - [distinguish between () and {} when creating objects](#distinguish-between--and--when-creating-objects)
  - [prefer alias declarations to typedefs](#prefer-alias-declarations-to-typedefs)
  - [declare functions noexcept if they won't emit exceptions](#declare-functions-noexcept-if-they-wont-emit-exceptions)
  - [use constexpr whenever possible](#use-constexpr-whenever-possible)
- [class](#class)
  - [perfer deleted functions to private undefined ones](#perfer-deleted-functions-to-private-undefined-ones)
  - [make const member functions thread safe](#make-const-member-functions-thread-safe)
  - [understand special member function generation](#understand-special-member-function-generation)
- [smart pointer](#smart-pointer)
  - [use `std::shared_ptr` for shared-ownership resource management](#use-stdshared_ptr-for-shared-ownership-resource-management)
  - [use `std::weak_ptr` for `std::shared_ptr` like pointers that can dangle](#use-stdweak_ptr-for-stdshared_ptr-like-pointers-that-can-dangle)
    - [scenario 1: cache](#scenario-1-cache)
    - [scenario 2: observer design pattern](#scenario-2-observer-design-pattern)
    - [scenario 3: `std::shared_ptr` cycle](#scenario-3-stdshared_ptr-cycle)
  - [prefer `std::make_unique` and `std::make_shared` to direct use of new](#prefer-stdmake_unique-and-stdmake_shared-to-direct-use-of-new)
- [std::move and std::forward](#stdmove-and-stdforward)
  - [std::move](#stdmove)
  - [std::forward](#stdforward)
    - [reference collapsing](#reference-collapsing)
  - [distinguish universal references from rvalue references](#distinguish-universal-references-from-rvalue-references)
  - [use std::move on rvalue references, std::forward on universal references](#use-stdmove-on-rvalue-references-stdforward-on-universal-references)
  - [avoid overloading on universal references](#avoid-overloading-on-universal-references)
    - [case 1](#case-1)
    - [case 2](#case-2)
    - [case 3](#case-3)
    - [case 4](#case-4)
  - [familiarize yourself with alternatives to overloading on universal references](#familiarize-yourself-with-alternatives-to-overloading-on-universal-references)
- [Lambda Expressions](#lambda-expressions)
  - [use init capture to move objects into closures](#use-init-capture-to-move-objects-into-closures)
  - [use decltype on auto&& parameters to std::forward them](#use-decltype-on-auto-parameters-to-stdforward-them)
  - [prefer lambdas to std::bind](#prefer-lambdas-to-stdbind)
- [The Concurrency API](#the-concurrency-api)
  - [use std::atomic for concurrency, volatile for special memory](#use-stdatomic-for-concurrency-volatile-for-special-memory)

# Introduction
In concept(though not always in practice), rvalues correspond to temporary objects returned from functions,
while lvalues correspond to objects you can refer to, either by name or by following a pointer or lvalue reference.

A useful heuristic to determine whether an expression is an lvalue is to ask if you can take its address.
If you can, it typically is. If you can't, it's usually an rvalue.

**In a function call, the expressions passed at the call site are the functions's arguemnts. The arguemnts are used to
initialize the function's parameters**.
```C++
void someFunc(Widget w);
Widget wid;
someFunc(wid);
someFUnc(std::move(wid));
```
In the first call to someFunc, the argument is wid. In the second call, the argument is std::move(wid).
In both calls, the parameter is w.

The distinction between arguments and parameters is important, because parameters are lvalues, but the arguemnts may be
rvalues or lvalues.
This is especially relevant during the process of perfect forwarding, whereby an argument passed to a function is passed
to a second function call such that
the original argument's rvalueness or lvalueness is preserved.

Well-designed functions are exception safe, meaning they offer at least the basic exception safety guarantee (i.e., the
basic guarantee).

- basic guarantee: assure calless that even if an exception is thrown, program invariants remain intact (i.e., no data
  structures are corrupted) and no resources are leaked.
- strong guarantee: assure callers that if an exception arises, the state of the program remains as it was priori to the
  call.

# deduce types
## template type deduction
If you are willing to overlook a pinch of pseudocode, we can think of a function template as looking like this:
```C++
template <typename T>
void f(ParamType param);
```
A call can look like this:
```C++
f(expr);  // call f with some expression
```
During compilation, compilers use expr to deduce two types: one for T and one for ParamType.
These types are frequently different, because ParamType often contains adornments, e.g., const or reference qualifiers.

### case 1: ParamType is a reference or pointer, but not a universal reference
In this case, type deduction works like this:

1. If expr's type is a reference, ignore the reference part.
1. Then pattern-match expr's type against ParamType to determine T.

```C++
template<typename T>
void f(T& param);  // param is a reference

int x = 27;
const int cx = x;
const int& rx = x;  // rx is reference to x as a const int

f(x);  // T is int, param's type is int&
f(cx);  // T is const int, param's type is const int&
f(rx);  // T is const int, param's type is const int&
```

```C++
template<typename T>
void f(const T& param);  // param is now a ref-to-const

int x = 27;
const int cx = x;
const int& rx = x;

f(x);  // T is int, param's type is const int&
f(cx);  // T is int, param's type is const int&
f(rx):  // T is int, param's type is const int&
```

If parm were a pointer (or a pointer to const) instread of a reference, things would work essentially the same way.
```C++
template<typename T>
void f(T* param);

int x = 27;
const int* px = &x;

f(&x);  // T is int, param's type is int*
f(px);  // T is const int, param's type is const int*
```

### case 2: ParamType is a universal reference
- If expr is an lvalue, both T and ParamType are deduced to be lvalue references. That's doubly unusual.
  1. It's the only situation in template tpe deduction where T is deduced to be a reference.
  1. Although ParamType is declared using the syntax for an rvalue reference, its deduced type is an lvalue reference.
- If expr is an rvalue, case 1 rules apply.

```C++
template<typename T>
void f(T&& param);  // param is now universal reference

int x = 27;
const int cx = x;
const int& rx = x;

f(x);  // x is lvalue, so T is int&, param's type is also int&
f(cx);  // cx is lvalue, so T is const int&, param's type is also const int&
f(rx);  // rx is lvalue, so T is const int&, param's type is also const int&
f(27);  // 27 is rvalue, so T is int, param's type is therefore int&&.
        // 27 is of type int&&, we first ignore the reference part, and we got int, so T is int, and param's type is int&&
```

### case 3: ParamType is neither a pointer nor a reference
```C++
template<typename T>
void f(T param);  // param is now passed by value
```
That means that **param will be a copy of whatever is passed in - a completely new object**.
The fact that param will be a new object motivates the rules that govern how T is deduced from expr.

1. As before, if expr's type is a reference, ignore the reference part.
1. If, after ignoring expr's reference-ness, expr is const, ignore that, too. If it's volatile, also ignore that.

```C++
int x = 27;
const int cx = x;
const int& rx = x;

f(x);  // T's and param's types are both int
f(cx);  // T's and param's types are again both int
f(rx);  // T's and param's types are still both int
```

It is important to recognize that const (and volatile) is ingored only for by-value-parameters.
As we've seen, for parameters that are references-to- or pointers-to-const, the constness of expr is preserved during type deduction.
```C++
const char* const ptr = "Fun with pointers";  // ptr is const pointer to const object

f(ptr)  // pass arg of type const char* const
```
Here, the right const declares ptr to be const, meaning ptr can not be made to point to a different location, nor can it be set to null.
The left const says that what ptr points to - the character string is cont, hence can not be modified.
When ptr is passed to f, the bits making up the pointer are copied into param.
In accord with the type deduction rule, the constness of ptr will be ignored, and the type deduced for param will be `const char*`, i.e., a modifiable pointer to a const character string.
The constness of what ptr points to is preserved during type deduction.

### array arguments
In many contexts, an array decays into a pointer to its first element. This decay is what permits code like this to compile:
```C++
const char name[] = "J. P. Briggs";  // name's type is const char[13]
const char* ptrToName = name;  // array decays to pointer
```
These types (`const char*` and `const char[13]`) are not the same, but because of the array-to-pointer decay rule, the code compilers.

```C++
template<typename T>
void f(T param);

f(name);  // T is deduced as const char*
```
Because array parameter declarations are treated as if they were pointer parameters, the type of the array that is passed to a template function by value is deduced to be a pointer type.
That means that in the call f(name), its type parameter T is deduct to be `const char*`.

Althouth functions can not declare parameters that are truly arrays, they can declare parameters that are references to arrays!
```C++
template<typename T>
void f(T& param);

f(name);  // T is deduced as const char[13], param's type is const char(&)[13]
```

And interestingly, the ability to declare references to arrays enables creation of a template that deduces the number of elements that an array contains:
```C++
// constexpr makes its result available during compilation.
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N]) noexcept {
  return N;
}

int keyVals[] = {1, 3, 5};
int mappedVals[arraySize(keyVals)];
```

### function arguments
Arrays are not the only things in C++ that can decay into pointers.
Function types can decay into function pointers, and everything we've discussed regarding type deduction for array applies to type deduction for functions.
```C++
void someFunc(int, double);  // someFunc's type is void(int, double)

template<typename T>
void f1(T param);

template<typename T>
void f2(T& param);

f1(someFunc);  // param deduced as ptr-to-func, type is void (*)(int, double)
f2(someFunc);  // param deduced as ref-to-func, type is void (&)(int, double)
```

## auto
With only one curious exception, auto type deduction is template type deduction.
There is a direct mapping between template type deduction and auto type deduction.
When a variable is declared using auto, auto plays the role of T in template, and type specifier for the variable acts as ParamType.
So there are also three cases.

- case 1: the type specifier is a pointer or reference, but not a universal reference.
- case 2: the type specifier is a universal reference.
- case 3: the type specifier is neither a pointer nor a reference.

```C++
auto x = 27;  // type specifier for x is auto, case 3
const auto cx = x;  // type specifier for x is const auto, case 3
const auto& rx = x;  // type specifier for x is const auto&, case 1

auto&& uref1 = x;  // x is int and lvalue, so uref1's type is int&
auto&& uref2 = cx;  // cx is const int and lvalue, so uref1's type is const int&
auto&& uref3 = 27;  // 27 is int and rvalue, so uref3's type is int&&

const char name[] = "R. N. Briggs";
auto arr1 = name;  // arr1's type is const char*
auto& arr2 = name;  // arr2's type is const char (&)[13]

void someFunc(int, double);
auto func1 = someFunc;  // func1's type is void(*)(int, double)
auto& func2 = someFunc;  // func2's type is void(&)(int, double)
```

Now let's talk about the one special type deduction for auto.
When the initializer for an auto-declared variable is enclosed in braces, the deduced type is `std::initializer_list`.
```C++
auto x = {11, 23, 9};  // x's type is std::initializer_list<int>

template<typename T>
void f_error(T& param);
f_error({11, 23, 9});  // error, can not deduce type for T

template<typename T>
void f_succ(std::initializer_list<T> param);
f_succ({11, 23, 9});  // T deduced as int, and param's type is std::initializer_list<int>
```

## decltype
Given a name or an expression, decltype tells you the name's or the expression's type.
```C++
int i = 33;
decltype(i) j = i * 2;

auto f = [](int a, int b){return a * b;};
decltype(f) g = f;  // the type of a lambda function is unique and unnamed
```

In C++11, the primary use for decltype is declaring function templates where the functions's return type depends on its paramter types.
For example, suppose we'd like to write a function that takes a container that supports indexing via [] plus an index, then authenticates the user before returning the result.
`oprator[]` on a container of objects of type T typically returns a `T&`.

```C++
// works since C++11, but requires refinement
template<typename Container, typename Index>
auto authAndAccess(Container& c, index i) -> decltype(c[i]) {
  authenticateUser();
  return c[i];
}
```
The use of auto before the function name has nothing to do with the type deduction.
Rather, it indicates that C++11's trailing return type syntax is being used, i.e., that the function's return type will be declared following the parameter list(after the "->").
A trailing return type has the advantage that the function's paramters can be used in the specification of the return type.

```C++
// C++14, works but still requires refinement
template<typename Conatainer, typename Index>
decltype(auto) authAndAccess(Container& c, Index i) {
  authenticateUser();
  return c[i];
}
```
auto sepecifies that the type is to be deduced, and decltype says that decltype rule should be used during the deduction.

The use of `decltype(auto)` is not limited to function return types. It can be also be convenient for declaring variables when you want to apply the decltype type deduction rules.
```C++
Widget w;
const Widget& cw = w;
auto myWidget1 = cw;  // auto type deduction, myWidget1's type is Widget
decltype(auto) myWidget2 = cw;  // decltype type deduction, myWidget2's type is const Widget
```

Look agagin at the declarition for the C++14 version of authAndAccess:
```C++
template<typename Conatainer, typename Index>
decltype(auto) authAndAccess(Container& c, Index i);
```
The container is passed by lvalue-reference-to-non-const, because returning a reference to an element of the container permits clients to modify that container.
But this also means it's not possible to pass rvalue containers to this function.
Admittedly, passing an rvalue container to authAndAccess is an edge case.
An rvalue container, being a temporary object, would typically be destroyed at the end of the statement.
Still, it could make sense to pass a temporary object to authAndAccess. A client might simpy want to make a copy of an element in the temporary container, for example:
```C++
std::deque<std::string> makeStringDeque();  // factory function
// make copy of 5th element of deque returned from makeStringDeque
auto s = authAndAccess(makeStringDeque(), 5);
```

```C++
// final C++14 version
template<typename Container, typename Index>
decltype(auto) authAndAccess(Container&& c, Index i) {
  authenticateUser();
  return std::forward<Container>(c)[i];
}

// final C++11 version
template<typename Container, typename Index>
auto authAndAccess(Container&& c, Index i) -> decltype(std::forward<Container>(c)[i]) {
  authenticateUser();
  return std::forward<Container>(c)[i];
}
```
## know how to view deduced types
### compiler diagnostics
An effective way to get a compiler to show a type it has deduced is to use that type in a way that leads to compilation problems.
The error message reporting the problem is virtually sure to mention the type that's causing it.
```C++
template<typename T>
class TD;  // declaration only for TD(Type Displayer)

const int value = 42;
auto x = value;
auto y = &value;
TD<decltype(x)> xType;
TD<decltype(y)> yType;
```
Attempts to instantiate this template will elicit an error message, because there's no template definition to instantiate.
```C++
error: aggregate 'wcg::TD<int> xType' has incomplete type and cannot be defined
error: aggregate 'wcg::TD<const int*> yType' has incomplete type and cannot be defined
```

### runtime output
Use the Boost TypeIndex library.
See example `code/thirdparty/boost/type_index_test.cc`

# moving to modern C++
## distinguish between () and {} when creating objects
C++98 has no way to directly indicate that an STL container should be created holding a particular set of values.

C++11 introduces **uniform initialization**: a single initialization syntax that can, at least in concept, be used anywhere and express everything.
It's based on braces, and for that reason, I prefer the term **braced initialization**.

A novel feature of braced initialization is that is prohibits implicit narrowing conversions among built-in types.
```C++
double x, y, z;
int sum1(x + y + z);  // ok, value of expression truncated to an int
int sum2{x + y + z};  // error, sum of doubles may not be expressible as int
```

Another noteworthy characteristic of braced initialization is its immunity to C++'s most vexing parse.
A side effect of C++'s rule that **anything that can be parsed as a declaration must be interpreted as one**.
The most vexing parse most frequently afflicts developers when they want to default-construct an object, but in inadvertently end up declaring a function instead.

The root of the problem is that if you want to call a constructor with an argument, you can do it as usual.
But if you want to **call a Widget ctor with zero arguments using the analogous syntax, you declare a function instead of an object**:
```C++
Widget w1(10);  // call Widget ctor with argument 10
Widget w2();  // most vexing parse! declares a function named w2 that returns a Widget
Widget w3{};  // calls Widget ctor with no args
```

During ctor overload resolution, braced initializers are matched to `std::initializer_list` parameters if at all possible, even if other ctors offer seemingly better matches.

## prefer alias declarations to typedefs
Alias declarations may be templaized (in which case they're called alias templates), while typedefs cannot.
For example, consider defining a synonym for a linked list that uses a custom allocator, MyAlloc
```C++
// alias declaration
// MyAllocList<T> is synonym for std::list<T, MyAlloc<T>>
template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;

MyAllocList<Widget> lw;  // client code
```

With a typedef, you pretty much have to create the cake from sratch:
```C++
// typedef declaration
// MyAllocList<T>::type is synonym for std::list<T, MyAlloc<T>>
template<typename T>
struct MyAllocList {
  typedef std::list<T, MyAlloc<T>> type;
};

MyAllocList<Widget>::type lw;  // client code
```

It gets worse. If you want to use the typedef inside a template for the purpose of creating a linked list holding objects of a type specifiied by a template parameter,
you have to precede the typedef name with typename:
```C++
template<typename T>
class Widget {
 private:
  typename MyAllocList<T>::type list;  // Widget<T> contains a MyAllocList<T> as a data member
};
```
Here, `MyAllocList<T>::type` refers to a type that is dependent on a template type parameter T.
`MyAllocList<T>::type` is thus a dependent type, and one of C++'s many endearing rulels is that **the name of dependent types must be preceded by typename**.

If MyAllocList is defined as an alias template, this need for typenmae vanishes:
```C++
template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;

template<typename T>
class Widget {
 private:
  MyAllocList<T> list;  // no typename and no ::type
};
```
To you, `MyAllocList<T>` may look just as dependent on the template parameter T.
But when compilers process the Widget template and encounter the use of `MyAllocList<T>`, they know that `MyAllocList<T>` is the name of a type, because it is an alias template: it must name a type.
`MyAllocList<T>` is thus a non-dependent type, and a typename specifier is neither required nor permitted.

When compilers see `MyAllocList<T>::type` (i.e., use of the nested typedef) in the Widget template, on the other hand, they can not know for sure that it names a type.
Because there might be a specialization of MyAllocList:
```C++
class Wine {};

tempalte<>
class MyAllocList<Wine> {
 private:
  enum class WineType { White, Red, Rose};
  WineType type;  // in this class, type is a data member
};
```
That's why compilers insist on your asserting that it is a type by preceding it with typename.

C++11 gives you the tools to perform some kinds of transformations in the form of type traits, defined in the header `<type_traits>`.
```C++
std::remove_const<T>::type  // yields T from const T
std::remove_reference<T>::type  // yields T from T& and T&&
std::add_lvalue_reference<T>::type  // yields T& from T
```
If you apply these type traits to a type parameter inside a template, you'd also have to precede each use with typename.
The reason is that C++ type traits are implemented as nested typedefs inside templatized structs. That's bad.

So in C++14, alias implementations are included
```C++
std::remove_const<T>::type  // C++11: const T -> T
std::remove_const_t<T>  // C++11 equivalent

std::remove_reference<T>::type  // C++11: T&/T&& -> T
std::remove_reference_t<T>  // C++14 equivalent

std::add_lvalue_reference<T>::type  // C++11: T -> T&
std::add_lvalue_reference_t<T>  // C++14 equivalent
```

## declare functions noexcept if they won't emit exceptions
By declaring a function, a method, or a lambda-function as noexcept, you specify that these won't throw an exception and
if they throw, you do not care and let the program just crash.

1. move operations (move assignment operator and move constructors)
1. swap operations
1. memory deallocators (operator delete, operator delete[])
1. destructors (though these are implicitly noexcept(true) unless you make them noexcept(false))

These functions should generally be noexcept, and it is most likely that library implementations can make use of the
noexcept property.
For example, std::vector can use non-throwing move operations without sacrificing strong exception guarantees.
Otherwise, it will have to fall back to copying elements (as it did in C++98).

This kind of optimization is on the algorithmic level and does not rely on compiler optimizations.
It can have a significant impact, especially if the elements are expensive to copy.

## use constexpr whenever possible
Conceptually, constexpr indicates a value that's not only constant, it's known during compilation.
```C++
{
  constexpr int size = 10;
  std::array<int, size> data;  // ok
}

{
  const int size = 10;
  std::array<int, size> data;  // ok
}
```
Simply put, all constexpr objects are const, but not all const objects are constexpr.

constexpr functionss produce compile-time constants when they are called with compile-time constants.
If they're called with values not known until runtime, they produce runtime values.
```C++
constexpr int foo(int i) noexcept {
    return i + 5;
}

std::array<int, foo(5)> arr;  // ok

int i = 10;
foo(i); // Call is Ok
    
std::array<int, foo(i)> arr1;  // error
```
It means that the traditionally fairly strict line between work done during compilation and work done at runtime begins
to blur, and some computations traditionally done at runtime can migrate to compile time.
The more code taking part in the migration, the faster your software will run, compilation may take longer, however.

# class
## perfer deleted functions to private undefined ones
An important adavantage of deleted functions is that any function may be deleted, while only member functions may be
private.
For example, suppose we have a non-member function that takes an integer and returns whether it's a lucky number:
```C++
bool isLucky(int number);
bool isLucky(char) = delete;  // reject char
bool isLucky(bool) = delete;  // reject bool
bool isLucky(double) = delete;  // reject double and float
```

Another trick that deleted functions can perform is to prevent use of template instantiations that should be disabled.
For example, suppose you need a template that works with built-in pointers.
```C++
template<typename T>
void processPointer(T* ptr);

template<>
void processPointer<void>(void*) = delete;

template<>
void processPointer<char>(char*) = delete;

template<>
void processPointer<const void>(const void*) = delete;

template<>
void processPointer<const char>(const char*) = delete;
```

Interestingly, if you have a function template inside a class, and you'd like to disable some instantiations by
declaring them private(in C++98 style), you can't.
Luckily, this issue doesn't arise for deleted functions, and they can also be deleted outside the class.
```C++
class Widget {
 public:
  template<typename T>
  void processPointer(T* ptr) {}

  template<>
  void processPointer<void>(void*) = delete;
};

template<>
void Widget::processPointer<char>(char*) = delete;  // still public and deleted
```

## make const member functions thread safe
const 成员函数还是有可能修改内部状态, 比如使用mutable 修饰的成员变量.
That means that there will be different threads reading and writing the same memory without synchronization, and that's
the definition of a data race. The function has undefined behavior.
需要使用mutex 来保护.

## understand special member function generation
The two copy operations(copy ctor and copy assignment) are independent: declaring one doesn't prevent compilers from
generating the other.

TODO: why copy operations 之前不相互影响, 而move operations 却要受限制.

The two move operations(move ctor and move assignment) are not independent. If you declare either, that prevents
compilers from generating the other.
The rationale is that if you declare, say, a move ctor, you're indicating that the move ctor is different from the
default memberwise move that compilers would generate.
And if there's something wrong with memberwise move construnction, there'd probably be something wrong with memberwise
move assignment, too.

Futhermore, move operations won't be generated for any class that explicitly declares a copy operation.
The justification is that declaring a copy operation (constructor or assignment) indicates that the normal approach to
copying an object(memberwise copy) isn't appropriate for the class,
and compilers figure that if memberwise copy isn't appropriate for the copy operations, memberwise move probably isn't
appropriate for the move operations.

C++11 does not generate move operations for a class with a user-declared destructor.

Note: `=default` and `=delete` count as user-defined.

Declare a move operation cause compilers to disable the copy operations. (The copy operations are disabled by deleting
them).

So default move operations are generated for classes(when needed) only if these three things are true:

1. No copy operations are declared in the class
1. No move operations are declared in the class
1. No desctructor is declared in the class

根据需要可以显示的要求compiler 生成默认的函数
```C++
class Widget {
 public:
  Widget(const Widget&) = default;
  Widget& operator=(const Widget&) = defalut;

  Widget(Widget&&) = default;
  Widget& operator=(Widget&&) = defalut;
};
```

# smart pointer
## use `std::shared_ptr` for shared-ownership resource management
Both `std::unique_ptr` and `std::shared_ptr` supports custom deleters, but the design of this support differs.
For `std::unique_ptr`, the type of the deleter is part of the type of the smart pointer. For `std::shared_ptr`, it's not.
```C++
auto loggingDel = [](Widget* pw) {
  makeLogEntry(pw);
  delete pw;
};

// deleter type is part of unique ptr type
std::unique_ptr<Widget, decltype(loggingDel)> upw(new Widget, loggingDel);
// deleter type is not part of shared ptr type
std::shared_ptr<Widget> spw(new Widget, loggingDel);
```

The `std::shared_ptr` design is more flexible. Consider two `std::shared_ptr<Widget>`s, each with a custom deleter of a
different type.
```C++
auto del1 = [](Widget* pw) { ... };
auto del2 = [](Widget* pw) { ... };

std::shared_ptr<Widget> pw1(new Widget, del1);
std::shared_ptr<Widget> pw2(new Widget, del2);
```
Because pw1 and pw2 have the same type, they can be placed in a container of objects of that type
`std::vector<std::shared_ptr<Widget>>`.
Thet could also be assigned to one another.
None of these things could be done with `std::unique_ptr` that differ in the types of their custom deleters.

There is a control block for each object managed by `std::shared_ptr`.
Control block contains reference count, weak count, a copy of the custom deleter, if one has been specified.

<img src="./pics/effective_modern_cpp/shared_ptr_control_block.png" alt="shared_ptr_control_block" width="50%"/>

`std::enable_shared_from_this`: a template for a base class you inherit from if you want a class managed by
`std::shared_ptr` to be able to safely create a `std::shared_ptr` from a `this` pointer.
```C++
class Widget : public std::enable_shared_from_this<Widget> {
 public:
  void process(std::vector<std::shared_ptr<Widget>>* processed_widgets) {
    // xxxx
    processed_widgets.emplace_back(std::shared_from_this());
  }
};
```
`std::enable_shared_from_this` defines a member function that creates a `std::shared_ptr` to the current object without
duplicating control blocks.
The design relies on the current object having an associated control block. For that to be the case, there must be an
existing `std::shared_ptr`.

Another difference from `std::unique_ptr`, `std::shared_ptr` has an API that's designed only for pointers to single
objects.
There's no `std::shared_ptr<T[]>`, but from time to time, "clever" programmers stumble on the idea of using
`std::shared_ptr<T>` to point to an array, specifying a custom deleter to perform an array delete.
This can be made to compile, but it's a horrible idea. `std::shared_ptr` offers no operator[](C++17 开始提供了).
Given the variety of C++11 alternatives to `std::vector`, declaring a smart pointer to a dump array is almost always a
sign of bad design.

## use `std::weak_ptr` for `std::shared_ptr` like pointers that can dangle
### scenario 1: cache
Consider a factory function that produces smart pointers to read-only objects based on a unique ID.
```C++
std::unique_ptr<const Widget> loadWidget(WidgetID id);
```
If loadWidget is an expensive call, a reasonable optimization would be using a cache.
For this caching factory function, a `std::unique_ptr` return type is not a good fit.
Callers should certainly receive smart pointers to cached objects, and callers should certainly determine the lifetime
of those objects, but the cache needs a pointer to the objects, too.
The cache's pointers need to be able to detect when they dangle, because when factory clients are finished using an
object returned by the factory, that object will be destroyed, and the corresponding cache entry will dangle.
The cached pointers should therefore be `std::weak_ptr`.
```C++
// 可以继续改进, 删除已经无用的
std::shared_ptr<const Widget> fastLoadWidget(WidgetID id) {
  static std::unordered_map<WidgetID, std::weak_ptr<const Widget>> cache;
  auto obj = cache[id].lock();  // obj is std::shared_ptr to cached object or null if the object is not in cache
  if (!obj) {
    obj = loadWidget(id);
    cache[id] = obj;
  }
  return obj;
}
```
cache 不能用shared_ptr, 因为如果cache 用了shared_ptr, 那么cache 住的对象就永远不会被销毁了, 即使caller 已经不用了.

### scenario 2: observer design pattern
The primary components of this pattern are subjects(objects whose state may change) and observers(objects to be notified
when state changes occur).
In most implementations, each subject contains a data member holding pointers to its observers. That makes it easy for
subjects to issue state change notifications.
Subjects have no interest in controlling the lifetime of their observers(i.e., when they are destroyed),
but they have a great interest in making sure that if an observer gets destroyed, subjects don't try to subsequently
access it.
A reasonable design is for each subject to hold a container of `std::weak_ptr` to its observers.

### scenario 3: `std::shared_ptr` cycle

## prefer `std::make_unique` and `std::make_shared` to direct use of new
The size and speed advantages of `std::make_shared` vis-à-vis direct use of new stem from `std::shared_ptr`'s control
block being placed in the same chunk of memory as the managed object.
When that object's reference count goes to zero, the object is destroyed (i.e., its destructor is called).
However, the memory it occupies can't be released until the control block has also been destroyed, because the same
chunk of dynamically allocated memory contains both.

As long as `std::weak_ptr`s refer to a control block (i.e., the weak count is greater than zero), that control block
must continue to exist.
And as long as a control block exists, the memory containing it must remain allocated.
The memory allocated by a `std::shared_ptr` make function, then, can't be deallocated until the last `std::shared_ptr`
and the last `std::weak_ptr` referring to it have been destroyed.

# std::move and std::forward
std::move doesn't move anything, std::forward doesn't forward anything.

At runtime, neither does anyting at all. They generate no executable, not a single byte. They are merely function
templates that perform casts.

**std::move unconditionally casts its argument to an rvalue, while std::forward perform this cast only if its argument
was initialized with an rvalue.**

## std::move
- [CppCon 2019: Klaus Iglberger "Back to Basics: Move Semantics (part 1 of 2)"](
https://www.youtube.com/watch?v=St0MNEU5b0o)
- [pdf](https://github.com/CppCon/CppCon2019/blob/master/Presentations/back_to_basics_move_semantics_part_1/back_to_basics_move_semantics_part_1__klaus_iglberger__cppcon_2019.pdf)

```C++
// C++14, still in namespace std
template<typename T>
decltype(auto) std::move(T&& param) {
  using ReturnType = std::remove_reference_t<T>&&;
  return static_cast<ReturnType>(param);
}
```
The returned rvalues are candidates for moving, so applying std::move to an object tells the compiler that this object
is eligible to be moved from.
That's why std::move has the name it does: to make it easy to designate objects that may be moved from.

1. Don't declare objects const if you want to able to move from them. Move requests on const objects are silently
  transformed into copy operations.
1. std::move not only doesn't actually move anything, it doesn't even guarantee that the object it's casting will be
  eligible to moved.

Make move operations noexcept.

A movie operation should move and leave its source in a valid state.
Ideally, that moved-from should be the default value of the type. Ensure that unless there is an exceptionally good
reason not to. However, not all types have a default value and for some types establishing the default value can be
expensive. The standard requires only that the moved-from object can be destroyed. Often, we can easily and cheaply do
better: The standard library assumes that it is possible to assign to a moved-from object. Always leave the moved-from
object in some (necessarily specified) valid state.

## std::forward
- [CppCon 2019: Klaus Iglberger "Back to Basics: Move Semantics (part 2 of 2)"](
https://www.youtube.com/watch?v=pIzaZbKUw2s)
- [pdf](https://github.com/CppCon/CppCon2019/blob/master/Presentations/back_to_basics_move_semantics_part_2/back_to_basics_move_semantics_part_2__klaus_iglberger__cppcon_2019.pdf)

Recall how std::forward is typically used. The most common scenario is a function template taking a universal reference
parameter that is to be passed to another function.
```C++
template<typename T>
void fun(T&& param) {
  someFunc(std::forward<T>(param));
}

void someFunc(const Widget& lval);
void someFunc(Widget&& rval);
```

Inside fun, param is always a lvalue(as it has a name), so if without std::forward, every call to someFunc will thus
want to invoke the lvalue overloaded version.
To prevent this, we need a mechanism for param to be cast to an rvalue if and only if the argument with which param was
initialized(the argument passed to fun) was an rvalue.
This is precisely what std::forward does. Thas's why std::forward is a conditional cast.

You may wonder how std::forward can know whether its argument was initialized with an rvalue.

The deduced template parameter T will encode whether the argument passwd to param was an lvalue or an rvalue.
The encoding mechanism is simple, when an lvalue is passwd as an argument, T is deduced to be an lvalue reference, when
an rvalue is passed, T is deduced to be a non-reference.

```C++
Widget widgetFactory();  // function returning rvalue
Widget w;  // an lvalue
func(w);  // call func with lvalue, T deduced to be Widget&
func(widgetFactory());  // call func with rvalue, T deduced to be Widget
```

### reference collapsing
If either reference is an lvalue reference, the result is an lvalue reference. Otherwise(i.e., if both are rvalue
references) the result is an rvalue reference.

1. `A& &` becomes `A&`
1. `A& &&` becomes `A&`
1. `A&& &` becomes `A&`
1. `A&& &&` becomes `A&&`

Here is how std::forward can be implemented
```C++
// C++14, in namespace std
template<typename T>
T&& std::forward(std::remove_refrence_t<T>& param) {
  retrun static_cast<T&&>(param);
}
```
Suppose that the argument passed to fun is an lvalue of type Widget. T will be deduced as Widget&, and the call to
std::forward will instantiate as `std::forward<Widget&>` yielding this:
```C++
Widget& && std::forward(std::remove_reference_t<Widget&>& param) {
  return static_cast<Widget& &&>(param);
}

// becomes
Widget& std::forward(Widget& param) {
  return static_cast<Widget&>(param);
}
```
We can see, when an lvalue argument is passed to the function template fun, std::forward is instantiated to take and
return an lvalue reference.

Suppose that the argument passed to f is an rvalue of type Widget, T will be deduced as Widget, and the call to
std::forward will instantiate as `std::forward<Widget>` yielding this:
```C++
Widget&& std::forward(std::remove_reference_t<Widget>& param) {
  return static_cast<Widget&&>(param);
}

// becomes
Widget&& std::forward(Widget& param) {
  return static_cast<Widget&&>(param);
}
```
In this case, std::forward will turn fun's parameter param(an lvalue) into an rvalue. The end result is that an rvalue
agrument passed to fun will be forwarded to someFunc as an rvalue.


```cpp
template<typename T, typename Arg...>
std::unique_ptr<T> std::make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```


## distinguish universal references from rvalue references
If a function template parameter has type `T&&` for a deduced type T, or if an object is defined using `auto&&`, the
parameter or object is a universal reference.
```C++
template<typename T>
void f(T&& param);  // universal reference

template<typename T>
void f(std::vector<T>&& param);  // rvalue reference

template<typename T>
void f(const T&& param);  // rvalue reference
```

Being in a template doesn't guarantee the presence of type deduction, consider the `push_back` member function in
`std::vector`:
```C++
// from C++ standards
template<class T, class Allocator = allocator<T>>
class vector {
 public:
  void push_back(T&& x);
  ...
};
```
`push_back`'s parameter certainly has the right form for a universal reference, but there's no type deduction in this
case.
That's because push back can't exist without a particular vector instantiation for it to be part of, and the type of
that instantiation fully determines the declaration for push back. For example:
```C++
std::vector<Widget> v;

class vector<Widget, allocator<Widget>> {
 public:
  void push_back(Widget&& x);  // rvalue reference
};
```
Now we can see that push back employs no type deduction.

In contrast, the conceptually similar `emplace_back` member function in std::vector does employ type deduction.
```C++
// still from C++ standards
template<class T, class Allocator = allocator<T>>
class vector {
  template<class ... Args>
  void emplace_back(Args&&... args);  // universal reference
  ...
};
```

C++14 lambda expressions may declare auto&& parameters.
For example, if you wanted to write a C++14 lambda to record the time taken in a arbitrary function invocation, you
could do this:
```C++
auto timeFunctionInvocation = [](auto&& func, auto&&... params) {
  start timer;
  std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);  // invoke func on params
  stop timer and record elapsed time;
};
```

## use std::move on rvalue references, std::forward on universal references
```C++
class Widget {
 public:
  Widget(Widget&& rhs) : name(std::move(rhs.name)), p(std::move(rhs.p)) {}
  ...
 private:
  std::string name;
  std::shared_ptr<SomeDataStructure> p;
};
```

```C++
class Widget {
 public:
  template<typename T>
  void setName(T&& newName) {
    name = std::forward<T>(newName);
    ...
  }
};
```
In short, rvalue references should be unconditionally cast to rvalues (via std::move) when forwarding them to other
functions, because they're always bound to rvalues.
And universal references should be conditionally cast to rvalues (via std::forward) when forwarding them, because
they're only sometimes bound to rvalues.

**If you're in a function that returns by value, and you're returning an object bound to an rvalue reference or a
universal reference, you'll want to apply std::move or std::forward when you return the reference**.
```C++
Matrix operator+(Matrix&& lhs, const Matrix& rhs) {
  lhs += rhs;
  return std::move(lhs);
}
```
Assume that Matrix has move constructor, lhs will be moved into the function's return value location.
If Matrix does not support moving, lhs will simply be copied, nothing hurts.

```C++
template<typename T>
Fraction reduceAndCopy(T&& frac) {
  frac.reduce();
  return std::forward<T>(frac);  // move rvalue into return value, copy lvalue
}
```

有些人就会滥用这个特性来过度优化.
```C++
// "copying" version of makeWidget
Widget makeWidget() {
  Widget w;
  return w;  // "copy" w into return value
}

// "moving" version of makeWidget
Widget makeWidget() {
  Widget w;
  return std::move(w);  // "move" w into return value, wrong, do not do this
}
```
The standardization Committe is way ahead of such programmers when it comes to this kind of optimization.
It was recognized long ago that the "copying" version of makeWidget can avoid the need to copy the local variable w by
constructing it in the memory allocated for the function's return value.
This is also known as the **return value optimization(RVO)**, and it's been expressly blessed by the C++ Standard with
two conditions:

1. The type of the local object is the same as that returned by the function
1. The local object is what's being returned directly.

The "copying" version has fulled both conditions.

While the "moving" version does not. What's being returned here isn't the local object w, it's a reference to w, the
result of std::move(w).
So compilers must move w into the function's return value location.

The part of the standard blessing the RVO goes on to say that if the conditions for RVO are met, but compilers choose
not to perform copy elision,
the object being returned must be treated as an rvalue.
In effect, the Standart requires that when RVO is permitted, either copy elision takes places or std::move is implicitly
applied to local objects being returned.

The situation is similar for by-value function parameters.
They're not eligible for copy elision with respect to their function's return value, but compilers must treat them as
rvalues if they're returned.
As a result, if your source code lookes like this:
```C++
// by value parameter of same type as function's return
Widget makeWidget(Widget w) {
  ...
  return w;
}
```
Compilers must treat as if it had been written this way:
```C++
Widget makeWidget(Widget w) {
  ...
  return std::move(w);  // treat as rvalue
}
```
所以完全没有必要自己显示的加 std::move, 这样反而不利于编译器的RVO.

## avoid overloading on universal references
Functions taking universal references are the greediest functions in C++.
They instantiate to create exact matches for almost any type of argument.
This is why combining overloading and universal references is almost always a bad idea.

```cpp
// function with lvalue reference (1)
void f(Widget&);

// function with lvalue reference-to-const (2)
void f(const Widget&);

// function with rvalue reference (3)
void f(Widget&&);

// function with rvalue reference-to-const (4)
void f(const Widget&&);

// function template with universal reference (5)
template<typename T>
void f(T&&);

// function template with rvalue reference-to-const (6)
template<typename T>
void f(const T&&);
```

### case 1
```cpp
void g() {
  Widget w{};
  f(w);
}
```
best match: 1 > 5 > 2

5 实例化, 可以产生与1 完全一样的signature, 但是当非模板和模板的都存在时, 优先用非模板的.

### case 2
```cpp
void g() {
  const Widget w{};
  f(w);
}
```
best match: 2 > 5

### case 3
```cpp
Widget getWidget();  // return rvalue
void g() {
  f(getWidget());
}
```
best match: 3 > 5 > 4 > 6 > 2

2 中的 const Widget& 会延长 getWidget 返回的rvalue 的生命周期.


### case 4
```cpp
const Widget getWidget();

void g() {
  f(getWidget());
}
```
best match: 4 > 6 > 5 > 2

## familiarize yourself with alternatives to overloading on universal references
By default, all templates are enabled, but a template using `std::enable_if` is enabled only if the condition specified
by `std::enable_if` is satisfied.

```C++
// the templatized constructor should be enabled only if T is a type other than Person
class Person {
 public:
  template<typename T, typename = std::enable_if_t<!std::is_base_of_v<Person, std::decay_t<T>> && !std::is_integral_v<std::remove_reference_t<T>>>>
  explict Person(T&& name) : name_(std::forward<T>(name)) {
    // assert during compilation
    static_assert(std::is_constructible<std::string, T>::value, "Parameter name can't be used to construct a std::string");
  }

  explict Person(int idx) : name_(nameFromIndx(idx)) {}

 private:
  std::string name_;
};
```

# Lambda Expressions
## use init capture to move objects into closures
```C++
class Widget {
 public:
  bool isValidated() const;
};

auto pw = std::make_unique<Widget>();
auto func = [p = std::move(pw)](){
  return p->isValidated();
};
```
The name p refers to a data member in the closure class, while the name pw refers to the object declared above the lambda.

## use decltype on auto&& parameters to std::forward them
One of the most exciting features of C++14 is generic lambdas --- lambdas that use auto in their parameter specifications.
The implementation of this feature is straight-forward: operator().
```C++
auto f = [](auto&& x){return func(normalize(std::forward<decltype(x)>(x)));};

// 产生的对应的 closure class
class SomeCompilerGeneratedClassName {
 public:
  template<typename T>
  auto operator()(T&& x) const {
    return func(normalize(std::forward<decltype(x)>(x)));
  }
};

template<typename T>
T&& std::forward(std::remove_reference_t<T>& param) {
  return static_cast<T&&>(param);
}
```
If client code wants to perfect-forward an rvalue of type Widget, it noramlly instantiates std::forward with the type
Widget(i.e., a non-reference type).
But now, what we have is `std::forward<decltype(x)>`, that is `std::forward<Widget&&>` with T as Widget&&, the forward
is instantiated as following:
```C++
Widget&& && std::forward(std::remove_reference_t<Widget&&>& param) {
  return static_cast<Widget&& &&>(param);
}

Widget&& std::forward(Widget& param) {
  return static_cast<Widget&&>(param);
}
```
If you compare this instantiation with the one that results when std::forward is called with T as Widget, you'll see
that they're identical. That's good news.

```C++
// variadic lambda
auto f = [](auto&&... params) {
  return func(normalize(std::forward<decltype(params)>(params)...));
};
```

## prefer lambdas to std::bind
In C++11, lambdas are almost always a better choice that std::bind. As of C++14, the case for lambdas isn't just
stronger, it's downright ironclad.

# The Concurrency API
## use std::atomic for concurrency, volatile for special memory
- std::atomic is useful for concurrent programming, but not for accessing special memory.
- volatile is useful for accessing special memory, but not for concurrent programming.

```C++
volatile int vi(0);
vi = 10;
std::cout << vi;
++vi;
--vi;
```
During execution of this code, if other threads are reading the value of vi, they may see anything, e.g., -12, 68,
4090727 - anything!
Such code would have undefined behavior, because these statements modify vi, so if other threads are reading vi at the
same time, there are simultaneous readers and writers of memory that's neither std::atomic nor protected by mutex, and
that's the definition of a data race.

As a general rule, compilers are permitted to reorder such unrelated assignments. That is, given this sequence of
assignments (where a, b, x and y correspond to independent variables).
```C++
a = b;
x = y;
```
compilers may generally reorder them as follows:
```C++
x = y;
a = b;
```
Even if compilers don't reorder them, the underlying hardware might do it.
However, the use of std::atomic imposes restrictions on how code can be reordered, and one such restriction is that no
code that precedes a write of a std::atomic variable may take place afterwards.
```C++
std::atomic<bool> valAvailable(false);
auto imptValue = computeImportantValue();
valAvailable = true;  // tell other task is's available
```
So not only must compilers retain the order of the assignments to imptValue and valAvailable, they must generate code
that ensures that the underlying hardware does, too.

Declaring valAvailable as volatile doesn't impose the same reordering restrictions.
No guarantee of operation atomicity and insufficient restrictions on code reordering --- explain why volatile's not
useful for concurrent programming.

volatile is for telling compilers that they're dealing with memory that doesn't behave normally.

Normal memory has the following characteristic:

1. if you write a value to a memory location, the value remains there until something overwrites it.
1. if you write a value to a memory location, never read it and then write to that memory location again, the first
  write can be eliminated, because it was never used.

```C++
volatile int x = 0;
auto y = x;  // y is deduced as int
y = x;
x = 10;
x = 20;

// compilers can treat if as if it had been written like this:
auto y = x;
x = 20;
```
Such optimizations are valid only if memory behaves normally. Special memory doesn't.
volatile is the way we tell compilers that we're dealing with special memory. Its meaning to compilers is "Don't perform
any optimizaiton on operatons on this memory".

Compilers are permitted to eliminate such redundant operations on std::atomic.

volatile and std::atomic can be used together for special memory that is concurrently accessed by multiple threads.

```C++
std::atomic<int> x(0);
std::atomic<int> y(x.load());
y.store(x.load());
```
Copy operatons for std::atomic are deleted. Because read x and write y are two independent atomic operations, hardware
can't do that in a single atomic operation.
So copy constructor isn't supported by std::atomic, copy assignment is deleted for the same reason.
And as copy operations are deleted, move operations are deleted too.

