#!/usr/bin/python

import sys
import struct as st

## binary content <=> binary 0 + binary content length + binary content
## read from stdin and print result to stdout

## binary content => binary 0 + binary content length + binary content
def encode():
	## kl = 0
	sys.stdout.write(st.pack('I', 0))
	
	## get value
	content = sys.stdin.read()
	
	## vl
	vl = len(content)

	sys.stdout.write(st.pack('I', vl))
	sys.stdout.write(content)
	
	print >> sys.stderr, "vl = ", vl
	return 0
	

## binary 0 + binary content length + binary content => binary content 
def decode():
	## key lenth
	kl = sys.stdin.read(4)
	kl = st.unpack('I', kl)[0]
	
	## key
	sys.stdin.read(kl)
	
	## value length
	vl = sys.stdin.read(4)
	vl = st.unpack('I', vl)[0]
	
	## value
	content = sys.stdin.read(vl)
	sys.stdout.write(content)
	
	print >> sys.stderr, "kl = ", kl, "vl = ", vl
	return 0
	

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print >> sys.stderr, "wrong arguments"
		print >> sys.stderr, "usage:", sys.argv[0], "encode|decode"
		sys.exit(1)

	method = sys.argv[1]
	if method == "encode":
		encode()
	elif method == "decode":
		decode()
	else:
		print >> sys.stderr, method, "not supported"
		print >> sys.stderr, "usage:", sys.argv[0], "encode|decode"
		sys.exit(1)

	sys.exit(0)

