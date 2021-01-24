- [const](#const)
  - [more const in C++](#more-const-in-c)
    - [const global variable](#const-global-variable)
    - [const function](#const-function)
    - [const class member](#const-class-member)
  - [STL iterators](#stl-iterators)
- [static](#static)
  - [static全局变量](#static全局变量)
  - [static局部变量](#static局部变量)
  - [static函数](#static函数)
    - [静态数据成员/成员函数(C++特有)](#静态数据成员成员函数c特有)
      - [Static - Singleton Pattern](#static---singleton-pattern)
- [goto](#goto)
- [extern](#extern)
- [volatile](#volatile)
- [mutable](#mutable)
  - [类中的 mutable](#类中的-mutable)
- [define](#define)

# const
[C++ TUTORIAL: KEYWORD - 2018](http://www.bogotobogo.com/cplusplus/cplusplus_keywords.php#const)

const in C does not mean something is constant. It just means a variable is read-only.

For pointers, we can specify whether the pointer itself is const, the data it points to is const, both, or neither:

```C++
char str[] = "constantness";
char* p = str;  // non-const pointer to non-const data
const char* pc = str;  // non-const pointer to const data
char* const cp = str;  // const pointer to non-const data
const char* const cpc = str;  // const pointer to const data
```
When const appears to the left of the `*`, what's pointed to is constant,
and if const appears to the right of the `*`, the pointer itself is constant.
If const appears on both sizes, both are constants.

const并不会阻止参数的修改, 防君子不防小人, 可以强制的把 `const char *` 转换为 `char *`类型, 然后就可以修改了

## more const in C++
### const global variable
在 C++(但不是在 C 语言)中, const 限定符对默认存储类型稍有影响.
在默认情况下, 全局变量的链接性为外部的, 但 const 全局变量的链接性为内部的.也就是说,在 C++ 看来, 全局 const 定义就像使用了 static 说明符一样.
因此,可以将 const 常量定义在头文件中供工程中的多个其它文件包含引用, 并且编译时不会产生变量重复定义的错误. 当然,也可以用 #define 宏定义.

However, if we want to make a constant have external linkage, we can use the extern keyword to override the default internal linkage: `extern const int a = 20;`

### const function
一个函数名后面加const
在函数名后面表示是常成员函数, 该函数不能修改对象内的任何成员(attention: static member 不是对象的成员, 因此可以修改, mutable 标记的member可以修改), 只能发生读操作, 不能发生写操作.
我们都知道在调用成员函数的时候编译器会将对象自身的地址作为隐藏参数传递给函数, 在const成员函数中,既不能改变this所指向的对象,也不能改变this所保存的地址,this的类型是一个指向const类型对象的const指针.

一个函数名字后有const, 这个函数必定是成员函数,也就是说普通函数后面不能有const修饰

Const object can not call non-const member functions(因为non-const member functions 可以修改成员变量, 导致object 就不是const 的了).

The example below shows that a const member function can be overloaded with a non-const member function that has the same parameter list.
In this case, the constness of the class object determines which of the two functions is invoked:
```C++
#include <iostream>
using namespace std;

class Testing {
 public:
  Testing(int n) : val(n) {}
  int getVal() const {
    cout << "getVal() const" << endl;
    return val;
  }
  int getVal() {
    cout << "getVal() non-const" << endl;
    return val;
  }

 private:
  int val;
};

int main() {
  const Testing ctest(10);
  Testing test(20);
  ctest.getVal(); // output: getVal() const
  test.getVal();  // getVal() non-const
  return 0;
}
```

### const class member
成员变量加const: const 成员, 只能在构造函数中的初始化列表中初始化

## STL iterators
Since STL iterators are modeled on pointers, an iterator behaves mush like a `T *` pointer.
So, declaring an iterator as const is like declaring a pointer const. If we want an iterator that points to something that can't be altered (`const T *`), we want to use a `const_iterator`:
```C++
vector<int> v;
vector<int>::const_iterator itc = v.begin();
*itc = 2012;  // error: *itc is cost
itc++;  // ok, itc is not const
```

How about `T* const iterator`:
```C++
vector<int> v;
const vector<int>::iterator cit = v.begin;
*cit = 2012;                                   // ok
cit++;                                         // error: cit is const
```

# static
- [C语言中的static 详细分析](http://blog.csdn.net/keyeagle/article/details/6708077/)
- [static在C和C++中的用法和区别](http://blog.csdn.net/skyereeee/article/details/8000512)

c语言代码是以文件为单位来组织的,在一个源程序的所有源文件中,一个外部变量(注意不是局部变量)或者函数只能在一个源程序中定义一次,如果有重复定义的话编译器就会报错.
伴随着不同源文件变量和函数之间的相互引用以及相互独立的关系,产生了extern和static关键字.

一个进程在内存中的布局

```plain
----------
栈区
----------
堆栈增长区
----------
堆区
----------
其他段
----------
.bbs 段
----------
.data 段
----------
.text 段
----------
```
其中

- .text段保存进程所执行的程序二进制文件,
- .data段保存进程所有的已初始化的全局变量
- .bss段保存进程未初始化的全局变量(其他段中还有很多乱七八糟的段,暂且不表).

在进程的整个生命周期中,.data段和.bss段内的数据时跟整个进程同生共死的,也就是在进程结束之后这些数据才会寿终就寝.

## static全局变量
当一个进程的全局变量被声明为static之后,就是静态全局变量.
静态全局变量和其他的全局变量的存储地点并没有区别,都是在.data段(已初始化)或者.bss段(未初始化)内,但是它只在定义它的源文件内有效,其他源文件无法访问它.

## static局部变量
普通的局部变量在栈空间上分配,这个局部变量所在的函数被多次调用时,每次调用这个局部变量在栈上的位置都不一定相同.
局部变量也可以在堆上动态分配,但是记得使用完这个堆空间后要释放之.

static局部变量中文名叫静态局部变量.它与普通的局部变量比起来有如下几个区别:

1. 位置:静态局部变量被编译器放在全局存储区.data(注意:不在.bss段内,原因见3)),所以它虽然是局部的,但是在程序的整个生命周期中存在.
2. 访问权限:静态局部变量只能被其作用域内的变量或函数访问.也就是说虽然它会在程序的整个生命周期中存在,由于它是static的,它不能被其他的函数和源文件访问.
3. 值:静态局部变量如果没有被用户初始化,则会被编译器自动赋值为0,以后每次调用静态局部变量的时候都用上次调用后的值.
  这个比较好理解,每次函数调用静态局部变量的时候都修改它然后离开,下次读的时候从全局存储区读出的静态局部变量就是上次修改后的值.

需要注意的是由于static局部变量的这种特性,使得含静态局部变量的函数变得不可重入,即每次调用可能会产生不同的结果.这在多线程编程时可能会成为一种隐患.需要多加注意.

## static函数
相信大家还记得C++面向对象编程中的private函数,私有函数只有该类的成员变量或成员函数可以访问.
在C语言中,也有"private函数",它就是接下来要说的static函数,完成面向对象编程中private函数的功能.

当你的程序中有很多个源文件的时候,你肯定会让某个源文件只提供一些外界需要的接口,
其他的函数可能是为了实现这些接口而编写,这些其他的函数你可能并不希望被外界(非本源文件)所看到,这时候就可以用static修饰这些"其他的函数".

所以static函数的作用域是本源文件,把它想象为面向对象中的private函数就可以了.

### 静态数据成员/成员函数(C++特有)
[STATIC VARIABLES AND STATIC CLASS MEMBERS - 2018](http://www.bogotobogo.com/cplusplus/statics.php)

C++ 重用了这个关键字,并赋予它与前面不同的第三种含义: 表示属于一个类而不是属于此类的任何特定对象的变量和函数.
比如在对某一个类的对象进行计数时, 计数生成多少个类的实例, 就可以用到静态数据成员.

- static members exist as members of the class rather than as an instance in each object of the class. 不占用sizeof 的结果
- this keyword is not available in a static member function.
- Static member function, it can only access static member data(including static const variable), or other static member functions while non-static member functions can access all data members of the class: static and non-static.
- A non-static member function can be declared as virtual but care must be taken not to declare a static member function as virtual.
- 不需要经由类的对象来调用.(虽然使用类的对象来调用也是合法的, 编译器不会报错)

Note again that we cannot initialize a static member variable inside the class declaration.
That's because the declaration is a description of how memory is to be allocated, but it doesn't allocate memory.
We allocate and initialize memory by creating an object using that format.

The exception to the initialization of a static data member inside the class declaration is if the static data member is a const of integral or enumeration type. 但是这能二选一

```C++
class Car {
 public:
  enum Color {silver = 0, maroon, red };
  int year;
  int mileage = 34289;  // error: not-static data members, only static const integral data members can be initialized within a class
  static int vin = 12345678;  // error: non-constant data member, only static const integral data members can be initialized within a class
  static const string model = "Sonata";  // error: not-integral type cannot have in-class initializer
  static const int engine = 6;  // allowed: static const integral type(二选一)
};

int Car::year = 2013;                      // error: non-static data members cannot be defined out-of-class
int Car::vin = 1234;                       // ok
const int engine = 6;                      // allowed: static const integral type(二选一)

int main() {
  return 0;
}
```

#### static - singleton pattern
Singleton design pattern is a good example of static member function and static member variable.
In this pattern, we put constructor in private section not in public section of a class. So, we can not access the constructor to make an instance of the class.
Instead, we put a public function which is static function. The getInstance() will make an instance only once.
Note that if this method is not static, there is no way to invoke the getInstance() even though it is public method. That's because we do not have any instance of Singleton.

```C++
#include <iostream>
using namespace std;

class Singleton {
 public:
  static Singleton* getInstance() {
    if (!instance) {
      instance = new Singleton();
      cout << "getInstance(): First instance\n";
      return instance;
    }
    else {
      cout << "getInstance(): previous instance\n";
      return instance;
    }
  }

private:
  Singleton() {}
  static Singleton* instance;
};
Singleton* Singleton::instance = nullptr;

int main() {
  Singleton* s1 = Singleton::getInstance();
  Singleton* s2 = Singleton::getInstance();

  return 0;
}
```
output
```plain
getInstance(): First instance
getInstance(): previous instance
```

# goto
在goto 之后是不允许定义的新的变量的, 局部变量也不行.
[crosses initialization error](http://stackoverflow.com/questions/14274225/statement-goto-can-not-cross-pointer-definition)

# extern
- extern可以置于变量或者函数前,以标示变量或者函数的定义在别的文件中,提示编译器遇到此变量和函数时在其他模块中寻找其定义
- 当它与"C"一起连用时, 如: `extern "C" void fun(int a, int b)`; 则告诉编译器按照C的规则去翻译相应的函数名而不是C++的, C++在翻译这个函数名时会把fun这个名字变得面目全非
- 在一个cpp文件定义了一个数组:`char a[6]`, h文件声明为`extern char a[]`, 而不是`extern char *a`, 两者的类型不一样

# volatile
volatile 指示变量随时可能发生变化的, 每次使用时都需要去内存里重新读取它的值, 与该变量有关的运算, 不要进行编译优化

[谈谈 C/C++ 中的 volatile](https://liam.page/2018/01/18/volatile-in-C-and-Cpp/)

程序中 volatile 的含义

对于程序员来说,程序本身的任何行为都必须是可预期的.那么,在程序当中,什么才叫 volatile 呢?
这个问题的答案也很简单:程序可能受到程序之外的因素影响.

考虑以下 C/C++ 代码.
```cpp
volatile int *p = /* ... */;
int a, b;
a = *p;
b = *p;
```
若忽略volatile ,那么p 就只是一个指向int 类型的指针. 这样一来,`a = *p` 和 `b = *p` 两句,就只需要从内存中读取一次就够了.
因为从内存(这里包括cpu cache 和main memory)中读取一次之后,CPU 的寄存器中就已经有了这个值,把这个值直接复用就可以了.
这样一来,编译器就会做优化,把两次访存的操作优化成一次.
这样做是基于一个假设:我们在代码里没有改变 p 指向内存地址的值,那么这个值就一定不会发生改变.

然而,由于MMIP(Memory mapped I/O)的存在,这个假设不一定是真的.例如说,假设p 指向的内存是一个硬件设备.这样一来,从 p 指向的内
存读取数据可能伴随着可观测的副作用:硬件状态的修改.此时,代码的原意可能是将硬件设备返回的连续两个int 分别保存在a 和b 当中.
这种情况下,编译器的优化就会导致程序行为不符合预期了.

总结来说,被 volatile 修饰的变量,在对其进行读写操作时,会引发一些可观测的副作用.而这些可观测的副作用,是由程序之外的因素决
定的.

> [cv (const and volatile) type qualifiers](https://en.cppreference.com/w/cpp/language/cv)

> Every access (read or write operation, member function call, etc.) made through a glvalue expression of
> volatile-qualified type is treated as a visible side-effect for the purposes of optimization (that is, within a single
> thread of execution, volatile accesses cannot be optimized out or reordered with another visible side effect that is
> sequenced-before or sequenced-after the volatile access. This makes volatile objects suitable for communication with a
> signal handler, but not with another thread of execution, see std::memory_order). Any attempt to refer to a volatile
> object through a non-volatile glvalue (e.g. through a reference or pointer to non-volatile type) results in undefined
> behavior.

volatile 可以解决多线程中的某些问题,这一错误认识荼毒多年.例如,在知乎「volatile」话题下的介绍就是「多线程开发中保持可见性
的关键字」.为了拨乱反正,这里先给出结论(注意这些结论都基于本文第一节提出的约定之上):**volatile 不能解决多线程中的问题**.

按照 [Hans Boehm & Nick Maclaren 的总结](
http://web.archive.org/web/20180120044239/http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2016.html),
volatile 只在三种场合下是合适的.

- 和信号处理(signal handler)相关的场合,
- 和内存映射硬件(memory mapped hardware)相关的场合,
- 和非本地跳转(setjmp 和 longjmp)相关的场合.

以下我们尝试来用 volatile 关键字解决多线程同步的一个基本问题:happens-before.

## code
### naive case
首先我们考虑这样一段(伪)代码.
```cpp
// global shared data
bool flag = false;

thread1() {
  flag = false;
  Type* value = new Type(/* parameters */);
  thread2(value);
  while (true) {
    if (flag == true) {  // (1)
      apply(value);
      break;
    }
  }
  thread2.join();
  if (nullptr != value) { delete value; }
  return;
}

thread2(Type* value) {
  // do some evaluations
  value->update(/* parameters */);  // (2)
  flag = true;
  return;
}
```
这段代码将 thread1 作为主线程, 等待 thread2 准备好value.
因此,thread2 在更新 value 之后将 flag 置为真,而 thread1 死循环地检测 flag.
简单来说,这段代码的意图希望实现 thread2 在 thread1 使用 value 之前执行完毕这样的语义.

对多线程编程稍有了解的人应该知道,这段代码是有问题的.问题主要出在两个方面.

1. 在thread1 中,flag = false 赋值之后,在 while 死循环里,没有任何机会修改 flag 的值,因此在运行之前,编译器优化可能会将
  if (flag == true) 的内容全部优化掉.
1. 在thread2 中,尽管逻辑上 update 需要发生在 flag = true 之前,但编译器和 CPU 并不知道,因此编译器优化和 CPU 乱序执行可能
  会使 flag = true 发生在 update 完成之前,因此 thread1 执行 apply(value) 时可能 value 还未准备好.

### 加一个 volatile 试试?
把flag 的声明改为
```cpp
volatile bool flag = false;
```
其他地方不动. 这样, (1) 处, 由于 flag == true 是对volatile 变量的访问, 故而if-block 不会被优化掉, 然后尽管flag 是
volatile-qualified, 但value 并不是.
因而编译器仍有可能将 (2) 处的update 和对flag 的赋值交换顺序.
此外, 由于 volatile 禁止了编译器对 flag 的优化,这样使用 volatile 不仅无法达成目的,反而会导致性能下降.

### 再加一个 volatile 呢?
在错误的理解中,可能会对 value 也加以 volatile 关键字修饰,颇有些「没有什么是一个 volatile 解决不了的,如果不行,那就两个」
的意思.
```cpp
volatile bool flag = false;
volatile Type* value = new Type();
```
因此 (2) 处对两个 volatile-qualified 变量进行访问时,编译器不会交换他们的顺序.看起来就万事大吉了.

然而,volatile 只作用在编译器上,但我们的代码最终是要运行在 CPU 上的.尽管编译器不会将 (2) 处换序,但 CPU 的乱序执行
(out-of-order execution)已是几十年的老技术了,在 CPU 执行时,value 和 flag 的赋值仍有可能是被换序了的(store-store).

### 到底应该怎样做?
回顾一下,我们最初遇到的问题其实需要解决两件事情.一是 flag 相关的代码块不能被轻易优化消失,二是要保证线程同步的
happens-before 语义. 但本质上,设计使用 flag 本身也就是为了构建 happens-before 语义.这也就是说,两个问题,后者才是核心,如有
其他不用 flag 的办法解决问题,那么 flag 就不重要.

对于当前问题,最简单的办法是使用原子操作.
```cpp
std::atomic<bool> flag{false};
```
由于对 `std::atomic<bool>` 的操作是原子的,同时构建了良好的内存屏障,因此整个代码的行为在标准下是良定义的.

除此之外,还可以结合使用互斥量和条件变量来避免 while 死循环空耗 CPU 的情况.

## const vs volatile
const, volatile的作用以及起作用的阶段

- const只在编译期有用,在运行期无用, const在编译期保证在C的"源代码"里面,没有对其修饰的变量进行修改的地方(如有则报错,编译
  不通过),而运行期该变量的值是否被改变则不受const的限制.
- volatile在编译期和运行期都有用, 在编译期告诉编译器:请不要做自以为是的优化,这个变量的值可能会变掉, 在运行期:每次用到该
  变量的值,都从内存中取该变量的值.
- const 和 volatile 不是反义词, 可以同时修饰一个变量, 表示一个变量在程序编译期不能被修改且不能被优化,在程序运行期,变量值
  可修改,但每次用到该变量的值都要从内存中读取,以防止意外错误.

# mutable
ref: [C++ 中mutable 关键字存在的必要性是什么?](https://www.zhihu.com/question/64969053/answer/226383958)

## 类中的 mutable
mutable 从字面意思上来说,是「可变的」之意.

若是要「顾名思义」,那么这个关键词的含义就有些意思了.显然,「可变的」只能用来形容变量,而不可能是「函数」或者「类」本身.
然而,既然是「变量」,那么它本来就是可变的,也没有必要使用 mutable 来修饰.那么,mutable 就只能用来形容某种不变的东西了.

C++ 中,不可变的变量,称之为常量,使用 const 来修饰.然而,若是 const mutable 联用,未免让人摸不着头脑-到底是可变还是不可变呢?

事实上,mutable 是用来修饰一个 const 示例的部分可变的数据成员的.如果要说得更清晰一点,就是说 mutable 的出现,将 C++ 中的 const 的概念分成了两种.

- 二进制层面的 const,也就是「绝对的」常量,在任何情况下都不可修改(除非用 `const_cast`).
- 引入 mutable 之后,C++ 可以有逻辑层面的 const,也就是对一个常量实例来说,从外部观察,它是常量而不可修改,但是内部可以有非常量的状态.
  当然,所谓的「逻辑 const」,只是为了方便理解,而创造出来的名词.
  显而易见,mutable 只能用来修饰类的数据成员,而被 mutable 修饰的数据成员,可以在 const 成员函数中修改.

这里举一个例子,展现这类情形.

```C++
class HashTable {
 public:
  // caution: not thread safe
  std::string lookup(const std::string& key) const {
    if (key == last_key_) {
      return last_value_;
    }

    std::string value{this->lookupInternal(key)};

    last_key_ = key;
    last_value_ = value;

    return value;
  }

 private:
  mutable std::string last_key_;
  mutable std::string last_value_;
};
```
这里,我们呈现了一个哈希表的部分实现.显然,对哈希表的查询操作,在逻辑上不应该修改哈希表本身.因此,HashTable::lookup 是一个 const 成员函数.
在 HashTable::lookup 中,我们使用了 `last_key_` 和 `last_value_` 实现了一个简单的「缓存」逻辑.
当传入的 key 与前次查询的 `last_key_` 一致时,直接返回 `last_value_`,否则,则返回实际查询得到的 value 并更新 `last_key_` 和 `last_value_`.

在这里,`last_key_` 和 `last_value_` 是 HashTable 的数据成员.按照一般的理解,const 成员函数是不允许修改数据成员的.
但是,另一方面,`last_key_` 和 `last_value_` 在类的外部是看不到的,从逻辑上说,修改它们的值,外部是无有感知的,因此也就不会破坏逻辑上的 const.
为了解决这一矛盾,我们用 mutable 来修饰 `last_key_` 和 `last_value_`,以便在 lookup 函数中更新缓存的键值.

# define
- `#str`: 生成`"str"`
- `std::c##out << "str"`: 相当于`std::cout << "str"`
- `\`是用来续行的
- `#@`给参数加上单引号.

