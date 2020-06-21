- `echo *`: 会输出当前目录的文件
- `echo "*"`: 才会输出`*`
- `watch -d -n 1 command`: 间隔1秒钟, 执行一次command, 并将结果的diff 高亮显示

# 监控
- 每个逻辑cpu的使用率, 只需要运行top命令, 按下数字键1即可

总核数 = 物理CPU个数 X 每颗物理CPU的核数
总逻辑CPU数 = 物理CPU个数 X 每颗物理CPU的核数 X 超线程数

- 查看物理CPU个数: `cat /proc/cpuinfo | grep "physical id" | sort | uniq | wc -l`
- 查看每个物理CPU中core的个数(即核数): `cat /proc/cpuinfo | grep "cpu cores" | uniq`
- 查看逻辑CPU的个数: `cat /proc/cpuinfo | grep "processor" | wc -l`

# 查找
- ctrl r: 回溯搜索(Backwards search)history缓冲区内的文本(在命令行下)
- j: autojump
- fd: a simple, fast and user-friendly alternative to 'find'
- locate: 其实是`find -name`的另一种写法,但是要比后者快得多, 原因在于它不搜索具体目录,而是搜索一个数据库(/var/lib/locatedb), 使用updatedb 来更新这个数据库
- ack: search inside files with no extensions: `ack -a -G regex what-to-search-for`
- grep: the exit status is 0 if selected lines are found and 1 otherwise

在directory及其子目录下的c 文件中查找keyword
```bash
grep -R --include="*.c" keyword directory
```

- -n参数,在文件中查找指定模式并显示匹配行的行号(同时会列出文件名)
- -v参数输出不包含指定模式的行
- -i: 不区分大小写
- -e 参数查找多个模式, **或**关系, 当和-v 一起使用时有疑问
- -f 用文件指定待查找的模式
- -c 参数计算模式匹配到的数量
- -B: 输出匹配行的前n行
- -A: 输出匹配行的后n行
- -C: 相当于-B and -A
- ^ 符号输出所有以某指定模式开头的行
- $ 符号输出所有以指定模式结尾的行, `^$` 就表示空行

# 网络
- `netstat -anupl`: a表示all, n表示用数字形式显示端口号, u/t,表示UDP/TCP协议类型, p是程序PID, l表示处于监听状态的
- `lsof -i:80`: 这个需要root 权限, 如果有显示说明80端口已经开放了,如果没有显示说明没有开放
- ss: socket statistics (show apps using the Internet)
- iftop: visually show network traffic
- `rsync -av src dst`
	- `-av`: 保证src 是dst 的子集
		1. 如果在src 删除了文件, 不会将删除文件的操作同步到 dst
		1. 如果在dst 删除了src 中存在的文件, 那么再次执行 rsync -av 后, 被删除的文件会再次出现在dst
		1. 如果在dst 中增加了文件, 那么再次执行 rsync -av 之后, 增加的文件不会被删除
	- `rsync -av a b/`: 将a这个目录整体同步到b下面, 得到 b/a 这样的文件结构
	- `rsync -av a/ b/`: 将a下面的文件同步到b下面, 得到 `b/files_of_a` 这样的文件结构
	- `--exclude ignore`: 忽略src/ignore
- `nc`: netcat
	- server: `nc -v -v -l -p 3000`: Listen on TCP port 3000, and once there is a connection, send stdin to the remote host, and receive data from the remote host to stdout. 也就是即可以收数据也可以发数据
	- client: `nc -v -v -n server_ip 3000`: open a TCP connection to port 3000 of server ip, 同样即可以发数据, 也可以收数据
	- eg: `nc -v -v -l -p 3000 <server.src >server.dst` and `nc -v -v -n 127.0.0.1 3000 <client.src >client.dst`, 执行之后 server.src 和 client.dst 一样, server.dst 和 client.src 一样

- wget 一个目录: `wget  -r ftp://xxx_path_xxx ./`

## ssh
产生一个新的密钥对(如果已有, 这步可以跳过) `cd ~/.ssh; ssh-keygen -t rsa -C "your_email@example.com"`

把公钥上传到想要免密登录的服务器, 可使用ssh-copy-id命令来完成,
```bash
ssh-copy-id root@remote-server -p port
```
代替了手动拷贝到 `authorized_keys` 中

