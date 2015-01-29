#encoding: utf-8
#!/usr/bin/env python
# Author: L-K
# Blog  : phey.cc
# Date  : 2014-10-22
# http://phey.cc/use_raw_socket_to_send_syn_package_under_linux_with_python27.html 
## send a syn packet

import socket
import sys
from struct import *

class sendSYN():
	def __init__(self):
		self.s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
		self.s.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)

	def checksum(self, msg):
		s = 0
		for i in range(0, len(msg), 2):
			w = (ord(msg[i]) << 8) + (ord(msg[i+1]) )
			s = s + w
		s = (s>>16) + (s & 0xffff);
		s = ~s & 0xffff
		return s

	def send(self, sip, sport, dip, dport):
		saddr = socket.inet_aton(sip)
		daddr = socket.inet_aton(dip)
 		ipHeader = pack('!BBHHHBBH4s4s' , (4 << 4) + 5, 0, 40, 54321, 0, 255, socket.IPPROTO_TCP, 10, saddr, daddr)
		tcpHeader = pack('!HHLLBBHHH' , sport, dport, 0, 0, (5 << 4) + 0, 0 + (1 << 1) + (0 << 2) + (0 <<3) + (0 << 4) + (0 << 5), socket.htons(5840), 0, 0)
		saddr = socket.inet_aton(sip)
		daddr = socket.inet_aton(dip)
		protocol = socket.IPPROTO_TCP
		tcpLength = len(tcpHeader)
		psh = pack('!4s4sBBH' , saddr , daddr , 0, protocol , tcpLength);
		psh = psh + tcpHeader;
		tcpChecksum = self.checksum(psh)
		tcpHeader = pack('!HHLLBBHHH' , sport, dport, 0, 0, (5 << 4) + 0, 0 + (1 << 1) + (0 << 2) + (0 <<3) + (0 << 4) + (0 << 5),  socket.htons (5840), tcpChecksum , 0)
## 		0 << 5: 000000
## 		0 << 4:  00000
## 		0 << 3:   0000
## 		0 << 2:    000
## 		1 << 1:     10
## 		前面5个相加就是: 000010: 分别对应urg|ack|psh|rst|syn|fin
		packet = ipHeader + tcpHeader
		self.s.sendto(packet, (dip , 0 ))

s = sendSYN()
## s.send("192.168.1.2", 65535, '192.168.1.1', 80)
s.send("192.168.1.187", 65535, '192.168.1.1', 80)
print "sent"

## 使用起来像这样
## root@cubietruck:~# python syn.raw.socket.py 
## root@cubietruck:~# ▊

## 用tcpdump抓包可以看到结果, 最后的那个RST包是系统自己发的, 可以通过iptables的output链去阻止这个RST包的发送
## root@cubietruck:~# tcpdump -nn host 192.168.1.1 and port 80
## tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
## listening on eth0, link-type EN10MB (Ethernet), capture size 65535 bytes
## 15:27:13.650922 IP 192.168.1.2.65535 > 192.168.1.1.80: Flags [S], seq 0, win 53270, length 0
## 15:27:13.651282 IP 192.168.1.1.80 > 192.168.1.2.65535: Flags [S.], seq 3771972433, ack 1, win 5840, options [mss 1460], length 0
## 15:27:13.651418 IP 192.168.1.2.65535 > 192.168.1.1.80: Flags [R], seq 1, win 0, length 0

## 通过iptables的output链去阻止这个RST包的发送
## sudo vi /etc/sysconfig/iptables
## iptables -A OUTPUT -p tcp --tcp-flags RST RST -j DROP

