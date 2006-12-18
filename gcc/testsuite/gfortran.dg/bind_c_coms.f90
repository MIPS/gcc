! { dg-do run }
! { dg-additional-sources bind_c_coms_driver.c }
! { dg-options "-w" }
! the -w option is to prevent the warning about long long ints
module bind_c_coms
  use, intrinsic :: iso_c_binding
  implicit none

  common /COM/ R, S
  real(c_double) :: r
  real(c_double) :: t 
  real(c_double) :: s 
  bind(c) :: /COM/, /SINGLE/, /MYCOM/
  common /SINGLE/ T
  common /MYCOM/ LONG_INTS
  integer(c_long) :: LONG_INTS
  common /MYCOM2/ LONG_LONG_INTS
  integer(c_long_long) :: long_long_ints
  bind(c) :: /mycom2/

contains
  subroutine test_coms() bind(c)
    r = r + .1d0;
    s = s + .1d0;
    t = t + .1d0;
    long_ints = long_ints + 1
    long_long_ints = long_long_ints + 1
  end subroutine test_coms
end module bind_c_coms
