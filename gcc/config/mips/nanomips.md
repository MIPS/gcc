;; Copyright (C) 2018 Free Software Foundation, Inc.
;;
;; nanomips.md   Machine Description for the nanoMIPS instruction set
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

(define_enum "processor" [
  i7200
  nms1
])

(define_c_enum "unspec" [
  ;; Bit reverse operations
  UNSPEC_BITREVB
  UNSPEC_BITREVH
  UNSPEC_BITREVW
  UNSPEC_BYTEREVH
  UNSPEC_BYTEREVW
])

(include "i7200.md")
(include "nms1.md")

(include "mips.md")

; The MIPS DSP Instructions.

(include "mips-dsp.md")


; The MIPS DSP REV 2 Instructions.

(include "mips-dspr2.md")

;; Synchronization Instructions.

(include "sync.md")

;; nanoMIPS exclusive Instruction

(define_insn "mips_addsi3_48"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(plus:SI (match_operand:SI 1 "register_operand" "0")
		 (match_operand:SI 2 "s32_operand" "i")))]
  "ISA_HAS_ADDIU48"
  "addiu\t%0,%1,%2 # ADDIU48";
  [(set_attr "alu_type" "add")
   (set_attr "compression" "nanomips48")
   (set_attr "mode" "SI")])

(define_insn "*add<mode>3_nanomips"
  [(set (match_operand:GPR 0 "register_operand" "=!u,d,!u,!u,!ks,!d,d,d,kd")
	(plus:GPR (match_operand:GPR 1 "register_operand" "!u,d,!u,!ks,!ks,0,d,d,0")
		  (match_operand:GPR 2 "arith_operand_add" "!u,d,Uead,Uuw6,Uesp,Usb4,Unbc,Uubp,kd")))]
  "TARGET_NANOMIPS"
{
  if (which_alternative == 0
      || which_alternative == 1)
    return "<d>addu\t%0,%1,%2";
  else
    return "<d>addiu\t%0,%1,%2";
}
  [(set_attr "alu_type" "add")
   (set_attr "compression" "nanomips32,*,nanomips32,nanomips32,nanomips32,nanomips32,nanomips32,nanomips32,nanomips")
   (set_attr "has_16bit_ver" "yes,yes,yes,rri_add,yes,no,no,no,yes")
   (set_attr "subset_16bit" "std,std,std,std,std,no,no,no,4x4")
   (set_attr "mode" "<MODE>")
   (set (attr "enabled")
	(cond [(and (eq_attr "alternative" "4")
		    (match_test "TARGET_64BIT"))
		  (const_string "no")
	       (and (eq_attr "alternative" "8")
		    (match_test "TARGET_NANOMIPS == NANOMIPS_NMS"))
		  (const_string "no")]
	      (const_string "yes")))])

(define_insn "move_balc_call_value"
  [(parallel [(set (match_operand 4 "" "")
		   (call (mem:SI (match_operand 2 "" ""))
			 (match_operand 3 "" "")))
	      (set (match_operand 0 "register_operand" "")
		   (match_operand 1 "reg_or_0_operand" ""))
	      (use (match_dup 0))
	      (clobber (reg:SI RETURN_ADDR_REGNUM))])]
  "TARGET_NANOMIPS
   && nanomips_move_balc_p (operands)"
  { return mips_output_jump (operands, 2, -1, true, true); }
  [(set_attr "jal" "direct")])

(define_insn "move_balc_call"
  [(parallel [(call (mem:SI (match_operand 2 "" ""))
		    (match_operand 3 "" ""))
	      (set (match_operand 0 "register_operand" "")
		   (match_operand 1 "reg_or_0_operand" ""))
	      (use (match_dup 0))
	      (clobber (reg:SI RETURN_ADDR_REGNUM))])]
  "TARGET_NANOMIPS
   && nanomips_move_balc_p (operands)"
  { return mips_output_jump (operands, 2, -1, true, true); }
  [(set_attr "jal" "direct")])

