f1 = function() {
    1 + 1
}
f1() # of course 2

f2 = function() {
   1
   + 1
}
f2() # returns 1

f3 = function() {
   return(1
   + 1)
}
f3() # 2; use return() if you want break lines, or

f4 = function() {
   1 +
   1
}
f4() # 2; do not put '+' in the beginning, as '+1==1'

