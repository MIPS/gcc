;; Machine description for AArch64 SVE.
;; Copyright (C) 2009-2019 Free Software Foundation, Inc.
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

;; Note on the handling of big-endian SVE
;; --------------------------------------
;;
;; On big-endian systems, Advanced SIMD mov<mode> patterns act in the
;; same way as movdi or movti would: the first byte of memory goes
;; into the most significant byte of the register and the last byte
;; of memory goes into the least significant byte of the register.
;; This is the most natural ordering for Advanced SIMD and matches
;; the ABI layout for 64-bit and 128-bit vector types.
;;
;; As a result, the order of bytes within the register is what GCC
;; expects for a big-endian target, and subreg offsets therefore work
;; as expected, with the first element in memory having subreg offset 0
;; and the last element in memory having the subreg offset associated
;; with a big-endian lowpart.  However, this ordering also means that
;; GCC's lane numbering does not match the architecture's numbering:
;; GCC always treats the element at the lowest address in memory
;; (subreg offset 0) as element 0, while the architecture treats
;; the least significant end of the register as element 0.
;;
;; The situation for SVE is different.  We want the layout of the
;; SVE register to be same for mov<mode> as it is for maskload<mode>:
;; logically, a mov<mode> load must be indistinguishable from a
;; maskload<mode> whose mask is all true.  We therefore need the
;; register layout to match LD1 rather than LDR.  The ABI layout of
;; SVE types also matches LD1 byte ordering rather than LDR byte ordering.
;;
;; As a result, the architecture lane numbering matches GCC's lane
;; numbering, with element 0 always being the first in memory.
;; However:
;;
;; - Applying a subreg offset to a register does not give the element
;;   that GCC expects: the first element in memory has the subreg offset
;;   associated with a big-endian lowpart while the last element in memory
;;   has subreg offset 0.  We handle this via TARGET_CAN_CHANGE_MODE_CLASS.
;;
;; - We cannot use LDR and STR for spill slots that might be accessed
;;   via subregs, since although the elements have the order GCC expects,
;;   the order of the bytes within the elements is different.  We instead
;;   access spill slots via LD1 and ST1, using secondary reloads to
;;   reserve a predicate register.

;; These constants are used as a const_int in some predicated unspecs,
;; after the predicate operand itself.
(define_constants
  [; Indicates that some lanes might be inactive and that the instruction
   ; must not operate on inactive inputs if doing so could induce a fault.
   (SVE_FORBID_NEW_FAULTS 0)

   ; Indicates either that all lanes are active or that the instruction may
   ; operate on inactive inputs even if doing so could induce a fault.
   (SVE_ALLOW_NEW_FAULTS 1)])

;; SVE data moves.
(define_expand "mov<mode>"
  [(set (match_operand:SVE_ALL 0 "nonimmediate_operand")
	(match_operand:SVE_ALL 1 "general_operand"))]
  "TARGET_SVE"
  {
    /* Use the predicated load and store patterns where possible.
       This is required for big-endian targets (see the comment at the
       head of the file) and increases the addressing choices for
       little-endian.  */
    if ((MEM_P (operands[0]) || MEM_P (operands[1]))
	&& can_create_pseudo_p ())
      {
	aarch64_expand_sve_mem_move (operands[0], operands[1], <VPRED>mode);
	DONE;
      }

    if (CONSTANT_P (operands[1]))
      {
	aarch64_expand_mov_immediate (operands[0], operands[1]);
	DONE;
      }

    /* Optimize subregs on big-endian targets: we can use REV[BHW]
       instead of going through memory.  */
    if (BYTES_BIG_ENDIAN
	&& aarch64_maybe_expand_sve_subreg_move (operands[0], operands[1]))
      DONE;
  }
)

;; A pattern for optimizing SUBREGs that have a reinterpreting effect
;; on big-endian targets; see aarch64_maybe_expand_sve_subreg_move
;; for details.  We use a special predicate for operand 2 to reduce
;; the number of patterns.
(define_insn_and_split "*aarch64_sve_mov<mode>_subreg_be"
  [(set (match_operand:SVE_ALL 0 "aarch64_sve_nonimmediate_operand" "=w")
	(unspec:SVE_ALL
	  [(match_operand:VNx16BI 1 "register_operand" "Upl")
	   (match_operand 2 "aarch64_any_register_operand" "w")]
	  UNSPEC_REV_SUBREG))]
  "TARGET_SVE && BYTES_BIG_ENDIAN"
  "#"
  "&& reload_completed"
  [(const_int 0)]
  {
    aarch64_split_sve_subreg_move (operands[0], operands[1], operands[2]);
    DONE;
  }
)

;; Unpredicated moves (little-endian).  Only allow memory operations
;; during and after RA; before RA we want the predicated load and
;; store patterns to be used instead.
(define_insn "*aarch64_sve_mov<mode>_le"
  [(set (match_operand:SVE_ALL 0 "aarch64_sve_nonimmediate_operand" "=w, Utr, w, w")
	(match_operand:SVE_ALL 1 "aarch64_sve_general_operand" "Utr, w, w, Dn"))]
  "TARGET_SVE
   && !BYTES_BIG_ENDIAN
   && ((lra_in_progress || reload_completed)
       || (register_operand (operands[0], <MODE>mode)
	   && nonmemory_operand (operands[1], <MODE>mode)))"
  "@
   ldr\t%0, %1
   str\t%1, %0
   mov\t%0.d, %1.d
   * return aarch64_output_sve_mov_immediate (operands[1]);"
)

;; Unpredicated moves (big-endian).  Memory accesses require secondary
;; reloads.
(define_insn "*aarch64_sve_mov<mode>_be"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w, w")
	(match_operand:SVE_ALL 1 "aarch64_nonmemory_operand" "w, Dn"))]
  "TARGET_SVE && BYTES_BIG_ENDIAN"
  "@
   mov\t%0.d, %1.d
   * return aarch64_output_sve_mov_immediate (operands[1]);"
)

;; Handle big-endian memory reloads.  We use byte PTRUE for all modes
;; to try to encourage reuse.
(define_expand "aarch64_sve_reload_be"
  [(parallel
     [(set (match_operand 0)
	   (match_operand 1))
      (clobber (match_operand:VNx16BI 2 "register_operand" "=Upl"))])]
  "TARGET_SVE && BYTES_BIG_ENDIAN"
  {
    /* Create a PTRUE.  */
    emit_move_insn (operands[2], CONSTM1_RTX (VNx16BImode));

    /* Refer to the PTRUE in the appropriate mode for this move.  */
    machine_mode mode = GET_MODE (operands[0]);
    machine_mode pred_mode
      = aarch64_sve_pred_mode (GET_MODE_UNIT_SIZE (mode)).require ();
    rtx pred = gen_lowpart (pred_mode, operands[2]);

    /* Emit a predicated load or store.  */
    aarch64_emit_sve_pred_move (operands[0], pred, operands[1]);
    DONE;
  }
)

;; A predicated load or store for which the predicate is known to be
;; all-true.  Note that this pattern is generated directly by
;; aarch64_emit_sve_pred_move, so changes to this pattern will
;; need changes there as well.
(define_insn_and_split "@aarch64_pred_mov<mode>"
  [(set (match_operand:SVE_ALL 0 "nonimmediate_operand" "=w, w, m")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	   (match_operand:SVE_ALL 2 "nonimmediate_operand" "w, m, w")]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE
   && (register_operand (operands[0], <MODE>mode)
       || register_operand (operands[2], <MODE>mode))"
  "@
   #
   ld1<Vesize>\t%0.<Vetype>, %1/z, %2
   st1<Vesize>\t%2.<Vetype>, %1, %0"
  "&& register_operand (operands[0], <MODE>mode)
   && register_operand (operands[2], <MODE>mode)"
  [(set (match_dup 0) (match_dup 2))]
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
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:SVE_ALL 1 "memory_operand" "m")]
	  UNSPEC_LD1_SVE))]
  "TARGET_SVE"
  "ld1<Vesize>\t%0.<Vetype>, %2/z, %1"
)

;; Predicated contiguous non-temporal load.
(define_insn "@aarch64_ldnt1<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:SVE_ALL 1 "memory_operand" "m")]
	  UNSPEC_LDNT1_SVE))]
  "TARGET_SVE"
  "ldnt1<Vesize>\t%0.<Vetype>, %2/z, %1"
)

(define_insn "maskstore<mode><vpred>"
  [(set (match_operand:SVE_ALL 0 "memory_operand" "+m")
	(unspec:SVE_ALL [(match_operand:<VPRED> 2 "register_operand" "Upl")
			 (match_operand:SVE_ALL 1 "register_operand" "w")
			 (match_dup 0)]
			UNSPEC_ST1_SVE))]
  "TARGET_SVE"
  "st1<Vesize>\t%1.<Vetype>, %2, %0"
)

;; Predicated contiguous non-temporal store.
(define_insn "@aarch64_stnt1<mode>"
  [(set (match_operand:SVE_ALL 0 "memory_operand" "+m")
	(unspec:SVE_ALL [(match_operand:<VPRED> 2 "register_operand" "Upl")
			 (match_operand:SVE_ALL 1 "register_operand" "w")
			 (match_dup 0)]
			UNSPEC_STNT1_SVE))]
  "TARGET_SVE"
  "stnt1<Vesize>\t%1.<Vetype>, %2, %0"
)

;; Predicated load and extend, with 8 elements per 128-bit block.
(define_insn "@aarch64_load_<ANY_EXTEND:optab><VNx8_WIDE:mode><VNx8_NARROW:mode>"
  [(set (match_operand:VNx8_WIDE 0 "register_operand" "=w")
	(ANY_EXTEND:VNx8_WIDE
	  (unspec:VNx8_NARROW
	    [(match_operand:VNx8BI 2 "register_operand" "Upl")
	     (match_operand:VNx8_NARROW 1 "memory_operand" "m")]
	    UNSPEC_LD1_SVE)))]
  "TARGET_SVE"
  "ld1<ANY_EXTEND:s><VNx8_NARROW:Vesize>\t%0.<VNx8_WIDE:Vetype>, %2/z, %1"
)

;; Predicated load and extend, with 4 elements per 128-bit block.
(define_insn "@aarch64_load_<ANY_EXTEND:optab><VNx4_WIDE:mode><VNx4_NARROW:mode>"
  [(set (match_operand:VNx4_WIDE 0 "register_operand" "=w")
	(ANY_EXTEND:VNx4_WIDE
	  (unspec:VNx4_NARROW
	    [(match_operand:VNx4BI 2 "register_operand" "Upl")
	     (match_operand:VNx4_NARROW 1 "memory_operand" "m")]
	    UNSPEC_LD1_SVE)))]
  "TARGET_SVE"
  "ld1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.<VNx4_WIDE:Vetype>, %2/z, %1"
)

;; Predicated load and extend, with 2 elements per 128-bit block.
(define_insn "@aarch64_load_<ANY_EXTEND:optab><VNx2_WIDE:mode><VNx2_NARROW:mode>"
  [(set (match_operand:VNx2_WIDE 0 "register_operand" "=w")
	(ANY_EXTEND:VNx2_WIDE
	  (unspec:VNx2_NARROW
	    [(match_operand:VNx2BI 2 "register_operand" "Upl")
	     (match_operand:VNx2_NARROW 1 "memory_operand" "m")]
	    UNSPEC_LD1_SVE)))]
  "TARGET_SVE"
  "ld1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.<VNx2_WIDE:Vetype>, %2/z, %1"
)

;; Predicated truncate and store, with 8 elements per 128-bit block.
(define_insn "@aarch64_store_trunc<VNx8_NARROW:mode><VNx8_WIDE:mode>"
  [(set (match_operand:VNx8_NARROW 0 "memory_operand" "+m")
	(unspec:VNx8_NARROW
	  [(match_operand:VNx8BI 2 "register_operand" "Upl")
	   (truncate:VNx8_NARROW
	     (match_operand:VNx8_WIDE 1 "register_operand" "w"))
	   (match_dup 0)]
	  UNSPEC_ST1_SVE))]
  "TARGET_SVE"
  "st1<VNx8_NARROW:Vesize>\t%1.<VNx8_WIDE:Vetype>, %2, %0"
)

;; Predicated truncate and store, with 4 elements per 128-bit block.
(define_insn "@aarch64_store_trunc<VNx4_NARROW:mode><VNx4_WIDE:mode>"
  [(set (match_operand:VNx4_NARROW 0 "memory_operand" "+m")
	(unspec:VNx4_NARROW
	  [(match_operand:VNx4BI 2 "register_operand" "Upl")
	   (truncate:VNx4_NARROW
	     (match_operand:VNx4_WIDE 1 "register_operand" "w"))
	   (match_dup 0)]
	  UNSPEC_ST1_SVE))]
  "TARGET_SVE"
  "st1<VNx4_NARROW:Vesize>\t%1.<VNx4_WIDE:Vetype>, %2, %0"
)

;; Predicated truncate and store, with 2 elements per 128-bit block.
(define_insn "@aarch64_store_trunc<VNx2_NARROW:mode><VNx2_WIDE:mode>"
  [(set (match_operand:VNx2_NARROW 0 "memory_operand" "+m")
	(unspec:VNx2_NARROW
	  [(match_operand:VNx2BI 2 "register_operand" "Upl")
	   (truncate:VNx2_NARROW
	     (match_operand:VNx2_WIDE 1 "register_operand" "w"))
	   (match_dup 0)]
	  UNSPEC_ST1_SVE))]
  "TARGET_SVE"
  "st1<VNx2_NARROW:Vesize>\t%1.<VNx2_WIDE:Vetype>, %2, %0"
)

;; Unpredicated gather loads.
(define_expand "gather_load<mode>"
  [(set (match_operand:SVE_SD 0 "register_operand")
	(unspec:SVE_SD
	  [(match_dup 5)
	   (match_operand:DI 1 "aarch64_sve_gather_offset_<Vesize>")
	   (match_operand:<V_INT_EQUIV> 2 "register_operand")
	   (match_operand:DI 3 "const_int_operand")
	   (match_operand:DI 4 "aarch64_gather_scale_operand_<Vesize>")
	   (mem:BLK (scratch))]
	  UNSPEC_LD1_GATHER))]
  "TARGET_SVE"
  {
    operands[5] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated gather loads for 32-bit elements.  Operand 3 is true for
;; unsigned extension and false for signed extension.
(define_insn "mask_gather_load<mode>"
  [(set (match_operand:SVE_S 0 "register_operand" "=w, w, w, w, w, w")
	(unspec:SVE_S
	  [(match_operand:VNx4BI 5 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	   (match_operand:DI 1 "aarch64_sve_gather_offset_w" "Z, vgw, rk, rk, rk, rk")
	   (match_operand:VNx4SI 2 "register_operand" "w, w, w, w, w, w")
	   (match_operand:DI 3 "const_int_operand" "Ui1, Ui1, Z, Ui1, Z, Ui1")
	   (match_operand:DI 4 "aarch64_gather_scale_operand_w" "Ui1, Ui1, Ui1, Ui1, i, i")
	   (mem:BLK (scratch))]
	  UNSPEC_LD1_GATHER))]
  "TARGET_SVE"
  "@
   ld1w\t%0.s, %5/z, [%2.s]
   ld1w\t%0.s, %5/z, [%2.s, #%1]
   ld1w\t%0.s, %5/z, [%1, %2.s, sxtw]
   ld1w\t%0.s, %5/z, [%1, %2.s, uxtw]
   ld1w\t%0.s, %5/z, [%1, %2.s, sxtw %p4]
   ld1w\t%0.s, %5/z, [%1, %2.s, uxtw %p4]"
)

;; Predicated gather loads for 64-bit elements.  The value of operand 3
;; doesn't matter in this case.
(define_insn "mask_gather_load<mode>"
  [(set (match_operand:SVE_D 0 "register_operand" "=w, w, w, w")
	(unspec:SVE_D
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl, Upl, Upl")
	   (match_operand:DI 1 "aarch64_sve_gather_offset_d" "Z, vgd, rk, rk")
	   (match_operand:VNx2DI 2 "register_operand" "w, w, w, w")
	   (match_operand:DI 3 "const_int_operand")
	   (match_operand:DI 4 "aarch64_gather_scale_operand_d" "Ui1, Ui1, Ui1, i")
	   (mem:BLK (scratch))]
	  UNSPEC_LD1_GATHER))]
  "TARGET_SVE"
  "@
   ld1d\t%0.d, %5/z, [%2.d]
   ld1d\t%0.d, %5/z, [%2.d, #%1]
   ld1d\t%0.d, %5/z, [%1, %2.d]
   ld1d\t%0.d, %5/z, [%1, %2.d, lsl %p4]"
)

;; Likewise, but with the offset being sign-extended from 32 bits.
(define_insn "*mask_gather_load<mode>_sxtw"
  [(set (match_operand:SVE_D 0 "register_operand" "=w, w")
	(unspec:SVE_D
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	   (match_operand:DI 1 "register_operand" "rk, rk")
	   (unspec:VNx2DI
	     [(match_dup 5)
	      (sign_extend:VNx2DI
		(truncate:VNx2SI
		  (match_operand:VNx2DI 2 "register_operand" "w, w")))]
	     UNSPEC_MERGE_PTRUE)
	   (match_operand:DI 3 "const_int_operand")
	   (match_operand:DI 4 "aarch64_gather_scale_operand_d" "Ui1, i")
	   (mem:BLK (scratch))]
	  UNSPEC_LD1_GATHER))]
  "TARGET_SVE"
  "@
   ld1d\t%0.d, %5/z, [%1, %2.d, sxtw]
   ld1d\t%0.d, %5/z, [%1, %2.d, sxtw %p4]"
)

;; Likewise, but with the offset being zero-extended from 32 bits.
(define_insn "*mask_gather_load<mode>_uxtw"
  [(set (match_operand:SVE_D 0 "register_operand" "=w, w")
	(unspec:SVE_D
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	   (match_operand:DI 1 "register_operand" "rk, rk")
	   (and:VNx2DI
	     (match_operand:VNx2DI 2 "register_operand" "w, w")
	     (match_operand:VNx2DI 6 "aarch64_sve_uxtw_immediate"))
	   (match_operand:DI 3 "const_int_operand")
	   (match_operand:DI 4 "aarch64_gather_scale_operand_d" "Ui1, i")
	   (mem:BLK (scratch))]
	  UNSPEC_LD1_GATHER))]
  "TARGET_SVE"
  "@
   ld1d\t%0.d, %5/z, [%1, %2.d, uxtw]
   ld1d\t%0.d, %5/z, [%1, %2.d, uxtw %p4]"
)

;; Predicated extending gather loads for 32-bit elements.  Operand 3 is
;; true for unsigned extension and false for signed extension.
(define_insn "@aarch64_gather_load_<ANY_EXTEND:optab><VNx4_WIDE:mode><VNx4_NARROW:mode>"
  [(set (match_operand:VNx4_WIDE 0 "register_operand" "=w, w, w, w, w, w")
	(ANY_EXTEND:VNx4_WIDE
	  (unspec:VNx4_NARROW
	    [(match_operand:VNx4BI 5 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	     (match_operand:DI 1 "aarch64_sve_gather_offset_<VNx4_NARROW:Vesize>" "Z, vg<VNx4_NARROW:Vesize>, rk, rk, rk, rk")
	     (match_operand:VNx4_WIDE 2 "register_operand" "w, w, w, w, w, w")
	     (match_operand:DI 3 "const_int_operand" "Ui1, Ui1, Z, Ui1, Z, Ui1")
	     (match_operand:DI 4 "aarch64_gather_scale_operand_<VNx4_NARROW:Vesize>" "Ui1, Ui1, Ui1, Ui1, i, i")
	     (mem:BLK (scratch))]
	    UNSPEC_LD1_GATHER)))]
  "TARGET_SVE"
  "@
   ld1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%2.s]
   ld1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%2.s, #%1]
   ld1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%1, %2.s, sxtw]
   ld1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%1, %2.s, uxtw]
   ld1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%1, %2.s, sxtw %p4]
   ld1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%1, %2.s, uxtw %p4]"
)

;; Predicated extending gather loads for 64-bit elements.  The value of
;; operand 3 doesn't matter in this case.
(define_insn "@aarch64_gather_load_<ANY_EXTEND:optab><VNx2_WIDE:mode><VNx2_NARROW:mode>"
  [(set (match_operand:VNx2_WIDE 0 "register_operand" "=w, w, w, w")
	(ANY_EXTEND:VNx2_WIDE
	  (unspec:VNx2_NARROW
	    [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl, Upl, Upl")
	     (match_operand:DI 1 "aarch64_sve_gather_offset_<VNx2_NARROW:Vesize>" "Z, vg<VNx2_NARROW:Vesize>, rk, rk")
	     (match_operand:VNx2_WIDE 2 "register_operand" "w, w, w, w")
	     (match_operand:DI 3 "const_int_operand")
	     (match_operand:DI 4 "aarch64_gather_scale_operand_<VNx2_NARROW:Vesize>" "Ui1, Ui1, Ui1, i")
	     (mem:BLK (scratch))]
	    UNSPEC_LD1_GATHER)))]
  "TARGET_SVE"
  "@
   ld1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%2.d]
   ld1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%2.d, #%1]
   ld1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d]
   ld1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d, lsl %p4]"
)

;; Likewise, but with the offset being sign-extended from 32 bits.
(define_insn "*aarch64_gather_load_<ANY_EXTEND:optab><VNx2_WIDE:mode><VNx2_NARROW:mode>_sxtw"
  [(set (match_operand:VNx2_WIDE 0 "register_operand" "=w, w")
	(ANY_EXTEND:VNx2_WIDE
	  (unspec:VNx2_NARROW
	    [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	     (match_operand:DI 1 "aarch64_reg_or_zero" "rk, rk")
	     (unspec:VNx2DI
	       [(match_dup 5)
		(sign_extend:VNx2DI
		  (truncate:VNx2SI
		    (match_operand:VNx2DI 2 "register_operand" "w, w")))]
	       UNSPEC_MERGE_PTRUE)
	     (match_operand:DI 3 "const_int_operand")
	     (match_operand:DI 4 "aarch64_gather_scale_operand_<VNx2_NARROW:Vesize>" "Ui1, i")
	     (mem:BLK (scratch))]
	    UNSPEC_LD1_GATHER)))]
  "TARGET_SVE"
  "@
   ld1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d, sxtw]
   ld1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d, sxtw %p4]"
)

;; Likewise, but with the offset being zero-extended from 32 bits.
(define_insn "*aarch64_gather_load_<ANY_EXTEND:optab><VNx2_WIDE:mode><VNx2_NARROW:mode>_uxtw"
  [(set (match_operand:VNx2_WIDE 0 "register_operand" "=w, w")
	(ANY_EXTEND:VNx2_WIDE
	  (unspec:VNx2_NARROW
	    [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	     (match_operand:DI 1 "aarch64_reg_or_zero" "rk, rk")
	     (and:VNx2DI
	       (match_operand:VNx2DI 2 "register_operand" "w, w")
	       (match_operand:VNx2DI 6 "aarch64_sve_uxtw_immediate"))
	     (match_operand:DI 3 "const_int_operand")
	     (match_operand:DI 4 "aarch64_gather_scale_operand_<VNx2_NARROW:Vesize>" "Ui1, i")
	     (mem:BLK (scratch))]
	    UNSPEC_LD1_GATHER)))]
  "TARGET_SVE"
  "@
   ld1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d, uxtw]
   ld1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d, uxtw %p4]"
)

;; Unpredicated scatter store.
(define_expand "scatter_store<mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(match_dup 5)
	   (match_operand:DI 0 "aarch64_sve_gather_offset_<Vesize>")
	   (match_operand:<V_INT_EQUIV> 1 "register_operand")
	   (match_operand:DI 2 "const_int_operand")
	   (match_operand:DI 3 "aarch64_gather_scale_operand_<Vesize>")
	   (match_operand:SVE_SD 4 "register_operand")]
	  UNSPEC_ST1_SCATTER))]
  "TARGET_SVE"
  {
    operands[5] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated scatter stores for 32-bit elements.  Operand 2 is true for
;; unsigned extension and false for signed extension.
(define_insn "mask_scatter_store<mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(match_operand:VNx4BI 5 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	   (match_operand:DI 0 "aarch64_sve_gather_offset_w" "Z, vgw, rk, rk, rk, rk")
	   (match_operand:VNx4SI 1 "register_operand" "w, w, w, w, w, w")
	   (match_operand:DI 2 "const_int_operand" "Ui1, Ui1, Z, Ui1, Z, Ui1")
	   (match_operand:DI 3 "aarch64_gather_scale_operand_w" "Ui1, Ui1, Ui1, Ui1, i, i")
	   (match_operand:SVE_S 4 "register_operand" "w, w, w, w, w, w")]
	  UNSPEC_ST1_SCATTER))]
  "TARGET_SVE"
  "@
   st1w\t%4.s, %5, [%1.s]
   st1w\t%4.s, %5, [%1.s, #%0]
   st1w\t%4.s, %5, [%0, %1.s, sxtw]
   st1w\t%4.s, %5, [%0, %1.s, uxtw]
   st1w\t%4.s, %5, [%0, %1.s, sxtw %p3]
   st1w\t%4.s, %5, [%0, %1.s, uxtw %p3]"
)

;; Predicated scatter stores for 64-bit elements.  The value of operand 2
;; doesn't matter in this case.
(define_insn "mask_scatter_store<mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl, Upl, Upl")
	   (match_operand:DI 0 "aarch64_sve_gather_offset_d" "Z, vgd, rk, rk")
	   (match_operand:VNx2DI 1 "register_operand" "w, w, w, w")
	   (match_operand:DI 2 "const_int_operand")
	   (match_operand:DI 3 "aarch64_gather_scale_operand_d" "Ui1, Ui1, Ui1, i")
	   (match_operand:SVE_D 4 "register_operand" "w, w, w, w")]
	  UNSPEC_ST1_SCATTER))]
  "TARGET_SVE"
  "@
   st1d\t%4.d, %5, [%1.d]
   st1d\t%4.d, %5, [%1.d, #%0]
   st1d\t%4.d, %5, [%0, %1.d]
   st1d\t%4.d, %5, [%0, %1.d, lsl %p3]"
)

;; Likewise, but with the offset being sign-extended from 32 bits.
(define_insn "*mask_scatter_store<mode>_sxtw"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	   (match_operand:DI 0 "register_operand" "rk, rk")
	   (unspec:VNx2DI
	     [(match_dup 5)
	      (sign_extend:VNx2DI
		(truncate:VNx2SI
		  (match_operand:VNx2DI 1 "register_operand" "w, w")))]
	     UNSPEC_MERGE_PTRUE)
	   (match_operand:DI 2 "const_int_operand")
	   (match_operand:DI 3 "aarch64_gather_scale_operand_d" "Ui1, i")
	   (match_operand:SVE_D 4 "register_operand" "w, w")]
	  UNSPEC_ST1_SCATTER))]
  "TARGET_SVE"
  "@
   st1d\t%4.d, %5, [%0, %1.d, sxtw]
   st1d\t%4.d, %5, [%0, %1.d, sxtw %p3]"
)

;; Likewise, but with the offset being zero-extended from 32 bits.
(define_insn "*mask_scatter_store<mode>_uxtw"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	   (match_operand:DI 0 "aarch64_reg_or_zero" "rk, rk")
	   (and:VNx2DI
	     (match_operand:VNx2DI 1 "register_operand" "w, w")
	     (match_operand:VNx2DI 6 "aarch64_sve_uxtw_immediate"))
	   (match_operand:DI 2 "const_int_operand")
	   (match_operand:DI 3 "aarch64_gather_scale_operand_d" "Ui1, i")
	   (match_operand:SVE_D 4 "register_operand" "w, w")]
	  UNSPEC_ST1_SCATTER))]
  "TARGET_SVE"
  "@
   st1d\t%4.d, %5, [%0, %1.d, uxtw]
   st1d\t%4.d, %5, [%0, %1.d, uxtw %p3]"
)

