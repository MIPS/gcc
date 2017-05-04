! Test OpenACC update if_present clause.

! { dg-do run }
! { dg-additional-options "-cpp" }

program main
  use openacc
  integer, parameter ::  n = 100
  integer ::  a, i, x(n), y(n)

  !$acc update self(a) if_present
  !$acc update if_present device(x(20:30))

  a = -1
  do i = 1, n
     x(i) = a
  end do

  !$acc enter data copyin(x)
  call t (n, a, x, y)

  !$acc update host(x(10:30))

  do i = 1, n
#if ACC_MEM_SHARED
     if (x(i) /= 1000 + i) call abort
#else
     if ((i < 10 .or. i > 30) .and. (x(i) /= i)) call abort
     if ((i >= 10 .and. i <= 30) .and. (x(i) /= 1000 + i)) call abort
#endif
  end do

  !$acc exit data delete(x)

  a = -1;
  do i = 1, n
     x(i) = a
  end do

  !$acc data copy (x)
  call t (n, a, x, y)
  !$acc end data

contains
  subroutine t (n, a, c, d)
    integer :: n, a, b, i, c(n), d(n)

    !$acc parallel loop present(c(1:n)) copyout(d(1:n))
    do i = 1, n
       d(i) = c(i)
    end do

    do i = 1, n
       if (d(i) /= a) call abort
       c(i) = i
    end do

    a = 0

    !$acc update device(c(1:n), a) if_present

    !$acc parallel loop reduction(+:a) present(c(1:n))
    do i = 1, n
       a = a + c(i)
    end do

    !$acc update host(a) if_present

    b = 0
    do i = 1, n
       b = b + i
    end do

    if (a /= b) call abort

    !$acc enter data copyin (b)
    !$acc parallel loop reduction (+:b)
    do i = 1, n
       b = b + i;
    end do

    !$acc update self(b)
    if (2*a /= b) call abort
    !$acc exit data delete(b)

    !$acc parallel loop present(c(1:n))
    do i = 1, n
       c(i) = i
    end do

    !$acc update host (c(1:n))

    !$acc parallel loop present(c(1:n))
    do i = 1, n
       c(i) = 1000 + i
    end do
  end subroutine t
end program main
