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
const auto&  => will bind to anything but make it const, giving us const_iterator
const auto&& => will bind only to rvalues
```

## 左值右值的定义
左值有持久的状态, 而右值要么是字面常量, 要么是在表达式求值过程中创建的临时变量.
右值引用指向将要倍销毁的对象, 因为我们可以从绑定到右值引用的对象"窃取"状态, 也就是将右值引用的资源"移动"到另一个对象中.

## 左值引用,右值引用
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

关于 lvaue 和 rvalue,在 c++11 以前存在一个有趣的现象:`T &` 指向 lvalue (左传引用), `const T &` 既可以指向 lvalue 也可以指向 rvalue.
但却没有一种引用类型,可以限制为只指向 rvalue.这乍看起来好像也不是很大的问题,但实际与看起来不一样,右值引用的缺失有时严重限制了我们在某些情况下,写出更高效的代码.
举个粟子,假设我们有一个类,它包含了一些资源:

```C++
#include <iostream>

using namespace std;

class Resource{
};

class holder{
public:
	holder(int i){
		cout << "constructor basic " << this << " " << i << endl;
		_resource = new Resource();
		_i = i;
	}

	~holder(){
		cout << "destructor " << this << " " << _i << endl;
		delete _resource;
	}

	holder(const holder& other){
		cout << "constructor const & " << this << " " << other._i << endl;
		_resource = new Resource(*other._resource);
		_i = other._i;
	}

	holder(holder& other){
		cout << "constructor & " << this << " " << other._i << endl;
		_resource = new Resource(*other._resource);
		_i = other._i;
	}

	holder& operator=(const holder& other){
		cout << "= const & " << _i << " " << other._i << endl;
		delete _resource;
		_resource = new Resource(*other._resource);
		_i = other._i;
		return *this;
	}

	holder& operator=(holder& other){
		cout << "= & " << _i << " " << other._i << endl;
		delete _resource;
		_resource = new Resource(*other._resource);
		_i = other._i;
		return *this;
	}

private:
	Resource* _resource;
	int _i;
};

holder get_holder(){
	static int i = 0;
	return holder(i++);
}

int main(){
	holder h(10);
	cout << endl;

	h = get_holder();
	cout << endl;

	return 0;
}
```
output

```
constructor basic 0x7ffc3b4fb2f0 10

constructor basic 0x7ffc3b4fb300 0
= const & 10 0
destructor 0x7ffc3b4fb300 0 // get_holder() 创建的临时对象的销毁

destructor 0x7ffc3b4fb2f0 0 // h 的销毁
```
理想情况下(不考虑返回值优化等因素),`h = get_holder()`这一小段代码的最后一条语句做了如下三件事情:

1. 销毁 h 中的资源
2. 拷贝 `get_holder()` 返回的资源
3. 销毁 `get_holder()` 返回的资源

显然我们可以发现这些事情中有些是不必要的:假如我们可以直接交换 h 中的资源与 `get_holder()` 返回的对象中的资源,那我们就可以直接省略掉第二步中的拷贝动作了.
而这里之所以交换能达到相同的效果,是因为 `get_holder()` 返回的是临时的变量,是个 rvalue,它的生命周期通常来说很短,具体在这里,就是赋值语句完成之后,任何人都没法再引用该 rvalue,它马上就要被销毁了,它所包含的资源也无法再被访问.

而如果是像下面这样的用法,我们显然不可以直接交换两者的资源:

```C++
holder h1;
holder h2;

h1 = h2;

