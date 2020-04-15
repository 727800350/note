**duck typing**: When I see a bird that walks like a duck and swins like a duck and quacks like a duck, I call that bird a duck. – James Whitcomb Riley

[How to use interfaces in Go](https://jordanorelli.com/post/32665860244/how-to-use-interfaces-in-go)

The `interface{}` type, the empty interface, is the source of much confusion. The `interface{}` type is the interface that has no methods.
So all types satisfy the empty interface.
That means that if you write a function that takes an interface{} value as a parameter, you can supply that function with any value. So, this function:
```go
func DoSomething(v interface{}) {
  // ...
}
```
will accept any parameter.

An interface value is constructed of two words of data;

- one word is used to point to a method table for the value's underlying type, that is the dynamic type
- and the other word is used to point to the actual data being held by that value.

Copying an interface value makes a copy of the thing stored in the interface value.
If the interface value holds a struct, copying the interface value makes a copy of the struct.
If the interface value holds a pointer, copying the interface value makes a copy of the pointer, but again not the data it points to.

# static type and dynamic type
```go
var foo interface{}  // foo is of static type interface{}

foo {
  value: nil
  type: nil
}

// foo is of dynamic type float64
foo = 3.14
foo {
  value: 3.14
  type: float64
}

// foo is of dynamic type *Person
foo = &Person{}
foo {
  value: 0x11face04
  type: *Person
}
```

## type switch
```go
func Println(x interface{}) {
  switch x.(type) {
  case bool:
    fmt.Println(x.(bool))
  case int:
    fmt.Println(x.(int))
  default:
    fmt.Println("unkown type")
  }
}
```
the clause `x.(type)` is only valid in type switch

# Why doesn't type T satisfy the Equal interface?
Consider this simple interface to represent an object that can compare itself with another value:
```go
type Equaler interface {
  Equal(Equaler) bool
}
```
and this type, T:
```go
// does not satisfy Equaler
type T int
func (t T) Equal(u T) bool {
  return t == u
}
```
Unlike the analogous situation in some polymorphic type systems, T does not implement Equaler.
The argument type of T.Equal is T, not literally the required type Equaler.

In Go, the type system does not promote the argument of Equal; that is the programmer's responsibility, as illustrated by the type T2, which does implement Equaler:
```go
// satisfies Equaler
type T2 int
func (t T2) Equal(u Equaler) bool {
  return t == u.(T2)
}
```
Even this isn't like other type systems, though, because in Go any type that satisfies Equaler could be passed as the argument to T2.Equal, and at run time we must check that the argument is of type T2.
Some languages arrange to make that guarantee at compile time.

# Can I convert a []T to an []interface{}?
Not directly. It is disallowed by the language specification because the two types do not have the same representation in memory.
It is necessary to copy the elements individually to the destination slice. This example converts a slice of int to a slice of interface{}:
```go
t := []int{1, 2, 3, 4}
s := make([]interface{}, len(t))
for i, v := range t {
  s[i] = v
}
```

# nil
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
An interface value is equal to nil only if both its dynamic type and value are nil. In the example above, Foo() returns `[*os.PathError, nil]` and we compare it with `[nil, nil]`.

If we convert nil to the correct type, the values are indeed equal.
```go
fmt.Println(err == (*os.PathError)(nil)) // true
```

