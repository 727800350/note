# strings
- `func Join(a []string, sep string) string`
- `func Split(s, sep string) []string`
- HasPrefix, HasSuffix

# strconv
- `func Atoi(s string) (int, error)`: Atoi is equivalent to ParseInt(s, 10, 0), converted to type int.
- `func ParseInt(s string, base int, bitSize int) (i int64, err error)`

# os
- `os.Args`: a slice of strings

# io
- `func Copy(dst Writer, src Reader) (written int64, err error)`: implemented by calling src.WriteTo(dst) if src implements the WriterTo interface, Otherwise by calling dst.ReadFrom(src), if dst implements the ReaderFrom interface.

# bufio
bufio pkg helps make input and output efficient and convenient.

- `func NewScanner(r io.Reader) *Scanner`
  ```go
  in := bufio.NewScanner(os.stdin)
  for in.Scan() {
    glog.Infoln(in.Text())  // Text 会把行尾的\n 自动去掉
  }
  ```

# ioutil
- `Discard`: is an io.Writer on which all Write calls succeed without doing anything.

- `func readAll(r io.Reader, capacity int64) (b []byte, err error)`
- `func ReadFile(filename string) ([]byte, error)`
- `func WriteFile(filename string, data []byte, perm os.FileMode) error`

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

- `func Now() Time`
- `func (t Time) Unix() int64`: ex `time.Now().Unix()`
- `func Since(t Time) Duration`:  shorthand for time.Now().Sub(t)
- `func Until(t Time) Duration`: shorthand for t.Sub(time.Now())

- `func After(d Duration) <-chan Time`: After waits for the duration to elapse and then sends the current time on the returned channel.
- `func Tick(d Duration) <-chan Time`: 每经过duration, 产生一个Time
- `func Sleep(d Duration)`

# flag
The non-flag arguments are available from `flag.Args()` as a slice of strings.

# context
The same Context may be passed to functions running in different goroutines; Contexts are safe for simultaneous use by multiple goroutines.

