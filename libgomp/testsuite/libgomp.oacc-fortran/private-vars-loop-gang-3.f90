! { dg-additional-options "-w" }

! Test of gang-private variables declared on loop directive, with broadcasting
! to partitioned vectors.

program main
  integer :: x, i, j, arr(0:32*32)

  do i = 0, 32*32-1
     arr(i) = i
  end do

  !$acc parallel copy(arr) num_gangs(32) num_workers(8) vector_length(32)
  !$acc loop gang private(x)
  do i = 0, 31
     x = i * 2;

     !$acc loop vector
     do j = 0, 31
        arr(i * 32 + j) = arr(i * 32 + j) + x
     end do
  end do
  !$acc end parallel

  do i = 0, 32 * 32 - 1
     if (arr(i) .ne. i + (i / 32) * 2) call abort
  end do
end program main
