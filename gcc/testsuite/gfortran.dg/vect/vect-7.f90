! { dg-do compile }
! { dg-require-effective-target vect_float }

subroutine foo(a,b)
real a,b
type bzz
   real d(100)
end type bzz
type bzzz
   real d(100)
   type (bzz) bz
end type bzzz
type (bzzz) e

dimension a(100), b(100)
read *,x,y
a=log(x)
b=log(y)
e%bz%d = a+b
print *,e%bz%d
end

! { dg-final { scan-tree-dump-times "vectorized 2 loops" 1 "vect" { xfail vect_no_align } } }
! { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target vect_no_align } } }
! { dg-final { scan-tree-dump-times "Alignment of access forced using peeling" 1 "vect" } }
! { dg-final { scan-tree-dump-times "Vectorizing an unaligned access" 1 "vect" { xfail vect_no_align } } }
! { dg-final { cleanup-tree-dump "vect" } }
