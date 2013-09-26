! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

! { dg-xfail-if "" { *-*-* } { "*" } { "" } } 
! { dg-error "" "" { xfail *-*-* } } 
program test
	implicit none
	integer i
	integer :: x1 = 3, a1(10)

!$acc kernels loop reduction (+:1,2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels loop

end program test
