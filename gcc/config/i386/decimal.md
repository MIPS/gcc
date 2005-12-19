;; Decimal floating point patterns.
;; Copyright (C) 2005 Free Software Foundation, Inc.

(define_expand "movsd"
  [(set (match_operand:SD 0 "nonimmediate_operand" "")
	(match_operand:SD 1 "general_operand" ""))]
  "ENABLE_DECIMAL_FLOAT"
  "ix86_expand_move (SDmode, operands); DONE;")

(define_expand "movdd"
  [(set (match_operand:DD 0 "nonimmediate_operand" "")
	(match_operand:DD 1 "general_operand" ""))]
  "ENABLE_DECIMAL_FLOAT"
  "ix86_expand_move (DDmode, operands); DONE;")

(define_expand "movtd"
  [(set (match_operand:TD 0 "nonimmediate_operand" "")
	(match_operand:TD 1 "general_operand" ""))]
  "ENABLE_DECIMAL_FLOAT"
  "ix86_expand_move (TDmode, operands); DONE;")

(define_insn "*pushsd2"
  [(set (match_operand:SD 0 "push_operand" "=<")
	(match_operand:SD 1 "general_no_elim_operand" "ri*m"))]
  "!TARGET_64BIT"
  "push{l}\t%1"
  [(set_attr "type" "push")
   (set_attr "mode" "SD")])

(define_insn "*movsd_constant"
  [(set (match_operand:SD 0 "nonimmediate_operand" "=r,m")
	(match_operand:SD 1 "const_double_operand" "i,i"))
   (clobber (reg:CC FLAGS_REG))]
  "ENABLE_DECIMAL_FLOAT"
  "mov{l}\t{%1, %0|%0, %1}"
  [(set_attr "type" "alu1")
   (set_attr "mode" "SD")
   (set_attr "length_immediate" "0")])

(define_insn "*movsd_gprs"
  [(set (match_operand:SD 0 "nonimmediate_operand" "=r,m,r")
	(match_operand:SD 1 "general_operand" "r,r,m"))]
  "ENABLE_DECIMAL_FLOAT"
{
   /* Same output for every alternative.  */
   return "mov\t{%1, %0|%0, %1}";
}
  [(set_attr "type" "imov")
   (set_attr "mode" "SD")])

(define_insn "*movdd_sse"
  [(set (match_operand:DD 0 "nonimmediate_operand" "=x,m,x")
	(match_operand:DD 1 "general_operand" "x,x,m"))]
  "ENABLE_DECIMAL_FLOAT && TARGET_SSE"
{
  switch (which_alternative)
    {
    case 0:
      return "movaps\t{%1, %0|%0, %1}";
    default:
      return "movlps\t{%1, %0|%0, %1}";
    }
}
  [(set_attr "type" "ssemov")
   (set_attr "mode" "DD")])

(define_insn "*movtd_sse"
  [(set (match_operand:TD 0 "nonimmediate_operand" "=x,m,x")
	(match_operand:TD 1 "general_operand" "x,x,m"))]
  "ENABLE_DECIMAL_FLOAT && TARGET_SSE"
{
  /* Same output for every alternative.  */
  return "movaps\t{%1, %0|%0, %1}";
}
  [(set_attr "type" "ssemov")
   (set_attr "mode" "TD")])