`ssh -o StrictHostKeyChecking=no user@ip "bash shell.sh" < /dev/null` 登陆一台机器, 自动yes, 并在登录后的机器上执行命令.
	- [StrictHostKeyChecking=no ssh: automatically accept keys](https://askubuntu.com/questions/123072/ssh-automatically-accept-keys).
	- 在远程执行的命令的stdout 和 stderr 是发起ssh 命令的机器.
	- 加上`/dev/null` 是为了避免可能的干扰

iterm 文件传输(本地和服务器都需要安转 lrzsz)
	- `rz`: 将文件传到远端服务器
	- `sz file`: 从远端服务器下载文件

putty connection refused error
`sudo service sshd status`
发现SSH: Could not load host key: `/etc/ssh/ssh_host_rsa_key`  
因为刚安装ssh之后, 这个文件不存在, 我们只是在`$HOME/.ssh` 中产生了key, 而没有在`/etc/ssh`中产生key.  
解决方法: `sudo ssh-keygen -A`

## iptables
iptables, 但更正确的名称是 iptables/netfilter.
iptables 是一个用户空间的模块. 作为用户, 你在命令行就是通过它将防火墙规则放进缺省的表里.
netfilter 是一个核心模块, 它内置于内核中, 进行实际的过滤.

iptables 将规则放进缺省的规则链(INPUT, OUTPUT 及FORWARD), 而所有流量(IP包)都会被相关的规则链检查, 根据当中的规则判断如何处理(如接纳或丢弃)每个封包.
这些动作称为目标, 而最常见的两个缺省目标就是 DROP 包, 或ACCEPT 包.

我们可以在过滤表的 3 条缺省规则链内加入规则, 来处理通过这些规则链的封包. 它们分别是:

1. INPUT: 所有以主机为目的地的封包
1. OUTPUT: 所有源自主机的封包
1. FORWARD: 这些封包的目的地或来源地都不是主机, 但路经主机(由它路由). 假若你的主机是一个路由器, 这条规则链将会被应用.

eg:

- `iptables -A INPUT -p tcp --dport 22 -j ACCEPT`: 允许 SSH 通过 tcp 端口 22 来连接本机
- `iptables -A OUTPUT -p tcp --tcp-flags RST RST -j DROP`: 将从本机发出的带有RST 标记的包丢弃

# File System
- 垃圾箱位置: .local/share/Trash/
- iotop: i/o stats
- `du -h --max-depth=0`: 查看目录大小  
- df: 查看磁盘使用情况  
- `dd`: 转换和拷贝文件(包括二进制文件, mbr文件)
	- if=filename: 输入的文件名
	- of=finename: 输出的文件名
	- bs=bytes: 一次读写的字节数, 默认是512 bytes
	- skip=blocks: 拷贝前, 跳过的输入文件的前blocks块, 块的大小有bs决定
	- count=blocks:只拷贝输入文件的前blocks块
- tar and gzip
	- for GNU tar:   gtar cvzf file.tar.gz filenames
	- for any tar:   tar cvf -  filenames | gzip > file.tar.gz
	- `cat file1 file2 | gzip > foo.gz` can get better compression than `gzip -c file1 file2 > foo.gz`
	- 将foo.tar.gz 解压到/to 目录: `tar -zxvf foo.tar.gz -C /to`
	- `tar cvzf all.tar.gz dir0  --exclude dir1 --exclude file`: 排除dir0下面的dir1和file
- 如果文件数量太大,rm提示超出参数列表长度的话,可以用下列命令来删除:`find . -name "*" -exec rm {} \; -print`
- umask: 指定是默认不具备的权限, r,w,x分别是4,2,1. 执行`umask` 得到`0002`, 后面三个数字分别对应o(owner), g(group), a(all), 最后一个2意味着a 默认不具备w 权限

# Text
- split: split a file into pieces
	- -a, --suffix-length=N, use suffixes of length N (default 2)
	- -d, --numeric-suffixes, use numeric suffixes instead of alphabetic
	- -l, --lines=NUMBER, put NUMBER lines per output file
- paste: merge lines of files(以列为对象), With no FILE, or when FILE is -, read standard input
- column: formats its input into multiple columns.
	- -s: Specify a set of characters to be used to delimit columns for the -t option.
	- -t: Determine the number of columns the input contains and create a table.
		Columns are delimited with whitespace, by default, or with the characters supplied using the -s option.  Useful for pretty-printing displays.
- tac: cat in reverse(以行为单位)

# OS
time

1. 实际时间(real time): 从command命令行开始执行到运行终止的消逝时间
2. 用户CPU时间(user CPU time): 命令执行完成花费的用户CPU时间,即命令在用户态中执行时间总和
3. 系统CPU时间(system CPU time): 命令执行完成花费的系统CPU时间,即命令在核心态中执行时间总和

其中,用户CPU时间和系统CPU时间之和为CPU时间,即命令占用CPU执行的时间总和.
实际时间要大于CPU时间,因为Linux是多任务操作系统,往往在执行一条命令时, 系统还要处理其它任务

- centos/redhat下查看某个文件或命令属于哪个rpm包: `yum provides /etc/passwd` or `rpm -qf /etc/passwd`
- ubuntu 及衍生版: `dpkg -S whereis` or `dpkg-query -S /usr/bin/whereis`. 具体参考ubuntu wiki skills页面, 不过该命令不如`rpm -qf`强大, 如`/etc/passwd`就查不出.

# 进程
- `ll /proc/PID`
	- cwd: 符号链接的是进程运行目录
	- exe: 符号连接就是执行程序的绝对路径
	- cmdline: 就是程序运行时输入的命令行命令
	- environ: 记录了进程运行时的环境变量
	- fd: 目录下是进程打开或使用的文件的符号连接
- kill: 当给多个进程号的时候, 可以同时杀死多个进行, `kill -s SIGINT pid` 不能在sh, 需要在bash 中启动
- `killall sh`
- `ps x`: An alternate description is that this option causes ps to list all processes owned by you (same EUID as ps)
- `ps -ef` 可以查看pid, ppid
- pstree: shows processes in a tree
- `ps aux`

## 定时任务crontab
cron把命令行保存在/etc/crontab文件里,每个系统用户如果设置了自己的cron,那都会在/var/spool/cron下面有对应用户名的crontab.
无论编写/var/spool/cron目录内的文件还是/etc/crontab文件,都能让cron准确无误地执行安排的任务,
区别是/var/spool/cron下各系统用户的crontab文件是对应用户级别的的任务配置,而/etc/crontab文件则是对应系统级别的任务配置.
cron服务器每分钟读取一次/var/crontab/cron目录内的所有文件和/etc/crontab文件

格式
```bash
# Example of job definition:
# .---------------- minute (0 - 59)
# |  .------------- hour (0 - 23)
# |  |  .---------- day of month (1 - 31)
# |  |  |  .------- month (1 - 12) OR jan,feb,mar,apr ...
# |  |  |  |  .---- day of week (0 - 6) (Sunday=0 or 7) OR sun,mon,tue,wed,thu,fri,sat
# |  |  |  |  |
# *  *  *  *  * user-name command to be executed
```

特别需要注意的是crontab 下的环境变量$PATH 和普通shell 下的 $PATH 是不一样的, 在crontab下, `$PATH=/usr/bin:/bin`, 所以需要特别注意command 的路径

crontab

- `crontab -l`: The current crontab will be displayed on standard output.
- `crontab -e`: edit the current crontab.  After you exit from the editor, the modified crontab will be installed automatically.

eg:

1. `3 3 * * 0 /usr/local/bin/qbbak`: 表示每周日凌晨3时3分执行目录/usr/local/bin中的qbbak文件
1. `* */6 * * * /usr/local/bin/esbbak`: 表示每6小时执行一次,0的时候执行一次, 6的时候执行第二次, 如果协程1-23/6, 那么就是1的时候执行第一次, 7的时候执行第二次
1. `20,30 * * * /usr/local/bin/esbak`: 表示在每小时20分和30分时执行
1. `* 23 * * 2-5 /usr/local/bin/esbbak`: 表示在星期二到星期五每天的23点执行

crontab 中生成时间必须加转义`date +"\%Y\%m\%d"`
