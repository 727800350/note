# go tool
[Go Tooling in Action](https://www.youtube.com/watch?v=uBjoTxosSys)

## go doc
- `go doc fmt`: 查看fmt 包的doc
- `go doc fmt Fprintf`: 查看 fmt 包中Fprintf 的doc
- `godoc -http :6060` 在localhost:6060 查看本地所有的doc

## go vet
Vet examines Go source code and reports suspicious constructs, such as Printf calls whose arguments do not align with the format string.
Vet uses heuristics that do not guarantee all reports are genuine problems

## go get 升级
- go get
  - if the latest A requires module B v1.2.3, while B v1.2.4 and v1.3.1 are also available, then `go get A` will use the latest A but then use B v1.2.3, as requested by A.
- go get -u: 将会升级到最新的次要版本或者修订版本(x.y.z, z是修订版本号, y是次要版本号)
  - `go get -u A` will use the latest A with B v1.3.1 (not B v1.2.3).
- go get -u=patch 将会升级到最新的修订版本
  - `go get -u=patch A@latest` will use the latest A with B v1.2.4 (not B v1.2.3)
  - `go get -u=patch A` will use a patch release of A instead
- go get package@version 将会升级到指定的版本号version


- the @latest suffix will use the latest git tag of gopls.
- 运行go get如果有版本的更改,那么go.mod文件也会更改

## go mod
Using Go 1.13, GO111MODULE's default (auto) changes:

- behaves like GO111MODULE=on anywhere there is a go.mod OR anywhere outside the GOPATH even if there is no go.mod. So you can keep all your repositories in your GOPATH with Go 1.13.
- behaves like GO111MODULE=off in the GOPATH with no go.mod.

with Go 1.14, the GO111MODULE variable has the same behavior as with Go 1.13.

```bash
GO111MODULE=on go get golang.org/x/tools/gopls@latest
```
when using `go get @latest` (for a binary, at least), you should avoid using -u so that it uses the dependencies as defined in go.sum.
Otherwise, it will update all the dependencies to their latest minor revision. And since a ton of projects choose to have breaking changes between minor versions (e.g. v0.2.0 to v0.3.0), using -u has a large chance of breaking things.

when you want to hack one of your dependencies, for example for testing purposes.

1. Solution 1: use go mod vendor + go build -mod=vendor.
  That will force go to use the vendor/ files instead of using the $GOPATH/pkg/mod ones. This option also solves the problem of vim and VSCode not opening the right version of a package's file.
1. Solution 2: add a 'replace' line at the end of your go.mod:
  ```
  use replace github.com/maelvls/beers => ../beers
  ```
  where ../beers is a local copy I made of the dependency I want to inspect and hack.

使用replace替换无法直接获取的package
由于某些已知的原因,并不是所有的package都能成功下载,比如:golang.org(这个下面可以使用GOPROXY 环境变量)下的包.
modules 可以通过在 go.mod 文件中使用 replace 指令替换成github上对应的库,比如:
```
replace (
  golang.org/x/crypto v0.0.0-20190313024323-a1f597ede03a => github.com/golang/crypto v0.0.0-20190313024323-a1f597ede03a
)
```

## go vendor
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

## go 的版本号
v0 is a compatibility-free zone

v2.3.4 semantic versioning

- major version: incremental for backwoods-incompatible changes
- minor version: incremental for new features
- patch version: increment for bug fixes

