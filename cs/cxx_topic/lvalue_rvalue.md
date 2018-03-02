ref: [c++中的左值与右值](https://www.cnblogs.com/catch/p/3500678.html)

## 左值右值的定义
```C++
int a;
int b;

a = 3;
b = 4;
a = b;
b = a;

// 以下写法不合法.
3 = a;
a+b = 4;
```

在 c 语言中,通常来说有名字的变量就是左值(如上面例子中的 a, b),而由运算操作(加减乘除,函数调用返回值等)所产生的中间结果(没有名字)就是右值,如上的 3 + 4, a + b 等.
我们暂且可以认为:左值就是在程序中能够寻值的东西,右值就是没法取到它的地址的东西(不完全准确),但如上概念到了 c++ 中,就变得稍有不同.
具体来说,在 c++ 中,每一个表达式都会产生一个左值,或者右值,相应的,该表达式也就被称作"左值表达式", "右值表达式".
对于基本数据类型来说(primitive types),左值右值的概念和 c 没有太多不同,不同的地方在于自定义的类型,而且这种不同比较容易让人混淆:

1. 对于基础类型,右值是不可被修改的(non-modifiable),也不可被 const, volatile 所修饰(cv-qualitification ignored)
2. 对于自定义的类型(user-defined types),右值却允许通过它的成员函数进行修改.

对于 1),这和 c 是一致的,2) 却是 C++ 中所独有, 因此,如果你看到 C++ 中如下的写法,千万不要惊讶:

```C++
#include <iostream>

using namespace std;

class cs{
public:
	cs(int i): _i(i) {
		cout << "cs(" << i <<") constructor!" << endl;
	}
	~cs(){
		cout << "cs destructor,i(" << _i << ")" << endl;
	}

	cs& operator=(const cs& other) {
		_i = other._i;
		cout << "cs operator=()" << endl;
		return *this;
	}

	int get_i() const {
		cout << "cs get op " << _i << endl;
		return _i;
	}
	void change(int i) {
		cout << "cs change op " << _i << " " << i << endl;
		_i = i;
	}

private:
	int _i;
};

cs get_cs(){
	static int i = 0;
	return cs(i++);
}

int main() {
	(get_cs() = cs(20)).change(323);
	cout << endl;

	get_cs() = cs(30);
	cout << endl;

	get_cs().change(40);
	cout << endl;

	// 当一个右值被 const 引用指向时,它的生命周期就被延长了, 直到引用被销毁,从而不会因此产生悬空(dangling)的引用
	const cs &ref = get_cs();
	cout << ref.get_i() << endl;
	cout << endl;

	return 0;
}
```

output
```
cs(20) constructor! // cs(20)
cs(0) constructor! // get_cs()
cs operator=() // =
cs change op 20 323 // change
cs destructor,i(323) // change 之后的对象
cs destructor,i(20) // cs(20) 返回的对象

cs(30) constructor!
cs(1) constructor!
cs operator=()
cs destructor,i(30)
cs destructor,i(30)

cs(2) constructor!
cs change op 2 40
cs destructor,i(40)

cs(3) constructor!
cs get op 3
3
cs destructor,i(3)
```

这个特性看起来多少有些奇怪,因为通常来说,自定义类型应该设计得和内置类型尽量一样(所谓 value type,value semantic),但允许成员函数改变右值这个特性却有意无意使得自定义类型特殊化了.
对此,我们其实可以这样想,也许会好理解点:自定义类型允许有成员函数,而通过右值调用成员函数是被允许的,但成员函数有可能不是 const 类型,因此通过调用右值的成员函数,也就可能会修改了该右值,done!

## 左值引用,右值引用
关于右值,在 c++11 以前有一个十分值得关注的语言的特性:右值能被 const 类型的引用所指向,所以如下代码是合法的.

```C++
const cs& ref = get_cs(); // correct
```
而且准确地说,右值只能被 const 类型的 reference 所指向,非 const 的引用则是非法的:

```C++
cs& ref = get_cs(); // error 
```
当一个右值被 const 引用指向时,它的生命周期就被延长了,这个用法我在前面一篇博客里讲到过它的相关应用.其中暗藏的逻辑其实就是:右值不能当成左值使用(但左值可以当成右值使用).
另外值得注意的是,对于前面提到的右值的两个特性:

1. 允许调用成员函数
2. 只能被 const reference 指向

```C++
void func(cs& c){
	cout << "c:" << c.get_i() << endl;
}

func(get_cs()); // error
func(get_cs() = get_cs()); // correct
```
其中: `func(get_cs() = get_cs())` 能够被正常编译执行的原因就在于,cs 的成员函数 `operator=()` 返回的是 `cs&` 不允许非 `const reference`.
引用 rvalue 并不是完美的,它事实上也引起了一些问题,比如说拷贝构造函数的接口不一致了,这是什么意思呢?

```C++
class cs{
public:      
	cs& operator=(const cs& c);
};

// 另一种写法
class cs2{
public:      
	cs2& operator=(cs2& c);
};
```
上面两种写法的不同之处就在于参数,一个是 const reference,一个是非 const.
对于自定义类型的参数,通常来说,如果函数不需要修改传进来的参数,我们往往就按 const reference 的写法,但对于 copy constructor 来说,它经常是需要修改参数的值,比如 auto_ptr.

```C++
class auto_ptr{
public:
	auto_ptr(auto_tr& p){
		ptr_ = p.ptr_;
		p.ptr_ = NULL;
	}
   
private:
	void*  ptr_;
};
```
所以,对于 `auto_ptr` 来说,它的 copy constructor 的参数类型是 non const reference.
有些情况下,这种写法应该被鼓励,毕竟 non const reference 比 const reference 更能灵活应对各种情况,从而保持一致的接口类型,当然也有代价,参数的语义表达不准确了.
除此更大的问题是如果拷贝构造函数写成这样子,却又对 rvalue 的使用带来了极大的不变,如前面所讲的例子,rvalue 不能被 non const reference 所引用,所以像 auto_ptr 的这样的类的 copy constructor 就不能接受 rvalue.

```C++
auto_ptr p(get_ptr()); // error

auto_ptr p = get_ptr(); // operator=() 同理,错误.
```
这也是 `auto_ptr` 很不好用的原因之一,为了解决这个问题,c++11 中引入了一种新的引用类型,该种引用是专门用来指向 rvalue 的,有了这种新类型,对 lvalue 和 rvalue 的引用就能明确区分开来了.
因为有了这种新的类型,接着就引出了 c++11 中新的语义,`move()`, `forward()` 等,这儿先卖个关子,我们下次再讲.

## std::move
ref: [c++11 中的 move 与 forward](http://www.cnblogs.com/catch/p/3507883.html)

关于 lvaue 和 rvalue,在 c++11 以前存在一个有趣的现象:`T &`  指向 lvalue (左传引用), `const T &` 既可以指向 lvalue 也可以指向 rvalue.
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
	std::swap(_resource, other._resource);
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

写到里,有的人也许会有疑问:  `T && ref` 指向的是右值(右值引用),那 ref 本身在函数内是左值还是右值?
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

这样一来,如果 holder 提供了 operator=(T&&) 重载,上述操作就相当于只是交换了三次指针,效率大大提升!move() 使得程序员在有需要的情况下能把 lvalue 当成右值来对待.

