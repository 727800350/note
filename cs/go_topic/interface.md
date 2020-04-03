# [interface](https://jordanorelli.com/post/32665860244/how-to-use-interfaces-in-go)
**duck typing**: When I see a bird that walks like a duck and swins like a duck and quacks like a duck, I call that bird a duck. – James Whitcomb Riley

## `interface{}`
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
one word is used to point to a method table for the value's underlying type, and the other word is used to point to the actual data being held by that value.

static type and dynamic type
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

