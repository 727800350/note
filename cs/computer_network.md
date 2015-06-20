# Introduction
Computer network

- Node: (Data processing, Communication protocol) Machine, Human, Communication entity in different networks
- Link: (data transmission, capacity transmission) Physical link, Virtual link, Different method of communication

Network: Organization and Management

Well structured material(hardware and software) to realize communication between computer

- Hardware (End device, transport media)
- Software (transmission function, control function)
- In structured

Routing hierarchical

- Tier‐1s ISP: National and international converge
- Tier‐2s ISP: Smaller converge, national or regional
- Tier‐3s ISP: Last hop access network and Closest to end systems

Standard organization

- ITU ‐international telecommunication union
- ISO ‐ international standard organization
- IEEE ‐the institute of electrical and electronics engineers
- IETF‐international engineer task force
- ...

网络类型

- circuit switching 电路交换: 独享
- message switching 报文交换: 没有建立物理连接, 每一个结点接收整个报文,检查目标结点地址,然后根据网络中的交通情况在适当的时候转发到下一个结点.经过多次的存储-转发,最后到达目标,因而这样的网络叫存储-转发网络.其中的交换结点要有足够大的存储空间(一般是磁盘),用以缓冲收到的长报文.
- packet switching 分组交换: 共享, 固定大小上限的分组

ATM 交换机是 circuit switching

Ad-hoc: multi-hop, infrastructure less, 节点之间可以相互通信, 并可以进行路由, 适用于战场的复杂的环境

不同层的消息叫法

- Application: message
- Transport: segment
- Network: datagram
- Data link: frame

## 体系结构
**OSI参考模型**

OSI是Open System Interconnect的缩写,意为开放式系统互联.国际标准化组织(ISO)制定了OSI模型.
这个模型把网络通信的工作分为7层,分别是**物理层,数据链路层,网络层,传输层,会话层,表示层和应用层**


|具体7层|数据格式|功能与连接方式|典型设备|
|-------|--------|--------------|--------|
|应用层 Application    |  -        |    网络服务与使用者应用程序间的一个接口    |    网关    |
|表示层 Presentation    |  -         |    数据表示,数据安全,数据压缩    |    -     |
|会话层 Session    |       -    |    建立,管理和终止会话    |       -  |
|传输层 Transport    |    数据组织成数据段Segment    |    用一个寻址机制来标识一个特定的应用程序(端口号)    | -        |
|网络层 Network    |    分割和重新组合数据包Packet    |    基于网络层地址(IP地址)进行不同网络系统间的路径选择    |    路由器    |
|数据链路层 Data Link    |    将比特信息封装成数据帧Frame    |    在物理层上建立,撤销,标识逻辑链接和链路复用以及差错校验等功能.通过使用接收系统的硬件地址或物理地址来寻址    |    网桥(广播风暴),交换机,网卡    |
|物理层 Physical    |    传输比特(bit)流    |    建立,维护和取消物理连接    |    光纤,同轴电缆,双绞线,中继器和集线器hub(冲突域)    |

**TCP/IP 参考模型**

- 应用层: 包括了所有的高层协议, 并且总是不断有新的协议加入
- 传输层
- 网络层
- 介质访问层

**一种建议的参考模型**

结合了OSI和TCP/IP的优点, 上课采用的参考模型
 
应用层 传输层 网络层 数据链路层 物理层

# Application layer
Application architecture

- C/S
- P2P
- Hybrid
- SOA (Service oriented architecture): 是构造分布式计算的应用程序的方法.它将应用程序功能作为服务发送给最终用户或者其他服务. 
它采用开放标准,与软件资源进行交互并采用表示的标准方式.
	- Based on the principle of maximum reusing software resources
	- Software resources is redefined as "services"
	- Create service logic
	- Provide the possibility of access this services in an order, workflow 流经的所有节点组成一个整体
	- Web service is one type of implementation

Web Cache

- Client Side:  Proxy
- Server Side:  Cache and  Server farmer

FTP: 两个TCP连接

1. control port 21
1. data port 20
当多个文件下载时, 可以并行下载

DNS server
- 3 classes
- Root
- TLD(top level DNS server)
- Authority DNS server

POP3: 主动将client 的数据推送到服务端

DHCP: 用广播