foo(h2);
```
因为 h2 是个 lvalue,它的生命周期较长,在赋值语句结束之后,变量仍然存在,还有可能要被别的地方使用.因此,rvalue 的短生命周期给我们提供了在某些情况优化代码的可能.

但这种可能在 c++11 以前是没法利用到的,因为我们没法在代码中对 rvalue 区别对待:在函数体中,程序员无法分辨传进来的参数到底是不是 rvalue,我们缺少一个 rvalue 的标记.

回忆一下,`T &` 指向的是 lvalue,而 `const T &` 指向的,却可能是 lvalue 或 rvalue,我们没有任何方式能够确认当前参数是不是 rvalue!
为了解决这个问题,c++11 中引入了一个新的引用类型: `T &&`,这种引用指向的变量是个 rvalue, 有了这个引用类型,我们前面提到的问题就迎刃而解了.

```C++
holder(holder&& other){
	_resource = other._resource;
	other._resource = NULL;
	_i = other._i;
}

holder& operator=(holder&& other){
	std::swap(_resource, other._resource); // std::swap 对movable 的变量有优化
	std::swap(_i, other._i);
	return *this;
}
```
因为有了右值引用,当我们再写如下代码的时候:

```C++
holder h1;
holder h2;

h1 = h2; // 调用operator(holder&);
h1 = get_holder(); // 调用operator(holder&&)
```
编译器就能根据当前参数的类型选择相应的函数,显然后者的实现是更高效的.

写到里,有的人也许会有疑问: `T && ref` 指向的是右值(右值引用),那 ref 本身在函数内是左值还是右值?
具体来说就是如下代码中,第二行所调用的是 `operator=(holder &)` 还是 `operator=(holder &&)`?

```C++
holder& operator=(holder&& other){
	holder h = other;
	return *this;
}
```
这个问题的本质还是怎么区分 rvalue? c++11 中对 rvalue 作了明确的定义:

Things that are declared as rvalue reference can be lvalues or rvalues. The distinguishing criterion is: if it has a name, then it is an lvalue. Otherwise, it is an rvalue.
如果一个变量有名字,它就是 lvalue,否则,它就是 rvalue.

根据这样的定义,上面的问题中,other 是有名字的变量(变量的类型是右值引用),因此是个 lvalue,因此调用的是 `operator=(holder &)`.

好了说了这么久,一直没说到 move(),现在我们来给出它的定义:

c++11 中的 move() 是这样一个函数,它接受一个参数,然后返回一个该参数对应的右值引用.
就这么简单!你甚至可以暂时想像它的原型是这样的(当然是错的,正确的原型我们后面再讲).

```C++
T&& move(T& val);
```

那么,这样一个 move() 函数,它有什么使用呢?用处大了!回到前面例子,我们用到了 std::swap() 这个函数,回想一下以前我们是怎么想来实现 swap 的呢?

```C++
void swap(T& a, T& b){
	T tmp = a;
	a = b;
	b = tmp;
}
```
想像一下,如果 T 是我们之前定义的 holder,这里面就多做了很多无用功,每一个赋值语句,就有一次资源销毁以及一次拷贝!而事实上我们只是要交换 a 与 b 的内容,中间的拷贝都是额外的负担,完全可以考虑消除这些无用功.

```C++
void swap(T& a, T& b){
	T tmp = move(a);
	a = move(b);
	b = move(tmp);
}
```

这样一来,如果 holder 提供了 `operator=(T &&)` 重载,上述操作就相当于只是交换了三次指针,效率大大提升!
move() 使得程序员在有需要的情况下能把 lvalue 当成右值来对待.

```C++
#include <iostream>
#include <vector>
#include <string>
 
int main(){
	std::string str = "Hello";
	std::vector<std::string> v;
 
	// uses the push_back(const T&) overload, which means we'll incur the cost of copying str
	v.push_back(str);
	std::cout << "After copy, str is \"" << str << "\"" << std::endl;
 
	// uses the rvalue reference push_back(T&&) overload, which means no strings will be copied;
	// instead, the contents of str will be moved into the vector.
	// This is less expensive, but str is now valid but unspecifie.
	v.push_back(std::move(str));
	std::cout << "After move, str is \"" << str << "\"" << std::endl;
 
	std::cout << "The contents of the vector are \"" << v[0] << "\", \"" << v[1] << "\"" << std::endl;

	return 0;
}
```

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

