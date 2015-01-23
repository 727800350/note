## What makes data wide or long?

## wide-format data:
#   ozone   wind  temp
# 1 23.62 11.623 65.55
# 2 29.44 10.267 79.10
# 3 59.12  8.942 83.90
# 4 59.96  8.794 83.97

## And this is long-format data:
#    variable  value
# 1     ozone 23.615
# 2     ozone 29.444
# 3     ozone 59.115
# 4     ozone 59.962
# 5      wind 11.623
# 6      wind 10.267
# 7      wind  8.942
# 8      wind  8.794
# 9      temp 65.548
# 10     temp 79.100
# 11     temp 83.903
# 12     temp 83.968

library(reshape2)
## change the column names to lower case to make them easier to work with
names(airquality) <- tolower(names(airquality))
head(airquality)
#   ozone solar.r wind temp month day
# 1    41     190  7.4   67     5   1
# 2    36     118  8.0   72     5   2
# 3    12     149 12.6   74     5   3
# 4    18     313 11.5   62     5   4
# 5    NA      NA 14.3   56     5   5
# 6    28      NA 14.9   66     5   6

## Maybe here we want to know the values of ozone, solar.r, wind, and temp for each month and day
aql <- melt(airquality, id.vars = c("month", "day"))
head(aql)
#   month day variable value
# 1     5   1    ozone    41
# 2     5   2    ozone    36
# 3     5   3    ozone    12
# 4     5   4    ozone    18
# 5     5   5    ozone    NA
# 6     5   6    ozone    28

tail(aql)
##     month day variable value
## 607     9  25     temp    63
## 608     9  26     temp    70
## 609     9  27     temp    77
## 610     9  28     temp    75
## 611     9  29     temp    76
## 612     9  30     temp    68

aql <- melt(airquality, id.vars = c("month", "day"),
  variable.name = "climate_variable", 
  value.name = "climate_value")
head(aql)
#   month day climate_variable climate_value
# 1     5   1            ozone            41
# 2     5   2            ozone            36
# 3     5   3            ozone            12
# 4     5   4            ozone            18
# 5     5   5            ozone            NA
# 6     5   6            ozone            28

## we need to tell dcast that month and day are the ID variables (we want a column for each) and that variable describes the measured variables. 
## Since there is only one remaining column, dcast will figure out that it contains the values themselves. We could explicitly declare this with value.var.
aqw <- dcast(aql, month + day ~ variable)
head(aqw)
#   month day ozone solar.r wind temp
# 1     5   1    41     190  7.4   67
# 2     5   2    36     118  8.0   72
# 3     5   3    12     149 12.6   74
# 4     5   4    18     313 11.5   62
# 5     5   5    NA      NA 14.3   56
# 6     5   6    28      NA 14.9   66
## So, besides re-arranging the columns, we’ve recovered our original data

## 在同一个month下, 因为有多天, 所以对于每个field, 每一个month 会有多个值, 在cast时, R 不清楚选择哪一个, 所以会警告, 
## 提示我们需要进行聚合, for example: min, max, mean, sum etc
# Aggregation function missing: defaulting to length
dcast(aql, month ~ variable)
#   month ozone solar.r wind temp
# 1     5    31      31   31   31
# 2     6    30      30   30   30
# 3     7    31      31   31   31
# 4     8    31      31   31   31
# 5     9    30      30   30   30

## na.rm = TRUE through the ... argument to remove NA values.
dcast(aql, month ~ variable, fun.aggregate = mean, na.rm = TRUE)
#   month ozone solar.r   wind  temp
# 1     5 23.62   181.3 11.623 65.55
# 2     6 29.44   190.2 10.267 79.10
# 3     7 59.12   216.5  8.942 83.90
# 4     8 59.96   171.9  8.794 83.97
# 5     9 31.45   167.4 10.180 76.90


## ========================================================
## another example

data(french_fries)
# what does the data look like?
head(french_fries)
##    time treatment subject rep potato buttery grassy rancid painty
##    61    1         1       3   1    2.9     0.0    0.0    0.0    5.5
##    25    1         1       3   2   14.0     0.0    0.0    1.1    0.0
##    62    1         1      10   1   11.0     6.4    0.0    0.0    0.0
##    26    1         1      10   2    9.9     5.9    2.9    2.2    0.0
##    63    1         1      15   1    1.2     0.1    0.0    1.1    5.1
##    27    1         1      15   2    8.8     3.0    3.6    1.5    2.3

