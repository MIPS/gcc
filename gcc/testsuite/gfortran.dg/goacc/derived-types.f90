! Test ACC UPDATE with derived types.

module dt
  integer, parameter :: n = 10
  type inner
     integer :: d(n)
  end type inner
  type dtype
     integer(8) :: a, b, c(n)
     type(inner) :: in
  end type dtype
end module dt

program derived_acc
  use dt
  
  implicit none
  type(dtype):: var
  integer i
  !$acc declare create(var)
  !$acc declare pcopy(var%a) ! { dg-error "Syntax error in OpenMP" }

  !$acc update host(var)
  !$acc update host(var%a)
  !$acc update device(var)
  !$acc update device(var%a)
  !$acc update self(var)
  !$acc update self(var%a)
  
  !$acc enter data copyin(var)
  !$acc enter data copyin(var%a)

  !$acc exit data copyout(var)
  !$acc exit data copyout(var%a)

  !$acc data copy(var%a) ! { dg-error "Syntax error in OpenMP" }
  !$acc end data ! { dg-error "Unexpected ..ACC END DATA" }
  
  !$acc data copy(var)
  !$acc end data

  !$acc data copyout(var%a) ! { dg-error "Syntax error in OpenMP" }
  !$acc end data ! { dg-error "Unexpected ..ACC END" }

  !$acc parallel loop pcopyout(var)
  do i = 1, 10
  end do  
  !$acc end parallel loop

  !$acc parallel loop copyout(var%a) ! { dg-error "Syntax error in OpenMP" }
  do i = 1, 10
  end do
  !$acc end parallel loop ! { dg-error "Unexpected ..ACC END" }

  !$acc parallel pcopy(var)
  !$acc end parallel

  !$acc parallel pcopy(var%a) ! { dg-error "Syntax error in OpenMP" }
  do i = 1, 10
  end do
  !$acc end parallel ! { dg-error "Unexpected ..ACC END" }
  
  !$acc kernels pcopyin(var)
  !$acc end kernels

  !$acc kernels pcopy(var%a) ! { dg-error "Syntax error in OpenMP" }
  do i = 1, 10
  end do
  !$acc end kernels ! { dg-error "Unexpected ..ACC END" }

  !$acc kernels loop pcopyin(var)
  do i = 1, 10
  end do
  !$acc end kernels loop

  !$acc kernels loop pcopy(var%a) ! { dg-error "Syntax error in OpenMP" }
  do i = 1, 10
  end do
  !$acc end kernels loop ! { dg-error "Unexpected ..ACC END" }
end program derived_acc
