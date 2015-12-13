# intro
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

# basic
https://testerhome.com/topics/2760

## docker run
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
- docker rm [container]: 删除container之前, 需要stop

命令中需要指定 container 时, 既可使用其名称, 也可使用其id.

## image
- docker rmi image_id: 删除image
- docker diff 它可以列出容器内发生变化的文件和目录.这些变化包括添加(A-add),删除(D-delete),修改(C-change).
- docker load -i app_box.tar 该命令从tar文件中载入镜像或仓库到STDIN
- docker save image 类似于load,该命令保存镜像为tar文件并发送到STDOUT

### 下载镜像
在本地 docker 环境中输入以下命令,就可以pull一个镜像到本地了.                    
`sudo docker pull index.tenxcloud.com/<namespace>/<repository>:<tag>`

注意:为了在本地方便使用,下载后您可以修改tag成短标签,比如
`sudo docker tag index.tenxcloud.com/mysql:latst mysql:latest`

除了docker官方的镜像仓库之外,还有很多第三方的docker镜像下载站点.
国内的docker镜像站点有:DockerPool,阿里云,对于国内用户来说,这些站点的速度要快很多.

- dockerpull: http://dockerpool.com/
- 阿里云: http://help.aliyun.com/knowledge_detail/5974865.html

### 创建镜像
#### 通过container
`docker commit [container] image:tag`

#### 通过docker file
`docker build [options] PATH | URL`

- --rm=true表示构建成功后,移除所有中间容器
- --no-cache=false表示在构建过程中不使用缓存

### 发布镜像
1. 在本地 docker 环境中输入以下命令进行登录: `sudo docker login index.tenxcloud.com`
1. 对这个image进行标记,在命令中输入: `sudo docker tag image:tag index.tenxcloud.com/username/image:tag(自定义仓库名)`
1. push到镜像仓库中: `sudo docker push index.tenxcloud.com/username/image:tag`

# install
1. yum install docker-devel docker
1. 启动docker 服务需要关闭selinux, 编辑/etc/selinux/config, 找到SELINUX 行修改成为:SELINUX=disabled, 之后重启系统
1. systemctl start docker

