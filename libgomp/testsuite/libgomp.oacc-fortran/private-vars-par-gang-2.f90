! { dg-additional-options "-w" }

! Test of gang-private variables declared on the parallel directive.

program main
  use openacc
  integer :: x = 5
  integer, parameter :: n = 32
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
      if (acc_on_device (acc_device_host) .eqv. .TRUE.) x = i * 2
      arr(i) = arr(i) + x
    end do
  !$acc end parallel

  do i = 1, n
    if (arr(i) .ne. (3 + i * 2)) call abort
  end do

end program main
