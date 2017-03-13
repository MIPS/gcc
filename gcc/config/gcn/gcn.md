;; Copyright (C) 2016-2017 Free Software Foundation, Inc.

;; This file is free software; you can redistribute it and/or modify it under
;; the terms of the GNU General Public License as published by the Free
;; Software Foundation; either version 3 of the License, or (at your option)
;; any later version.

;; This file is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
;; FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
;; for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;;- See file "rtl.def" for documentation on define_insn, match_*, et. al.

;; -------------------------------------------------------------------------
;; Constants and enums
;; -------------------------------------------------------------------------

; Named registers
(define_constants
  [(FIRST_SGPR_REG		 0)
   (LAST_SGPR_REG		 101)
   (FLAT_SCRATCH_REG		 102)
   (FLAT_SCRATCH_LO_REG		 102)
   (FLAT_SCRATCH_HI_REG		 103)
   (XNACK_MASK_REG		 104)
   (XNACK_MASK_LO_REG		 104)
   (XNACK_MASK_HI_REG		 105)
   (VCC_REG			 106)
   (VCC_LO_REG			 106)
   (VCC_HI_REG			 107)
   (VCCZ_REG			 108)
   (TBA_REG			 109)
   (TBA_LO_REG			 109)
   (TBA_HI_REG			 110)
   (TMA_REG			 111)
   (TMA_LO_REG			 111)
   (TMA_HI_REG			 112)
   (TTMP0_REG			 113)
   (TTMP11_REG			 124)
   (M0_REG			 125)
   (EXEC_REG			 126)
   (EXEC_LO_REG			 126)
   (EXEC_HI_REG			 127)
   (EXECZ_REG			 128)
   (SCC_REG			 129)
; Lazyness gap of unused hard regs.
   (FIRST_VGPR_REG		 160)
   (LAST_VGPR_REG		 415)]
   )

; Named unspec values
(define_c_enum "unspecv" [
  UNSPECV_PROLOGUE_USE])

(define_c_enum "unspec" [
  UNSPEC_VECTOR])

;; -------------------------------------------------------------------------
;; Attributes
;; -------------------------------------------------------------------------

; Every instructions should set MODE and TYPE.  Other attributes should be
; more or less determined by their conditionals.

; Main data type used by the insn
(define_attr "mode"
  "unknown,none,BI,QI,HI,SI,DI,TI,HF,SF,DF,V2SF,V2SI,V3DI,V64SI,V64DI,V64SF,
   V64DF,V4SF,V2DF,V3SF,V4SI,V3SI,V2DI,V64HF,V64HI,V64QI"
  (const_string "unknown"))

; Instruction type (encoding) as described the specification.
; The following table summarizes possible operands of individual instruction
; types and corresponding constraints.
;
; sop2 - scalar, two inputs, one output
;	 ssrc0/ssrc1: sgpr 0-102; flat_scratch,xnack,vcc,tba,tma,ttmp0-11,exec
;		      vccz,execz,scc,inline immedate,fp inline immediate
;	 sdst: sgpr 0-102; flat_scratch,xnack,vcc,tba,tma,ttmp0-11,exec
;
; 	 Constraints "=SD, SD", "SSA,SSB","SSB,SSA"
;
; sopk - scalar, inline constant input, one output
;	 simm16: 16bit inline constant
;	 sdst: same as sop2/ssrc0
;
;	 Constraints "=SD", "J"
;
; sop1 - scalar, one input, one output
;	 ssrc0: same as sop2/ssrc0.  FIXME: manual omit VCCZ
;	 sdst: same as sop2/sdst
;
;	 Constraints "=SD", "SSA"
;
; sopc - scalar, two inputs, one comparsion
;	 ssrc0: same as sop2/ssc0.
;
; 	 Constraints "SSI,SSA","SSA,SSI"
;
; sopp - scalar, one constant input, one special
;	 simm16
;
; smem - scalar memory
;	 sbase: aligned pair of sgprs.  Specify {size[15:0], base[47:0]} in
;               dwords
;	 sdata: sgpr0-102, flat_scratch, xnack, vcc, tba, tma
;	 offset: sgpr or 20bit unsigned byte offset
;
; vop2 - vector, two inputs, one output
;	 vsrc0: sgpr0-102,flat_scratch,xnack,vcc,tba,ttmp0-11,m0,exec,
;		inline constant -16 to -64, fp inline immediate, vccz, execz,
;		scc, lds, literal constant, vgpr0-255
;	 vsrc1: vgpr0-255
;	 vdst: vgpr0-255
;	 Limitations: At most one SGPR, at most one constant
;		      if constant is used, SGPR must be M0
;		      Only SRC0 can be LDS_DIRECT
;
;	 constraints: "=v", "vBSS", "v"
;
; vop1 - vector, one input, one output
;	 vsrc0: same as vop2/src0
;	 vdst: vgpr0-255
;
;	 constraints: "=v", "vBSS"
;
; vopc - vector, two inputs, one comparsion output;
;	 vsrc0: same as vop2/src0
;	 vsrc1: vgpr0-255
;	 vdst:
;
;	 constraints: "vASS", "v"
;
; vop3a - vector, three inputs, one output
;	 vdst: vgpr0-255, for v_cmp sgpr or vcc
;	 abs,clamp
;	 vsrc0: sgpr0-102,vcc,tba,ttmp0-11,m0,exec,
;		inline constant -16 to -64, fp inline immediate, vccz, execz,
;		scc, lds_direct
;		FIXME: really missing 1/pi? really 104 SGPRs
;
; vop3b - vector, three inputs, one vector output, one scalar output
;	 vsrc0,vsrc1,vsrc2: same as vop3a vsrc0
;	 vdst: vgpr0-255
;	 sdst: sgpr0-103/vcc/tba/tma/ttmp0-11
;
;
; mult is for insn representing multiple instructions, vmult is for insn
; representing multiple instruction that include vector

