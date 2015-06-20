# encoding: utf-8
import shelve
## anydbm允许我们将一个磁盘上的文件与一个"dict-like"对象关联起来
## 操作这个"dict-like"对象，就像操作dict对象一样,最后可以将"dict-like"的数据持久化到文件
## 对这个"dict-like"对象进行操作的时候，key和value的类型必须是字符串
## shelve模块是anydbm的增强版，它支持在"dict-like"对象中存储任何可以被pickle序列化的对象，但key也必须是字符串

def CreateData():
    try:
        db = shelve.open('db.dat', 'c')
        # key与value必须是字符串
        db['int'] = 1
        db['float'] = 2.3
        db['string'] = "I like python."
        db['key'] = 'value'
    finally:
        db.close()
        
def LoadData():
    db = shelve.open('db.dat', 'r')
    for item in db.items():
        print item,len(item),type(item)
    db.close()
        
if __name__ == '__main__':
    CreateData()
    LoadData()
