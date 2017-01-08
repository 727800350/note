# Libpcap File Format
The file has a global header containing some global information followed by zero or more records for each captured packet, looking like this:
```
Global Header Packet Header Packet Data Packet Header Packet Data Packet Header Packet Data ... 
```
Global Header 
```c
typedef struct pcap_hdr_s {
        u_int32_t magic_number;   /* magic number */
        u_int16_t version_major;  /* major version number */
        u_int16_t version_minor;  /* minor version number */
        int32_t  thiszone;       /* GMT to local correction */
        u_int32_t sigfigs;        /* accuracy of timestamps */
        u_int32_t snaplen;        /* max length of captured packets, in octets */
        u_int32_t network;        /* data link type */
} pcap_hdr_t;
```

- magic_number: 
used to detect the file format itself and the byte ordering. 
The writing application writes 0xa1b2c3d4 with it is native byte ordering format into this field. 
The reading application will read either 0xa1b2c3d4 (identical) or 0xd4c3b2a1 (swapped). 
If the reading application reads the swapped 0xd4c3b2a1 value, it knows that all the following fields will have to be swapped too. 
For nanosecond-resolution files, the writing application writes 0xa1b23c4d, with the two nibbles of the two lower-order bytes swapped, and the reading application will read either 0xa1b23c4d (identical) or 0x4d3cb2a1 (swapped).
- version_major, version_minor: the version number of this file format (current version is 2.4) 
- sigfigs: in theory, the accuracy of time stamps in the capture; in practice, all tools set it to 0
- snaplen: the "snapshot length" for the capture (typically 65535 or even more, but might be limited by the user)

libpcap捕获时，使用pcap_loop之类的函数，在调用处理的handle的时候，返回的第一个参数的类型为pcap_pkthdr，第二个参数为uint8_t的指针，前者中有两个数据域的东东：caplen和len，如下：

Packet Header 
```c
struct pcap_pkthdr {
	u_int32_t ts_sec;    /* time stamp */
	u_int32_t ts_usec;
	u_int32_t caplen;     /* length of portion present */
	u_int32_t len;        /* length this packet (off wire) */
};
```
- caplen——真正实际捕获的包的长度 
- len——这个包的长度
因为在某些情况下你不能保证捕获的包是完整的，例如一个包长1480，但是你捕获到1000的时候，可能因为某些原因就中止捕获了，所以caplen是记录实际捕获的包长，也就是1000，而len就是1480。
- ts：时间戳 , 这个时间是包到达的时间还是其他时间还不太确定, 又有对之前截取的pcap 文件多次解析时发现时间是不变的, 所以这个ts 时间戳很可能就是包到达的时间

delay us 微秒 1 * 10^{-6} second
输出long 用 %ld

```
#include <pcap/pcap.h>
const u_char *pcap_next(pcap_t *p, struct pcap_pkthdr *h);
```c
read the next packet (by calling `pcap_dispatch()` with a cnt of 1) and 
returns a `u_char` pointer to the data in that packet.   
The `pcap_pkthdr` structure pointed to by h is filled in with the appropriate values for the packet.  
也就是说`pcap_next` 读到下一个packet, 然后这个packet 的信息(比如获得的时间, 长度caplen and len)被填充到`pcap_pkthdr *h` 中, 而真正的数据部分是在返回值中.  
**attention**:
The packet  data  is  not  to  be freed by the caller, and is not guaranteed to be valid after the next call to pcap_next_ex(), pcap_next(), pcap_loop(), or pcap_dispatch(); 
if the code needs it to remain valid, it must make a copy of it. 

each time the returned pkt value is the same, meaning that libpcap library use a fiexed buffer to store the packet content.  
and when reading from a pcap file, when the EOF is reached, the returned value is NULL, so NULL could be used to test this event.

```c
#include <pcap/pcap.h>
typedef void ( *pcap_handler)(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes);

int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user);
int pcap_dispatch(pcap_t *p, int cnt, pcap_handler callback, u_char *user);
```

20分钟的数据 1.9GB 176W packets

单纯的`pkt_size` 的variance 很大, 尤其是对于那些包数比较大的flow来说, 我记得有一种度量是衡量相对的variance, 将包的数量考虑进去
