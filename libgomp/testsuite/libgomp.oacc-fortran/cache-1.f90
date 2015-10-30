! { dg-do run }

program main
    integer, parameter :: N = 8
    integer, dimension (N) :: a, b
    integer :: i
    integer :: idx, len

    idx = 1
    len = 2

    !$acc parallel copyin (a(1:N)) copyout (b(1:N))
        do i = 1, N

    !$acc cache (a)
    !$acc cache (a, b)
    !$acc cache (a(2))
    !$acc cache (a(3), b(4))
    !$acc cache (a(1:N))
    !$acc cache (a(1:N), b(1:N))
    !$acc cache (a(idx))
    !$acc cache (a(idx:len), b(idx:len))
    !$acc cache (a(i:i+2))
    !$acc cache (a(i:i+2), b(i+1:i+3))

            b(i) = a(i)
        end do
    !$acc end parallel

end program
