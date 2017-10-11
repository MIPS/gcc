! Test derived types in data clauses.

! { dg-do run }

module newtype
  type dtype
     integer :: a, b, c
     integer, allocatable :: ary(:)
  end type dtype
end module newtype

program main
  use newtype
  implicit none
  integer, parameter :: n = 100
  integer i
  type (dtype), dimension(n) :: d1
  type (dtype) :: d2
  external process

  allocate (d2%ary(n))

  !$acc enter data create (d2%ary)

  do i = 1, n
     d2%ary(i) = 1
  end do

  !$acc update device (d2%ary)

  call process (n, d2%ary)

  !$acc exit data copyout (d2%ary)

  do i = 1, n
     if (d2%ary(i) /= i + 1) call abort
  end do

  !$acc data copy(d1(1:n))
  !$acc parallel loop
  do i = 1, n
     d1(i)%a = i
     d1(i)%b = i-1
     d1(i)%c = i+1
  end do
  !$acc end data

  do i = 1, n
     if (d1(i)%a /= i) call abort
     if (d1(i)%b /= i-1) call abort
     if (d1(i)%c /= i+1) call abort
  end do

  deallocate (d2%ary)
end program main

subroutine process (a, b)
  use newtype
  implicit none
  integer :: a, i
  integer :: b(a)

  !$acc parallel loop present (b(1:a))
  do i = 1, a
     b(i) = b(i) + i
  end do
end subroutine process
