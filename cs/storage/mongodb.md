# intro
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

MongoDB 内部采用 B-Tree 作为索引结构,此索引基于最左优先原则,且必须保证查询顺序与索引字段的顺序一致才有效.

# api
查询的字段必须在document 中存在, 如果要查不存在的, 需要使用 $exists operator.
例如, 查询所有不存在name 字段的记录
```mongodb
db.users.find({name: {$exists: false}});
```

## operator
### query
#### Evaluation Query Operators
[$regex](https://docs.mongodb.com/manual/reference/operator/query/regex/)
```mongodb
{ <field>: { $regex: /pattern/, $options: '<options>' } }
{ <field>: { $regex: 'pattern', $options: '<options>' } }
{ <field>: { $regex: /pattern/<options> } }
```

## collection
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

## go
bson tag 为 `bson:"-"` 时, replaceOne 和 insertOne 等方法保存到mongodb 中时, 这个tag 的字段不会保存到mongodb 中.

```go
type Document struct {
  ID         primitive.ObjectID `json:"id" bson:"_id,omitempty"`
  Name       string             `json:"name" bson:"name"`
  Closed     bool               `json:"closed" bson:"closed"`
  CreateTime int64              `json:"create_time" bson:"create_time"`
  ModifyTime int64              `json:"modify_time" bson:"modify_time"`
}
```
`_id` 后面的 omitempty 是必需的, 这样在insertOne 的时候, 默认的ObjectID 才会被忽略掉, 从而mongodb 会生成一个新的, 否则就
用的是默认的ObjectID (primitive.NilObjectID) 作为`_id`

