- 判断硬盘是hdd 还是ssd: `cat /sys/block/xxx/queue/rotational`的返回值(其中xxx为你的硬盘设备名称,例如sda等等),如果返回1则表示磁盘可旋转,那么就是HDD了,反之,如果返回0,则表示磁盘不可以旋转,那么就有可能是SSD了.

# switch
## cisco
Cisco交换机提供了几种配置模式或称之为配置视图,各配置模式下所能使用的配置命令各不相同,这几种配置模式如下:

- 普通用户模式:开机直接进入普通用户模式,在该模式下我们只能查询交换机的一些基础信息如版本号show version.提示信息switch>
- 特权用户模式:在普通用户模式下输入enable 命令即可进入特权用户模式,在该模式下我们可以查看交换机的配置信息和调试信息等等.提示信息switch#
- 全局配置模式:在特权用户模式下输入configure terminal 命令即可进入全局配置模式,在该模式下主要完成全局参数的配置.提示信息switch(config)#
- 接口配置模式:在全局配置模式下输入interface interface-list 即可进入接口配置模式.在该模式下主要完成接口参数的配置.提示信息switch(config-if)#
- VLAN 配置模式:在全局配置模式下输入vlan database 即可进入VLAN 配置模式,该配置模式下可以完成VLAN 的一些相关配置.

思科交换机提供了强有力的帮助功能,在任何模式下均可以使用"?"来查看命令的格式或参数.具体用法如下:

1. 在任何模式下直接键入"?" 查询任何模式下可以使用的所有命令
2. 在前缀字符后键入"?" 可以查看该视图下以该前缀开头的所有命令,如键入"s?"可以查询所有以字符s开头的所有命令
3. 命令单词后跟一个空格和一个"" 如"show  ?"用来查看show命令的参数.

交换机的**端口工作模式**一般可以分为三种:Access,Multi,Trunk.
trunk: 模式的端口用于交换机与交换机,交换机与路由器,大多用于级联网络设备所以也叫干道模式.
Access: 多用于接入层也叫接入模式.目前般交换设备端口默认都access模式
Multi: 在一个线路中承载多个vlan,但不像trunk,它不对承载的数据打标签.主要用于接入支持多vlan的服务器或者一些网络分析设备.现在基本不使用此类接口,在cisco的网络设备中,也基本不支持此类接口了.

当对端口执行 `no switchport`后, 端口变成路由口,启用三层功能,直接连接终端.
这时, 如果使用`show run`查看, 可以再这些interface 下面看到`no switchport, no ip address`, 因为, 变成了路由口之后, 需要一个IP 地址, 这个端口才能正常工作
switchport是交换口, 二层功能, 不需要IP地址就能工作, 所有二层功能的端口都会相互转发数据, 所以正常工作不需要IP 地址
这时想要恢复到原来默认的状态, 可以执行 `switchport`

