<link rel="stylesheet" href="http://yandex.st/highlightjs/6.2/styles/googlecode.min.css"> 
<script src="http://code.jquery.com/jquery-1.7.2.min.js"></script>
<script src="http://yandex.st/highlightjs/6.2/highlight.min.js"></script> 
<script>hljs.initHighlightingOnLoad();</script>
<script type="text/javascript">
 $(document).ready(function(){
      $("h1,h2,h3,h4,h5,h6").each(function(i,item){
        var tag = $(item).get(0).localName;
        $(item).attr("id","wow"+i);
        $("#category").append('<a class="new'+tag+'" href="#wow'+i+'">'+$(this).text()+'</a></br>');
        $(".newh1").css("margin-left",0);
        $(".newh2").css("margin-left",20);
        $(".newh3").css("margin-left",40);
        $(".newh4").css("margin-left",60);
        $(".newh5").css("margin-left",80);
        $(".newh6").css("margin-left",100);
      });
 });
</script>
<div id="category"></div>

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
	1. 找出TOP N(10,100,1000, ...)用户,协议
	1. 找出80/20的流量占用规律
	1. 比较当前的流量与昨天,上周,上个月,...的区别
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

#### 网络流量监测的方法
监测的方法有传统统计方法(如利用SNMP, RMON), Flow based 方法(如NetFlow), Packet/Content Based 方法及分布式数据采集和关联分析方法等.

- **传统统计方法**测量信息直接来自网络设备, 只包括字节数, 报文数量等最基本的内容, 不适用于流量检测要求较高的场合.  
- **Flow Based 方法**利用路由器已有的能力进行数据采集, 以传输层端口定义业务\todo{这个地方对吗?}, 与真实情况有较大的误差.  
- **Packet/Content Based 方法**使用专用采集设备, 采用链路分光或镜像的方式采集原始报文, 能够对业务进行深入的识别和分析.  
- **分布式数据采集和关联分析方法**, 对网络中多个采集点数据进行汇总和关联, 提供全面的分析能力.

业务流量分析的方法是针对流来分析, 具体方法有: 基于不同流定义数据的过滤, 汇聚和关联; 分析特定指标按照时间的变化情况(趋势分析); 在特定条件下, 对于某个指标进行排名; 对相同或不同时段, 指标, 趋势等的比较(比较分析); 多个指标间关联(关联分析)等.

具体分类方法

- 基于SNMP
	+ SNMP 只能够识别到设备的物理端口(**SNMP采集的数据是基于端口的,无法提供端到端的准确流量信息,因此对流向的统计手段不明确**)
	+ MRTG, HP OpenView (RMON, RMONII - **共享以太网时代的技术,已经过时**)
- 基于抓包分析
	+ 可以对数据包进行详细的7层分析
	+ 重点在协议分析,而非用户流量访问行为统计
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

主动测量就是通过向网络、服务器或应用发送测试流量，以获取与这些对象相关的性能指标。例如，可以向网络发送数据包并不断提高发送速率直至网络饱和，以此来测量网络的最大负载能力。

_mib2, rmon, rmon2, netflow, sflow_等技术就是帮助我们分析网络的流量, 区别在于获得的数据内容丰富程度不一样.

+ mib2和rmon只提供 osi 2层信息,mrtg,solarwind等主要使用这些信息
+ netflow提供的osi 3－4 的信息, 无法提供2层数据, 没有原始包信息,  无法进行协议解码诊断
+ sflow宣称提供3－7层信息,但实现的设备很少,主要就是foundry, extreme.
+ rmon2提供的信息最丰富,包括osi 1－7层,但是需要额外产品的支持才能实现,其实sniffer,wildpacket,netscout就是rmon2的实现,只不过wildpackets还支持rmon1的信息读取,而sniffer不行,netscout除了支持mib2,rmon,rmon2(需要专用硬件实 现),还支持netflow.

至于rmon2和netflow/sflow技术那种更好,有很多的争论,关键是要看在什么环境下,什么需求下,才能说哪种技术比较适合.没有最好的,只有最适合的.

#### 网络流量监控硬件
高密度现场可编程逻辑器件, 包括复杂可编程逻辑器件CPLD(Complex Programable Logic Device)和现场可编程门阵列FPGA(field-programmable gate array), 能够将大量逻辑功能集中在一个单片集成电路(Integrate Circuit, IC)中, 也使具有高速网络流量监控功能的硬件实现成为可能.

流量监控硬件一般采用超大规模集成电路芯片, 实现流数据的特征提取(流识别, 分类), 快速查表, 统计计数, 数据缓存, 流质量分析记忆流控策略. 同时, 基于硬件结构的报文转发引擎, 可提供双向线速转发, 流量监控过程不影响网络性能.

