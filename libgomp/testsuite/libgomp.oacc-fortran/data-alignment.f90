! Test if the array data associated with c is properly aligned
! on the accelerator.  If it is not, this program will crash.

! { dg-do run }

integer function routine_align()
  implicit none
  integer, parameter :: n = 10000
  real*8, dimension(:), allocatable :: c
  integer :: i, idx

  allocate (c(n))
  routine_align = 0
  c = 0.0

  !$acc data copyin(idx) copy(c(1:n))

  !$acc parallel vector_length(32)
  !$acc loop vector
  do i=1, n
     c(i) = i
  enddo
  !$acc end parallel

  !$acc end data
end function routine_align


! main driver
program routine_align_main
  implicit none
  integer :: success
  integer routine_align
  success = routine_align()
end program routine_align_main
