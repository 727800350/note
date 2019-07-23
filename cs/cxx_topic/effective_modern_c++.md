# introduction
In concept(though not always in practice), rvalues correspond to temporary objects returned from functions,
while lvalues correspond to objects you can refer to, either by name or by following a pointer or lvalue reference.

A useful heuristic to determine whether an expression is an lvalue is to ask if you can take its address.
If you can, it typically is. If you can't, it's usually an rvalue.

In a function call, the expressions passed at the call site are the functions's arguemnts. The parameters are used to initialize the function's parameters.
```C++
void someFunc(Widget w);
Widget wid;
someFunc(wid);
someFUnc(std::move(wid));
```
In the first call to someFunc, the argument is wid. In the second call, the argument is std::move(wid). In both calls, the parameter is w.

The distinction between arguments and parameters is important, because parameters are lvalues, but the arguemnts may be rvalues or lvalues.
This is especially relevant during the process of perfect forwarding, whereby an argument passed to a function is passed to a second function call such that
the original argument's rvalueness or lvalueness is preserved.

Well-designed functions are exception safe, meaning they offer at least the basic exception safety guarantee (i.e., the basic guarantee).
Such functions assure calless that even if an exception is thrown, program invariants remain intact (i.e., no data structures are corrupted) and no resources are leaked.
Functions offering the strong exception safety guarantee (i.e., the strong guarantee) assure callers that if an exception arises, the state of the program remains as it was priori to the call.

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
f(rx);  // xx is lvalue, so T is const int&, param's type is also const int&
f(27);  // 27 is rvalue, so T is int, param's type is therefore int&&.
        // 27 is of type int&&, we first ignore the reference part, and we got int, so T is int, and param's type is int&&
```

### case 3: ParamType is neither a pointer nor a reference
```C++
template<typename T>
void f(T param);  // param is now passed by value
```
That means that param will be a copy of whatever is passed in - a completely new object.
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