**网络处理器**  
传统的基于GPP(general purpose processor)的网络设备只能满足灵活性的要求; 而基于ASIC(application specific integrated circuit专用集成电路)的网络设备只满足高性能要求.  
具有可编程性, 强大处理能力的网络处理器(NP, network processor). 同时具有GPP的灵活性和ASIC的高速处理能力.

特点  
并行处理: 包括线程级的并行和处理器级的并行  
深层数据处理: 也叫智能处理, 根据不同的服务要求对数据包进行不同深度的处理.

**内容可寻址存储器**Content addressable memory(CAM): 硬件过滤, 快速查找

基于**哈希技术**的流测量和管理算法是网络监控领域的一种关键技术.

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
		- 每种应用,用户最大/最小带宽策略
		- 每对会话(session)最大/最小带宽策略
		- 基于优先级策略


### NetFlow
<img src="http://clipart-finder.com/data/png/juanjo_Router.png" alt="Router 图标" title="Router 图标" width="200" />

NetFlow Architecture
<img src="http://upload.wikimedia.org/wikipedia/commons/9/9e/NetFlow_Architecture_2012.png" alt="NetFlow Architecture" title="NetFlow Architecture" width="500" />

- flow exporter,即能够吐出flow record的设备或者吐出flow信息的源，最基本的是Cisco路由器。或者是通过采packet生成流的probe(也叫路由器仿真器)，例如NetFlow exporter。
- flow collector，即能够接受这些吐出来的flow record信息(例如netflow-tool)，压缩归档保存为流文件。
- flow analysis,即能够分析这些数据源，得出相关的重要信息，例如netflow-scan，并保存(如flow-scan输出分析结果到RRD数据库)。
- reporter(web portal),即能够对分析后的数据，进行图表展示，例如rrd-tool。

谁在什么时间,在什么地方,使用何种协议,访问谁,具体的流量是多少等问题.
who, what, where, when, and how

+ 扩大到了基于流的字节数和报文数计数, 流一般由5元组确定. 而**SNMP 只能够识别到设备的物理端口**
+ 由7 个字段来识别一个流: 源和目的IP, 源和目的端口, 协议类型, ToS(DSCP), **输入/输出的逻辑端口 (input/output interface index)**
+ Flows通过UDP(缺省端口2055)报文输出
+ DDOS Monitoring
- 单向Flow还是双向Flow
- 双向Flow可包含更多的信息,如数据包的往返时间,TCP的有关行为
- 应用(Application) flows:根据报文的包头信息区分应用的种类
- 聚合的(Aggregated) Flow :将若干条flow按照一定的规则合成一条flow,即 flows of flows.

#### 采样模式
三种:确定式采样,定时采样和随机采样.  
确定式采样固定的每N个报文采第N个,N可以指定.随机采样则每N个报文随机采其中一个.定时采样固定的每N毫秒采一个.一般的,随机采样是最佳选择.

