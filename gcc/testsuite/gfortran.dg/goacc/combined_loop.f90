! { dg-do compile } 
!
! PR fortran/64726
!
subroutine oacc1()
  implicit none
  integer :: i
  integer  :: a
  !$acc parallel loop reduction(+:a)
  do i = 1,5
  enddo
end subroutine oacc1
