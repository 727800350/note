## set the default repo
local({r <- getOption("repos")
	r["CRAN"] <- "http://cran.case.edu/"
	options(repos = r)})

## Every R session starts with excuting the .First function
.First <- function(){
	cat("\nWelcome at ", date(),"\n")
}

## Every R session ends with excuting the .First function
.Last <- function(){
	cat("\nGoodbye at ", date(),"\n")
}
