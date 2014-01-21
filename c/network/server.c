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
	struct sockaddr_in servaddr, cliaddr;//internet socket address
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, n;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	//将server端的listenfd与固定的IP：prot绑定到一起，方便client访问
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	//设置最大连接数,同时将listenfd设置为监听状态
	listen(listenfd, 20);

	printf("Accepting connections ...\n");
	//每次循环处理一个client的链接，而accept()的返回值是另外一个文件描述符connfd，之后与客户端之间就通过这个connfd通讯，最后关闭connfd断开连接，而不关闭listenfd，再次回到循环开头listenfd仍然用作accept的参数。accept()成功返回一个文件描述符，出错返回-1。
	while (1) {
		cliaddr_len = sizeof(cliaddr);

		//获得client的ip信息，之后server的write会通过connfd传递给这个ip地址的机器
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

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