;; Predicated truncating scatter stores for 32-bit elements.  Operand 2 is
;; true for unsigned extension and false for signed extension.
(define_insn "@aarch64_scatter_store_trunc<VNx4_NARROW:mode><VNx4_WIDE:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(match_operand:VNx4BI 5 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	   (match_operand:DI 0 "aarch64_sve_gather_offset_<VNx4_NARROW:Vesize>" "Z, vg<VNx4_NARROW:Vesize>, rk, rk, rk, rk")
	   (match_operand:VNx4SI 1 "register_operand" "w, w, w, w, w, w")
	   (match_operand:DI 2 "const_int_operand" "Ui1, Ui1, Z, Ui1, Z, Ui1")
	   (match_operand:DI 3 "aarch64_gather_scale_operand_<VNx4_NARROW:Vesize>" "Ui1, Ui1, Ui1, Ui1, i, i")
	   (truncate:VNx4_NARROW
	     (match_operand:VNx4_WIDE 4 "register_operand" "w, w, w, w, w, w"))]
	  UNSPEC_ST1_SCATTER))]
  "TARGET_SVE"
  "@
   st1<VNx4_NARROW:Vesize>\t%4.s, %5, [%1.s]
   st1<VNx4_NARROW:Vesize>\t%4.s, %5, [%1.s, #%0]
   st1<VNx4_NARROW:Vesize>\t%4.s, %5, [%0, %1.s, sxtw]
   st1<VNx4_NARROW:Vesize>\t%4.s, %5, [%0, %1.s, uxtw]
   st1<VNx4_NARROW:Vesize>\t%4.s, %5, [%0, %1.s, sxtw %p3]
   st1<VNx4_NARROW:Vesize>\t%4.s, %5, [%0, %1.s, uxtw %p3]"
)

;; Predicated truncating scatter stores for 64-bit elements.  The value of
;; operand 2 doesn't matter in this case.
(define_insn "@aarch64_scatter_store_trunc<VNx2_NARROW:mode><VNx2_WIDE:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl, Upl, Upl")
	   (match_operand:DI 0 "aarch64_sve_gather_offset_<VNx2_NARROW:Vesize>" "Z, vg<VNx2_NARROW:Vesize>, rk, rk")
	   (match_operand:VNx2DI 1 "register_operand" "w, w, w, w")
	   (match_operand:DI 2 "const_int_operand")
	   (match_operand:DI 3 "aarch64_gather_scale_operand_<VNx2_NARROW:Vesize>" "Ui1, Ui1, Ui1, i")
	   (truncate:VNx2_NARROW
	     (match_operand:VNx2_WIDE 4 "register_operand" "w, w, w, w"))]
	  UNSPEC_ST1_SCATTER))]
  "TARGET_SVE"
  "@
   st1<VNx2_NARROW:Vesize>\t%4.d, %5, [%1.d]
   st1<VNx2_NARROW:Vesize>\t%4.d, %5, [%1.d, #%0]
   st1<VNx2_NARROW:Vesize>\t%4.d, %5, [%0, %1.d]
   st1<VNx2_NARROW:Vesize>\t%4.d, %5, [%0, %1.d, lsl %p3]"
)

;; Likewise, but with the offset being sign-extended from 32 bits.
(define_insn "*aarch64_scatter_store_trunc<VNx2_NARROW:mode><VNx2_WIDE:mode>_sxtw"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	   (match_operand:DI 0 "register_operand" "rk, rk")
	   (unspec:VNx2DI
	     [(match_dup 5)
	      (sign_extend:VNx2DI
		(truncate:VNx2SI
		  (match_operand:VNx2DI 1 "register_operand" "w, w")))]
	     UNSPEC_MERGE_PTRUE)
	   (match_operand:DI 2 "const_int_operand")
	   (match_operand:DI 3 "aarch64_gather_scale_operand_<VNx2_NARROW:Vesize>" "Ui1, i")
	   (truncate:VNx2_NARROW
	     (match_operand:VNx2_WIDE 4 "register_operand" "w, w"))]
	  UNSPEC_ST1_SCATTER))]
  "TARGET_SVE"
  "@
   st1<VNx2_NARROW:Vesize>\t%4.d, %5, [%0, %1.d, sxtw]
   st1<VNx2_NARROW:Vesize>\t%4.d, %5, [%0, %1.d, sxtw %p3]"
)

;; Likewise, but with the offset being zero-extended from 32 bits.
(define_insn "*aarch64_scatter_store_trunc<VNx2_NARROW:mode><VNx2_WIDE:mode>_uxtw"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	   (match_operand:DI 0 "aarch64_reg_or_zero" "rk, rk")
	   (and:VNx2DI
	     (match_operand:VNx2DI 1 "register_operand" "w, w")
	     (match_operand:VNx2DI 6 "aarch64_sve_uxtw_immediate"))
	   (match_operand:DI 2 "const_int_operand")
	   (match_operand:DI 3 "aarch64_gather_scale_operand_<VNx2_NARROW:Vesize>" "Ui1, i")
	   (truncate:VNx2_NARROW
	     (match_operand:VNx2_WIDE 4 "register_operand" "w, w"))]
	  UNSPEC_ST1_SCATTER))]
  "TARGET_SVE"
  "@
   st1<VNx2_NARROW:Vesize>\t%4.d, %5, [%0, %1.d, uxtw]
   st1<VNx2_NARROW:Vesize>\t%4.d, %5, [%0, %1.d, uxtw %p3]"
)

;; SVE structure moves.
(define_expand "mov<mode>"
  [(set (match_operand:SVE_STRUCT 0 "nonimmediate_operand")
	(match_operand:SVE_STRUCT 1 "general_operand"))]
  "TARGET_SVE"
  {
    /* Big-endian loads and stores need to be done via LD1 and ST1;
       see the comment at the head of the file for details.  */
    if ((MEM_P (operands[0]) || MEM_P (operands[1]))
	&& BYTES_BIG_ENDIAN)
      {
	gcc_assert (can_create_pseudo_p ());
	aarch64_expand_sve_mem_move (operands[0], operands[1], <VPRED>mode);
	DONE;
      }

    if (CONSTANT_P (operands[1]))
      {
	aarch64_expand_mov_immediate (operands[0], operands[1]);
	DONE;
      }
  }
)

;; Unpredicated structure moves (little-endian).
(define_insn "*aarch64_sve_mov<mode>_le"
  [(set (match_operand:SVE_STRUCT 0 "aarch64_sve_nonimmediate_operand" "=w, Utr, w, w")
	(match_operand:SVE_STRUCT 1 "aarch64_sve_general_operand" "Utr, w, w, Dn"))]
  "TARGET_SVE && !BYTES_BIG_ENDIAN"
  "#"
  [(set_attr "length" "<insn_length>")]
)

;; Unpredicated structure moves (big-endian).  Memory accesses require
;; secondary reloads.
(define_insn "*aarch64_sve_mov<mode>_le"
  [(set (match_operand:SVE_STRUCT 0 "register_operand" "=w, w")
	(match_operand:SVE_STRUCT 1 "aarch64_nonmemory_operand" "w, Dn"))]
  "TARGET_SVE && BYTES_BIG_ENDIAN"
  "#"
  [(set_attr "length" "<insn_length>")]
)

;; Split unpredicated structure moves into pieces.  This is the same
;; for both big-endian and little-endian code, although it only needs
;; to handle memory operands for little-endian code.
(define_split
  [(set (match_operand:SVE_STRUCT 0 "aarch64_sve_nonimmediate_operand")
	(match_operand:SVE_STRUCT 1 "aarch64_sve_general_operand"))]
  "TARGET_SVE && reload_completed"
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
	  emit_insn (gen_rtx_SET (subdest, subsrc));
	}
    DONE;
  }
)

;; Predicated structure moves.  This works for both endiannesses but in
;; practice is only useful for big-endian.
(define_insn_and_split "@aarch64_pred_mov<mode>"
  [(set (match_operand:SVE_STRUCT 0 "aarch64_sve_struct_nonimmediate_operand" "=w, w, Utx")
	(unspec:SVE_STRUCT
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	   (match_operand:SVE_STRUCT 2 "aarch64_sve_struct_nonimmediate_operand" "w, Utx, w")]
	  UNSPEC_MERGE_PTRUE))]
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
	aarch64_emit_sve_pred_move (subdest, operands[1], subsrc);
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

    if (CONSTANT_P (operands[1]))
      {
	aarch64_expand_mov_immediate (operands[0], operands[1]);
	DONE;
      }
  }
)

(define_insn "*aarch64_sve_mov<mode>"
  [(set (match_operand:PRED_ALL 0 "nonimmediate_operand" "=Upa, m, Upa, Upa")
	(match_operand:PRED_ALL 1 "aarch64_mov_operand" "Upa, Upa, m, Dn"))]
  "TARGET_SVE
   && (register_operand (operands[0], <MODE>mode)
       || register_operand (operands[1], <MODE>mode))"
  "@
   mov\t%0.b, %1.b
   str\t%1, %0
   ldr\t%0, %1
   * return aarch64_output_sve_mov_immediate (operands[1]);"
)

;; Match PTRUES Pn.B when both the predicate and flags are useful.
(define_insn "*aarch64_sve_ptruevnx16bi_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand 2)
	   (match_operand 3)
	   (match_operator:VNx16BI 1 "aarch64_sve_ptrue_svpattern_immediate"
	     [(unspec:VNx16BI
		[(match_operand 4)
		 (match_operand:VNx16BI 5 "aarch64_simd_imm_zero")]
	        UNSPEC_PTRUE)])
	   (const_int 1)]
	  UNSPEC_PTEST))
   (set (match_operand:VNx16BI 0 "register_operand" "=Upa")
	(match_dup 1))]
  "TARGET_SVE"
  {
    return aarch64_output_sve_ptrues (operands[1]);
  }
)

;; Match PTRUES Pn.[HSD] when both the predicate and flags are useful.
(define_insn "*aarch64_sve_ptrue<mode>_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand 2)
	   (match_operand 3)
	   (subreg:PRED_HSD
	     (match_operator:VNx16BI 1 "aarch64_sve_ptrue_svpattern_immediate"
	       [(unspec:VNx16BI
		  [(match_operand 4)
		   (match_operand:PRED_HSD 5 "aarch64_simd_imm_zero")]
		  UNSPEC_PTRUE)]) 0)
	   (const_int 1)]
	  UNSPEC_PTEST))
   (set (match_operand:VNx16BI 0 "register_operand" "=Upa")
	(match_dup 1))]
  "TARGET_SVE"
  {
    return aarch64_output_sve_ptrues (operands[1]);
  }
)

;; Match PTRUES Pn.B when only the flags result is useful (which is
;; a way of testing VL).
(define_insn "*aarch64_sve_ptruevnx16bi_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand 2)
	   (match_operand 3)
	   (match_operator:VNx16BI 1 "aarch64_sve_ptrue_svpattern_immediate"
	     [(unspec:VNx16BI
		[(match_operand 4)
		 (match_operand:VNx16BI 5 "aarch64_simd_imm_zero")]
	        UNSPEC_PTRUE)])
	   (const_int 1)]
	  UNSPEC_PTEST))
   (clobber (match_scratch:VNx16BI 0 "=Upa"))]
  "TARGET_SVE"
  {
    return aarch64_output_sve_ptrues (operands[1]);
  }
)

;; Match PTRUES Pn.[HWD] when only the flags result is useful (which is
;; a way of testing VL).
(define_insn "*aarch64_sve_ptrue<mode>_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand 2)
	   (match_operand 3)
	   (subreg:PRED_HSD
	     (match_operator:VNx16BI 1 "aarch64_sve_ptrue_svpattern_immediate"
	       [(unspec:VNx16BI
		  [(match_operand 4)
		   (match_operand:PRED_HSD 5 "aarch64_simd_imm_zero")]
		  UNSPEC_PTRUE)]) 0)
	   (const_int 1)]
	  UNSPEC_PTEST))
   (clobber (match_scratch:VNx16BI 0 "=Upa"))]
  "TARGET_SVE"
  {
    return aarch64_output_sve_ptrues (operands[1]);
  }
)

;; Handle extractions from a predicate by converting to an integer vector
;; and extracting from there.
(define_expand "vec_extract<vpred><Vel>"
  [(match_operand:<VEL> 0 "register_operand")
   (match_operand:<VPRED> 1 "register_operand")
   (match_operand:SI 2 "nonmemory_operand")
   ;; Dummy operand to which we can attach the iterator.
   (reg:SVE_I V0_REGNUM)]
  "TARGET_SVE"
  {
    rtx tmp = gen_reg_rtx (<MODE>mode);
    emit_insn (gen_aarch64_sve_dup<mode>_const (tmp, operands[1],
						CONST1_RTX (<MODE>mode),
						CONST0_RTX (<MODE>mode)));
    emit_insn (gen_vec_extract<mode><Vel> (operands[0], tmp, operands[2]));
    DONE;
  }
)

(define_expand "vec_extract<mode><Vel>"
  [(set (match_operand:<VEL> 0 "register_operand")
	(vec_select:<VEL>
	  (match_operand:SVE_ALL 1 "register_operand")
	  (parallel [(match_operand:SI 2 "nonmemory_operand")])))]
  "TARGET_SVE"
  {
    poly_int64 val;
    if (poly_int_rtx_p (operands[2], &val)
	&& known_eq (val, GET_MODE_NUNITS (<MODE>mode) - 1))
      {
	/* The last element can be extracted with a LASTB and a false
	   predicate.  */
	rtx sel = force_reg (<VPRED>mode, CONST0_RTX (<VPRED>mode));
	emit_insn (gen_extract_last_<mode> (operands[0], sel, operands[1]));
	DONE;
      }
    if (!CONST_INT_P (operands[2]))
      {
	/* Create an index with operand[2] as the base and -1 as the step.
	   It will then be zero for the element we care about.  */
	rtx index = gen_lowpart (<VEL_INT>mode, operands[2]);
	index = force_reg (<VEL_INT>mode, index);
	rtx series = gen_reg_rtx (<V_INT_EQUIV>mode);
	emit_insn (gen_vec_series<v_int_equiv> (series, index, constm1_rtx));

	/* Get a predicate that is true for only that element.  */
	rtx zero = CONST0_RTX (<V_INT_EQUIV>mode);
	rtx cmp = gen_rtx_EQ (<V_INT_EQUIV>mode, series, zero);
	rtx sel = gen_reg_rtx (<VPRED>mode);
	emit_insn (gen_vec_cmp<v_int_equiv><vpred> (sel, cmp, series, zero));

	/* Select the element using LASTB.  */
	emit_insn (gen_extract_last_<mode> (operands[0], sel, operands[1]));
	DONE;
      }
  }
)

;; Extract element zero.  This is a special case because we want to force
;; the registers to be the same for the second alternative, and then
;; split the instruction into nothing after RA.
(define_insn_and_split "*vec_extract<mode><Vel>_0"
  [(set (match_operand:<VEL> 0 "aarch64_simd_nonimmediate_operand" "=r, w, Utv")
	(vec_select:<VEL>
	  (match_operand:SVE_ALL 1 "register_operand" "w, 0, w")
	  (parallel [(const_int 0)])))]
  "TARGET_SVE"
  {
    operands[1] = gen_rtx_REG (<V128>mode, REGNO (operands[1]));
    switch (which_alternative)
      {
	case 0:
	  return "umov\\t%<vwcore>0, %1.<Vetype>[0]";
	case 1:
	  return "#";
	case 2:
	  return "st1\\t{%1.<Vetype>}[0], %0";
	default:
	  gcc_unreachable ();
      }
  }
  "&& reload_completed
   && REG_P (operands[0])
   && REGNO (operands[0]) == REGNO (operands[1])"
  [(const_int 0)]
  {
    emit_note (NOTE_INSN_DELETED);
    DONE;
  }
  [(set_attr "type" "neon_to_gp_q, untyped, neon_store1_one_lane_q")]
)

;; Extract an element from the Advanced SIMD portion of the register.
;; We don't just reuse the aarch64-simd.md pattern because we don't
;; want any change in lane number on big-endian targets.
(define_insn "*vec_extract<mode><Vel>_v128"
  [(set (match_operand:<VEL> 0 "aarch64_simd_nonimmediate_operand" "=r, w, Utv")
	(vec_select:<VEL>
	  (match_operand:SVE_ALL 1 "register_operand" "w, w, w")
	  (parallel [(match_operand:SI 2 "const_int_operand")])))]
  "TARGET_SVE
   && IN_RANGE (INTVAL (operands[2]) * GET_MODE_SIZE (<VEL>mode), 1, 15)"
  {
    operands[1] = gen_rtx_REG (<V128>mode, REGNO (operands[1]));
    switch (which_alternative)
      {
	case 0:
	  return "umov\\t%<vwcore>0, %1.<Vetype>[%2]";
	case 1:
	  return "dup\\t%<Vetype>0, %1.<Vetype>[%2]";
	case 2:
	  return "st1\\t{%1.<Vetype>}[%2], %0";
	default:
	  gcc_unreachable ();
      }
  }
  [(set_attr "type" "neon_to_gp_q, neon_dup_q, neon_store1_one_lane_q")]
)

;; Extract an element in the range of DUP.  This pattern allows the
;; source and destination to be different.
(define_insn "*vec_extract<mode><Vel>_dup"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(vec_select:<VEL>
	  (match_operand:SVE_ALL 1 "register_operand" "w")
	  (parallel [(match_operand:SI 2 "const_int_operand")])))]
  "TARGET_SVE
   && IN_RANGE (INTVAL (operands[2]) * GET_MODE_SIZE (<VEL>mode), 16, 63)"
  {
    operands[0] = gen_rtx_REG (<MODE>mode, REGNO (operands[0]));
    return "dup\t%0.<Vetype>, %1.<Vetype>[%2]";
  }
)

;; Extract an element outside the range of DUP.  This pattern requires the
;; source and destination to be the same.
(define_insn "*vec_extract<mode><Vel>_ext"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(vec_select:<VEL>
	  (match_operand:SVE_ALL 1 "register_operand" "0")
	  (parallel [(match_operand:SI 2 "const_int_operand")])))]
  "TARGET_SVE && INTVAL (operands[2]) * GET_MODE_SIZE (<VEL>mode) >= 64"
  {
    operands[0] = gen_rtx_REG (<MODE>mode, REGNO (operands[0]));
    operands[2] = GEN_INT (INTVAL (operands[2]) * GET_MODE_SIZE (<VEL>mode));
    return "ext\t%0.b, %0.b, %0.b, #%2";
  }
)

;; Extract the last active element of operand 1 into operand 0.
;; If no elements are active, extract the last inactive element instead.
(define_insn "extract_last_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=r, w")
	(unspec:<VEL>
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (match_operand:SVE_ALL 2 "register_operand" "w, w")]
	  UNSPEC_LASTB))]
  "TARGET_SVE"
  "@
   lastb\t%<vwcore>0, %1, %2.<Vetype>
   lastb\t%<Vetype>0, %1, %2.<Vetype>"
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
	  (match_operand:<VEL> 1 "aarch64_sve_dup_operand" "r, w, Uty")))
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

;; Duplicate an Advanced SIMD vector to fill an SVE vector (LE version).
(define_insn "@aarch64_vec_duplicate_vq<mode>_le"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(vec_duplicate:SVE_ALL
	  (match_operand:<V128> 1 "register_operand" "w")))]
  "TARGET_SVE && !BYTES_BIG_ENDIAN"
  {
    operands[1] = gen_rtx_REG (<MODE>mode, REGNO (operands[1]));
    return "dup\t%0.q, %1.q[0]";
  }
)

;; Duplicate an Advanced SIMD vector to fill an SVE vector (BE version).
;; The SVE register layout puts memory lane N into (architectural)
;; register lane N, whereas the Advanced SIMD layout puts the memory
;; lsb into the register lsb.  We therefore have to describe this in rtl
;; terms as a reverse of the V128 vector followed by a duplicate.
(define_insn "@aarch64_vec_duplicate_vq<mode>_be"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(vec_duplicate:SVE_ALL
	  (vec_select:<V128>
	    (match_operand:<V128> 1 "register_operand" "w")
	    (match_operand 2 "descending_int_parallel"))))]
  "TARGET_SVE
   && BYTES_BIG_ENDIAN
   && known_eq (INTVAL (XVECEXP (operands[2], 0, 0)),
		GET_MODE_NUNITS (<V128>mode) - 1)"
  {
    operands[1] = gen_rtx_REG (<MODE>mode, REGNO (operands[1]));
    return "dup\t%0.q, %1.q[0]";
  }
)

;; This is used for vec_duplicate<mode>s from memory, but can also
;; be used by combine to optimize selects of a a vec_duplicate<mode>
;; with zero.
(define_insn "sve_ld1r<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (vec_duplicate:SVE_ALL
	     (match_operand:<VEL> 2 "aarch64_sve_ld1r_operand" "Uty"))
	   (match_operand:SVE_ALL 3 "aarch64_simd_imm_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "ld1r<Vesize>\t%0.<Vetype>, %1/z, %2"
)

;; Load 128 bits from memory under predicate control and duplicate to
;; fill a vector.
(define_insn "@aarch64_sve_ld1rq<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:<V128> 1 "aarch64_sve_ld1rq_operand" "UtQ")]
	  UNSPEC_LD1RQ))]
  "TARGET_SVE"
  "ld1rq<Vesize>\t%0.<Vetype>, %2/z, %1"
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
	  (match_operand:<VEL> 1 "aarch64_sve_index_operand" "Usi, r, r")
	  (match_operand:<VEL> 2 "aarch64_sve_index_operand" "r, Usi, r")))]
  "TARGET_SVE"
  "@
   index\t%0.<Vetype>, #%1, %<vw>2
   index\t%0.<Vetype>, %<vw>1, #%2
   index\t%0.<Vetype>, %<vw>1, %<vw>2"
)

;; Optimize {x, x, x, x, ...} + {0, n, 2*n, 3*n, ...} if n is in range
;; of an INDEX instruction.
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

;; Unpredicated LD[234].
(define_expand "vec_load_lanes<mode><vsingle>"
  [(set (match_operand:SVE_STRUCT 0 "register_operand")
	(unspec:SVE_STRUCT
	  [(match_dup 2)
	   (match_operand:SVE_STRUCT 1 "memory_operand")]
	  UNSPEC_LDN))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated LD[234].
(define_insn "vec_mask_load_lanes<mode><vsingle>"
  [(set (match_operand:SVE_STRUCT 0 "register_operand" "=w")
	(unspec:SVE_STRUCT
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:SVE_STRUCT 1 "memory_operand" "m")]
	  UNSPEC_LDN))]
  "TARGET_SVE"
  "ld<vector_count><Vesize>\t%0, %2/z, %1"
)

;; Unpredicated ST[234].  This is always a full update, so the dependence
;; on the old value of the memory location (via (match_dup 0)) is redundant.
;; There doesn't seem to be any obvious benefit to treating the all-true
;; case differently though.  In particular, it's very unlikely that we'll
;; only find out during RTL that a store_lanes is dead.
(define_expand "vec_store_lanes<mode><vsingle>"
  [(set (match_operand:SVE_STRUCT 0 "memory_operand")
	(unspec:SVE_STRUCT
	  [(match_dup 2)
	   (match_operand:SVE_STRUCT 1 "register_operand")
	   (match_dup 0)]
	  UNSPEC_STN))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated ST[234].
(define_insn "vec_mask_store_lanes<mode><vsingle>"
  [(set (match_operand:SVE_STRUCT 0 "memory_operand" "+m")
	(unspec:SVE_STRUCT
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:SVE_STRUCT 1 "register_operand" "w")
	   (match_dup 0)]
	  UNSPEC_STN))]
  "TARGET_SVE"
  "st<vector_count><Vesize>\t%1, %2, %0"
)

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

(define_insn "@aarch64_sve_tbl<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL
	  [(match_operand:SVE_ALL 1 "register_operand" "w")
	   (match_operand:<V_INT_EQUIV> 2 "register_operand" "w")]
	  UNSPEC_TBL))]
  "TARGET_SVE"
  "tbl\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
)

(define_insn "@aarch64_sve_<perm_insn><mode>"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(unspec:PRED_ALL [(match_operand:PRED_ALL 1 "register_operand" "Upa")
			  (match_operand:PRED_ALL 2 "register_operand" "Upa")]
			 PERMUTE))]
  "TARGET_SVE"
  "<perm_insn>\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
)

(define_insn "@aarch64_sve_<perm_insn><mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL [(match_operand:SVE_ALL 1 "register_operand" "w")
			 (match_operand:SVE_ALL 2 "register_operand" "w")]
			PERMUTE))]
  "TARGET_SVE"
  "<perm_insn>\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
)

(define_insn "*aarch64_sve_rev64<mode>"
  [(set (match_operand:SVE_BHS 0 "register_operand" "=w")
	(unspec:SVE_BHS
	  [(match_operand:VNx2BI 1 "register_operand" "Upl")
	   (unspec:SVE_BHS [(match_operand:SVE_BHS 2 "register_operand" "w")]
			   UNSPEC_REV64)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "rev<Vesize>\t%0.d, %1/m, %2.d"
)

(define_insn "*aarch64_sve_rev32<mode>"
  [(set (match_operand:SVE_BH 0 "register_operand" "=w")
	(unspec:SVE_BH
	  [(match_operand:VNx4BI 1 "register_operand" "Upl")
	   (unspec:SVE_BH [(match_operand:SVE_BH 2 "register_operand" "w")]
			  UNSPEC_REV32)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "rev<Vesize>\t%0.s, %1/m, %2.s"
)

(define_insn "*aarch64_sve_rev16vnx16qi"
  [(set (match_operand:VNx16QI 0 "register_operand" "=w")
	(unspec:VNx16QI
	  [(match_operand:VNx8BI 1 "register_operand" "Upl")
	   (unspec:VNx16QI [(match_operand:VNx16QI 2 "register_operand" "w")]
			   UNSPEC_REV16)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "revb\t%0.h, %1/m, %2.h"
)

(define_insn "@aarch64_sve_rev<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL [(match_operand:SVE_ALL 1 "register_operand" "w")]
			UNSPEC_REV))]
  "TARGET_SVE"
  "rev\t%0.<Vetype>, %1.<Vetype>")

(define_insn "@aarch64_sve_rev<mode>"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(unspec:PRED_ALL [(match_operand:PRED_ALL 1 "register_operand" "Upa")]
			 UNSPEC_REV))]
  "TARGET_SVE"
  "rev\t%0.<Vetype>, %1.<Vetype>")

(define_insn "@aarch64_sve_dup_lane<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(vec_duplicate:SVE_ALL
	  (vec_select:<VEL>
	    (match_operand:SVE_ALL 1 "register_operand" "w")
	    (parallel [(match_operand:SI 2 "const_int_operand")]))))]
  "TARGET_SVE
   && IN_RANGE (INTVAL (operands[2]) * GET_MODE_SIZE (<VEL>mode), 0, 63)"
  "dup\t%0.<Vetype>, %1.<Vetype>[%2]"
)

;; Use DUP.Q to duplicate a 128-bit segment of a register.
;;
;; The vec_select:<V128> sets memory lane number N of the V128 to lane
;; number op2 + N of op1.  (We don't need to distinguish between memory
;; and architectural register lane numbering for op1 or op0, since the
;; two numbering schemes are the same for SVE.)
;;
;; The vec_duplicate:SVE_ALL then copies memory lane number N of the
;; V128 (and thus lane number op2 + N of op1) to lane numbers N + I * STEP
;; of op0.  We therefore get the correct result for both endiannesses.
;;
;; The wrinkle is that for big-endian V128 registers, memory lane numbering
;; is in the opposite order to architectural register lane numbering.
;; Thus if we were to do this operation via a V128 temporary register,
;; the vec_select and vec_duplicate would both involve a reverse operation
;; for big-endian targets.  In this fused pattern the two reverses cancel
;; each other out.
(define_insn "@aarch64_sve_dupq_lane<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(vec_duplicate:SVE_ALL
	  (vec_select:<V128>
	    (match_operand:SVE_ALL 1 "register_operand" "w")
	    (match_operand 2 "ascending_int_parallel"))))]
  "TARGET_SVE
   && (INTVAL (XVECEXP (operands[2], 0, 0))
       * GET_MODE_SIZE (<VEL>mode)) % 16 == 0
   && IN_RANGE (INTVAL (XVECEXP (operands[2], 0, 0))
		* GET_MODE_SIZE (<VEL>mode), 0, 63)"
  {
    unsigned int byte = (INTVAL (XVECEXP (operands[2], 0, 0))
			 * GET_MODE_SIZE (<VEL>mode));
    operands[2] = gen_int_mode (byte / 16, DImode);
    return "dup\t%0.q, %1.q[%2]";
  }
)

;; Note that the immediate (third) operand is the lane index not
;; the byte index.
(define_insn "@aarch64_sve_ext<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w, ?&w")
	(unspec:SVE_ALL [(match_operand:SVE_ALL 1 "register_operand" "0, w")
			 (match_operand:SVE_ALL 2 "register_operand" "w, w")
			 (match_operand:SI 3 "const_int_operand")]
			UNSPEC_EXT))]
  "TARGET_SVE
   && IN_RANGE (INTVAL (operands[3]) * GET_MODE_SIZE (<VEL>mode), 0, 255)"
  {
    operands[3] = GEN_INT (INTVAL (operands[3]) * GET_MODE_SIZE (<VEL>mode));
    return (which_alternative == 0
	    ? "ext\\t%0.b, %0.b, %2.b, #%3"
	    : "movprfx\t%0, %1\;ext\\t%0.b, %0.b, %2.b, #%3");
  }
  [(set_attr "movprfx" "*,yes")]
)

