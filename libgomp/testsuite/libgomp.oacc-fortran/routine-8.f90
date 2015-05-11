
! { dg-do run } 
! { dg-error "Invalid" "TODO" { xfail *-*-* } 51 }

program main
  integer, parameter :: n = 10
  integer :: a(n)
  integer :: i

  !$acc parallel copy (a) vector_length (5)
  !$acc loop
    do i = 1, n
      a(i) = foo (i);
    end do
  !$acc end parallel

  do i = 1, n
    if (a(i) .ne. subr1 (i)) call abort
  end do

  do i = 1, n
    a(i) = 0
  end do

  !$acc parallel copy (a) vector_length (5)
  !$acc loop
    do i = 1, n
      a(i) = bar (i);
    end do
  !$acc end parallel

  do i = 1, n
    if (a(i) .ne. subr2 (i)) call abort
  end do

contains

function subr1 (n) result (rc)
  !$acc routine bind (foo)
  integer :: n, rc

  if ((n .eq. 0) .or. (n .eq. 1)) then
    rc = 1
  else
    rc = n * foo (n - 1);
  end if

end function

function subr2 (n) result (rc)
  !$acc routine bind ("bar")
  integer :: n, rc

  if ((n .eq. 0) .or. (n .eq. 1)) then
    rc = 1
  else
    rc = n * bar (n - 1);
  end if

end function

end program main
