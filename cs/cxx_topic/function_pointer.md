ref: [深入理解C语言函数指针](http://www.cnblogs.com/windlaughing/archive/2013/04/10/3012012.html)

```C++
int myFun(int x){
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
void (*funP)(int);   // 也可写成void(*funP)(int x),但习惯上一般不这样.
void (*funA)(int);
void myFun(int x);

//一般的函数调用
myFun(100);

// myFun与funP的类型关系类似于int 与int *的关系.
funP = &myFun;  // 将myFun函数的地址赋给funP变量
(*funP)(200);  // 通过函数指针变量来调用函数

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

