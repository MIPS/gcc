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
			(TI    "vw4")])

;; Map into the appropriate suffix based on the type
(define_mode_attr VSs	[(V16QI "sp")
			 (V8HI  "sp")
			 (V4SI  "sp")
			 (V4SF  "sp")
			 (V2DF  "dp")
			 (V2DI  "dp")
			 (TI    "sp")])

;; Map into the register class used
(define_mode_attr VSr	[(V16QI "v")
			 (V8HI  "v")
			 (V4SI  "v")
			 (V4SF  "wf")
			 (V2DI  "wd")
			 (V2DF  "wd")
			 (TI    "wd")])

;; Same size integer type for floating point data
(define_mode_attr VSi [(V4SF  "v4si")
		       (V2DF  "v2di")])

(define_mode_attr VSI [(V4SF  "V4SI")
		       (V2DF  "V2DI")])

;; Word size for same size conversion
(define_mode_attr VSc [(V4SF "w")
		       (V2DF "d")])

;; Bitsize for DF load with update
(define_mode_attr VSbit [(SI "32")
			 (DI "64")])

(define_constants
  [(UNSPEC_VSX_CONCAT_V2DF	500)])

;; VSX moves
(define_insn "*vsx_mov<mode>"
  [(set (match_operand:VSX_L 0 "nonimmediate_operand" "=Z,<VSr>,<VSr>,?Z,?wa,?wa,*o,*r,*r,<VSr>,?wa,v")
	(match_operand:VSX_L 1 "input_operand" "<VSr>,Z,<VSr>,wa,Z,wa,r,o,r,j,j,W"))]
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
      return "xvmov<VSs> %x0,%x1";

    case 6:
    case 7:
    case 8:
      return "#";

    case 9:
    case 10:
      return "xxlxor %x0,%x0,%x0";

    case 11:
      return output_vec_const_move (operands);

    default:
      gcc_unreachable ();
    }
}
  [(set_attr "type" "vecstore,vecload,vecsimple,vecstore,vecload,vecsimple,store,load,*,vecsimple,vecsimple,*")])

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


;; VSX vector floating point arithmetic instructions
(define_insn "*vsx_add<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (plus:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvadd<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_sub<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (minus:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		     (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvsub<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_mul<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (mult:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvmul<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_div<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (div:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
	 	   (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvdiv<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfdiv")])

(define_insn "*vsx_fre<mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(unspec:VSX_F [(match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")]
		      UNSPEC_FRES))]
  "flag_finite_math_only && VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvre<VSs> %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_neg<mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (neg:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvneg<VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_abs<mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (abs:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvabs<VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_nabs<mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (neg:VSX_F
	 (abs:VSX_F
	  (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>"))))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvnabs<VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_smax<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (smax:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvmax<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "*vsx_smin<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (smin:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvmin<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "*vsx_sqrt<mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (sqrt:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvsqrt<VSs> %x0,%x1"
  [(set_attr "type" "vecfdiv")])

;; Fused vector multiply/add instructions
(define_insn "*vsx_fmadd<mode>4"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,<VSr>")
	(plus:VSX_F
	 (mult:VSX_F
	  (match_operand:VSX_F 1 "vsx_register_operand" "%<VSr>,<VSr>")
	  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,0"))
	 (match_operand:VSX_F 3 "gpc_reg_operand" "0,<VSr>")))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD"
  "@
   xvmadda<VSs> %x0,%x1,%x2
   xvmaddm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_fmsub<mode>4"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,<VSr>")
	(minus:VSX_F
	 (mult:VSX_F
	  (match_operand:VSX_F 1 "vsx_register_operand" "%<VSr>,<VSr>")
	  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,0"))
	 (match_operand:VSX_F 3 "vsx_register_operand" "0,<VSr>")))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD"
  "@
   xvmsuba<VSs> %x0,%x1,%x2
   xvmsubm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_fnmadd<mode>4_1"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,<VSr>")
	(neg:VSX_F
	 (plus:VSX_F
	  (mult:VSX_F
	   (match_operand:VSX_F 1 "vsx_register_operand" "%<VSr>,<VSr>")
	   (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,0"))
	  (match_operand:VSX_F 3 "vsx_register_operand" "0,<VSr>"))))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD
   && HONOR_SIGNED_ZEROS (DFmode)"
  "@
   xvnmadda<VSs> %x0,%x1,%x2
   xvnmaddm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_fnmadd<mode>4_2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,<VSr>")
	(minus:VSX_F
	 (mult:VSX_F
	  (neg:VSX_F
	   (match_operand:VSX_F 1 "gpc_reg_operand" "%<VSr>,<VSr>"))
	  (match_operand:VSX_F 2 "gpc_reg_operand" "<VSr>,0"))
	 (match_operand:VSX_F 3 "vsx_register_operand" "0,<VSr>")))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD
   && !HONOR_SIGNED_ZEROS (DFmode)"
  "@
   xvnmadda<VSs> %x0,%x1,%x2
   xvnmaddm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_fnmsub<mode>4_1"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,<VSr>")
	(neg:VSX_F
	 (minus:VSX_F
	  (mult:VSX_F
	   (match_operand:VSX_F 1 "vsx_register_operand" "%<VSr>,<VSr>")
	   (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,0"))
	  (match_operand:VSX_F 3 "vsx_register_operand" "0,<VSr>"))))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD
   && HONOR_SIGNED_ZEROS (DFmode)"
  "@
   xvnmsuba<VSs> %x0,%x1,%x2
   xvnmsubm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_fnmsub<mode>4_2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,<VSr>")
	(minus:VSX_F
	 (match_operand:VSX_F 3 "vsx_register_operand" "0,<VSr>")
	 (mult:VSX_F
	  (match_operand:VSX_F 1 "vsx_register_operand" "%<VSr>,<VSr>")
	  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,0"))))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD
   && !HONOR_SIGNED_ZEROS (DFmode)"
  "@
   xvnmsuba<VSs> %x0,%x1,%x2
   xvnmsubm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "vecfloat")])

