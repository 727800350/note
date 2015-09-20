## move app to the last column
moveme <- function(invec, movecommand){
	movecommand <- lapply(strsplit(strsplit(movecommand, ";")[[1]], ",|\\s+"), function(x) x[x != ""])
	movelist <- lapply(movecommand, function(x){
		Where <- x[which(x %in% c("before", "after", "first", "last")):length(x)]
		ToMove <- setdiff(x, Where)
		list(ToMove, Where)
	})
	myVec <- invec
	for(i in seq_along(movelist)){
		temp <- setdiff(myVec, movelist[[i]][[1]])
		A <- movelist[[i]][[2]][1]
		if(A %in% c("before", "after")){
			ba <- movelist[[i]][[2]][2]
			if(A == "before") {
				after <- match(ba, temp) - 1
			}
			else if(A == "after"){
				after <- match(ba, temp)
			}
		}
		else if(A == "first"){
			after <- 0
		}
		else if(A == "last"){
			after <- length(myVec)
		}
		myVec <- append(temp, values = movelist[[i]][[1]], after = after)
	}
	myVec
}

## check for na, nan and inf in a data frame
valid <- function(x){
	if(is.na(x)){
		return(1)
	}else if(is.nan(x)){
		return(2)
	}else if(is.infinite(x)){
		return(3)
	}else{
		return(0)
	}
}

valid.df <- function(df){
	nr <- nrow(df)
	nc <- ncol(df)
	i <- 1
	while(i <= nr){
		j <- 1
		while(j <= nc){
			r <- valid(df[i,j])
			if(r != 0){
				cat("row", i, "col", j, "is not valid of type:", r, "colname:" , colnames(df)[j], "\n")
				return(r)
			}
			j <- j + 1
		}
		i <- i + 1
	}
}

## set the selection solution
## to be refined, for example: when the left number of classes is small, we could select more cases
number <- function(x){
	if(x < 1000){
		return(x)
	}
	else if(x < 5000){
		return(floor(x / 2))
	}
	else if(x < 10000){
		return(floor(x / 5))
	}
	else if(x < 100000){
		return(floor(x / 10))
	}
	else{
		return(floor(x / 20))
	}
}

