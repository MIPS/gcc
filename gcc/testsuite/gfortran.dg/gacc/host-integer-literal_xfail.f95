! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

program test
	implicit none
	integer i
	integer :: x1 = 3, a1(10)

!$acc update host (10) ! { dg-error "Syntax error in OpenACC variable list" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do

end program test