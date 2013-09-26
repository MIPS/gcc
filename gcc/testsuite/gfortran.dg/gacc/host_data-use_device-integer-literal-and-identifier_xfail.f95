! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

! { dg-xfail-if "" { *-*-* } { "*" } { "" } } 
! { dg-error "" "" { xfail *-*-* } } 
program test
	implicit none
	integer i
	integer :: x1 = 3, a1(10)

!$acc host_data use_device (10, x1) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end host_data

end program test
