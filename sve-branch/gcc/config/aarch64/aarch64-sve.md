;; Machine description for AArch64 AdvSIMD architecture.
;; Copyright (C) 2009-2016 Free Software Foundation, Inc.
;; Contributed by ARM Ltd.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful, but
;; WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(define_expand "mov<mode>"
  [(set (match_operand:SVE_ALL 0 "nonimmediate_operand")
	(match_operand:SVE_ALL 1 "general_operand"))]
  "TARGET_SVE"
  {
    if (MEM_P (operands[0]) || MEM_P (operands[1]))
      {
	aarch64_expand_sve_mem_move (operands[0], operands[1], <VPRED>mode,
				     gen_pred_mov<mode>);
	DONE;
      }
    if (CONSTANT_P (operands[1]))
      {
	aarch64_expand_mov_immediate (operands[0], operands[1],
				      gen_vec_duplicate<mode>);
	DONE;
      }
  }
)

;; This will always load and store the elements in little-endian order.
;: We therefore try to restrict its use to spill slots and make sure that
;; all loads and stores to spill slots go through this pattern, so that
;; everything agrees on the local endianness.  In particular:
;;
;; 1) The pattern doesn't accept moves involving memory operands before
;;    register allocation.  The moves should use the richer pred_mov<mode>
;;    pattern instead.
;;
;; 2) Big-endian targets use TARGET_CANNOT_SUBSTITUTE_MEM_EQUIV_P to disallow
;;    memory equivalences during register allocation.  This should ensure
;;    that any memory replacements of registers are fresh spill slots.
;;
;; ??? There is still the hole that this is effectively a REVB operation
;; on big-endian targets, rather than the simple move that the RTL pattern
;; claims.
(define_insn "aarch64_sve_mov<mode>"
  [(set (match_operand:SVE_ALL 0 "aarch64_sve_nonimmediate_operand" "=w, Utr, w, w")
	(match_operand:SVE_ALL 1 "aarch64_sve_general_operand" "Utr, w, w, Dn"))]
  "TARGET_SVE
   && ((lra_in_progress || reload_completed)
       || (register_operand (operands[0], <MODE>mode)
	   && nonmemory_operand (operands[1], <MODE>mode)))"
  "@
   ldr\t%0, %1
   str\t%1, %0
   mov\t%0.d, %1.d
   * return aarch64_output_sve_mov_immediate (operands[1]);"
)

(define_insn "pred_mov<mode>"
  [(set (match_operand:SVE_ALL 0 "nonimmediate_operand" "=w, m")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (match_operand:SVE_ALL 2 "nonimmediate_operand" "m, w")]
	  UNSPEC_PRED_MOVE))]
  "TARGET_SVE
   && (register_operand (operands[0], <MODE>mode)
       || register_operand (operands[2], <MODE>mode))"
  "@
   ld1<Vesize>\t%0.<Vetype>, %1/z, %2
   st1<Vesize>\t%2.<Vetype>, %1, %0"
)

(define_expand "mov<mode>"
  [(set (match_operand:SVE_STRUCT 0 "nonimmediate_operand")
	(match_operand:SVE_STRUCT 1 "general_operand"))]
  "TARGET_SVE"
  {
    if (MEM_P (operands[0]) || MEM_P (operands[1]))
      {
	aarch64_expand_sve_mem_move (operands[0], operands[1], <VPRED>mode,
				     gen_pred_mov<mode>);
	DONE;
      }
    if (CONSTANT_P (operands[1]))
      {
	aarch64_expand_mov_immediate (operands[0], operands[1]);
	DONE;
      }
  }
)

;; See the comments above the SVE_ALL aarch64_sve_mov<mode> for details
;; of the memory handling.
(define_insn_and_split "aarch64_sve_mov<mode>"
  [(set (match_operand:SVE_STRUCT 0 "aarch64_sve_nonimmediate_operand" "=w, Utr, w, w")
	(match_operand:SVE_STRUCT 1 "aarch64_sve_general_operand" "Utr, w, w, Dn"))]
  "TARGET_SVE
   && ((lra_in_progress || reload_completed)
       || (register_operand (operands[0], <MODE>mode)
	   && nonmemory_operand (operands[1], <MODE>mode)))"
  "#"
  "&& reload_completed"
  [(const_int 0)]
  {
    rtx dest = operands[0];
    rtx src = operands[1];
    if (REG_P (dest) && REG_P (src))
      aarch64_simd_emit_reg_reg_move (operands, <VSINGLE>mode, <vector_count>);
    else
      for (unsigned int i = 0; i < <vector_count>; ++i)
	{
	  rtx subdest = simplify_gen_subreg (<VSINGLE>mode, dest, <MODE>mode,
					     i * BYTES_PER_SVE_VECTOR);
	  rtx subsrc = simplify_gen_subreg (<VSINGLE>mode, src, <MODE>mode,
					    i * BYTES_PER_SVE_VECTOR);
	  emit_insn (gen_aarch64_sve_mov<mode> (subdest, subsrc));
	}
    DONE;
  }
  [(set_attr "length" "<insn_length>")]
)

(define_insn_and_split "pred_mov<mode>"
  [(set (match_operand:SVE_STRUCT 0 "aarch64_sve_struct_nonimmediate_operand" "=w, Utx")
	(unspec:SVE_STRUCT
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (match_operand:SVE_STRUCT 2 "aarch64_sve_struct_nonimmediate_operand" "Utx, w")]
	  UNSPEC_PRED_MOVE))]
  "TARGET_SVE
   && (register_operand (operands[0], <MODE>mode)
       || register_operand (operands[2], <MODE>mode))"
  "#"
  "&& reload_completed"
  [(const_int 0)]
  {
    for (unsigned int i = 0; i < <vector_count>; ++i)
      {
	rtx subdest = simplify_gen_subreg (<VSINGLE>mode, operands[0],
					   <MODE>mode,
					   i * BYTES_PER_SVE_VECTOR);
	rtx subsrc = simplify_gen_subreg (<VSINGLE>mode, operands[2],
					  <MODE>mode,
					  i * BYTES_PER_SVE_VECTOR);
	emit_insn (gen_pred_mov<vsingle> (subdest, operands[1], subsrc));
      }
    DONE;
  }
  [(set_attr "length" "<insn_length>")]
)

(define_expand "mov<mode>"
  [(set (match_operand:PRED_ALL 0 "nonimmediate_operand")
	(match_operand:PRED_ALL 1 "general_operand"))]
  "TARGET_SVE"
  {
    if (GET_CODE (operands[0]) == MEM)
      operands[1] = force_reg (<MODE>mode, operands[1]);
  }
)

(define_expand "movmisalign<mode>"
  [(set (match_operand:SVE_ALL 0 "nonimmediate_operand")
	(match_operand:SVE_ALL 1 "general_operand"))]
  "TARGET_SVE"
  {
    /* Equivalent to a normal move for our purpooses.  */
    emit_move_insn (operands[0], operands[1]);
    DONE;
  }
)

(define_insn "maskload<mode><vpred>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL
	  [(match_operand:SVE_ALL 1 "memory_operand" "m")
	   (match_operand:<VPRED> 2 "register_operand" "Upl")]
	  UNSPEC_LD1_SVE))]
  "TARGET_SVE"
  "ld1<Vesize>\t%0.<Vetype>, %2/z, %1"
)

