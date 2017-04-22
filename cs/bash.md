命令行快捷键:

- CTRL-P: 从history缓冲区召回上一次的命令(在命令行下), 可以按下多次
- CTRL-R: 回溯搜索(Backwards search)history缓冲区内的文本(在命令行下)
- CTRL-T: 交换光标位置与光标的前一个位置的字符内容(在命令行下).比如:echo $var;,假设光标在a上,那么,按下C-T之后,v和a将会交换位置:echo $avr;.
- CTRL-U: 擦除从光标位置开始到行首的所有字符内容
- CTRL-W: 当在控制台或一个xterm窗口敲入文本时, CTRL-W 会删除从在光标处往后(回)的第一个空白符之间的内容.在某些设置里, CTRL-W 删除光标往后(回)到第一个非文字和数字之间的字符.
- CTRL-Y: 将之前已经清除的文本粘贴回来(主要针对CTRL-U或CTRL-W)
- CTRL-/: 撤消操作,Undo.

- `echo *`: 会输出当前目录的文件
- `echo "*"`: 才会输出`*`

- `watch -d -n 1 command`: 间隔1秒钟, 执行一次command, 并将结果的diff 高亮显示

# Help
man

- `man number item`: find item in section number, eg: `man 3 fopen`  
- `man -k word`: 关键字查找, 查找包含word(命令本身或者解释中) 的command  
- `man -f word`: 根据关键字在联机帮助中搜索完全匹配的条目, Equivalent to whatis  

把man page 转成文本件,如: `man ls | col -b > ~/Desktop/man_ls.txt`  
`man -t ls > man_ls.ps && ps2pdf man_ls.ps && rm man_ls.ps`: print

## 查找
[find](http://os.51cto.com/art/200908/141119_all.htm)的使用格式如下:
```
$ find <指定目录> <指定条件> <指定动作>
- <指定目录>: 所要搜索的目录及其所有子目录.默认为当前目录.
- <指定条件>: 所要搜索的文件的特征.
- <指定动作>: 对搜索结果进行特定的处理.
```
如果什么参数也不加,find**默认搜索当前目录及其子目录**,并且不过滤任何结果(也就是返回所有文件),将它们全都显示在屏幕上.
```
$ find . -name 'my*' 搜索当前目录(含子目录,以下同)中,所有文件名以my开头的文件.
$ find . -name 'my*' -ls 搜索当前目录中,所有文件名以my开头的文件,并显示它们的详细信息.
find . -name "*.txt" -print
$ find . -type f -mmin -10 搜索当前目录中,所有过去10分钟中更新过的普通文件.如果不加-type f参数,则搜索普通文件+特殊文件+目录.
$ find . ! -type d -print 在当前目录下查找除目录以外的所有类型的文件
```

指定条件
```
-name 按照文件名查找文件.
-iname 按照文件名(不区分大小写)查找文件.
-perm 按照文件权限来查找文件.
-user 按照文件属主来查找文件.
-group 按照文件所属的组来查找文件.
-mtime -n +n 按照文件的更改时间来查找文件, - n表示文件更改时间距现在n天以内,+ n表示文件更改时间距现在n天以前.
-newer file1 ! file2 查找更改时间比文件file1新但比文件file2旧的文件.
-type 查找某一类型的文件,诸如:
    b - 块设备文件.
    d - 目录.
    c - 字符设备文件.
    p - 管道文件.
    l - 符号链接文件.
    f - 普通文件.
```

使用find命令时,只要把想要的操作写在一个文件里,就可以用exec来配合find命令查找,很方便的

```
find . -type f -exec ls -l {} \;
输出当前目录下的所有普通文件的属性信息
 \ 的意思,是作为转义字符来用的, 且 \ 与 ; 之间不能有空格

与下面的命令效果一样
find . -type f | xargs ls -l
```
在使用find命令的-exec选项处理匹配到的文件时, find命令将所有匹配到的文件一起传递给exec执行.
但有些系统对能够传递给exec的命令长度有限制,这样在find命令运行几分钟之后,就会出现溢出错误.错误信息通常是"参数列太长"或"参数列溢出".
这就是xargs命令的用处所在,特别是与find命令一起使用.

find命令把匹配到的文件传递给xargs命令,而xargs命令每次只获取一部分文件而不是全部,不像-exec选项那样.
这样它可以先处理最先获取的一部分文件,然后是下一批,并如此继续下去.

在有些系统中,使用-exec选项会为处理每一个匹配到的文件而发起一个相应的进程,并非将匹配到的文件全部作为参数一次执行,这样在有些情况下就会出现进程过多,系统性能下降的问题,因而效率不高,

而使用xargs命令则只有一个进程.另外,在使用xargs命令时,究竟是一次获取所有的参数,还是分批取得参数,以及每一次获取参数的数目都会根据该命令的选项及系统内核中相应的可调参数来确定.

locate命令其实是"find -name"的另一种写法,但是要比后者快得多,原因在于它不搜索具体目录,而是搜索一个数据库(/var/lib/locatedb), 使用updatedb 来更新这个数据库

grep  
Normally, the exit status is 0 if selected lines are found and 1 otherwise. 
But the exit status is 2 if an error occurred, unless the -q or --quiet or --silent option is used and a selected line is found

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

时间:
```bash
date +%Y%m%d_%H%M%S ## 20150519_161232
date +%Y%m%d        ## 显示前天年月日
date +%Y%m%d --date="+1 day"  ## 显示后一天的日期, day, week, month, year
```

Uppercase to lowercase:
`$ tr '[:upper:]' '[:lower:]' < input` or `tr '[A-Z]' '[a-z]' < input`

`tr -d'\n'` 删除回车符

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
	- `rsync -av a/ b/`: 将a下面的文件同步到b下面, 得到 b/files_of_a 这样的文件结构
	- `--exclude ignore`: 忽略src/ignore
- wget 一个目录: `wget  -r ftp://xxx_path_xxx ./`

## 远程主机相关的
`ssh -o StrictHostKeyChecking=no user@ip "bash shell.sh" < /dev/null` 登陆一台机器, 并在登录后的机器上执行命令.
但是这个命令的stdout 仍然在发起ssh 命令的机器的stdout, stderr 同样在发起ssh 命令的机器的stderr 上.  
当加上 `bash shell.sh 1>out 2>log` 时, 才会将stdout, stderr 输出到ip 的机器上.  
加上/dev/null 是为了避免可能的干扰

添加ssh 公钥到github

1. 产生一个新的密钥对(如果已有, 这步可以跳过) `cd ~/.ssh; ssh-keygen -t rsa -C "your_email@example.com"`
1. 把生成的公钥加到github中
1. 测试添加是否成功: `ssh -T git@github.com`

putty connection refused error
```
sudo service sshd status
```
发现SSH: Could not load host key: `/etc/ssh/ssh_host_rsa_key`  
因为刚安装ssh之后, 这个文件不存在, 我们只是在`$HOME/.ssh` 中产生了key, 而没有在`/etc/ssh`中产生key.  
解决方法:
```
sudo ssh-keygen -A
```

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
- 如果文件数量太大，rm提示超出参数列表长度的话，可以用下列命令来删除：`find . -name "*" -exec rm {} \; -print`
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
```
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

