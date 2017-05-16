! { dg-do run }
! { dg-skip-if "" { *-*-* } { "*" } { "-DACC_MEM_SHARED=0" } }

program refcount_test
  use openacc
  integer, allocatable :: h(:)
  integer i, N

  N = 256
  allocate (h(N))

  do i = 1, N
     h(i) = i
  end do

  !$acc data create (h(1:N))
  !$acc enter data create (h(1:N))
  !$acc end data

  if (acc_is_present (h) .eqv. .FALSE.) call abort

  !$acc exit data delete (h(1:N))

  if (acc_is_present (h) .eqv. .TRUE.) call abort

end program refcount_test
