;; GCC midlevel RTL description
;; Copyright (C) 2002 Free Software Foundation, Inc.
;; Contributed by Jan Hubicka
;;
;; This file is part of GNU CC.
;;
;; GNU CC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.
;;
;; GNU CC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GNU CC; see the file COPYING.  If not, write to
;; the Free Software Foundation, 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

(define_insn_and_split "_generic_move"
  [(set (match_operand 0 "generic_dest_operand" "")
        (match_operand 1 "generic_src_operand" ""))]
  "cfun->rtl_form >= RTL_FORM_MID"
  "#"
  "cfun->rtl_form == RTL_FORM_MIDLOW"
  [(const_int 0)]
{
  emit_move_insn (operands[0], operands[1]);
  DONE;
})

(define_insn_and_split "generic_unary_operation"
  [(set (match_operand 0 "generic_dest_operand" "")
	(match_operator 2 "generic_unary_operator"
	   [(match_operand 1 "generic_src_operand" "")]))]
  "cfun->rtl_form >= RTL_FORM_MID
   && !COMPLEX_MODE_P (GET_MODE (operands[0]))"
  "#"
  "cfun->rtl_form == RTL_FORM_MIDLOW"
  [(const_int 0)]
{
  rtx res;
  res = expand_simple_unop (GET_MODE (operands[0]), GET_CODE (operands[2]),
			    operands[1], operands[0], 1);
  if (!res)
    abort ();
  if (res != operands[0])
    emit_move_insn (operands[0], res);
  DONE;
})

(define_insn_and_split "_generic_binary_operation"
  [(set (match_operand 0 "generic_dest_operand" "")
	(match_operator 3 "generic_binary_operator"
	   [(match_operand 1 "generic_src_operand" "")
	    (match_operand 2 "generic_src_operand" "")]))]
  "cfun->rtl_form >= RTL_FORM_MID
   && !COMPLEX_MODE_P (GET_MODE (operands[0]))"
  "#"
  "cfun->rtl_form == RTL_FORM_MIDLOW"
  [(const_int 0)]
{
  rtx res;

  res = expand_generic_binop (GET_CODE (operands[3]), operands[1],
			      operands[2], operands[0]);
  if (!res)
    abort ();
  if (res != operands[0])
    emit_move_insn (operands[0], res);
  DONE;
})

(define_insn_and_split "_generic_simplejump"
  [(set (pc) (label_ref (match_operand 0 "" "")))]
  "cfun->rtl_form >= RTL_FORM_MID"
  "#"
  "cfun->rtl_form == RTL_FORM_MIDLOW"
  [(const_int 0)]
{
  emit_insn (gen_jump (operands[0]));
  DONE;
})

(define_insn_and_split "_generic_indirectjump"
  [(set (pc) (match_operand 0 "generic_src_operand" ""))]
  "cfun->rtl_form >= RTL_FORM_MID"
  "#"
  "cfun->rtl_form == RTL_FORM_MIDLOW"
  [(const_int 0)]
{
  emit_insn (gen_indirect_jump (operands[0]));
  DONE;
})

(define_insn_and_split "_generic_condjump"
  [(set (pc)
	(if_then_else (match_operator 3 "comparison_operator"
			 [(match_operand 0 "generic_src_operand" "")
			  (match_operand 1 "generic_src_operand" "")])
		      (label_ref (match_operand 2 "" ""))
		      (pc)))]
  "cfun->rtl_form >= RTL_FORM_MID && GET_MODE (operands[0]) != VOIDmode
   && !COMPLEX_MODE_P (GET_MODE (operands[0]))"
  "#"
  "cfun->rtl_form == RTL_FORM_MIDLOW"
  [(const_int 0)]
{
  emit_cmp_and_jump_insns (operands[0], operands[1], GET_CODE (operands[3]),
			   NULL_RTX, GET_MODE (operands[0]),
			   INTEGRAL_MODE_P (GET_MODE (operands[0]))
			   && (unsigned_condition (GET_CODE (operands[3]))
			       == GET_CODE (operands[3])),
			   operands[2]);
  DONE;
})
