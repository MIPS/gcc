! Test of worker-private variables declared on a loop directive, broadcasting
! to vector-partitioned mode.  Aggregate worker variable.

program main
  type vec2
     integer x, y
  end type vec2
  
  integer :: i, j, k, idx, arr(0:32*32*32)
  type(vec2) :: pt
  
  do i = 0, 32*32*32-1
     arr(i) = i
  end do

  !$acc parallel copy(arr) num_gangs(32) num_workers(8) vector_length(32)
  !$acc loop gang
  do i = 0, 31
     !$acc loop worker private(pt)
     do j = 0, 31
        pt%x = ieor(i, j * 3)
        pt%y = ior(i, j * 5)
        
        !$acc loop vector
        do k = 0, 31
           arr(i * 1024 + j * 32 + k) = arr(i * 1024 + j * 32 + k) + pt%x * k
        end do

        !$acc loop vector
        do k = 0, 31
           arr(i * 1024 + j * 32 + k) = arr(i * 1024 + j * 32 + k) + pt%y * k
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
