定义class的结尾}后需要一个分号,和结构体的定义一样.

- this: The keyword this represents a pointer to the object whose member function is being executed. It is a pointer to the object itself.

private, public or protected.

- private: members of a class are accessible only from within other members of **the same class** or from their **friends**.
- protected: members are accessible from members of their **same class** and from their **friends**, but also from members of their **derived classes**.
- public: members are accessible from **anywhere** where the object is visible.

By default, all members of a class are private. 结构体的成员默认是public

Static members

A class can contain static members, either data or functions.
Static data members of a class are also known as "class variables", because there is **only one unique value for all the objects of that same class**.
For example, it may be used for a variable within a class that can contain a counter with the number of objects of that class that are currently allocated.

const对象不能调用非const函数

### 拷贝构造函数和赋值运算符
拷贝构造函数必须以引用的方式传递参数.这是因为,在值传递的方式传递给一个函数的时候,会调用拷贝构造函数生成函数的实参.如果拷贝构造函数的参数仍然是以值的方式,就会无限循环的调用下去,直到函数的栈溢出.

拷贝构造函数使用传入对象的值生成一个新的对象的实例,而赋值运算符是将对象的值复制给一个已经存在的实例.
调用的是拷贝构造函数还是赋值运算符,主要是看是否有新的对象实例产生.
如果产生了新的对象实例,那调用的就是拷贝构造函数,如果没有,那就是对已有的对象赋值,调用的是赋值运算符.

```C++
class Person{
public:
	Person(){
		LOG(INFO) << "default constructor";
	}

	Person(const Person& p){
		LOG(INFO) << "copy Constructor";
	}

	Person& operator=(const Person& p){
		LOG(INFO) << "assign operator";
		this->name = p.name;
		return *this;
	}

private:
	string name;
};

void f(Person p){
	return;
}

Person f1(){
	Person p;
	return p;
}

int main(int argc, char* argv[]){
	Person p; // default
	Person p1 = p; // copy, 虽然使用了"=",但是实际上使用对象p来创建一个新的对象p1.也就是产生了新的对象,所以调用的是拷贝构造函数.

	Person p2; // default
	p2 = p;	// assign, p2 已经是一个Person 对象, 所以这里是赋值运算符
	f(p2); // copy

	p2 = f1(); // default and assign

	Person p3 = f1(); // default, 应该是首先调用拷贝构造函数创建临时对象,然后再调用拷贝构造函数使用刚才创建的临时对象创建新的对象p3,也就是会调用两次拷贝构造函数.不过,编译器也没有那么傻,应该是直接调用拷贝构造函数使用返回值创建了对象p3.

	return 0;
}
```

## Virtual Function
[C++类内存分布(非常重要)](https://www.cnblogs.com/jerry19880126/p/3616999.html)

```C++
class Animal{
public:
	void walk(){
		LOG(INFO) << "animal walk";
	}

class Dog : public Animal{
public:
	void walk(){
		LOG(INFO) << "dog walk";
	}
};

int main(int argc, char* argv[]){
	Animal animal;
	animal.walk(); // 输出animal walk

	Dog dog;
	dog.walk(); // 输出dog walk

	Animal *x = &dog;
	x->walk(); // 输出animal walk, 而不是预期的 dog walk
}
```
将Animal 的walk 定义为虚函数, `virtual void walk()`, 则可以达到这个目的, 这就是虚函数.
通过这种方式, 可以实现运行时多态性, 比如有一个api: `void walk(Animal animal)`, 在编译的时候, 并不清楚animal 是具体哪一种动物, 也不需要关心, 运行的时候, 根据传入的参数来动态决定.

实现的方式, 会有一个虚函数表, 类里面会有一个指针指向这个表.

进一步, 如果不想让Animal 实例化, 只作为接口派生其他类, 可以将walk 定义为纯虚函数`virtual void walk() = 0`, 这样子类必须实现walk 这个函数.

### 虚析构函数
```C++
class Base{
public:
    Base(){
		LOG(INFO) << "constructor in base";
	};
    virtual ~Base(){
		LOG(INFO) << "destructor in base";
	};

    virtual void faire(){
		LOG(INFO) << "do something in base";
	};
};

class Derived : public Base{
public:
    Derived(){
		LOG(INFO) << "constructor in derived";
	};
    ~Derived(){
		LOG(INFO) << "destructor in derived";
	}

    void faire(){
		LOG(INFO) << "do something in derived";
	}
};
```

```C++
Base *instance = new Derived();
instance->faire();
delete instance;
```
在将 `~Base` 声明为虚函数时, 上面的代码会输出:
```
I0605 15:45:05.518903   159 test.cpp:14] constructor in base
I0605 15:45:05.519145   159 test.cpp:28] constructor in derived
I0605 15:45:05.519162   159 test.cpp:35] do something in derived
I0605 15:45:05.519176   159 test.cpp:31] destructor in derived
I0605 15:45:05.519187   159 test.cpp:17] destructor in base
```
可以看到, 析构的时候, 先调用的Derived 的析构函数, 然后再调用的Base 的析构函数.

如果将 `~Base` 不声明为虚函数, 上面的代码会输出
```
I0605 15:50:43.765009   167 test.cpp:14] constructor in base
I0605 15:50:43.765822   167 test.cpp:28] constructor in derived
I0605 15:50:43.765849   167 test.cpp:35] do something in derived
I0605 15:50:43.765859   167 test.cpp:17] destructor in base
```
只调用了Base 的析构函数(因为instance 声明的是Base 类型的), Derived 中就可能发生内存泄露, 资源没有释放.

## 派生
派生类构造函数的语法:
```
派生类名::派生类名(参数总表):基类名1(参数表1),....基类名n(参数名n),内嵌子对象1(参数表1),...内嵌子对象n(参数表n){
	派生类新增成员的初始化语句;
}
```
**注:构造函数的初始化顺序并不以上面的顺序进行,而是根据声明的顺序初始化.**

派生类构造函数执行的次序:

1. 调用基类构造函数,调用顺序按照它们被继承时声明的顺序(**从左到右**),
1. 调用内嵌成员对象的构造函数,调用顺序按照它们在类中声明的顺序,
1. 派生类的构造函数体中的内容.

派生类的析构函数的功能是在该对象消亡之前进行一些必要的清理工作,析构函数没有类型,也没有参数.
析构函数的执行顺序与构造函数相反.

1. 派生类的构造函数体中的内容
1. 调用内嵌成员对象的构造函数,调用顺序按照它们在类中声明的逆顺序
1. 调用基类构造函数,调用顺序按照它们被继承时声明的逆顺序

如果某派生类的多个基类拥有同名的成员,同时,派生类又新增这样的同名成员,在这种情况下,派生类成员将覆盖所有基类的同名成员.这就需要显示的指定基类, 才能调用基类的同名成员.

如果某个派生类的部分或全部直接基类是从另一个共同的基类派生而来,在这些直接基类中,从上一级基类继承来的成员就拥有相同的名称,因此派生类中也就会产生同名现象,
对这种类型的同名成员也要使用作用域分辨符来唯一标识,而且必须用直接基类进行限定

为了解决前面提到的多重拷贝的问题,可以将共同基类设置为**虚基类**,这时从**不同的路径继承过来的同名数据成员在内存中就只有一个拷贝,同一个函数也只有一个映射**.
虚基类的声明是在派生类的声明过程,其语法形式为:
```
class 派生类名::virtual 继承方式 基类名,
```

