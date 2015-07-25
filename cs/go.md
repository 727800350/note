# intro

# package
每个 Go 程序都是由包组成的.

程序运行的入口是包 main.
```
package main

import (
	"fmt"
	"math/rand"
)

func main() {
	fmt.Println("My favorite number is", rand.Intn(10))
}
```

Go支持C语言风格的/* */块注释, //行注释

# IO
fmt.Println()

io 包指定了 io.Reader 接口, 它表示从数据流读取.

Go 标准库包含了这个接口的许多实现, 包括文件,网络连接,压缩,加密等等.

io.Reader 接口有一个 Read 方法:

func (T) Read(b []byte) (n int, err error)
Read 用数据填充指定的字节 slice,并且返回填充的字节数和错误信息. 在遇到数据流结尾时,返回 io.EOF 错误.

[read from a string](../demo/go/reader_string.go)

# 基本类型
Go 的基本类型有Basic types

- bool
- string
- int  int8  int16  int32  int64
- uint uint8 uint16 uint32 uint64 uintptr
- byte // uint8 的别名
- rune // int32 的别名, 代表一个Unicode码
- float32 float64
- complex64 complex128

变量在定义时没有明确的初始化时会赋值为**零值**.

- 数值类型为 0 ,
- 布尔类型为 false ,
- 字符串为 "" (空字符串)
- 指针为nil
- slice 的零值是 nil, 一个 nil 的 slice 的长度和容量是 0.

## 类型转换
表达式 T(v) 将值 v 转换为类型 T .
与 C 不同的是 Go 的在不同类型之间的项目赋值时需要显式转换.

## 类型推导
在定义一个变量但不指定其类型时(使用没有类型的 var 或 := 语句), 变量的类型由右值推导得出.

## 常量
常量的定义与变量类似,只不过使用 const 关键字.
常量可以是字符,字符串,布尔或数字类型的值.
const pi = 3.14

## Exported names
在 Go 中,首字母大写的名称是被导出的.
eg: `Foo` 和 `FOO` 都是被导出的名称.名称 `foo` 是不会被导出的.
在import 了 math 包之后, math 中的常量pi就就成为exported name, 但是需要使用首字母大写的形式来访问: `math.Pi`

var 语句定义了一个变量的列表
```
var c, python, java bool
var i, j int = 1, 2
var c, python, java = true, false, "no!" //如果初始化是使用表达式,则可以省略类型,变量从初始值中获得类型.
var p *int // int 类型的指针
var a [10]int // 数组
```

在函数中, := 简洁赋值语句在明确类型的地方,可以用于替代 var 定义.

function variable (analogous to a function pointer in C): `f func(func(int,int) int, int) int`  
Or if f returns a function: `f func(func(int,int) int, int) func(int, int) int`
It still reads clearly, from left to right, and it is always obvious which name is being declared - the name comes first.

The distinction between type and expression syntax makes it easy to write and invoke closures in Go:
`sum := func(a, b int) int {return a + b} (3, 4)`

结构体
```
type Vertex struct {
	X int
	Y int
}
v := Vertex{1, 2}
p := &v // 结构体指针, 访问也通过`.`, 而不是 `->`, eg: p.X

p  = &Vertex{1, 2} // 类型为 *Vertex
```

## slice
一个 slice 会指向一个序列的值,并且包含了长度信息.
s := []int{2, 3, 5, 7, 11, 13}
len(s)

s[lo:hi]
表示从 lo 到 hi-1 的 slice 元素,含两端.因此

slice 由函数 make 创建.这会分配一个零长度的数组并且返回一个 slice 指向这个数组:
a := make([]int, 5)  // len(a)=5
为了指定容量,可传递第三个参数到 make:
b := make([]int, 0, 5) // len(b)=0, cap(b)=5

slice2 := append(slice1, element) // 向slice1 中添加元素element, 结果为slice2, 注意slice1 保持不变

## map
```
type Vertex struct {
	Lat, Long float64
}

// string 到 vertex 的map 映射
var m = map[string]Vertex{
	"Bell Labs": Vertex{
		40.68433, -74.39967,
	},
	"Google": Vertex{
		37.42202, -122.08408,
	},
}
```

