#encoding: utf-8
#!/usr/bin/python

## extract information from the kindle note file "My Clippings.txt"
title = "UNIX Network Programming"

## the kindle note source file, My Clippings.txt
file_in = open("My Clippings.txt","r")

def extract_title(title):
		file_info = open(title+".txt","w")
		line = file_in.readline()
		while line:
## 				find the title
				if line.find(title) != -1:
## 						the location of this note
						location = file_in.readline()
##		 				before the content of the note, there is an empty line
						vide = file_in.readline()
## 						the content of this note
						content = file_in.readline()
						print >>file_info,content
				line = file_in.readline()
		file_info.close()
file_in.close()

def main():
		extract_title(title)

if __name__ == "main":
		main()
