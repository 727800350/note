# docker
## intro
build once, configure once and run anywhere

Docker有三个组件和三个基本元素.
三个组件分别是:

1. Docker Client 是用户界面,它支持用户与Docker Daemon之间通信.
1. Docker Daemon运行于主机上,处理服务请求.
1. Docker Index是中央registry,支持拥有公有与私有访问权限的Docker容器镜像的备份.

三个基本要素分别是:

1. Docker Containers负责应用程序的运行,包括操作系统,用户添加的文件以及元数据.
1. Docker Images是一个只读模板,用来运行Docker容器.
1. DockerFile是文件指令集,用来说明如何自动创建Docker镜像.

![结构图](http://dockerone.com/uploads/article/20141229/b28a5a6c1709746deb0aa2530be49af1.png)

Docker使用以下操作系统的功能来提高容器技术效率:

- Namespaces 充当隔离的第一级.确保一个容器中运行一个进程而且不能看到或影响容器外的其它进程.
- Control Groups是LXC的重要组成部分,具有资源核算与限制的关键功能.
- UnionFS(文件系统)作为容器的构建块.为了支持Docker的轻量级以及速度快的特性,它创建了用户层.

# docker run
`docker run options image command args`

options:

- -i: 交互模式, Keep STDIN open even if not attached
- -t: 分配一个tty
- -p 选项用于设置虚拟机的端口,让我们可以在本地访问Jupyter notebook服务器.
- -d 选项用于以detached模式运行容器,也就是作为背景进程运行.
- -v 选项让我们指定在本地机器中使用哪个文件夹存储notebook.通过这种方式可以在container 与 host 之间传输文件
- --device=[]: Add a host device to the container (e.g. --device=/dev/sdc:/dev/xvdc:rwm)
- -e|--env[=[]]: Set environment variables
- --env-file[=[]]: Read in a line delimited file of environment variables
- -h|--host‐name[=HOSTNAME]: set hostname
- --name[=NAME]: 这个最好设置, 之后操作container 的时候都可以通过这个名字来指定, 而不用使用不便于记忆的 UUID
- --mac-address[=MAC-ADDRESS]
- --volumes-from[=[]]: 从其他container(不需要启动) 到获取挂载目录
- --rm=true|false: Automatically remove the container when it exits (incompatible with -d). The default is false.
- -w:  The default working directory for running binaries within a container is the root directory (/), 可以指定任意一个目录, container 启动的时候会自动创建这个目录

## container
container 只是 image的一个实例

The operator can identify a container in three ways:

1. UUID long identifier ("f78375b1c487e03c9438c729345e54db9d20cfa2ac1fc3494b6eb60872e74778")
1. UUID short identifier ("f78375b1c487")
1. Name ("jonah")

- docker ps 查看正在运行的所有 container 信息	
- docker ps -a 查看所有 container, 包括正在运行和已经关闭的	
- docker logs -f [container] 输出指定 container 的 stdout 信息(用来看log, 效果和 tail -f 类似, 会实时输出)
- docker port [container] [port] 获取 container 指定端口映射关系	
- docker top [container] 查看 container 进程列表
- docker inspect [container] 查看 container 详细信息	
- docker start/stop/restart [container]
- docker rm [container]: 删除container之前, 需要stop, `docker rm $(docker ps -a -q)` 删除所有容器

命令中需要指定 container 时, 既可使用其名称, 也可使用其id.

## image
- `docker rmi image_id`: 删除image
- docker diff 它可以列出容器内发生变化的文件和目录.这些变化包括添加(A-add),删除(D-delete),修改(C-change).

### 下载镜像
在本地 docker 环境中输入以下命令,就可以pull一个镜像到本地了.                    
`sudo docker pull index.tenxcloud.com/<namespace>/<repository>:<tag>`

注意:为了在本地方便使用,下载后您可以修改tag成短标签,比如
`sudo docker tag index.tenxcloud.com/mysql:latst mysql:latest`

### 创建镜像
当前目录下包含Dockerfile, 使用命令build来创建新的image:
`docker build -t image:tag PATH`

注意: Docker Client会默认发送Dockerfile同级目录下的所有文件到Docker daemon中, 所以无关的文件不要出现在同级目录

### 导入导出
容器
```Bash
docker export [container] > container.tar
cat container.tar | docker import - image-export:latest
```

镜像
```Bash
docker save [image] > image.tar
docker load < image.tar
```
注意: save 的image 不能通过import 的方式来导入, 只能使用load的方式

- 导出后再导入(exported-imported)的镜像会丢失所有的历史, 意味着将无法回滚到之前的层(layer)
- 保存后再加载(saveed-loaded)的镜像没有丢失历史和层(layer), 就可以做到层回滚(可以执行docker tag <LAYER ID> <IMAGE NAME>来回滚之前的层)

# Kubernetes
[十分钟带你理解Kubernetes核心概念](http://dockone.io/article/932)

## intro
Kubernetes(k8s)是自动化容器操作的开源平台,这些操作包括部署,调度和节点集群间扩展.
如果你曾经用过Docker容器技术部署容器,那么可以将Docker看成Kubernetes内部使用的低级别组件.Kubernetes不仅仅支持Docker,还支持Rocket,这是另一种容器技术.
使用Kubernetes可以:

- 自动化容器的部署和复制
- 随时扩展或收缩容器规模
- 将容器组织成组,并且提供容器间的负载均衡
- 很容易地升级应用程序容器的新版本
- 提供容器弹性,如果容器失效就替换它,等等...

实际上,使用Kubernetes只需一个部署文件,使用一条命令就可以部署多层容器(前端,后台等)的完整集群:
```bash
$ kubectl create -f single-config-file.yaml
```

## cluster
kubectl是和Kubernetes API交互的命令行程序.现在介绍一些核心概念.

<img src="./pics/kubernetes/cluster.png" alt="kubernetes cluster" align="middle" width="70%"/>

### pod
Pod 是 Kubernetes 中最小的可互动单元.一个 Pod 可以由多个容器组成,这些容器运行在同一个节点上,并共享资源,如文件系统,内核命名空间和IP地址(可以使用localhost互相通信).
在微服务世界中,一个 Pod 可以是执行后台工作或服务请求的微服务的单个实例.

### label and annotation
Label 是 Kubernetes 及其最终用户用于过滤系统中相似资源的方式,也是资源与资源相互"访问"或关联的粘合剂.
比如说,为 Deployment 打开端口的 Service.不论是监控,日志,调试或是测试,任何 Kubernetes 资源都应打上标签以供后续查验.
例如,给系统中所有 Worker Pod 打上标签:app=worker,之后即可在 kubectl 或 Kubernetes API 中使用 --selector 字段对其进行选择.

Annotation 与 Label 非常相似,但通常用于以自由的字符串形式保存不同对象的元数据,例如"更改原因: 安全补丁升级".

### Replication Controller
Replication Controller确保任意时间都有指定数量的Pod"副本"在运行.
如果为某个Pod创建了Replication Controller并且指定3个副本,它会创建3个Pod,并且持续监控它们.如果某个Pod不响应,那么Replication Controller会替换它,保持总数为3.

### Service
如果Pods是短暂的,那么重启时IP地址可能会改变,怎么才能从前端容器正确可靠地指向后台容器呢?

Service是定义一系列Pod以及访问这些Pod的策略的一层抽象.Service通过Label找到Pod组.因为Service是抽象的,所以在图表里通常看不到它们的存在,这也就让这一概念更难以理解.

现在,假定有2个后台Pod,并且定义后台Service的名称为'backend-service',lable选择器为(tier=backend, app=myapp).backend-service 的Service会完成如下两件重要的事情:

1. 会为Service创建一个本地集群的DNS入口,因此前端Pod只需要DNS查找主机名为 'backend-service',就能够解析出前端应用程序可用的IP地址.
1. 现在前端已经得到了后台服务的IP地址,但是它应该访问2个后台Pod的哪一个呢?
  Service在这2个后台Pod之间提供透明的负载均衡,会将请求分发给其中的任意一个(如下面的动画所示).通过每个Node上运行的代理(kube-proxy)完成.这里有更多技术细节.

#### Service Discovery
Service Discovery(服务发现)作为编排系统,Kubernetes 控制着不同工作负载的众多资源,负责管理 Pod,作业及所有需要通信的物理资源的网络.
为此,Kubernetes 使用了 ETCD.ETCD 是 Kubernetes 的"内部"数据库,Master 通过它来获取所有资源的位置.
Kubernetes 还为服务提供了实际的"服务发现", 所有 Pod 使用了一个自定义的 DNS 服务器,通过解析其他服务的名称以获取其 IP 地址和端口.它在 Kubernetes 集群中"开箱即用",无须进行设置.

### Node
节点(上图橘色方框)是物理或者虚拟机器,作为Kubernetes worker,通常称为Minion.每个节点都运行如下Kubernetes关键组件:

- Kubelet:是主节点代理.
- Kube-proxy:Service使用其将链接路由到Pod,如上文所述.
- Docker或Rocket:Kubernetes使用的容器技术来创建容器.

### Kubernetes Master
Kubernetes Master提供集群的独特视角,并且拥有一系列组件,比如Kubernetes API Server.API Server提供可以用来和集群交互的REST端点.master节点包括用来创建和复制Pod的Replication Controller.

