! Verify that OpenACC declared allocatable arrays have implicit
! OpenACC enter and exit pragmas at the time of allocation and
! deallocation.

! { dg-additional-options "-fdump-tree-original" }

program allocate
  implicit none
  integer, allocatable :: a(:)
  integer, parameter :: n = 100
  integer i
  !$acc declare create(a)

  allocate (a(n))

  !$acc parallel loop copyout(a)
  do i = 1, n
     a(i) = i
  end do

  deallocate (a)
end program allocate

! { dg-final { scan-tree-dump-times "pragma acc enter data map.declare_allocate" 1 "original" } }
! { dg-final { scan-tree-dump-times "pragma acc exit data map.declare_deallocate" 1 "original" } }
