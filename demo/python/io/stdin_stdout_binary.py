#!/usr/bin/python

import sys
import struct as st

value = sys.stdin.read(4)
while value:
	## 	process binary value
	value = int(st.unpack('i', value)[0])&0xffffffff
	print >> sys.stdout, value

	value = st.pack('i', value)
	sys.stderr.write(value)

	value = sys.stdin.read(4)

sys.exit(0)

