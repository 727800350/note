1. 创建存放本地rpm的文件夹
```
mkdir -p /yum/server
```
1. 将iso挂载后, 将Packages文件夹中的rpm包copy到/yum/server中
1. 安装repository工具包
```
yum install -y createrepo
```
1. 建立reposity的索引, 命令的执行需要一会儿时间
```
createrepo /yum/server
```
1. 安装yum-plugin-priorities, 设置repo的优先级
```
yum install -y yum-plugin-priorities
```
`priority=1` 为最高级, 默认的优先级为99
1. 创建repo文件 `/etc/yum.repos.d/localyum.repo`
在里面加入如下内容
```
[localyum]
Name=localyum
baseurl=file:///yum/server
enable=1
gpgcheck=0
priority=1
```
1. over, `yum clean all`一下
