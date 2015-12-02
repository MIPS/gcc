! { dg-do run }
! { dg-additional-options "-cpp" }

! { dg-xfail-if "TODO" { *-*-* } }
! { dg-excess-errors "TODO" }

program test
  implicit none

  integer, target :: i, arr(1000)
  integer, pointer :: ip, parr(:), iph, parrh(:)

  !$acc data copyin(i, arr)
  !$acc host_data use_device(i, arr)
  ip => i
  parr => arr
  !$acc end host_data
  !$acc end data

  iph => i
  parrh => arr

#if ACC_MEM_SHARED
  if (.not. associated(ip, iph)) call abort
  if (.not. associated(parr, parrh)) call abort
#else
  if (associated(ip, iph)) call abort
  if (associated(parr, parrh)) call abort
#endif

end program test
