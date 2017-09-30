GNU C的一大特色(却不被初学者所知)就是`__attribute__`机制.
`__attribute__`可以设置函数属性(Function Attribute),变量属性(Variable Attribute)和类型属性(Type Attribute).

函数属性(Function Attribute)
函数属性可以帮助开发者把一些特性添加到函数声明中,从而可以使编译器在错误检查方面的功能更强大(通过 –Wall 来击活该功能).

`__attribute__` format
该`__attribute__`属性可以给被声明的函数加上类似printf或者scanf的特征,它可以使编译器检查函数声明和函数实际调用参数之间的格式化字符串是否匹配.该功能十分有用,尤其是处理一些很难发现的bug.
format的语法格式为:
`format(archetype, format string index, first parameter index)`

- format属性告诉编译器,按照printf, scanf, strftime或strfmon的参数表格式规则对该函数的参数进行检查.
- archetype指定是哪种风格,
- format string index指定传入函数的第几个参数是格式化字符串, 其中index 从 1 开始算
- first parameter check指定从函数的第几个参数开始按上述规则进行检查

```C++
void my_printf(const char *fmt, ...){
	const int line_size = 1024;
	char buffer[line_size] = {'\0'};
	va_list vl;
	va_start(vl, fmt);
	vsprintf(buffer, fmt, vl);
	fputs(buffer, stdout);
	va_end(vl);
}
```
代码不复杂,使用了va_list等几个宏和vsprintf来实现可变参数.但调用my_printf不会进行参数类型检查.

当把函数声明为: `void my_printf(const char *fmt, ...);`
```C++
	int a = 250;
	char *p = NULL;
	my_printf("hehe %d, %s\n", p, a);
	my_printf("ddd: %d\n");
	my_printf("ddd: \n", a);
```
这段代码编译不会报错, 原因就是my_printf不够严格,当然对于指针的打印,可以使用%d,但如果使用%s来打印一个整数,就会有段错误.
如果在编译阶段就能看到警告,就不会等到运行时才发现,特别是当代码工程大,而打印语句在一定条件下才触发,问题就更难找了.

当把函数声明为: `void my_printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));`, 编译时就会报下面的错误
```
attribute.cpp: In function `int main()':
attribute.cpp:16: warning: int format, pointer arg (arg 2)
attribute.cpp:16: warning: format argument is not a pointer (arg 3)
attribute.cpp:17: warning: too few arguments for format
attribute.cpp:18: warning: too many arguments for format
```

如果N,M指定的位置不对,编译会报错,提示: `error: format string argument not a string type`

特别要说明的是在C++类中如使用`__attribute__((format(printf, N, M)))` 则要注意参数位置,C++默认隐藏有this指针,所以N,M的值要加1.但是类的静态成员没有this指针.

