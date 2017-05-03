! Ensure that the auto clause falls back to seq parallelism when the
! OpenACC loop is not explicitly independent.

! { dg-additional-options "-fopt-info-note-omp" }

program test
  implicit none
  integer, parameter :: n = 100
  integer i, j, k, l
  
  !$acc parallel loop auto ! { dg-message "Detected parallelism <acc loop seq>" }
  do i = 1, n
     !$acc loop auto independent ! { dg-message "Detected parallelism <acc loop gang>" }
     do j = 1, n
        !$acc loop worker vector ! { dg-message "Detected parallelism <acc loop worker vector>" }
        do k = 1, n
        end do
     end do
  end do
 
  !$acc parallel loop auto independent ! { dg-message "Detected parallelism <acc loop gang worker>" }
  do i = 1, n
     !$acc loop auto ! { dg-message "Detected parallelism <acc loop seq>" }
     do j = 1, n
        !$acc loop auto ! { dg-message "Detected parallelism <acc loop seq>" }
        do k = 1, n
           !$acc loop auto independent ! { dg-message "Detected parallelism <acc loop vector>" }
           do l = 1, n
           end do
        end do
     end do
  end do

  !$acc parallel loop gang ! { dg-message "Detected parallelism <acc loop gang>" }
  do i = 1, n
     !$acc loop worker ! { dg-message "Detected parallelism <acc loop worker>" }
     do j = 1, n
        !$acc loop vector ! { dg-message "Detected parallelism <acc loop vector>" }
        do k = 1, n
           !$acc loop auto independent ! { dg-message "Detected parallelism <acc loop seq>" }
           ! { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 }
           do l = 1, n
           end do
           !$acc loop auto ! { dg-message "Detected parallelism <acc loop seq>" }
           do l = 1, n
           end do
        end do
     end do
  end do
  

  !$acc parallel loop ! { dg-message "Detected parallelism <acc loop seq>" }
  ! { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 }
  do i = 1, n
     !$acc loop gang worker ! { dg-message "Detected parallelism <acc loop gang worker>" }
     do j = 1, n
        !$acc loop auto ! { dg-message "Detected parallelism <acc loop seq>" }
	do k = 1, n
          !$acc loop vector ! { dg-message "Detected parallelism <acc loop vector>" }
          do l = 1, n
          end do
       end do
       !$acc loop auto independent ! { dg-message "Detected parallelism <acc loop vector>" }
       do l = 1, n
       end do
    end do
    !$acc loop worker ! { dg-message "Detected parallelism <acc loop worker>" }
    do j = 1, n
       !$acc loop vector ! { dg-message "Detected parallelism <acc loop vector>" }
       do k = 1, n
       end do
    end do
  end do

  !$acc parallel loop ! { dg-message "Detected parallelism <acc loop gang>" }
  do i = 1, n
     !$acc loop ! { dg-message "Detected parallelism <acc loop worker>" }
     do j = 1, n
        !$acc loop ! { dg-message "Detected parallelism <acc loop seq>" }
        ! { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 }
        do k = 1, n
           !$acc loop ! { dg-message "Detected parallelism <acc loop vector>" }
           do l = 1, n
           end do
        end do
     end do
  end do
end program test
