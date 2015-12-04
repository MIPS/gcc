! Test of worker-private variables declared on a loop directive.

program main
  integer :: x, i, j, arr(0:32*32)
  common x

  do i = 0, 32*32-1
     arr(i) = i
  end do

  !$acc parallel copy(arr) num_gangs(32) num_workers(8) vector_length(32)
  !$acc loop gang private(x)
  do i = 0, 31
     !$acc loop worker private(x)
     do j = 0, 31
        x = ieor(i, j * 3)
        arr(i * 32 + j) = arr(i * 32 + j) + x
     end do
  end do
  !$acc end parallel

  do i = 0, 32 * 32 - 1
     if (arr(i) .ne. i + ieor(i / 32, mod(i, 32) * 3)) call abort
  end do
end program main