(define_insn "maskstore<mode><vpred>"
  [(set (match_operand:SVE_ALL 0 "memory_operand" "+m")
	(unspec:SVE_ALL [(match_operand:SVE_ALL 1 "register_operand" "w")
			 (match_operand:<VPRED> 2 "register_operand" "Upl")
			 (match_dup 0)]
			UNSPEC_ST1_SVE))]
  "TARGET_SVE"
  "st1<Vesize>\t%1.<Vetype>, %2, %0"
)

(define_insn "*aarch64_sve_mov<mode>"
  [(set (match_operand:PRED_ALL 0 "nonimmediate_operand" "=Upa, m, Upa, Upa, Upa")
	(match_operand:PRED_ALL 1 "general_operand" "Upa, Upa, m, Dz, Dm"))]
  "TARGET_SVE
   && (register_operand (operands[0], <MODE>mode)
       || register_operand (operands[1], <MODE>mode))"
  "@
   mov\t%0.b, %1.b
   str\t%1, %0
   ldr\t%0, %1
   pfalse\t%0.b
   * return aarch64_output_ptrue (<MODE>mode, '<Vetype>');"
)

(define_expand "vec_duplicate<mode>"
  [(parallel
    [(set (match_operand:SVE_ALL 0 "register_operand")
	  (vec_duplicate:SVE_ALL
	    (match_operand:<VEL> 1 "aarch64_sve_dup_operand")))
     (clobber (scratch:<VPRED>))])]
  "TARGET_SVE"
  {
    if (MEM_P (operands[1]))
      {
	rtx ptrue = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
	emit_insn (gen_sve_ld1r<mode> (operands[0], ptrue, operands[1],
				       CONST0_RTX (<MODE>mode)));
	DONE;
      }
  }
)

;; Accept memory operands for the benefit of combine, and also in case
;; the scalar input gets spilled to memory during RA.  We want to split
;; the load at the first opportunity in order to allow the PTRUE to be
;; optimized with surrounding code.
(define_insn_and_split "*vec_duplicate<mode>_reg"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w, w, w")
	(vec_duplicate:SVE_ALL
	  (match_operand:<VEL> 1 "aarch64_sve_dup_operand" "r, w, Utw")))
   (clobber (match_scratch:<VPRED> 2 "=X, X, Upl"))]
  "TARGET_SVE"
  "@
   mov\t%0.<Vetype>, %<vwcore>1
   mov\t%0.<Vetype>, %<Vetype>1
   #"
  "&& MEM_P (operands[1])"
  [(const_int 0)]
  {
    if (GET_CODE (operands[2]) == SCRATCH)
      operands[2] = gen_reg_rtx (<VPRED>mode);
    emit_move_insn (operands[2], CONSTM1_RTX (<VPRED>mode));
    emit_insn (gen_sve_ld1r<mode> (operands[0], operands[2], operands[1],
				   CONST0_RTX (<MODE>mode)));
    DONE;
  }
  [(set_attr "length" "4,4,8")]
)

;; This is used for vec_duplicate<mode>s from memory, but can also
;; be used by combine to optimize selects of a a vec_duplicate<mode>
;; with zero.
(define_insn "sve_ld1r<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (vec_duplicate:SVE_ALL
	     (match_operand:<VEL> 2 "aarch64_sve_ld1r_operand" "Utw"))
	   (match_operand:SVE_ALL 3 "aarch64_simd_imm_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "ld1r<Vesize>\t%0.<Vetype>, %1/z, %2"
)

;; Implement a predicate broadcast by shifting the low bit of the scalar
;; input into the top bit and using a WHILELO.  An alternative would be to
;; duplicate the input and do a compare with zero.
(define_expand "vec_duplicate<mode>"
  [(set (match_operand:PRED_ALL 0 "register_operand")
	(vec_duplicate:PRED_ALL (match_operand 1 "register_operand")))]
  "TARGET_SVE"
  {
    rtx tmp = gen_reg_rtx (DImode);
    rtx op1 = gen_lowpart (DImode, operands[1]);
    emit_insn (gen_ashldi3 (tmp, op1, gen_int_mode (63, DImode)));
    emit_insn (gen_while_ultdi<mode> (operands[0], const0_rtx, tmp));
    DONE;
  }
)

(define_insn "vec_series<mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, w")
	(vec_series:SVE_I
	  (match_operand:<VEL> 1 "aarch64_sve_index_operand" "r, Di, r")
	  (match_operand:<VEL> 2 "aarch64_sve_index_operand" "r, r, Di")))]
  "TARGET_SVE"
  "@
   index\t%0.<Vetype>, %<vw>1, %<vw>2
   index\t%0.<Vetype>, #%1, %<vw>2
   index\t%0.<Vetype>, %<vw>1, #%2"
)

(define_insn "*vec_series<mode>_plus"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(plus:SVE_I
	  (vec_duplicate:SVE_I
	    (match_operand:<VEL> 1 "register_operand" "r"))
	  (match_operand:SVE_I 2 "immediate_operand")))]
  "TARGET_SVE && aarch64_check_zero_based_sve_index_immediate (operands[2])"
  {
    operands[2] = aarch64_check_zero_based_sve_index_immediate (operands[2]);
    return "index\t%0.<Vetype>, %<vw>1, #%2";
  }
)

(define_expand "vec_load_lanes<vrl2><mode>"
  [(set (match_operand:<VRL2> 0 "register_operand")
	(unspec:<VRL2>
	  [(match_dup 2)
	   (match_operand:<VRL2> 1 "memory_operand")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)]
	  UNSPEC_LD2))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "vec_mask_load_lanes<vrl2><mode>"
  [(set (match_operand:<VRL2> 0 "register_operand" "=w")
	(unspec:<VRL2>
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:<VRL2> 1 "memory_operand" "m")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)]
	  UNSPEC_LD2))]
  "TARGET_SVE"
  "ld2<Vesize>\t{%S0.<Vetype> - %T0.<Vetype>}, %2/z, %1"
)

(define_expand "vec_load_lanes<vrl3><mode>"
  [(set (match_operand:<VRL3> 0 "register_operand")
	(unspec:<VRL3>
	  [(match_dup 2)
	   (match_operand:<VRL3> 1 "memory_operand")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)]
	  UNSPEC_LD3))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "vec_mask_load_lanes<vrl3><mode>"
  [(set (match_operand:<VRL3> 0 "register_operand" "=w")
	(unspec:<VRL3>
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:<VRL3> 1 "memory_operand" "m")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)]
	  UNSPEC_LD3))]
  "TARGET_SVE"
  "ld3<Vesize>\t{%S0.<Vetype> - %U0.<Vetype>}, %2/z, %1"
)

(define_expand "vec_load_lanes<vrl4><mode>"
  [(set (match_operand:<VRL4> 0 "register_operand")
	(unspec:<VRL4>
	  [(match_dup 2)
	   (match_operand:<VRL4> 1 "memory_operand")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)]
	  UNSPEC_LD4))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "vec_mask_load_lanes<vrl4><mode>"
  [(set (match_operand:<VRL4> 0 "register_operand" "=w")
	(unspec:<VRL4>
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:<VRL4> 1 "memory_operand" "m")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)]
	  UNSPEC_LD4))]
  "TARGET_SVE"
  "ld4<Vesize>\t{%S0.<Vetype> - %V0.<Vetype>}, %2/z, %1"
)

