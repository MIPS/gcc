! { dg-do run }
! { dg-options "-ftree-parallelize-loops=32" }

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
