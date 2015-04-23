! { dg-do run }
! { dg-options "-Wreturn-type" }
!
! Check that pr58586 is fixed now.
! Based on a contribution by Vladimir Fuka
! Contibuted by Andre Vehreschild

module test_pr58586_mod
  implicit none

  type :: a
  end type

  type :: c
     type(a), allocatable :: a
  end type

  type :: d
  contains
     procedure :: init => d_init
  end type

  type, extends(d) :: e
  contains
     procedure :: init => e_init
  end type

  type :: b
     integer, allocatable :: a
  end type

contains

  subroutine add (d)
    type(b), value :: d
  end subroutine

  subroutine add_c (d)
    type(c), value :: d
  end subroutine

  type(c) function c_init() ! { dg-warning "not set" }
  end function

  type(c) function d_init(this) ! { dg-warning "not set" }
    class(d) :: this
  end function

  type(c) function e_init(this)
    class(e) :: this
    allocate (e_init%a)
  end function
end module test_pr58586_mod

program test_pr58586
  use test_pr58586_mod

  class(d), allocatable :: od
  class(e), allocatable :: oe
  ! These two are merely to check, if compilation works
  call add(b())
  call add(b(null()))

  ! This needs to execute, to see whether the segfault at runtime is resolved
  call add_c(c_init())

  allocate(od)
  call add_c(od%init())
  deallocate(od)
  allocate(oe)
  call add_c(oe%init())
  deallocate(oe)
   
end program