(define_insn "add<mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, w, ?w, ?w, w")
	(plus:SVE_I
	  (match_operand:SVE_I 1 "register_operand" "%0, 0, 0, w, w, w")
	  (match_operand:SVE_I 2 "aarch64_sve_add_operand" "vsa, vsn, vsi, vsa, vsn, w")))]
  "TARGET_SVE"
  "@
   add\t%0.<Vetype>, %0.<Vetype>, #%D2
   sub\t%0.<Vetype>, %0.<Vetype>, #%N2
   * return aarch64_output_sve_vector_inc_dec (\"%0.<Vetype>\", operands[2]);
   movprfx\t%0, %1\;add\t%0.<Vetype>, %0.<Vetype>, #%D2
   movprfx\t%0, %1\;sub\t%0.<Vetype>, %0.<Vetype>, #%N2
   add\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
  [(set_attr "movprfx" "*,*,*,yes,yes,*")]
)

(define_insn "sub<mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, ?&w")
	(minus:SVE_I
	  (match_operand:SVE_I 1 "aarch64_sve_arith_operand" "w, vsa, vsa")
	  (match_operand:SVE_I 2 "register_operand" "w, 0, w")))]
  "TARGET_SVE"
  "@
   sub\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>
   subr\t%0.<Vetype>, %0.<Vetype>, #%D1
   movprfx\t%0, %2\;subr\t%0.<Vetype>, %0.<Vetype>, #%D1"
  [(set_attr "movprfx" "*,*,yes")]
)

;; An unshifted and unscaled ADR.  This is functionally equivalent to an ADD,
;; but the svadrb intrinsics should preserve the user's choice.
(define_insn "@aarch64_adr<mode>"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w")
	(unspec:SVE_SDI
	  [(match_operand:SVE_SDI 1 "register_operand" "w")
	   (match_operand:SVE_SDI 2 "register_operand" "w")]
	  UNSPEC_ADR))]
  "TARGET_SVE"
  "adr\t%0.<Vetype>, [%1.<Vetype>, %2.<Vetype>]"
)

;; Same, but with the offset being sign-extended from the low 32 bits.
(define_insn_and_split "aarch64_adr_sxtw"
  [(set (match_operand:VNx2DI 0 "register_operand" "=w")
	(unspec:VNx2DI
	  [(match_operand:VNx2DI 1 "register_operand" "w")
	   (unspec:VNx2DI
	     [(match_operand 3)
	      (sign_extend:VNx2DI
		(truncate:VNx2SI
		  (match_operand:VNx2DI 2 "register_operand" "w")))]
	     UNSPEC_MERGE_PTRUE)]
	  UNSPEC_ADR))]
  "TARGET_SVE"
  "adr\t%0.d, [%1.d, %2.d, sxtw]"
  "&& !CONSTANT_P (operands[3])"
  [(const_int 0)]
  {
    emit_insn (gen_aarch64_adr_sxtw (operands[0], operands[1], operands[2],
				     CONSTM1_RTX (VNx2BImode)));
    DONE;
  }
)

;; Same, but with the offset being zero-extended from the low 32 bits.
(define_insn "*aarch64_adr_uxtw"
  [(set (match_operand:VNx2DI 0 "register_operand" "=w")
	(unspec:VNx2DI
	  [(match_operand:VNx2DI 1 "register_operand" "w")
	   (and:VNx2DI
	     (match_operand:VNx2DI 2 "register_operand" "w")
	     (match_operand:VNx2DI 3 "aarch64_sve_uxtw_immediate"))]
	  UNSPEC_ADR))]
  "TARGET_SVE"
  "adr\t%0.d, [%1.d, %2.d, uxtw]"
)

;; ADR with a nonzero shift.
(define_insn "@aarch64_adr<mode>_shift"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w")
	(plus:SVE_SDI
	  (ashift:SVE_SDI
	    (match_operand:SVE_SDI 2 "register_operand" "w")
	    (match_operand:SVE_SDI 3 "const_1_to_3_operand"))
	  (match_operand:SVE_SDI 1 "register_operand" "w")))]
  "TARGET_SVE"
  "adr\t%0.<Vetype>, [%1.<Vetype>, %2.<Vetype>, lsl %3]"
)

;; Same, but with the index being sign-extended from the low 32 bits.
(define_insn_and_rewrite "*aarch64_adr_shift_sxtw"
  [(set (match_operand:VNx2DI 0 "register_operand" "=w")
	(plus:VNx2DI
	  (ashift:VNx2DI
	    (unspec:VNx2DI
	      [(match_operand 4)
	       (sign_extend:VNx2DI
		 (truncate:VNx2SI
		   (match_operand:VNx2DI 2 "register_operand" "w")))]
	      UNSPEC_MERGE_PTRUE)
	    (match_operand:VNx2DI 3 "const_1_to_3_operand"))
	  (match_operand:VNx2DI 1 "register_operand" "w")))]
  "TARGET_SVE"
  "adr\t%0.d, [%1.d, %2.d, sxtw %3]"
  "&& !CONSTANT_P (operands[4])"
  {
    operands[4] = CONSTM1_RTX (VNx2BImode);
  }
)

;; Same, but with the index being zero-extended from the low 32 bits.
(define_insn "*aarch64_adr_shift_uxtw"
  [(set (match_operand:VNx2DI 0 "register_operand" "=w")
	(plus:VNx2DI
	  (ashift:VNx2DI
	    (and:VNx2DI
	      (match_operand:VNx2DI 2 "register_operand" "w")
	      (match_operand:VNx2DI 4 "aarch64_sve_uxtw_immediate"))
	    (match_operand:VNx2DI 3 "const_1_to_3_operand"))
	  (match_operand:VNx2DI 1 "register_operand" "w")))]
  "TARGET_SVE"
  "adr\t%0.d, [%1.d, %2.d, uxtw %3]"
)

;; Unpredicated integer binary operations that have an immediate form.
(define_expand "<optab><mode>3"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_dup 3)
	   (SVE_INT_BINARY_IMM:SVE_I
	     (match_operand:SVE_I 1 "register_operand")
	     (match_operand:SVE_I 2 "aarch64_sve_<sve_imm_pred>_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Integer binary operations that have an immediate form, predicated
;; with a PTRUE.  We don't actually need the predicate for the first
;; and third alternatives, but using Upa or X isn't likely to gain much
;; and would make the instruction seem less uniform to the register
;; allocator.
(define_insn_and_split "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, ?&w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (SVE_INT_BINARY_IMM:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "%0, 0, w, w")
	     (match_operand:SVE_I 3 "aarch64_sve_<sve_imm_pred>_operand" "<sve_imm_con>, w, <sve_imm_con>, w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   #
   <sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   #
   movprfx\t%0, %2\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  ; Split the unpredicated form after reload, so that we don't have
  ; the unnecessary PTRUE.
  "&& reload_completed
   && !register_operand (operands[3], <MODE>mode)"
  [(set (match_dup 0) (SVE_INT_BINARY_IMM:SVE_I (match_dup 2) (match_dup 3)))]
  ""
  [(set_attr "movprfx" "*,*,yes,yes")]
)

;; Unpredicated binary operations with a constant (post-RA only).
;; These are generated by splitting a predicated instruction whose
;; predicate is unused.
(define_insn "*post_ra_<optab><mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w,?&w")
	(SVE_INT_BINARY_IMM:SVE_I
	  (match_operand:SVE_I 1 "register_operand" "0,w")
	  (match_operand:SVE_I 2 "aarch64_sve_<sve_imm_pred>_immediate")))]
  "TARGET_SVE && reload_completed"
  "@
   <sve_int_op>\t%0.<Vetype>, %0.<Vetype>, #%<sve_imm_prefix>2
   movprfx\t%0, %1\;<sve_int_op>\t%0.<Vetype>, %0.<Vetype>, #%<sve_imm_prefix>2"
  [(set_attr "movprfx" "*,yes")]
)

(define_insn "@aarch64_pred_fma<mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, ?&w")
	(plus:SVE_I
	  (unspec:SVE_I
	    [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand" "%0, w, w")
			 (match_operand:SVE_I 3 "register_operand" "w, w, w"))]
	    UNSPEC_MERGE_PTRUE)
	  (match_operand:SVE_I 4 "register_operand" "w, 0, w")))]
  "TARGET_SVE"
  "@
   mad\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   mla\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0, %4\;mla\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,*,yes")]
)

;; Predicated fma operations with select.
(define_expand "cond_fma<mode>"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand")
	   (plus:SVE_I
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand")
			 (match_operand:SVE_I 3 "register_operand"))
	     (match_operand:SVE_I 4 "register_operand"))
	   (match_operand:SVE_I 5 "register_operand")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  {
    /* Swap the multiplication operands if the fallback value is the
       second of the two.  */
    if (rtx_equal_p (operands[3], operands[5]))
      std::swap (operands[2], operands[3]);
  }
)

;; Predicated fma operations with select first operand.
(define_insn "*cond_fma<mode>_2"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (plus:SVE_I
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand" "0, w")
			 (match_operand:SVE_I 3 "register_operand" "w, w"))
	     (match_operand:SVE_I 4 "register_operand" "w, w"))
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   mad\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   movprfx\t%0, %2\;mad\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated fma operations with select operand 4.
(define_insn "*cond_fma<mode>_4"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (plus:SVE_I
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand" "w, w")
			 (match_operand:SVE_I 3 "register_operand" "w, w"))
	     (match_operand:SVE_I 4 "register_operand" "0, w"))
	   (match_dup 4)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   mla\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0, %4\;mla\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated fma operations with select.
;; Inactive lanes are distinct from the other inputs.
(define_insn_and_rewrite "*cond_fma<mode>_any"
  [(set (match_operand:SVE_I 0 "register_operand" "=&w, &w, &w, &w, &w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	   (plus:SVE_I
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand" "w, 0, w, w, w, w")
			 (match_operand:SVE_I 3 "register_operand" "w, w, 0, w, w, w"))
	     (match_operand:SVE_I 4 "register_operand" "w, w, w, 0, w, w"))
	   (match_operand:SVE_I 5 "aarch64_simd_reg_or_zero" "Dz, Dz, Dz, Dz, 0, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE
   && !rtx_equal_p (operands[2], operands[5])
   && !rtx_equal_p (operands[3], operands[5])
   && !rtx_equal_p (operands[4], operands[5])"
  "@
   movprfx\t%0.<Vetype>, %1/z, %4.<Vetype>\;mla\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;mad\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;mad\t%0.<Vetype>, %1/m, %2.<Vetype>, %4.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %4.<Vetype>\;mla\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/m, %4.<Vetype>\;mla\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   #"
  "&& reload_completed
   && !CONSTANT_P (operands[5])
   && !rtx_equal_p (operands[0], operands[5])"
  {
    emit_insn (gen_vcond_mask_<mode><vpred> (operands[0], operands[4],
					     operands[5], operands[1]));
    operands[5] = operands[4] = operands[0];
  }
  [(set_attr "movprfx" "yes")]
)

;; Predicated fnma operations with select.
(define_expand "cond_fnma<mode>"
  [(set (match_operand:SVE_I 0 "register_operand")
   (unspec:SVE_I
	[(match_operand:<VPRED> 1 "register_operand")
	 (minus:SVE_I
	   (match_operand:SVE_I 4 "register_operand")
	   (mult:SVE_I (match_operand:SVE_I 2 "register_operand")
		       (match_operand:SVE_I 3 "register_operand")))
	 (match_operand:SVE_I 5 "register_operand")]
	UNSPEC_SEL))]
  "TARGET_SVE"
  {
    /* Swap the multiplication operands if the fallback value is the
       second of the two.  */
    if (rtx_equal_p (operands[3], operands[5]))
      std::swap (operands[2], operands[3]);
  }
)

;; Predicated fnma operations with select input 2.
(define_insn "*cond_fnma<mode>_2"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (minus:SVE_I
	     (match_operand:SVE_I 4 "register_operand" "w, w")
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand" "0, w")
			 (match_operand:SVE_I 3 "register_operand" "w, w")))
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   msb\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   movprfx\t%0, %2\;msb\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated fnma operations with select input 4.
(define_insn "*cond_fnma<mode>_4"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (minus:SVE_I
	     (match_operand:SVE_I 4 "register_operand" "0, w")
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand" "w, w")
			 (match_operand:SVE_I 3 "register_operand" "w, w")))
	   (match_dup 4)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   mls\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0, %4\;mls\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated fnma operations with select.
;; Inactive lanes are distinct from the other inputs.
(define_insn_and_rewrite "*cond_fnma<mode>_any"
  [(set (match_operand:SVE_I 0 "register_operand" "=&w, &w, &w, &w, &w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	   (minus:SVE_I
	     (match_operand:SVE_I 4 "register_operand" "0, w, w, w, w, w")
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand" "w, 0, w, w, w, w")
			 (match_operand:SVE_I 3 "register_operand" "w, w, 0, w, w, w")))
	   (match_operand:SVE_I 5 "aarch64_simd_reg_or_zero" "Dz, Dz, Dz, Dz, 0, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE
   && !rtx_equal_p (operands[2], operands[5])
   && !rtx_equal_p (operands[3], operands[5])
   && !rtx_equal_p (operands[4], operands[5])"
  "@
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;mls\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;msb\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;msb\t%0.<Vetype>, %1/m, %2.<Vetype>, %4.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %4.<Vetype>\;mls\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/m, %4.<Vetype>\;mls\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   #"
  "&& reload_completed
   && !CONSTANT_P (operands[5])
   && !rtx_equal_p (operands[0], operands[5])"
  {
    emit_insn (gen_vcond_mask_<mode><vpred> (operands[0], operands[4],
					     operands[5], operands[1]));
    operands[5] = operands[4] = operands[0];
  }
  [(set_attr "movprfx" "yes")]
)

(define_insn "@aarch64_pred_fnma<mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, ?&w")
	(minus:SVE_I
	  (match_operand:SVE_I 4 "register_operand" "w, 0, w")
	  (unspec:SVE_I
	    [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	     (mult:SVE_I (match_operand:SVE_I 2 "register_operand" "%0, w, w")
			 (match_operand:SVE_I 3 "register_operand" "w, w, w"))]
	    UNSPEC_MERGE_PTRUE)))]
  "TARGET_SVE"
  "@
   msb\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   mls\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0, %4\;mls\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,*,yes")]
)

;; Unpredicated highpart multiplication.
(define_expand "<su>mul<mode>3_highpart"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_dup 3)
	   (unspec:SVE_I [(match_operand:SVE_I 1 "register_operand")
			  (match_operand:SVE_I 2 "register_operand")]
			 MUL_HIGHPART)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated unspec integer operations.
(define_insn "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_I [(match_operand:SVE_I 2 "register_operand" "%0, w")
			  (match_operand:SVE_I 3 "register_operand" "w, w")]
			 SVE_INT_BINARY_REG)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   <sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated unspec integer operations with select.
(define_expand "@cond_<optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand")
	   (unspec:SVE_I
	     [(match_operand:SVE_I 2 "register_operand")
	      (match_operand:SVE_I 3 "register_operand")]
	     SVE_INT_BINARY_REG)
	   (match_operand:SVE_I 4 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
{
  /* Only target code is aware of these operations, so we don't need
     to handle the fully-general case.  */
  gcc_assert (rtx_equal_p (operands[2], operands[4])
	      || CONSTANT_P (operands[4]));
})

;; Predicated unspec integer operations with select matching the first input.
(define_insn "*cond_<optab><mode>_2"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_I
	     [(match_operand:SVE_I 2 "register_operand" "0, w")
	      (match_operand:SVE_I 3 "register_operand" "w, w")]
	     SVE_INT_BINARY_REG)
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,yes")])

;; Predicated unspec integer operations with select matching zero.
(define_insn "*cond_<optab><mode>_z"
  [(set (match_operand:SVE_I 0 "register_operand" "=&w, &w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_I
	     [(match_operand:SVE_I 2 "register_operand" "%0, w")
	      (match_operand:SVE_I 3 "register_operand" "w, w")]
	     SVE_INT_BINARY_REG)
	   (match_operand:SVE_I 4 "aarch64_simd_imm_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "yes")])

;; Unpredicated division.
(define_expand "<optab><mode>3"
  [(set (match_operand:SVE_SDI 0 "register_operand")
	(unspec:SVE_SDI
	  [(match_dup 3)
	   (SVE_INT_BINARY_SD:SVE_SDI
	     (match_operand:SVE_SDI 1 "register_operand")
	     (match_operand:SVE_SDI 2 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Division predicated with a PTRUE.
(define_insn "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w, w, ?&w")
	(unspec:SVE_SDI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	   (SVE_INT_BINARY_SD:SVE_SDI
	     (match_operand:SVE_SDI 2 "register_operand" "0, w, w")
	     (match_operand:SVE_SDI 3 "register_operand" "w, 0, w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   <sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   <sve_int_op>r\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0, %2\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,*,yes")]
)

;; Unpredicated ABS, NEG, NOT and POPCOUNT.
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

;; ABS, NEG, NOT and POPCOUNT predicated with a PTRUE.
(define_insn "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (SVE_INT_UNARY:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<sve_int_op>\t%0.<Vetype>, %1/m, %2.<Vetype>"
)

;; Predicated NEG, NOT, ABS and POPCOUNT with select.
(define_insn "@cond_<optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, &w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	   (SVE_INT_UNARY:SVE_I (match_operand:SVE_I 2 "register_operand" "w, w, w"))
	   (match_operand:SVE_I 3 "aarch64_simd_reg_or_zero" "0, Dz, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_int_op>\t%0.<Vetype>, %1/m, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %2.<Vetype>
   movprfx\t%0, %3\;<sve_int_op>\t%0.<Vetype>, %1/m, %2.<Vetype>"
  [(set_attr "movprfx" "*,yes,yes")]
)

;; Predicated integer unary operations.
(define_insn "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (unspec:SVE_I
	     [(match_operand:SVE_I 2 "register_operand" "w")]
	     SVE_INT_UNARY)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<sve_int_op>\t%0.<Vetype>, %1/m, %2.<Vetype>"
)

;; Predicated integer unary operations with select.
(define_insn "@cond_<optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, &w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	   (unspec:SVE_I
	     [(match_operand:SVE_I 2 "register_operand" "w, w, w")]
	     SVE_INT_UNARY)
	   (match_operand:SVE_I 3 "aarch64_simd_reg_or_zero" "0, Dz, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_int_op>\t%0.<Vetype>, %1/m, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %2.<Vetype>
   movprfx\t%0, %3\;<sve_int_op>\t%0.<Vetype>, %1/m, %2.<Vetype>"
  [(set_attr "movprfx" "*,yes,yes")]
)

;; Vector AND, ORR and XOR.
(define_insn "<optab><mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?w, w")
	(LOGICAL:SVE_I
	  (match_operand:SVE_I 1 "register_operand" "%0, w, w")
	  (match_operand:SVE_I 2 "aarch64_sve_logical_operand" "vsl, vsl, w")))]
  "TARGET_SVE"
  "@
   <logical>\t%0.<Vetype>, %0.<Vetype>, #%C2
   movprfx\t%0, %1\;<logical>\t%0.<Vetype>, %0.<Vetype>, #%C2
   <logical>\t%0.d, %1.d, %2.d"
  [(set_attr "movprfx" "*,yes,*")]
)

;; Predicated BIC with select.
(define_expand "@cond_bic<mode>"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand")
	   (and:SVE_I
	     (not:SVE_I (match_operand:SVE_I 3 "register_operand"))
	     (match_operand:SVE_I 2 "register_operand"))
	   (match_operand:SVE_I 4 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
)

;; Predicated BIC with select 2nd operand.
(define_insn "*cond_bic<mode>_2"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (and:SVE_I
	     (not:SVE_I (match_operand:SVE_I 3 "register_operand" "w, w"))
	     (match_operand:SVE_I 2 "register_operand" "0, w"))
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   bic\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;bic\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated BIC with select matching zero.
(define_insn "*cond_bic<mode>_z"
  [(set (match_operand:SVE_I 0 "register_operand" "=&w, &w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (and:SVE_I
	     (not:SVE_I (match_operand:SVE_I 3 "register_operand" "w, w"))
	     (match_operand:SVE_I 2 "register_operand" "0, w"))
	   (match_operand:SVE_I 4 "aarch64_simd_imm_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;bic\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;bic\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "yes")]
)

;; Vector AND, ORR and XOR on floating-point modes.  We avoid subregs
;; by providing this, but we need to use UNSPECs since rtx logical ops
;; aren't defined for floating-point modes.
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
(define_insn "@aarch64_bic<mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(and:SVE_I
	  (not:SVE_I (match_operand:SVE_I 2 "register_operand" "w"))
	  (match_operand:SVE_I 1 "register_operand" "w")))]
  "TARGET_SVE"
  "bic\t%0.d, %1.d, %2.d"
)

;; Predicate AND.  We can reuse one of the inputs as the GP.
(define_insn "and<mode>3"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL (match_operand:PRED_ALL 1 "register_operand" "Upa")
		      (match_operand:PRED_ALL 2 "register_operand" "Upa")))]
  "TARGET_SVE"
  "and\t%0.b, %1/z, %1.b, %2.b"
)

;; Unpredicated predicate ORR and XOR.
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

;; Predicated predicate AND, ORR and XOR.
(define_insn "@aarch64_pred_<optab><mode>_z"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL
	  (LOGICAL:PRED_ALL
	    (match_operand:PRED_ALL 2 "register_operand" "Upa")
	    (match_operand:PRED_ALL 3 "register_operand" "Upa"))
	  (match_operand:PRED_ALL 1 "register_operand" "Upa")))]
  "TARGET_SVE"
  "<logical>\t%0.b, %1/z, %2.b, %3.b"
)

;; Perform a logical operation on operands 2 and 3, using operand 1 as
;; the GP (which is known to be a PTRUE).  Store the result in operand 0
;; and set the flags in the same way as for PTEST.
(define_insn "*<optab><mode>3_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_operand 4)
	   (and:PRED_ALL
	     (LOGICAL:PRED_ALL
	       (match_operand:PRED_ALL 2 "register_operand" "Upa")
	       (match_operand:PRED_ALL 3 "register_operand" "Upa"))
	     (match_dup 4))
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL (LOGICAL:PRED_ALL (match_dup 2) (match_dup 3))
		      (match_dup 4)))]
  "TARGET_SVE"
  "<logical>s\t%0.b, %1/z, %2.b, %3.b"
)

;; Same with just the flags result.
(define_insn "*<optab><mode>3_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_operand 4)
	   (and:PRED_ALL
	     (LOGICAL:PRED_ALL
	       (match_operand:PRED_ALL 2 "register_operand" "Upa")
	       (match_operand:PRED_ALL 3 "register_operand" "Upa"))
	     (match_dup 4))
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (clobber (match_scratch:VNx16BI 0 "=Upa"))]
  "TARGET_SVE"
  "<logical>s\t%0.b, %1/z, %2.b, %3.b"
)

;; Unpredicated predicate inverse.
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

;; Predicated predicate inverse.
(define_insn "*one_cmpl<mode>3"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL
	  (not:PRED_ALL (match_operand:PRED_ALL 2 "register_operand" "Upa"))
	  (match_operand:PRED_ALL 1 "register_operand" "Upa")))]
  "TARGET_SVE"
  "not\t%0.b, %1/z, %2.b"
)

;; Predicated predicate BIC and ORN.
(define_insn "aarch64_pred_<nlogical><mode>_z"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL
	  (NLOGICAL:PRED_ALL
	    (not:PRED_ALL (match_operand:PRED_ALL 3 "register_operand" "Upa"))
	    (match_operand:PRED_ALL 2 "register_operand" "Upa"))
	  (match_operand:PRED_ALL 1 "register_operand" "Upa")))]
  "TARGET_SVE"
  "<nlogical>\t%0.b, %1/z, %2.b, %3.b"
)

;; Same, but set the flags as a side-effect.
(define_insn "*<optab><mode>3_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_operand 4)
	   (and:PRED_ALL
	     (NLOGICAL:PRED_ALL
	       (not:PRED_ALL
		 (match_operand:PRED_ALL 3 "register_operand" "Upa"))
	       (match_operand:PRED_ALL 2 "register_operand" "Upa"))
	     (match_dup 4))
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL (NLOGICAL:PRED_ALL
			(not:PRED_ALL (match_dup 3))
			(match_dup 2))
		      (match_dup 4)))]
  "TARGET_SVE"
  "<nlogical>s\t%0.b, %1/z, %2.b, %3.b"
)

;; Same with just the flags result.
(define_insn "*<optab><mode>3_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_operand 4)
	   (and:PRED_ALL
	     (NLOGICAL:PRED_ALL
	       (not:PRED_ALL
		 (match_operand:PRED_ALL 3 "register_operand" "Upa"))
	       (match_operand:PRED_ALL 2 "register_operand" "Upa"))
	     (match_dup 4))
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (clobber (match_scratch:VNx16BI 0 "=Upa"))]
  "TARGET_SVE"
  "<nlogical>s\t%0.b, %1/z, %2.b, %3.b"
)

;; Predicated predicate NAND and NOR.
(define_insn "aarch64_pred_<logical_nn><mode>_z"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL
	  (NLOGICAL:PRED_ALL
	    (not:PRED_ALL (match_operand:PRED_ALL 2 "register_operand" "Upa"))
	    (not:PRED_ALL (match_operand:PRED_ALL 3 "register_operand" "Upa")))
	  (match_operand:PRED_ALL 1 "register_operand" "Upa")))]
  "TARGET_SVE"
  "<logical_nn>\t%0.b, %1/z, %2.b, %3.b"
)

;; Same, but set the flags as a side-effect.
(define_insn "*<optab><mode>3_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_operand 4)
	   (and:PRED_ALL
	     (NLOGICAL:PRED_ALL
	       (not:PRED_ALL
		 (match_operand:PRED_ALL 2 "register_operand" "Upa"))
	       (not:PRED_ALL
		 (match_operand:PRED_ALL 3 "register_operand" "Upa")))
	     (match_dup 4))
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(and:PRED_ALL (NLOGICAL:PRED_ALL
			(not:PRED_ALL (match_dup 2))
			(not:PRED_ALL (match_dup 3)))
		      (match_dup 4)))]
  "TARGET_SVE"
  "<logical_nn>s\t%0.b, %1/z, %2.b, %3.b"
)

;; Same with just the flags result.
(define_insn "*<optab><mode>3_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_operand 4)
	   (and:PRED_ALL
	     (NLOGICAL:PRED_ALL
	       (not:PRED_ALL
		 (match_operand:PRED_ALL 2 "register_operand" "Upa"))
	       (not:PRED_ALL
		 (match_operand:PRED_ALL 3 "register_operand" "Upa")))
	     (match_dup 4))
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (clobber (match_scratch:VNx16BI 0 "=Upa"))]
  "TARGET_SVE"
  "<logical_nn>s\t%0.b, %1/z, %2.b, %3.b"
)

