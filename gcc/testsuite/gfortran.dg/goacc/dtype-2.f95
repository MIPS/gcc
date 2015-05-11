! { dg-do compile }
! { dg-prune-output "sorry, unimplemented: device_type clause is not supported yet" }

program dtype
  integer i1, i2, i3, i4, i5, i6

!! ACC PARALLEL DEVICE_TYPE:

!$acc parallel device_type (nvidia) async (1) num_gangs (100) &
!$acc&  num_workers (100) vector_length (32) wait (1) copy (i1)
!$acc end parallel

!! ACC KERNELS DEVICE_TYPE:

!$acc kernels dtype (nvidia) async wait copy (i1)
!$acc end kernels

!! ACC LOOP DEVICE_TYPE:

!$acc parallel
!$acc loop dtype (nvidia) gang tile (1) private (i1)
  do i1 = 1, 10
  end do
!$acc end parallel

!! ACC UPDATE:

!$acc update host(i1) device_type(nvidia) async(2) wait (2) self(i2)

end program dtype

! { dg-error "Unclassifiable OpenACC directive" "" { target *-*-* } 9 }
! { dg-error "Unexpected" "" { target *-*-* } 11 }

! { dg-error "Unclassifiable OpenACC directive" "" { target *-*-* } 15 }
! { dg-error "Unexpected" "" { target *-*-* } 16 }

! { dg-error "Unclassifiable OpenACC directive" "" { target *-*-* } 21 }

! { dg-error "Unclassifiable OpenACC directive" "" { target *-*-* } 28 }
