#!/usr/bin/python
## process kindle note, My Clipings.txt
## read from stdin and print the result to stdout

import sys
import os

txt = {}

## flag: indicate whether the next line is a key line
flag = True

## key: keep track of the last key line
key = ""

for line in sys.stdin:
	line = line.strip()

## 	key line	
	if flag:
		key = line
		if key not in txt.keys():
			txt[key] = []
		flag = False

## 	skip lines
	if len(line) == 0 or line.find("- Highlight") != -1:
		continue
	if line.find("==") != -1:
		flag = True
		continue

## 	content line
	if key != line:
		txt[key].append(line)

## print out the parsed result
for key in txt.keys():
	print >> sys.stdout, key
	for line in txt[key]:
		print >> sys.stdout, line
	print >> sys.stdout, os.linesep

sys.exit(0)

