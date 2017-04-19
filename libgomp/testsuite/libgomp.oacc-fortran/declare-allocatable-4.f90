! Test declare create with allocatable arrays and scalars.  The unused
! declared array 'b' caused an ICE in the past.

! { dg-do run }

module vars
  implicit none
  integer, parameter :: n = 100
  real*8, allocatable :: a, b(:)
 !$acc declare create (a, b)
end module vars

program test
  use vars
  implicit none
  integer :: i

  interface
     subroutine sub1
     end subroutine sub1

     subroutine sub2
     end subroutine sub2

     real*8 function fun1 (ix)
       integer ix
       !$acc routine seq
     end function fun1

     real*8 function fun2 (ix)
       integer ix
       !$acc routine seq
     end function fun2
  end interface

  if (allocated (a)) call abort
  if (allocated (b)) call abort

  ! Test the usage of an allocated declared array inside an acc
  ! routine subroutine.

  allocate (a)
  allocate (b(n))

  if (.not.allocated (b)) call abort

  call sub1

  !$acc update self(a)
  if (a /= 50) call abort

  deallocate (a)
  deallocate (b)

end program test

! Set 'a' to 50.

subroutine sub1
  use vars
  implicit none
  integer i

  a = 50
  !$acc update device(a)
end subroutine sub1
