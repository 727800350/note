# ggplot2 examples
## http://www.statmethods.net/advgraphs/ggplot2.html
library(ggplot2) 

# create factors with value labels 
mtcars$gear <- factor(mtcars$gear,levels=c(3,4,5), labels=c("3gears","4gears","5gears")) 
mtcars$am <- factor(mtcars$am,levels=c(0,1), labels=c("Automatic","Manual")) 
mtcars$cyl <- factor(mtcars$cyl,levels=c(4,6,8), labels=c("4cyl","6cyl","8cyl")) 

## > head(mtcars)
##                    mpg  cyl disp  hp drat    wt  qsec vs        am   gear carb
## Mazda RX4         21.0 6cyl  160 110 3.90 2.620 16.46  0    Manual 4gears    4
## Mazda RX4 Wag     21.0 6cyl  160 110 3.90 2.875 17.02  0    Manual 4gears    4
## Datsun 710        22.8 4cyl  108  93 3.85 2.320 18.61  1    Manual 4gears    1
## Hornet 4 Drive    21.4 6cyl  258 110 3.08 3.215 19.44  1 Automatic 3gears    1
## Hornet Sportabout 18.7 8cyl  360 175 3.15 3.440 17.02  0 Automatic 3gears    2
## Valiant           18.1 6cyl  225 105 2.76 3.460 20.22  1 Automatic 3gears    1
## mpg: miles per gallon
## cyl: 气缸数量
## hp:hore power
## gear: 几个档位
## am: 手动还是自动
## wt: 重量

# Kernel density plots for mpg
# grouped by number of gears (indicated by color)
qplot(mpg, data=mtcars, geom="density", fill=gear, alpha=I(.5), 
   main="Distribution of Gas Milage", xlab="Miles Per Gallon", ylab="Density")

# Scatterplot of mpg vs. hp for each combination of gears and cylinders
# in each facet, transmittion type is represented by shape and color
qplot(hp, mpg, data=mtcars, shape=am, color=am, facets=gear~cyl, size=I(3),
   xlab="Horsepower", ylab="Miles per Gallon") 

# Separate regressions of mpg on weight for each number of cylinders
qplot(wt, mpg, data=mtcars, geom=c("point", "smooth"), 
   method="lm", formula=y~x, color=cyl, 
   main="Regression of MPG on Weight", xlab="Weight", ylab="Miles per Gallon")

# Boxplots of mpg by number of gears 
# observations (points) are overlayed and jittered
qplot(gear, mpg, data=mtcars, geom=c("boxplot", "jitter"), fill=gear, 
	main="Mileage by Gear Number", xlab="", ylab="Miles per Gallon")
