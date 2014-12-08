! { dg-do compile }
! Testing fix for 
! PR fortran/60255 
!
program test
    implicit none
    character(LEN=:), allocatable :: S
    call subP(S)
    call sub1("test")
    call sub2()

contains

  subroutine sub1(dcl)
    character(len=*), target :: dcl
    class(*), pointer :: ucp

    ucp => dcl

    select type (ucp)
    type is (character(len=*))
      print *,"char"
    type is (real)
      print *,"real"
    class default
      print *,"something else"
    end select

  end subroutine
  
  subroutine sub2 
    character(len=:), allocatable, target :: dcl
    class(*), pointer :: ucp

    dcl = "ttt"
    ucp => dcl

    select type (ucp)
    type is (character(len=*))
      print *,"char"
    type is (real)
      print *,"real"
    class default
      print *,"something else"
    end select

  end subroutine

  subroutine subP(P)
        class(*) :: P
  end subroutine
 
end program
