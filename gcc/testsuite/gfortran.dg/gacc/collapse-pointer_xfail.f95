! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

! { dg-xfail-if "" { *-*-* } { "*" } { "" } } 

program test
	implicit none
	integer i
	integer :: x1 = 3, a1(10)
	integer, pointer :: y
	integer, target :: x2 = 4
	y => x2

!$acc kernels   
!$acc loop collapse (y) ! { gd-error "Constant expression required" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels

!$acc kernels loop collapse (y) ! { gd-error "Constant expression required" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels loop

!$acc parallel   
!$acc loop collapse (y) ! { gd-error "Constant expression required" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel

!$acc parallel loop collapse (y) ! { gd-error "Constant expression required" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel loop

end program test
