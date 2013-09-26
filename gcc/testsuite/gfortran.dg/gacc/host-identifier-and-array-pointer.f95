! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

program test
	implicit none
	integer i
	integer :: x1 = 3, x2 = 4, a1(10)

!$acc update host (a1, x2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do

end program test
! { dg-final { scan-tree-dump "update" "original" } } 
! { dg-final { scan-tree-dump "host" "original" } } 
! { dg-final { cleanup-tree-dump "original" } } 