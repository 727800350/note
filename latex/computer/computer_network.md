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

## Internet Routing
- AS's run intra-domain routing protocols
	- Distance Vector, e.g., Routing Information Protocol (RIP)
	- Link State, e.g., Open Shortest Path First (OSPF)
- Between AS's runs inter-domain routing protocols
	- Border Gateway Routing (BGP)

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
