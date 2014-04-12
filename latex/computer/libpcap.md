
	 struct pcap_pkthdr {
        struct timeval ts;      /* time stamp */
        bpf_u_int32 caplen;     /* length of portion present */
        bpf_u_int32 len;        /* length this packet (off wire) */
	};
Now, It is my understanding that `caplen` is the length of the data we have captured while `len` is the length of the packet on the wire.  
In some cases (e.g. when setting the snaplen too low when opening the pcap device) we might **capture only parts of the packet, that length will be 'caplen'**, while **'len' is the original length**. Thus, caplen should be equal to or less than len, but never greater than len.

	const u_char *pcap_next(pcap_t *p, struct pcap_pkthdr *h);
read the next packet (by calling `pcap_dispatch()` with a cnt of 1) and 
returns a `u_char` pointer to the data in that packet.   
The `pcap_pkthdr` structure pointed to by h is filled in with the appropriate values for the packet.  
也就是说`pcap_next` 读到下一个packet, 然后这个packet 的信息(比如获得的时间, 长度caplen and len)被填充到`pcap_pkthdr *h` 中, 而真正的数据部分是在返回值中.
