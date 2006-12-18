! { dg-do run }
! { dg-additional-sources c_ptr_tests_driver.c }
module c_ptrTests
  use, intrinsic :: iso_c_binding

  ! TODO::
  ! in order to be associated with a C address, 
  ! the derived type needs to be C interoperable, 
  ! which requires bind(c) and all fields interoperable 
  ! currently, the compiler doesn't verify that the 2nd arg 
  ! to c_f_pointer is C interoperable.. --Rickett, 03.01.06
  type, bind(c) :: myType
     type(c_ptr) :: myServices
     type(c_funptr) :: mySetServices
     type(c_ptr) :: myPort
  end type myType

  type, bind(c) :: f90Services
     integer(c_int) :: compId
     type(c_ptr) :: globalServices
  end type f90Services

  interface
     subroutine test(myDouble) bind(c)
       use, intrinsic :: iso_c_binding, only: c_double, c_ptr
       real(c_double), value :: myDouble
     end subroutine test
  end interface

  contains
    
    subroutine ptr_test(c_self, services, myDouble) bind(c)
      use, intrinsic :: iso_c_binding
      implicit none

      type(c_ptr), value :: c_self, services
      real(c_double), value :: myDouble
      type(myType), pointer :: self
      type(f90Services), pointer :: localServices

      call c_f_pointer(c_self, self)
      if(.not. associated(self)) then
         call abort()
      end if
      self%myServices = services

      ! this is a C routine
      call test(myDouble)

      ! get access to the local services obj from C
      call c_f_pointer(self%myServices, localServices)
    end subroutine ptr_test
end module c_ptrTests
