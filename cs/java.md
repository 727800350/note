## 命令行调用
```java
public class Main{
	public static void main(String[] args){
		for(int i = 0; i < args.length; ++i){
			System.out.println(args[i]);
		}
	}
}
```
当不传入参数时, `args.length` 为0, 而不像C++ 中程序的名字也占用位置.

## Data Types
基本数据类型只有8个, boolean, char, byte, short, int, long, float, double.
除了基本类型(primitive type)和枚举类型(enumeration type), 剩下的都是引用类型.

但是为了将基本数据类型当成对象操作, Java 为每一个基本数据类型都引入了对应的包装类型(Wrapper Class), 从JDK1.5 开始, 引入了自动的装箱/拆箱机制, 使得二者可以相互转换.  
Boolean, Character, Byte, Short, Integer, Long, Float, Double

```java
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

```java
short s1 = 1;
s1 = s1 + 1; // 编译出错, 1 是int 类型, s1 + 1 也是int 类型, 因此需要强制转换

short s1 = 1;
s1 += 1; // 正确, 隐含的强制转换
```

装箱与拆箱
```java
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
- `x == y`: 如果引用的是同一个对象, 则为true
- `x.equals(y) == true`, 那么它们的hashCode 一定相同(hashCode 的规定就是这样的).

### String
String 是final 类, 不可以被继承, String 是只读字符串.

- `x.equals(y)`: true if and only if the argument is not null and 包含同样的内容.

String和 StringBuilder, StringBuffer的区别?  
Java 平台提供了两种类型的字符串:String和 StringBuffer/StringBuilder, 它们可以储存和操作字符串.
其中String是只读字符串,也就意味着String引用的字符串内容是不能被改变的.
而StringBuffer和StringBuilder类表示的字符串对象可以直接进行修改.
StringBuilder是JDK 1.5中引入的,它和StringBuffer的方法完全相同,区别在于它是在单线程环境下使用的,因为它的所有方面都没有被synchronized修饰,因此它的效率也比StringBuffer略高.

### Array
对象数组与基本类型数组在运用上几乎一模一样, 唯一差别在于, 前者持有得是reference, 后者直接持有基本型别之值.

对于基本类型(比如int)的数组，在new语句创建数组实例时，会为该数组分配指定长度的内存空间，并把数组的每个元素的初始化为0. 而引用类型的数组在创建的时候初始化值为null
```java
int[] nums = new int[3];  // 三个数字的值都是0
int[] nums = new int[]{1, 2, 3};  // 创建的时候进行初始化

Object[] objs = new Object[3];  // 创建了一个Object数组，长度为3，值都是null
objs[0] = new Integer(5);
objs[1] = new String("hello world");  // 将前两个Object 进行赋值

Object[] objs = new Object[]{new Integer(47), new Float(3.14), new Double(11.11), new String("string")}; //  创建的时候初始化
```

- `length`: length 属性

### ArrayList
ArrayList "会自动扩增容量的Array", 类似于C++ 中的 `std::vector`

```java
ArrayList<String> cityList = new ArrayList<String>();
cityList.add("London");
```

- `add()`将对象插入
- `get(i)`配合索引值将它们取出, 不支持`[]`

# math
```java
Math.round(11.5) = 12
Math.round(-11.5) = 11
```
Math.round 都是通过 + 0.5 然后向下取整

