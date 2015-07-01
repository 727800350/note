# Java
[环境变量配置](http://jingyan.baidu.com/article/f96699bb8b38e0894e3c1bef.html)

## Data Types
基本数据类型只有8个, byte, short, int, log, float, double, char, boolean;  
除了基本类型(primitive type)和枚举类型(enumeration type), 剩下的都是引用类型.

但是为了将基本数据类型当成对象操作, Java 为每一个基本数据类型都引入了对应的包装类型(Wrapper Class), 从JDK1.5 开始, 引入了自动的装箱/拆箱机制, 使得二者可以相互转换.  
Boolean, Character, Byte, Short, Integer, Long, Float, Double

```
switch(expr)
```
expr 可以是byte, short, char, int, enum, String, 但是不能是long

char 型变量中能不能存贮一个中文汉字?为什么?  
答:char类型可以存储一个中文汉字,因为Java中使用的编码是Unicode(不选择任何特定的编码,直接使用字符在字符集中的编号,这是统一的唯一方法),
一个char类型占2个字节(16bit)(todo, Java 中sizeof?),所以放一个中文是没问题的.  
补充:使用Unicode意味着字符在JVM内部和外部有不同的表现形式,在JVM内部都是Unicode,当这个字符被从JVM内部转移到外部时(例如存入文件系统中),需要进行编码转换.
所以Java中有字节流和字符流,以及在字符流和字节流之间进行转换的转换流,如InputStreamReader和OutputStreamReader,这两个类是字节流和字符流之间的适配器类,承担了编码转换的任务,
对于C程序员来说,要完成这样的编码转换恐怕要依赖于union(联合体/共用体)共享内存的特征来实现了.

`float f = 3.4` 编译错误.  
3.4 是双精度数, 将double 类型复制到float, 需要强制转换, 或者写成`float f = 3.4F`

```
short s1 = 1;
s1 = s1 + 1; // 编译出错, 1 是int 类型, s1 + 1 也是int 类型, 因此需要强制转换

short s1 = 1;
s1 += 1; // 正确, 隐含的强制转换
```

装箱与拆箱
```
Integer a = new Integer(3);
Integer b = 3;
int c = 3;
System.out.println(a == b); // false, 因为没有引用同一个对象
System.out.println(a == c); // true, a 自动拆箱成int 类型, 再和c 进行比较
System.out.println(b == c);
```

数组有length 属性, String 有length()方法.
但是在Javascript中, 获得字符串的长度也是通过length 属性得到的.

### 引用类型
如果两个对象, x.equals(y) == true, 那么它们的hashCode 一定相同.(hashCode 的规定就是这样的).

String 是final 类, 不可以被继承.  
对String类型的最好重用方式是关联(has a)而不是继承(is a)

String 是只读字符串.

String和StringBuilder,StringBuffer的区别?  
答:Java 平台提供了两种类型的字符串:String和StringBuffer/StringBuilder,它们可以储存和操作字符串.
其中String是只读字符串,也就意味着String引用的字符串内容是不能被改变的.
而StringBuffer和StringBuilder类表示的字符串对象可以直接进行修改.
StringBuilder是JDK 1.5中引入的,它和StringBuffer的方法完全相同,区别在于它是在单线程环境下使用的,因为它的所有方面都没有被synchronized修饰,因此它的效率也比StringBuffer略高.

补充1:有一个面试题问:有没有哪种情况用+做字符串连接比调用StringBuffer/StringBuilder对象的append方法性能更好?  
如果连接后得到的字符串在静态存储区中是早已存在的,那么用+做字符串连接是优于StringBuffer/StringBuilder的append方法的

```
String a = "Programming";
String b = new String("Programming");
String c = "Program" + "ming";
System.out.println(a == b); // false
System.out.println(a == c); // true, 可能因为a 和c 都是字面量, 不知道这么解释对不对
System.out.println(a.equals(b)); // true
System.out.println(a.equals(c)); // true
System.out.println(a.intern() == b.intern()); //true
```
- `String.equals`: true if and only if the argument is not null and is a String object that represents the same sequence of characters as this object.
- `a.intern() == t.intern()` is true if and only if `s.equals(t)` is true

如何实现对象克隆?  
答:有两种方式:

1. 实现Cloneable接口并重写Object类中的clone()方法,
2. 实现Serializable接口,通过对象的序列化和反序列化实现克隆,可以实现真正的深度克隆,代码如下.

注意:基于序列化和反序列化实现的克隆不仅仅是深度克隆,更重要的是通过泛型限定,可以检查出要克隆的对象是否支持序列化,这项检查是编译器完成的,不是在运行时抛出异常,
这种是方案明显优于使用Object类的clone方法克隆对象.

### Java 的内存机制
[ref](http://www.cnblogs.com/xwdreamer/archive/2012/04/01/2428857.html)  
Java 把内存划分成两种:一种是栈内存,另一种是堆内存.在函数中定义的一些基本类型的变量和对象的引用变量都是在函数的栈内存中分配.  
堆内存用来存放由 new 创建的对象和数组,在堆中分配的内存,由 Java 虚拟机的自动垃圾回收器来管理.  
在堆中产生了一个数组或者对象之后,还可以在栈中定义一个特殊的变量,让栈中的这个变量的取值等于数组或对象在堆内存中的首地址,
栈中的这个变量就成了数组或对象的引用变量,以后就可以在程序中使用栈中的引用变量来访问堆中的数组或者对象,引用变量就相当于是为数组或者对象起的一个名称.
引用变量是普通的变量,定义时在栈中分配,引用变量在程序运行到其作用域之外后被释放.
而数组和对象本身在堆中分配,即使程序运行到使用 new 产生数组或者对象的语句所在的代码块之外,数组和对象本身占据的内存不会被释放,
数组和对象在没有引用变量指向它的时候,才变为垃圾,不能在被使用,但仍然占据内存空间不放,在随后的一个不确定的时间被垃圾回收器收走(释放掉).
这也是 Java 比较占内存的原因,实际上,栈中的变量指向堆内存中的变量,这就是 Java 中的指针!

- 通常我们定义一个基本类型数据类型的变量, 一个对象的引用, 还有就是函数调用的现场保存都使用内存中的栈空间
- 而通过new 关键字和构造器创建的对象放在堆空间.
- 程序中的字面量(literal) 如直接书写的100, "hello" 和常量都是放在静态存储区中.

栈空间操作最快但是也是很小, 通常大量的对象都是放在堆空间, 整个内存包括硬盘上的虚拟内存都可以被当成堆空间来使用

```
String str = new String("hello");
```
上面的语句中str 放在栈中, 用new 创建的字符串对象放在堆中, 而"hello" 这个字面量放在静态存储区.  
创建了2个String object.一个是静态存储区的"hello", 一个是用new创建的.

### Array与ArrayList的主要区别
1. 精辟阐述: 可以将 ArrayList想象成一种"会自动扩增容量的Array".

1. Array():最高效,但是其容量固定且无法动态改变, ArrayList: 容量可动态增长,但牺牲效率,

1. Java中一切皆对象,Array也是对象.不论你所使用得Array型别为何, Array名称本身实际上是个reference,指向heap之内得某个实际对象.这个对象可经由"Array初始化语法"被自动产生,也可以以new表达式手动产生.

1. Array可做为函数返回值,因为它本身是对象的reference

1. 对象数组与基本类型数组在运用上几乎一模一样,唯一差别在于,前者持有得是reference,后者直接持有基本型别之值,

1. 容器所持有的其实是一个个reference指向Object,进而才能存储任意型别.当然这不包括基本型别,因为基本型别并不继承自任何classes.

1. 面对Array,我们可以直接持有基本型别数值的Array(例如:`int [] num;`),也可以持有reference(指向对象)的Array, 但是容器类仅能持有reference(指向对象),若要将基本型别置于容器内,需要使用wrapper类.
但是wrapper类使用起来可能不很容易上手,此外,primitives Array的效率比起"容纳基本型别之外覆类(的reference)"的容器好太多了. 
当然,如果你的操作对象是基本型别,而且需要在空间不足时自动扩增容量,Array便不适合,此时就得使用外覆类的容器了.

1. 某些情况下,容器类即使没有转型至原来的型别,仍然可以运作无误.有一种情况尤其特别:编译器对String class提供了一些额外的支持,使它可以平滑运作.

1. 对数组的一些基本操作,像排序,搜索与比较等是很常见的.因此在Java中提供了Arrays类协助这几个操作:sort(),binarySearch(),equals(),fill(),asList().
不过Arrays类没有提供删除方法,而ArrayList中有remove()方法,不知道是否是不需要在Array中做删除等操作的原因(因为此时应该使用链表).

1. ArrayList的使用也很简单:产生ArrayList,利用add()将对象置入,利用get(i)配合索引值将它们取出.这一切就和Array的使用方式完全相同,只不过少了`[]`而已.

1. 类型识别: ArrayList存入对象时,抛弃类型信息,所有对象屏蔽为Object,编译时不检查类型,但是运行时会报错.

1. ArrayList可以存任何Object,如String等.

## 变量的初始化
```
void f(){
	int i;
	i++; // Error -- i not initialized
}
```
你会得到一个提示内容为i 可能没被初始化的错误消息. 当然, 编译器可能已经为i 赋予了一个默认值,但是没被初始化的局部变量有可能是程序员的一个错误,而默认值恰恰掩盖了这个错误. 强制程序员赋予初始值极有可能捕获程序缺陷.

不过假如 primitives 是类的 field,情况会有些不同. 类的每一个 primitive 都被确保得到一个初始值. 下面的程序验证了这一点.
```
public class InitialValues {
	boolean t;
	char c;
	byte b;
	short s;
	int i;
	long l;
	float f;
	double d;
	InitialValues reference;
	
	void print(){
		System.out.println("Data type initial value");
	}
	
	public static void main(String[] args) {
		InitialValues iv = new InitialValues();
		iv.print();
	}
}
```
通过在调试界面看到, primitives 变量都被默认初始化为0, reference 被默认初始化为null.

如果你想为一个变量赋予初始值,具体该怎么做呢? 
一种直接的方式就是当你在类里定义变量的时候就简单明了地为它赋值.(注意 C++不允许你这么初始化值,尽管一些 C++新手总要想当然地这么做.)
```
boolean bool = true;
char ch = 'x';
byte b = 47;
short s = 0xff;
int i = 999;
long lng = 1;
float f = 3.14f;
double d = 3.14159
Depth d = new Depth();
```

在一个类里,初始化的顺序由变量在类里被定义的顺序来决定. 变量的定义可能散落在类的各个地方并且与方法的定义交织在一起,但是变量初始化后,方法才可以被调用—甚至是constructor.

### 数组的初始化
下面的两种方式都可以
```
int [ ] a1;
int a1[] ;
```
编译器不允许你在这个时候告诉它数组有多大. 这使我们回忆起有关"引用"的问题. 到现在你所拥有的只是一个指向数组的引用(你已经为那个引用分配了足够的存贮空间),但是还没有为数组对象本身赋予空间.  
要给数组创造存贮空间,你必须编写一个初始化表达式. 对于数组,初始化可以在你的代码的任何地方出现,但是你也可以使用一种特殊的初始化表达式,
这种表达式必须在数组被创建的地方执行. 此类特殊的初始化由一组被花括号包围的值构成. 在这种情况下,存贮空间的分配(相当于使用 new)由编译器来负责. 举个例子:
```
int [ ] a1 = {1, 2, 3, 4, 5 };
```
对象数组
```
Object [] obs = new Object[]{new Integer(47), new Float(3.14), new Double(11.11)};
Object [] obs = new Object[]{"one", "two", "three"};
```

## Class

作用域    当前类  同包 子类 其他
--------|--------|----|-----|----
public  |      √| √ |  √ |  √
protected|  √   |√  |  √ |  ×
default  | √    | √ | ×  |  ×
private  | √    |  ×| ×  |  ×

类的成员不写访问修饰时默认为default.默认对于同一个包中的其他类相当于公开(public),对于不是同一个包中的其他类相当于私有(private).
受保护(protected)对子类相当于公开,对不是同一包中的没有父子关系的类相当于私有.

一个".java"源文件中是否可以包含多个类(不是内部类)?有什么限制?  
答:可以,但一个源文件中最多只能有一个公开类(public class)而且文件名必须和公开类的类名完全保持一致.

重载(Overload) 和 重写(Override)的区别:

- 两者都是实现多态的方式, 重载是编译时的多态, 重写是运行时的多态
- 重载发生在一个类中, 同名的方法如果有不同的参数(类型或者个数)不同, 则为重载. 因为在编译时就可以看到参数的不同, 因而是编译时的多态.
- 重写是发生在子类和父类之间, 重写要求子类被重写方法与父类被重写方法有相同的返回类型(重载对返回类型没有特殊要求), 不能比父类被重写方法声明更多的异常(里氏代换原则).

重载的方法能够根据返回类型进行区分?  
不能, 因为有时我们调用函数对函数的返回类型是不关心的. 只写 f(), 这时, 我们判断不出.

构造器(constructor)是否可被重写(override)?  
答:构造器不能被继承,因此不能被重写,但可以被重载.

### static
创建对象时构造器的调用顺序是:先初始化静态成员,然后调用父类构造器,再初始化非静态成员,最后调用自身构造器.
```
class A{  
    static{  
        System.out.print("1");  
    }  
  
    public A(){  
        System.out.print("2");  
    }  
}  
  
class B extends A{  
    static{  
        System.out.print("a");  
    }  
  
    public B(){  
        System.out.print("b");  
    }  
}  
  
public class Hello{  
    public static void main(String[] args){  
        A ab = new B();  
        ab = new B();  
    }  
}
```
执行结果: `1a2b2b`.  
最后的2b 是 `ab = new B()` 时产生的. static的代码只执行一次, 因此只有一个1a

是否可以从一个静态(static)方法内部发出对非静态(non-static)方法的调用?  
答:不可以,静态方法只能访问静态成员,因为非静态方法的调用要先创建对象,因此在调用静态方法时可能对象并没有被初始化.

静态嵌套类(Static Nested Class)和内部类(Inner Class)的不同?  
答:Static Nested Class是被声明为静态(static)的内部类,它可以不依赖于外部类实例被实例化.
而通常的内部类需要在外部类实例化后才能实例化,其语法看起来挺诡异的.

Anonymous Inner Class(匿名内部类)是否可以继承其它类?是否可以实现接口?  
答:可以继承其他类或实现其他接口,在Swing编程中常用此方式来实现事件监听和回调.

内部类可以引用它的包含类(外部类)的成员吗?有没有什么限制?  
答:一个内部类对象可以访问创建它的外部类对象的成员,包括私有成员.

### 抽象与接口
抽象类(abstract class)和接口(interface)有什么异同?  
答:抽象类和接口都不能够实例化,但可以定义抽象类和接口类型的引用.一个类如果继承了某个抽象类或者实现了某个接口都需要对其中的抽象方法全部进行实现,否则该类仍然需要被声明为抽象类.
接口比抽象类更加抽象,因为抽象类中可以定义构造器,可以有抽象方法和具体方法,而接口中不能定义构造器而且其中的方法全部都是抽象方法.
抽象类中的成员可以是private,默认,protected,public的,而接口中的成员全都是public的.
抽象类中可以定义成员变量,而接口中定义的成员变量实际上都是常量.有抽象方法的类必须被声明为抽象类,而抽象类未必要有抽象方法.

抽象的(abstract)方法是否可同时是静态的(static),是否可同时是本地方法(native),是否可同时被synchronized修饰?  
答:都不能.  
抽象方法需要子类重写,而静态的方法是无法被重写的,因此二者是矛盾的.  
本地方法是由本地代码(如C代码)实现的方法,而抽象方法是没有实现的,也是矛盾的.  
synchronized和方法的实现细节有关,抽象方法不涉及实现细节,因此也是相互矛盾的.

接口是否可继承(extends)接口? 抽象类是否可实现(implements)接口? 抽象类是否可继承具体类(concrete class)?  
答:接口可以继承接口.抽象类可以实现(implements)接口,抽象类可继承具体类,但前提是具体类必须有明确的构造函数.

描述一下JVM 加载class文件的原理机制?  
答:JVM 中类的装载是由类加载器(ClassLoader) 和它的子类来实现的,Java中的类加载器是一个重要的Java 运行时系统组件,它负责在运行时查找和装入类文件中的类.  
补充:

1. 由于Java的跨平台性,经过编译的Java源程序并不是一个可执行程序,而是一个或多个类文件.
当Java程序需要使用某个类时,JVM会确保这个类已经被加载,连接(验证,准备和解析)和初始化.
类的加载是指把类的.class文件中的数据读入到内存中,通常是创建一个字节数组读入.class文件,然后产生与所加载类对应的Class对象.
加载完成后,Class对象还不完整,所以此时的类还不可用.当类被加载后就进入连接阶段,这一阶段包括验证,准备(为静态变量分配内存并设置默认的初始值)和解析(将符号引用替换为直接引用)三个步骤.
最后JVM对类进行初始化,包括:1如果类存在直接的父类并且这个类还没有被初始化,那么就先初始化父类,2如果类中存在初始化语句,就依次执行这些初始化语句.

2. 类的加载是由类加载器完成的,类加载器包括:根加载器(BootStrap),扩展加载器(Extension),系统加载器(System)和用户自定义类加载器(java.lang.ClassLoader的子类).
从JDK 1.2开始,类加载过程采取了父亲委托机制(PDM).PDM更好的保证了Java平台的安全性,在该机制中,JVM自带的Bootstrap是根加载器,其他的加载器都有且仅有一个父类加载器.
类的加载首先请求父类加载器加载,父类加载器无能为力时才由其子类加载器自行加载.JVM不会向Java程序提供对Bootstrap的引用.下面是关于几个类加载器的说明:

- Bootstrap:一般用本地代码实现,负责加载JVM基础核心类库(rt.jar),
- Extension:从java.ext.dirs系统属性所指定的目录中加载类库,它的父加载器是Bootstrap,
- System:又叫应用类加载器,其父类是Extension.它是应用最广泛的类加载器.它从环境变量classpath或者系统属性java.class.path所指定的目录中记载类,是用户自定义加载器的默认父加载器.

## Cast
First, you must understand, that by casting you are not actually changing the object itself, you are just labeling it differently.

[Cast ex](../demo/java/Cast.java)

![Cast example](http://uploads.codecall.net/monthly_05_2014/post-27003-0-90059300-1399049433.jpg)

For example, if you create a Cat and upcast it to Animal, then the object doesnot stop from being a Cat. 
It is still a Cat, but it is just treated as any other Animal and it is Cat properties are **hidden**(数据还是在实实在在的在那里, 只是无法访问) until it is downcasted to a Cat again.
```
Mammal m = (Mammal)new Cat();
```
is equal to
```
Mammal m = new Cat();
```
Upcasting 是自动进行的.

m 具有cat类型的数据, 但是m 是一个Mammal 类的实例.
也就是说 `m.f_cat();` 不能编译通过, `((Cat) m).f_cat()` 编译通过且运行正常.

But downcasting must always be done manually.
```
Mammal m = new Mammal();
Cat c = (Cat)m;
```
Such code passes compiling, but throws "java.lang.ClassCastException: Mammal cannot be cast to Cat" exception during running, 
because it is trying to cast a Mammal, which is not a Cat, to a Cat.

Variables can hold instance of objects that are equal or are hierarchically below them. 
For example Cat c; can hold instances of Cat and anything that is extended from a Cat. Animal can hold Animal, Mammal, etc..  
Remember, that instances will always be upcasted to the variable level.

```
Math.round(11.5) = 12
Math.round(-11.5) = 11
```
Math.round 都是通过 + 0.5 然后向下取整

## GC
Java 中会存在内存泄漏吗,请简单描述.  
答:理论上Java因为有垃圾回收机制(GC)不会存在内存泄露问题(这也是Java被广泛使用于服务器端编程的一个重要原因),
然而在实际开发中,可能会存在**无用但可达**的对象,这些对象不能被GC回收也会发生内存泄露.
一个例子就是Hibernate的Session(一级缓存)中的对象属于持久态,垃圾回收器是不会回收这些对象的,然而这些对象中可能存在无用的垃圾对象.下面的例子也展示了Java中发生内存泄露的情况:  
[内存泄露的demo](../demo/java/MyStack.java)  
上面的代码实现了一个栈(先进后出(FILO))结构,乍看之下似乎没有什么明显的问题,它甚至可以通过你编写的各种单元测试.
然而其中的pop方法却存在内存泄露的问题,当我们用pop方法弹出栈中的对象时,该对象不会被当作垃圾回收,即使使用栈的程序不再引用这些对象,因为栈内部维护着对这些对象的**过期引用(obsolete reference)**.
在支持垃圾回收的语言中,内存泄露是很隐蔽的,这种内存泄露其实就是无意识的对象保持.
如果一个对象引用被无意识的保留起来了,那么垃圾回收器不会处理这个对象,也不会处理该对象引用的其他对象,
即使这样的对象只有少数几个,也可能会导致很多的对象被排除在垃圾回收之外,从而对性能造成重大影响,极端情况下会引发Disk Paging(物理内存与硬盘的虚拟内存交换数据),甚至造成OutOfMemoryError.

## Junit Test
对于Web 项目, 通过Junit Test, 可以不必把项目部署到tomcat 上就可以对业务逻辑进行测试.
而且, 实际的项目开发, 业务逻辑和前段展现, 很可能不是同一个人进行, 所以进行业务逻辑的测试时不应该依赖于前端.

测试完成后, 要恢复现场. 例如测试数据库的插入方法, 测试之后, 需要把插入的测试数据删除掉.

# 关键字
谈谈final, finally, finalize的区别

- final 用于声明属性,方法和类,分别表示**属性不可变,方法不可覆盖,类不可继承**.
- finally是异常处理语句结构的一部分,表示总是执行.
- finalize是Object类的一个方法,在垃圾收集器执行的时候会调用被回收对象的此方法,可以覆盖此方法提供垃圾收集时的其他资源回收,例如关闭文件等.

# Servlet
request:浏览器---> web服务器----> Java应用程序服务器
response:顺序反过来

在ASP中,微软把asp程序服务器捆绑在web服务器IIS(Internet information service)中  
Java应用程序服务器(web容器):jackarta tomcat,bea WebLogic  
http web服务器:IIS,Apache  
tomcat内建的有web服务器,但是这个内建的服务器功能比较弱小

## Tomcat
tomcat目录结构

- bin: 存放各种平台下启动和关闭Tomcat的脚本文件.
- common: 在common目录下的lib目录,存放Tomcat服务器和所有web应用都能访问的JAR.
- shared: 在shared目录下的lib目录,存放所有web应用能访问的,但Tomcat不能访问的JAR.
- server: 在server/webapps目录中,存放Tomcat自带的两个APP-admin和manager应用,来管理Tomcat-web服务用的.
- work: Tomcat把各种由jsp生成的servlet文件放在这个目录下.
- temp: Tomcat运行时候存放临时文件用的.
- logs: 存放Tomcat的日志文件
- webapps: web应用的发布目录,把 java开发的web站点或war文件放入这个目录下就可以通过tomcat服务器访问了.
- conf: Tomcat的各种配置文件,最重要的是 server.xml. 

Tomcat配置文件

- conf/server.xml	服务器的主配置文件  
一个<Server>包含一个或多个<Service>, 一个<Service>包含唯一一个< Engine>和一个或多个<Connector>,
多个 <Connector>共享一个<Engine>, 一个<Engine>包含多个<Host>,
每个 <Host>定义一个虚拟主机,包含一个或多个web应用<Context>, <Context>元素是代表一个在虚拟主机上运行的Web应用.

- conf/web.xml	定义所有Web应用的配置(缺省的Servlet定义和MIME类型定义)

- conf/tomcat-user.xml  定义了tomcat用户的信息(用于权限与安全)


Web Application的概念
```
Web Application Name
	WEB-INF
		web.xml: 该web app的配置文件
		lib: 该web app用到的库文件
		classes: 存放编译好的servlet
	META-INF: 存放该web app的上下文信息,符合J2EE标准
```
Web Application可以直接放在webapp下面. 也可以通过配置文件指定到其他目录 <host>里面
```
<Context path="/虚拟路径名" docBase="目录位置" debug="0" reloadable="true"/>
```

MIME:multipurpose Internet mail extensions: 就是设定某种扩展名的文件用一种特定的应用程序来打开的方式类型

## IO
字符流:
```
Printwriter writer=response.getwriter();
writer.println(string);
```

字节流:
```
servletOutputStream out=response.getOutputStream();
out.write(bytes);
```

## Servlet的生命周期
生命全过程:

- 加载 ClassLoader 
- 实例化 new 
- 初始化 init(ServletConfig)
- 处理请求 service doGet doPost
- 退出服务 destroy()

只有一个对象
API中的过程:init()//只执行一次, 第一次初始化的时候
```
public void init(ServletConfig config) throws ServletException
```
service()
```
public void service(ServletRequest req, ServletResponse res) throws ServletException, java.io.IOException
```
destroy()//webapp 退出的时候
```
public void destroy() 
```

Servlet的多线程机制 

Servlet体系结构是建立在Java多线程机制之上的,它的生命周期是由Web容器负责的.

当客户端第一次请求某个Servlet时,Servlet 容器将会根据web.xml配置文件实例化这个Servlet类.当有新的客户端请求该Servlet时,一般不会再实例化该Servlet类,也就是有 多个线程在使用这个实例.

Servlet容器会自动使用线程池等技术来支持系统的运行


## Servlet编程接口

GenericServlet是所有Servlet的鼻祖,
用于HTTP的Servlet编程都通过继承 `javax.servlet.http.HttpServlet` 实现

请求处理方法:(分别对应http协议的7种请求)

- doGet: 响应Get请求,常用
- doPost: 响应Post请求,常用
- doPut: 用于http1.1协议
- doDelete: 用于http1.1协议
- doHead: 仅响应Get请求的头部
- doOptions: 用于http1.1协议
- doTrace: 用于http1.1协议

实例的个数:在非分布的情况下,通常一个Servlet在服务器中有一个实例

```
public void doGet(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException
public void doPost(HttpServletRequest req, HttpServletResponse res) throws ServletException,IOException
```

## cookie
- Cookie以"名-值"对的形式保存数据
- 只能拿自己webapp写入的东西
- 一个servlet/jsp设置的cookies能够被同一个路径下面或者子路径下面的servlet/jsp读到 (路径 = URL)(路径 != 真实文件路径)

set cookie
```
Cookie cookie = new Cookie(name,value);
response.addCookie(cookie);
```

get cookie
```
Cookie[] cookies = request.getCookies();
PrintWriter out = response.getWriter();
for(Cookie cookie in cookies){
	 out.println("<TR>\n" + "<TD>" + cookie.getName() + "</TD>\n" + " <TD>" + cookie.getValue() + "</TD></TR>\n");
}
```

Web上保持状态的手段:cookie session application persistence

## session
- 在某段时间一连串客户端与服务器端的"交易" 
- 在Jsp/Servlet中,如果浏览器不支持Cookie,可以通过URL重写来实现,就是将一些额外数据追加到表示会话的每个URL末尾,服务器在该标示符与其存储的有关的该会话的数据之间建立关联.
	如hello.jsp?jsessionid=1234
- 可以通过程序来终止一个会话.如果客户端在一定时间内没有操作,服务器会自动终止会话.
- 通过HttpSession来读写Session

规则

- 如果浏览器支持Cookie, 创建Session的时候会把SessionID保存在Cookie里
- 如果不支持Cookie, 必须自己编程使用URL重写的方式实现Session

获得session

- getRequestedSessionId():返回随客户端请求到来的会话ID.可能与当前的会话ID相同,也可能不同.
- getSession(boolean isNew):如果会话已经存在,则返回一个HttpSession,如果不存在并且isNew为true,则会新建一个HttpSession

HttpSession的常用方法

- getAttributeNames()/getAttribute()
- getCreateTime()
- getId()
- getMaxInactiveInterval()
- invalidate()
- isNew()
- setAttribute()
- setMaxInactivateInterval()

## Application
- 用于保存整个WebApplication的生命周期内都可以访问的数据
- 在API中表现为ServletContext
- 通过HttpServlet的getServletContext方法可以拿到, `ServletContext application = this.getServletContext();`
- 通过ServletContext的 get / setAttribute方法取得/设置相关属性

## Servlet中的请求转发
- RequestDispatcher接口对象允许将请求转发到其他服务器资源
- 通过RequestDispatcher的forward(HttpServletRequest,HttpServletResponse)方法可以将请求转发
- 通过ServletContext的getRequestDispatcher(String url)方法来获得RequestDispatcher对象,并且指定转发的目标url资源
- 可以通过HttpServletRequest的setAttribute(String name,String value)来设置需要传递的参数,然后在代理servlet中就可以使用HttpServerRequest的getAttribute(String name)来获得对应的值

重定向是在客户端完成的,一般浏览器的地址栏会改变,
而请求分派是在服务器端完成的,url不变,且一般分派到另外一个servlet或者jsp

如果想把某项信息出现在servlet生成的网页上,比如admin的Email:

1. 可以将这项信息写在servlet的代码中,但是hardcode不好
2. 将这项信息写在web.xml的配置文件中,然后再servlet的代码中调用这个配置信息
3. 如果这项信息经常改变,让servlet从一个数据库中读取这个数据(因为如果还是把这个信息放在配置文件中,那么就需要经常重新部署这个应用,这在实际的生产中是不允许的)

servletconfig中的初始化参数只是针对于一个servlet的,
而servletcontext(逻辑上更应该叫做Appcontext)的初始化参数,是针对于整个web应用的,这个应用中所有的servlet,jsp等都可以访问servletcontext的初始化参数.

每个JVM的每个web应用有一个servletcontext(分布式应用,一致性问题)
每个JVM的每个servlet都有一个servletconfig,  
(一个JVM 可以含有多个web 应用, 而每个web 应用又可以含有多个servlet)

一个httpsession只能出现在一个JVM上,当一个用户的请求被指派到另外一个JVM上时,这个用户的session会迁移到相应的JVM上

一个写得好的servlet,你不会看到任何实例变量或者至少看不到非final变量
因为servlet的所有线程都可以处理实例变量,所以你无法保证你用的实例变量不被同一个servlet的其他线程修改
doGet方法不是线程安全的,因为可能出现多个线程同时访问一个servlet的doget方法,但是init方法是线程安全的,web容器能够保证只有一个请求可以访问这个函数,因为servlet只被初始化一次.

- 类变量:类中独立于方法之外的变量,用static修饰
- 实例变量:类中独立于方法之外的变量,不用static修饰
- 局部变量:类的方法中的变量
- final:常量

- forward()永久性的移交任务
- include()请求别人完成工作,然后再由发送者继续工作

对于服务器来说,你的IP地址就是路由器的地址

序列化serialization:将对象的状态信息转换为可以存储或传输的形式的过程.在序列化期间,对象将其当前状态写入到临时或持久性存储区.以后,可以通过从存储区中读取或反序列化对象的状态,重新创建该对象.

# JSP
JSP---Java Server Pages, 拥有servlet的特性与优点(本身就是一个servlet),  可以直接在HTML中内嵌JSP代码  
JSP程序由JSP Engine先将它转换成Servlet代码,接着将它编译成类文件载入执行, 只有当客户端第一次请求JSP时,才需要将其转换,编译

JSP传统语法

- Declaration
- Scriptlet `<%程序代码区%>`, 可以放入任何的Java程序代码
- Expression `<%=       %>`:  =后面必须是字符串变量或者可以被转换成字符串的表达式, 不需要以;结束, 只有一行
- Comment
- Directives `<%@Directive 属性="属性值"%>`: Directive(编译指令)相当于在编译期间的命令, 常见的Directive: page include taglib
- Action动作指令, Action(动作指令)在运行期间的命令, 常见的:
	- jsp:useBean
		- jsp:setProperty
		- jsp:getProperty
	- jsp:include
	- jsp:forward
	- jsp:param
```
<jsp:forward page="urlSpec">
	<jsp:param name="paramName" value="paramValue"/>
</jsp:forward>
```
	- jsp:plugin
	- 嵌入applet
- 内置对象

JSTL

JSF


JSP的内置对象

- out: Out内置对象是一个缓冲的输出流,用来给客户端返回信息.它是`javax.servlet.jsp.JspWriter`的一个实例
- request: request内置对象表示的是调用JSP页面的请求.通常,request对象是`javax.servlet.http.HttpServletRequest`接口的一个实例, 典型应用:通过`request.getParameter("paramName")`
- response: 表示的是返回给客户端的响应, 是`javax.servlethttp.HttpServletResponse`接口的一个实例, 经常用于设置HTTP标题,添加cookie,设置响应内容的类型和状态,发送HTTP重定向和编码URL
- pageContext  用的很少
- session: `<% @page session="true"%>`(默认)--表示session功能已经在jsp页面中启动
- application
- config  用的很少
- exception
- Page 用的很少

表达式会成为`out.print()`的参数,`<%= code %>`等价于`<%out.print(code);%>`

## Servlet和JSP的通信
从JSP调用Servlet可用<jsp:forward>请求信息自动传递到Servlet, 
```
<jsp:forward page="/servlet/servletfile"/>
```
或者通过sendRedirect
```
response.sendRedirect("../servlet/ShowParameters?a=b");
```

从Servlet调用JSP使用 RequestDispatcher接口的forward(req, res)方法
```
getServletConfig().getServletContext().getRequestDispatcher("/ServletUseJsp.jsp").forward(req, resp);
```
或者通过sendRedirect

# Spring
entity(model) --- service --- DAO -- DAOImplementation
解析xml 文件: jdom, dom4j

Spring在读取xml配置文件中的bean的时候采用的是反射机制, 读取的jvm的.class文件(采用classLoader), 然后再用调用类的方法.

动态装配

AOP: Aspect Oriented Programming

The Spring Framework

- **Core Container** consists of the Core, Beans, Context, and Expression Language modules
	- Core module provides the fundamental parts of the framework, including the IoC and Dependency Injection features
	- The Context module builds on the solid base provided by the Core and Beans modules
- **Data Access/Integration** layer consists of the JDBC, ORM, OXM, JMS and Transaction modules
	- The ORM module provides integration layers for popular object-relational mapping APIs, including JPA, JDO, Hibernate, and iBatis
- **Web layer** consists of the Web, Web-Servlet, Web-Struts, and Web-Portlet modules
	- The Web-Servlet module  contains  Spring  model-view-controller  (MVC) implementation for web applications
- Few other important modules like AOP, Aspects, Instrumentation, Web and Test modules

Bean Configuration file(an XML file) acts as cement that glues the beans ie. classes together. 
This file needs to be created **under the src directory**.

The Spring container is at the core of the Spring Framework. 
The container will create the objects, wire them together, configure them, and manage their complete lifecycle from creation till destruction. 
The Spring container uses dependency injection (DI) to manage the components that make up an application.

The container 通过读取配置文件中的metadata来进行上面提到的各种操作.
The configuration metadata can be represented either by XML, Java annotations, or Java code.

The Spring IoC container makes use of Java POJO(Plain Old Java Object) classes and configuration metadata to produce a fully configured and executable system or application.

两种container:

1. Spring BeanFactory Container  
	- This is the simplest container providing basic support for DI and defined by the `org.springframework.beans.factory.BeanFactory` interface.
	- The most commonly used BeanFactory implementation is the XmlBeanFactoryclass.
1. Spring ApplicationContext Container  
	- This container adds more enterprise-specific functionality such as the ability to resolve textual messages from a properties file and 
		the ability to publish application events to interested event listeners. This container is defined by the `org.springframework.context.ApplicationContext` interface
	- The most commonly used ApplicationContext implementations are
		1. FileSystemXmlApplicationContext: 使用xml 文件的绝对路径
		1. ClassPathXmlApplicationContext: xml 文件在classpath的路径里面
		1. WebXmlApplicationContext: Web Application

由于ApplicationContext container 包括 BeanFactory container 的所有功能, 因此一般建议使用ApplicationContext container, 除非是在内存等资源有限的情况或者对性能要求比较高的情况下(例如Mobile).

## Spring Bean
Spring Bean Properties

- class This attribute is mandatory and specify the bean class to be used to create the bean.
- name This attribute specifies the bean identifier uniquely. In XML-based configuration metadata, you use the id and/or name attributes to specify the bean identifier(s).
- scope(`scope = ""`) This attribute specifies the scope of the objects created from a particular bean definition
- constructor-arg This is used to inject the dependencies and will be discussed in next chapters.
- properties This is used to inject the dependencies
- autowiring mode This is used to inject the dependencies
- lazy-initialization mode(`lazy-init = "true"` A lazy-initialized bean tells the IoC container to create a bean instance when it is first requested, rather than at startup.
- initialization method(`init-method = ""`) A callback to be called just after all necessary properties on the bean have been set by the container.
- destruction method(`destroy-method = ""`) A callback to be used when the container containing the bean is destroyed.

Scope 的可选项:

- singleton This scopes the bean definition to a single instance per Spring IoC container (default) 只存在一个
- prototype This scopes a single bean definition to have any number of object instances, 每次需要的时候都会创建一个新的
- request This scopes a bean definition to an HTTP request. Only valid in the context of a web-aware Spring ApplicationContext.
- session This scopes a bean definition to an HTTP session. Only valid in the context of a web-aware Spring ApplicationContext.
- global-session This scopes a bean definition to a global HTTP session. Only valid in the context of a web-aware Spring ApplicationContext.

prototype 不要和init-method, destroymethod 一起使用, applicationcontext 不会监控prototype的lifecycle.

当beans 的init 和destroy 方法都一样时, 可以在beans的层级统一设置
```
<beans default-init-method ="init" default-destroy-method ="destroy" >
	<bean id ="..."  class ="..." ></bean>
</beans>
```

Post processor
postProcessBeforeInitialization: 在初始化一个bean 之前调用的方法
postProcessAfterInitialization: 在初始化一个bean 之后调用的方法.
和之前的init-method, destroy-method 的顺序为: postProcessorBeforeInitialization -> Bean 的创建 -> init-method -> postProcessorAfterInitialization -> destroy-method -> Bean的销毁

Bean 的定义也可以有继承, 只需要在`<bean parent = "pid"></bean>` 中指定parent的id 就可以了.
还可以定义bean template

## Dependency Injection
IoC: Inversion of Control 控制反转(交给容器控制)  
DI: Dependency Injection 依赖注入  
Dependency Injection is merely one concrete example of Inversion of Control.

Dependency Injection (or sometime called wiring) helps in gluing these classes together and same time keeping them independent.

Consider you have an application which has a text editor component and you want to provide spell checking. Your standard code would look something like this:
```
public class TextEditor {
	private SpellChecker spellChecker;
	public TextEditor() {
		spellChecker = new SpellChecker();
	}
}
```
What we have done here is create a dependency between the TextEditor and the SpellChecker. 
In an inversion of control scenario we would instead do something like this:
```
public class TextEditor {
	private SpellChecker spellChecker;
	public TextEditor(SpellChecker spellChecker) {
		this.spellChecker = spellChecker;
	}
}
```
这里, 我们在创建一个texteditor 的时候将spellchecker 作为一个参数传进去, 就可以解决上面实现中依赖问题, 从而得到了很大的灵活性.
在Spring Framework 中, 这种工作时通过一个xml 文件来进行配置的.
Here, we have removed the total control from TextEditor and kept it somewhere else (ie. XML
configuration file) and the dependency ( ie. class SpellChecker) is being injected into the class TextEditor through a Class Constructor.

DI exists in two major variants 

1. Constructor-based dependency injection  
Constructor-based DI is accomplished when the container invokes a class constructor with a number of arguments, each representing a dependency on other class.
2. Setter-based dependency injection  
Setter-based DI is accomplished by the container calling setter methods on your beans after invoking a no-argument constructor or no-argument static factory method to instantiate your bean.

You can mix both, Constructor-based and Setter-based DI but it is a good rule of thumb to use
constructor arguments for mandatory dependencies and setters for optional dependencies.

### Constructor-based dependency injection  
TextEditor 的构造函数
```
public TextEditor(SpellChecker spellChecker) {
		System.out.println("Inside TextEditor constructor.");
		this.spellChecker = spellChecker;
}
```
Beans.xml 中将两者关联起来, spellChecker 作为textEditor的构造函数的参数.
```
<bean id ="textEditor"  class ="com.tutorialspoint.TextEditor" > <constructor-arg ref ="spellChecker" /> </bean>
<bean id ="spellChecker"  class ="com.tutorialspoint.SpellChecker" > </bean>
```
当构造函数需要多个参数时, constructor-arg 的顺序要与构造函数中参数的顺序一致.

### Setter-based Dependency Injection
TextEditor 中设置spellChecker 的方法
```
public void setSpellChecker(SpellChecker spellChecker) {
	System.out.println("Inside SetTextEditor");
	this.spellChecker = spellChecker;
}
```
Beans.xml 将两者关联起来, 通过setter 来设置参数, 而不是构造函数.
```
<bean id ="textEditor"  class ="com.tutorialspoint.TextEditor" >
<property name="spellChecker" ref="spellChecker" />
</bean>

<bean id ="spellChecker"  class ="com.tutorialspoint.SpellChecker" >
</bean>
```
in case you are passing a reference to an object, you need to use ref attribute of <property> tag and 
if you are passing a value directly then you should use value attribute

除了可以按照上面的方法进行设置外, 还可以通过inner bean的方式进行设置(直接将ref 的 bean 写到property 里面).
```
<bean id ="textEditor"  class ="com.tutorialspoint.TextEditor" >
	<property name="spellChecker">
		<bean id ="spellChecker"  class ="com.tutorialspoint.SpellChecker" />
	</property>
</bean>
```

### Spring Injecting Collection

- <list>  This helps in wiring ie injecting a list of values, allowing duplicates.
- <set>  This helps in wiring a set of values but without any duplicates.
- <map> This can be used to inject a collection of name-value pairs where name and value can be of any type.
- <props> This can be used to inject a collection of name-value pairs where the name and value are both Strings.

```
<property name="addressList" >
	<list>
		<value> INDIA </value>
		<value> Pakistan </value>
		<value> USA </value>
		<value> USA </value>
	</list>
</property>

<property name="addressList" >
	<list>
		<ref bean="address1" />
		<ref bean="address2" />
		<value> Pakistan </value>
	</list>
</property>
```

empty string: `<property name="email" value="" />`  
null value: `<property name="email" ><null/></property>`

autowire 自动装配

Spring Annotation Based Configuration  
Annotation injection is performed before XML injection, 
thus the latter configuration will override the former for properties wired through both approaches

annotation 默认是关闭的, 需要在Beans.xml 加入下面的内容来开启
```
<context: annotation-config>
```

Spring Java Based Configuration

## Event Handling in Spring
1. ContextRefreshedEvent  
This event is published when the ApplicationContext is either initialized or refreshed.
This can also be raised using the refresh() method on the ConfigurableApplicationContext interface.
2. ContextStartedEvent  
This event is published when the ApplicationContext is started using the start() method on theConfigurableApplicationContext interface. 
You can poll your database or you can re/start any stopped application after receiving this event.
3. ContextStoppedEvent  
This event is published when the ApplicationContext is stopped using the stop() method on theConfigurableApplicationContext interface. 
You can do required housekeep work after receiving this event.
4. ContextClosedEvent  
This event is published when the ApplicationContext is closed using the close() method on theConfigurableApplicationContext interface. 
A closed context reaches its end of life; it cannot be refreshed or restarted.
5. RequestHandledEvent  
This is a web-specific event telling all beans that an HTTP request has been serviced.

Spring event handling is single-threaded so if an event is published, until and unless all the receivers get the message, the processes are blocked and the flow will not continue.

To listen a context event, a bean should implement the ApplicationListener interface which has just one method `onApplicationEvent()`.

```
public class CStartEventHandler implements ApplicationListener<ContextStartedEvent>{
	public void onApplicationEvent(ContextStartedEvent event) {
		System.out.println("ContextStartedEvent Received");
	}
}

public class CStopEventHandler implements ApplicationListener<ContextStopedEvent>{
	public void onApplicationEvent(ContextStartedEvent event) {
		System.out.println("ContextStopedEvent Received");
	}
}
```

## AOP 面向切面编程
Aspect Oriented Programming entails breaking down program logic into distinct parts called so-called concerns. 
The functions that span multiple points of an application are called cross-cutting concerns and these cross-cutting concerns are conceptually separate from the application business logic. 
There are various common good examples of aspects like logging, auditing, declarative transactions, security, and caching etc.

Spring AOP module provides interceptors to intercept an application, for example, when a method is executed, you can add extra functionality before or after the method execution.

通过AOP, 我们可以在不改变原有的代码情况下, 而增加业务逻辑.

动态代理

AOP 里面的概念

- JointPoint: 连接点
- PointCut: JointPoint集合
- Aspect: 切面
- Advice: 可以简单的理解成 @Before, @AfterReturning @AfterThrowing @After @Around (before and after)
- Introduction An introduction allows you to add new methods or attributes to existing classes.
- Target: 被代理对象, 把我们的逻辑织入到哪个对象上面去
- Weaving: 织布 Weaving is the process of linking aspects with other application types or objects to create an advised object. This can be done at compile time, load time, or at runtime

AOP 的两种实现方式

1. XML Schema based   
Aspects are implemented using regular classes along with XML based configuration.
1. @AspectJ based  
@AspectJ refers to a style of declaring aspects as regular Java classes annotated with Java 5 annotations

## Spring JDBC Framework
Spring JDBC provides several approaches and correspondingly different classes to interface with the database.
the most popular approach which makes use of JdbcTemplateclass of the framework

Instances of the JdbcTemplate class are threadsafe once configured. So you can configure a single instance of a JdbcTemplate and then safely inject this shared reference into multiple DAOs.

DAO: Data Access Object

DataSource 一个抽象接口
因为拿到connection 的方法很多, 所以需要一个抽象接口
dbcp: database connection pool
```
<bean id="dataSource" destroy-method="close" class="org.apache.commons.dhcp.BasicDataSource">
	<property name="driverClassName" value="com.mysql.jdbc.Driver"/>
	<property name="url" value="jdbc:mysql://localhost:3306/db"/>
	<property name="username" value="root"/>
	<property name="password" value="password"/>
</bean>
```
在上面是把数据库的参数直接写到了Beans.xml 的配置文件中, 我们还可以通过下面占位符的方式, 把数据库的连接参数单独放在一个文件中.
```
<bean class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
	<property name="locations">
		<value>classpath:jdbc.properties</value>
	</property>
</bean>
在classpath 的目录下有一个jdbc.properties的文件
<bean id="dataSource" destroy-method="close" class="org.apache.commons.dhcp.BasicDataSource">
	<property name="driverClassName" value="${jdbc.driverClassName}$"/>
	<property name="url" value="${jdbc.driverClassName}$"/>
	<property name="username" value="${jdbc.username}$"/>
	<property name="password" value="${jdbc.password}$"/>
</bean>
```

# mybatis
MyBatis is a first class persistence framework with support for custom SQL, stored procedures and advanced mappings. 
MyBatis eliminates almost all of the JDBC code and manual setting of parameters and retrieval of results. 
MyBatis can use simple XML or Annotations for configuration and map primitives, Map interfaces and Java POJOs (Plain Old Java Objects) to database records.

mybatis-x.x.x.jar
```
<dependency>
<groupId>org.mybatis</groupId>
<artifactId>mybatis</artifactId>
<version>x.x.x</version>
</dependency>
```

# 架构
用户注册的例子
```
client --- jsp --- UserManager --- UserManagerImpl --- User --- UserDAO --- UserDAOImpl --- DB
```
在这里, jsp 里包含了view, 和 control的功能.  
我们可以将strut 2 加进来, 实现control的功能;  
把spring 加进来实现model 的功能, 把UserManager 交给Spring 管理;  
把hibernate加到UserDAOImpl 和 DB 之间, 用来隔绝具体的数据库.
```
client --- jsp --- struts 2 filter --- Action --- view:jsp --- UserManager --- UserManagerImpl --- User --- UserDAO --- UserDAOImpl --- DB
```

# Maven
Maven是一个优秀的项目构建工具.

Maven帮助我们自动化项目构建过程,具体来说包括项目的清理,编译,测试,生成测试报告,打包和部署.
只需要输入很简洁的命令,就能实现这些构建过程.比如输入`mvn clean compile` ,就能完成项目的清理和编译.
同时,Maven提供jar包的依赖管理

[Maven的安装](http://blog.csdn.net/zhangbaidu555/article/details/12955639)

## [命令行创建Maven项目](http://my.oschina.net/u/1162561/blog/145869)
1. 打开命令行, 输入 `mvn archetype:generate`
1. 界面上会输出很多信息,并提示输入要选择的archetype编号, 这里我们直接按回车键,即选择默认编号.
1. 然后提示选择其version, 按回车键,选择默认编号对应的version.
1. 之后会提示输入groupId, artifactId, groupId 输入 com.eric.test, artifactId 输入test.(groupId指 创建项目的小组的唯一标示. ArtifactId是 该产品id)
1. 后面的version和package,系统都提供了默认值.如果要采纳默认值,直接按回车键.
1. 最后Y:处,表示是否确认以上信息,填Y按回车则开始创建.
1. 至此,项目创建成功.
1. `mvn compile` 进行编译
1. `mvn test` 进行测试
1. `mvn package` 打包, 会在target 的文件夹生成一个`test-1.0-SNAPSHOT.jar` 的文件, 也就是artifactId-version.jar 的命名方式
1. `java -cp target/test-1.0-SNAPSHOT.jar com.eric.test.App` 就会运行jar 包中的 com.eric.test package下的App(App 是默认创建的含有Main函数的类), 输出为 `Hello World!`

![生成的目录结构](http://i.imgbox.com/OkX8oksm.png)

注意:

1. 如果是第一次构建项目,所有依赖的jar包都要从maven的中央仓库下载.
2. SNAPSHOT表示该版本处于开发阶段,是不稳定的版本.

生成的pom.xml的内容如下:
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.eric.test</groupId>
  <artifactId>test</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>jar</packaging>

  <name>test</name>
  <url>http://maven.apache.org</url>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>

  <dependencies>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>3.8.1</version>
      <scope>test</scope>
    </dependency>
  </dependencies>
</project>
```

pom.xml 文件中个条目的解释

- modelVersion: POM model version (always 4.0.0).
- groupId:  创建项目的小组的唯一标示. Group or organization that the project belongs to. Often expressed as an inverted domain name
- artifactId:  该产品id.会作为产生的jar/war包名的一部分.它和version一起构成输出包的包名.
- Version: 产品版本
- Packaging: 打包的类型, 默认是jar, 可以设置为war
- Name: 项目名称,用于maven产生的文档中.
- url:指定项目站点,通常用于maven产生的文档中. 
- description:描述此项目,通常用于maven产生的文档中.
- Properties: 属性.这里设置编码方式为UTF-8
- Dependencies: 依赖的jar包 

我们注意到,在dependency中,也有<groupId>,<artifactId>,<version>这样的节点.这三个元素是jar包坐标的重要组成部分.它指定了我们要从中央仓库获取的jar.

那么如何知道所要jar的坐标呢?
可以在[Maven搜索站](http://search.maven.org/)中搜索.
在搜索结果中找到需要的jar包,把相应的坐标填写在pom.xml文件中.

此外,我们还注意到,<dependency>下有一个<scope>test</scope>.它设定了jar包的作用域,这里指junit包仅在测试时使用.在正式打包的时候,并不会引入到工程中.
如果去掉这行代码,那么Maven会采用默认的compile范围,那么无论在测试或者打包时,该jar包都会被工程引用,生成的包中会含有junit. 

如果在运行jar 包时, 出现类似于下面的错误, 可能是由于jar 不可执行造成的
```
[eric@alien spring-demo]$ java -cp target/gs-maven-0.1.0.jar hello.Application
Exception in thread "main" java.lang.NoClassDefFoundError: org/springframework/context/ApplicationContext
        at java.lang.Class.getDeclaredMethods0(Native Method)
        at java.lang.Class.privateGetDeclaredMethods(Class.java:2570)
        at java.lang.Class.getMethod0(Class.java:2813)
        at java.lang.Class.getMethod(Class.java:1663)
        at sun.launcher.LauncherHelper.getMainMethod(LauncherHelper.java:494)
        at sun.launcher.LauncherHelper.checkAndLoadMain(LauncherHelper.java:486)
Caused by: java.lang.ClassNotFoundException: org.springframework.context.ApplicationContext
        at java.net.URLClassLoader$1.run(URLClassLoader.java:366)
        at java.net.URLClassLoader$1.run(URLClassLoader.java:355)
        at java.security.AccessController.doPrivileged(Native Method)
        at java.net.URLClassLoader.findClass(URLClassLoader.java:354)
        at java.lang.ClassLoader.loadClass(ClassLoader.java:425)
        at sun.misc.Launcher$AppClassLoader.loadClass(Launcher.java:308)
        at java.lang.ClassLoader.loadClass(ClassLoader.java:358)
        ... 6 more
```
需要借助maven-shade-plugin 来生成可执行的jar 包.
```
<plugin>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-shade-plugin</artifactId>
  <version>1.2.1</version>
  <executions>
    <execution>
      <phase>package</phase>
      <goals>
        <goal>shade</goal>
      </goals>
      <configuration>
      <transformers>
        <transformer implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
          <mainClass>package_path_to_mainclass.mainclass</mainClass>
        </transformer>
      </transformers>
    </configuration>
  </executions>
</plugin>
```

## Maven Phases
Although hardly a comprehensive list, these are the most common default lifecycle phases executed.

- validate: validate the project is correct and all necessary information is available
- compile: compile the source code of the project
- test: test the compiled source code using a suitable unit testing framework. These tests should not require the code be packaged or deployed
- package: take the compiled code and package it in its distributable format, such as a JAR.
- integration-test: process and deploy the package if necessary into an environment where integration tests can be run
- verify: run any checks to verify the package is valid and meets quality criteria
- install: install the package into the local repository, for use as a dependency in other projects locally
- deploy: done in an integration or release environment, copies the final package to the remote repository for sharing with other developers and projects.
- clean: cleans up artifacts created by prior builds, 会删除target 目录
- site: generates site documentation for this project
mvn eclipse:eclipse 生成 Eclipse 项目文件及包引用定义,注意,需确保定义Classpath Variables: M2_REPO,指向本地maven类库目录.

Phases are actually mapped to underlying goals. The specific goals executed per phase is dependant upon the packaging type of the project. 
For example, package executes jar:jar if the project type is a JAR, and war:war if the project type is - you guessed it - a WAR.

An interesting thing to note is that phases and goals may be executed in sequence.
```
mvn clean dependency:copy-dependencies package
```
This command will clean the project, copy dependencies, and package the project (executing all phases up to package, of course).

# MyEclipse
[MyEclipse10.0中的Maven配置](http://blog.csdn.net/zhangbaidu555/article/details/12955639)
