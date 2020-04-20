# strconv
- `func Atoi(s string) (int, error)`: Atoi is equivalent to ParseInt(s, 10, 0), converted to type int.
- `func ParseInt(s string, base int, bitSize int) (i int64, err error)`

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

# context
The same Context may be passed to functions running in different goroutines; Contexts are safe for simultaneous use by multiple goroutines.

