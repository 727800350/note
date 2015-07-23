> theDates <- c("1 jan 2012","1 jan 2013","1 jan 2014")
> d <- as.Date(theDates,"%d %b %Y")
> d 
## [1] "2012-01-01" "2013-01-01" "2014-01-01"
> class(d)
## [1] "Date"
> mode(d)
## [1] "numeric"
> typeof(d)
## [1] "double"

> infamy <- as.Date(-179,origin="1942-06-04")

