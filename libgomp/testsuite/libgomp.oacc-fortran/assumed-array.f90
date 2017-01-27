! Test data movement on array pointers.

! { dg-do run }

module assumed
contains
  subroutine test_explicit(b)
   implicit none
   integer, pointer :: b(:)

   !$acc parallel copy(b)
   b(:) = 1
   !$acc end parallel
 end subroutine test_explicit
 subroutine test_implicit(b)
   implicit none
   integer, pointer :: b(:)

   !$acc parallel
   b(:) = 2
   !$acc end parallel
 end subroutine test_implicit
end module assumed
  use assumed
  integer, pointer :: a(:)
  integer, target :: b(100)
  a => b

  call test_explicit (a)
  if (any (a .ne. 1, 1)) call abort

  call test_implicit (a)
  if (any (a .ne. 2, 1)) call abort
end