# let us melt the data
head(ff_m <- melt(french_fries, id = 1:4))
##   time treatment subject rep variable value
## 1    1         1       3   1   potato   2.9
## 2    1         1       3   2   potato  14.0
## 3    1         1      10   1   potato  11.0
## 4    1         1      10   2   potato   9.9
## 5    1         1      15   1   potato   1.2
## 6    1         1      15   2   potato   8.8

# let's look for missing values
cast(ff_m, subject ~ time, length)
##    subject  1  2  3  4  5  6  7  8  9 10
## 1        3 30 30 30 30 30 30 30 30 30  0
## 2       10 30 30 30 30 30 30 30 30 30 30
## 3       15 30 30 30 30 30 30 30 30 30 30
## 4       16 30 30 30 30 30 30 30 30 30 30
## 5       19 30 30 30 30 30 30 30 30 30 30
## 6       31 30 30 30 30 30 30 30 30  0 30
## 7       51 30 30 30 30 30 30 30 30 30 30
## 8       52 30 30 30 30 30 30 30 30 30 30
## 9       63 30 30 30 30 30 30 30 30 30 30
## 10      78 30 30 30 30 30 30 30 30 30 30
## 11      79 30 30 30 30 30 30 30 30 30  0
## 12      86 30 30 30 30 30 30 30 30  0 30

cast(ff_m, subject ~ time, function(x) 30 - length(x))
##    subject 1 2 3 4 5 6 7 8  9 10
## 1        3 0 0 0 0 0 0 0 0  0 30
## 2       10 0 0 0 0 0 0 0 0  0  0
## 3       15 0 0 0 0 0 0 0 0  0  0
## 4       16 0 0 0 0 0 0 0 0  0  0
## 5       19 0 0 0 0 0 0 0 0  0  0
## 6       31 0 0 0 0 0 0 0 0 30  0
## 7       51 0 0 0 0 0 0 0 0  0  0
## 8       52 0 0 0 0 0 0 0 0  0  0
## 9       63 0 0 0 0 0 0 0 0  0  0
## 10      78 0 0 0 0 0 0 0 0  0  0
## 11      79 0 0 0 0 0 0 0 0  0 30
## 12      86 0 0 0 0 0 0 0 0 30  0

# we can use more complicated functions too 
cast(ff_m, variable ~., range)
cast(ff_m, variable~., function(x) c(min=min(x), max=max(x)))
cast(ff_m, treatment + subject ~ variable, mean, margins = "treatment", subset = subject %in% c(3,10))
##   treatment subject    potato   buttery     grassy   rancid   painty
## 1         1       3  6.216667 0.3722222 0.18888889 2.105556 3.111111
## 2         1      10  9.955000 6.7500000 0.58500000 4.020000 1.375000
## 3         1   (all)  8.184211 3.7289474 0.39736842 3.113158 2.197368
## 4         2       3  6.738889 0.5888889 0.10555556 3.138889 2.477778
## 5         2      10  9.995000 6.9800000 0.47500000 2.150000 0.820000
## 6         2   (all)  8.452632 3.9526316 0.30000000 2.618421 1.605263
## 7         3       3  5.294444 0.7666667 0.09444444 2.855556 2.866667
## 8         3      10 10.030000 6.4500000 0.14500000 3.110000 0.690000
## 9         3   (all)  7.786842 3.7578947 0.12105263 2.989474 1.721053

head(cast(ff_m, ... ~ variable))
##   time treatment subject rep potato buttery grassy rancid painty
## 1    1         1       3   1    2.9     0.0    0.0    0.0    5.5
## 2    1         1       3   2   14.0     0.0    0.0    1.1    0.0
## 3    1         1      10   1   11.0     6.4    0.0    0.0    0.0
## 4    1         1      10   2    9.9     5.9    2.9    2.2    0.0
## 5    1         1      15   1    1.2     0.1    0.0    1.1    5.1
## 6    1         1      15   2    8.8     3.0    3.6    1.5    2.3

