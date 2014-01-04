## a demo of subprocess.call
import subprocess
uname = "uname"
arg = "-a"
print "gatherign info"
subprocess.call([uname,arg])
