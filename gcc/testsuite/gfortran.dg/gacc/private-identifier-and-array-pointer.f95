! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

program test
	implicit none
	integer i
	integer :: x1 = 3, x2 = 4, a1(10)

!$acc kernels   
!$acc loop private (a1, x2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels

!$acc kernels loop private (a1, x2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels loop

!$acc parallel private (a1, x2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel

!$acc parallel loop private (a1, x2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel loop

!$acc parallel   
!$acc loop private (a1, x2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel

end program test
! { dg-final { scan-tree-dump "parallel" "original" } } 
! { dg-final { scan-tree-dump "parallel" "original" } } 
! { dg-final { scan-tree-dump "kernels" "original" } } 
! { dg-final { scan-tree-dump "loop" "original" } } 
! { dg-final { scan-tree-dump "private" "original" } } 
! { dg-final { cleanup-tree-dump "original" } } 