;; Predicated SXT[BHW].
(define_insn "@aarch64_pred_sxt<SVE_HSDI:mode><SVE_PARTIAL:mode>"
  [(set (match_operand:SVE_HSDI 0 "register_operand" "=w")
	(unspec:SVE_HSDI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (sign_extend:SVE_HSDI
	     (truncate:SVE_PARTIAL
	       (match_operand:SVE_HSDI 2 "register_operand" "w")))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE && (~<narrower_mask> & <self_mask>) == 0"
  "sxt<SVE_PARTIAL:Vesize>\t%0.<SVE_HSDI:Vetype>, %1/m, %2.<SVE_HSDI:Vetype>"
)

;; Predicated SXT[BHW] with select.
(define_insn "@aarch64_cond_sxt<SVE_HSDI:mode><SVE_PARTIAL:mode>"
  [(set (match_operand:SVE_HSDI 0 "register_operand" "=w, ?&w, ?&w")
	(unspec:SVE_HSDI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	   (sign_extend:SVE_HSDI
	     (truncate:SVE_PARTIAL
	       (match_operand:SVE_HSDI 2 "register_operand" "w, w, w")))
	   (match_operand:SVE_HSDI 3 "aarch64_simd_reg_or_zero" "0, Dz, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE && (~<narrower_mask> & <self_mask>) == 0"
  "@
   sxt<SVE_PARTIAL:Vesize>\t%0.<SVE_HSDI:Vetype>, %1/m, %2.<SVE_HSDI:Vetype>
   movprfx\t%0.<SVE_HSDI:Vetype>, %1/z, %2.<SVE_HSDI:Vetype>\;sxt<SVE_PARTIAL:Vesize>\t%0.<SVE_HSDI:Vetype>, %1/m, %2.<SVE_HSDI:Vetype>
   movprfx\t%0, %3\;sxt<SVE_PARTIAL:Vesize>\t%0.<SVE_HSDI:Vetype>, %1/m, %2.<SVE_HSDI:Vetype>"
  [(set_attr "movprfx" "*,yes,yes")]
)

;; Unpredicated LSL, LSR and ASR by a vector.
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

;; LSL, LSR and ASR by a vector, predicated with a PTRUE.  We don't
;; actually need the predicate for the first alternative, but using Upa
;; or X isn't likely to gain much and would make the instruction seem
;; less uniform to the register allocator.
(define_insn_and_split "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (ASHIFT:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "w, 0, w, w")
	     (match_operand:SVE_I 3 "aarch64_sve_<lr>shift_operand" "D<lr>, w, 0, w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   #
   <shift>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   <shift>r\t%0.<Vetype>, %1/m, %3.<Vetype>, %2.<Vetype>
   movprfx\t%0, %2\;<shift>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  "&& reload_completed
   && !register_operand (operands[3], <MODE>mode)"
  [(set (match_dup 0) (ASHIFT:SVE_I (match_dup 2) (match_dup 3)))]
  ""
  [(set_attr "movprfx" "*,*,*,yes")]
)

;; Unpredicated shift operations by a constant (post-RA only).
;; These are generated by splitting a predicated instruction whose
;; predicate is unused.
(define_insn "*post_ra_v<optab><mode>3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(ASHIFT:SVE_I
	  (match_operand:SVE_I 1 "register_operand" "w")
	  (match_operand:SVE_I 2 "aarch64_simd_<lr>shift_imm")))]
  "TARGET_SVE && reload_completed"
  "<shift>\t%0.<Vetype>, %1.<Vetype>, #%2"
)

;; LSL, LSR and ASR by a scalar, which expands into one of the vector
;; shifts above.
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

(define_expand "@cond_<optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	   [(match_operand:<VPRED> 1 "register_operand")
	    (ASHIFT:SVE_I
	      (match_operand:SVE_I 2 "register_operand")
	      (match_operand:SVE_I 3 "aarch64_sve_<lr>shift_operand"))
	    (match_operand:SVE_I 4 "aarch64_simd_reg_or_zero")]
	 UNSPEC_SEL))]
  "TARGET_SVE"
)

(define_insn "*cond<optab><mode>_m"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, ?&w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (ASHIFT:SVE_I
	      (match_operand:SVE_I 2 "register_operand" "0, 0, w, w")
	      (match_operand:SVE_I 3 "aarch64_sve_<lr>shift_operand" "D<lr>, w, D<lr>, w"))
	   (match_dup 2)]
	 UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <shift>\t%0.<Vetype>, %1/m, %2.<Vetype>, #%3
   <shift>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;<shift>\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0, %2\;<shift>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,*,yes,yes")]
)

(define_insn "*cond<optab><mode>_z"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, &w, ?&w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (ASHIFT:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "w, w, 0, w")
	     (match_operand:SVE_I 3 "aarch64_sve_<lr>shift_operand" "D<lr>, 0, w, w"))
	   (match_operand:SVE_I 4 "aarch64_simd_imm_zero")]
	 UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<shift>\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0.<Vetype>, %1/z, %3.<Vetype>\;<shift>r\t%0.<Vetype>, %1/m, %3.<Vetype>, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<shift>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<shift>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "yes")]
)

(define_insn "@aarch64_wide_<sve_int_op><mode>"
  [(set (match_operand:SVE_BHSI 0 "register_operand" "=w")
	(unspec:SVE_BHSI
	  [(match_operand:SVE_BHSI 1 "register_operand" "w")
	   (match_operand:VNx2DI 2 "register_operand" "w")]
	  SVE_SHIFT_WIDE))]
  "TARGET_SVE"
  "<sve_int_op>\t%0.<Vetype>, %1.<Vetype>, %2.d"
)

(define_expand "@cond_<sve_int_op><mode>"
  [(set (match_operand:SVE_BHSI 0 "register_operand")
	(unspec:SVE_BHSI
	  [(match_operand:<VPRED> 1 "register_operand")
	   (unspec:SVE_BHSI
	     [(match_operand:SVE_BHSI 2 "register_operand")
	      (match_operand:VNx2DI 3 "register_operand")]
	     SVE_SHIFT_WIDE)
	   (match_operand:SVE_BHSI 4 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
)

(define_insn "*cond<sve_int_op><mode>_m"
  [(set (match_operand:SVE_BHSI 0 "register_operand" "=w, ?&w")
	(unspec:SVE_BHSI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_BHSI
	     [(match_operand:SVE_BHSI 2 "register_operand" "0, w")
	      (match_operand:VNx2DI 3 "register_operand" "w, w")]
	     SVE_SHIFT_WIDE)
	   (match_dup 2)]
	 UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.d
   movprfx\t%0, %2\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.d"
  [(set_attr "movprfx" "*, yes")])

(define_insn "*cond<sve_int_op><mode>_z"
  [(set (match_operand:SVE_BHSI 0 "register_operand" "=&w, &w")
	(unspec:SVE_BHSI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_BHSI
	     [(match_operand:SVE_BHSI 2 "register_operand" "0, w")
	      (match_operand:VNx2DI 3 "register_operand" "w, w")]
	     SVE_SHIFT_WIDE)
	   (match_operand:SVE_BHSI 4 "aarch64_simd_imm_zero")]
	 UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.d
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.d"
  [(set_attr "movprfx" "yes")])

;; Test the bits of operand 2 selected by operand 0.  Operands 1 and 3
;; exist only to help combining patterns:
;;
;; - Operand 1 is operand 0 reinterpreted to have the same mode as operand 2
;; - Operand 3 is true if operand 1 is known to be all-true
;;
;; When PRED_ALL != VNx16BI, operand 0 is known to be a canonical predicate
;; for PRED_ALL, i.e. one in which the upper bits in each element are zero.
;;
;; We need the GP to be in VNx16BI rather than PRED_ALL because not all
;; bits of VNx{8,4,2}BI are significant.  E.g. any VNx8BI predicate with
;; the pattern (1x1x1x1x)* acts as a PTRUE for 16-bit data.  However,
;; because PTEST is always a .B operation, (ptest (ptrue) (x:VNx8BI)) must
;; use a (ptrue) that is exactly (10101010)* rather merely (1x1x1x1x)*.
;; Providing operand 1 as well makes it possible to write combiner
;; patterns that use PRED_ALL.  Providing operand 3 makes it possible
;; to combine patterns without proving that operand 0 or 1 folds to
;; a particular constant.
(define_insn "aarch64_ptest<mode>"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC [(match_operand:VNx16BI 0 "register_operand" "Upa")
			(match_operand:PRED_ALL 1)
			(match_operand:PRED_ALL 2 "register_operand" "Upa")
			(match_operand 3 "const_int_operand")]
		       UNSPEC_PTEST))]
  "TARGET_SVE"
  "ptest\t%0, %2.b"
)

;; Set element I of the result if operand1 + J < operand2 for all J in [0, I].
;; with the comparison being unsigned.
(define_insn "@while_<while_optab_cmp><GPI:mode><PRED_ALL:mode>"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(unspec:PRED_ALL [(match_operand:GPI 1 "aarch64_reg_or_zero" "rZ")
			  (match_operand:GPI 2 "aarch64_reg_or_zero" "rZ")]
			 SVE_WHILE))
   (clobber (reg:CC_NZC CC_REGNUM))]
  "TARGET_SVE"
  "while<cmp_op>\t%0.<PRED_ALL:Vetype>, %<w>1, %<w>2"
)

;; WHILELO sets the flags in the same way as a PTEST with a PTRUE GP.
;; Handle the case in which both results are useful.  The GP operand
;; to the PTEST isn't needed, so we allow it to be anything.
(define_insn_and_rewrite "*while_<while_optab_cmp><GPI:mode><PRED_ALL:mode>_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand 3)
	   (match_operand 4)
	   (unspec:PRED_ALL
	     [(match_operand:GPI 1 "aarch64_reg_or_zero" "rZ")
	      (match_operand:GPI 2 "aarch64_reg_or_zero" "rZ")]
	     SVE_WHILE)
	   (const_int 1)]
	  UNSPEC_PTEST))
   (set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(unspec:PRED_ALL [(match_dup 1)
			  (match_dup 2)]
			 SVE_WHILE))]
  "TARGET_SVE"
  "while<cmp_op>\t%0.<PRED_ALL:Vetype>, %<w>1, %<w>2"
  ;; Force the compiler to drop the unused predicate operand, so that we
  ;; don't have an unnecessary PTRUE.
  "&& (!CONSTANT_P (operands[3]) || !CONSTANT_P (operands[4]))"
  {
    operands[3] = CONSTM1_RTX (VNx16BImode);
    operands[4] = CONSTM1_RTX (<PRED_ALL:MODE>mode);
  }
)

;; Same, but handle the case in which only the flags result is useful.
(define_insn_and_rewrite "*while_<while_optab_cmp><GPI:mode><PRED_ALL:mode>_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand 3)
	   (match_operand 4)
	   (unspec:PRED_ALL
	     [(match_operand:GPI 1 "aarch64_reg_or_zero" "rZ")
	      (match_operand:GPI 2 "aarch64_reg_or_zero" "rZ")]
	     SVE_WHILE)
	   (const_int 1)]
	  UNSPEC_PTEST))
   (clobber (match_scratch:PRED_ALL 0 "=Upa"))]
  "TARGET_SVE"
  "while<cmp_op>\t%0.<PRED_ALL:Vetype>, %<w>1, %<w>2"
  ;; Force the compiler to drop the unused predicate operand, so that we
  ;; don't have an unnecessary PTRUE.
  "&& (!CONSTANT_P (operands[3]) || !CONSTANT_P (operands[4]))"
  {
    operands[3] = CONSTM1_RTX (VNx16BImode);
    operands[4] = CONSTM1_RTX (<PRED_ALL:MODE>mode);
  }
)

;; Unary BRKs (BRKA and BRKB).  Note that unlike most other instructions
;; that have both merging and zeroing forms, these don't operate
;; elementwise and so don't fit the IFN_COND model.
(define_insn "@aarch64_brk<brk_op>"
  [(set (match_operand:VNx16BI 0 "register_operand" "=Upa, Upa")
	(unspec:VNx16BI
	  [(match_operand:VNx16BI 1 "register_operand" "Upa, Upa")
	   (match_operand:VNx16BI 2 "register_operand" "Upa, Upa")
	   (match_operand:VNx16BI 3 "aarch64_simd_reg_or_zero" "Dz, 0")]
	  SVE_BRK_UNARY))]
  "TARGET_SVE"
  "@
   brk<brk_op>\t%0.b, %1/z, %2.b
   brk<brk_op>\t%0.b, %1/m, %2.b"
)

;; Same, but also producing a flags result.
(define_insn "*aarch64_brk<brk_op>_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa, Upa")
	   (match_dup 1)
	   (unspec:VNx16BI
	     [(match_dup 1)
	      (match_operand:VNx16BI 2 "register_operand" "Upa, Upa")
	      (match_operand:VNx16BI 3 "aarch64_simd_reg_or_zero" "Dz, 0")]
	     SVE_BRK_UNARY)
	   (match_operand 4 "const_int_operand")]
	  UNSPEC_PTEST))
   (set (match_operand:VNx16BI 0 "register_operand" "=Upa, Upa")
	(unspec:VNx16BI
	  [(match_dup 1)
	   (match_dup 2)
	   (match_dup 3)]
	  SVE_BRK_UNARY))]
  "TARGET_SVE"
  "@
   brk<brk_op>s\t%0.b, %1/z, %2.b
   brk<brk_op>s\t%0.b, %1/m, %2.b"
)

;; Same, but with only the flags result being interesting.
(define_insn "*aarch64_brk<brk_op>_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa, Upa")
	   (match_dup 1)
	   (unspec:VNx16BI
	     [(match_dup 1)
	      (match_operand:VNx16BI 2 "register_operand" "Upa, Upa")
	      (match_operand:VNx16BI 3 "aarch64_simd_reg_or_zero" "Dz, 0")]
	     SVE_BRK_UNARY)
	   (match_operand 4 "const_int_operand")]
	  UNSPEC_PTEST))
   (clobber (match_scratch:VNx16BI 0 "=Upa, Upa"))]
  "TARGET_SVE"
  "@
   brk<brk_op>s\t%0.b, %1/z, %2.b
   brk<brk_op>s\t%0.b, %1/m, %2.b"
)

;; Binary BRKs (BRKN, BRKPA, BRKPB).
(define_insn "@aarch64_brk<brk_op>"
  [(set (match_operand:VNx16BI 0 "register_operand" "=Upa")
	(unspec:VNx16BI
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_operand:VNx16BI 2 "register_operand" "Upa")
	   (match_operand:VNx16BI 3 "register_operand" "<brk_reg_con>")]
	  SVE_BRK_BINARY))]
  "TARGET_SVE"
  "brk<brk_op>\t%0.b, %1/z, %2.b, %<brk_reg_opno>.b"
)

;; Same, but also producing a flags result.
(define_insn "*aarch64_brk<brk_op>_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_dup 1)
	   (unspec:VNx16BI
	     [(match_dup 1)
	      (match_operand:VNx16BI 2 "register_operand" "Upa")
	      (match_operand:VNx16BI 3 "register_operand" "<brk_reg_con>")]
	     SVE_BRK_BINARY)
	   (match_operand 4 "const_int_operand")]
	  UNSPEC_PTEST))
   (set (match_operand:VNx16BI 0 "register_operand" "=Upa")
	(unspec:VNx16BI
	  [(match_dup 1)
	   (match_dup 2)
	   (match_dup 3)]
	  SVE_BRK_BINARY))]
  "TARGET_SVE"
  "brk<brk_op>s\t%0.b, %1/z, %2.b, %<brk_reg_opno>.b"
)

;; Same, but with only the flags result being interesting.
(define_insn "*aarch64_brk<brk_op>_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_dup 1)
	   (unspec:VNx16BI
	     [(match_dup 1)
	      (match_operand:VNx16BI 2 "register_operand" "Upa")
	      (match_operand:VNx16BI 3 "register_operand" "<brk_reg_con>")]
	     SVE_BRK_BINARY)
	   (match_operand 4 "const_int_operand")]
	  UNSPEC_PTEST))
   (clobber (match_scratch:VNx16BI 0 "=Upa"))]
  "TARGET_SVE"
  "brk<brk_op>s\t%0.b, %1/z, %2.b, %<brk_reg_opno>.b"
)

;; Integer comparisons predicated with a PTRUE.
(define_insn "*cmp<cmp_op><mode>"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(unspec:<VPRED>
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (SVE_INT_CMP:<VPRED>
	     (match_operand:SVE_I 2 "register_operand" "w, w")
	     (match_operand:SVE_I 3 "aarch64_sve_cmp_<sve_imm_con>_operand" "<sve_imm_con>, w"))]
	  UNSPEC_MERGE_PTRUE))
   (clobber (reg:CC_NZC CC_REGNUM))]
  "TARGET_SVE"
  "@
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #%3
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; Integer comparisons predicated with a PTRUE in which only the flags result
;; is interesting.
(define_insn "*cmp<cmp_op><mode>_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upl, Upl")
	   (match_operand 4)
	   (unspec:<VPRED>
	     [(match_dup 4)
	      (SVE_INT_CMP:<VPRED>
		(match_operand:SVE_I 2 "register_operand" "w, w")
		(match_operand:SVE_I 3 "aarch64_sve_cmp_<sve_imm_con>_operand" "<sve_imm_con>, w"))]
	     UNSPEC_MERGE_PTRUE)
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (clobber (match_scratch:<VPRED> 0 "=Upa, Upa"))]
  "TARGET_SVE"
  "@
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #%3
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; Integer comparisons predicated with a PTRUE in which both the flag and
;; predicate results are interesting.
(define_insn "*cmp<cmp_op><mode>_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upl, Upl")
	   (match_operand 4)
	   (unspec:<VPRED>
	     [(match_dup 4)
	      (SVE_INT_CMP:<VPRED>
		(match_operand:SVE_I 2 "register_operand" "w, w")
		(match_operand:SVE_I 3 "aarch64_sve_cmp_<sve_imm_con>_operand" "<sve_imm_con>, w"))]
	     UNSPEC_MERGE_PTRUE)
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(unspec:<VPRED>
	  [(match_dup 4)
	   (SVE_INT_CMP:<VPRED>
	     (match_dup 2)
	     (match_dup 3))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #%3
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; Predicated integer comparisons, formed by combining a PTRUE-predicated
;; comparison with an AND.  Split the instruction into its preferred form
;; (below) at the earliest opportunity, in order to get rid of the
;; redundant operand 1.
(define_insn_and_split "*pred_cmp<cmp_op><mode>_combine"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
       (and:<VPRED>
	 (unspec:<VPRED>
	   [(match_operand:<VPRED> 1)
	    (SVE_INT_CMP:<VPRED>
	      (match_operand:SVE_I 2 "register_operand" "w, w")
	      (match_operand:SVE_I 3 "aarch64_sve_cmp_<sve_imm_con>_operand" "<sve_imm_con>, w"))]
	   UNSPEC_MERGE_PTRUE)
	 (match_operand:<VPRED> 4 "register_operand" "Upl, Upl")))
   (clobber (reg:CC_NZC CC_REGNUM))]
  "TARGET_SVE"
  "#"
  "&& 1"
  [(parallel
     [(set (match_dup 0)
	  (and:<VPRED>
	    (SVE_INT_CMP:<VPRED>
	      (match_dup 2)
	      (match_dup 3))
	    (match_dup 4)))
      (clobber (reg:CC_NZC CC_REGNUM))])]
)

;; Predicated integer comparisons.
(define_insn "@aarch64_pred_cmp<cmp_op><mode>"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(and:<VPRED>
	  (SVE_INT_CMP:<VPRED>
	    (match_operand:SVE_I 2 "register_operand" "w, w")
	    (match_operand:SVE_I 3 "aarch64_sve_cmp_<sve_imm_con>_operand" "<sve_imm_con>, w"))
	  (match_operand:<VPRED> 1 "register_operand" "Upl, Upl")))
   (clobber (reg:CC_NZC CC_REGNUM))]
  "TARGET_SVE"
  "@
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #%3
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; Predicated integer comparisons in which only the flags result
;; is interesting.
(define_insn "*aarch64_pred_cmp<cmp_op><mode>_test"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (match_operand 4)
	   (and:<VPRED>
	     (SVE_INT_CMP:<VPRED>
	       (match_operand:SVE_I 2 "register_operand" "w, w")
	       (match_operand:SVE_I 3 "aarch64_sve_cmp_<sve_imm_con>_operand" "<sve_imm_con>, w"))
	     (match_dup 4))
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (clobber (match_scratch:<VPRED> 0 "=Upa, Upa"))]
  "TARGET_SVE"
  "@
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #%3
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; Predicated integer comparisons in which both the flag and
;; predicate results are interesting.
(define_insn "*aarch64_pred_cmp<cmp_op><mode>_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (match_operand 4)
	   (and:<VPRED>
	     (SVE_INT_CMP:<VPRED>
	       (match_operand:SVE_I 2 "register_operand" "w, w")
	       (match_operand:SVE_I 3 "aarch64_sve_cmp_<sve_imm_con>_operand" "<sve_imm_con>, w"))
	     (match_dup 4))
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(and:<VPRED>
	  (SVE_INT_CMP:<VPRED>
	    (match_dup 2)
	    (match_dup 3))
	  (match_dup 4)))]
  "TARGET_SVE"
  "@
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #%3
   cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; Predicated integer wide comparisons.
(define_insn "@aarch64_pred_cmp<cmp_op><mode>_wide"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa")
	(and:<VPRED>
	  (unspec:<VPRED>
	    [(match_operand:SVE_BHSI 2 "register_operand" "w")
	     (match_operand:VNx2DI 3 "register_operand" "w")]
	    SVE_COND_INT_CMP)
	  (match_operand:<VPRED> 1 "register_operand" "Upl")))
   (clobber (reg:CC_NZC CC_REGNUM))]
  "TARGET_SVE"
  "cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.d"
)

;; Predicated integer wide comparisons in which only the flags result
;; is interesting.
(define_insn "*aarch64_pred_cmp<cmp_op><mode>_wide_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upl")
	   (match_operand 4)
	   (and:<VPRED>
	     (unspec:<VPRED>
	       [(match_operand:SVE_BHSI 2 "register_operand" "w")
		(match_operand:VNx2DI 3 "register_operand" "w")]
	       SVE_COND_INT_CMP)
	     (match_dup 4))
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (clobber (match_scratch:<VPRED> 0 "=Upa"))]
  "TARGET_SVE"
  "cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.d"
)

;; Predicated integer wide comparisons in which both the flag and
;; predicate results are interesting.
(define_insn "*aarch64_pred_cmp<cmp_op><mode>_wide_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upl")
	   (match_operand 4)
	   (and:<VPRED>
	     (unspec:<VPRED>
	       [(match_operand:SVE_BHSI 2 "register_operand" "w")
		(match_operand:VNx2DI 3 "register_operand" "w")]
	       SVE_COND_INT_CMP)
	     (match_dup 4))
	   (match_operand 5 "const_int_operand")]
	  UNSPEC_PTEST))
   (set (match_operand:<VPRED> 0 "register_operand" "=Upa")
	(and:<VPRED>
	  (unspec:<VPRED>
	    [(match_dup 2)
	     (match_dup 3)]
	    SVE_COND_INT_CMP)
	  (match_dup 4)))]
  "TARGET_SVE"
  "cmp<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.d"
)

;; Floating-point comparisons predicated with a PTRUE.
(define_insn "*fcm<cmp_op><mode>"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(unspec:<VPRED>
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (SVE_FP_CMP:<VPRED>
	     (match_operand:SVE_F 2 "register_operand" "w, w")
	     (match_operand:SVE_F 3 "aarch64_simd_reg_or_zero" "Dz, w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "@
   fcm<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #0.0
   fcm<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

(define_insn "*fcmuo<mode>"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa")
	(unspec:<VPRED>
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (unordered:<VPRED>
	     (match_operand:SVE_F 2 "register_operand" "w")
	     (match_operand:SVE_F 3 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "fcmuo\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; Floating-point comparisons predicated on a PTRUE, with the results ANDed
;; with another predicate P.  This does not have the same trapping behavior
;; as predicating the comparison itself on P, but it's a legitimate fold,
;; since we can drop any potentially-trapping operations whose results
;; are not needed.
;;
;; Split the instruction into its preferred form (below) at the earliest
;; opportunity, in order to get rid of the redundant operand 1.
(define_insn_and_split "*fcm<cmp_op><mode>_and_combine"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(and:<VPRED>
	  (unspec:<VPRED>
	    [(match_operand:<VPRED> 1)
	     (SVE_FP_CMP
	       (match_operand:SVE_F 2 "register_operand" "w, w")
	       (match_operand:SVE_F 3 "aarch64_simd_reg_or_zero" "Dz, w"))]
	    UNSPEC_MERGE_PTRUE)
	  (match_operand:<VPRED> 4 "register_operand" "Upl, Upl")))]
  "TARGET_SVE"
  "#"
  "&& 1"
  [(set (match_dup 0)
	(and:<VPRED>
	  (SVE_FP_CMP:<VPRED>
	    (match_dup 2)
	    (match_dup 3))
	  (match_dup 4)))]
)

(define_insn_and_split "*fcmuo<mode>_and_combine"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa")
	(and:<VPRED>
	  (unspec:<VPRED>
	    [(match_operand:<VPRED> 1)
	     (unordered
	       (match_operand:SVE_F 2 "register_operand" "w")
	       (match_operand:SVE_F 3 "register_operand" "w"))]
	    UNSPEC_MERGE_PTRUE)
	  (match_operand:<VPRED> 4 "register_operand" "Upl")))]
  "TARGET_SVE"
  "#"
  "&& 1"
  [(set (match_dup 0)
	(and:<VPRED>
	  (unordered:<VPRED>
	    (match_dup 2)
	    (match_dup 3))
	  (match_dup 4)))]
)

;; Unpredicated floating-point comparisons, with the results ANDed
;; with another predicate.  This is a valid fold for the same reasons
;; as above.
(define_insn "*fcm<cmp_op><mode>_and"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(and:<VPRED>
	  (SVE_FP_CMP:<VPRED>
	    (match_operand:SVE_F 2 "register_operand" "w, w")
	    (match_operand:SVE_F 3 "aarch64_simd_reg_or_zero" "Dz, w"))
	  (match_operand:<VPRED> 1 "register_operand" "Upl, Upl")))]
  "TARGET_SVE"
  "@
   fcm<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #0.0
   fcm<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

(define_insn "@aarch64_fcmuo<mode>_and"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa")
	(and:<VPRED>
	  (unordered:<VPRED>
	    (match_operand:SVE_F 2 "register_operand" "w")
	    (match_operand:SVE_F 3 "register_operand" "w"))
	  (match_operand:<VPRED> 1 "register_operand" "Upl")))]
  "TARGET_SVE"
  "fcmuo\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; Predicated floating-point comparisons.
(define_insn "@aarch64_pred_fcm<cmp_op><mode>"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa, Upa")
	(unspec:<VPRED>
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (match_operand:SVE_F 2 "register_operand" "w, w")
	   (match_operand:SVE_F 3 "aarch64_simd_reg_or_zero" "Dz, w")]
	  SVE_COND_FP_CMP))]
  "TARGET_SVE"
  "@
   fcm<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, #0.0
   fcm<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; Predicated floating-point absolute comparisons.
(define_insn "@aarch64_pred_fac<cmp_op><mode>"
  [(set (match_operand:<VPRED> 0 "register_operand" "=Upa")
	(unspec:<VPRED>
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (abs:SVE_F (match_operand:SVE_F 2 "register_operand" "w"))
	   (abs:SVE_F (match_operand:SVE_F 3 "register_operand" "w"))]
	  SVE_COND_FP_ABS_CMP))]
  "TARGET_SVE"
  "fac<cmp_op>\t%0.<Vetype>, %1/z, %2.<Vetype>, %3.<Vetype>"
)

;; vcond_mask operand order: true, false, mask
;; UNSPEC_SEL operand order: mask, true, false (as for VEC_COND_EXPR)
;; SEL operand order:        mask, true, false
(define_expand "@vcond_mask_<mode><vpred>"
  [(set (match_operand:SVE_ALL 0 "register_operand")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 3 "register_operand")
	   (match_operand:SVE_ALL 1 "aarch64_sve_dup_reg_or_imm")
	   (match_operand:SVE_ALL 2 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  {
    if (register_operand (operands[1], <MODE>mode))
      operands[2] = force_reg (<MODE>mode, operands[2]);
  }
)

(define_insn "*vcond_mask_<mode><vpred>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w, w, w, w, ?w, ?&w, ?&w")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 3 "register_operand" "Upa, Upa, Upa, Upa, Upl, Upl, Upl")
	   (match_operand:SVE_ALL 1 "aarch64_sve_dup_reg_or_imm" "w, vss, vss, Ufc, Ufc, vss, Ufc")
	   (match_operand:SVE_ALL 2 "aarch64_simd_reg_or_zero" "w, 0, Dz, 0, Dz, w, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE
   && (!register_operand (operands[1], <MODE>mode)
       || register_operand (operands[2], <MODE>mode))"
  "@
   sel\t%0.<Vetype>, %3, %1.<Vetype>, %2.<Vetype>
   mov\t%0.<Vetype>, %3/m, #%I1
   mov\t%0.<Vetype>, %3/z, #%I1
   fmov\t%0.<Vetype>, %3/m, #%1
   movprfx\t%0.<Vetype>, %3/z, %0.<Vetype>\;fmov\t%0.<Vetype>, %3/m, #%1
   movprfx\t%0, %2\;mov\t%0.<Vetype>, %3/m, #%I1
   movprfx\t%0, %2\;fmov\t%0.<Vetype>, %3/m, #%1"
  [(set_attr "movprfx" "*,*,*,*,yes,yes,yes")]
)

(define_insn "@aarch64_sel_dup<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w, w, ?w, ?&w, ?&w, ?&w")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 3 "register_operand" "Upa, Upa, Upl, Upl, Upl, Upl")
	   (vec_duplicate:SVE_ALL
	     (match_operand:<VEL> 1 "register_operand" "r, w, r, w, r, w"))
	   (match_operand:SVE_ALL 2 "aarch64_simd_reg_or_zero" "0, 0, Dz, Dz, w, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   mov\t%0.<Vetype>, %3/m, %<vwcore>1
   mov\t%0.<Vetype>, %3/m, %<Vetype>1
   movprfx\t%0.<Vetype>, %3/z, %0.<Vetype>\;mov\t%0.<Vetype>, %3/m, %<vwcore>1
   movprfx\t%0.<Vetype>, %3/z, %0.<Vetype>\;mov\t%0.<Vetype>, %3/m, %<Vetype>1
   movprfx\t%0, %2\;mov\t%0.<Vetype>, %3/m, %<vwcore>1
   movprfx\t%0, %2\;mov\t%0.<Vetype>, %3/m, %<Vetype>1"
  [(set_attr "movprfx" "*,*,yes,yes,yes,yes")]
)

