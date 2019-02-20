# [unique ptr](http://www.cplusplus.com/reference/memory/unique_ptr)
For safety reasons, they **do not support pointer arithmetics**, and only support move assignment(disabling copy assignments).

- `get()`: Returns the stored pointer. 拿到这个指针后可以像以前那样直接操作这快内存, 但是要注意, unique ptr 的ownership 并没有发生变化
- `*`: Returns a reference to the managed object, equivalent to `*get()`.
- `->`: 可以像以前那样直接访问成员变量
- `[i]`: Returns a reference to the i-th object (zero-based) in the managed array. equivalent to `get()[i]`.
- 不能把unique ptr 赋值给其他unique ptr, 可以`std::move()`, 操作之后, original becomes nullptr and the new one owns the pointer;

```C++
#include <memory>
std::unique_ptr<int> x(new int);
std::unique_ptr<int[]> x(new int[5]);
```

# [shared ptr](http://www.cplusplus.com/reference/memory/shared_ptr)
- For multiple users of one pointer/object
- Refcountered, useful but over-used. 所以尽量不要使用

If two shared ptr are constructed (or made) from the same (non-shared ptr) pointer, they will both be owning the pointer without sharing it,
causing potential access problems when one of them releases it (deleting its managed object) and leaving the other pointing to an invalid location.

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
class S : std::enable_shared_from_this<S>{
public:
  std::shared_ptr<S> not_dangerous(){
    return std::shared_from_this();
  }
};

int main(){
   std::shared_ptr<S> sp1(new S);
   std::shared_ptr<S> sp2 = sp1->not_dangerous();
   return 0;
}
```
When you do this, keep in mind that the object on which you call `shared_from_this` must be owned by a `shared_ptr` object. This won't work:
```C++
int main(){
   S *p = new S;
   std::shared_ptr<S> sp2 = p->not_dangerous();     // don't do this
}
```

# [weak ptr](http://www.cplusplus.com/reference/memory/weak_ptr)
weak ptr, is able to share pointers with shared ptr objects without owning them.

C++ Primer 中讲解为什么需要weak pointer 不是很清楚, blog [C++11智能指针之weak ptr](http://blog.csdn.net/Xiejingfa/article/details/50772571) 从一个循环引用的例子来开篇, 间接解释了为什么需要weak pointer.

```C++
#include <iostream>
#include <memory>

using namespace std;

class ClassB;

class ClassA {
public:
	ClassA(){
		cout << "ClassA Constructor..." << endl;
	}

	~ClassA(){
		cout << "ClassA Destructor..." << endl;
	}

	shared_ptr<ClassB> pb;
};

class ClassB{
public:
	ClassB(){
		cout << "ClassB Constructor..." << endl;
	}

	~ClassB(){
		cout << "ClassB Destructor..." << endl;
	}

	shared_ptr<ClassA> pa;
};

int main(){
	shared_ptr<ClassA> spa = make_shared<ClassA>();
	shared_ptr<ClassB> spb = make_shared<ClassB>();
	cout << spa.use_count() << "," << spb.use_count() << endl; // 1,1

	spa->pb = spb;
	spb->pa = spa;
	cout << spa.use_count() << "," << spb.use_count() << endl; // 2,2

	return 0;
}
```
上面的代码输出:
```
ClassA Constructor...
ClassB Constructor...
Program ended with exit code: 0
```
从上面代码中, ClassA 和ClassB 间存在着循环引用, 从运行结果中我们可以看到: 当main函数运行结束后, spa 和spb 管理的动态资源并没有得到释放, 产生了内存泄露.

原因: 在两个赋值语句之后, spa 和 spb 的use_count 都是2, 退出函数之后, use_count 会都减1, 但是减了之后, use_count 还是1, 不是0, 所以内存不会被自动释放.

解决方案: 使用weak_ptr,来打破这种循环引用, 这样在退出main 函数后, use_count 会被减为0, 内存就会被自动释放.

