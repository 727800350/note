exit <- function(){
	q()
}

mystats <- function(x, na.omit = TRUE){
	if(na.omit)
		x <- x[!is.na(x)]
	m <- mean(x)
	n <- length(x)
	s <- sd(x)
	skew <- sum((x-m)^3/s^3)/n
	kurt <- sum((x-m)^4/s^4)/n - 3
## 	return(c(n = n, mean = m, sd = s, skew = skew, kurt = kurt))
	return(c(mean = m, sd = s, skew = skew, kurt = kurt))
}

a4.w <- 8.27
a4.h <- 11.69

