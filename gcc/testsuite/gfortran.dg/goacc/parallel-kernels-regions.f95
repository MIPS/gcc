! { dg-do compile } 

! OpenACC 2.0 allows nested parallel/kernels regions
! However, in middle-end there is check for nested parallel

program test
  implicit none

  integer :: i

  !$acc parallel
    !$acc kernels 
    !$acc end kernels
  !$acc end parallel

  !$acc parallel
    !$acc parallel ! { dg-error "may not be nested" }
    !$acc end parallel
  !$acc end parallel

  !$acc parallel
    !$acc parallel ! { dg-error "may not be nested" }
    !$acc end parallel
    !$acc kernels 
    !$acc end kernels
  !$acc end parallel

  !$acc kernels
    !$acc kernels 
    !$acc end kernels
  !$acc end kernels

  !$acc kernels
    !$acc parallel 
    !$acc end parallel
  !$acc end kernels

  !$acc kernels
    !$acc parallel 
    !$acc end parallel
    !$acc kernels 
    !$acc end kernels
  !$acc end kernels

  !$acc parallel
    !$acc data ! { dg-error "may not be nested" }
    !$acc end data
  !$acc end parallel

  !$acc kernels
    !$acc data
    !$acc end data
  !$acc end kernels
  
end program test
! { dg-excess-errors "unimplemented" }