## 从系统与字符串转换来的时间类型是不一样的
> now <- Sys.time()
> now
## [1] "2015-01-23 22:47:11 CST"
> class(now)
## [1] "POSIXct" "POSIXt"
> mode(now)
## [1] "numeric"
> typeof(now)
## [1] "double"

> t <- strptime("2005-06-24 10:05:35","%Y-%m-%d %H:%M:%S")
> class(t)
## [1] "POSIXlt" "POSIXt"
> mode(t)
## [1] "list"
> typeof(t)
## [1] "list"

## 但是还是可以直接作差的
> now - t
## Time difference of 3500.529 days
> difftime(now, t)
## Time difference of 3500.529 days

