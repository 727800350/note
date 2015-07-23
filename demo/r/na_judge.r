## 几个特殊例子
pi / 0 ## = Inf a non-zero number divided by zero creates infinity
0 / 0  ## =  NaN

1/0 + 1/0 # Inf
1/0 - 1/0 # NaN, infinity - infinity 的结果不定, 所以是NaN

## numeric
y <- c(1,2,3,NA)
is.na(y) # returns a vector (F F F T)

## charactor
ch <- c("test", " ", "", NA)
is.na(ch) # return FALSE FALSE FALSE  TRUE

## factor
head(flows$app_name)
## [1] MICROSOFT-DS MICROSOFT-DS <NA> <NA> <NA> <NA>
## Levels: DOMAIN FTP HTTP MICROSOFT-DS NETBIOS-NS SMTP
is.na(head(flows$app_name))
## [1] FALSE FALSE  TRUE  TRUE  TRUE  TRUE

## data.frame
head(flows[,c('app_name', 'srcaddr')])
##       app_name      srcaddr
## 1 MICROSOFT-DS 10.68.106.21
## 2 MICROSOFT-DS  10.231.2.89
## 3         <NA> 10.192.24.50
## 4         <NA>    10.1.17.1
## 5         <NA>   10.2.88.12
## 6         <NA>    10.1.17.1
is.na(head(flows[,c('app_name', 'srcaddr')]))
##   app_name srcaddr
## 1    FALSE   FALSE
## 2    FALSE   FALSE
## 3     TRUE   FALSE
## 4     TRUE   FALSE
## 5     TRUE   FALSE
## 6     TRUE   FALSE
complete.cases(head(flows[,c('app_name', 'srcaddr')]))
## [1]  TRUE  TRUE FALSE FALSE FALSE FALSE
!complete.cases(head(flows[,c('app_name', 'srcaddr')]))
## [1] FALSE FALSE  TRUE  TRUE  TRUE  TRUE
