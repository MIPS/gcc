! Program to test the transpose intrinsic
program intrinsic_transpose
   integer, dimension (3, 3) :: a, b

   a = 0
   b = reshape ((/1, 2, 3, 4, 5, 6, 7, 8, 9/), (/3, 3/))
   a = transpose (b)
   if (any (a .ne. reshape ((/1, 4, 7, 2, 5, 8, 3, 6, 9/), (/3, 3/)))) &
      call abort
end
