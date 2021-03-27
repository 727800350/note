# [When nil isn't Equal to nil](https://www.calhoun.io/when-nil-isnt-equal-to-nil/)
Every pointer in Go has two basic pieces of information; the type of the pointer, and the value it points to. We will represent these as a pair like (type, value)

The fact that every pointer variable needs a type is why we can’t have a nil value assigned to a variable without declaring the type as well. That is, the following code will NOT compile.
```go
// This does not work because we do not know the type
n := nil
```

```go
var a *int = nil  // (*int, nil)
var b interface{} = nil  // (<nil>, nil)

fmt.Println("a == nil:", a == nil)  // true
fmt.Println("b == nil:", b == nil)  // true
fmt.Println("a == b:", a == b)  // false
```
The `<nil>` type is technically a type, and it satisfies the empty interface, so it is used when no other type information can be determined by the compiler.

**when we compare a variable to a hard-coded nil our compiler once again needs to determine what type to give that nil value**.
When this happens the compiler makes the same decision it would make if assigning nil to the variable.

对于上面的例子

- a == nil 实际比较的是 `(*int, nil) == (*int, nil)`
- b == nil `(<nil>, nil) == (<nil>, nil)`

```go
var a *int = nil
var b interface{} = a  // (*int, nil)

fmt.Println("a == nil:", a == nil)  // true
fmt.Println("b == nil:", b == nil)  // false
fmt.Println("a == b:", a == b)  // true
```

A nil interface value, which contains no value at all, is not the same as in interface value containning a pointer that happends to be nil.

[Why is nil not equal to nil in this example?](https://yourbasic.org/golang/gotcha-why-nil-error-not-equal-nil)
```go
func Foo() error {
  var err *os.PathError = nil
  // …
  return err
}

func main() {
  err := Foo()
  fmt.Println(err)        // <nil>
  fmt.Println(err == nil) // false
}
```
An interface value is equal to nil only if both its dynamic type and value are nil. In the example above, Foo() returns `(*os.PathError, nil)` and we compare it with `(<nil>, nil)`.

If we convert nil to the correct type, the values are indeed equal.
```go
fmt.Println(err == (*os.PathError)(nil)) // true
```

# Compiler type decisions can also be demonstrated with numbers
We saw how nil can be coerced into the correct type by the compiler, but this isn’t actually the only situation where the compiler makes type decisions like this.
For instance, when you assign hard-coded numbers to a variable the compiler will decide which type should be used based on the context of the program.
```go
var a int = 12  // (int, 12)
var b float64 = 12  // (float64, 12)
var c interface{} = a  // (int, 12)

fmt.Println("a==12:", a == 12)  // true
fmt.Println("b==12:", b == 12)  // true
fmt.Println("c==12:", c == 12)  // true
fmt.Println("a==c:", a == c)  // true
fmt.Println("b==c:", b == c)  // false
```

Another interesting note specific to numbers is that when comparing 12 to an interface, the compiler will always coerce it into an int. 这也是为什么上面的 c == 12 判定为true 的原因.
This is similar to how nil gets coerced into `(<nil>, nil)` when compared to an interface.

```go
var b float64 = 12  // (float64, 12)
var c interface{} = b  // (float64, 12)

fmt.Println("c==12:", c == 12)  // false
fmt.Printf("hard-coded=(%T,%v)\n", 12, 12)  // (int, 12)
```

# summary
When we compare hard-coded values with variables the compiler has to assume they have some specific type and follows some set of rules to make this happen.

If you find yourself working with various types that can all be assigned to nil, one common technique for avoiding issue is to explicitly assign things to nil. That is, instead of a = b write:
```go
var a *int = nil
var b interface{}

if a == nil {
  b = nil
}
```