;; Selects between a duplicated immediate and zero.
(define_insn "aarch64_sve_dup<mode>_const"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (match_operand:SVE_I 2 "aarch64_sve_dup_immediate")
	   (match_operand:SVE_I 3 "aarch64_simd_imm_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "mov\t%0.<Vetype>, %1/z, #%2"
)

;; Predicate selection.
(define_insn "@vcond_mask_<mode><mode>"
  [(set (match_operand:PRED_ALL 0 "register_operand" "=Upa")
	(ior:PRED_ALL
	  (and:PRED_ALL
	    (match_operand:PRED_ALL 3 "register_operand" "Upa")
	    (match_operand:PRED_ALL 1 "register_operand" "Upa"))
	  (and:PRED_ALL
	    (not (match_dup 3))
	    (match_operand:PRED_ALL 2 "register_operand" "Upa"))))]
  "TARGET_SVE"
  "sel\t%0.b, %3, %1.b, %2.b"
)

;; Integer (signed) vcond.  Don't enforce an immediate range here, since it
;; depends on the comparison; leave it to aarch64_expand_sve_vcond instead.
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

;; Integer vcondu.  Don't enforce an immediate range here, since it
;; depends on the comparison; leave it to aarch64_expand_sve_vcond instead.
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

;; Floating-point vcond.  All comparisons except FCMUO allow a zero
;; operand; aarch64_expand_sve_vcond handles the case of an FCMUO
;; with zero.
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

;; Signed integer comparisons.  Don't enforce an immediate range here, since
;; it depends on the comparison; leave it to aarch64_expand_sve_vec_cmp_int
;; instead.
(define_expand "vec_cmp<mode><vpred>"
  [(parallel
    [(set (match_operand:<VPRED> 0 "register_operand")
	  (match_operator:<VPRED> 1 "comparison_operator"
	    [(match_operand:SVE_I 2 "register_operand")
	     (match_operand:SVE_I 3 "nonmemory_operand")]))
     (clobber (reg:CC_NZC CC_REGNUM))])]
  "TARGET_SVE"
  {
    aarch64_expand_sve_vec_cmp_int (operands[0], GET_CODE (operands[1]),
				    operands[2], operands[3]);
    DONE;
  }
)

;; Unsigned integer comparisons.  Don't enforce an immediate range here, since
;; it depends on the comparison; leave it to aarch64_expand_sve_vec_cmp_int
;; instead.
(define_expand "vec_cmpu<mode><vpred>"
  [(parallel
    [(set (match_operand:<VPRED> 0 "register_operand")
	  (match_operator:<VPRED> 1 "comparison_operator"
	    [(match_operand:SVE_I 2 "register_operand")
	     (match_operand:SVE_I 3 "nonmemory_operand")]))
     (clobber (reg:CC_NZC CC_REGNUM))])]
  "TARGET_SVE"
  {
    aarch64_expand_sve_vec_cmp_int (operands[0], GET_CODE (operands[1]),
				    operands[2], operands[3]);
    DONE;
  }
)

;; Floating-point comparisons.  All comparisons except FCMUO allow a zero
;; operand; aarch64_expand_sve_vec_cmp_float handles the case of an FCMUO
;; with zero.
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

;; Branch based on predicate equality or inequality.
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
    rtx ptrue = force_reg (VNx16BImode, aarch64_ptrue_all (<data_bytes>));
    rtx cast_ptrue = gen_lowpart (<MODE>mode, ptrue);
    rtx pred;
    if (operands[2] == CONST0_RTX (<MODE>mode))
      pred = operands[1];
    else
      {
	pred = gen_reg_rtx (<MODE>mode);
	emit_insn (gen_aarch64_pred_xor<mode>_z (pred, cast_ptrue, operands[1],
						 operands[2]));
      }
    emit_insn (gen_aarch64_ptest<mode> (ptrue, cast_ptrue, pred, const1_rtx));
    operands[1] = gen_rtx_REG (CC_NZCmode, CC_REGNUM);
    operands[2] = const0_rtx;
  }
)

;; Unpredicated floating-point MIN/MAX.
(define_expand "<maxmin_uns><mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (const_int SVE_ALLOW_NEW_FAULTS)
	   (match_operand:SVE_F 1 "register_operand")
	   (match_operand:SVE_F 2 "aarch64_sve_float_maxmin_operand")]
	  SVE_COND_MAXMIN))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Unpredicated floating-point binary operations that have no immediate forms.
(define_expand "<optab><mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (const_int SVE_ALLOW_NEW_FAULTS)
	   (match_operand:SVE_F 1 "register_operand")
	   (match_operand:SVE_F 2 "register_operand")]
	  SVE_COND_FP_BINARY_REG))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated floating-point binary operations that have no immediate forms.
(define_insn "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	   (match_operand:SI 4 "const_int_operand" "i, i, i")
	   (match_operand:SVE_F 2 "register_operand" "0, w, w")
	   (match_operand:SVE_F 3 "register_operand" "w, 0, w")]
	  SVE_COND_FP_BINARY_REG_REORDER))]
  "TARGET_SVE"
  "@
   <sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   <sve_fp_op_rev>\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0, %2\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,*,yes")]
)

;; Predicated FCADD.
(define_insn "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (match_operand:SI 4 "const_int_operand" "i, i")
	   (match_operand:SVE_F 2 "register_operand" "0, w")
	   (match_operand:SVE_F 3 "register_operand" "w, w")]
	  SVE_COND_FP_BINARY_REG_NOREORDER))]
  "TARGET_SVE"
  "@
   <sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype><maybe_op_imm>
   movprfx\t%0, %2\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype><maybe_op_imm>"
  [(set_attr "movprfx" "*,yes")]
)

;; General splitter for conditional binary integer operations
;; that cannot use MOVPRFX and instead require a SEL.
(define_split
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand")
	   (match_operator:SVE_I 5 "aarch64_sve_any_binary_operator"
	     [(match_operand:SVE_I 2 "register_operand")
	      (match_operand:SVE_I 3 "register_operand")])
	   (match_operand:SVE_I 4 "register_operand")]
	  UNSPEC_SEL))]
  "TARGET_SVE
   && reload_completed
   && !rtx_equal_p (operands[0], operands[4])
   && !rtx_equal_p (operands[2], operands[4])
   && !rtx_equal_p (operands[3], operands[4])"
  [(set (match_dup 0)
	(unspec:SVE_I [(match_dup 1) (match_dup 2) (match_dup 4)] UNSPEC_SEL))
   (set (match_dup 0)
	(unspec:SVE_I
	  [(match_dup 1)
	   (match_op_dup 5 [(match_dup 0) (match_dup 3)])
	   (match_dup 0)]
	  UNSPEC_SEL))]
)

;; Predicated integer operations with select.
(define_expand "@cond_<optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand")
	   (SVE_INT_BINARY:SVE_I
	     (match_operand:SVE_I 2 "register_operand")
	     (match_operand:SVE_I 3 "<pred_rhs2_operand>"))
	   (match_operand:SVE_I 4 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
)

(define_insn "@aarch64_<su_optab><optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, ?&w, ?&w, w")
	(unspec:SVE_I
	  [(SBINQOPS:SVE_I
	     (match_operand:SVE_I 1 "register_operand" "0, 0, w, w, w")
	     (match_operand:SVE_I 2 "aarch64_sve_svqadd_operand" "vsQ, vsS, vsQ, vsS, w"))]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <binqops_op>\t%0.<Vetype>, %0.<Vetype>, #%D2
   <binqops_op_rev>\t%0.<Vetype>, %0.<Vetype>, #%N2
   movprfx\t%0, %1\;<binqops_op>\t%0.<Vetype>, %0.<Vetype>, #%D2
   movprfx\t%0, %1\;<binqops_op_rev>\t%0.<Vetype>, %0.<Vetype>, #%N2
   <binqops_op>\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
 [(set_attr "movprfx" "*,*,yes,yes,*")]
)

(define_insn "@aarch64_<su_optab><optab><mode>"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w, w")
	(unspec:SVE_I
	  [(USBINQOPS:SVE_I
	     (match_operand:SVE_I 1 "register_operand" "0, w, w")
	     (match_operand:SVE_I 2 "aarch64_sve_arith_operand" "vsa, vsa, w"))]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <binqops_op>\t%0.<Vetype>, %0.<Vetype>, #%D2
   movprfx\t%0, %1\;<binqops_op>\t%0.<Vetype>, %0.<Vetype>, #%D2
   <binqops_op>\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
 [(set_attr "movprfx" "*,yes,*")]
)

(define_expand "@cond_<optab><mode>"
  [(set (match_operand:SVE_SDI 0 "register_operand")
	(unspec:SVE_SDI
	  [(match_operand:<VPRED> 1 "register_operand")
	   (SVE_INT_BINARY_SD:SVE_SDI
	     (match_operand:SVE_SDI 2 "register_operand")
	     (match_operand:SVE_SDI 3 "register_operand"))
	   (match_operand:SVE_SDI 4 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
)

;; Predicated integer operations with select matching the first operand.
(define_insn "*cond_<optab><mode>_2"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (SVE_INT_BINARY_REG:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "0, w")
	     (match_operand:SVE_I 3 "register_operand" "w, w"))
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

(define_insn "*cond_and<mode>_2"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, w, ?&w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (and:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "0, 0, w, w")
	     (match_operand:SVE_I 3 "aarch64_sve_pred_and_operand" "vsx, w, vsx, w"))
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   uxt%e3\t%0.<Vetype>, %1/m, %0.<Vetype>
   and\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;uxt%e3\t%0.<Vetype>, %1/m, %2.<Vetype>
   movprfx\t%0, %2\;and\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,*,yes,yes")]
)

(define_insn "*cond_<optab><mode>_2"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w, ?&w")
	(unspec:SVE_SDI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (SVE_INT_BINARY_SD:SVE_SDI
	     (match_operand:SVE_SDI 2 "register_operand" "0, w")
	     (match_operand:SVE_SDI 3 "register_operand" "w, w"))
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated integer operations with select matching the second operand.
(define_insn "*cond_<optab><mode>_3"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (SVE_INT_BINARY:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "w, w")
	     (match_operand:SVE_I 3 "register_operand" "0, w"))
	   (match_dup 3)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_int_op_rev>\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0, %3\;<sve_int_op_rev>\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

(define_insn "*cond_<optab><mode>_3"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w, ?&w")
	(unspec:SVE_SDI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (SVE_INT_BINARY_SD:SVE_SDI
	     (match_operand:SVE_SDI 2 "register_operand" "w, w")
	     (match_operand:SVE_SDI 3 "register_operand" "0, w"))
	   (match_dup 3)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_int_op_rev>\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0, %3\;<sve_int_op_rev>\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated integer operations with select matching zero.
(define_insn "*cond_<optab><mode>_any"
  [(set (match_operand:SVE_I 0 "register_operand" "=&w, &w, &w, &w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl")
	   (SVE_INT_BINARY_REG:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "0, w, w, w, w")
	     (match_operand:SVE_I 3 "register_operand" "w, 0, w, w, w"))
	   (match_operand:SVE_I 4 "aarch64_simd_reg_or_zero" "Dz, Dz, Dz, 0, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_int_op_rev>\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   #"
  [(set_attr "movprfx" "yes")]
)

(define_insn "*cond_and<mode>_any"
  [(set (match_operand:SVE_I 0 "register_operand" "=&w, &w, &w, w, &w, &w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl, Upl")
	   (and:SVE_I
	     (match_operand:SVE_I 2 "register_operand" "%0, w, w, w, w, w, w")
	     (match_operand:SVE_I 3 "aarch64_sve_pred_and_operand" "w, vsx, w, vsx, w, vsx, w"))
	   (match_operand:SVE_I 4 "aarch64_simd_reg_or_zero" "Dz, Dz, Dz, 0, 0, w, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;and\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;uxt%e3\t%0.<Vetype>, %1/m, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;and\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   uxt%e3\t%0.<Vetype>, %1/m, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;and\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %4\;uxt%e3\t%0.<Vetype>, %1/m, %2.<Vetype>
   #"
  [(set_attr "movprfx" "yes,yes,yes,*,yes,yes,yes")]
)

(define_insn "*cond_<optab><mode>_any"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=&w, &w, &w, &w, &w")
	(unspec:SVE_SDI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl")
	   (SVE_INT_BINARY_SD:SVE_SDI
	     (match_operand:SVE_SDI 2 "register_operand" "0, w, w, w, w")
	     (match_operand:SVE_SDI 3 "register_operand" "w, 0, w, w, w"))
	   (match_operand:SVE_SDI 4 "aarch64_simd_reg_or_zero" "Dz, Dz, Dz, 0, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_int_op_rev>\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;<sve_int_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   #"
  [(set_attr "movprfx" "yes")]
)

;; Predicated ASRD.
(define_expand "@cond_asrd<mode>"
  [(set (match_operand:SVE_I 0 "register_operand")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand")
	   (unspec:SVE_I
	     [(match_operand:SVE_I 2 "register_operand")
	      (match_operand:SVE_I 3 "aarch64_simd_rshift_imm")]
	     UNSPEC_ASRD)
	   (match_operand:SVE_I 4 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
)

;; Predicated ASRD with select matching the first input.
(define_insn "*cond_asrd<mode>_2"
  [(set (match_operand:SVE_I 0 "register_operand" "=w, ?&w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_I
	     [(match_operand:SVE_I 2 "register_operand" "0, w")
	      (match_operand:SVE_I 3 "aarch64_simd_rshift_imm")]
	     UNSPEC_ASRD)
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   asrd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0, %2\;asrd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3"
  [(set_attr "movprfx" "*,yes")])

;; Predicated ASRD with select matching zero.
(define_insn "*cond_asrd<mode>_z"
  [(set (match_operand:SVE_I 0 "register_operand" "=w")
	(unspec:SVE_I
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (unspec:SVE_I
	     [(match_operand:SVE_I 2 "register_operand" "w")
	      (match_operand:SVE_I 3 "aarch64_simd_rshift_imm")]
	     UNSPEC_ASRD)
	   (match_operand:SVE_I 4 "aarch64_simd_imm_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;asrd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3"
  [(set_attr "movprfx" "yes")])

;; Set operand 0 to the last active element in operand 3, or to tied
;; operand 1 if no elements are active.
(define_insn "fold_extract_last_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=r, w")
	(unspec:<VEL>
	  [(match_operand:<VEL> 1 "register_operand" "0, 0")
	   (match_operand:<VPRED> 2 "register_operand" "Upl, Upl")
	   (match_operand:SVE_ALL 3 "register_operand" "w, w")]
	  UNSPEC_CLASTB))]
  "TARGET_SVE"
  "@
   clastb\t%<vwcore>0, %2, %<vwcore>0, %3.<Vetype>
   clastb\t%<vw>0, %2, %<vw>0, %3.<Vetype>"
)

;; Unpredicated integer add reduction.
(define_expand "reduc_plus_scal_<mode>"
  [(match_operand:<VEL> 0 "register_operand")
   (match_operand:SVE_I 1 "register_operand")]
  "TARGET_SVE"
  {
    rtx pred = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
    rtx tmp = <VEL>mode == DImode ? operands[0] : gen_reg_rtx (DImode);
    emit_insn (gen_aarch64_pred_reduc_uadd_<mode> (tmp, pred, operands[1]));
    if (tmp != operands[0])
      emit_move_insn (operands[0], gen_lowpart (<VEL>mode, tmp));
    DONE;
  }
)

;; Predicated integer add reduction.  The result is always 64-bits.
(define_insn "@aarch64_pred_reduc_<optab>_<mode>"
  [(set (match_operand:DI 0 "register_operand" "=w")
	(unspec:DI [(match_operand:<VPRED> 1 "register_operand" "Upl")
		    (match_operand:SVE_I 2 "register_operand" "w")]
		   SVE_INT_ADDV))]
  "TARGET_SVE && <max_elem_bits> >= <elem_bits>"
  "<su>addv\t%d0, %1, %2.<Vetype>"
)

;; Unpredicated integer MIN/MAX reduction.
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

;; Predicated integer MIN/MAX reduction.
(define_insn "@aarch64_pred_reduc_<maxmin_uns>_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(unspec:<VEL> [(match_operand:<VPRED> 1 "register_operand" "Upl")
		       (match_operand:SVE_I 2 "register_operand" "w")]
		      MAXMINV))]
  "TARGET_SVE"
  "<maxmin_uns_op>v\t%<Vetype>0, %1, %2.<Vetype>"
)

;; Unpredicated floating-point ADD/MIN/MAX reduction.
(define_expand "reduc_<optab>_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand")
	(unspec:<VEL> [(match_dup 2)
		       (match_operand:SVE_F 1 "register_operand")]
		      SVE_FP_REDUC))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated floating-point ADD/MIN/MAX reduction.
(define_insn "@aarch64_pred_reduc_<optab>_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(unspec:<VEL> [(match_operand:<VPRED> 1 "register_operand" "Upl")
		       (match_operand:SVE_F 2 "register_operand" "w")]
		      SVE_FP_REDUC))]
  "TARGET_SVE"
  "<sve_fp_op>\t%<Vetype>0, %1, %2.<Vetype>"
)

;; Unpredicated bitwise reduction.
(define_expand "reduc_<optab>_scal_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand")
	(unspec:<VEL> [(match_dup 2)
		       (match_operand:SVE_I 1 "register_operand")]
		      BITWISEV))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated bitwise reduction.
(define_insn "@aarch64_pred_reduc_<optab>_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(unspec:<VEL> [(match_operand:<VPRED> 1 "register_operand" "Upl")
		       (match_operand:SVE_I 2 "register_operand" "w")]
		      BITWISEV))]
  "TARGET_SVE"
  "<bit_reduc_op>\t%<Vetype>0, %1, %2.<Vetype>"
)

;; Unpredicated in-order FP reductions.
(define_expand "fold_left_plus_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand")
	(unspec:<VEL> [(match_dup 3)
		       (match_operand:<VEL> 1 "register_operand")
		       (match_operand:SVE_F 2 "register_operand")]
		      UNSPEC_FADDA))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; In-order FP reductions predicated with PTRUE.
(define_insn "@aarch64_pred_fold_left_plus_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(unspec:<VEL> [(match_operand:<VPRED> 1 "register_operand" "Upl")
		       (match_operand:<VEL> 2 "register_operand" "0")
		       (match_operand:SVE_F 3 "register_operand" "w")]
		      UNSPEC_FADDA))]
  "TARGET_SVE"
  "fadda\t%<Vetype>0, %1, %<Vetype>0, %3.<Vetype>"
)

;; Predicated form of the above in-order reduction.
(define_insn "*pred_fold_left_plus_<mode>"
  [(set (match_operand:<VEL> 0 "register_operand" "=w")
	(unspec:<VEL>
	  [(match_operand:<VEL> 1 "register_operand" "0")
	   (unspec:SVE_F
	     [(match_operand:<VPRED> 2 "register_operand" "Upl")
	      (match_operand:SVE_F 3 "register_operand" "w")
	      (match_operand:SVE_F 4 "aarch64_simd_imm_zero")]
	     UNSPEC_SEL)]
	  UNSPEC_FADDA))]
  "TARGET_SVE"
  "fadda\t%<Vetype>0, %2, %<Vetype>0, %3.<Vetype>"
)

;; Unpredicated floating-point addition.
(define_expand "add<mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (const_int SVE_ALLOW_NEW_FAULTS)
	   (match_operand:SVE_F 1 "register_operand")
	   (match_operand:SVE_F 2 "aarch64_sve_float_arith_with_sub_operand")]
	  UNSPEC_COND_ADD))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated floating-point addition, values of inactive results don't matter.
(define_insn_and_split "@aarch64_pred_add<mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, w, w, ?&w, ?&w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl, Upl")
	   (match_operand:SI 4 "const_int_operand" "i, i, Ui1, Z, i, i, Z")
	   (match_operand:SVE_F 2 "register_operand" "%0, 0, w, 0, w, w, w")
	   (match_operand:SVE_F 3 "aarch64_sve_float_arith_with_sub_operand" "vsA, vsN, w, w, vsA, vsN, w")]
	  UNSPEC_COND_ADD))]
  "TARGET_SVE"
  "@
   fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, #%N3
   #
   fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0, %2\;fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, #%N3
   movprfx\t%0, %2\;fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  ; Split the unpredicated form after reload, so that we don't have
  ; the unnecessary PTRUE.
  "&& reload_completed
   && register_operand (operands[3], <MODE>mode)
   && INTVAL (operands[4]) == SVE_ALLOW_NEW_FAULTS"
  [(set (match_dup 0) (plus:SVE_F (match_dup 2) (match_dup 3)))]
  ""
  [(set_attr "movprfx" "*,*,*,*,yes,yes,yes")]
)

;; Unpredicated floating-point subtraction.
(define_expand "sub<mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (const_int SVE_ALLOW_NEW_FAULTS)
	   (match_operand:SVE_F 1 "aarch64_sve_float_arith_operand")
	   (match_operand:SVE_F 2 "register_operand")]
	  UNSPEC_COND_SUB))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated floating-point subtraction, values of inactive results
;; don't matter.
(define_insn_and_split "@aarch64_pred_sub<mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, w, w, ?&w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	   (match_operand:SI 4 "const_int_operand" "i, Ui1, Z, Z, i, Z")
	   (match_operand:SVE_F 2 "aarch64_sve_float_arith_operand" "vsA, w, 0, w, vsA, w")
	   (match_operand:SVE_F 3 "register_operand" "0, w, w, 0, w, w")]
	  UNSPEC_COND_SUB))]
  "TARGET_SVE"
  "@
   fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, #%2
   #
   fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0, %3\;fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, #%2
   movprfx\t%0, %2\;fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  ; Split the unpredicated form after reload, so that we don't have
  ; the unnecessary PTRUE.
  "&& reload_completed
   && register_operand (operands[2], <MODE>mode)
   && INTVAL (operands[4]) == SVE_ALLOW_NEW_FAULTS"
  [(set (match_dup 0) (minus:SVE_F (match_dup 2) (match_dup 3)))]
  ""
  [(set_attr "movprfx" "*,*,*,*,yes,yes")]
)

;; Unpredicated floating-point binary operations that can take a single-bit
;; immediate.
(define_expand "<optab><mode>3"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 3)
	   (const_int SVE_ALLOW_NEW_FAULTS)
	   (match_operand:SVE_F 1 "register_operand")
	   (match_operand:SVE_F 2 "aarch64_sve_float_<sve_imm_pred>_operand")]
	  SVE_COND_FP_BINARY_I1))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated floating-point multiplication.
(define_insn_and_split "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, w, ?&w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl")
	   (match_operand:SI 4 "const_int_operand" "i, Ui1, Z, i, Z")
	   (match_operand:SVE_F 2 "register_operand" "%0, w, 0, w, w")
	   (match_operand:SVE_F 3 "aarch64_sve_float_mul_operand" "vsM, w, w, vsM, w")]
	  SVE_COND_FP_MUL))]
  "TARGET_SVE"
  "@
   fmul\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   #
   fmul\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;fmul\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0, %2\;fmul\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  ; Split the unpredicated form after reload, so that we don't have
  ; the unnecessary PTRUE.
  "&& reload_completed
   && register_operand (operands[3], <MODE>mode)
   && INTVAL (operands[4]) == SVE_ALLOW_NEW_FAULTS"
  [(set (match_dup 0) (mult:SVE_F (match_dup 2) (match_dup 3)))]
  ""
  [(set_attr "movprfx" "*,*,*,yes,yes")]
)

;; Unpredicated multiplication by selected lanes.
(define_insn "@aarch64_mul_lane_<mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w")
	(mult:SVE_F
	  (unspec:SVE_F
	    [(match_operand:SVE_F 2 "register_operand" "w")
	     (match_operand:SI 3 "const_int_operand")]
	    UNSPEC_SVE_LANE_SELECT)
	  (match_operand:SVE_F 1 "register_operand" "w")))]
  "TARGET_SVE"
  "fmul\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>[%3]"
)

;; Predicated floating-point MAX/MIN.
(define_insn "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, ?&w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (match_operand:SI 4 "const_int_operand")
	   (match_operand:SVE_F 2 "register_operand" "%0, 0, w, w")
	   (match_operand:SVE_F 3 "aarch64_sve_float_maxmin_operand" "vsB, w, vsB, w")]
	  SVE_COND_MAXMIN))]
  "TARGET_SVE"
  "@
   <sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   <sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0, %2\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,*,yes,yes")]
)

;; Unpredicated floating-point binary operations (post-RA only).
;; These are generated by splitting a predicated instruction whose
;; predicate is unused.
(define_insn "*post_ra_<sve_fp_op><mode>3"
  [(set (match_operand:SVE_F 0 "register_operand" "=w")
	(SVE_UNPRED_FP_BINARY:SVE_F
	  (match_operand:SVE_F 1 "register_operand" "w")
	  (match_operand:SVE_F 2 "register_operand" "w")))]
  "TARGET_SVE && reload_completed"
  "<sve_fp_op>\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>")

;; Unredicated fma, fms, fnms and fnma.
;; Unpredicated fma (%0 = (%1 * %2) + %3).
;; Unpredicated fms (%0 = (%1 * %2) - %3).
;; Unpredicated fnms (%0 = (-%1 * %2) - %3).
;; Unpredicated fnma (%0 = (-%1 * %2) + %3).
(define_expand "<optab><mode>4"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 4)
	   (const_int SVE_ALLOW_NEW_FAULTS)
	   (match_operand:SVE_F 1 "register_operand")
	   (match_operand:SVE_F 2 "register_operand")
	   (match_operand:SVE_F 3 "register_operand")]
	  SVE_COND_FP_TERNARY_REORDER))]
  "TARGET_SVE"
  {
    operands[4] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated fma, fms, fnms and fnma.
(define_insn "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (match_operand:SI 5 "const_int_operand")
	   (match_operand:SVE_F 2 "register_operand" "0, w, w, w")
	   (match_operand:SVE_F 3 "register_operand" "w, 0, w, w")
	   (match_operand:SVE_F 4 "register_operand" "w, w, 0, w")]
	  SVE_COND_FP_TERNARY_REORDER))]
  "TARGET_SVE"
  "@
   <sve_fmad_op>\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   <sve_fmad_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %4.<Vetype>
   <sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0, %4\;<sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,*,*,yes")]
)

;; Predicated FCMLA.
(define_insn "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (match_operand:SI 5 "const_int_operand")
	   (match_operand:SVE_F 2 "register_operand" "w, w")
	   (match_operand:SVE_F 3 "register_operand" "w, w")
	   (match_operand:SVE_F 4 "register_operand" "0, w")]
	  SVE_COND_FP_TERNARY_NOREORDER))]
  "TARGET_SVE"
  "@
   <sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype><maybe_op_imm>
   movprfx\t%0, %4\;<sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype><maybe_op_imm>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated FMLA, FMLS and FCMLA by selected lanes.  It doesn't seem
;; worth using (fma ...) for FMLA and FMLS since target-independent code
;; won't understand the indexing.
(define_insn "@aarch64_<optab>_lane_<mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, ?&w")
	(unspec:SVE_F
	  [(match_operand:SVE_F 1 "register_operand" "w, w")
	   (unspec:SVE_F
	     [(match_operand:SVE_F 2 "register_operand" "w, w")
	      (match_operand:SI 3 "const_int_operand")]
	     UNSPEC_SVE_LANE_SELECT)
	   (match_operand:SVE_F 4 "register_operand" "0, w")]
	  SVE_FP_TERNARY_LANE))]
  "TARGET_SVE"
  "@
   <sve_fp_op>\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>[%3]<maybe_op_imm>
   movprfx\t%0, %4\;<sve_fp_op>\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>[%3]<maybe_op_imm>"
  [(set_attr "movprfx" "*,yes")]
)

