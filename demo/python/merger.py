import sys

## input
## tn: key \t 0
## img: key \t 1 \t objurl
## output:
## key \t 1 \t objurl if objurl found
## key \t 0 if none

d = {}

def process_dict(dictionary, key):
	if key in dictionary:
		value = dictionary[key]
		if len(value) == 0:
			print >> sys.stdout, ("%s\t%d"%(key, 0))
			del dictionary[key]
		else:
			for element in value:
				print >> sys.stdout, ("%s\t%d\t%s"%(key, 1, element))
			del dictionary[key]

def main():
	key_old = ''
	flag_old = ''
	value_old = ''
	key = ''
	flag = ''
	value = ''
	
	for line in sys.stdin:
		line = line.strip()
		terms = line.split('\t')
		key = terms[0]
		flag = terms[1]
		if flag == '1':
			value = terms[2]
		
		if key not in d and flag == '0':
			d[key] = []

		if key in d and flag == '1':
			d[key].append(value)
	
		if key_old == '':
			key_old = key
			flag_old = flag
			value_old = value
	
		if key != key_old:
			process_dict(d, key_old)
			key_old = key
			flag_old = flag
			value_old = value
## 		print d, len(d)
	
	for k in d.keys():
		process_dict(d, k)

if __name__ == "__main__":
	main()

