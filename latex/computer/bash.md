# 文字编辑
**strings**  
returns each string of printable characters in files. Its main uses are to determine the contents of and to extract text from binary files (i.e., non-text files).

**xargs**  
将参数列表转换成小块分段传递给其他命令,以避免参数列表过长的问题  
Commands like `grep` and `awk` can accept the standard input as a parameter, or argument by using a pipe. 
However, others like `cp` and `echo` disregard the standard input stream and rely solely on the arguments found after the command. 
Additionally, under the Linux kernel before version 2.6.23, arbitrarily long lists of parameters could not be passed to a command, 
so `xargs` breaks the list of arguments into sublists small enough to be acceptable.

下面的命令:  
	
	rm `find /path -type f`
如果path目录下文件过多就会因为"参数列表过长"而报错无法执行.但改用xargs以后,问题即获解决.

	find /path -type f -print0 | xargs -0 rm
本例中xargs将find产生的长串文件列表拆散成多个子串,然后对每个子串调用rm.这样要比如下使用find命令效率高的多

**grep**  
在directory及其子目录下的c 文件中查找keyword

	grep -R --include="*.c" keyword directory

`-B num` to set how many lines before the match and `-A num` for the number of lines after the match, for example:

	grep -B 3 -A 2 foo README.txt

If you want the same amount of lines before and after you can use `-C num`.

	grep -C 3 foo README.txt

This will show 3 lines before and 3 lines after.

**umask**  
	
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

奇怪的是,怎么umask会有4组数字啊?第一组是特殊权限用的,我们看后面3组即可.

在默认权限的属性上,目录与文件是不一样的.由于我们不希望文件具有可执行的权力,默认情况中,文件是没有可执行(x)权限的.因此:  

- 若用户建立为"文件"则默认"没有可执行(x)项目",即只有rw这两个项目,也就是最大为666分,默认属性如下:
`-rw-rw-rw-`
- 若用户建立为"目录",则由于x与是否可以进入此目录有关,因此默认为所有权限均开放,即为777分,默认属性如下:
`drwxrwxrwx`

umask指定的是**该默认值需要减掉的权限**.因为r,w,x分别是4,2,1.  
所以,当要去掉能写的权限,就是输入2,而如果要去掉能读的权限,也就是4,那么要去掉读与写的权限,也就是6,而要去掉执行与写入的权限,也就是3.  
So, when creating a new file, the permission is: `the default - umask = -rw-rw-rw- - 0002 = -rw-rw-r--`

**column**
The column utility formats its input into multiple columns.

- -s: Specify a set of characters to be used to delimit columns for the -t option.
- -t: Determine the number of columns the input contains and create a table.  Columns are delimited with whitespace, by default, or with the characters supplied using the -s option.  Useful for pretty-printing displays.

eg: `mount | column -t`

tac: cat in reverse(以行为单位)

# 程序
**tkgvizmakefile**  
create Tk graphs from Makefiles

	tkgvizmakefile -f Makefile -T ps -o graph.ps

