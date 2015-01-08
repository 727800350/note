[git官方教程](http://git-scm.com/book/zh/%E8%B5%B7%E6%AD%A5-%E5%85%B3%E4%BA%8E%E7%89%88%E6%9C%AC%E6%8E%A7%E5%88%B6)

[廖雪峰的git 教程](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/001375840202368c74be33fbd884e71b570f2cc3c0d1dcf000)

添加密钥, 参见[bash](./bash.md) 中的ssh部分

在日常的git操作中,git checkout,是我们的常用命令,既可以是某一个具体的commit hash值,也可以是某个分支名称, 还可以是tag名称

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

# FAQ
**错误提交了一个commit, 需要把它撤销掉**
```
git reset --hard <commit_id>  ##  commit_id 为这个commit 之前的一个commit_id
git push origin HEAD --force
```

**Git: Checkout all files except one**  
When I do a git status, I see files like this:

	modified:  dir/A/file.txt
	modified:  dir/B/file.txt
	modified:  dir/C/file.txt
	modified:  dir/D/file.txt
What I want to do is to discard changes to all files EXCEPT for dir/C/file.txt

I want to do something like this:

	git checkout -- dir/!C/file.txt

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

delete the branch locally

	[eric@iis ntop]$ git branch -d note

clean the remote branch status

	[eric@iis ntop]$ git remote prune origin

query the remote and automatically set the origin/HEAD pointer to the remote's current branch.

	[eric@iis ntop]$ git remote set-head origin -a

delete the origin/HEAD symbolic ref

	[eric@iis ntop]$ git remote set-head origin -d
Done

**commit 之后发现commit的message有错误**  
`git commit --amend`   
lets you edit the previous commit's message

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

