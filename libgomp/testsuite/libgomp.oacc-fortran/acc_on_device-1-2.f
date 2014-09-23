! TODO: Remove -DACC_DEVICE_TYPE_host once that is set by the test harness.
! { dg-additional-options "-cpp -DACC_DEVICE_TYPE_host" }
! TODO: Have to disable the acc_on_device builtin for we want to test
! the libgomp library function?  The command line option
! '-fno-builtin-acc_on_device' is valid for C/C++/ObjC/ObjC++ but not
! for Fortran.

      USE OPENACC
      IMPLICIT NONE

!Host.

      IF (.NOT. ACC_ON_DEVICE (ACC_DEVICE_NONE)) CALL ABORT
      IF (.NOT. ACC_ON_DEVICE (ACC_DEVICE_HOST)) CALL ABORT
      IF (ACC_ON_DEVICE (ACC_DEVICE_NOT_HOST)) CALL ABORT


!Host via offloading fallback mode.

!$ACC PARALLEL IF(.FALSE.)
      IF (.NOT. ACC_ON_DEVICE (ACC_DEVICE_NONE)) CALL ABORT
      IF (.NOT. ACC_ON_DEVICE (ACC_DEVICE_HOST)) CALL ABORT
      IF (ACC_ON_DEVICE (ACC_DEVICE_NOT_HOST)) CALL ABORT
!$ACC END PARALLEL


#if !ACC_DEVICE_TYPE_host

! Offloaded.

!$ACC PARALLEL
      IF (ACC_ON_DEVICE (ACC_DEVICE_NONE)) CALL ABORT
      IF (ACC_ON_DEVICE (ACC_DEVICE_HOST)) CALL ABORT
      IF (.NOT. ACC_ON_DEVICE (ACC_DEVICE_NOT_HOST)) CALL ABORT
!$ACC END PARALLEL

#endif

      END
