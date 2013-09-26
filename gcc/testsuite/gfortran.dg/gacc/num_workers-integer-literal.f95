! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

program test
	implicit none
	integer i
	integer :: x1 = 3, a1(10)

!$acc parallel num_workers (10) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel

end program test
! { dg-final { scan-tree-dump "parallel" "original" } } 
! { dg-final { scan-tree-dump "num_workers" "original" } } 
! { dg-final { cleanup-tree-dump "original" } } 