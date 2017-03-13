;; Constraint definitions for GCN.
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

(define_constraint "I"
  "Inline integer constant"
  (and (match_code "const_int")
       (match_test "ival >= -16 && ival <= 64")))

(define_constraint "J"
  "Signed integer 16-bit inline constant"
  (and (match_code "const_int")
       (match_test "((unsigned HOST_WIDE_INT) ival + 0x8000) < 0x10000")))

(define_constraint "K"
  "Integer 32-bit constant"
  (and (match_code "const_int")
       (match_test "trunc_int_for_mode (ival, SImode) == ival")))

(define_constraint "O"
  "Integer one constant"
  (and (match_code "const_int")
       (match_test "ival == 1")))

(define_constraint "G"
  "Inline floating point constant."
  (and (match_code "const_double")
       (match_test "gcn_inline_fp_constant_p (op, false) > 0")))

(define_constraint "H"
  "floating point constant representable as inline or 32bit immediate."
  (and (match_code "const_double")
       (match_test "gcn_fp_constant_p (op, false) > 0")))

(define_constraint "A"
  "Inline immediate parameter"
  (and (match_code "const_int,const_double,const_vector")
       (match_test "gcn_inline_constant_p (op)")))

(define_constraint "B"
  "Inline immediate parameter"
  (and (match_code "const_int,const_double,const_vector")
	(match_test "gcn_constant_p (op)")))

(define_constraint "U"
  "unspecified value"
  (match_code "unspec"))

(define_register_constraint "v" "VGPR_REGS"
  "VGPR registers")

(define_register_constraint "Sg" "SGPR_REGS"
  "SGPR registers")

(define_register_constraint "SD" "SGPR_DST_REGS"
  "registers useable as a destination of scalar operation")

(define_register_constraint "SS" "SGPR_SRC_REGS"
  "registers useable as a source of scalar operation")

(define_register_constraint "Sm" "SGPR_MEM_SRC_REGS"
  "registers useable as a source of scalar memory operation")

(define_register_constraint "Sv" "SGPR_VOP3A_SRC_REGS"
  "registers useable as a source of VOP3A instruction")

(define_register_constraint "ca" "ALL_CONDITIONAL_REGS"
  "SCC VCCZ or EXECZ")

(define_register_constraint "cs" "SCC_CONDITIONAL_REG"
  "SCC")

(define_register_constraint "cv" "VCCZ_CONDITIONAL_REG"
  "VCCZ")

(define_register_constraint "cV" "VCC_CONDITIONAL_REG"
  "VCC")

(define_register_constraint "ce" "EXECZ_CONDITIONAL_REG"
  "EXECZ")

(define_register_constraint "e" "EXEC_MASK_REG"
  "EXEC")

(define_memory_constraint "RB"
  "Buffer memory address to scratch memory."
  (and (match_code "mem")
       (match_test "MEM_ADDR_SPACE (op) == ADDR_SPACE_SCRATCH")))

(define_memory_constraint "RF"
  "Buffer memory address to flat memory."
  (and (match_code "mem")
       (match_test "MEM_ADDR_SPACE (op) == ADDR_SPACE_FLAT")))

(define_memory_constraint "RS"
  "Buffer memory address to scalar flat memory."
  (and (match_code "mem")
       (match_test "MEM_ADDR_SPACE (op) == ADDR_SPACE_SCALAR_FLAT")))

(define_memory_constraint "RL"
  "Buffer memory address to LDS memory."
  (and (match_code "mem")
       (match_test "MEM_ADDR_SPACE (op) == ADDR_SPACE_LDS")))

(define_memory_constraint "RG"
  "Buffer memory address to GDS memory."
  (and (match_code "mem")
       (match_test "MEM_ADDR_SPACE (op) == ADDR_SPACE_GDS")))

(define_memory_constraint "RD"
  "Buffer memory address to GDS or LDS memory."
  (and (match_code "mem")
       (ior (match_test "MEM_ADDR_SPACE (op) == ADDR_SPACE_GDS")
	    (match_test "MEM_ADDR_SPACE (op) == ADDR_SPACE_LDS"))))
