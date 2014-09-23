! TODO: Remove -DACC_DEVICE_TYPE_host once that is set by the test harness.
! { dg-additional-options "-cpp -DACC_DEVICE_TYPE_host" }
! TODO: Have to disable the acc_on_device builtin for we want to test the
! libgomp library function?  The command line option
! '-fno-builtin-acc_on_device' is valid for C/C++/ObjC/ObjC++ but not for
! Fortran.

use openacc
implicit none

! Host.

if (.not. acc_on_device (acc_device_none)) call abort
if (.not. acc_on_device (acc_device_host)) call abort
if (acc_on_device (acc_device_not_host)) call abort


! Host via offloading fallback mode.

!$acc parallel if(.false.)
if (.not. acc_on_device (acc_device_none)) call abort
if (.not. acc_on_device (acc_device_host)) call abort
if (acc_on_device (acc_device_not_host)) call abort
!$acc end parallel


#if !ACC_DEVICE_TYPE_host

! Offloaded.

!$acc parallel
if (acc_on_device (acc_device_none)) call abort
if (acc_on_device (acc_device_host)) call abort
if (.not. acc_on_device (acc_device_not_host)) call abort
!$acc end parallel

#endif

end
