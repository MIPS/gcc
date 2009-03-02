;; Expander definitions for vector support between altivec & vsx.  No
;; instructions are in this file, this file provides the generic vector
;; expander, and the actual vector instructions will be in altivec.md and
;; vsx.md

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


;; Vector int modes
(define_mode_iterator VEC_I [V16QI V8HI V4SI])

;; Vector float modes
(define_mode_iterator VEC_F [V4SF V2DF])

;; Vector logical modes
(define_mode_iterator VEC_L [V16QI V8HI V4SI V2DI V4SF V2DF TI])

;; Vector modes for moves.  Don't do TImode here.
(define_mode_iterator VEC_M [V16QI V8HI V4SI V2DI V4SF V2DF])

;; Vector comparison modes
(define_mode_iterator VEC_C [V16QI V8HI V4SI V4SF V2DF])

;; Base type from vector mode
(define_mode_attr VEC_base [(V16QI "QI")
			    (V8HI  "HI")
			    (V4SI  "SI")
			    (V2DI  "DI")
			    (V4SF  "SF")
			    (V2DF  "DF")
			    (TI    "TI")])

;; Vector move instructions.
(define_expand "mov<mode>"
  [(set (match_operand:VEC_M 0 "nonimmediate_operand" "")
	(match_operand:VEC_M 1 "any_operand" ""))]
  "VECTOR_MEM_ALTIVEC_OR_VSX_P (<MODE>mode)"
{
  /* modes without special handling just generate the normal SET operation.  */
  if (<MODE>mode != TImode && <MODE>mode != V2DImode && <MODE>mode != V2DFmode)
    {
      rs6000_emit_move (operands[0], operands[1], <MODE>mode);
      DONE;
    }
  else if (!vlogical_operand (operands[0], <MODE>mode)
	   && !vlogical_operand (operands[1], <MODE>mode))
    operands[1] = force_reg (<MODE>mode, operands[1]);
})

;; Generic vector floating point load/store instructions.  These will match
;; insns defined in vsx.md or altivec.md depending on the switches.
(define_expand "vector_load_<mode>"
  [(set (match_operand:VEC_M 0 "vfloat_operand" "")
	(match_operand:VEC_M 1 "memory_operand" ""))]
  "VECTOR_MEM_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "vector_store_<mode>"
  [(set (match_operand:VEC_M 0 "memory_operand" "")
	(match_operand:VEC_M 1 "vfloat_operand" ""))]
  "VECTOR_MEM_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

;; Splits if a GPR register was chosen for the move
(define_split
  [(set (match_operand:VEC_L 0 "nonimmediate_operand" "")
        (match_operand:VEC_L 1 "input_operand" ""))]
  "VECTOR_MEM_ALTIVEC_OR_VSX_P (<MODE>mode)
   && reload_completed
   && gpr_or_gpr_p (operands[0], operands[1])"
  [(pc)]
{
  rs6000_split_multireg_move (operands[0], operands[1]);
  DONE;
})


;; Generic floating point vector arithmetic support
(define_expand "add<mode>3"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(plus:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")
		    (match_operand:VEC_F 2 "vfloat_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "sub<mode>3"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(minus:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")
		     (match_operand:VEC_F 2 "vfloat_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "mul<mode>3"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(mult:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")
		    (match_operand:VEC_F 2 "vfloat_operand" "")))]
  "(VECTOR_UNIT_VSX_P (<MODE>mode)
    || (VECTOR_UNIT_ALTIVEC_P (<MODE>mode) && TARGET_FUSED_MADD))"
  "
{
  if (<MODE>mode == V4SFmode && VECTOR_UNIT_ALTIVEC_P (<MODE>mode))
    {
      emit_insn (gen_altivec_mulv4sf3 (operands[0], operands[1], operands[2]));
      DONE;
    }
}")

