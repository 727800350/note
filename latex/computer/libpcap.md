libpcap捕获时，使用pcap_loop之类的函数，在调用处理的handle的时候，返回的第一个参数的类型为pcap_pkthdr，第二个参数为uint8_t的指针，前者中有两个数据域的东东：caplen和len，如下：

	 struct pcap_pkthdr {
        struct timeval ts;      /* time stamp */
        bpf_u_int32 caplen;     /* length of portion present */
        bpf_u_int32 len;        /* length this packet (off wire) */
	};

caplen——真正实际捕获的包的长度 
len——这个包的长度
因为在某些情况下你不能保证捕获的包是完整的，例如一个包长1480，但是你捕获到1000的时候，可能因为某些原因就中止捕获了，所以caplen是记录实际捕获的包长，也就是1000，而len就是1480。


ts：时间戳 , 这个时间是包到达的时间还是其他时间还不太确定, 又有对之前截取的pcap 文件多次解析时发现时间是不变的, 所以这个ts 时间戳很可能就是包到达的时间
```
typedef struct timeval {
  long tv_sec;
  long tv_usec;
} timeval;
```

delay us 微秒 1 * 10^{-6} second
输出long 用 %ld

	const u_char *pcap_next(pcap_t *p, struct pcap_pkthdr *h);
read the next packet (by calling `pcap_dispatch()` with a cnt of 1) and 
returns a `u_char` pointer to the data in that packet.   
The `pcap_pkthdr` structure pointed to by h is filled in with the appropriate values for the packet.  
也就是说`pcap_next` 读到下一个packet, 然后这个packet 的信息(比如获得的时间, 长度caplen and len)被填充到`pcap_pkthdr *h` 中, 而真正的数据部分是在返回值中.


20分钟的数据 1.9GB 176W packets

单纯的`pkt_size` 的variance 很大, 尤其是对于那些包数比较大的flow来说, 我记得有一种度量是衡量相对的variance, 将包的数量考虑进去
