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

