! Check zero sized arrays work correcly
! From PR12704
program intrinsic_mmloc_4
  integer, allocatable, dimension(:) :: d
  integer, allocatable, dimension(:,:) :: a
  integer, dimension(2) :: b

  allocate (d(0))
  print *, maxloc (d, 1)
  if (maxloc (d, 1) .ne. 0) call abort()
  allocate (a(1, 0))
  b = minloc (a)
  print *, b
  if (any (b .ne. 0)) call abort()
end program
