! { dg-do compile }
! { dg-prune-output "sorry, unimplemented: device_type clause is not supported yet" }

      IMPLICIT NONE

      INTEGER X

!$ACC PARALLEL DTYPE (NVIDIA) NUM_GANGS(10) COPYOUT(X) ! { dg-error "Unclassifiable OpenACC directive" }
      X = 0
!$ACC END PARALLEL ! { dg-error "Unexpected" }

      END
