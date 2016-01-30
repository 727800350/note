#!/usr/bin/python

import sys
import subprocess as sp

def main():
	p = sp.Popen("./subprocess_demo", stdin = sp.PIPE, stdout = sp.PIPE, stderr = sp.PIPE, shell = False)
	## p.stdin is the stdin of ./app

	## write to p.stdin, so that app can read from stdin
	p.stdin.write('3\n')
	p.stdin.write('4\n')
	sys.stdout.write(p.stdout.read())

if __name__ == "__main__":
	main()

