! { dg-additional-options "-w" }

! Test of gang-private variables declared on loop directive.

program main
  integer :: x, i, arr(32)

  do i = 1, 32
     arr(i) = i
  end do

  !$acc parallel copy(arr) num_gangs(32) num_workers(8) vector_length(32)
  !$acc loop gang private(x)
  do i = 1, 32
     x = i * 2;
     arr(i) = arr(i) + x
  end do
  !$acc end parallel

  do i = 1, 32
     if (arr(i) .ne. i * 3) call abort
  end do
end program main
