# -*- coding: utf-8 -*-
#!/usr/local/bin/python3

import argparse
import logging
import os
import shutil
import sys
import time

## debug, info, warn/warning, error, fatal/critical
logging.basicConfig(level = logging.DEBUG, format = '%(levelname)s %(asctime)s [%(filename)s][%(lineno)d][%(funcName)s] %(message)s')
log = logging.getLogger()

parser = argparse.ArgumentParser(description = 'Python script')
parser.add_argument('--mode', action = 'store', dest = 'mode', default = 'log', type = str, help = 'log|copy|rename')
arg = parser.parse_args()

def process(path):
	if not os.path.exists(path):
		log.warning('{0} does not exist'.format(path))
		return 0

	dir_name = os.path.dirname(path)
	file_name = os.path.basename(path)
	if not file_name.startswith('av'):
		log.warning('{0} is not a valid bilibili video path'.format(path))
		return 0

	name, extension = os.path.splitext(file_name)
	name = name.replace('【', '[').replace('】', ']').replace('（', '(').replace('）', ')')
	pos = name.find(' ')
	if pos >= 2:
		av = name[:pos]
		rest = name[pos + 1:]
		new_path = os.path.join(dir_name, rest + ' ' + av + extension)

		if arg.mode == 'log':
			log.info('from {0} to {1}'.format(path, new_path))
		elif arg.mode == 'copy':
			log.info('copying {0} to {1}'.format(path, new_path))
			shutil.copyfile(path, new_path)
		elif arg.mode == 'rename':
			log.info('raname {0} to {1}'.format(path, new_path))
			os.rename(path, new_path)
	else:
		log.warning('{0} has been skipped')
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

