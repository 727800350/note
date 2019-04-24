ref: [c++中的左值与右值](https://www.cnblogs.com/catch/p/3500678.html)

## `auto&&`
[What does auto&& tell us?](https://stackoverflow.com/questions/13230480/what-does-auto-tell-us)

By using `auto&& var = <initializer>` you are saying: I will accept any initializer regardless of whether it is an lvalue or rvalue expression and I will preserve its constness.
This is typically used for forwarding (usually with `T&&`). The reason this works is because a "universal reference", `auto&&` or `T&&`, will bind to anything.

`const auto&` will also bind to anything. The difference is that it's const! You won't be able to later bind it to any non-const references or invoke any member functions that are not marked const.

As an example, imagine that you want to get a std::vector, take an iterator to its first element and modify the value pointed to by that iterator in some way:

```C++
auto&& vec = some_expression_that_may_be_rvalue_or_lvalue;
auto i = std::begin(vec);
(*i)++;
```
This code will compile just fine regardless of the initializer expression. The alternatives to auto&& fail in the following ways:

```C++
auto         => will copy the vector, but we wanted a reference
auto&        => will only bind to modifiable lvalues
auto&&       => will bind to anything, 转发引用(推荐用于声明变量)
const auto&  => will bind to anything but make it const
const auto&& => will bind only to rvalues
```

## 左值右值的定义
左值有持久的状态, 而右值要么是字面常量, 要么是在表达式求值过程中创建的临时变量.
右值引用指向将要倍销毁的对象, 因为我们可以从绑定到右值引用的对象"窃取"状态, 也就是将右值引用的资源"移动"到另一个对象中.

变量表达式都是左值, 带来的结果就是, 我们不能将一个右值引用绑定到一个右值引用类型的变量上.
```C++
int&& rr1 = 42;  // 正确, 字面常量是右值
int&& rr2 = rr1;  // 错误, 表达式rr1 是左值
```
其实有了右值表示临时对象这一个观察结果, 变量是左值这一特性并不令人惊讶. 毕竟, 变量是持久的, 直至离开作用域才被销毁.

关于右值,在 c++11 以前有一个十分值得关注的语言的特性:右值能被 const 类型的引用所指向,所以如下代码是合法的.
```C++
Foo get_foo(){
	return Foo();
}

const Foo& ref = get_foo(); // correct
```
当一个右值被 const 引用指向时,它的生命周期就被延长了,这个用法我在前面一篇博客里讲到过它的相关应用.其中暗藏的逻辑其实就是:右值不能当成左值使用(但左值可以当成右值使用).
另外值得注意的是,对于前面提到的右值的两个特性:

1. 允许调用成员函数
2. 只能被 const reference 指向

```C++
void func(Foo& c);
func(get_foo()); // error
func(get_foo() = get_foo()); // correct
```
其中: `func(get_foo() = get_foo())` 能够被正常编译执行的原因就在于,`Foo::operator=()` 返回的是 `Foo&` 不允许非 `const reference`.
引用 rvalue 并不是完美的,它事实上也引起了一些问题,比如说拷贝构造函数的接口不一致了,这是什么意思呢?

## std::move
ref: [c++11 中的 move 与 forward](http://www.cnblogs.com/catch/p/3507883.html)

std::move 告诉编译器, 我们有一个左值, 但我们希望像一个右值一样处理它.
我们必须认识到, 调用std::move 就意味着承诺: 除了对它赋值或者销毁外, 我们将不再使用它, 在调用move 之后, 我们不能对移后原对象的值做任何假设.

# std::forward
除了 move() 语义之外，右值引用的提出还解决另一个问题：完美转发 (perfect forwarding).

转发问题针对的是模板函数，这些函数主要处理的是这样一个问题：
假设我们有这样一个模板函数，它的作用是：缓存一些 object，必要的时候创建新的。

```C++
template<class TYPE, class ARG>
TYPE* acquire_obj(ARG arg){
	static list<TYPE*> caches;
	TYPE* ret;

	if(!caches.empty()){
		ret = caches.pop_back();
		ret->reset(arg);
		return ret;
	}

	ret = new TYPE(arg);
	return ret;
}
```

这个模板函数的作用简单来说，就是转发一下参数 arg 给 TYPE 的 reset() 函数和构造函数，除此它就没再干别的事情，
在这个函数当中，我们用了值传递的方式来传递参数，显然是比较低效的，多了次没必要的拷贝，于是我们准备改成传递引用的方式，同时考虑到要能接受 rvalue 作为参数，最后做出艰难的决定改成如下样子：

```C++
template<class TYPE, class ARG>
TYPE* acquire_obj(const ARG& arg){
	//...
}
```
但这样写很不灵活：

1. 首先，如果 reset() 或 TYPE 的构造函数不接受 const 类型的引用，那上述的函数就不能使用了，必须另外提供非 const TYPE& 的版本，参数一多的话，很麻烦。
2. 其次，如果 reset() 或 TYPE 的构造函数能够接受 rvalue 作为参数的话，这个特性在 `acquire_obj()` 里头也永远用不上。

其中1) 好理解，2) 是什么意思？

2) 说的是这样的问题，即使 TYPE 存在 `TYPE (TYPE&& other)` 这样的构造函数，它在上述 `acquire_obj()` 中也永远不会被调用，
原因是在 `acquire_obj()` 中，传递给 TYPE 构造函数的，永远是 lvalue（因为 arg 有名字），哪怕外面调用 acquire_obj() 时，用户传递进来的是 rvalue，请看如下示例：

```C++
holder get_holder();
holder* h = acquire_obj<holder, holder>(get_holder());
```
虽然在上面的代码中，我们传递给 `acquire_obj()` 的是一个 rvalue，但是在 `acuire_obj()` 内部，我们再使用这个参数时，它却永远是 lvalue，因为它有名字 --- 有名字的就是 lvalue。
`acquire_obj()` 这个函数它的基本功能本来只是传发一下参数，理想状况下它不应该改变我们传递的参数的类型，但上面的写法却没有做到这点，而在 c++11 以前也没法做到。
forward() 函数的出现，就是为了解决这个问题。

forward() 函数的作用：它接受一个参数，然后返回该参数本来所对应的类型的引用。

```C++
template<class TYPE, class ARG>
TYPE* acquire_obj(ARG&& arg){
	return new TYPE(forward<ARG>(arg));
}
```

