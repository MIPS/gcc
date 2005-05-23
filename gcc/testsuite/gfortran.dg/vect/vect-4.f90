! { dg-do compile }
! { dg-require-effective-target vect_float }

SUBROUTINE SAXPY(X, Y, A)
DIMENSION X(64), Y(64)
Y = Y + A * X
END

! { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { xfail vect_no_align } } } 
! { dg-final { scan-tree-dump-times "Alignment of access forced using peeling" 1 "vect" { xfail vect_no_align } } }
! { dg-final { scan-tree-dump-times "Vectorizing an unaligned access" 1 "vect" { xfail vect_no_align } } }
