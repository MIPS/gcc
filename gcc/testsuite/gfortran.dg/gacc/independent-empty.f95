! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

program test
	implicit none
	integer i
	integer :: x1 = 3, a1(10)

!$acc kernels   
!$acc loop independent  
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels

!$acc kernels loop independent  
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels loop

end program test
! { dg-final { scan-tree-dump "kernels" "original" } } 
! { dg-final { scan-tree-dump "loop" "original" } } 
! { dg-final { scan-tree-dump "independent" "original" } } 
! { dg-final { cleanup-tree-dump "original" } } 