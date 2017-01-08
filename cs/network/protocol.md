数据包
```
datalink header: ip header: TCP header: DNS header: Payload
							UDP			HTTP
										...
```

# 介质访问控制子层
## Ethernet
EtherType is a two-octet field in an Ethernet frame. It is used to indicate which protocol is encapsulated in the payload of an Ethernet Frame.

EtherType numbering generally starts from 0x0800. In modern implementations of Ethernet, 
the field within the Ethernet frame used to describe the EtherType also can be used to represent the size of the payload of the Ethernet Frame.

Ethernet v2 framing considered these octets to represent EtherType 
while the original IEEE 802.3 framing considered these octets to represent the size of the payload in bytes.

EtherType values be greater than or equal to 1536 (0x0600).   
That value was chosen because the maximum length (MTU) of the data field of an Ethernet 802.3 frame was 1500 bytes (0x05DC). 
Thus, values of 1500 (0x05DC) and below for this field indicate that the field is used as the size of the payload of the Ethernet Frame 
while values of 1536 and above indicate that the field is used to represent EtherType. The interpretation of values 1501–1535, inclusive, is undefined.

[EtherType for some notable protocols](http://en.wikipedia.org/wiki/EtherType)

- 0x0800	Internet Protocol version 4 (IPv4)
- 0x86DD	Internet Protocol Version 6 (IPv6)

address family
```C
#define AF_INET 2
#define AF_INET6	10	/* IP version 6
```

# Internet layer
路由选择, 拥塞控制与网络互联

## IP v4
一种头部的定义方式
```C
struct ip{
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		unsigned int ip_hl:4;       /* header length */
		unsigned int ip_v:4;        /* version */
	#endif

	#if __BYTE_ORDER == __BIG_ENDIAN
		unsigned int ip_v:4;        /* version */
		unsigned int ip_hl:4;       /* header length */
	#endif

	u_int8_t ip_tos;            /* type of service */
	u_short ip_len;         /* total length */
	u_short ip_id;          /* identification */
	u_short ip_off;         /* fragment offset field */
	#define IP_RF 0x8000            /* reserved fragment flag */
	#define IP_DF 0x4000            /* dont fragment flag */
	#define IP_MF 0x2000            /* more fragments flag */
	#define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
	u_int8_t ip_ttl;            /* time to live */
	u_int8_t ip_p;          /* protocol */
	u_short ip_sum;         /* checksum */
	struct in_addr ip_src, ip_dst;  /* source and dest address */
};
```

另外一种头部的的定义方式
```C
/usr/include/netinet/ip.h
struct iphdr{
	#if __BYTE_ORDER == __LITTLE_ENDIAN
	    unsigned int ihl:4;
	    unsigned int version:4;
	#elif __BYTE_ORDER == __BIG_ENDIAN
	    unsigned int version:4;
	    unsigned int ihl:4;
	#else
		# error	"Please fix <bits/endian.h>"
	#endif

	u_int8_t tos;
	u_int16_t tot_len;
	u_int16_t id;
	u_int16_t frag_off;
	u_int8_t ttl;
	u_int8_t protocol;
	u_int16_t check;
	u_int32_t saddr;
	u_int32_t daddr;
};
```
	
- Internet Header Length (IHL): (4 bits) is the Internet Header Length (IHL)
- Total Length: 16-bit field defines the entire packet (fragment) size, including header and data, in bytes.

内网IP

- 10.0.0.0 ~ 10.255.255.255, 默认子网掩码:255.0.0.0
- 172.16.0.0 ~ 172.31.255.255, 默认子网掩码:255.255.0.0
- 192.168.0.0 ~ 192.168.255.255, 默认子网掩码:255.255.255.0

内网是可以上网的, 内网需要一台服务器或路由器做网关,通过它来上网  
做网关的服务器有一个网关(服务器/路由器)的IP地址,其它内网电脑的IP可根据它来随意设置,前提是IP前三个数要跟它一样,第四个可从0-255中任选但要跟服务器的IP不同

## IP v6
IPv6地址为128位长,但通常写作8组,每组为四个十六进制数的形式.例如: FE80:0000:0000:0000:AAAA:0000:00C2:0002

零压缩法: 如果几个连续段位的值都是0,那么这些0就可以简单的以::来表示,上述地址就可以写成 FE80::AAAA:0000:00C2:0002

这里要注意的是只能简化连续的段位的0,其前后的0都要保留,比如FE80的最后的这个0,不能被简化.
还有这个只能用一次,在上例中的AAAA后面的0000就不能再次简化.当然也可以在AAAA后面使用::,这样的话前面的12个0就不能压缩了.
这个限制的目的是为了能准确还原被压缩的0.不然就无法确定每个::代表了多少个0.

/usr/include/netinet/ip6.h

- IPv6 的分组头长度仅是IPv4 分组头部长度的2倍
- IPv4使用动态主机配置协议, 而IPv6支持地址自动配置
	
## ICMP
ICMP 是网络层协议

# Transport layer
The port number is an unsigned 16-bit integer, so the max is `2 ^ 16 -1 = 65535`.

## TCP
建立, 三次握手.
一个可能出现但是非常罕见的情况,两方同时打开,这是有四次握手, 双方同时发送SYN进入SYN-SENT状态, 当对方分别收到相应的SYN,在同时发出SYN+ACK,进入SYN+RECV状态,这样就包括了四次.

释放, 四次握手, 有时候也简化为三次握手

TCP差错控制

- 传输出错报文段(受损伤): 超时重传
- 丢失的报文段: 超时重传
- 重复的报文段: 丢弃重复的
- 乱序的报文段: 不确认, 知道收到以前的报文段
- 确认丢失: 累计确认

TCP计时器

- 重传计时器: 控制丢失的丢弃的报文段; 发送报文段时, 就启动该报文段的重传计时器; 仅仅待确认的报文段(ACK)不设置超时计时器,这就表示不重传这种报文段
- 坚持计时器: 处理零窗口大小通知; 当发送端的TCP收到一个零窗口的确认时, 就启动坚持计时器; 探测报文段
- 保持计时器keep-alive timer: 防止两个TCP之间的连接长期处于空闲状态, 服务器收到客户的信息后就将计时器复位
- 时间等待计时器TIME-WAIT: 释放连接时使用; 当被动释放连接的一方发出同意释放报文段时, 打开时间等待计时器

```C
/usr/include/netinet/tcp.h
struct tcphdr{
    u_int16_t th_sport;		/* source port */
    u_int16_t th_dport;		/* destination port */
    tcp_seq th_seq;		/* sequence number */
    tcp_seq th_ack;		/* acknowledgement number */

	#if __BYTE_ORDER == __LITTLE_ENDIAN
	    u_int8_t th_x2:4;		/* (unused) */
	    u_int8_t th_off:4;		/* data offset */
	#endif

	#if __BYTE_ORDER == __BIG_ENDIAN
	    u_int8_t th_off:4;		/* data offset */
	    u_int8_t th_x2:4;		/* (unused) */
	#endif

	u_int8_t th_flags;
	#define TH_FIN	0x01
	#define TH_SYN	0x02
	#define TH_RST	0x04
	#define TH_PUSH	0x08
	#define TH_ACK	0x10
	#define TH_URG	0x20

	u_int16_t th_win;		/* window */
	u_int16_t th_sum;		/* checksum */
	u_int16_t th_urp;		/* urgent pointer */
};
```
	
```
|----------------|----------------|-------------
|     source     |     dest       |
|----------------|----------------|
|               seq               |
|---------------------------------|
|               ack_seq           | 20 Bytes
|----|----|------|----------------|
|doff|res1|      |     window     |
|----|----|------|----------------|
|     check      |     urg_ptr    |
|----------------|----------------|-------------
|             options             | 4 Bytes
|---------------------------------|  
```

- doff(Data offset): TCP header length, 以4 Bytes(32位字)作为单位进行计量
- TCP flags(1个Byte, 前两个bit 为固定的0) 的顺序为: `urg, ack, psh, rst, syn, fin`共6个

Hex demo

TCP header(in hex)=05320017 00000001 000000000 500207FF 00000000  
since each hex = 4 bits , we need to first split the above hex as such
	
	05 32 00 17 00 00 00 01 00 00 00 00 50 02 07 FF 00 00 00 00

Analyse

	source port is 2 bytes take 05 32 = 1330
	next 2 bytes as destination address 00 17 == 23 (default TCP port)
	next 4 bytes as sequence number 00 00 00 01 ==1
	next 4 bytes as ack 00 00 00 00 == 0
	next 4 bits as haeder length 5 ==5 -- this indicates number of sets of 4 bytes which makes the header lenght = 20bytes..
	next 6 bits are reserved i.e.0 =0000and 2 bits from hex 0
	next 6 bits are control bits = remaining 2 bits from hex 0 and 4 bits of 2
	next 2 bytes indicate the window length 07 FF == 2047 bytes
	Checksum 2 bytes 00 00 = 0
	Urgent pointer 2bytes 00 00 =0

## UDP
```C
CFLAGS+=-D_BSD_SOURCE
/usr/include/netinet/udp.h
struct udphdr{
	u_int16_t uh_sport;		/* source port */
	u_int16_t uh_dport;		/* destination port */
	u_int16_t uh_ulen;		/* udp length */
	u_int16_t uh_sum;		/* udp checksum */
};
```

- Length: A field that specifies the length in bytes of the UDP header and UDP data.  
- The minimum length is 8 bytes since that is the length of the header

