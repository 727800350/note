#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1234
#define MAXDATASIZE 100

int main(){
int sockfd; //socket descriptors;
struct sockaddr_in server; //server's address information
struct sockaddr_in client; //client's address information
socklen_t sin_size;
int num;
char msg[MAXDATASIZE]; //buffer for message

//creating udp socket
if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
	//handle exception
	perror("creating socket failed");
	exit(1);
}

bzero(&server,sizeof(server));
server.sin_family=
server.sin_port=htons(PORT);
server.sin_addr.s_addr=htonl(INADDR_ANY);
if(bind(sockfd,(struct sockaddr *)&server,sizeof(struct sockaddr)) == -1){
	//handle error
	perror("bind error");
	exit(1);
}

sin_size=sizeof(struct sockaddr_in);
while(1){
	num=recvfrom(sockfd,msg,MAXDATASIZE,0,(struct sockaddr *)&client, &sin_size);
	if(num<0){
		perror("recvfrom error\n");
		exit(1);
	}

	msg[num]='\0';
	printf("you got a message (%s) from (%s)\n ",msg,inet_ntoa(client.sin_addr));
	sendto(sockfd,"welcome to my server.\n",22,0,(struct sockaddr *)&client,sin_size);

	if(!strcmp(msg,"quit")) break;

}

close(sockfd);
return 0;
}
