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
(define_mode_iterator VEC_I [V4SI V8HI V16QI])

;; Vector float modes
(define_mode_iterator VEC_F [V4SF V2DF])

;; Vector logical modes
(define_mode_iterator VEC_L [V4SI V8HI V16QI V4SF V2DF])

;; Vector floating point move instructions.
(define_expand "mov<mode>"
  [(set (match_operand:VEC_F 0 "nonimmediate_operand" "")
	(match_operand:VEC_F 1 "any_operand" ""))]
  "TARGET_ALTIVEC || TARGET_VSX"
{
  rs6000_emit_move (operands[0], operands[1], <MODE>mode);
  DONE;
})

;; Generic vector floating point load/store instructions.  These will match
;; insns defined in vsx.md or altivec.md depending on the switches.
(define_expand "vector_load_<mode>"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(match_operand:VEC_F 1 "memory_operand" ""))]
  "TARGET_ALTIVEC || TARGET_VSX"
{
  rs6000_emit_move (operands[0], operands[1], <MODE>mode);
  DONE;
})

(define_expand "vector_store_<mode>"
  [(set (match_operand:VEC_F 0 "memory_operand" "")
	(match_operand:VEC_F 1 "vfloat_operand" ""))]
  "TARGET_ALTIVEC || TARGET_VSX"
{
  rs6000_emit_move (operands[0], operands[1], <MODE>mode);
  DONE;
})

;; Generic floating point vector arithmetic support
(define_expand "add<mode>3"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(plus:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")
		    (match_operand:VEC_F 2 "vfloat_operand" "")))]
  "(<MODE>mode == V4SFmode && TARGET_ALTIVEC) || TARGET_VSX"
  "")

(define_expand "sub<mode>3"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(minus:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")
		     (match_operand:VEC_F 2 "vfloat_operand" "")))]
  "(<MODE>mode == V4SFmode && TARGET_ALTIVEC) || TARGET_VSX"
  "")

(define_expand "mul<mode>3"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(mult:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")
		    (match_operand:VEC_F 2 "vfloat_operand" "")))]
  "(<MODE>mode == V4SFmode && TARGET_ALTIVEC && TARGET_FUSED_MADD)
   || TARGET_VSX"
  "
{
  if (<MODE>mode == V4SFmode && !TARGET_VSX)
    {
      emit_insn (gen_mulv4sf3_altivec (operands[0], operands[1], operands[2]));
      DONE;
    }
}")

(define_expand "div<mode>3"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(div:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")
		   (match_operand:VEC_F 2 "vfloat_operand" "")))]
  "TARGET_VSX"
  "")

