rm(list = ls())  # 首先清理当前环境
getAnywhere(c)$where  # 查看缺省情况下 c 是从哪里来的
## [1] "package:base"   "namespace:base"

c <- 1  # 试图覆盖 c
getAnywhere(c)$where  # 再看看接下去 c 会从哪里被找到
## [1] ".GlobalEnv"     "package:base"   "namespace:base"

c  # 查看 c 这个对象，它首先被从 .GlobalEnv 中找到，于是就返回了我们所覆盖后的数值：
## [1] 1

c(1:2)  # 但若是以函数方式使用， .GlobalEnv 中的 c 不能满足需要，于是继续找到了 package:base 中的 c
## [1] 1 2

c <- function(x) { x[1] + x[2] }  # 如果把 c 覆盖成为另一个函数，则就更清楚了
c(1:2)  # 这时就是我们的函数生效了
## [1] 3

base::c(1:2)  # 当然，你还是可以直接指定使用 package:base 中的 c
## [1] 1 2

rm(c)  # 要恢复，就得把 .GlobalEnv 中的 c 删掉
c(1:2)  # 现在恢复了
## [1] 1 2

getAnywhere(c)$where
## [1] "package:base"   "namespace:base"

## rm(c, pos = "package:base")  # 系统的那个 c 应该指定 pos（或 envir）来删除，当然你是不会成功的
## Error in rm(c, pos = "package:base") :
##   cannot remove variables from the base environment

