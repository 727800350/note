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