(define_attr "type"
	     "unknown,sop1,sop2,sopk,sopc,sopp,smem,dsmem,vop2,vop1,vopc,
	      vop3a,vop3b,vintr,lds,mubuf,mtbuf,exp,flat,mult,vmult"
	     (const_string "unknown"))

; Set if instruction is executed in scalar or vector unit

(define_attr "unit" "unknown,scalar,vector"
   (cond [(eq_attr "type" "sop1,sop2,sopk,sopc,sopp,smem,mult")
	    (const_string "scalar")
	  (eq_attr "type" "vop2,vop1,vopc,vop3a,vop3b,vintr,
			   mubuf,mtbuf,flat,vmult")
	    (const_string "vector")]
	 (const_string "unknown")))

; All vector instructions runs in 64bit threads which as predicated by EXEC
; registers.  EXEC register is assumed to be non-zero first time program starts.
; Because scalar operations are often also offloaded to the vector unit, we
; use modes switching to model three states of EXEC register:
;  - any:     Instruction do not care
;  - init:    Instruction must be executed with exec passed to function
;  - subinit: Instruction must be executed with non-zero EXEC which is subset
;	      of what was passed to function
;  - full:    Instruction expect exec to be all ones (for full sized vector ops)

(define_attr "exec" "any,unknown,init,subinit,user,full"
   (cond [(eq_attr "unit" "scalar")
	    (const_string "any")
	  (eq_attr "mode" "none,BI,QI,HI,SI,DI,TI,HF,SF,DF,V2SF,V3DI,V4SF,
			   V2DF,V3SF,V4SI,V2DI,V3SI")
	    (const_string "init")
	  (eq_attr "mode" "V64SI,V64DI,V64SF,V64DF,V64HF,V64HI,V64QI")
		(const_string "full")]
	 (const_string "unknown")))

;; -------------------------------------------------------------------------
;; Iterators useful across the wole machine description
;; -------------------------------------------------------------------------

; Scalar registers can generally be operated in SI or DI.
(define_mode_iterator SIDI_MODE [SI DI])
; Double reg vector operations
(define_mode_iterator V64_INT_MODE [DI V64DI])
; Default pointer is either 32bit or 64bit
;(define_mode_iterator P [(SI "Pmode == SImode") (DI "Pmode == DImode")])
;(define_mode_iterator V64P [(V64SI "Pmode == SImode") (V64DI "Pmode == DImode")])

(include "predicates.md")
(include "constraints.md")

;; -------------------------------------------------------------------------
;; Attibutes.
;; -------------------------------------------------------------------------

; Translate RTX code into GCN instruction mnemonics
(define_code_attr mnemonic
    [(minus "sub%i")
     (plus "add%i")
     (ashift "lshl%b")
     (lshiftrt "lshr%b")
     (ashiftrt "ashr%i")
     (and "and%b")
     (ior "or%b")
     (xor "xor%b")
     (mult "mul%i")
     (smin "min%i")
     (smax "max%i")
     (umin "min%u")
     (umax "max%u")])

(define_code_attr revmnemonic
    [(minus "subrev%i")
     (ashift "lshlrev%b")
     (lshiftrt "lshrrev%b")
     (ashiftrt "ashrrev%i")])

; Translate RTX code into corresponding expander name.
(define_code_attr expander
    [(and "and")
     (ior "ior")
     (xor "xor")
     (plus "add")
     (minus "sub")
     (ashift "ashl")
     (lshiftrt "lshr")
     (ashiftrt "ashr")
     (mult "mul")
     (smin "smin")
     (smax "smax")
     (umin "umin")
     (umax "umax")])

(define_mode_attr native_mode
  [(V64DI "V64SI") (DI "SI")])
;; -------------------------------------------------------------------------
;; nop instruction
;; -------------------------------------------------------------------------

(define_insn "nop"
  [(const_int 0)]
  ""
  "s_nop\t0x")

;; -------------------------------------------------------------------------
;; Trap
;; -------------------------------------------------------------------------

(define_insn "trap"
  [(trap_if (const_int 1) (const_int 0))]
  ""
  "s_trap")

;; -------------------------------------------------------------------------
;; Moves
;; -------------------------------------------------------------------------

; All modes GCN support move operation in
(define_mode_iterator S_MOV_MODE [BI SI DI SF DF V2SI V2SF V64SI V64DI])
; All modes GCN support move in single vector or scalar reg
(define_mode_iterator S_MOV1_MODE [BI SI SF])
; All modes GCN support move in pair of vector or scalar regs
(define_mode_iterator S_MOV2_MODE [DI DF])
; Loads and sctores can do 3,4,8,16 double words.
(define_mode_iterator S_MOV3_MODE [V3SI V3SF])
(define_mode_iterator S_MOV4_MODE [TI V4SI V4SF V2DI V2DF])
(define_mode_iterator S_MOV34_MODE [V3SI V3SF TI V4SI V4SF V2DI V2DF])

;; All modes we support moves in.
(define_mode_iterator ES_MOV_MODE [BI QI HI SI DI SF DF V2SI V2SF V64SI V64DI])
(define_expand "mov<mode>"
  [(set (match_operand:ES_MOV_MODE 0 "gcn_simple_mem_or_reg_operand")
	(match_operand:ES_MOV_MODE 1 "gcn_load_operand"))]
  ""
{
  if (gcn_expand_mov(<MODE>mode, operands[0], operands[1]))
    DONE;
})

; We need BImode move so we can reload flags registers

;(define_insn "*movbi"
  ;[(set (match_operand:BI 0 "register_operand" "=SD ,v")
	;;(match_operand:BI 1 "nonmemory_operand" "SSA,vSSA"))]
  ;""
  ;"@
   ;s_mov_b32\t%0, %1
   ;v_mov_b32\t%0, %1"
  ;[(set_attr "type" "sop1,vop1")
   ;(set_attr "mode" "SI")])

; 32bit move pattern

; FIXME: Ducumentation describe s_store_dword, but it is not accepted by
; LLVM AS
(define_insn "*mov<mode>_scalar"
  [(set (match_operand:S_MOV1_MODE 0 "register_operand" "=SD,SD, SD,Sm,v, Sm")
	(match_operand:S_MOV1_MODE 1 "gcn_load_operand" "SSA,SSJ,B, RS,SS,v"))]
  ""
  "@
   s_mov_b32\t%0, %1
   s_movk_i32\t%0, %1
   s_mov_b32\t%0, %1
   s_load_dword\t%0, %A1\n\ts_waitcnt\tlgkmcnt(0)
   v_writelane_b32\t%0, %1, 0
   v_readlane_b32\t%0, %1, 0"
  [(set_attr "type" "sop1,sopk,sop1,smem,vop3a,vop3a")
   (set_attr "mode" "SI")])

; FIXME: Why readfirstlane has both VOP1 and VOP3 encoding?
(define_insn_and_split "*mov<mode>"
  [(set (match_operand:S_MOV1_MODE 0 "nonimmediate_operand" "=SD, SD, SD,^Sm,v, v, Sm,^v,^Sm,RD,RF,v ,v ")
	(match_operand:S_MOV1_MODE 1 "gcn_load_operand"     " SSA,SSJ,B,  RS,vB,SS,v, SS,v  ,v ,v ,RD,RF"))
   (use (match_operand:DI 2 "gcn_exec_operand"              " n,  n,  n,  n, e, e, e, O, O  ,e ,e ,e ,e "))]
  "(register_operand (operands[0], VOIDmode) || register_operand (operands[1], VOIDmode))
   && (REG_P (operands[2]) || !gcn_vgpr_move_p (operands[0], operands[1]))"
  "@
   #
   #
   #
   #
   v_mov_b32\t%0, %1
   v_mov_b32\t%0, %1
   v_readfirstlane_b32\t%0, %1
   #
   #
   ds_write_b32\t%A0, %1%O0
   flat_store%s0\t%A0, %1
   ds_read_b32\t%0, %A1%O1
   flat_load_dword\t%0, %A1\n\ts_waitcnt\tlgkmcnt(0),vmcnt(0)"
  "gcn_sgpr_move_p (operands[0], operands[1])
   || (reload_completed && GET_CODE (operands[2]) == CONST_INT)"
  [(set (match_dup 0) (match_dup 1))]
{}
  [(set_attr "type" "sop1,sopk,sop1,smem,vop1,vop1,vop1,vop3a,vop3a,dsmem,flat,dsmem,flat")
   (set_attr "mode" "SI")])

(define_insn_and_split "*mov<mode>_scalar"
  [(set (match_operand:S_MOV2_MODE 0 "register_operand" "=SD,SD,Sm,v, Sm")
	(match_operand:S_MOV2_MODE 1 "general_operand" "SSB,SSn,RS,SS,v"))]
  ""
  "@
   s_mov%b0\t%0, %1
   #
   s_load%s0\t%0, %A1\n\ts_waitcnt\tlgkmcnt(0)
   #
   #"
   "(reload_completed && !gcn_sgpr_move_p (operands[0], operands[1]))
    || (GET_CODE (operands[1]) == CONST_INT && !gcn_constant_p (operands[1]))"
   [(set (match_dup 0) (match_dup 1))
    (set (match_dup 2) (match_dup 3))]
{
  gcn_split_operands (<MODE>mode, operands, 2, 2);
}
  [(set_attr "type" "sop1,mult,smem,vmult,vmult")
   (set_attr "mode" "DI,SI,DI,SI,SI")])

(define_insn_and_split "*mov<mode>_scalar"
  [(set (match_operand:S_MOV2_MODE 0 "nonimmediate_operand" "=SD,SD,Sm,v, v ,^v, ^Sm,RD,RF,v ,v ")
	(match_operand:S_MOV2_MODE 1 "general_operand"      "SSB,SSn,RS,vn,SS, SS,v ,v ,v ,RD,RF"))
   (use (match_operand:DI 2 "gcn_exec_operand" "n,n,n,e,e,O,O,e,e,e,e"))]
  "(register_operand (operands[0], VOIDmode) || register_operand (operands[1], VOIDmode))
   && (REG_P (operands[2]) || !gcn_vgpr_move_p (operands[0], operands[1]))"
  "@
   #
   #
   #
   #
   #
   #
   #
   ds_write_b64\t%A0, %1%O0
   flat_store%s0\t%A0, %1
   ds_read_b64\t%0, %A1%O1
   flat_load_dword\t%0, %A1\n\ts_waitcnt\tlgkmcnt(0),vmcnt(0)"
  "gcn_sgpr_move_p (operands[0], operands[1])
   || (reload_completed && GET_CODE (operands[2]) == CONST_INT)"
  [(set (match_dup 0) (match_dup 1))]
{}
  [(set_attr "type" "sop1,mult,smem,vmult,vmult,vmult,vmult,dsmem,flat,dsmem,flat")
   (set_attr "mode" "DI,SI,DI,SI,SI,SI,SI,DI,DI,DI,DI")])


; Split to move by pieces but be sure that we do not split s_mov_b64
(define_split
  [(set (match_operand:S_MOV2_MODE 0 "register_operand")
	(match_operand:S_MOV2_MODE 1 "nonmemory_operand"))
   (use (match_operand:DI 2 "register_operand"))]
  "gcn_vgpr_register_operand (operands[0], VOIDmode)
   || gcn_vgpr_register_operand (operands[1], VOIDmode)"
  [(parallel [(set (match_dup 0) (match_dup 1)) (use (match_dup 4))])
   (parallel [(set (match_dup 2) (match_dup 3)) (use (match_dup 4))])]
{
  operands[4] = operands[2];
  gcn_split_operands (<MODE>mode, operands, 2, 2);
})

(define_insn "*mov<mode>"
  [(set (match_operand:S_MOV34_MODE 0 "register_operand" "=SD,SD, SD,Sm,v, ^v, ^Sm")
	(match_operand:S_MOV34_MODE 1 "gcn_load_operand" "SSA,SSJ,B, RS,vB, SS, v"))
   (use (match_operand:DI 2 "gcn_exec_operand" "n,n,n,n,e,e,n"))]
  ""
  "@
   #
   #
   #
   s_load%s0\t%0, %A1\n\ts_waitcnt\tlgkmcnt(0)
   #
   #
   #"
  [(set_attr "type" "mult,mult,mult,smem,vmult,vmult,vmult")
   (set_attr "mode" "DI,SI,SI,<MODE>,SI,SI,SI")])

; Watch for partial overlap - register triples are not aligned.
(define_split
  [(set (match_operand:S_MOV3_MODE 0 "register_operand")
	(match_operand:S_MOV3_MODE 1 "nonmemory_operand"))]
  "(REG_P (operands[0]) && REG_P (operands[1]))
    && REGNO (operands[0]) > REGNO (operands[1])
    && REGNO (operands[0]) < REGNO (operands[1] + 3)"
  [(set (match_dup 0) (match_dup 1))
   (set (match_dup 2) (match_dup 3))
   (set (match_dup 4) (match_dup 5))]
{
  gcn_split_operands (<MODE>mode, operands, 3, 2);
})


(define_split
  [(set (match_operand:S_MOV3_MODE 0 "register_operand")
	(match_operand:S_MOV3_MODE 1 "nonmemory_operand"))]
  "reload_completed"
  [(set (match_dup 0) (match_dup 1))
   (set (match_dup 2) (match_dup 3))]
{
  operands[2] = gcn_operand_part (GET_MODE (operands[0]), operands[0], 2);
  operands[3] = gcn_operand_part (GET_MODE (operands[0]), operands[1], 2);
  operands[0] = gcn_operand_doublepart
		     (GET_MODE (operands[0]), operands[0], 0);
  operands[1] = gcn_operand_doublepart
		     (GET_MODE (operands[0]), operands[1], 0);
})

(define_split
  [(set (match_operand:S_MOV4_MODE 0 "register_operand")
	(match_operand:S_MOV4_MODE 1 "nonmemory_operand"))]
  "reload_completed"
  [(set (match_dup 0) (match_dup 1))
   (set (match_dup 2) (match_dup 3))]
{
  operands[2] = gcn_operand_doublepart (GET_MODE (operands[0]), operands[0], 1);
  operands[3] = gcn_operand_doublepart (GET_MODE (operands[0]), operands[1], 1);
  operands[0] = gcn_operand_doublepart (GET_MODE (operands[0]), operands[0], 0);
  operands[1] = gcn_operand_doublepart (GET_MODE (operands[0]), operands[1], 0);
})

;; -------------------------------------------------------------------------
;; Prologue/Epilogue
;; -------------------------------------------------------------------------

(define_insn "prologue_use"
  [(unspec_volatile [(match_operand 0)] UNSPECV_PROLOGUE_USE)]
  ""
  "")

;; Flat scratch initialization.
(define_expand "prologue"
  [(const_int 0)]
  ""
{
   gcn_expand_prologue ();
})

(define_expand "epilogue"
  [(simple_return)])

;; -------------------------------------------------------------------------
;; Control flow
;; -------------------------------------------------------------------------

(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0)))]
  ""
  "s_branch\t%0"
  [(set_attr "type" "sopp")])

