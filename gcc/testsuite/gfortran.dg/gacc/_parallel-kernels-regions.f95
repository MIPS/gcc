! { dg-do compile } 
! { dg-options "-fopenacc" } 

program test
	implicit none

	integer :: i

	!$acc parallel
		!$acc kernels ! { dg-error "parallel/kernels regions" }
		!$acc end kernels
	!$acc end parallel

	!$acc parallel
		!$acc parallel ! { dg-error "parallel/kernels regions" }
		!$acc end parallel
	!$acc end parallel

	!$acc parallel
		!$acc parallel ! { dg-error "parallel/kernels regions" }
		!$acc end parallel
		!$acc kernels ! { dg-error "parallel/kernels regions" }
		!$acc end kernels
	!$acc end parallel

	!$acc kernels
		!$acc kernels ! { dg-error "parallel/kernels regions" }
		!$acc end kernels
	!$acc end kernels

	!$acc kernels
		!$acc parallel ! { dg-error "parallel/kernels regions" }
		!$acc end parallel
	!$acc end kernels

	!$acc kernels
		!$acc parallel ! { dg-error "parallel/kernels regions" }
		!$acc end parallel
		!$acc kernels ! { dg-error "parallel/kernels regions" }
		!$acc end kernels
	!$acc end kernels

	!$acc parallel
		!$acc data
		!$acc end data
	!$acc end parallel

	!$acc kernels
		!$acc data
		!$acc end data
	!$acc end kernels
	
end program test