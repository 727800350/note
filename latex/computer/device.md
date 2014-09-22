# switch
## cisco
[基本命令](http://network.51cto.com/art/200604/25601.htm)

**交换机基本状态**
```
switch: ;ROM状态， 路由器是rommon>
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
如果你同时配了enable secret和enable password的时候，后者就失效了，登陆的时候，只需输入enable secret的密码即可

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
Switch(config-if-range)#switchport access vlan 2
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
```

**VTP**
VTP(VLAN Trunking Protocol): 是VLAN中继协议,也被称为虚拟局域网干道协议.它是思科私有协议.
作用是十几台交换机在企业网中,配置VLAN工作量大,可以使用VTP协议,把一台交换机配置成VTP Server, 其余交换机配置成VTP Client,
这样他们可以自动学习到server 上的VLAN 信息.
```
switch(config)#int f0/1 ;进入端口1
switch(config-if)#switchport access vlan 2 ;当前端口加入vlan 2(先要新建一个vlan 2)
switch(config-if)#switchport mode trunk ;设置为干线
switch(config-if)#switchport trunk allowed vlan 1，2 ;设置允许的vlan
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

## Cisco交换机Telnet的配置
[ref](http://blog.sina.com.cn/s/blog_62449fcf0100hv7b.html)

```
configure terminal 
line vty 0 15 
password password  ## 设置telnet的密码
login  ## 登陆要求口令验证
end
```

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
