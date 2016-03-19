! { dg-additional-options "-fdump-tree-gimple" }

subroutine foo ()
  implicit none
  integer :: p,k
  integer  :: a
  !$acc parallel loop reduction(+:a)
  do p = 1,5
  enddo
  !$acc end parallel loop
  !$acc kernels loop reduction(+:a)
  do k = 2,6
  enddo
  !$acc end kernels loop
end subroutine

! { dg-final { scan-tree-dump-times "target oacc_parallel firstprivate.a." 1 "gimple" } }
! { dg-final { scan-tree-dump-times "acc loop reduction..:a. private.p." 1 "gimple" } }
