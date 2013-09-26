! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

program test
	implicit none
	integer i
	integer :: x1 = 3, a1(10)

!$acc kernels   
!$acc loop private (10) ! { dg-error "Syntax error in OpenACC variable list" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels

!$acc kernels loop private (10) ! { dg-error "Syntax error in OpenACC variable list" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do

!$acc parallel private (10) ! { dg-error "Syntax error in OpenACC variable list" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do

!$acc parallel loop private (10) ! { dg-error "Syntax error in OpenACC variable list" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do

!$acc parallel   
!$acc loop private (10) ! { dg-error "Syntax error in OpenACC variable list" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel

end program test