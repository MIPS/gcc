      SUBROUTINE sub_1
      IMPLICIT NONE
!$ACC ROUTINE (ABORT) SEQ VECTOR ! { dg-error "Intrinsic symbol specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible clauses specifying the level of parallelism" }

      CALL ABORT
      END SUBROUTINE sub_1

      MODULE m_w_1
      IMPLICIT NONE
!$ACC ROUTINE (ABORT) VECTOR GANG ! { dg-error "Intrinsic symbol specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible clauses specifying the level of parallelism" }

      CONTAINS
      SUBROUTINE sub_2
      CALL ABORT
      END SUBROUTINE sub_2
      END MODULE m_w_1
