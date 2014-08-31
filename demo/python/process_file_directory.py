#encoding: utf-8
#!/usr/bin/python

## 遍历处理给定目录下的文件
## 两种方式: 广度优先与深度优先

import os 

def main():
	print "hello"

## 第一种：使用os.walk:
## 广度优先
def Test1(rootDir): 
    list_dirs = os.walk(rootDir)
    for root, dirs, files in list_dirs: 
        for d in dirs: 
            print os.path.join(root, d)      
        for f in files: 
            print os.path.join(root, f) 

## 第二种：使用os.listdir:
## 深度优先
def Test2(rootDir): 
    for lists in os.listdir(rootDir): 
        path = os.path.join(rootDir, lists) 
        print path 
        if os.path.isdir(path): #depth first
            Test2(path) 
	
if __name__ == "__main__":
	main()
