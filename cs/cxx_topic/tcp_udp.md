# TCP
Note that TCP does not guarantee that the data will be received by the other endpoint, as this is impossible. It delivers data to the other endpoint if possible, and notifies the user (by giving up on retransmissions and breaking the connection) if it is not possible. Therefore, TCP cannot be described as a 100% reliable protocol; it provides **reliable delivery of data or reliable notification of failure.**

**socket中TCP的三次握手建立连接**  
![socket中TCP的三次握手建立连接详解](http://images.cnblogs.com/cnblogs_com/skynet/201012/201012122157467258.png)  
从图中可以看出:

1. 当客户端调用connect时,触发了连接请求,向服务器发送了SYN J包,这时connect进入阻塞状态.  
1. 服务器监听到连接请求,即收到SYN J包,调用accept函数接收请求向客户端发送SYN K ,ACK J+1,这时accept进入阻塞状态.  
1. 客户端收到服务器的SYN K ,ACK J+1之后,这时connect返回,并对SYN K进行确认,服务器收到ACK K+1时,accept返回,至此三次握手完毕,连接建立.

客户端的`connect`在三次握手的第二个次返回,而服务器端的`accept`在三次握手的第三次返回.

内核为任何一个给定的监听套接字维护两个队列:

1. **未完成连接队列(incomplete connection queue)**: 每个这样的SYN 分节对应其中一项:
已由某个客户发出并到达服务器,而服务器正在等待完成相应的TCP三路握手过程.这些套接字处于SYN_RCVD状态
2. **已完成连接队列(completed connection queue)**, 每个已完成TCP 三路握手过程的客户对应其中一项. 这些套接字处于established 状态

当来自客户的SYN到达时,TCP在未完成连接队列中创建一个新项.  
如果三路握手正常完成, 该项就从未完成连接队列移到已完成连接队列的末尾.  
当进程调用accept时,已完成连接队列中的队头项将返回给进程,或者如果该队列为空,那么进程将被投入睡眠,直到TCP 在该队列中放入一项才唤醒它

**TCP FLAG 标记**  
基于标记的TCP包匹配经常被用于过滤试图打开新连接的TCP数据包.  
TCP标记和他们的意义如下所列:

* F : FIN - 结束; 结束会话
* S : SYN - 同步; 表示开始会话请求
* R : RST - 复位;中断一个连接
* P : PUSH - 推送; 数据包立即发送
* A : ACK - 应答
* U : URG - 紧急
* E : ECE - 显式拥塞提醒回应
* W : CWR - 拥塞窗口减少
 
## 异常情况分析
[针对TCP连接异常断开的分析](https://www.cnblogs.com/549294286/p/5208357.html)
[code](https://gist.github.com/ericuni/4d6eb09a6af73d15fa3004707c958f32)

### 服务器进程终止
1. 在同一个主机上启动服务器和客户, 并在客户上输入一行文本, 以验证一切正常. 正常情况下, 改行文本将由服务器回射给客户.

	```
	[root@dev:git]$ ./client
	hello
	hello

	[root@dev:git]$ netstat -nap | grep tcp
	tcp        0      0 0.0.0.0:9998            0.0.0.0:*               LISTEN       178/server
	tcp        0      0 127.0.0.1:9998          127.0.0.1:33050         ESTABLISHED  180/server
	tcp        1      0 127.0.0.1:33050         127.0.0.1:9998          ESTABLISHED  179/client
	```
1. 找到服务器子进程的ID,通过kill命令杀死它.作为进程终止处理的部分工作,子进程中所有打开着的描述字都被关闭.这就导致向客户发送一个FIN,而客户TCP则响应以一个ACK.这就是TCP连接终止的前一半工作.

	```
	[root@dev:git]$ ./server
	child 180 exit
	```
1. 子进程终止时,内核将给父进程递交SIGCHLD信号.
1. 客户上没有发生任何特殊之事.客户TCP接受来自服务器TCP的FIN并响应一个ACK,然后问题是客户进程此时阻塞在fgets调用上,等待从终端接受一行文本.它是看不到这个FIN的.
1. 此时我们如果运行netstat命令,可以看到如下的套接口的状态, `FIN_WAIT2`即为我们杀掉的那个子进程的,因为我们知道主动关闭的那端在发送完fin并接受对端的ack后将进入`fin_wait2`状态,此时它在等待对端的fin.

	```
	[root@dev:git]$ netstat -nap | grep tcp
	tcp        0      0 0.0.0.0:9998            0.0.0.0:*               LISTEN      178/server
	tcp        0      0 127.0.0.1:9998          127.0.0.1:33050         FIN_WAIT2   -
	tcp        1      0 127.0.0.1:33050         127.0.0.1:9998          CLOSE_WAIT  179/client
	```
1. 现在我们在客户上在输入一行文本,我们可以看到如下的输出

	```
	[root@dev:git]$ ./client
	hello again
	server term prematurely.
	```
1. 当我们输入"hello again"时,客户TCP接着把数据发送给服务器,TCP允许这么做,因为客户TCP接受到FIN只是表示服务器进程已关闭了连接的服务端,从而不再往其中发送任何数据而已.
	FIN的接受并没有告知客户TCP服务器进程已经终止(在这个例子中它确实是终止了).当服务器TCP接收到来自客户的数据时,既然先前打开那个套接口的进程已经终止,于是响应一个RST.
1. 然而客户进程看不到这个RST,因为它在调用write后立即调用read,并且由于第2步中接收到FIN,所调用的read立即返回0(表示)EOF.我们的客户此时并未预期收到EOF,于是以出错信息"server term prematurely."(服务器过早终止)退出.
	- 我们的上述讨论还取决于程序的时序.客户调用read既可能发生在服务器的RST被客户收到之前,也可能发生在收到之后.
		如果read发生在收到RST之前(如本例子所示),那么结果是客户得到一个未预期的EOF,否则结果是由readline返回一个ECONNRESET("connection reset by peer"对方复位连接)错误.
1. 当客户终止时,它所有打开着的描述字都被关闭.

本例子的问题在于:当FIN到达套接口时,客户正阻塞在fgets调用上.客户实际上在应对两个描述字-套接口和用户输入,它不能单纯阻塞在这两个源中某个特定源的输入上,而是应该阻塞在其任何一个源的输入上.(可用select等io复用的函数实现)

### 服务器主机崩溃
1. 当服务器主机崩溃时,已有的网络连接上发不出任何东西.这里我们假设的是主机崩溃,而不是执行了关机命令.
2. 我们在客户上输入一行文本,它由write写入内核,再由客户TCP作为一个数据分节送出.客户随后阻塞于read调用,等待服务器的应答.
3. 这种情况下,客户TCP持续重传数据分节,试图从服务器上接受一个ACK.(源自Berkeley的实现重传该数据分节12次,共等待约9分钟才放弃重传.)
	当客户TCP最终放弃时(假设这段时间内,服务器主机没有重新启动或者如果是服务器主机为崩溃但从网络上不可达的情况,那么假设主机仍然不可达),返回客户进程一个错误.既然客户阻塞在read调用上,该调用将返回一个错误.
	假设服务器已崩溃,从而对客户的数据分节根本没有响应,那么所返回的错误是ETIMEDOUT.
	然而如果某个中间路由器判定服务器主机已不可达, 从而响应以一个"destination unreachable",那么所返回的错误是EHOSTUNREACH或ENETUNREACH.
    尽管我们的客户最后还是发现对端主机已崩溃或不可达,不过有时候我们需要更快地检测出这种情况,而不是不得不等待9分钟.所用的方法就是对read调用设置一个超时.
    另外我们刚讨论的情形只有在向服务器主机发送数据时,才能检测出它已经崩溃,如果我们不主动发送主句也想检测出服务器主机的崩溃,那么就需要用到SO_KEEPALIVE这个套接口选项.

### 服务器主机崩溃后重启
在前一节的分析中,当我们发送数据时,服务器主机仍然处于崩溃状态,这节,我们将在发送数据前重新启动崩溃了的服务器主机.
模拟这种情况的简单方法就是:建立连接,再从网络上断开服务器主机,将它关机后再重启,最后把它重新连接到网络中.
如前一节所述,如果在服务器主机崩溃时客户不主动给服务器发送数据,那么客户不会知道服务器主机已经崩溃.

所发生的步骤如下:

1. 启动客户服务器,在客户上输入一行文本已确认连接已建立.
2. 服务器主机崩溃并重启.
3. 在客户上输入一行文本,它将作为一个TCP数据分节发送到服务器主机.
4. 当服务器主机崩溃后重启时,它的TCP丢失了崩溃前的所有连接信息,因此服务器TCP对于所收到的来自客户的数据分节响应以一个RST.
5. 当客户TCP收到该RST时,客户正阻塞于read调用,导致该调用返回ECONNRESET错误.

### 服务器主机关机
Unix系统关机时, init进程通常先给所有进程发送SIGTERM信号(该信号可被捕获),再等待一段固定的时间(一般在5~20秒之间),然后给所有仍在运行的进程发送SIGKILL信号(该信号不能被捕获).
这么做是留给所有运行中的进程一小段时间来清除和终止.如果我们不捕获SIGTERM信号并终止,我们的服务器将由SIGKILL信号终止.
当服务器进程终止时,它的所有打开着的描述字都被关闭,随后发生的步骤与第一节中讨论过的一样.
正如第一节中所述的情形,我们必须在客户中使用select或poll函数,使得服务器进程的终止已经发生,客户马上检测到.

### 关于SIGPIPE导致的程序退出
当server close一个连接时, server 会向client 发送一个FIN, client read 会返回0. 通过read 返回0, 我们就能知道对方已经关闭了连接, 通常这时候我们就需要关闭本端的连接.
如果client 端接着发数据, 根据TCP协议的规定, 会收到一个RST响应(同时errno 会被置为104).
如果client 再往server发送数据时, 系统会发出一个SIGPIPE信号给client进程(同时errno 会被置为32), 告诉进程这个连接已经断开了, 不要再写了.

SIGPIPE信号的默认执行动作是terminate(终止,退出),所以client会退出. 若不想客户端退出可以把SIGPIPE设为`SIG_IGN`, `signal(SIGPIPE, SIG_IGN);`

服务器采用了fork的话,要收集垃圾进程,防止僵尸进程的产生,可以这样处理:
```C++
signal(SIGCHLD, SIG_IGN); // 交给系统init去回收.
```
这里子进程就不会产生僵尸进程了.

errno

- errno 32: Broken pipe
- errno 104: Connection reset by peer

**长连接的情况下出现了不同程度的延时**  
在一些长连接的条件下, 发送一个小的数据包, 结果会发现从数据write成功到接收端需要等待一定的时间后才能接收到, 而改成短连接这个现象就消失了
(如果没有消失,那么可能网络本身确实存在延时的问题,特别是跨机房的情况下).  
在长连接的处理中出现了延时, 而且时间固定,基本都是40ms, 出现40ms延时最大的可能就是由于没有设置`TCP_NODELAY`.  
在长连接的交互中,有些时候一个发送的数据包非常的小,加上一个数据包的头部就会导致浪费,而且由于传输的数据多了,就可能会造成网络拥塞的情况,
在系统底层默认采用了Nagle算法,可以把连续发送的多个小包组装为一个更大的数据包然后再进行发送.
但是对于我们交互性的应用程序意义就不大了,在这种情况下我们发送一个小数据包的请求,就会立刻进行等待,不会还有后面的数据包一起发送,
这个时候Nagle算法就会产生负作用,在我们的环境下会产生40ms的延时,这样就会导致客户端的处理等待时间过长, 导致程序压力无法上去.
在代码中无论是服务端还是客户端都是建议设置这个选项,避免某一端造成延时.所以对于长连接的情况我们建议都需要设置`TCP_NODELAY`.  
对于服务端程序而言, 采用的模式一般是bind -> listen -> accept, 这个时候accept出来的句柄的各项属性其实是从listen的句柄中继承,
所以对于多数服务端程序只需要对于listen进行监听的句柄设置一次`TCP_NODELAY`就可以了,不需要每次都accept一次.

# UDP
We also say that UDP provides a connectionless service, as there need not be any long-term relationship between a UDP client and server. 
For example, a UDP client can create a socket and send a datagram to a given server and then immediately send another datagram on the same socket to a different server. 
Similarly, a UDP server can receive several datagrams on a single UDP socket, each from a different client.

![UDP客户/服务器程序所用的套接字函数](http://pic002.cnblogs.com/images/2012/367190/2012081121141279.jpg)  
如上图所示, 客户不与服务器建立连接, 而是只管使用`sendto`函数给服务器发送数据报, 其中必须指定目的地(即服务器)第地址作为参数. 
类似的, 服务器不接受来自客户的连接, 而是只管调用`recvfrom` 函数, 等待来自某个客户的数据到达.
recvfrom将接收到的数据与client 的地址一并返回, 因此服务器可以把响应发送给正确的客户.

写一个长度为0 的数据报是可行的. 在UDP情况下, 这会形成一个只包含一个IP首部和一个UDP首部而没有数据的IP数据报. 这也意味着对于UDP协议, recvfrom返回0 值是可接受的.
他并不像TCP套接字上read 返回0值那样表示对端已关闭连接. 既然UDP是无连接的, 因此也没有诸如关闭一个UDP连接之类的事情.

大多数TCP服务器是并发的, 而大多数UDP服务器是迭代的

