! Verify the GOMP_MAP_FIRSTPRIVATE_INT optimziation on various types.

! { dg-do run }

program test
  implicit none

  integer (kind=1)  :: i1i, i1o
  integer (kind=2)  :: i2i, i2o
  integer (kind=4)  :: i4i, i4o
  integer (kind=8)  :: i8i, i8o
  integer (kind=16) :: i16i, i16o

  logical (kind=1)  :: l1i, l1o
  logical (kind=2)  :: l2i, l2o
  logical (kind=4)  :: l4i, l4o
  logical (kind=8)  :: l8i, l8o
  logical (kind=16) :: l16i, l16o

  real (kind=4)  :: r4i, r4o
  real (kind=8)  :: r8i, r8o

  complex (kind=4)  :: c4i, c4o
  complex (kind=8)  :: c8i, c8o

  character (kind=1) :: ch1i, ch1o
  character (kind=4) :: ch4i, ch4o

  i1i = 1
  i2i = 2
  i4i = 3
  i8i = 4
  i16i = 5

  l1i = .true.
  l2i = .false.
  l4i = .true.
  l8i = .true.
  l16i = .false.

  r4i = .5
  r8i = .25

  c4i = (2, -2)
  c8i = (4, -4)

  ch1i = "a"
  ch4i = "b"

  !$acc parallel firstprivate(i1i, i2i, i4i, i8i, i16i) &
  !$acc copyout(i1o, i2o, i4o, i8o, i16o) &
  !$acc firstprivate(l1i, l2i, l4i, l8i, l16i) &
  !$acc copyout(l1o, l2o, l4o, l8o, l16o) &
  !$acc firstprivate(r4i, r8i) copyout(r4o, r8o) &
  !$acc firstprivate(c4i, c8i) copyout(c4o, c8o) &
  !$acc firstprivate(ch1i, ch4i) &
  !$acc copyout(ch1o, ch4o)
  i1o = i1i
  i2o = i2i
  i4o = i4i
  i8o = i8i
  i16o = i16i

  l1o = l1i
  l2o = l2i
  l4o = l4i
  l8o = l8i
  l16o = l16i

  r4o = r4i
  r8o = r8i

  c4o = c4i
  c8o = c8i

  ch1o = ch1i
  ch4o = ch4i
  !$acc end parallel

  if (i1i /= i1o) call abort
  if (i2i /= i2o) call abort
  if (i4i /= i4o) call abort
  if (i8i /= i8o) call abort
  if (i16i /= i16o) call abort

  if (l1i .neqv. l1o) call abort
  if (l2i .neqv. l2o) call abort
  if (l4i .neqv. l4o) call abort
  if (l8i .neqv. l8o) call abort
  if (l16i .neqv. l16o) call abort

  if (r4i /= r4o) call abort
  if (r8i /= r8o) call abort

  if (c4i /= c4o) call abort
  if (c8i /= c8o) call abort

  if (ch1i /= ch1o) call abort
  if (ch4i /= ch4o) call abort

  call subtest(i1i, i2i, i4i, i8i, i16i, i1o, i2o, i4o, i8o, i16o, &
               l1i, l2i, l4i, l8i, l16i, l1o, l2o, l4o, l8o, l16o, &
               r4i, r8i, r4o, r8o, c4i, c8i, c4o, c8o, &
               ch1i, ch4i, ch1o, ch4o)
end program test

subroutine subtest(i1i, i2i, i4i, i8i, i16i, i1o, i2o, i4o, i8o, i16o, &
                   l1i, l2i, l4i, l8i, l16i, l1o, l2o, l4o, l8o, l16o, &
                   r4i, r8i, r4o, r8o, c4i, c8i, c4o, c8o, &
                   ch1i, ch4i, ch1o, ch4o)
  implicit none

  integer (kind=1)  :: i1i, i1o
  integer (kind=2)  :: i2i, i2o
  integer (kind=4)  :: i4i, i4o
  integer (kind=8)  :: i8i, i8o
  integer (kind=16) :: i16i, i16o

  logical (kind=1)  :: l1i, l1o
  logical (kind=2)  :: l2i, l2o
  logical (kind=4)  :: l4i, l4o
  logical (kind=8)  :: l8i, l8o
  logical (kind=16) :: l16i, l16o

  real (kind=4)  :: r4i, r4o
  real (kind=8)  :: r8i, r8o

  complex (kind=4)  :: c4i, c4o
  complex (kind=8)  :: c8i, c8o

  character (kind=1) :: ch1i, ch1o
  character (kind=4) :: ch4i, ch4o

  i1i = -i1i
  i2i = -i2i
  i4i = -i4i
  i8i = -i8i
  i16i = -i16i

  l1i = .not. l1i
  l2i = .not. l2i
  l4i = .not. l4i
  l8i = .not. l8i
  l16i = .not. l16i

  r4i = -r4i
  r8i = -r8i

  c4i = -c4i
  c8i = -c8i

  ch1i = "z"
  ch4i = "y"

  !$acc parallel firstprivate(i1i, i2i, i4i, i8i, i16i) &
  !$acc copyout(i1o, i2o, i4o, i8o, i16o) &
  !$acc firstprivate(l1i, l2i, l4i, l8i, l16i) &
  !$acc copyout(l1o, l2o, l4o, l8o, l16o) &
  !$acc firstprivate(r4i, r8i) copyout(r4o, r8o) &
  !$acc firstprivate(c4i, c8i) copyout(c4o, c8o) &
  !$acc firstprivate(ch1i, ch4i) &
  !$acc copyout(ch1o, ch4o)
  i1o = i1i
  i2o = i2i
  i4o = i4i
  i8o = i8i
  i16o = i16i

  l1o = l1i
  l2o = l2i
  l4o = l4i
  l8o = l8i
  l16o = l16i

  r4o = r4i
  r8o = r8i

  c4o = c4i
  c8o = c8i

  ch1o = ch1i
  ch4o = ch4i
  !$acc end parallel

  if (i1i /= i1o) call abort
  if (i2i /= i2o) call abort
  if (i4i /= i4o) call abort
  if (i8i /= i8o) call abort
  if (i16i /= i16o) call abort

  if (l1i .neqv. l1o) call abort
  if (l2i .neqv. l2o) call abort
  if (l4i .neqv. l4o) call abort
  if (l8i .neqv. l8o) call abort
  if (l16i .neqv. l16o) call abort

  if (r4i /= r4o) call abort
  if (r8i /= r8o) call abort

  if (c4i /= c4o) call abort
  if (c8i /= c8o) call abort

  if (ch1i /= ch1o) call abort
  if (ch4i /= ch4o) call abort
end subroutine subtest
