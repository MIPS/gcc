! Test that a user can override the compiler's "avoid offloading" decision.

! { dg-do run }
! { dg-additional-options "-cpp" }
!     Override the compiler's "avoid offloading" decision.
! { dg-additional-options "-foffload-force" }

      IMPLICIT NONE
      INCLUDE "openacc_lib.h"

      INTEGER :: D
      INTEGER, VOLATILE :: X
      LOGICAL :: Y

!$ACC DATA COPYOUT(X, Y)
!$ACC KERNELS
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
