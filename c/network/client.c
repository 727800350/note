/* client.c */
/*客户首先与服务器连接，然后接收用户输入的字符串，在将字符串发送给服务器，接受服务器发回的信息并显示
 * 之后，继续等待用户输入直至用户输入ctrl+D，当用户输入ctrl+D后，客户关闭连接并退出
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 1234 //open port on remote host
#define MAXDATASIZE 1000 //max number of bytes of data

void process(FILE *fp,int socket);
char* getMessage(char *sendline,int len,FILE *p);

int main(int argc, char *argv[])
{
	int sockfd; //file descriptors
	struct hostent *he; //structure that will get information about remote host
	struct sockaddr_in server; //server's address information

	if (argc != 2) {
		printf("Usage: %s<IP Address>\n",argv[0]);
		exit(1);
	}

	if((he=gethostbyname(argv[1]))==NULL){
		printf("gethostbyname() error\n");
		exit(1);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1){
		printf("socket() error");
		exit(1);
	}

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	//inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
	server.sin_port = htons(PORT);
	server.sin_addr=*((struct in_addr *)he->h_addr);
	/*
    server sockaddr_in
    he hostend
	struct sockaddr_in {
        short sin_family;
        unsigned short sin_port;
        struct in_addr sin_addr;
        unsigned char sin_zero[8];
    };

    struct hostent {
        char *h_name;
        char **h_aliases;
        int h_addrtype;
        int h_length;
        char **h_addr_list;
        #define h_addr h_addr_list[0]
    };

    struct in_addr {
          unsigned long s_addr; // that’s a 32-bit long, or 4 bytes
    };
	*/

	//和server相应的端口连接
	//connect和bind的参数形式一致，区别在于bind的参数是自己的地址，而connect的参数是对方的地址
	if((connect(sockfd, (struct sockaddr *)&server, sizeof(server)))==-1){
		printf("connect() error");
		exit(1);
	}

	process(stdin,sockfd);
	close(sockfd);
	return 0;
}

void process(FILE *fd,int sockfd){
	char sendline[MAXDATASIZE],recvline[MAXDATASIZE];
	int numbytes;

	printf("connect to server.\n");
	printf("Input name: ");
	if(fgets(sendline,MAXDATASIZE,fd)==NULL){
		printf("\nExit.\n");
		return;
	}
	send(sockfd,sendline,strlen(sendline),0);

	//send message to server
	while(getMessage(sendline,MAXDATASIZE,fd)!=NULL){
		send(sockfd,sendline,strlen(sendline),0);

		if((numbytes=recv(sockfd,recvline,MAXDATASIZE,0))==0){
			printf("server terminated\n");
			return;
		}
		recvline[numbytes]='\0';

		//print server's welcome message
		printf("server message: %s\n",recvline);
	}
	printf("\nExit.\n");

}

char* getMessage(char *sendline,int len,FILE *fp){
	printf("Input String to server:");
	return (fgets(sendline,MAXDATASIZE,fp));
}
