! { dg-do compile } 
! { dg-options "-fopenacc -fmax-errors=100" } 

! test clauses added in OpenACC ver 2.0

program test
    use openacc_lib
	implicit none
	integer :: i

	! async-values ::= integer-expression | "acc_async_noval" | "acc_async_sync" | empty
    !$acc kernels async(i)
    !$acc end kernels
    !$acc parallel async(i)
    !$acc end parallel

    !$acc kernels async(0, 1) { dg-error "Unclassifiable" }
    !$acc parallel async(0, 1) { dg-error "Unclassifiable" }

    !$acc kernels async
    !$acc end kernels
    !$acc parallel async
    !$acc end parallel

    !$acc kernels async(acc_async_noval)
    !$acc end kernels
    !$acc parallel async(acc_async_noval)
    !$acc end parallel

    !$acc kernels async(acc_async_sync)
    !$acc end kernels
    !$acc parallel async(acc_async_sync)
    !$acc end parallel

    !$acc kernels async() { dg-error "Invalid character" }
    !$acc parallel async() { dg-error "Invalid character" }

    !$acc kernels async("a") { dg-error "Unclassifiable" }
    !$acc parallel async("a") { dg-error "Unclassifiable" }

    !$acc kernels async(.true.) { dg-error "Unclassifiable" }
    !$acc parallel async(.true.) { dg-error "Unclassifiable" }

    ! default(none)
    !$acc kernels default(none)
    !$acc end kernels
    !$acc parallel default(none)
    !$acc end parallel

    !$acc kernels default (none)
    !$acc end kernels
    !$acc parallel default (none)
    !$acc end parallel

    !$acc kernels default ( none )
    !$acc end kernels
    !$acc parallel default ( none )
    !$acc end parallel

    !$acc kernels default { dg-error "Unclassifiable" }
    !$acc parallel default { dg-error "Unclassifiable" }

    !$acc kernels default() { dg-error "Unclassifiable" }
    !$acc parallel default() { dg-error "Unclassifiable" }

    !$acc kernels default(i) { dg-error "Unclassifiable" }
    !$acc parallel default(i) { dg-error "Unclassifiable" }

    !$acc kernels default(1) { dg-error "Unclassifiable" }
    !$acc parallel default(1) { dg-error "Unclassifiable" }
end
