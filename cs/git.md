[git官方教程](http://git-scm.com/book/zh/%E8%B5%B7%E6%AD%A5-%E5%85%B3%E4%BA%8E%E7%89%88%E6%9C%AC%E6%8E%A7%E5%88%B6)

[廖雪峰的git 教程](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/001375840202368c74be33fbd884e71b570f2cc3c0d1dcf000)

添加密钥, 参见[bash](./bash.md) 中的ssh部分

在日常的git操作中,git checkout,是我们的常用命令,既可以是某一个具体的commit hash值,也可以是某个分支名称, 还可以是tag名称

- `git fetch origin branch1:branch2`: 将远程分支branch1 拉到本地作为branch2
- `git add -p <file>` 把对某个文件的修改添加到下次提交中

## git cherry-pick
What git cherry-pick does, basically, is take a commit from somewhere else, and "play it back" wherever you are right now.
Because this introduces the same change with a different parent, Git builds a new commit with a different ID.

实际问题: 在本地 master 分支上做了一个commit(38361a68138140827b31b72f8bbfd88b3705d77a), 如何把它放到本地 old_cc 分支上?
```
$ git checkout old_cc
$ git cherry-pick 38361a68138140827b31b72f8bbfd88b3705d77a
如果顺利, 就会正常commit
如果出现冲突, 像解决git merge 一样解决就行了
```

## git stash
Stash the changes in a dirty working directory away

- `git stash`: 备份当前的工作区的内容,从最近的一次提交中读取相关内容,让工作区保证和上次提交的内容一致.同时,将当前的工作区内容保存到Git栈中
- `git stash list`: 显示Git栈内的所有备份,可以利用这个列表来决定从那个地方恢复
  ```
  stash@{0}: WIP on branch x
  stash@{1}: WIP on branch y
  ```
- `git stash pop`: 从Git栈中读取最近一次保存的内容,恢复工作区的相关内容.由于可能存在多个Stash的内容,所以用栈来管理,pop会从最近的一个stash中读取内容并恢复, 可以通过list 的结果来恢复特定的, 例如 `git stash pop stash@{1}`
- `git stash clear`: 清空Git栈

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

## git log
- `git log -p filename`: 显示该文件每次提交的diff
- `git log filename`: 可以看到该文件相关的commit记录
- `git blame filename`: 谁, 在什么时间, 修改了文件的什么内容
- `git show commit-id`: 根据commit-id查看某个提交
- `git show commit-id filename`: 查看某次提交中的某个文件变化

## git grep
- git grep "hello" 从当前目录的所有文件中搜索
- git grep "hello" v2.5 在某一版本中搜索

## 合并与重置
- `git merge <branch>` 将分支合并到当前HEAD中:
- `git rebase <branch>` 将当前HEAD版本重置到分支中(请勿重置已发布的提交!)
- `git rebase --abort` 退出重置
- `git rebase --continue` 解决冲突后继续重置:

## 撤销
- `git reset HEAD` 移除缓存区的所有文件(i.e. 撤销上次git add):
- `git checkout HEAD <file>` 放弃某个文件的所有本地修改:
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

# FAQ
## 撤销操作
1. 运行了`git add`, 但是还没有运行`git commit`, 可以执行下面的两个方法:

  - `git rm --cached <added_file_to_undo>`
  - `git reset .`: (to undo my entire initial add), 修改的文件还将处于修改的状态

1. Checkout all files except one. When I do a git status, I see files like this:
  ```
  modified:  dir/A/file.txt
  modified:  dir/B/file.txt
  modified:  dir/C/file.txt
  modified:  dir/D/file.txt
  ```
  What I want to do is to discard changes to all files EXCEPT for dir/C/file.txt 操作步骤:
  ```
  git add dir/C/file.txt # this file will stay modified and staged
  git checkout .
  ```

1. commit 之后发现commit的message有错误, `git commit --amend`: lets you edit the previous commit message

1. commit 之后想起忘了add 另外一个修改的文件, 操作步骤如下
  ```
  # Edit hello.py and main.py
  git add hello.py
  git commit

  # Realize you forgot to add the changes from main.py
  git add main.py
  git commit --amend --no-edit
  ````

## 分支操作
1. Using git in two computers, after merging one branch a into master with computer c1, delte the branch a in c1 both locally and remotely,
switch to computer c2, delelte the branch a locally, and want to delete the branch a remotely in c2,
but an error is reported, as the remote branch a has been deleted by computer c1, so what should I do to clean the branch status in computer c2?
original branch status:

    [eric@iis ntop]$ git branch -a
    * master
      note
      remotes/origin/HEAD -> origin/master
      remotes/origin/master
      remotes/origin/note
note branch 是在其他机器上删除的一个分支

  1. `$ git branch -d note`: delete the branch locally
  1. `$ git remote prune origin`: clean the remote branch status
  1. `$ git remote set-head origin -a`: query the remote and automatically set the origin/HEAD pointer to the remote current branch.
  1. `$ git remote set-head origin -d`: delete the origin/HEAD symbolic ref

1. 比如你同事在Git的remote branch中新增branch xxx, 但是你发现在本地中查看存在的branch时, 并看不到他增加的branch, `git fetch` 可以将远程新的分支拉到本地


# git config
## [Using ssh over https](https://help.github.com/articles/using-ssh-over-the-https-port/)
有时, 由于众所周知的原因, 无法进行ssh 连接, 可以让ssh 通过https

1. To test if SSH over the HTTPS port is possible, run this SSH command:

  ```
  $ ssh -T -p 443 git@ssh.github.com
  # Hi username! You have successfully authenticated, but GitHub does not provide shell access.
  ```
If that worked, great!
1. Enabling SSH connections over HTTPS

  ```
  [eric@alien ~]$ cat .ssh/config
  Host github.com
    Hostname ssh.github.com
      Port 443
  ```
1. verification

  ```
  $ ssh -T git@github.com
  # Hi username! You have successfully authenticated, but GitHub does not provide shell access.
  ```

## [GIT更新fork出来的代码](http://blog.sina.com.cn/s/blog_411fed0c0102vhlb.html)
1. 增加远程分支: `git remote add source https://github.com/antirez/redis.git`
  如果你运行命令:git remote -v你会发现多出来了一个source的远程分支.如下:

  ```
  origin  git@github.com:ericuni/redis.git (fetch)
  origin  git@github.com:ericuni/redis.git (push)
  source  https://github.com/antirez/redis.git (fetch)
  source  https://github.com/antirez/redis.git (push)
  ```
  .git/config 中remote 的部分会变为:

  ```
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

```
[remote "origin"]
  url = git@github.com:ericuni/note.git
  url = git@git.oschina.net:ericuni/note.git
  fetch = +refs/heads/*:refs/remotes/origin/*
```
然后通过git remote -v 就可以看到多出来的一个
```
origin  git@github.com:ericuni/note.git (fetch)
origin  git@github.com:ericuni/note.git (push)
origin  git@git.oschina.net:ericuni/note.git (push)
```
这样之后的 git push origin master 就会将commit 同时推送到 github 和 oschina 上.

# theory in git
[这才是真正的 Git(freeCodeConf 2019 深圳站)](https://www.bilibili.com/video/av77252063)

## Git是怎么储存信息的
这里会用一个简单的例子让大家直观感受一下git是怎么储存信息的。

首先我们先创建两个文件
```bash
$ git init
$ echo '111' > a.txt
$ echo '222' > b.txt
$ git add *.txt
```
Git会将整个数据库储存在.git/目录下，如果你此时去查看.git/objects目录，你会发现仓库里面多了两个object。

```
$ tree .git/objects
.git/objects
├── 58
│   └── c9bdf9d017fcd178dc8c073cbfcbb7ff240d6c
├── c2
│   └── 00906efd24ec5e783bee7f23b5d7c941b0c12c
├── info
└── pack
```
好奇的我们来看一下里面存的是什么东西.
```bash
$ cat .git/objects/58/c9bdf9d017fcd178dc8c073cbfcbb7ff240d6c
xKOR0a044K%
```
怎么是一串乱码？这是因为Git将信息压缩成二进制文件。但是不用担心，因为Git也提供了一个能够帮助你探索它的api.
git cat-file [-t] [-p] [-s] -t可以查看object的类型，-p可以查看object储存的具体内容, -s 查看大小.
```bash
$ git cat-file -t 58c9bdf9d017fcd178dc8c073cbfcbb7ff240d6c
blob
$ git cat-file -p 58c9bdf9d017fcd178dc8c073cbfcbb7ff240d6c
111
$ git cat-file -s 58c9bdf9d017fcd178dc8c073cbfcbb7ff240d6c
4
```
可以发现这个object是一个blob类型的节点，他的内容是111，也就是说这个object储存着a.txt文件的内容。

这里我们遇到第一种Git object，blob类型，它只储存的是一个文件的内容，不包括文件名等其他信息。然后将这些信息经过SHA1哈希算法得到对应的哈希值
58c9bdf9d017fcd178dc8c073cbfcbb7ff240d6c，作为这个object在Git仓库中的唯一身份证。

注意:

1. 传文件的绝对路径会错误, `$ git cat-file -t .git/objects/58/c9bdf9d017fcd178dc8c073cbfcbb7ff240d6c`, 提示. fatal: Not a valid object name .git/objects/58/c9bdf9d017fcd178dc8c073cbfcbb7ff240d6c.
	因为sha1 需要40个字节, 所以会需要把目录的名字也包括进去
1. -s 的结果是4, 因为还有一个换行符"\n"

```bash
$ git commit -am '[+] init'
$ tree .git/objects
.git/objects
├── 0c
│   └── 96bfc59d0f02317d002ebbf8318f46c7e47ab2
├── 4c
│   └── aaa1a9ae0b274fba9e3675f9ef071616e5b209
...
```
我们会发现当我们commit完成之后，Git仓库里面多出来两个object。同样使用cat-file命令，我们看看它们分别是什么类型以及具体的内容是什么。
```
$ git cat-file -t 4caaa1a9ae0b274fba9e3675f9ef071616e5b209
tree
$ git cat-file -p 4caaa1a9ae0b274fba9e3675f9ef071616e5b209
100644 blob 58c9bdf9d017fcd178dc8c0...     a.txt
100644 blob c200906efd24ec5e783bee7... b.txt
```
这里我们遇到了第二种Git object类型——tree，它将当前的目录结构打了一个快照。
从它储存的内容来看可以发现它储存了一个目录结构（类似于文件夹），以及每一个文件（或者子文件夹）的权限、类型、对应的身份证（SHA1值）、以及文件名。

```bash
$ git cat-file -t 0c96bf
commit
$ git cat-file -p 0c96bf
tree 4caaa1a9ae0b274fba9e3675f9ef071616e5b209
author lzane 李泽帆  1573302343 +0800
committer lzane 李泽帆  1573302343 +0800
[+] init
```
接着我们发现了第三种Git object类型——commit，它储存的是一个提交的信息，包括对应目录结构的快照tree的哈希值，上一个提交的哈希值（这里由于是第一个提交，所以没有父节点。
在一个merge提交中还会出现多个父节点），提交的作者以及提交的具体时间，最后是该提交的信息。

到这里我们就知道Git是怎么储存一个提交的信息的了，那有同学就会问，我们平常接触的分支信息储存在哪里呢？
```bash
$ cat .git/HEAD
ref: refs/heads/master

$ cat .git/refs/heads/master
0c96bfc59d0f02317d002ebbf8318f46c7e47ab2
```

所以整个的树形结构就是下面这样的:

