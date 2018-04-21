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

## Friendship and inheritance
### Friend functions
In principle, private and protected members of a class cannot be accessed from outside the same class in which
they are declared. However, this rule does not affect friends.

### Friend classes
Just as we have the possibility to define a friend function, we can also define a class as friend of another one,
granting that first class access to the protected and private members of the second one.

In this example, we have declared CRectangle as a friend of `CSquare` so that CRectangle member functions could
have access to the protected and private members of `CSquare`, more concretely to `CSquare::side`, which describes the side width of the square.

派生类的声明:
```
class 派生类名:继承方式 基类名1, 继承方式 基类名2,...,继承方式 基类名n{
	派生类成员声明;
};
```
继承方式规定了如何访问基类继承的成员.继承方式有public, private, protected.
如果不显示给出继承方式,默认为private继承.
继承方式指定了派生类成员以及类外对象对于从基类继承来的成员的访问权限.
**不管是哪种继承方式, 基类的私有成员都不能访问**

- 公有继承
**基类的公有和保护成员的访问属性在派生类中不变**,
即基类的公有成员和保护成员被继承到派生类中仍作为派生类的公有成员和保护成员.派生类的其他成员可以直接访问它们.
无论派生类的成员还是派生类的对象都无法访问基类的私有成员.

- 私有继承  
**基类中的公有成员和保护成员都以私有成员身份出现在派生类中**,
基类的公有成员和保护成员被继承后作为派生类的私有成员,派生类的其他成员可以直接访问它们,但是在类外部通过派生类的对象无法访问.
无论是派生类的成员还是通过派生类的对象,都无法访问从基类继承的私有成员.
通过多次私有继承后,对于基类的成员都会成为不可访问.因此私有继承比较少用.
 
- 保护继承  
**基类的公有成员和私有成员都以保护成员的身份出现在派生类中**,
派生类的其他成员可以直接访问从基类继承来的公有和保护成员,但是类外部通过派生类的对象无法访问它们.
无论派生类的成员还是派生类的对象,都无法访问基类的私有成员.

派生类构造函数的语法:
```
派生类名::派生类名(参数总表):基类名1(参数表1),....基类名n(参数名n),内嵌子对象1(参数表1),...内嵌子对象n(参数表n)
{
	派生类新增成员的初始化语句;
}
```
**注:构造函数的初始化顺序并不以上面的顺序进行,而是根据声明的顺序初始化.**

派生类构造函数执行的次序:

1. 调用基类构造函数,调用顺序按照它们被继承时声明的顺序(**从左到右**),
1. 调用内嵌成员对象的构造函数,调用顺序按照它们在类中声明的顺序,
1. 派生类的构造函数体中的内容.

[inheritance construnctor demo](../demo/cpp/class/inheritance_constructor.cpp)

派生类的析构函数的功能是在该对象消亡之前进行一些必要的清理工作,析构函数没有类型,也没有参数.
析构函数的执行顺序与构造函数相反.

1. 派生类的构造函数体中的内容
1. 调用内嵌成员对象的构造函数,调用顺序按照它们在类中声明的逆顺序
1. 调用基类构造函数,调用顺序按照它们被继承时声明的逆顺序

[inheritance destrunctor demo](../demo/cpp/class/inheritance_destructor.cpp)

如果某派生类的多个基类拥有同名的成员,同时,派生类又新增这样的同名成员,在这种情况下,派生类成员将覆盖所有基类的同名成员.这就需要显示的指定基类, 才能调用基类的同名成员.

[scope class](../demo/cpp/class/scope_class.cpp)

如果某个派生类的部分或全部直接基类是从另一个共同的基类派生而来,在这些直接基类中,从上一级基类继承来的成员就拥有相同的名称,因此派生类中也就会产生同名现象,
对这种类型的同名成员也要使用作用域分辨符来唯一标识,而且必须用直接基类进行限定

[scope class2](../demo/cpp/class/scope_class2.cpp)

为了解决前面提到的多重拷贝的问题,可以将共同基类设置为**虚基类**,这时从**不同的路径继承过来的同名数据成员在内存中就只有一个拷贝,同一个函数也只有一个映射**.
虚基类的声明是在派生类的声明过程,其语法形式为:
```
class 派生类名::virtual 继承方式 基类名,
```
[scope virtual class](../demo/cpp/class/scope_class_virtual.cpp)

虚基类及其派生类的构造函数
一般而言,派生类只对其直接基类的构造函数传递参数,但是在虚基类中,不管是直接或间接虚基类的所有派生类,都必须在构造函数的成员初始化列表中列出对虚基类的初始化.

[virtual class constructor](../demo/cpp/class/scope_class_virtual_constructor.cpp)

以上例子看上去B0的构造函数好像被调用了三次,但是实际上只有D1类中的D1(int a):B0(a), B1(a), B2(a)
才是真正的调用了B0构造函数.
 
赋值兼容规则是指在需要基类对象的任何地方都可以使用公有派生类的对象来替代(派生类的对象可以赋值给基类对象)
在替代之后,派生类对象就可以作为基类的对象使用,但只能使用从基类继承的成员.

[class assignment](../demo/cpp/class/class_assignment.cpp)

### Virtual members

### Polymorphism

### Abstract base classes

