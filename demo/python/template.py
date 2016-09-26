#!/usr/bin/python
#encoding: utf-8

import sys
import logging
import argparse

## debug, info, warnning, error, critical
logging.basicConfig(level = logging.DEBUG, format = "%(levelname)s %(asctime)s [%(filename)s][%(lineno)d][%(funcName)s] %(message)s")
log = logging.getLogger()

def main():
	parser = argparse.ArgumentParser(description = 'python template')
	parser.add_argument('-n', action = 'store', dest = 'num', default = 0, type = int, help = 'number')
	arg = parser.parse_args()
	print >> sys.stdout, "hello world"
	log.debug(arg.num)

if __name__ == "__main__":
	main()
