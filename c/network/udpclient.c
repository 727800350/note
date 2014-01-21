#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 1234
#define MAXDATASIZE 100

int main(int argc,char *argv[]){
int fd,numbytes; //file descriptors
char buf[MAXDATASIZE];  //buf will store recevied text

struct hostent *he; //structure that will get information about remote host

struct sockaddr_in server,reply;	//server's information

if(argc!=3){
	printf("Usage: %s<IP address> <Message>\n",argv[0]);
	exit(1);
}

if((he=gethostbyname(argv[1])) == NULL){
	printf("gethostbyname() error\n");
	exit(1);
}

if((fd=socket(AF_INET,SOCK_DGRAM,0)) == -1){
	printf("socket() error");
	exit(1);
}

bzero(&server,sizeof(server));
server.sin_family=AF_INET;
server.sin_port=htons(PORT);
server.sin_addr = *((struct in_addr *)he->h_addr);
sendto(fd,argv[2],strlen(argv[2]),0,(struct sockaddr *)&server,sizeof(struct sockaddr));

while(1){
	socklen_t len;
	if((numbytes=recvfrom(fd,buf,MAXDATASIZE,0,(struct sockaddr *)&reply,&len)) == -1){
		printf("recvfrom() error");
		exit(1);
	}

	if(len!=sizeof(struct sockaddr) || memcmp((const void *)&server,(const void *)&reply,len)!=0){
		printf("receive message from other server\n");
		continue;
	}
	
	buf[numbytes]='\0';

	//print server's welcome message
	printf("server message : %s\n",buf);
	break;
}
close(fd);
return 0;
}
