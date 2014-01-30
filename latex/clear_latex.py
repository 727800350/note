#encoding:utf-8
import os
import shutil
## 广度优先搜索
def ch(s):
		return s.decode("utf-8").encode("cp936")

def Test1(rootDir):
		list_dirs = os.walk(rootDir)
		for root,dirs,files in list_dirs:
				for d in dirs:
						print os.path.join(root,d)
				for f in files:
						print os.path.join(root,f)

## 深度优先搜寻
def Test2(rootDir):
		for lists in os.listdir(rootDir):
				path = os.path.join(rootDir,lists)
				print path
				if os.path.isdir(path):
						Test2(path)

## 显示目录深度
def Test3(rootDir,level =1):
		if level==1:
				print rootDir
		for lists in os.listdir(rootDir):
				path = os.path.join(rootDir,lists)
				print "|  "*(level-1)+"|--"+lists
				if os.path.isdir(path):
						Test3(path,level+1)

## 可以采用listdir, 只需要使用isfile判断一下
def clear_latex(srcDir):
		extensions = ["aux","log","out","toc","bak"]
		print "Files in",srcDir,"\nwith extensions",extensions,"will be removed"
		print os.linesep
		for f in os.listdir(srcDir):
				path = os.path.join(srcDir,f)
				if os.path.isfile(path):
						name,ext = os.path.splitext(path)
						if ext[1:] in extensions:
								print f,"will be removed"
								os.remove(path)
				if os.path.isdir(path):
						clear_latex(path)
def main():
	clear_latex(".")

if __name__=="__main__":
		main()
