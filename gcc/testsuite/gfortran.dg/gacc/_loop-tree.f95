! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original -fdump-tree-gimple" } 

! test for tree-dump-original and spaces-commas

program test
	implicit none
	integer :: i, j, k, m, sum

	!$acc kernels 
	!$acc loop seq collapse(2)
	DO i = 1,10
		DO j = 1,10
		ENDDO
	ENDDO

	!$acc loop independent gang (3)
	DO i = 1,10
		!$acc loop worker(3)
		DO j = 1,10
			!$acc loop vector(5)
			DO k = 1,10
			ENDDO
		ENDDO
	ENDDO
	!$acc end kernels

	sum = 0
	!$acc parallel
	!$acc loop private(m) reduction(+:sum)
	DO i = 1,10
		sum = sum + 1
	ENDDO
	!$acc end parallel

end program test
! { dg-final { scan-tree-dump "pragma acc loop" "original" } } 

! { dg-final { scan-tree-dump "seq" "original" } }
! { dg-final { scan-tree-dump "collapse" "original" } } 
! { dg-final { scan-tree-dump "independent" "original" } } 
! { dg-final { scan-tree-dump "gang" "original" } } 
! { dg-final { scan-tree-dump "worker" "original" } } 
! { dg-final { scan-tree-dump "vector" "original" } } 

! { dg-final { scan-tree-dump "private" "original" } } 
! { dg-final { scan-tree-dump "reduction" "original" } } 

! { dg-final { scan-tree-dump "pragma acc loop" "gimple" } } 

! { dg-final { scan-tree-dump "seq" "gimple" } }
! { dg-final { scan-tree-dump "collapse" "gimple" } } 
! { dg-final { scan-tree-dump "independent" "gimple" } } 
! { dg-final { scan-tree-dump "gang" "gimple" } } 
! { dg-final { scan-tree-dump "worker" "gimple" } } 
! { dg-final { scan-tree-dump "vector" "gimple" } } 

! { dg-final { scan-tree-dump "private" "gimple" } } 
! { dg-final { scan-tree-dump "reduction" "gimple" } } 
! { dg-final { cleanup-tree-dump "gimple" } } 
! { dg-final { cleanup-tree-dump "original" } } 