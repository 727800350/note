program main
implicit none
character(len = 20):: filename = "fortran.md"
logical alive, is_opened

inquire(file = filename, exist = alive, opened = is_opened)
if(alive) then
        print *, filename, "exist."
else
        print *, filename, "does not exist."
end if

if(is_opened) then
        print *, filename, "has been opened."
else
        print *, filename, "has not been opened."
end if
stop
end

