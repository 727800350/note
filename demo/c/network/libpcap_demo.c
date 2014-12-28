#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet.h>
#include <unistd.h>
#include <strings.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <signal.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <errors.h>
#include <time.h>

#define snaplen 1000
#define promisc 1

pcap_t *pcap;
int datalink;
char src[20], dst[20];
struct pcap_pkthdr hdr;
int pcap_open(char *);
char *net_pcap(int *);
void analysis(char *, int);
void tcp(char *,int);
void udp(char *,int);
void icmp(char *,int);
void cleanup(int);

int main(int argc, char **argv){
	int len;
	char *ptr;
	if(argc >2){
		printf("Usage: %s \"filter condition\"",argv[0]);
		exit(1);
	}
	if(pcap_open(argv[1]) == -1){
		printf("I can not open device.\n");
		exit(0);
	}

	setuid(getuid());
	signal(SIGTERM,cleanup);
	signal(SIGINT,cleanup);
	signal(SIGHUP,cleanup);
	while(1){
		ptr = next_pcap(&len);
		analysis(ptr,len);
	}
	pcap_close(pcap);
}

int pcap_open(char *filter){
	char *dev, *error;
	struct bpf_program fp;
	bpf_uint32 net, netmask;
// 	查找网络设备
	if((dev = pcap_lookdev(error)) == NULL)
		return(-1);
	printf("from network device: %s\n",dev);

// 	获得网络数据报捕获描述符
	if((pcap = pcap_open_live(dev, 1000, promisc, 0, error)) == NULL)
		return(-1);
// 	如果设置了过滤条件
	if(filter != NULL){
		if(pcap_lookupnet(dev, &net, &netmask, error) == -1){
			pcap_close(pcap);
			return(-1);
		}
		if(pcap_compile(dev, &fp, filter, 0, netmask) == -1){
			pcap_close(pcap);
			return(-1);
		}
		if(pcap_setfilter(pcap,&fp) == -1){
			pcap_close(pcap);
			return(-1);
		}
	}
	if((datalink = pcap_datalink(pcap)) < 0){
		pcap_close(pcap);
		return(-1);
	}
	return(0);
}

char *next_pcap(int *len){
	char *ptr;
	while((ptr = (char *)pcap_next(pcap, &hdr)) == NULL);
	*len = hdr.caplen;
	return ptr;
}

void analysis(char *ptr, int len){
	int hlen;
	struct ip *ip;
	struct ether_header *eptr;
	struct tm *time;
	switch(datalink){
		case DLT_NULL:
				ptr += 4;
				len -= 4;
				break;
		case DLT_EN10MB:
				eptr = (struct ether_header *)ptr;
				if(ntohs(eptr->ether_type) != ETHERTYPE_IP){
					printf("Ethernet type %x not IP.\n",ntohs(eptr->ether_type));
					return;
				}
// 				以太网头部长 14 字节
				ptr += 14;
				len -= 14;
				break;
		case DLT_SLIP:
		case DLT_PPP:
				ptr += 24;
				len -= 24;
				break;
		default:
				return;
	}
// 	获取指向IP头部的指针
	ip = (struct ip *) ptr;
	if(ip->ip_v != IPVERSION){
		printf("Error ip version.\n");
		return;
	}
	hlen = ip->ip_hl<<2;
	if(hlen < sizeof(struct ip)){
		printf("IP len %d is error.\n",ip->ip_hl);
		return;
	}
	inet_ntop(AF_INET, (void *)&ip->ip_src, src, 20);
	inet_ntop(AF_INET, (void *)&ip->ip_dst, dst, 20);
	time = gmtime((time_t *)&hdr.ts.tv_sec);
	printf("%8.8s:%ld", ctime((time_t *)&hdr.ts.tv_sec) + 11, hdr.ts.tv_usec);

// 	根据不同的协议选择不同的分析程序, 同时将传输层头部指针传递给相应函数
	switch(ip->ip_p){
		case IPROTO_UDP:
				udp(ptr+hlen, len - hlen);
				break;
		case IPROTO_TCP:
				tcp(ptr+hlen, len - hlen);
				break;
		case IPROTO_ICMP:
				icmp(ptr+hlen, len - hlen);
				break;
		default:
				printf("Not recognize packet.\n");
				break;
	}
}

void tcp(char *ptr, int len){
	struct tcphdr *tcph;
	tcph = (struct tcphdr *)ptr;
	printf("%15s:%-4d->%15s:%-4d", src, ntohs(tcph->source),dst, ntohs(tcph->dest));
	printf("Seq:%ld ACK:%ld", ntohl(tcph->seq), ntohl(tcph->ack-seq));
	printf("Win:%-6d\n", ntohs(tcph->window));
}

void udp(char *ptr, int len){
	struct udphdr *udph;
	udph = (struct udphdr *)ptr;
	printf("%15s -> %15s\n", src, dst);
}

void icmp(char *ptr, int len){
	printf("%15s -> %15s\n", src, dst);
}

void cleanup(int signo){
	struct pcap_stat stat;
	fflush(stdout);
	if(pcap_stats(pcap, &stat) < 0){
		perror(pcap_geterr(pcap));
		exit(0);
	}
	printf("%d packest received by filter\n", stat.ps_recv);
	printf("%d packest droped by kernel\n", stat.ps_drop);
	exit(0);
}

