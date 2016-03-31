! { dg-additional-options "-Wuninitialized" }

program test
  implicit none
  integer :: i, j, k

  !$acc parallel loop gang num_gangs(i) ! { dg-warning "is used uninitialized in this function" }
  do i = 0, 1
  end do
  !$acc end parallel loop

  !$acc parallel loop worker num_workers(j) ! { dg-warning "is used uninitialized in this function" }
  do j = 0, 1
  end do
  !$acc end parallel loop

  !$acc parallel loop vector vector_length(k) ! { dg-warning "is used uninitialized in this function" }
  do k = 0, 1
  end do
  !$acc end parallel loop
end program test
