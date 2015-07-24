! { dg-do run }

program main
  integer :: a, n
  
  n = 10

  !$acc parallel copy (a, n)
     a = foo (n)
  !$acc end parallel 

  if (a .ne. n * n) call abort

contains

function foo (n) result (rc)
  !$acc routine nohost

  integer, intent (in) :: n
  integer :: rc

  rc = n * n

end function

end program main

! { dg-output "not found" { target openacc_host_selected } }