;; This is always a full update, so the (match_dup 0) is redundant.
;; There doesn't seem to be any obvious benefit to treating the all-true
;; case differently though.  In particular, it's very unlikely that we'll
;; only find out during RTL that a store_lanes is dead.
(define_expand "vec_store_lanes<vrl2><mode>"
  [(set (match_operand:<VRL2> 0 "memory_operand")
	(unspec:<VRL2>
	  [(match_dup 2)
	   (match_operand:<VRL2> 1 "register_operand")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)
	   (match_dup 0)]
	  UNSPEC_ST2))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "vec_mask_store_lanes<vrl2><mode>"
  [(set (match_operand:<VRL2> 0 "memory_operand" "+m")
	(unspec:<VRL2>
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:<VRL2> 1 "register_operand" "w")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)
	   (match_dup 0)]
	  UNSPEC_ST2))]
  "TARGET_SVE"
  "st2<Vesize>\t{%S1.<Vetype> - %T1.<Vetype>}, %2, %0"
)

;; See the comment above vec_store_lanes<vrl2><mode>.
(define_expand "vec_store_lanes<vrl3><mode>"
  [(set (match_operand:<VRL3> 0 "memory_operand")
	(unspec:<VRL3>
	  [(match_dup 2)
	   (match_operand:<VRL3> 1 "register_operand")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)
	   (match_dup 0)]
	  UNSPEC_ST3))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "vec_mask_store_lanes<vrl3><mode>"
  [(set (match_operand:<VRL3> 0 "memory_operand" "+m")
	(unspec:<VRL3>
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:<VRL3> 1 "register_operand" "w")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)
	   (match_dup 0)]
	  UNSPEC_ST3))]
  "TARGET_SVE"
  "st3<Vesize>\t{%S1.<Vetype> - %U1.<Vetype>}, %2, %0"
)

;; See the comment above vec_store_lanes<vrl2><mode>.
(define_expand "vec_store_lanes<vrl4><mode>"
  [(set (match_operand:<VRL4> 0 "memory_operand")
	(unspec:<VRL4>
	  [(match_dup 2)
	   (match_operand:<VRL4> 1 "register_operand")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)
	   (match_dup 0)]
	  UNSPEC_ST4))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "vec_mask_store_lanes<vrl4><mode>"
  [(set (match_operand:<VRL4> 0 "memory_operand" "+m")
	(unspec:<VRL4>
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:<VRL4> 1 "register_operand" "w")
	   (unspec:SVE_ALL [(const_int 0)] UNSPEC_VSTRUCTDUMMY)
	   (match_dup 0)]
	  UNSPEC_ST4))]
  "TARGET_SVE"
  "st4<Vesize>\t{%S1.<Vetype> - %V1.<Vetype>}, %2, %0"
)

(define_expand "vec_perm_const<mode>"
  [(match_operand:SVE_ALL 0 "register_operand")
   (match_operand:SVE_ALL 1 "register_operand")
   (match_operand:SVE_ALL 2 "register_operand")
   (match_operand:<V_INT_EQUIV> 3)]
  "TARGET_SIMD"
{
  unsigned int nunits;
  if (GET_MODE_NUNITS (<MODE>mode).is_constant (&nunits)
      && aarch64_expand_vec_perm_const (operands[0], operands[1],
					operands[2], operands[3], nunits))
    DONE;
  else
    FAIL;
})

(define_expand "vec_perm<mode>"
  [(match_operand:SVE_ALL 0 "register_operand")
   (match_operand:SVE_ALL 1 "register_operand")
   (match_operand:SVE_ALL 2 "register_operand")
   (match_operand:<V_INT_EQUIV> 3 "aarch64_sve_vec_perm_operand")]
  "TARGET_SVE && GET_MODE_NUNITS (<MODE>mode).is_constant ()"
  {
    aarch64_expand_sve_vec_perm (operands[0], operands[1],
				 operands[2], operands[3]);
    DONE;
  }
)

(define_insn "sve_tbl1<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL
	  [(match_operand:SVE_ALL 1 "register_operand" "w")
	   (match_operand:<V_INT_EQUIV> 2 "register_operand" "w")]
	  UNSPEC_TBL))]
  "TARGET_SVE"
  "tbl\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
)

(define_insn "sve_<perm_insn><perm_hilo><mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL [(match_operand:SVE_ALL 1 "register_operand" "w")
			 (match_operand:SVE_ALL 2 "register_operand" "w")]
			PERMUTE))]
  "TARGET_SVE"
  "<perm_insn><perm_hilo>\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
)

(define_insn "sve_rev64<mode>"
  [(set (match_operand:SVE_BHS 0 "register_operand" "=w")
	(unspec:SVE_BHS [(match_operand:SVE_BHS 1 "register_operand" "w")]
			UNSPEC_REV64))]
  "TARGET_SVE"
  "rev<Vesize>\t%0.d, %1.d"
)

(define_insn "sve_rev32<mode>"
  [(set (match_operand:SVE_BH 0 "register_operand" "=w")
	(unspec:SVE_BH [(match_operand:SVE_BH 1 "register_operand" "w")]
		       UNSPEC_REV32))]
  "TARGET_SVE"
  "rev<Vesize>\t%0.s, %1.s"
)

(define_insn "sve_rev16v32qi"
  [(set (match_operand:V32QI 0 "register_operand" "=w")
	(unspec:V32QI [(match_operand:V32QI 1 "register_operand" "w")]
		      UNSPEC_REV16))]
  "TARGET_SVE"
  "revb\t%0.h, %1.h"
)

(define_insn "sve_dup_lane<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(vec_duplicate:SVE_ALL
	  (vec_select:<VEL>
	    (match_operand:SVE_ALL 1 "register_operand" "w")
	    (parallel [(match_operand:SI 2 "const_int_operand" "i")]))))]
  "TARGET_SVE
   && IN_RANGE (INTVAL (operands[2]) * GET_MODE_SIZE (<VEL>mode), 0, 63)"
  "dup\t%0.<Vetype>, %1.<Vetype>[%2]"
)

;; Note that the immediate (third) operand is the lane index not
;; the byte index.
(define_insn "aarch64_ext<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL [(match_operand:SVE_ALL 1 "register_operand" "w")
			 (match_operand:SVE_ALL 2 "register_operand" "w")
			 (match_operand:SI 3 "const_int_operand" "i")]
			UNSPEC_EXT))]
  "TARGET_SVE
   && IN_RANGE (INTVAL (operands[3]) * GET_MODE_SIZE (<VEL>mode), 0, 255)"
  {
    operands[3] = GEN_INT (INTVAL (operands[3]) * GET_MODE_SIZE (<VEL>mode));
    return "ext\\t%0.b, %1.b, %2.b, #%3";
  }
)

(define_insn "add<mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, w, w")
	(plus:SVE_I
	  (match_operand:SVE_I 1 "register_operand" "%0, 0, 0, w")
	  (match_operand:SVE_I 2 "aarch64_sve_add_operand" "vsa, vsn, vsi, w")))]
  "TARGET_SVE"
  "@
   add\t%0.<Vetype>, %0.<Vetype>, #%2
   sub\t%0.<Vetype>, %0.<Vetype>, #%N2
   * return aarch64_output_sve_inc_dec_immediate (\"%0.<Vetype>\", operands[2]);
   add\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
)

(define_insn "sub<mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w")
	(minus:SVE_I
	  (match_operand:SVE_I 1 "aarch64_sve_arith_operand" "w, vsa")
	  (match_operand:SVE_I 2 "register_operand" "w, 0")))]
  "TARGET_SVE"
  "@
   sub\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>
   subr\t%0.<Vetype>, %0.<Vetype>, #%1"
)

