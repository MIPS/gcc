! { dg-do compile }

      IMPLICIT NONE

      INTEGER X

!$ACC PARALLEL DTYPE (NVIDIA) NUM_GANGS(10) COPYOUT(X) ! { dg-error "Unclassifiable OpenACC directive" }
      X = 0
!$ACC END PARALLEL ! { dg-error "Unexpected" }

      END
