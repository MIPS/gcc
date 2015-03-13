! { dg-do compile }
! Check that the compiler reports the errors, but does not segfault.
! Contributed by: Andre Vehreschild  <vehre@gcc.gnu.org>
!
program test
    implicit none
    class(*), pointer :: P
    class(*), allocatable :: P2

    allocate(P2, source=convertType(P))

contains

  function convertType(in)
    class(*), intent(in) :: in ! { dg-error "must be dummy, allocatable or pointer"}
    class(*) :: convertType
  end function
end program test
