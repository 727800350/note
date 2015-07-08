#!/home/work/.jumbo/bin/expect
if {$argc < 1} {
        send_user "usage: $argv0 ip/machinename\n"
exit 0
}
##set key [lindex $argv 0]
##set timeout 2
set host_ip [lindex $argv 0]
set password [lindex $argv 1]
set password "123456"
set username "zhengkuan"
##spawn scp -i $key $src_file $username@$host_ip:$dest_file
#expect "not known" {send_user "FATAL:host not exist\n" ;exit 2}
#expect "yes/no" {send "yes\r"}
#expect "Permission" {send_user "FATAL:your oldpassword wrong\n";exit 3}
#expect "successfully" {send_user "succeed \n";exit 0}

spawn ssh $username@$host_ip
expect {
        "(yes/no)?"
                {
                        send "yes\n"
                        expect "password:" {send "$password\n"}
                        #expect "*password:" {send "$passwd\r"}
                }
        "password:"
                {
                        send "$password\n"
                }
}
#expect "100%"
interact
#expect eof
#spawn `hostname` && ps aux|grep "plat" && ls /home/work/search/platsearch/


    #expect中的动态库
    #在expect的实际使用中，我们可能没有权限在测试机或者rd开发机上安装expect。
    #比如，在case前置到rd的自动化工具开发中，工具运行时需要从固定的测试机拷贝数据和环境，
    #需要使用expect来避免在工具运行过程中输入密码，但rd的机器上没有安装expect， 
    #需要把expect拷贝到rd机器上，但是expect 运行需要load libexpect5.42.so, 
    #默认的路径是 /usr/lib64，而我们没有该路径的写权限，因此无法将libexpect5.42.so　放入该目录，
    #可以将libexpect5.42.so放在自己有权限的路径下， 可以通过修改　LD_LIBRARY_PATH　来实现，如下： 
    #export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/tmp/shy/antispam_unit_test/bin: 
    #这样expect就可以找到并load libexpect5.42.so 了。
    #为了避免修改rd机器的变量，最好把export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/tmp/shy/antispam_unit_test/bin:   
    #写在自己的工具脚本里，写在调用expect工具之前，这样工具运行完毕后LD_LIBRARY_PATH的修改也就随之消失了，
    #对rd的机器没有任何影响
#
