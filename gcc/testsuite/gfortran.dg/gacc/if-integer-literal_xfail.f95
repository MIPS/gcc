! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

program test
	implicit none
	integer i
	integer :: x1 = 3, a1(10)

!$acc update if (1) ! { dg-error "requires a scalar LOGICAL expression" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do

!$acc data if (1) ! { dg-error "requires a scalar LOGICAL expression" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end data

!$acc parallel if (1) ! { dg-error "requires a scalar LOGICAL expression" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end parallel

!$acc kernels if (1) ! { dg-error "requires a scalar LOGICAL expression" }
	do i = 1,10
		x1 = x1 + 1
		a1(i) = x1
	end do
!$acc end kernels

end program test