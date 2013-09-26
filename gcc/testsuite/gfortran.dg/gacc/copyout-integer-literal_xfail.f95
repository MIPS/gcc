! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

! { dg-xfail-if "" { *-*-* } { "*" } { "" } } 

program test
	implicit none
	integer i
	integer :: x1 = 3, a1(10)

!$acc data copyout (10) ! { gd-error "Syntax error in OpenACC variable list" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end data

!$acc parallel copyout (10) ! { gd-error "Syntax error in OpenACC variable list" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel

!$acc kernels copyout (10) ! { gd-error "Syntax error in OpenACC variable list" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels

end program test