;; Vector conditional expressions
(define_insn "*vsx_eq<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(eq:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcmpeq<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "*vsx_gt<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(gt:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcmpgt<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "*vsx_ge<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(ge:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcmpge<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "vsx_vsel<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(if_then_else:VSX_F (ne (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
				(const_int 0))
			    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")
			    (match_operand:VSX_F 3 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xxsel %x0,%x3,%x2,%x1"
  [(set_attr "type" "vecperm")])

;; Copy sign
(define_insn "*vsx_copysign<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(if_then_else:VSX_F
	 (ge:VSX_F (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")
		   (const_int 0))
	 (abs:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>"))
	 (neg:VSX_F (abs:VSX_F (match_dup 1)))))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcpsgn<VSs> %x0,%x2,%x1"
  [(set_attr "type" "vecsimple")])

(define_insn "*vsx_ftrunc<mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
  	(fix:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvr<VSs>piz %x0,%x1"
  [(set_attr "type" "vecperm")])

(define_insn "*vsx_float<VSi><mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(float:VSX_F (match_operand:<VSI> 1 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcvsx<VSc><VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_floatuns<VSi><mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(unsigned_float:VSX_F (match_operand:<VSI> 1 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcvux<VSc><VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_fix_trunc<mode><VSi>2"
  [(set (match_operand:<VSI> 0 "vsx_register_operand" "=<VSr>")
	(fix:<VSI> (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcv<VSs>sx<VSc>s %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_fixuns_trunc<mode><VSi>2"
  [(set (match_operand:<VSI> 0 "vsx_register_operand" "=<VSr>")
	(unsigned_fix:<VSI> (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "xvcv<VSs>ux<VSc>s %x0,%x1"
  [(set_attr "type" "vecfloat")])


;; VSX scalar double precision floating point operations
(define_insn"*vsx_adddf3"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(plus:DF (match_operand:DF 1 "vsx_register_operand" "ws")
		     (match_operand:DF 2 "vsx_register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsadddp %x0,%x1,%x2"
  [(set_attr "type" "fp")
   (set_attr "fp_type" "fp_addsub_d")])

(define_insn"*vsx_subdf3"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(minus:DF (match_operand:DF 1 "vsx_register_operand" "ws")
		      (match_operand:DF 2 "vsx_register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xssubdp %x0,%x1,%x2"
  [(set_attr "type" "fp")
   (set_attr "fp_type" "fp_addsub_d")])

(define_insn"*vsx_muldf3"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(mult:DF (match_operand:DF 1 "vsx_register_operand" "ws")
		     (match_operand:DF 2 "vsx_register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsmuldp %x0,%x1,%x2"
  [(set_attr "type" "dmul")
   (set_attr "fp_type" "fp_mul_d")])

(define_insn"*vsx_divdf3"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(div:DF (match_operand:DF 1 "vsx_register_operand" "ws")
		    (match_operand:DF 2 "vsx_register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsdivdp %x0,%x1,%x2"
  [(set_attr "type" "ddiv")])

(define_insn "*vsx_fredf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(unspec:DF [(match_operand:DF 1 "vsx_register_operand" "ws")]
		   UNSPEC_FRES))]
  "flag_finite_math_only && VECTOR_UNIT_VSX_P (DFmode)"
  "xsredp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_sqrtdf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
        (sqrt:DF (match_operand:DF 1 "vsx_register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xssqrtdp %x0,%x1"
  [(set_attr "type" "dsqrt")])

(define_insn"*vsx_negdf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(neg:DF (match_operand:DF 1 "vsx_register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsnegdp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn"vsx_absdf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(abs:DF (match_operand:DF 1 "vsx_register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsabsdp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn"*vsx_nabsdf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(neg:DF (abs:DF (match_operand:DF 1 "vsx_register_operand" "ws"))))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsnabsdp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_smaxdf3"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
        (smax:DF (match_operand:DF 1 "vsx_register_operand" "ws")
		 (match_operand:DF 2 "vsx_register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsmaxdp %x0,%x1,%x2"
  [(set_attr "type" "fp")])


(define_insn "*vsx_smindf3"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
        (smin:DF (match_operand:DF 1 "vsx_register_operand" "ws")
		 (match_operand:DF 2 "vsx_register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsmindp %x0,%x1,%x2"
  [(set_attr "type" "fp")])

;; Fused vector multiply/add instructions
(define_insn "*vsx_fmadddf4"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws,ws")
	(plus:DF (mult:DF (match_operand:DF 1 "vsx_register_operand" "%ws,ws")
			  (match_operand:DF 2 "vsx_register_operand" "ws,0"))
		 (match_operand:DF 3 "gpc_reg_operand" "0,ws")))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD"
  "@
   xsmaddadp %x0,%x1,%x2
   xsmaddmdp %x0,%x1,%x3"
  [(set_attr "type" "dmul")
   (set_attr "fp_type" "fp_maddsub_d")])

(define_insn "*vsx_fmsubdf4"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws,ws")
	(minus:DF (mult:DF (match_operand:DF 1 "vsx_register_operand" "%ws,ws")
			   (match_operand:DF 2 "vsx_register_operand" "ws,0"))
		  (match_operand:DF 3 "vsx_register_operand" "0,ws")))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD"
  "@
   xsmsubadp %x0,%x1,%x2
   xsmsubmdp %x0,%x1,%x3"
  [(set_attr "type" "dmul")
   (set_attr "fp_type" "fp_maddsub_d")])

(define_insn "*vsx_fnmadddf4_1"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws,ws")
	(neg:DF
	 (plus:DF (mult:DF (match_operand:DF 1 "vsx_register_operand" "%ws,ws")
			   (match_operand:DF 2 "vsx_register_operand" "ws,0"))
		  (match_operand:DF 3 "vsx_register_operand" "0,ws"))))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD
   && HONOR_SIGNED_ZEROS (DFmode)"
  "@
   xsnmaddadp %x0,%x1,%x2
   xsnmaddmdp %x0,%x1,%x3"
  [(set_attr "type" "dmul")
   (set_attr "fp_type" "fp_maddsub_d")])

(define_insn "*vsx_fnmadddf4_2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws,ws")
	(minus:DF (mult:DF (neg:DF
			    (match_operand:DF 1 "gpc_reg_operand" "%ws,ws"))
			   (match_operand:DF 2 "gpc_reg_operand" "ws,0"))
		  (match_operand:DF 3 "vsx_register_operand" "0,ws")))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD
   && !HONOR_SIGNED_ZEROS (DFmode)"
  "@
   xsnmaddadp %x0,%x1,%x2
   xsnmaddmdp %x0,%x1,%x3"
  [(set_attr "type" "dmul")
   (set_attr "fp_type" "fp_maddsub_d")])

(define_insn "*vsx_fnmsubdf4_1"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws,ws")
	(neg:DF
	 (minus:DF
	  (mult:DF (match_operand:DF 1 "vsx_register_operand" "%ws,ws")
		   (match_operand:DF 2 "vsx_register_operand" "ws,0"))
	  (match_operand:DF 3 "vsx_register_operand" "0,ws"))))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD
   && HONOR_SIGNED_ZEROS (DFmode)"
  "@
   xsnmsubadp %x0,%x1,%x2
   xsnmsubmdp %x0,%x1,%x3"
  [(set_attr "type" "dmul")
   (set_attr "fp_type" "fp_maddsub_d")])

(define_insn "*vsx_fnmsubdf4_2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws,ws")
	(minus:DF
	 (match_operand:DF 3 "vsx_register_operand" "0,ws")
	 (mult:DF (match_operand:DF 1 "vsx_register_operand" "%ws,ws")
		  (match_operand:DF 2 "vsx_register_operand" "ws,0"))))]
  "VECTOR_UNIT_VSX_P (DFmode) && TARGET_FUSED_MADD
   && !HONOR_SIGNED_ZEROS (DFmode)"
  "@
   xsnmsubadp %x0,%x1,%x2
   xsnmsubmdp %x0,%x1,%x3"
  [(set_attr "type" "dmul")
   (set_attr "fp_type" "fp_maddsub_d")])

;; For the conversions, limit the register class for the integer value to be
;; the fprs.  For the unsigned tests, there isn't a generic double -> unsigned
;; conversion in rs6000.md so don't test VECTOR_UNIT_VSX_P, just test against
;; VSX.

(define_insn "*vsx_floatdidf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(float:DF (match_operand:DI 1 "vsx_register_operand" "!f#r")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xscvsxddp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_floatunsdidf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(unsigned_float:DF (match_operand:DI 1 "vsx_register_operand" "!f#r")))]
  "TARGET_HARD_FLOAT && TARGET_VSX"
  "xscvuxddp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_fix_truncdfdi2"
  [(set (match_operand:DI 0 "vsx_register_operand" "=!f#r")
	(fix:DI (match_operand:DF 1 "vsx_register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xscvdpsxds %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_fixuns_truncdfdi2"
  [(set (match_operand:DI 0 "vsx_register_operand" "=!f#r")
	(unsigned_fix:DI (match_operand:DF 1 "vsx_register_operand" "ws")))]
  "TARGET_HARD_FLOAT && TARGET_VSX"
  "xscvdpuxds %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_btruncdf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(unspec:DF [(match_operand:DF 1 "vsx_register_operand" "ws")]
		   UNSPEC_FRIZ))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsrdpiz %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_floordf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(unspec:DF [(match_operand:DF 1 "vsx_register_operand" "ws")]
		   UNSPEC_FRIM))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsrdpim %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_ceildf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(unspec:DF [(match_operand:DF 1 "vsx_register_operand" "ws")]
		   UNSPEC_FRIP))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsrdpip %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "vsx_copysigndf3"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(if_then_else:DF (ge:DF (match_operand:DF 2 "vsx_register_operand" "ws")
				(const_int 0))
			 (abs:DF (match_operand:DF 1 "vsx_register_operand" "ws"))
			 (neg:DF (abs:DF (match_dup 1)))))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xscpsgndp %x0,%x2,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_ftruncdf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
  	(fix:DF (match_operand:DF 1 "register_operand" "ws")))]
  "VECTOR_UNIT_VSX_P (DFmode)"
  "xsrdppiz %x0,%x1"
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
	 [(match_operand:DF 1 "vsx_register_operand" "f,wa")
	  (match_operand:DF 2 "vsx_register_operand" "f,wa")]
	 UNSPEC_VSX_CONCAT_V2DF))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "xxpermdi %x0,%x1,%x2,0"
  [(set_attr "type" "vecperm")])

;; Set a double into one element
(define_insn "vsx_set_v2df"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd")
	(vec_merge:V2DF
	 (match_operand:V2DF 1 "vsx_register_operand" "wd")
	 (vec_duplicate:V2DF (match_operand:DF 2 "vsx_register_operand" "ws"))
	 (match_operand:QI 3 "u5bit_cint_operand" "i")))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
{
  operands[3] = GEN_INT (INTVAL (operands[3]) & 1);
  return \"xxpermdi %x0,%x1,%x2,%3\";
}
  [(set_attr "type" "vecperm")])

;; Extract a DF element from V2DF
(define_insn "vsx_extract_v2df"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
	(vec_select:DF (match_operand:V2DF 1 "vsx_register_operand" "wd")
		       (parallel
			[(match_operand:QI 2 "u5bit_cint_operand" "i")])))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
{
  operands[3] = GEN_INT (INTVAL (operands[2]) & 1);
  return \"xxpermdi %x0,%x1,%x1,%3\";
}
  [(set_attr "type" "vecperm")])

;; General V2DF permute
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
  operands[5] = GEN_INT (((INTVAL (operands[2]) & 1) << 1)
			 | (INTVAL (operands[4]) & 1));
  return \"xxpermdi %x0,%x1,%x3,%5\";
}
  [(set_attr "type" "vecperm")])

;; V2DF splat
(define_insn "vsx_splatv2df"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd,wd")
	(vec_duplicate:V2DF
	 (match_operand:DF 1 "input_operand" "ws,Z")))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "@
   xxpermdi %x0,%x1,%x1,0
   lxvdsx %x0,%y1"
  [(set_attr "type" "vecperm,vecload")])

;; V4SF splat
(define_insn "*vsx_xxspltw"
  [(set (match_operand:V4SF 0 "vsx_register_operand" "=wf")
	(vec_duplicate:V4SF
	 (vec_select:SF (match_operand:V4SF 1 "vsx_register_operand" "wf")
			(parallel
			 [(match_operand:QI 2 "u5bit_cint_operand" "i")]))))]
  "VECTOR_UNIT_VSX_P (V4SFmode)"
  "xxspltw %x0,%x1,%2"
  [(set_attr "type" "vecperm")])

;; V4SF interleave
(define_insn "*vsx_xxmrghw"
  [(set (match_operand:V4SF 0 "register_operand" "=v")
        (vec_merge:V4SF (vec_select:V4SF (match_operand:V4SF 1 "register_operand" "v")
                                         (parallel [(const_int 0)
                                                    (const_int 2)
                                                    (const_int 1)
                                                    (const_int 3)]))
                        (vec_select:V4SF (match_operand:V4SF 2 "register_operand" "v")
                                         (parallel [(const_int 2)
                                                    (const_int 0)
                                                    (const_int 3)
                                                    (const_int 1)]))
                      (const_int 5)))]
  "VECTOR_UNIT_VSX_P (V4SFmode)"
  "xxmrghw %x0,%x1,%x2"
  [(set_attr "type" "vecperm")])

(define_insn "*vsx_xxmrglw"
  [(set (match_operand:V4SF 0 "register_operand" "=v")
        (vec_merge:V4SF
	 (vec_select:V4SF (match_operand:V4SF 1 "register_operand" "v")
			  (parallel [(const_int 2)
				     (const_int 0)
				     (const_int 3)
				     (const_int 1)]))
	 (vec_select:V4SF (match_operand:V4SF 2 "register_operand" "v")
			  (parallel [(const_int 0)
				     (const_int 2)
				     (const_int 1)
				     (const_int 3)]))
	 (const_int 5)))]
  "VECTOR_UNIT_VSX_P (V4SFmode)"
  "xxmrglw %x0,%x1,%x2"
  [(set_attr "type" "vecperm")])
