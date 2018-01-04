;; DFA-based pipeline description for nms1.
;;
;; Copyright (C) 2018 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
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

;; FIXME: let's use 24KC scheduler temporarily for nanoMIPS32R6s
(define_automaton "r24k_cpu, r24k_mdu")

;; Integer execution unit.
(define_cpu_unit "r24k_iss"		"r24k_cpu")
(define_cpu_unit "r24k_ixu_arith"	"r24k_cpu")
(define_cpu_unit "r24k_mul3a"	        "r24k_mdu")
(define_cpu_unit "r24k_mul3b"	        "r24k_mdu")
(define_cpu_unit "r24k_mul3c"	        "r24k_mdu")

;; --------------------------------------------------------------
;; Producers
;; --------------------------------------------------------------

;; 1. Loads: lb, lbu, lh, lhu, ll, lw, lwl, lwr, lwpc, lwxs
(define_insn_reservation "r24k_int_load" 2
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "load"))
  "r24k_iss+r24k_ixu_arith")

;; 2. Arithmetic: add, addi, addiu, addiupc, addu, and, andi, clo, clz,
;;    ext, ins, lui, movn, movz, nor, or, ori, rotr, rotrv, seb, seh, sll,
;;    sllv, slt, slti, sltiu, sltu, sra, srav, srl, srlv, sub, subu, wsbh,
;;    xor, xori
;; (movn/movz is not matched, we'll need to split condmov to
;;  differentiate between integer/float moves)
(define_insn_reservation "r24k_int_arith" 1
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "arith,const,logical,move,nop,shift,signext,slt"))
  "r24k_iss+r24k_ixu_arith")

;; 3. Links: bgezal, bgezall, bltzal, bltzall, jal, jalr, jalx
;; 3a. jr/jalr consumer
(define_insn_reservation "r24k_int_jump" 1
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "call,jump"))
  "r24k_iss+r24k_ixu_arith")

;; 3b. branch consumer
(define_insn_reservation "r24k_int_branch" 1
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "branch"))
  "r24k_iss+r24k_ixu_arith")

;; 4. MDU: fully pipelined multiplier
;; mult - delivers result to hi/lo in 1 cycle (pipelined)
(define_insn_reservation "r24k_int_mult" 1
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "imul"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; madd, msub - delivers result to hi/lo in 1 cycle (pipelined)
(define_insn_reservation "r24k_int_madd" 1
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "imadd"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; mul - delivers result to gpr in 5 cycles
(define_insn_reservation "r24k_int_mul3" 5
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "imul3"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)*5")

;; mfhi, mflo, mflhxu - deliver result to gpr in 5 cycles
(define_insn_reservation "r24k_int_mfhilo" 5
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "mfhi,mflo"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; mthi, mtlo, mtlhx - deliver result to hi/lo, thence madd, handled as bypass
(define_insn_reservation "r24k_int_mthilo" 1
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "mthi,mtlo"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; div - default to 36 cycles for 32bit operands.  Faster for 24bit, 16bit and
;; 8bit, but is tricky to identify.
(define_insn_reservation "r24k_int_div" 36
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "idiv"))
  "r24k_iss+(r24k_mul3a+r24k_mul3b+r24k_mul3c)*36")


;; 5. Cop: cfc1, di, ei, mfc0, mtc0
;; (Disabled until we add proper cop0 support)
;;(define_insn_reservation "r24k_int_cop" 3
;;  (and (eq_attr "cpu" "nms1")
;;       (eq_attr "type" "cop0"))
;;  "r24k_iss+r24k_ixu_arith")


;; 6. Store
(define_insn_reservation "r24k_int_store" 1
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "store"))
  "r24k_iss+r24k_ixu_arith")


;; 7. Multiple instructions
(define_insn_reservation "r24k_int_multi" 1
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "multi"))
  "r24k_iss+r24k_ixu_arith+(r24k_mul3a+r24k_mul3b+r24k_mul3c)")

;; 8. Unknowns - Currently these include blockage, consttable and alignment
;;    rtls. They do not really affect scheduling latency, (blockage affects
;;    scheduling via log links, but not used here).
(define_insn_reservation "r24k_int_unknown" 0
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "unknown,atomic,syncloop"))
  "r24k_iss")

;; 9. Prefetch
(define_insn_reservation "r24k_int_prefetch" 1
  (and (eq_attr "cpu" "nms1")
       (eq_attr "type" "prefetch,prefetchx"))
  "r24k_iss+r24k_ixu_arith")
