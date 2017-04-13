# IO
同一个文件可以用fopen同时打开多次, 读取是独立的, 各个FILE指针是不相互干扰的.

- `FILE *fopen(const char *path, const char *mode);`
- `int fclose(FILE *stream);`

- `char *fgets(char *s, int n, FILE *stream);`: 最多读 n - 1 个字符, `\n`也会被存储起来, s[n-1]存储`\0`作为字符串的结尾, 发生错误或者没有内容可读, 返回NULL
```C++
int ret = 0;
const int MAX_LEN = 1024;
char *line = new char[MAX_LEN];
while(fgets(line, MAX_LEN, stdin) != NULL){
	line[strlen(len) - 1] = '\0'; ## set '\n' to '\0', could also strchr \n first to make sure
	ret = process(line);
	if(ret != 0){
		fprintf(stderr, "process error\n");
		return -1;
	}
}
delete []line;
```

- `size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);`
```c
while(true){
	ret = fread(buffer, sizeof(char), buffer_size, stdin);
	if(ret < 0){
		fprintf(stderr, "read error");
		return -1;
	}
	if(ret == 0){
		fprintf(sterr, "eof");
		break;
	}
}
```
- `size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);`
- `void setbuf(FILE *steam, char *buf);` 将buf设置为stream 的缓冲区. 为了关闭缓冲,可以将buf参数设置为NULL.
- `void setlinebuf(FILE *stream);`
- `int snprintf(char *str, size_t size, const char *format, ...);`: write at most size bytes, including the trailing '\0'('\0' 是自动加的)

printf是一个行缓冲函数, 先写到缓冲区(默认一般为 1024 bytes), 满足条件后, 才将缓冲区刷到对应文件中, 刷缓冲区的条件如下:

1. 缓冲区填满
2. 写入的字符中有\n 或者 \r
3. 调用fflush手动刷新缓冲区.  当我们执行printf的进程或者线程结束的时候会主动调用flush来刷新缓冲区
4. 调用scanf要从缓冲区中读取数据时,也会将缓冲区内的数据刷新

满足上面4个条件之一缓冲区就会刷新(真正调用write来写入)

注: fork(返回值为0的是子进程) 的时候, 会将缓冲区也一起拷贝到子进程

对齐

- `printf("%15s\n", insertTime);`: 右对齐,15位长度,不够补空格
- `printf("%015s\n", insertTime);`: 右对齐,15位长度,不够补0
- `printf("%-15.2f\n", insertTime1);`: 左对齐,15位长度,带两位小数,不够补空格

文件位置跳转

`int fseek(FILE *stream, long offset, int base)`: 设置文件指针stream的位置(从base偏移offset字节). stream 不能为stdin, stdin 是流式的.

- `SEEK_SET`: 文件开头
- `SEEK_CUR`: 当前位置
- `SEEK_END`: 文件结尾

获得文件长度
```C
fseek(fp, 0, SEEK_END);
len = ftell(fp);
fseek(fp, 0, SEEK_SET);
```

