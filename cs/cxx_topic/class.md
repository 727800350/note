# constructor and operator
```C++
class Empty {}; // sizeof(Empty) 为 1
```
Thanks to C++ compiler, actually it becomes something like this:
```C++
class Empty {
 public:
  Empty() {}  // default constructor
  Empty(const Empty&) {}  // copy constructor
  ~Empty() {}  // destructor
  Empty& operator=(const Empty&) {  // assignment operator
    return *this;
  }
};
```

```C++
Empty eA;  // default constructor
~Empty();  // destructor
Empty eB(eA);  // copy constructor(creating a new object)
eB = eA;  // assignment operator(assign to an existing object)
Empty eC = eA;  // copy constructor(creating a new object)
```

## operator=
赋值运算符(`operator=`)返回类型一般声明为类型的引用, 并在函数结尾时返回实例自身的引用(即`*this`).这里主要有两个原因:

1. 返回引用可以减少一次拷贝构造和析构函数导致不必要的开销,因为如果返回值类型不是引用,会创建一个匿名对象,这个匿名对象时个右值,获取return的值.
1. 可以实现连续赋值, 在例子中 `b = c = a;`, 运算顺序 `b = (c = a);` 返回值不是引用类型也是可以的, c得到一个右值,再将右值赋给b,所以逻辑上没有问题的.
  但是如果是 `(b = c) = a;` 这里将会出错,`b = c`后b是一个右值,所以`b = a`时将会出错.

## copy contructor
The copy constructor should have one of the following forms:

- `MyClass(const MyClass&);`
- `MyClass(MyClass&);`
- `MyClass(volatile MyClass&);`
- `MyClass(volatile const MyClass&);`

copy constructor 必须以引用的方式传递参数.
因为,在值传递的方式传递给一个函数的时候,会调用拷贝构造函数生成函数的实参.如果拷贝构造函数的参数仍然是以值的方式,就会无限循环的调用下去,直到函数的栈溢出.
**拷贝构造函数使用传入对象的值生成一个新的对象的实例,而赋值运算符是将对象的值复制给一个已经存在的实例**.

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

  Complx& operator=(const Complx& c) {
    if (this == &c) {
      return *this;
    }
    real = c.real;
    imag = c.imag;
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const Complx& c); // friend 标记不能少

  double size() const {
    return sqrt(real * real + imag * imag);
  }

 private:
  double real;
  double imag;
};

