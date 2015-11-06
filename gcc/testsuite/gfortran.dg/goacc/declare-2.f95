
module amod

contains

subroutine asubr (b)
  implicit none
  integer :: b(8)

  !$acc declare copy (b) ! { dg-error "Invalid clause in module" }
  !$acc declare copyout (b) ! { dg-error "Invalid clause in module" }
  !$acc declare present (b) ! { dg-error "Invalid clause in module" }
  !$acc declare present_or_copy (b) ! { dg-error "Invalid clause in module" }
  !$acc declare present_or_copyin (b) ! { dg-error "Invalid clause in module" }
  !$acc declare present_or_copyout (b) ! { dg-error "Invalid clause in module" }
  !$acc declare present_or_create (b) ! { dg-error "Invalid clause in module" }
  !$acc declare deviceptr (b) ! { dg-error "Invalid clause in module" }
  !$acc declare create (b) copyin (b) ! { dg-error "present on multiple clauses" }

end subroutine

end module

subroutine bsubr (foo)
  implicit none

  integer, dimension (:) :: foo

  !$acc declare copy (foo) ! { dg-error "assumed-size dummy array" }
  !$acc declare copy (foo(1:2)) ! { dg-error "assumed-size dummy array" }

end subroutine

program test
  integer :: a(8)
  integer :: b(8)
  integer :: c(8)

  !$acc declare create (a) copyin (a) ! { dg-error "present on multiple clauses" }
  !$acc declare copyin (b)
  !$acc declare copyin (b) ! { dg-error "present on multiple clauses" }
  !$acc declare copy (c(1:2)) ! { dg-error "Subarray: 'c' not allowed" }

end program
