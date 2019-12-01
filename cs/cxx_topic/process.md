### 同步进程和异步进程
- fork, fork-exec, `posix_spawn` 等函数创建的进程都是异步进程, 父进程在成功创建子进程后并不挂起, 两个进程将会各自独立的运行.
- system 创建的进程是同步进程,它会创建一个shell来执行命令或可执行文件.父进程将会被挂起,直到子进程结束,且system调用返回.
- wait, waitpid 函数: 异步进程可以通过执行它将自身挂起,等到子进程结束.当子进程结束后,等待中的父进程收集子进程的退出状态,防止出现僵死进程.

### exit() and `_eixt()`
exit()函数定义在stdlib.h中,而`_exit()`定义在unistd.h中.
exit()和`_exit()`都用于正常终止一个函数.
但`_exit()`直接是一个`sys_exit`系统调用,
而exit()则通常是普通函数库中的一个函数.它会先执行一些清除操作,例如调用执行各终止处理函数,关闭所有标准IO等,然后调用`sys_exit`.

```C++
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("I'm ok!\n");
  printf("Good good study!");
  _exit(0);
}
// 执行输出：
// I'm ok!
```

```C++
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("I'm ok!\n");
  printf("Good good study!");
  exit(0);
}
// 执行输出：
// I'm ok!
// Good good study!
```

