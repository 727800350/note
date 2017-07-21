# Java
[环境变量配置](http://jingyan.baidu.com/article/f96699bb8b38e0894e3c1bef.html)

## 命令行调用

- -Xms<size>        set initial Java heap size
- -Xmx<size>        set maximum Java heap size
- -Xss<size>        set java thread stack size

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

# 关键字
谈谈final, finally, finalize的区别

- final 用于声明属性,方法和类,分别表示**属性不可变,方法不可覆盖,类不可继承**.
- finally是异常处理语句结构的一部分,表示总是执行.
- finalize是Object类的一个方法,在垃圾收集器执行的时候会调用被回收对象的此方法,可以覆盖此方法提供垃圾收集时的其他资源回收,例如关闭文件等.

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

