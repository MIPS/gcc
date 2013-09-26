! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

program test
	implicit none
	integer :: i, j
	integer :: x1 = 3, a1(10)

!$acc kernels   
!$acc loop collapse (2) 
	do i = 1,10
		do j = 1,2
			x1 = x1 + 1
			a1(i) = x1
		enddo
	end do
!$acc end kernels

!$acc kernels loop collapse (2) 
	do i = 1,10
		do j = 1,2
			x1 = x1 + 1
			a1(i) = x1
		enddo
	end do
!$acc end kernels loop

!$acc parallel   
!$acc loop collapse (2) 
	do i = 1,10
		do j = 1,2
			x1 = x1 + 1
			a1(i) = x1
		enddo
	end do
!$acc end parallel

!$acc parallel loop collapse (2) 
	do i = 1,10
		do j = 1,2
			x1 = x1 + 1
			a1(i) = x1
		enddo
	end do
!$acc end parallel loop

end program test
! { dg-final { scan-tree-dump "parallel" "original" } } 
! { dg-final { scan-tree-dump "kernels" "original" } } 
! { dg-final { scan-tree-dump "loop" "original" } } 
! { dg-final { scan-tree-dump "collapse" "original" } } 
! { dg-final { cleanup-tree-dump "original" } } 
