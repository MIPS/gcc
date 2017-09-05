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
  i6001
  m6001
])

(include "i6001.md")

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
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "load"))
  "r24k_iss+r24k_ixu_arith")

;; 2. Arithmetic: add, addi, addiu, addiupc, addu, and, andi, clo, clz,
;;    ext, ins, lui, movn, movz, nor, or, ori, rotr, rotrv, seb, seh, sll,
;;    sllv, slt, slti, sltiu, sltu, sra, srav, srl, srlv, sub, subu, wsbh,
;;    xor, xori
;; (movn/movz is not matched, we'll need to split condmov to
;;  differentiate between integer/float moves)
(define_insn_reservation "r24k_int_arith" 1
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "arith,const,logical,move,nop,shift,signext,slt"))
  "r24k_iss+r24k_ixu_arith")

;; 3. Links: bgezal, bgezall, bltzal, bltzall, jal, jalr, jalx
;; 3a. jr/jalr consumer
(define_insn_reservation "r24k_int_jump" 1
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "call,jump"))
  "r24k_iss+r24k_ixu_arith")

;; 3b. branch consumer
(define_insn_reservation "r24k_int_branch" 1
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "branch"))
  "r24k_iss+r24k_ixu_arith")

;; 4. MDU: fully pipelined multiplier
;; mult - delivers result to hi/lo in 1 cycle (pipelined)
(define_insn_reservation "r24k_int_mult" 1
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "imul"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; madd, msub - delivers result to hi/lo in 1 cycle (pipelined)
(define_insn_reservation "r24k_int_madd" 1
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "imadd"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; mul - delivers result to gpr in 5 cycles
(define_insn_reservation "r24k_int_mul3" 5
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "imul3"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)*5")

;; mfhi, mflo, mflhxu - deliver result to gpr in 5 cycles
(define_insn_reservation "r24k_int_mfhilo" 5
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "mfhi,mflo"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; mthi, mtlo, mtlhx - deliver result to hi/lo, thence madd, handled as bypass
(define_insn_reservation "r24k_int_mthilo" 1
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "mthi,mtlo"))
  "r24k_iss+(r24k_mul3a|r24k_mul3b|r24k_mul3c)")

;; div - default to 36 cycles for 32bit operands.  Faster for 24bit, 16bit and
;; 8bit, but is tricky to identify.
(define_insn_reservation "r24k_int_div" 36
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "idiv"))
  "r24k_iss+(r24k_mul3a+r24k_mul3b+r24k_mul3c)*36")


;; 5. Cop: cfc1, di, ei, mfc0, mtc0
;; (Disabled until we add proper cop0 support)
;;(define_insn_reservation "r24k_int_cop" 3
;;  (and (eq_attr "cpu" "m6001")
;;       (eq_attr "type" "cop0"))
;;  "r24k_iss+r24k_ixu_arith")


;; 6. Store
(define_insn_reservation "r24k_int_store" 1
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "store"))
  "r24k_iss+r24k_ixu_arith")


;; 7. Multiple instructions
(define_insn_reservation "r24k_int_multi" 1
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "multi"))
  "r24k_iss+r24k_ixu_arith+(r24k_mul3a+r24k_mul3b+r24k_mul3c)")

;; 8. Unknowns - Currently these include blockage, consttable and alignment
;;    rtls. They do not really affect scheduling latency, (blockage affects
;;    scheduling via log links, but not used here).
(define_insn_reservation "r24k_int_unknown" 0
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "unknown,atomic,syncloop"))
  "r24k_iss")

;; 9. Prefetch
(define_insn_reservation "r24k_int_prefetch" 1
  (and (eq_attr "cpu" "m6001")
       (eq_attr "type" "prefetch,prefetchx"))
  "r24k_iss+r24k_ixu_arith")

(include "mips-common.md")

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
   (set_attr "has_16bit_ver" "yes,yes,yes,rri_add,yes,no,no,no,yes")
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
  [(set_attr "type" "move")
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
  [(set_attr "type" "move")
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
      || GET_CODE (operands[1]) != REG)
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
  rtx to;
  rtx op1;
  int i;

  if (GET_CODE (operands[2]) != CONST_INT
      || INTVAL (operands[2]) > 8
      || INTVAL (operands[2]) < 2
      || GET_CODE (operands[1]) != MEM
      || MEM_VOLATILE_P (operands[0])
      || GET_CODE (operands[0]) != REG)
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

(define_c_enum "unspec" [
  UNSPEC_ADDRESS_FIRST
])
