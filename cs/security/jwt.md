jwt 即 JSON Web Token,是一种认证协议,一般用来校验请求的身份信息和身份权限.

- [什么是 JWT -- JSON WEB TOKEN](https://www.jianshu.com/p/576dbf44b2ae)
- [jwt生成的token,应该存在哪里?](https://juejin.cn/post/6977280069437751310)

# 起源
## 传统的session认证
先来说说需要服务端存储的操作,即传统的session会话的做法.
首先要做用户登陆,先要在服务端维护一个登陆表,这个登陆表可以放在缓存里,也可以放进数据库里.
当用户登陆的时候,把用户信息写入这个登陆表,然后导出一个登陆id,也就是所谓的session,把这个session返回给客户的,让客户端下次
请求把这个信息带上来.

对于前端的小伙伴来说,这个过程通常无感知,后端的老哥们使用一个叫set-cookie的http头字段,自己把数据写入浏览器cookie里了.然后
请求的时候,浏览器又会自己把cookie写进请求头里面.

当客户端请求进入服务端时,服务端拿到cookie里面的session,然后到登陆表里面去查用户信息,校验用户权限,然后即可完成正常的业务
交互.

## 基于session认证所显露的问题
Session: 每个用户经过我们的应用认证之后,我们的应用都要在服务端做一次记录,以方便用户下次请求的鉴别,通常而言session都是保
存在内存中,而随着认证用户的增多,服务端的开销会明显增大.

扩展性: 用户认证之后,服务端做认证记录,如果认证的记录被保存在内存中的话,这意味着用户下次请求还必须要请求在这台服务器上,这
样才能拿到授权的资源,这样在分布式的应用上,相应的限制了负载均衡器的能力.这也意味着限制了应用的扩展能力.

CSRF: 因为是基于cookie来进行用户识别的, cookie如果被截获,用户就会很容易受到跨站请求伪造的攻击.

## 基于token的鉴权机制
基于token的鉴权机制类似于http协议也是无状态的,它不需要在服务端去保留用户的认证信息或者会话信息.这就意味着基于token认证机
制的应用不需要去考虑用户在哪一台服务器登录了,这就为应用的扩展提供了便利.

流程上是这样的:

1. 用户使用用户名密码来请求服务器
1. 服务器进行验证用户的信息
1. 服务器通过验证发送给用户一个token
1. 客户端存储token,并在每次请求时附送上这个token值
1. 服务端验证token值,并返回数据

这个token必须要在每次请求时传递给服务端,它应该保存在请求头里, 另外,服务端要支持CORS(跨来源资源共享)策略,一般我们在服务端
这么做就可以了`Access-Control-Allow-Origin: *`.

# jwt
[Introduction to JSON Web Tokens](https://jwt.io/introduction)

JSON Web Tokens consist of three parts separated by dots (.), which are:

- Header
- Payload
- Signature

Therefore, a JWT typically looks like the following.
```plain
xxxxx.yyyyy.zzzzz
```