(define_expand "mul<mode>3"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_dup 3)
	   (mult:SVE_I
	     (match_operand:SVE_I 1 "register_operand")
	     (match_operand:SVE_I 2 "aarch64_sve_mul_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; We don't actually need the predicate for the first operand, but using Upa
;; or X isn't likely to gain much and would make the instruction seem less
;; uniform to the register allocator.
(define_insn "*mul<mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (mult:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "%0, 0")
	     (match_operand:SVE_I 3 "aarch64_sve_mul_operand" "vsm, w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   mul\t%0.<Vetype>, %0.<Vetype>, #%3
   mul\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
)

(define_insn "*madd<mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w")
	(plus:SVE_I
	  (unspec:SVE_I
	    [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand" "%0, w")
			 (match_operand:SVE_I 3 "register_operand" "w, w"))]
	    UNSPEC_MERGE_PTRUE)
	  (match_operand:SVE_I 4 "register_operand" "w, 0")))]
  "TARGET_SVE"
  "@
   mad\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   mla\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
)

(define_insn "*msub<mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w")
	(minus:SVE_I
	  (match_operand:SVE_I 4 "register_operand" "w, 0")
	  (unspec:SVE_I
	    [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand" "%0, w")
			 (match_operand:SVE_I 3 "register_operand" "w, w"))]
	    UNSPEC_MERGE_PTRUE)))]
  "TARGET_SVE"
  "@
   msb\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   mls\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
)

(define_expand "<optab><mode>2"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_dup 2)
	   (SVE_INT_UNARY:SVE_I (match_operand:SVE_I 1 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*<optab><mode>2"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (SVE_INT_UNARY:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<sve_int_op>\t%0.<Vetype>, %1/m, %2.<Vetype>"
)

(define_insn "<optab><mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w")
	(LOGICAL:SVE_I
	  (match_operand:SVE_I 1 "register_operand" "%0, w")
	  (match_operand:SVE_I 2 "aarch64_sve_logical_operand" "vsl, w")))]
  "TARGET_SVE"
  "@
   <logical>\t%0.<Vetype>, %0.<Vetype>, #%C2
   <logical>\t%0.d, %1.d, %2.d"
)

(define_insn "*<optab><mode>3"
  [(set (match_operand:SVE_F 0 "register_operand" "=w")
	(unspec:SVE_F [(match_operand:SVE_F 1 "register_operand" "w")
		       (match_operand:SVE_F 2 "register_operand" "w")]
		      LOGICALF))]
  "TARGET_SVE"
  "<logicalf_op>\t%0.d, %1.d, %2.d"
)

;; REG_EQUAL notes on "not<mode>3" should ensure that we can generate
;; this pattern even though the NOT instruction itself is predicated.
(define_insn "bic<mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(and:SVE_I
	  (not:SVE_I (match_operand:SVE_I 1 "register_operand" "w"))
	  (match_operand:SVE_I 2 "register_operand" "w")))]
  "TARGET_SVE"
  "bic\t%0.d, %2.d, %1.d"
)

(define_insn "and<mode>3"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL (match_operand:PRED_ALL 1 "register_operand" "Upa")
		      (match_operand:PRED_ALL 2 "register_operand" "Upa")))]
  "TARGET_SVE"
  "and\t%0.b, %1/z, %1.b, %2.b"
)

(define_expand "<optab><mode>3"
  [(set (match_operand:PRED_ALL 0 "register_operand")
	(and:PRED_ALL
	  (LOGICAL_OR:PRED_ALL
	    (match_operand:PRED_ALL 1 "register_operand")
	    (match_operand:PRED_ALL 2 "register_operand"))
	  (match_dup 3)))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<MODE>mode, CONSTM1_RTX (<MODE>mode));
  }
)

(define_insn "pred_<optab><mode>3"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL
	  (LOGICAL:PRED_ALL
	    (match_operand:PRED_ALL 2 "register_operand" "Upa")
	    (match_operand:PRED_ALL 3 "register_operand" "Upa"))
	  (match_operand:PRED_ALL 1 "register_operand" "Upa")))]
  "TARGET_SVE"
  "<logical>\t%0.b, %1/z, %2.b, %3.b"
)

;; Perform a logical operation on the active bits of operands 2 and 3,
;; using operand 1 as the GP.  Store the result in operand 0 and set the flags
;; in the same way as for PTEST.  The (and ...) in the UNSPEC_PTEST_PTRUE is
;; logically redundant, but means that the tested value is structurally
;; equivalent to rhs of the second set.
(define_insn "*pred_<optab><mode>3_cc"
  [(set (reg:CC CC_REGNUM)
	(compare:CC
	  (unspec:SI [(match_operand:PRED_ALL 1 "register_operand" "Upa")
		      (and:PRED_ALL
			(LOGICAL:PRED_ALL
			  (match_operand:PRED_ALL 2 "register_operand" "Upa")
			  (match_operand:PRED_ALL 3 "register_operand" "Upa"))
			(match_dup 1))]
		     UNSPEC_PTEST_PTRUE)
	  (const_int 0)))
   (set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL (LOGICAL:PRED_ALL (match_dup 2) (match_dup 3))
		      (match_dup 1)))]
  "TARGET_SVE"
  "<logical>s\t%0.b, %1/z, %2.b, %3.b"
)

(define_expand "one_cmpl<mode>2"
  [(set (match_operand:PRED_ALL 0 "register_operand")
	(and:PRED_ALL
	  (not:PRED_ALL (match_operand:PRED_ALL 1 "register_operand"))
	  (match_dup 2)))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<MODE>mode, CONSTM1_RTX (<MODE>mode));
  }
)

(define_insn "*one_cmpl<mode>3"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL
	  (not:PRED_ALL (match_operand:PRED_ALL 2 "register_operand" "Upa"))
	  (match_operand:PRED_ALL 1 "register_operand" "Upa")))]
  "TARGET_SVE"
  "not\t%0.b, %1/z, %2.b"
)

(define_insn "*<nlogical><mode>3"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL
	  (NLOGICAL:PRED_ALL
	    (not:PRED_ALL (match_operand:PRED_ALL 2 "register_operand" "Upa"))
	    (match_operand:PRED_ALL 3 "register_operand" "Upa"))
	  (match_operand:PRED_ALL 1 "register_operand" "Upa")))]
  "TARGET_SVE"
  "<nlogical>\t%0.b, %1/z, %3.b, %2.b"
)

(define_insn "*<logical_nn><mode>3"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL
	  (NLOGICAL:PRED_ALL
	    (not:PRED_ALL (match_operand:PRED_ALL 2 "register_operand" "Upa"))
	    (not:PRED_ALL (match_operand:PRED_ALL 3 "register_operand" "Upa")))
	  (match_operand:PRED_ALL 1 "register_operand" "Upa")))]
  "TARGET_SVE"
  "<logical_nn>\t%0.b, %1/z, %2.b, %3.b"
)

