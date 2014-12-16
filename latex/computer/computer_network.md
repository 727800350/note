- circuit switching 电路交换: 独享
- message switching 报文交换: 共享
- take turns: 由supernode进行控制, 例如蓝牙

ATM 交换机是 circuit switching

Ad-hoc: multi-hop, infrastructure less, 节点之间可以相互通信, 并可以进行路由, 适用于战场的复杂的环境

- TDMA: Time Division Multiple Access 时分
- FDMA: Frequency Division Multiple Access 频分
- CDMA: Code Division Multiple Access 码分
- SDMA: Space Division Multiple Access 空分
在移动通信中，能实现空间分割的基本技术就是采用自适应阵列天线，在不同的用户方向上形成不同的波束

不同的小区用不同的频率; 上行频率与下行频率也不同; 之后再进行时分.

Taking turns  
缺点: 拿到token的节点down了怎么办? token 的伪造

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
