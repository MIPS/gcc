! Test declare create with allocatable arrays.

! { dg-do run }

module vars
  implicit none
  integer, parameter :: n = 100
  real*8, allocatable :: b(:)
 !$acc declare create (b)
end module vars

program test
  use vars
  use openacc
  implicit none
  real*8 :: a
  integer :: i

  interface
     subroutine sub1
       !$acc routine gang
     end subroutine sub1

     subroutine sub2
     end subroutine sub2

     real*8 function fun1 (ix)
       integer ix
       !$acc routine seq
     end function fun1

     real*8 function fun2 (ix)
       integer ix
       !$acc routine seq
     end function fun2
  end interface

  if (allocated (b)) call abort

  ! Test local usage of an allocated declared array.

  allocate (b(n))

  if (.not.allocated (b)) call abort
  if (acc_is_present (b) .neqv. .true.) call abort

  a = 2.0

  !$acc parallel loop
  do i = 1, n
     b(i) = i * a
  end do

  if (.not.acc_is_present (b)) call abort

  !$acc update host(b)

  do i = 1, n
     if (b(i) /= i*a) call abort
  end do

  deallocate (b)

  ! Test the usage of an allocated declared array inside an acc
  ! routine subroutine.

  allocate (b(n))

  if (.not.allocated (b)) call abort
  if (acc_is_present (b) .neqv. .true.) call abort

  !$acc parallel
  call sub1
  !$acc end parallel

  if (.not.acc_is_present (b)) call abort

  !$acc update host(b)

  do i = 1, n
     if (b(i) /= i*2) call abort
  end do

  deallocate (b)

  ! Test the usage of an allocated declared array inside a host
  ! subroutine.

  call sub2

  if (.not.acc_is_present (b)) call abort

  !$acc update host(b)

  do i = 1, n
     if (b(i) /= 1.0) call abort
  end do

  deallocate (b)

  if (allocated (b)) call abort

  ! Test the usage of an allocated declared array inside an acc
  ! routine function.

  allocate (b(n))

  if (.not.allocated (b)) call abort
  if (acc_is_present (b) .neqv. .true.) call abort

  !$acc parallel loop
  do i = 1, n
     b(i) = 1.0
  end do

  !$acc parallel loop
  do i = 1, n
     b(i) = fun1 (i)
  end do

  if (.not.acc_is_present (b)) call abort

  !$acc update host(b)

  do i = 1, n
     if (b(i) /= i) call abort
  end do

  deallocate (b)

  ! Test the usage of an allocated declared array inside a host
  ! function.

  allocate (b(n))

  if (.not.allocated (b)) call abort
  if (acc_is_present (b) .neqv. .true.) call abort

  !$acc parallel loop
  do i = 1, n
     b(i) = 1.0
  end do

  !$acc update host(b)

  do i = 1, n
     b(i) = fun2 (i)
  end do

  if (.not.acc_is_present (b)) call abort

  do i = 1, n
     if (b(i) /= i*i) call abort
  end do

  deallocate (b)
end program test

! Set each element in array 'b' at index i to i*2.

subroutine sub1 ! { dg-warning "region is worker partitioned" }
  use vars
  implicit none
  integer i
  !$acc routine gang

  !$acc loop
  do i = 1, n
     b(i) = i*2
  end do
end subroutine sub1

! Allocate array 'b', and set it to all 1.0.

subroutine sub2
  use vars
  use openacc
  implicit none
  integer i

  allocate (b(n))

  if (.not.allocated (b)) call abort
  if (acc_is_present (b) .neqv. .true.) call abort

  !$acc parallel loop
  do i = 1, n
     b(i) = 1.0
  end do
end subroutine sub2

! Return b(i) * i;

real*8 function fun1 (i)
  use vars
  implicit none
  integer i
  !$acc routine seq

  fun1 = b(i) * i
end function fun1

! Return b(i) * i * i;

real*8 function fun2 (i)
  use vars
  implicit none
  integer i

  fun2 = b(i) * i * i
end function fun2
