! { dg-do run }

program reduction
  integer, parameter :: n = 10
  integer sum
  include "openacc_lib.h"

  sum = 0

  !$acc parallel reduction(+:sum) num_gangs (n)
  sum = sum + 1
  !$acc end parallel

  if (acc_get_device_type () .eq. acc_device_nvidia) then
     if (sum .ne. n) call abort
  else
     if (sum .ne. 1) call abort
  end if

  ! Test reductions inside subroutines

  sum = 0
  call redsub (sum, n)

  if (acc_get_device_type () .eq. acc_device_nvidia) then
     if (sum .ne. n) call abort
  else
     if (sum .ne. 1) call abort
  end if
end program reduction

subroutine redsub(sum, n)
  integer :: sum, n

  !$acc parallel reduction(+:sum) num_gangs (10)
  sum = sum + 1
  !$acc end parallel
end subroutine redsub
