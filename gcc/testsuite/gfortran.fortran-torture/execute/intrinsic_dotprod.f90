! Program to test the DOT_PRODUCT intrinsic
program testforall
   implicit none
   integer, dimension (3) :: a
   integer, dimension (3) :: b
   real, dimension(3) :: c
   real r

   a = (/1, 2, 3/);
   b = (/4, 5, 6/);
   c = (/4, 5, 6/);

   if (dot_product(a, b) .ne. 32) call abort

   r = dot_product(a, c)
   if (abs(r - 32.0) .gt. 0.001) call abort
end program
