import sys

## input
## tn: cs \t 0
## img: cs \t 1 \t objurl
## output:
## cs \t 1 \t objurl if objurl found
## cs \t 0 if none

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
	cs_old = ''
	flag_old = ''
	value_old = ''
	cs = ''
	flag = ''
	value = ''
	
	for line in sys.stdin:
		line = line.strip()
		terms = line.split('\t')
		cs = terms[0]
		flag = terms[1]
		if flag == '1':
			value = terms[2]
		
		if cs not in d and flag == '0':
			d[cs] = []

		if cs in d and flag == '1':
			d[cs].append(value)
	
		if cs_old == '':
			cs_old = cs
			flag_old = flag
			value_old = value
	
		if cs != cs_old:
			process_dict(d, cs_old)
			cs_old = cs
			flag_old = flag
			value_old = value
## 		print d, len(d)
	
	for k in d.keys():
		process_dict(d, k)

if __name__ == "__main__":
	main()

