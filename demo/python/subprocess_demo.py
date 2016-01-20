#!/usr/bin/python

import sys
import subprocess

def main():
	p = subprocess.Popen("./subprocess_demo", stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE, shell = False)
	## p.stdin is the stdin of ./app

	## write to p.stdin, so that app can read from stdin
	p.stdin.write('3\n')
	p.stdin.write('4\n')
	sys.stdout.write(p.stdout.read())

if __name__ == "__main__":
	main()

