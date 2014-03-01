#### 网络流量管理的目的
1. 能够清楚知道网络流量的种类
1. 知道网络带宽的详细使用情况:何人/何地/做什么/使用多少
1. 明确了解哪里是整个网络的潜在瓶颈,何处有异常 
1. 通过长期的数据采集和分析,掌握整个网络的流量分布和趋势,进行相关的预测,进而指导未来的网络规划

#### 流量管理的步骤
- 定义目标
	1. 找出"杀手"级应用(killer Applications)
	1. 确定流量模型(TOP N用户,协议)
	1. 什么地方有问题?
	1. 确定是内部还是外部的问题, etc
- 实施测量
	1. 部署测量 (netflow, sflow, SNMP, 网络探针probe)
	1. 部署应用程序
	1. 数据采集
- 分析结果
	1. 找出TOP N（10、100、1000、···）用户、协议
	1. 找出80/20的流量占用规律
	1. 比较当前的流量与昨天、上周、上个月、···的区别
	1. 瓶颈分析
	1. VLAN, 子网划分, 路由/交换分析
	1. 网络流量增长趋势
- 改进方案
	1. 调整网络结构和拓扑
	1.  定义新的路由/交换规则
	1.  实施QoS
	1.  进行负载均衡, etc.
- 管理控制
	1. 调整计费规则
	1. 行政管理手段, etc.

#### 网络流量检测的方法
- 基于SNMP
	1. MRTG
	1. HP OpenView (RMON, RMONII - **共享以太网时代的技术，已经过时**)
- 基于抓包分析
	+ 可以对数据包进行详细的7层分析
	+ 重点在协议分析，而非用户流量访问行为统计
	+ 无法应对大流量(>100Mbps)数据分析
	+ 仅能在短时间内对流经接口的数据包进行分析
	+ Ex: Sniffer, Ethereal
- 基于网络探针: 
	+ 可深入的对网络2层和3层特性进行详细分析
	+ 信息流复制, 然后按需提取信息
	+ Ex: Agilent NetMetrix and Probes, NetScout nGenius Performance Manager and nGenius Probes. 
- 基于Flow
	+ 可适应长期的流量数据采集和分析
	+ 可适应大网络流量环境下的流量分析
	+ 为当前和未来的流量分析技术发展趋势
	+ Ex: NetFlow, sFlow(sFlow资源(CPU, 内存, 带宽)占用比NetFlow 少很多)

#### 流量控制技术
- MPLS流量工程(Traffic Engineering, TE)
	+ 路由器等网络设备自身实现
	+ DiffServ (Differentiated Service)
- TCP整形技术(Traffic Shaping)
	+ 控制信息源发送量和时间
	+ 主动动态调控
	+ 带宽分区
		- 为某类信息流在总带宽内划定虚拟管道
		- 提供信息流带宽总用量上/下限带宽限制
	+ 策略设置
		- 每种应用、用户最大/最小带宽策略
		- 每对会话（session）最大/最小带宽策略
		- 基于优先级策略


### NetFlow
<img src="http://clipart-finder.com/data/png/juanjo_Router.png" alt="Router 图标" title="Router 图标" width="200" />

NetFlow Architecture
<img src="http://upload.wikimedia.org/wikipedia/commons/9/9e/NetFlow_Architecture_2012.png" alt="NetFlow Architecture" title="NetFlow Architecture" width="500" />

谁在什么时间、在什么地方、使用何种协议、访问谁、具体的流量是多少等问题。

+ 扩大到了基于流的字节数和报文数计数, 流一般由5元组确定. 而**SNMP 只能够识别到设备的物理端口**
+ 由7 个字段来识别一个流: 源和目的IP, 源和目的端口, 协议类型, ToS(DSCP), **输入/输出的逻辑端口 (input/output interface index)**
+ Flows通过UDP（缺省端口2055）报文输出
+ DDOS Monitoring
- 单向Flow还是双向Flow
- 双向Flow可包含更多的信息，如数据包的往返时间，TCP的有关行为
- 应用(Application) flows：根据报文的包头信息区分应用的种类
- 聚合的(Aggregated) Flow ：将若干条flow按照一定的规则合成一条flow，即 flows of flows.

#### BGP ROUTING and AS-PATH
The Internet may be partitioned into independent **Autonomous Systems (AS)**, typically containing a collection of routers under common administration. 
_Routers within the same AS run the same routing algorithm_. Routing in the Internet can be divided into internal, or inside an AS, and external, or among ASes. 
The commonly used protocol for exchanging external AS routing information is the **Border Gateway Protocol (BGP)**. BGP is a path-vector protocol that operates at the level of address blocks, or AS prefixes. Each **AS prefix** consists of a 32-bit address and a mask length, e.g., 192.0.2.0/24 consists of addresses from 192.0.2.0 to 192.0.2.255.  
用于BGP路由中的每个自治系统都被分配一个唯一的自治系统编号（ASN), 对BGP来说，因为ASN是区别整个相互连接的网络中的各个网络的唯一标识，所以这个自治系统编号非常重要.  
[Exploring Autonomous System Numbers](http://www.cisco.com/web/about/ac123/ac147/archived_issues/ipj_9-1/autonomous_system_numbers.html)

An AS-PATH is a sequence of intermediate ASes between source and destination routers that form a directed route for packets to travel. Neighboring ASes use BGP to exchange update messages about how to reach different AS prefixes. After each router makes a new local decision on the best route to a destination, it will send that route, or path information, along with accompanying distance metrics and path attributes, to each of its peers. As this information travels through the network, each router along the path prepends its unique AS number to a list of ASes in the BGP message. This list is the route's AS-PATH. An AS-PATH in conjunction with an AS prefix provides a specific handle for a one-way transit route through the network.

### Application Identification
#### Network-Based Application Recognition (NBAR)
Network Based Application Recognition (NBAR) is the mechanism used by some Cisco routers and switches to recognize a dataflow by inspecting some packets sent.
- Validation or reclassification of ToS marking based on Deep & Stateful Packet Inspection
- Protocol Discovery with application statistics
- User-Defined Custom Application Classification
- Real-time loadable PDLMs could provide rapid-update mechanism for new signatures

### 用户行为分析
- TOP N用户的流量统计（发送、接收, 端口, 协议种类), 从哪里来, 到何处去, 分别使用何种协议（应用）?
- 占用网络带宽的TOP N种应用是什么？各占多少百分比？
- 哪些站点的访问量最高？
- 用户使用网络多长时间？
- 用户的使用模式是什么？
- 工作时间和非工作时间的流量模式各是什么？


#### RADIUS 
NAS网络接入服务器(Network Access Server)
远端用户拨入验证服务(RADIUS, Remote Authentication Dial In User Service)是一个AAA(authentication, authorization and accounting)协议.  
意思就是同时兼顾验证(authentication),授权(authorization)及计费(accounting)三种服务的一种网络传输协议(protocol).  
通常用于网络存取,或流动IP服务,适用于局域网及漫游服务.
NAS服务器和RADIUS 服务器通过UDP 协议进行通信, 双方使用不经网络传输的共享密钥

# Words
- type of service (ToS)
- DDOS(Distributed Denial of service):很多DOS攻击源一起攻击某台服务器就组成了DDOS攻击
