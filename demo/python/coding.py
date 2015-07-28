#!/usr/bin/python

import sys
import struct as st
import time

def encode():
## 	head = st.pack('i', int(time.time()))
	head = 20150415
	print >> sys.stderr, "head:", head
	head = st.pack('i', head)
	sys.stdout.write(head)
	content = sys.stdin.read()
	sys.stdout.write(content)

def decode():
	head = sys.stdin.read(4)
	head = st.unpack('i', head)[0]
	print >> sys.stderr, "head:", head
	content = sys.stdin.read()
	sys.stdout.write(content)

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print >> sys.stderr, "wrong arguments"
		print >> sys.stderr, "usage:", sys.argv[0], "encode|decode"
		sys.exit(1)
	
	type = sys.argv[1]
	if type == "encode":
		encode()
	if type == "decode":
		decode()

	sys.exit(0)

