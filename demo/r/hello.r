hello <- function(n){
	.C("hello", as.integer(n))
}

dyn.load("hello.so")
hello(30)