(define_expand "v<optab><mode>3"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_dup 3)
	   (ASHIFT:SVE_I
	     (match_operand:SVE_I 1 "register_operand")
	     (match_operand:SVE_I 2 "aarch64_sve_<lr>shift_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*v<optab><mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (ASHIFT:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "w, 0")
	     (match_operand:SVE_I 3 "aarch64_sve_<lr>shift_operand" "D<lr>, w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   <shift>\t%0.<Vetype>, %2.<Vetype>, #%3
   <shift>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
)

(define_expand "<ASHIFT:optab><mode>3"
  [(set (match_operand:SVE_I 0 "register_operand")
	(ASHIFT:SVE_I (match_operand:SVE_I 1 "register_operand")
		      (match_operand:<VEL> 2 "general_operand")))]
  "TARGET_SVE"
  {
    rtx amount;
    if (CONST_INT_P (operands[2]))
      {
	amount = gen_const_vec_duplicate (<MODE>mode, operands[2]);
	if (!aarch64_sve_<lr>shift_operand (operands[2], <MODE>mode))
	  amount = force_reg (<MODE>mode, amount);
      }
    else
      {
	amount = gen_reg_rtx (<MODE>mode);
	emit_insn (gen_vec_duplicate<mode> (amount,
					    convert_to_mode (<VEL>mode,
							     operands[2], 0)));
      }
    emit_insn (gen_v<optab><mode>3 (operands[0], operands[1], amount));
    DONE;
  }
)

;; Test all bits of operand 1.  Operand 0 is a PTRUE GP.
;;
;; Using UNSPEC_PTEST_PTRUE allows combine patterns to assume that the GP
;; is a PTRUE even if the optimizers haven't yet been able to propagate
;; the constant.  We should use a separate unspec code for PTESTs involving
;; GPs that might not be PTRUEs.
(define_insn "ptest_ptrue<mode>"
  [(set (reg:CC CC_REGNUM)
	(compare:CC
	  (unspec:SI [(match_operand:PRED_ALL 0 "register_operand" "Upa")
		      (match_operand:PRED_ALL 1 "register_operand" "Upa")]
		     UNSPEC_PTEST_PTRUE)
	  (const_int 0)))]
  "TARGET_SVE"
  "ptest\t%0, %1.b"
)

(define_insn "while_ult<GPI:mode><PRED_ALL:mode>"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(unspec:PRED_ALL [(match_operand:GPI 1 "aarch64_reg_or_zero" "rZ")
			  (match_operand:GPI 2 "aarch64_reg_or_zero" "rZ")]
			 UNSPEC_WHILE_LO))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_SVE"
  "whilelo\t%0.<PRED_ALL:Vetype>, %<w>1, %<w>2"
)

(define_insn_and_split "while_ult<GPI:mode><PRED_ALL:mode>_cc"
  [(set (reg:CC CC_REGNUM)
	(compare:CC
	  (unspec:SI [(match_operand:PRED_ALL 1)
		      (unspec:PRED_ALL
			[(match_operand:GPI 2 "aarch64_reg_or_zero" "rZ")
			 (match_operand:GPI 3 "aarch64_reg_or_zero" "rZ")]
			UNSPEC_WHILE_LO)]
		     UNSPEC_PTEST_PTRUE)
	  (const_int 0)))
   (set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(unspec:PRED_ALL [(match_dup 2)
			  (match_dup 3)]
			 UNSPEC_WHILE_LO))]
  "TARGET_SVE"
  "whilelo\t%0.<PRED_ALL:Vetype>, %<w>2, %<w>3"
  ;; Force the compiler to drop the unused predicate operand, so that we
  ;; don't have an unnecessary PTRUE.
  "&& !CONSTANT_P (operands[1])"
  [(const_int 0)]
  {
    emit_insn (gen_while_ult<GPI:mode><PRED_ALL:mode>_cc
	       (operands[0], CONSTM1_RTX (<MODE>mode),
		operands[2], operands[3]));
    DONE;
  }
)

;; Predicated comparison.
(define_insn "*vec_cmp<cmp_op>_<mode>"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(unspec:<VPRED>
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (match_operand:SVE_I 2 "register_operand" "w, w")
	   (match_operand:SVE_I 3 "aarch64_sve_cmp_<imm_con>_operand" "w, <imm_con>")]
	  SVE_COND_INT_CMP))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_SVE"
  "@
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #%3"
)

;; Predicated comparison in which only the flags result is interesting.
(define_insn "*vec_cmp<cmp_op>_<mode>_ptest"
  [(set (reg:CC CC_REGNUM)
	(compare:CC
	  (unspec:SI
	    [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	     (unspec:<VPRED>
	       [(match_operand:SVE_I 2 "register_operand" "w, w")
		(match_operand:SVE_I 3 "aarch64_sve_cmp_<imm_con>_operand" "w, <imm_con>")
		(match_dup 3)]
	       SVE_COND_INT_CMP)]
	    UNSPEC_PTEST_PTRUE)
	  (const_int 0)))
   (clobber (match_scratch:<VPRED> 0 "=Upa, Upa"))]
  "TARGET_SVE"
  "@
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #%3"
)

;; Predicated comparison in which both the flag and predicate results
;; are interesting.
(define_insn "*vec_cmp<cmp_op>_<mode>_cc"
  [(set (reg:CC CC_REGNUM)
	(compare:CC
	  (unspec:SI
	    [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	     (unspec:<VPRED>
	       [(match_dup 1)
		(match_operand:SVE_I 2 "register_operand" "w, w")
		(match_operand:SVE_I 3 "aarch64_sve_cmp_<imm_con>_operand" "w, <imm_con>")]
	       SVE_COND_INT_CMP)]
	    UNSPEC_PTEST_PTRUE)
	  (const_int 0)))
   (set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(unspec:<VPRED>
	  [(match_dup 1)
	   (match_dup 2)
	   (match_dup 3)]
	  SVE_COND_INT_CMP))]
  "TARGET_SVE"
  "@
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #%3"
)

(define_insn "*vec_fcm<cmp_op><mode>"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(unspec:<VPRED>
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (match_operand:SVE_F 2 "register_operand" "w, w")
	   (match_operand:SVE_F 3 "aarch64_simd_reg_or_zero" "w, Dz")]
	  SVE_COND_FP_CMP))]
  "TARGET_SVE"
  "@
   fcm<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>
   fcm<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #0.0"
)

(define_insn "*vec_fcmuo<mode>"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa")
	(unspec:<VPRED>
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (match_operand:SVE_F 2 "register_operand" "w")
	   (match_operand:SVE_F 3 "register_operand" "w")]
	  UNSPEC_COND_UO))]
  "TARGET_SVE"
  "fcmuo\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; vcond_mask operand order: true, false, mask
;; UNSPEC_SEL operand order: mask, true, false (as for VEC_COND_EXPR)
;; SEL operand order:        mask, true, false
(define_insn "vcond_mask_<mode><vpred>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 3 "register_operand" "Upa")
	   (match_operand:SVE_ALL 1 "register_operand" "w")
	   (match_operand:SVE_ALL 2 "register_operand" "w")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "sel\t%0.<Vetype>, %3, %1.<Vetype>, %2.<Vetype>"
)

