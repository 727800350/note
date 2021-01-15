# IO
- Console 类: Read从标准输入流读取下一个字符.
- ReadKey ()获取用户按下的下一个字符或功能键.按下的键显示在控制台窗口中.ReadKey(Boolean)获取用户按下的下一个字符或功能键.按下的键可以选择显示在控制台窗口中.
- ReadLine从标准输入流读取下一行字符.
- Write()将指定值的文本表示形式写入标准输出流.

重定向
```csharp
StreamWriter log_out = null;       
log_out = new StreamWriter("logfile.txt");
// Redirect standard out to logfile.txt. 
```

```c#
Console.SetOut(log_out);
//执行一次程序将内容输出到重定向的文件中,再执行一次程序,上次的内容会消失(好像是文件又重新建的)

Console.SetOut(Console.Out);
Console.in, console.out, console.error
```
文件流
```c#
FileStream(string path,FileMode mode)
FileMode.Append     //把输出附加到文件末端
FileMode.Create     //创建一个新的输出文件,已存在的文件会被销毁

FileMode.createNew  //
FileMode.open       //打开一个已经存在的文件

FileMode.openOrCreate       //
FileMode.Truncate   //打开一个已经存在的文件,文件的长度减小为0
```

# Data Type
对象之间的赋值是引用赋值,类似于指针的形式,而不是按值赋值

```c#
int i = new int(); // initialize i to zero
int j = new int();
j = i;//这样还是按值赋值的,i与j的地址是不一样的
```

- String.Trim: 返回一个新字符串,它相当于从当前String 对象中移除了一组指定字符的所有前导匹配项和尾随匹配项.
- Trim      从当前 String 对象移除所有前导空白字符和尾部空白字符.
- string.IsNullOrEmpty()//判断空值最好用这个方法

```c#
Int.tryparse(str,out num);//返回true or false
int.Parse()将数字的字符串表示形式转换为其等效的 32 位有符号整数.
int num = int.Parse(Console.ReadLine());
```

string之间的赋值是按值赋值的

Convert类的静态方法用于支持与 .NET Framework 的基础数据类型之间的转换.
受支持的基类型是 Boolean, Char, SByte, Byte, Int16, Int32, Int64, UInt16, UInt32, UInt64, Single, Double, Decimal, DateTime 和 String.
