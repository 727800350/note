# intro
在 Go 中,首字母大写的名称是被导出的.
eg: `Foo` 和 `FOO` 都是被导出的名称.名称 `foo` 是不会被导出的.
在import 了 math 包之后, math 中的常量pi就就成为exported name, 但是需要使用首字母大写的形式来访问: `math.Pi`

# IO
- `fmt.Println`
- `fmt.Printf`

io 包指定了 io.Reader 接口, 它表示从数据流读取, 有一个 Read 方法:
`func (T) Read(b []byte) (n int, err error)`
Read 用数据填充指定的字节 slice,并且返回填充的字节数和错误信息. 在遇到数据流结尾时,返回 io.EOF 错误.

- Go 标准库包含了这个接口的许多实现, 包括文件, 网络连接, 压缩, 加密等

[read from a string](../demo/go/reader_string.go)

# 数据类型
All assignment operations in Go are copy operations.

slice and map contain internal pointers, so copies point to the same underlying data.

## 基本类型
Go 的基本类型有Basic types

- bool
- string: string is immutable
- int, int8, int16, int32, int64, uint, uint8, uint16, uint32, uint64, uintptr
- byte  // uint8 的别名
- rune  // int32 的别名, 代表一个Unicode码
- float32, float64
- complex64, complex128

变量在定义时没有明确的初始化时会赋值为**零值**.

- 数值类型为 0 ,
- 布尔类型为 false ,
- 字符串为 "" (空字符串)
- 指针为nil
- slice 的零值是 nil, 一个 nil 的 slice 的长度和容量是0.

## array
- the size of an array is part of its type, which limits its expressive power.
- copies refer to different underlying data, 也就是值拷贝

## slice
slices, which built on fixed-size arrays to give a flexible, extensible data structure.

slice 由函数 make 创建.这会分配一个零长度的数组并且返回一个 slice 指向这个数组:
a := make([]int, 5)  // len(a)=5
为了指定容量,可传递第三个参数到 make:
b := make([]int, 0, 5) // len(b)=0, cap(b)=5

slice2 := append(slice1, element) // 向slice1 中添加元素element, 结果为slice2, 注意slice1 保持不变

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

# Function
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

# goroutine
goroutine 是由 Go 运行时环境管理的轻量级线程.
the functionality is analogous to the & on the end of a shell command

- `runtime.GOMAXPROCS(num int)`: 控制并发度, -1 会用到所有的core
- `go run -race file.go`: dead lock 检测

# channel
Do not communicate by sharing memory, share memory by communicating.

channel 是有类型的管道,可以用 channel 操作符 `<-`(箭头的指向就是数据流的方向) 对其发送或者接收值.
```go
ch <- v  // 将 v 送入 channel ch.
v := <-ch  // 从 ch 接收,并且赋值给 v.
```

和 map 与 slice 一样,channel 使用前必须创建: `ch := make(chan int)`

默认情况下,在另一端准备好之前,发送和接收都会阻塞.
这使得 goroutine 可以在没有明确的锁或竞态变量的情况下进行同步.

channel 可以是 带缓冲的.为 make 提供第二个参数作为缓冲长度来初始化一个缓冲 channel:
`ch := make(chan int, 100)`
通过`cap(chan)` 获取缓冲区大小
向缓冲 channel 发送数据的时候,只有在缓冲区满的时候才会阻塞.

