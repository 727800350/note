默认的 JSON 只支持以下几种 Go 类型:

- bool for JSON booleans
- float64 for JSON numbers
- string for JSON strings
- nil for JSON null

在 Go 中并不是所有的类型都能进行序列化

- JSON object key 只支持 string
- channel, complex, function 等 type 无法进行序列化
- 数据中如果存在循环引用, 则不能进行序列化, 因为序列化时会进行递归
- pointers are dereferenced before the JSON is encoded. In other words, it's as if you used the real value, not a pointer.
- 只有 struct 中支持导出的 field 才能被 JSON package 序列化, 即首字母大写的 field.

# Struct Tag
## 指定 JSON filed name
JSON object 中的 name 一般都是小写,我们可以通过 struct tag 来实现:
```go
type MyStruct struct {
    SomeField string `json:"some_field"`
}
```
SomeField 序列化之后会变成 `some_field`.

## 指定 field 是 empty 时的行为
使用 omitempty 可以告诉 Marshal 函数如果 field 的值是对应类型的 zero-value,那么序列化之后的 JSON object 中不包含此 field:
```go
type MyStruct struct {
    SomeField string `json:"some_field,omitempty"`
}

m := MyStruct{}
b, err := json.Marshal(m) //{}
```
如果 SomeField == "" ,序列化之后的对象就是 {}.

## 跳过 field
Struct tag "-" 表示跳过指定的 filed:
```go
type MyStruct struct {
    SomeField string `json:"some_field"`
    Passwd string `json:"-"`
}

m := MyStruct{}
b, err := json.Marshal(m) //{"some_feild":""}
```
即序列化的时候不输出,这样可以有效保护需要保护的字段不被序列化.

## Numbers
Javascript only has one numeric type float64
```go
type MyStruct struct {
    Int64String int64 `json:",string"`
}
```

# 自定义序列化函数
Go JSON package 中定了两个 Interface Marshaler 和 Unmarshaler ,实现这两个 Interface 可以让你定义的 type 支持序列化操作.

