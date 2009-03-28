;; VSX patterns.
;; Copyright (C) 2009
;; Free Software Foundation, Inc.
;; Contributed by Michael Meissner <meissner@linux.vnet.ibm.com>

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; Iterator for both scalar and vector floating point types supported by VSX
(define_mode_iterator VSX_B [DF V4SF V2DF])

;; Iterator for vector floating point types supported by VSX
(define_mode_iterator VSX_F [V4SF V2DF])

;; Iterator for logical types supported by VSX
(define_mode_iterator VSX_L [V16QI V8HI V4SI V2DI V4SF V2DF TI])

;; Map into the appropriate load/store name based on the type
(define_mode_attr VSm  [(V16QI "vw4")
			(V8HI  "vw4")
			(V4SI  "vw4")
			(V4SF  "vw4")
			(V2DF  "vd2")
			(V2DI  "vd2")
			(DF    "d")
			(TI    "vw4")])

;; Map into the appropriate suffix based on the type
(define_mode_attr VSs	[(V16QI "sp")
			 (V8HI  "sp")
			 (V4SI  "sp")
			 (V4SF  "sp")
			 (V2DF  "dp")
			 (V2DI  "dp")
			 (DF    "dp")
			 (TI    "sp")])

;; Map into the register class used
(define_mode_attr VSr	[(V16QI "v")
			 (V8HI  "v")
			 (V4SI  "v")
			 (V4SF  "wf")
			 (V2DI  "wd")
			 (V2DF  "wd")
			 (DF    "ws")
			 (TI    "wd")])

;; Map into the register class used for float<->int conversions
(define_mode_attr VSr2	[(V2DF  "wd")
			 (V4SF  "wf")
			 (DF    "!f#r")])

(define_mode_attr VSr3	[(V2DF  "wa")
			 (V4SF  "wa")
			 (DF    "!f#r")])

;; Same size integer type for floating point data
(define_mode_attr VSi [(V2DF  "v2di")
		       (V4SF  "v4si")
		       (DF    "di")])

(define_mode_attr VSI [(V2DF  "V2DI")
		       (V4SF  "V4SI")
		       (DF    "DI")])

;; Word size for same size conversion
(define_mode_attr VSc [(V2DF "d")
		       (V4SF "w")
		       (DF   "d")])

;; Bitsize for DF load with update
(define_mode_attr VSbit [(SI "32")
			 (DI "64")])

;; Map into either s or v, depending on whether this is a scalar or vector
;; operation
(define_mode_attr VSv	[(V2DF "v")
			 (V4SF "v")
			 (DF   "s")])

;; Appropriate type for add ops (and other simple FP ops)
(define_mode_attr VStype_simple	[(V2DF "vecfloat")
				 (V4SF "vecfloat")
				 (DF   "fp")])

(define_mode_attr VSfptype_simple [(V2DF "fp_addsub_d")
				   (V4SF "fp_addsub_s")
				   (DF   "fp_addsub_d")])

;; Appropriate type for multiply ops
(define_mode_attr VStype_mul	[(V2DF "vecfloat")
				 (V4SF "vecfloat")
				 (DF   "dmul")])

(define_mode_attr VSfptype_mul	[(V2DF "fp_mul_d")
				 (V4SF "fp_mul_s")
				 (DF   "fp_mul_d")])

;; Appropriate type for divide ops.  For now, just lump the vector divide with
;; the scalar divides
(define_mode_attr VStype_div	[(V2DF "ddiv")
				 (V4SF "sdiv")
				 (DF   "ddiv")])

(define_mode_attr VSfptype_div	[(V2DF "fp_div_d")
				 (V4SF "fp_div_s")
				 (DF   "fp_div_d")])

;; Appropriate type for sqrt ops.  For now, just lump the vector sqrt with
;; the scalar sqrt
(define_mode_attr VStype_sqrt	[(V2DF "dsqrt")
				 (V4SF "sdiv")
				 (DF   "ddiv")])

(define_mode_attr VSfptype_sqrt	[(V2DF "fp_sqrt_d")
				 (V4SF "fp_sqrt_s")
				 (DF   "fp_sqrt_d")])

;; Constants for creating unspecs
(define_constants
  [(UNSPEC_VSX_CONCAT_V2DF	500)
   (UNSPEC_VSX_XVCVDPSP		501)
   (UNSPEC_VSX_XVCVDPSXWS	502)
   (UNSPEC_VSX_XVCVDPUXWS	503)
   (UNSPEC_VSX_XVCVSPDP		504)
   (UNSPEC_VSX_XVCVSXWDP	505)
   (UNSPEC_VSX_XVCVUXWDP	506)
   (UNSPEC_VSX_XVMADD		507)
   (UNSPEC_VSX_XVMSUB		508)
   (UNSPEC_VSX_XVNMADD		509)
   (UNSPEC_VSX_XVNMSUB		510)
   (UNSPEC_VSX_XVRSQRTE		511)
   (UNSPEC_VSX_XVTDIV		512)
   (UNSPEC_VSX_XVTSQRT		513)])

