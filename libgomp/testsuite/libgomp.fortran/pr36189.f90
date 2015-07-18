! PR target/36189
program foo
  implicit none
  common /mycom/ arr(2)
  common /mycom2/ arr2(1000000000_8:1000000002_8)
!$omp threadprivate (/mycom/)
  integer i
  integer*8 j
  real*8 arr
  real*8 arr2
  do i=1,2
    write(*,*) i
    arr(i)=0.0d0
  enddo
  do j=1000000000_8,1000000002_8
    write(*,*) j
    arr2(j)=0.0d0
  end do
end program