(define_insn "cjump"
  [(set (pc)
	(if_then_else (match_operator:BI 1 "gcn_conditional_operator"
			  [(match_operand:BI
				 2 "gcn_conditional_register_operand" "ca")
			   (const_int 0)])
		      (label_ref (match_operand 0))
		      (pc)))]
  ""
  "s_cbranch%C1\t%0"
  [(set_attr "type" "sopp")])

(define_insn "return"
  [(simple_return)]
  ""
  "s_endpgm"
  [(set_attr "type" "sopp")])

;; -------------------------------------------------------------------------
;; Conditionals
;; -------------------------------------------------------------------------

(define_insn "cstoresi4"
  [(set (match_operand:BI 0 "gcn_conditional_register_operand" "=cs,cs,cs,cs")
	(match_operator:BI 1 "gcn_compare_operator"
            [(match_operand:SI 2 "gcn_alu_operand" "SSA,SSA,SSB,SS")
	     (match_operand:SI 3 "gcn_alu_operand" "SSA,SSK,SS ,SSB")]))]
  ""
  "@
   s_cmp%D1\t%2, %3
   s_cmpk%D1\t%2, %3
   s_cmp%D1\t%2, %3
   s_cmp%D1\t%2, %3"
  [(set_attr "type" "sopc,sopk,sopk,sopk")
   (set_attr "mode" "SI")])

