! { dg-do compile } 
! { dg-options "-fopenacc" } 
program test
	implicit none
	integer :: i

	!$acc declare copy(i)

end program test