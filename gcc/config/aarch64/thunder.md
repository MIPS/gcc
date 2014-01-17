;; Cavium Thunder pipeline description
;; Copyright (C) 2011 Free Software Foundation, Inc.
;;
;; Written by Andrew Pinski  <apinski@cavium.com>

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.
;;   Copyright (C) 2004, 2005, 2006 Cavium Networks.


;; Thunder is a dual-issue processor that can issue all instructions on
;; pipe0 and a subset on pipe1.


(define_automaton "thunder_main, thunder_mult, thunder_divide")

(define_cpu_unit "thunder_pipe0" "thunder_main")
(define_cpu_unit "thunder_pipe1" "thunder_main")
(define_cpu_unit "thunder_mult" "thunder_mult")
(define_cpu_unit "thunder_divide" "thunder_divide")

(define_insn_reservation "thunder_add" 1
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "adc_imm,adc_reg,adr,alu_imm,alu_reg,alus_imm,alus_reg,extend,logic_imm,logic_reg,logics_imm,logics_reg,mov_imm,mov_reg"))
  "thunder_pipe0 | thunder_pipe1")

(define_insn_reservation "thunder_shift" 1
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "bfm,extend,shift_imm,shift_reg"))
  "thunder_pipe0 | thunder_pipe1")


;; Arthimentic instructions with an extra shift or extend is two cycles.
;; FIXME: This needs more attributes on aarch64 than what is currently there;
;;    this is conserative for now.
;; Except this is not correct as this is only for !(LSL && shift by 0/1/2/3)
;; Except this is not correct as this is only for !(zero extend)

(define_insn_reservation "thunder_arith_shift" 2
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "alu_ext,alu_shift_imm,alu_shift_reg,alus_ext,logic_shift_imm,logic_shift_reg,logics_shift_imm,logics_shift_reg,alus_shift_imm"))
  "thunder_pipe0 | thunder_pipe1")

(define_insn_reservation "thunder_csel" 2
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "csel"))
  "thunder_pipe0 | thunder_pipe1")

;; Multiply and mulitply accumulate and count leading zeros can only happen on pipe 1

(define_insn_reservation "thunder_mul" 4
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "mul,muls,mla,mlas,clz,smull,umull")
       (not (eq_attr "mode" "TI")))
  "thunder_pipe1 + thunder_mult")

;; Multiply high instructions take an extra cycle and cause the muliply unit to
;; be busy for an extra cycle.

(define_insn_reservation "thunder_mul_high" 5
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "mul,muls,mla,mlas,clz")
       (not (eq_attr "mode" "TI")))
  "thunder_pipe1 + thunder_mult, thunder_mult")

(define_insn_reservation "thunder_div32" 22
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "udiv,sdiv")
       (eq_attr "mode" "SI"))
  "thunder_pipe1 + thunder_divide, thunder_divide * 21")

(define_insn_reservation "thunder_div64" 38
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "udiv,sdiv")
       (eq_attr "mode" "DI"))
  "thunder_pipe1 + thunder_divide, thunder_divide * 34")

;; Stores take one cycle in pipe 0
(define_insn_reservation "thunder_store" 1
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "store1"))
  "thunder_pipe0")

;; Store pair are single issued
(define_insn_reservation "thunder_storepair" 1
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "store2"))
  "thunder_pipe0 + thunder_pipe1")


;; loads (and load pairs) from L1 take 3 cycles in pipe 0
(define_insn_reservation "thunder_load" 3
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "load1, load2"))
  "thunder_pipe0")

(define_insn_reservation "thunder_brj" 1
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "branch,trap,call"))
  "thunder_pipe1")

;; FPU

(define_insn_reservation "thunder_fadd" 4
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "faddd,fadds"))
  "thunder_pipe1")

(define_insn_reservation "thunder_fconst" 1
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "fconsts,fconstd"))
  "thunder_pipe1")

;; Moves between fp are 2 cycles including min/max/select/abs/neg
(define_insn_reservation "thunder_fmov" 2
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "fmov,f_minmaxs,f_minmaxd,f_sels,f_seld,ffarithd,ffariths"))
  "thunder_pipe1")

(define_insn_reservation "thunder_fmovgpr" 2
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "f_mrc, f_mcr"))
  "thunder_pipe1")

(define_insn_reservation "thunder_fmul" 6
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "fmacs,fmacd,fmuls,fmuld"))
  "thunder_pipe1")

(define_insn_reservation "thunder_fdivs" 12
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "fdivs"))
  "thunder_pipe1 + thunder_divide, thunder_divide*8")

(define_insn_reservation "thunder_fdivd" 22
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "fdivd"))
  "thunder_pipe1 + thunder_divide, thunder_divide*18")

(define_insn_reservation "thunder_fsqrts" 17
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "fsqrts"))
  "thunder_pipe1 + thunder_divide, thunder_divide*13")

(define_insn_reservation "thunder_fsqrtd" 28
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "fsqrtd"))
  "thunder_pipe1 + thunder_divide, thunder_divide*31")

;; The rounding conversion inside fp is 4 cycles
(define_insn_reservation "thunder_frint" 4
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "f_rints, f_rintd"))
  "thunder_pipe1")

;; Float to integer with a move from int to/from float is 6 cycles
(define_insn_reservation "thunder_f_cvt" 6
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "f_cvt, f_cvtf2i"))
  "thunder_pipe1")

;; Assume both pipes are needed for unknown and multiple-instruction
;; patterns.

(define_insn_reservation "thunder_unknown" 1
  (and (eq_attr "tune" "thunder")
       (eq_attr "type" "untyped,multiple"))
  "thunder_pipe0 + thunder_pipe1")


