! Test that a user can override the compiler's "avoid offloading" decision.

! { dg-do run }
! { dg-additional-options "-cpp" }
! The warning is only triggered for -O2 and higher.
! { dg-xfail-if "n/a" { openacc_nvidia_accel_selected } { "-O0" "-O1" } { "" } }

      IMPLICIT NONE
      INCLUDE "openacc_lib.h"

      INTEGER :: D
      INTEGER, VOLATILE :: X
      LOGICAL :: Y

!     Override the compiler's "avoid offloading" decision.
#if defined ACC_DEVICE_TYPE_nvidia
      D = ACC_DEVICE_NVIDIA
#elif defined ACC_DEVICE_TYPE_host
      D = ACC_DEVICE_HOST
#else
# error Not ported to this ACC_DEVICE_TYPE
#endif
      CALL ACC_INIT (D)

!$ACC DATA COPYOUT(X, Y)
!$ACC KERNELS /* { dg-warning "OpenACC kernels construct will be executed sequentially; will by default avoid offloading to prevent data copy penalty" "" { target openacc_nvidia_accel_selected } } */
      X = 33
      Y = ACC_ON_DEVICE (ACC_DEVICE_HOST)
!$ACC END KERNELS
!$ACC END DATA

      IF (X .NE. 33) CALL ABORT
#if defined ACC_DEVICE_TYPE_nvidia
      IF (Y) CALL ABORT
#else
      IF (.NOT. Y) CALL ABORT
#endif

      END
