### 虚继承
```C++
struct Manager: Employee { ... };
struct Worker: Employee { ... };
struct MiddleManager : Manager, Worker { ... };
```

如果经理类和工人类都继承自雇员类, 很自然地, 它们每个类都会从雇员类获得一份数据拷贝.
如果不作特殊处理, 一线经理类的实例将含有两个雇员类实例, 它们分别来自两个雇员基类.
如果雇员类成员变量不多,问题不严重,如果成员变量众多,则那份多余的拷贝将造成实例生成时的严重开销.更糟的是,这两份不同的雇员实例可能分别被修改,造成数据的不一致.
因此, 我们需要让经理类和工人类进行特殊的声明,说明它们愿意共享一份雇员基类实例数据.

很不幸,在C++中,这种"共享继承"被称为"虚继承",把问题搞得似乎很抽象.虚继承的语法很简单,在指定基类时加上virtual关键字即可

```C++
struct Manager: virtual Employee { ... };
struct Worker: virtual Employee { ... };
struct MiddleManager : Manager, Worker { ... };
```
当虚继承时,一般说来,派生类地址和其虚基类地址之间的偏移量是不固定的,
因为如果这个派生类又被进一步继承的话,最终派生类会把共享的虚基类实例数据放到一个与上一层派生类不同的偏移量处.
请看下例:

