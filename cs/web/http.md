[When should you use Path Variable, and how about Query Parameter?](
https://medium.com/@fullsour/when-should-you-use-path-variable-and-query-parameter-a346790e8a6d)

As a best practice, almost of developers are recommending following way. If you want to identify a resource, you should
use Path Variable. But if you want to sort or filter items, then you should use query parameter. So, for example you can
define like this:

- `/users`: Fetch a list of users
- `/users?occupation=programer`: Fetch a list of programer user
- `/users/123`: Fetch a user who has id 123

# [RESTful API 设计指南](http://www.ruanyifeng.com/blog/2014/05/restful_api.html)

HTTP动词
对于资源的具体操作类型,由HTTP动词表示.
常用的HTTP动词有下面五个(括号里是对应的SQL命令).

- GET(SELECT): 从服务器取出资源(一项或多项).
- POST(CREATE): 在服务器新建一个资源.
- PUT(UPDATE): 在服务器更新资源(客户端提供改变后的完整资源).
- PATCH(UPDATE): 在服务器更新资源(客户端提供改变的属性).
- DELETE(DELETE): 从服务器删除资源

下面是一些例子.

- GET /zoos: 列出所有动物园
- POST /zoos: 新建一个动物园
- GET /zoos/ID: 获取某个指定动物园的信息
- PUT /zoos/ID: 更新某个指定动物园的信息(提供该动物园的全部信息)
- PATCH /zoos/ID: 更新某个指定动物园的信息(提供该动物园的部分信息)
- DELETE /zoos/ID: 删除某个动物园
- GET /zoos/ID/animals: 列出某个指定动物园的所有动物
- DELETE /zoos/ID/animals/ID: 删除某个指定动物园的指定动物

标准的http code 可以表示的错误类型太有限, 不要使用标准的http code, 统一返回200, 然后在response 中专门也有一个字段用来标
志业务错误码. 一般返回的数据长下面这样:
```json
{
  "code": 0,
  "message": "",
  "data": {
  }
}
```