(define_insn "*sel_dup<mode>_const"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (match_operand:SVE_I 2 "aarch64_sve_dup_immediate")
	   (match_operand:SVE_I 3 "aarch64_simd_imm_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "mov\t%0.<Vetype>, %1/z, #%2"
)

(define_expand "vcond<mode><v_int_equiv>"
  [(set (match_operand:SVE_ALL 0 "register_operand")
	(if_then_else:SVE_ALL
	  (match_operator 3 "comparison_operator"
	    [(match_operand:<V_INT_EQUIV> 4 "register_operand")
	     (match_operand:<V_INT_EQUIV> 5 "nonmemory_operand")])
	  (match_operand:SVE_ALL 1 "register_operand")
	  (match_operand:SVE_ALL 2 "register_operand")))]
  "TARGET_SVE"
  {
    aarch64_expand_sve_vcond (<MODE>mode, <V_INT_EQUIV>mode, operands);
    DONE;
  }
)

(define_expand "vcond<mode><v_fp_equiv>"
  [(set (match_operand:SVE_SD 0 "register_operand")
	(if_then_else:SVE_SD
	  (match_operator 3 "comparison_operator"
	    [(match_operand:<V_FP_EQUIV> 4 "register_operand")
	     (match_operand:<V_FP_EQUIV> 5 "aarch64_simd_reg_or_zero")])
	  (match_operand:SVE_SD 1 "register_operand")
	  (match_operand:SVE_SD 2 "register_operand")))]
  "TARGET_SVE"
  {
    aarch64_expand_sve_vcond (<MODE>mode, <V_FP_EQUIV>mode, operands);
    DONE;
  }
)

(define_expand "vcondu<mode><v_int_equiv>"
  [(set (match_operand:SVE_ALL 0 "register_operand")
	(if_then_else:SVE_ALL
	  (match_operator 3 "comparison_operator"
	    [(match_operand:<V_INT_EQUIV> 4 "register_operand")
	     (match_operand:<V_INT_EQUIV> 5 "nonmemory_operand")])
	  (match_operand:SVE_ALL 1 "register_operand")
	  (match_operand:SVE_ALL 2 "register_operand")))]
  "TARGET_SVE"
  {
    aarch64_expand_sve_vcond (<MODE>mode, <V_INT_EQUIV>mode, operands);
    DONE;
  }
)

(define_expand "vec_cmp<mode><vpred>"
  [(parallel
    [(set (match_operand:<VPRED> 0 "register_operand")
	  (match_operator:<VPRED> 1 "comparison_operator"
	    [(match_operand:SVE_I 2 "register_operand")
	     (match_operand:SVE_I 3 "nonmemory_operand")]))
     (clobber (reg:CC CC_REGNUM))])]
  "TARGET_SVE"
  {
    aarch64_expand_sve_vec_cmp_int (operands[0], GET_CODE (operands[1]),
				    operands[2], operands[3]);
    DONE;
  }
)

(define_expand "vec_cmpu<mode><vpred>"
  [(parallel
    [(set (match_operand:<VPRED> 0 "register_operand")
	  (match_operator:<VPRED> 1 "comparison_operator"
	    [(match_operand:SVE_I 2 "register_operand")
	     (match_operand:SVE_I 3 "nonmemory_operand")]))
     (clobber (reg:CC CC_REGNUM))])]
  "TARGET_SVE"
  {
    aarch64_expand_sve_vec_cmp_int (operands[0], GET_CODE (operands[1]),
				    operands[2], operands[3]);
    DONE;
  }
)

(define_expand "vec_cmp<mode><vpred>"
  [(set (match_operand:<VPRED> 0 "register_operand")
	(match_operator:<VPRED> 1 "comparison_operator"
	  [(match_operand:SVE_F 2 "register_operand")
	   (match_operand:SVE_F 3 "aarch64_simd_reg_or_zero")]))]
  "TARGET_SVE"
  {
    aarch64_expand_sve_vec_cmp_float (operands[0], GET_CODE (operands[1]),
				      operands[2], operands[3], false);
    DONE;
  }
)

(define_expand "cbranch<mode>4"
  [(set (pc)
	(if_then_else
	  (match_operator 0 "aarch64_equality_operator"
	    [(match_operand:PRED_ALL 1 "register_operand")
	     (match_operand:PRED_ALL 2 "aarch64_simd_reg_or_zero")])
	  (label_ref (match_operand 3 ""))
	  (pc)))]
  ""
  {
    rtx ptrue = force_reg (<MODE>mode, CONSTM1_RTX (<MODE>mode));
    rtx pred;
    if (operands[2] == CONST0_RTX (<MODE>mode))
      pred = operands[1];
    else
      {
	pred = gen_reg_rtx (<MODE>mode);
	emit_insn (gen_pred_xor<mode>3 (pred, ptrue, operands[1],
					operands[2]));
      }
    emit_insn (gen_ptest_ptrue<mode> (ptrue, pred));
    operands[1] = gen_rtx_REG (CCmode, CC_REGNUM);
    operands[2] = const0_rtx;
  }
)

;; Max/Min operations.
(define_expand "<su><maxmin><mode>3"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_dup 3)
	   (MAXMIN:SVE_I (match_operand:SVE_I 1 "register_operand")
			 (match_operand:SVE_I 2 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*<su><maxmin><mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (MAXMIN:SVE_I (match_operand:SVE_I 2 "register_operand" "%0")
			 (match_operand:SVE_I 3 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<su><maxmin>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
)

(define_expand "<su><maxmin><mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (FMAXMIN:SVE_F (match_operand:SVE_F 1 "register_operand")
			  (match_operand:SVE_F 2 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*<su><maxmin><mode>3"
  [(set (match_operand:SVE_F 0 "register_operand" "=w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (FMAXMIN:SVE_F (match_operand:SVE_F 2 "register_operand" "%0")
			  (match_operand:SVE_F 3 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "f<maxmin>nm\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
)

(define_expand "<maxmin_uns><mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (unspec:SVE_F [(match_operand:SVE_F 1 "register_operand")
			  (match_operand:SVE_F 2 "register_operand")]
			 FMAXMIN_UNS)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*<maxmin_uns><mode>3"
  [(set (match_operand:SVE_F 0 "register_operand" "=w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (unspec:SVE_F [(match_operand:SVE_F 2 "register_operand" "%0")
			  (match_operand:SVE_F 3 "register_operand" "w")]
			 FMAXMIN_UNS)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<maxmin_uns_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
)

(define_expand "reduc_plus_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand")
	(unspec:<VEL> [(match_dup 2)
		       (match_operand:SVE_I 1 "register_operand")]
		      UNSPEC_ADDV))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*reduc_plus_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(unspec:<VEL> [(match_operand:<VPRED> 1 "register_operand" "Upl")
		       (match_operand:SVE_I 2 "register_operand" "w")]
		      UNSPEC_ADDV))]
  "TARGET_SVE"
  "uaddv\t%d0, %1, %2.<Vetype>"
)

(define_expand "reduc_plus_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand")
	(unspec:<VEL> [(match_dup 2)
		       (match_operand:SVE_F 1 "register_operand")]
		      UNSPEC_FADDV))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*reduc_plus_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(unspec:<VEL> [(match_operand:<VPRED> 1 "register_operand" "Upl")
		       (match_operand:SVE_F 2 "register_operand" "w")]
		      UNSPEC_FADDV))]
  "TARGET_SVE"
  "faddv\t%<Vetype>0, %1, %2.<Vetype>"
)

(define_expand "reduc_<maxmin_uns>_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand")
	(unspec:<VEL> [(match_dup 2)
		       (match_operand:SVE_I 1 "register_operand")]
		      MAXMINV))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*reduc_<maxmin_uns>_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(unspec:<VEL> [(match_operand:<VPRED> 1 "register_operand" "Upl")
		       (match_operand:SVE_I 2 "register_operand" "w")]
		      MAXMINV))]
  "TARGET_SVE"
  "<maxmin_uns_op>v\t%<Vetype>0, %1, %2.<Vetype>"
)

(define_expand "reduc_<maxmin_uns>_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand")
	(unspec:<VEL> [(match_dup 2)
		       (match_operand:SVE_F 1 "register_operand")]
		      FMAXMINV))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*reduc_<maxmin_uns>_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(unspec:<VEL> [(match_operand:<VPRED> 1 "register_operand" "Upl")
		       (match_operand:SVE_F 2 "register_operand" "w")]
		      FMAXMINV))]
  "TARGET_SVE"
  "<maxmin_uns_op>v\t%<Vetype>0, %1, %2.<Vetype>"
)

