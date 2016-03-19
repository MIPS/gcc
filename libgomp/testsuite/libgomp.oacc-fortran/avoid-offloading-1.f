! Test that the compiler decides to "avoid offloading".

! { dg-do run }
! { dg-additional-options "-cpp" }
! The warning is only triggered for -O2 and higher.
! { dg-xfail-if "n/a" { openacc_nvidia_accel_selected } { "-O0" "-O1" } { "" } }

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
#if defined ACC_DEVICE_TYPE_host || defined ACC_DEVICE_TYPE_nvidia
      IF (.NOT. Y) CALL ABORT
#else
# error Not ported to this ACC_DEVICE_TYPE
#endif

      END
