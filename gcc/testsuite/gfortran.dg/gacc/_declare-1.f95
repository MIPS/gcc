! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 
program test
	implicit none
	integer :: i

	!$acc declare copy(i)

end program test
! { dg-final { scan-tree-dump "pragma acc data copy" "original" } } 
! { dg-final { cleanup-tree-dump "original" } } 