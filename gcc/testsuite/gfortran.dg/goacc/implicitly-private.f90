! Ensure that implicitly private variables do not clash with those
! that are explicitly private.

program main
  implicit none

  integer i

  !$acc parallel loop private(i)
  do i = 1, 100
  end do
end program main
