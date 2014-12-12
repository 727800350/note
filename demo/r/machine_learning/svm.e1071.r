library(e1071)
library(MASS)
data(cats)
summary(cats)
model <- svm(Sex~., data = cats)
print(model)
summary(model)
plot(model, cats)

index <- 1:nrow(cats)
test_index <- sample(index, trunc(length(index)/3))
test_set <- cats[test_index,]
train_set <- cats[-test_index,]
model <- svm(Sex~., data = train_set)
prediction <- predict(model, test_set[,-1])
tab <- table(pred = prediction, true = test_set[,1])
print(tab)
classAgreement(tab)

tuned <- tune.svm(Sex~., data = train_set, gamma = 10^(-6:-1), cost = (1:3))
summary(tuned)

