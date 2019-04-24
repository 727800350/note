# [unique ptr](http://www.cplusplus.com/reference/memory/unique_ptr)
- For safety reasons, they **do not support pointer arithmetics**, and only support move assignment(disabling copy assignments).
- 性能出色, 析构和delete 没有差异, 可大量使用
- 必要时可以转换为shared ptr
- unique ptr 可以自定义Deleter, 但是这可能会改变类型, 因为Deleter 也是模板参数的一部分, `std::unique_ptr<FILE, decltype(&fclose)> f(fopen(...), &fclose)`
- 工厂类建议返回`std::unique_ptr<T>`
- `get()`: Returns the stored pointer. 拿到这个指针后可以像以前那样直接操作这快内存, 但是要注意, unique ptr 的ownership 并没有发生变化
- `*`: Returns a reference to the managed object, equivalent to `*get()`.
- `->`: 可以像以前那样直接访问成员变量
- `[i]`: Returns a reference to the i-th object (zero-based) in the managed array. equivalent to `get()[i]`.
- 不能把unique ptr 赋值给其他unique ptr, 可以`std::move()`, 操作之后, original becomes nullptr and the new one owns the pointer;

# [shared ptr](http://www.cplusplus.com/reference/memory/shared_ptr)
- 性能开销大
- shared ptr 的Deleter 不影响类型(通过类型消除实现)

```C++
auto sp1 = std::make_shared<int>(1);
auto sp2 = sp1;
auto sp3 = std::make_shared<int>(2);
fprintf(stdout, "%d %d %d\n", *sp1, *sp2, *sp3); // 1 1 2
sp1.swap(sp3);
fprintf(stdout, "%d %d %d\n", *sp1, *sp2, *sp3); // 2 1 1
```
swap 仅仅是交换了sp1 和 sp3 的指向, sp2 没有发生变化, 还是指向原来的1

## [`std::enable_shared_from_this`](https://stackoverflow.com/questions/712279/what-is-the-usefulness-of-enable-shared-from-this)
allows an object t(裸指针) that is currently managed by a `std::shared_ptr` named pt to safely generate additional `std::shared_ptr` instances pt1, pt2, ... that all share ownership of t with pt.

code like this won't work correctly:
```C++
int *ip = new int;
std::shared_ptr<int> sp1(ip);
std::shared_ptr<int> sp2(ip);
```
Neither of the two `shared_ptr` objects knows about the other, so both will try to release the resource when they are destroyed. That usually leads to problems.

The way to avoid this problem is to use the class template `enable_shared_from_this`. The template takes one template type argument, which is the name of the class that defines the managed resource. That class must, in turn, be derived publicly from the template; like this:
```C++
class S : std::enable_shared_from_this<S> {
 public:
  std::shared_ptr<S> not_dangerous(){
  return std::shared_from_this();
  }
};

int main() {
  std::shared_ptr<S> sp1(new S);
  std::shared_ptr<S> sp2 = sp1->not_dangerous();
  return 0;
}
```
When you do this, keep in mind that the object on which you call `shared_from_this` must be owned by a `shared_ptr` object. This won't work:
```C++
int main() {
  S *p = new S;
  std::shared_ptr<S> sp2 = p->not_dangerous();   // don't do this
}
```

## [借`shared_ptr`实现copy-on-write](https://blog.csdn.net/q5707802/article/details/79261515)
在<Linux多线程服务端编程使用muduoC++网络库>2.8节说"借`shared_ptr`实现copy-on-write". 那么copy-on-write是怎样的技术?
COW(Copy-On-Write)通过浅拷贝(shallow copy)只复制引用而避免复制值,当的确需要进行写入操作时,首先进行值拷贝,再对拷贝后的值执行写入操作,这样减少了无谓的复制耗时.

特点如下:

- 读取安全(但是不保证缓存一致性),写入安全(代价是加了锁,而且需要全量复制)
- 不建议用于频繁读写场景下,全量复制很容易造成GC停顿.
- 适用于对象空间占用大,修改次数少,而且对数据实效性要求不高的场景.

这里的安全指在进行读取或者写入的过程中,数据不被修改.

copy-on-write最擅长的是并发读取场景,即多个线程/进程可以通过对一份相同快照,去处理实效性要求不是很高但是仍然要做的业务,如Unix下的fork()系统调用,标准C++类std::string等采用了 copy-on-write,在真正需要一个存储空间时才去分配内存,这样会极大地降低程序运行时的内存开销.

Copy-On-Write的原理:Copy-On-Write使用了"引用计数(retainCount)"的机制(在Objective-C和Java中有应用).

COW技术的精髓:

1. 如果你是数据的唯一拥有者,那么你可以直接修改数据.
1. 如果你不是数据的唯一拥有者,那么你拷贝它之后再修改.

