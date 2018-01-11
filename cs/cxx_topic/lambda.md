C++11 的 lambda 表达式规范有好几种

1. `[ capture ] ( params ) { body }`

- capture 指定了在可见域范围内 lambda 表达式的代码内可见得外部变量的列表，具体解释如下
	- `[]`: 未定义变量.试图在Lambda内使用任何外部变量都是错误的.
	- `[x, &y]`: x 按值捕获, y 按引用捕获.
	- `[&]`: 用到的任何外部变量都隐式按引用捕获
	- `[=]`: 用到的任何外部变量都隐式按值捕获
	- `[&, x]`: x显式地按值捕获. 其它变量按引用捕获
	- `[=, &z]`: z按引用捕获. 其它变量按值捕获
- params 指定 lambda 表达式的参数

```C++
#include <vector>
#include <algorithm>
std::vector<int> vec{0, 1, 2, 3, 4};
std::for_each(std::begin(vec), std::end(vec), [](int &x){x++;}); // 修改vec 里面的元素, 因此需要用ref

int sum = 0;
std::for_each(std::begin(vec), std::end(vec), [&sum](int x){sum += x;}); // 不修改vec 的元素, 又是基本类型, 可以直接用value
```

