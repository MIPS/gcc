! { dg-do run }
! Testing fix for 
! PR fortran/60255 
!
program test
    implicit none
    character(LEN=:), allocatable :: S
    call subP(S)
    call sub2()
    call sub1("test")

contains

  subroutine sub1(dcl)
    character(len=*), target :: dcl
    class(*), pointer :: ucp
!    character(len=:), allocatable ::def

    ucp => dcl

    select type (ucp)
    type is (character(len=*))
      if (len(ucp) .NE. 4) then
        call abort()
!      else
!        def = ucp
!        if (len(def) .NE. 4) then
!          call abort()   ! This abort is expected currently           
!        end if
      end if
    class default
      call abort()
    end select
  end subroutine
  
  subroutine sub2 
    character(len=:), allocatable, target :: dcl
    class(*), pointer :: ucp

    dcl = "ttt"
    ucp => dcl

    select type (ucp)
    type is (character(len=*))
      if (len(ucp) .NE. 3) then
        call abort()
      end if
    class default
      call abort()
    end select
  end subroutine

  subroutine subP(P)
        class(*) :: P
  end subroutine
 
end program
