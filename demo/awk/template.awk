#!/bin/awk -f
## usage: script.awk file

BEGIN {
	FS=" ";
	OFS="\t";
	n = 0;
}

{
	print $0;
	n = n + 1;
}

END {
	OFS=" ";
	print "number of lines:", n;
}
