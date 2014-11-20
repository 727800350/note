library(class)
library(e1071)
data(iris)
summary(iris)
## 在脚本中画的图好像会自动保存到当前文件夹下
pairs(iris[1:4], main = "Iris Data (red=setosa,green=versicolor,blue=virginica)", 
	  pch = 21, bg = c("red", "green3", "blue")[unclass(iris$Species)])
classifier <- naiveBayes(iris[,1:4], iris[,5])
table(predict(classifier, iris[,-5]), iris[,5])

