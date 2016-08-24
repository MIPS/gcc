! Test present data clauses in acc offloaded regions when the subarray
! inside the present clause does not have the same base offset value
! as the subarray in the enclosing acc data or acc enter data variable.

program test
  implicit none

  integer, parameter :: n = 30, m = 10
  integer :: i
  integer, allocatable :: data(:)
  logical bounded

  allocate (data(n))

  data(:) = -1

  !$acc data copy (data(5:20))
  call test_data (data, n, m)
  !$acc end data

  do i = 1, n
     bounded = i < m .or. i >= m+m
     if (bounded .and. (data(i) /= -1)) then
        call abort
     else if (.not. bounded .and. data(i) /= 10) then
        call abort
     end if
  end do

  deallocate (data)
end program test

subroutine test_data (data, n, m)
  implicit none

  integer :: n, m, data(n), i

  !$acc parallel loop present (data(m:m))
  do i = m, m+m-1
     data(i) = m
  end do
  !$acc end parallel loop
end subroutine test_data
