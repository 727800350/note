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
```
switch: ;ROM状态, 路由器是rommon>
hostname> ;用户模式
hostname# ;特权模式
hostname(config)# ;全局配置模式
hostname(config-if)# ;接口状态
```

**交换机口令设置**
```
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

```
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
```
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
```
switch(config)#no vlan 2 ;删除vlan 2
switch(config)#no interface vlan 2
```
这两个命令的先后顺序还不太确定

**VTP**
VTP(VLAN Trunking Protocol): 是VLAN中继协议,也被称为虚拟局域网干道协议.它是思科私有协议.
作用是十几台交换机在企业网中,配置VLAN工作量大,可以使用VTP协议,把一台交换机配置成VTP Server, 其余交换机配置成VTP Client,
这样他们可以自动学习到server 上的VLAN 信息.

```
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

**交换机设置IP地址**
确定interface to be configured as a Layer 3 interface, and enter interface configuration mode.
```
interface {{fastethernet | gigabitethernet} interface-id} | {vlan vlan-id} | {port-channel port-channel-number}
```
eg: `switch(config)#interface vlan 1 ;进入vlan 1`

```
switch(config-if)#ip address 192.168.2.100 255.255.255.0  ;设置IP地址及其subnet mask
switch(config)#ip default-gateway  ;设置默认网关
switch(config)#no shutdown ## Enable the interface.
switch(config)#end ## Return to privileged EXEC mode
```

验证
```
show interfaces [interface-id] 
show ip interface [interface-id]
show running-config interface [interface-id]
```

optional: 保存
```
copy running-config startup-config

```

**交换机显示命令**
```
switch#write ;保存配置信息
switch#show vtp ;查看vtp配置信息
switch#show run ;查看当前配置信息
switch#show vlan ;查看vlan配置信息
switch#show interface ;查看端口信息
switch#show int f0/0 ;查看指定端口信息
```

**端口**
```
#sh ip int b 命令查看端口状态
```

`#sh cdp nei命令查看相邻路由器`

## 交换机连接
### 串口
SecureCRT
在connection中的protocol 选择 Serial
然后再connection 下面的Serial 中: Port COM6, Baud rate: 9600, Data bits: 8, Parity: None, Stop bits: 1

### Telnet 连接
[ref](http://blog.sina.com.cn/s/blog_62449fcf0100hv7b.html)

```
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
```
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

# 磁盘阵列 RAID
独立硬盘冗余阵列(RAID, Redundant Array of Independent Disks),旧称廉价磁盘冗余阵列(Redundant Array of Inexpensive Disks),简称硬盘阵列.
其基本思想就是把多个相对便宜的硬盘组合起来,成为一个硬盘阵列组,使性能达到甚至超过一个价格昂贵,容量巨大的硬盘.
根据选择的版本不同,RAID比单颗硬盘有以下一个或多个方面的好处:增强数据集成度,增强容错功能,增加处理量或容量.另外,磁盘阵列对于电脑来说,看起来就像一个单独的硬盘或逻辑存储单元.

目前对RAID级别的定义可以获得业界广泛认同的有4种,RAID 0,RAID 1,RAID 0＋1和RAID 5, etc.

- RAID 0是无数据冗余的存储空间条带化,具有成本低,读写性能极高,存储空间利用率高等特点,适用于音,视频信号存储,临时文件的转储等对速度要求极其严格的特殊应用.但由于没有数据冗余,其安全性大大降低,构成阵列的任何一块硬盘的损坏都将带来灾难性的数据损失.这种方式其实没有冗余功能,没有安全保护,只是提高了磁盘读写性能和整个服务器的磁盘容量.一般只适用磁盘数较少,磁盘容易比较紧缺的应用环境中,如果在RAID 0中配置4块以上的硬盘,对于一般应用来说是不明智的.  
- RAID 1是两块硬盘数据完全镜像,安全性好,技术简单,管理方便,读写性能均好.因为它是一一对应的,所以它无法单块硬盘扩展,要扩展,必须同时对镜像的双方进行同容量的扩展.因为这种冗余方式为了安全起见,实际上只利用了一半的磁盘容量,数据空间浪费大.  
- RAID 0＋1综合了RAID 0和RAID 1的特点,独立磁盘配置成RAID 0,两套完整的RAID 0互相镜像.它的读写性能出色,安全性高,但构建阵列的成本投入大,数据空间利用率低. 
- RAID 5是目前应用最广泛的RAID技术.各块独立硬盘进行条带化分割,相同的条带区进行奇偶校验(异或运算),校验数据平均分布在每块硬盘上.以n块硬盘构建的RAID 5阵列可以有n－1块硬盘的容量,存储空间利用率非常高.任何一块硬盘上的数据丢失,均可以通过校验数据推算出来.它和RAID 3最大的区别在于校验数据是否平均分布到各块硬盘上.RAID 5具有数据安全,读写速度快,空间利用率高等优点,应用非常广泛,但不足之处是如果1块硬盘出现故障以后,整个系统的性能将大大降低. 

RAID 1,RAID 0＋1,RAID 5阵列配合热插拔(也称热可替换)技术,可以实现数据的在线恢复,
即当RAID阵列中的任何一块硬盘损坏时,不需要用户关机或停止应用服务,就可以更换故障硬盘,修复系统,恢复数据,对实现高可用系统具有重要的意义.

## 配置磁盘阵列
首先是**进入设置界面**, 在网上查到有的说在开机界面通过以下快捷键
Ctrl + H, 或
Ctrl + C, 或
Ctrl + A, 或
Ctrl + M 进入webbios设置界面.

在IBM 的服务器上, 上述快捷键都没有成功, 最后在[IBM 进入RAID设置](http://thinkpig007.blog.51cto.com/971471/1252154)

1. 开机,按F1进入BIOS
2. 选择 system setting
3. Adpters and UEFI Drivers 适配器与"统一的可扩展固定接口"(Unified Extensible Firmware Interface)驱动
4. 选择 LSI EFI SAS Driver 下的PCI的驱动设置
5. Press 1 for EFI webBIOS 有两种方式进行BIOS管理,一个是WEB,一个是命令行方式.我们选择web方式
6. 点击 start

[阵列卡 WebBIOS配置](http://wenku.baidu.com/view/b15cca9c6bec0975f465e22d.html?re=view)

阵列卡上的硬盘通常在Unconfigured Good状态下才能配置阵列,如果硬盘处于Unconfigured Bad状态或者JBOD等不正常状态需要尝试将硬盘状态调整为Unconfigured Good.
方式是单击bad 状态的硬盘, 然后选中make good选项框, 然后点击go.

当新添加的硬盘上已经有其他RAID配置信息时,这组硬盘会被识别成Foreign状态,需要用到导入(Import)阵列信息的操作
注意:阵列信息的导入操作存在风险,有可能造成数据丢失,请谨慎操作.

选择New Configuration,选择后会清除现有阵列卡上的所有阵列信息,如果此时阵列卡上已经配置了其他阵列,此处请选择Add Configuration.

配置RAID1需要2块硬盘,配置RAID5至少需要3块硬盘.

配置的阵列状态是正常的Optimal,阵列中每个硬盘的状态都是在线的Online.

