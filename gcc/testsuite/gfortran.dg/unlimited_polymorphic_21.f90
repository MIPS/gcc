! { dg-do run }
! Testing fix for 
! PR fortran/60289
!
program test   
    implicit none
  
    class(*), pointer :: P
   
    allocate(character(20)::P)

    select type(P)
        type is (character(*))
            P ="some test string"
            if (P .ne. "some test string") then
                call abort ()
            end if
            if (len(P) .ne. len("some test string")) then
                call abort ()
            end if
        class default
            call abort ()
    end select

    deallocate(P)
end program test 

