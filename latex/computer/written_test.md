## 静态, 动态链接库

如下关于链接的说法错误的是()
1. 一个静态库中不能包含两个同名全局函数的定义
1. 一个动态库中不能包含两个同名全局函数的定义
1. 如果两个静态库都包含一个同名全局函数,他们不能同时被链接
1. 如果两个动态库都包含一个同名全局函数,他们不能同时被链接

选择C

定义在这3个地方的函数,会调用哪个函数呢?
1. 程序和静态库定义了同名函数,链接报错:重定义
2. 程序或动态库定义的同名函数,会覆盖动态库中定义的函数
3. 动态库中定义的同名函数,先链接覆盖后链接的函数

下面有关静态链接和动态链接,说法正确的是?
1. 静态链接库 不能有同名全局函数
1. 动态链接库不能有同名全局函数
1. 两个静态链接库 的同名全局函数不能同时加载
1. 两个动态链接库的同名全局函数不能同时加载

两个静态库有同名函数, 链接时会出错, 报函数重复定义.
同名函数出现在动态库中,编译链接都可以通过,但是调用会出问题,会出现覆盖问题.
静态链接方法:#pragma comment(lib, "test.lib") ,静态链接的时候,载入代码就会把程序会用到的动态代码或动态代码的地址确定下来
动态链接方法:LoadLibrary()/GetProcessAddress()和FreeLibrary(),使用这种方式的程序并不在一开始就完成动态链接,而是直到真正调用动态库代码时,载入程序才计算(被调用的那部分)动态代码的逻辑地址,
然后等到某个时候,程序又需要调用另外某块动态代码时,载入程序又去计算这部分代码的逻辑地址,所以,这种方式使程序初始化时间较短,但运行期间的性能比不上静态链接的程序.

目标程序,又称为"目的程序",为源程序经编译可直接被计算机运行的机器码集合,在计算机文件上以.obj作扩展名

# IO
```
#include <stdio.h>
int main(){
        char *a = "hello";
        char *b = "tencent";
        printf("%s %s %s\n", a, b);
        return 0;
}
```
编译可以通过, 只是会产生警告, 
输出结果不可预知

```
#include <stdio.h>
int main(){
        int i = 1;
        printf("%d, %d\n", i++, i+1);
        return 0;
}
```
编译可以通过, 只是会产生警告.
输出结果取决于编译器, gcc 编译输出1,2, clang 编译输出1,3

```
#include <stdio.h>
int main(){
		int i = 1;
		printf("%d\n", i, i+1);
		return 0;
}
```
编译可以通过, 只是会产生警告.
输出1.
因为, 读取参数是从右开始的, 所以i 是后入栈的, 当需要参数填充到 %d时, 就是后入栈的i 先出栈.

# 内存对齐
```
#include <stdio.h>
#pragma pack(push) //保存对齐状态
#pragma pack(4)//设定为4字节对齐

typedef struct _A{
        char a;
        int b;
        float c;
        double d;
        int *pa;
        char *pc;
        short e;
}A;
#pragma pack(pop) //恢复对齐状态

int main(){
        printf("size = %lu\n", sizeof(A));
        return 0;
}
```
输出结果: 在64位的系统上, 输出为 40.
64位的系统上, 指针占8个字节.

char a 占用4个字节, int b, float c 各占4个字节, double d 占8个字节, int *pa, char *pc 各占8个字节, short e 占4个字节.
所以总的大小占用: 40个字节
```
typedef struct _A{
        char a;
        short e;
        int b;
        float c;
        double d;
        int *pa;
        char *pc;
}A;
```
将short e 移到char a的后面, char a 和short e 一起占用4个字节, short e占用后两个字节.
这时输出为36.

当把对齐字节数改为8时, 
原来的A, 共占用48个字节.
char a 和int b一起占用8个字节, float c 占8个字节, double d 占8个字节, int *pa, char *pc 各占8个字节, short e 占8个字节.

对于将short e 移动到前面的A, 输出40.
char a , short e, int b一起占用8个字节, float c 占8个字节, double d 占8个字节, int *pa, char *pc 各占8个字节.

在MVC模式中, 进行业务流程/状态的处理以及业务规则的制定的是()
1. Model 2. Manager 3. View 4. Controller  
选择4, controller, 因为是流程

关于递归, 下面说法正确的是()
1. 递归函数是一个直接调用自己或通过一系列的调用语句间接地调用自己的函数.
1. 对二叉树, 广义表适宜用递归来进行描述操作.
1. 递归函数重复多次的自我调用不会对栈造成任何影响
1. 递归函数面对的大部分问题都可以通过迭代函数来解决.

选择1,2,4
