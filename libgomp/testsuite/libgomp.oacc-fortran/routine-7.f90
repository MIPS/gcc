! Test acc routines inside modules.

! { dg-do run }

module routines
  integer, parameter :: N = 32

contains
  subroutine vector (a)
    implicit none
    !$acc routine vector
    integer, intent (inout) :: a(N)
    integer :: i

    !$acc loop vector
    do i = 1, N
       a(i) = 1
    end do
  end subroutine vector

  subroutine worker (a)
    implicit none
    !$acc routine worker
    integer, intent (inout) :: a(N)
    integer :: i

    !$acc loop worker
    do i = 1, N
       a(i) = 2
    end do
  end subroutine worker

  subroutine gang (a)
    implicit none
    !$acc routine gang
    integer, intent (inout) :: a(N)
    integer :: i

    !$acc loop gang
    do i = 1, N
       a(i) = 3
    end do
  end subroutine gang

  subroutine seq (a)
    implicit none
    !$acc routine seq
    integer, intent (inout) :: a(N)
    integer :: i

    do i = 1, N
       a(i) = 4
    end do
  end subroutine seq
end module routines

program main
  use routines
  implicit none

  integer :: i
  integer :: a(N)

  !$acc parallel
  call seq (a)
  !$acc end parallel

  do i = 1, N
    if (a(i) .ne. 4) call abort
  end do

  !$acc parallel
  call gang (a)
  !$acc end parallel

  do i = 1, N
    if (a(i) .ne. 3) call abort
  end do

  !$acc parallel
  call worker (a)
  !$acc end parallel

  do i = 1, N
    if (a(i) .ne. 2) call abort
  end do

  !$acc parallel
  call vector (a)
  !$acc end parallel

  do i = 1, N
    if (a(i) .ne. 1) call abort
  end do
end program main
