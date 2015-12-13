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
- docker rm [container]: 删除container之前, 需要stop, `docker rm $(docker ps -a -q)` 删除所有容器

命令中需要指定 container 时, 既可使用其名称, 也可使用其id.

## image
- docker rmi image_id: 删除image, `docker rmi $(docker images -q -a)`删除所有的
- docker diff 它可以列出容器内发生变化的文件和目录.这些变化包括添加(A-add),删除(D-delete),修改(C-change).

### 下载镜像
在本地 docker 环境中输入以下命令,就可以pull一个镜像到本地了.                    
`sudo docker pull index.tenxcloud.com/<namespace>/<repository>:<tag>`

注意:为了在本地方便使用,下载后您可以修改tag成短标签,比如
`sudo docker tag index.tenxcloud.com/mysql:latst mysql:latest`

除了docker官方的镜像仓库之外,还有很多第三方的docker镜像下载站点.
国内的docker镜像站点有:DockerPool,阿里云,对于国内用户来说,这些站点的速度要快很多.

- dockerpull: http://dockerpool.com/.  修改/etc/sysconfig/docker文件，修改OPTIONS='--selinux-enabled --insecure-registry dl.dockerpool.com:5000'
- 阿里云: http://help.aliyun.com/knowledge_detail/5974865.html, 暂时还没有连接成功

### 创建镜像
#### 通过container
`docker commit [container] image:tag`

#### 通过docker file
当前目录下包含Dockerfile, 使用命令build来创建新的image:
`docker build -t image:tag PATH`

注意: Docker Client会默认发送Dockerfile同级目录下的所有文件到Docker daemon中, 所以无关的文件不要出现在同级目录

### 发布镜像
1. 在本地 docker 环境中输入以下命令进行登录: `sudo docker login index.tenxcloud.com`
1. 对这个image进行标记,在命令中输入: `sudo docker tag image:tag index.tenxcloud.com/username/image:tag(自定义仓库名)`
1. push到镜像仓库中: `sudo docker push index.tenxcloud.com/username/image:tag`

注: 删除remote tag, docker client 还没有实现, [ref](https://github.com/docker/docker/issues/8759)

#### 导入导出
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

- 导出后再导入(exported-imported)的镜像会丢失所有的历史, 意味着将无法回滚到之前的层(layer)
- 保存后再加载(saveed-loaded)的镜像没有丢失历史和层(layer), 就可以做到层回滚(可以执行docker tag <LAYER ID> <IMAGE NAME>来回滚之前的层)

# install
1. yum install docker-devel docker
1. 启动docker 服务需要关闭selinux, 编辑/etc/selinux/config, 找到SELINUX 行修改成为:SELINUX=disabled, 之后重启系统
1. systemctl start docker

# Dockerfile
所有的 Dockerfile 命令格式都是: `INSTRUCTION arguments`

## FROM 命令
这个设置基本的镜像,为后续的命令使用,所以应该作为Dockerfile的第一条指令. 
```
FROM <image>
FROM <image>:<tag>
```
比如: `FROM ubuntu`
如果没有指定 tag ,则默认tag是latest,如果都没有则会报错.

## RUN 命令
RUN命令会在上面FROM指定的镜像里执行任何命令,然后提交(commit)结果,提交的镜像会在后面继续用到.

`RUN <shell command>`

RUN命令等价于:
```
docker run image command
docker commit container_id
```

## MAINTAINER
MAINTAINER name, email

## ENTRYPOINT 命令
ENTRYPOINT 命令设置在容器启动时执行命令.  

`ENTRYPOINT cmd param1 param2 ...`

## USER 命令
指定运行用户, eg: `USER eric`

## EXPOSE 命令
EXPOSE 命令可以设置一个端口在运行的镜像中暴露在外.
`EXPOSE <port> [<port>...]`

container内部服务开启的端口.主机上要用还得在启动container时,做host-container的端口映射: `docker run -d -p 127.0.0.1:33301:22 centos6-ssh`.
container ssh服务的22端口被映射到主机的33301端口

## ENV 命令
用于设置环境变量, 设置了后,后续的RUN命令都可以使用.
`ENV <key> <value>`

## ADD 命令
从src复制文件到container的dest路径: `ADD <src> <dest>`

- <src> 是相对被构建的源目录的相对路径,可以是文件或目录的路径,也可以是一个远程的文件url
- <dest> 是container中的绝对路径

## VOLUME 命令
创建一个挂载点用于共享目录, `VOLUME <mountpoint>`.
但是如果想把本地的目录挂载到这个目录, 还是需要在docker run的时候, 使用 -v 参数来显示的指定

## WORKDIR 命令
配置RUN, CMD, ENTRYPOINT 工作路径, 可以设置多次,如果是相对路径,则相对前一个 WORKDIR 命令.

`WORKDIR /path/to/workdir`

比如: `WORKDIR /a WORKDIR b WORKDIR c RUN pwd`, 其实是在 /a/b/c 下执行 pwd

## CMD 命令
一个Dockerfile里只能有一个CMD,如果有多个,只有最后一个生效.
`CMD command param1 param2`

与entrypoint 的关系

- Without entrypoint, CMD is command that is executed.
- With entrypoint, CMD is passed to entrypoint as argument.