[**pythonbrew**](http://www.pythoner.cn/home/blog/python-version-switch-pythonbrew/)  
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

# 网络
**ss**
socket statistics (show apps using the Internet)

iftop: visually show network traffic

## 远程主机相关的
**ssh**  

	cd ~/.ssh
	ssh-keygen -t rsa -C "your_email@example.com"
Creates a new ssh key, using the provided email as a label
We want the default settings so when asked to enter a file in which to save the key, just press enter.
把生成的公钥加到github中

	ssh -T git@github.com
	ssh -T git@git.oschina.net

**putty connection refused error**  
	
	sudo service sshd status
发现SSH: Could not load host key: `/etc/ssh/ssh_host_rsa_key`  
因为刚安装ssh之后, 这个文件不存在, 我们只是在`$HOME/.ssh` 中产生了key, 而没有在`/etc/ssh`中产生key.  
解决方法:
	
	sudo ssh-keygen -A

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

**PSCP**, the PuTTY Secure Copy client, and it is a command line application

The format for pscp is straight forward:

	pscp [options] source destination

To copy a Windows file to Linux system, at the DOS prompt, type

	pscp c:/music.mp3  ubuntu@10.0.0.3:/home/ubuntu/Music

The reverse works as well (copy Linux file to Windows)

	pscp ubuntu@10.0.0.3:/home/ubuntu/Music/music.mp3 c:/

**scp**  
secure copy, 用于在Linux下进行远程拷贝文件的命令,和它类似的命令有cp,不过cp只是在本机进行拷贝不能跨服务器,而且scp传输是加密的.可能会稍微影响一下速度.

通过scp.exe 将文件从windows 传输到linux, 如果文件名中含有中文, scp.exe 会将中文从默认的gbk编码转换为utf-8

而如果压缩包里含有中文文件名, 从windows 传输到linux下, 解压之后, 中文文件会乱码.

1. 获取远程服务器上的文件

		scp -P 2222 root@www.vpser.net:/root/lnmp0.4.tar.gz /home/lnmp0.4.tar.gz
上端口大写P 为参数,2222 表示更改SSH端口后的端口,如果没有更改SSH端口可以不用添加该参数. root@www.vpser.net 表示使用root用户登录远程服务器www.vpser.net,:/root/lnmp0.4.tar.gz 表示远程服务器上的文件,最后面的/home/lnmp0.4.tar.gz表示保存在本地上的路径和文件名.

2. 获取远程服务器上的目录

		scp -P 2222 -r root@www.vpser.net:/root/lnmp0.4/ /home/lnmp0.4/
上端口大写P 为参数,2222 表示更改SSH端口后的端口,如果没有更改SSH端口可以不用添加该参数.-r 参数表示递归复制(即复制该目录下面的文件和目录),root@www.vpser.net 表示使用root用户登录远程服务器www.vpser.net,:/root/lnmp0.4/ 表示远程服务器上的目录,最后面的/home/lnmp0.4/表示保存在本地上的路径.

3. 将本地文件上传到服务器上

		scp -P 2222 /home/lnmp0.4.tar.gz root@www.vpser.net:/root/lnmp0.4.tar.gz
上端口大写P 为参数,2222 表示更改SSH端口后的端口,如果没有更改SSH端口可以不用添加该参数. /home/lnmp0.4.tar.gz表示本地上准备上传文件的路径和文件名.root@www.vpser.net 表示使用root用户登录远程服务器www.vpser.net,:/root/lnmp0.4.tar.gz 表示保存在远程服务器上目录和文件名.

4. 将本地目录上传到服务器上

		scp -P 2222 -r /home/lnmp0.4/ root@www.vpser.net:/root/lnmp0.4/
上 端口大写P 为参数,2222 表示更改SSH端口后的端口,如果没有更改SSH端口可以不用添加该参数.-r 参数表示递归复制(即复制该目录下面的文件和目录),/home/lnmp0.4/表示准备要上传的目录,root@www.vpser.net 表示使用root用户登录远程服务器www.vpser.net,:/root/lnmp0.4/ 表示保存在远程服务器上的目录位置.

**hostname centos 7**  
sudo hostnamectl set-hostname human.earth
sudo hostnamectl status

# File System
iotop: i/o stats

`du`: 查看目录大小  
查看某个目录的大小 `/home/master/documents`  
查看目录下所有目录的大小并按大小降序排列:`sudo du -sm /etc/* | sort -nr | less`  
`-h` 以适合于human 的格式显示, 他会自动根据文件大小以KB, MB, GB等来显示大小, `-m`以MB为计量单位

`df`: 查看磁盘使用情况  
于du不同的是,du是面向文件的命令,只计算被文件占用的空间.不计算文件系统metadata 占用的空间.df则是基于文件系统总体来计算,通过文件系统中未分配空间来确定系统中已经分配空间的大小

**pgrep**  
`pgrep` looks through the currently running processes and **lists the process IDs** which matches the selection criteria to stdout. All the criteria have to match.  
`-n`: Select only the newest (most recently started) of the matching processes.
`shell> strace -c -p $(pgrep -n php-cgi)`

zgrep: grep within compressed files
zless: look at compressed files

`dd`: 转换和拷贝文件(包括二进制文件)
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

# OS
**设置默认的runlevel**
编辑 `/etc/inittab` 中设置

**开机自动启动**  
例如要开机后自动启动mysql,apache,vsftpd服务,用以下命令即可:

	chkconfig mysqld on

要关闭自动启动的话,把on改为off就行了

	chkconfig mysqld off

不过要注意的是,如果某个服务尚未被添加到chkconfig列表中,则现需要使用–add参数将其添加进去:

	chkconfig –add postfix

如果要查询当前自动启动的服务,可以输入:

	chkconfig –list
	chkconfig –list httpd

打开terminal的命令方式:  
`Alt+F2`后在出现"运行应用程序"中输入`x-terminal-emulator`(一般在你输入到`x-term`后系统会自己显示全部)或者输入`gnome-terminal`

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

1. `9 9 * * * /usr/local/bin/cvsb`
表示每天早上9点9分执行目录/usr/local/bin/中的cvsb文件
2. `3 3 * * 0 /usr/local/bin/qbbak`
表示每周日凌晨3时3分执行目录/usr/local/bin中的qbbak文件
3. `* */6 * * * /usr/local/bin/esbbak`
表示每6小时执行一次,执行时间从第一次执行起计算,当然也可以自定义执行时间,比如0 */6 * * *那程序就会在整点执行
4. `20,30 * * * /usr/local/bin/esbak`
表示在每小时20分和30分时执行
5. `* 23 * * 2-5 /usr/local/bin/esbbak`
表示在星期二到星期五每天的23点执行

**kill**
当给多个进程号的时候, 可以同时杀死多个进行

pstree: shows processes in a tree

# 杂项
**man**  
`man number item`: find item in section number, eg: `man 3 fopen`  
`man -k word`: 关键字查找, 查找包含word(命令本身或者解释中) 的command  
`man -f word`: 根据关键字在联机帮助中搜索完全匹配的条目, Equivalent to whatis  
把man page 转成文本件,如: `man ls | col -b > ~/Desktop/man_ls.txt`  
`man -t ls > man_ls.ps && ps2pdf man_ls.ps && rm man_ls.ps`: print

**ubuntu root password**  
安装完Ubuntu后忽然意识到没有设置root密码. Ubuntu的默认root密码是随机的,即每次开机都有一个新的root密码.  
设置一个新的固定的root密码的方法: `$sudo passwd`,然后输入当前用户的密码,enter,终端会提示我们输入新的密码并确认,此时的密码就是root新密码

**rpm 解包**  
RPM包括是使用cpio格式打包的,因此可以先转成cpio然后解压,如下所示:

	rpm2cpio xxx.rpm | cpio -div

# systemd
**修改默认运行级别**  
以前Fedora版本中修改运行级别方法为:  
以root身份登入,编辑`/etc/inittab`文件  
找到 `id:5:initdefault:`这一行,将默认运行级别5(图形模式)改为3(文本模式),即 `id:3:initdefault:`  
然后重启

但Fedora15之后使用systemd创建符号链接指向默认运行级别

	rm /etc/systemd/system/default.target
	ln -sf /lib/systemd/system/multi-user.target /etc/systemd/system/default.target
	reboot

启动

	#systemctl start mysqld.service
	#systemctl restart mysqld.service
	
	设置开机自启动, 在新版的REHL中使用mariadb
	#systemctl enable mysqld.service
	
	停止开机自启动
	#systemctl disable mysqld.service
	
	#systemctl status mysqld.service
与原来的chkconfig可以兼容, `chkconfig mysqld off`
