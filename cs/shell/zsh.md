# theme
[romkatv/powerlevel10k](https://github.com/romkatv/powerlevel10k)

# plugins
## 设置自动补全
1. github clone项目到$ZSH_CUSTOM/plugins中
```bash
git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions
```
2. 修改zsh配置文件.zshrc添加插件, 同时增加一行代码使得提示代码为灰色
```zshrc
plugins=(git zsh-autosuggestions)
export TERM=xterm-256color
```

## [incr](https://mimosa-pudica.net/zsh-incremental.html)
incr是一款自动提示插件,功能非常强大
```bash
wget http://mimosa-pudica.net/src/incr-0.2.zsh
mkdir ~/.oh-my-zsh/plugins/incr
mv incr-0.2.zsh ~/.oh-my-zsh/plugins/incr
echo 'source ~/.oh-my-zsh/plugins/incr/incr*.zsh' >> ~/.zshrc
source ~/.zshrc
```

## zsh-syntax-highlighting
这个插件的主要作用就是在提高颜值(高亮你的zsh可用命令),安装如下:
```bash
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
```

加入到插件列表
```zshrc
plugins=(
  git
  zsh-autosuggestions
  zsh-syntax-highlighting
)
```

