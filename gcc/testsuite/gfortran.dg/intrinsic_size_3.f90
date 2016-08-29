! { dg-do compile }
! { dg-options "-fdump-tree-original" }
!
! PR fortran/55852
!
! Contributed by A. Kasahara
!
program bug
  implicit none

  Real, allocatable:: a(:)
  integer(2) :: iszs

  allocate(a(1:3))

  iszs = ubound((a), 1)! Was ICEing
!  print*, ubound((a), 1) ! Was ICEing
! print*, ubound(a, 1)   ! OK
! print*, lbound((a), 1) ! OK
! print*, lbound(a, 1)   ! OK

  stop
end program bug

! { dg-final { scan-tree-dump-times "iszs = \\(integer\\(kind=2\\)\\) a.dim.0..extent;" 1 "original" } }
