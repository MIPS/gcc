! { dg-do compile } 
! { dg-options "-fopenacc" } 

program test
	implicit none

	integer :: i

	if (.true.) then
		!$acc parallel 
	end if ! { dg-error "Unexpected" }
	!$acc end parallel 
	end if

	if (.true.) then
		!$acc kernels 
	end if ! { dg-error "Unexpected" }
	!$acc end kernels 
	end if

	!$acc parallel
	if (.true.) then
		!$acc end parallel ! { dg-error "Unexpected" }
	end if 
	!$acc end parallel

	!$acc kernels
	if (.true.) then
		!$acc end kernels ! { dg-error "Unexpected" }
	end if 
	!$acc end kernels

	!$acc parallel
	if (.true.) then
	end if
	!$acc end parallel

	!$acc kernels
	if (.true.) then
	end if
	!$acc end kernels

	if (.true.) then
		!$acc parallel
		!$acc end parallel
	end if

	if (.true.) then
		!$acc kernels
		!$acc end kernels
	end if

	!acc parallel
	! TODO: implement in lowering pass
	goto 10 ! { dg-error "branch|exit" }
	!$acc end parallel
10	i = 0


end program test 