;; Unpredicated FNEG, FABS and FSQRT.
(define_expand "<optab><mode>2"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_dup 2)
	   (const_int SVE_ALLOW_NEW_FAULTS)
	   (match_operand:SVE_F 1 "register_operand")]
	  SVE_COND_FP_UNARY))]
  "TARGET_SVE"
  {
    operands[2] = force_reg (<VPRED>mode, CONSTM1_RTX (<VPRED>mode));
  }
)

;; Predicated FNEG, FABS and FSQRT.
(define_insn "@aarch64_pred_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (match_operand:SI 3 "const_int_operand" "i")
	   (match_operand:SVE_F 2 "register_operand" "w")]
	  SVE_COND_FP_UNARY))]
  "TARGET_SVE"
  "<sve_fp_op>\t%0.<Vetype>, %1/m, %2.<Vetype>"
)

;; Predicated FNEG, FABS and FSQRT with select.
(define_insn "@cond_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, &w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_operand:SVE_F 2 "register_operand" "w, w, w")]
	     SVE_COND_FP_UNARY)
	   (match_operand:SVE_F 3 "aarch64_simd_reg_or_zero" "0, Dz, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_fp_op>\t%0.<Vetype>, %1/m, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %2.<Vetype>
   movprfx\t%0, %3\;<sve_fp_op>\t%0.<Vetype>, %1/m, %2.<Vetype>"
  [(set_attr "movprfx" "*,yes,yes")]
)

;; Unpredicated FRINTy.
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

;; FRINTy predicated with a PTRUE.
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

;; Unpredicated conversion of floats to integers of the same size (HF to HI,
;; SF to SI or DF to DI).
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

;; Conversion of SF to DI, SI or HI, predicated with a PTRUE.
(define_insn "*<fix_trunc_optab>v16hsf<mode>2"
  [(set (match_operand:SVE_HSDI 0 "register_operand" "=w")
	(unspec:SVE_HSDI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (FIXUORS:SVE_HSDI
	     (match_operand:VNx8HF 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "fcvtz<su>\t%0.<Vetype>, %1/m, %2.h"
)

;; Conversion of SF to DI or SI, predicated with a PTRUE.
(define_insn "*<fix_trunc_optab>vnx4sf<mode>2"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w")
	(unspec:SVE_SDI
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (FIXUORS:SVE_SDI
	     (match_operand:VNx4SF 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "fcvtz<su>\t%0.<Vetype>, %1/m, %2.s"
)

;; Conversion of DF to DI or SI, predicated with a PTRUE.
(define_insn "*<fix_trunc_optab>vnx2df<mode>2"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w")
	(unspec:SVE_SDI
	  [(match_operand:VNx2BI 1 "register_operand" "Upl")
	   (FIXUORS:SVE_SDI
	     (match_operand:VNx2DF 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "fcvtz<su>\t%0.<Vetype>, %1/m, %2.d"
)

;; Unpredicated conversion of integers to floats of the same size
;; (HI to HF, SI to SF or DI to DF).
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

;; Conversion of DI, SI or HI to the same number of HFs, predicated
;; with a PTRUE.
(define_insn "*<optab><mode>vnx8hf2"
  [(set (match_operand:VNx8HF 0 "register_operand" "=w")
	(unspec:VNx8HF
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (FLOATUORS:VNx8HF
	     (match_operand:SVE_HSDI 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<su_optab>cvtf\t%0.h, %1/m, %2.<Vetype>"
)

;; Conversion of DI or SI to the same number of SFs, predicated with a PTRUE.
(define_insn "*<optab><mode>vnx4sf2"
  [(set (match_operand:VNx4SF 0 "register_operand" "=w")
	(unspec:VNx4SF
	  [(match_operand:<VPRED> 1 "register_operand" "Upl")
	   (FLOATUORS:VNx4SF
	     (match_operand:SVE_SDI 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<su_optab>cvtf\t%0.s, %1/m, %2.<Vetype>"
)

;; Conversion of DI or SI to DF, predicated with a PTRUE.
(define_insn "aarch64_sve_<optab><mode>vnx2df2"
  [(set (match_operand:VNx2DF 0 "register_operand" "=w")
	(unspec:VNx2DF
	  [(match_operand:VNx2BI 1 "register_operand" "Upl")
	   (FLOATUORS:VNx2DF
	     (match_operand:SVE_SDI 2 "register_operand" "w"))]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "<su_optab>cvtf\t%0.d, %1/m, %2.<Vetype>"
)

;; Conversion of DFs to the same number of SFs, or SFs to the same number
;; of HFs.
(define_insn "*trunc<Vwide><mode>2"
  [(set (match_operand:SVE_HSF 0 "register_operand" "=w")
	(unspec:SVE_HSF
	  [(match_operand:<VWIDE_PRED> 1 "register_operand" "Upl")
	   (unspec:SVE_HSF
	     [(match_operand:<VWIDE> 2 "register_operand" "w")]
	     UNSPEC_FLOAT_CONVERT)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "fcvt\t%0.<Vetype>, %1/m, %2.<Vewtype>"
)

;; Conversion of SFs to the same number of DFs, or HFs to the same number
;; of SFs.
(define_insn "aarch64_sve_extend<mode><Vwide>2"
  [(set (match_operand:<VWIDE> 0 "register_operand" "=w")
	(unspec:<VWIDE>
	  [(match_operand:<VWIDE_PRED> 1 "register_operand" "Upl")
	   (unspec:<VWIDE>
	     [(match_operand:SVE_HSF 2 "register_operand" "w")]
	     UNSPEC_FLOAT_CONVERT)]
	  UNSPEC_MERGE_PTRUE))]
  "TARGET_SVE"
  "fcvt\t%0.<Vewtype>, %1/m, %2.<Vetype>"
)

;; Unpack the low or high half of a predicate, where "high" refers to
;; the low-numbered lanes for big-endian and the high-numbered lanes
;; for little-endian.
(define_expand "vec_unpack<su>_<perm_hilo>_<mode>"
  [(match_operand:<VWIDE> 0 "register_operand")
   (unspec:<VWIDE> [(match_operand:PRED_BHS 1 "register_operand")]
		   UNPACK)]
  "TARGET_SVE"
  {
    emit_insn ((<hi_lanes_optab>
		? gen_aarch64_sve_punpkhi_<PRED_BHS:mode>
		: gen_aarch64_sve_punpklo_<PRED_BHS:mode>)
	       (operands[0], operands[1]));
    DONE;
  }
)

;; PUNPKHI and PUNPKLO.
(define_insn "aarch64_sve_punpk<perm_hilo>_<mode>"
  [(set (match_operand:<VWIDE> 0 "register_operand" "=Upa")
	(unspec:<VWIDE> [(match_operand:PRED_BHS 1 "register_operand" "Upa")]
			UNPACK_UNSIGNED))]
  "TARGET_SVE"
  "punpk<perm_hilo>\t%0.h, %1.b"
)

;; Unpack the low or high half of a vector, where "high" refers to
;; the low-numbered lanes for big-endian and the high-numbered lanes
;; for little-endian.
(define_expand "vec_unpack<su>_<perm_hilo>_<SVE_BHSI:mode>"
  [(match_operand:<VWIDE> 0 "register_operand")
   (unspec:<VWIDE> [(match_operand:SVE_BHSI 1 "register_operand")] UNPACK)]
  "TARGET_SVE"
  {
    emit_insn ((<hi_lanes_optab>
		? gen_aarch64_sve_<su>unpkhi_<SVE_BHSI:mode>
		: gen_aarch64_sve_<su>unpklo_<SVE_BHSI:mode>)
	       (operands[0], operands[1]));
    DONE;
  }
)

;; SUNPKHI, UUNPKHI, SUNPKLO and UUNPKLO.
(define_insn "aarch64_sve_<su>unpk<perm_hilo>_<SVE_BHSI:mode>"
  [(set (match_operand:<VWIDE> 0 "register_operand" "=w")
	(unspec:<VWIDE> [(match_operand:SVE_BHSI 1 "register_operand" "w")]
			UNPACK))]
  "TARGET_SVE"
  "<su>unpk<perm_hilo>\t%0.<Vewtype>, %1.<Vetype>"
)

;; Unpack one half of a VNx4SF to VNx2DF, or one half of a VNx8HF to VNx4SF.
;; First unpack the source without conversion, then float-convert the
;; unpacked source.
(define_expand "vec_unpacks_<perm_hilo>_<mode>"
  [(match_operand:<VWIDE> 0 "register_operand")
   (unspec:SVE_HSF [(match_operand:SVE_HSF 1 "register_operand")]
		   UNPACK_UNSIGNED)]
  "TARGET_SVE"
  {
    /* Use ZIP to do the unpack, since we don't care about the upper halves
       and since it has the nice property of not needing any subregs.
       If using UUNPK* turns out to be preferable, we could model it as
       a ZIP whose first operand is zero.  */
    rtx temp = gen_reg_rtx (<MODE>mode);
    emit_insn ((<hi_lanes_optab>
		? gen_aarch64_sve_zip2<mode>
		: gen_aarch64_sve_zip1<mode>)
		(temp, operands[1], operands[1]));
    rtx ptrue = force_reg (<VWIDE_PRED>mode, CONSTM1_RTX (<VWIDE_PRED>mode));
    emit_insn (gen_aarch64_sve_extend<mode><Vwide>2 (operands[0],
						     ptrue, temp));
    DONE;
  }
)

;; Unpack one half of a VNx4SI to VNx2DF.  First unpack from VNx4SI
;; to VNx2DI, reinterpret the VNx2DI as a VNx4SI, then convert the
;; unpacked VNx4SI to VNx2DF.
(define_expand "vec_unpack<su_optab>_float_<perm_hilo>_vnx4si"
  [(match_operand:VNx2DF 0 "register_operand")
   (FLOATUORS:VNx2DF
     (unspec:VNx2DI [(match_operand:VNx4SI 1 "register_operand")]
		    UNPACK_UNSIGNED))]
  "TARGET_SVE"
  {
    /* Use ZIP to do the unpack, since we don't care about the upper halves
       and since it has the nice property of not needing any subregs.
       If using UUNPK* turns out to be preferable, we could model it as
       a ZIP whose first operand is zero.  */
    rtx temp = gen_reg_rtx (VNx4SImode);
    emit_insn ((<hi_lanes_optab>
		? gen_aarch64_sve_zip2vnx4si
		: gen_aarch64_sve_zip1vnx4si)
	       (temp, operands[1], operands[1]));
    rtx ptrue = force_reg (VNx2BImode, CONSTM1_RTX (VNx2BImode));
    emit_insn (gen_aarch64_sve_<FLOATUORS:optab>vnx4sivnx2df2 (operands[0],
							       ptrue, temp));
    DONE;
  }
)

;; Predicate pack.  Use UZP1 on the narrower type, which discards
;; the high part of each wide element.
(define_insn "vec_pack_trunc_<Vwide>"
  [(set (match_operand:PRED_BHS 0 "register_operand" "=Upa")
	(unspec:PRED_BHS
	  [(match_operand:<VWIDE> 1 "register_operand" "Upa")
	   (match_operand:<VWIDE> 2 "register_operand" "Upa")]
	  UNSPEC_PACK))]
  "TARGET_SVE"
  "uzp1\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
)

;; Integer pack.  Use UZP1 on the narrower type, which discards
;; the high part of each wide element.
(define_insn "vec_pack_trunc_<Vwide>"
  [(set (match_operand:SVE_BHSI 0 "register_operand" "=w")
	(unspec:SVE_BHSI
	  [(match_operand:<VWIDE> 1 "register_operand" "w")
	   (match_operand:<VWIDE> 2 "register_operand" "w")]
	  UNSPEC_PACK))]
  "TARGET_SVE"
  "uzp1\t%0.<Vetype>, %1.<Vetype>, %2.<Vetype>"
)

;; Convert two vectors of DF to SF, or two vectors of SF to HF, and pack
;; the results into a single vector.
(define_expand "vec_pack_trunc_<Vwide>"
  [(set (match_dup 4)
	(unspec:SVE_HSF
	  [(match_dup 3)
	   (unspec:SVE_HSF [(match_operand:<VWIDE> 1 "register_operand")]
			   UNSPEC_FLOAT_CONVERT)]
	  UNSPEC_MERGE_PTRUE))
   (set (match_dup 5)
	(unspec:SVE_HSF
	  [(match_dup 3)
	   (unspec:SVE_HSF [(match_operand:<VWIDE> 2 "register_operand")]
			   UNSPEC_FLOAT_CONVERT)]
	  UNSPEC_MERGE_PTRUE))
   (set (match_operand:SVE_HSF 0 "register_operand")
	(unspec:SVE_HSF [(match_dup 4) (match_dup 5)] UNSPEC_UZP1))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (<VWIDE_PRED>mode, CONSTM1_RTX (<VWIDE_PRED>mode));
    operands[4] = gen_reg_rtx (<MODE>mode);
    operands[5] = gen_reg_rtx (<MODE>mode);
  }
)

;; Convert two vectors of DF to SI and pack the results into a single vector.
(define_expand "vec_pack_<su>fix_trunc_vnx2df"
  [(set (match_dup 4)
	(unspec:VNx4SI
	  [(match_dup 3)
	   (FIXUORS:VNx4SI (match_operand:VNx2DF 1 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))
   (set (match_dup 5)
	(unspec:VNx4SI
	  [(match_dup 3)
	   (FIXUORS:VNx4SI (match_operand:VNx2DF 2 "register_operand"))]
	  UNSPEC_MERGE_PTRUE))
   (set (match_operand:VNx4SI 0 "register_operand")
	(unspec:VNx4SI [(match_dup 4) (match_dup 5)] UNSPEC_UZP1))]
  "TARGET_SVE"
  {
    operands[3] = force_reg (VNx2BImode, CONSTM1_RTX (VNx2BImode));
    operands[4] = gen_reg_rtx (VNx4SImode);
    operands[5] = gen_reg_rtx (VNx4SImode);
  }
)

;; General splitter for conditional binary floating-point operations
;; that cannot use MOVPRFX and instead require a SEL.
(define_split
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 5)
	      (match_operand:SVE_F 2 "register_operand")
	      (match_operand:SVE_F 3)]
	     SVE_COND_FP_BINARY)
	   (match_operand:SVE_F 4 "register_operand")]
	  UNSPEC_SEL))]
  "TARGET_SVE
   && reload_completed
   && !rtx_equal_p (operands[0], operands[4])
   && !rtx_equal_p (operands[2], operands[4])
   && !(<can_reorder_p> && rtx_equal_p (operands[3], operands[4]))"
  [(set (match_dup 0)
	(unspec:SVE_F [(match_dup 1) (match_dup 2) (match_dup 4)] UNSPEC_SEL))
   (set (match_dup 0)
	(unspec:SVE_F
	  [(match_dup 1)
	   (unspec:SVE_F [(match_dup 1)
			  (match_dup 5)
			  (match_dup 0)
			  (match_dup 3)] SVE_COND_FP_BINARY)
	   (match_dup 0)]
	  UNSPEC_SEL))]
)

;; Predicated floating-point addition with select.
(define_expand "cond_add<mode>"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (const_int SVE_FORBID_NEW_FAULTS)
	      (match_operand:SVE_F 2 "register_operand")
	      (match_operand:SVE_F 3 "aarch64_sve_float_arith_with_sub_operand")]
	     UNSPEC_COND_ADD)
	   (match_operand:SVE_F 4 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
{
  if (rtx_equal_p (operands[3], operands[4]))
    std::swap (operands[2], operands[3]);
})

;; Predicated floating-point addition with select matching first operand.
(define_insn "*cond_add<mode>_2"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, w, ?&w, ?&w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 4)
	      (match_operand:SVE_F 2 "register_operand" "0, 0, 0, w, w, w")
	      (match_operand:SVE_F 3 "aarch64_sve_float_arith_with_sub_operand" "vsA, vsN, w, vsA, vsN, w")]
	     UNSPEC_COND_ADD)
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, #%N3
   fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0, %2\;fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, #%N3
   movprfx\t%0, %2\;fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,*,*,yes,yes,yes")]
)

;; Synthetic predication of floating-point addition with select unmatched.
(define_insn "*cond_add<mode>_any"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, &w, &w, &w, &w, &w, &w, ?&w, ?&w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl, Upl, Upl, Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 5)
	      (match_operand:SVE_F 2 "register_operand" "w, w, 0, w, w, w, w, w, w, w, w")
	      (match_operand:SVE_F 3 "aarch64_sve_float_arith_with_sub_operand" "vsA, vsN, w, 0, w, vsA, vsN, w, vsA, vsN, w")]
	     UNSPEC_COND_ADD)
	   (match_operand:SVE_F 4 "aarch64_simd_reg_or_zero" "Dz, Dz, Dz, Dz, Dz, 0, 0, 0, w, w, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE
   && !rtx_equal_p (operands[2], operands[4])
   && !rtx_equal_p (operands[3], operands[4])"
  "@
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, #%N3
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, #%N3
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;fadd\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   #
   #
   #"
  [(set_attr "movprfx" "yes")]
)

;; Predicated floating-point subtraction with select.
(define_expand "cond_sub<mode>"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (const_int SVE_FORBID_NEW_FAULTS)
	      (match_operand:SVE_F 2 "aarch64_sve_float_arith_operand")
	      (match_operand:SVE_F 3 "register_operand")]
	     UNSPEC_COND_SUB)
	   (match_operand:SVE_F 4 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
)

;; Predicated floating-point subtraction with select matching first operand.
(define_insn "*cond_sub<mode>_2"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 4)
	      (match_operand:SVE_F 2 "register_operand" "0, w")
	      (match_operand:SVE_F 3 "register_operand" "w, w")]
	     UNSPEC_COND_SUB)
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated floating-point subtraction with select matching second operand.
(define_insn "*cond_sub<mode>_3"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, ?&w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 4)
	      (match_operand:SVE_F 2 "aarch64_sve_float_arith_operand" "vsA, w, vsA, w")
	      (match_operand:SVE_F 3 "register_operand" "0, 0, w, w")]
	     UNSPEC_COND_SUB)
	   (match_dup 3)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, #%2
   fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0, %3\;fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, #%2
   movprfx\t%0, %3\;fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>"
  [(set_attr "movprfx" "*,*,yes,yes")]
)

;; Synthetic predication of floating-point subtraction with select unmatched.
(define_insn_and_rewrite "*cond_sub<mode>_any"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, &w, &w, &w, &w, &w, ?&w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 5)
	      (match_operand:SVE_F 2 "aarch64_sve_float_arith_operand" "vsA, 0, w, w, vsA, w, vsA, w")
	      (match_operand:SVE_F 3 "register_operand" "w, w, 0, w, w, w, w, w")]
	     UNSPEC_COND_SUB)
	   (match_operand:SVE_F 4 "aarch64_simd_reg_or_zero" "Dz, Dz, Dz, Dz, 0, 0, w, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE
   && !rtx_equal_p (operands[2], operands[4])
   && !rtx_equal_p (operands[3], operands[4])"
  "@
   movprfx\t%0.<Vetype>, %1/z, %3.<Vetype>\;fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, #%2
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/m, %3.<Vetype>\;fsubr\t%0.<Vetype>, %1/m, %0.<Vetype>, #%2
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;fsub\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   #
   #"
  ; Handle the split for FSUBR (the last-but-one alternative).  The generic
  ; splitter above handles the final all-register alternative.
  "&& reload_completed
   && !rtx_equal_p (operands[0], operands[4])
   && CONSTANT_P (operands[2])
   && !CONSTANT_P (operands[4])"
  {
    emit_insn (gen_vcond_mask_<mode><vpred> (operands[0], operands[3],
					     operands[4], operands[1]));
    operands[4] = operands[3] = operands[0];
  }
  [(set_attr "movprfx" "yes")]
)

;; Predicated floating-point binary operations with select, covering
;; operations that can take a single-bit immediate.
(define_expand "@cond_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (const_int SVE_FORBID_NEW_FAULTS)
	      (match_operand:SVE_F 2 "register_operand")
	      (match_operand:SVE_F 3 "aarch64_sve_float_<sve_imm_pred>_operand")]
	     SVE_COND_FP_BINARY_I1)
	   (match_operand:SVE_F 4 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
{
  if (!CONSTANT_P (operands[4]) && rtx_equal_p (operands[3], operands[4]))
    std::swap (operands[2], operands[3]);
})

;; Predicated floating-point binary operations with select matching second
;; operand, covering operations that can take a single-bit immediate.
(define_insn "*cond_mul<mode>_2"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, w, ?&w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 4)
	      (match_operand:SVE_F 2 "register_operand" "0, 0, w, w")
	      (match_operand:SVE_F 3 "aarch64_sve_float_<sve_imm_pred>_operand" "<sve_imm_con>, w, <sve_imm_con>, w")]
	     SVE_COND_FP_BINARY_I1)
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   <sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0, %2\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*,*,yes,yes")]
)

;; Predicated floating-point binary operations in which the values of inactive
;; lanes are distinct from the other inputs, covering operations that can take
;; a single-bit immediate.
(define_insn "*cond_mul<mode>_any"
  [(set (match_operand:SVE_F 0 "register_operand" "=&w, &w, &w, &w, &w, &w, ?&w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 5)
	      (match_operand:SVE_F 2 "register_operand" "%0, 0, w, w, w, w, w, w")
	      (match_operand:SVE_F 3 "aarch64_sve_float_<sve_imm_pred>_operand" "<sve_imm_con>, w, <sve_imm_con>, w, <sve_imm_con>, w, <sve_imm_con>, w")]
	     SVE_COND_FP_BINARY_I1)
	   (match_operand:SVE_F 4 "aarch64_simd_reg_or_zero" "Dz, Dz, Dz, Dz, 0, 0, w, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, #%3
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   #
   #"
  [(set_attr "movprfx" "yes")]
)

;; Predicated floating-point operations with select.
(define_expand "@cond_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (const_int SVE_FORBID_NEW_FAULTS)
	      (match_operand:SVE_F 2 "register_operand")
	      (match_operand:SVE_F 3 "register_operand")]
	     SVE_COND_FP_BINARY_REG)
	   (match_operand:SVE_F 4 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
)

;; Predicated floating-point operations with select matching first operand.
(define_insn "*cond_<optab><mode>_2"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 4)
	      (match_operand:SVE_F 2 "register_operand" "0, w")
	      (match_operand:SVE_F 3 "register_operand" "w, w")]
	     SVE_COND_FP_BINARY_REG)
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype><maybe_op_imm>
   movprfx\t%0, %2\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype><maybe_op_imm>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated floating-point operations with select matching second operand.
(define_insn "*cond_<optab><mode>_3"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 4)
	      (match_operand:SVE_F 2 "register_operand" "w, w")
	      (match_operand:SVE_F 3 "register_operand" "0, w")]
	     SVE_COND_FP_BINARY_REG_REORDER)
	   (match_dup 3)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_fp_op_rev>\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0, %3\;<sve_fp_op_rev>\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated floating-point binary operations in which the values of
;; inactive lanes are distinct from the other inputs.
(define_insn "*cond_<optab><mode>_any"
  [(set (match_operand:SVE_F 0 "register_operand" "=&w, &w, &w, &w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 5)
	      (match_operand:SVE_F 2 "register_operand" "0, w, w, w, w")
	      (match_operand:SVE_F 3 "register_operand" "w, 0, w, w, w")]
	     SVE_COND_FP_BINARY_REG_REORDER)
	   (match_operand:SVE_F 4 "aarch64_simd_reg_or_zero" "Dz, Dz, Dz, 0, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE
   && !rtx_equal_p (operands[2], operands[4])
   && !rtx_equal_p (operands[3], operands[4])"
  "@
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_fp_op_rev>\t%0.<Vetype>, %1/m, %0.<Vetype>, %2.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype>
   #"
  [(set_attr "movprfx" "yes")]
)

;; Predicated FCADD operations in which the values of inactive lanes are
;; distinct from the other inputs.
(define_insn "*cond_<optab><mode>_any"
  [(set (match_operand:SVE_F 0 "register_operand" "=&w, &w, &w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 5)
	      (match_operand:SVE_F 2 "register_operand" "0, w, w, w")
	      (match_operand:SVE_F 3 "register_operand" "w, w, w, w")]
	     SVE_COND_FP_BINARY_REG_NOREORDER)
	   (match_operand:SVE_F 4 "aarch64_simd_reg_or_zero" "Dz, Dz, 0, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE && !rtx_equal_p (operands[2], operands[4])"
  "@
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype><maybe_op_imm>
   movprfx\t%0.<Vetype>, %1/z, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype><maybe_op_imm>
   movprfx\t%0.<Vetype>, %1/m, %2.<Vetype>\;<sve_fp_op>\t%0.<Vetype>, %1/m, %0.<Vetype>, %3.<Vetype><maybe_op_imm>
   #"
  [(set_attr "movprfx" "yes")]
)

;; General splitter for conditional ternary floating-point operations
;; that cannot use MOVPRFX and instead require a SEL.
(define_split
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 6)
	      (match_operand:SVE_F 2 "register_operand")
	      (match_operand:SVE_F 3 "register_operand")
	      (match_operand:SVE_F 4 "register_operand")]
	     SVE_COND_FP_TERNARY)
	   (match_operand:SVE_F 5 "register_operand")]
	  UNSPEC_SEL))]
  "TARGET_SVE
   && reload_completed
   && !rtx_equal_p (operands[0], operands[5])
   && !(<can_reorder_p> && rtx_equal_p (operands[2], operands[5]))
   && !(<can_reorder_p> && rtx_equal_p (operands[3], operands[5]))
   && !rtx_equal_p (operands[4], operands[5])"
  [(set (match_dup 0)
	(unspec:SVE_F [(match_dup 1) (match_dup 4) (match_dup 5)] UNSPEC_SEL))
   (set (match_dup 0)
	(unspec:SVE_F
	  [(match_dup 1)
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_dup 6)
	      (match_dup 2)
	      (match_dup 3)
	      (match_dup 0)]
	     SVE_COND_FP_TERNARY)
	   (match_dup 0)]
	  UNSPEC_SEL))]
)

;; Predicated floating-point ternary operations with select.
(define_expand "@cond_<optab><mode>"
  [(set (match_operand:SVE_F 0 "register_operand")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (const_int SVE_FORBID_NEW_FAULTS)
	      (match_operand:SVE_F 2 "register_operand")
	      (match_operand:SVE_F 3 "register_operand")
	      (match_operand:SVE_F 4 "register_operand")]
	     SVE_COND_FP_TERNARY)
	   (match_operand:SVE_F 5 "aarch64_simd_reg_or_zero")]
	  UNSPEC_SEL))]
  "TARGET_SVE"
{
  /* Swap the multiplication operands if the fallback value is the
     second of the two.  */
  if (<can_reorder_p> && rtx_equal_p (operands[3], operands[5]))
    std::swap (operands[2], operands[3]);
})

;; Predicated floating-point ternary operations using the FMAD-like form.
(define_insn "*cond_<optab><mode>_2"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 5)
	      (match_operand:SVE_F 2 "register_operand" "0, w")
	      (match_operand:SVE_F 3 "register_operand" "w, w")
	      (match_operand:SVE_F 4 "register_operand" "w, w")]
	     SVE_COND_FP_TERNARY_REORDER)
	   (match_dup 2)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_fmad_op>\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   movprfx\t%0, %2\;<sve_fmad_op>\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated floating-point ternary operations using the FMLA-like form.
(define_insn "*cond_<optab><mode>_4"
  [(set (match_operand:SVE_F 0 "register_operand" "=w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 5)
	      (match_operand:SVE_F 2 "register_operand" "w, w")
	      (match_operand:SVE_F 3 "register_operand" "w, w")
	      (match_operand:SVE_F 4 "register_operand" "0, w")]
	     SVE_COND_FP_TERNARY)
	   (match_dup 4)]
	  UNSPEC_SEL))]
  "TARGET_SVE"
  "@
   <sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype><maybe_op_imm>
   movprfx\t%0, %4\;<sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype><maybe_op_imm>"
  [(set_attr "movprfx" "*,yes")]
)

