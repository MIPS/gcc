;; Copyright (C) 2017 Free Software Foundation, Inc.
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
  m7000
])

(include "i7200.md")

;; FIXME: let's use 24KC scheduler temporarily for nanoMIPS32R6/nanoMIPS32R6s
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
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "load"))
  "r24k_iss+r24k_ixu_arith")

;; 2. Arithmetic: add, addi, addiu, addiupc, addu, and, andi, clo, clz,
;;    ext, ins, lui, movn, movz, nor, or, ori, rotr, rotrv, seb, seh, sll,
;;    sllv, slt, slti, sltiu, sltu, sra, srav, srl, srlv, sub, subu, wsbh,
;;    xor, xori
;; (movn/movz is not matched, we'll need to split condmov to
;;  differentiate between integer/float moves)
(define_insn_reservation "r24k_int_arith" 1
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "arith,const,logical,move,nop,shift,signext,slt"))
  "r24k_iss+r24k_ixu_arith")

;; 3. Links: bgezal, bgezall, bltzal, bltzall, jal, jalr, jalx
;; 3a. jr/jalr consumer
(define_insn_reservation "r24k_int_jump" 1
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "call,jump"))
  "r24k_iss+r24k_ixu_arith")

;; 3b. branch consumer
(define_insn_reservation "r24k_int_branch" 1
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "branch"))
  "r24k_iss+r24k_ixu_arith")

;; 4. MDU: fully pipelined multiplier
;; mult - delivers result to hi/lo in 1 cycle (pipelined)
(define_insn_reservation "r24k_int_mult" 1
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "imul"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; madd, msub - delivers result to hi/lo in 1 cycle (pipelined)
(define_insn_reservation "r24k_int_madd" 1
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "imadd"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; mul - delivers result to gpr in 5 cycles
(define_insn_reservation "r24k_int_mul3" 5
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "imul3"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)*5")

;; mfhi, mflo, mflhxu - deliver result to gpr in 5 cycles
(define_insn_reservation "r24k_int_mfhilo" 5
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "mfhi,mflo"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; mthi, mtlo, mtlhx - deliver result to hi/lo, thence madd, handled as bypass
(define_insn_reservation "r24k_int_mthilo" 1
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "mthi,mtlo"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; div - default to 36 cycles for 32bit operands.  Faster for 24bit, 16bit and
;; 8bit, but is tricky to identify.
(define_insn_reservation "r24k_int_div" 36
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "idiv"))
  "r24k_iss+(r24k_mul3a+r24k_mul3b+r24k_mul3c)*36")


;; 5. Cop: cfc1, di, ei, mfc0, mtc0
;; (Disabled until we add proper cop0 support)
;;(define_insn_reservation "r24k_int_cop" 3
;;  (and (eq_attr "cpu" "m7000")
;;       (eq_attr "type" "cop0"))
;;  "r24k_iss+r24k_ixu_arith")


;; 6. Store
(define_insn_reservation "r24k_int_store" 1
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "store"))
  "r24k_iss+r24k_ixu_arith")


;; 7. Multiple instructions
(define_insn_reservation "r24k_int_multi" 1
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "multi"))
  "r24k_iss+r24k_ixu_arith+(r24k_mul3a+r24k_mul3b+r24k_mul3c)")

;; 8. Unknowns - Currently these include blockage, consttable and alignment
;;    rtls. They do not really affect scheduling latency, (blockage affects
;;    scheduling via log links, but not used here).
(define_insn_reservation "r24k_int_unknown" 0
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "unknown,atomic,syncloop"))
  "r24k_iss")

;; 9. Prefetch
(define_insn_reservation "r24k_int_prefetch" 1
  (and (eq_attr "cpu" "m7000")
       (eq_attr "type" "prefetch,prefetchx"))
  "r24k_iss+r24k_ixu_arith")

(include "mips.md")

; The MIPS DSP Instructions.

(include "mips-dsp.md")


; The MIPS DSP REV 2 Instructions.

(include "mips-dspr2.md")

(define_insn "*mips_addsi3_48"
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
   && mips_movep_target_p (operands[0], operands[2])"
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
  "ISA_HAS_MOVEP_REV
   && mips_movep_target_p (operands[1], operands[3])"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])])

(define_insn "*movep<MOVEP1:mode><MOVEP2:mode>_rev"
  [(set (match_operand:MOVEP1 0 "movep_rev_operand")
	(match_operand:MOVEP1 1 "register_operand"))
   (set (match_operand:MOVEP2 2 "movep_rev_operand")
	(match_operand:MOVEP2 3 "register_operand"))]
  "ISA_HAS_MOVEP_REV
   && mips_movep_target_p (operands[1], operands[3])"
{
  if (REGNO (operands[1]) < REGNO (operands[3]))
    return "movep\t%0,%2,%z1,%z3";
  else
    return "movep\t%2,%0,%z3,%z1";
}
  [(set_attr "type" "move")
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

(define_c_enum "unspec" [
  UNSPEC_ADDRESS_FIRST
])
