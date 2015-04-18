hash()

hash(key=letters, values=1:26 )
## > hash(key=letters, values=1:26)
## <hash> containing 2 key-value pair(s).
##   key : a b c d e f g h i j k l m n o p q r s t u v w x y z
##   values :  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26

## 注意和下面的区别
hash(letters, 1:26)
## > hash(letters, 1:26)
## <hash> containing 26 key-value pair(s).
##   a : 1
##   b : 2
## 	...
##   y : 25
##   z : 26

hash(1:3, lapply(1:3, seq, 1))

hash(a=1, b=2, c=3)
## > h
## <hash> containing 3 key-value pair(s).
##   a : 1
##   b : 2
##   c : 3
hash(c(a=1, b=2, c=3)) ## 结果同上
hash(list(a=1,b=2,c=3)) ## 结果同上

hash(c("foo","bar","baz"), 1:3)
hash(c("foo","bar","baz"), lapply(1:3, seq, 1))

h <- hash(letters, 1:26)
## 取其中元素
h$a
h$b
h[[ "a" ]]
h[ letters[1:3]]

## 对其中元素重新赋值
h$a<-100
h[['a']]<-letters

is.hash(h)
as.list(h)

clear(h)
rm(h)

