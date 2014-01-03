## process the defines.inc.php to output as another format
## par exemple:define('item',        0);->item	0
## a github demo
file = open("defines.inc.php")
out = open("defines.inc.php.out","w")
for line in file:
		if line.startswith("define"):
## 		if line.startswith("define") and line.find("1-7")!=-1:
				strs = line.split("(")
				strs = strs[1]
				strs = strs.split(")")[0]
				strs = strs.replace(" ","").replace("\t","").replace("'","")
## 				print strs
				strs = strs.split(",")
				if len(strs)>2:
						print>>out, line,"\tPLUS 2"
				else:
## 						print out item and number
## 						print>>out, strs[0],"\t",strs[1]
## 						print out number and then item
						print>>out, strs[1],"\t",strs[0]
print "over"
out.close()
file.close()
