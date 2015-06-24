状态栏: `Session: basic 1 3`  
表示 位于名为basic 的 session, 且位于这个session 的第一个window, 第三个 panel

以后建立tmux的session,还是使用下面的方法:
`tmux new-session -s basic` (basic 为这个session 的名字)  
或者简写为:
`tmux new -s basic`

- prefix-d 挂起(detach)当前的session,回退到您的终端,以后如果还希望进入这个session,可以在终端输入 tmux attach -t xxxx
- prefix-x 关闭当前的panel,如果只剩window了的话,也会关闭当前的window
- prefix-& 退出当前的session
- prefix-c 新建一个window
- prefix-n 转到下一个window
- prefix-w 显示当前的所有window
- prefix-p 显示panel的index,当然我们设定了从1开始.
- prefix-number - 跳转到对应index的panel

复制过程

我的prefix为:C-a

1. prefix Esc 进入复制模式
2. 参考上表移动鼠标(可以通过vi 模式也可以通过方向键)到要复制的区域,移动鼠标时可用vim的搜索功能"/","?"
3. 按 v 开始选择复制区域
4. 选择完成后按y 或者 Enter键退出
5. prefix p 粘贴

