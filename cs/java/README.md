## Data Types
- 基本数据类型只有8个, boolean, char, byte, short, int, long, float, double. 除了基本类型(primitive type)和枚举类型(enumeration type), 剩下的都是引用类型.
- 但是为了将基本数据类型当成对象操作, Java 为每一个基本数据类型都引入了对应的包装类型(Wrapper Class), Boolean, Character, Byte, Short, Integer, Long, Float, Double
- byte 是一个字节, char 2个字节(16bit)
- Java 中没有unsigned 的修饰符

```java
switch(expr)  // expr 可以是byte, short, char, int, enum, String, 但是不能是long
```

### 引用类型
- `x == y`: 如果引用的是同一个对象, 则为true
- `x.equals(y) == true`, 那么它们的hashCode 一定相同(hashCode 的规定就是这样的).

### String
String 是final 类, 不可以被继承, String 是只读字符串.

- `x.equals(y)`: true if and only if the argument is not null and 包含同样的内容.
- `length()`: length 是方法
- 'Integer.parseInt()': 将 String 转换为int

String 和 StringBuilder, StringBuffer的区别?  
Java 平台提供了两种类型的字符串: String和 StringBuffer/StringBuilder, 它们可以储存和操作字符串.
其中String是只读字符串,也就意味着String引用的字符串内容是不能被改变的.
而StringBuffer和StringBuilder类表示的字符串对象可以直接进行修改.
StringBuilder是JDK 1.5中引入的,它和StringBuffer的方法完全相同,区别在于它是在单线程环境下使用的,因为它的所有方面都没有被synchronized修饰,因此它的效率也比StringBuffer略高.

### Array
对象数组与基本类型数组在运用上几乎一模一样, 唯一差别在于, 前者持有得是reference, 后者直接持有基本型别之值.

- `length`: length 是属性

对于基本类型(比如int)的数组，在new语句创建数组实例时，会为该数组分配指定长度的内存空间，并把数组的每个元素的初始化为0. 而引用类型的数组在创建的时候初始化值为null
```java
int[] nums = new int[3];  // 三个数字的值都是0
int[] nums = new int[]{1, 2, 3};  // 创建的时候进行初始化

Object[] objs = new Object[3];  // 创建了一个Object数组，长度为3，值都是null
objs[0] = new Integer(5);
objs[1] = new String("hello world");  // 将前两个Object 进行赋值

Object[] objs = new Object[]{new Integer(47), new Float(3.14), new Double(11.11), new String("string")}; //  创建的时候初始化
```

