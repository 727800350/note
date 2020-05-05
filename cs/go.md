# intro
[Go by Example](https://gobyexample.com)

在 Go 中,首字母大写的名称是被导出的.
eg: `Foo` 和 `FOO` 都是被导出的名称.名称 `foo` 是不会被导出的.
在import 了 math 包之后, math 中的常量pi就就成为exported name, 但是需要使用首字母大写的形式来访问: `math.Pi`

# IO
## fmt
Formatted printing in Go uses a style similar to C's printf family but is richer and more general.

- fmt.Printf, fmt.Fprintf, fmt.Sprintf
- fmt.Println, fmt.Printf, fmt.Print

- `%v`: When printing a struct, the modified format `%+v` annotates the fields of the structure with their names, and for any value the alternate format `%#v` prints the value in full Go syntax.
- `%q`: That quoted string format is also available through %q when applied to a value of type string or []byte. The alternate format %#q will use backquotes instead if possible.
  (The %q format also applies to integers and runes, producing a single-quoted rune constant.)
- `%x`: works on strings, byte arrays and byte slices as well as on integers, generating a long hexadecimal string, and with a space in the format (`% x`) it puts spaces between the bytes.
- `%T`: prints the type(actually dynamic type) of a value.

when you pass a value to the fmt.Print function, it checks to see if it implements the fmt.Stringer interface
```go
type Stringer interface {
  String() string
}
```
Any type that implements a String() string method is a stringer, and the fmt package will use that method to format values of that type.

io 包指定了 io.Reader 接口, 它表示从数据流读取, 有一个 Read 方法:
`func (T) Read(b []byte) (n int, err error)`
Read 用数据填充指定的字节 slice,并且返回填充的字节数和错误信息. 在遇到数据流结尾时,返回 io.EOF 错误.

- Go 标准库包含了这个接口的许多实现, 包括文件, 网络连接, 压缩, 加密等

[read from a string](../demo/go/reader_string.go)

# variable
- A declaration names a program entity and specifies some or all of its properties. There are four major kinds of declarations: var, const, type and func.
- Package-level(even if none main pkg) variables are initialized before main begins.
- `s := ""` may be used only within a function, not for package level-variables.
- tuple assignment: all of the right-hand side expressions are evaluated before any of the variables are updated, making this form most useful when some of the variables appear on both sides of assignment, as happends, for example,
  when swapping the values of two variables.
  ```go
  i, j = j, i  // swap values of i and j
  a[i], a[j] = a[j], a[i]

  // the greatest common divisor
  func gcd(x, y int) int {
    for y != 0 {
      x, y = y, x % y
    }
    return x
  }
  ```

A type declaration defines a new named type that has the same underlying-type as an existing type.
```go
type name underlying-type
```
The underlying type determines its structure and reprensentation, and also the set of intrinsic operations it supports, which are the same as if the underlying type had been used directly.

That meas that arithmetic operations work the same for Celsius and Fahrenheit as they do for float64.

```go
type Celsius float64
type Fahrenheit float64
```
Even though both have the same underlying type float64, they are not the same type, so they cannot be compared or combined in arithmetic expressions.

# 数据类型
Go's type fall into four categoires: basic types, aggregate types, refrence type and interface types.

All assignment operations in Go are copy operations.

slice and map contain internal pointers, so copies point to the same underlying data.

变量在定义时没有明确的初始化时会赋值为**零值**.

- 数值类型为 0 ,
- 布尔类型为 false ,
- 字符串为 "" (空字符串)
- 引用类型为nil
- slice 的零值是 nil, 一个 nil 的 slice 的长度和容量是0.

## basic types
- bool
- string: string is immutable
- int, int8, int16, int32, int64, uint, uint8, uint16, uint32, uint64, uintptr
- byte  // uint8 的别名
- rune  // int32 的别名, 代表一个Unicode码
- float32, float64
- complex64, complex128

### string
A string is an immutable sequence of bytes.

The array underlying a string is hidden from view; there is no way to access its contents except through the string.
That means that when we do either of these conversions, a copy of the array must be made. Go takes care of this, of course, so you don't have to.
After either of these conversions, modifications to the array underlying the byte slice don't affect the corresponding string.

字符串和字节数组有密切关系,我们可以轻易的将它们转换成对方:
```go
stra := "the spice must flow"
byts := []byte(stra)
strb := string(byts)
```
在你使用[]byte(X)或者string(X)时务必注意,你创建的是数据的拷贝.这是由于字符串的不可变性.

如果确定得到的[]byte 不会被修改, 可以考虑使用unsafe.Pointer 来强制转换, 具体可见 [golang-string 和 bytes 之间的 unsafe 转换](https://jaycechant.info/2019/golang-unsafe-cast-between-string-and-bytes)

The built-in len function returns the number of bytes (not runes) in a string.
Attempting to access a byte outside its range results in a panic.

当字符串有由unicode字符码runes组成时.如果你计算字符串的长度时,可能得到的结果和你期待的不同.下面结果是输出3:
```go
fmt.Println(len("�"))
```
如果你通过range遍历一个字符串,你将得到runes,而不是bytes.当然,当你将一个字符串转换成一个[]byte时,你将得到正确的数据.

Immutability means that it is safe for two copies of a string to share the same underlying memory, making it cheap to copy strings of any length.(string 传参也是这样的?)
Similarly, a string s and a substring like s[7:] may share the same data, so the substring operation is also cheap.

Raw string literals, delimited by back quotes, are interpreted literally. Within the quotes, any character may appear except back quote. They can contain line breaks, and backslashes have no special meaning.

#### UTF8
UTF-8 is a variable-length encoding of Unicode points as bytes. UTF-8 was invented by Ken Thompson and Rob Pike, two of the creators of Go.
It uses between 1 and 4 bytes to represent each rune, but only 1 byte for ASCII characters, and only 2 or 3 bytes for most runes in common use.

Many Unicode characters are hard to typ e on a key board or to distinguish visually from similarlooking ones; some are even invisible.
Unicode escapes in Go string literals allow us to specify them by their numeric code point value. There are two forms, \uhhhh for a 16-bit value and \Uhhhhhhhh for a 32-bit value, where each h is a hexadecimal digit;
the need for the 32-bit form arises very infrequently.

Thus, for example, the following string literals all represent the same six-byte string:
```go
"世界"
"\xe4\xb8\x96\xe7\x95\x8c"
"\u4e16\u754c"
"\U00004e16\U0000754c"
```
The three escape sequences above provide alternative notations for the first string , but the values they denote are identical.

```go
import "unicode/utf8"
s := "Hello, BF"
fmt.Println(len(s)) // "13"
fmt.Println(utf8.RuneCountInString(s)) // "9"
```

To process those characters, we nee d a UTF-8 decoder. The unicode/utf8 package provides one that we can use like this:
```go
for i := 0; i < len(s); {
  r, size := utf8.DecodeRuneInString(s[i:])
  fmt.Printf("%d\t%c\n", i, r)
  i += size
}
```

Go's range loop, when applied to a string, performs UTF-8 decoding implicitly.

- A []rune conversion applied to a UTF-8 encoded string returns the sequence of Unicode code points that the string encodes.
- If a slice of runes is converted to a string, it produces the concatenation of the UTF-8 encodings of each rune.
- Converting an integer value to a string interprets the interger as a rune value, and yields the UTF-8 representation of that rune.

```go
// "program" in Japanese katakana
s := "プログラム"
fmt.Printf("% x\n", s) // "e3 83 97 e3 83 ad e3 82 b0 e3 83 a9 e3 83 a0"
r := []rune(s)
fmt.Printf("%x\n", r) // "[30d7 30ed 30b0 30e9 30e0]"

fmt.Println(string(r)) // "プログラム"

fmt.Println(string(65)) // "A", not "65"
fmt.Println(string(0x4eac)) // "京"
```
The verb % x in the first Printf inserts a space between each pair of hex digits.

### untyped constants
many constants are not committed to a particular type. The compiler represents these uncommitted constants with much greater numeric precisiont han values of basic types, and arithmetic on them is more precise than machine arithmetic;
you may assume at least 256 bits of precision.
There are six flavors of these uncommitted constants, called untyped boolean, untyped integer, untyped rune, untyped floating-point, untyped complex, and untyped string.

By deferring this commitment, untyped constants not only retain their higher precision until later, but they can participate in many more expressions than committe dconstants without requiring conversions.
For example, the values ZiB and YiB are too big to store in any integer variable, but they are legitimate constants that may be used in expressions like this one:
```go
fmt.Println(YiB/ZiB) // "1024"
```

the floating-point constant math.Pi may be used wherever any floating-point or complex value is needed:
```go
var x float32 = math.Pi
var y float64 = math.Pi
var z complex128 = math.Pi
```
If math.Pi had been committed to a specific type such as float64, the result would not be as precise, and type conversions would be required to use it when a float32 or complex128 value is wanted.

## aggregate types
array and struct

### array
An array type definition specifies a length and an element type. For example, the type `[4]int` represents an array of four integers.

Go's arrays are values. An array variable denotes the entire array; it is not a pointer to the first array element (as would be the case in C).
This means that when you assign or pass around an array value you will make a copy of its contents.
(To avoid the copy you could pass a pointer to the array, but then that's a pointer to an array, not an array.)

### struct
The key point is that Go gives the programmer tools to limit allocation by controlling the layout of data structures. Consider this simple type definition of a data structure containing a buffer (array) of bytes:
```go
type X struct {
  a, b, c int
  buf [256]byte
}
```
In Java, the buf field would require a second allocation and accesses to it a second level of indirection.
In Go, however, the buffer is allocated in a single block of memory along with the containing struct and no indirection is required.
For systems programming, this design can have a better performance as well as reducing the number of items known to the collector. At scale it can make a significant difference.

To give the programmer this flexibility, Go must support what we call interior pointers to objects allocated in the heap.
The X.buf field in the example above lives within the struct but it is legal to capture the address of this inner field, for instance to pass it to an I/O routine.
In Java, as in many garbage-collected languages, it is not possible to construct an interior pointer like this, but in Go it is idiomatic.
This design point affects which collection algorithms can be used, and may make them more difficult, but after careful thought we decided that it was necessary to allow interior pointers
because of the benefits to the programmer and the ability to reduce pressure on the (perhaps harder to implement) collector.
So far, our experience comparing similar Go and Java programs shows that use of interior pointers can have a significant effect on total arena size, latency, and collection times.

## refrence types
Refrence types are a diverse group that inclues pointers.

- slice
- map
- function
- channel

### slice
built on fixed-size arrays to give a flexible, extensible data structure.

It's important to understand that even though a slice contains a pointer, it is itself a value.
Under the covers, it is a struct value holding a pointer, a length and a capacity. It is not a pointer to a struct.
```go
type sliceHeader struct {
  Length        int
  Capacity      int
  ZerothElement *byte
}
```

The type specification for a slice is `[]T`, where T is the type of the elements of the slice. Unlike an array type, a slice type has no specified length.

The zero value of a slice is nil(the element pointer is still nil). The len and cap functions will both return 0 for a nil slice.
However the slice created by `array[0:0]` has length zero (and maybe even capacity zero) but its pointer is not nil, so it is not a nil slice.

- `func copy(dst, src []T) int`: The copy function supports copying between slices of different lengths (it will copy only up to the smaller number of elements).
  In addition, copy can handle source and destination slices that share the same underlying array, handling overlapping slices correctly.
- `func append(s []T, x ...T) []T`

As mentioned earlier, re-slicing a slice doesn't make a copy of the underlying array.
The full array will be kept in memory until it is no longer referenced. Occasionally this can cause the program to hold all the data in memory when only a small piece of it is needed.
To fix this problem one can copy the interesting data to a new slice before returning it:
```go
func CopyDigits(filename string) []byte {
  b, _ := ioutil.ReadFile(filename)
  b = digitRegexp.Find(b)
  c := make([]byte, len(b))
  copy(c, b)
  return c
}
```

### map
可以比较的数据类型才能作为一个map的键,所以map的键都是基本类型.

### channel
Do not communicate by sharing memory, share memory by communicating.

channels are first class values, just like strings or integers.

```go
ch <- v  // 将 v 送入 channel ch.
v := <-ch  // 从 ch 接收,并且赋值给 v.
```

和 map 与 slice 一样,channel 使用前必须创建: `ch := make(chan int)`

By default channels are unbuffered, meaning that they will only accept sends (chan <-) if there is a corresponding receive (<- chan) ready to receive the sent value.
Buffered channels accept a limited number of values without a corresponding receiver for those values.

The select statement provides another way to handle multiple channels.
It's like a switch, but each case is a communication:

- All channels are evaluated
- Selection blocks until one communication can proceed
- If multiple can proceed, select chooses pseudo-randomly.
- A defalut clause, if present, executes immediately if no channel is ready.

当case上读一个通道时,如果这个通道是nil,则该case永远阻塞.
这个功能有1个妙用,select通常处理的是多个通道,当某个读通道关闭了,但不想select再继续关注此case,继续处理其他case,把该通道设置为nil即可.

```go
select {
  case v1 := <- c1:
    fmt.Printf("received %v from c1\n", v1)
  case v2 := <- c2:
    fmt.Printf("received %v from c2\n", v2)
  default:
    fmt.Printf("no one ready to communicate\n")
}
```

## 类型转换
表达式 T(v) 将值 v 转换为类型 T .
与 C 不同的是 Go 的在不同类型之间的项目赋值时需要显式转换.

结构体
```go
type Vertex struct {
  X int
  Y int
}
v := Vertex{1, 2}
p := &v // 结构体指针, 访问也通过`.`, 而不是 `->`, eg: p.X

p  = &Vertex{1, 2} // 类型为 *Vertex
```

## new and make
Go has two allocation primitives, the built-in functions new and make. They do different things and apply to different types.

new is a built-in function that allocates memory, but unlike its namesakes in some other languages it does not initialize the memory, it only zeros it.
It returns a pointer to a newly allocated zero value of type T.

`new(File)` and `&File{}` are equivalent

The built-in function `make(T, args)` serves a purpose different from `new(T)`. It creates slices, maps, and channels only, and it returns an initialized (not zeroed) value of type `T (not *T)`.
The reason for the distinction is that these three types represent, under the covers, references to data structures that must be initialized before use.
A slice, for example, is a three-item descriptor containing a pointer to the data (inside an array), the length, and the capacity, and until those items are initialized, the slice is nil.
For slices, maps, and channels, make initializes the internal data structure and prepares the value for use.

# function
函数也是值.
```go
func add(x int, y int) int {
  return x + y
}
```
由于两个参数x, y 都是int 类型, 可以缩写为: `x, y int`

多值返回
```go
func swap(x, y string) (string, string) {
  return y, x
}
a, b := swap("hello", "world")
```

Finally, each source file can define its own niladic init function to set up whatever state is required.
init is called after all the variable declarations in the package have evaluated their initializers, and those are evaluated only after all the imported packages have been initialized.

## method
Go 没有类.然而,仍然可以在结构体类型上定义方法.

方法接收者 出现在 func 关键字和方法名之间的参数中.
```go
type Vertex struct {
  X, Y float64
}

func (v *Vertex) Abs() float64 {
  return math.Sqrt(v.X * v.X + v.Y * v.Y)
}

func main() {
  v := &Vertex{3, 4}
  fmt.Println(v.Abs())
}
```
`v *Vertex` 就是接收者, 使用指针与C 中原因一样, 通过指针可以直接访问原来的变量, 而不是副本

你可以对包中的 任意 类型定义任意方法,而不仅仅是针对结构体.
但是,不能对来自其他包的类型或基础类型定义方法.
[demo](../demo/go/method_for_MyFloat64.go)

# defer
The behavior of defer statements is straightforward and predictable. There are three simple rules:

1. A deferred function's arguments are evaluated when the defer statement is evaluated.
  ```go
  // the expression "i" is evaluated when the Println call is deferred.
  // the deferred call will print "0" after the function returns.
  func a() {
    i := 0
    defer fmt.Println(i)
    i++
    return
  }
  ```
1. Deferred function calls are executed in Last In First Out order after the surrounding function returns.
  ```go
  // prints "3210":
  func b() {
    for i := 0; i < 4; i++ {
      defer fmt.Print(i)
    }
  }
  ```
1. Deferred functions may read and assign to the returning function's named return values.
   [return xxx这一条语句并不是一条原子指令](https://tiancaiamao.gitbooks.io/go-internals/content/zh/03.4.html)
   用一个简单的转换规则改写一下,就不会迷糊了.改写规则是将return语句拆成两句写,return xxx会被改写成:
  ```
  返回值 = xxx
  调用defer函数
  空的return
  ```
  因此

  ```go
  func f() (result int) {
    defer func() {
      result++
    }()
    return 0
  }
  ```
  实际执行的时候, 相当于
  ```go
  func f() (result int) {
    result = 0  // return语句不是一条原子调用,return xxx其实是赋值＋ret指令
    func() {  // defer被插入到return之前执行,也就是赋返回值和ret指令之间
      result++
    }()
    return
  }
  ```

# error
The error type is an interface type. An error variable represents any value that can describe itself as a string. Here is the interface's declaration:
```go
type error interface {
  Error() string
}
```
a useful function is the fmt package's Errorf. It formats a string according to Printf's rules and returns it as an error created by errors.New.
```go
if f < 0 {
  return 0, fmt.Errorf("math: square root of negative number %g", f)
}
```

