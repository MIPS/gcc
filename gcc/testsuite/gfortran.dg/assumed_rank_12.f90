! { dg-do run }
! { dg-options "-fdump-tree-original" }
!
! PR fortran/48820
!
! Ensure that the value of scalars to assumed-rank arrays is
! copied back - and everything happens in the correct order.

call sub(f())
contains
subroutine sub(x)
  integer, pointer :: x(..)
end subroutine sub
function f() result(res)
  integer, pointer :: res
end function f
end

! { dg-final { scan-tree-dump " = f \\(\\);.*desc.0.type = 1025;.*desc.0.base_addr = .void .. D.\[0-9\]+;.*desc.0.elem_len = (20|32);.*desc.0.version = 1;.*desc.0.rank = 0;.*desc.0.attribute = 1;.*sub \\(&desc.0\\);.*D.\[0-9\]+ = .integer.kind=4. .. desc.0.base_addr;" "original" } }
! { dg-final { cleanup-tree-dump "original" } }
