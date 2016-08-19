命令行快捷键:

- CTRL-P: 从history缓冲区召回上一次的命令(在命令行下), 可以按下多次
- CTRL-R: 回溯搜索(Backwards search)history缓冲区内的文本(在命令行下)
- CTRL-T: 交换光标位置与光标的前一个位置的字符内容(在命令行下).比如:echo $var;,假设光标在a上,那么,按下C-T之后,v和a将会交换位置:echo $avr;.
- CTRL-U: 擦除从光标位置开始到行首的所有字符内容
- CTRL-W: 当在控制台或一个xterm窗口敲入文本时, CTRL-W 会删除从在光标处往后(回)的第一个空白符之间的内容.在某些设置里, CTRL-W 删除光标往后(回)到第一个非文字和数字之间的字符.
- CTRL-Y: 将之前已经清除的文本粘贴回来(主要针对CTRL-U或CTRL-W)
- CTRL-/: 撤消操作,Undo.

- `echo *`: 会输出当前目录的文件
- `echo "*"`: 才会输出*

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
```
grep -R --include="*.c" keyword directory
```

- -l参数列出包含指定模式的文件的文件名
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

pgrep  
looks through the currently running processes and **lists the process IDs** which matches the selection criteria to stdout. All the criteria have to match.  
`-n`: Select only the newest (most recently started) of the matching processes.  
`shell> strace -c -p $(pgrep -n php-cgi)`

zgrep: grep within compressed files
zless: look at compressed files

`printf "%05o\n" 65` (5 characters width, padded with zeros)

时间:
```
$ date +%Y%m%d_%H_%M_%S
20150519_16_12_32

$ date +"%F %T"
2015-06-11 15:11:49

$ date "+%Y%m%d"
20160129
$ date -d "1 day ago" "+%Y%m%d"
20160128
```

