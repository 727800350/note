# STL 标准模板库
## [std::vector](http://www.cplusplus.com/reference/vector/vector/)
vector封装了数组, vector使用连续内存存储的,支持[]运算符; 对于新添加的元素,从尾部追加

Do not use pointers to std::vector, 也就是说直接使用std::vector这个对象,而不要使用这个对应的指针.
This class already manage memory allocation and deallocation. 在超出变量的scope, 会自动调用它的析构函数来释放内存

- `push_back`: 将要`push_back`的元素拷贝到新分配的内存中, 如果元素是指针, 那么只拷贝指针本身, 而不会拷贝指针所指向的实际内容.
	对于string 等object, 即使push_back中传入的参数是reference(别名) 类型, push到vector中的是一个完整的拷贝, 而不是一直指向原来的object 的指针,
	所以即使原来的object被删除了, vector中的仍然可以正常访问.
- erase: 删除元素, 如果元素是指向某个对象的指针, 元素本身在该vector种会被删除, 但是指针所指向的对象不会被删除
- back: Returns a reference to the last element in the vector.
- empty: 用来检测容器是否为空的.
- clear: 可以清空所有元素, 也就是改变vector 的size, 但是capacity 一般不会改变.
- swap: vector<T>().swap(x); 改变x 占用内存的方式, 实际上是把x 的内存交换给一个新的vector<T>()对象

[vector push_back operation demo](../demo/cpp/stl/vector-push_back.cpp)

## [std::list](http://www.cplusplus.com/reference/list/list)
implemented as doubly-linked lists

- `push_front`: 在头插入
- `push_back`: 在尾插入

## [std::unordered_map](http://www.cplusplus.com/reference/unordered_map/unordered_map/)
Internally, the elements are not sorted in any particular order, but organized into buckets depending on their hash values
to allow for fast access to individual elements directly by their key values (with a constant average time complexity on average).

unordered_map containers are faster than map containers to access individual elements by their key,
although they are generally less efficient for range iteration through a subset of their elements.

尽量不要使用`char *` 作为key, 而要使用string.
`char *`是一个地址, 也就是一个unsigned int, 所以实际上的key 是一个unsigned int 类型的值

- `std::unordered_map::operator[]`: If k matches the key of an element in the container, the function returns a reference to its mapped value.
	If k does not match the key of any element in the container, the function inserts a new element with that key(值为mapped value 的类型的默认值) and
	returns a reference to its mapped value. 反应在map.size() 上
- `std::unordered_map::find`: Searches the container for an element with k as key and returns an iterator to it if found,
	otherwise it returns an iterator to unordered_map::end (the element past the end of the container).
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

# 参数传递
Things are passed by value unless you specify otherwise using the &-operator
(note that this operator is also used as the 'address-of' operator, but in a different context).
```C++
void foo(vector<int> bar); // by value
void foo(vector<int> &bar); // by reference (non-const, so modifyable inside foo
void foo(vector<int> const &bar); // by const-reference
```
You can also choose to pass a pointer to a vector void `foo(vector<int> *bar)`,
but unless you know what you know exactly what are doing, do not do this.

# [std::string](http://www.cplusplus.com/reference/string/string/)
- `string (const char* s)`
- `string (const char* s, size_t n)`: 当用字符数组表示的二进制串中可能含有 0 字符时, 需要明确二机制串的长度
- `size()` or `length()`: Return length of string, `capacity()` 返回的是storage space currently allocated for the string
- `c_str()`: 返回一个分配给`const char*`的地址,其内容已设定为**不可变更**
- `find()`: 参数可以为 `string, char, const char*`类型, 若没有找到, 返回std::string::npos

scan in values from a string

使用C语言的方式, sscanf
```
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

In both cases the array of characters myword is declared with a size of 6 elements of type char: the 5 characters
that compose the word "Hello" plus a final null character ('\0') which specifies the end of the sequence and that,
in the second case, when using double quotes, '\0' is appended automatically.

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

`new (std:nothrow)`:
对于比较健壮的代码来说,nothrow使用的情况比直接new多.
直接new如果失败要抛出异常的, 结果就是为了健壮性代码里到处都是try. 另外new出现异常, 表示内存几经到达边缘了, 再用try+catch来消耗内存也是无济于事
所以一般健壮的分配方式都用new (nothrow) xxx的(当然也有用malloc等的),之后判断NULL就OK

demo: `g_stmola = new (std::nothrow) select_mola[mola_num];`

[Pointers to functions](../demo/c++/pointer_function.cpp)

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

## 二维数组
[C++二维数组new小结(zz)](http://www.cnblogs.com/beyondstorm/archive/2008/08/26/1276278.html)

# other
c++ 中不要使用 goto, 在goto 之后是不允许定义的新的变量的, 局部变量也不行.
[crosses initialization error](http://stackoverflow.com/questions/14274225/statement-goto-can-not-cross-pointer-definition)

# 关键字
- `dynamic_cast` can be used only with pointers and references to objects. used to cast a class to one of its base classes
- `static_cast` not only from the derived class to its base, but also from a base class to its derived(可能runtime error).
- typeid: allows to check the type of an expression, returns a reference to a constant object of type `type_info` from `<typeinfo>`.  string 和 string & 的typeid 是一样的

# [C++的头文件和实现文件分别写什么](http://www.cnblogs.com/ider/archive/2011/06/30/what_is_in_cpp_header_and_implementation_file.html)

![declaration and implementation](http://i.imgbox.com/A7NmGCr8.png)

**inline限定符**

在头文件中,可以对函数用inline限定符来告知编译器,这段函数非常的简单,可以直接嵌入到调用定义之处.

当然inline的函数并不一定会被编译器作为inline来实现,如果函数过于复杂,编译器也会拒绝inline.

因此简单说来,代码最好短到只有3-5行的才作为inline.有循环,分支,递归的函数都不要用做inline.

对于**在类定义内定义实现的函数,编译器自动当做有inline请求(也是不一定inline的)**.因此在下边,我把带有inline限定符的函数成员和写在类定义体内的函数成员统称为"要inline的函数成员"

对于要inline的函数成员其定义放在.h文件,其他函数的实现都放在.cpp文件中.

