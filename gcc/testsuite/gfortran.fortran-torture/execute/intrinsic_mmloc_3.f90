! Check we do the right thing with extreme values.
! From PR12704
program intrinsic_mmloc_3
  integer, dimension(3) :: d

  d = -huge (d)
  if (maxloc (d, 1) .ne. 1) call abort()
end program
