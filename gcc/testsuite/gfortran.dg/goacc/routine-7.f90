! Test acc routines inside modules.

! { dg-additional-options "-O0" }

module routines
  integer a
contains
  subroutine vector ! { dg-warning "partitioned but does not contain" }
    implicit none
    !$acc routine vector
  end subroutine vector

  subroutine worker ! { dg-warning "partitioned but does not contain" }
    implicit none
    !$acc routine worker
  end subroutine worker

  subroutine gang ! { dg-warning "partitioned but does not contain" }
    implicit none
    !$acc routine gang
  end subroutine gang

  subroutine seq
    implicit none
    !$acc routine seq
  end subroutine seq
end module routines

program main
  use routines
  implicit none

  integer :: i

  !$acc parallel loop gang
  do i = 1, 10
     call gang ! { dg-error "routine call uses same OpenACC parallelism as containing loop" }
     call worker
     call vector
     call seq
  end do
  !$acc end parallel loop

  !$acc parallel loop worker
  do i = 1, 10
     call gang ! { dg-error "routine call uses same OpenACC parallelism as containing loop" }
     call worker ! { dg-error "routine call uses same OpenACC parallelism as containing loop" }
     call vector
     call seq
  end do
  !$acc end parallel loop

  !$acc parallel loop vector
  do i = 1, 10
     call gang ! { dg-error "routine call uses same OpenACC parallelism as containing loop" }
     call worker ! { dg-error "routine call uses same OpenACC parallelism as containing loop" }
     call vector ! { dg-error "routine call uses same OpenACC parallelism as containing loop" }
     call seq
  end do
  !$acc end parallel loop

  !$acc parallel loop seq
  do i = 1, 10
     call gang
     call worker
     call vector
     call seq
  end do
  !$acc end parallel loop
end program main
