/* server.c */
/*cas:多进程并发服务器
 * 当连接建立后，服务器调用fork()函数产生新的子进程，
 * 子进程处理客户请求，同时父进程关闭连接套接字，然后等待另一个客户的连接
 * 子进程首先关闭监听套接字，然后处理客户请求，最后关闭链接套接字并退出进程
 * 服务器等候客户连接，一旦有连接则显示客户的地址，然后接受来自客户的的信息（字符串）。每当收到一个字符串，则显示该字符串
 * 并将字符串反转，并将反转的结果发回给客户，之后继续等待接受客户的信息直至该客户关闭连接
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>

#define PORT 1234
#define BACKLOG 5
#define MAXDATASIZE 1000

void process_cli(int connectfd, struct sockaddr_in client);

int main(void) {
	struct sockaddr_in server, client; //internet socket address for server and client
	socklen_t sin_size;
	pid_t pid;
	int listenfd, connectfd;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		/*handle exception*/
		perror("creating socket failed");
		exit(1);
	}

	int opt = SO_REUSEADDR;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

	if ((bind(listenfd, (struct sockaddr *) &server, sizeof(struct sockaddr)))
			== -1) {
		perror("bind error");
		exit(1);
	}

	if ((listen(listenfd, BACKLOG)) == -1) {
		perror("listen error");
		exit(1);
	}
	sin_size = sizeof(struct sockaddr_in);

	printf("Accepting connections ...\n");
	while (1) {
		if ((connectfd = accept(listenfd, (struct sockaddr *) &client,
				&sin_size)) == -1) {
			perror("accept() error");
			exit(1);
		}
		
		//create child process to service the client
		if((pid=fork())>0){
			//parent process
			close(connectfd);
			continue;  //等待下一个连接
		}
		else if(pid==0){
			//child process
			close(listenfd);
			process_cli(connectfd,client);
			exit(0);
		}
		else {
			printf("fork error\n");
			exit(0);
		}
	close(listenfd);
	}//end while
}

void process_cli(int connectfd, struct sockaddr_in client) {
	int num;
	char recvbuf[MAXDATASIZE], sendbuf[MAXDATASIZE],cli_name[MAXDATASIZE];

	/*print client's ip*/
	printf("you got a connection from %s\n", inet_ntoa(client.sin_addr));

	//print client's name from client
	num=recv(connectfd,cli_name,MAXDATASIZE,0);
	if(num==0){
		close(connectfd);
		printf("client disconnectd");
		return;
	}
	cli_name[num-1]='\0';
	printf("client's name is %s\n",cli_name);

	char *servermessage="you have revevied an empty string";
	char *clientmessage="you have typed an empty string";
	while ((num = recv(connectfd, recvbuf, MAXDATASIZE, 0)) > 0) {

		//if num=1,说明用户直接按下回车键，而没有有效信息
		if(num==1){
			puts(servermessage);
			send(connectfd,clientmessage,strlen(clientmessage),0);
		}
		else {
			recvbuf[num] = '\0';
			printf("Received client message:[%s] %s\n",cli_name, recvbuf);
			/* Reverse the message*/
			int i = 0;
			for (i = 0; i < num - 1; i++) {
				sendbuf[i] = recvbuf[num - i - 2];
			}
			sendbuf[num - 1] = '\0';
			//send to the client welcome message
			send(connectfd, sendbuf, strlen(sendbuf), 0);
		}
	}
	close(connectfd);
}
