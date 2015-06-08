! { dg-do run  { target openacc_nvidia_accel_selected } }

module vars
  integer z
  !$acc declare create (z)
end module vars

subroutine subr6 (a, d)
  integer, parameter :: N = 8
  integer :: i
  integer :: a(N)
  !$acc declare deviceptr (a)
  integer :: d(N)

  i = 0

  !$acc parallel copy (d)
    do i = 1, N
      d(i) = a(i) + a(i)
    end do
  !$acc end parallel

end subroutine

subroutine subr5 (a, b, c, d)
  integer, parameter :: N = 8
  integer :: i
  integer :: a(N)
  !$acc declare present_or_copyin (a)
  integer :: b(N)
  !$acc declare present_or_create (b)
  integer :: c(N)
  !$acc declare present_or_copyout (c)
  integer :: d(N)
  !$acc declare present_or_copy (d)

  i = 0

  !$acc parallel
    do i = 1, N
      b(i) = a(i)
      c(i) = b(i)
      d(i) = d(i) + b(i)
    end do
  !$acc end parallel

end subroutine

subroutine subr4 (a, b)
  integer, parameter :: N = 8
  integer :: i
  integer :: a(N)
  !$acc declare present (a)
  integer :: b(N)
  !$acc declare copyout (b)

  i = 0

  !$acc parallel
  do i = 1, N
    b(i) = a(i)
  end do
  !$acc end parallel

end subroutine

subroutine subr3 (a, c)
  integer, parameter :: N = 8
  integer :: i
  integer :: a(N)
  !$acc declare present (a)
  integer :: c(N)
  !$acc declare copyin (c)

  i = 0

  !$acc parallel
  do i = 1, N
    a(i) = c(i)
    c(i) = 0
  end do
  !$acc end parallel

end subroutine

subroutine subr2 (a, b, c)
  integer, parameter :: N = 8
  integer :: i
  integer :: a(N)
  !$acc declare present (a)
  integer :: b(N)
  !$acc declare create (b)
  integer :: c(N)
  !$acc declare copy (c)

  i = 0

  !$acc parallel
  do i = 1, N
    b(i) = a(i)
    c(i) = b(i) + c(i) + 1
  end do
  !$acc end parallel

end subroutine

subroutine subr1 (a, b, c)
  integer, parameter :: N = 8
  integer :: i
  integer :: a(N)
  !$acc declare present (a)
  integer :: b(N)
  integer :: c(N)

  i = 0

  !$acc parallel
  do i = 1, N
    a(i) = a(i) + 1
  end do
  !$acc end parallel

end subroutine

subroutine test (a, e)
  use openacc
  logical :: e
  integer, parameter :: N = 8
  integer :: a(N)

  if (acc_is_present (a) .neqv. e) call abort

end subroutine

subroutine subr0 (a, b, c, d)
  integer, parameter :: N = 8
  integer :: a(N)
  !$acc declare copy (a)
  integer :: b(N)
  integer :: c(N)
  integer :: d(N)

  call test (a, .true.)
  call test (b, .false.)
  call test (c, .false.)

  call subr1 (a, b, c)

  call test (a, .true.)
  call test (b, .false.)
  call test (c, .false.)

  call subr2 (a, b, c)

  call test (a, .true.)
  call test (b, .false.)
  call test (c, .false.)

  do i = 1, N
    if (c(i) .ne. 8) call abort
  end do

  call subr3 (a, c)

  call test (a, .true.)
  call test (b, .false.)
  call test (c, .false.)

  do i = 1, N
    if (a(i) .ne. 2) call abort
    if (c(i) .ne. 8) call abort
  end do

  call subr4 (a, b)

  call test (a, .true.)
  call test (b, .false.)
  call test (c, .false.)

  do i = 1, N
    if (b(i) .ne. 8) call abort
  end do

  call subr5 (a, b, c, d)

  call test (a, .true.)
  call test (b, .false.)
  call test (c, .false.)
  call test (d, .false.)

  do i = 1, N
    if (c(i) .ne. 8) call abort
    if (d(i) .ne. 13) call abort
  end do

  call subr6 (a, d)

  call test (a, .true.)
  call test (d, .false.)

  do i = 1, N
    if (d(i) .ne. 16) call abort
  end do

end subroutine

program main
  use vars
  use openacc
  integer, parameter :: N = 8
  integer :: a(N)
  integer :: b(N)
  integer :: c(N)
  integer :: d(N)

  a(:) = 2
  b(:) = 3
  c(:) = 4
  d(:) = 5

  if (acc_is_present (z) .neqv. .true.) call abort

  call subr0 (a, b, c, d)

  call test (a, .false.)
  call test (b, .false.)
  call test (c, .false.)
  call test (d, .false.)

  do i = 1, N
    if (a(i) .ne. 8) call abort
    if (b(i) .ne. 8) call abort
    if (c(i) .ne. 8) call abort
    if (d(i) .ne. 16) call abort
  end do


end program