std::ostream& operator<<(std::ostream& os, const Complx& c){
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
Complx c6 = c1 + c2;  // #1
```
But the definition also allows us to use the following:
```C++
Complx c7;
c1 + c2 = c7;  // #2
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

# 初始化列表
构造函数的执行可以分成两个阶段,初始化阶段和计算阶段,初始化阶段先于计算阶段.

- 初始化阶段: 所有类类型(class type)的成员都会在初始化阶段初始化,即使该成员没有出现在构造函数的初始化列表中.
- 计算阶段: 一般用于执行构造函数体内的赋值操作

成员是按照他们在类中出现的顺序进行初始化的, 而不是按照他们在初始化列表出现的顺序初始化的.

```C++
class Foo {
 public:
  Foo() {
    LOG(INFO) << "default construct for Foo";
  }

  Foo(const Foo& foo) {
    LOG(INFO) << "copy constructor for Foo";
    this->a_ = foo.a_;
  }

  Foo& operator=(const Foo& foo) {
    LOG(INFO) << "assignment for Foo";
    this->a_ = foo.a_;
    return *this;
  }

 private:
  int a_;
};
```

不使用初始化列表时
```C++
class Bar {
 public:
  Bar(const Foo &foo) {
    foo_ = foo;
  }

 private:
  Foo foo_;
};

Foo foo;
Bar bar(foo);
```

```log
I0825 09:31:59.868443 167398 main_test.cc:16] default construct for Foo
I0825 09:31:59.868752 167398 main_test.cc:16] default construct for Foo
I0825 09:31:59.868777 167398 main_test.cc:25] assignment for Foo
```

使用初始化列表
```C++
class Bar {
 public:
  Bar(const Foo &foo) : foo_(foo) {}

 private:
  Foo foo_;
};

Foo foo;
Bar bar(foo);
```

```log
I0825 09:33:34.305039 167620 main_test.cc:16] default construct for Foo
I0825 09:33:34.305356 167620 main_test.cc:20] copy constructor for Foo
```
可以看到使用初始化列表之后, 可以减少一次构造函数的调用

# Virtual Function
- Once a function is declared as virtual, it remains virtual in all the dervied classes. 既不需要在dervied class 中显式指定virtual
- 虚函数不能定义为内联函数, inline是在编译器将函数类容替换到函数调用处, 是静态编译的, 而虚函数是动态调用的, 在编译器并不知道需要调用的是父类还是子类的虚函数, 所以不能够inline声明展开, 所以编译器会忽略

## [虚函数的实现的基本原理](https://www.cnblogs.com/malecrab/p/5572730.html)
简单地说,每一个含有虚函数(无论是其本身的,还是继承而来的)的类都至少有一个与之对应的虚函数表,其中存放着该类所有的虚函数对应的函数指针.例:
![内存布局](./pics/virtual_function_table.png)

其中(为了描述方便,本文在探讨对象内存布局时,将忽略内存对齐对布局的影响)

- B的虚函数表中存放着B::foo和B::bar两个函数指针.
- D的虚函数表中存放的既有继承自B的虚函数B::foo,又有重写(override)了基类虚函数B::bar的D::bar,还有新增的虚函数D::quz.

从编译器的角度来说, B的虚函数表很好构造, D的虚函数表构造过程相对复杂. 下面给出了构造D的虚函数表的一种方式(仅供参考):
![虚函数表构建过程](./pics/build_virtual_function_table.png)

### parameter of an overriding function
虚函数body 可以被覆盖掉, 但是默认参数还是使用base class 中声明的.
```C++
class A {
 public:
  virtual ~A() {}
  virtual int foo(int x = 99) {
    std::cout << "A:foo" << std::endl;
    return x;
  }
};

class B : public A {
 public:
  int foo(int x = 77) {
    std::cout << "B:foo" << std::endl;
    return x;
  }
};

int main(int argc, char* argv[]) {
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
class Base {
 public:
  Base() {
    LOG(INFO) << "constructor in base";
  };
  virtual ~Base() {
    LOG(INFO) << "destructor in base";
  };

  virtual void faire() {
    LOG(INFO) << "do something in base";
  };
};

class Derived : public Base {
 public:
  Derived() {
    LOG(INFO) << "constructor in derived";
  };
  ~Derived() {
    LOG(INFO) << "destructor in derived";
  }

  void faire() {
    LOG(INFO) << "do something in derived";
  }
};
```

```C++
Base* instance = new Derived();
instance->faire();
delete instance;
```
在将 `~Base` 声明为虚函数时, 上面的代码会输出:
```log
I0605 15:45:05.518903   159 test.cpp:14] constructor in base
I0605 15:45:05.519145   159 test.cpp:28] constructor in derived
I0605 15:45:05.519162   159 test.cpp:35] do something in derived
I0605 15:45:05.519176   159 test.cpp:31] destructor in derived
I0605 15:45:05.519187   159 test.cpp:17] destructor in base
```
可以看到, 析构的时候, 先调用的Derived 的析构函数, 然后再调用的Base 的析构函数.

如果将 `~Base` 不声明为虚函数, 上面的代码会输出
```log
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
  virtual ~RowOrientedBenchmark(){ }
  virtual void DoRow(int i) {
    LOG(INFO) << "DoRow in RowOrientedBenchmark";
  }
  void run() {
    for(int i = 0; i < 10; ++i){
      DoRow(i);
    }
  }
};

class ReadBenchmark : public RowOrientedBenchmark {
 public:
  void DoRow(int i) {
    LOG(INFO) << "DoRow in ReadBenchmark";
  }
};

auto bc1 = std::make_unique<RowOrientedBenchmark>();
bc1->run(); // output "DoRow in RowOrientedBenchmark"

auto bc2 = std::make_unique<RowOrientedBenchmark>();
bc2->run(); // output "DoRow in ReadBenchmark"
```
通过这种方式可以看到一个明显的优点, 先在base 中把框架确定下来(这里就是run), 然后再子类中具体实现怎么做.

### 子类调用父类的方法
```C++
class A {
 public:
  virtual ~A() {}
  void faire() {
    LOG(INFO) << "A faire";
  }
  virtual void say() {
    LOG(INFO) << "A say";
  }
};

class B : public A {
 public:
  void faire() {
    A::faire();
    LOG(INFO) << "B faire";
  }
  /* 如果派生类在虚函数声明时使用了override描述符,那么该函数必须重载其基类中的同名函数,否则代码将无法通过编译. 不加也可以, 只是加了编译检查会更严格.
   * 如果是声明和实现分开到.h 和 .cpp 文件中, 声明可以加override 关键字, 但是实现不能加.
   */
  void say() override {
    A::say();
    LOG(INFO) << "B say";
  }
};

std::unique_ptr<A> a(new B);
a->faire(); // output "A faire"
a->say(); // output "A say" "B say"

std::unique_ptr<B> b(new B);
b->faire(); // output "A faire" "B faire"
b->say(); // output "A say" "B say"
```
在子类的虚函数中也可以调用父类的同名函数.

# 嵌套类
嵌套中的局部类可以访问外层的private 元素(包括变量和函数), eg:
```C++
class A {
 public:
  void say() {
    LOG(INFO) << "this is A";
  }

  class B {
   public:
    void faire() {
      A a(2);
      a.say();
      LOG(INFO) << a.x_;
    }
  };

 private:
  A(int x) : x_(x) {}

 private:
  int x_;
};

A::B b;
b.faire();

// A a; // compile error, as A's constructor is private
```

## 派生
派生类构造函数的语法:
```info
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
```info
class 派生类名::virtual 继承方式 基类名,
```

```C++
class Worker {
 public:
  std::string name;
};

class Student: public virtual Worker {
 public:
  int studentID;
};

class Assistant: virtual public Worker {
 public:
  int employerID;
};

class StudentAssitant: public Student, public Assistant {};
```
Now a StudentAssitant object will contain a single copy of a Worker. Actually, the inherited Student and Assitant
objects share a common Worker object instead of each carrying its own copy.

Here, the virtual key word does not have any obvious connection to the virtual in virtual functions.

