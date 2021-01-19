# intro
tmux: terminal multiplexer

使用它最直观的好处就是,通过一个终端登录远程主机并运行tmux后,在其中可以开启多个控制台而无需再"浪费"多余的终端来连接这台远程主机,当然其功能远不止于此.

tmux采用C/S模型,包括以下单元

- server:服务器,执行tmux命令就开启了一个服务器.
- session:会话,一个服务器可以包含多个会话, 一个session可以看作是tmux管理下的伪终端的一个集合
- window:窗口,一个会话可以包含多个窗口, 每个window都是一个伪终端,会占据整个屏幕
- panel:面板,一个窗口可以包含多个面板.

[结构示意图](http://images.cnblogs.com/cnblogs_com/itech/linux/tmux.png)

# 快捷操作
状态栏: `Session: basic 1 3`  
表示 位于名为basic 的 session, 且位于这个session 的第一个window, 第三个 panel

- prefix-? 列出所有快捷键,按q返回 

## session
位于terminal

- tmux new-session -s basic: 创建名为basic 的session, 简写为 `tmux new -s basic`
- tmux attach -t session_name: 键入session, 简写为 `tmux a -t session_name`
- tmux kill-session -t $session_name 删除指定session
- tmux kill-server 删除所有session

位于tmux

- prefix-d 挂起(detach)当前的session,回退到您的终端
- prefix-D 选择要脱离的会话,在同时开启了多个会话时使用 
- prefix-& 退出当前的session
- prefix-$ rename session
- prefix-s 获取现有会话的列表, 列表中的每个会话都有一个 ID,该 ID 是从 0 开始的.按下对应的 ID 就可以进入会话.
- prefix :kill-server 删除所有session
- prefix :kill-session 删除session

## window
- prefix-c 新建一个window
- prefix-n 转到下一个window
- prefix-w 显示当前的所有window
- prefix-& 删除window
- prefix-, 重命名window

## panel
- prefix-x 关闭当前的panel,如果只剩window了的话,也会关闭当前的window
- prefix-backspace 更换pane排版, 横竖进行变换
- prefix-p 显示panel的index,当然我们设定了从1开始.
- prefix-number - 跳转到对应index的panel

# 复制过程
我的prefix为:C-a

1. prefix-Esc 进入复制模式
2. 参考上表移动鼠标(可以通过vi 模式也可以通过方向键)到要复制的区域,移动鼠标时可用vim的搜索功能"/","?"
3. 按 v 开始选择复制区域
4. 选择完成后按y 或者 Enter键退出
5. prefix-p 粘贴

使用鼠标选中之后(也就是进行复制), prefix-p 也是可以粘贴的

直接用鼠标复制的时候,是直接使用vim处理的, 
如果需要复制到其他地方, 比如在windows 通过xshell 进行操作, 需要复制到win 的系统剪切板中, 那么需要鼠标复制的同时按住 shift.

将复制下来的文本发送到系统的剪贴板中
在默认情况下,当从 Tmux 中复制文本时,复制下来的文本只能粘贴到同一个 Tmux 会话中.
若要使复制下来的文本可以粘贴到任何位置,就需要让 Tmux 将文本复制到系统的剪贴板.为此,我们需要这样做:

1. reattach-to-user-namespace
1. 在配置文件 ~/.tmux.conf 中加入以下内容:
```
# invoke reattach-to-user-namespace every time a new window/pane opens
set-option -g default-command "reattach-to-user-namespace -l bash"
```

# 设置
[.bashrc/.profile is not loaded on new tmux session (or window) — why?](
https://stackoverflow.com/questions/9652126/bashrc-profile-is-not-loaded-on-new-tmux-session-or-window-why)

tmux 会自动加载 .bash_profile, 而不会自动加载 .bashrc, 所以可以在 .bash_profile 中增加 `source ~/.bashrc` 的内容.

[lose vim colorscheme in tmux mode](https://stackoverflow.com/questions/10158508/lose-vim-colorscheme-in-tmux-mode)
To fix the issue, I have set up an alias in ~/.bashrc:
```bash
alias tmux="TERM=screen-256color-bce tmux"
```
And set up the default-terminal option in ~/.tmux.conf:
```tmux
set -g default-terminal "xterm"
```
Lastly, do `source ~/.bashrc` to load new alias.

查看prefix现有绑定键:
`tmux show-options -g | grep prefix`

要在tmux内置命令中修改及时生效,可在终端中输入以下命令:
```
tmux set -g prefix C-x
tmux unbind C-b 
tmux bind C-x send-prefix
```
其中第一个-g设置全局生效快捷键.其实一般第一个设置好后新的快捷键就可以生效,如果需要再使用第二个和第三个命令.

要永久生效,则在创建或修改系统级的/etc/tmux.conf或用户级的~/.tmux.conf,里面的内容是上面三个单独命令集,如下:
```
set -g prefix C-x
unbind C-b
bind C-x send-prefix
```
这个不是及时生效,需要重启系统. 或者运行 `tmux source-file ~/.tmux.conf`


