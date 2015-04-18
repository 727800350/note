program main
! 声明一个可以当成目标的变量
integer, target ::a = 1 
! 声明一个可以指向整数的指针
integer, pointer ::p 
! 把指针p 指向a
p => a 
print *, p
a = 2
print *, p
p = 3
print *, a
stop
end
