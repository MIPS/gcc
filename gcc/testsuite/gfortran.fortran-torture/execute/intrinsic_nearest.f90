!Program to test NEAREST intrinsic function.

program test_nearest
  real s, r, x, y
  integer i
  equivalence (s,i)

  r = 2.0
  s = 3.0
  call test_n (s, r)

  i = z'00800000'
  call test_n (s, r)

  i = z'007fffff'
  call test_n (s, r)

  i = z'00800100'
  call test_n (s, r)

  i = z'7f800000'
  call test_n(s, r)

  i = z'7f7fffff'
  x = nearest(s, -r)
  y = nearest(x, r)
  if (y .ne. s) call abort()

  s = 0
  x = nearest(s, r)
  y = nearest(s, -r)
  if (.not. (x .gt. s .and. y .lt. s )) call abort()
                                                                                
  i = z'7f800000'
  call test_n(s, r)
end

subroutine test_n(s1, r)
  real r, s1, x

  x = nearest(s1, r)
  if (nearest(x, -r) .ne. s1) call abort()
  x = nearest(s1, -r)
  if (nearest(x, r) .ne. s1) call abort()

  s1 = -s1
  x = nearest(s1, r)
  if (nearest(x, -r) .ne. s1) call abort()
  x = nearest(s1, -r)
  if (nearest(x, r) .ne. s1) call abort()
end
  
