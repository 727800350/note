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


    #expect�еĶ�̬��
    #��expect��ʵ��ʹ���У����ǿ���û��Ȩ���ڲ��Ի�����rd�������ϰ�װexpect��
    #���磬��caseǰ�õ�rd���Զ������߿����У���������ʱ��Ҫ�ӹ̶��Ĳ��Ի��������ݺͻ�����
    #��Ҫʹ��expect�������ڹ������й������������룬��rd�Ļ�����û�а�װexpect�� 
    #��Ҫ��expect������rd�����ϣ�����expect ������Ҫload libexpect5.42.so, 
    #Ĭ�ϵ�·���� /usr/lib64��������û�и�·����дȨ�ޣ�����޷���libexpect5.42.so�������Ŀ¼��
    #���Խ�libexpect5.42.so�����Լ���Ȩ�޵�·���£� ����ͨ���޸ġ�LD_LIBRARY_PATH����ʵ�֣����£� 
    #export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/tmp/shy/antispam_unit_test/bin: 
    #����expect�Ϳ����ҵ���load libexpect5.42.so �ˡ�
    #Ϊ�˱����޸�rd�����ı�������ð�export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/tmp/shy/antispam_unit_test/bin:   
    #д���Լ��Ĺ��߽ű��д�ڵ���expect����֮ǰ����������������Ϻ�LD_LIBRARY_PATH���޸�Ҳ����֮��ʧ�ˣ�
    #��rd�Ļ���û���κ�Ӱ��
#
