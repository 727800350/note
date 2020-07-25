
ref

- [廖雪峰的git 教程](https://www.liaoxuefeng.com/wiki/896043488029600)
- [图解Git](https://marklodato.github.io/visual-git-guide/index-zh-cn.html)

添加密钥, 参见[bash](./bash.md) 中的ssh部分

# 常用
- `git stash`: stash the changes in a dirty working directory away
- `git remote prune origin`: clean the remote branch status

## 撤销
- `git reset HEAD` 移除缓存区的所有文件(i.e. 撤销上次git add):
- `git revert <commit>` 重置一个提交(通过创建一个截然不同的新提交)
- `git reset <commit>` 将HEAD重置到上一次提交的版本,并将之后的修改标记为未添加到缓存区的修改:
- `git reset --keep <commit>` 将HEAD重置到上一次提交的版本,并保留未提交的本地修改:

## 回滚
1. `git reset --hard <commit>` reset index和working directory, 自从<commit>以来在working directory中的任何改变都被丢弃, 并把HEAD指向<commit>.
  git log 看不到这个commit 之后的commits, 但是git reflog 可以看到
1. `git push --force origin master`

- 把<commit> 换成某一个tag的名字, 可以回滚到指定的tag
- 不指定<commit>时, 默认为HEAD, 即最新的一次提交

Git Your branch is ahead of 'origin/master' by X commits解决方法

- If you work in another way and your local changes should be pushed then just, `git push origin`
- If your local changes are bad then just remove them or reset your local master to the state on remote. `git reset --hard origin/master`

## git cherry-pick
What git cherry-pick does, basically, is take a commit from somewhere else, and "play it back" wherever you are right now.
Because this introduces the same change with a different parent, Git builds a new commit with a different ID.

实际问题: 在本地 master 分支上做了一个commit(38361a68138140827b31b72f8bbfd88b3705d77a), 如何把它放到本地 old cc 分支上?
```bash
git checkout old_cc
git cherry-pick 38361a68138140827b31b72f8bbfd88b3705d77a
```
如果顺利, 就会正常commit
如果出现冲突, 像解决git merge 一样解决就行了

## git log
- `git log -p filename`: 显示该文件每次提交的diff
- `git log filename`: 可以看到该文件相关的commit记录
- `git blame filename`: 谁, 在什么时间, 修改了文件的什么内容
- `git show commit-id`: 根据commit-id查看某个提交
- `git show commit-id filename`: 查看某次提交中的某个文件变化

## git grep
- `git grep "hello"` 从当前目录的所有文件中搜索
- `git grep "hello"` v2.5 在某一版本中搜索

## git tag
- `git tag` 列出现有标签的命令非常简单
- `git tag -l 'v1.4.2.\*'` // 匹配标签
- `git tag -a v1.4 -m 'my version 1.4'`  // 含附注标签
- `git show v1.4`
- `git push origin v1.5`
- `git pull/push origin --tags`
- `git tag -d v1.5`: 删除本地tag
- `git push origin :refs/tags/v1.5`: 删除远程的tag

Git 使用的标签有两种类型:轻量级的(lightweight)和含附注的(annotated).

- 轻量级标签(-lw)就像是个不会变化的分支,实际上它就是个指向特定提交对象的引用.
- 含附注标签(-a),实际上是存储在仓库中的一个独立对象,它有自身的校验和信息,包含着标签的名字,电子邮件地址和日期,以及标签说明,
标签本身也允许使用 GNU Privacy Guard (GPG) 来签署或验证.

一般我们都建议使用含附注型的标签,以便保留相关信息:当然,如果只是临时性加注标签,或者不需要旁注额外信息,用轻量级标签也没问题

# FAQ
## 撤销操作
### 撤销 git add
运行了`git add`, 但是还没有运行`git commit`, 可以执行下面的两个方法:

- `git rm --cached <added_file_to_undo>`
- `git reset .`: (to undo my entire initial add), 修改的文件还将处于修改的状态

### 撤销commit message
commit 之后发现commit的message有错误.

`git commit --amend`: lets you edit the previous commit message

### commit 之后忘记add 另外一个修改的文件
```bash
# Edit hello.py and main.py
git add hello.py
git commit

# Realize you forgot to add the changes from main.py
git add main.py
git commit --amend --no-edit
```

# git config
## [Using ssh over https](https://help.github.com/articles/using-ssh-over-the-https-port/)
有时, 由于众所周知的原因, 无法进行ssh 连接, 可以让ssh 通过https

1. To test if SSH over the HTTPS port is possible, run this SSH command:

  ```bash
  $ ssh -T -p 443 git@ssh.github.com
  # Hi username! You have successfully authenticated, but GitHub does not provide shell access.
  ```
If that worked, great!
1. Enabling SSH connections over HTTPS

  ```bash
  [eric@alien ~]$ cat .ssh/config
  Host github.com
    Hostname ssh.github.com
      Port 443
  ```
1. verification

  ```bash
  $ ssh -T git@github.com
  # Hi username! You have successfully authenticated, but GitHub does not provide shell access.
  ```

## [GIT更新fork出来的代码](http://blog.sina.com.cn/s/blog_411fed0c0102vhlb.html)
1. 增加远程分支: `git remote add source https://github.com/antirez/redis.git`
  如果你运行命令:git remote -v你会发现多出来了一个source的远程分支.如下:

  ```info
  origin  git@github.com:ericuni/redis.git (fetch)
  origin  git@github.com:ericuni/redis.git (push)
  source  https://github.com/antirez/redis.git (fetch)
  source  https://github.com/antirez/redis.git (push)
  ```
  .git/config 中remote 的部分会变为:

  ```info
  [remote "origin"]
    url = git@github.com:ericuni/redis.git
    fetch = +refs/heads/*:refs/remotes/origin/*
  [remote "source"]
    url = https://github.com/antirez/redis.git
    fetch = +refs/heads/*:refs/remotes/source/*
  ```
1. 把对方的代码拉到你本地: `git fetch source`
1. 合并对方的代码: `git merge source/master`
1. 把最新的代码推送到你的github上: `git push origin master`

## [git 给远程库添加多个url地址](http://my.oschina.net/shede333/blog/299032)
修改.git/config 中 url 部分

```info
[remote "origin"]
  url = git@github.com:ericuni/note.git
  url = git@git.oschina.net:ericuni/note.git
  fetch = +refs/heads/*:refs/remotes/origin/*
```
然后通过git remote -v 就可以看到多出来的一个
```info
origin  git@github.com:ericuni/note.git (fetch)
origin  git@github.com:ericuni/note.git (push)
origin  git@git.oschina.net:ericuni/note.git (push)
```
这样之后的 git push origin master 就会将commit 同时推送到 github 和 oschina 上.

# theory in git
这才是真正的Git --- Git内部原理揭秘!

- [blog](https://mp.weixin.qq.com/s/UQKrAR3zsdTRz8nFiLk2uQ)
- [video](https://www.bilibili.com/video/av77252063)
