# IO
- stdin/stdout 属于标准库处理的输入流, 其声明为 FILE 型的, 对应的函数前面都有f开头, 如fopen/fread/fwrite/fclose 标准库调用等;
- `int fileno(FILE *fp)` 得到fp 对应的file descriptor, fd 对应的为系统API接口库, 函数主要包括 open/read/write/close 等系统级调用. stdin, stdout, stderr 还可以通过`STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO` 得到

- `FILE *fopen(const char *path, const char *mode);` 同一个文件可以用fopen同时打开多次, 读取是独立的, 各个FILE指针是不相互干扰的.
- `int fclose(FILE *stream);`

- `char *fgets(char *s, int n, FILE *stream);`: 最多读 n - 1 个字符, `\n`也会被存储起来, s[n-1]存储`\0`作为字符串的结尾, 发生错误或者没有内容可读, 返回NULL
	如若该行(包括最后一个换行符)的字符数超过n - 1, 则fgets返回一个不完整的行,但是,缓冲区总是以NULL字符结尾,对fgets的下一次调用会继续读该行
- `size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);`
- `size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);`
- `int snprintf(char *str, size_t size, const char *format, ...);`: write at most size bytes, including the trailing '\0'('\0' 是自动加的)

- `void setbuf(FILE *steam, char *buf);` 将buf设置为stream 的缓冲区. 为了关闭缓冲,可以将buf参数设置为NULL.
- `void setlinebuf(FILE *stream);`
- `fflush(FILE *fp)`: 是把C库中的缓冲调用write函数写到磁盘[其实是写到内核的缓冲区]
- `fsync(FILE *fp)`: 是把内核缓冲刷到磁盘上

printf是一个行缓冲函数, 先写到缓冲区(默认一般为 1024 bytes), 满足条件后, 才将缓冲区刷到对应文件中, 刷缓冲区的条件如下:

1. 缓冲区填满
2. 写入的字符中有\n 或者 \r
3. 调用fflush手动刷新缓冲区. 当我们执行printf的进程或者线程结束的时候会主动调用flush来刷新缓冲区
4. 调用scanf要从缓冲区中读取数据时,也会将缓冲区内的数据刷新

满足上面4个条件之一缓冲区就会刷新(真正调用write来写入)

注: fork(返回值为0的是子进程) 的时候, 会将缓冲区也一起拷贝到子进程

fwrite 是线程安全的.
If you're using a single FILE object to perform output on an open file, then whole **fwrite calls on that FILE will be atomic**, i.e. lock is held on the FILE for the duration of the fprintf call.
Since a FILE is local to a single process's address space, this setup is only possible in multi-threaded applications;
it does not apply to multi-process setups where several different processes are accessing separate FILE objects referring to the same underlying open file.
Even though you're using fprintf here, each process has its own FILE it can lock and unlock without the others seeing the changes, so writes can end up interleaved.

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
- `size_t size()` or `length()`: Return length of string, `capacity()` 返回的是storage space currently allocated for the string
- `const char *c_str()`: 返回一个分配给`const char*`的地址,其内容已设定为**不可变更**
- `size_t find()`: 第一个参数可以为 `string, char, const char*`类型, 若没有找到, 返回std::string::npos; 第二个参数pos(默认为0)表示从string 的pos位置开始查找
- `string substr(size_t pos = 0, size_t len = npos) const`: 新构造一个string 对象, len 是可以为0的, 表示std::string(""), 默认到string的结尾
- `void clear()`: 清空

### [字符数组与字符指针](http://blog.csdn.net/qiumm/article/details/5657120)
- `const char *str1 = "abc";` 字符指针指向的是一个**字符串常量**(存储在程序的常量区)的首地址.
- `char str2[] = "abc";`: str2是字符数组,它存放了一个字符串, 编译会自动在str2 末尾添加`\0`.  
	str2是一个数组,可以改变数组中保存的内容(但是数组的名字str2本身, 它是一个常量, 也就是说str2 is not assignable)
	因为定义的是一个字符数组,所以就相当于定义了一些空间来存放"abc",而又因为字符数组就是把字符一个一个地存放的,
	所以编译器把这个语句解析为 `char str2[3] = {'a','b','c'};`, 然后补零, 所以最终结果是 `char str2[4] = {'a','b','c','\0'};`  
	如果这个语句是在函数内部写的话, 那么这里的`"abc\0"`, 因为不是常量, 所以应该被放在栈上.

