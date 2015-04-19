program main
implicit none 

logical ::flag = .true.
INTEGER array(3)

!! logical
print *, flag
flag = .false.
print *, flag

!! array
array = (/1,3,5/)
print *, array
end