#### BGP ROUTING and AS-PATH
The Internet may be partitioned into independent **Autonomous Systems (AS)**, typically containing a collection of routers under common administration. 
_Routers within the same AS run the same routing algorithm_. Routing in the Internet can be divided into internal, or inside an AS, and external, or among ASes. 
The commonly used protocol for exchanging external AS routing information is the **Border Gateway Protocol (BGP)**. BGP is a path-vector protocol that operates at the level of address blocks, or AS prefixes. Each **AS prefix** consists of a 32-bit address and a mask length, e.g., 192.0.2.0/24 consists of addresses from 192.0.2.0 to 192.0.2.255.  
用于BGP路由中的每个自治系统都被分配一个唯一的自治系统编号(ASN), 对BGP来说,因为ASN是区别整个相互连接的网络中的各个网络的唯一标识,所以这个自治系统编号非常重要.  
[Exploring Autonomous System Numbers](http://www.cisco.com/web/about/ac123/ac147/archived_issues/ipj_9-1/autonomous_system_numbers.html)

An AS-PATH is a sequence of intermediate ASes between source and destination routers that form a directed route for packets to travel. Neighboring ASes use BGP to exchange update messages about how to reach different AS prefixes. After each router makes a new local decision on the best route to a destination, it will send that route, or path information, along with accompanying distance metrics and path attributes, to each of its peers. As this information travels through the network, each router along the path prepends its unique AS number to a list of ASes in the BGP message. This list is the route's AS-PATH. An AS-PATH in conjunction with an AS prefix provides a specific handle for a one-way transit route through the network.

### sFlow
- 线速运行的"永远在线"技术,可以将sFlow技术嵌入到网络路由器和交换机ASIC芯片中
- sFlow使用两种独立的采样方法来获取数据: 针对交换数据流的基于数据包统计采样方法和基于时间采样的针对网络接口统计数据(类似RMON的轮询).
- sFlow还能使用不同的采样率,对交换机或仅对其中一些端口实施监视
- 非完全基于流的技术(不保留流的状态信息); 而NetFlow为完全基于流的技术(比如两个包同属于相同的流,必须得统计在一个流纪录,在输出流之前必须保留流的状态.)

### IPFIX 
Internet  Protocol Flow Information eXport
网络中流量统计信息的格式趋于标准化
IPFIX偏向于使用流控制传输协议(SCTP)作为其传输层协议,但同时也允许使用传输控制协议(TCP)和用户数据报协议(UDP)(基于Netflow V9)

### Application Identification
#### 业务识别方法
1. 基于端口口
2. 基于协议解码: 必须了解业务协议编码和协议过程; 需要维护业务连接状态, 资源消耗大; 对于加密协议难以分析
3. 基于业务签名: 业务签名难以总结
4. 基于传输层模式的业务识别: 仅对业务传输层连接进行分析, 而不分析报文内容. 根据分析对象的不同, 可分为**报文级别的分析**(协议类型, 报文长度, 报文到达间隔等), **流级别的分析**(流到达间隔, 流持续时间, 流量等)和**流之间特征的分析**(流并发数目, 流之间的关系).&nbsp;&nbsp;&nbsp;&nbsp;  随着数据挖掘技术的发展, 开始将**机器学习**中的算法, 如主成分分析和密度估计, 最大期望算法等引入到流量识别的研究当中, 将其与基于流的传输层识别方法相结合
4. 基于行为模式: 如在通信过程中所使用的协议个数, 连接的主机数目, 连接的端口数, 连接到的IP地址, 信令包与数据包的比例等. 例如, P2P业务一般连接到的IP地址个数和端口个数相近. 非P2P流量同时具有比较高的连接成功率和连接响应成功率, 而P2P 流量即使有一项很高, 另外一项也不高.

#### 传统P2P应用识别与控制方法

- 归纳出常用的一个或多个固定端口(如KuGoo软件通用的商业端口是7000). 但是现在大多数P2P软件都不再使用固定端口，或使用动态端口，或在软件中设有端口设置功能供用户自行设置端口，甚至有的P2P软件使用8O等其它业务的固定端口号，以欺骗流量检测设备。
- 用关键字进行P2P流量识别. 对于利用关键字识别P2P流量，关键字符串的部分或全部字节可能随软件的运行环境、版本等的改变而改变。
- 就是利用大于1024的TCP/UDP端口数进行P2P识别的技术。但是，该方法无法将P2P业务和端口扫描、DdoS攻击等类型的流量区分开来。
- 利用IP地址连接的通信对端IP地址的数量进行P2P识别的技术。但是该方法无法区分一个IP地址是否属于一个为众多用户提供服务器的设备，还是一个用户在大量发起P2P连接。

因此综上所述，传统的P2P应用识别技术已经不能应对P2P技术本身的发展和变化了。

#### BMC P2P应用识别与控制技术概述
一种基于数学建模技术应用于P2P网络的流量识别与控制的方法, 通过对P2P技术特征的分析，得出典型的P2P流量模型，通过**模型特征值**的提取，能够非常高效便捷的对它们进行识别。  
对网络中的每一个IP地址的通信会话进行数学建模，采集该IP地址活动会话所连接的对端IP地址分布、TCP/UDP端口分布和会话状态信息，利用数学建模技术产生数学模型特征值；将数学模型特征值与预设的P2P流量数学模型参数进行匹配.  
上述预设的P2P流量数学模型参数包括最小IP地址扩散度Cip、最小TCP/UDP端口扩散度Mport、最大会话接通率Cratemax和最小会话接通率Cratemin，上述四个参数被配置到数学模型库中。  
本技术优于传统P2P识别技术之处在于:

1. 本技术不依赖于对报文的内容进行关键字或者特征码识别，因此，本技术能够对加密的P2P和未知的新出现的P2P流量进行识别；
2. 本技术不是仅仅依靠对高于1024的TCP/UDP端口进行统计，识别P2P，它避免了把P2P流量和网络扫描、DdoS攻击等流量混淆起来。因此更精确；
3. 本技术不是仅仅依靠对IP地址所连接的对端IP地址数量进行统计，因此避免了把P2P流量和网站服务器的流量混淆。

#### Network-Based Application Recognition (NBAR)
Network Based Application Recognition (NBAR) is the mechanism used by some Cisco routers and switches to recognize a dataflow by inspecting some packets sent.
- Validation or reclassification of ToS marking based on Deep & Stateful Packet Inspection
- Protocol Discovery with application statistics
- User-Defined Custom Application Classification
- Real-time loadable PDLMs could provide rapid-update mechanism for new signatures

### 用户行为分析
- TOP N用户的流量统计(发送,接收, 端口, 协议种类), 从哪里来, 到何处去, 分别使用何种协议(应用)?
- 占用网络带宽的TOP N种应用是什么?各占多少百分比?
- 哪些站点的访问量最高?
- 用户使用网络多长时间?
- 用户的使用模式是什么?
- 工作时间和非工作时间的流量模式各是什么?

常用工具:  
SPSS, SAS; PostgreSQL(对象-关系型数据库); Matlab; 
WEKA(开源, Java实现. 集合了大量能承担数据挖掘任务的机器学习算法, 包括对数据进行预处理, 分类, 回归, 聚类, 关联规则分析等)

### 网络业务分析
IP网络对业务提供网络层(IP层) 和传输层(TCP/UDP协议)的底层支持. 网络层主要提供分组传输, 差错控制而后路由支持. 传输层提供端到端的数据传输, 差错控制, 流量控制和QoS支持.  
TCP协议不能保证时延和带宽, 实现起来相对复杂, 效率相对较低.  
UDP协议提供不可靠的传输, 可靠性由应用层负责. 不提供流量控制和带宽保证, 但是实时性高, 具有实现简单, 高效的特点.  
一些典型业务: FTP, Telnet, HTTP, SMTP等业务使用TCP; NFS, SNMP, TFTP等业务使用UDP. 还有一些业务同时使用TCP和UDP两种协议, 如DNS, P2P.

使用P2P 模式: 即时通信(QQ, Skype); 协同工作(如Groove, KDT); P2P网络中搜索(Infrasearch, Pointera)

#### 典型业务
**DNS业务**: 
DNS 是一个典型的客户/服务器 交互系统. 一个多层次, 基于域的命名系统, 使用分布式数据库实现这种命名机制.

**BT 业务**: 
拥有完整文件的Peer 被称为seed.  
BT网络的中央索引服务器称为Tracker, 负责保存共享文件的信息(主要是文件的SHA1 Hash 值)并共享每个文件的用户的信息(Peer ID, IP地址, 监听端口号, 是否完成下载). 共享同一个资源的Peer 群通过Tracker 共享用户和文件的信息, Peer 之间通过这些信息上传和下载的方式共享文件.  
BT 网络中Web server 用于发布种子文件并提供种子文件的下载. 种子文件用于保存共享文件的信息: Tracker 地址, 文件名, 文件长度, 分块大小, 分块HASH 值等

#### 业务监测和分析的内容
涉及网络流量分析的各个方面, 包括流量流向分析, 组分分析, 质量分析, 业务模式分析等.
用户角度(主要是进行用户行为分析)和网络角度(用户群体的集合特征)

#### RADIUS 
NAS网络接入服务器(Network Access Server)
远端用户拨入验证服务(RADIUS, Remote Authentication Dial In User Service)是一个AAA(authentication, authorization and accounting)协议.  
意思就是同时兼顾验证(authentication),授权(authorization)及计费(accounting)三种服务的一种网络传输协议(protocol).  
通常用于网络存取,或流动IP服务,适用于局域网及漫游服务.
NAS服务器和RADIUS 服务器通过UDP 协议进行通信, 双方使用不经网络传输的共享密钥

# Words
- type of service (ToS)
- DDOS(Distributed Denial of service): 很多DOS攻击源一起攻击某台服务器就组成了DDOS攻击
- Observation Point: 观察点,通常就是一个接口.定义为在网络中可以观察到IP报文的点
- Observation Domain:观察域,通常就是一个线卡,由很多接口组成.定义为一组观察点,这些观察点都运行了NetFlow服务,可提供一组流的汇聚记录
- Cisco Express Forwarding (CEF)
- Distributed CEF (dCEF)
- Line card 线卡: 交换机, 路由器或其它网络设备的访问线路与访问设备间的一种设备接口.具有多种速率的数字和模拟线卡,都可以用来连接电缆或光纤线路.数字线卡主要为数据网络提供数据包转发, ping 响应以及数据包分片等功能.
- MRTG: Multi Router Traffic Grapher
- service-level agreement (SLA) 