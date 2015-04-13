module global1
implicit none
integer a,b
common  a,b
end module

subroutine f1
use global
implicit none
a = 1
b = 2
write(*,*) a,b
end

subroutine f2
use global
implicit none
a = 3
b = 4
write(*,*) a,b
end

program  main
use global
implicit none
write(*,*) "call f1"
call f1
write(*,*) "call f2"
call f2
end program
