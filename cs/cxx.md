# IO
- stdin/stdout 属于标准库处理的输入流, 其声明为 FILE 型的, 对应的函数前面都有f开头, 如fopen/fread/fwrite/fclose 标准库调用等;
- `int fileno(FILE *fp)` 得到fp 对应的file descriptor, fd 对应的为系统API接口库, 函数主要包括 open/read/write/close 等系统级调用. stdin, stdout, stderr 还可以通过`STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO` 得到
- `FILE *fdopen(int fd, const char *mode)`: 从file descriptor 得到文件指针, 失败返回NULL, 错误代码在宏errno中

- `FILE *fopen(const char *path, const char *mode);` 同一个文件可以用fopen同时打开多次, 读取是独立的, 各个FILE指针是不相互干扰的.
- `int fclose(FILE *stream);`

- `char *fgets(char *s, int n, FILE *stream);`: 最多读 n - 1 个字符, `\n`也会被存储起来, s[n-1]存储`\0`作为字符串的结尾, 发生错误或者没有内容可读, 返回NULL.
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

#include <limits>
std::numeric_limits<int>::max()
std::numeric_limits<int>::min()

#include <limits.h>
INT_MAX, INT_MIN
```

# STL 标准模板库
- [iterator](http://www.cplusplus.com/reference/iterator): 5类, 分别为 Input, Output, Forward, Bidirectional, Random Access

- [std::to_string](http://www.cplusplus.com/reference/string/to_string): 对于数字型可以使用
- [std::begin](http://www.cplusplus.com/reference/iterator/begin), and [std::end](http://www.cplusplus.com/reference/iterator/end): for Container, returns `cont.end()`, for Array, returns `arr + N`.
- [std::find](http://www.cplusplus.com/reference/algorithm/find)

	```C++
	#include <algorithm>
	std::vector<int> v{0, 1, 2, 3, 4};
	auto pos = std::find(std::begin(v), std::end(v), 3);
	assert(pos != std::end(v));
	```
- [`std::lower_bound(first, last, value)`](http://www.cplusplus.com/reference/algorithm/lower_bound/) 第一个 >= value 的iterator, 如果没有符合的元素, 返回last
- `std::upper_bound(first, last, value)` 第一个 > value 的iterator, 如果没有符合的元素, 返回last
- [std::for_each](http://www.cplusplus.com/reference/algorithm/for_each): 避免显示的for 循环.
	不能使用auto 来推断x 的类型, 必须显式指定

	```C++
	#include <algorithm>
	std::for_each(std::begin(vec), std::end(vec), [](int &x){x++;}); // 修改vec 里面的元素, 因此需要用ref
	```

## containers
- [std::vector](http://www.cplusplus.com/reference/vector/vector): vector封装了数组, vector使用连续内存存储的, 支持[]运算符
	- erase: 删除元素, 同时返回下一个元素的iterator, 如果删除最后一个元素,则返回vec.end().

	```C++
	int array[] = {-1, 1, 2, 7, 11, 15};
	std::vector<int> vec(array, array + sizeof(array) / sizeof(int));

	std::vector<int> vec{3, 4, 2, 8, 15, 267};
	```
	- [vector push back operation demo](../demo/cxx/stl/vector-push_back.cpp)

- [std::list](http://www.cplusplus.com/reference/list/list): implemented as doubly-linked lists
- [std::unordered map](http://www.cplusplus.com/reference/unordered_map/unordered_map): unordered map 内部实现了一个哈希表, 因此其元素的排列顺序是杂乱的, 无序的. [自定义key 和value](../demo/cxx/stl/unordered_map_customized_key.cpp)

	```C++
	// 将整个container 作为一个整体:
	for(auto it = mymap.begin(); it != mymap.end(); ++it){
		std::cout << " " << it->first << ":" << it->second << std::endl;
	}

	// 分bucket单独遍历:
	std::cout << "mymap's buckets contain:\n";
	for(unsigned i = 0;i < mymap.bucket_count(); ++i){
		for(auto local_it = mymap.begin(i); local_it!= mymap.end(i); ++local_it ){
			std::cout << " " << local_it->first << ":" << local_it->second << std::endl;
		}
	}
	```

- [std::map](http://www.cplusplus.com/reference/map/map/): map内部实现了一个红黑树,该结构具有自动排序的功能,因此map内部的所有元素都是有序的.

	```C++
	struct Server {
		std::string ip;
		int port;
	};

	// 对于自定义的Compare, 需要满足strict weak ordering, 只有当left < right 时返回true
	// The map object uses this expression to determine both the order the elements follow in the container and whether two element keys are equivalent (by comparing them reflexively: they are equivalent if !comp(a, b) && !comp(b, a)).
	struct ServerCompare {
		bool operator()(const Server* left, const Server* right) const {
			if (left != right) {
				return left->ip < right->ip < 0;
			}
			return false;
		}
	};

	std::map<const Server*, int, ServerCompare> map;
	```

- [std::stack](http://www.cplusplus.com/reference/stack/stack): 栈
- [std::queue](http://www.cplusplus.com/reference/queue/queue): 队列
	- pop: This calls the removed element's destructor
- [std::deque](http://www.cplusplus.com/reference/deque/deque): 双端队列
	- [map + vector 实现的](https://stackoverflow.com/questions/6292332/what-really-is-a-deque-in-stl), 将很多个固定大小的vector 连接到一起, 也就是内存是不连续的
	- 支持random access operator[]
- [std::priority queue](http://www.cplusplus.com/reference/queue/priority_queue): 优先队列
	- `std::priority_queue<int, std::vector<int>, std::less<int>> pq_max`: top() 为最大值的优先级队列
	- `std::priority_queue<int, std::vector<int>, std::greater<int>> pq_min`: top() 为最小值的优先级队列
- [unordered set](http://www.cplusplus.com/reference/unordered_set): hash 表实现的
	- the elements in an unordered set cannot be modified once in the container, they can be inserted and removed, though.
- [set](http://www.cplusplus.com/reference/set/set): 红黑树实现的有序集合, 内部不允许重复元素
- [multiset](http://www.cplusplus.com/reference/set/multiset): 红黑树实现的有序集合, 默认是增序, 元素可以重复

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
- `size_t size()` or `length()`: length of string
- `capacity()`: storage space currently allocated for the string
- `const char *c_str()`: 返回一个分配给`const char*`的地址,其内容已设定为**不可变更**
- `size_t find()`: 第一个参数可以为 `string, char, const char*`类型, 若没有找到, 返回std::string::npos; 第二个参数pos(默认为0)表示从string 的pos位置开始查找
- `string substr(size_t pos = 0, size_t len = npos) const`: 新构造一个string 对象, len 是可以为0的, 表示std::string(""), 默认到string的结尾, 尽力而为, 如果len 超过, 则只到结尾
- `void clear()`: 清空

### [字符数组与字符指针](http://blog.csdn.net/qiumm/article/details/5657120)
- `const char *str1 = "abc";` 字符指针指向的是一个**字符串常量**(存储在程序的常量区)的首地址.
- `char str2[] = "abc";`: str2是字符数组,它存放了一个字符串, 编译会自动在str2 末尾添加`\0`.  

# std::atomic
x 为 `std::atomic<int>`

- `x.load()`: 返回x的值.
- `x.store(n)`: 把x设为n, 什么都不返回.
- `x.fetch_add(n)`, `x.fetch_sub(n)`, `x.fetch_xxx(n)`: x += n, x-= n(或更多指令), 返回修改之前的值.
- `x.exchange(n)`: 把x设为n, 返回设定之前的值.

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

```C
char b = 'a';
printf("%lu,%lu", sizeof('a'), sizeof(b)); // OUTPUT in C: 4,1, in C++: 1,1
```
Note character literals are of type int in C; but of type char in C++.

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

# Memory
new delete malloc free 都是thread safe

单个元素
```c++
int *p = new int;
delete p;
```

数组
```c++
int *p = new int[num];
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

