[stackOverFlow](http://stackoverflow.com/questions/5270760/whats-the-meaning-of-origin-in-git-push-origin-master)
origin is the default name of the remote git repository you cloned from. Have a look at *.git/refs/remotes/origin/** and .git/config within your sources to see how git knows about it.

[关于git中分支的困惑](http://my.eoe.cn/com360/archive/3572.html)
先说下我对git仓库结构的理解，就以我是一个git用户而言，
仓库分为：
远程仓库(blasd(remote) repository）和本地仓库（local repository）
本地仓库有分为：
工作区（workspace），中间状态（staged），本地仓库（local repository）
中间状态介于工作区和本地仓库的中间状态，比如，在工作区增加了一个test.txt文件，已经add了，
但是还没有commit的状态。
现在大家可能还没有看到到底解决的是什么困惑，大家一步步跟着来吧。
1. clone 代码代码到本地
地址是一个测试远程仓库：https://github.com/com360/android-app.git
输入以下命令：

1
 $git clone https://github.com/com360/android-app.git
在clone完之后，git会自动为你将此远程仓库命名为origin,origin只是为远程仓库起的一个别名，运行git remote –v 可以查看origin的含义；并且git会建立一个指向远程仓库origin的master 分支的标识，git使用
(远程仓库名)/(分支名) 这样的形式表示远程分支，所以origin/master指向的是一个我们clone到本地的远程分支，被代表为远程创仓库和分支。
同时，git会建立一个本地master分支，它指向的是从远程仓库的分支下载到本地的副本。

2.查看分支有哪些
输入如下命令：
$ git branch -a
* master
  remotes/eoecn/master
  remotes/origin/HEAD -> origin/master
  remotes/origin/master
su@SUCHANGLI /e/eoe_client/android-app (master)
git branch -a 用于查看所有的分支。
*master就是你本地的分支
remotes/eoecn/master和remotes/origin/master是远程分支
是和eoecn/master，origin/master相同的意思
可以只看远程仓库的分支

$ git branch -r
  eoecn/master
  origin/HEAD -> origin/master
  origin/master
su@SUCHANGLI /e/eoe_client/android-app (master)
3.比较分支的不同

$ git diff origin/master master
上面的命令就是比较远程的分支和本地分支的不同
为了验证remotes/origin/master和origin/master指向是相同的可以使用上面的命令

$ git diff origin/master remotes/origin/master
提交更新到远程仓库 如下命令：
$ git push origin master
其中origin指定了你要push到哪个远程仓库
正常的形式其实是:，冒号前表示本地分支的名字，冒号后表示远程仓库的分支的名字。
上面的命令省略了远程仓库分支的名称，如果省略了代表远程的分支的名字使用本地分支的名字。
上面的命令等价于
$ git push origin master:master

远程分支和本地分支的名字相同，可以省略远程分支的名字
注意：如果远程没有master分支的话，就会新建一个以此名字命名的远程分支
也可以这样写：
$ git push origin master:refs/for/branch1 
在本地仓库中找到名字为master的分支，用他去更新远程仓库下面名字为branch1的分支

还有一个小技巧，你不知道当前分支的名称的话，可以用前面的命令去查看
如果不想查看，可以这样
$ git push origin HEAD:refs/for/branch1 
使用HEAD作为当前分支的代表符号

如果本地分支名称给省略了，代表用空的分支去更新远程分支，间接的实现对远程分支内容的删除
例如：
$ git push origin :branch1
看到这里就明白了要解决的问题，哪个代表本地分支，哪个代表远程分支。
如果错误，欢迎指正。

