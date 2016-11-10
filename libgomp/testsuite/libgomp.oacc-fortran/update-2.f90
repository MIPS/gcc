! Test ACC UPDATE with derived types. The DEVICE clause depends on an
! accelerator being present.

! { dg-do run  { target openacc_nvidia_accel_selected } }

module dt
  integer, parameter :: n = 10
  type inner
     integer :: d(n)
  end type inner
  type mytype
     integer(8) :: a, b, c(n)
     type(inner) :: in
  end type mytype
end module dt

program derived_acc
  use dt

  implicit none
  integer i, res
  type(mytype) :: var

  var%a = 0
  var%b = 1
  var%c(:) = 10
  var%in%d(:) = 100

  var%c(:) = 10

  !$acc enter data copyin(var)

  !$acc parallel loop present(var)
  do i = 1, 1
     var%a = var%b
  end do
  !$acc end parallel loop

  !$acc update host(var%a)

  if (var%a /= var%b) call abort

  var%b = 100

  !$acc update device(var%b)

  !$acc parallel loop present(var)
  do i = 1, 1
     var%a = var%b
  end do
  !$acc end parallel loop

  !$acc update host(var%a)

  if (var%a /= var%b) call abort

  !$acc parallel loop present (var)
  do i = 1, n
     var%c(i) = i
  end do
  !$acc end parallel loop

  !$acc update host(var%c)

  var%a = -1

  do i = 1, n
     if (var%c(i) /= i) call abort
     var%c(i) = var%a
  end do

  !$acc update device(var%a)
  !$acc update device(var%c)

  res = 0

  !$acc parallel loop present(var) reduction(+:res)
  do i = 1, n
     if (var%c(i) /= var%a) res = res + 1
  end do

  if (res /= 0) call abort

  var%c(:) = 0

  !$acc update device(var%c)

  !$acc parallel loop present(var)
  do i = 5, 5
     var%c(i) = 1
  end do
  !$acc end parallel loop

  !$acc update host(var%c(5))

  do i = 1, n
     if (i /= 5 .and. var%c(i) /= 0) call abort
     if (i == 5 .and. var%c(i) /= 1) call abort
  end do

  !$acc parallel loop present(var)
  do i = 1, n
     var%in%d = var%a
  end do
  !$acc end parallel loop

  !$acc update host(var%in%d)

  do i = 1, n
     if (var%in%d(i) /= var%a) call abort
  end do

  var%c(:) = 0

  !$acc update device(var%c)

  var%c(:) = -1

  !$acc parallel loop present(var)
  do i = n/2, n
     var%c(i) = i
  end do
  !$acc end parallel loop

  !$acc update host(var%c(n/2:n))

  do i = 1,n
     if (i < n/2 .and. var%c(i) /= -1) call abort
     if (i >= n/2 .and. var%c(i) /= i) call abort
  end do

  var%in%d(:) = 0
  !$acc update device(var%in%d)

  !$acc parallel loop present(var)
  do i = 5, 5
     var%in%d(i) = 1
  end do
  !$acc end parallel loop

  !$acc update host(var%in%d(5))

  do i = 1, n
     if (i /= 5 .and. var%in%d(i) /= 0) call abort
     if (i == 5 .and. var%in%d(i) /= 1) call abort
  end do

  !$acc exit data delete(var)

  call derived_acc_subroutine(var)
end program derived_acc

subroutine derived_acc_subroutine(var)
  use dt

  implicit none
  integer i, res
  type(mytype) :: var

  var%a = 0
  var%b = 1
  var%c(:) = 10
  var%in%d(:) = 100

  var%c(:) = 10

  !$acc enter data copyin(var)

  !$acc parallel loop present(var)
  do i = 1, 1
     var%a = var%b
  end do
  !$acc end parallel loop

  !$acc update host(var%a)

  if (var%a /= var%b) call abort

  var%b = 100

  !$acc update device(var%b)

  !$acc parallel loop present(var)
  do i = 1, 1
     var%a = var%b
  end do
  !$acc end parallel loop

  !$acc update host(var%a)

  if (var%a /= var%b) call abort

  !$acc parallel loop present (var)
  do i = 1, n
     var%c(i) = i
  end do
  !$acc end parallel loop

  !$acc update host(var%c)

  var%a = -1

  do i = 1, n
     if (var%c(i) /= i) call abort
     var%c(i) = var%a
  end do

  !$acc update device(var%a)
  !$acc update device(var%c)

  res = 0

  !$acc parallel loop present(var) reduction(+:res)
  do i = 1, n
     if (var%c(i) /= var%a) res = res + 1
  end do

  if (res /= 0) call abort

  var%c(:) = 0

  !$acc update device(var%c)

  !$acc parallel loop present(var)
  do i = 5, 5
     var%c(i) = 1
  end do
  !$acc end parallel loop

  !$acc update host(var%c(5))

  do i = 1, n
     if (i /= 5 .and. var%c(i) /= 0) call abort
     if (i == 5 .and. var%c(i) /= 1) call abort
  end do

  !$acc parallel loop present(var)
  do i = 1, n
     var%in%d = var%a
  end do
  !$acc end parallel loop

  !$acc update host(var%in%d)

  do i = 1, n
     if (var%in%d(i) /= var%a) call abort
  end do

  var%c(:) = 0

  !$acc update device(var%c)

  var%c(:) = -1

  !$acc parallel loop present(var)
  do i = n/2, n
     var%c(i) = i
  end do
  !$acc end parallel loop

  !$acc update host(var%c(n/2:n))

  do i = 1,n
     if (i < n/2 .and. var%c(i) /= -1) call abort
     if (i >= n/2 .and. var%c(i) /= i) call abort
  end do

  var%in%d(:) = 0
  !$acc update device(var%in%d)

  !$acc parallel loop present(var)
  do i = 5, 5
     var%in%d(i) = 1
  end do
  !$acc end parallel loop

  !$acc update host(var%in%d(5))

  do i = 1, n
     if (i /= 5 .and. var%in%d(i) /= 0) call abort
     if (i == 5 .and. var%in%d(i) /= 1) call abort
  end do

  !$acc exit data delete(var)
end subroutine derived_acc_subroutine