# Transport layer
## TCP
建立, 三次握手  
打开连接时,一个可能出现但是非常罕见的情况,两方同时打开,这是有四次握手,双方同时发送SYN进入SYN-SENT状态,当对方分别收到相应的SYN,在同时发出SYN+ACK,进入SYN+RECV状态,这样就包括了四次.

释放, 四次握手, 有时候也简化为三次握手

TCP差错控制

- 传输出错报文段(受损伤): 超时重传
- 丢失的报文段: 超时重传
- 重复的报文段: 丢弃重复的
- 乱序的报文段: 不确认, 知道收到以前的报文段
- 确认丢失: 累计确认

TCP计时器

- 重传计时器  
	控制丢失的丢弃的报文段  
	发送报文段时, 就启动该报文段的重传计时器  
	仅仅待确认的报文段(ACK)不设置超时计时器,这就表示不重传这种报文段  
- 坚持计时器  
	处理零窗口大小通知  
	当发送端的TCP收到一个零窗口的确认时, 就启动坚持计时器  
	探测报文段  
- 保持计时器keep-alive timer  
	防止两个TCP之间的连接长期处于空闲状态  
	服务器收到客户的信息后就将计时器复位  
- 时间等待计时器TIME-WAIT  
	释放连接时使用  
	当被动释放连接的一方发出同意释放报文段时, 打开时间等待计时器

# Network layer
Network layer services

- Host to host communication
	- Virtual circuit (ATM, FR): 里面一定有信号系统(类似于电信网络的信令), 路由表是临时建立的
	- Datagram (Internet)
- Forwarding
	- transfer a packet from an incoming link to an outgoing link
within a single router
- Routing
	- Network layer determine the route or path taken by packets as they flow from a sender to a receiver

广播

- 直接广播地址  
主机号全为1, 例如C类地址201.1.16.255, 路由器将目的地址为直接广播地址的分组以广播方式发送到网络号为201.1.16.0中的所有主机
- 受限广播地址  
网络号与主机号全为1, 将分组发送该网络中的所有主机, 路由器将阻挡该分组通过
- 这个网络上的特定主机地址  
网络号全为0, 主机号为确定的值, 如0.0.20.125. 该分组将被限制在该网络内部, 由特定主机号的(20.125)对应的主机接受该分组

NAT
网络地址转换(NAT,Network Address Translation)

## Internet Routing
- AS's run intra-domain routing protocols
	- Distance Vector, e.g., Routing Information Protocol (RIP)  
		基于Bellman-Foed算法的距离矢量路由协议
		一个自治系统内的路由器都要与同一个系统内的其他路由器交换路由表信息, 因此适用于规模较小的自治系统
		从一个路由器到达下一跳路由器的距离为1. 主机直接连到路由器, 距离为0
	- Link State, e.g., Open Shortest Path First (OSPF)  
		在OSPF协议中,当链路状态发生变化时要用Flooding向所有路由器发送信息.
		此算法不要求维护网络的拓扑结构和相关的路由计算,仅要求接收到信息的节点以广播方式转发数据包.
- Between AS's runs inter-domain routing protocols
	- Border Gateway Routing (BGP)  
		BGP发言人

## IP protocol
地址解析

- ARP address resolution protocol IP地址 -> MAC地址
- RRP reverse resolution protocol MAC地址 ->IP地址
- ARP高速缓存表

## ICMP protocol
Internal control message protocol
ICMP的差错与查询, 控制功能对于保证IP协议的可靠运行是至关重要的
ICMP报文不是直接传送给数据链路层, 而是要封装成IP数据报, 然后才传送到数据链路层

# Datalink layer
Data Link Layer -- Services

- Providing a well-defined services' interface to the upper layer- network.
- Provide necessary services for data transmission over link between two adjacent nodes
	- Unacknowledged connectionless services (Most LAN)
	- Acknowledged connectionless service(wireless network)
	- Acknowledged connection-oriented service(e.g PPP)
- Control the transmission flow
- Dealing with the transmission errors

Two types of "links":

- point-to-point
	- PPP for dial-up access
	- point-to-point link between Ethernet switch and host
- broadcast (shared wire or medium)
	- traditional Ethernet
	- upstream HFC(Hybrid Fiber Coaxial)
	- 802.11 wireless LAN

