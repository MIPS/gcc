! { dg-do compile }
! { dg-require-effective-target vect_float }

subroutine foo(a,b)
real a,b
type bzz
   real d(100)
end type bzz
type (bzz) e

dimension a(100), b(100)
read *,x,y
a=log(x)
b=log(y)
e%d = a+b
print *,e%d
end

! { dg-final { scan-tree-dump-times "vectorized 3 loops" 1 "vect" } }
! first two loops:
! { dg-final { scan-tree-dump-times "Alignment of access forced using peeling" 2 "vect" } }
! third loop:
! { dg-final { scan-tree-dump-times "Vectorizing an unaligned access" 2 "vect" { xfail vect_no_align } } }
! { dg-final { scan-tree-dump-times "Alignment of access forced using versioning" 2 "vect" { target vect_no_align } } }
! { dg-final { cleanup-tree-dump "vect" } }
