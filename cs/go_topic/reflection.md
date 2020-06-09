reflect pkg defines two important types, Type and Value.

A Type represents a Go type. It is an interface with many methods for discriminating among types and inspecting their components, like the fields of a struct or the parameters of a function.
The reflect.TypeOf function accepts any interface{} and returns its dynamic type as a reflect.Type

A reflect.Value can hold a value of any type.
The reflect.ValueOf function accepts any interface{} and returns a reflect.Value containing the interface's dynamic value.
As with reflect.TypeOf the results of reflect.ValueOf are always conrete, but a reflect.Value can hould interface values too.

Because reflect.TypeOf returns an interface values's dynamic type, it always returns a concrete type.
So, for example, the code below prints `*os.File`, not `io.Writer`
```go
var w io.Writer = os.Stdout
fmt.Printfln(reflect.TypeOf(w))  // *os.File
```
fmt.Printf provides a shorthand %T, that uses reflect.TypeOf internaly.

