! Ensure that the middle end does not assign gang level parallelism to
! orphan loop containing reductions.

! { dg-additional-options "-fopt-info-note-omp" }

subroutine s1 ! { dg-warning "region is gang partitioned but does not contain gang partitioned code" }
  implicit none
  !$acc routine gang
  integer i, sum

  !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop worker vector>" }
  do i = 1, 10
     sum = sum + 1
  end do
end subroutine s1

subroutine s2 ! { dg-warning "region is gang partitioned but does not contain gang partitioned code" }
  implicit none
  !$acc routine gang
  integer i, j, sum

  !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop worker>" }
  do i = 1, 10
     !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop vector>" }
     do j = 1, 10
        sum = sum + 1
     end do
  end do
end subroutine s2

subroutine s3 ! { dg-warning "region is gang partitioned but does not contain gang partitioned code" }
  implicit none
  !$acc routine gang
  integer i, j, k, sum

  !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop worker>" }
  do i = 1, 10
     !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop seq>" }
     ! { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 }
     do j = 1, 10
        !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop vector>" }
        do k = 1, 10
           sum = sum + 1
        end do
     end do
  end do
end subroutine s3

subroutine s4
  implicit none

  integer i, j, k, sum

  !$acc parallel copy(sum)
  !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop gang vector>" }
  do i = 1, 10
     sum = sum + 1
  end do
  !$acc end parallel

  !$acc parallel copy(sum)
  !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop gang worker>" }
  do i = 1, 10
     !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop vector>" }
     do j = 1, 10
        sum = sum + 1
     end do
  end do
  !$acc end parallel

  !$acc parallel copy(sum)
  !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop gang>" }
  do i = 1, 10
     !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop worker>" }
     do j = 1, 10
        !$acc loop reduction (+:sum) ! { dg-message "Detected parallelism <acc loop vector>" }
        do k = 1, 10
           sum = sum + 1
        end do
     end do
  end do
  !$acc end parallel
end subroutine s4