(define_expand "cbranchsi4"
  [(match_operator 0 "gcn_compare_operator"
          [(match_operand:SI 1 "gcn_alu_operand")
	   (match_operand:SI 2 "gcn_alu_operand")])
   (match_operand 3)]
  ""
{
  rtx cc = gen_reg_rtx (BImode);
  emit_insn (gen_cstoresi4 (cc, operands[0], operands[1], operands[2]));
  emit_jump_insn (gen_cjump (operands[3], gen_rtx_NE (BImode, cc, const0_rtx),
		  cc));
  DONE;
})

; FIXME: s_cmp_eq_64 is not accepted by llvm-as.

(define_insn "cstoredi4_vec_and_scalar"
  [(set (match_operand:BI 0 "gcn_conditional_register_operand" "=cs,cv,cv")
	(match_operator:BI 1 "gcn_compare_64bit_operator"
            [(match_operand:DI 2 "gcn_alu_operand" "%SSA,vSS,v")
	     (match_operand:DI 3 "gcn_alu_operand" " SSB,v,  vB")]))
   (use (match_operand:DI 4 "gcn_exec_operand" "n,e,e"))]
  "0"
  "@
   #
   v_cmp%E1\tvcc, %3, %2
   v_cmp%E1\tvcc, %3, %2"
  [(set_attr "type" "unknown,vopc,vopc")
   (set_attr "mode" "DI")])

