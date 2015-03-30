! { dg-do run }
!
! Contributed by Reinhold Bader
!
program assumed_shape_01
  implicit none
  type :: cstruct
     integer :: i
     real :: r(2)
  end type cstruct

  type(cstruct), pointer :: u(:)
  integer, allocatable :: iv(:)
  integer, allocatable :: im(:,:)
  integer, parameter :: cim(2,3) = reshape([1,2,3, 2,3,4], [2,3])
  integer :: i
  integer, parameter :: lcim(2,10) = reshape([(i, i=1,10),(i,i=1,10)], [2,10])

  allocate(iv, source= [ 1, 2, 3, 4])
  if (any(iv /= [ 1, 2, 3, 4])) call abort()
  deallocate(iv)

  allocate(iv, source=(/(i, i=1,10)/))
  if (any(iv /= (/(i, i=1,10)/))) call abort()

  ! Now 2D
  allocate(im, source= cim)
  if (any(im /= cim)) call abort()
  deallocate(im)

  allocate(im, source= reshape([iv, iv], [2, size(iv, 1)]))
  if (any(im /= lcim)) call abort()
  deallocate(im)
  deallocate(iv)

! The following is VALID Fortran 2008 but NOT YET supported 
  allocate(u, source=[cstruct( 4, [1.1,2.2] )] )
  if (u(1)%i /= 4 .or. any(abs(u(1)%r(:) - [1.1,2.2]) > 1E-6)) call abort()
  deallocate (u)

  allocate(iv, source= arrval())
  if (any(iv /= [ 1, 2, 4, 5, 6])) call abort()
  deallocate(iv)
contains
  function arrval()
    integer, dimension(5) :: arrval
    arrval = [ 1, 2, 4, 5, 6]
  end function
end program assumed_shape_01
