Package names usually take the singular form.

An internal pkg may be imported only by another pkg that is inside the tree rooted at the parent of the internal directory.
For example, given the pkgs below
```
net/http
net/http/internal/chunked
net/http/httputil
net/url
```
net/http/internal/chunked could be imported from net/http or net/http/httputil, but not from net/url.

# strings
- `func Join(a []string, sep string) string`
- `func Split(s, sep string) []string`
- HasPrefix, HasSuffix
- `func ToUpper(s string) string`, ToLower
- `func LastIndex(s, substr string) int`

# strconv
- `func Atoi(s string) (int, error)`: Atoi is equivalent to ParseInt(s, 10, 0), converted to type int.
- `func ParseInt(s string, base int, bitSize int) (i int64, err error)`: bitSize 0 for int, 32 for int32, 64 for int64. In any case, the type of result is always int64.

# bytes
- `Buffer`: A Buffer is a variable-sized buffer of bytes with Read and Write methods.
  The bytes.Buffer type is extremely versatile, and it may be used as a replacement for a file whenever an IO function requires a sink for bytes(io.Writer) as Fprintf does above or a source of bytes(io.Reader).

# unicode
- `func IsDigit(r rune) bool`, IsLetter, IsUpper, IsLower
- `func ToUpper(r rune) rune`, ToLower

# os
- `os.Args`: a slice of strings

# io
- `func Copy(dst Writer, src Reader) (written int64, err error)`: implemented by calling src.WriteTo(dst) if src implements the WriterTo interface, Otherwise by calling dst.ReadFrom(src), if dst implements the ReaderFrom interface.

# bufio
bufio pkg helps make input and output efficient and convenient.

- `func NewScanner(r io.Reader) *Scanner`: 默认以\n为分隔符
  ```go
  in := bufio.NewScanner(os.stdin)
  for in.Scan() {
    glog.Infoln(in.Text())  // Text 会把行尾的\n 自动去掉
  }
  ```
- `func NewReader(rd io.Reader) *Reader`

# ioutil
- `Discard`: is an io.Writer on which all Write calls succeed without doing anything.

- `func readAll(r io.Reader, capacity int64) (b []byte, err error)`
- `func ReadFile(filename string) ([]byte, error)`
- `func WriteFile(filename string, data []byte, perm os.FileMode) error`
- `func ReadDir(dirname string) ([]os.FileInfo, error)`

# net
## http
- `func Get(url string) (resp *Response, err error)`: When err is nil, resp always contains a non-nil resp.Body(of type io.ReadCloser). Caller should close resp.Body when done reading from it.

# time
```go
const (
    Nanosecond  Duration = 1
    Microsecond          = 1000 * Nanosecond
    Millisecond          = 1000 * Microsecond
    Second               = 1000 * Millisecond
    Minute               = 60 * Second
    Hour                 = 60 * Minute
)
```

This is one of those weird things in go due to its simplistic type system (lack of operator overloading in this case).
you have to cast the multiplication to Duration * Duration = Duration, instead of the original one which actually makes more sense: Duration * int = Duration
```go
seconds := 3600
time.Duration(seconds) * time.Second  // 3600s
```

- `func Now() Time`
- `func (t Time) Unix() int64`: ex `time.Now().Unix()`
- `func Since(t Time) Duration`:  shorthand for time.Now().Sub(t)
- `func Until(t Time) Duration`: shorthand for t.Sub(time.Now())

- `func After(d Duration) <-chan Time`: immediately returns a channel, and starts a new goroutine that sends a single value on that channel after the specified time.
- `func Tick(d Duration) <-chan Time`: 每经过duration, 产生一个Time, it behaves as if it creates a goroutinue that calls time.Sleep in a loop, sending an event each time it wakes up.
  ```go
  func main() {
    // ...create abort channel...
    fmt.Println("commencing countdown. Press return to abort.")
    tick := time.Tick(1 * time.Second)
    for countdown := 10; countdown > 0; countdown-- {
      fmt.Println(countdown)
      select {
        case <- tick:
          // Do nothing.
        case <- abort:
          fmt.Println("Launch aborted!")
          return
      }
    }
  }
  ```
  When the countdown function above returns, it stops receiving from tick, but the ticker goroutine is still there, try in vain to send on a channel from which no goroutine is receiving - a goroutine leak.
  The Tick function is convenient, but it's appropriate only when the tick will be needed throughout the lifetime of the application. Otherwise, we should use this pattern.
  ```go
  ticker := time.NewTicker(1 * time.Second)
  <- ticker.C  // receive from the ticker's channel
  ticker.Stop() // cause the ticker's goroutine to terminate
  ```

- `func Sleep(d Duration)`

- `func Parse(layout, value string) (Time, error)`

# flag
The non-flag arguments are available from `flag.Args()` as a slice of strings.

- `flag.Duration`, 可以解析诸如 50ms, 2m30s, 1.5h 这样的duration

# math
```go
var z float64
glog.Infoln(z, -z, 1/z, -1/z, z/z)  // output 0 -0 +Inf -Inf NaN
glog.Infoln(math.IsNaN(z / z))  // true
glog.Infoln(z/z == math.NaN())  // false, any comparison with NaN always yields false
```
但是直接写`1/0`就会编译错误.

# context
The same Context may be passed to functions running in different goroutines; Contexts are safe for simultaneous use by multiple goroutines.

