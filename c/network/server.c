/* server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<ctype.h>
#include <arpa/inet.h>

#define MAXLINE 80
#define SERV_PORT 8000

int main(void)
{
// 	sockaddr_in is for ipv4, sockaddr_in6 is for ipv6
	struct sockaddr_in servaddr, cliaddr;//internet socket address
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, n;

// 	指定期望的通信协议类型, AF_INET(AF is for address family) 为ipv4, sock_stream 为字节流套接字
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

// 	bzero is from Berkeley, such functions: bcopy, bcmp
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	// 通配地址, inaddr_any=0, 对于ipv4, 这么写可以, 但是对于ipv6, 地址是一个结构体, 不能这么用
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT); // h for host, and n for network

	//将server端的listenfd与固定的IP：prot绑定到一起，方便client访问
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
// 	需要进行指针转换是因为: 套接字函数是在ansi C 之前定义的, 不能使用void * 类型来屏蔽掉不同的套接字地址类型, 所以采用一个通用的套接字地址结构sockaddr
// 	ipV6 之后新的通用套接字地址结构是: sockaddr_storage

	// 当用socket 函数创建一个套接字时, 他被假定为一个主动套接字, 也就是一个将调用connect 发起连接的客户套接字.
	// listen 函数把一个未连接的套接字转换成一个被动套接字, 指示内核应该接受指向该套接字的连接请求
	// 设置最大连接数
	listen(listenfd, 20);

	printf("Accepting connections ...\n");
	//每次循环处理一个client的链接，而accept()的返回值是另外一个文件描述符connfd，之后与客户端之间就通过这个connfd通讯，最后关闭connfd断开连接，而不关闭listenfd
	//再次回到循环开头listenfd仍然用作accept的参数。accept()成功返回一个文件描述符，出错返回-1。
	while (1) {
		cliaddr_len = sizeof(cliaddr);

		//获得client的ip信息，之后server的write会通过connfd传递给这个ip地址的机器
		// cliaddr_len 用指针类型. 调用前, 我们将由cliaddr_len所指向的整数值置为由cliaddr所指的套接字地址结构的长度.
		// 返回时, 该整数由内核填充为存放在该套接字地址结构内的确切字节数, 也就是意味着, 内核可能会改变套接字地址的字节数
		// todo: 但是不知道为什么要改变
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
// 		listening socket and connected socket
// 		一个服务器通常只创建一个监听套接字, 它在该服务器的生命周期内一直存在. 内核为每个服务器接收的客户连接创建一个已连接套接字(也就是完成TCP三次握手).
// 		当服务器完成对某个给定客户的服务时, 相应的已连接套接字就被关闭

		n = read(connfd, buf, MAXLINE);
		//client将数据写到client建立的socketfd中，server端的listenfd处于监听状态，connfd通过listenfd接受客户端的链接，所以可以从connfd中读到client写给sockfd的数据
		printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
		       //inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),ntohs(cliaddr.sin_port));
		       //inet_ntop 将cliaddr的IP存放到str中
		printf("str is: %s\n",str);
		printf("buf is: %s\n",buf);
		for (i = 0; i < n; i++)
			buf[i] = toupper(buf[i]);
		write(connfd, buf,strlen(buf));
		close(connfd);
	}
}