;; VSX moves
(define_insn "*vsx_mov<mode>"
  [(set (match_operand:VSX_L 0 "nonimmediate_operand" "=Z,<VSr>,<VSr>,?Z,?wa,?wa,*o,*r,*r,<VSr>,?wa,v,wZ,v")
	(match_operand:VSX_L 1 "input_operand" "<VSr>,Z,<VSr>,wa,Z,wa,r,o,r,j,j,W,v,wZ"))]
  "VECTOR_MEM_VSX_P (<MODE>mode)
   && (register_operand (operands[0], <MODE>mode) 
       || register_operand (operands[1], <MODE>mode))"
{
  switch (which_alternative)
    {
    case 0:
    case 3:
      return "stx<VSm>%U0x %x1,%y0";

    case 1:
    case 4:
      return "lx<VSm>%U0x %x0,%y1";

    case 2:
    case 5:
      return "xxlor %x0,%x1,%x1";

    case 6:
    case 7:
    case 8:
      return "#";

    case 9:
    case 10:
      return "xxlxor %x0,%x0,%x0";

    case 11:
      return output_vec_const_move (operands);

    case 12:
      return "stvx %1,%y0";

    case 13:
      return "lvx %0,%y1";

    default:
      gcc_unreachable ();
    }
}
  [(set_attr "type" "vecstore,vecload,vecsimple,vecstore,vecload,vecsimple,store,load,*,vecsimple,vecsimple,*,vecstore,vecload")])

;; Load/store with update
;; Define insns that do load or store with update.  Because VSX only has
;; reg+reg addressing, pre-decrement or pre-inrement is unlikely to be
;; generated.
;;
;; In all these cases, we use operands 0 and 1 for the register being
;; incremented because those are the operands that local-alloc will
;; tie and these are the pair most likely to be tieable (and the ones
;; that will benefit the most).

(define_insn "*vsx_load<mode>_update64"
  [(set (match_operand:VSX_L 3 "vsx_register_operand" "=<VSr>,?wa")
	(mem:VSX_L (plus:DI (match_operand:DI 1 "gpc_reg_operand" "0,0")
			    (match_operand:DI 2 "gpc_reg_operand" "r,r"))))
   (set (match_operand:DI 0 "gpc_reg_operand" "=b,b")
	(plus:DI (match_dup 1)
		 (match_dup 2)))]
  "TARGET_64BIT && TARGET_UPDATE && VECTOR_MEM_VSX_P (<MODE>mode)"
  "lx<VSm>ux %x3,%0,%2"
  [(set_attr "type" "vecload")])

(define_insn "*vsx_load<mode>_update32"
  [(set (match_operand:VSX_L 3 "vsx_register_operand" "=<VSr>,?wa")
	(mem:VSX_L (plus:SI (match_operand:SI 1 "gpc_reg_operand" "0,0")
			    (match_operand:SI 2 "gpc_reg_operand" "r,r"))))
   (set (match_operand:SI 0 "gpc_reg_operand" "=b,b")
	(plus:SI (match_dup 1)
		 (match_dup 2)))]
  "TARGET_32BIT && TARGET_UPDATE && VECTOR_MEM_VSX_P (<MODE>mode)"
  "lx<VSm>ux %x3,%0,%2"
  [(set_attr "type" "vecload")])

(define_insn "*vsx_store<mode>_update64"
  [(set (mem:VSX_L (plus:DI (match_operand:DI 1 "gpc_reg_operand" "0,0")
			    (match_operand:DI 2 "gpc_reg_operand" "r,r")))
	(match_operand:VSX_L 3 "gpc_reg_operand" "<VSr>,?wa"))
   (set (match_operand:DI 0 "gpc_reg_operand" "=b,b")
	(plus:DI (match_dup 1)
		 (match_dup 2)))]
  "TARGET_64BIT && TARGET_UPDATE && VECTOR_MEM_VSX_P (<MODE>mode)"
  "stx<VSm>ux %x3,%0,%2"
  [(set_attr "type" "vecstore")])

