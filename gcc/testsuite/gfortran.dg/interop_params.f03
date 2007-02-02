! { dg-do compile }
module interop_params
use, intrinsic :: iso_c_binding

type my_f90_type
   integer :: i
   real :: x
end type my_f90_type

contains
  subroutine test_0(my_f90_int, my_type) bind(c)
    use, intrinsic :: iso_c_binding
    integer, value :: my_f90_int ! { dg-warning "may not be C interoperable" }
    type(my_f90_type) :: my_type ! { dg-warning "may not be C interoperable" }
  end subroutine test_0

  subroutine test_1(my_f90_real) bind(c)
    real(c_int), value :: my_f90_real ! { dg-error "is for type INTEGER" } 
  end subroutine test_1
end module interop_params
