! Validate calls to ACC ROUTINES.  Ensure that the loop containing the
! call has sufficient parallelism to for the routine.

      subroutine sub
      implicit none
      integer, parameter :: n = 100
      integer :: a(n), i, j
      external gangr, workerr, vectorr, seqr
!$acc routine (gangr) gang
!$acc routine (workerr) worker
!$acc routine (vectorr) vector
!$acc routine (seqr) seq

!
! Test subroutine calls inside nested loops.
!

!$acc parallel loop
      do i = 1, n
         !$acc loop
         do j = 1, n
            call workerr (a, n)
         end do
      end do
!$acc end parallel loop

!$acc parallel loop
      do i = 1, n
!$acc loop gang
         do j = 1, n
            call gangr (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
         end do
      end do
!$acc end parallel loop

!
! Test calls to seq routines
!

!$acc parallel loop
      do i = 1, n
         call seqr (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop gang
      do i = 1, n
         call seqr (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop worker
      do i = 1, n
         call seqr (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop vector
      do i = 1, n
         call seqr (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop seq
      do i = 1, n
         call seqr (a, n)
      end do
!$acc end parallel loop

!
! Test calls to gang routines
!

!$acc parallel loop
      do i = 1, n
         call gangr (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop gang
      do i = 1, n
         call gangr (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop worker
      do i = 1, n
         call gangr (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop vector
      do i = 1, n
         call gangr (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop seq
      do i = 1, n
         call gangr (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!
! Test calls to worker routines
!

!$acc parallel loop
      do i = 1, n
         call workerr (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop gang
      do i = 1, n
         call workerr (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop worker
      do i = 1, n
         call workerr (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop vector
      do i = 1, n
         call workerr (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop seq
      do i = 1, n
         call workerr (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!
! Test calls to vector routines
!

!$acc parallel loop
      do i = 1, n
         call vectorr (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop gang
      do i = 1, n
         call vectorr (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop worker
      do i = 1, n
         call vectorr (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop vector
      do i = 1, n
         call vectorr (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop seq
      do i = 1, n
         call vectorr (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop
      end subroutine sub

      subroutine func
      implicit none
      integer, parameter :: n = 100
      integer :: a(n), i, j
      integer gangf, workerf, vectorf, seqf
!$acc routine (gangf) gang
!$acc routine (workerf) worker
!$acc routine (vectorf) vector
!$acc routine (seqf) seq

!
! Test subroutine calls inside nested loops.
!

!$acc parallel loop
      do i = 1, n
!$acc loop
         do j = 1, n
            a(1) = workerf (a, n)
         end do
      end do
!$acc end parallel loop

!$acc parallel loop
      do i = 1, n
!$acc loop gang
         do j = 1, n
            a(1) = gangf (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
         end do
      end do
!$acc end parallel loop

!
! Test calls to seq routines
!

!$acc parallel loop
      do i = 1, n
         a(1) = seqf (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop gang
      do i = 1, n
         a(1) = seqf (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop worker
      do i = 1, n
         a(1) = seqf (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop vector
      do i = 1, n
         a(1) = seqf (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop seq
      do i = 1, n
         a(1) = seqf (a, n)
      end do
!$acc end parallel loop

!
! Test calls to gang routines
!

!$acc parallel loop
      do i = 1, n
         a(1) = gangf (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop gang
      do i = 1, n
         a(1) = gangf (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop worker
      do i = 1, n
         a(1) = gangf (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop vector
      do i = 1, n
         a(1) = gangf (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop seq
      do i = 1, n
         a(1) = gangf (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!
! Test calls to worker routines
!

!$acc parallel loop
      do i = 1, n
         a(1) = workerf (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop gang
      do i = 1, n
         a(1) = workerf (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop worker
      do i = 1, n
         a(1) = workerf (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop vector
      do i = 1, n
         a(1) = workerf (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop seq
      do i = 1, n
         a(1) = workerf (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!
! Test calls to vector routines
!

!$acc parallel loop
      do i = 1, n
         a(1) = vectorf (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop gang
      do i = 1, n
         a(1) = vectorf (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop worker
      do i = 1, n
         a(1) = vectorf (a, n)
      end do
!$acc end parallel loop

!$acc parallel loop vector
      do i = 1, n
         a(1) = vectorf (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop

!$acc parallel loop seq
      do i = 1, n
         a(1) = vectorf (a, n) ! { dg-error "Insufficient ..ACC LOOP parallelism" }
      end do
!$acc end parallel loop
      end subroutine func