;; For MOVEP.
(define_peephole2
  [(set (match_operand:MOVEP1 0 "register_operand" "")
	(match_operand:MOVEP1 1 "movep_or_0_operand" ""))
   (set (match_operand:MOVEP2 2 "register_operand" "")
	(match_operand:MOVEP2 3 "movep_or_0_operand" ""))]
  "ISA_HAS_MOVEP
   && mips_movep_no_overlap_p (operands[0], operands[2], operands[1],
      operands[3])
   && mips_movep_target_p (operands[0], operands[2])"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])])

;; The behavior of the MOVEP insn is undefined if placed in a delay slot.
(define_insn "*movep<MOVEP1:mode><MOVEP2:mode>"
  [(set (match_operand:MOVEP1 0 "register_operand")
	(match_operand:MOVEP1 1 "movep_or_0_operand"))
   (set (match_operand:MOVEP2 2 "register_operand")
	(match_operand:MOVEP2 3 "movep_or_0_operand"))]
  "TARGET_NANOMIPS
   && ISA_HAS_MOVEP
   && mips_movep_no_overlap_p (operands[0], operands[2], operands[1],
      operands[3])
   && mips_movep_target_p (operands[0], operands[2])"
{
  if (REGNO (operands[0]) < REGNO (operands[2]))
    return "movep\t%0,%2,%z1,%z3";
  else
    return "movep\t%2,%0,%z3,%z1";
}
  [(set_attr "type" "move_move")
   (set_attr "can_delay" "no")])

;; MOVEP reversed, the pair is now a source rather than destination
(define_peephole2
  [(set (match_operand:MOVEP1 0 "movep_rev_operand" "")
	(match_operand:MOVEP1 1 "register_operand" ""))
   (set (match_operand:MOVEP2 2 "movep_rev_operand" "")
	(match_operand:MOVEP2 3 "register_operand" ""))]
  "ISA_HAS_MOVEP_REV
   && mips_movep_no_overlap_p (operands[0], operands[2], operands[1],
      operands[3])
   && mips_movep_target_p (operands[1], operands[3])"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])])

(define_insn "*movep<MOVEP1:mode><MOVEP2:mode>_rev"
  [(set (match_operand:MOVEP1 0 "movep_rev_operand")
	(match_operand:MOVEP1 1 "register_operand"))
   (set (match_operand:MOVEP2 2 "movep_rev_operand")
	(match_operand:MOVEP2 3 "register_operand"))]
  "ISA_HAS_MOVEP_REV
   && mips_movep_no_overlap_p (operands[0], operands[2], operands[1],
      operands[3])
   && mips_movep_target_p (operands[1], operands[3])"
{
  if (REGNO (operands[1]) < REGNO (operands[3]))
    return "movep\t%0,%2,%z1,%z3";
  else
    return "movep\t%2,%0,%z3,%z1";
}
  [(set_attr "type" "move_move")
   (set_attr "can_delay" "no")])

(define_insn "*store_word_multiple8"
  [(match_parallel 0 "store_multiple_operation"
    [(set (match_operand:SI 1 "memory_operand" "=ZA")
	  (match_operand:SI 2 "register_operand" "r"))
     (set (match_operand:SI 3 "memory_operand" "=m")
	  (match_operand:SI 4 "register_operand" "r"))
     (set (match_operand:SI 5 "memory_operand" "=m")
	  (match_operand:SI 6 "register_operand" "r"))
     (set (match_operand:SI 7 "memory_operand" "=m")
	  (match_operand:SI 8 "register_operand" "r"))
     (set (match_operand:SI 9 "memory_operand" "=m")
	  (match_operand:SI 10 "register_operand" "r"))
     (set (match_operand:SI 11 "memory_operand" "=m")
	  (match_operand:SI 12 "register_operand" "r"))
     (set (match_operand:SI 13 "memory_operand" "=m")
	  (match_operand:SI 14 "register_operand" "r"))
     (set (match_operand:SI 15 "memory_operand" "=m")
	  (match_operand:SI 16 "register_operand" "r"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 8"
  { return mips_output_word_multiple (true, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*store_word_multiple7"
  [(match_parallel 0 "store_multiple_operation"
    [(set (match_operand:SI 1 "memory_operand" "=ZA")
	  (match_operand:SI 2 "register_operand" "r"))
     (set (match_operand:SI 3 "memory_operand" "=m")
	  (match_operand:SI 4 "register_operand" "r"))
     (set (match_operand:SI 5 "memory_operand" "=m")
	  (match_operand:SI 6 "register_operand" "r"))
     (set (match_operand:SI 7 "memory_operand" "=m")
	  (match_operand:SI 8 "register_operand" "r"))
     (set (match_operand:SI 9 "memory_operand" "=m")
	  (match_operand:SI 10 "register_operand" "r"))
     (set (match_operand:SI 11 "memory_operand" "=m")
	  (match_operand:SI 12 "register_operand" "r"))
     (set (match_operand:SI 13 "memory_operand" "=m")
	  (match_operand:SI 14 "register_operand" "r"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 7"
  { return mips_output_word_multiple (true, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*store_word_multiple6"
  [(match_parallel 0 "store_multiple_operation"
    [(set (match_operand:SI 1 "memory_operand" "=ZA")
	  (match_operand:SI 2 "register_operand" "r"))
     (set (match_operand:SI 3 "memory_operand" "=m")
	  (match_operand:SI 4 "register_operand" "r"))
     (set (match_operand:SI 5 "memory_operand" "=m")
	  (match_operand:SI 6 "register_operand" "r"))
     (set (match_operand:SI 7 "memory_operand" "=m")
	  (match_operand:SI 8 "register_operand" "r"))
     (set (match_operand:SI 9 "memory_operand" "=m")
	  (match_operand:SI 10 "register_operand" "r"))
     (set (match_operand:SI 11 "memory_operand" "=m")
	  (match_operand:SI 12 "register_operand" "r"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 6"
  { return mips_output_word_multiple (true, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*store_word_multiple5"
  [(match_parallel 0 "store_multiple_operation"
    [(set (match_operand:SI 1 "memory_operand" "=ZA")
	  (match_operand:SI 2 "register_operand" "r"))
     (set (match_operand:SI 3 "memory_operand" "=m")
	  (match_operand:SI 4 "register_operand" "r"))
     (set (match_operand:SI 5 "memory_operand" "=m")
	  (match_operand:SI 6 "register_operand" "r"))
     (set (match_operand:SI 7 "memory_operand" "=m")
	  (match_operand:SI 8 "register_operand" "r"))
     (set (match_operand:SI 9 "memory_operand" "=m")
	  (match_operand:SI 10 "register_operand" "r"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 5"
  { return mips_output_word_multiple (true, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*store_word_multiple4"
  [(match_parallel 0 "store_multiple_operation"
    [(set (match_operand:SI 1 "memory_operand" "=ZA")
	  (match_operand:SI 2 "register_operand" "r"))
     (set (match_operand:SI 3 "memory_operand" "=m")
	  (match_operand:SI 4 "register_operand" "r"))
     (set (match_operand:SI 5 "memory_operand" "=m")
	  (match_operand:SI 6 "register_operand" "r"))
     (set (match_operand:SI 7 "memory_operand" "=m")
	  (match_operand:SI 8 "register_operand" "r"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 4"
  { return mips_output_word_multiple (true, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*store_word_multiple3"
  [(match_parallel 0 "store_multiple_operation"
    [(set (match_operand:SI 1 "memory_operand" "=ZA")
	  (match_operand:SI 2 "register_operand" "r"))
     (set (match_operand:SI 3 "memory_operand" "=m")
	  (match_operand:SI 4 "register_operand" "r"))
     (set (match_operand:SI 5 "memory_operand" "=m")
	  (match_operand:SI 6 "register_operand" "r"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 3"
  { return mips_output_word_multiple (true, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*store_word_multiple2"
  [(match_parallel 0 "store_multiple_operation"
    [(set (match_operand:SI 1 "memory_operand" "=ZA")
	  (match_operand:SI 2 "register_operand" "r"))
     (set (match_operand:SI 3 "memory_operand" "=m")
	  (match_operand:SI 4 "register_operand" "r"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 2"
  { return mips_output_word_multiple (true, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_expand "store_multiple"
  [(match_par_dup 3 [(set (match_operand:SI 0 "" "")
			  (match_operand:SI 1 "" ""))
		     (use (match_operand:SI 2 "" ""))])]
  "ISA_HAS_NEW_LWM_SWM"
{
  int regno;
  int count;
  rtx to;
  rtx op0;
  int i;

  if (GET_CODE (operands[2]) != CONST_INT
      || INTVAL (operands[2]) > 8
      || INTVAL (operands[2]) < 2
      || GET_CODE (operands[0]) != MEM
      || MEM_VOLATILE_P (operands[0])
      || GET_CODE (operands[1]) != REG
      || !reload_completed)
    FAIL;

  count = INTVAL (operands[2]);
  regno = REGNO (operands[1]);

  operands[3] = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (count));
  to = force_reg (SImode, XEXP (operands[0], 0));
  op0 = replace_equiv_address (operands[0], to);

  XVECEXP (operands[3], 0, 0)
    = gen_rtx_SET (adjust_address_nv (op0, SImode, 0), operands[1]);

  for (i = 1; i < count; i++)
    XVECEXP (operands[3], 0, i)
      = gen_rtx_SET (adjust_address_nv (op0, SImode, i * 4),
		     gen_rtx_REG (SImode, regno + i));
})

(define_insn "*load_word_multiple8"
  [(match_parallel 0 "load_multiple_operation"
    [(set (match_operand:SI 1 "register_operand" "=&r")
	  (match_operand:SI 2 "memory_operand" "ZA"))
     (set (match_operand:SI 3 "register_operand" "=&r")
	  (match_operand:SI 4 "memory_operand" "m"))
     (set (match_operand:SI 5 "register_operand" "=&r")
	  (match_operand:SI 6 "memory_operand" "m"))
     (set (match_operand:SI 7 "register_operand" "=&r")
	  (match_operand:SI 8 "memory_operand" "m"))
     (set (match_operand:SI 9 "register_operand" "=&r")
	  (match_operand:SI 10 "memory_operand" "m"))
     (set (match_operand:SI 11 "register_operand" "=&r")
	  (match_operand:SI 12 "memory_operand" "m"))
     (set (match_operand:SI 13 "register_operand" "=&r")
	  (match_operand:SI 14 "memory_operand" "m"))
     (set (match_operand:SI 15 "register_operand" "=r")
	  (match_operand:SI 16 "memory_operand" "m"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 8"
  { return mips_output_word_multiple (false, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*load_word_multiple7"
  [(match_parallel 0 "load_multiple_operation"
    [(set (match_operand:SI 1 "register_operand" "=&r")
	  (match_operand:SI 2 "memory_operand" "ZA"))
     (set (match_operand:SI 3 "register_operand" "=&r")
	  (match_operand:SI 4 "memory_operand" "m"))
     (set (match_operand:SI 5 "register_operand" "=&r")
	  (match_operand:SI 6 "memory_operand" "m"))
     (set (match_operand:SI 7 "register_operand" "=&r")
	  (match_operand:SI 8 "memory_operand" "m"))
     (set (match_operand:SI 9 "register_operand" "=&r")
	  (match_operand:SI 10 "memory_operand" "m"))
     (set (match_operand:SI 11 "register_operand" "=&r")
	  (match_operand:SI 12 "memory_operand" "m"))
     (set (match_operand:SI 13 "register_operand" "=r")
	  (match_operand:SI 14 "memory_operand" "m"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 7"
  { return mips_output_word_multiple (false, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*load_word_multiple6"
  [(match_parallel 0 "load_multiple_operation"
    [(set (match_operand:SI 1 "register_operand" "=&r")
	  (match_operand:SI 2 "memory_operand" "ZA"))
     (set (match_operand:SI 3 "register_operand" "=&r")
	  (match_operand:SI 4 "memory_operand" "m"))
     (set (match_operand:SI 5 "register_operand" "=&r")
	  (match_operand:SI 6 "memory_operand" "m"))
     (set (match_operand:SI 7 "register_operand" "=&r")
	  (match_operand:SI 8 "memory_operand" "m"))
     (set (match_operand:SI 9 "register_operand" "=&r")
	  (match_operand:SI 10 "memory_operand" "m"))
     (set (match_operand:SI 11 "register_operand" "=r")
	  (match_operand:SI 12 "memory_operand" "m"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 6"
  { return mips_output_word_multiple (false, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*load_word_multiple5"
  [(match_parallel 0 "load_multiple_operation"
    [(set (match_operand:SI 1 "register_operand" "=&r")
	  (match_operand:SI 2 "memory_operand" "ZA"))
     (set (match_operand:SI 3 "register_operand" "=&r")
	  (match_operand:SI 4 "memory_operand" "m"))
     (set (match_operand:SI 5 "register_operand" "=&r")
	  (match_operand:SI 6 "memory_operand" "m"))
     (set (match_operand:SI 7 "register_operand" "=&r")
	  (match_operand:SI 8 "memory_operand" "m"))
     (set (match_operand:SI 9 "register_operand" "=r")
	  (match_operand:SI 10 "memory_operand" "m"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 5"
  { return mips_output_word_multiple (false, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*load_word_multiple4"
  [(match_parallel 0 "load_multiple_operation"
    [(set (match_operand:SI 1 "register_operand" "=&r")
	  (match_operand:SI 2 "memory_operand" "ZA"))
     (set (match_operand:SI 3 "register_operand" "=&r")
	  (match_operand:SI 4 "memory_operand" "m"))
     (set (match_operand:SI 5 "register_operand" "=&r")
	  (match_operand:SI 6 "memory_operand" "m"))
     (set (match_operand:SI 7 "register_operand" "=r")
	  (match_operand:SI 8 "memory_operand" "m"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 4"
  { return mips_output_word_multiple (false, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*load_word_multiple3"
  [(match_parallel 0 "load_multiple_operation"
    [(set (match_operand:SI 1 "register_operand" "=&r")
	  (match_operand:SI 2 "memory_operand" "ZA"))
     (set (match_operand:SI 3 "register_operand" "=&r")
	  (match_operand:SI 4 "memory_operand" "m"))
     (set (match_operand:SI 5 "register_operand" "=r")
	  (match_operand:SI 6 "memory_operand" "m"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 3"
  { return mips_output_word_multiple (false, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*load_word_multiple2"
  [(match_parallel 0 "load_multiple_operation"
    [(set (match_operand:SI 1 "register_operand" "=&r")
	  (match_operand:SI 2 "memory_operand" "ZA"))
     (set (match_operand:SI 3 "register_operand" "=r")
	  (match_operand:SI 4 "memory_operand" "m"))])]
  "ISA_HAS_NEW_LWM_SWM && XVECLEN (operands[0], 0) == 2"
  { return mips_output_word_multiple (false, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_expand "load_multiple"
  [(match_par_dup 3 [(set (match_operand:SI 0 "" "")
			  (match_operand:SI 1 "" ""))
		     (use (match_operand:SI 2 "" ""))])]
  "ISA_HAS_NEW_LWM_SWM"
{
  int regno;
  int count;
  rtx op1;
  int i;

  if (GET_CODE (operands[2]) != CONST_INT
      || INTVAL (operands[2]) > 8
      || INTVAL (operands[2]) < 2
      || GET_CODE (operands[1]) != MEM
      || MEM_VOLATILE_P (operands[1])
      || GET_CODE (operands[0]) != REG
      || !reload_completed)
    FAIL;

  count = INTVAL (operands[2]);
  regno = REGNO (operands[0]);

  operands[3] = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (count));
  op1 = replace_equiv_address (operands[1],
			       force_reg (SImode, XEXP (operands[1], 0)));

  for (i = 0; i < count; i++)
    XVECEXP (operands[3], 0, i)
      = gen_rtx_SET (gen_rtx_REG (SImode, regno + i),
		     adjust_address_nv (op1, SImode, i * 4));
})

;; Operand 0 is the register containing the destination address
;; Operand 1 is the register containing the source address
;; Operand 2 is the register containing the number of words to move

(define_expand "movmemsi_multireg"
  [(parallel
    [(set (match_operand 0 "" "")
	  (match_operand 1 "" ""))
     (use (match_operand 2 "" ""))])]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
{
  gcc_assert (TARGET_NANOMIPS == NANOMIPS_NMF
	      && IN_RANGE (INTVAL (operands[2]), 1, 8));
  nanomips_expand_movmemsi_multireg (operands[0], operands[1],
				     INTVAL (operands[2]));
  DONE;
})

(define_insn_and_split "load_store_multiple8"
  [(set (match_operand:BLK 0 "memory_operand" "=ZA")
	(match_operand:BLK 1 "memory_operand" "ZA"))
   (clobber (reg:SI 12))
   (clobber (reg:SI 13))
   (clobber (reg:SI 14))
   (clobber (reg:SI 15))
   (clobber (reg:SI 16))
   (clobber (reg:SI 17))
   (clobber (reg:SI 18))
   (clobber (reg:SI 19))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  nanomips_load_store_multiple_split (operands[0], operands[1], 8);
  DONE;
}
 [(set_attr "move_type" "store")
  (set_attr "mode" "SI")])

(define_insn_and_split "load_store_multiple7"
  [(set (match_operand:BLK 0 "memory_operand" "=ZA")
	(match_operand:BLK 1 "memory_operand" "ZA"))
   (clobber (reg:SI 12))
   (clobber (reg:SI 13))
   (clobber (reg:SI 14))
   (clobber (reg:SI 15))
   (clobber (reg:SI 16))
   (clobber (reg:SI 17))
   (clobber (reg:SI 18))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  nanomips_load_store_multiple_split (operands[0], operands[1], 7);
  DONE;
}
 [(set_attr "move_type" "store")
  (set_attr "mode" "SI")])

(define_insn_and_split "load_store_multiple6"
  [(set (match_operand:BLK 0 "memory_operand" "=ZA")
	(match_operand:BLK 1 "memory_operand" "ZA"))
   (clobber (reg:SI 12))
   (clobber (reg:SI 13))
   (clobber (reg:SI 14))
   (clobber (reg:SI 15))
   (clobber (reg:SI 16))
   (clobber (reg:SI 17))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  nanomips_load_store_multiple_split (operands[0], operands[1], 6);
  DONE;
}
 [(set_attr "move_type" "store")
  (set_attr "mode" "SI")])

(define_insn_and_split "load_store_multiple5"
  [(set (match_operand:BLK 0 "memory_operand" "=ZA")
	(match_operand:BLK 1 "memory_operand" "ZA"))
   (clobber (reg:SI 12))
   (clobber (reg:SI 13))
   (clobber (reg:SI 14))
   (clobber (reg:SI 15))
   (clobber (reg:SI 16))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  nanomips_load_store_multiple_split (operands[0], operands[1], 5);
  DONE;
}
 [(set_attr "move_type" "store")
  (set_attr "mode" "SI")])

(define_insn_and_split "load_store_multiple4"
  [(set (match_operand:BLK 0 "memory_operand" "=ZA")
	(match_operand:BLK 1 "memory_operand" "ZA"))
   (clobber (reg:SI 12))
   (clobber (reg:SI 13))
   (clobber (reg:SI 14))
   (clobber (reg:SI 15))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  nanomips_load_store_multiple_split (operands[0], operands[1], 4);
  DONE;
}
 [(set_attr "move_type" "store")
  (set_attr "mode" "SI")])

(define_insn_and_split "load_store_multiple3"
  [(set (match_operand:BLK 0 "memory_operand" "=ZA")
	(match_operand:BLK 1 "memory_operand" "ZA"))
   (clobber (reg:SI 12))
   (clobber (reg:SI 13))
   (clobber (reg:SI 14))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  nanomips_load_store_multiple_split (operands[0], operands[1], 3);
  DONE;
}
 [(set_attr "move_type" "store")
  (set_attr "mode" "SI")])

(define_insn_and_split "load_store_multiple2"
  [(set (match_operand:BLK 0 "memory_operand" "=ZA")
	(match_operand:BLK 1 "memory_operand" "ZA"))
   (clobber (reg:SI 12))
   (clobber (reg:SI 13))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  nanomips_load_store_multiple_split (operands[0], operands[1], 2);
  DONE;
}
 [(set_attr "move_type" "store")
  (set_attr "mode" "SI")])

;; Load word pair as LWM.
(define_peephole2
  [(set (match_operand:SI 0 "d_operand" "")
	(match_operand:SI 1 "non_volatile_mem_operand" ""))
   (set (match_operand:SI 2 "d_operand" "")
	(match_operand:SI 3 "non_volatile_mem_operand" ""))]
  "ISA_HAS_NEW_LWM_SWM && umips_load_store_pair_p (true, operands) == 1"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])])

(define_peephole2
  [(set (match_operand:SI 0 "d_operand" "")
	(match_operand:SI 1 "non_volatile_mem_operand" ""))
   (set (match_operand:SI 2 "d_operand" "")
	(match_operand:SI 3 "non_volatile_mem_operand" ""))]
  "ISA_HAS_NEW_LWM_SWM && umips_load_store_pair_p (true, operands) == 2"
  [(parallel [(set (match_dup 2) (match_dup 3))
	      (set (match_dup 0) (match_dup 1))])])

;; The behavior of the LWP insn is undefined if placed in a delay slot.
(define_insn "*lwp"
  [(parallel [(set (match_operand:SI 0 "register_operand" "=&d")
		   (match_operand:SI 1 "non_volatile_mem_operand" "ZO"))
	      (set (match_operand:SI 2 "register_operand" "=d")
		   (match_operand:SI 3 "non_volatile_mem_operand" "m"))])]

  "ISA_HAS_NEW_LWM_SWM && umips_load_store_pair_p (true, operands)"
{
  umips_output_load_store_pair (true, operands);
  return "";
}
  [(set_attr "type" "load")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

;; Store load pair as SWM.
(define_peephole2
  [(set (match_operand:SI 0 "non_volatile_mem_operand" "")
	(match_operand:SI 1 "d_operand" ""))
   (set (match_operand:SI 2 "non_volatile_mem_operand" "")
	(match_operand:SI 3 "d_operand" ""))]
  "ISA_HAS_NEW_LWM_SWM && umips_load_store_pair_p (false, operands) == 1"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])])

(define_peephole2
  [(set (match_operand:SI 0 "non_volatile_mem_operand" "")
	(match_operand:SI 1 "d_operand" ""))
   (set (match_operand:SI 2 "non_volatile_mem_operand" "")
	(match_operand:SI 3 "d_operand" ""))]
  "ISA_HAS_NEW_LWM_SWM && umips_load_store_pair_p (false, operands) == 2"
  [(parallel [(set (match_dup 2) (match_dup 3))
	      (set (match_dup 0) (match_dup 1))])])

;; The behavior of the SWP insn is undefined if placed in a delay slot.
(define_insn "*swp"
  [(set (match_operand:SI 0 "non_volatile_mem_operand" "=ZO")
	(match_operand:SI 1 "register_operand" "d"))
   (set (match_operand:SI 2 "non_volatile_mem_operand" "=m")
	(match_operand:SI 3 "register_operand" "d"))]
  "ISA_HAS_NEW_LWM_SWM && umips_load_store_pair_p (false, operands)"
{
  umips_output_load_store_pair (false, operands);
  return "";
}
  [(set_attr "type" "store")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn_and_split "ctzsi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ctz:SI (match_operand:SI 1 "register_operand" "r")))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "#"
  "&& reload_completed"
  [(set (match_dup 0) (unspec:SI [(match_dup 1)] UNSPEC_BITREVW))
   (set (match_dup 0) (clz:SI (match_dup 0)))]
  ""
  [(set_attr "type" "clz")
   (set_attr "mode" "SI")
   (set_attr "insn_count" "2")])

(define_insn "mips_align"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (match_operand:SI 2 "register_operand" "r")
		    (match_operand:SI 3 "const_0_to_3_operand" "")]
		    UNSPEC_ALIGN))]
  "TARGET_NANOMIPS"
  "align\t%0,%1,%2,%3"
  [(set_attr "type" "arith")
   (set_attr "mode" "SI")])

(define_insn "mips_bitrevb"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")]
		    UNSPEC_BITREVB))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "bitrevb\t%0,%1"
  [(set_attr "type" "shift")
   (set_attr "mode" "SI")])

(define_insn "mips_bitrevh"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")]
		    UNSPEC_BITREVH))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "bitrevh\t%0,%1"
  [(set_attr "type" "shift")
   (set_attr "mode" "SI")])

(define_insn "mips_bitrevw"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")]
		    UNSPEC_BITREVW))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "bitrevw\t%0,%1"
  [(set_attr "type" "shift")
   (set_attr "mode" "SI")])

(define_insn "mips_byterevh"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")]
		    UNSPEC_BYTEREVH))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "byterevh\t%0,%1"
  [(set_attr "type" "shift")
   (set_attr "mode" "SI")])

(define_insn "mips_byterevw"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")]
		    UNSPEC_BYTEREVW))]
  "TARGET_NANOMIPS == NANOMIPS_NMF"
  "byterevw\t%0,%1"
  [(set_attr "type" "shift")
   (set_attr "mode" "SI")])

(define_c_enum "unspec" [
  UNSPEC_ADDRESS_FIRST
])
