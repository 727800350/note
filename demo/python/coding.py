#!/usr/bin/python

import sys
import struct as st

def encode():
	content = sys.stdin.read()
	value = st.pack('i', 20150415)
	sys.stdout.write(value)
	sys.stdout.write(content)

def decode():
	sys.stdin.read(4)
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

