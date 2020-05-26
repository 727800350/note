# Intro
sed接受一个或多个编辑命令,并且每读入一行后就依次应用这些命令.
当读入第一行输入后,sed对其应用所有的命令,然后将结果输出.接着再读入第二行输入,对其应用所有的命令^并重复这个过程.

[sed 简明教程](http://coolshell.cn/articles/9104.html)

sed script file should only contain sed commands

正则表达式的一些最基本的东西:

- `^` 表示一行的开头.如:/^#/ 以#开头的匹配.
- `$` 表示一行的结尾.如:/}$/ 以}结尾的匹配.
- `<` 表示词首. 如 \<abc 表示以 abc 为首的詞.
- `>` 表示词尾. 如 abc\> 表示以 abc 結尾的詞.
- `.` 表示任何单个字符.
- `*` 表示某个字符出现了0次或多次.
- `[ ]` 字符集合. 如:[abc]表示匹配a或b或c,还有[a-zA-Z]表示匹配所有的26个字符.如果其中有^表示反,如[^a]表示非a的字符

# 基本操作
关于address,几乎上述所有的命令都是这样的(注:其中的!表示匹配成功后是否执行命令)
```
[address[,address]][!]{cmd}
```
关于address可以使用相对位置,如: `$ sed '/dog/,+3s/^/# /g' pets.txt`, 其中的+3表示后面连续3行

命令打包, cmd可以是多个,它们可以用分号分开,可以用大括号括起来作为嵌套命令.下面是几个例子:

- `$ sed '3,6 {/This/d}' pets.txt`: 对第3到第6行, 如果匹配到This, 就删除这行
- `$ sed '3,6 {/This/{/fish/d}}' pets.txt`: 对3行到第6行,匹配/This/成功后,再匹配/fish/,成功后执行d命令
- `$ sed '1,${/This/d;s/^ *//g}' pets.txt` 从第一行到最后一行,如果匹配到This,则删除之,如果前面有空格,则去除空格

# s:替换
```
[address]s/pattern/replacement/flags
```
flags

- n: 1~512 之间的数字,表示指定要替换的字符串出现第几次时才进行替换,例如,一行中有 3 个 A,但用户只想替换第二个 A,这是就用到这个标记,
- g: 对数据中所有匹配到的内容进行替换,如果没有 g,则只会在第一次匹配成功时做替换操作.例如,一行数据中有 3 个 A,则只会替换第一个 A,
- n 和g 一起使用, eg: 第3个以后的src: `sed -i '/pattern/s/src/dst/3g' my.txt`

指代变量

- 我们可以使用&来当做被匹配的变量: `$ sed 's/my/[&]/g' my.txt`, 这里& 就会表示被匹配的my
- 圆括号匹配: `$ sed 's/This is my \(a\) is \(b\)/\1 and \2/g' my.txt`: \1 表示被匹配的a, \2 表示被匹配的b

使用shell 变量: `sed "/pattern/s/target/${target}/"`

## N: 缓冲区
**把当前行和下一行作为一行进行处理**, 换行符保留在新的一行中

- `$ sed 'N;s/my/your/' pets.txt`

# a: 追加, i 前插
a命令就是append, i命令就是insert,它们是用来添加行的

- 在第1行前插入一行(insert): `$ sed "1 i insert" my.txt`
- 最后1行后追加一行(append): `$ sed "$ a append" my.txt`
- 先匹配再添加文本: `$ sed "/pattern/a append" my.txt` 匹配到/fish/后就追加一行

# c: 整行替换
- 替换第二行: `$ sed "2 c new line" my.txt`
- 替换匹配行: `$ sed "/fish/c new line" my.txt`

# d: 删除
```
[address]d
```

- 删除匹配行: `$ sed '/fish/d' my.txt`
- 删除第二行: `$ sed '2d' my.txt`
- 从第二行开始删除: `$ sed '2,$d' my.txt`
- `sed '/start/,/end/d'`: 从start 匹配行开始, end 匹配行结束执行删除, 包括边界的两行

# sed y 转换脚本命令
y 转换命令是唯一可以处理单个字符的 sed 脚本命令,其基本格式如下:
```
[address]y/inchars/outchars/
```
转换命令会对 inchars 和 outchars 值进行一对一的映射,即 inchars 中的第一个字符会被转换为 outchars 中的第一个字符,第二个字符会被转换成 outchars 中的第二个字符...这个映射过程会一直持续到处理完指定字符.
如果 inchars 和 outchars 的长度不同,则 sed 会产生一条错误消息.

# p: 打印
类似 grep

- 只显示匹配行: `$ sed -n '/fish/p' my.txt`
- 显示区间: `$ sed -n '/dog/,/fish/p' my.txt`
- 从第1行到fish那一行: `$ sed -n '1,/fish/p' my.txt`

