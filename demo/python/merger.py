import sys

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
	-jobconf stream.num.map.output.key.fields=2 \
	-jobconf num.key.fields.for.partition=1 \
'''

d = {}

'''
flag_0: whether record from data source with flag 0 has value
flag_1: whether record from data source with flag 1 has value
sep: field seperator between value from data source 0 and data source 1 if both flag_0 and flag_1 are True, with space as default
'''
flag_0 = True
flag_1 = True
sep = ":"

def process_dict(dictionary, key):
	if key in dictionary:
		value = dictionary[key]
		if flag_0:
			if len(value) == 1:
				print >> sys.stdout, ("%s\t%d\t%s"%(key, 0, value[0]))
				del dictionary[key]
			else:
				if flag_1:
					for i in range(1, len(value)):
						print >> sys.stdout, ("%s\t%d\t%s"%(key, 1, value[0] + sep + value[i]))
					del dictionary[key]
				else:
					print >> sys.stdout, ("%s\t%d\t%s"%(key, 1, value[0]))
					del dictionary[key]
		else:
			if len(value) == 0:
				print >> sys.stdout, ("%s\t%d"%(key, 0))
				del dictionary[key]
			else:
				if flag_1:
					for element in value: 
						print >> sys.stdout, ("%s\t%d\t%s"%(key, 1, element))
					del dictionary[key]
				else:
					print >> sys.stdout, ("%s\t%d"%(key, 1))
					del dictionary[key]


def main():
	print >> sys.stderr, "flag_0: " + str(flag_0) + "; flag_1: " + str(flag_1)
	key_old = ''
	flag_old = ''
	value_old = ''
	key = ''
	flag = ''
	value = ''
	
	for line in sys.stdin:
		## parser the line
		line = line.strip()
		terms = line.split('\t')
		key = terms[0]
		flag = terms[1]

		if flag == '0':
			if flag_0:
				value = terms[2]
		elif flag == '1':
			if flag_1:
				value = terms[2]
			else:
				value = ""
		else:
			print >> sys.stderr, "flag " + flag + " not supported"
			sys.exit(1)
		
		## a new key
		if key not in d and flag == '0':
			d[key] = []
			if flag_0:
				d[key].append(value)

		## key aleady exists
		if key in d and flag == '1':
			d[key].append(value)
	
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
	
	## the last possible key
	for k in d.keys():
		process_dict(d, k)

if __name__ == "__main__":
	main()

