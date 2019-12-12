We also say that UDP provides a connectionless service, as there need not be any long-term relationship between a UDP client and server. 
For example, a UDP client can create a socket and send a datagram to a given server and then immediately send another datagram on the same socket to a different server. 
Similarly, a UDP server can receive several datagrams on a single UDP socket, each from a different client.

![UDP客户/服务器程序所用的套接字函数](http://pic002.cnblogs.com/images/2012/367190/2012081121141279.jpg)  
如上图所示, 客户不与服务器建立连接, 而是只管使用`sendto`函数给服务器发送数据报, 其中必须指定目的地(即服务器)地址作为参数. 
类似的, 服务器不接受来自客户的连接, 而是只管调用`recvfrom` 函数, 等待来自某个客户的数据到达.
recvfrom将接收到的数据与client 的地址一并返回, 因此服务器可以把响应发送给正确的客户.

写一个长度为0 的数据报是可行的. 在UDP情况下, 这会形成一个只包含一个IP首部和一个UDP首部而没有数据的IP数据报. 这也意味着对于UDP协议, recvfrom返回0 值是可接受的.
他并不像TCP套接字上read 返回0值那样表示对端已关闭连接. 既然UDP是无连接的, 因此也没有诸如关闭一个UDP连接之类的事情.

大多数TCP服务器是并发的, 而大多数UDP服务器是迭代的

由于UDP面向无连接,它可以随时发送数据.再加上UDP本身的处理既简单又高效,因此经常用于以下几个方面:

1. 包总量较少的通信(DNS,SNMP等)
2. 视频,音频等多媒体通信(即时通信)
3. 限定于LAN等特定网络中的应用通信
4. 广播通信(广播,多播)

可能有人会认为,鉴于TCP是可靠的传输协议,那么它一定优于UDP.其实不然.TCP与UDP的优缺点无法简单地,绝对地去做比较.那么,对这两种协议应该如何加以区分使用呢?下面,我就对此问题做一简单说明.

- TCP用于在传输层有必要实现可靠的情况.由于它是面向有连接并具备顺序控制,重发控制等机制的,所以它可以为应用提供可靠传输.
- 而UDP主要用于那些对高速传输和实时性有较高要求的通信或广播通信. 我们拿通过IP电话进行通话作为例子. 如果使用TCP,数据在传送途中如果丢失会重发,但这样无法流畅地传输通话人的声音,会导致无法进行正常交流.
	而采用UDP,它不会进行重发处理.从而也就不会有声音大幅度延迟到达的问题.即使有部分数据丢失,也只是会影响某一小部分的通话.
  此外,在多播与广播通信中也使用UDP而不是TCP.RIP,DHCP等基于广播的协议也要依赖于UDP.因此,TCP和UDP应该根据应用的目的按需使用.

