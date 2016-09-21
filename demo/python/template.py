#!/usr/bin/python
#encoding: utf-8

import sys
import logging

logging.basicConfig(level = logging.DEBUG, format = "%(levelname)s %(asctime)s [%(filename)s][%(lineno)d][%(funcName)s] %(message)s")
log = logging.getLogger()

def main():
	print >> sys.stdout, "hello world"

if __name__ == "__main__":
	main()
