! { dg-do run }
! { dg-additional-options "-cpp" }

program test
  use openacc
  integer, parameter :: N = 8
  real, allocatable :: a(:,:), b(:,:)
  real, allocatable :: c(:), d(:)
  integer i, j

  i = 0
  j = 0

  allocate (a(N,N))
  allocate (b(N,N))

  a(:,:) = 3.0
  b(:,:) = 0.0

  !$acc enter data copyin (a(1:N,1:N), b(1:N,1:N))

  !$acc parallel present (a(1:N,1:N), b(1:N,1:N))
  do i = 1, n
    do j = 1, n
      b(j,i) = a (j,i)
    end do
  end do
  !$acc end parallel

  !$acc exit data copyout (a(1:N,1:N), b(1:N,1:N))

  do i = 1, n
    do j = 1, n
      if (a(j,i) .ne. 3.0) call abort
      if (b(j,i) .ne. 3.0) call abort
    end do
  end do

  allocate (c(N))
  allocate (d(N))

  c(:) = 3.0
  d(:) = 0.0

  !$acc enter data copyin (c(1:N)) create (d(1:N)) async
  !$acc wait
  
  !$acc parallel present (c(1:N), d(1:N))
    do i = 1, N
      d(i) = c(i) + 1
    end do
  !$acc end parallel

  !$acc exit data copyout (c(1:N), d(1:N)) async
  !$acc wait

  do i = 1, N
    if (d(i) .ne. 4.0) call abort
  end do

  c(:) = 3.0
  d(:) = 0.0

  !$acc enter data copyin (c(1:N)) async
  !$acc enter data create (d(1:N)) wait
  !$acc wait

  !$acc parallel present (c(1:N), d(1:N))
    do i = 1, N
      d(i) = c(i) + 1
    end do
  !$acc end parallel
  
  !$acc exit data delete (c(1:N)) copyout (d(1:N)) async
  !$acc exit data async
  !$acc wait

  do i = 1, N
    if (d(i) .ne. 4.0) call abort
  end do

#if !ACC_MEM_SHARED

  c(:) = 3.0
  d(:) = 0.0

  !$acc enter data present_or_copyin (c(0:N))

  if (acc_is_present (c) .eqv. .FALSE.) call abort

  !$acc exit data copyout (c(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort

  !$acc exit data delete (c(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort

  do i = 1, N
    if (c(i) .ne. 3.0) call abort
  end do

  c(:) = 5.0
  d(:) = 9.0

  !$acc enter data present_or_copyin (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .FALSE.) call abort
  if (acc_is_present (d) .eqv. .FALSE.) call abort

  !$acc exit data copyout (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort
  if (acc_is_present (d) .eqv. .TRUE.) call abort

  !$acc exit data delete (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort
  if (acc_is_present (d) .eqv. .TRUE.) call abort

  do i = 1, N
    if (c(i) .ne. 5.0) call abort
    if (d(i) .ne. 9.0) call abort
  end do

  !$acc enter data present_or_create (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .FALSE.) call abort
  if (acc_is_present (d) .eqv. .FALSE.) call abort

  !$acc parallel present (c(0:N), d(0:N))
    do i = 1, N
      c(i) = 1.0;
      d(i) = 2.0;
    end do
  !$acc end parallel

  !$acc exit data copyout (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort
  if (acc_is_present (d) .eqv. .TRUE.) call abort

  do i = 1, N
    if (c(i) .ne. 1.0) call abort
    if (d(i) .ne. 2.0) call abort
  end do

  !$acc enter data present_or_create (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .FALSE.) call abort
  if (acc_is_present (d) .eqv. .FALSE.) call abort

  !$acc enter data present_or_create (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .FALSE.) call abort
  if (acc_is_present (d) .eqv. .FALSE.) call abort

  !$acc exit data delete (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort
  if (acc_is_present (d) .eqv. .TRUE.) call abort

  !$acc exit data delete (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort
  if (acc_is_present (d) .eqv. .TRUE.) call abort

  !$acc enter data create (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .FALSE.) call abort
  if (acc_is_present (d) .eqv. .FALSE.) call abort

  !$acc enter data present_or_copyin (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .FALSE.) call abort
  if (acc_is_present (d) .eqv. .FALSE.) call abort

  !$acc exit data delete (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort
  if (acc_is_present (d) .eqv. .TRUE.) call abort

  !$acc exit data delete (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort
  if (acc_is_present (d) .eqv. .TRUE.) call abort

  !$acc enter data present_or_copyin (c(0:N))

  if (acc_is_present (c) .eqv. .FALSE.) call abort
  if (acc_is_present (d) .eqv. .TRUE.) call abort

  !$acc exit data delete (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort
  if (acc_is_present (d) .eqv. .TRUE.) call abort

  !$acc exit data delete (c(0:N), d(0:N))

  if (acc_is_present (c) .eqv. .TRUE.) call abort
  if (acc_is_present (d) .eqv. .TRUE.) call abort

#endif

end program test
