! Test of worker-private variables declared on a loop directive, broadcasting
! to vector-partitioned mode.  Addressable worker variable.

program main
  integer :: i, j, k, idx, arr(0:32*32*32)
  integer, target :: x
  integer, pointer :: p

  do i = 0, 32*32*32-1
     arr(i) = i
  end do

  !$acc parallel copy(arr) num_gangs(32) num_workers(8) vector_length(32)
  !$acc loop gang
  do i = 0, 31
     !$acc loop worker private(x, p)
     do j = 0, 31
        p => x
        x = ieor(i, j * 3)

        !$acc loop vector
        do k = 0, 31
           arr(i * 1024 + j * 32 + k) = arr(i * 1024 + j * 32 + k) + x * k
        end do

        p = ior(i, j * 5)

        !$acc loop vector
        do k = 0, 31
           arr(i * 1024 + j * 32 + k) = arr(i * 1024 + j * 32 + k) + x * k
        end do
     end do
  end do
  !$acc end parallel

  do i = 0, 32 - 1
     do j = 0, 32 -1
        do k = 0, 32 - 1
           idx = i * 1024 + j * 32 + k
           if (arr(idx) .ne. idx + ieor(i, j * 3) * k + ior(i, j * 5) * k) then
              call abort
           end if
        end do
     end do
  end do
end program main
