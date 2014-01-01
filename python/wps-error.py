## extract the proper information
## here, we want to get the library file name, for example
## in "./wpp: error while loading shared libraries: libXrender.so.1: cannot open", we want the name libXrender.so.1

## input and output file
wps = open("wps.txt","r")
out = open("out.txt","w")
no = 0
for line in wps:
		no = no + 1
## 		test this file if it works
## 		if no == 1:
## 		the data range, from line 1 to line 99
		if no <= 99:
				if line.find("error while") != -1:
						words = line.split(" ")
						print>>out, words[6]
out.close()
wps.close()
