报文结构各个字段的含义及大小参见:
[DNS Message Header and Question Section Format](http://www.tcpipguide.com/free/t_DNSMessageHeaderandQuestionSectionFormat.htm)

![dnsheaderformat](http://www.tcpipguide.com/free/diagrams/dnsheaderformat.png)

## ![报文](http://1811.img.pp.sohu.com.cn/images/blog/2012/5/28/15/27/e6055747_138545c4ff8g213.gif)
![报文](http://i.imgbox.com/U26Avc6h.gif)

[DNS报文结构](http://zhaotao110.blog.sohu.com/218341780.html)

[DNS报文](http://1862.img.pp.sohu.com.cn/images/blog/2012/5/28/14/11/e6055747_13854174a5fg214.gif)

[查询问题](http://1881.img.pp.sohu.com.cn/images/blog/2012/5/28/14/15/e6055747_138541ad28dg214.gif)

[回答,授权,额外](http://1821.img.pp.sohu.com.cn/images/blog/2012/5/28/14/16/e6055747_138541bc6e8g214.gif)

## 字段说明

(1) DNS报文

- 标识: 占16位,唯一匹配查询和应答
- 标志: 占16位,说明报文类型
- 问题数, 回答资源记录数, 授权资源记录数, 额外资源记录数:占16位,其中问题数>=1
- 查询问题: 位数不定
- 回答资源记录, 授权资源记录, 额外资源记录: 位数不定

(2) 标志

- QR(query or response): 占1位,0:查询,1:响应
- opcode: 占4位,0:标准查询,1:反向查询,2:服务器状态查询,3: reserved, 4:primary server tells secondary servers to Update DNS, 5:allows resource records to be added, deleted or updated selectively
- AA(authority answer): 占1位,0:非授权,1:授权回答
- TC(truncated): 占1位,0:未截断,1:截断
- RD(Recursive Desire): 占1位,0:非递归,1:递归
- RA(Recursive Available): 占1位,0:不提供递归,1:提供递归
- rcode: 占4位,0:无差错,3:名字差错...

(3) 查询问题

- 查询名: 位数不定,以点为分隔符,分别计数,以0结尾,可用指针压缩,无需填充
- 查询类型: 占16位,A, NS, CNAME, PTR等
- 查询类: 占16位,通常为1,表示互联网地址

(4) 回答,授权,额外

- 域名: 位数不定,存储格式同上
- 类型: 占16位,同上
- 类: 占16位,同上
- 生存时间: 占32位,资源记录在缓存中保留的时间,单位为秒
- 资源数据长度: 占16位,以字节为单位,依类型而定
- 资源数据: 位数不定,依类型而定

## 数据结构

	/*DNS header*/
	struct dnshdr
	{
	    unsigned short int id;         /* identifier*/
	
	    unsigned char  rd:1;           /* recursion desired */
	    unsigned char  tc:1;           /* truncated message */
	    unsigned char  aa:1;           /* authoritive answer */
	    unsigned char  opcode:4;       /* purpose of message */
	    unsigned char  qr:1;           /* response flag */
	
	    unsigned char  rcode:4;        /* response code */
	    unsigned char  unused:2;       /* unused bits */
	    unsigned char  pr:1;           /* primary server required (non standard) */
	    unsigned char  ra:1;           /* recursion available */
	
	    unsigned short int que_num;    /* question number*/
	    unsigned short int rep_num;    /* respones number*/
	    unsigned short int num_rr;     /* authority answer number*/
	    unsigned short int num_rrsup;  /* additional answer number*/
	};


# [DOMAIN NAMES - CONCEPTS AND FACILITIES](http://www.ietf.org/rfc/rfc1034.txt)
Each node has a label, which is zero to 63 octets in length.

One label is reserved, and that is the null (i.e., zero length) label used for the root.

**The domain name of a node is the list of the labels on the path from the node to the root of the tree**.  
By convention, the labels that compose a domain name are printed or read left to right, from the most specific (lowest, farthest from the root) to the least specific (highest, closest
to the root).

hostname is a special type of domain name which identifies Internet hosts.  
Host name to address mappings were maintained by the Network Information Center (NIC) in a single file (HOSTS.TXT)  
host names a subset of the usual text representation for domain names.

## Name syntax
The following syntax will result in fewer problems with many
applications that use domain names (e.g., mail, TELNET).

	<domain> ::= <subdomain> | " "
	
	<subdomain> ::= <label> | <subdomain> "." <label>
	
	<label> ::= <letter> [ [ <ldh-str> ] <let-dig> ]
	
	<ldh-str> ::= <let-dig-hyp> | <let-dig-hyp> <ldh-str>
	
	<let-dig-hyp> ::= <let-dig> | "-"
	
	<let-dig> ::= <letter> | <digit>
	
	<letter> ::= any one of the 52 alphabetic characters A through Z in
	upper case and a through z in lower case
	
	<digit> ::= any one of the ten digits 0 through 9

## Resource Records
**A domain name identifies a node**.  Each node has a set of resource
information, which may be empty.  The set of resource information
associated with a particular name is composed of separate resource
records (RRs).  The order of RRs in a set is not significant, and need
not be preserved by name servers, resolvers, or other parts of the DNS.

When we talk about a specific RR, we assume it has the following:

- **owner**           which is the domain name where the RR is found.

- **type**            which is an encoded 16 bit value that specifies the type of the resource in this resource record.  Types refer to abstract resources.  
	This memo uses the following types:
	- **A**:            a host address
	- **CNAME**:        identifies the canonical name of an alias
	- **HINFO**         identifies the CPU and OS used by a host
	- **MX**            identifies a mail exchange for the domain.  See [RFC-974 for details.
	- **NS** 			the authoritative name server for the domain
	- **PTR** 			a pointer to another part of the domain name space
	- **SOA** 			identifies the start of a zone of authority

- **class**  which is an encoded 16 bit value which identifies a protocol family or instance of a protocol.  
	This memo uses the following classes:
	- **IN**              the Internet system
	- **CH**              the Chaos system

- **TTL**       which is the time to live of the RR.  This field is a 32 bit integer in units of seconds, an is primarily used by resolvers when they cache RRs.  The TTL describes how long a RR can be cached before it should be discarded.

- **RDATA**     which is the type and sometimes class dependent data which describes the resource:

	- **A**               For the IN class, a 32 bit IP address For the CH class, a domain name followed by a 16 bit octal Chaos address.
	- **CNAME**           a domain name.
	- **MX**              a 16 bit preference value (lower is better) followed by a host name willing to act as a mail exchange for the owner domain.
	- **NS**              a host name.
	- **PTR**             a domain name.
	- **SOA**             several fields.

For example, we might show the RRs carried in a message as:

    ISI.EDU.        MX      10 VENERA.ISI.EDU.
                    MX      10 VAXA.ISI.EDU.
    VENERA.ISI.EDU. A       128.9.0.32
                    A       10.1.0.52
    VAXA.ISI.EDU.   A       10.2.0.27
                    A       128.9.0.33

The MX RRs have an RDATA section which consists of a 16 bit number
followed by a domain name.  The address RRs use a standard IP address
format to contain a 32 bit internet address.

This example shows six RRs, with two RRs at each of three domain names.

Similarly we might see:

    XX.LCS.MIT.EDU. IN      A       10.0.0.44
                    CH      A       MIT.EDU. 2420

This example shows two addresses for XX.LCS.MIT.EDU, each of a different
class.

## Queries
In the Internet, queries are carried in UDP datagrams or over
TCP connections.

For example, a mailer tying to send mail to `Mockapetris@ISI.EDU` might
ask the resolver for mail information about `ISI.EDU`, resulting in a
query for `QNAME=ISI.EDU, QTYPE=MX, QCLASS=IN`.  

The response's answer section would be:

    ISI.EDU.        MX      10 VENERA.ISI.EDU.
                    MX      10 VAXA.ISI.EDU.

while the additional section might be:

    VAXA.ISI.EDU.   A       10.2.0.27
                    A       128.9.0.33
    VENERA.ISI.EDU. A       10.1.0.52
                    A       128.9.0.32

Because the server assumes that if the requester wants mail exchange
information, it will probably want the addresses of the mail exchanges
soon afterward.

# [Clarifications to the DNS Specification](http://www.ietf.org/rfc/rfc2181.txt)
**Server Reply Source Address Selection**  
   Some multi-homed hosts running DNS servers generate a reply using a
   source address that is not the same as the destination address from
   the client's request packet.  Such replies will be discarded by the
   client because the source address of the reply does not match that of
   a host to which the client sent the original request.  That is, it
   appears to be an unsolicited response.
   
**Port Number Selection**  
   Replies to all queries must be directed to the port from which they
   were sent.  When queries are received via TCP this is an inherent
   part of the transport protocol.  For queries received by UDP the
   server must take note of the source port and use that as the
   destination port in the response.
   
**Resource Record Sets**  
   Each DNS Resource Record (RR) has a label, class, type, and data.  It
   is meaningless for two records to ever have label, class, type and
   data all equal - servers should suppress such duplicates if
   encountered.  It is however possible for most record types to exist
   with the same label, class and type, but with different data.  Such a
   group of records is hereby defined to be a Resource Record Set
   (RRSet).
   
**Sending RRs from an RRSet**  
   A query for a specific (or non-specific) label, class, and type, will
   always return all records in the associated RRSet - whether that be
   one or more RRs.  The response must be marked as "truncated" if the
   entire RRSet will not fit in the response.
   
**Receiving RRSets**  
   Servers must never merge RRs from a response with RRs in their cache
   to form an RRSet.     
   When considering whether to accept an RRSet in a reply, or retain an
   RRSet already in its cache instead, a server should consider the
   relative likely trustworthiness of the various data.
   
**Name syntax**  
   Occasionally it is assumed that the Domain Name System serves only
   the purpose of mapping Internet host names to data, and mapping
   Internet addresses to host names.  This is not correct, the DNS is a
   general (if somewhat limited) hierarchical database, and can store
   almost any kind of data, for almost any purpose.
   
   That one restriction
   relates to the length of the label and the full name.  The length of
   any one label is limited to between 1 and 63 octets.  A full domain
   name is limited to 255 octets (including the separators).  The zero
   length full name is defined as representing the root of the DNS tree,
   and is typically written and displayed as "."

# Other
DNS uses its own unique **encoding** for Unicode characters.   
For example:  
`www.á.com`: How the DNS name may appear in, say, a web page  
`www.xn--1ca.com`: What the browser converts the name to when sending it as a DNS request "over the wire"

The reason why DNS uses a complicated special encoding for non-ASCII Unicode characters instead of simply using UTF-8 is because RFC1035 section 2.3.1 says that names can only have letters, numbers, and the '-' (hyphen) character. I felt, a decade ago, that it would have been possible to add real UTF-8 support to DNS, but there was too much anxiety that doing so would break unspecified legacy DNS routers, appliances, etc.

# DNS 相关协议
## NETBIOS
NETBIOS(Network Basic Input/Output System)协议是由IBM公司开发,主要用于数十台计算机的小型局域网.

该协议是一种在局域网上的程序可以使用的应用程序编程接口(API),为程序提供了请求低级服务的统一的命令集,作用是为了给局域网提供网络以及其他特殊功能.  
系统可以利用WINS服务,广播及Lmhost文件等多种模式将NetBIOS名--特指基于NETBIOS协议获得计算机名称-解析为相应IP地址,实现信息通讯,所以在局域网内部使用NetBIOS协议可以方便地实现消息通信及资源的共享.因为它占用系统资源少,传输效率高,所以几乎所有的局域网都是在NetBIOS协议的基础上工作的.

不管使用哪一种传输方式,NetBIOS提供三种不同的服务:

- 名字服务:名字登记和解析
- 会话服务:可靠的基于连接的通信
- 数据包服务:不可靠的无连接通信

## LLMNR
在DNS 服务器不可用时,DNS 客户端计算机可以使用本地链路多播名称解析 (LLMNR—Link-Local Multicast Name Resolution)(也称为多播 DNS 或 mDNS)来解析本地网段上的名称.例如,如果路由器出现故障,从网络上的所有 DNS 服务器切断了子网,则支持 LLMNR 的子网上的客户端可以继续在对等基础上解析名称,直到网络连接还原为止.

LLMNR工作过程

1. 主机在自己的内部名称缓存中查询名称.如果在缓存中没有找到了名称,那么主机就会向自己配置的主DNS服务器发送查询请求.如果主机没有收到回应或收到了错误信息,主机还会尝试搜索配置的备用DNS服务器.如果主机没有配置DNS服务器,或者如果在连接DNS服务器的时候没有遇到错误但失败了,那么名称解析会失败,并转为使用LLMNR.
2. 主机通过用户数据报协议(UDP)发送多播查询,查询主机名对应的IP地址,这个查询会被限制在本地子网(也就是所谓的链路局部)内.
1. 链路局部范围内每台支持LLMNR,并且被配置为响应传入查询的主机在收到这个查询请求后,会将被查询的名称和自己的主机名进行比较.如果没有找到匹配的主机名,那么计算机就会丢弃这个查询.如果找到了匹配的主机名,这台计算机会传输一条包含了自己IP地址的单播信息给请求该查询的主机
