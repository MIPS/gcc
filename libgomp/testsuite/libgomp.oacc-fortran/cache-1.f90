
program main
    integer, parameter :: N = 8
    integer, dimension (N) :: a, b
    integer :: i
    integer :: idx, len

    idx = 1
    len = 2

    !$acc parallel copyin (a(1:N)) copyout (b(1:N))
        do i = 1, N

    !$acc cache (a(1:N))
    !$acc cache (a(0:N))
    !$acc cache (a(0:N), b(0:N))
    !$acc cache (a(0))
    !$acc cache (a(0), a(1), b(0:N))
    !$acc cache (a(idx))
    !$acc cache (a(idx:len))

            b(i) = a(i)
        end do
    !$acc end parallel

end program
