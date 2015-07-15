! { dg-do compile }
! { dg-additional-options "-O2" }
! { dg-additional-options "-ftree-parallelize-loops=32" }
! { dg-additional-options "-fdump-tree-parloops_oacc_kernels-all" }
! { dg-additional-options "-fdump-tree-optimized" }

! Based on autopar/outer-1.c.

program main
  implicit none
  integer, parameter         :: n = 500
  integer, dimension (0:n-1, 0:n-1) :: x
  integer                    :: i, j, ii, jj


  !$acc kernels copyout (x)
  !$acc loop independent
  do ii = 0, n - 1
     do jj = 0, n - 1
        x(jj, ii) = ii + jj + 3
     end do
  end do
  !$acc end kernels

  do i = 0, n - 1
     do j = 0, n - 1
        if (x(i, j) .ne. i + j + 3) call abort
     end do
  end do

end program main

! Check that only one loop is analyzed, and that it can be parallelized.
! { dg-final { scan-tree-dump-times "SUCCESS: may be parallelized, marked independent" 1 "parloops_oacc_kernels" } }
! { dg-final { scan-tree-dump-not "FAILED:" "parloops_oacc_kernels" } }
! { dg-final { scan-tree-dump-times "parallelizing outer loop" 1 "parloops_oacc_kernels" } }

! Check that the loop has been split off into a function.
! { dg-final { scan-tree-dump-times "(?n);; Function MAIN__._omp_fn.0 " 1 "optimized" } }

! { dg-final { scan-tree-dump-times "(?n)pragma omp target oacc_parallel.*num_gangs\\(32\\)" 1 "parloops_oacc_kernels" } }