(define_expand "neg<mode>3"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(neg:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")))]
  "(<MODE>mode == V4SFmode && TARGET_ALTIVEC) || TARGET_VSX"
  "
{
  if (<MODE>mode == V4SFmode && !TARGET_VSX)
    {
      emit_insn (gen_negv4sf2_altivec (operands[0], operands[1]));
      DONE;
    }
}")

(define_expand "abs<mode>2"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(abs:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")))]
  "(<MODE>mode == V4SFmode && TARGET_ALTIVEC) || TARGET_VSX"
  "
{
  if (<MODE>mode == V4SFmode && !TARGET_VSX)
    {
      emit_insn (gen_absv4sf2_altivec (operands[0], operands[1]));
      DONE;
    }
}")

(define_expand "smin<mode>3"
  [(set (match_operand:VEC_F 0 "register_operand" "")
        (smin:VEC_F (match_operand:VEC_F 1 "register_operand" "")
		    (match_operand:VEC_F 2 "register_operand" "")))]
  "(<MODE>mode == V4SFmode && TARGET_ALTIVEC) || TARGET_VSX"
  "")

(define_expand "smax<mode>3"
  [(set (match_operand:VEC_F 0 "register_operand" "")
        (smax:VEC_F (match_operand:VEC_F 1 "register_operand" "")
		    (match_operand:VEC_F 2 "register_operand" "")))]
  "(<MODE>mode == V4SFmode && TARGET_ALTIVEC) || TARGET_VSX"
  "")


(define_expand "sqrt<mode>2"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(sqrt:VEC_F (match_operand:VEC_F 1 "vfloat_operand" "")))]
  "TARGET_VSX"
  "")

(define_expand "vcond<mode>"
  [(set (match_operand:VEC_F 0 "vfloat_operand" "")
	(if_then_else:VEC_F
	 (match_operator 3 "comparison_operator"
			 [(match_operand:VEC_F 4 "vfloat_operand" "")
			  (match_operand:VEC_F 5 "vfloat_operand" "")])
	 (match_operand:VEC_F 1 "vfloat_operand" "")
	 (match_operand:V4SF 2 "vfloat_operand" "")))]
  "TARGET_ALTIVEC"
  "
{
  if (rs6000_emit_vector_cond_expr (operands[0], operands[1], operands[2],
				    operands[3], operands[4], operands[5]))
    DONE;
  else
    FAIL;
}")



;; Vector integer move instructions.
(define_expand "mov<mode>"
  [(set (match_operand:VEC_I 0 "nonimmediate_operand" "")
	(match_operand:VEC_I 1 "any_operand" ""))]
  "TARGET_ALTIVEC"
{
  rs6000_emit_move (operands[0], operands[1], <MODE>mode);
  DONE;
})

;; Generic vector integer load/store instructions.
(define_expand "vector_load_<mode>"
  [(set (match_operand:VEC_I 0 "vint_operand" "")
	(match_operand:VEC_I 1 "memory_operand" ""))]
  "TARGET_ALTIVEC"
{
  rs6000_emit_move (operands[0], operands[1], <MODE>mode);
  DONE;
})

(define_expand "vector_store_<mode>"
  [(set (match_operand:VEC_I 0 "memory_operand" "")
	(match_operand:VEC_I 1 "vint_operand" ""))]
  "TARGET_ALTIVEC"
{
  rs6000_emit_move (operands[0], operands[1], <MODE>mode);
  DONE;
})


;; Vector logical instructions
(define_expand "xor<mode>3"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (xor:VEC_L (match_operand:VEC_L 1 "vlogical_operand" "")
		   (match_operand:VEC_L 2 "vlogical_operand" "")))]
  "TARGET_ALTIVEC || TARGET_VSX"
  "")

(define_expand "ior<mode>3"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (ior:VEC_L (match_operand:VEC_L 1 "vlogical_operand" "")
		   (match_operand:VEC_L 2 "vlogical_operand" "")))]
  "TARGET_ALTIVEC || TARGET_VSX"
  "")

(define_expand "and<mode>3"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (and:VEC_L (match_operand:VEC_L 1 "vlogical_operand" "")
		   (match_operand:VEC_L 2 "vlogical_operand" "")))]
  "TARGET_ALTIVEC || TARGET_VSX"
  "")

(define_expand "one_cmpl<mode>2"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (not:VEC_L (match_operand:VEC_L 1 "vlogical_operand" "")))]
  "TARGET_ALTIVEC || TARGET_VSX"
  "")
  
(define_expand "nor<mode>3"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (not:VEC_L (ior:VEC_L (match_operand:VEC_L 1 "vlogical_operand" "")
			      (match_operand:VEC_L 2 "vlogical_operand" ""))))]
  "TARGET_ALTIVEC || TARGET_VSX"
  "")

(define_expand "andc<mode>3"
  [(set (match_operand:VEC_L 0 "vlogical_operand" "")
        (and:VEC_L (not:VEC_L (match_operand:VEC_L 2 "vlogical_operand" ""))
		   (match_operand:VEC_L 1 "vlogical_operand" "")))]
  "TARGET_ALTIVEC || TARGET_VSX"
  "")

