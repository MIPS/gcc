! { dg-do run }
! { dg-additional-sources iso-ts-29113_1_c.c }
!
! Test whether accessing the array from C works using
! TS29113's ISO_Fortran_binding.h
!
! The C examples are based on TS29113's.
!
use iso_c_binding
implicit none
real(C_float) :: A(100,100)
real(C_float), allocatable :: B(:,:)
integer :: i, j

type, bind(c) :: t
  real(c_double) :: x
  complex(c_double_complex) :: y
end type t

interface
  subroutine test_address(x) bind(C)
    import
    real(C_float) :: x(..)
  end subroutine test_address
  subroutine test_allocate(x) bind(C)
    import
    real(C_float) :: x(..)
  end subroutine test_allocate
  subroutine test_deallocate(x) bind(C)
    import
    real(C_float) :: x(..)
  end subroutine test_deallocate
end interface

do i = 1, 100
  do j = 1, 100
    A(j,i) = j + 1000*i
  end do
end do
call test_address (A)

call test_allocate (B)
if (.not. allocated (B)) call abort()
if (any (lbound (B) /= [-1,5])) call abort()
if (any (ubound (B) /= [100,500])) call abort()

call test_deallocate (B)
if (allocated (B)) call abort()
end