(define_insn "*vsx_store<mode>_update32"
  [(set (mem:VSX_L (plus:SI (match_operand:SI 1 "gpc_reg_operand" "0,0")
			    (match_operand:SI 2 "gpc_reg_operand" "r,r")))
	(match_operand:VSX_L 3 "gpc_reg_operand" "<VSr>,?wa"))
   (set (match_operand:SI 0 "gpc_reg_operand" "=b,b")
	(plus:SI (match_dup 1)
		 (match_dup 2)))]
  "TARGET_32BIT && TARGET_UPDATE && VECTOR_MEM_VSX_P (<MODE>mode)"
  "stx<VSm>ux %x3,%0,%2"
  [(set_attr "type" "vecstore")])

(define_insn "*vsx_loaddf_update<VSbit>"
  [(set (match_operand:DF 3 "vsx_register_operand" "=ws,?wa")
	(mem:DF (plus:P (match_operand:P 1 "gpc_reg_operand" "0,0")
			(match_operand:P 2 "gpc_reg_operand" "r,r"))))
   (set (match_operand:P 0 "gpc_reg_operand" "=b,b")
	(plus:P (match_dup 1)
		(match_dup 2)))]
  "TARGET_<VSbit>BIT && TARGET_UPDATE && VECTOR_MEM_VSX_P (DFmode)"
  "lxsdux %x3,%0,%2"
  [(set_attr "type" "vecload")])

(define_insn "*vsx_storedf_update<VSbit>"
  [(set (mem:DF (plus:P (match_operand:P 1 "gpc_reg_operand" "0,0")
			(match_operand:P 2 "gpc_reg_operand" "r,r")))
	(match_operand:DF 3 "gpc_reg_operand" "ws,?wa"))
   (set (match_operand:P 0 "gpc_reg_operand" "=b,b")
	(plus:P (match_dup 1)
		(match_dup 2)))]
  "TARGET_<VSbit>BIT && TARGET_UPDATE && VECTOR_MEM_VSX_P (DFmode)"
  "stxsdux %x3,%0,%2"
  [(set_attr "type" "vecstore")])

;; We may need to have a varient on the pattern for use in the prologue
;; that doesn't depend on TARGET_UPDATE.


