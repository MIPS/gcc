! Program to test caracter string return values
function test()
   implicit none
   character(len=10) :: test
   test = "World"
end function

program strret
   implicit none
   character(len=15) :: s
   character(len=10) :: test

   s = test()
   if (s .ne. "World") call abort

   s = "Hello " // test()
   if (s .ne. "Hello World") call abort
end
