! Test that the compiler decides to "avoid offloading".

! { dg-do run }
! { dg-additional-options "-cpp" }
! { dg-additional-options "-fopt-info-optimized-omp" }
! As __OPTIMIZE__ is defined for -O1 and higher, we don't have an (easy) way to
! distinguish -O1 (where we will offload) from -O2 (where we won't offload), so
! for -O1 testing, we expect to abort.
! { dg-xfail-run-if "" { openacc_nvidia_accel_selected } { "-O1" } { "" } }

      IMPLICIT NONE
      INCLUDE "openacc_lib.h"

      INTEGER :: I
      INTEGER, VOLATILE :: X
      LOGICAL :: Y

!$ACC DATA COPYOUT(X, Y)
!$ACC KERNELS
      X = 33 ! { dg-warning "note: beginning .gang-single. region in OpenACC .kernels. construct" }
      Y = ACC_ON_DEVICE (ACC_DEVICE_HOST);
!$ACC END KERNELS
!$ACC END DATA

      ! The following will trigger "avoid offloading".
!$ACC KERNELS
!$ACC LOOP AUTO ! { dg-warning "note: forwarded loop nest in OpenACC .kernels. construct to .parloops. for analysis" }
! { dg-warning "OpenACC kernels construct will be executed sequentially; will by default avoid offloading to prevent data copy penalty" "" { target { openacc_nvidia_accel_selected && opt_levels_2_plus } } 27 }
! { dg-warning "note: assigned OpenACC seq loop parallelism" "" { target *-*-* } 27 }
      DO I = 1, X
         IF (X .EQ. 0) THEN
            X = 1
         END IF
      END DO
!$ACC END KERNELS

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
