! { dg-do compile }
! { dg-options "-fdump-tree-original -fcoarray=single" }
!
! PR fortran/57906
! PR fortran/52052
!
type t
  integer, allocatable :: x(:)[:]
  class(*), allocatable :: z(:)[:]
  class(*), allocatable :: d[:]
end type t
type t2
  type(t) :: y
end type t2
type(t2) :: a, b
a = b
end

! { dg-final { scan-tree-dump "a.y.x.base_addr = D.\[0-9\]+.y.x.base_addr;" "original" } }
! { dg-final { scan-tree-dump "a.y.z._data.base_addr = D.\[0-9\]+.y.z._data.base_addr;" "original" } }
! { dg-final { scan-tree-dump "a.y.d._data.base_addr = D.\[0-9\]+.y.d._data.base_addr;" "original" } }
! { dg-final { cleanup-tree-dump "original" } }