shared_ptr是采用引用计数方式的智能指针,如果当前只有一个观察者,则其引用计数为1,可以通过shared_ptr::unique()判断.
用shared_ptr来实现COW时,主要考虑两点:(1)读数据  (2)写数据

通过shared_ptr实现copy-on-write的原理如下:

1. read端在读之前,创建一个新的智能指针指向原指针,这个时候引用计数加1,读完将引用计数减1,这样可以保证在读期间其引用计数大于1,可以阻止并发写.
  ```C++
  // 假设g_ptr是一个全局的shared_ptr<Foo>并且已经初始化.
  void read() {
    std::shared_ptr<Foo> tmpptr;
    {
      lock();
      tmpptr = g_ptr; // 此时引用计数为2,通过gdb调试可以看到
    }
    // read
  }
  ```
这部分是shared_ptr最基本的用法,还是很好理解的,read()函数调用结束,tmpptr作为栈上变量离开作用域,自然析构,原数据对象的引用计数也变为1.

2. write端在写之前,先检查引用计数是否为1. 如果引用计数为1,则你是数据的唯一拥有者,直接修改; 
如果引用计数大于1,则你不是数据的唯一拥有者,还有其它拥有者, 此时数据正在被其它拥有者read,则不能再原来的数据上并发写,应该创建一个副本,并在副本上修改,然后用副本替换以前的数据.
这就需要用到一些shared_ptr的编程技法了(注意需要注意不能有weak_ptr, 否则引用计数即使是1, 也可能有其他地方可能正在读):
  ```C++
  void write(){
    lock()
    if(!g_ptr.unique()) {
      g_ptr.reset(new Foo(*g_ptr));
    }
    assert(g_ptr.unique());
    // write
  }
  ```
假设一个线程读,一个线程写,当写线程进入到if循环中时,原对象的引用计数为2,分别为tmpptr和g_ptr,此时reset()函数将原对象的引用计数减1,并且g_ptr已经指向了新的对象(用原对象构造),这样就完成了数据的拷贝,并且原对象还在,只是引用计数变成了1.

注意,reset()函数仅仅只是将原对象的引用计数减1,并没有将原对象析构,当原对象的引用计数为0时才会被析构.

## `std::shard_ptr<void>`
[`std::shared_ptr<void>`的工作原理](https://www.cnblogs.com/imjustice/p/how_shared_ptr_void_works.html)

delete语句会至少产生两个动作,一个是调用指针对应类型的析构函数,然后去调用operator delete释放内存.
所以如果delete的指针和其指向的真实类型不一样的时候,编译器只会调用指针类型的析构函数,这也就为什么基类的析构函数需要声明称虚函数才能够保证delete基类指针的时候子类析构函数能够被正确的调用.
operator delete是都会被调用到的,所以指针指向的那块内存是能够"正常的"被释放掉用.

意味着如果一个class Foo的成员变量申请了动态内存,
```C++
void* foo = new Foo;
delete foo;
```
`delete foo;` 中所包含的第二个动作 operator delete 是不负责删除成员变量申请的动态内存, 而要靠第一个动作中的Foo 类型的析构函数.

```C++
std::shared_ptr<void> foo = std::make_shared<Foo>();
```
用`std::shared_ptr<void>` 在引用数降为0 而释放的时候, 是能够正确调用析构函数的.
因为`std::shared_ptr` 里面有一个控制块, 里面会存放一个要维护的指针,一个计数,一个删除器(deleter),一个分配(allocator).
有了这个deleter, 就能正确的调用析构函数.

这种实现方式给`std::shared_ptr`带来额外好处:
Effective C++ 条款07告诉我们"要为多态基类声明 virtual 析构函数". 当然我认为建议依然有效,
但是用了`std::shared_ptr`以后带来的一个额外好处就是即便你的析构函数忘记写成virtual也能帮你正确的调用析构函数.

# [weak ptr](http://www.cplusplus.com/reference/memory/weak_ptr)
- weak ptr, is able to share pointers with shared ptr objects without owning them.
- 从效率的角度来看,`std::weak_ptr`和`std::shared_ptr`几乎一致
- 可能使用`std::weak_ptr`的情况包括: 缓存, 观察模式中的观察者列表, 以及防止`std::shared_ptr`环路. [ref](https://blog.csdn.net/coolmeme/article/details/43266319)

观察者模式:
该模式的主要部件是主题(Subjects, 状态可以改变的对象)和观察者(Observers, 状态改变发生后被通知的对象).
多数实现中, 每个subject包含了一个数据成员, 保持着指向observer的指针,这样很容易在subject发生状态改变时发通知.
subject没有兴趣控制它们的observer的生命周期(不关心何时它们被销毁),但他要确认一个observer是否被销毁,这样避免去访问.
一个合理的设计就是每个subject保存一个容器,容器里放着每个observer的`std::weak_ptr`, 这样在subject在使用前就可以检查observer指针是否是悬浮的.

