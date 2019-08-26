# Accustoming yourself to C++
今天的C++ 已经是个 multiparadigm programming language, 一个同时支持过程形式(procedual), 面向对象形式(object-oriented), 函数形式(functional), 泛型形式(generic), 元编程形式(metaprogramming).

对内置(也就是C-like) 类型而言, pass-by-value 通常比pass-by-reference 高效;
但当从C part of C++ 移往object-oriented C++, 由于用户自定义构造函数和析构函数的存在, pass-by-reference 往往更好.

## prefer const, enum, and inlines to `#define`s
```C++
class GamePlayer {
 private:
  static const int NumTurns = 5;  // 常量声明式
  int socres_[NumTurns];
};
```
然后你所看到的是NumTurns 的声明式而非定义式. 通常C++ 要求你对你所使用的任何东西提供一个定义式, 但如果它是class专属static 常量且为整型(integral type, 例如int, char, bool), 则需特殊处理.
只要不取它们的地址, 你可以声明并使用他们, 而无须提供定义式.
但如果你取某个class专属常量的地址, 或纵使你不取其地址而编译器坚持要看到一个定义式, 你就必须另外提供定义式如下:
```C++
const int GamePlayer::NumTurns;  // NumTurns 的定义
```
定义式需要放到一个实现文件而非头文件. 由于class 常量已在声明时获得初值, 因此定义时不可以再设初值.

## use const whenever possible
```C++
class TextBlock {
 public:
 // operator[] for const object
  const char& operator[](std::size_t position) const {
    return text_[position];
  }
 // operator[] for non-const object
  char& operator[](std::size_t position) {
    return text_[position];
  }
 private:
  std::string text_;
};
```
如果在return 之前有很多复杂的操作, 比如boundary checking, log access data, verify data integrity, 对于const 和 non-const 这些操作都是一样的, 为了避免代码重复, 除了可以把这些部分抽象出单独的函数外
```C++
class TextBlock {
 public:
 // operator[] for const object
  const char& operator[](std::size_t position) const {
    ...
    return text_[position];
  }
 // operator[] for non-const object
  char& operator[](std::size_t position) {
    return const_cast<char&>(static_cast<const TextBlock&>(*this)[position]);
  }
 private:
  std::string text_;
};
```

## Minimize casting
- `const_cast`: 通常被用来cast away the constness. 它也是唯一有此能力的 C++-style 转型操作符
- `dynamic_cast`: 主要用来执行safe downcasting, 也是唯一可能耗费重大运行成本的转型动作(一个很普遍的实现版本基于class 名称至字符串比较).
- `reinterpret_cast`: 意图执行低级转型
- `static_cast`: 用来强迫隐式转换, 例如将 non-const 对象转换为const 对象, 或将int 转换为 double.
  它也可以用来执行上述多种转换的反向转换, 例如将pointer-to-based 转为 pointer-to-derived, 但它无法将 const 转为non-const, 这个只有`const_cast` 才办得到.

## make sure that objects are initialized before they're used
函数内的 static 对象称为local static 对象, 其他static 对象称为non-local static 对象.

如果某编译单元内的某个non-local static 对象的初始化使用了另外一个编译单元的某个non-local static 对象, 它所用到的这个对象可能尚未被初始化,
因为C++ 对定义于不同编译单元内的 non-local static 对象的初始化次序并无明确定义.

幸运的是一个小小的设计便可以完全消除这个问题. 唯一需要做的就是将每个non-local static 对象搬到自己的专属函数内.
这种手法的基础在于: C++ 保证, 函数内的local static 对象会在 该函数被调用期间, 首次遇上该对象之定义式时被初始化.
所以如果你以函数调用替换直接访问non-local static 对象, 你就获得了保证.
```C++
class FileSystem {};
FileSystem& tfs() {
  static FileSystem fs;
  return fs;
}
```
任何一种 non-const static 对象, 不论它是local 还是non-local, 在多线程环境下等待某事发生都会有麻烦.
处理这个麻烦的手法一种做法是: 在程序的单线程启动阶段手工调用所有reference-returning 函数, 以消除与初始化有关的race conditions

