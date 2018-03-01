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

