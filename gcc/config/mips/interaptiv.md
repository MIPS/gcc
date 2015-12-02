;; DFA-based pipeline description for interAptiv core.
;;
;; Copyright (C) 2015 Free Software Foundation, Inc.
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

(define_automaton "interaptiv_int,interaptiv_mem,interaptiv_fpu")

;; Integer execution unit.
(define_cpu_unit "interaptiv_alu,interaptiv_mdu" "interaptiv_int")

;; Load/store unit.
(define_cpu_unit "interaptiv_ldst" "interaptiv_mem")

;; Floating-point execution unit.
(define_cpu_unit "interaptiv_fpalu,interaptiv_fpmov" "interaptiv_fpu")

;; Integer ALU instructions, latency 1:
;; add, addi, addiu, addu, and, andi, clo, clz, ext, ins, lui, nor, or, ori,
;; rotr, rotrv, seb, seh, sll, sllv, slt, slti, sltiu, sltu, srl, shlv, sub,
;; subu, wsbh, xor, xori
(define_insn_reservation "interaptiv_int1_arith" 1
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "arith,clz,const,logical,move,shift,signext,slt"))
  "interaptiv_alu")

;; Integer conditional move on GPR instructions, latency 1:
;; movn, movz
(define_insn_reservation "interaptiv_int1_cmovgpr" 1
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "condmove")
       (eq_attr "cond_type" "gpr")
       (eq_attr "mode" "SI,DI"))
  "interaptiv_alu")

;; Integer conditional move on FCC instructions, latency 2:
;; movf, movt
(define_insn_reservation "interaptiv_int2_cmovfcc" 2
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "condmove")
       (eq_attr "cond_type" "fcc")
       (eq_attr "mode" "SI,DI"))
  "interaptiv_alu")

;; Integer rdhwr instruction, latency 2:
;; rdhwr
(define_insn_reservation "interaptiv_int2_rdhwr" 2
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "rdhwr"))
  "interaptiv_alu")

;; Jump, branch and trap instructions, excluding branch on coprocessor
;; condition instructions, latency 1:
;; b<cond>, b<cond>al, b<cond>all, b<cond>l, j, jal, jalr, jr, t<cond>
(define_insn_reservation "interaptiv_int1_branch" 1
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "branch,call,jump,trap")
       (not (eq_attr "cond_type" "fcc")))
  "interaptiv_alu")

;; Branch on coprocessor condition instructions, latency 2:
;; bc1f, bc1fl, bc1t, bc1tl
(define_insn_reservation "interaptiv_int2_bcc" 2
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "branch")
       (eq_attr "cond_type" "fcc"))
  "interaptiv_alu")

;; Integer multiplication instructions targeting HI/LO, latency 1:
;; madd, maddu, msub, msubu, multu
(define_insn_reservation "interaptiv_int1_mul" 1
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "imul,imadd"))
  "interaptiv_mdu")

;; Integer accumulator write instructions, latency 2:
;; mthi, mtlo
(define_insn_reservation "interaptiv_int2_mtx" 2
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "mthi,mtlo"))
  "interaptiv_mdu")

;; Integer accumulator read and multiplication instructions targeting GPR,
;; latency 5:
;; mfhi, mflo, mul
(define_insn_reservation "interaptiv_int5_mfx" 5
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "imul3,mfhi,mflo"))
  "interaptiv_mdu")

;; Integer unsigned division instruction, latency 31, repeat rate 31:
;; divu
(define_insn_reservation "interaptiv_int31_divu" 31
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "idiv")
       (eq_attr "idiv_type" "udiv"))
  "interaptiv_mdu*31")

;; Integer signed division instruction, latency 33, repeat rate 33:
;; div
(define_insn_reservation "interaptiv_int33_div" 33
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "idiv")
       (eq_attr "idiv_type" "div"))
  "interaptiv_mdu*33")

;; Integer prefetch instruction, latency 1:
;; pref
(define_insn_reservation "interaptiv_int1_pref" 1
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "prefetch"))
  "interaptiv_ldst")

;; Integer store instructions, latency 1:
;; sb, sh, sw, swl, swr
(define_insn_reservation "interaptiv_int1_store" 1
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "store"))
  "interaptiv_ldst")

;; Integer load instructions, latency 2:
;; lb, lbu, lh, lhu, lw, lwl, lwr
(define_insn_reservation "interaptiv_int2_load" 2
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "load"))
  "interaptiv_ldst")

;; Integer indexed load instructions, latency 4:
;; lbux, lhx, lwx
(define_insn_reservation "interaptiv_int4_load" 4
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "idxload"))
  "interaptiv_ldst")

;; Floating-point compare instructions, latency 2:
;; c.<cond>.fmt
(define_insn_reservation "interaptiv_fpu2_cmp" 2
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "fcmp"))
  "interaptiv_fpalu")

;; Floating-point add/subtract, sign manipulation and single multiply
;; instructions, latency 4:
;; abs.fmt, add.fmt, madd.s, msub.s, mul.s, neg.fmt, nmadd.s, nmsub.s, sub.fmt
(define_insn_reservation "interaptiv_fpu4_arith" 4
  (and (eq_attr "cpu" "interaptiv")
       (ior (eq_attr "type" "fabs,fadd,fneg")
	    (and (eq_attr "type" "fmul,fmadd")
		 (eq_attr "mode" "SF"))))
  "interaptiv_fpalu")

