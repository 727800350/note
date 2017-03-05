# shared_ptr

# uniq_ptr

# weak_ptr
C++ Primer 中讲解为什么需要weak pointer 不是很清楚, blog [C++11智能指针之weak_ptr](http://blog.csdn.net/Xiejingfa/article/details/50772571) 从一个循环引用的例子来开篇, 间接解释了为什么需要weak pointer.

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

