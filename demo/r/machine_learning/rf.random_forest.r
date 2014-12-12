library(randomForest)
data(iris)
set.seed(100)
ind <- sample(2, nrow(iris), replace = TRUE, prob = c(0.8,0.2))
iris.rf <- randomForest(Species~., iris[ind == 1,], ntree = 50, nPerm = 10, mtry = 3, proximity = TRUE, importance = TRUE)
print(iris.rf)
iris.pred <- predict(iris.rf, iris[ind == 2,])
table(observed = iris[ind == 2, "Species"], predicted = iris.pred)

importance(iris.rf)

## get a specific tree from the generated random forest
getTree(iris.rf, k = 1, labelVar = F)

plot(iris.rf)
