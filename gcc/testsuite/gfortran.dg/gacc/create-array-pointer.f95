! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

program test
	implicit none
	integer i
	integer :: x1 = 3, a1(10)

!$acc data create (a1) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end data

!$acc parallel create (a1) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel

!$acc kernels create (a1) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels

end program test
! { dg-final { scan-tree-dump "data" "original" } } 
! { dg-final { scan-tree-dump "parallel" "original" } } 
! { dg-final { scan-tree-dump "kernels" "original" } } 
! { dg-final { scan-tree-dump "create" "original" } } 
! { dg-final { cleanup-tree-dump "original" } } 
