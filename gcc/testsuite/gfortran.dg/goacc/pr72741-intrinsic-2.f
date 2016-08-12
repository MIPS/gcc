! Check for invalid clauses with intrinsic function specified in !$ACC ROUTINE ( NAME ).

      SUBROUTINE sub_1
      IMPLICIT NONE
!$ACC ROUTINE (ABORT) WORKER ! { dg-error "Intrinsic function specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible GANG, WORKER, or VECTOR clause" "" { xfail *-*-* } }
! { dg-bogus "invalid function name abort" "" { xfail *-*-* } .-1 }
!$ACC ROUTINE (ABORT) GANG ! { dg-error "Intrinsic function specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible GANG, WORKER, or VECTOR clause" "" { xfail *-*-* } }
! { dg-bogus "invalid function name abort" "" { xfail *-*-* } .-1 }
!$ACC ROUTINE (ABORT) VECTOR ! { dg-error "Intrinsic function specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible GANG, WORKER, or VECTOR clause" "" { xfail *-*-* } }
! { dg-bogus "invalid function name abort" "" { xfail *-*-* } .-1 }

      CALL ABORT
      END SUBROUTINE sub_1

      MODULE m_w_1
      IMPLICIT NONE
!$ACC ROUTINE (ABORT) VECTOR ! { dg-error "Intrinsic function specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible GANG, WORKER, or VECTOR clause" "" { xfail *-*-* } }
! { dg-bogus "invalid function name abort" "" { xfail *-*-* } .-1 }
!$ACC ROUTINE (ABORT) WORKER ! { dg-error "Intrinsic function specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible GANG, WORKER, or VECTOR clause" "" { xfail *-*-* } }
! { dg-bogus "invalid function name abort" "" { xfail *-*-* } .-1 }
!$ACC ROUTINE (ABORT) GANG ! { dg-error "Intrinsic function specified in \\!\\\$ACC ROUTINE \\( NAME \\) at \\(1\\), with incompatible GANG, WORKER, or VECTOR clause" "" { xfail *-*-* } }
! { dg-bogus "invalid function name abort" "" { xfail *-*-* } .-1 }

      CONTAINS
      SUBROUTINE sub_2
      CALL ABORT
      END SUBROUTINE sub_2
      END MODULE m_w_1
