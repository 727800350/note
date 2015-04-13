program main
implicit none 
integer s,i

!! do continue demo
s = 0
do 10 i=1,100
    s = s + i
10 continue 
print *, "sum = ", s

!! do end do demo
s = 0
do i=1,100
    s = s + i
end do 
print *, "sum = ", s

!! do while demo
i = 1
s = 0
do while(i <= 100)
    s = s + i
    i = i + 1
end do
print *, "sum = ", s

end program
