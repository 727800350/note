# function pointer
ref: [深入理解C语言函数指针](http://www.cnblogs.com/windlaughing/archive/2013/04/10/3012012.html)

```C++
int myFun(int x) {
  fprintf(stdout, "myFun: %d\n", x);
  return x;
}
```
一个数据变量的内存地址可以存储在相应的指针变量中,函数的首地址也可以存储在某个函数指针变量中. 这样,我就可以通过这个函数指针变量来调用所指向的函数了.

在C系列语言中,任何一个变量,总是要先声明,之后才能使用的.函数指针变量也应该要先声明.
```C++
// 声明一个指向一个接受int 参数, 返回值为int的函数指针变量.
int (*funP)(int);
funP = myFun;
```
实际上和 `int a` 差不多, 也就是类型是`int (*)(int)`, 名字是funP, myFun 也是这个类型.
```C++
typedef int(*FUN)(int); // 将FUN 定义为一个类型, 和 typedef int a 一样
```

```C++
void (*funP)(int); // 也可写成void(*funP)(int x),但习惯上一般不这样.
void (*funA)(int);
void myFun(int x);

//一般的函数调用
myFun(100);

// myFun与funP的类型关系类似于int 与int *的关系.
funP = &myFun; // 将myFun函数的地址赋给funP变量
(*funP)(200); // 通过函数指针变量来调用函数

//myFun与funA的类型关系类似于int 与int 的关系.
funA = myFun;
funA(300);

//三个貌似错乱的调用
funP(400);
(*funA)(600);
(*myFun)(1000);
```

总结:

- 其实, myFun的函数名与funP, funA函数指针都是一样的, 即都是函数指针. myFun函数名是一个函数指针常量,而funP,funA是函数数指针变量,这是它们的关系.
- 但函数名调用如果都得如`(*myFun)(10)`这样, 那书写与读起来都是不方便和不习惯的. 所以C语言的设计者们才会设计成又可允许myFun(10)这种形式地调用(这样方便多了,并与数学中的函数形式一样).
- 为了统一调用方式, funP函数指针变量也可以funP(10)的形式来调用.
- 赋值时,可以写成`funP = &myFun`形式, 也可以写成`funP = myFun`.
- 但是在声明时, `void myFun(int)` 不能写成`void (*myFun)(int )`. `void (*funP)(int)`不能写成`void funP(int)`.
- 函数指针变量也可以存入一个数组内.数组的声明方法:`int (*fArray[10])(int)`;

- `printf("myFun\t0x%p, 0x%p\n", &myFun, myFun);`: 输出两个地址是一样的
- `typedef void(*FunType)(int);`: 前加一个typedef关键字, 这样就定义一个名为FunType函数指针类型, 而不是一个FunType变量

关于参数

- 编译期检查返回值类型是否匹配
- 如果声明时不指定参数类型,则编译和运行时都不会检查参数列表,可以多传参数,也可以少传参数.输入结果未知.
- 如果声明是指定参数列表,则会在编译期检查赋值函数的参数列表是否匹配

# lambda
当我们编写了一个lambda 后, 编译器将该表达式翻译成一个未命名类的未命名对象. 这个类含有一个重载的函数调用运算符.

- 当一个lambda 表达式通过引用捕获变量时, 将由程序负责确保lambda执行时, 引用所引的对象确实存在.
- 通过值捕获的变量将拷贝到lambda 中, 因此这种lambda 产生的类必须为每个捕获的变量建立对应的数据成员, 同时构建构造函数.

`[ capture ] ( params ) { body }`

- capture 指定了在可见域范围内 lambda 表达式的代码内可见得外部变量的列表,具体解释如下
  - `[]`: 未定义变量.试图在Lambda内使用任何外部变量都是错误的.
  - `[x, &y]`: x 按值捕获, y 按引用捕获.
  - `[&]`: 用到的任何外部变量都隐式按引用捕获
  - `[=]`: 用到的任何外部变量都隐式按值捕获
  - `[&, x]`: x显式地按值捕获. 其它变量按引用捕获
  - `[=, &z]`: z按引用捕获. 其它变量按值捕获
- params 指定 lambda 表达式的参数

```C++
[] () { return 1; } // compiler knows this returns an integer
[] () -> int { return 1; } // now we're telling the compiler what we want
```

# std::function and std::bind
[C++11 中的std::function和std::bind](https://www.jianshu.com/p/f191e88dcc80)

std::function 是一个可调用对象包装器,是一个类模板, 可以容纳除了类成员函数指针之外的所有可调用对象

std::bind 是一个通用的函数适配器,它接受一个可调用对象,生成一个新的可调用对象来"适应"原对象的参数列表, 绑定后的结果可以使用std::function保存.
借助std::bind 的返回值为std::function 类型的特性, 可以将任意函数, 包括class member function, class static function 转换为std::function.

# Closure
closure implementation using lambda:
```C++
#include <functional>
std::function<int(void)> func(int x) {
  int state = x;
  return [&]() {
    return state++;
  };
}

auto f = func(100);
fprintf(stdout, "%d\n", f()); // 100
fprintf(stdout, "%d\n", f()); // 101
fprintf(stdout, "%d\n", f()); // 102
```

# Functors: Function Objects in C++
```C++
class myFunctorClass {
 public:
  myFunctorClass(int x) : x_(x) {}
  // C++ allows you to overload operator(), the "function call" operator
  int operator()(int y) {
    return x_ + y;
  }

 private:
  int x_;
};

myFunctorClass addFive(5);
std::cout << addFive(6);
```
the act of constructing an object lets you give the functor information that it can use inside the implementation of its function-like behavior(when the functor is called through operator()).

[Do we have closures in C++?](https://stackoverflow.com/questions/12635184/do-we-have-closures-in-c)

If you understand **closure as a reference to a function that has an embedded, persistent, hidden and unseparable context(memory, state).**

```C++
class summer {
 public:
  summer() : sum_(0) {}
  int operator() (int x){
    return sum_ += x;
  }
 private:
  int sum_;  // sum 保存求和的状态
};

summer adder;
adder(3);
auto res = adder(4);
ASSERT_EQ(7, res);
```

