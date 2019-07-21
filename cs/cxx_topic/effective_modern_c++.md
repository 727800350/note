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

## auto