```C++
struct G: virtual C {
  int g1;
  void gf();
};
```
![G的内存布局](http://img.my.csdn.net/uploads/201108/9/5419476_13128780806dbk.jpg)
- GdGvbptrG(In G, the displacement of G's virtual base pointer to G)意思是:在G中, G对象的指针与G的虚基类表指针之间的偏移量,在此可见为0,因为G对象内存布局第一项就是虚基类表指针
- GdGvbptrC(In G, the displacement of G's virtual base pointer to C)意思是:在G中,C对象的指针与G的虚基类表指针之间的偏移量,在此可见为8.

```C++
struct H: virtual C {
  int h1;
  void hf();
};
```
![H 的内存布局](http://img.my.csdn.net/uploads/201108/9/5419476_1312878118CwgC.jpg)

```C++
struct I: G, H {
  int i1;
  void if();
};
```
![I的内存布局](http://img.my.csdn.net/uploads/201108/9/5419476_13128781759JQM.jpg)

从上面这些图可以直观地看到,在G对象中,内嵌的C基类对象的数据紧跟在G的数据之后,在H对象中,内嵌的C基类对象的数据也紧跟在H的数据之后.但是,在I对象中,内存布局就并非如此了.
VC++实现的内存布局中,G对象实例中G对象和C对象之间的偏移,不同于I对象实例中G对象和C对象之间的偏移.
当使用指针访问虚基类成员变量时,由于指针可以是指向派生类实例的基类指针,所以,编译器不能根据声明的指针类型计算偏移,而必须找到另一种间接的方法,从派生类指针计算虚基类的位置.
在VC++ 中,对每个继承自虚基类的类实例,将增加一个隐藏的"虚基类表指针"(vbptr)成员变量,从而达到间接计算虚基类位置的目的.
该变量指向一个全类共享的偏移量表(同一个类的不同实例共享),表中项目记录了对于该类 而言,"虚基类表指针"与虚基类之间的偏移量.

观察前面的G,H和I, 我们可以得到如下关于VC++虚继承下内存布局的结论:

1. 首先排列非虚继承的基类实例,
2. 有虚基类时,为每个基类增加一个隐藏的vbptr,除非已经从非虚继承的类那里继承了一个vbptr,
3. 排列派生类的新数据成员,
4. 在实例最后,排列每个虚基类的一个实例.

### 虚函数
虚函数的实现要求对象携带额外的信息,这些信息用于在运行时确定该对象应该调用哪一个虚函数.
典型情况下,这一信息具有一种被称为 vptr(virtual table pointer,虚函数表指针)的指针的形式.
vptr 指向一个被称为 vtbl(virtual table,虚函数表)的函数指针数组,每一个包含虚函数的类都关联到 vtbl.
当一个对象调用了虚函数,实际的被调用函数通过下面的步骤确定:找到对象的 vptr 指向的 vtbl,然后在 vtbl 中寻找合适的函数指针.

```C++
class CA {
 public:
  int a;
};

class CB {
 public:
  int b;
};

class CL : public CB, public CA {
 public:
  int c;
};
```
对于CL类,它的内存布局是:
```C
int b;
int a;
int c;
```
但是把CA 类改为下面的
```C++
class CA {
 public:
  int a;
  virtual void af(){};
};
```
对于CL类,它的内存布局是:
```C
vfptr
int a;
int b;
int c;
```
不懂`int a` 的顺序也变了

一个C++成员函数只是类范围内的又一个成员.X类每一个非静态的成员函数都会接受一个特殊的隐藏参数-this指针,类型为`X* const`.
该指针在后台初始化为指向成员函数工作于其上的对象.同样,在成员函数体内,成员变量的访问是通过在后台计算与this指针的偏移来进行.


如果从多个有虚函数的基类继承,一个实例就有可能包含多个vfptr
```C++
struct P {
  int p1;
  void pf();
  virtual void pvf(){
    cout << "P::pvf" << endl;
  };
};

struct R {
  int r1;
  virtual void pvf(){
    cout << "R::pvf" << endl;
  };
  virtual void rvf(){
    cout << "R::rvf" << endl;
  };
};

struct S : P, R {
  int s1;
  // overrides P::pvf and R::pvf
  void pvf() {
    cout << "S::pvf" << endl;
  };
  // overrides R::rvf
  void rvf() {
    cout << "S::rvf" << endl;
  };
  void svf() {
    cout << "S::svf" << endl;
  };
};
```

```C++
S s;
S* ps = &s;  
((P*)ps)->pvf(); // (*(P*)ps)->P::vfptr[0])((S*)(P*)ps), 输出 S::pvf
((R*)ps)->pvf(); // (*(R*)ps)->R::vfptr[0])((S*)(R*)ps), 输出 S::pvf
ps->pvf();       // one of the above; calls S::pvf(), 输出 S::pvf
```
- 调用`((P*)ps)->pvf()`时,先到P的虚函数表中取出第一项,然后把ps转化为`S*`作为this指针传递进去. todo: 调用的是P::pvf, 为什么this 指针要是`S *`类型的
- 调用`((R*)ps)->pvf()`时,先到R的虚函数表中取出第一项,然后把ps转化为`S*`作为this指针传递进去

因为`S::pvf()`覆盖了`P::pvf()`和`R::pvf()`,在S的虚函数表中,相应的项也应该被覆盖.
然而,我们很快注意到,不光可以用`P*`,还可以用`R*`来调用`pvf()`.问题出现了:R的地址与P和S的地址不同.表达式 `(R*)ps`与表达式`(P*)ps`指向类布局中不同的位置.
因为函数`S::pvf`希望获得一个`S*`作为隐藏的this指针参数,虚函数必须把`R*`转化为 `S*`.
因此,在S对R虚函数表的拷贝中,pvf函数对应的项,指向的是一个"调整块"的地址,该调整块使用必要的计算,把`R*`转换为需要的`S*`.
译者注:这就是`thunk1: this -= sdPR; goto S::pvf`干的事.先根据P和R在S中的偏移,调整this为`P*`,也就是`S*`,然后跳转到相应的虚函数处执行.

### 虚继承下的虚函数
```C++
struct P {
  int p1;
  void pf();
  virtual void pvf() {
    cout << "P::pvf" << endl;
  };
};

struct T: virtual P {
  int t1;
  void pvf() {
    cout << "T::pvf" << endl;
  };
  virtual void tvf() {
    cout << "T::tvf" << endl;
  };
};
```
![内存布局](http://img.my.csdn.net/uploads/201108/9/5419476_1312878510F137.jpg)

T虚继承P,覆盖P的虚成员函数,声明了新的虚函数.
如果采用在基类虚函数表末尾添加新项的方式,则访问虚函数总要求访问虚基类.
在VC++中,为了避免获取虚函数表时,转换到虚基类P的高昂代价,T中的新虚函数通过一个新的虚函数表获取,从而带来了一个新的虚函数表指针.该指针放在T实例的顶端.
因为sizeof(struct T) = 4(T::vfptr) + 4(T::vbptr) + 4(T::t1) + 4(P::vfptr) + 4(P::p1)

