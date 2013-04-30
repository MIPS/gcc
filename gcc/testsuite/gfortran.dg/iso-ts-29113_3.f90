! { dg-do run }
! { dg-additional-sources iso-ts-29113_3_c.c }
! { dg-options "" }
!
! dg-options is required to silence -pedantic warnings for
! the C code.
!
! Test whether accessing the array from C works using
! TS29113's ISO_Fortran_binding.h
!
! The C examples are based on TS29113's.
!
module m
  use iso_c_binding
  implicit none

  interface
    subroutine test_establish1() bind(C)
    end subroutine test_establish1
    subroutine test_establish2() bind(C)
    end subroutine test_establish2
    subroutine test_section1(x) bind(C)
      import
      real(c_float) :: x(:)
    end subroutine test_section1
    subroutine test_section2(x) bind(C)
      import
      real(c_float) :: x(:,:)
    end subroutine test_section2
  end interface

  real, target :: A(100)
  real, target :: B(100, 100)
contains
  subroutine check_section1(x) bind(C)
    real(c_float), target :: x(:)

    if (size (x) /= size (A(3::5))) call abort ()
    if (lbound (x,1) /= lbound (A(3::5),1)) call abort ()
    if (ubound (x,1) /= ubound (A(3::5),1)) call abort ()
    if (loc (x(1)) /= loc (A(2))) call abort ()  ! FIXME: Should be A(3::5), lower_bound 0<->1 issue
    if (any (x /= A(2::5))) call abort ()
  end subroutine
  subroutine check_section2(x) bind(C)
    real(c_float), target :: x(:)

    if (size (x) /= size (B(:,42))) call abort ()
    if (lbound (x,1) /= lbound (B(:,42),1)) call abort ()
    if (ubound (x,1) /= ubound (B(:,42),1)) call abort ()
    if (loc (x(1)) /= loc (B(1,41))) call abort () ! FIXME: Should be B(1,42), lower_bound 0<->1 issue
    if (any (x /= B(:,41))) call abort () ! FIXME: Should be B(:,42), lower_bound 0<->1 issue
  end subroutine
end module m

use m
implicit none
integer :: i,j

call test_establish1 ()
call test_establish2 ()

A = [(i+100, i=0,99)]
call test_section1 (A)

do j = 1, 100
  do i = 1, 100
    B(i,j) = -i - 1000*j
  end do
end do
call test_section2 (B)
end
