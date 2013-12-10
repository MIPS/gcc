! { dg-do compile } 
! { dg-options "-fopenacc -fmax-errors=100" } 

subroutine foo (ia1)
integer :: i1, i2, i3
integer, dimension (*) :: ia1
integer, dimension (10) :: ia2
real :: r1
real, dimension (5) :: ra1
double precision :: d1
double precision, dimension (4) :: da1
complex :: c1
complex, dimension (7) :: ca1
logical :: l1
logical, dimension (3) :: la1
character (5) :: a1
type t
  integer :: i
end type
type(t) :: t1
type(t), dimension (2) :: ta1
real, pointer :: p1 => NULL()
integer, allocatable :: aa1 (:,:)
save i2
common /blk/ i1

!$acc parallel reduction (+:ia2)	! { dg-error "SCALAR variable" }
!$acc end parallel
!$acc parallel reduction (+:ra1)	! { dg-error "SCALAR variable" }
!$acc end parallel
!$acc parallel reduction (+:ca1)	! { dg-error "SCALAR variable" }
!$acc end parallel
!$acc parallel reduction (+:da1)	! { dg-error "SCALAR variable" }
!$acc end parallel
!$acc parallel reduction (.and.:la1)	! { dg-error "SCALAR variable" }
!$acc end parallel
!$acc parallel reduction (+:i3, r1, d1, c1)
!$acc end parallel
!$acc parallel reduction (*:i3, r1, d1, c1)
!$acc end parallel
!$acc parallel reduction (-:i3, r1, d1, c1)
!$acc end parallel
!$acc parallel reduction (.and.:l1)
!$acc end parallel
!$acc parallel reduction (.or.:l1)
!$acc end parallel
!$acc parallel reduction (.eqv.:l1)
!$acc end parallel
!$acc parallel reduction (.neqv.:l1)
!$acc end parallel
!$acc parallel reduction (min:i3, r1, d1)
!$acc end parallel
!$acc parallel reduction (max:i3, r1, d1)
!$acc end parallel
!$acc parallel reduction (iand:i3)
!$acc end parallel
!$acc parallel reduction (ior:i3)
!$acc end parallel
!$acc parallel reduction (ieor:i3)
!$acc end parallel
!$acc parallel reduction (+:/blk/)	! { dg-error "Syntax error" }
!$acc end parallel			! { dg-error "Unexpected" }
!$acc parallel reduction (*:p1)		! { dg-error "POINTER object" }
!$acc end parallel
!$acc parallel reduction (-:aa1)
!$acc end parallel
!$acc parallel reduction (*:ia1)	! { dg-error "Assumed size" }
!$acc end parallel
!$acc parallel reduction (+:l1)		! { dg-error "must be of numeric type, got LOGICAL" }
!$acc end parallel
!$acc parallel reduction (*:la1)	! { dg-error "must be of numeric type, got LOGICAL" }
!$acc end parallel
!$acc parallel reduction (-:a1)		! { dg-error "must be of numeric type, got CHARACTER" }
!$acc end parallel
!$acc parallel reduction (+:t1)		! { dg-error "must be of numeric type, got TYPE" }
!$acc end parallel
!$acc parallel reduction (*:ta1)	! { dg-error "must be of numeric type, got TYPE" }
!$acc end parallel
!$acc parallel reduction (.and.:i3)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (.or.:ia2)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (.eqv.:r1)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (.neqv.:ra1)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (.and.:d1)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (.or.:da1)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (.eqv.:c1)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (.neqv.:ca1)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (.and.:a1)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (.or.:t1)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (.eqv.:ta1)	! { dg-error "must be LOGICAL" }
!$acc end parallel
!$acc parallel reduction (min:c1)	! { dg-error "must be INTEGER or REAL" }
!$acc end parallel
!$acc parallel reduction (max:ca1)	! { dg-error "must be INTEGER or REAL" }
!$acc end parallel
!$acc parallel reduction (max:l1)	! { dg-error "must be INTEGER or REAL" }
!$acc end parallel
!$acc parallel reduction (min:la1)	! { dg-error "must be INTEGER or REAL" }
!$acc end parallel
!$acc parallel reduction (max:a1)	! { dg-error "must be INTEGER or REAL" }
!$acc end parallel
!$acc parallel reduction (min:t1)	! { dg-error "must be INTEGER or REAL" }
!$acc end parallel
!$acc parallel reduction (max:ta1)	! { dg-error "must be INTEGER or REAL" }
!$acc end parallel
!$acc parallel reduction (iand:r1)	! { dg-error "must be INTEGER" }
!$acc end parallel
!$acc parallel reduction (ior:ra1)	! { dg-error "must be INTEGER" }
!$acc end parallel
!$acc parallel reduction (ieor:d1)	! { dg-error "must be INTEGER" }
!$acc end parallel
!$acc parallel reduction (ior:da1)	! { dg-error "must be INTEGER" }
!$acc end parallel
!$acc parallel reduction (iand:c1)	! { dg-error "must be INTEGER" }
!$acc end parallel
!$acc parallel reduction (ior:ca1)	! { dg-error "must be INTEGER" }
!$acc end parallel
!$acc parallel reduction (ieor:l1)	! { dg-error "must be INTEGER" }
!$acc end parallel
!$acc parallel reduction (iand:la1)	! { dg-error "must be INTEGER" }
!$acc end parallel
!$acc parallel reduction (ior:a1)	! { dg-error "must be INTEGER" }
!$acc end parallel
!$acc parallel reduction (ieor:t1)	! { dg-error "must be INTEGER" }
!$acc end parallel
!$acc parallel reduction (iand:ta1)	! { dg-error "must be INTEGER" }
!$acc end parallel

end subroutine