# Multiple Access protocols
Medium access control MAC 介质访问控制  
介质访问控制方法是指控制多个结点利用公共传输介质和接收数据的方法.  
介质访问控制方法是所有"共享介质"类型局域网都必须解决的问题:

1. 哪个结点发送数据
1. 发送时是否会出现冲突
1. 出现冲突时如何处理


Three broad types:

- Channel Partitioning
	- FDMA, TDMA, CDMA
	- Divide the physical transport into distinct pieces,
	- Allocate piece to node for exclusive use
- Random Access
	- Share the same channel by multiple nodes
	- Channel not divided, allow collisions
	- "recover" from collisions
	- E.g 802.3, 802.11 ...
- "Taking turns"
	- With a central coordinator
	- Tightly coordinate shared access to avoid collisions

Examples of random access MAC protocols:

- ALOHA
- Slotted ALOHA
- CSMA(Carrier Sense Multiple Access), CSMA/CD(With Collision Detection), CSMA/CA(with collision avoidance)

有人将CSMA/CD的工作过程形象的比喻成很多人在一间黑屋子中举行讨论会,参加会议的人都是只能听到其他人的声音.
每个人在说话前必须先倾听,只有等会场安静下来后,他才能够发言.
人们将发言前监听以确定是否已有人在发言的动作成为"载波侦听",将在会场安静的情况下每人都有平等机会讲话成为"多路访问",
如果有两人或两人以上同时说话,大家就无法听清其中任何一人的发言,这种情况称为发生"冲突".
发言人在发言过程中要及时发现是否发生冲突,这个动作称为"冲突检测".
如果发言人发现冲突已经发生,这时他需要停止讲话,然后随机后退延迟,再次重复上述过程,直至讲话成功.
如果失败次数太多,他也许就放弃这次发言的想法.通常尝试16次后放弃.

CSMA/CD媒体访问控制方法的工作原理,可以概括如下:
```
先听后说,边听边说,
一旦冲突,立即停说,
等待时机,然后再说,
注:"听",即监听,检测之意,"说",即发送数据之意.
```

1. 载波侦听过程  
Ethernet的物理层规定发送的数据采用曼彻斯特编码方式, 如果总线上已经有数据在传输, 总线的电平将会按曼彻斯特编码规律出现跳变, 则可以判定此时为"总线忙".  
反之, 总线的电平将不发生跳变, 可以判定此时为"总线空闲".  
如果一个结点已经准备好发送的数据帧, 并且总线此时处于空闲状态, 泽这个结点就可以启动发送

2. 冲突检测  
有一种极端的情况, 节点A向节点B发送了数据, 在数据信号正好快要到达B的时候, 节点B也发送了数据, 此时冲突发生. 等到冲突的信号传送回到A, 已经经过了两倍的时间延迟2t, 其中t=D/V, D为总线长度, V为传播速度.  
如果超过两倍的传播延迟2t 时间没有检测出冲突, 就能肯定该节点已经取得总线访问权, 因此人们将2D/V 定义为冲突窗口(collision window)


"Taking Turns" MAC protocols

- Polling:
	- master node "invites" slave nodes to transmit in turn
	- Bluetooth, 802.15
- Token passing:
	- No master node exist
	- control token passed from one node to next sequentially
	- FDDI(Fiber Distributed Data Interface), IEEE 802.5
	- 缺点: 拿到token的节点down了怎么办? token 的伪造

MAC地址也有被用来作为路由的, 例如在SDN中

Gbits: 用户主机与switch 直接联系

## 网络装置
### Repeater 中继器
- 工作在物理层
- 模拟设备, 用于连接两根电缆段, 在一段上出现的信号被放大后放到另外一个段上
- 只理解电压值

### Hub 集线器
- 集线器有许多条互联的输入线路
- 在任何一条线路上到达的帧都将被发送到所有其他的线路上, 如果两帧同时到达, 他们将冲突, 就好像他们在同一根同轴电缆一样
- 连接到同一个Hub上的所有线路必须运行在同样的速度上
- 整个集线器构成了一个**冲突域**

### Bridge
- 运行在数据链路层
- 连接不同的LAN, 将数据从一个lan转发到另外一个lan中, 而不同的lan可以采用不同的协议
- 网桥工作在混杂模式下
- 根据其内部的一张大的散列表进行"路由", 逆向学习算法
- 隔离冲突域
- 存在广播风暴的问题

