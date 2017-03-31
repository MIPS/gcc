! Test if the bind clause accepts named arguments in quotes, and
! implicit none symbols.

module routines
implicit none
contains

  subroutine sr1 (a)
    implicit none
    !$acc routine seq
    integer a

    a = 1
  end subroutine sr1
  
  subroutine sr (a)
    implicit none
    !$acc routine seq bind(sr1)
    integer a
    
    a = 0
  end subroutine sr

  integer function f1 (a)
    implicit none
    !$acc routine seq bind("f")
    integer a
    f1 = -1
  end function f1
end module routines

program main
  use routines
  implicit none

  integer z

  !$acc parallel copyout (z)
  call sr (z)
  !$acc end parallel

  print *, z
end program main
