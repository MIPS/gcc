! Test the output of -fopt-info-note-omp.

! { dg-additional-options "-fopt-info-note-omp" }

program test
  implicit none

  integer x, y, z

  !$acc parallel loop seq ! { dg-message "note: Detected parallelism <acc loop seq>" }
  do x = 1, 10
  end do

  !$acc parallel loop gang ! { dg-message "note: Detected parallelism <acc loop gang>" }
  do x = 1, 10
  end do

  !$acc parallel loop worker ! { dg-message "note: Detected parallelism <acc loop worker>" }
  do x = 1, 10
  end do

  !$acc parallel loop vector ! { dg-message "note: Detected parallelism <acc loop vector>" }
  do x = 1, 10
  end do

  !$acc parallel loop gang vector ! { dg-message "note: Detected parallelism <acc loop gang vector>" }
  do x = 1, 10
  end do

  !$acc parallel loop gang worker ! { dg-message "note: Detected parallelism <acc loop gang worker>" }
  do x = 1, 10
  end do

  !$acc parallel loop worker vector ! { dg-message "note: Detected parallelism <acc loop worker vector>" }
  do x = 1, 10
  end do

  !$acc parallel loop gang worker vector ! { dg-message "note: Detected parallelism <acc loop gang worker vector>" }
  do x = 1, 10
  end do

  !$acc parallel loop ! { dg-message "note: Detected parallelism <acc loop gang vector>" }
  do x = 1, 10
  end do

  !$acc parallel loop ! { dg-message "note: Detected parallelism <acc loop gang worker>" }
  do x = 1, 10
     !$acc loop ! { dg-message "note: Detected parallelism <acc loop vector>" }
     do y = 1, 10
     end do
  end do

  !$acc parallel loop gang ! { dg-message "note: Detected parallelism <acc loop gang>" }
  do x = 1, 10
     !$acc loop worker ! { dg-message "note: Detected parallelism <acc loop worker>" }
     do y = 1, 10
        !$acc loop vector ! { dg-message "note: Detected parallelism <acc loop vector>" }
        do z = 1, 10
        end do
     end do
  end do
end program test
