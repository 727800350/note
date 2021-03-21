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
- Internet Header Length (IHL): 4 bits
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

- IPv6 的分组头长度仅是IPv4 分组头部长度的2倍
- IPv4使用动态主机配置协议, 而IPv6支持地址自动配置
	
## ICMP
ICMP 是网络层协议

# Transport layer
The port number is an unsigned 16-bit integer, so the max is `2 ^ 16 -1 = 65535`.

## TCP
```plain
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
```plain
source port is 2 bytes take 05 32 = 1330
next 2 bytes as destination address 00 17 == 23 (default TCP port)
next 4 bytes as sequence number 00 00 00 01 ==1
next 4 bytes as ack 00 00 00 00 == 0
next 4 bits as haeder length 5 ==5 -- this indicates number of sets of 4 bytes which makes the header lenght = 20bytes.
next 6 bits are reserved i.e.0 =0000and 2 bits from hex 0
next 6 bits are control bits = remaining 2 bits from hex 0 and 4 bits of 2
next 2 bytes indicate the window length 07 FF == 2047 bytes
Checksum 2 bytes 00 00 = 0
Urgent pointer 2bytes 00 00 =0
```

## UDP
- Length: A field that specifies the length in bytes of the UDP header and UDP data.  
- The minimum length is 8 bytes since that is the length of the header

