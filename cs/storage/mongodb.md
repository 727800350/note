mongodb 是文档数据库,其很多概念可以与 RDBMS 一一对应

| RDMS     | mongodb    |
| ---      | ---        |
| database | database   |
| table    | collection |
| row      | document   |
| column   | field      |

ObjectId

ObjectId is auto generated when we insert any new document.

TODO: 是client 产生还是server 产生?
从mongo 有提供lib 来让client 生成这一点来看, 应该是由client 生成的, 但是也有可能client 和 server 都可以.

The ObjectId value is a 12-byte value consists of

- 前4个字节表示时间戳: 是文档创建时的时间,它是递增的,可以保证ObjectId总体递增的顺序.
- 接着3位是机器识别码: 也就是在同一台机器中,每次生成ObjectId中,这三个字节总相等
- 紧接2位着是进程id的值
- a 3-byte incrementing counter, initialized to a random value

This is by design, to make it unique across all the databases in the world so one ObjectId will be unique to that
document only.

[Collection Methods](https://docs.mongodb.com/manual/reference/method/js-collection/)

db.collection.insertOne(doc)
doc 可以是一个json 结构, 例如
```json
{
  "name": "xxx"
}
```
也可以把key 的双引号省略掉
```
{
  name: "xxx"
}
```
建议都按照标准json 的格式来.

