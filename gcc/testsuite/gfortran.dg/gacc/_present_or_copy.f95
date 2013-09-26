! { dg-do compile } 
! { dg-options "-fopenacc -fdump-tree-original" } 

PROGRAM test
	IMPLICIT NONE
	INTEGER :: a(10), i, b(10)

	!$ACC DATA copy(b)
	DO i = 1, 10
		b(i) = i
	ENDDO
	!$ACC PARALLEL
	!$ACC LOOP
	DO i = 1, 10
		a(i) = b(i) ! { dg-warning "present_or_copy" }
	ENDDO
	!$ACC END PARALLEL

	!$ACC END DATA
END
! { dg-final { scan-tree-dump "present_or_copy" "original" } } 
! { dg-final { cleanup-tree-dump "original" } } 
