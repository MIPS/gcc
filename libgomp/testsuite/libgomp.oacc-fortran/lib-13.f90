! { dg-do run }

program main
  use openacc
  implicit none

  integer :: i, j, nprocs
  integer, parameter :: N = 1000000

  nprocs = 2

  do j = 1, nprocs
    !$acc parallel async (j)
      do i = 1, N
      end do
    !$acc end parallel
  end do

  if (acc_async_test (1) .neqv. .TRUE.) call abort
  if (acc_async_test (2) .neqv. .TRUE.) call abort

  call acc_wait_all_async (nprocs + 1)

  if (acc_async_test (nprocs + 1) .neqv. .TRUE.) call abort

  call acc_wait_all ()

end program
