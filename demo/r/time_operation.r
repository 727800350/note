> earlier <- strptime("2000-01-01 00:00:00","%Y-%m-%d %H:%M:%S")
> later <- strptime("2000-01-01 00:00:20","%Y-%m-%d %H:%M:%S")
> later-earlier
## Time difference of 20 secs
> as.double(later-earlier)
## [1] 20

> earlier <- strptime("2000-01-01 00:00:00","%Y-%m-%d %H:%M:%S")
> later <- strptime("2000-01-01 01:00:00","%Y-%m-%d %H:%M:%S")
> later-earlier
## Time difference of 1 hours
> as.double(later-earlier)
## [1] 1

> up <- as.Date("1961-08-13")
> down <- as.Date("1989-11-9")
> down-up
## Time difference of 10315 days
## 以上说明时间直接作差的结果取决于差值的大小.
## 使用`difftime`可以避免这个问题
difftime(time1, time2, 'arg')
## arg should be one of "auto", "secs", "mins", "hours", "days", "weeks"

> earlier <- strptime("2000-01-01 00:00:00","%Y-%m-%d %H:%M:%S")
> later <- strptime("2000-01-01 01:00:00","%Y-%m-%d %H:%M:%S")
> difftime(later,earlier)
## Time difference of 1 hours
> difftime(later,earlier,units="secs")
## Time difference of 3600 secs

## difftime 的另外一种用法
> diff <- as.difftime("00:30:00","%H:%M:%S",units="hour")
> diff
## Time difference of 0.5 hours
> Sys.time()
## [1] "2014-01-23 16:45:39 EST"
> Sys.time()+diff
## [1] "2014-01-23 17:15:41 EST"

## The last thing to mention is that once a time stamp is cast into one of R internal formats comparisons can be made in a natural way.
> diff <- as.difftime("00:30:00","%H:%M:%S",units="hour")
> now <- Sys.time()
> later <- now + diff
> now
## [1] "2014-01-23 16:47:48 EST"
> later
## [1] "2014-01-23 17:17:48 EST"
>
> now < later