## C API
### string.h
- `void *memset(void *s, int c, size_t n)`
- `void *memcpy(void *dest, const void *src, size_t n)`
- `void *memmove( void* dest, const void* src, size_t n)`: 即使src 和dst 有交叉, 也可以正确完整的copy过去, 但是运行比memcpy 慢
- `int memcmp(const void *s1, const void *s2, size_t n)`

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
new delete malloc free 都是thread safe

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

`realloc`: 重新分配内存, 但是原来的内容保留下来

- 当realloc 执行失败时, 返回null, 但是原来的内容不会被更改, 所以不能将realloc的返回指针复制给原来的指针,否则会覆盖掉原来的内容,
- 当执行成功时, realloc 会将原来的内存释放掉, 返回的指针可能和原来的不一样, 所以我们只需要负责释放realloc 返回的内存就可以了, 之前的内存不用管了(realloc会处理)

在C 和C++ 中, 结构体是可以直接赋值的. C 中是可以的, C++ 为了保持向后兼容, 因此也有了
```C
struct st{
    int a;
    char b[100];
    char *c;
}x, y;
// set elements in x
y = x;
assert(memcmp(&x, &y, sizeof(struct st)) == 0);
```

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

# Process 进程
- `pid_t getpid(void)`
- `pid_t waitpid(pid_t pid, int *status, int options)`
	- pid: 可以为特定的进程pid, 也可以为-1 表示任何子进程
	- status: 不同的位保存着不同的信息, 有一些特定的macro 来对返回的status 进行操作, WIFEXITED, WEXITSTATUS, WIFSIGNALED, WTERMSIG
	- options: 0 会阻塞; WNOHANG 若pid 进程没有结束, 立刻返回0