[cut](http://www.thegeekstuff.com/2013/06/cut-command-examples/)

- `$ cut -c1-3` ## 第1到第3个字符
- `$ cut -c3-` ## 第3个字符一直到最后一个字符
- `$ cut -d':' -f1`
- `$ cut -d':' -f1,6`
- `$ cut -d':' --complement -s -f7` ## Select All Fields Except the Specified Fields

Uppercase to lowercase:
`$ tr '[:upper:]' '[:lower:]' < input` or `tr '[A-Z]' '[a-z]' < input`

`tr -d'\n'` 删除回车符

# 网络
使用netstst命令查看正在监听状态的端口:

- `netstat -anupl`: a表示all, n表示用数字形式显示端口号, u/t,表示UDP/TCP协议类型, p是程序PID, l表示处于监听状态的
- `lsof -i:80`: 这个需要root 权限, 如果有显示说明80端口已经开放了,如果没有显示说明没有开放

**ss**
socket statistics (show apps using the Internet)

iftop: visually show network traffic

## 远程主机相关的
ssh

登陆一台机器, 并在登录后的机器上执行命令
```
ssh user@ip "bash shell.sh" < /dev/null
```
登陆到ip 机器上, 在 ip 上执行bash shell.sh  
但是这个命令的stdout 仍然在发起ssh 命令的机器的stdout, stderr 同样在发起ssh 命令的机器的stderr 上.  
当加上 bash shell.sh 1>out 2>log 时, 才会将stdout, stderr 输出到ip 的机器上.  
也可以使用双引号, 使用双引号可以传入参数, 但是当有很多参数时, 可以通过将这些参数先写到一个本地文件, 然后将这个文件scp 到远程目的机器上,
然后在shell.sh 加入一个source file.conf, 从而可以引入这些变量.  
加上/dev/null 是为了避免可能的干扰

```
cd ~/.ssh
ssh-keygen -t rsa -C "your_email@example.com"
```
Creates a new ssh key, using the provided email as a label
We want the default settings so when asked to enter a file in which to save the key, just press enter.
把生成的公钥加到github中

```
ssh -T git@github.com
ssh -T git@git.oschina.net
```

**putty connection refused error**  
```
sudo service sshd status
```
发现SSH: Could not load host key: `/etc/ssh/ssh_host_rsa_key`  
因为刚安装ssh之后, 这个文件不存在, 我们只是在`$HOME/.ssh` 中产生了key, 而没有在`/etc/ssh`中产生key.  
解决方法:
```
sudo ssh-keygen -A
```

**putty 中文乱码**  
Window->Appearance->Font: 选择Courier New  
Window->Translation->Remote character set: UTF-8  
之后, 在Session->Saved Sessions 中起一个名字, 然后Save, 下次使用的时候, 直接load, 就可以了

如果要从A远程登录到B, 把A 的公钥追加到 B的 `$HOME/.ssh/authorized_keys` 文件中  
但是使用putty 还有一点不同, 具体的操作步骤如下:

1. 按照正常的方式在机器A上生成密钥对, id_rsa and id_rsa.pub 两个文件
1. 打开[puttygen](http://the.earth.li/~sgtatham/putty/latest/x86/puttygen.exe)
1. 通过conversions->import key 导入私钥
1. 点击save private key, 保存到.ssh目录下
1. 在机器上新建`~/.ssh/authorized_keys`, 并修改`~/.ssh`和`~/.ssh/authorized_keys`文件的权限: `chmod 700 ~/.ssh` and `chmod 600 ~/.ssh/authorized_keys`
1. 将putty key generator 界面上的公钥复制到`~/.ssh/authorized_keys`文件中
1. 打开putty, 在connection->ssh->auth 的private key file for authentication中将上面保存的putty专用的密钥写到这里
1. over

**scp**  
secure copy, 用于在Linux下进行远程拷贝文件的命令,和它类似的命令有cp,不过cp只是在本机进行拷贝不能跨服务器,而且scp传输是加密的.可能会稍微影响一下速度.

通过scp.exe 将文件从windows 传输到linux, 如果文件名中含有中文, scp.exe 会将中文从默认的gbk编码转换为utf-8

而如果压缩包里含有中文文件名, 从windows 传输到linux下, 解压之后, 中文文件会乱码.

scp拷贝的另一个命令 rsync就可以实现意外中断后,下次继续传,命令如下: `rsync -av src dst`

- `-av`: 保证src 是dst 的子集
	1. 如果在src 删除了文件, 不会将删除文件的操作同步到 dst
	1. 如果在dst 删除了src 中存在的文件, 那么再次执行 rsync -av 后, 被删除的文件会再次出现在dst
	1. 如果在dst 中增加了文件, 那么再次执行 rsync -av 之后, 增加的文件不会被删除
- `rsync -av a b/`: 将a这个目录整体同步到b下面, 得到 b/a 这样的文件结构
- `rsync -av a/ b/`: 将a下面的文件同步到b下面, 得到 b/files_of_a 这样的文件结构

# File System
垃圾箱位置: .local/share/Trash/

iotop: i/o stats

`du`: 查看目录大小  
查看某个目录的大小 `/home/master/documents`  
查看目录下所有目录的大小并按大小降序排列:`sudo du -sm /etc/* | sort -nr | less`  
`-h` 以适合于human 的格式显示, 他会自动根据文件大小以KB, MB, GB等来显示大小, `-m`以MB为计量单位

如果当前目录下文件和文件夹很多,使用不带参数du的命令,可以循环列出所有文件和文件夹所使用的空间.
这对查看究竟是那个地方过大是不利的,所以得指定深入目录的层数,`--max-depth=`,这是个极为有用的参数.
当指定为0时, 就只查看当前目录的大小

`df`: 查看磁盘使用情况  
于du不同的是,du是面向文件的命令,只计算被文件占用的空间.不计算文件系统metadata 占用的空间.df则是基于文件系统总体来计算,通过文件系统中未分配空间来确定系统中已经分配空间的大小

`dd`: 转换和拷贝文件(包括二进制文件, mbr文件)
参数
```
if=filename:输入的文件名
of=finename:输出的文件名
bs=bytes:一次读写的字节数,默认是512bytes
skip=blocks:拷贝前,跳过的输入文件的前blocks块,块的大小有bs决定
count=blocks:只拷贝输入文件的前blocks块
```
例如,现在有一个文件file,大小为116616字节:
```
[root]# du -b file  
116616  file   
```
将其分割为两文件file1和file2,那我们就设置每块为1024字节,将file的前60块放入file1,余下的放入file2:
```
[root]# dd if=file bs=1024 count=60 skip=0  of=file1
[root]# dd if=file bs=1024 count=60 skip=60 of=file2
第二行仍然写上count=60好像不对, 我去掉这个也是可以运行的, 116616/1024=113.88
```
然后用cat将两个文件合并为file.bak,要注意文件的顺序:
```
[root]# cat file1 file2 > file.bak  
[root]# md5sum file  
3ff53f7c30421ace632eefff36148a70  file  
[root]# md5sum file.bak  
3ff53f7c30421ace632eefff36148a70  file.bak   
```
可以证明两个文件时完全相同的.

**tar and gzip**
```
for GNU tar:   gtar cvzf file.tar.gz filenames
for any tar:   tar cvf -  filenames | gzip > file.tar.gz
```

can get better compression by compressing all members at once:
```
cat file1 file2 | gzip > foo.gz
```
compresses better than
```
gzip -c file1 file2 > foo.gz
```
-c --stdout --to-stdout
Write output on standard output; keep original files unchanged.  
If there are several input files, the output consists of a sequence of independently compressed  members. 
To obtain better compression, concatenate all input files before compressing them.

将foo.tar.gz 解压到/to 目录
```
tar -xvf foo.tar.gz -C /to
```

排除特定文件进行打包
```
dir0/dir1
dir0/dir2
dir0/file
dir0/dir3
```
`tar cvzf all.tar.gz dir0  --exclude dir1 --exclude file`: 排除dir0下面的dir1和file

split - split a file into pieces
`split [OPTION] [INPUT [PREFIX]]`

- -a, --suffix-length=N, use suffixes of length N (default 2)
- -d, --numeric-suffixes, use numeric suffixes instead of alphabetic
- -l, --lines=NUMBER, put NUMBER lines per output file

paste: merge lines of files(以列为对象), With no FILE, or when FILE is -, read standard input

wget
如果是一个目录
`wget  -r ftp://xxx_path_xxx ./`

# OS
**设置默认的runlevel**
编辑 `/etc/inittab` 中设置

**time**  

1. 实际时间(real time): 从command命令行开始执行到运行终止的消逝时间
2. 用户CPU时间(user CPU time): 命令执行完成花费的用户CPU时间,即命令在用户态中执行时间总和
3. 系统CPU时间(system CPU time): 命令执行完成花费的系统CPU时间,即命令在核心态中执行时间总和

其中,用户CPU时间和系统CPU时间之和为CPU时间,即命令占用CPU执行的时间总和.
实际时间要大于CPU时间,因为Linux是多任务操作系统,往往在执行一条命令时, 系统还要处理其它任务
```
# time date
Sun Mar 26 22:45:34 GMT-8 2006
real    0m0.136s
user    0m0.010s
sys     0m0.070s
```

# 进程
通过ps及top命令查看进程信息时,只能查到相对路径,查不到的进程的详细信息,如绝对路径等.这时,我们需要通过以下的方法来查看进程的详细信息:
Linux在启动一个进程时,系统会在/proc下创建一个以PID命名的文件夹,在该文件夹下会有我们的进程的信息,其中包括一个名为exe的文件即记录了绝对路径,通过ll或ls –l命令即可查看.

`ll /proc/PID`

- cwd: 符号链接的是进程运行目录
- exe: 符号连接就是执行程序的绝对路径
- cmdline: 就是程序运行时输入的命令行命令
- environ: 记录了进程运行时的环境变量
- fd: 目录下是进程打开或使用的文件的符号连接

nohup命令:
如果你正在运行一个进程,而且你觉得在退出帐户时该进程还不会结束,那么可以使用nohup命令.该命令可以在你退出帐户/关闭终端之后继续运行相应的进程.nohup就是不挂断的意思( no hang up).
该命令的一般形式为:`nohup command &`  
如果使用nohup命令提交作业,那么在缺省情况下该作业的所有输出都被重定向到一个名为nohup.out的文件中,除非另外指定了输出文件:
```
nohup command > myout.file 2>&1 &
```
在上面的例子中,0 – stdin (standard input),1 – stdout (standard output),2 – stderr (standard error) ,
2>&1是将标准错误(2)重定向到标准输出(&1),标准输出(&1)再被重定向输入到myout.file文件中.
使用 jobs 查看任务.
使用 fg %n　关闭.

**kill**
当给多个进程号的时候, 可以同时杀死多个进行

killall sh

ps x: An alternate description is that this option causes ps to list all processes owned by you (same EUID as ps)

`ps -ef` 可以查看pid, ppid

pstree: shows processes in a tree

```
ps aux
USER       PID %CPU %MEM   VSZ  RSS  TTY                   STAT     START   TIME     COMMAND
```
- VSZ     該进程使用的虚拟內存量(KB),
- RSS     該進程占用的固定內存量(KB)(驻留中页的数量),
- START   該進程被觸發启动时间,
- TIME      該进程實際使用CPU運行的时间,
- COMMAND   命令的名称和参数,

STAT狀態位常見的狀態字符

- D 无法中断的休眠状态(通常 IO 的进程),
- R 正在运行可中在队列中可过行的,
- S 处于休眠状态,
- T 停止或被追踪,
- W 进入内存交换  (从内核2.6开始无效),
- X 死掉的进程   (基本很少見),
- Z 僵尸进程,
- < 优先级高的进程
- N 优先级较低的进程
- L 有些页被锁进内存,
- s 进程的领导者(在它之下有子进程),
- l 多进程的(使用 CLONE_THREAD, 类似 NPTL pthreads),
- + 位于后台的进程组

**定时任务crontab**

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
- `crontab -r`: The current crontab will be removed.
- `crontab -e`: edit the current crontab.  After you exit from the editor, the modified crontab will be installed automatically.

eg:

1. `3 3 * * 0 /usr/local/bin/qbbak`: 表示每周日凌晨3时3分执行目录/usr/local/bin中的qbbak文件
1. `* */6 * * * /usr/local/bin/esbbak`: 表示每6小时执行一次,0的时候执行一次, 6的时候执行第二次, 如果协程1-23/6, 那么就是1的时候执行第一次, 7的时候执行第二次
1. `20,30 * * * /usr/local/bin/esbak`: 表示在每小时20分和30分时执行
1. `* 23 * * 2-5 /usr/local/bin/esbbak`: 表示在星期二到星期五每天的23点执行

## iptables
iptables,但更正确的名称是 iptables／netfilter.
iptables 是一个用户空间的模块.作为用户,你在命令行就是通过它将防火墙规则放进缺省的表里.
netfilter 是一个核心模块,它内置于内核中,进行实际的过滤.

iptables 将规则放进缺省的规则链(INPUT,OUTPUT 及 FORWARD),而所有流量(IP 封包)都会被相关的规则链检查,根据当中的规则判断如何处理每个封包,例如:接纳或丢弃它.
这些动作称为目标,而最常见的两个缺省目标就是 DROP 来丢弃封包,或 ACCEPT 来接纳封包.

规则链

我们可以在过滤表的 3 条缺省规则链内加入规则,来处理通过这些规则链的封包.它们分别是:

- INPUT - 所有以主机为目的地的封包.
- OUTPUT - 所有源自主机的封包.
- FORWARD - 这些封包的目的地或来源地都不是主机,但路经主机(由它选路).假若你的主机是一个路由器,这条规则链将会被应用.

规则是以列表的方式被加进每条规则链.
每个封包会被头一条规则开始检查,才至最后一条.假若封包与其中一条规则吻合,相应的动作便会被执行,例如接纳(ACCEPT)或丢弃(DROP)封包.
一旦有吻合的规则,这个封包便会按照规则来处理,而不再被规则链内的其它规则所检查.
假如封包通过所有检查而不符合任何规则链内的任何一条规则,那应这条规则链的缺省动作将会被执行.这就是所谓的缺省政策,可以设置为接纳(ACCEPT)或丢弃(DROP)封包.

规则链拥有缺省政策这个概念带来两个基本的可能性,而我们必须考虑它们才能决定如何组织我们的防火墙.

1. 我们可以缺省一个政策来丢弃(DROP)所有封包,然后刻意加入规则来接纳(ACCEPT)源自被信任的 IP 地址的封包,或者打开那些提供服务的端口,如:bittorrent,FTP 服务器,网页服务器,Samba 文件服务器等.  
2. 我们可以缺省一个政策来接纳(ACCEPT)所有封包,然后刻意加入规则来拦截(DROP)来自有问题的 IP 地址或系列的封包,也或者阻止封包进出只作私人用途或未提供服务的端口.

普遍来说,第一个方法多数用在 INPUT 规则链,因为我们会希望控制哪些东西可以访问我们的机器,而第二个方法多数用在 OUTPUT 规则链,因为我们多数信赖那些离开(源自)我们机器的封包.

```
## 允许 SSH 通过 tcp 端口 22 来连接.
iptables -A INPUT -p tcp --dport 22 -j ACCEPT 

## 将外来的带有RST 标记的包丢弃
iptables -A OUTPUT -p tcp --tcp-flags RST RST -j DROP 
```

**ubuntu root password**  
安装完Ubuntu后忽然意识到没有设置root密码. Ubuntu的默认root密码是随机的,即每次开机都有一个新的root密码.  
设置一个新的固定的root密码的方法: `$sudo passwd`,然后输入当前用户的密码,enter,终端会提示我们输入新的密码并确认,此时的密码就是root新密码

umask指定的是**该默认值需要减掉的权限**.因为r,w,x分别是4,2,1.  
所以,当要去掉能写的权限,就是输入2,而如果要去掉能读的权限,也就是4,那么要去掉读与写的权限,也就是6,而要去掉执行与写入的权限,也就是3.  
So, when creating a new file, the permission is: `the default - umask = -rw-rw-rw- - 0002 = -rw-rw-r--`

**column**
The column utility formats its input into multiple columns.

- -s: Specify a set of characters to be used to delimit columns for the -t option.
- -t: Determine the number of columns the input contains and create a table.  Columns are delimited with whitespace, by default, or with the characters supplied using the -s option.  Useful for pretty-printing displays.

eg: `mount | column -t`

tac: cat in reverse(以行为单位)

**umask**  
```	
[eric@human ~]$ umask 
0002
[eric@human ~]$ umask -S
u=rwx,g=rwx,o=rx
[eric@human ~]$ touch file
[eric@human ~]$ ll file
-rw-rw-r--. 1 eric eric 0 May 19 12:31 file
[eric@human ~]$ mkdir dir
[eric@human ~]$ ll
drwxrwxr-x. 2 eric eric    4096 May 19 12:39 dir
```	

奇怪的是,怎么umask会有4组数字啊?第一组是特殊权限用的,我们看后面3组即可.

在默认权限的属性上,目录与文件是不一样的.由于我们不希望文件具有可执行的权力,默认情况中,文件是没有可执行(x)权限的.因此:  

- 若用户建立为"文件"则默认"没有可执行(x)项目",即只有rw这两个项目,也就是最大为666分,默认属性如下: `-rw-rw-rw-`
- 若用户建立为"目录",则由于x与是否可以进入此目录有关,因此默认为所有权限均开放,即为777分,默认属性如下: `drwxrwxrwx`

**tkgvizmakefile**  
create Tk graphs from Makefiles

tkgvizmakefile -f Makefile -T ps -o graph.ps

[pythonbrew](http://www.pythoner.cn/home/blog/python-version-switch-pythonbrew/)  
nbrew 是用来管理多版本 python 的利器一枚  
然后在你的 `~/.bashrc`文件中追加下面这一行,重启终端即可:

[[ -s "$HOME/.pythonbrew/etc/bashrc" ]] && source "$HOME/.pythonbrew/etc/bashrc"

- 列出可安装的 python 版本:pythonbrew list --know
- 安装某个版本的 python: pythonbrew install 2.7.3
- 删除已安装的某版本的 python: pythonbrew uninstall 2.7.3
- 列出已安装的 python 版本(当前使用的版本后会用星号标记): pythonbrew list
- 使用某个版本的 python (仅当前终端窗口有效): pythonbrew use 2.7.3
- 切换到某个版本的 python (一直有效): pythonbrew switch 2.7.3
- 清理陈旧的源码目录和档案包: pythonbrew cleanup
- 升级到pythonbrew到最新版本: pythonbrew update
- 禁用pythonbrew(即切换回原始环境): pythonbrew off

# [ffmpeg](http://blog.csdn.net/jixiuffff/article/details/5709976)
格式转换(将file.avi 转换成output.flv)
`ffmpeg -i file.avi output.flv`

[doc](https://www.ffmpeg.org/ffmpeg.html)

To refer to input files in options, you must use their indices(0-based). 
E.g. the first input file is 0, the second is 1, etc. 
Similarly, streams within a file are referred to by their indices. E.g. 2:3 refers to the fourth stream in the third input file. 

By default, ffmpeg includes only one stream of each type (video, audio, subtitle) present in the input files and adds them to each output file.
You can disable some of those defaults by using the -vn/-an/-sn options.

-codec:a:1 ac3 contains the a:1 stream specifier, which matches the second audio stream. Therefore, it would select the ac3 codec for the second audio stream.
-b:a 128k matches all audio streams.

## map
select the stream with index 2 from input file a.mov and stream with index 6 from input b.mov, and copy them to the output file out.mov:
`ffmpeg -i a.mov -i b.mov -c copy -map 0:2 -map 1:6 out.mov`

To select all video and the third audio stream from an input file:
`ffmpeg -i INPUT -map 0:v -map 0:a:2 OUTPUT`

## 合并视频与音频
现在有个视频video.avi,有个音频 audio.mp3,将其合并成output.avi
```
ffmpeg -i video.avi -vcodec copy -an video2.avi   
ffmpeg -i video2.avi -i audio.mp3 -vcodec copy -acodec copy output.avi
```
- -i 表示输入文件
- -vcodec copy: vcodec codec(output), set the video codec, an alias for -codec:v
- -acodec copy 这个说的应该是音频跟上面一样
- -an: 表示disable audio recording, 表示去掉video.avi 原有的音频, 相应的-vn, 表示disable video recording

Stream copy is a mode selected by supplying the copy parameter to the -codec option. 
It makes ffmpeg omit the decoding and encoding step for the specified stream, so it does only demuxing and muxing. 
It is useful for changing the container format or modifying container-level metadata. 

## 声音与视频的分离

1. `ffmpeg -i 人生若只如初见.flv -vn r.mp3`: 从flv 文件中提取声音并保存为mp3 格式
1. `ffmpeg -i 人生若只如初见.flv -an r.flv`: 只留视频不留声音

如果你觉得mp3 文件有点大, 想变小一点那么可以通过-ab 选项改变音频的比特率(bitrate)  
`ffmpeg -i input.mp3 -ab 128 output.mp3`    //这里将比特率设为128

可以用file 命令查看一下源文件的信息

## 切头去尾
`ffmpeg -ss 00:00:10 -t 00:01:22 -i 五月天-突然好想你.mp3  output.mp3` 只要从第10秒开始截取,共截取1:22时长的内容

## 视频文件的连接
好像必须先将文件转成mpg, dv 等格式的文件后才能进行连接

连接复数的AVI影片档之范例(在此范例中须一度暂时将AVI档转换成MPEG-1档(MPEG-1, MPEG-2 PS, DV格式亦可连接))
```
## 上面将input1.avi, input2.avi 合并成outputfile.avi
ffmpeg -i input1.avi -sameq inputfile_01.mpg -r 20
ffmpeg -i input2.avi -sameq inputfile_02.mpg  -r 20
cat inputfile_01.mpg inputfile_02.mpg > inputfile_all.mpg
ffmpeg -i inputfile_all.mpg -sameq outputfile.avi
```
- -sameq 表示相同的质量(可能指的是画面, 不太清楚)
- -r 指频率, frame rate

如果你的文件数量太大，rm提示超出参数列表长度的话，可以用下列命令来删除：`find . -name "*" -exec rm {} \; -print`

