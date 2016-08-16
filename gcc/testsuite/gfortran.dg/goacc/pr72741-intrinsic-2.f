! Check for invalid clauses with intrinsic function specified in !$ACC ROUTINE ( NAME ).

      SUBROUTINE sub_1
      IMPLICIT NONE
!$ACC ROUTINE (ABORT) WORKER ! { dg-error "Intrinsic symbol specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible clauses specifying the level of parallelism" }
!$ACC ROUTINE (ABORT) GANG ! { dg-error "Intrinsic symbol specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible clauses specifying the level of parallelism" }
!$ACC ROUTINE (ABORT) VECTOR ! { dg-error "Intrinsic symbol specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible clauses specifying the level of parallelism" }

      CALL ABORT
      END SUBROUTINE sub_1

      MODULE m_w_1
      IMPLICIT NONE
!$ACC ROUTINE (ABORT) VECTOR ! { dg-error "Intrinsic symbol specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible clauses specifying the level of parallelism" }
!$ACC ROUTINE (ABORT) WORKER ! { dg-error "Intrinsic symbol specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible clauses specifying the level of parallelism" }
!$ACC ROUTINE (ABORT) GANG ! { dg-error "Intrinsic symbol specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible clauses specifying the level of parallelism" }

      CONTAINS
      SUBROUTINE sub_2
      CALL ABORT
      END SUBROUTINE sub_2
      END MODULE m_w_1
