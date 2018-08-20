# constructor and operator
```C++
class Empty{}; // sizeof(Empty) 为 1
```
Thanks to C++ compiler, actually it becomes something like this:
```C++
class Empty{
public:
	Empty(){}                           // default constructor
	Empty(const Empty&){}               // copy constructor
	~Empty(){}                          // destructor
	Empty& operator=(const Empty&){     // assignment operator
		return *this;
	}
};
```

```C++
Empty eA;                        // default constructor
~Empty();                        // destructor
Empty eB(eA);                    // copy constructor(creating a new object)
eB = eA;                         // assignment operator(assign to an existing object)
Empty eC = eA;                   // copy constructor(creating a new object)
```

The copy constructor should have one of the following forms:

- `MyClass(const MyClass &);`
- `MyClass(MyClass & );`
- `MyClass(volatile MyClass &);`
- `MyClass(volatile const MyClass &);`

copy constructor 必须以引用的方式传递参数.
因为,在值传递的方式传递给一个函数的时候,会调用拷贝构造函数生成函数的实参.如果拷贝构造函数的参数仍然是以值的方式,就会无限循环的调用下去,直到函数的栈溢出.
拷贝构造函数使用传入对象的值生成一个新的对象的实例,而赋值运算符是将对象的值复制给一个已经存在的实例.

```C++
Empty f1(const Empty &e){
	Empty x;
	return e;
}

Empty f2(const Empty &e){
	Empty x;
	return x;
}

Empty e;
Empty e1 = f1(e);
Empty e2 = f2(e);
```

调用f1, 会发生

1. default constructor, 创建x
1. copy constructor, 从e创建一个临时对象, 供出f1 的作用域之后使用
1. destuctor, 出f1 的scope 之后销毁x
1. 注: copy constructor 出来的对象, 没有看到显式的销毁, 因为编译器会进行优化, 从e copy 出来的一个对象直接成了e1

调用f2, 会发生

1. default constructor, 创建x
1. 注: 应该是首先从x copy 出一个临时对象tmp, 然后再由tmp copy 出e2, 也就是会调用两次拷贝构造函数.不过,都被编译器优化掉了

friend关键字,它能让被修饰的对象冲破本class的封装特性,从而能够访问本class的私有对象.

- 如果你在A类中,申明了函数func()是你的friend,那么func()就可以使用A类的所有成员变量,无论它在什么地方定义的.
- 如果你在A类中,申明了B类是你的friend,那么B类中的方法就可以访问A类的所有成员变量.