(define_expand "reduc_<bit_reduc>_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand")
	(unspec:<VEL> [(match_dup 2)
		       (match_operand:SVE_I 1 "register_operand")]
		      BITWISEV))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*reduc_<bit_reduc>_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(unspec:<VEL> [(match_operand:<VPRED> 1 "register_operand" "Upl")
		       (match_operand:SVE_I 2 "register_operand" "w")]
		      BITWISEV))]
  "TARGET_SVE"
  "<bit_reduc_op>\t%<Vetype>0, %1, %2.<Vetype>"
)

(define_expand "add<mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (plus:SVE_F
	     (match_operand:SVE_F 1 "register_operand")
	     (match_operand:SVE_F 2 "aarch64_sve_float_arith_with_sub_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*add<mode>3"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	   (plus:SVE_F
	      (match_operand:SVE_F 2 "register_operand" "%0, 0, w")
	      (match_operand:SVE_F 3 "aarch64_sve_float_arith_with_sub_operand" "vfa, vfn, w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, #%N3
   fadd\t%0.<Vetype>, %2.<Vetype>, %3.<Vetype>"
)

(define_expand "sub<mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (minus:SVE_F
	     (match_operand:SVE_F 1 "aarch64_sve_float_arith_operand")
	     (match_operand:SVE_F 2 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*sub<mode>3"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, w, w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (minus:SVE_F
	     (match_operand:SVE_F 2 "aarch64_sve_float_arith_operand" "w, 0, 0, vfa")
	     (match_operand:SVE_F 3 "aarch64_sve_float_arith_with_sub_operand" "w, vfa, vfn, 0"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE
   && (register_operand (operands[2], <MODE>mode)
       || register_operand (operands[3], <MODE>mode))"
  "@
   fsub\t%0.<Vetype>, %2.<Vetype>, %3.<Vetype>
   fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%N3
   fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, #%2"
)

(define_expand "mul<mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (mult:SVE_F
	     (match_operand:SVE_F 1 "register_operand")
	     (match_operand:SVE_F 2 "aarch64_sve_float_mul_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*mul<mode>3"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (mult:SVE_F
	     (match_operand:SVE_F 2 "register_operand" "%0, w")
	     (match_operand:SVE_F 3 "aarch64_sve_float_mul_operand" "vfm, w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   fmul\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   fmul\t%0.<Vetype>, %2.<Vetype>, %3.<Vetype>"
)

;; Note: fma is %0 = (%1 * %2) + %3

(define_expand "fma<mode>4"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 4)
	   (fma:SVE_F (match_operand:SVE_F 1 "register_operand")
		      (match_operand:SVE_F 2 "register_operand")
		      (match_operand:SVE_F 3 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[4] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*fma<mode>4"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (fma:SVE_F (match_operand:SVE_F 2 "register_operand" "%0, w")
		      (match_operand:SVE_F 3 "register_operand" "w, w")
		      (match_operand:SVE_F 4 "register_operand" "w, 0"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   fmad\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   fmla\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
)

(define_expand "fnma<mode>4"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 4)
	   (fma:SVE_F (neg:SVE_F
			(match_operand:SVE_F 1 "register_operand"))
		      (match_operand:SVE_F 2 "register_operand")
		      (match_operand:SVE_F 3 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[4] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*fnma<mode>4"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (fma:SVE_F (neg:SVE_F
			(match_operand:SVE_F 2 "register_operand" "%0, w"))
		      (match_operand:SVE_F 3 "register_operand" "w, w")
		      (match_operand:SVE_F 4 "register_operand" "w, 0"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   fmsb\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   fmls\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
)

(define_expand "fnms<mode>4"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 4)
	   (fma:SVE_F (neg:SVE_F
			(match_operand:SVE_F 1 "register_operand"))
		      (match_operand:SVE_F 2 "register_operand")
		      (neg:SVE_F
			(match_operand:SVE_F 3 "register_operand")))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[4] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*fnms<mode>4"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (fma:SVE_F (neg:SVE_F
			(match_operand:SVE_F 2 "register_operand" "%0, w"))
		      (match_operand:SVE_F 3 "register_operand" "w, w")
		      (neg:SVE_F
			(match_operand:SVE_F 4 "register_operand" "w, 0")))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   fnmad\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   fnmla\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
)

(define_expand "fms<mode>4"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 4)
	   (fma:SVE_F (match_operand:SVE_F 1 "register_operand")
		      (match_operand:SVE_F 2 "register_operand")
		      (neg:SVE_F
			(match_operand:SVE_F 3 "register_operand")))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[4] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*fms<mode>4"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (fma:SVE_F (match_operand:SVE_F 2 "register_operand" "%0, w")
		      (match_operand:SVE_F 3 "register_operand" "w, w")
		      (neg:SVE_F
			(match_operand:SVE_F 4 "register_operand" "w, 0")))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   fnmsb\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   fnmls\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
)

(define_expand "div<mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (div:SVE_F (match_operand:SVE_F 1 "register_operand")
		      (match_operand:SVE_F 2 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*div<mode>3"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (div:SVE_F (match_operand:SVE_F 2 "register_operand" "0, w")
		      (match_operand:SVE_F 3 "register_operand" "w, 0"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   fdiv\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   fdivr\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>"
)

(define_expand "<optab><mode>2"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 2)
	   (SVE_FP_UNARY:SVE_F (match_operand:SVE_F 1 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*<optab><mode>2"
  [(set (match_operand:SVE_F 0 "register_operand" "=w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (SVE_FP_UNARY:SVE_F (match_operand:SVE_F 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<sve_fp_op>\t%0.<Vetype>, %1/m, %2.<Vetype>"
)

(define_expand "<frint_pattern><mode>2"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 2)
	   (unspec:SVE_F [(match_operand:SVE_F 1 "register_operand")]
			 FRINT)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*<frint_pattern><mode>2"
  [(set (match_operand:SVE_F 0 "register_operand" "=w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (unspec:SVE_F [(match_operand:SVE_F 2 "register_operand" "w")]
			 FRINT)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "frint<frint_suffix>\t%0.<Vetype>, %1/m, %2.<Vetype>"
)

;; Convert float to integer of the same size (sf to si or df to di).
(define_expand "<fix_trunc_optab><mode><v_int_equiv>2"
  [(set (match_operand:<V_INT_EQUIV> 0 "register_operand")
	(unspec:<V_INT_EQUIV>
	  [(match_dup 2)
	   (FIXUORS:<V_INT_EQUIV>
	     (match_operand:SVE_F 1 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*<fix_trunc_optab>v8sf<mode>2"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w")
	(unspec:SVE_SDI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (FIXUORS:SVE_SDI
	     (match_operand:V8SF 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "fcvtz<su>\t%0.<Vetype>, %1/m, %2.s"
)

(define_insn "*<fix_trunc_optab>v4df<mode>2"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w")
	(unspec:SVE_SDI
	  [(match_operand:V4BI 1 "register_operand" "Upl")
	   (FIXUORS:SVE_SDI
	     (match_operand:V4DF 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "fcvtz<su>\t%0.<Vetype>, %1/m, %2.d"
)

;; Convert integer to float of the same size (si to sf or di to df).
(define_expand "<optab><v_int_equiv><mode>2"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 2)
	   (FLOATUORS:SVE_F
	     (match_operand:<V_INT_EQUIV> 1 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

(define_insn "*<optab><mode>v8sf2"
  [(set (match_operand:V8SF 0 "register_operand" "=w")
	(unspec:V8SF
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (FLOATUORS:V8SF
	     (match_operand:SVE_SDI 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<su_optab>cvtf\t%0.s, %1/m, %2.<Vetype>"
)

(define_insn "*<optab><mode>v4df2"
  [(set (match_operand:V4DF 0 "register_operand" "=w")
	(unspec:V4DF
	  [(match_operand:V4BI 1 "register_operand" "Upl")
	   (FLOATUORS:V4DF
	     (match_operand:SVE_SDI 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<su_optab>cvtf\t%0.d, %1/m, %2.<Vetype>"
)

(define_insn "*truncv4dfv8sf2"
  [(set (match_operand:V8SF 0 "register_operand" "=w")
	(unspec:V8SF
	  [(match_operand:V4BI 1 "register_operand" "Upl")
	   (unspec:V8SF
	     [(match_operand:V4DF 2 "register_operand" "w")]
	     UNSPEC_FLOAT_CONVERT)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "fcvt\t%0.s, %1/m, %2.d"
)

(define_insn "*extendv8sfv4df2"
  [(set (match_operand:V4DF 0 "register_operand" "=w")
	(unspec:V4DF
	  [(match_operand:V4BI 1 "register_operand" "Upl")
	   (unspec:V4DF
	     [(match_operand:V8SF 2 "register_operand" "w")]
	     UNSPEC_FLOAT_CONVERT)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "fcvt\t%0.d, %1/m, %2.s"
)

(define_insn "vec_unpack<su>_<perm_hilo>_<mode>"
  [(set (match_operand:<VWIDE> 0 "register_operand" "=Upa")
	(unspec:<VWIDE> [(match_operand:PRED_BHS 1 "register_operand" "Upa")]
			UNPACK))]
  "TARGET_SVE"
  "punpk<perm_hilo>\t%0.h, %1.b"
)

(define_insn "vec_unpack<su>_<perm_hilo>_<SVE_BHSI:mode>"
  [(set (match_operand:<VWIDE> 0 "register_operand" "=w")
	(unspec:<VWIDE> [(match_operand:SVE_BHSI 1 "register_operand" "w")]
			UNPACK))]
  "TARGET_SVE"
  "<su>unpk<perm_hilo>\t%0.<Vewtype>, %1.<Vetype>"
)

;; Used by the vec_unpacks_<perm_hilo>_v8sf expander to unpack the bit
;; representation of a V8SF without conversion.  The choice between signed
;; and unsigned isn't significant.
(define_insn "*vec_unpacku_<perm_hilo>_v8sf_no_convert"
  [(set (match_operand:V8SF 0 "register_operand" "=w")
	(unspec:V8SF [(match_operand:V8SF 1 "register_operand" "w")]
		     UNPACK_UNSIGNED))]
  "TARGET_SVE"
  "uunpk<perm_hilo>\t%0.d, %1.s"
)

;; Float to double
;; unpack from v8sf with no conversion
;; then float convert the unpacked v8sf to v4df
(define_expand "vec_unpacks_<perm_hilo>_v8sf"
  [(set (match_dup 2)
	(unspec:V8SF [(match_operand:V8SF 1 "register_operand")]
		     UNPACK_UNSIGNED))
   (set (match_operand:V4DF 0 "register_operand")
	(unspec:V4DF [(match_dup 3)
		      (unspec:V4DF [(match_dup 2)] UNSPEC_FLOAT_CONVERT)]
		     UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[2] = gen_reg_rtx (V8SFmode);
    operands[3] = force_reg (V4BImode, CONSTM1_RTX (V4BImode));
  }
)

;; Int to double
;; unpack from v8si to v4di
;; then convert v8si to v4df
(define_expand "vec_unpack<su_optab>_float_<perm_hilo>_v8si"
  [(set (match_dup 2)
	(unspec:V4DI [(match_operand:V8SI 1 "register_operand")]
		     UNPACK_UNSIGNED))
   (set (match_operand:V4DF 0 "register_operand")
	(unspec:V4DF [(match_dup 3)
		      (FLOATUORS:V4DF (match_dup 4))]
		     UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[2] = gen_reg_rtx (V4DImode);
    operands[3] = force_reg (V4BImode, CONSTM1_RTX (V4BImode));
    operands[4] = gen_rtx_SUBREG (V8SImode, operands[2], 0);
  }
)

(define_insn "vec_pack_trunc_<Vwide>"
  [(set (match_operand:PRED_BHS 0 "register_operand" "=Upa")
	(unspec:PRED_BHS
	  [(match_operand:<VWIDE> 1 "register_operand" "Upa")
	   (match_operand:<VWIDE> 2 "register_operand" "Upa")]
	  UNSPEC_PACK))]
  "TARGET_SVE"
  "uzp1\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
)

(define_insn "vec_pack_trunc_<Vwide>"
  [(set (match_operand:SVE_BHSI 0 "register_operand" "=w")
	(unspec:SVE_BHSI
	  [(match_operand:<VWIDE> 1 "register_operand" "w")
	   (match_operand:<VWIDE> 2 "register_operand" "w")]
	  UNSPEC_PACK))]
  "TARGET_SVE"
  "uzp1\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
)

(define_insn "*vec_pack_trunc_v4df_no_convert"
  [(set (match_operand:V8SF 0 "register_operand" "=w")
	(unspec:V8SF [(match_operand:V8SF 1 "register_operand" "w")
		      (match_operand:V8SF 2 "register_operand" "w")]
		     UNSPEC_PACK))]
  "TARGET_SVE"
  "uzp1\t%0.s, %1.s, %2.s"
)

;; Double to float
;; float convert both inputs from v4df to v8sf
;; then pack them together with no conversion
(define_expand "vec_pack_trunc_v4df"
  [(set (match_dup 4)
	(unspec:V8SF
	  [(match_dup 3)
	   (unspec:V8SF [(match_operand:V4DF 1 "register_operand")]
			UNSPEC_FLOAT_CONVERT)]
	  UNSPEC_MERGE_PTRUE))
   (set (match_dup 5)
	(unspec:V8SF
	  [(match_dup 3)
	   (unspec:V8SF [(match_operand:V4DF 2 "register_operand")]
			UNSPEC_FLOAT_CONVERT)]
	  UNSPEC_MERGE_PTRUE))
   (set (match_operand:V8SF 0 "register_operand")
	(unspec:V8SF [(match_dup 4) (match_dup 5)] UNSPEC_PACK))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (V4BImode, CONSTM1_RTX (V4BImode));
    operands[4] = gen_reg_rtx (V8SFmode);
    operands[5] = gen_reg_rtx (V8SFmode);
  }
)

;; Double to int
;; float convert both inputs from v4df to v8si
;; then pack v4di to v8si
(define_expand "vec_pack_<su>fix_trunc_v4df"
  [(set (match_dup 4)
	(unspec:V8SI
	  [(match_dup 3)
	   (FIXUORS:V8SI (match_operand:V4DF 1 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))
   (set (match_dup 5)
	(unspec:V8SI
	  [(match_dup 3)
	   (FIXUORS:V8SI (match_operand:V4DF 2 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))
   (set (match_operand:V8SI 0 "register_operand")
	(unspec:V8SI [(match_dup 6) (match_dup 7)] UNSPEC_PACK))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (V4BImode, CONSTM1_RTX (V4BImode));
    operands[4] = gen_reg_rtx (V8SImode);
    operands[5] = gen_reg_rtx (V8SImode);
    operands[6] = gen_rtx_SUBREG (V4DImode, operands[4], 0);
    operands[7] = gen_rtx_SUBREG (V4DImode, operands[5], 0);
  }
)
