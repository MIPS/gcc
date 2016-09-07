! { dg-do run }
!
! Check pr57117 is fixed.

program pr57117
  implicit none

  type :: ti
  end type

  class(ti), allocatable :: x(:,:), y(:,:)

  allocate(x(2,6))
  allocate(y, source=transpose(x))

  if (any( ubound(y) /= [6,2])) call abort()
  deallocate (x,y)
end