(define_split
  [(set (match_operand:BI 0 "gcn_conditional_register_operand" "")
	(match_operator:BI 1 "gcn_compare_64bit_operator"
            [(match_operand:DI 2 "gcn_alu_operand" "")
	     (match_operand:DI 3 "gcn_alu_operand" "")]))
   (use (match_operand:DI 4 "" ""))]
  "REG_P (operands[0]) && REGNO (operands[0]) == SCC_REG && 0"
  [(set (match_dup 0)
        (match_op_dup 1 [(match_dup 2) (match_dup 3)]))])

(define_insn "cstoredi4_scalar"
  [(set (match_operand:BI 0 "gcn_conditional_register_operand" "=cs")
	(match_operator 1 "gcn_compare_64bit_operator"
            [(match_operand:DI 2 "gcn_alu_operand" "%SSA")
	     (match_operand:DI 3 "gcn_alu_operand" "SSB")]))]
  "0"
  "s_cmp%D1\t%2, %3"
  [(set_attr "type" "vopc")
   (set_attr "mode" "DI")])

(define_insn "cstoredi4"
  [(set (match_operand:BI 0 "gcn_conditional_register_operand" "=cv")
	(match_operator:BI 1 "gcn_compare_operator"
            [(match_operand:DI 2 "gcn_alu_operand" "vSSA")
	     (match_operand:DI 3 "gcn_alu_operand" "v")]))
   (use (match_operand:DI 4 "gcn_exec_operand" "e"))]
  ""
  "v_cmp%E1\tvcc, %2, %3"
  [(set_attr "type" "vopc")
   (set_attr "mode" "DI")])

(define_expand "cbranchdi4"
  [(match_operator 0 "gcn_compare_operator"
          [(match_operand:DI 1 "gcn_alu_operand")
	   (match_operand:DI 2 "gcn_alu_operand")])
   (match_operand 3)]
  ""
{
  rtx cc = gen_reg_rtx (BImode);
  emit_insn (gen_cstoredi4 (cc, operands[0], operands[1], operands[2],
			    gcn_scalar_exec ()));
  emit_jump_insn (gen_cjump (operands[3], gen_rtx_NE (BImode, cc, const0_rtx),
		  cc));
  DONE;
})

;; -------------------------------------------------------------------------
;; ALU special cases: Plus
;; -------------------------------------------------------------------------

(define_code_iterator plus_minus [plus minus])

(define_expand "<expander>si3"
  [(parallel [(set (match_operand:SI 0 "register_operand")
		   (plus_minus:SI
			    (match_operand:SI 1 "gcn_alu_operand")
			    (match_operand:SI 2 "gcn_alu_operand")))
	      (use (match_dup 3))
	      (clobber (reg:BI SCC_REG))
	      (clobber (reg:CC VCC_REG))])]
  ""
{
  operands[3] = gcn_scalar_exec ();
})

(define_insn "*addsi3_vec_and_scalar"
  [(set (match_operand:SI 0 "register_operand"         "=SD, SD, SD, v")
        (plus:SI
		 (match_operand:SI 1 "gcn_alu_operand" "%SSA,0  ,SSA,v")
		 (match_operand:SI 2 "gcn_alu_operand" " SSA,SSJ,B,  vBSS")))
   (use (match_operand:DI 3 "gcn_exec_operand" "n,n,n,v"))
   (clobber (reg:BI SCC_REG))
   (clobber (reg:CC VCC_REG))]
  ""
  "#")

(define_predicate "plus_minus_operator"
  (match_code "plus,minus"))

(define_split
  [(set (match_operand:SIDI_MODE 0 "register_operand" "")
        (match_operator:SIDI_MODE 3 "plus_minus_operator"
			 [(match_operand:SIDI_MODE 1 "gcn_alu_operand" "")
		          (match_operand:SIDI_MODE 2 "gcn_alu_operand" "")]))
   (use (match_operand:DI 4 "" ""))
   (clobber (reg:BI SCC_REG))
   (clobber (reg:CC VCC_REG))]
  "gcn_sgpr_register_operand (operands[0], VOIDmode)"
  [(parallel [(set (match_dup 0) (match_op_dup 3 [(match_dup 1) (match_dup 2)]))
	      (clobber (reg:BI SCC_REG))])])

(define_split
  [(set (match_operand:SIDI_MODE 0 "register_operand" "")
        (match_operator:SIDI_MODE 3 "plus_minus_operator"
			 [(match_operand:SIDI_MODE 1 "gcn_alu_operand" "")
		          (match_operand:SIDI_MODE 2 "gcn_alu_operand" "")]))
   (use (match_operand:DI 4 "" ""))
   (clobber (reg:BI SCC_REG))
   (clobber (reg:CC VCC_REG))]
  "gcn_vgpr_register_operand (operands[0], VOIDmode)"
  [(parallel [(set (match_dup 0) (match_op_dup 3 [(match_dup 1) (match_dup 2)]))
	      (use (match_dup 4))
	      (clobber (reg:CC VCC_REG))])])

(define_insn "*addsi3_scalar"
  [(set (match_operand:SI 0 "register_operand"         "=SD, SD, SD")
        (plus:SI
		 (match_operand:SI 1 "gcn_alu_operand" "%SSA,0  ,SSA")
		 (match_operand:SI 2 "gcn_alu_operand" " SSA,SSJ,B")))
   (clobber (reg:BI SCC_REG))]
  ""
  "@
    s_add_i32\t%0, %1, %2
    s_addk_i32\t%0, %2
    s_add_i32\t%0, %1, %2"
  [(set_attr "type" "sop2,sopk,sop2")
   (set_attr "mode" "SI")])


