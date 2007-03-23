; This file contains MIPS instructions that support fixed-point operations.

; All supported fixed-point modes
(define_mode_macro FIXED [(QQ "") (HQ "") (SQ "") (DQ "TARGET_64BIT")
			  (UQQ "") (UHQ "") (USQ "") (UDQ "TARGET_64BIT")
			  (HA "") (SA "") (DA "TARGET_64BIT")
			  (UHA "") (USA "") (UDA "TARGET_64BIT")])

; For signed add/sub with saturation
(define_mode_macro ADDSUB [(HQ "") (SQ "") (HA "") (SA "") (V2HQ "") (V2HA "")])
(define_mode_attr addsubfmt [(HQ "ph") (SQ "w") (HA "ph") (SA "w")
			     (V2HQ "ph") (V2HA "ph")])

; For unsigned add/sub with saturation
(define_mode_macro UADDSUB [(UQQ "TARGET_DSP") (UHQ "TARGET_DSPR2")
			    (UHA "TARGET_DSPR2") (V4UQQ "TARGET_DSP")
			    (V2UHQ "TARGET_DSPR2") (V2UHA "TARGET_DSPR2")])
(define_mode_attr uaddsubfmt [(UQQ "qb") (UHQ "ph") (UHA "ph")])

; For signed multiplication with saturation
(define_mode_macro MULQ [(V2HQ "TARGET_DSP") (HQ "TARGET_DSP")
			 (SQ "TARGET_DSPR2")])
(define_mode_attr mulqfmt [(V2HQ "ph") (HQ "ph") (SQ "w")])

; In GPR templates, a string like "<dd>subu" will expand to "subu" in the
; 32-bit version and "dsubu" in the 64-bit version.
(define_mode_attr dd [(QQ "") (HQ "") (SQ "") (DQ "d")
		      (UQQ "") (UHQ "") (USQ "") (UDQ "d")
		      (HA "") (SA "") (DA "d")
		      (UHA "") (USA "") (UDA "d")])

; The integer mode has the same size as the fixed-point mode.
(define_mode_attr imode [(QQ "QI") (HQ "HI") (SQ "SI") (DQ "DI")
			 (UQQ "QI") (UHQ "HI") (USQ "SI") (UDQ "DI")
			 (HA "HI") (SA "SI") (DA "DI")
			 (UHA "HI") (USA "SI") (UDA "DI")
			 (V4UQQ "SI") (V2UHQ "SI") (V2UHA "SI")
			 (V2HQ "SI") (V2HA "SI")])

(define_insn "add<mode>3"
  [(set (match_operand:FIXED 0 "register_operand" "=d")
	(plus:FIXED (match_operand:FIXED 1 "register_operand" "d")
		    (match_operand:FIXED 2 "register_operand" "d")))]
  ""
  "<dd>addu\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "<imode>")])

(define_insn "usadd<mode>3"
  [(parallel
    [(set (match_operand:UADDSUB 0 "register_operand" "=d")
	  (us_plus:UADDSUB (match_operand:UADDSUB 1 "register_operand" "d")
			   (match_operand:UADDSUB 2 "register_operand" "d")))
     (set (reg:CCDSP CCDSP_OU_REGNUM)
	  (unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_ADDQ_S))])]
  ""
  "addu_s.<uaddsubfmt>\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "<imode>")])

(define_insn "ssadd<mode>3"
  [(parallel
    [(set (match_operand:ADDSUB 0 "register_operand" "=d")
	  (ss_plus:ADDSUB (match_operand:ADDSUB 1 "register_operand" "d")
			  (match_operand:ADDSUB 2 "register_operand" "d")))
     (set (reg:CCDSP CCDSP_OU_REGNUM)
	  (unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_ADDQ_S))])]
  "TARGET_DSP"
  "addq_s.<addsubfmt>\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "<imode>")])

(define_insn "sub<mode>3"
  [(set (match_operand:FIXED 0 "register_operand" "=d")
        (minus:FIXED (match_operand:FIXED 1 "register_operand" "d")
		     (match_operand:FIXED 2 "register_operand" "d")))]
  ""
  "<dd>subu\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "<imode>")])

(define_insn "ussub<mode>3"
  [(parallel
    [(set (match_operand:UADDSUB 0 "register_operand" "=d")
	  (us_minus:UADDSUB (match_operand:UADDSUB 1 "register_operand" "d")
			    (match_operand:UADDSUB 2 "register_operand" "d")))
     (set (reg:CCDSP CCDSP_OU_REGNUM)
	  (unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_SUBQ_S))])]
  ""
  "subu_s.<uaddsubfmt>\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "<imode>")])

(define_insn "sssub<mode>3"
  [(parallel
    [(set (match_operand:ADDSUB 0 "register_operand" "=d")
	  (ss_minus:ADDSUB (match_operand:ADDSUB 1 "register_operand" "d")
			   (match_operand:ADDSUB 2 "register_operand" "d")))
     (set (reg:CCDSP CCDSP_OU_REGNUM)
	  (unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_SUBQ_S))])]
  "TARGET_DSP"
  "subq_s.<addsubfmt>\t%0,%1,%2"
  [(set_attr "type" "arith")
   (set_attr "mode" "<imode>")])

(define_insn "ssmul<mode>3"
  [(parallel
    [(set (match_operand:MULQ 0 "register_operand" "=d")
          (ss_mult:MULQ (match_operand:MULQ 1 "register_operand" "d")
			(match_operand:MULQ 2 "register_operand" "d")))
     (set (reg:CCDSP CCDSP_OU_REGNUM)
          (unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_MULQ_RS_PH))
     (clobber (match_scratch:DI 3 "=x"))])]
  ""
  "mulq_rs.<mulqfmt>\t%0,%1,%2"
  [(set_attr "type"     "imul3")
   (set_attr "mode"     "<imode>")])

