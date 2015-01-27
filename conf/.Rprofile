options(scipen = 999) ## disable scientific notation
options(tab.width = 2)
options(width = 120)
options(papersize = "a4")

## set the default repo
local({r <- getOption("repos")
	r["CRAN"] <- "http://cran.case.edu/"
	options(repos = r)})

## Every R session starts with excuting the .First function
.First <- function(){
	library(ggplot2)
	library(magrittr)
	source("/home/eric/.Rfun.r")
	cat("\nWelcome at ", date(),"\n")
}

## Every R session ends with excuting the .First function
.Last <- function(){
	cat("\nGoodbye at ", date(),"\n")
}

