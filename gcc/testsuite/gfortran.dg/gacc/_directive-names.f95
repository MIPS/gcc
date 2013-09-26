! { dg-do compile } 
! { dg-options "-fopenacc" } 

program test
	implicit none

	!$acc parallel
	!$acc end parallel

	!$acc kernels
	!$acc end kernels

	!$acc data
	!$acc end data

	!$acc host_data
	!$acc end host_data
	
end program test