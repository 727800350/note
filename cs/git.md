[git官方教程](http://git-scm.com/book/zh/%E8%B5%B7%E6%AD%A5-%E5%85%B3%E4%BA%8E%E7%89%88%E6%9C%AC%E6%8E%A7%E5%88%B6)

[廖雪峰的git 教程](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/001375840202368c74be33fbd884e71b570f2cc3c0d1dcf000)

添加密钥, 参见[bash](./bash.md) 中的ssh部分

在日常的git操作中,git checkout,是我们的常用命令,既可以是某一个具体的commit hash值,也可以是某个分支名称, 还可以是tag名称

## git add
- git add -p <file> 把对某个文件的修改添加到下次提交中

## git cherry-pick. 
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

The modifications stashed away by this command can be 

- listed with `git stash list`, 
- inspected with `git stash show`
- restored (potentially on top of a different commit) with `git stash apply`. 

Calling git stash without any arguments is equivalent to `git stash save`.
A stash is by default listed as "WIP on branchname …", but you can give a more descriptive message on the command line when you create one.

The latest stash you created is stored in refs/stash; 
older stashes are found in the reflog of this reference and can be named using the usual reflog syntax 
(e.g. stash@{0} is the most recently created stash, stash@{1} is the one before it, stash@{2.hours.ago} is also possible).

- git stash: 备份当前的工作区的内容,从最近的一次提交中读取相关内容,让工作区保证和上次提交的内容一致.同时,将当前的工作区内容保存到Git栈中
- git stash pop: 从Git栈中读取最近一次保存的内容,恢复工作区的相关内容.由于可能存在多个Stash的内容,所以用栈来管理,pop会从最近的一个stash中读取内容并恢复
- git stash list: 显示Git栈内的所有备份,可以利用这个列表来决定从那个地方恢复
- git stash clear: 清空Git栈

把当前分支中未提交的修改移动到其他分支
```
git stash
git checkout branch2
git statsh pop
```

## git tag
- git tag 列出现有标签的命令非常简单
- git tag -l 'v1.4.2.\*' // 匹配标签
- git tag -a v1.4 -m 'my version 1.4'  // 含附注标签
- git show v1.4
- git push origin v1.5
- git push origin --tags

Git 使用的标签有两种类型:轻量级的(lightweight)和含附注的(annotated).

- 轻量级标签(-lw)就像是个不会变化的分支,实际上它就是个指向特定提交对象的引用.
- 含附注标签(-a),实际上是存储在仓库中的一个独立对象,它有自身的校验和信息,包含着标签的名字,电子邮件地址和日期,以及标签说明,
标签本身也允许使用 GNU Privacy Guard (GPG) 来签署或验证.

一般我们都建议使用含附注型的标签,以便保留相关信息:当然,如果只是临时性加注标签,或者不需要旁注额外信息,用轻量级标签也没问题

### 删除远程Tag
显示本地 tag
```
git tag 
tag_name
```
删除本地tag
```
git tag -d tag_name 
```
用push, 删除远程tag
```
git push origin :refs/tags/tag_name
```

## git log
- git log -p <file> 显示某个文件的所有修改
- git blame <file> 谁, 在什么时间, 修改了文件的什么内容

## git grep
- git grep "hello" 从当前目录的所有文件中搜索
- git grep "hello" v2.5 在某一版本中搜索

## 合并与重置
- git merge <branch> 将分支合并到当前HEAD中:
- git rebase <branch> 将当前HEAD版本重置到分支中(请勿重置已发布的提交!)
- git rebase --abort 退出重置
- git rebase --continue 解决冲突后继续重置:

## 撤销
- git reset --hard HEAD 放弃工作目录下的所有修改:
- git reset HEAD 移除缓存区的所有文件(i.e. 撤销上次git add):
- git checkout HEAD <file> 放弃某个文件的所有本地修改:
- git revert <commit> 重置一个提交(通过创建一个截然不同的新提交)
- git reset --hard <commit> 将HEAD重置到指定的版本,并抛弃该版本之后的所有修改:
- git reset <commit> 将HEAD重置到上一次提交的版本,并将之后的修改标记为未添加到缓存区的修改:
- git reset --keep <commit> 将HEAD重置到上一次提交的版本,并保留未提交的本地修改:

# repo
## Create repo
创建 git 仓库

	mkdir projet
	cd projet
	git init
	touch README.md
	git add README.md
	git commit -m "first commit" 
	git remote add origin git@git.oschina.net:eric_uni/projet.git 
	git push -u origin master

已有项目?

	cd projet
	git remote add origin git@git.oschina.net:eric_uni/projet.git 
	git push -u origin master

Git Stripspace

- 去掉行尾空白符
- 多个空行压缩成一行
- 必要时在文件末尾增加一个空行
- 使用此命令时必须传入一个文件,像这样:
```
$ git stripspace < README.md
```

提交空的改动
```
git commit -m "Big-ass commit" --allow-empty
```
这样做在如下几种情况下是有意义的:

- 标记一批工作或一个新功能的开始.
- 记录你对项目进行了跟代码无关的改动.
- 跟使用你仓库的其他人交流.
- 作为仓库的第一次提交,因为第一次提交日后是不能被rebase的: git commit -m "init repo" --allow-empty.

在任意diff页面的URL后加上?w=1,可以去掉那些只是空白字符的变化,使你能更专注于代码的变化
在diff或者file页面的URL后面加上?ts=4,这样当显示tab字符的长度时就会是4个空格的长度,不再是默认的8个空格.ts后面的数字还可以根据你个人的偏好进行修改.不过,在在Gists页面和raw file页面不起作用
查看某个用户的所有提交历史,只需在commits页面URL后加上?author=username.

当你点击某个仓库的分支(Branches)选项卡时
```
https://github.com/{user}/{repo}/branches
```
你会看到一个包含所有未合并的分支的列表.

有的时候我们需要将多个分支与一个非主分支(master)进行对比,此时可以通过在URL后加入要比较的分支名来实现:
```
https://github.com/{user}/{repo}/branches/{branch}
```
可以在URL后加上?merged=1来查看已经合并了的分支.

如果我们想要比较两个分支,可以像下面一样修改URL:
```
https://github.com/user/repo/compare/{range}
其中{range} = master...4-1-stable
```
例如:
```
https://github.com/rails/rails/compare/master...4-1-stable
https://github.com/rails/rails/compare/master@{1.day.ago}...master
https://github.com/rails/rails/compare/master@{2014-10-04}...master
这样你就能查看master分支上一段时间或者指定日期内的改动.
```

# FAQ
I mistakenly added files using the command, `git add file`
I have not yet run git commit.
Is there a way to undo this or remove these files from the commit?

可以执行下面的两个方法:

1. `git rm --cached <added_file_to_undo>`
1. `git reset .`: (to undo my entire initial add) only to get this (not so) helpful message:

**错误提交了一个commit, 需要把它撤销掉**  
但是这个好像只针对于刚提交就意识到了错误

1. `git reset --hard <commit_id>`  ##  commit_id 为这个commit 之前的一个commit_id
1. `git push origin HEAD --force`
可以使用`HEAD~1` 表示刚刚commit 的 commit_id

**Git: Checkout all files except one**  
When I do a git status, I see files like this:

	modified:  dir/A/file.txt
	modified:  dir/B/file.txt
	modified:  dir/C/file.txt
	modified:  dir/D/file.txt
What I want to do is to discard changes to all files EXCEPT for dir/C/file.txt

Answer:

	git add dir/C/file.txt # this file will stay modified and staged
	git checkout .
If you want to unstage the file after that:

	git reset

<br/>
**Using git in two computers, after merging one branch a into master with computer c1, delte the branch a in c1 both locally and remotely,
switch to computer c2, delelte the branch a locally, and want to delete the branch a remotely in c2, 
but an error is reported, as the remote branch a has been deleted by computer c1, so what should I do to clean the branch status in computer c2?**

original branch status:

	[eric@iis ntop]$ git branch -a
	* master
	  note
	  remotes/origin/HEAD -> origin/master
	  remotes/origin/master
	  remotes/origin/note
note branch 是在其他机器上删除的一个分支

1. `$ git branch -d note` delete the branch locally
1. `$ git remote prune origin` clean the remote branch status
1. `$ git remote set-head origin -a` query the remote and automatically set the origin/HEAD pointer to the remote current branch.
1. `$ git remote set-head origin -d` delete the origin/HEAD symbolic ref

**commit 之后发现commit的message有错误**  
`git commit --amend`   
lets you edit the previous commit message

**commit 之后想起忘了add 另外一个修改的文件**
```
# Edit hello.py and main.py
git add hello.py
git commit

# Realize you forgot to add the changes from main.py
git add main.py
git commit --amend --no-edit
```

# 如何导入外部Git仓库到中国源代码托管平台(Git@OSC)

针对最近有很多网友提问,如何导入外部代码仓库(Github,bitbucket,Google Code等等)到Git@OSC ,给出如下解决方案:

## 方案1
从原始地址clone一份bare仓库

1. `git clone --bare  https://github.com/bartaz/impress.js.git` (例子)  
在Git@OSC上创建一个项目(http://git.oschina.net/projects/new)  
这里注意,不要勾选使用Readme初始化项目,不要选择项目的授权协议和.gitignore 文件,因为这些会导致项目有第一个提交.

1. 记下新建项目后的地址,推荐使用http或者ssh方式皆可,大项目推荐ssh方式.  
`cd impress.js.git`  
`git push --mirror git@git.oschina.net:username/impress-js.git`  
此命令执行完成后即完成导入,删除 impress.js.git 文件夹即可.

## 方案2
此方案,手头已经有了项目的完成仓库,则无需再从第三方代码托管平台上clone下来.

1. 到Git@OSC上创建项目,同样不要选择以上所说的三项.

1. 命令行进入项目目录,git status 确保项目状态为:  
`nothing to commit, working directory clean`  
如果状态不是这样,则需要通过提交,暂存等操作,使项目当前状态为clean.

1. 添加Git@OSC的remote  
`git remote add git-osc git@git.oschina.net:username/translate4j.git`  

1. 推送所有分支和tags  
`git push git-osc --all`  
`git push git-osc --tags`  
以上两条命令执行完毕,即完成导入.

## 方案3
此方案针对导入已有的SVN仓库

1. 同上,先新建项目,不要选择以上所说的三项

1. 使用git-svn工具clone svn仓库,git-svn 已经是Git的默认组建,如果你安装的是较新版本的Git客户端的话,则不需要再单独下载这个组件  
`git svn clone http://translate4j.googlecode.com/svn/trunk/ translate4j`  

1. 同样,进入项目目录,添加remote  
`git remote add git-osc git@git.oschina.net:username/impress.js.git`

1. 同样,推送所有分支和标签  
`git push git-osc --all`  
`git push git-osc --tags`  
以上两条命令执行完毕,即推送完成

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
	如果你运行命令:git remote -v你会发现多出来了一个Bob的远程分支.如下:
	```
	bob https://github.com/DoubleSpout/rrestjs.git (fetch)
	bob https://github.com/DoubleSpout/rrestjs.git (push)
	origin git@github.com:yss/rrestjs.git (fetch)
	origin git@github.com:yss/rrestjs.git (push)
	```
1. 把对方的代码拉到你本地: `git fetch bob`
1. 合并对方的代码: `git merge bob/master`
1. 把最新的代码推送到你的github上: `git push origin master`

