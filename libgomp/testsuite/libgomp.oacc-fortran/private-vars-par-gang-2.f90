
! { dg-additional-options "-cpp" }

! Test of gang-private variables declared on the parallel directive.

#if defined(ACC_DEVICE_TYPE_host) || defined(ACC_DEVICE_TYPE_host_nonshm)
#define ACTUAL_GANGS 1
#else
#define ACTUAL_GANGS 32
#endif

program main
  integer :: x = 5
  integer, parameter :: n = ACTUAL_GANGS
  integer :: arr(n)

  do i = 1, n
    arr(i) = 3
  end do

  !$acc parallel private(x) copy(arr) num_gangs(n) num_workers(8) vector_length(32)
    !$acc loop gang(static:1)
    do i = 1, n
      x = i * 2;
    end do

   !$acc loop gang(static:1)
    do i = 1, n
      arr(i) = arr(i) + x
    end do
  !$acc end parallel

  do i = 1, n
    if (arr(i) .ne. (3 + i * 2)) call abort
  end do

end program main
