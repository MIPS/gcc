! Program to test the LEN intrinsic
program test
  character(len=10) a
  character(len=8) w
  integer n
  a = w (n)

  if ((a .ne. "12345678") .or. (n .ne. 8)) call abort
end

function w(i)
  character(len=8) w
  integer i
  w = "012345678"
  i = len(w)
end