; FIXME: Implemented for now only in SCC registers.  Vectors are analogous
; but we need to expand into vector patterns.
(define_expand "adddi3"
  [(match_operand:DI 0 "register_operand")
   (match_operand:DI 1 "gcn_alu_operand")
   (match_operand:DI 2 "gcn_alu_operand")]
  ""
{
  emit_insn (gen_addsi3_scalar_carry (
		gcn_operand_part (DImode, operands[0], 0),
		gcn_operand_part (DImode, operands[1], 0),
		gcn_operand_part (DImode, operands[2], 0)));
  rtx val = gcn_operand_part (DImode, operands[2], 1);
  if (val != const0_rtx)
    emit_insn (gen_addcsi3_scalar (
		  gcn_operand_part (DImode, operands[0], 1),
		  gcn_operand_part (DImode, operands[1], 1),
		  gcn_operand_part (DImode, operands[2], 1)));
  else
    emit_insn (gen_addcsi3_scalar_zero (
		  gcn_operand_part (DImode, operands[0], 1),
		  gcn_operand_part (DImode, operands[1], 1)));
  DONE;
})

(define_insn "addsi3_scalar_carry"
  [(set (match_operand:SI 0 "register_operand"         "=SD")
        (plus:SI (match_operand:SI 1 "gcn_alu_operand" "%SSA")
		 (match_operand:SI 2 "gcn_alu_operand" "SSB")))
   (set (reg:BI SCC_REG)
	(ltu:BI (plus:SI (match_dup 1)
			 (match_dup 2))
		(match_dup 1)))]
  ""
  "s_add_u32\t%0, %1, %2"
  [(set_attr "type" "sop2")
   (set_attr "mode" "SI")])

(define_insn "addsi3_scalar_carry_cst"
  [(set (match_operand:SI 0 "register_operand"         "=SD")
        (plus:SI (match_operand:SI 1 "gcn_alu_operand" "SSA")
		 (match_operand:SI 2 "const_int_operand" "n")))
   (set (reg:BI SCC_REG)
	(geu:BI (plus:SI (match_dup 1)
			 (match_dup 2))
		(match_operand:SI 3 "const_int_operand" "n")))]
  "INTVAL (operands[2]) == -INTVAL (operands[3])"
  "s_add_u32\t%0, %1, %2"
  [(set_attr "type" "sop2")
   (set_attr "mode" "SI")])

(define_insn "addcsi3_scalar"
  [(set (match_operand:SI 0 "register_operand"                  "=SD")
        (plus:SI (plus:SI (zero_extend:SI (reg:BI SCC_REG))
			  (match_operand:SI 1 "gcn_alu_operand" "%SSA"))
		 (match_operand:SI 2 "gcn_alu_operand"          "SSB")))
   (set (reg:BI SCC_REG)
	(ior:BI (ltu:BI (plus:SI (plus:SI (zero_extend:SI (reg:BI SCC_REG))
					 (match_dup 1))
				(match_dup 2))
		       (match_dup 2))
	        (ltu:BI (plus:SI (zero_extend:SI (reg:BI SCC_REG)) (match_dup 1))
		        (match_dup 1))))]
  ""
  "s_addc_u32\t%0, %1, %2"
  [(set_attr "type" "sop2")
   (set_attr "mode" "SI")])

(define_insn "addcsi3_scalar_zero"
  [(set (match_operand:SI 0 "register_operand" "=SD")
        (plus:SI (zero_extend:SI (reg:BI SCC_REG))
		 (match_operand:SI 1 "gcn_alu_operand" "SSA")))
   (set (reg:BI SCC_REG)
	(ltu:BI (plus:SI (zero_extend:SI (reg:BI SCC_REG)) (match_dup 1))
		(match_dup 1)))]
  ""
  "s_addc_u32\t%0, %1, 0"
  [(set_attr "type" "sop2")
   (set_attr "mode" "SI")])

;; -------------------------------------------------------------------------
;; ALU special cases: Minus
;; -------------------------------------------------------------------------

(define_expand "subdi3"
  [(match_operand:DI 0 "register_operand")
   (match_operand:DI 1 "gcn_alu_operand")
   (match_operand:DI 2 "gcn_alu_operand")]
  ""
{
  emit_insn (gen_subsi3_scalar_carry (
		gcn_operand_part (DImode, operands[0], 0),
		gcn_operand_part (DImode, operands[1], 0),
		gcn_operand_part (DImode, operands[2], 0)));
  rtx val = gcn_operand_part (DImode, operands[2], 1);
  if (val != const0_rtx)
    emit_insn (gen_subcsi3_scalar (
		  gcn_operand_part (DImode, operands[0], 1),
		  gcn_operand_part (DImode, operands[1], 1),
		  gcn_operand_part (DImode, operands[2], 1)));
  else
    emit_insn (gen_subcsi3_scalar_zero (
		  gcn_operand_part (DImode, operands[0], 1),
		  gcn_operand_part (DImode, operands[1], 1)));
  DONE;
})

(define_insn "subsi3_scalar_carry"
  [(set (match_operand:SI 0 "register_operand"          "=SD, SD")
        (minus:SI (match_operand:SI 1 "gcn_alu_operand" "SSA,SSB")
		  (match_operand:SI 2 "gcn_alu_operand" "SSB,SSA")))
   (set (reg:BI SCC_REG)
	(gtu:BI (minus:SI (match_dup 1)
			  (match_dup 2))
		(match_dup 1)))]
  ""
  "s_sub_u32\t%0, %1, %2"
  [(set_attr "type" "sop2")
   (set_attr "mode" "SI")])

