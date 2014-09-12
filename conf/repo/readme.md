put thre repo file in `/etc/yum.repos.d`

put the key file in `/etc/pki/rpm-gpg`

运行以下命令生成缓存
```
    yum clean all
    yum makecache
```
这样以后的搜索更加快
