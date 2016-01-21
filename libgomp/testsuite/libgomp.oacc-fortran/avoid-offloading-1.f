! Test that the compiler decides to "avoid offloading".

! { dg-do run }
! { dg-additional-options "-cpp" }
! The warning is only triggered for -O2 and higher.
! { dg-xfail-if "n/a" { openacc_nvidia_accel_selected } { "-O0" "-O1" } { "" } }
! As __OPTIMIZE__ is defined for -O1 and higher, we don't have an (easy) way to
! distinguish -O1 (where we will offload) from -O2 (where we won't offload), so
! for -O1 testing, we expect to abort.
! { dg-xfail-run-if "" { openacc_nvidia_accel_selected } { "-O1" } { "" } }

      IMPLICIT NONE
      INCLUDE "openacc_lib.h"

      INTEGER, VOLATILE :: X
      LOGICAL :: Y

!$ACC DATA COPYOUT(X, Y)
!$ACC KERNELS /* { dg-warning "OpenACC kernels construct will be executed sequentially; will by default avoid offloading to prevent data copy penalty" "" { target openacc_nvidia_accel_selected } } */
      X = 33
      Y = ACC_ON_DEVICE (ACC_DEVICE_HOST);
!$ACC END KERNELS
!$ACC END DATA

      IF (X .NE. 33) CALL ABORT
#if defined ACC_DEVICE_TYPE_nvidia
# if !defined __OPTIMIZE__
      IF (Y) CALL ABORT
# else
      IF (.NOT. Y) CALL ABORT
# endif
#elif defined ACC_DEVICE_TYPE_host
      IF (.NOT. Y) CALL ABORT
#else
# error Not ported to this ACC_DEVICE_TYPE
#endif

      END
