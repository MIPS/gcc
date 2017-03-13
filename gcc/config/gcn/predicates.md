;; Predicate definitions for GCN.
;; Copyright (C) 2016-2017 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.
;; Return true if VALUE can be stored in a sign extended immediate field.

(define_predicate "gcn_16bit_immediate_operand"
  (and (match_code "const_int")
	(match_test "satisfies_constraint_I (op)")))

(define_predicate "gcn_conditional_register_operand"
  (match_operand 0 "register_operand")
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  if (!REG_P (op))
    return 0;

  return REGNO (op) == VCCZ_REG
	 || REGNO (op) == SCC_REG
	 || REGNO (op) == EXECZ_REG
	 || REGNO (op) >= FIRST_PSEUDO_REGISTER;
})

(define_predicate "gcn_sgpr_register_operand"
  (match_operand 0 "register_operand")
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  if (!REG_P (op))
    return 0;

  return REGNO (op) < FIRST_PSEUDO_REGISTER && !VGPR_REGNO_P (REGNO (op));
})

(define_predicate "gcn_simple_mem_or_reg_operand"
  (match_operand 0 "nonimmediate_operand")
{
  if (GET_CODE (op) == MEM
      && GET_CODE (XEXP (op, 0)) != REG)
    return false;
  return true;
})

(define_predicate "gcn_vgpr_register_operand"
  (match_operand 0 "register_operand")
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  if (!REG_P (op))
    return 0;

  return VGPR_REGNO_P (REGNO (op));
})

(define_predicate "gcn_inline_immediate_operand"
  (match_code "const_int,const_double,const_vector")
{
  return gcn_inline_constant_p (op);
})

(define_predicate "gcn_vec0_operand"
  (match_code "const_vector")
{
  return CONST_VECTOR_ELT (op, 0) == const0_rtx && gcn_inline_constant_p (op);
})

(define_predicate "gcn_vec1_operand"
  (match_code "const_vector")
{
  return CONST_VECTOR_ELT (op, 0) == const1_rtx && gcn_inline_constant_p (op);
})

(define_predicate "gcn_32bit_immediate_operand"
  (match_code "const_int,const_double,const_vector")
{
  return gcn_constant_p (op);
})

; LRA works smoother when exec values are immediate constants
; prior register allocation.
(define_predicate "gcn_exec_operand"
  (ior (match_operand 0 "register_operand")
       (match_code "const_int")))

(define_predicate "gcn_exec_reg_operand"
  (match_operand 0 "register_operand"))

(define_predicate "gcn_load_operand"
  (ior (match_operand 0 "nonimmediate_operand")
       (match_operand 0 "gcn_32bit_immediate_operand")))

(define_predicate "gcn_alu_operand"
  (ior (match_operand 0 "register_operand")
       (match_operand 0 "gcn_32bit_immediate_operand")))

(define_predicate "gcn_ds_memory_operand"
  (and (match_code "mem")
       (and (match_test "MEM_ADDR_SPACE (op) == ADDR_SPACE_LDS || MEM_ADDR_SPACE (op) == ADDR_SPACE_GDS")
	    (match_operand 0 "memory_operand"))))

(define_predicate "gcn_valu_dst_operand"
  (ior (match_operand 0 "register_operand")
       (match_operand 0 "gcn_ds_memory_operand")))

(define_predicate "gcn_valu_src0_operand"
  (ior (match_operand 0 "register_operand")
       (ior (match_operand 0 "gcn_32bit_immediate_operand")
	    (match_operand 0 "gcn_ds_memory_operand"))))

(define_predicate "gcn_valu_src1_operand"
  (match_operand 0 "register_operand"))

(define_predicate "gcn_valu_src1com_operand"
  (ior (match_operand 0 "register_operand")
       (match_operand 0 "gcn_32bit_immediate_operand")))

(define_predicate "gcn_conditional_operator"
  (match_code "eq,ne"))

(define_predicate "gcn_compare_64bit_operator"
  (match_code "eq,ne"))

(define_predicate "gcn_compare_operator"
  (match_code "eq,ne,gt,ge,lt,le,gtu,geu,ltu,leu"))

(define_predicate "vec_and_scalar_commutative_64bit_operator"
  (match_code "and,ior,xor"))

(define_predicate "vec_and_scalar_64bit_operator"
  (match_code "and,ior,xor,ashift,lshiftrt,ashiftrt"))

(define_predicate "binary_operator"
  (match_code "and,ior,xor,ashift,lshiftrt,ashiftrt"))

(define_predicate "gcn_register_or_unspec_operand"
  (ior (match_operand 0 "register_operand")
       (and (match_code "unspec")
            (match_test "XINT (op, 1) == UNSPEC_VECTOR"))))

(define_predicate "gcn_register_ds_or_unspec_operand"
  (ior (match_operand 0 "register_operand")
       (ior (match_operand 0 "gcn_ds_memory_operand")
	    (and (match_code "unspec")
              (match_test "XINT (op, 1) == UNSPEC_VECTOR")))))

(define_predicate "gcn_buffer_memory_operand"
  (and (match_code "mem")
       (and (match_test "MEM_ADDR_SPACE (op) == ADDR_SPACE_SCRATCH")
	    (match_operand 0 "memory_operand"))))