(define_insn "subsi3_scalar_carry_cst"
  [(set (match_operand:SI 0 "register_operand"         "=SD")
        (minus:SI (match_operand:SI 1 "gcn_alu_operand" "SSA")
		 (match_operand:SI 2 "const_int_operand" "n")))
   (set (reg:BI SCC_REG)
	(leu:BI (minus:SI (match_dup 1)
			 (match_dup 2))
		(match_operand:SI 3 "const_int_operand" "n")))]
  "INTVAL (operands[2]) == -INTVAL (operands[3])"
  "s_sub_u32\t%0, %1, %2"
  [(set_attr "type" "sop2")
   (set_attr "mode" "SI")])

(define_insn "subcsi3_scalar"
  [(set (match_operand:SI 0 "register_operand"                    "=SD, SD")
        (minus:SI (minus:SI (zero_extend:SI (reg:BI SCC_REG))
			    (match_operand:SI 1 "gcn_alu_operand" "SSA,SSB"))
		 (match_operand:SI 2 "gcn_alu_operand"            "SSB,SSA")))
   (set (reg:BI SCC_REG)
	(ior:BI (gtu:BI (minus:SI (minus:SI (zero_extend:SI (reg:BI SCC_REG))
					    (match_dup 1))
				 (match_dup 2))
			(match_dup 1))
	        (gtu:BI (minus:SI (zero_extend:SI (reg:BI SCC_REG)) (match_dup 1))
		        (match_dup 1))))]
  ""
  "s_subb_u32\t%0, %1, %2"
  [(set_attr "type" "sop2")
   (set_attr "mode" "SI")])

(define_insn "subcsi3_scalar_zero"
  [(set (match_operand:SI 0 "register_operand" "=SD")
        (minus:SI (zero_extend:SI (reg:BI SCC_REG))
		  (match_operand:SI 1 "gcn_alu_operand" "SSA")))
   (set (reg:BI SCC_REG)
	(gtu:BI (minus:SI (zero_extend:SI (reg:BI SCC_REG)) (match_dup 1))
		(match_dup 1)))]
  ""
  "s_subb_u32\t%0, %1, 0"
  [(set_attr "type" "sop2")
   (set_attr "mode" "SI")])

;; -------------------------------------------------------------------------
;; ALU: mult
;; -------------------------------------------------------------------------

(define_expand "mulsi3"
  [(set (match_operand:SI 0 "register_operand")
        (mult:SI
		 (match_operand:SI 1 "gcn_alu_operand")
		 (match_operand:SI 2 "gcn_alu_operand")))
   (use (match_dup 3))]
""
{
  operands[3] = gcn_scalar_exec ();
})

; Vector multiply has vop3a encoding, but no corresponding vop2a, so no long
; immediate.
(define_insn_and_split "*mulsi3_vec_and_scalar"
  [(set (match_operand:SI 0 "register_operand"         "=SD, SD,SD, v")
        (mult:SI
		 (match_operand:SI 1 "gcn_alu_operand" "%SSA,0 ,SSA,v")
		 (match_operand:SI 2 "gcn_alu_operand" " SSA,J, B,  vASS")))
   (use (match_operand:DI 3 "gcn_exec_operand" "n,n,n,e"))]
  ""
  "@
    #
    #
    #
    v_mul_lo_i32\t%0, %1, %2"
  "gcn_sgpr_register_operand (operands[0], VOIDmode)"
  [(set (match_operand:SI 0 "register_operand"         "=SD, SD,SD, v")
        (mult:SI
		 (match_operand:SI 1 "gcn_alu_operand" "%SSA,0 ,SSA,vSS")
		 (match_operand:SI 2 "gcn_alu_operand" " SSA,J, B,  vA")))]
{}
  [(set_attr "type" "sop2,sopk,sop2,vop3a")
   (set_attr "mode" "SI")])

(define_insn "*mulsi3_scalar"
  [(set (match_operand:SI 0 "register_operand"         "=SD, SD,SD")
        (mult:SI
		 (match_operand:SI 1 "gcn_alu_operand" "%SSA,0 ,SSA")
		 (match_operand:SI 2 "gcn_alu_operand" " SSA,J, B")))]
  ""
  "@
    s_mul_i32\t%0, %1, %2
    s_mulk_i32\t%0, %2
    s_mul_i32\t%0, %1, %2"
  [(set_attr "type" "sop2,sopk,sop2")
   (set_attr "mode" "SI")])

;; -------------------------------------------------------------------------
;; ALU the generic 32bit case
;; -------------------------------------------------------------------------

(define_code_iterator vec_and_scalar [and ior xor ashift lshiftrt
				      ashiftrt smin smax umin umax])

(define_expand "<expander>si3"
  [(parallel [(set (match_operand:SI 0 "register_operand")
	           (vec_and_scalar:SI
			 (match_operand:SI 1 "gcn_alu_operand")
		         (match_operand:SI 2 "gcn_alu_operand")))
	      (use (match_dup 3))
	      (clobber (reg:BI SCC_REG))])]
  ""
{
  operands[3] = gcn_scalar_exec ();
})

;; No plus and mult - they have variant with 16bit immediate and thus are defined later.
(define_code_iterator vec_and_scalar_com [and ior xor smin smax umin umax])

(define_insn "*<expander>si3"
  [(set (match_operand:SI 0 "register_operand" "=SD,v")
        (vec_and_scalar_com:SI (match_operand:SI 1 "gcn_alu_operand" "%SSA,v")
			       (match_operand:SI 2 "gcn_alu_operand" "SSB,vSSB")))
   (use (match_operand:DI 3 "gcn_exec_operand" "n,e"))
   (clobber (reg:BI SCC_REG))]
  ""
  "#"
  [(set_attr "type" "sop2,vop2")
   (set_attr "mode" "SI")])

(define_insn "*<expander>si3_scalar"
  [(set (match_operand:SI 0 "register_operand" "=SD,v")
        (vec_and_scalar_com:SI (match_operand:SI 1 "register_operand" "%SSA,v")
			       (match_operand:SI 2 "gcn_alu_operand" "SSB,vSSB")))
   (clobber (reg:BI SCC_REG))]
  ""
  "s_<mnemonic>0\t%0, %1, %2"
  [(set_attr "type" "sop2")
   (set_attr "mode" "SI")])