- m[key] = elem 在 map m 中插入或修改一个元素:
- elem = m[key] 获得元素:
- delete(m, key) 删除元素:
- elem, ok = m[key] 通过双赋值检测某个键存在: 如果 key 在 m 中, ok 为 true.否则, ok 为 false,并且 elem 是 map 的元素类型的零值.

# Function
函数也是值.
```
func add(x int, y int) int {
	return x + y
}
```
由于两个参数x,y 都是int 类型, 可以缩写为: `x, y int`

多值返回
```
func swap(x, y string) (string, string) {
	return y, x
}
a, b := swap("hello", "world")
```

命名返回值
Go 的返回值可以被命名,并且像变量那样使用.
```
func split(sum int) (x, y int) {
	x = sum * 4 / 9
	y = sum - x
	return x,y
}
```

## method
Go 没有类.然而,仍然可以在结构体类型上定义方法.

方法接收者 出现在 func 关键字和方法名之间的参数中.
```
type Vertex struct {
	X, Y float64
}

func (v *Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
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

## closure 闭包

# 流程
## for
Go 只有一种循环结构- for 循环.

基本的 for 循环除了没有了 ( ) 之外(甚至强制不能使用它们),看起来跟 C 或者 Java 中做的一样,而 { } 是必须的.

```
sum := 0
for i := 0; i < 10; i++ {
	sum += i
}
```

跟 C 或者 Java 中一样,可以让前置,后置语句为空.
```
sum := 1
for ; sum < 1000; {
	sum += sum
}
```

for 是 Go 的 "while"
```
sum := 1
for sum < 1000 {
	sum += sum
}
```

死循环
```
for {
}
```

### range
for 循环的 range 格式可以对 slice 或者 map 进行迭代循环.
```
var pow = []int{1, 2, 4, 8, 16, 32, 64, 128}
for i, v := range pow {
	fmt.Printf("2**%d = %d\n", i, v)
}
```
i 为 index, v index 处的value

```
pow := make([]int, 10)
for i := range pow {
	pow[i] = 1 << uint(i)
}
```
可以省略掉value 部分

```
for _, value := range pow {
	fmt.Printf("%d\n", value)
}
```
可以通过赋值给 _ 来忽略序号index 或者value

## if
if 也一样

跟 for 一样, if 语句可以在条件之前执行一个简单的语句.
```
func pow(x, n, lim float64) float64 {
	if v := math.Pow(x, n); v < lim {
		return v
	}
	return lim
}
```

## switch
```
switch os := runtime.GOOS; os
case "darwin":
	fmt.Println("OS X.")
case "linux":
	fmt.Println("Linux.")
default:
	// freebsd, openbsd,
	// plan9, windows...
	fmt.Printf("%s.", os)
```
没有条件的 switch 同 switch true 一样.
```
t := time.Now()
switch {
	case t.Hour() < 12:
		fmt.Println("Good morning!")
	case t.Hour() < 17:
		fmt.Println("Good afternoon.")
	default:
		fmt.Println("Good evening.")
}
```
这一构造使得可以用更清晰的形式来编写长的 if-then-else 链.

# goroutine
goroutine 是由 Go 运行时环境管理的轻量级线程.
the functionality is analogous to the & on the end of a shell command

`go f(x, y, z)` 开启一个新的 goroutine 执行 `f(x, y, z)`
f,x,y 和 z 是当前 goroutine 中定义的,但是在新的 goroutine 中运行 f.

# channel
channel 是有类型的管道,可以用 channel 操作符 `<-` 对其发送或者接收值.
```
ch <- v    // 将 v 送入 channel ch.
v := <-ch  // 从 ch 接收,并且赋值给 v.
```
("箭头"就是数据流的方向.)

和 map 与 slice 一样,channel 使用前必须创建: `ch := make(chan int)`

默认情况下,在另一端准备好之前,发送和接收都会阻塞.
这使得 goroutine 可以在没有明确的锁或竞态变量的情况下进行同步.

channel 可以是 带缓冲的.为 make 提供第二个参数作为缓冲长度来初始化一个缓冲 channel:
`ch := make(chan int, 100)`
通过`cap(chan)` 获取缓冲区大小
向缓冲 channel 发送数据的时候,只有在缓冲区满的时候才会阻塞.

Buffering removes synchronization.
Buffering makes them more like Erlang mailboxes.

Do not communicate by sharing memory, share memory by communicating.