[C++ TUTORIAL - OBJECT RETURNING - 2018](http://www.bogotobogo.com/cplusplus/object_returning.php)

```C++
class Complx {
public:
	Complx() {}
	Complx(double r, double i): real(r), imag(i) {}

	Complx operator+(const Complx & c) const{
		return Complx(real + c.real, imag + c.imag);
	}

	Complx & operator=(const Complx &c){
		real = c.real;
		imag = c.imag;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream &os, const Complx &c); // friend 标记不能少

	double size() const{
		return sqrt(real * real + imag * imag);
	}

private:
	double real;
	double imag;
};

std::ostream& operator<<(std::ostream &os, const Complx &c){
	os << "(" << c.real << ", " << c.imag << ")";
	return os;
}
```

## Returning a Reference to a Non-const Object
The return value of `operator=()` is used for chained assignment:
```C++
Complx c3(20,40);
Complx c4, c5;
c5 = c4 = c3;
```
In the code, the return value of `c4.operator=(c3)` is assigned to c5.
Returning either a Complx object or a reference to a Complx object would work. But using a reference allows the function to avoid calling the Complx copy constructor to create a new Complx object.
In this case, the return type is not const because operator=() method returns a reference to c4, which is modified.

The return value of `operator<<()` is used for chained output:
```C++
cout << c4 << " got its value from c3" << endl;
```
In the code, the return value of `operator<<(cout, c4)` becomes the object used to display the string " got its value from c3". Here, the return type must be `ostream &` and not just ostream.

## Returning a const Object
The `Complx::operator+()` in the example has a strange property. The intended use is this:
```C++
Complx c6 = c1 + c2;	// #1
```
But the definition also allows us to use the following:
```C++
Complx c7;
c1 + c2 = c7;	// #2
```
This code is possible because the copy constructor constructs a temporary object to represent the return value.
So, in the code, the expression c1 + c2 stands for that temporary object. In statement #1, the temporary object is assigned to c6. In statement #2, c7 is assigned to the temporary object.

The temporary object is used and then discarded. For instance, in statement #2, the program computes the sum of c1 and c2, copies the answer into the temporary return object, overwrites the contents with the contents of c7,
and then discards the temporary object. The original complex numbers are all left unchanged.

If we declare the return type as a const object, we can avoid the problem.
```C++
const Complx operator+(const Complx & c) const{
	return Complx(real + c.real, imag + c.imag);
}
```

private, public or protected.

- private: members of a class are accessible only from within other members of **the same class** or from their **friends**.
- protected: members are accessible from members of their **same class** and from their **friends**, but also from members of their **derived classes**.
- public: members are accessible from **anywhere** where the object is visible.

By default, all members of a class are private. 结构体的成员默认是public

const对象不能调用非const函数

## Virtual Function
Once a function is declared as virtual, it remains virtual in all the dervied classes. 既不需要在dervied class 中显式指定virtual

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

### vector of base objects
```C++
class A{
public:
	A(int n = 0) : m(n) {}
	virtual ~A() { }

	virtual int getVal() const {
		std::cout << "A::getVal() = ";
		return m;
	}

protected:
	int m;
};

class B : public A{
public:
	B(int n = 0) : A(n) {}

	int getVal() const {
		std::cout << "B::getVal() = ";
		return m + 1;
	}
};

int main(){
	const A a(1);
	const B b(3);
	const A *pA[2] = {&a, &b};
	std::cout << pA[0]->getVal() << std::endl; // A::getVal() = 1
	std::cout << pA[1]->getVal() << std::endl; // B::getVal() = 4

	std::vector<A> vA;
	vA.push_back(a);
	vA.push_back(b);
	std::cout << vA[0].getVal() << std::endl; // A::getVal() = 1
	std::cout << vA[1].getVal() << std::endl; // A::getVal() = 3

	const A &c = b;
	std::cout << c.getVal() << std::endl;	 // B::getVal() = 4

	return 0;
}
```
从执行结果可以看到, 把 derived object 放到base object 的 vector 中, 就真的成了base object(通过往B 中添加新的member, 然后求sizeof 可以验证).

Deleting an array of objects using a base class pointer is undefined
```C++
A *a = new B[10];
delete []a;
```
因为a 是一个数组, `a[1] = *(a + 1)`, 而a 是A 类型的, 所以a + 1 实际上偏移的是 sizeof(A), 与我们实际需要的不符.

### parameter of an overriding function
虚函数body 可以被覆盖掉, 但是默认参数还是使用base class 中声明的.
```C++
class A{
public:
	virtual ~A() {}
	virtual int foo(int x = 99){
		std::cout << "A:foo" << std::endl;
		return x;
	}
};

class B : public A{
public:
	int foo(int x = 77){
		std::cout << "B:foo" << std::endl;
		return x;
	}
};

int main(int argc, char** argv){
	A* a = new B;
	std::cout << a->foo() << std::endl; // output B:foo and 99
	delete a;

	return 0;
}
```

### 虚析构函数
- Constructors can't be virtual! 因为构造函数被invoked 时候, 说明object 还未创建完成, 也就不能做到虚函数的runtime dynamic binding.
- Virtual Destructor can be pure, but we must provide a function body for the pure virtual destructor. 作用仅仅是在没有其他纯虚函数的情况下, 指名这个class 不能被实例化

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

### 在非虚函数中调用虚函数
```C++
class RowOrientedBenchmark{
public:
	virtual ~RowOrientedBenchmark(){}
	virtual void DoRow(int i){
		LOG(INFO) << "DoRow in RowOrientedBenchmark";
	}
	void run(){
		for(int i = 0; i < 10; ++i){
			DoRow(i);
		}
	}
};

class ReadBenchmark : public RowOrientedBenchmark{
public:
	void DoRow(int i){
		LOG(INFO) << "DoRow in ReadBenchmark";
	}
};

std::unique_ptr<RowOrientedBenchmark> bc1(new RowOrientedBenchmark);
bc1->run(); // output "DoRow in RowOrientedBenchmark"

std::unique_ptr<RowOrientedBenchmark> bc2(new ReadBenchmark);
bc2->run(); // output "DoRow in ReadBenchmark"
```
通过这种方式可以看到一个明显的优点, 先在base 中把框架确定下来(这里就是run), 然后再子类中具体实现怎么做.

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

```C++
class Worker{
public:
	std::string name;
};

class Student: public virtual Worker{
public:
	int studentID;
};

class Assistant: virtual public Worker{
public:
	int employerID;
};

class StudentAssitant: public Student, public Assistant {};
```
Now a StudentAssitant object will contain a single copy of a Worker. Actually, the inherited Student and Assitant objects share a common Worker object instead of each carrying its own copy.

Here, the virtual key word does not have any obvious connection to the virtual in virtual functions.