- `void exit(int status)`与`void _exit(int status)` [最大区别](http://blog.csdn.net/lwj103862095/article/details/8640037)就在于
	exit()函数在调用exit系统之前要检查文件的打开情况, 把文件缓冲区的内容写回文件.

# thread
`fork` 是昂贵的. fork要把父进程的内存镜像复制到子进程, 并在子进程中复制所有描述符, 如此, 等等.  
子进程获取父进程数据空间,堆和栈的副本, 包括缓冲区

同一进程内的所有线程共享

1. 相同的全局内存(也就是全局变量)
1. 打开的文件(即描述符)
1. 信号处理函数和信号处置
1. 进程指令
1. 大多数数据
1. 当前工作目录
1. 用户ID和组ID
1. All threads share a common heap. so access to the allocator/deallocator must be synchronized. 

不过每个线程拥有各自的

1. 线程ID
1. 寄存器集合, 包括程序计数器和栈指针
1. 栈(用于存放局部变量和返回地址)
1. errno. [ref](http://learn.akae.cn/media/ch35s02.html). 
	pthread库的函数都是通过返回值返回错误号,虽然每个线程也都有一个errno,但这是为了兼容其它函数接口而提供的,pthread库本身并不使用它. 
	所以errno 还是看成同一个进程的所有线程共享一个全局的errno.
1. 信号掩码
1. 优先级

一般情况下,线程终止后,其终止状态一直保留到其它线程调用`pthread_join`获取它的状态为止.  
但是线程也可以被置为`detach`状态,这样的线程一旦终止就**立刻回收它占用的所有资源**,而不保留终止状态.  
不能对一个已经处于`detach`状态的线程调用`pthread_join`,这样的调用将返回`EINVAL`

## 线程特定数据
也被称为线程私有数据,是一种存储和查找一个特定线程相关数据的机制.每个线程访问它自己独立的数据,而不用担心和其它线程的访问的同步.

线程特定数据看似很复杂,其实我们可以把它理解为就是一个索引和指针.key结构中存储的是索引,pthread结构中存储的是指针,指向线程中的私有数据,通常是malloc函数返回的指针.

## 互斥锁 Mutual Exclusive Lock
用于同步的手段很多, 其中以互斥锁应用最为广泛(信号量是一种将资源数目从1泛化到n的互斥锁).
当线程进入临界区前获得锁,只有获得了锁的线程才可能继续执行,当退出临界区后归还锁.如果锁被占用,则线程进入阻塞状态.

优先级翻转.
现代操作系统通过优先级继承较好的解决了这个问题, 但程序员需要注意自己代码所运行的平台是否有这个机制, 然后正确的设置线程属性方可.
此外,如果所有优先级都调到一个数量级,那么还需要注意lock convoy问题.发生lock convoy的场景犹如2人迎面通过一独木桥,2人相遇后均主动放弃退回,然后再次上桥相遇.

挂起等待和唤醒等待线程的操作如何实现?
每个Mutex有一个等待队列,一个线程要在Mutex上挂起等待,首先在把自己加入等待队列中,然后置线程状态为睡眠,然后调用调度器函数切换到别的线程.
一个线程要唤醒等待队列中的其它线程,只需从等待队列中取出一项,把它的状态从睡眠改为就绪,加入就绪队列,那么下次调度器函数执行时就有可能切换到被唤醒的线程.

而`pthread_mutex_t`实现基于Linux的futex, 当临界区足够小时, 一次`pthread_mutex_lock`消耗很非常小

## 条件变量 condition variable
条件变量也是同步的一种手段,由一把锁(mutex)和一个condition组成.
它可以使线程阻塞在某一条件上,比如`queue.not_empty()`.当条件满足时,线程唤醒.需要注意是要小心虚假唤醒,即当wait返回后,需要再次判断条件是否满足.

## pratice
对于本地文件的多线程

reader 线程扫描文件, 将各个kv 的offset 记录下来, 存储到 thread safe queue 中

多个 worker 线程从 queue 中取数据, 然后读取文件.
读取文件可以采用内存映射的方式, 方便进行偏移, 也可以用ftell 的方式.
这样避免了由于数据是在本地, 所以reader 线程很快, 就会把整个文件加载到内存中去.

对于writer
如果输出需要多个fwrite 操作时
取决于是加锁解锁快, 还是本地分配内存, copy, 释放快
但是一个进程下的线程是共享一个heap, 所以内存的申请和释放也是需要同步

如果writer 是单个的fwrite, 就不存在竞争的情况.

# 关键字
## goto
在goto 之后是不允许定义的新的变量的, 局部变量也不行.
[crosses initialization error](http://stackoverflow.com/questions/14274225/statement-goto-can-not-cross-pointer-definition)

## const
const in C does not mean something is constant. It just means a variable is read-only.

1. 为了防止传递的函数参数不被修改,在调用函数的形参中用const关键字.
2. `const int *p`: p是指向int常量的指针, `*p`是不变的, 但是可以将p指向其他变量
3. `int * const p`: p是指针常量, 也就是p不能变, 但是可以改变 `*p`
4. `const int * const p`: 同时满足前面两种情况
5. const并不会阻止参数的修改, 防君子不防小人, 可以强制的把 `const char *` 转换为 `char *`类型, 然后就可以修改了

在 C++(但不是在 C 语言)中,const 限定符对默认存储类型稍有影响.
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

# GCC
高版本的gcc glibc 编译后在低版本的glibc上运行导致,可能导致Floating Point Exception运行时错误.
这是由于高版本gcc在link的时候默认会采用选项--hash-style=gnu,而使用这种Hash表的方式可以大大提升动态链结时的效率.
而老版本的glibc本并未支持,我们只要在在程序连接选项中加上-Wl,--hash-style=sysv就可以解决这个问题了

