put thre repo file in `/etc/yum.repos.d`

put the key file in `/etc/pki/rpm-gpg`

运行以下命令生成缓存
```
    yum clean all
    yum makecache
```
这样以后的搜索更加快

```
# vim /etc/yum.repos.d/源名.repo 
[repo_name] 
name= 源全名 
baseurl= 源地址 
enabled= 1启用 0禁用 
priority= 优先级 (范围: 1-99, 1 最高)
```
建议 base, updates, epel, mosquito-myrepo 优先级为 1，其他源 (rpmfusion, remi, Nux Dextop, RPMforge 等) 设为 2.
这样会减少源之间的软件包冲突.  
更新时有冲突的，可使用 `#yum update --exclude=Package_Name` 来排除某个软件包.  
因安装脚本报错而无法卸载的，可使用 `#rpm -e --noscripts Package_Name` 来卸载软件包，目前正在除虫.  
查看本源包含的软件包列表： `#yum list available --disablerepo=* --enablerepo=mosquito-myrepo`