(define_code_iterator vec_and_scalar_nocom [ashift lshiftrt ashiftrt])

(define_insn "*<expander>si3_vec_and_scalar"
  [(set (match_operand:SI 0 "register_operand" "=SD,SD,v")
        (vec_and_scalar_nocom:SI (match_operand:SI 1 "gcn_alu_operand" "SSB,SSA,v")
				 (match_operand:SI 2 "gcn_alu_operand" "SSA,SSB,vSSB")))
   (use (match_operand:DI 3 "gcn_exec_operand" "n,n,e"))
   (clobber (reg:BI SCC_REG))]
  ""
  "#"
  [(set_attr "type" "sop2,sop2,vop2")
   (set_attr "mode" "SI")])

(define_insn "<expander>si3_scalar"
  [(set (match_operand:SI 0 "register_operand" "=SD,SD")
        (vec_and_scalar_nocom:SI (match_operand:SI 1 "gcn_alu_operand" "SSB,SSA")
				 (match_operand:SI 2 "gcn_alu_operand" "SSA,SSB")))
   (clobber (reg:BI SCC_REG))]
  ""
  "@
   s_<mnemonic>0\t%0, %1, %2
   s_<mnemonic>0\t%0, %1, %2"
  [(set_attr "type" "sop2,sop2")
   (set_attr "mode" "SI")])

;; -------------------------------------------------------------------------
;; ALU the generic 64bit case
;; -------------------------------------------------------------------------

(define_code_iterator vec_and_scalar64_com [and ior xor])

(define_expand "<expander>di3"
  [(set (match_operand:DI 0 "register_operand")
        (vec_and_scalar64_com:DI (match_operand:DI 1 "gcn_alu_operand")
				 (match_operand:DI 2 "gcn_alu_operand")))
   (use (match_dup 3))
   (clobber (reg:BI SCC_REG))]
  ""
{
  operands[3] = gcn_scalar_exec ();
})

(define_insn "*<expander>di3_vec_and_scalar"
  [(set (match_operand:DI 0 "register_operand" "=SD,v")
        (vec_and_scalar64_com:DI (match_operand:DI 1 "register_operand" "%SSA,v")
				 (match_operand:DI 2 "gcn_alu_operand" "SSB,vSSB")))
   (use (match_operand:DI 3 "gcn_exec_operand" "n,e"))
   (clobber (reg:BI SCC_REG))]
  ""
  "#"
  [(set_attr "type" "sop2,vop2")
   (set_attr "mode" "DI")])

(define_insn "*<expander>di3_scalar"
  [(set (match_operand:DI 0 "register_operand" "=SD")
        (vec_and_scalar64_com:DI (match_operand:DI 1 "register_operand" "%SSA")
				 (match_operand:DI 2 "gcn_alu_operand" "SSB")))
   (clobber (reg:BI SCC_REG))]
  ""
  "s_<mnemonic>0\t%0, %1, %2"
  [(set_attr "type" "sop2")
   (set_attr "mode" "DI")])

(define_expand "<expander>di3"
  [(set (match_operand:DI 0 "register_operand")
        (vec_and_scalar_nocom:DI (match_operand:DI 1 "gcn_alu_operand")
				 (match_operand:DI 2 "gcn_alu_operand")))
   (use (match_dup 3))
   (clobber (reg:BI SCC_REG))]
  ""
{
  operands[3] = gcn_scalar_exec ();
})

(define_insn "*<expander>di3_vec_and_scalar"
  [(set (match_operand:DI 0 "register_operand" "=SD,SD,v")
        (vec_and_scalar_nocom:DI
		    (match_operand:DI 1 "gcn_alu_operand" "SSB,SSA,v")
		    (match_operand:SI 2 "gcn_alu_operand" "SSA,SSB,vSSB")))
   (clobber (reg:BI SCC_REG))]
  ""
  "s_<mnemonic>0\t%0, %1, %2"
  [(set_attr "type" "sop2,sop2,vop2")
   (set_attr "mode" "DI")])

(define_insn "*<expander>di3_scalar"
  [(set (match_operand:DI 0 "register_operand" "=SD,SD")
        (vec_and_scalar_nocom:DI
		    (match_operand:DI 1 "gcn_alu_operand" "SSB,SSA")
		    (match_operand:SI 2 "gcn_alu_operand" "SSA,SSB")))
   (clobber (reg:BI SCC_REG))]
  ""
  "s_<mnemonic>0\t%0, %1, %2"
  [(set_attr "type" "sop2,sop2")
   (set_attr "mode" "DI")])

;; -------------------------------------------------------------------------
;; Generic splitters choosing proper insn variant once we decided on using
;; vector or scalar ALU
;; -------------------------------------------------------------------------

(define_split
  [(set (match_operand 0 "gcn_sgpr_register_operand")
        (match_operator 4 "binary_operator"
			     [(match_operand 1 "gcn_alu_operand")
			      (match_operand 2 "gcn_alu_operand")]))
   (use (match_operand:DI 3 ""))
   (clobber (reg:BI SCC_REG))]
  ""
  [(parallel [(set (match_dup 0)
		   (match_op_dup 4 [(match_dup 1) (match_dup 2)]))
              (clobber (reg:BI SCC_REG))])])

(define_split
  [(set (match_operand 0 "gcn_vgpr_register_operand")
        (match_operator 4 "binary_operator"
			     [(match_operand 1 "gcn_alu_operand")
			      (match_operand 2 "gcn_alu_operand")]))
   (use (match_operand:DI 3 ""))
   (clobber (reg:BI SCC_REG))]
  ""
  [(parallel [(set (match_dup 0)
		   (match_op_dup 4 [(match_dup 1) (match_dup 2)]))
              (use (match_dup 3))])])


(include "gcn-valu.md")