;; Predicated floating-point ternary operations in which the value for
;; inactive lanes are distinct from the other inputs.
(define_insn "*cond_<optab><mode>_any"
  [(set (match_operand:SVE_F 0 "register_operand" "=&w, &w, &w, &w, &w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 6)
	      (match_operand:SVE_F 2 "register_operand" "w, 0, w, w, w, w")
	      (match_operand:SVE_F 3 "register_operand" "w, w, 0, w, w, w")
	      (match_operand:SVE_F 4 "register_operand" "w, w, w, 0, w, w")]
	     SVE_COND_FP_TERNARY_REORDER)
	   (match_operand:SVE_F 5 "aarch64_simd_reg_or_zero" "Dz, Dz, Dz, Dz, 0, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE
   && !rtx_equal_p (operands[2], operands[5])
   && !rtx_equal_p (operands[3], operands[5])
   && !rtx_equal_p (operands[4], operands[5])"
  "@
   movprfx\t%0.<Vetype>, %1/z, %4.<Vetype>\;<sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_fmad_op>\t%0.<Vetype>, %1/m, %3.<Vetype>, %4.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_fmad_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %4.<Vetype>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   movprfx\t%0.<Vetype>, %1/m, %4.<Vetype>\;<sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype>
   #"
  [(set_attr "movprfx" "yes")]
)

(define_insn "*cond_<optab><mode>_any"
  [(set (match_operand:SVE_F 0 "register_operand" "=&w, &w, &w, ?&w")
	(unspec:SVE_F
	  [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl, Upl, Upl")
	   (unspec:SVE_F
	     [(match_dup 1)
	      (match_operand 6)
	      (match_operand:SVE_F 2 "register_operand" "w, w, w, w")
	      (match_operand:SVE_F 3 "register_operand" "w, w, w, w")
	      (match_operand:SVE_F 4 "register_operand" "w, 0, w, w")]
	     SVE_COND_FP_TERNARY_NOREORDER)
	   (match_operand:SVE_F 5 "aarch64_simd_reg_or_zero" "Dz, Dz, 0, w")]
	  UNSPEC_SEL))]
  "TARGET_SVE && !rtx_equal_p (operands[4], operands[5])"
  "@
   movprfx\t%0.<Vetype>, %1/z, %4.<Vetype>\;<sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype><maybe_op_imm>
   movprfx\t%0.<Vetype>, %1/z, %0.<Vetype>\;<sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype><maybe_op_imm>
   movprfx\t%0.<Vetype>, %1/m, %4.<Vetype>\;<sve_fmla_op>\t%0.<Vetype>, %1/m, %2.<Vetype>, %3.<Vetype><maybe_op_imm>
   #"
  [(set_attr "movprfx" "yes")]
)

;; Shift an SVE vector left and insert a scalar into element 0.
(define_insn "vec_shl_insert_<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=?w, w, ??&w, ?&w")
	(unspec:SVE_ALL
	  [(match_operand:SVE_ALL 1 "register_operand" "0, 0, w, w")
	   (match_operand:<VEL> 2 "aarch64_reg_or_zero" "rZ, w, rZ, w")]
	  UNSPEC_INSR))]
  "TARGET_SVE"
  "@
   insr\t%0.<Vetype>, %<vwcore>2
   insr\t%0.<Vetype>, %<Vetype>2
   movprfx\t%0, %1\;insr\t%0.<Vetype>, %<vwcore>2
   movprfx\t%0, %1\;insr\t%0.<Vetype>, %<Vetype>2"
  [(set_attr "movprfx" "*,*,yes,yes")]
)

(define_expand "copysign<mode>3"
  [(match_operand:SVE_F 0 "register_operand")
   (match_operand:SVE_F 1 "register_operand")
   (match_operand:SVE_F 2 "register_operand")]
  "TARGET_SVE"
  {
    rtx sign = gen_reg_rtx (<V_INT_EQUIV>mode);
    rtx mant = gen_reg_rtx (<V_INT_EQUIV>mode);
    rtx int_res = gen_reg_rtx (<V_INT_EQUIV>mode);
    int bits = GET_MODE_UNIT_BITSIZE (<MODE>mode) - 1;

    rtx arg1 = lowpart_subreg (<V_INT_EQUIV>mode, operands[1], <MODE>mode);
    rtx arg2 = lowpart_subreg (<V_INT_EQUIV>mode, operands[2], <MODE>mode);

    emit_insn (gen_and<v_int_equiv>3
	       (sign, arg2,
		aarch64_simd_gen_const_vector_dup (<V_INT_EQUIV>mode,
						   HOST_WIDE_INT_M1U
						   << bits)));
    emit_insn (gen_and<v_int_equiv>3
	       (mant, arg1,
		aarch64_simd_gen_const_vector_dup (<V_INT_EQUIV>mode,
						   ~(HOST_WIDE_INT_M1U
						     << bits))));
    emit_insn (gen_ior<v_int_equiv>3 (int_res, sign, mant));
    emit_move_insn (operands[0], gen_lowpart (<MODE>mode, int_res));
    DONE;
  }
)

(define_expand "xorsign<mode>3"
  [(match_operand:SVE_F 0 "register_operand")
   (match_operand:SVE_F 1 "register_operand")
   (match_operand:SVE_F 2 "register_operand")]
  "TARGET_SVE"
  {
    rtx sign = gen_reg_rtx (<V_INT_EQUIV>mode);
    rtx int_res = gen_reg_rtx (<V_INT_EQUIV>mode);
    int bits = GET_MODE_UNIT_BITSIZE (<MODE>mode) - 1;

    rtx arg1 = lowpart_subreg (<V_INT_EQUIV>mode, operands[1], <MODE>mode);
    rtx arg2 = lowpart_subreg (<V_INT_EQUIV>mode, operands[2], <MODE>mode);

    emit_insn (gen_and<v_int_equiv>3
	       (sign, arg2,
		aarch64_simd_gen_const_vector_dup (<V_INT_EQUIV>mode,
						   HOST_WIDE_INT_M1U
						   << bits)));
    emit_insn (gen_xor<v_int_equiv>3 (int_res, arg1, sign));
    emit_move_insn (operands[0], gen_lowpart (<MODE>mode, int_res));
    DONE;
  }
)

;; Unpredicated DOT product.
(define_insn "<sur>dot_prod<vsi2qi>"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w, ?&w")
	(plus:SVE_SDI (unspec:SVE_SDI [(match_operand:<VSI2QI> 1 "register_operand" "w, w")
				       (match_operand:<VSI2QI> 2 "register_operand" "w, w")]
		       DOTPROD)
	(match_operand:SVE_SDI 3 "register_operand" "0, w")))]
  "TARGET_SVE"
  "@
   <sur>dot\\t%0.<Vetype>, %1.<Vetype_fourth>, %2.<Vetype_fourth>
   movprfx\t%0, %3\;<sur>dot\\t%0.<Vetype>, %1.<Vetype_fourth>, %2.<Vetype_fourth>"
  [(set_attr "movprfx" "*,yes")]
)

;; Unpredicated DOT product by selected lanes.
(define_insn "@aarch64_<sur>dot_prod_lane<vsi2qi>"
  [(set (match_operand:SVE_SDI 0 "register_operand" "=w, ?&w")
	(plus:SVE_SDI
	  (unspec:SVE_SDI
	    [(match_operand:<VSI2QI> 1 "register_operand" "w, w")
	     (unspec:<VSI2QI>
	       [(match_operand:<VSI2QI> 2 "register_operand" "w, w")
		(match_operand:SI 3 "const_int_operand")]
	       UNSPEC_SVE_LANE_SELECT)]
	    DOTPROD)
	  (match_operand:SVE_SDI 4 "register_operand" "0, w")))]
  "TARGET_SVE"
  "@
   <sur>dot\\t%0.<Vetype>, %1.<Vetype_fourth>, %2.<Vetype_fourth>[%3]
   movprfx\t%0, %4\;<sur>dot\\t%0.<Vetype>, %1.<Vetype_fourth>, %2.<Vetype_fourth>[%3]"
  [(set_attr "movprfx" "*,yes")]
)

;; FFR handling
;; ------------
;;
;; Logically we want to divide FFR-related instructions into regions
;; that contain exactly one of:
;;
;; - a single write to the FFR
;; - any number of reads from the FFR (but only one read is likely)
;; - any number of LDFF1 and LDNF1 instructions
;;
;; However, LDFF1 and LDNF1 instructions should otherwise behave like
;; normal loads as far as possible.  This means that they should be
;; schedulable within a region in the same way that LD1 would be,
;; and they should be deleted as dead if the result is unused.  The loads
;; should therefore not write to the FFR, since that would both serialize
;; the loads with respect to each other and keep the loads live for any
;; later RDFFR.
;;
;; We get around this by using a fake "FFR token" (FFRT) to help describe
;; the dependencies.  Writing to the FFRT starts a new "FFRT region",
;; while using the FFRT keeps the instruction within its region.
;; Specifically:
;;
;; - Writes start a new FFRT region as well as setting the FFR:
;;
;;       W1: parallel (FFRT = <new value>, FFR = <actual FFR value>)
;;
;; - Loads use an LD1-like instruction that also uses the FFRT, so that the
;;   loads stay within the same FFRT region:
;;
;;       L1: load data while using the FFRT
;;
;;   In addition, any FFRT region that includes a load also has at least one
;;   instance of:
;;
;;       L2: FFR = update(FFR, FFRT)  [ghost instruction]
;;
;;   to make it clear that the region both reads from and writes to the FFR.
;;
;; - Reads do the following:
;;
;;       R1: FFRT = FFR               [ghost instruction]
;;       R2: read from the FFRT
;;       R3: FFRT = update(FFRT)      [ghost instruction]
;;
;;   R1 and R3 both create new FFRT regions, so that previous LDFF1s and
;;   LDNF1s cannot move forwards across R1 and later LDFF1s and LDNF1s
;;   cannot move backwards across R3.
;;
;; This way, writes are only kept alive by later loads or reads,
;; and write/read pairs fold normally.  For two consecutive reads,
;; the first R3 is made dead by the second R1, which in turn becomes
;; redundant with the first R1.  We then have:
;;
;;     first R1: FFRT = FFR
;;     first read from the FFRT
;;     second read from the FFRT
;;     second R3: FFRT = update(FFRT)
;;
;; i.e. the two FFRT regions collapse into a single one with two
;; independent reads.
;;
;; The model still prevents some valid optimizations though.  For example,
;; if all loads in an FFRT region are deleted as dead, nothing would remove
;; the L2 instructions.

;; [W1 above] Write to the FFR and start a new FFRT scheduling region.
(define_insn "aarch64_wrffr"
  [(set (reg:VNx16BI FFR_REGNUM)
	(match_operand:VNx16BI 0 "aarch64_simd_reg_or_minus_one" "Dm, Upa"))
   (set (reg:VNx16BI FFRT_REGNUM)
	(match_dup 0))]
  "TARGET_SVE"
  "@
   setffr
   wrffr\t%0.b"
)

;; [L2 above] Introduce a read from and write to the FFR in the current
;; FFRT region, so that the FFR value is live on entry to the region and
;; so that the FFR value visibly changes within the region.  This is used
;; (possibly multiple times) in an FFRT region that includes LDFF1 or LDNF1
;; instructions.
(define_insn "aarch64_update_ffr_for_load"
  [(set (reg:VNx16BI FFR_REGNUM)
	(unspec:VNx16BI [(reg:VNx16BI FFRT_REGNUM)
			 (reg:VNx16BI FFR_REGNUM)] UNSPEC_UPDATE_FFR))]
  "TARGET_SVE"
  ""
  [(set_attr "type" "ghost")]
)

;; [R1 above] Notionally copy the FFR to the FFRT, so that the current
;; FFR value can be read from there by the RDFFR instructions below.
;; This acts as a scheduling barrier for earlier LDFF1 and LDNF1
;; instructions and creates a natural dependency with earlier writes.
(define_insn "aarch64_copy_ffr_to_ffrt"
  [(set (reg:VNx16BI FFRT_REGNUM)
	(reg:VNx16BI FFR_REGNUM))]
  "TARGET_SVE"
  ""
  [(set_attr "type" "ghost")]
)

;; [R2 above] Read the FFR via the FFRT.
(define_insn "aarch64_rdffr"
  [(set (match_operand:VNx16BI 0 "register_operand" "=Upa")
	(reg:VNx16BI FFRT_REGNUM))]
  "TARGET_SVE"
  "rdffr\t%0.b"
)

;; Likewise with zero predication.
(define_insn "aarch64_rdffr_z"
  [(set (match_operand:VNx16BI 0 "register_operand" "=Upa")
	(and:VNx16BI
	  (reg:VNx16BI FFRT_REGNUM)
	  (match_operand:VNx16BI 1 "register_operand" "Upa")))]
  "TARGET_SVE"
  "rdffr\t%0.b, %1/z"
)

;; Read the FFR to test for a fault, without using the predicate result.
(define_insn "*aarch64_rdffr_z_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_dup 1)
	   (and:VNx16BI
	     (reg:VNx16BI FFRT_REGNUM)
	     (match_dup 1))
	   (match_operand 2 "const_int_operand")]
	  UNSPEC_PTEST))
   (clobber (match_scratch:VNx16BI 0 "=Upa"))]
  "TARGET_SVE"
  "rdffrs\t%0.b, %1/z"
)

;; Same for unpredicated RDFFR when tested with a known PTRUE.
(define_insn "*aarch64_rdffr_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_dup 1)
	   (reg:VNx16BI FFRT_REGNUM)
	   (const_int 1)]
	  UNSPEC_PTEST))
   (clobber (match_scratch:VNx16BI 0 "=Upa"))]
  "TARGET_SVE"
  "rdffrs\t%0.b, %1/z"
)

;; Read the FFR with zero predication and test the result.
(define_insn "*aarch64_rdffr_z_cc"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_dup 1)
	   (and:VNx16BI
	     (reg:VNx16BI FFRT_REGNUM)
	     (match_dup 1))
	   (match_operand 2 "const_int_operand")]
	  UNSPEC_PTEST))
   (set (match_operand:VNx16BI 0 "register_operand" "=Upa")
	(and:VNx16BI
	  (reg:VNx16BI FFRT_REGNUM)
	  (match_dup 1)))]
  "TARGET_SVE"
  "rdffrs\t%0.b, %1/z"
)

;; Same for unpredicated RDFFR when tested with a known PTRUE.
(define_insn "*aarch64_rdffr_ptest"
  [(set (reg:CC_NZC CC_REGNUM)
	(unspec:CC_NZC
	  [(match_operand:VNx16BI 1 "register_operand" "Upa")
	   (match_dup 1)
	   (reg:VNx16BI FFRT_REGNUM)
	   (const_int 1)]
	  UNSPEC_PTEST))
   (set (match_operand:VNx16BI 0 "register_operand" "=Upa")
	(reg:VNx16BI FFRT_REGNUM))]
  "TARGET_SVE"
  "rdffrs\t%0.b, %1/z"
)

;; [R3 above] Arbitrarily update the FFRT after a read from the FFR.
;; This acts as a scheduling barrier for later LDFF1 and LDNF1 instructions.
(define_insn "aarch64_update_ffrt"
  [(set (reg:VNx16BI FFRT_REGNUM)
	(unspec:VNx16BI [(reg:VNx16BI FFRT_REGNUM)] UNSPEC_UPDATE_FFRT))]
  "TARGET_SVE"
  ""
  [(set_attr "type" "ghost")]
)

;; Contiguous non-extending first-faulting or non-faulting loads.
(define_insn "@aarch64_ld<f>f1<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w")
	(unspec:SVE_ALL
	  [(match_operand:<VPRED> 2 "register_operand" "Upl")
	   (match_operand:SVE_ALL 1 "aarch64_sve_ld<f>f1_operand" "Ut<f>")
	   (reg:VNx16BI FFRT_REGNUM)]
	  SVE_LDFF1_LDNF1))]
  "TARGET_SVE"
  "ld<f>f1<Vesize>\t%0.<Vetype>, %2/z, %1"
)

;; Predicated first-faulting or non-faulting load and extend, with 8 elements
;; per 128-bit block.
(define_insn "@aarch64_ld<f>f1_<ANY_EXTEND:optab><VNx8_WIDE:mode><VNx8_NARROW:mode>"
  [(set (match_operand:VNx8_WIDE 0 "register_operand" "=w")
	(ANY_EXTEND:VNx8_WIDE
	  (unspec:VNx8_NARROW
	    [(match_operand:VNx8BI 2 "register_operand" "Upl")
	     (match_operand:VNx8_NARROW 1 "aarch64_sve_ld<f>f1_operand" "Ut<f>")
	     (reg:VNx16BI FFRT_REGNUM)]
	    SVE_LDFF1_LDNF1)))]
  "TARGET_SVE"
  "ld<f>f1<ANY_EXTEND:s><VNx8_NARROW:Vesize>\t%0.<VNx8_WIDE:Vetype>, %2/z, %1"
)

;; Predicated first-faulting or non-faulting load and extend, with 4 elements
;; per 128-bit block.
(define_insn "@aarch64_ld<f>f1_<ANY_EXTEND:optab><VNx4_WIDE:mode><VNx4_NARROW:mode>"
  [(set (match_operand:VNx4_WIDE 0 "register_operand" "=w")
	(ANY_EXTEND:VNx4_WIDE
	  (unspec:VNx4_NARROW
	    [(match_operand:VNx4BI 2 "register_operand" "Upl")
	     (match_operand:VNx4_NARROW 1 "aarch64_sve_ld<f>f1_operand" "Ut<f>")
	     (reg:VNx16BI FFRT_REGNUM)]
	    SVE_LDFF1_LDNF1)))]
  "TARGET_SVE"
  "ld<f>f1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.<VNx4_WIDE:Vetype>, %2/z, %1"
)

;; Predicated first-faulting or non-faulting load and extend, with 2 elements
;; per 128-bit block.
(define_insn "@aarch64_ld<f>f1_<ANY_EXTEND:optab><VNx2_WIDE:mode><VNx2_NARROW:mode>"
  [(set (match_operand:VNx2_WIDE 0 "register_operand" "=w")
	(ANY_EXTEND:VNx2_WIDE
	  (unspec:VNx2_NARROW
	    [(match_operand:VNx2BI 2 "register_operand" "Upl")
	     (match_operand:VNx2_NARROW 1 "aarch64_sve_ld<f>f1_operand" "Ut<f>")
	     (reg:VNx16BI FFRT_REGNUM)]
	    SVE_LDFF1_LDNF1)))]
  "TARGET_SVE"
  "ld<f>f1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.<VNx2_WIDE:Vetype>, %2/z, %1"
)

;; Predicated first-faulting gather loads for 32-bit elements.  Operand
;; 3 is true for unsigned extension and false for signed extension.
(define_insn "@aarch64_ldff1_gather<mode>"
  [(set (match_operand:SVE_S 0 "register_operand" "=w, w, w, w, w, w")
	(unspec:SVE_S
	  [(match_operand:VNx4BI 5 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	   (match_operand:DI 1 "aarch64_sve_gather_offset_w" "Z, vgw, rk, rk, rk, rk")
	   (match_operand:VNx4SI 2 "register_operand" "w, w, w, w, w, w")
	   (match_operand:DI 3 "const_int_operand" "i, i, Z, Ui1, Z, Ui1")
	   (match_operand:DI 4 "aarch64_gather_scale_operand_w" "Ui1, Ui1, Ui1, Ui1, i, i")
	   (mem:BLK (scratch))
	   (reg:VNx16BI FFRT_REGNUM)]
	  UNSPEC_LDFF1_GATHER))]
  "TARGET_SVE"
  "@
   ldff1w\t%0.s, %5/z, [%2.s]
   ldff1w\t%0.s, %5/z, [%2.s, #%1]
   ldff1w\t%0.s, %5/z, [%1, %2.s, sxtw]
   ldff1w\t%0.s, %5/z, [%1, %2.s, uxtw]
   ldff1w\t%0.s, %5/z, [%1, %2.s, sxtw %p4]
   ldff1w\t%0.s, %5/z, [%1, %2.s, uxtw %p4]"
)

;; Predicated first-faulting gather loads for 64-bit elements.  The value
;; of operand 3 doesn't matter in this case.
(define_insn "@aarch64_ldff1_gather<mode>"
  [(set (match_operand:SVE_D 0 "register_operand" "=w, w, w, w")
	(unspec:SVE_D
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl, Upl, Upl")
	   (match_operand:DI 1 "aarch64_sve_gather_offset_d" "Z, vgd, rk, rk")
	   (match_operand:VNx2DI 2 "register_operand" "w, w, w, w")
	   (match_operand:DI 3 "const_int_operand")
	   (match_operand:DI 4 "aarch64_gather_scale_operand_d" "Ui1, Ui1, Ui1, i")
	   (mem:BLK (scratch))
	   (reg:VNx16BI FFRT_REGNUM)]
	  UNSPEC_LDFF1_GATHER))]
  "TARGET_SVE"
  "@
   ldff1d\t%0.d, %5/z, [%2.d]
   ldff1d\t%0.d, %5/z, [%2.d, #%1]
   ldff1d\t%0.d, %5/z, [%1, %2.d]
   ldff1d\t%0.d, %5/z, [%1, %2.d, lsl %p4]"
)

;; Likewise, but with the offset being sign-extended from 32 bits.
(define_insn "*aarch64_ldff1_gather<mode>_sxtw"
  [(set (match_operand:SVE_D 0 "register_operand" "=w, w")
	(unspec:SVE_D
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	   (match_operand:DI 1 "register_operand" "rk, rk")
	   (unspec:VNx2DI
	     [(match_dup 5)
	      (sign_extend:VNx2DI
		(truncate:VNx2SI
		  (match_operand:VNx2DI 2 "register_operand" "w, w")))]
	     UNSPEC_MERGE_PTRUE)
	   (match_operand:DI 3 "const_int_operand")
	   (match_operand:DI 4 "aarch64_gather_scale_operand_d" "Ui1, i")
	   (mem:BLK (scratch))
	   (reg:VNx16BI FFRT_REGNUM)]
	  UNSPEC_LDFF1_GATHER))]
  "TARGET_SVE"
  "@
   ldff1d\t%0.d, %5/z, [%1, %2.d, sxtw]
   ldff1d\t%0.d, %5/z, [%1, %2.d, sxtw %p4]"
)

;; Likewise, but with the offset being zero-extended from 32 bits.
(define_insn "*aarch64_ldff1_gather<mode>_uxtw"
  [(set (match_operand:SVE_D 0 "register_operand" "=w, w")
	(unspec:SVE_D
	  [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	   (match_operand:DI 1 "register_operand" "rk, rk")
	   (and:VNx2DI
	     (match_operand:VNx2DI 2 "register_operand" "w, w")
	     (match_operand:VNx2DI 6 "aarch64_sve_uxtw_immediate"))
	   (match_operand:DI 3 "const_int_operand")
	   (match_operand:DI 4 "aarch64_gather_scale_operand_d" "Ui1, i")
	   (mem:BLK (scratch))
	   (reg:VNx16BI FFRT_REGNUM)]
	  UNSPEC_LDFF1_GATHER))]
  "TARGET_SVE"
  "@
   ldff1d\t%0.d, %5/z, [%1, %2.d, uxtw]
   ldff1d\t%0.d, %5/z, [%1, %2.d, uxtw %p4]"
)

;; Predicated extending first-faulting gather loads for 32-bit elements.
;; Operand 3 is true for unsigned extension and false for signed extension.
(define_insn "@aarch64_ldff1_gather_<ANY_EXTEND:optab><VNx4_WIDE:mode><VNx4_NARROW:mode>"
  [(set (match_operand:VNx4_WIDE 0 "register_operand" "=w, w, w, w, w, w")
	(ANY_EXTEND:VNx4_WIDE
	  (unspec:VNx4_NARROW
	    [(match_operand:VNx4BI 5 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
	     (match_operand:DI 1 "aarch64_sve_gather_offset_<VNx4_NARROW:Vesize>" "Z, vg<VNx4_NARROW:Vesize>, rk, rk, rk, rk")
	     (match_operand:VNx4_WIDE 2 "register_operand" "w, w, w, w, w, w")
	     (match_operand:DI 3 "const_int_operand" "i, i, Z, Ui1, Z, Ui1")
	     (match_operand:DI 4 "aarch64_gather_scale_operand_<VNx4_NARROW:Vesize>" "Ui1, Ui1, Ui1, Ui1, i, i")
	     (mem:BLK (scratch))
	     (reg:VNx16BI FFRT_REGNUM)]
	    UNSPEC_LDFF1_GATHER)))]
  "TARGET_SVE"
  "@
   ldff1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%2.s]
   ldff1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%2.s, #%1]
   ldff1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%1, %2.s, sxtw]
   ldff1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%1, %2.s, uxtw]
   ldff1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%1, %2.s, sxtw %p4]
   ldff1<ANY_EXTEND:s><VNx4_NARROW:Vesize>\t%0.s, %5/z, [%1, %2.s, uxtw %p4]"
)

;; Predicated extending first-faulting gather loads for 64-bit elements.
;; The value of operand 3 doesn't matter in this case.
(define_insn "@aarch64_ldff1_gather_<ANY_EXTEND:optab><VNx2_WIDE:mode><VNx2_NARROW:mode>"
  [(set (match_operand:VNx2_WIDE 0 "register_operand" "=w, w, w, w")
	(ANY_EXTEND:VNx2_WIDE
	  (unspec:VNx2_NARROW
	    [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl, Upl, Upl")
	     (match_operand:DI 1 "aarch64_sve_gather_offset_<VNx2_NARROW:Vesize>" "Z, vg<VNx2_NARROW:Vesize>, rk, rk")
	     (match_operand:VNx2_WIDE 2 "register_operand" "w, w, w, w")
	     (match_operand:DI 3 "const_int_operand")
	     (match_operand:DI 4 "aarch64_gather_scale_operand_<VNx2_NARROW:Vesize>" "Ui1, Ui1, Ui1, i")
	     (mem:BLK (scratch))
	     (reg:VNx16BI FFRT_REGNUM)]
	    UNSPEC_LDFF1_GATHER)))]
  "TARGET_SVE"
  "@
   ldff1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%2.d]
   ldff1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%2.d, #%1]
   ldff1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d]
   ldff1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d, lsl %p4]"
)

;; Likewise, but with the offset being sign-extended from 32 bits.
(define_insn "*aarch64_ldff1_gather_<ANY_EXTEND:optab><VNx2_WIDE:mode><VNx2_NARROW:mode>_sxtw"
  [(set (match_operand:VNx2_WIDE 0 "register_operand" "=w, w")
	(ANY_EXTEND:VNx2_WIDE
	  (unspec:VNx2_NARROW
	    [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	     (match_operand:DI 1 "aarch64_reg_or_zero" "rk, rk")
	     (unspec:VNx2DI
	       [(match_dup 5)
		(sign_extend:VNx2DI
		  (truncate:VNx2SI
		    (match_operand:VNx2DI 2 "register_operand" "w, w")))]
	       UNSPEC_MERGE_PTRUE)
	     (match_operand:DI 3 "const_int_operand")
	     (match_operand:DI 4 "aarch64_gather_scale_operand_<VNx2_NARROW:Vesize>" "Ui1, i")
	     (mem:BLK (scratch))
	     (reg:VNx16BI FFRT_REGNUM)]
	    UNSPEC_LDFF1_GATHER)))]
  "TARGET_SVE"
  "@
   ldff1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d, sxtw]
   ldff1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d, sxtw %p4]"
)

;; Likewise, but with the offset being zero-extended from 32 bits.
(define_insn "*aarch64_ldff1_gather_<ANY_EXTEND:optab><VNx2_WIDE:mode><VNx2_NARROW:mode>_uxtw"
  [(set (match_operand:VNx2_WIDE 0 "register_operand" "=w, w")
	(ANY_EXTEND:VNx2_WIDE
	  (unspec:VNx2_NARROW
	    [(match_operand:VNx2BI 5 "register_operand" "Upl, Upl")
	     (match_operand:DI 1 "aarch64_reg_or_zero" "rk, rk")
	     (and:VNx2DI
	       (match_operand:VNx2DI 2 "register_operand" "w, w")
	       (match_operand:VNx2DI 6 "aarch64_sve_uxtw_immediate"))
	     (match_operand:DI 3 "const_int_operand")
	     (match_operand:DI 4 "aarch64_gather_scale_operand_<VNx2_NARROW:Vesize>" "Ui1, i")
	     (mem:BLK (scratch))
	     (reg:VNx16BI FFRT_REGNUM)]
	    UNSPEC_LDFF1_GATHER)))]
  "TARGET_SVE"
  "@
   ldff1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d, uxtw]
   ldff1<ANY_EXTEND:s><VNx2_NARROW:Vesize>\t%0.d, %5/z, [%1, %2.d, uxtw %p4]"
)

