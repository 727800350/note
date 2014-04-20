import sys
import os

def main(filename):
	tex = open(filename+".tex","w")
	sys.stdout = tex
	markdown = open(filename+".md","r")
	sep = "\n"
	test = "\r"
	for line in markdown:

		line = line.replace(test,"#")
## 		sections
		if line.find("### ") != -1:
			line = line.replace(sep,"")
			line = line.split(" ")[1]
			line = "\subsubsection{" + line +"}"
		if line.find("## ") != -1:
			line = line.replace(sep,"")
			line = line.split(" ")[1]
			line = "\subsection{" + line +"}"
		if line.find("# ") != -1:
			line = line.replace(sep,"")
			line = line.split(" ")[1]
			line = "\section{" + line +"}"

## 		new line
		if line.endswith("  "+sep):
			line = line.split("  ")[0]
## 			print line
			line = line + "\\\\"

## 		hyperlink
		line = line.replace("%","\\%")
		
## 		image: download the image and then use the figure environment

		print line,

	markdown.close()
	tex.close()

if  __name__ == "__main__":
	print "excute the main"
	main("materiau_electronique")