# 运算符
[优先级表](https://www.slyar.com/blog/c-operator-priority.html)

- `<<`, `>>`: 左移,右移
- `&` or `bitand`: 按位与
- `|` or  `bitor`: 按位或, `|=`: 按位或后赋值
- `^` or  `xor`: 按位异或(对应位相同的,结果为 0,不相同为 1)
- `&&` or `and`: 逻辑与
- `||` or `or`: 逻辑或

注: bitand, bitor, xor, and, or 是C++标准中的

# Data Types

```
#include <stdint.h>
uint64_t, int64_t, uint32_t, int32_t, uint16_t, int16_t, uint8_t, int8_t ...

#include <limits.h>
INT_MAX, INT_MIN
```

# STL 标准模板库
## [std::vector](http://www.cplusplus.com/reference/vector/vector/)
vector封装了数组, vector使用连续内存存储的,支持[]运算符; 对于新添加的元素, 从尾部追加

Do not use pointers to std::vector, 也就是说直接使用std::vector这个对象,而不要使用这个对应的指针.
This class already manage memory allocation and deallocation. 在超出变量的scope, 会自动调用它的析构函数来释放内存

`#include <vector>`

- size(): 容器元素的个数
- empty(): 用来检测容器是否为空的.
- `void push_back (const value_type& val)`: 将元素拷贝到新分配的内存中. 如果元素是指针, 那么只拷贝指针本身, 而不会拷贝指针所指向的实际内容, 也就是值(指针本身的值)拷贝.
	对于string 等object, 即使push_back中传入的参数是reference(别名) 类型, push到vector中的是一个完整的拷贝, 而不是一直指向原来的object 的指针,
	所以即使原来的object被删除了, vector中的仍然可以正常访问.
- erase: 删除元素, 如果元素是指向某个对象的指针, 元素本身在该vector种会被删除, 但是指针所指向的对象不会被删除
- back: Returns a reference to the last element in the vector.
- clear: 可以清空所有元素, 也就是改变vector 的size, 但是capacity 一般不会改变.
- swap: vector<T>().swap(x); 改变x 占用内存的方式, 实际上是把x 的内存交换给一个新的vector<T>()对象

初始化一个vector
```
int array[] = {-1, 1, 2, 7, 11, 15};
std::vector<int> vec(array, array + sizeof(array) / sizeof(int));
```
[vector push back operation demo](../demo/cxx/stl/vector-push_back.cpp)

## [std::list](http://www.cplusplus.com/reference/list/list)
implemented as doubly-linked lists

- `push_front`: 在头插入
- `push_back`: 在尾插入

## [std::unordered map](http://www.cplusplus.com/reference/unordered_map/unordered_map/)
Internally, the elements are not sorted in any particular order, but organized into buckets depending on their hash values
to allow for fast access to individual elements directly by their key values (with a constant average time complexity on average).

`unordered_map` containers are faster than map containers to access individual elements by their key,
although they are generally less efficient for range iteration through a subset of their elements.

尽量不要使用`char *` 作为key, 而要使用string.
`char *`是一个地址, 也就是一个unsigned int, 所以实际上的key 是一个unsigned int 类型的值

`#include <unordered_map>`

- `std::unordered_map::operator[]`: If k matches the key of an element in the container, the function returns a reference to its mapped value.
	If k does not match the key of any element in the container, the function inserts a new element with that key(值为mapped value 的类型的默认值) and
	returns a reference to its mapped value. 反应在map.size() 上
- `std::unordered_map::find(const key_type& k)`: returns an iterator if found, otherwise it returns `map.end()`
- `map.insert(pair)`: 向map 中添加元素
	- `std::pair<std::string,double> myshopping ("baking powder",0.3);`
	- `std::make_pair<std::string,double>("eggs",6.0)`

将整个container 作为一个整体:
```C++
for(auto it = mymap.begin(); it != mymap.end(); ++it){
	std::cout << " " << it->first << ":" << it->second << std::endl;
}
```

分bucket单独遍历:
```C++
std::cout << "mymap's buckets contain:\n";
for(unsigned i = 0;i < mymap.bucket_count(); ++i){
	for(auto local_it = mymap.begin(i); local_it!= mymap.end(i); ++local_it ){
		std::cout << " " << local_it->first << ":" << local_it->second << std::endl;
	}
}
```

## [std::map](http://www.cplusplus.com/reference/map/map/)
- `size_type std::map::count(const key_type& k) const`: Count elements with a specific key
- `size_type erase (const key_type& k);`: 删除元素, 返回1

```C++
#include <map>
for(std::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
	std::cout << it->first << " => " << it->second << '\n';
}
```

## [std::priority queue](http://www.cplusplus.com/reference/queue/priority_queue/)
```C++
template<class T,
	class Container = std::vector<T>,
	class Compare = std::less<typename Container::value_type>
	> class priority_queue;
```
1. 第一个参数指定元素的类型, 可以是简单的例如int, 也可以是复杂的, `std::pair<int, int>` 等类型
1. 第二个参数指定 priority queue 使用的底层存储结构
1. 第三个参数指定排序标准, 默认使用std::less 进行compare, 也就是降序, 因此 top 每次取到的都是最大值

`#include <queue>`

- `empty()`
- `size_type size() const`
- `const value_type& top() const`: Returns a constant reference to the top element
- `void pop()`: Remove top element, 注意, 这个函数没有返回值, 一般和top() 结合使用
- `void push (const value_type& val)`

[demo](../demo/cxx/stl/priority_queue.cpp)

## [algorithm](http://www.cplusplus.com/reference/algorithm/)
- `template <class BidirectionalIterator> void reverse(BidirectionalIterator first, BidirectionalIterator last)`: std::vector 可以直接使用, std::string 不能使用(因为string 是immutable)
- `template <class RandomAccessIterator, class Compare> void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)`: 最后一个参数可选, 对于简单类型的数据, 默认是升序, 不保证stable, `std::stable_sort` 可以保证

# 参数传递
当把数组作为参数传递给函数后, 实际上传递的是一个指针, 所以在函数里面用sizeof和在函数外面对数组用sizeof 得到的结果是不一样的.

```C++
void foo(vector<int> bar); // by value
void foo(vector<int> &bar); // by reference (non-const, so modifyable inside foo
void foo(vector<int> const &bar); // by const-reference
```
You can also choose to pass a pointer to a vector void `foo(vector<int> *bar)`,
but unless you know what you know exactly what are doing, do not do this.

# std::string and char
## [std::string](http://www.cplusplus.com/reference/string/string/)
- `string (const char* s)`
- `string (const char* s, size_t n)`: 当用字符数组表示的二进制串中可能含有 0 字符时, 需要明确二机制串的长度
- `size()` or `length()`: Return length of string, `capacity()` 返回的是storage space currently allocated for the string
- `c_str()`: 返回一个分配给`const char*`的地址,其内容已设定为**不可变更**
- `find()`: 第一个参数可以为 `string, char, const char*`类型, 若没有找到, 返回std::string::npos; 第二个参数pos(默认为0)表示从string 的pos位置开始查找
- `string substr(size_t pos = 0, size_t len = npos) const`: 新构造一个string 对象, len 是可以为0的, 表示std::string(""), 默认到string的结尾

### [字符数组与字符指针](http://blog.csdn.net/qiumm/article/details/5657120)
- `char *str1 = "abc";` 字符指针指向的是一个**字符串常量**(存储在程序的常量区)的首地址, str1即指向字符串的首地址.  
	所以尽管str1的类型不是`const char *`,并且`str1[0] = 'x'`;也能编译通过, 但是执行`str1[0] = 'x';`就会发生运行时异常,因为这个语句试图去修改程序常量区中的东西.  
	但是建议的写法应该是**`const char* str1 = "abc";`**, 这样如果后面写`str1[0] = 'x'`, 的话编译器就不会让它编译通过,也就避免了上面说的运行时异常.  
	如果这个语句写在函数体内,那么虽然这里的`"abc"`被放在常量区中,但是str1本身只是一个普通的指针变量,所以ptr是被放在栈上的, 只不过是它所指向的东西被放在常量区罢了.  
	**C++ 中这种写法会warning**

- `char str2[] = "abc";`: str2是字符数组,它存放了一个字符串, 编译会自动在str2 末尾添加`\0`.  
	str2是一个数组,可以改变数组中保存的内容(但是数组的名字str2本身, 它是一个常量, 也就是说str2 is not assignable)
	因为定义的是一个字符数组,所以就相当于定义了一些空间来存放"abc",而又因为字符数组就是把字符一个一个地存放的,
	所以编译器把这个语句解析为 `char str2[3] = {'a','b','c'};`, 然后补零, 所以最终结果是 `char str2[4] = {'a','b','c','\0'};`  
	如果这个语句是在函数内部写的话, 那么这里的`"abc\0"`, 因为不是常量, 所以应该被放在栈上.

## C API
### string.h
- `void *memset(void *s, int c, size_t n);`
- `void *memcpy(void *dest, const void *src, size_t n);`
- `int memcmp(const void *s1, const void *s2, size_t n);`

- `strcpy(ptr2, ptr1)` is equivalent to `while(\*ptr2++ = *ptr1++)`  
	**So if you want the string which you have copied to be used in another function (as it is created in heap section) you can use `strdup`, else strcpy is enough.**
	The functions `strcpy` and `strncpy` are part of the **C standard library** and **operate on existing memory**.   

- `char *strdup(const char *s);`相当于 `ptr2 = malloc(strlen(ptr1)+1); strcpy(ptr2, ptr1);`
- `char *strndup(const char *s, size_t n);`  
	By constrast, `strdup` is a Posix function, and it performs **dynamic memory allocation** for you. 
	It returns a pointer to **newly allocated memory** into which it has copied the string. But you are now responsible for this memory and **must eventually free it**.

- `char *strchr(const char *s, int c);` 第一次出现, 没有找到返回NULL
- `char *strrchr(const char *s, int c);`; 最后一次出现

- `int strcmp(const char *s1, const char *s2);`, strcasecmp, strncasecmp

- `char *strtok(char *s, const char *delim);` 将字符串分割成一个个片段. 参数str 指向欲分割的字符串, 参数delim 则为分割字符串.
	每次调用strtok()时, 匹配成功的字符会被置为`\0` 字符, 返回当前的字符串, 同时str会指向下一个片段.也就是说**源字符串会改变**.
	在第一次调用时, 必需给予参数str, 往后的调用则将参数str 设置成NULL.
	[ex](http://c.biancheng.net/cpp/html/175.html)

#### stdlib.h
- `int atoi(const char *nptr)`
- `long int atol(const char *nptr)`, atoll
- `long int strtol(const char *nptr, char **endptr, int base);`, endptr一般设为NULL, strtoul, strtoll, strtoull 类似
- `double atof(const char *nptr)`

#### ctypes.h
- `int isprint(int c)`: 判断是否为可打印字符
- `int isspace(int c)`: 判断是否为' ', '\t', '\r', '\n', '\v', '\f'等
- `int isalpha(int c)`
- `int isdigit(int c)`
- `int toupper(int c), tolower, islower, isupper`

scan in values from a string using sscanf
```C++
string str("2.5,6.7");
double val1, val2;
if(sscanf(str.c_str(),"%lf,%lf", &val1, &val2) == 2){
    // got them!
}
```

we can initialize the array of char elements called myword with a null-terminated sequence of characters
by either one of these two methods:

1. `char myword [] = { 'H', 'e', 'l', 'l', 'o', '\0' };`
1. `char myword [] = "Hello";`

In both cases the array of characters myword is declared with a size of 6 elements of type char: 5 characters that compose the word "Hello" plus a final null character ('\0').

```C
char b = 'a';
printf("%lu, %lu", sizeof('a'), sizeof(b)); // OUTPUT: 4,1
```
Note character literals are of type int in C; but of type char in C++.

# Memory
单个元素
```c++
pointer = new type
delete pointer;
```

数组
```c++
char *p = new char[number_of_elements];
delete []p;
```

`new (std:nothrow)`: 直接new如果失败要抛出异常的, 结果就是为了健壮性代码里到处都是try. 另如果new出现异常, 表示内存几经到达边缘了, 再用try+catch来消耗内存也是无济于事
所以一般健壮的分配方式都用new (nothrow) xxx的(当然也有用malloc等的),之后判断NULL就OK

`g_stmola = new (std::nothrow) select_mola[mola_num];`

`realloc`: 重新分配内存, 但是原来的内容保留下来

- 当realloc 执行失败时, 返回null, 但是原来的内容不会被更改, 所以不能将realloc的返回指针复制给原来的指针,否则会覆盖掉原来的内容,
	而且也不能通过判断原来的指针是否为null来判断realloc窒息你给成功与否
- 当执行成功时, realloc 会将原来的内存释放掉, 返回的指针可能和原来的不一样, 所以我们只需要负责释放realloc 返回的内存就可以了, 之前的内存不用管了(realloc会处理)

## 指针
### 指针数组与数组指针
- 指针作为元素的数组: `int *ptr_array[10]`, 每个元素都是指针, 共10个元素.
- 指向数组的指针(行数组指针): `int (*)array_ptr[10]`, 指向一个10元素一维数组的指针, 所以`array_ptr` 为一个一维数组

### [二维数组与指针](http://blog.csdn.net/yangchang999/article/details/6664069)
[C++二维数组new小结(zz)](http://www.cnblogs.com/beyondstorm/archive/2008/08/26/1276278.html)

二维数组其实就是一个一维数组, 在内存中没有二维的概念.
如果把二维数组的每一行看成一个整体, 即看成一个数组中的一个元素, 那么整个二维数组就是一个一维数组, 它以每一行作为它的元素.实际确实也是这样的.

对于一个一维数组`int arr[3]`.
arr 是一个一维数组对象, `sizeof(arr)`得到 12(一共3个元素)
第i 个元素的表示方法(v 表示用值表示, p 表示用指针表示)

1. v => arr[i]
1. p => arr + i

对于一个二维数组`int arr[2][3]`.
arr 是一个二维数组对象, `sizeof(arr)` 得到24(一共6个元素).

第i行j列个元素的表示方法, value 或者point

1. `v => arr[i][j]`
1. `p => arr[0] + i * 3 + j`: arr[0] 是一维数组的名字, 类似于一个`*int`, 所以arr[0] + i 是以int 为单位加的
1. `p => *(arr + i) + j`: arr 是二维数组的名字, 所以arr的元素是一维数组(3个int组成), arr + i 是以行为单位来偏移, 但是arr + i 还是一个指向一维数组的指针

数组arr 包含2个元素, 分别为`arr[0], arr[1]`, 每个元素为包含3 个int 的一维数组.
如`arr[0]`的3个元素为 `arr[0][0], arr[0][1], arr[0][2]`

二维数组作为函数参数

- `void print(int **arr, int rows)`
- `void print(int arr[][3], int rows)`, 在函数里面, arr 是一个指向3元素一维数组的指针(也就是arr 是一个行数组指针), `*arr` 得到这个一维数组.
- `void print((void *)arr, int rows, int cols)`: 自己进行位移

[Pointers to functions](../demo/cxx/pointer_function.cpp)

`std::copy(val.begin(), val.end(), std::ostream_iterator<int>(std::cout, ", "))`: describes types that can be used to identify and traverse the elements of a container.
	Iterator is the base concept used by other iterator types: InputIterator, OutputIterator, ForwardIterator,BidirectionalIterator, and RandomAccessIterator.
	Iterators can be thought of as an abstraction of pointers.

- An OutputIterator is an Iterator that can write to the pointed-to element.
- An InputIterator is an Iterator that can read from the pointed-to element.
	InputIterators only guarantee validity for single pass algorithms:
	once an InputIterator i has been incremented, all copies of its previous value may be invalidated.
- A ForwardIterator is an Iterator that can read data from the pointed-to element.
	Unlike an InputIterator, it guarantees validity when used in multipass algorithms.
- A BidirectionalIterator is a ForwardIterator that can be moved in both directions (i.e. incremented and decremented).
- A RandomAccessIterator is a BidirectionalIterator that can be moved to point to any element in constant time.
	A standard pointer is an example of a type that satisfies this concept.

# other
c++ 中不要使用 goto, 在goto 之后是不允许定义的新的变量的, 局部变量也不行.
[crosses initialization error](http://stackoverflow.com/questions/14274225/statement-goto-can-not-cross-pointer-definition)

# Time
```C
#include <time.h>
time_t time(time_t *t);
```
returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.
If t is non-NULL, the return value is also stored in the memory pointed to by t.

```C
#include <time.h>
#ifndef _TM_DEFINED
struct tm {
	int tm_sec; /* 秒–取值区间为[0,59] */
	int tm_min; /* 分 - 取值区间为[0,59] */
	int tm_hour; /* 时 - 取值区间为[0,23] */
	int tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */
	int tm_mon; /* 月份(从一月开始,0代表一月) - 取值区间为[0,11] */
	int tm_year; /* 年份,其值从1900开始 */
	int tm_wday; /* 星期–取值区间为[0,6],其中0代表星期天,1代表星期一,以此类推 */
	int tm_yday; /* 从每年的1月1日开始的天数–取值区间为[0,365],其中0代表1月1日,1代表1月2日,以此类推 */
	int tm_isdst; /* 夏令时标识符,实行夏令时的时候,tm_isdst为正.不实行夏令时的进候,tm_isdst为0,不了解情况时,tm_isdst()为负.*/
	long int tm_gmtoff; /*指定了日期变更线东面时区中UTC东部时区正秒数或UTC西部时区的负秒数*/
	const char *tm_zone; /*当前时区的名字(与环境变量TZ有关)*/
};
#define _TM_DEFINED
#endif
```
gmtime()和localtime()可以将time()获得的日历时间time_t结构体转换成tm结构体.  
其中gmtime()函数是将日历时间转化为世界标准时间(即格林尼治时间0,并返回一个tm结构体来保存这个时间,
而localtime()函数是将日历时间转化为本地时间.

[conversion demo](../demo/cxx/time.c) 里面还有将时间转化为mysql 的格式的

The struct timeval structure represents an elapsed time. 
```C
#include <sys/time.h>
struct timeval{
	long int tv_sec; // the number of whole seconds of elapsed time since the Epoch((00:00:00 UTC, January 1, 1970)
	long int tv_usec; // microseconds 微秒, 百万分之一秒
}

int gettimeofday(struct timeval *tv, struct timezone *tz);
```

`time_t` just stores seconds, so  
`time_t time = (time_t)ut_tv.tv_sec;`  
Should work, but since you are just looking for a difference, there is always the magic of subtraction.


# 关键字
## const
const in C does not mean something is constant. It just means a variable is read-only.

1. 为了防止传递的函数参数不被修改,在调用函数的形参中用const关键字.
2. `const int *p`: p是指向int常量的指针, `*p`是不变的, 但是可以将p指向其他变量
3. `int * const p`: p是指针常量, 也就是p不能变, 但是可以改变 `*p`
4. `const int * const p`: 同时满足前面两种情况
5. const并不会阻止参数的修改, 防君子不防小人, 可以强制的把 `const char *` 转换为 `char *`类型, 然后就可以修改了

在 C++(但不是在 C 语言)中，const 限定符对默认存储类型稍有影响.
在默认情况下,全局变量的链接性为外部的,但 const 全局变量的链接性为内部的.也就是说,在 C++ 看来,全局 const 定义就像使用了 static 说明符一样.
因此,可以将 const 常量定义在头文件中供工程中的多个其它文件包含引用,并且编译时不会产生变量重复定义的错误.当然,也可以用 #define 宏定义.

ref: [头文件中定义const全局变量应注意的问题](http://blog.csdn.net/ace_fei/article/details/8587403)

## extern
- extern可以置于变量或者函数前,以标示变量或者函数的定义在别的文件中,提示编译器遇到此变量和函数时在其他模块中寻找其定义
- 当它与"C"一起连用时, 如: `extern "C" void fun(int a, int b)`; 则告诉编译器按照C的规则去翻译相应的函数名而不是C++的, C++在翻译这个函数名时会把fun这个名字变得面目全非
- 在一个cpp文件定义了一个数组:`char a[6]`, h文件声明为`extern char a[]`, 而不是`extern char *a`, 两者的类型不一样

## static
- static 表示静态的变量, 分配内存的时候, 存储在静态区, 不存储在栈上面
- static 修饰的全局变量的作用域只能是本身的编译单元, 也就是所在的cpp文件
- 一般定义static全局变量时, 把它放在cpp文件而不是h文件中(理论上也可以放), 这样就不会给其他模块造成不必要的信息污染

## define  
- `#str`: 生成`"str"`
- `std::c##out << "str"`: 相当于`std::cout << "str"`
- `\`是用来续行的  
- `#@`给参数加上单引号.

## 高级
- volatile 指示变量随时可能发生变化的, 每次使用时都需要去内存里重新读取它的值, 与该变量有关的运算, 不要进行编译优化
- `sig_atomic_t`: 保证该变量的操作都是原子的

- `dynamic_cast` can be used only with pointers and references to objects. used to cast a class to one of its base classes
- `static_cast` not only from the derived class to its base, but also from a base class to its derived(可能runtime error).
- typeid: allows to check the type of an expression, returns a reference to a constant object of type `type_info` from `<typeinfo>`.  string 和 string & 的typeid 是一样的

# [C++的头文件和实现文件分别写什么](http://www.cnblogs.com/ider/archive/2011/06/30/what_is_in_cpp_header_and_implementation_file.html)

![declaration and implementation](http://i.imgbox.com/A7NmGCr8.png)

# Linux
[Example of Parsing Arguments with getopt](http://www.gnu.org/software/libc/manual/html_node/Getopt.html)
optstring中后面的`:`表示需要接值, 如果`::`, 则表示值可有可无.

- `optarg` 存数据
- `optopt` 存储出错的option(如缺参数),或者不认识的option
- `optind`, 表示下一次运行getopt时将读取数组第optind个

[parse options demo](../demo/cxx/parse_options.c)

# GCC
高版本的gcc glibc 编译后在低版本的glibc上运行导致,可能导致Floating Point Exception运行时错误.
这是由于高版本gcc在link的时候默认会采用选项--hash-style=gnu,而使用这种Hash表的方式可以大大提升动态链结时的效率.
而老版本的glibc本并未支持,我们只要在在程序连接选项中加上-Wl,--hash-style=sysv就可以解决这个问题了