(define_expand "div<mode>3"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(div:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")
		   (match_operand:VEC_F 2 "vfloat_operand" "")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "")

(define_expand "neg<mode>2"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(neg:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "
{
  if (<MODE>mode == V4SFmode && VECTOR_UNIT_ALTIVEC_P (<MODE>mode))
    {
      emit_insn (gen_altivec_negv4sf2 (operands[0], operands[1]));
      DONE;
    }
}")

(define_expand "abs<mode>2"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(abs:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "
{
  if (<MODE>mode == V4SFmode && VECTOR_UNIT_ALTIVEC_P (<MODE>mode))
    {
      emit_insn (gen_altivec_absv4sf2 (operands[0], operands[1]));
      DONE;
    }
}")

(define_expand "smin<mode>3"
  [(set (match_operand:VEC_F 0 "register_operand" "")
        (smin:VEC_F (match_operand:VEC_F 1 "register_operand" "")
		    (match_operand:VEC_F 2 "register_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "smax<mode>3"
  [(set (match_operand:VEC_F 0 "register_operand" "")
        (smax:VEC_F (match_operand:VEC_F 1 "register_operand" "")
		    (match_operand:VEC_F 2 "register_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")


(define_expand "sqrt<mode>2"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(sqrt:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")))]
  "VECTOR_UNIT_VSX_P (<MODE>mode)"
  "")

(define_insn "ftrunc<mode>2"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
  	(fix:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")


;; Vector comparisons
(define_expand "vcond<mode>"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(if_then_else:VEC_F
	 (match_operator 3 "comparison_operator"
			 [(match_operand:VEC_F 4 "vfloat_operand" "")
			  (match_operand:VEC_F 5 "vfloat_operand" "")])
	 (match_operand:VEC_F 1 "vfloat_operand" "")
	 (match_operand:VEC_F 2 "vfloat_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "
{
  if (rs6000_emit_vector_cond_expr (operands[0], operands[1], operands[2],
				    operands[3], operands[4], operands[5]))
    DONE;
  else
    FAIL;
}")

(define_expand "vcond<mode>"
  [(set (match_operand:VEC_I 0 "vint_operand" "")
	(if_then_else:VEC_I
	 (match_operator 3 "comparison_operator"
			 [(match_operand:VEC_I 4 "vint_operand" "")
			  (match_operand:VEC_I 5 "vint_operand" "")])
	 (match_operand:VEC_I 1 "vint_operand" "")
	 (match_operand:VEC_I 2 "vint_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_P (<MODE>mode)"
  "
{
  if (rs6000_emit_vector_cond_expr (operands[0], operands[1], operands[2],
				    operands[3], operands[4], operands[5]))
    DONE;
  else
    FAIL;
}")

(define_expand "vcondu<mode>"
  [(set (match_operand:VEC_I 0 "vint_operand" "=v")
	(if_then_else:VEC_I
	 (match_operator 3 "comparison_operator"
			 [(match_operand:VEC_I 4 "vint_operand" "")
			  (match_operand:VEC_I 5 "vint_operand" "")])
	 (match_operand:VEC_I 1 "vint_operand" "")
	 (match_operand:VEC_I 2 "vint_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_P (<MODE>mode)"
  "
{
  if (rs6000_emit_vector_cond_expr (operands[0], operands[1], operands[2],
				    operands[3], operands[4], operands[5]))
    DONE;
  else
    FAIL;
}")

(define_expand "vector_eq<mode>"
  [(set (match_operand:VEC_C 0 "vlogical_operand" "")
	(eq:VEC_C (match_operand:VEC_C 1 "vlogical_operand" "")
		  (match_operand:VEC_C 2 "vlogical_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "vector_gt<mode>"
  [(set (match_operand:VEC_C 0 "vlogical_operand" "")
	(gt:VEC_C (match_operand:VEC_C 1 "vlogical_operand" "")
		  (match_operand:VEC_C 2 "vlogical_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "vector_ge<mode>"
  [(set (match_operand:VEC_C 0 "vlogical_operand" "")
	(ge:VEC_C (match_operand:VEC_C 1 "vlogical_operand" "")
		  (match_operand:VEC_C 2 "vlogical_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "vector_gtu<mode>"
  [(set (match_operand:VEC_I 0 "vint_operand" "")
	(gtu:VEC_I (match_operand:VEC_I 1 "vint_operand" "")
		   (match_operand:VEC_I 2 "vint_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_P (<MODE>mode)"
  "")

(define_expand "vector_geu<mode>"
  [(set (match_operand:VEC_I 0 "vint_operand" "")
	(geu:VEC_I (match_operand:VEC_I 1 "vint_operand" "")
		   (match_operand:VEC_I 2 "vint_operand" "")))]
  "VECTOR_UNIT_ALTIVEC_P (<MODE>mode)"
  "")

;; Note the arguments for __builtin_altivec_vsel are op2, op1, mask
;; which is in the reverse order that we want
(define_expand "vector_vsel<mode>"
  [(match_operand:VEC_F 0 "vlogical_operand" "")
   (match_operand:VEC_F 1 "vlogical_operand" "")
   (match_operand:VEC_F 2 "vlogical_operand" "")
   (match_operand:VEC_F 3 "vlogical_operand" "")]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "
{
  if (VECTOR_UNIT_VSX_P (<MODE>mode))
    emit_insn (gen_vsx_vsel<mode> (operands[0], operands[3],
				   operands[2], operands[1]));
  else
    emit_insn (gen_altivec_vsel<mode> (operands[0], operands[3],
				       operands[2], operands[1]));
  DONE;
}")

(define_expand "vector_vsel<mode>"
  [(match_operand:VEC_I 0 "vlogical_operand" "")
   (match_operand:VEC_I 1 "vlogical_operand" "")
   (match_operand:VEC_I 2 "vlogical_operand" "")
   (match_operand:VEC_I 3 "vlogical_operand" "")]
  "VECTOR_UNIT_ALTIVEC_P (<MODE>mode)"
  "
{
  emit_insn (gen_altivec_vsel<mode> (operands[0], operands[3],
				     operands[2], operands[1]));
  DONE;
}")


;; Vector logical instructions
(define_expand "xor<mode>3"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (xor:VEC_L (match_operand:VEC_L 1 "vlogical_operand" "")
		   (match_operand:VEC_L 2 "vlogical_operand" "")))]
  "VECTOR_MEM_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "ior<mode>3"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (ior:VEC_L (match_operand:VEC_L 1 "vlogical_operand" "")
		   (match_operand:VEC_L 2 "vlogical_operand" "")))]
  "VECTOR_MEM_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "and<mode>3"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (and:VEC_L (match_operand:VEC_L 1 "vlogical_operand" "")
		   (match_operand:VEC_L 2 "vlogical_operand" "")))]
  "VECTOR_MEM_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "one_cmpl<mode>2"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (not:VEC_L (match_operand:VEC_L 1 "vlogical_operand" "")))]
  "VECTOR_MEM_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")
  
(define_expand "nor<mode>3"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (not:VEC_L (ior:VEC_L (match_operand:VEC_L 1 "vlogical_operand" "")
			      (match_operand:VEC_L 2 "vlogical_operand" ""))))]
  "VECTOR_MEM_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")

(define_expand "andc<mode>3"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (and:VEC_L (not:VEC_L (match_operand:VEC_L 2 "vlogical_operand" ""))
		   (match_operand:VEC_L 1 "vlogical_operand" "")))]
  "VECTOR_MEM_ALTIVEC_OR_VSX_P (<MODE>mode)"
  "")


;; Vector initialization, set, extract
(define_expand "vec_init<mode>"
  [(match_operand:VEC_C 0 "vlogical_operand" "")
   (match_operand:VEC_C 1 "vec_init_operand" "")]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
{
  rs6000_expand_vector_init (operands[0], operands[1]);
  DONE;
})

(define_expand "vec_set<mode>"
  [(match_operand:VEC_C 0 "vlogical_operand" "")
   (match_operand:<VEC_base> 1 "register_operand" "")
   (match_operand 2 "const_int_operand" "")]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
{
  rs6000_expand_vector_set (operands[0], operands[1], INTVAL (operands[2]));
  DONE;
})

(define_expand "vec_extract<mode>"
  [(match_operand:<VEC_base> 0 "register_operand" "")
   (match_operand:VEC_C 1 "vlogical_operand" "")
   (match_operand 2 "const_int_operand" "")]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (<MODE>mode)"
{
  rs6000_expand_vector_extract (operands[0], operands[1],
				INTVAL (operands[2]));
  DONE;
})

;; Interleave patterns
(define_expand "vec_interleave_highv4sf"
  [(set (match_operand:V4SF 0 "vfloat_operand" "")
        (vec_merge:V4SF
	 (vec_select:V4SF (match_operand:V4SF 1 "vfloat_operand" "")
			  (parallel [(const_int 2)
				     (const_int 0)
				     (const_int 3)
				     (const_int 1)]))
	 (vec_select:V4SF (match_operand:V4SF 2 "vfloat_operand" "")
			  (parallel [(const_int 0)
				     (const_int 2)
				     (const_int 1)
				     (const_int 3)]))
	 (const_int 5)))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (V4SFmode)"
  "")

(define_expand "vec_interleave_lowv4sf"
  [(set (match_operand:V4SF 0 "vfloat_operand" "")
        (vec_merge:V4SF
	 (vec_select:V4SF (match_operand:V4SF 1 "vfloat_operand" "")
			  (parallel [(const_int 0)
				     (const_int 2)
				     (const_int 1)
				     (const_int 3)]))
	 (vec_select:V4SF (match_operand:V4SF 2 "vfloat_operand" "")
			  (parallel [(const_int 2)
				     (const_int 0)
				     (const_int 3)
				     (const_int 1)]))
	 (const_int 5)))]
  "VECTOR_UNIT_ALTIVEC_OR_VSX_P (V4SFmode)"
  "")

(define_expand "vec_interleave_highv2df"
  [(set (match_operand:V2DF 0 "vfloat_operand" "")
	(vec_concat:V2DF
	 (vec_select:DF (match_operand:V2DF 1 "vfloat_operand" "")
			(parallel [(const_int 0)]))
	 (vec_select:DF (match_operand:V2DF 2 "vfloat_operand" "")
			(parallel [(const_int 0)]))))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "")

(define_expand "vec_interleave_lowv2df"
  [(set (match_operand:V2DF 0 "vfloat_operand" "")
	(vec_concat:V2DF
	 (vec_select:DF (match_operand:V2DF 1 "vfloat_operand" "")
			(parallel [(const_int 1)]))
	 (vec_select:DF (match_operand:V2DF 2 "vfloat_operand" "")
			(parallel [(const_int 1)]))))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "")

;; For 2 element vectors, even/odd is the same as high/low
(define_expand "vec_extract_evenv2df"
  [(set (match_operand:V2DF 0 "vfloat_operand" "")
	(vec_concat:V2DF
	 (vec_select:DF (match_operand:V2DF 1 "vfloat_operand" "")
			(parallel [(const_int 0)]))
	 (vec_select:DF (match_operand:V2DF 2 "vfloat_operand" "")
			(parallel [(const_int 0)]))))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "")

(define_expand "vec_extract_oddv2df"
  [(set (match_operand:V2DF 0 "vfloat_operand" "")
	(vec_concat:V2DF
	 (vec_select:DF (match_operand:V2DF 1 "vfloat_operand" "")
			(parallel [(const_int 1)]))
	 (vec_select:DF (match_operand:V2DF 2 "vfloat_operand" "")
			(parallel [(const_int 1)]))))]
  "VECTOR_UNIT_VSX_P (V2DFmode)"
  "")
