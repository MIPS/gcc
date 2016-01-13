module array_mod
  real :: array(5) = (/ 1.0, 2.0, 3.0, 4.0, 5.0 /)
end module array_mod

module test_mod
  use array_mod

contains

  subroutine test_acc

    implicit none

    integer :: i
    real :: sum = 0.0

    !$acc parallel loop gang copy (sum) reduction (+:sum)
    do i = 1, 5
       sum = sum + array(i)
    end do
    !$acc end parallel loop
  end subroutine test_acc
end module test_mod
