! Test the output of -fopt-info-note-omp.

! { dg-additional-options "-fopt-info-note-omp" }

program test
  implicit none

  integer x, y, z

  !$acc parallel loop seq
  do x = 1, 10
  end do

  !$acc parallel loop gang
  do x = 1, 10
  end do

  !$acc parallel loop worker
  do x = 1, 10
  end do

  !$acc parallel loop vector
  do x = 1, 10
  end do

  !$acc parallel loop gang vector
  do x = 1, 10
  end do

  !$acc parallel loop gang worker
  do x = 1, 10
  end do

  !$acc parallel loop worker vector
  do x = 1, 10
  end do

  !$acc parallel loop gang worker vector
  do x = 1, 10
  end do

  !$acc parallel loop
  do x = 1, 10
  end do

  !$acc parallel loop
  do x = 1, 10
     !$acc loop
     do y = 1, 10
     end do
  end do

  !$acc parallel loop gang
  do x = 1, 10
     !$acc loop worker
     do y = 1, 10
        !$acc loop vector
        do z = 1, 10
        end do
     end do
  end do
end program test

! { dg-message "note-parallelism.f90:10:0: note: Detected parallelism <acc loop seq>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:14:0: note: Detected parallelism <acc loop gang>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:18:0: note: Detected parallelism <acc loop worker>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:22:0: note: Detected parallelism <acc loop vector>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:26:0: note: Detected parallelism <acc loop gang vector>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:30:0: note: Detected parallelism <acc loop gang worker>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:34:0: note: Detected parallelism <acc loop worker vector>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:38:0: note: Detected parallelism <acc loop gang worker vector>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:42:0: note: Detected parallelism <acc loop gang vector>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:46:0: note: Detected parallelism <acc loop gang worker>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:48:0: note: Detected parallelism <acc loop vector>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:53:0: note: Detected parallelism <acc loop gang>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:55:0: note: Detected parallelism <acc loop worker>" "" { target *-*-* } 0 }
! { dg-message "note-parallelism.f90:57:0: note: Detected parallelism <acc loop vector>" "" { target *-*-* } 0 }
