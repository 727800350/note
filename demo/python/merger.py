#!/usr/bin/python
""" merger script """

import sys
import logging
import argparse

'''
input:
key \t 0 \t value1
key \t 1 \t value2

output:
key \t 1 \t value1 \space value2 if value found
key \t 0 \t value1
the key in the output is from the data source with flag 0

ex:
input
k1 \t 0
k1 \t 1 \t v1
k2 \t 0
k2 \t 1 \t v2
k3 \t 0
k4 \t 1 \t v4

output:
k1 \t 1 \t v1
k2 \t 1 \t v2
k3 \t 0

when using hadoop
	-partitioner "org.apache.hadoop.mapred.lib.KeyFieldBasedPartitioner" \
	-jobconf stream.num.map.output.key.fields=2 \
	-jobconf num.key.fields.for.partition=1 \
'''

## debug, info, warn/warning, error, fatal/critical
logging.basicConfig(level = logging.DEBUG, format = '%(levelname)s %(asctime)s [%(filename)s][%(lineno)d][%(funcName)s] %(message)s')
log = logging.getLogger()

parser = argparse.ArgumentParser(description = 'merge txt')
parser.add_argument('-0', action = 'store', dest = 'flag_0', default = 0, type = int, help = 'whether set 0 has value')
parser.add_argument('-1', action = 'store', dest = 'flag_1', default = 0, type = int, help = 'whether set 1 has value')
parser.add_argument('-s', action = 'store', dest = 'sep', default = ' ', type = str, help = 'seperator between values if both flag_0 and flag_1 are 1')
parser.add_argument('-n', action = 'store', dest = 'num', default = 0, type = int, help = 'max number of list element, 0 means no limit')
arg = parser.parse_args()

d = {}

def compose_fv(flag, v0, v1):
	""" compose flag and value when found """
	if arg.flag_0:
		if arg.flag_1:
			if v1 is None:
				return flag + '\t' + v0
			else:
				return flag + '\t' + v0 + arg.sep + v1
		else:
			return flag + '\t' + v0
	else:
		if arg.flag_1:
			if v1 is None:
				return flag
			else:
				return flag + '\t' + v1
		else:
			return flag


def process_dict(d, key):
	""" process a dict with only a key """
	if key + '_0' in d:
		vs0 = d[key + '_0']
		if key + '_1' in d:
			vs1 = d[key + '_1']
			for v0 in vs0:
				for v1 in vs1:
					print >> sys.stdout, '%s\t%s' % (key, compose_fv('1', v0, v1))
		else:
			## only v0
			for v0 in vs0:
				print >> sys.stdout, '%s\t%s' % (key, compose_fv('0', v0, None))
	## clean
	if key + '_0' in d:
		del d[key + '_0']
	if key + '_1' in d:
		del d[key + '_1']


## init variables
key_old = ''
flag_old = ''
value_old = ''
key = ''
flag = ''
value = ''

num = 0
for line in sys.stdin:
	## parser the line
	line = line.strip()
	vec = line.split('\t')
	if len(vec) < 2:
		log.warning('%s format error' % line)
		continue
	key = vec[0]
	flag = vec[1]

	if flag == '0':
		if arg.flag_0:
			if len(vec) < 3:
				continue
			value = vec[2]
		else:
			value = ''
	elif flag == '1':
		if arg.flag_1:
			if len(vec) < 3:
				continue
			value = vec[2]
		else:
			value = ''
	else:
		log.warning('%s format error' % line)
		continue

	## limit memeory usage
	if arg.num > 0 and key + '_' + flag in d and len(d[key + '_' + flag]) >= arg.num:
		log.warning('%s exceeds' % line)
		continue

	## a new key
	if key + '_0' not in d:
		if flag == '0':
			d[key + '_0'] = []
			d[key + '_0'].append(value)
	else:
	## key aleady exists
		if flag == '1':
			if key + '_1' not in d:
				d[key + '_1'] = []
			d[key + '_1'].append(value)
		else:
			d[key + '_0'].append(value)

	## the first line
	if key_old == '':
		key_old = key
		flag_old = flag
		value_old = value

	## other lines
	if key != key_old:
		process_dict(d, key_old)
		key_old = key
		flag_old = flag
		value_old = value

	num = num + 1 ## end of for line in sys.stdin

## the last possible key
process_dict(d, key)

log.info('total input num: %d' % num)
sys.exit(0)