;; Floating-point convert to floating point instructions, latency 4:
;; cvt.d.fmt, cvt.s.fmt
(define_insn_reservation "interaptiv_fpu4_cvt2float" 4
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "fcvt")
       (eq_attr "cnv_mode" "D2S,I2D,I2S,S2D"))
  "interaptiv_fpalu")

;; Floating-point convert to fixed point instructions, latency 5:
;; ceil.fmt.fmt, cvt.l.fmt, cvt.w.fmt, floor.fmt.fmt, round.fmt.fmt
;; trunc.fmt.fmt
(define_insn_reservation "interaptiv_fpu5_cvt2fixed" 5
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "fcvt")
       (eq_attr "cnv_mode" "D2I,S2I"))
  "interaptiv_fpalu")

;; Floating-point double multiply instructions, latency 5, repeat rate 2:
;; madd.d, msub.d, nmadd.d, nmsub.d
(define_insn_reservation "interaptiv_fpu5_muld" 5
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "fmul,fmadd")
       (eq_attr "mode" "DF"))
  "interaptiv_fpalu*2")

;; Floating-point single reciprocal divide instruction, latency 13,
;; repeat rate 10:
;; recip.s
(define_insn_reservation "interaptiv_fpu13_recips" 13
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "frdiv")
       (eq_attr "mode" "SF"))
  "interaptiv_fpalu*10")

;; Floating-point single divide, square root and reciprocal square root
;; instructions, latency 17, repeat rate 14:
;; div.s, rsqrt.s, sqrt.s
(define_insn_reservation "interaptiv_fpu17_divs" 17
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "fdiv,frsqrt,fsqrt")
       (eq_attr "mode" "SF"))
  "interaptiv_fpalu*14")

;; Floating-point double reciprocal divide instruction, latency 25,
;; repeat rate 21:
;; recip.d
(define_insn_reservation "interaptiv_fpu25_recipd" 25
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "frdiv")
       (eq_attr "mode" "DF"))
  "interaptiv_fpalu*21")

;; Floating-point double divide, square root and reciprocal square root
;; instruction, latency 32, repeat rate 29:
;; div.d, rsqrt.d, sqrt.d
(define_insn_reservation "interaptiv_fpu32_divd" 32
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "fdiv,frsqrt,fsqrt")
       (eq_attr "mode" "DF"))
  "interaptiv_fpalu*29")

;; Floating-point prefetch instruction, latency 1:
;; prefx
(define_insn_reservation "interaptiv_fpu1_pref" 1
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "prefetchx"))
  "interaptiv_fpmov")

;; Floating-point store instructions, latency 1:
;; sdc1, sdxc1, suxc1, swc1, swxc1
(define_insn_reservation "interaptiv_fpu1_store" 1
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "fpstore,fpidxstore"))
  "interaptiv_fpmov")

;; Floating-point load instructions, latency 2:
;; ldc1, ldxc1, luxc1, lwc1, lwxc1
(define_insn_reservation "interaptiv_fpu2_load" 2
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "fpload,fpidxload"))
  "interaptiv_fpmov")

;; Floating-point move and control to/from instructions, latency 2:
;; cfc1, ctc1, mfc1, mfhc1, mtc1, mthc1
(define_insn_reservation "interaptiv_fpu2_xfer" 2
  (and (eq_attr "cpu" "interaptiv")
       (eq_attr "type" "mfc,mtc,cfc,ctc"))
  "interaptiv_fpmov")

;; Floating-point move and conditional move instructions, latency 4:
;; mov.fmt, movf.fmt, movn.fmt, movt.fmt, movz.fmt
(define_insn_reservation "interaptiv_fpu4_move" 4
  (and (eq_attr "cpu" "interaptiv")
       (ior (eq_attr "type" "fmove")
	    (and (eq_attr "type" "condmove")
		 (eq_attr "mode" "SF,DF"))))
  "interaptiv_fpmov")

;; Integer latency-1 instructions to address generation, latency 2:
(define_bypass 2 "interaptiv_int1_*" "interaptiv_*_pref,interaptiv_*_load")
(define_bypass 2 "interaptiv_int1_*" "interaptiv_*_store"
  "!mips_store_data_bypass_p")

;; Integer latency-2 instructions to address generation, latency 3:
(define_bypass 3 "interaptiv_int2_*" "interaptiv_*_pref,interaptiv_*_load")
(define_bypass 3 "interaptiv_int2_*" "interaptiv_*_store"
  "!mips_store_data_bypass_p")

;; Integer latency-4 instructions to address generation, latency 5:
(define_bypass 5 "interaptiv_int4_*" "interaptiv_*_pref,interaptiv_*_load")
(define_bypass 5 "interaptiv_int4_*" "interaptiv_*_store"
  "!mips_store_data_bypass_p")

;; Integer latency-5 instructions to address generation, latency 6:
(define_bypass 6 "interaptiv_int5_*" "interaptiv_*_pref,interaptiv_*_load")
(define_bypass 6 "interaptiv_int5_*" "interaptiv_*_store"
  "!mips_store_data_bypass_p")
