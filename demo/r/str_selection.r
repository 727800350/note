> str <- "this is a test"
> str
## [1] "this is a test"
> r <- strsplit(str, " ") ## "this" "is"   "a"    "test"
> length(r)  ## 不知道为什么length 是1 
## [1] 1
> mode(r)
## [1] "list"
> sapply(r,"[",1)
## [1] "this"
> r[1]
## [[1]]
## [1] "this" "is"   "a"    "test"
> length(r[[1]])
## [1] 4
> r[[1]][4]
## [1] "test"

