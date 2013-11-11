! { dg-do compile } 
! { dg-options "-fopenacc" } 
program test
	implicit none
	integer :: a(10), b(10, 10), c(3:7), i

	!$acc parallel copy(a(1:5))
	!$acc end parallel
	!$acc parallel copy(a(1 + 0 : 5 + 2))
	!$acc end parallel
	!$acc parallel copy(a(:3))
	!$acc end parallel
	!$acc parallel copy(a(3:))
	!$acc end parallel
	!$acc parallel copy(a(:)) ! { dg-error "Syntax error in variable list" }
	!$acc parallel copy(a(2:3,2:3)) ! { dg-error "Number of dimensions" }
	!$acc end parallel
	! TODO: there must be warning
	!$acc parallel copy (a(:11))
	!$acc end parallel
	!$acc parallel copy (a(i:))
	!$acc end parallel

	!$acc parallel copy (a(:b)) ! { dg-error "scalar INTEGER expression" }
	!$acc end parallel

	!$acc parallel copy (b(1:3,2:4))
	!$acc end parallel 
	!$acc parallel copy (b(2:3)) ! { dg-error "Number of dimensions" }
	!$acc end parallel
	!$acc parallel copy (b(1:, 4:6)) ! { dg-warning "whole dimension" }
	!$acc end parallel

	! TODO: there must be warning
	!$acc parallel copy (c(2:))
	!$acc end parallel
end program test