;; VSX scalar and vector floating point arithmetic instructions
(define_insn "*vsx_add<mode>3"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
        (plus:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")
		    (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>add<VSs> %x0,%x1,%x2"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "*vsx_sub<mode>3"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
        (minus:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")
		     (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>sub<VSs> %x0,%x1,%x2"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "*vsx_mul<mode>3"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
        (mult:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")
		    (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>mul<VSs> %x0,%x1,%x2"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

(define_insn "*vsx_div<mode>3"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
        (div:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")
	 	   (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>div<VSs> %x0,%x1,%x2"
  [(set_attr "type" "<VStype_div>")
   (set_attr "fp_type" "<VSfptype_div>")])

(define_insn "vsx_tdiv<mode>3"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")
		       (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,wa")]
		      UNSPEC_VSX_XVTDIV))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>tdiv<VSs> %x0,%x1,%x2"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_fre<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")]
		      UNSPEC_FRES))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>re<VSs> %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "*vsx_neg<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
        (neg:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>neg<VSs> %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "*vsx_abs<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
        (abs:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>abs<VSs> %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_nabs<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
        (neg:VSX_B
	 (abs:VSX_B
	  (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa"))))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>nabs<VSs> %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_smax<mode>3"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
        (smax:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")
		    (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>max<VSs> %x0,%x1,%x2"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "*vsx_smin<mode>3"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
        (smin:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")
		    (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>min<VSs> %x0,%x1,%x2"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "*vsx_sqrt<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
        (sqrt:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>sqrt<VSs> %x0,%x1"
  [(set_attr "type" "<VStype_sqrt>")
   (set_attr "fp_type" "<VSfptype_sqrt>")])

(define_insn "vsx_rsqrte<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")]
		      UNSPEC_VSX_XVRSQRTE))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>rsqrte<VSs> %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_tsqrt<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")]
		      UNSPEC_VSX_XVTSQRT))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>tsqrt<VSs> %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

;; Fused vector multiply/add instructions

;; Note we have a pattern for the multiply/add operations that uses unspec and
;; does not check -mfused-madd to allow users to use these ops when they know
;; they want the fused multiply/add.

(define_expand "vsx_fmadd<mode>4"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "")
	(plus:VSX_B
	 (mult:VSX_B
	  (match_operand:VSX_B 1 "vsx_register_operand" "")
	  (match_operand:VSX_B 2 "vsx_register_operand" ""))
	 (match_operand:VSX_B 3 "vsx_register_operand" "")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
{
  if (!TARGET_FUSED_MADD)
    {
      emit_insn (gen_vsx_fmadd<mode>4_2 (operands[0], operands[1], operands[2],
					 operands[3]));
      DONE;
    }
})

(define_insn "*vsx_fmadd<mode>4_1"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,<VSr>,?wa,?wa")
	(plus:VSX_B
	 (mult:VSX_B
	  (match_operand:VSX_B 1 "vsx_register_operand" "%<VSr>,<VSr>,wa,wa")
	  (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,0,wa,0"))
	 (match_operand:VSX_B 3 "vsx_register_operand" "0,<VSr>,0,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode) && TARGET_FUSED_MADD"
  "@
   x<VSv>madda<VSs> %x0,%x1,%x2
   x<VSv>maddm<VSs> %x0,%x1,%x3
   x<VSv>madda<VSs> %x0,%x1,%x2
   x<VSv>maddm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

(define_insn "vsx_fmadd<mode>4_2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,<VSr>,?wa,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "%<VSr>,<VSr>,wa,wa")
		       (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,0,wa,0")
		       (match_operand:VSX_B 3 "vsx_register_operand" "0,<VSr>,0,wa")]
		      UNSPEC_VSX_XVMADD))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "@
   x<VSv>madda<VSs> %x0,%x1,%x2
   x<VSv>maddm<VSs> %x0,%x1,%x3
   x<VSv>madda<VSs> %x0,%x1,%x2
   x<VSv>maddm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

(define_expand "vsx_fmsub<mode>4"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "")
	(minus:VSX_B
	 (mult:VSX_B
	  (match_operand:VSX_B 1 "vsx_register_operand" "")
	  (match_operand:VSX_B 2 "vsx_register_operand" ""))
	 (match_operand:VSX_B 3 "vsx_register_operand" "")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
{
  if (!TARGET_FUSED_MADD)
    {
      emit_insn (gen_vsx_fmsub<mode>4_2 (operands[0], operands[1], operands[2],
					 operands[3]));
      DONE;
    }
})

(define_insn "*vsx_fmsub<mode>4_1"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,<VSr>,?wa,?wa")
	(minus:VSX_B
	 (mult:VSX_B
	  (match_operand:VSX_B 1 "vsx_register_operand" "%<VSr>,<VSr>,wa,wa")
	  (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,0,wa,0"))
	 (match_operand:VSX_B 3 "vsx_register_operand" "0,<VSr>,0,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode) && TARGET_FUSED_MADD"
  "@
   x<VSv>msuba<VSs> %x0,%x1,%x2
   x<VSv>msubm<VSs> %x0,%x1,%x3
   x<VSv>msuba<VSs> %x0,%x1,%x2
   x<VSv>msubm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

(define_insn "vsx_fmsub<mode>4_2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,<VSr>,?wa,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "%<VSr>,<VSr>,wa,wa")
		       (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,0,wa,0")
		       (match_operand:VSX_B 3 "vsx_register_operand" "0,<VSr>,0,wa")]
		      UNSPEC_VSX_XVMSUB))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "@
   x<VSv>msuba<VSs> %x0,%x1,%x2
   x<VSv>msubm<VSs> %x0,%x1,%x3
   x<VSv>msuba<VSs> %x0,%x1,%x2
   x<VSv>msubm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

(define_expand "vsx_fnmadd<mode>4"
  [(match_operand:VSX_B 0 "vsx_register_operand" "")
   (match_operand:VSX_B 1 "vsx_register_operand" "")
   (match_operand:VSX_B 2 "vsx_register_operand" "")
   (match_operand:VSX_B 3 "vsx_register_operand" "")]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
{
  if (TARGET_FUSED_MADD && HONOR_SIGNED_ZEROS (DFmode))
    {
       emit_insn (gen_vsx_fnmadd<mode>4_1 (operands[0], operands[1],
					   operands[2], operands[3]));
       DONE;
    }
  else if (TARGET_FUSED_MADD && !HONOR_SIGNED_ZEROS (DFmode))
    {
       emit_insn (gen_vsx_fnmadd<mode>4_2 (operands[0], operands[1],
					   operands[2], operands[3]));
       DONE;
    }
  else
    {
       emit_insn (gen_vsx_fnmadd<mode>4_3 (operands[0], operands[1],
					   operands[2], operands[3]));
       DONE;
    }
})

(define_insn "vsx_fnmadd<mode>4_1"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,<VSr>,?wa,?wa")
	(neg:VSX_B
	 (plus:VSX_B
	  (mult:VSX_B
	   (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,<VSr>,wa,wa")
	   (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,0,wa,0"))
	  (match_operand:VSX_B 3 "vsx_register_operand" "0,<VSr>,0,wa"))))]
  "VECTOR_UNIT_VSX_P (<MODE>mode) && TARGET_FUSED_MADD
   && HONOR_SIGNED_ZEROS (DFmode)"
  "@
   x<VSv>nmadda<VSs> %x0,%x1,%x2
   x<VSv>nmaddm<VSs> %x0,%x1,%x3
   x<VSv>nmadda<VSs> %x0,%x1,%x2
   x<VSv>nmaddm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

(define_insn "vsx_fnmadd<mode>4_2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,<VSr>,?wa,?wa")
	(minus:VSX_B
	 (mult:VSX_B
	  (neg:VSX_B
	   (match_operand:VSX_B 1 "gpc_reg_operand" "<VSr>,<VSr>,wa,wa"))
	  (match_operand:VSX_B 2 "gpc_reg_operand" "<VSr>,0,wa,0"))
	 (match_operand:VSX_B 3 "vsx_register_operand" "0,<VSr>,0,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode) && TARGET_FUSED_MADD
   && !HONOR_SIGNED_ZEROS (DFmode)"
  "@
   x<VSv>nmadda<VSs> %x0,%x1,%x2
   x<VSv>nmaddm<VSs> %x0,%x1,%x3
   x<VSv>nmadda<VSs> %x0,%x1,%x2
   x<VSv>nmaddm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

(define_insn "vsx_fnmadd<mode>4_3"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,<VSr>,?wa,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,<VSr>,wa,wa")
		       (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,0,wa,0")
		       (match_operand:VSX_B 3 "vsx_register_operand" "0,<VSr>,0,wa")]
		      UNSPEC_VSX_XVNMADD))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "@
   x<VSv>nmadda<VSs> %x0,%x1,%x2
   x<VSv>nmaddm<VSs> %x0,%x1,%x3
   x<VSv>nmadda<VSs> %x0,%x1,%x2
   x<VSv>nmaddm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

(define_expand "vsx_fnmsub<mode>4"
  [(match_operand:VSX_B 0 "vsx_register_operand" "")
   (match_operand:VSX_B 1 "vsx_register_operand" "")
   (match_operand:VSX_B 2 "vsx_register_operand" "")
   (match_operand:VSX_B 3 "vsx_register_operand" "")]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
{
  if (TARGET_FUSED_MADD && HONOR_SIGNED_ZEROS (DFmode))
    {
       emit_insn (gen_vsx_fnmsub<mode>4_1 (operands[0], operands[1],
					   operands[2], operands[3]));
       DONE;
    }
  else if (TARGET_FUSED_MADD && !HONOR_SIGNED_ZEROS (DFmode))
    {
       emit_insn (gen_vsx_fnmsub<mode>4_2 (operands[0], operands[1],
					   operands[2], operands[3]));
       DONE;
    }
  else
    {
       emit_insn (gen_vsx_fnmsub<mode>4_3 (operands[0], operands[1],
					   operands[2], operands[3]));
       DONE;
    }
})

(define_insn "vsx_fnmsub<mode>4_1"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,<VSr>,?wa,?wa")
	(neg:VSX_B
	 (minus:VSX_B
	  (mult:VSX_B
	   (match_operand:VSX_B 1 "vsx_register_operand" "%<VSr>,<VSr>,wa,wa")
	   (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,0,wa,0"))
	  (match_operand:VSX_B 3 "vsx_register_operand" "0,<VSr>,0,wa"))))]
  "VECTOR_UNIT_VSX_P (<MODE>mode) && TARGET_FUSED_MADD
   && HONOR_SIGNED_ZEROS (DFmode)"
  "@
   x<VSv>nmsuba<VSs> %x0,%x1,%x2
   x<VSv>nmsubm<VSs> %x0,%x1,%x3
   x<VSv>nmsuba<VSs> %x0,%x1,%x2
   x<VSv>nmsubm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

(define_insn "vsx_fnmsub<mode>4_2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,<VSr>,?wa,?wa")
	(minus:VSX_B
	 (match_operand:VSX_B 3 "vsx_register_operand" "0,<VSr>,0,wa")
	 (mult:VSX_B
	  (match_operand:VSX_B 1 "vsx_register_operand" "%<VSr>,<VSr>,wa,wa")
	  (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,0,wa,0"))))]
  "VECTOR_UNIT_VSX_P (<MODE>mode) && TARGET_FUSED_MADD
   && !HONOR_SIGNED_ZEROS (DFmode)"
  "@
   x<VSv>nmsuba<VSs> %x0,%x1,%x2
   x<VSv>nmsubm<VSs> %x0,%x1,%x3
   x<VSv>nmsuba<VSs> %x0,%x1,%x2
   x<VSv>nmsubm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

(define_insn "vsx_fnmsub<mode>4_3"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,<VSr>,?wa,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "%<VSr>,<VSr>,wa,wa")
		       (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,0,wa,0")
		       (match_operand:VSX_B 3 "vsx_register_operand" "0,<VSr>,0,wa")]
		      UNSPEC_VSX_XVNMSUB))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "@
   x<VSv>nmsuba<VSs> %x0,%x1,%x2
   x<VSv>nmsubm<VSs> %x0,%x1,%x3
   x<VSv>nmsuba<VSs> %x0,%x1,%x2
   x<VSv>nmsubm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "<VStype_mul>")
   (set_attr "fp_type" "<VSfptype_mul>")])

;; Vector conditional expressions (no scalar version for these instructions)
(define_insn "vsx_eq<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,?wa")
	(eq:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>,wa")
		  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcmpeq<VSs> %x0,%x1,%x2"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_gt<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,?wa")
	(gt:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>,wa")
		  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcmpgt<VSs> %x0,%x1,%x2"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "*vsx_ge<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,?wa")
	(ge:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>,wa")
		  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcmpge<VSs> %x0,%x1,%x2"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_vsel<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,?wa")
	(if_then_else:VSX_F (ne (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>,wa")
				(const_int 0))
			    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,wa")
			    (match_operand:VSX_F 3 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xxsel %x0,%x3,%x2,%x1"
  [(set_attr "type" "vecperm")])

;; Copy sign
(define_insn "vsx_copysign<mode>3"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
	(if_then_else:VSX_B
	 (ge:VSX_B (match_operand:VSX_B 2 "vsx_register_operand" "<VSr>,wa")
		   (const_int 0))
	 (abs:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa"))
	 (neg:VSX_B (abs:VSX_B (match_dup 1)))))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>cpsgn<VSs> %x0,%x2,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

;; For the conversions, limit the register class for the integer value to be
;; the fprs because we don't want to add the altivec registers to movdi/movsi.
;; For the unsigned tests, there isn't a generic double -> unsigned conversion
;; in rs6000.md so don't test VECTOR_UNIT_VSX_P, just test against VSX.
(define_insn "vsx_ftrunc<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
  	(fix:VSX_B (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>r<VSs>piz %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_float<VSi><mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
	(float:VSX_B (match_operand:<VSI> 1 "vsx_register_operand" "<VSr2>,<VSr3>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>cvsx<VSc><VSs> %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_floatuns<VSi><mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
	(unsigned_float:VSX_B (match_operand:<VSI> 1 "vsx_register_operand" "<VSr2>,<VSr3>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>cvux<VSc><VSs> %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_fix_trunc<mode><VSi>2"
  [(set (match_operand:<VSI> 0 "vsx_register_operand" "=<VSr2>,?<VSr3>")
	(fix:<VSI> (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>cv<VSs>sx<VSc>s %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_fixuns_trunc<mode><VSi>2"
  [(set (match_operand:<VSI> 0 "vsx_register_operand" "=<VSr2>,?<VSr3>")
	(unsigned_fix:<VSI> (match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>cv<VSs>ux<VSc>s %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

;; Math rounding functions
(define_insn "vsx_btrunc<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")]
		      UNSPEC_FRIZ))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>r<VSs>iz %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_floor<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")]
		      UNSPEC_FRIM))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>r<VSs>im %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])

(define_insn "vsx_ceil<mode>2"
  [(set (match_operand:VSX_B 0 "vsx_register_operand" "=<VSr>,?wa")
	(unspec:VSX_B [(match_operand:VSX_B 1 "vsx_register_operand" "<VSr>,wa")]
		      UNSPEC_FRIP))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "x<VSv>r<VSs>ip %x0,%x1"
  [(set_attr "type" "<VStype_simple>")
   (set_attr "fp_type" "<VSfptype_simple>")])


;; VSX convert to/from double vector

;; Convert from 64-bit to 32-bit types
;; Note, favor the Altivec registers since the usual use of these instructions
;; is in vector converts and we need to use the Altivec vperm instruction.

(define_insn "vsx_xvcvdpsp"
  [(set (match_operand:V4SF 0 "vsx_register_operand" "=v,?wa")
	(unspec:V4SF [(match_operand:V2DF 1 "vsx_register_operand" "wd,wa")]
		     UNSPEC_VSX_XVCVDPSP))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "xvcvdpsp %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "vsx_xvcvdpsxws"
  [(set (match_operand:V4SI 0 "vsx_register_operand" "=v,?wa")
	(unspec:V4SI [(match_operand:V2DF 1 "vsx_register_operand" "wd,wa")]
		     UNSPEC_VSX_XVCVDPSXWS))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "xvcvdpsxws %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "vsx_xvcvdpuxws"
  [(set (match_operand:V4SI 0 "vsx_register_operand" "=v,?wa")
	(unspec:V4SI [(match_operand:V2DF 1 "vsx_register_operand" "wd,wa")]
		     UNSPEC_VSX_XVCVDPUXWS))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "xvcvdpuxws %x0,%x1"
  [(set_attr "type" "vecfloat")])

;; Convert from 32-bit to 64-bit types
(define_insn "vsx_xvcvspdp"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd,?wa")
	(unspec:V2DF [(match_operand:V4SF 1 "vsx_register_operand" "wf,wa")]
		     UNSPEC_VSX_XVCVSPDP))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "xvcvspdp %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "vsx_xvcvsxwdp"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd,?wa")
	(unspec:V2DF [(match_operand:V4SI 1 "vsx_register_operand" "wf,wa")]
		     UNSPEC_VSX_XVCVSXWDP))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "xvcvsxwdp %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "vsx_xvcvuxwdp"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd,?wa")
	(unspec:V2DF [(match_operand:V4SI 1 "vsx_register_operand" "wf,wa")]
		     UNSPEC_VSX_XVCVUXWDP))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "xvcvuxwdp %x0,%x1"
  [(set_attr "type" "vecfloat")])


;; Logical and permute operations
(define_insn "*vsx_and<mode>3"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>,?wa")
        (and:VSX_L
	 (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>,?wa")
	 (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>,?wa")))]
  "VECTOR_MEM_VSX_P (<MODE>mode)"
  "xxland %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*vsx_ior<mode>3"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>,?wa")
        (ior:VSX_L (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>,?wa")
		   (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>,?wa")))]
  "VECTOR_MEM_VSX_P (<MODE>mode)"
  "xxlor %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*vsx_xor<mode>3"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>,?wa")
        (xor:VSX_L
	 (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>,?wa")
	 (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>,?wa")))]
  "VECTOR_MEM_VSX_P (<MODE>mode)"
  "xxlxor %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*vsx_one_cmpl<mode>2"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>,?wa")
        (not:VSX_L
	 (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>,?wa")))]
  "VECTOR_MEM_VSX_P (<MODE>mode)"
  "xxlnor %x0,%x1,%x1"
  [(set_attr "type" "vecsimple")])
  
(define_insn "*vsx_nor<mode>3"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>,?wa")
        (not:VSX_L
	 (ior:VSX_L
	  (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>,?wa")
	  (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>,?wa"))))]
  "VECTOR_MEM_VSX_P (<MODE>mode)"
  "xxlnor %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*vsx_andc<mode>3"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>,?wa")
        (and:VSX_L
	 (not:VSX_L
	  (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>,?wa"))
	 (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>,?wa")))]
  "VECTOR_MEM_VSX_P (<MODE>mode)"
  "xxlandc %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])


;; Permute operations

(define_insn "vsx_concat_v2df"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd,?wa")
	(unspec:V2DF
	 [(match_operand:DF 1 "vsx_register_operand" "ws,wa")
	  (match_operand:DF 2 "vsx_register_operand" "ws,wa")]
	 UNSPEC_VSX_CONCAT_V2DF))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "xxpermdi %x0,%x1,%x2,0"
  [(set_attr "type" "vecperm")])

;; Set a double into one element
(define_insn "vsx_set_v2df"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd,?wa")
	(vec_merge:V2DF
	 (match_operand:V2DF 1 "vsx_register_operand" "wd,wa")
	 (vec_duplicate:V2DF (match_operand:DF 2 "vsx_register_operand" "ws,f"))
	 (match_operand:QI 3 "u5bit_cint_operand" "i,i")))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
{
  if (INTVAL (operands[3]) == 0)
    return \"xxpermdi %x0,%x1,%x2,1\";
  else if (INTVAL (operands[3]) == 1)
    return \"xxpermdi %x0,%x2,%x1,0\";
  else
    gcc_unreachable ();
}
  [(set_attr "type" "vecperm")])

;; Extract a DF element from V2DF
(define_insn "vsx_extract_v2df"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws,f,?wa")
	(vec_select:DF (match_operand:V2DF 1 "vsx_register_operand" "wd,wd,wa")
		       (parallel
			[(match_operand:QI 2 "u5bit_cint_operand" "i,i,i")])))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
{
  gcc_assert (UINTVAL (operands[2]) <= 1);
  operands[3] = GEN_INT (INTVAL (operands[2]) << 1);
  return \"xxpermdi %x0,%x1,%x1,%3\";
}
  [(set_attr "type" "vecperm")])

;; General V2DF permute, extract_{high,low,even,odd}
(define_insn "vsx_xxpermdi"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd")
	(vec_concat:V2DF
	 (vec_select:DF (match_operand:V2DF 1 "vsx_register_operand" "wd")
			(parallel
			 [(match_operand:QI 2 "u5bit_cint_operand" "i")]))
	 (vec_select:DF (match_operand:V2DF 3 "vsx_register_operand" "wd")
			(parallel
			 [(match_operand:QI 4 "u5bit_cint_operand" "i")]))))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
{
  gcc_assert ((UINTVAL (operands[2]) <= 1) && (UINTVAL (operands[4]) <= 1));
  operands[5] = GEN_INT (((INTVAL (operands[2]) & 1) << 1)
			 | (INTVAL (operands[4]) & 1));
  return \"xxpermdi %x0,%x1,%x3,%5\";
}
  [(set_attr "type" "vecperm")])

;; V2DF splat
(define_insn "vsx_splatv2df"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd,wd,wd,?wa,?wa,?wa")
	(vec_duplicate:V2DF
	 (match_operand:DF 1 "input_operand" "ws,f,Z,wa,wa,Z")))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "@
   xxpermdi %x0,%x1,%x1,0
   xxpermdi %x0,%x1,%x1,0
   lxvdsx %x0,%y1
   xxpermdi %x0,%x1,%x1,0
   xxpermdi %x0,%x1,%x1,0
   lxvdsx %x0,%y1"
  [(set_attr "type" "vecperm,vecperm,vecload,vecperm,vecperm,vecload")])

;; V4SF splat
(define_insn "*vsx_xxspltw"
  [(set (match_operand:V4SF 0 "vsx_register_operand" "=wf,?wa")
	(vec_duplicate:V4SF
	 (vec_select:SF (match_operand:V4SF 1 "vsx_register_operand" "wf,wa")
			(parallel
			 [(match_operand:QI 2 "u5bit_cint_operand" "i,i")]))))]
  "VECTOR_UNIT_VSX_P (V4SFmode)"
  "xxspltw %x0,%x1,%2"
  [(set_attr "type" "vecperm")])

;; V4SF interleave
(define_insn "vsx_xxmrghw"
  [(set (match_operand:V4SF 0 "register_operand" "=wf,?wa")
        (vec_merge:V4SF
	 (vec_select:V4SF (match_operand:V4SF 1 "vsx_register_operand" "wf,wa")
			  (parallel [(const_int 0)
				     (const_int 2)
				     (const_int 1)
				     (const_int 3)]))
	 (vec_select:V4SF (match_operand:V4SF 2 "vsx_register_operand" "wf,wa")
			  (parallel [(const_int 2)
				     (const_int 0)
				     (const_int 3)
				     (const_int 1)]))
	 (const_int 5)))]
  "VECTOR_UNIT_VSX_P (V4SFmode)"
  "xxmrghw %x0,%x1,%x2"
  [(set_attr "type" "vecperm")])

(define_insn "vsx_xxmrglw"
  [(set (match_operand:V4SF 0 "register_operand" "=wf,?wa")
        (vec_merge:V4SF
	 (vec_select:V4SF
	  (match_operand:V4SF 1 "register_operand" "wf,wa")
	  (parallel [(const_int 2)
		     (const_int 0)
		     (const_int 3)
		     (const_int 1)]))
	 (vec_select:V4SF
	  (match_operand:V4SF 2 "register_operand" "wf,?wa")
	  (parallel [(const_int 0)
		     (const_int 2)
		     (const_int 1)
		     (const_int 3)]))
	 (const_int 5)))]
  "VECTOR_UNIT_VSX_P (V4SFmode)"
  "xxmrglw %x0,%x1,%x2"
  [(set_attr "type" "vecperm")])


;; Reload patterns for VSX loads/stores.  We need a scratch register to convert
;; the stack temporary address from reg+offset to reg+reg addressing.
(define_expand "vsx_reload_<VSX_L:mode>_<P:ptrsize>_to_mem"
  [(parallel [(match_operand:VSX_L 0 "memory_operand" "")
              (match_operand:VSX_L 1 "register_operand" "=wa")
              (match_operand:P 2 "register_operand" "=&b")])]
  "VECTOR_MEM_VSX_P (<MODE>mode)"
{
  rs6000_vector_secondary_reload (operands[0], operands[1], operands[2], true);
  DONE;
})

(define_expand "vsx_reload_<VSX_L:mode>_<P:ptrsize>_to_reg"
  [(parallel [(match_operand:VSX_L 0 "register_operand" "=wa")
              (match_operand:VSX_L 1 "memory_operand" "")
              (match_operand:P 2 "register_operand" "=&b")])]
  "VECTOR_MEM_VSX_P (<MODE>mode)"
{
  rs6000_vector_secondary_reload (operands[0], operands[1], operands[2], false);
  DONE;
})
