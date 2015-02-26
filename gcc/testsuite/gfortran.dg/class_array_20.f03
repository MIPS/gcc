! {dg-do run}
!
! Test contributed by Thomas L. Clune via pr60322
!                  and Antony Lewis via pr64692

program class_array_20
  implicit none

  type Foo
  end type

  type(foo), dimension(2:3) :: arg
  double precision P(2,2)

  ! Checking for PR/60322
  call copyFromClassArray([Foo(), Foo()])
  call copyFromClassArray(arg)
  call copyFromClassArray(arg(:))

  ! Checking for PR/64692
  P(1:2, 1)=[1.d0,2.d0]
  P(1:2,2) = [3.d0,4.d0]
  call AddArray(P(1:2,2))
contains

  subroutine copyFromClassArray(classarray)
    class (Foo), intent(in) :: classarray(:)

    if (lbound(classarray, 1) .ne. 1) call abort()
    if (ubound(classarray, 1) .ne. 2) call abort()
    if (size(classarray) .ne. 2) call abort()
  end subroutine

  subroutine AddArray(P)
    class(*), target, intent(in) :: P(:)
    class(*), pointer :: Pt(:)

    allocate(Pt(1:size(P)), source= P)

    select type (P)
      type is (double precision)
        if (abs(P(1)-3.d0) .gt. 1.d-8) call abort()
        if (abs(P(2)-4.d0) .gt. 1.d-8) call abort()
    end select

    select type (Pt)
      type is (double precision)
        if (abs(Pt(1)-3.d0) .gt. 1.d-8) call abort()
        if (abs(Pt(2)-4.d0) .gt. 1.d-8) call abort()
    end select
  end subroutine
end program class_array_20

