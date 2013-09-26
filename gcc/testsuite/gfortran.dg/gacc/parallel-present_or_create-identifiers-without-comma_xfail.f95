! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

! { dg-xfail-if "" { *-*-* } { "*" } { "" } } 
! { dg-error "" "" { xfail *-*-* } } 
program test
	implicit none
	integer i
	integer :: x1 = 3, x2 = 4, a1(10)

!$acc parallel present_or_create (x1 x2) 
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel

end program test
