! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

program test
	implicit none
	integer i
	integer :: x1 = 3, x2 = 4, a1(10)

!$acc data present (x1, x2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end data

!$acc parallel present (x1, x2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel

!$acc kernels present (x1, x2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels

end program test
! { dg-final { scan-tree-dump "data" "original" } } 
! { dg-final { scan-tree-dump "parallel" "original" } } 
! { dg-final { scan-tree-dump "kernels" "original" } } 
! { dg-final { scan-tree-dump "present" "original" } } 
! { dg-final { cleanup-tree-dump "original" } } 