;; Copyright (C) 2013-2016 Free Software Foundation, Inc.
;;
;; micromips.md   Machine Description for the microMIPS instruction set
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

(define_insn "*store_word_multiple"
  [(match_parallel 0 ""
       [(set (match_operand:SI 1 "memory_operand")
	     (match_operand:SI 2 "register_operand"))])]
  "ISA_HAS_LWM_SWM && umips_word_multiple_pattern_p (true, operands[0])"
  { return umips_output_word_multiple (true, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

(define_insn "*load_word_multiple"
  [(match_parallel 0 ""
       [(set (match_operand:SI 1 "register_operand")
	     (match_operand:SI 2 "memory_operand"))])]
  "ISA_HAS_LWM_SWM && umips_word_multiple_pattern_p (false, operands[0])"
  { return umips_output_word_multiple (false, operands[0]); }
  [(set_attr "type" "multimem")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

;; For LWP.
(define_peephole2
  [(set (match_operand:SI 0 "d_operand" "")
        (match_operand:SI 1 "non_volatile_mem_operand" ""))
   (set (match_operand:SI 2 "d_operand" "")
        (match_operand:SI 3 "non_volatile_mem_operand" ""))]
  "ISA_HAS_LWP_SWP && umips_load_store_pair_p (true, operands)"
  [(parallel [(set (match_dup 0) (match_dup 1))
              (set (match_dup 2) (match_dup 3))])])

;; The behavior of the LWP insn is undefined if placed in a delay slot.
(define_insn "*lwp"
  [(parallel [(set (match_operand:SI 0 "d_operand")
		   (match_operand:SI 1 "non_volatile_mem_operand"))
	      (set (match_operand:SI 2 "d_operand")
		   (match_operand:SI 3 "non_volatile_mem_operand"))])]

  "ISA_HAS_LWP_SWP && umips_load_store_pair_p (true, operands)"
{
  umips_output_load_store_pair (true, operands);
  return "";
}
  [(set_attr "type" "load")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

;; For SWP.
(define_peephole2
  [(set (match_operand:SI 0 "non_volatile_mem_operand" "")
        (match_operand:SI 1 "d_operand" ""))
   (set (match_operand:SI 2 "non_volatile_mem_operand" "")
        (match_operand:SI 3 "d_operand" ""))]
  "ISA_HAS_LWP_SWP && umips_load_store_pair_p (false, operands)"
  [(parallel [(set (match_dup 0) (match_dup 1))
              (set (match_dup 2) (match_dup 3))])])

;; The behavior of the SWP insn is undefined if placed in a delay slot.
(define_insn "*swp"
  [(set (match_operand:SI 0 "non_volatile_mem_operand")
	(match_operand:SI 1 "d_operand"))
   (set (match_operand:SI 2 "non_volatile_mem_operand")
	(match_operand:SI 3 "d_operand"))]
  "ISA_HAS_LWP_SWP && umips_load_store_pair_p (false, operands)"
{
  umips_output_load_store_pair (false, operands);
  return "";
}
  [(set_attr "type" "store")
   (set_attr "mode" "SI")
   (set_attr "can_delay" "no")])

;; For JRADDIUSP.
(define_insn "jraddiusp"
  [(return)
   (use (reg:SI 31))
   (set (reg:SI 29)
	(plus:SI (reg:SI 29)
		 (match_operand 0 "uw5_operand")))]
  "ISA_HAS_JRADDIUSP"
  "jraddiusp\t%0"
  [(set_attr "type"	"trap")
   (set_attr "can_delay" "no")
   (set_attr "mode"	"SI")])

;; For MOVEP.
(define_peephole2
  [(set (match_operand 0 "register_operand" "")
	(match_operand 1 "movep_operand" ""))
   (set (match_operand 2 "register_operand" "")
	(match_operand 3 "movep_operand" ""))]
  "TARGET_MICROMIPS
   && umips_movep_no_overlap_p (operands[0], operands[2], operands[1],
      operands[3])
   && umips_movep_target_p (operands[0], operands[2])"
  [(parallel [(set (match_dup 0) (match_dup 1))
              (set (match_dup 2) (match_dup 3))])])

;; The behavior of the MOVEP insn is undefined if placed in a delay slot.
(define_insn "*movep"
  [(set (match_operand 0 "register_operand")
	(match_operand 1 "movep_operand"))
   (set (match_operand 2 "register_operand")
	(match_operand 3 "movep_operand"))]
  "TARGET_MICROMIPS
   && umips_movep_no_overlap_p (operands[0], operands[2], operands[1],
      operands[3])
   && umips_movep_target_p (operands[0], operands[2])"
{
  if (REGNO (operands[0]) < REGNO (operands[2]))
    return "movep\t%0,%2,%z1,%z3";
  else
    return "movep\t%2,%0,%z3,%z1";
}
  [(set_attr "type" "move")
   (set_attr "can_delay" "no")])

;; MOVEP reversed, the pair is now a source rather than destination
(define_peephole2
  [(set (match_operand:MOVEP1 0 "movep_rev_operand" "")
	(match_operand:MOVEP1 1 "register_operand" ""))
   (set (match_operand:MOVEP2 2 "movep_rev_operand" "")
	(match_operand:MOVEP2 3 "register_operand" ""))]
  "TARGET_MICROMIPS_R7
   && umips_movep_no_overlap_p (operands[0], operands[2], operands[1],
      operands[3])
    && umips_movep_target_p (operands[1], operands[3])"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])])

(define_insn "*movep<MOVEP1:mode><MOVEP2:mode>_rev"
  [(parallel [(set (match_operand:MOVEP1 0 "movep_rev_operand")
		   (match_operand:MOVEP1 1 "register_operand"))
	      (set (match_operand:MOVEP2 2 "movep_rev_operand")
		   (match_operand:MOVEP2 3 "register_operand"))])]
  "TARGET_MICROMIPS_R7
   && umips_movep_no_overlap_p (operands[0], operands[2], operands[1],
      operands[3])
   && umips_movep_target_p (operands[1], operands[3])"
{
  if (REGNO (operands[1]) < REGNO (operands[3]))
    return "movep\t%0,%2,%z1,%z3";
  else
    return "movep\t%2,%0,%z3,%z1";
}
  [(set_attr "type" "move")
   (set_attr "mode" "<MODE>")
   (set_attr "can_delay" "no")])

(define_peephole2
  [(set (match_operand 0 "register_operand")
	(match_operand 1 "register_operand"))
   (parallel [(set (match_operand 4 "register_operand")
		   (call (mem:SI (match_operand 2 ""))
			 (match_operand 3 "" "")))
	      (clobber (reg:SI RETURN_ADDR_REGNUM))])]
  "TARGET_MICROMIPS_R7
   && umips_move_balc_p (operands)"
  [(const_int 0)]
{
  rtx move_balc = gen_move_balc_call_value (operands[0], operands[1],
					    operands[2], operands[3],
					    operands[4]);
  rtx_insn *insn = emit_call_insn (move_balc);

  rtx note = find_reg_note (NEXT_INSN (curr_insn), REG_EH_REGION, NULL_RTX);
  if (note != NULL_RTX)
    add_reg_note (insn, REG_EH_REGION, XEXP (note, 0));

  note = find_reg_note (NEXT_INSN (curr_insn), REG_CALL_DECL, NULL_RTX);
  if (note != NULL_RTX)
    add_reg_note (insn, REG_CALL_DECL, XEXP (note, 0));

  DONE;
})

(define_peephole2
  [(set (match_operand 0 "register_operand")
	(match_operand 1 "register_operand"))
   (parallel [(call (mem:SI (match_operand 2 ""))
			 (match_operand 3 "" ""))
	      (clobber (reg:SI RETURN_ADDR_REGNUM))])]
  "TARGET_MICROMIPS_R7
   && umips_move_balc_p (operands)"
  [(const_int 0)]
{
  rtx move_balc = gen_move_balc_call (operands[0], operands[1],
				      operands[2], operands[3]);
  rtx_insn *insn = emit_call_insn (move_balc);

  rtx note = find_reg_note (NEXT_INSN (curr_insn), REG_EH_REGION, NULL_RTX);
  if (note != NULL_RTX)
    add_reg_note (insn, REG_EH_REGION, XEXP (note, 0));

  note = find_reg_note (NEXT_INSN (curr_insn), REG_CALL_DECL, NULL_RTX);
  if (note != NULL_RTX)
    add_reg_note (insn, REG_CALL_DECL, XEXP (note, 0));

  DONE;
})

(define_insn "move_balc_call_value"
  [(parallel [(set (match_operand 4 "" "")
		   (call (mem:SI (match_operand 2 "" ""))
			 (match_operand 3 "" "")))
	      (set (match_operand 0 "register_operand" "")
		   (match_operand 1 "reg_or_0_operand" ""))
	      (use (match_dup 0))
	      (clobber (reg:SI RETURN_ADDR_REGNUM))])]
  "TARGET_MICROMIPS_R7
   && umips_move_balc_p (operands)"
  {
    return mips_output_jump (operands, 2, -1, true, true);
  }
  [(set_attr "jal" "direct")])

(define_insn "move_balc_call"
  [(parallel [(call (mem:SI (match_operand 2 "" ""))
		    (match_operand 3 "" ""))
	      (set (match_operand 0 "register_operand" "")
		   (match_operand 1 "reg_or_0_operand" ""))
	      (use (match_dup 0))
	      (clobber (reg:SI RETURN_ADDR_REGNUM))])]
  "TARGET_MICROMIPS_R7
   && umips_move_balc_p (operands)"
  {
    return mips_output_jump (operands, 2, -1, true, true);
  }
  [(set_attr "jal" "direct")])
