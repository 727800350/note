# Objects
assign operator
```
n <- 15
15 -> n
```

All objects have two intrinsic attributes: mode and length. 
The mode is the basic type of the elements of the object; there are four main modes:
**numeric, character, complex , and logical** (FALSE or TRUE). 
Other modes exist but they do not represent data, for instance function or expression. 
The **length** is the number of elements of the object. 
`mode(x)`, `length(x)`

overview of the type of objects representing data

| object                                     | modes                                     | several modes possible in the same object? |
|--------------------------------------------|-------------------------------------------|--------------------------------------------|
| vector                                     | numeric, character, complex or logical 	 | No  
| factor                                     | numeric or character 					 | No
| array                                      | numeric, character, complex or logical 	 | No  
| matrix                                     | numeric, character, complex or logical 	 | No  
| data frame                                 | numeric, character, complex or logical 	 | No  
| ts                                         | numeric, character, complex or logical 	 | No  
| list                                       | numeric, character, complex or logical, function, expression, ... 	 | No  

- A factor is a categorical variable
- A data frame is a table composed with one or several vectors and/or factors all of the same length but possibly of different modes. 
- A 'ts' is a time series data set and so contains additional attributes such as frequency and dates. 
- A list can contain any type of object, included lists!

a matrix with 2 lines and 2 columns has for dim the pair of values [2, 2], but its length is 4.

list objects in memory  
`ls()`: only the names of objects
`ls(pattern = "m")`: list the objects with the name containing "m", or using `pat` for short
`ls.str()`: display some details

delete objects in memory  
`rm(x)`, `rm(x,y)` remove the object x and y from memory

# help
`?command`, `help(command)`, `help("command")`.  
The last one must be used to access help with non-conventional characters:
```
> ?*
Error: syntax error
> help("*")
Arithmetic package:base R Documentation
Arithmetic Operators
...
```

`help.start()`: start the help system in browser
