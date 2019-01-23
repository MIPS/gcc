! Test that a user can override the compiler's "avoid offloading" decision.

! { dg-do run }
! { dg-additional-options "-cpp" }
! { dg-additional-options "-fopt-info-optimized-omp" }

      IMPLICIT NONE
      INCLUDE "openacc_lib.h"

      INTEGER :: D
      INTEGER :: I
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
!$ACC KERNELS
      X = 33 ! { dg-warning "note: beginning .gang-single. region in OpenACC .kernels. construct" }
      Y = ACC_ON_DEVICE (ACC_DEVICE_HOST)
!$ACC END KERNELS
!$ACC END DATA

      ! The following will trigger "avoid offloading".
!$ACC KERNELS
!$ACC LOOP AUTO ! { dg-warning "note: forwarded loop nest in OpenACC .kernels. construct to .parloops. for analysis" }
! { dg-warning "OpenACC kernels construct will be executed sequentially; will by default avoid offloading to prevent data copy penalty" "" { target { openacc_nvidia_accel_selected && opt_levels_2_plus } } 34 }
! { dg-warning "note: assigned OpenACC seq loop parallelism" "" { target *-*-* } 34 }
      DO I = 1, X
         IF (X .EQ. 0) THEN
            X = 1
         END IF
      END DO
!$ACC END KERNELS

      IF (X .NE. 33) CALL ABORT
#if defined ACC_DEVICE_TYPE_nvidia
      IF (Y) CALL ABORT
#else
      IF (.NOT. Y) CALL ABORT
#endif

      END
