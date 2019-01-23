! Test that a user can override the compiler's "avoid offloading" decision.

! { dg-do run }
! { dg-additional-options "-cpp" }
! { dg-additional-options "-fopt-info-optimized-omp" }
!     Override the compiler's "avoid offloading" decision.
! { dg-additional-options "-foffload-force" }

      IMPLICIT NONE
      INCLUDE "openacc_lib.h"

      INTEGER :: D
      INTEGER :: I
      INTEGER, VOLATILE :: X
      LOGICAL :: Y

!$ACC DATA COPYOUT(X, Y)
!$ACC KERNELS
      X = 33 ! { dg-warning "note: beginning .gang-single. region in OpenACC .kernels. construct" }
      Y = ACC_ON_DEVICE (ACC_DEVICE_HOST)
!$ACC END KERNELS
!$ACC END DATA

      ! The following would trigger "avoid offloading".
!$ACC KERNELS
!$ACC LOOP AUTO ! { dg-warning "note: forwarded loop nest in OpenACC .kernels. construct to .parloops. for analysis" }
! { dg-warning "note: assigned OpenACC seq loop parallelism" "" { target *-*-* } 26 }
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
