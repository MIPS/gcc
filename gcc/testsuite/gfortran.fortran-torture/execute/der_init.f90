! Program to test derived type initializers and constructors
program der_init
   implicit none
   type t
      integer :: i
      integer :: j = 4
   end type
   integer :: m, n

   ! Explicit initializer
   type (t) :: var = t(1, 2)
   ! Type (default) initializer
   type (t) :: var2

   if (var%i .ne. 1 .or. var%j .ne. 2) call abort
   if (var2%j .ne. 4) call abort
   var2 = t(6, 5)
   if (var2%i .ne. 6 .or. var2%j .ne. 5) call abort
   ! Non-constant constructor
   n = 1
   m = 5
   var2 = t(n, n + m)
   if (var2%i .ne. 1 .or. var2%j .ne. 6) call abort
end program