### Switch
- 运行在数据链路层
- 通常被用来连接独立的计算机
- 直通型交换(cut-through switch): 不等包的完整到达, 就转发了
- examines frame header and **selectively** forwards frame based on MAC dest address
- plug-and-play, self-learning: Switch do not need to be configured

**三层交换**  
综合路由器和交换机功能的产品, 即三层交换机  
将第二层的交换(高性能和强大的网络流量转发能力)和第三层的路由功能(具有网络可伸缩性)结合起来, 在集成一些特殊的服务面形成的.  
用硬件来实现转发, 而路由器是通过CPU 来进行  
流交换: 路由一次, 交换多次(生活实例: 旅行团, 路由器是对每个人都检票, 而三层交换机只对导游验票)  
把IP地址封装到了帧里面

### Router
- 运行在网络层
- 彻底的解决广播风暴

### 传输网关
将两台使用了不同的面向连接传输协议的计算机连接起来  
例如, 一个计算机使用TCP/IP, 另一台计算机使用ATM, 则传输网关可以将他们连接起来, 对传输的分组进行重新格式化

### 应用网关
应用网关理解数据的格式和内容, 并将消息从一个格式翻译为另外一种格式  
例如, 电子邮件网关可以将Internet消息翻译为移动电话的SMS消息

# wireless 802.11
CRC校验  
MAC protocol: CSMA/CA(Carrier Sense Multiple Access/Collision Derect)  
发的时候不侦听, 需要进行确认, 而802.3(以太网)是边发边听

802.11 collision avoidance

- RTS, request to send 发送这个包的node请求预留资源
- CTS, clear to send 发送这个包的node宣告资源预留成功

无线网络资源相对于有限资源更加宝贵, 但是速度还更加慢  
路由器能够自动设置自己的信号覆盖范围.

# cellular network 蜂窝网
每个蜂窝单元使用一组频率, 并且这组频率不会被他的任何一个邻居使用.  
蜂窝网络之所以比以前的系统有更大的容量, 关键在于它使用了相对较小的蜂窝单元, 并且在相近(但不相邻)的蜂窝单元中重复使用相同的传输频率. 蜂窝单元越小, 容量增加越多. 此外, 蜂窝单元越小, 意味着所需要的功率越小, 因而发射器和手持机也越小, 越便宜.  
当由于运动赛事, 音乐会等活动, 大量的人聚集在一起时, 运营商可以使用可移动的塔(塔上有卫星链路), 建立临时的微蜂窝单元.

- TDMA: Time Division Multiple Access 时分
- FDMA: Frequency Division Multiple Access 频分
- CDMA: Code Division Multiple Access 码分
- SDMA: Space Division Multiple Access 空分
在移动通信中,能实现空间分割的基本技术就是采用自适应阵列天线,在不同的用户方向上形成不同的波束

不同的小区用不同的频率; 上行频率与下行频率也不同; 之后再进行时分.

360度全向天线  
BTS: 基站

- GSM, 2G, 通过信令网络进行短信的发送
- GPRS, 2.5G, 增加了网关
- 3G, packet switching(用于信息) 与 circuit switching(用于语音) 共存
- 4G, ALl IP, 也就是只有packet switching

越区切换: 用户的手机进入到不同的蜂窝网络中不同的cellular时, 通话不能中断.  
用户位置感知与资源预留

漫游: 身份认证信息, 用户相关信息都存储在当地(手机号的注册地), 这样当在非注册地使用网络时, 在进行身份认证时, 需要向用户的手机号注册地请求数据, 然后进行比对. 所以成本会高一点, 但是仍然比现在的漫游费低很多很多倍.

认证, 加密, 完整性, 访问控制(一般通过防火墙来进行)

# 大作业
## Tor 洋葱路由
从目录服务器获取一个list of nodes  
层层加密, 要发送信息的节点进行加密操作. 例如A要发送信息M到B, 目录服务器随机选择的一条路径为1,2,3. 则信息的顺序为
```
A -> 1 -> 2-> 3-> B
```
那么, 加密时, 

1. A首先使用3的公钥K3对M进行加密成K(K3, M), 
1. 然后再用2的公钥K2进行加密成为K(K2, K(K3, M)), 
1. 最后使用1的公钥进行加密成为K(K1, K(K2, K(K3, M)))

这样就形成了一个洋葱的结构, 一层包一层
