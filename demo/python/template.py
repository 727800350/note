#!/usr/bin/python
#encoding: utf-8

import argparse
import logging
import os
import sys
import time

## debug, info, warn/warning, error, fatal/critical
logging.basicConfig(level = logging.DEBUG, format = '%(levelname)s %(asctime)s [%(filename)s][%(lineno)d][%(funcName)s] %(message)s')
log = logging.getLogger()

parser = argparse.ArgumentParser(description = 'Python script')
parser.add_argument('--num', action = 'store', dest = 'num', default = 0, type = int, help = 'number')
parser.add_argument('--dict', action = 'store', dest = 'dict', default = 'dict.txt', type = str, help = 'dict file')
arg = parser.parse_args()

def process(line):
  print(line)
  return 0


num = 0
time_start = time.time()
for line in sys.stdin:
  line = line.replace(os.linesep, '')
  ret = process(line)
  if ret != 0:
    log.error('process %s error', line)
    sys.exit(1)
  num = num + 1
time_total = time.time() - time_start
qps = num / time_total
log.info('num: %d process time: %fs qps: %d' % (num, time_total, qps))

sys.exit(0)