;; Count the number of elements in an svpattern.  Operand 1 is the pattern,
;; operand 2 is the number of elements that fit in a 128-bit block, and
;; operand 3 is a multiplier in the range [1, 16].
;;
;; Note that this pattern isn't used for SV_ALL (but would work for that too).
(define_insn "aarch64_sve_cnt_pat"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(zero_extend:DI
	  (unspec:SI [(match_operand:DI 1 "const_int_operand")
		      (match_operand:DI 2 "const_int_operand")
		      (match_operand:DI 3 "const_int_operand")]
		     UNSPEC_SVE_CNT_PAT)))]
  "TARGET_SVE"
  {
    return aarch64_output_sve_cnt_pat_immediate ("cnt", "%x0", operands + 1);
  }
)

;; Increment a DImode register by the number of elements in an svpattern.
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "@aarch64_sve_<inc_dec><mode>_pat"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(ANY_PLUS:DI (zero_extend:DI
		       (unspec:SI [(match_operand:DI 2 "const_int_operand")
				   (match_operand:DI 3 "const_int_operand")
				   (match_operand:DI 4 "const_int_operand")]
				  UNSPEC_SVE_CNT_PAT))
		     (match_operand:DI_ONLY 1 "register_operand" "0")))]
  "TARGET_SVE"
  {
    return aarch64_output_sve_cnt_pat_immediate ("<inc_dec>", "%x0",
						 operands + 2);
  }
)

;; Increment an SImode register by the number of elements in an svpattern.
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "*aarch64_sve_incsi_pat"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI (unspec:SI [(match_operand:DI 2 "const_int_operand")
			     (match_operand:DI 3 "const_int_operand")
			     (match_operand:DI 4 "const_int_operand")]
			    UNSPEC_SVE_CNT_PAT)
		 (match_operand:SI 1 "register_operand" "0")))]
  "TARGET_SVE"
  {
    return aarch64_output_sve_cnt_pat_immediate ("inc", "%x0", operands + 2);
  }
)

;; Saturating increment of an SImode register by the number of elements
;; in an svpattern, extending the result to 64 bits.
;;
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "@aarch64_sve_<inc_dec><mode>_pat"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(<paired_extend>:DI
	  (SAT_PLUS:SI
	    (unspec:SI [(match_operand:DI 2 "const_int_operand")
			(match_operand:DI 3 "const_int_operand")
			(match_operand:DI 4 "const_int_operand")]
		       UNSPEC_SVE_CNT_PAT)
	    (match_operand:SI_ONLY 1 "register_operand" "0"))))]
  "TARGET_SVE"
  {
    const char *registers = (<CODE> == SS_PLUS ? "%x0, %w0" : "%w0");
    return aarch64_output_sve_cnt_pat_immediate ("<inc_dec>", registers,
						 operands + 2);
  }
)

;; Increment a vector of DIs by the number of elements in an svpattern.
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "@aarch64_sve_<inc_dec><mode>_pat"
  [(set (match_operand:VNx2DI 0 "register_operand" "=w, ?&w")
	(ANY_PLUS:VNx2DI
	  (vec_duplicate:VNx2DI
	    (zero_extend:DI
	      (unspec:SI [(match_operand:DI 2 "const_int_operand")
			  (match_operand:DI 3 "const_int_operand")
			  (match_operand:DI 4 "const_int_operand")]
			 UNSPEC_SVE_CNT_PAT)))
	  (match_operand:VNx2DI_ONLY 1 "register_operand" "0, w")))]
  "TARGET_SVE"
  {
    if (which_alternative == 1)
      output_asm_insn ("movprfx\t%0, %1", operands);
    return aarch64_output_sve_cnt_pat_immediate ("<inc_dec>", "%0.<Vetype>",
						 operands + 2);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Increment a vector of SIs by the number of elements in an svpattern.
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "@aarch64_sve_<inc_dec><mode>_pat"
  [(set (match_operand:VNx4SI 0 "register_operand" "=w, ?&w")
	(ANY_PLUS:VNx4SI
	  (vec_duplicate:VNx4SI
	    (unspec:SI [(match_operand:DI 2 "const_int_operand")
			(match_operand:DI 3 "const_int_operand")
			(match_operand:DI 4 "const_int_operand")]
		       UNSPEC_SVE_CNT_PAT))
	  (match_operand:VNx4SI_ONLY 1 "register_operand" "0, w")))]
  "TARGET_SVE"
  {
    if (which_alternative == 1)
      output_asm_insn ("movprfx\t%0, %1", operands);
    return aarch64_output_sve_cnt_pat_immediate ("<inc_dec>", "%0.<Vetype>",
						 operands + 2);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Increment a vector of HIs by the number of elements in an svpattern.
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "@aarch64_sve_<inc_dec><mode>_pat"
  [(set (match_operand:VNx8HI 0 "register_operand" "=w, ?&w")
	(ANY_PLUS:VNx8HI
	  (vec_duplicate:VNx8HI
	    (match_operator:HI 5 "subreg_lowpart_operator"
	      [(unspec:SI [(match_operand:DI 2 "const_int_operand")
			   (match_operand:DI 3 "const_int_operand")
			   (match_operand:DI 4 "const_int_operand")]
			  UNSPEC_SVE_CNT_PAT)]))
	  (match_operand:VNx8HI_ONLY 1 "register_operand" "0, w")))]
  "TARGET_SVE"
  {
    if (which_alternative == 1)
      output_asm_insn ("movprfx\t%0, %1", operands);
    return aarch64_output_sve_cnt_pat_immediate ("<inc_dec>", "%0.<Vetype>",
						 operands + 2);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Decrement a DImode register by the number of elements in an svpattern.
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "@aarch64_sve_<inc_dec><mode>_pat"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(ANY_MINUS:DI (match_operand:DI_ONLY 1 "register_operand" "0")
		      (zero_extend:DI
			(unspec:SI [(match_operand:DI 2 "const_int_operand")
				    (match_operand:DI 3 "const_int_operand")
				    (match_operand:DI 4 "const_int_operand")]
				   UNSPEC_SVE_CNT_PAT))))]
  "TARGET_SVE"
  {
    return aarch64_output_sve_cnt_pat_immediate ("<inc_dec>", "%x0",
						 operands + 2);
  }
)

;; Decrement an SImode register by the number of elements in an svpattern.
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "*aarch64_sve_decsi_pat"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI (match_operand:SI 1 "register_operand" "0")
		  (unspec:SI [(match_operand:DI 2 "const_int_operand")
			      (match_operand:DI 3 "const_int_operand")
			      (match_operand:DI 4 "const_int_operand")]
			     UNSPEC_SVE_CNT_PAT)))]
  "TARGET_SVE"
  {
    return aarch64_output_sve_cnt_pat_immediate ("dec", "%x0", operands + 2);
  }
)

;; Saturating decrement of an SImode register by the number of elements
;; in an svpattern, extending the result to 64 bits.
;;
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "@aarch64_sve_<inc_dec><mode>_pat"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(<paired_extend>:DI
	  (SAT_MINUS:SI
	    (match_operand:SI_ONLY 1 "register_operand" "0")
	    (unspec:SI [(match_operand:DI 2 "const_int_operand")
			(match_operand:DI 3 "const_int_operand")
			(match_operand:DI 4 "const_int_operand")]
		       UNSPEC_SVE_CNT_PAT))))]
  "TARGET_SVE"
  {
    const char *registers = (<CODE> == SS_MINUS ? "%x0, %w0" : "%w0");
    return aarch64_output_sve_cnt_pat_immediate ("<inc_dec>", registers,
						 operands + 2);
  }
)

;; Decrement a vector of DIs by the number of elements in an svpattern.
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "@aarch64_sve_<inc_dec><mode>_pat"
  [(set (match_operand:VNx2DI 0 "register_operand" "=w, ?&w")
	(ANY_MINUS:VNx2DI
	  (match_operand:VNx2DI_ONLY 1 "register_operand" "0, w")
	  (vec_duplicate:VNx2DI
	    (zero_extend:DI
	      (unspec:SI [(match_operand:DI 2 "const_int_operand")
			  (match_operand:DI 3 "const_int_operand")
			  (match_operand:DI 4 "const_int_operand")]
			 UNSPEC_SVE_CNT_PAT)))))]
  "TARGET_SVE"
  {
    if (which_alternative == 1)
      output_asm_insn ("movprfx\t%0, %1", operands);
    return aarch64_output_sve_cnt_pat_immediate ("<inc_dec>", "%0.<Vetype>",
						 operands + 2);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Decrement a vector of SIs by the number of elements in an svpattern.
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "@aarch64_sve_<inc_dec><mode>_pat"
  [(set (match_operand:VNx4SI 0 "register_operand" "=w, ?&w")
	(ANY_MINUS:VNx4SI
	  (match_operand:VNx4SI_ONLY 1 "register_operand" "0, w")
	  (vec_duplicate:VNx4SI
	    (unspec:SI [(match_operand:DI 2 "const_int_operand")
			(match_operand:DI 3 "const_int_operand")
			(match_operand:DI 4 "const_int_operand")]
		       UNSPEC_SVE_CNT_PAT))))]
  "TARGET_SVE"
  {
    if (which_alternative == 1)
      output_asm_insn ("movprfx\t%0, %1", operands);
    return aarch64_output_sve_cnt_pat_immediate ("<inc_dec>", "%0.<Vetype>",
						 operands + 2);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Decrement a vector of HIs by the number of elements in an svpattern.
;; See aarch64_sve_cnt_pat for the counting behavior.
(define_insn "@aarch64_sve_<inc_dec><mode>_pat"
  [(set (match_operand:VNx8HI 0 "register_operand" "=w, ?&w")
	(ANY_MINUS:VNx8HI
	  (match_operand:VNx8HI_ONLY 1 "register_operand" "0, w")
	  (vec_duplicate:VNx8HI
	    (match_operator:HI 5 "subreg_lowpart_operator"
	      [(unspec:SI [(match_operand:DI 2 "const_int_operand")
			   (match_operand:DI 3 "const_int_operand")
			   (match_operand:DI 4 "const_int_operand")]
			  UNSPEC_SVE_CNT_PAT)]))))]
  "TARGET_SVE"
  {
    if (which_alternative == 1)
      output_asm_insn ("movprfx\t%0, %1", operands);
    return aarch64_output_sve_cnt_pat_immediate ("<inc_dec>", "%0.<Vetype>",
						 operands + 2);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Count the number of set bits in a predicate.  Operand 3 is true if
;; operand 1 is known to be all-true.
(define_insn "@aarch64_pred_cntp<mode>"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(zero_extend:DI
	  (unspec:SI [(match_operand:PRED_ALL 1 "register_operand" "Upl")
		      (match_operand:PRED_ALL 2 "register_operand" "Upa")
		      (match_operand:SI 3 "const_int_operand")]
		     UNSPEC_CNTP)))]
  "TARGET_SVE"
  "cntp\t%x0, %1, %2.<Vetype>")

;; Increment a DImode register by the number of set bits in a predicate.
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_sve_<inc_dec><DI_ONLY:mode><PRED_ALL:mode>_cntp"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(ANY_PLUS:DI
	  (zero_extend:DI
	    (unspec:SI [(match_operand:PRED_ALL 2)
			(match_operand:PRED_ALL 3 "register_operand" "Upa")
			(const_int 1)]
		       UNSPEC_CNTP))
	  (match_operand:DI_ONLY 1 "register_operand" "0")))]
  "TARGET_SVE"
  "<inc_dec>p\t%x0, %3.<PRED_ALL:Vetype>"
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<PRED_ALL:MODE>mode);
  }
)

;; Increment an SImode register by the number of set bits in a predicate.
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_incsi<mode>_cntp"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI
	  (unspec:SI [(match_operand:PRED_ALL 2)
		      (match_operand:PRED_ALL 3 "register_operand" "Upa")
		      (const_int 1)]
		     UNSPEC_CNTP)
	  (match_operand:SI 1 "register_operand" "0")))]
  "TARGET_SVE"
  "incp\t%x0, %3.<Vetype>"
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<MODE>mode);
  }
)

;; Saturating increment of an SImode register by the number of set bits
;; in a predicate, extending the result to 64 bits.
;;
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_sve_<inc_dec><SI_ONLY:mode><PRED_ALL:mode>_cntp"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(<paired_extend>:DI
	  (SAT_PLUS:SI
	    (unspec:SI [(match_operand:PRED_ALL 2)
			(match_operand:PRED_ALL 3 "register_operand" "Upa")
			(const_int 1)]
		       UNSPEC_CNTP)
	    (match_operand:SI_ONLY 1 "register_operand" "0"))))]
  "TARGET_SVE"
  {
    if (<CODE> == SS_PLUS)
      return "<inc_dec>p\t%x0, %3.<PRED_ALL:Vetype>, %w0";
    else
      return "<inc_dec>p\t%w0, %3.<PRED_ALL:Vetype>";
  }
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<PRED_ALL:MODE>mode);
  }
)

;; Increment a vector of DIs by the number of set bits in a predicate.
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_sve_<inc_dec><mode>_cntp"
  [(set (match_operand:VNx2DI 0 "register_operand" "=w, ?&w")
	(ANY_PLUS:VNx2DI
	  (vec_duplicate:VNx2DI
	    (zero_extend:DI
	      (unspec:SI
		[(match_operand:<VPRED> 2)
		 (match_operand:<VPRED> 3 "register_operand" "Upa, Upa")
		 (const_int 1)]
		UNSPEC_CNTP)))
	  (match_operand:VNx2DI_ONLY 1 "register_operand" "0, w")))]
  "TARGET_SVE"
  "@
   <inc_dec>p\t%0.d, %3
   movprfx\t%0, %1\;<inc_dec>p\t%0.d, %3"
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<VPRED>mode);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Increment a vector of SIs by the number of set bits in a predicate.
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_sve_<inc_dec><mode>_cntp"
  [(set (match_operand:VNx4SI 0 "register_operand" "=w, ?&w")
	(ANY_PLUS:VNx4SI
	  (vec_duplicate:VNx4SI
	    (unspec:SI
	      [(match_operand:<VPRED> 2)
	       (match_operand:<VPRED> 3 "register_operand" "Upa, Upa")
	       (const_int 1)]
	      UNSPEC_CNTP))
	  (match_operand:VNx4SI_ONLY 1 "register_operand" "0, w")))]
  "TARGET_SVE"
  "@
   <inc_dec>p\t%0.s, %3
   movprfx\t%0, %1\;<inc_dec>p\t%0.s, %3"
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<VPRED>mode);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Increment a vector of HIs by the number of set bits in a predicate.
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_sve_<inc_dec><mode>_cntp"
  [(set (match_operand:VNx8HI 0 "register_operand" "=w, ?&w")
	(ANY_PLUS:VNx8HI
	  (vec_duplicate:VNx8HI
	    (match_operator:HI 4 "subreg_lowpart_operator"
	      [(unspec:SI
		 [(match_operand:<VPRED> 2)
		  (match_operand:<VPRED> 3 "register_operand" "Upa, Upa")
		  (const_int 1)]
		 UNSPEC_CNTP)]))
	  (match_operand:VNx8HI_ONLY 1 "register_operand" "0, w")))]
  "TARGET_SVE"
  "@
   <inc_dec>p\t%0.h, %3
   movprfx\t%0, %1\;<inc_dec>p\t%0.h, %3"
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<VPRED>mode);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Decrement a DImode register by the number of set bits in a predicate.
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_sve_<inc_dec><DI_ONLY:mode><PRED_ALL:mode>_cntp"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(ANY_MINUS:DI
	  (match_operand:DI_ONLY 1 "register_operand" "0")
	  (zero_extend:DI
	    (unspec:SI [(match_operand:PRED_ALL 2)
			(match_operand:PRED_ALL 3 "register_operand" "Upa")
			(const_int 1)]
		       UNSPEC_CNTP))))]
  "TARGET_SVE"
  "<inc_dec>p\t%x0, %3.<PRED_ALL:Vetype>"
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<PRED_ALL:MODE>mode);
  }
)

;; Decrement an SImode register by the number of set bits in a predicate.
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_decsi<mode>_cntp"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (unspec:SI [(match_operand:PRED_ALL 2)
		      (match_operand:PRED_ALL 3 "register_operand" "Upa")
		      (const_int 1)]
		     UNSPEC_CNTP)))]
  "TARGET_SVE"
  "decp\t%x0, %3.<Vetype>"
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<MODE>mode);
  }
)

;; Saturating decrement of an SImode register by the number of set bits
;; in a predicate, extending the result to 64 bits.
;;
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_sve_<inc_dec><SI_ONLY:mode><PRED_ALL:mode>_cntp"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(<paired_extend>:DI
	  (SAT_MINUS:SI
	    (match_operand:SI_ONLY 1 "register_operand" "0")
	    (unspec:SI [(match_operand:PRED_ALL 2)
			(match_operand:PRED_ALL 3 "register_operand" "Upa")
			(const_int 1)]
		       UNSPEC_CNTP))))]
  "TARGET_SVE"
  {
    if (<CODE> == SS_MINUS)
      return "<inc_dec>p\t%x0, %3.<PRED_ALL:Vetype>, %w0";
    else
      return "<inc_dec>p\t%w0, %3.<PRED_ALL:Vetype>";
  }
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<PRED_ALL:MODE>mode);
  }
)

;; Decrement a vector of DIs by the number of set bits in a predicate.
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_sve_<inc_dec><mode>_cntp"
  [(set (match_operand:VNx2DI 0 "register_operand" "=w, ?&w")
	(ANY_MINUS:VNx2DI
	  (match_operand:VNx2DI_ONLY 1 "register_operand" "0, w")
	  (vec_duplicate:VNx2DI
	    (zero_extend:DI
	      (unspec:SI
		[(match_operand:<VPRED> 2)
		 (match_operand:<VPRED> 3 "register_operand" "Upa, Upa")
		 (const_int 1)]
		UNSPEC_CNTP)))))]
  "TARGET_SVE"
  "@
   <inc_dec>p\t%0.d, %3
   movprfx\t%0, %1\;<inc_dec>p\t%0.d, %3"
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<VPRED>mode);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Decrement a vector of SIs by the number of set bits in a predicate.
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_sve_<inc_dec><mode>_cntp"
  [(set (match_operand:VNx4SI 0 "register_operand" "=w, ?&w")
	(ANY_MINUS:VNx4SI
	  (match_operand:VNx4SI_ONLY 1 "register_operand" "0, w")
	  (vec_duplicate:VNx4SI
	    (unspec:SI
	      [(match_operand:<VPRED> 2)
	       (match_operand:<VPRED> 3 "register_operand" "Upa, Upa")
	       (const_int 1)]
	      UNSPEC_CNTP))))]
  "TARGET_SVE"
  "@
   <inc_dec>p\t%0.s, %3
   movprfx\t%0, %1\;<inc_dec>p\t%0.s, %3"
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<VPRED>mode);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Decrement a vector of HIs by the number of set bits in a predicate.
;; See aarch64_sve_cntp for a description of the operands.
(define_insn_and_rewrite "@aarch64_sve_<inc_dec><mode>_cntp"
  [(set (match_operand:VNx8HI 0 "register_operand" "=w, ?&w")
	(ANY_MINUS:VNx8HI
	  (match_operand:VNx8HI_ONLY 1 "register_operand" "0, w")
	  (vec_duplicate:VNx8HI
	    (match_operator:HI 4 "subreg_lowpart_operator"
	      [(unspec:SI
		 [(match_operand:<VPRED> 2)
		  (match_operand:<VPRED> 3 "register_operand" "Upa, Upa")
		  (const_int 1)]
		 UNSPEC_CNTP)]))))]
  "TARGET_SVE"
  "@
   <inc_dec>p\t%0.h, %3
   movprfx\t%0, %1\;<inc_dec>p\t%0.h, %3"
  "&& !CONSTANT_P (operands[2])"
  {
    operands[2] = CONSTM1_RTX (<VPRED>mode);
  }
  [(set_attr "movprfx" "*,yes")]
)

;; Contiguous predicated prefetches.  Operand 2 gives the real prefetch
;; operation (as an svprfop), with operands 3 and 4 providing distilled
;; information.
(define_insn "@aarch64_sve_prefetch<mode>"
  [(prefetch (unspec:DI
	       [(match_operand:<VPRED> 0 "register_operand" "Upl")
		(match_operand:SVE_I 1 "aarch64_sve_prefetch_operand" "UP<Vesize>")
		(match_operand:DI 2 "const_int_operand")]
	       UNSPEC_SVE_PREFETCH)
	     (match_operand:DI 3 "const_int_operand")
	     (match_operand:DI 4 "const_int_operand"))]
  "TARGET_SVE"
  {
    operands[1] = gen_rtx_MEM (<MODE>mode, operands[1]);
    return aarch64_output_sve_prefetch ("prf<Vesize>", operands[2], "%0, %1");
  }
)

;; Predicated gather prefetches for 32-bit bases and offsets.  The operands
;; are:
;; 0: the governing predicate
;; 1: the scalar component of the address
;; 2: the vector component of the address
;; 3: 1 for sign-extension, 0 for zero-extension
;; 4: the scale multiplier
;; 5: a vector zero that identifies the mode of data being accessed
;; 6: the prefetch operator (an svprfop)
;; 7: the normal RTL prefetch rw flag
;; 8: the normal RTL prefetch locality value
(define_insn "@aarch64_sve_gather_prefetch<SVE_I:mode><VNx4SI_ONLY:mode>"
  [(prefetch (unspec:DI
	       [(match_operand:VNx4BI 0 "register_operand" "Upl, Upl, Upl, Upl, Upl, Upl")
		(match_operand:DI 1 "aarch64_sve_gather_offset_<SVE_I:Vesize>" "Z, vg<SVE_I:Vesize>, rk, rk, rk, rk")
		(match_operand:VNx4SI_ONLY 2 "register_operand" "w, w, w, w, w, w")
		(match_operand:DI 3 "const_int_operand" "i, i, Z, Ui1, Z, Ui1")
		(match_operand:DI 4 "aarch64_gather_scale_operand_<SVE_I:Vesize>" "Ui1, Ui1, Ui1, Ui1, i, i")
		(match_operand:SVE_I 5 "aarch64_simd_imm_zero")
		(match_operand:DI 6 "const_int_operand")]
	       UNSPEC_SVE_PREFETCH_GATHER)
	     (match_operand:DI 7 "const_int_operand")
	     (match_operand:DI 8 "const_int_operand"))]
  "TARGET_SVE"
  {
    static const char *const insns[][2] = {
      "prf<SVE_I:Vesize>", "%0, [%2.s]",
      "prf<SVE_I:Vesize>", "%0, [%2.s, #%1]",
      "prfb", "%0, [%1, %2.s, sxtw]",
      "prfb", "%0, [%1, %2.s, uxtw]",
      "prf<SVE_I:Vesize>", "%0, [%1, %2.s, sxtw %p4]",
      "prf<SVE_I:Vesize>", "%0, [%1, %2.s, uxtw %p4]"
    };
    const char *const *parts = insns[which_alternative];
    return aarch64_output_sve_prefetch (parts[0], operands[6], parts[1]);
  }
)

;; Predicated gather prefetches for 64-bit elements.  The value of operand 3
;; doesn't matter in this case.
(define_insn "@aarch64_sve_gather_prefetch<SVE_I:mode><VNx2DI_ONLY:mode>"
  [(prefetch (unspec:DI
	       [(match_operand:VNx2BI 0 "register_operand" "Upl, Upl, Upl, Upl")
		(match_operand:DI 1 "aarch64_sve_gather_offset_<SVE_I:Vesize>" "Z, vg<SVE_I:Vesize>, rk, rk")
		(match_operand:VNx2DI_ONLY 2 "register_operand" "w, w, w, w")
		(match_operand:DI 3 "const_int_operand")
		(match_operand:DI 4 "aarch64_gather_scale_operand_<SVE_I:Vesize>" "Ui1, Ui1, Ui1, i")
		(match_operand:SVE_I 5 "aarch64_simd_imm_zero")
		(match_operand:DI 6 "const_int_operand")]
	       UNSPEC_SVE_PREFETCH_GATHER)
	     (match_operand:DI 7 "const_int_operand")
	     (match_operand:DI 8 "const_int_operand"))]
  "TARGET_SVE"
  {
    static const char *const insns[][2] = {
      "prf<SVE_I:Vesize>", "%0, [%2.d]",
      "prf<SVE_I:Vesize>", "%0, [%2.d, #%1]",
      "prfb", "%0, [%1, %2.d]",
      "prf<SVE_I:Vesize>", "%0, [%1, %2.d, lsl %p4]"
    };
    const char *const *parts = insns[which_alternative];
    return aarch64_output_sve_prefetch (parts[0], operands[6], parts[1]);
  }
)

;; Likewise, but with the offset being sign-extended from 32 bits.
(define_insn "*aarch64_sve_gather_prefetch<SVE_I:mode><VNx2DI_ONLY:mode>_sxtw"
  [(prefetch (unspec:DI
	       [(match_operand:VNx2BI 0 "register_operand" "Upl, Upl")
		(match_operand:DI 1 "register_operand" "rk, rk")
		(unspec:VNx2DI_ONLY
		  [(match_dup 0)
		   (sign_extend:VNx2DI
		     (truncate:VNx2SI
		       (match_operand:VNx2DI 2 "register_operand" "w, w")))]
		  UNSPEC_MERGE_PTRUE)
		(match_operand:DI 3 "const_int_operand")
		(match_operand:DI 4 "aarch64_gather_scale_operand_<SVE_I:Vesize>" "Ui1, i")
		(match_operand:SVE_I 5 "aarch64_simd_imm_zero")
		(match_operand:DI 6 "const_int_operand")]
	       UNSPEC_SVE_PREFETCH_GATHER)
	     (match_operand:DI 7 "const_int_operand")
	     (match_operand:DI 8 "const_int_operand"))]
  "TARGET_SVE"
  {
    static const char *const insns[][2] = {
      "prfb", "%0, [%1, %2.d, sxtw]",
      "prf<SVE_I:Vesize>", "%0, [%1, %2.d, sxtw %p4]"
    };
    const char *const *parts = insns[which_alternative];
    return aarch64_output_sve_prefetch (parts[0], operands[6], parts[1]);
  }
)

;; Likewise, but with the offset being zero-extended from 32 bits.
(define_insn "*aarch64_sve_gather_prefetch<SVE_I:mode><VNx2DI_ONLY:mode>_uxtw"
  [(prefetch (unspec:DI
	       [(match_operand:VNx2BI 0 "register_operand" "Upl, Upl")
		(match_operand:DI 1 "register_operand" "rk, rk")
		(and:VNx2DI_ONLY
		  (match_operand:VNx2DI 2 "register_operand" "w, w")
		  (match_operand:VNx2DI 9 "aarch64_sve_uxtw_immediate"))
		(match_operand:DI 3 "const_int_operand")
		(match_operand:DI 4 "aarch64_gather_scale_operand_<SVE_I:Vesize>" "Ui1, i")
		(match_operand:SVE_I 5 "aarch64_simd_imm_zero")
		(match_operand:DI 6 "const_int_operand")]
	       UNSPEC_SVE_PREFETCH_GATHER)
	     (match_operand:DI 7 "const_int_operand")
	     (match_operand:DI 8 "const_int_operand"))]
  "TARGET_SVE"
  {
    static const char *const insns[][2] = {
      "prfb", "%0, [%1, %2.d, uxtw]",
      "prf<SVE_I:Vesize>", "%0, [%1, %2.d, uxtw %p4]"
    };
    const char *const *parts = insns[which_alternative];
    return aarch64_output_sve_prefetch (parts[0], operands[6], parts[1]);
  }
)

;; Compact active elements and pad with zeros.
(define_insn "@aarch64_sve_compact<mode>"
  [(set (match_operand:SVE_SD 0 "register_operand" "=w")
	(unspec:SVE_SD [(match_operand:<VPRED> 1 "register_operand" "Upl")
			(match_operand:SVE_SD 2 "register_operand" "w")]
		       UNSPEC_SVE_COMPACT))]
  "TARGET_SVE"
  "compact\t%0.<Vetype>, %1, %2.<Vetype>"
)

;; Like EXT, but start at the first active element.
(define_insn "@aarch64_sve_splice<mode>"
  [(set (match_operand:SVE_ALL 0 "register_operand" "=w, ?&w")
	(unspec:SVE_ALL [(match_operand:<VPRED> 1 "register_operand" "Upl, Upl")
			 (match_operand:SVE_ALL 2 "register_operand" "0, w")
			 (match_operand:SVE_ALL 3 "register_operand" "w, w")]
			UNSPEC_SVE_SPLICE))]
  "TARGET_SVE"
  "@
   splice\t%0.<Vetype>, %1, %0.<Vetype>, %3.<Vetype>
   movprfx\t%0, %2\;splice\t%0.<Vetype>, %1, %0.<Vetype>, %3.<Vetype>"
  [(set_attr "movprfx" "*, yes")]
)
