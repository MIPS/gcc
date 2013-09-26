! { dg-do compile } 
! { dg-options "-fopenacc" } 

program test
	! only one directive-name may appear in directive
	!$acc parallel kernels ! { dg-error "Unclassifiable OpenACC directive" }
end