[基本命令](http://network.51cto.com/art/200604/25601.htm)

**交换机基本状态**
```info
switch: ;ROM状态, 路由器是rommon>
hostname> ;用户模式
hostname# ;特权模式
hostname(config)# ;全局配置模式
hostname(config-if)# ;接口状态
```

**交换机口令设置**
```info
switch>enable ;进入特权模式
switch#config terminal ;进入全局配置模式
switch(config)#hostname  ;设置交换机的主机名
switch(config)#enable secret xxx ;设置特权加密口令
switch(config)#enable password xxa ;设置特权非密口令
switch(config)#line console 0 ;进入控制台口
switch(config-line)#line vty 0 4 ;进入虚拟终端
switch(config-line)#login ;允许登录
switch(config-line)#password xx ;设置登录口令xx
switch#exit ;返回命令
```
如果你同时配了enable secret和enable password的时候,后者就失效了,登陆的时候,只需输入enable secret的密码即可

**交换机VLAN(Virtual Local Area Networks)**
By creating a VLAN you are separating the network. There are some several reasons,
like separating your production network from a guest network, and separating a test environment from a production network.
This all adds security to your network.
新交换机出厂时的默认配置是预配置为VLAN1
VTP 模式为服务器

在一个switch上创建vlan 分为两个步骤:
[reference](http://ciscoskills.net/2011/04/01/creating-vlans/)

1. put the ports 1 – 10 into an access mode
2. apply a VLAN 2 on ports 1 – 10.

```info
Switch>enable
Switch#configure terminal
Switch(config)#interface range gigabitEthernet 1/0/1 - 10  ## 端口的名字可能不同
Switch(config-if-range)#switchport mode access
Switch(config-if-range)#switchport access vlan 2  ## 让这些端口只能为vlan 2 carry traffic
% Access VLAN does not exist. Creating vlan 2
Switch(config-if-range)#exit
Switch(config)#exit
Switch#show vlan  ## 可以看到端口的1 到10 已经在新建的一个vlan 2中
```

**创建一个自定义名字的vlan**
```info
Switch(config)#interface vlan 3
Switch(config-if)#exit
Switch(config)#vlan 3
Switch(config-vlan)#name NewName
Switch(config-vlan)#exit
Switch(config)#interface range gigabitEthernet 1/0/11 - 15
Switch(config-if-range)#switchport mode access
Switch(config-if-range)#switchport access vlan 3
Switch(config-if-range)#exit
Switch(config)#exit
Switch#show vlan  ## 可以看到端口的11 到15 已经在新建的一个一个名为NewName 的vlan 3中
```
So in the end we have three VLANs on this switch, VLAN 1, VLAN 2, VLAN 3 (NewName).
If we connected PCs to these three different VLANs none of them would be able to communicate,
because each VLAN is like a different network. So you would need to configure a router in order for inter-vlan commutation to work.

**删除vlan**
```info
switch(config)#no vlan 2 ;删除vlan 2
switch(config)#no interface vlan 2
```
这两个命令的先后顺序还不太确定

**VTP**
VTP(VLAN Trunking Protocol): 是VLAN中继协议,也被称为虚拟局域网干道协议.它是思科私有协议.
作用是十几台交换机在企业网中,配置VLAN工作量大,可以使用VTP协议,把一台交换机配置成VTP Server, 其余交换机配置成VTP Client,
这样他们可以自动学习到server 上的VLAN 信息.

```cmd
switch(config)#int f0/1 ;进入端口1
switch(config-if)#switchport access vlan 2 ;当前端口加入vlan 2(先要新建一个vlan 2)
switch(config-if)#switchport mode trunk ;设置为干线
switch(config-if)#switchport trunk allowed vlan 1,2 ;设置允许的vlan
switch(config-if)#switchport trunk encap dot1q ;设置vlan 中继
switch(config)#vtp domain name ;建立vtp域
switch(config)#vtp mode server|client|transparent  ;设置发vtp模式
switch(config)#vtp password  ;设置vtp密码
Switch(config)#show vtp status ; 查看VTP运行状态
Switch(config)#show vtp counters ; 查看交换机收到和发出广告的数目
```

**交换机显示命令**
```cmd
switch#write ;保存配置信息
switch#show vtp ;查看vtp配置信息
switch#show run ;查看当前配置信息
switch#show vlan ;查看vlan配置信息
switch#show interface ;查看端口信息
switch#show int f0/0 ;查看指定端口信息
```

**端口**
```bash
sh ip int b 命令查看端口状态
```

`#sh cdp nei命令查看相邻路由器`

## 交换机连接
### 串口
SecureCRT
在connection中的protocol 选择 Serial
然后再connection 下面的Serial 中: Port COM6, Baud rate: 9600, Data bits: 8, Parity: None, Stop bits: 1

### Telnet 连接
[ref](http://blog.sina.com.cn/s/blog_62449fcf0100hv7b.html)

```bash
configure terminal
line vty 0 15
password password  ## 设置telnet的密码
login  ## 登陆要求口令验证
end
```

之后就可以通过telnet 进行连接了
`telnet ip_address`

`line vty 0 15`:
Configure the number of Telnet sessions (lines), and enter line configuration mode.
There are 16 possible sessions on a command-capable switch. The 0 and 15 mean that you are configuring all 16 possible Telnet sessions.

路由器发生故障或状态发生变化的时候,会发出告警,报错,提示信息.如果是用telnet 命令登录路由器的话,不能直接看到这些信息,需要输入
`# ter mon`
命令后,才能使这些信息显示在MS-DOS窗口上

## 端口镜像
```bash
configure terminal
no monitor session {session_number | all | local | remote}
monitor session session_number source {interface interface_id | vlan vlan_id} [both |rx |tx]
monitor session session_number destination {interface interface_id | vlan vlan_id [encapsulation replicate]}
end
show monitor [session session_number] or show run
copy running-config startup-config ## Optional) Save the configuration in the configuration file
```
配置source的几种方式

- both: Monitor both received and sent traffic. This is the default.
- rx: Monitor received traffic.
- tx: Monitor sent traffic.

做了镜像端口,连接这个端口就变成单向接收模式了, 也就是说连接这个端口的网卡只能接收数据, 而不能发送数据, 那么主机将无法上网了.
那,如果想要远程连接抓包怎么办呢?就在这台主机上再加一块网卡就可以上网,也可以远程了.

# router
网关(Gateway)又称网间连接器,协议转换器. 网关在网络层以上实现网络互连,是最复杂的网络互连设备,仅用于两个高层协议不同的网络互连.
网关既可以用于广域网互连,也可以用于局域网互连. 网关是一种充当转换重任的计算机系统或设备.
在使用不同的通信协议,数据格式或语言,甚至体系结构完全不同的两种系统之间,网关是一个翻译器.
与网桥只是简单地传达信息不同,网关对收到的信息要重新打包,以适应目的系统的需求.
**网关基本上都是路由器**, 如果机器能够ping 通网关,那么一般机器就是可以上网的,如果不能上网, 那么可以尝试ping 一下DNS 服务器

# IBM 服务器
## 指示灯
- PS1 指示灯:当此指示灯发亮时,表明电源1 出现故障.
- PS2 指示灯:当此指示灯发亮时,表明电源2 出现故障.
- Temp 指示灯:当此指示灯发亮时,表明系统温度超出阈值级别.
- 风扇指示灯:当此指示灯点亮时,表明散热风扇或电源风扇出现故障或运行太慢.风扇发生故障还会导致over Temp 指示灯发亮.
- VRM 指示灯:当此指示灯发亮时,表明微处理器托盘上的某个vrm 出现故障.
- CPU 指示灯:当此指示灯发亮时,表明某个微处理器出现故障.
- PCI 指示灯:当此指示灯发亮时,表明某个PCI 总线发生错误.
- MEM 指示灯:当此指示灯发亮时,表明发生内存错误.
- DASD 指示灯:当此指示灯发亮时,表明某个热插拔硬盘驱动器出现故障.
- NR 指示灯:当此指示灯发亮时,表明存在两个电源但电源并不冗余.
- NMI 指示灯:当此指示灯发亮时,表明出现一个不可屏蔽中断.
- SP  指示灯:当此指示灯发亮时,表明服务处理器遇到错误.
- EXP 指示灯:当此指示灯发亮时,表明某个连接的i/o 扩展单元出现故障.
- log 指示灯:当此指示灯发亮时,表明您应该查看事件日志或remote supervisor
- Adapter ii 日志以获取有关非最佳条件的信息.
- over spec 指示灯:当此指示灯发亮时,表明对电源的需求超过了指定的电源供应.

remind 按钮:按下此按钮可重新设置操作员信息面板上的系统错误指示灯并将服务器置于提醒方式.  
在提醒方式下,故障并没有清除但系统错误指示灯会闪烁(每2 秒闪烁一次)而不是持续发亮,如果出现另一个系统错误,则系统错误指示灯将会持续发亮.

IBM服务器硬盘指示灯

- 如果是亮红灯,那证明硬盘是坏掉了,一般是肯定的.
- 如果黄灯闪烁是硬盘在同步数据,同步完成会熄灭,DASD灯亮需作测试报告以进一步确定故障原因
- 如果是长亮黄灯,不一定是盘挂了,但是可能性比较大,有时候也可能是你的系统问题,比如RAID没做好.(如果你不会操作RAID管理软件的话,比较简单的办法就是把黄灯的盘拔下来再安上去,看看能不能自动同步,必须在开机的状态下,如果重新插拔后故障盘出现黄灯闪烁,那就是已经同步了,等同步完成,也就是黄灯不闪,绿灯正常的闪了,那就再观察一段时间,不出问题的话就没什么了)
- 如果是绿灯长亮, 一般是没有数据在读写.或者硬盘没有格式化.  
