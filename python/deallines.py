import os ## for os.path.isfile()

def dealline(line) :
    print(line) ## 针对line我可以做很多事情

def getfilename() :
    return input('Please input file name(input exit() for exit):').strip()

class More : ## MORE功能
    linenum = 0
    size = 10
    def work(self) :
        if self.linenum >= self.size :
            if input('--MORE--').strip().lower() == 'exit()' :
                return False
            self.linenum = 0
        else :
            self.linenum += 1
        return True

while True :
    try :
        filename = getfilename()

        if filename.lower() == 'exit()' : ## 退出
            break

        if os.path.isfile(filename) : ## 判断文件是否存在

            f = open(filename)
            try :
                lines = f.readlines()

                m = More()
                for line in lines:
                    if False == m.work() :
                        break
                    dealline(line)
            
                ## input()
            finally :
                f.close()

        else :
            print('File does not exists.')
            ##input()
    except :
