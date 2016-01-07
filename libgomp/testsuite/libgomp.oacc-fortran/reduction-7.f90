! { dg-do run }
! { dg-additional-options "-w" }

! subroutine reduction with firstprivate variables

program reduction
  integer, parameter    :: n = 100
  integer               :: i, j, vsum, cs, arr(n)

  call redsub_bogus (cs, n)
  call redsub_combined (cs, n, arr)

  vsum = 0

  ! Verify the results
  do i = 1, n
     vsum = i
     do j = 1, n
        vsum = vsum + 1;
     end do
     if (vsum .ne. arr(i)) call abort ()
  end do
end program reduction

! Bogus reduction on an impliclitly firstprivate variable.  The results do
! survive the parallel region.  The goal here is to ensure that gfortran
! doesn't ICE.

subroutine redsub_bogus(sum, n)
  integer :: sum, n, arr(n)
  integer :: i

  !$acc parallel
  !$acc loop gang worker vector reduction (+:sum)
  do i = 1, n
     sum = sum + 1
  end do
  !$acc end parallel
end subroutine redsub_bogus

! This reduction involving a firstprivate variable yields legitimate results.

subroutine redsub_combined(sum, n, arr)
  integer :: sum, n, arr(n)
  integer :: i, j

  !$acc parallel copy (arr)
  !$acc loop gang
  do i = 1, n
     sum = i;

     !$acc loop reduction(+:sum)
     do j = 1, n
        sum = sum + 1
     end do

     arr(i) = sum
  end do
  !$acc end parallel
end subroutine redsub_combined