## [柔性数组](https://www.cnblogs.com/pluviophile/p/7571410.html)
```C++
struct node_t{
	int a;
	char c[]; // 一个符号地址, 不占用空间, sizeof(node_t) 为 4
};

const char *str = "Hello World";
node_t *node = (node_t *)new char[sizeof(node_t) + strlen(str) + 1];
strcpy(node->c, str);
delete node;
```

# Process 进程
- `pid_t getpid(void)`
- `pid_t waitpid(pid_t pid, int *status, int options)`
	- pid: 可以为特定的进程pid, 也可以为-1 表示任何子进程
	- status: 不同的位保存着不同的信息, 有一些特定的macro 来对返回的status 进行操作, WIFEXITED, WEXITSTATUS, WIFSIGNALED, WTERMSIG
	- options: 0 会阻塞; WNOHANG 若pid 进程没有结束, 立刻返回0
- `void exit(int status)`与`void _exit(int status)` [最大区别](http://blog.csdn.net/lwj103862095/article/details/8640037)就在于
	exit()函数在调用exit系统之前要检查文件的打开情况, 把文件缓冲区的内容写回文件.

# thread
一般情况下,线程终止后,其终止状态一直保留到其它线程调用`pthread_join`获取它的状态为止.  
但是线程也可以被置为`detach`状态,这样的线程一旦终止就**立刻回收它占用的所有资源**,而不保留终止状态.  
不能对一个已经处于`detach`状态的线程调用`pthread_join`,这样的调用将返回`EINVAL`

## 线程特定数据
也被称为线程私有数据,是一种存储和查找一个特定线程相关数据的机制.每个线程访问它自己独立的数据,而不用担心和其它线程的访问的同步.

线程特定数据看似很复杂,其实我们可以把它理解为就是一个索引和指针.key结构中存储的是索引,pthread结构中存储的是指针,指向线程中的私有数据,通常是malloc函数返回的指针.

## 互斥锁 Mutual Exclusive Lock
用于同步的手段很多, 其中以互斥锁应用最为广泛(信号量是一种将资源数目从1泛化到n的互斥锁).
当线程进入临界区前获得锁,只有获得了锁的线程才可能继续执行,当退出临界区后归还锁.如果锁被占用,则线程进入阻塞状态.

而`pthread_mutex_t`实现基于Linux的futex, 当临界区足够小时, 一次`pthread_mutex_lock`消耗很非常小

通过lock guard, 进入critical region 时, 自动加锁, 出的时候自动归还锁.
```C++
std::mutex mutex;
{
	std::lock_guard<std::mutex> guard(mutex);
	do xxx
}
```

### 读写锁(rwlock)
[c++ 读写锁](https://www.cnblogs.com/i80386/p/4478021.html)

pthread读写锁把对共享资源的访问分为读者和写者,读者只对共享资源进行读访问,写者只对共享资源进行写操作.
在互斥机制,读者和写者都需要独立独占互斥量以独占共享资源, 在读写锁机制下,允许同时有多个读者访问共享资源,只有写者才需要独占资源.
相比互斥机制,读写机制由于允许多个读者同时访问共享资源,进一步提高了多线程的并发度.

读写锁机制

- 写者: 写者使用写锁,如果当前没有读者,也没有其他写者,写者立即获得写锁, 否则写者将等待,直到没有读者和写者.
- 读者: 读者使用读锁,如果当前没有写者,读者立即获得读锁,否则读者等待,直到没有写者.

读写锁特性:

- 同一时刻只有一个线程可以获得写锁,同一时刻可以有多个线程获得写锁.
- 读写锁出于写锁状态时,所有试图对读写锁加锁的线程,不管是读者试图加读锁,还是写者试图加写锁,都会被阻塞.
- 读写锁处于读锁状态时,有写者试图加写锁时,之后的其他线程的读锁请求会被阻塞,以避免写者长时间的获取不到写锁.

API: `pthread.h` 中 `pthread_rwlock_*` 函数

## 条件变量 condition variable
条件变量也是同步的一种手段,由一把锁(mutex)和一个condition组成.
它可以使线程阻塞在某一条件上,比如`queue.not_empty()`.当条件满足时,线程唤醒.需要注意是要小心虚假唤醒,即当wait返回后,需要再次判断条件是否满足.

使用`pthread_cond_wait`方式如下:
```
pthread _mutex_lock(&mutex)
while或if(线程执行的条件是否成立)
	pthread_cond_wait(&cond, &mutex);
线程执行
pthread_mutex_unlock(&mutex);
```

`pthread_cond_wait` 内部包含以下几步:

1. 线程放在等待队列上,解锁
2. 等待 `pthread_cond_signal` 或者 `pthread_cond_broadcast` 信号之后去竞争锁
3. 若竞争到互斥索则加锁

下面来讲一下:`pthread_cond_wait`和`pthread_cond_singal`是怎样配对使用的:

- 等待线程:
	1. `pthread_cond_wait` 前要先加锁
	1. `pthread_cond_wait` 内部会解锁, 然后等待条件变量被其它线程激活
	1. `pthread_cond_wait` 被激活后会再自动加锁
- 激活线程:
	1. 加锁(和等待线程用同一个锁)
	1. `pthread_cond_signal`发送信号(阶跃信号前最好判断有无等待线程)
	1. 解锁

## 高级
- `dynamic_cast` can be used only with pointers and references to objects. used to cast a class to one of its base classes
- `static_cast` not only from the derived class to its base, but also from a base class to its derived(可能runtime error).
- typeid: allows to check the type of an expression, returns a reference to a constant object of type `type_info` from `<typeinfo>`.  string 和 string & 的typeid 是一样的


