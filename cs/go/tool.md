[Go Tooling in Action](https://www.youtube.com/watch?v=uBjoTxosSys)

# go test
- `-run regex`: cause go test to run only thouse tests whose function name matches the pattern
- `-testify.m`: 对于使用了 github.com/stretchr/testify/suite 的测试, 指定这个suite 下的具体某一个测试
- `-count=1`: 关闭test cache
- `-gcflags=all=-l`: 对于使用了 github.com/agiledragon/gomonkey 需要防止默认的 inline (如果inline了估计就不好在所有引用
  到代码的地方设置jmp指令了, 所以要防止inline)

- `t.Fatal` or `t.Fatalf` must be called from the same goroutine as the Test function, not from another one created
  during the test.

Sometimes an external test pkg may need privileged access to the internals of the pkg under test.
In such cases, we use a trick: we add declarations to in in-pkg `_test.go` file to expose the necessary internals to the
external test.
This file thus offers the test a back door to the pkg. If the source file exists only for this purpose and contains no
tests itself, it is called `export_test.go`
```go
// export_test.go
package fmt
var IsIspace = isSpace  // isSpace is a function
```

`go test -c`: 生成用于运行测试的可执行文件,但不执行它,这个可执行文件会被命名为pkg.test

```bash
./pkg.test -test.v -test.run xxx
```

# go list
`...` wildcard, matches any substring of a pkg's import path.

- `go list ...`: list all pkgs with a Go workspace
- `go list gopl.io/...`: list all pkgs within a specific subtree
- `go list ...xml...`: list all pkgs related to a particular topic
- `go list -json`: print the entire record in json format

# go doc
- `go doc fmt`: 查看fmt 包的doc
- `go doc fmt.Fprintf`: 查看 fmt 包中Fprintf 的doc
- `go doc -src fmt.Fprintf`: 查看 fmt 包中Fprintf 的源码
- `godoc -http :6060` 在localhost:6060 查看本地所有的doc

# go vet
Vet examines Go source code and reports suspicious constructs, such as Printf calls whose arguments do not align with the format string.
Vet uses heuristics that do not guarantee all reports are genuine problems

# go get
- go get
  - if the latest A requires module B v1.2.3, while B v1.2.4 and v1.3.1 are also available, then `go get A` will use the latest A but then use B v1.2.3, as requested by A.
- go get -u: 将会升级到最新的次要版本或者修订版本(x.y.z, z是修订版本号, y是次要版本号)
  - `go get -u A` will use the latest A with B v1.3.1 (not B v1.2.3).
- go get -u=patch 将会升级到最新的修订版本
  - `go get -u=patch A@latest` will use the latest A with B v1.2.4 (not B v1.2.3)
  - `go get -u=patch A` will use a patch release of A instead
- go get package@version 将会升级到指定的版本号version
- `go get pkg/...`: download an entire subtree or repository

- the @latest suffix will use the latest git tag of gopls.
- 运行go get如果有版本的更改,那么go.mod文件也会更改

```bash
GO111MODULE=on go get golang.org/x/tools/gopls@latest
```
when using `go get @latest` (for a binary, at least), you should avoid using -u so that it uses the dependencies as defined in go.sum.
Otherwise, it will update all the dependencies to their latest minor revision. And since a ton of projects choose to have breaking changes between minor versions (e.g. v0.2.0 to v0.3.0), using -u has a large chance of breaking things.

# go mod
[Go Modules 内部分享](https://xuanwo.io/2019/05/27/go-modules/)

go mod 常用命令

| command  | description                    |
| ---      | ---                            |
| tidy     | 拉取缺少的模块, 移除不用的模块 |
| graph    | 文本的形式打印模块依赖图       |
| why $pkg | 解释为什么需要依赖这个模块     |

## 导入版本控制
如果旧包和新包具有相同的导入路径,则新包必须向后兼容旧包.
换言之,如果他们的导入路径不同,他们就无需保持兼容.

v2.3.4 semantic versioning

- major version: incremental for backwoods-incompatible changes
- minor version: incremental for new features
- patch version: increment for bug fixes

其次, 当主版本号大于等于 v2 时,这个 Module 的 import path 必须在尾部加上 /vN.

- 在 go.mod 文件中: module github.com/my/mod/v2
- 在 require 的时候: require github.com/my/mod/v2 v2.0.0
- 在 import 的时候: import "github.com/my/mod/v2/mypkg"

最后,当主版本号为 v0 或者 v1 时,尾部的 /v0 或 /v1 可以省略.

根据语义化版本的要求,v0 是不需要保证兼容性的,可以随意的引入破坏性变更,所以不需要显式的写出来,
而省略 v1 更大程度上是现实的考虑,毕竟 99% 的包都不会有 v2,同时考虑到现有代码库的兼容,省略 v1 是一个合情合理的决策.

## go.mod
Using Go 1.13, GO111MODULE's default (auto) changes:

- behaves like GO111MODULE=on anywhere there is a go.mod OR anywhere outside the GOPATH even if there is no go.mod.
  So you can keep all your repositories in your GOPATH with Go 1.13.
- behaves like GO111MODULE=off in the GOPATH with no go.mod.

with Go 1.14, the GO111MODULE variable has the same behavior as with Go 1.13.

### module
用来声明当前 module,如果当前版本大于 v1 的话,还需要在尾部显式的声明 /vN.
```info
module /path/to/your/mod/v2

module github.com/Xuanwo/go-mod-intro/v2
```

### require
这是最为常用的部分,在 mod 之后可以写任意有效的,能指向一个引用的字符串,比如 Tag,Branch,Commit 或者是使用 latest 来表示引用最新的 commit.
如果对应的引用刚好是一个 Tag 的话,这个字符串会被重写为对应的 tag,
如果不是的话,这个字符串会被规范化为形如 v2.0.0-20180128182452-d3ae77c26ac8 这样的字符串.
我们后面会发现这个字符串与底层的 mod 存储形式是相对应的.
```info
require /your/mod tag/branch/commit

require github.com/google/go-github/v24 v24.0.1
require gopkg.in/urfave/cli.v2 v2.0.0-20180128182452-d3ae77c26ac8
```

### replace
replace 这边的花样比较多,主要是两种,一个是与 require 类似,可以指向另外一个 repo,另一种是可以指向本地的一个目录.

需要额外注意的是,如果引用一个本地路径的话,那这个目录下必须要有 go.mod 文件,这个目录可以是绝对路径,也可以是相对路径.

```info
replace original_name => real_name tag/branch/commit
replace original_name => local_path

replace test.dev/common => git.example.com/bravo/common.git v0.0.0-20190520075948-958a278528f8
replace test.dev/common => ../../another-porject/common-go
replace github.com/qiniu/x => github.com/Xuanwo/qiniu_x v0.0.0-20190416044656-4dd63e731f37
```

使用replace替换无法直接获取的package
由于某些已知的原因,并不是所有的package都能成功下载,比如:golang.org(这个下面可以使用GOPROXY 环境变量)下的包.
modules 可以通过在 go.mod 文件中使用 replace 指令替换成github上对应的库,比如:
```go
replace (
  golang.org/x/crypto v0.0.0-20190313024323-a1f597ede03a => github.com/golang/crypto v0.0.0-20190313024323-a1f597ede03a
)
```

# go vendor
govender 是对Golang的包依赖管理的一个插件,该工具将项目依赖的外部包拷贝到项目下的 vendor 目录下,并通过 vendor.json 文件来记录依赖包的版本,方便用户使用相对稳定的依赖.

安装govendor
```bash
go get -u github.com/kardianos/govendor
```

- `govendor add`: Add packages from $GOPATH
- `govendor fetch`: Add new or update vendor folder packages from remote repository

# go tool options
-toolexec: add a prefix to every command it runs
```bash
% go build -toolexec=... github.com/pkg/profile
% go test -toolexec=... net/http
```

-x that shows each command the go tool invokes
```bash
% go build -x fmt
WORK=/var/folders/lv/028ssy8n19v56_k2q43kkb5r0000gn/T/go-build339335935
mkdir -p $WORK/fmt/_obj/
mkdir -p $WORK/
cd /Users/dfc/go/src/fmt
/Users/dfc/go/pkg/tool/darwin_amd64/compile -o $WORK/fmt.a -trimpath $WORK -p fmt -complete -buildi
```

# env
- `GOOS=windows go build`: 生成exe 文件
- `runtime.GOOS`, `runtime.GOARCH`

Some packages may need to compile different versions of the code for certain platforms or processors.
If a file name includes an operating system or processor architecutre name like `net_linux.go` or `asm_amd64.s`, then the go tool will compile the file when building for that target.

