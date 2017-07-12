#!/usr/bin/python
#encoding: utf-8
""" Python script """

import sys
import logging
import argparse
import time
import struct as st

## debug, info, warn/warning, error, fatal/critical
logging.basicConfig(level = logging.DEBUG, format = '%(levelname)s %(asctime)s [%(filename)s][%(lineno)d][%(funcName)s] %(message)s')
log = logging.getLogger()

parser = argparse.ArgumentParser(description = 'Python script')
parser.add_argument('-n', action = 'store', dest = 'num', default = 0, type = int, help = 'number')
parser.add_argument('-f', action = 'store', dest = 'dict', default = 'dict.txt', type = str, help = 'dict file')
arg = parser.parse_args()

def process(kl, k, vl, v):
    """ core procedure """
    print >> sys.stdout, '%s\t%d' % (k, vl)
    return 0


num = 0
time_start = time.time()
while True:
    kl_raw = sys.stdin.read(4)
    if kl_raw == '':
        break
    kl = st.unpack('i', kl_raw)[0]
    k = sys.stdin.read(kl)
    vl = st.unpack('i', sys.stdin.read(4))[0]
    v = sys.stdin.read(vl)
    ret = process(kl, k, vl, v)
    if ret != 0:
        log.fatal('process %s %d error' % (k, vl))
        sys.exit(1)
    num = num + 1
time_total = time.time() - time_start
qps = num / time_total
log.info('num: %d process time: %fs qps: %d' % (num, time_total, qps))
sys.exit(0)

