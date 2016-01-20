#!/usr/bin/python

import pprint
import pickle

def load(filename):
	with open(filename, "rb") as f:
		while True:
			try:
				yield pickle.load(f)
			except EOFError:
				break

def main():
## 	dump
	dict = {'a': [1, 2.0, 3, 4+6j], 'b': ('string', u'Unicode string'), 'c': None}
	list = [1, 2, 3]

	output = open('data.pkl', 'wb')

	# Pickle dictionary using protocol 0.
	pickle.dump(dict, output)

	# Pickle the list using the highest protocol available.
	pickle.dump(list, output, -1)

	output.close()

## load method 1
	pkl_file = open('data.pkl', 'rb')

	data1 = pickle.load(pkl_file)
	pprint.pprint(data1)

	data2 = pickle.load(pkl_file)
	pprint.pprint(data2)

	pkl_file.close()

## load method 2
	pp = pprint.PrettyPrinter(indent = 1);
	pool = load('data.pkl')
	for item in pool:
		pp.pprint(item)

if __name__ == "__main__":
	main()

