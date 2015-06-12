! { dg-do compile }

program dtype
  integer i1, i2, i3, i4, i5, i6

!! ACC PARALLEL DEVICE_TYPE:

!$acc parallel device_type (nVidia) async (1) num_gangs (100) &
!$acc&  num_workers (100) vector_length (32) wait (1) copy (i1)
!$acc end parallel

!! ACC KERNELS DEVICE_TYPE:

!$acc kernels dtype (nvidia) async wait copy (i1)
!$acc end kernels

!! ACC LOOP DEVICE_TYPE:

!$acc parallel
!$acc loop dtype (nVidia) gang tile (1) private (i1)
  do i1 = 1, 10
  end do
!$acc end parallel

!! ACC UPDATE:

!$acc update host(i1) device_type(nvidia) async(2) wait (2) self(i2)

end program dtype

! { dg-error "Unclassifiable OpenACC directive" "" { target *-*-* } 8 }
! { dg-error "Unexpected" "" { target *-*-* } 10 }

! { dg-error "Unclassifiable OpenACC directive" "" { target *-*-* } 14 }
! { dg-error "Unexpected" "" { target *-*-* } 15 }

! { dg-error "Unclassifiable OpenACC directive" "" { target *-*-* } 20 }

! { dg-error "Unclassifiable OpenACC directive" "" { target *-*-* } 27 }
