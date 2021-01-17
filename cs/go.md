- [intro](#intro)
- [IO](#io)
  - [fmt](#fmt)
- [variable](#variable)
- [数据类型](#数据类型)
  - [basic types](#basic-types)
    - [string](#string)
      - [UTF8](#utf8)
    - [untyped constants](#untyped-constants)
  - [aggregate types](#aggregate-types)
    - [array](#array)
    - [struct](#struct)
      - [struct embedding and anonymous fields](#struct-embedding-and-anonymous-fields)
  - [refrence types](#refrence-types)
    - [slice](#slice)
    - [map](#map)
    - [channel](#channel)
  - [类型转换](#类型转换)
  - [new and make](#new-and-make)
- [function](#function)
  - [anonymous functions](#anonymous-functions)
  - [capture iteration variables](#capture-iteration-variables)
  - [variadic functions](#variadic-functions)
  - [defer](#defer)
  - [panic and recover](#panic-and-recover)
    - [recover 的实现与设计](#recover-的实现与设计)
- [method](#method)
  - [Composing types by struct embedding](#composing-types-by-struct-embedding)
  - [method values and expressions](#method-values-and-expressions)
- [error](#error)

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

If an array's element type is comparable then the array is comparable too.

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

The struct type with no fields is called the empty struct, written struct{}. It has size zero and carries no information but may be useful nonetheless.
Some go programmers use it instead of bool as the value type of a map that represents a set, to emphasize that only the keys are significant.
but the space saving is marginal and the syntax more cumbersome, so we generally avoid it.

If all the fields of a struct are comparable, the struct itself is comparable.
Comparable struct types, like other comparable types, may be used as the key type of a map.

#### struct embedding and anonymous fields
Go lets us declare a field with a type but no name; such fields are called anonymous fields.
The type of the field must be a named type or a pointer to a named type.
It provides a convenient syntactic shortcut so that a simple dot expression like x.f can stand for a chain of fields like x.d.e.f.
```go
type Point struct {
  X, Y int
}
type Circle struct {
  Point
  Radius int
}
type Wheel struct {
  Circle
  Spokes int
}

var w Wheel
w.X = 8  // equivalent to w.Circle.Point.X = 8
```
However there's no coresponding shorthand for the struct literal syntax.
```go
x := Wheel{
  Circle: Circle{
    Point:  Point{X: 8, Y: 8},
    Radius: 5,
  },
  Spokes: 20,
}
```

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

slices are not comparable, so we cannot use == to test whether two slices contains the same elements.
The standard library provides the highly optimized bytes.Equal function for comparing two slices of bytes ([]byte), but for other types of slice, we must do the comparison ourselves.

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

Most operations on maps, including lookup, delete, len and range loops are safe to perform on a nil map reference, since
it behaves like an empty map.
But storing to a nil map causes panic. You must allocate the map before you can store into it.

[Why does Go forbid taking the address of (&) map member, yet allows (&) slice element?](
  https://stackoverflow.com/questions/32495402/why-does-go-forbid-taking-the-address-of-map-member-yet-allows-slice-el)

There is a major difference between slices and maps: Slices are backed by a backing array and maps are not.

If a map grows or shrinks a potential pointer to a map element may become a dangling pointer pointing into nowhere
(uninitialised memory). The problem here is not "confusion of the user" but that it would break a major design element
of Go: No dangling pointers.

If a slice runs out of capacity a new, larger backing array is created and the old backing array is copied into the new;
and the old backing array remains existing. Thus any pointers obtained from the "ungrown" slice pointing into the old
backing array are still valid pointers to valid memory.

### channel
Do not communicate by sharing memory, share memory by communicating.

channels are first class values, just like strings or integers.

- The zero value for a channel is nil. Send and receive operations on a nil channel block forever, a case in a select
  statement whose channel is nil is never selected.
- 关闭已经关闭的channel会导致panic
- 发送值到已经关闭的channel会导致panic
- 读取关闭后的无缓存通道,不管通道中是否有数据,返回值都为zero value 和false.
- 读取关闭后的有缓存通道,将缓存数据读取完后,再读取返回值为zero value 和false.
- range 遍历通道,通道写完后,必须关闭通道,否则range 遍历会出现死锁

```go
ch <- v  // 将 v 送入 channel ch.
v := <-ch  // 从 ch 接收,并且赋值给 v.
```

和 map 与 slice 一样,channel 使用前必须创建: `ch := make(chan int)`

By default channels are unbuffered, meaning that they will only accept sends (chan <-) if there is a corresponding
receive (<- chan) ready to receive the sent value.
Buffered channels accept a limited number of values without a corresponding receiver for those values.

The select statement provides another way to handle multiple channels.
It's like a switch, but each case is a communication:

- All channels are evaluated
- Selection blocks until one communication can proceed
- If multiple can proceed, select chooses pseudo-randomly.
- A defalut clause, if present, executes immediately if no channel is ready.

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

## anonymous functions
Function values can have state(closure):
```go
func squares() func() int {
  var x int
  return func() int {
    x++
    return x * x
  }
}

f := squares()
fmt.Println(f())  // 1
fmt.Println(f())  // 4
fmt.Println(f())  // 9
```
Here again we see an example where the lifetime of a varaible is not determined by its scope:
the variable x exists after squares has returned within main, even thouth x is hidden inside f.

When an anonymous function requires rescursion, we must first declare a variable and then assign the anonymous function to that variable.

## capture iteration variables
Consider a program that must create a set of directories and later remove them.
```go
var rmdirs []func()
for _, d := range tempDirs() {
  dir := d               // NOTE: necessary!
  os.MkdirAll(dir, 0755) // creates parent directories too
  rmdirs = append(rmdirs, func() {
    os.RemoveAll(dir)
  })
}

// ...do some work...
for _, rmdir := range rmdirs {
  rmdir() // clean up
}
```
You may b e wondering why we assigned the loop variable d to a new local variable dir within the loop body, instead of just naming the loop variable dir as in this subtly incorrect variant:
```go
var rmdirs []func()
for _, dir := range tempDirs() {
  os.MkdirAll(dir, 0755)
  rmdirs = append(rmdirs, func() {
    os.RemoveAll(dir) // NOTE: incorrect!
  })
}
```
The reason is a consequence of the scope rules for loop variables. In the programimme diately above, the for loop introduces a new lexical block in which the variable dir is declared.
**All function values created by this loop "capture" and share the same variable—an addressable storage location, not its value at that particular moment.**
The value of dir is updated in successive iterations, so by the time the cleanup functions are called, the dir variable has been updated several times by the now-completed for loop.
Thus dir holds the value from the final iteration, and consequently all calls to os.RemoveAll will attempt to remove the same directory.

Frequently, the inner variable introduce d to work around this problem—dir in our example— is given the exact same name as the outer variable of which it is a copy,
leading to o dd-looking but crucial variable declarations like this:
```go
for _, dir := range tempDirs() {
  dir := dir // declares inner dir, initialized to outer dir
  // ...
}
```

The problem of iteration variable capture is most often encountered when using the go statement or with defer since both may delay the execution of a function value until after the loop has finished.

## variadic functions
```go
func sum(vals ...int) int {
  total := 0
  for _, val := range vals {
    total += val
  }
  return total
}

fmt.Println(sum())
fmt.Println(sum(3))
fmt.Println(sum(1, 2, 3, 4))
```
Within the body of the function, the type of vals is an []int type.

Implicitly, the caller allocates an array, copies the arguments into it, and passes a slice of the entire array to the function.
```go
values := []int{1, 2, 3, 4}
fmt.Println(sum(values...)) // "10"
```

## defer
Deferred functions run after return statements have updated the function's result variables.

The behavior of defer statements is straightforward and predictable. There are three simple rules:

1. A deferred function's arguments are evaluated when the defer statement is evaluated.
```go
func TestInt(t *testing.T) {
	x := 0
	defer t.Log("1", x)
	defer func() {
		t.Log("2", x)
	}()
	x++
	t.Log("3", x)
}

func TestSlice(t *testing.T) {
	x := make([]int, 0)
	defer t.Log("1", x, len(x))
	defer func() {
		t.Log("2", x, len(x))
	}()
	x = append(x, 1, 2)
	t.Log("3", x, len(x))
}

func TestMap(t *testing.T) {
	x := make(map[int]bool, 0)
	defer t.Log("1", x, len(x))
	defer func() {
		t.Log("2", x, len(x))
	}()
	x[1] = true
	x[2] = true
	t.Log("3", x, len(x))
}
```
```out
=== RUN   TestInt
    TestInt: main_test.go:14: 3 1
    TestInt: main_test.go:11: 2 1
    TestInt: main_test.go:15: 1 0
--- PASS: TestInt (0.00s)
=== RUN   TestSlice
    TestSlice: main_test.go:24: 3 [1 2] 2
    TestSlice: main_test.go:21: 2 [1 2] 2
    TestSlice: main_test.go:25: 1 [] 0
--- PASS: TestSlice (0.00s)
=== RUN   TestMap
    TestMap: main_test.go:35: 3 map[1:true 2:true] 2
    TestMap: main_test.go:31: 2 map[1:true 2:true] 2
    TestMap: main_test.go:36: 1 map[1:true 2:true] 0
--- PASS: TestMap (0.00s)
```
**需要注意slice 和 map 的区别**
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
  ```info
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
  所以最终的返回值是1, 而不是0

The defer statement can also be used to pair "on entry" and "on exit" actions when debugging a complex function.
```go
func bigSlowOperation() {
  defer trace("bigSlowOperation")() // don't forget the extra parentheses
  // ...lots of work...
  time.Sleep(10 * time.Second) // simulate slow operation by sleeping
}

func trace(msg string) func() {
  start := time.Now()
  log.Printf("enter %s", msg)
  return func() { log.Printf("exit %s (%s)", msg, time.Since(start)) }
}
```
Each time bigSlowOperation is called, it logs its entry and exit and the elapsed time between them.

## panic and recover
For diagnostic purposes, the runtime package lets the programmer dump the stack using the same machinery.
```go
func main() {
  defer printStack()
  f(3)
}

func printStack() {
  var buf [4096]byte
  n := runtime.Stack(buf[:], false)
  os.Stdout.Write(buf[:n])
}
```

假设函数包含多个defer函数, 前面的defer通过recover()消除panic后, 函数中剩余的defer仍然会执行, 但不能再次recover().
但是之后的defer 实际执行是在panic.go 中.
```go
func main() {
  glog.Infoln("gopher")

  defer glog.Infoln("before")
  defer func() {
    glog.Errorln(recover())
  }()
  defer glog.Infoln("after")

  panic("panic")
}
```
```log
[09:05:21 liuqi.victor@n227-022-231:gocode]$ go run main.go
I0922 09:06:22.801349 3869408 main.go:19] gopher
I0922 09:06:22.801456 3869408 panic.go:969] after
E0922 09:06:22.801466 3869408 main.go:23] panic
I0922 09:06:22.801475 3869408 main.go:27] before
```

### recover 的实现与设计
[recover源码剖析](https://my.oschina.net/renhc/blog/3217650)

```go
// recover/compile.go
package recover

func compile() {
  defer func() {
    recover()
  }()
}
```
然后使用以下命令编译代码:
```bash
go tool compile -S recover/compile.go
```
接着根据代码行号找出recover()语句对应的汇编码:
```asm
0x0024 00036 (recover/compile.go:5)     PCDATA  $0, $1
0x0024 00036 (recover/compile.go:5)     PCDATA  $1, $0
0x0024 00036 (recover/compile.go:5)     LEAQ    ""..fp+40(SP), AX
0x0029 00041 (recover/compile.go:5)     PCDATA  $0, $0
0x0029 00041 (recover/compile.go:5)     MOVQ    AX, (SP)
0x002d 00045 (recover/compile.go:5)     CALL    runtime.gorecover(SB)
```
我们可以看到recover()函数调用被替换成了runtime.gorecover()函数. runtime.gorecover()实现源码位于src/runtime/panic.go.

runtime.gorecover()函数实现很简短:
```go
// Must be in a function running as part of a deferred call during the panic.
// Must be called from the topmost function of the call
// (the function used in the defer statement).
// p.argp is the argument pointer of that topmost deferred function call.
// Compare against argp reported by caller.
// If they match, the caller is the one who can recover.
func gorecover(argp uintptr) interface{} {
  gp := getg()
  // 获取panic实例, 只有发生了panic, 实例才不为nil
  p := gp._panic
  // recover限制条件
  if p != nil && !p.goexit && !p.recovered && argp == uintptr(p.argp) {
    p.recovered = true
    return p.arg
  }
  return nil
}
```

通过代码的if语句可以看到需要满足四个条件才可以恢复panic,且四个条件缺一不可:

1. p != nil: 必须存在panic,
1. !p.goexit: 非runtime.Goexit(),
1. !p.recovered: panic还未被恢复,
1. argp == uintptr(p.argp): recover()必须被defer()直接调用(TODO: 这里直接调用的表述有疑问, 这里的defer 直接调用, 都是指
  `defer func(){ call })`, 至于panic 的调用有没有嵌套func(){}() 不重要.
  [Handling panics](https://golang.org/ref/spec#Handling_panics)

内置函数recover 没有参数,runtime.gorecover 函数却有参数,为什么呢? 这正是为了限制recover()必须被defer()直接调用.

runtime.gorecover 函数的参数为调用recover 函数的参数地址, 通常是defer函数的参数地址, 同时 panic实例中也保存了当前defer
函数的参数地址, 如果二者一致,说明recover()被defer函数直接调用, 否则不是, 无法recover. 举例如下:
```go
func foo() {
  defer func() { // 假设函数为A
    func() { // 假设函数为B
      // runtime.gorecover(B),传入函数B的参数地址
      // argp == uintptr(p.argp) 检测失败,无法恢复
      if err := recover(); err != nil {
        fmt.Println("A")
      }
    }()
  }()
  panic(errors.New("panic test"))
}
```
通过上面的分析,从代码层面我们理解了为什么recover()函数必须被defer直接调用才会生效.但为什么要有这样的设计呢?

考虑下面的代码:
```go
func foo() {
  defer func() {
    thirdPartPkg.Clean() // 调用第三方包清理资源
  }()

  if err != nil { // 条件不满足触发panic
    panic(xxx)
  }
}
```
有时我们会在代码里显式地触发panic,同时往往还会在defer函数里调用第三方包清理资源,如果第三方包也使用了recover(),那么我们触
发的panic将会被拦截,而且这种拦截可能是非预期的,并不我们期望的结果.

# method
Methods may be declared on **any named type in the same package**, so long as its underlying type is neither a pointer
nor an interface.

## Composing types by struct embedding
A similar mechanism of field embedding applies to the method. We can call methods of the embedded field using a receiver
of outer ColoredPoint, even though the outer type has no declared methods.
```go
var (
  mutex   sync.Mutex
  mapping = make(map[string]string)
)

func Lookup(key string) string {
  mutex.Lock()
  v := mapping[key]
  mutex.Unlock()
  return v
}
```

This version below is functionally equivalent but groups together two related variables in a single package-level
variable, cache:
```go
var cache = struct {
  sync.Mutex
  mapping map[string]string
}{
  mapping: make(map[string]string),
}

func Lookup(key string) string {
  cache.Lock()
  v := cache.mapping[key]
  cache.Unlock()
  return v
}
```
The new variable gives more expressive names to the variables related to the cache, and because the sync.Mutex field is embedded within it, its Lock and Unlock methods are promoted to the unnamed struct type.

## method values and expressions
Usually we select and call a method in the same expression, as in p.Distance(), but it's possible to separate these two operations.
The selector p.Distance yields a method value, a function that binds a method (Point.Distance) to a specific receiver value p. This function can then be invoked without a receiver value; it needs only the non-receiver arguments.
```go
func (p Point) Distance(q Point) { /* ... */ }

p := Point{1, 2}
q := Point{4, 6}
distanceFromP := p.Distance // method value
fmt.Println(distanceFromP(q))
```

Method values are useful when a package's API calls for a function value, and the client's desired behavior for that function is to call a method on a specific receiver.
```go
type Rocket struct { /* ... */ }
func (r *Rocket) Launch() { /* ... */ }

r := &Rocket{}
time.AfterFunc(10 * time.Second, func() { r.Launch() })
```
The method value syntax is shorter:
```go
time.AfterFunc(10 * time.Second, r.Launch())
```

Related to the method value is the method expression.
A method expression, written `T.f or (*T).f` where T is a type, yields a function value with a regular first parameter taking the place of the receiver, so it can be called in the usual way.
```go
p := Point{1, 2}
q := Point{4, 6}
distance := Point.Distance   // method expression
fmt.Println(distance(p, q))  // "5"
fmt.Printf("%T\n", distance) // "func(Point, Point) float64"

scale := (*Point).ScaleBy
scale(&p, 2)
fmt.Println(p)            // "{2 4}"
fmt.Printf("%T\n", scale) // "func(*Point, float64)"
```

Method expressions can be helpful when you need a value to represent a choice among several methods belonging to the same type so that you can call the chosen method with many different receivers.
In the following example, the variable op represents either the addition or the subtraction method of type Point, and Path.TranslateBy calls it for each point int he Path:
```go
type Point struct{ X, Y float64 }
func (p Point) Add(q Point) Point { return Point{p.X + q.X, p.Y + q.Y} }
func (p Point) Sub(q Point) Point { return Point{p.X - q.X, p.Y - q.Y} }
type Path []Point

func (path Path) TranslateBy(offset Point, add bool) {
  var op func(p, q Point) Point
  if add {
    op = Point.Add
  } else {
    op = Point.Sub
  }
  for i := range path {
    // Call either path[i].Add(offset) or path[i].Sub(offset).
    path[i] = op(path[i], offset)
  }
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

Go's approach sets it apart from many other languages in which failures are reported using exceptions, not ordinary values.
The reason for this design is that exceptions tend to entangle the description of an error with the control flow required to handle it, often leading to an undesirable outcome:
routine errors are reported to the end user in the form of an incomprehensible stack trace, full of information about the structure of the program but lacking intelligible context about what went wrong.
By contrast, Go programs use ordinary control-flow mechanisms like if and return to respond to errors. This style undeniably demands that more attention be paid to error-handling logic, but that is precisely the point.

If the failure has only one possible cause, the result is boolean, usually called ok.

Error should provide a clear causal chain from the root problem to the overall failure, reminiscent of a NASA accident investigation:
```info
genesis: crashed: no parachute: G-switch failed: bad relay orientation
```
Because error messages are frequently chained together, message strings should not be capitalized and newlines should be avoided.
The resulting errors may be long, but they will be self-contained when found by tools like grep.

