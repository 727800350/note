- circuit switching 电路交换: 独享
- message switching 报文交换: 没有建立物理连接, 每一个结点接收整个报文,检查目标结点地址,然后根据网络中的交通情况在适当的时候转发到下一个结点.经过多次的存储-转发,最后到达目标,因而这样的网络叫存储-转发网络.其中的交换结点要有足够大的存储空间(一般是磁盘),用以缓冲收到的长报文.
- packet switching 分组交换: 共享, 固定大小上限的分组
- take turns: 由supernode进行控制, 例如蓝牙

Taking turns  
缺点: 拿到token的节点down了怎么办? token 的伪造

ATM 交换机是 circuit switching

Ad-hoc: multi-hop, infrastructure less, 节点之间可以相互通信, 并可以进行路由, 适用于战场的复杂的环境

MAC地址也有被用来作为路由的, 例如在SDN中

Gbits: 用户主机与switch 直接联系

- Bridge: 将数据从一个lan转发到另外一个lan中, 而不同的lan可以采用不同的协议
- swith: 老师说也有类似于路由器的转发表, 我印象中是所有端口转发啊

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
