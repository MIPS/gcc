! { dg-do run }
!
! Ensure that SIZE/SHAPE/UBOUND/LBOUND work properly with
! assumed-rank arrays for scalars and assumed-size arrays
!
program main
  implicit none
  integer :: A(2,2)
  integer :: B
  call foo(A)
  call test2(B)
contains
  subroutine foo(x)
    integer :: x(2,*)
    call bar(x)
  end subroutine foo
  subroutine bar(y)
    integer :: y(..)
!   print *, rank(y)   ! 2
!   print *, lbound(y) ! 1 1
!   print *, ubound(y) ! 2 -1
!   print *, shape(y)  ! 2 -1
!   print *, size(y)   ! -2
    if (rank(y) /= 2) call abort ()
    if (any (lbound(y) /= [1, 1])) call abort
    if (any (ubound(y) /= [2,-1])) call abort
    if (any (shape(y)  /= [2,-1])) call abort
    if (size(y,1) /=  2) call abort
    if (size(y,2) /= -1) call abort
    if (size(y)   /= -2) call abort
  end subroutine bar
  subroutine test2(z)
    integer :: z(..)
    if (rank(z) /= 0) call abort() ! 1
    if (size(lbound(z)) /= 0) call abort() ! zero-sized array
    if (size(ubound(z)) /= 0) call abort() ! zero-sized array
    if (size(shape(z))  /= 0) call abort() ! zero-sized array
    if (size(z) /= 1) call abort() ! 1
  end subroutine test2
end program main
