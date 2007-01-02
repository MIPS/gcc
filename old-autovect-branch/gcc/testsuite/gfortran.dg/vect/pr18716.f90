! { dg-do compile }
! { dg-require-effective-target vect_float }
! PR tree-opt/18716
subroutine test(n, v)
  implicit none
  integer :: n, i
  real(4) :: v(n)

  do i = 1, n
    v(i) = 0
  end do

  return
end
! { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } }
! { dg-final { cleanup-tree-dump "vect" } } 
