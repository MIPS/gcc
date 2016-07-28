program main
  implicit none
  !$acc routine(abort) seq

  print *, "CheCKpOInT"
  !$acc parallel
  call abort
  !$acc end parallel

end program main

! { dg-output "CheCKpOInT" }
! { dg-shouldfail "" }
