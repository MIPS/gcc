; Generate all FAKE instructions for all fixed-point modes

; All fixed-point modes
(define_mode_macro FIXED [(QQ "") (HQ "") (SQ "") (DQ "") (TQ "") (UQQ "") (UHQ "") (USQ "") (UDQ "") (UTQ "") (HA "") (SA "") (DA "") (TA "") (UHA "") (USA "") (UDA "") (UTA "")])

; All signed fixed-point modes
(define_mode_macro S_FIXED [(QQ "") (HQ "") (SQ "") (DQ "") (TQ "") (HA "") (SA "") (DA "") (TA "")])

; All unsigned fixed-point modes
(define_mode_macro U_FIXED [(UQQ "") (UHQ "") (USQ "") (UDQ "") (UTQ "") (UHA "") (USA "") (UDA "") (UTA "")])

; All signed and unsigned operators
(define_code_macro bin_op [plus minus mult])
(define_code_macro s_bin_op [ss_plus ss_minus ss_mult div ss_div])
(define_code_macro u_bin_op [us_plus us_minus us_mult udiv us_div])
(define_code_macro unary_op [neg])
(define_code_macro s_unary_op [ss_neg])
(define_code_macro u_unary_op [us_neg])
(define_code_macro shift_op [ashift])
(define_code_macro s_shift_op [ashiftrt ss_ashift])
(define_code_macro u_shift_op [lshiftrt us_ashift])
(define_code_macro fpcond [ne eq])
(define_code_macro s_fpcond [ge gt le lt])
(define_code_macro u_fpcond [geu gtu leu ltu])

; All instruction names
(define_code_attr myinsn [(plus "add")
                          (minus "sub")
                          (mult "mul")
                          (ss_plus "ssadd")
                          (ss_minus "sssub")
                          (ss_mult "ssmul")
                          (div "div")
                          (ss_div "ssdiv")
                          (us_plus "usadd")
                          (us_minus "ussub")
                          (us_mult "usmul")
                          (udiv "udiv")
                          (us_div "usdiv")
                          (neg "neg")
                          (ss_neg "ssneg")
			  (us_neg "usneg")
			  (ashift "ashl")
			  (ashiftrt "ashr")
			  (ss_ashift "ssashl")
			  (lshiftrt "lshr")
			  (us_ashift "usashl")
			  (ne "ne")
			  (eq "eq")
			  (ge "ge")
			  (gt "gt")
			  (le "le")
			  (lt "lt")
			  (geu "geu")
			  (gtu "gtu")
			  (leu "leu")
			  (ltu "ltu")
])

(define_insn "<myinsn><mode>3"
  [(set (match_operand:FIXED 0 "register_operand" "=d")
        (bin_op:FIXED (match_operand:FIXED 1 "register_operand" "d")
		      (match_operand:FIXED 2 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1,%2"
  [(set_attr "type" "arith")])

(define_insn "<myinsn><mode>3"
  [(set (match_operand:S_FIXED 0 "register_operand" "=d")
        (s_bin_op:S_FIXED (match_operand:S_FIXED 1 "register_operand" "d")
			   (match_operand:S_FIXED 2 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1,%2"
  [(set_attr "type" "arith")])

(define_insn "<myinsn><mode>3"
  [(set (match_operand:U_FIXED 0 "register_operand" "=d")
        (u_bin_op:U_FIXED (match_operand:U_FIXED 1 "register_operand" "d")
			   (match_operand:U_FIXED 2 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1,%2"
  [(set_attr "type" "arith")])

(define_insn "<myinsn><mode>2"
  [(set (match_operand:FIXED 0 "register_operand" "=d")
        (unary_op:FIXED (match_operand:FIXED 1 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1"
  [(set_attr "type" "arith")])

(define_insn "<myinsn><mode>2"
  [(set (match_operand:S_FIXED 0 "register_operand" "=d")
        (s_unary_op:S_FIXED (match_operand:S_FIXED 1 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1"
  [(set_attr "type" "arith")])

(define_insn "<myinsn><mode>2"
  [(set (match_operand:U_FIXED 0 "register_operand" "=d")
        (u_unary_op:U_FIXED (match_operand:U_FIXED 1 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1"
  [(set_attr "type" "arith")])

(define_insn "<myinsn><mode>3"
  [(set (match_operand:FIXED 0 "register_operand" "=d")
        (shift_op:FIXED (match_operand:FIXED 1 "register_operand" "d")
			(match_operand:SI 2 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1,%2"
  [(set_attr "type" "shift")])

(define_insn "<myinsn><mode>3"
  [(set (match_operand:S_FIXED 0 "register_operand" "=d")
        (s_shift_op:S_FIXED (match_operand:S_FIXED 1 "register_operand" "d")
			    (match_operand:SI 2 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1,%2"
  [(set_attr "type" "shift")])

(define_insn "<myinsn><mode>3"
  [(set (match_operand:U_FIXED 0 "register_operand" "=d")
        (u_shift_op:U_FIXED (match_operand:U_FIXED 1 "register_operand" "d")
			    (match_operand:SI 2 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1,%2"
  [(set_attr "type" "shift")])

(define_expand "cmp<mode>"
  [(set (cc0)
        (compare:CC (match_operand:FIXED 0 "register_operand")
                    (match_operand:FIXED 1 "register_operand")))]
  ""
{
  cmp_operands[0] = operands[0];
  cmp_operands[1] = operands[1];
  DONE;
})

(define_insn "*<myinsn><mode>"
  [(set (reg:CCDSP CCDSP_CC_REGNUM)
        (fpcond:CCDSP (match_operand:FIXED 0 "register_operand" "d")
		      (match_operand:FIXED 1 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1"
  [(set_attr "type" "arith")])

(define_insn "*<myinsn><mode>"
  [(set (reg:CCDSP CCDSP_CC_REGNUM)
        (s_fpcond:CCDSP (match_operand:S_FIXED 0 "register_operand" "d")
		        (match_operand:S_FIXED 1 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1"
  [(set_attr "type" "arith")])

(define_insn "*<myinsn><mode>"
  [(set (reg:CCDSP CCDSP_CC_REGNUM)
        (u_fpcond:CCDSP (match_operand:U_FIXED 0 "register_operand" "d")
		        (match_operand:U_FIXED 1 "register_operand" "d")))]
  ""
  "<myinsn>_<mode>\t%0,%1"
  [(set_attr "type" "arith")])

(define_insn "*fpbranch"
  [(set (pc)
        (if_then_else
         (ne (reg:CCDSP CCDSP_CC_REGNUM)
	     (match_operand:SI 0 "immediate_operand" "I"))
         (label_ref (match_operand 1 "" ""))
         (pc)))]
  ""
  "%*fpbranch\t%1%/"
  [(set_attr "type" "branch")
   (set_attr "mode" "none")])

