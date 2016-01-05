! { dg-do run }
! { dg-additional-options "-w" }

! subroutine reduction

program reduction
  integer, parameter    :: n = 40, c = 10
  integer               :: i, vsum, gs, ws, vs, cs

  call redsub_gang (gs, n, c)
  call redsub_worker (ws, n, c)
  call redsub_vector (vs, n, c)
  call redsub_combined (cs, n, c)

  vsum = 0

  ! Verify the results
  do i = 1, n
     vsum = vsum + c
  end do

  if (gs .ne. vsum) call abort ()
  if (ws .ne. vsum) call abort ()
  if (vs .ne. vsum) call abort ()
  if (cs .ne. vsum) call abort ()
end program reduction

subroutine redsub_gang(sum, n, c)
  integer :: sum, n, c

  sum = 0

  !$acc parallel copyin (n, c) num_gangs(n) copy(sum)
  !$acc loop reduction(+:sum) gang
  do i = 1, n
     sum = sum + c
  end do
  !$acc end parallel
end subroutine redsub_gang

subroutine redsub_worker(sum, n, c)
  integer :: sum, n, c

  sum = 0

  !$acc parallel copyin (n, c) num_workers(4) vector_length (32) copy(sum)
  !$acc loop reduction(+:sum) worker
  do i = 1, n
     sum = sum + c
  end do
  !$acc end parallel
end subroutine redsub_worker

subroutine redsub_vector(sum, n, c)
  integer :: sum, n, c

  sum = 0

  !$acc parallel copyin (n, c) vector_length(32) copy(sum)
  !$acc loop reduction(+:sum) vector
  do i = 1, n
     sum = sum + c
  end do
  !$acc end parallel
end subroutine redsub_vector

subroutine redsub_combined(sum, n, c)
  integer :: sum, n, c

  sum = 0

  !$acc parallel num_gangs (8) num_workers (4) vector_length(32) copy(sum)
  !$acc loop reduction(+:sum) gang worker vector
  do i = 1, n
     sum = sum + c
  end do
  !$acc end parallel
end subroutine redsub_combined
