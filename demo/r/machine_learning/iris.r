autonorm <- function(data){
	min <- min(data)
	max <- max(data)
	for(i in 1:length(data))
		data[i] <- (data[i] - min)/(max - min)
	return(data)
}

iris <- read.csv("iris.data")
print(iris)
print(dim(iris))

x <- iris[13,1:4]
y <- iris[79,1:4]
z <- iris[100,1:4]
train <- iris[c(-13,-79,-100),]
print(dim(train))
data <- apply(as.matrix(train[,1:4]), 2, autonorm)
print(dim(data))

## 将测试数据进行同样规则的归一化
x <- (x - apply(train[,1:4],2,min)) / (apply(train[,1:4],2,max) - apply(train[,1:4],2,min))
y <- (y - apply(train[,1:4],2,min)) / (apply(train[,1:4],2,max) - apply(train[,1:4],2,min))
z <- (z - apply(train[,1:4],2,min)) / (apply(train[,1:4],2,max) - apply(train[,1:4],2,min))

dis <- rep(0,length(data[,1]))
## 计算测试数据与训练数据的距离
for(i in 1:length(data[,1]))
	dis[i] <- sqrt(sum((z - data[i,1:4])^2))
## 取出与测试数据距离最小的5行数据, 然后将他们的第5列输出
table(train[order(dis)[1:5],5])

library(kknn)
data(iris)
m <- dim(iris)[1]
print(m)
val <- sample(m, size = round(m/5), replace = FALSE, prob = rep(1/m, m))
print(val)
iris.learn <- iris[-val,]
iris.valid <- iris[val,]
iris.kknn <- kknn(Species~., iris.learn, iris.valid, distance = 7, kernel = "optimal")
summary(iris.kknn)
fit <- fitted(iris.kknn)
table(iris.valid$Species, fit)

