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

;; -------------------------------------------------------------------------
;; Moves
; Vector modes for one vector register
(define_mode_iterator VEC_1REG_MODE
		      [V64QI V64HI V64SI V64HF V64SF])

(define_mode_iterator VEC_1REG_INT_MODE
		      [V64QI V64HI V64SI])

; Vector modes for two vector registers
(define_mode_iterator VEC_2REG_MODE
		      [V64DI V64DF])

; All of above
(define_mode_iterator VEC_REG_MODE
		       [V64QI V64HI V64SI V64HF V64SF   ; Single reg
		        V64DI V64DF])		        ; Double reg

; Modes supporting integer vector operations
(define_mode_iterator V_INT_MODE [SI V64SI])

; Modes we can perform flat memory operations in.
; FIXME: We can also do 96bit, 128bit and 256bit loads into multiple
; registers.  Eventually add modes for this.
(define_mode_iterator VEC_FLAT_MODE
		      [V64QI V64HI V64SI V64HF V64SF 	; Single reg
		       V64DI V64DF])			; Double reg

; Modes we can perform scalar flat memory operations in.
(define_mode_iterator SCALAR_FLAT_MODE
		      [BI QI HI HF SI SF                   ; single regs
		       DI DF V2SI V2SF                     ; two regs
		       V3SI V3SF                           ; three regs
		       TI V4SI V4SF V2DI V2DF])            ; four regs

;; Mapping of full vector modes to shorter vectors
(define_mode_attr scalar_mode
  [(V64QI "QI") (V64HI "HI") (V64SI "SI")
   (V64HF "HF") (V64SF "SF") (V64DI "DI") (V64DF "DF")])

(define_mode_attr v2_mode
  [(V64QI "V2QI") (V64HI "V2HI") (V64SI "V2SI")
   (V64HF "V2HF") (V64SF "V2SF") (V64DI "V2DI") (V64DF "V2DF")])

(define_mode_attr v4_mode
  [(V64QI "V4QI") (V64HI "V4HI") (V64SI "V4SI")
   (V64HF "V4HF") (V64SF "V4SF") (V64DI "V4DI") (V64DF "V4DF")])

(define_mode_attr v8_mode
  [(V64QI "V8QI") (V64HI "V8HI") (V64SI "V8SI")
   (V64HF "V8HF") (V64SF "V8SF") (V64DI "V8DI") (V64DF "V8DF")])

(define_mode_attr v16_mode
  [(V64QI "V16QI") (V64HI "V16HI") (V64SI "V16SI")
   (V64HF "V16HF") (V64SF "V16SF") (V64DI "V16DI") (V64DF "V16DF")])

(define_mode_attr v32_mode
  [(V64QI "V32QI") (V64HI "V32HI") (V64SI "V32SI")
   (V64HF "V32HF") (V64SF "V32SF") (V64DI "V32DI") (V64DF "V32DF")])

(define_insn "*simple_buffer_mov<mode>"
  [(set (match_operand:SCALAR_FLAT_MODE 0 "nonimmediate_operand" "=RBm,v")
	(match_operand:SCALAR_FLAT_MODE 1 "general_operand"      "v,   RBm"))
   (use (match_operand:TI 2 "register_operand"                   "Sg,  Sg"))
   (use (match_operand:DI 3 "gcn_exec_reg_operand"                   "e,   e"))]
  "memory_operand (operands[0], VOIDmode) != memory_operand (operands[1], VOIDmode)"
  "@
   buffer_store%s1 %1, off, %2, %A0\n\ts_waitcnt vmcnt(0) expcnt(0)
   buffer_load%s0 %0, off, %2, %A1\n\ts_waitcnt vmcnt(0)"
  [(set_attr "type" "flat")
   (set_attr "mode" "<MODE>")])

;; This ethernal uglyness makes LRA to sort-of work.
(define_insn_and_split "*ugly_reload_mov<mode>"
  [(set (match_operand:SCALAR_FLAT_MODE 0 "nonimmediate_operand" "=RBm,v,  ^RBm,^v,   SD, SD, v,   v , ^v,  ^v , v,  Sm, RDRF,v")
	(match_operand:SCALAR_FLAT_MODE 1 "general_operand"      "v,   RBm, v,   RBm, SSB,SSn,vn,  SS,  vn,  SS, SS, vSS,v   ,RDRF"))
   (use (match_operand:TI 2 "register_operand"                   "Sg,  Sg,  Sg,  Sg,  Sg, Sg, Sg,  Sg,  Sg,  Sg, Sg, Sg, Sg  ,Sg"))
   (use (match_operand:DI 3 "gcn_exec_operand"                   "e   ,e   ,Sgn, Sgn, SSn,SSn,e   ,e   ,Sgn, Sgn,SSO,SSO,e   ,e"))
   (clobber (match_operand:DI 4 "register_operand" "=&Sg,Sg,Sg,Sg,Sg,Sg,Sg,Sg,Sg,Sg,Sg,Sg,Sg,Sg"))]
  "(!immediate_operand (operands[1], VOIDmode) || register_operand (operands[0], VOIDmode))
   && (!memory_operand (operands[0], VOIDmode) || !memory_operand (operands[1], VOIDmode))"
  "#"
  "!memory_operand (operands[0],<MODE>mode) && !memory_operand (operands[1],<MODE>mode)
   && !gcn_vgpr_register_operand (operands[0], <MODE>mode)
   && !gcn_vgpr_register_operand (operands[1], <MODE>mode)"
  [(set (match_dup 0) (match_dup 1))]
{}
  [(set_attr "type" "flat")
   (set_attr "mode" "<MODE>")])

(define_split
  [(set (match_operand:SCALAR_FLAT_MODE 0 "nonimmediate_operand")
	(match_operand:SCALAR_FLAT_MODE 1 "general_operand"))
   (use (match_operand:TI 2 "register_operand"))
   (use (match_operand:DI 3 "gcn_exec_operand"))
   (clobber (match_scratch:DI 4 ""))]
  "REG_P (operands[3]) && REGNO (operands[3]) == EXEC_REG
   && (memory_operand (operands[0], VOIDmode) || memory_operand (operands[1], VOIDmode))"
  [(parallel [(set (match_dup 0) (match_dup 1))
              (use (match_dup 2))
              (use (match_dup 3))])])

(define_split
  [(set (match_operand:SCALAR_FLAT_MODE 0 "nonimmediate_operand")
	(match_operand:SCALAR_FLAT_MODE 1 "general_operand"))
   (use (match_operand:TI 2 "register_operand"))
   (use (match_operand:DI 3 "gcn_exec_operand"))
   (clobber (match_scratch:DI 4 ""))]
  "REG_P (operands[3]) && REGNO (operands[3]) == EXEC_REG"
  [(parallel [(set (match_dup 0) (match_dup 1))
              (use (match_dup 3))])])

(define_split
  [(set (match_operand:SCALAR_FLAT_MODE 0 "nonimmediate_operand")
	(match_operand:SCALAR_FLAT_MODE 1 "general_operand"))
   (use (match_operand:TI 2 "register_operand"))
   (use (match_operand:DI 3 "gcn_exec_operand"))
   (clobber (match_operand:DI 4 "register_operand"))]
  "(memory_operand (operands[0], VOIDmode) || memory_operand (operands[1], VOIDmode))"
  [(set (match_dup 4) (reg:DI EXEC_REG))
   (set (reg:DI EXEC_REG) (match_dup 3))
   (parallel [(set (match_dup 0) (match_dup 1))
              (use (match_dup 2))
              (use (reg:DI EXEC_REG))])
   (set (reg:DI EXEC_REG) (match_dup 4))])

(define_split
  [(set (match_operand:SCALAR_FLAT_MODE 0 "nonimmediate_operand")
	(match_operand:SCALAR_FLAT_MODE 1 "general_operand"))
   (use (match_operand:TI 2 "register_operand"))
   (use (match_operand:DI 3 "gcn_exec_operand"))
   (clobber (match_operand:DI 4 "register_operand"))]
  ""
  [(set (match_dup 4) (reg:DI EXEC_REG))
   (set (reg:DI EXEC_REG) (match_dup 3))
   (parallel [(set (match_dup 0) (match_dup 1))
              (use (reg:DI EXEC_REG))])
   (set (reg:DI EXEC_REG) (match_dup 4))])

(define_insn "*mov<mode>"
  [(set (match_operand:VEC_1REG_MODE 0 "nonimmediate_operand" "=v,RD,RF,v ,v")
	(vec_merge:VEC_1REG_MODE
	   (match_operand:VEC_1REG_MODE 1 "gcn_load_operand"  "vB,v ,v,RD,RF")
	   (match_operand:VEC_1REG_MODE 3 "gcn_register_or_unspec_operand" "0U,0U,0U,0U,0U")
	  (match_operand:DI 2 "gcn_exec_reg_operand" "e,e,e,e,e")))]
  "!memory_operand (operands[0], VOIDmode) || register_operand (operands[1], VOIDmode)"
  "@
   v_mov_b32\t%0, %1
   ds_write_b32\t%A0, %1%O0
   flat_store%s1\t%A0, %1
   ds_read_b32\t%0, %A1%O1
   flat_load%s0\t%0, %A1\n\ts_waitcnt\tlgkmcnt(0),vmcnt(0)"
  [(set_attr "type" "vop1,dsmem,flat,dsmem,flat")
   (set_attr "mode" "<MODE>")])

(define_insn "*mov<mode>"
  [(set (match_operand:VEC_2REG_MODE 0 "register_operand" "=&v")
	(vec_merge:VEC_2REG_MODE
	   (match_operand:VEC_2REG_MODE 1 "gcn_alu_operand" "0vB")
	   (match_operand:VEC_2REG_MODE 3 "gcn_register_or_unspec_operand" "0U")
	  (match_operand:DI 2 "gcn_exec_reg_operand" "e")))]
  "!memory_operand (operands[0], VOIDmode) || register_operand (operands[1], VOIDmode)"
  "v_mov_b32\t%L0, %L1\n\tv_mov_b32\t%H0, %H1"
  [(set_attr "type" "vop1")
   (set_attr "mode" "<MODE>")])

; TODO: Add zero/sign extending variants.

;; -------------------------------------------------------------------------
;; Vector lane moves
;; -------------------------------------------------------------------------

; v_writelane/readlane works regardless of exec flags.
; We allow source to be scratch
(define_insn "*vec_set<mode>"
  [(set (match_operand:VEC_1REG_MODE 0 "register_operand" "=v")
	(vec_merge:VEC_1REG_MODE
		 (vec_duplicate:VEC_1REG_MODE
			 (match_operand:<scalar_mode> 1 "register_operand" "SS"))
		 (match_operand:VEC_1REG_MODE 3 "gcn_register_or_unspec_operand" "0U")
		 (ashift (const_int 1) (match_operand:SI 2 "gcn_alu_operand" "SSB"))))]
  ""
  "v_writelane_b32 %0, %1, %2"
  [(set_attr "type" "vop3a")
   (set_attr "mode" "<scalar_mode>")
   (set_attr "exec" "any")])

; FIXME: 64bit operations really should be splitters, but I am not sure how
; to represent vertical subregs.
(define_insn "*vec_set<mode>"
  [(set (match_operand:VEC_2REG_MODE 0 "register_operand" "=v")
	(vec_merge:VEC_2REG_MODE
		 (vec_duplicate:VEC_2REG_MODE
			 (match_operand:<scalar_mode> 1 "register_operand" "SS"))
		 (match_operand:VEC_2REG_MODE 3 "gcn_register_or_unspec_operand" "0U")
		 (ashift (const_int 1) (match_operand:SI 2 "gcn_alu_operand" "SSB"))))]
  ""
  "v_writelane_b32 %L0, %L1, %2\n\tv_writelane_b32 %H0, %H1, %2"
  [(set_attr "type" "vop3a")
   (set_attr "mode" "<scalar_mode>")
   (set_attr "exec" "any")])

(define_expand "vec_set<mode>"
  [(set (match_operand:VEC_1REG_MODE 0 "register_operand" "")
	(vec_merge:VEC_1REG_MODE
		 (vec_duplicate:VEC_1REG_MODE
			 (match_operand:<scalar_mode> 1 "register_operand" ""))
		 (match_dup 0)
		 (ashift (const_int 1) (match_operand:SI 2 "gcn_alu_operand" ""))))]
  "")

(define_insn "*vec_set<mode>_1"
  [(set (match_operand:VEC_1REG_MODE 0 "register_operand" "")
	(vec_merge:VEC_1REG_MODE
		 (vec_duplicate:VEC_1REG_MODE
			 (match_operand:<scalar_mode> 1 "register_operand" ""))
		 (match_operand:VEC_1REG_MODE 3 "gcn_register_or_unspec_operand" "")
		 (match_operand:SI 2 "const_int_operand")))]
  "((unsigned) exact_log2 (INTVAL (operands[2])) < 64)"
{
  operands[2] = GEN_INT (exact_log2 (INTVAL (operands[2])));
  return "v_writelane_b32 %0, %1, %2";
}
  [(set_attr "type" "vop3a")
   (set_attr "mode" "<scalar_mode>")
   (set_attr "exec" "any")])

(define_insn "*vec_set<mode>_1"
  [(set (match_operand:VEC_2REG_MODE 0 "register_operand" "")
	(vec_merge:VEC_2REG_MODE
		 (vec_duplicate:VEC_2REG_MODE
			 (match_operand:<scalar_mode> 1 "register_operand" ""))
		 (match_operand:VEC_2REG_MODE 3 "gcn_register_or_unspec_operand" "")
		 (match_operand:SI 2 "const_int_operand")))]
  "((unsigned) exact_log2 (INTVAL (operands[2])) < 64)"
{
  operands[2] = GEN_INT (exact_log2 (INTVAL (operands[2])));
  return "v_writelane_b32 %L0, %L1, %2\nv_writelane_b32 %H0, %H1, %2";
}
  [(set_attr "type" "vop3a")
   (set_attr "mode" "<scalar_mode>")
   (set_attr "exec" "any")])

(define_insn "vec_duplicate<mode>"
  [(set (match_operand:VEC_1REG_MODE 0 "register_operand" "=v")
	(vec_merge:VEC_1REG_MODE
	   (vec_duplicate:VEC_1REG_MODE
		  (match_operand:<scalar_mode> 1 "gcn_alu_operand" "SSB"))
	   (match_operand:VEC_1REG_MODE 3 "gcn_register_or_unspec_operand" "0U")
	   (match_operand:DI 2 "gcn_exec_reg_operand" "e")))]
  ""
  "v_mov_b32\t%0, %1"
  [(set_attr "type" "vop3a")
   (set_attr "mode" "<scalar_mode>")
   (set_attr "exec" "any")])

(define_insn "vec_duplicate<mode>"
  [(set (match_operand:VEC_2REG_MODE 0 "register_operand" "=v")
	(vec_merge:VEC_2REG_MODE
	   (vec_duplicate:VEC_2REG_MODE
		  (match_operand:<scalar_mode> 1 "register_operand" "SS"))
	   (match_operand:VEC_2REG_MODE 3 "gcn_register_or_unspec_operand" "0U")
	   (match_operand:DI 2 "gcn_exec_reg_operand" "e")))]
  ""
  "v_mov_b32\t%L0, %L1\n\tv_mov_b32\t%H0, %H1"
  [(set_attr "type" "vop3a")
   (set_attr "mode" "<scalar_mode>")
   (set_attr "exec" "any")])

(define_insn "vec_extract<mode>"
  [(set (match_operand:<scalar_mode> 0 "register_operand" "=Sm")
	(vec_select:<scalar_mode>
	  (match_operand:VEC_1REG_MODE 1 "register_operand" "v")
	  (parallel [(match_operand:SI 2 "gcn_alu_operand" "SSB")])))]
  ""
  "v_readlane_b32 %0, %1, %2"
  [(set_attr "type" "vop3a")
   (set_attr "mode" "<scalar_mode>")
   (set_attr "exec" "any")])

(define_insn "vec_extract<mode>"
  [(set (match_operand:<scalar_mode> 0 "register_operand" "=Sm")
	(vec_select:<scalar_mode>
	  (match_operand:VEC_2REG_MODE 1 "register_operand" "v")
	  (parallel [(match_operand:SI 2 "gcn_alu_operand" "SSB")])))]
  ""
  "v_readlane_b32 %L0, %L1, %L2
   v_readlane_b32 %H0, %H1, %H2"
  [(set_attr "type" "vop3a")
   (set_attr "mode" "<scalar_mode>")
   (set_attr "exec" "any")])

(define_expand "vec_init<mode>"
  [(match_operand:VEC_REG_MODE 0 "register_operand")
   (match_operand 1)]
  ""
{
  gcn_expand_vector_init (operands[0], operands[1]);
  DONE;
})

;; -------------------------------------------------------------------------
;; Generic expanders for vector operations.
;; -------------------------------------------------------------------------

(define_mode_iterator V64SIDI [V64SI V64DI])

; Integer operations that produce condition code
(define_expand "<expander><mode>3"
  [(parallel [
       (set (match_operand:V64SIDI 0 "register_operand" "")
	    (vec_merge:V64SIDI
		(plus_minus:V64SIDI
			 (match_operand:V64SIDI 1 "register_operand" "")
			 (match_operand:V64SIDI 2 "gcn_alu_operand" ""))
		(match_dup 4)
		(match_dup 3)))
       (clobber (reg:DI VCC_REG))])]
  ""
{
  operands[3] = gcn_full_exec_reg ();
  operands[4] = gcn_gen_undef (<MODE>mode);
})

(define_mode_iterator VEC_INT_MODE [V64QI V64HI V64SI V64DI])
(define_code_iterator bitop [and ior xor])

(define_expand "<expander><mode>3"
  [(set (match_operand:VEC_INT_MODE 0 "gcn_valu_dst_operand" "")
	(vec_merge:VEC_INT_MODE
	    (bitop:VEC_INT_MODE
		     (match_operand:VEC_INT_MODE 1 "gcn_valu_src0_operand" "")
		     (match_operand:VEC_INT_MODE 2 "gcn_valu_src1com_operand" ""))
	    (match_dup 4)
	    (match_dup 3)))]
  ""
{
  operands[3] = gcn_full_exec_reg ();
  operands[4] = gcn_gen_undef (<MODE>mode);
})

(define_code_iterator shiftop [ashift lshiftrt ashiftrt])
(define_expand "<expander><mode>3"
  [(set (match_operand:VEC_INT_MODE 0 "register_operand" "")
	(vec_merge:VEC_INT_MODE
	    (shiftop:VEC_INT_MODE
		     (match_operand:VEC_INT_MODE 1 "register_operand" "")
		     (match_operand:V64SI 2 "gcn_alu_operand" ""))
	    (match_dup 4)
	    (match_dup 3)))]
  ""
{
  operands[3] = gcn_full_exec_reg ();
  operands[4] = gcn_gen_undef (<MODE>mode);
})

;; -------------------------------------------------------------------------
;; ALU special cases: Plus
;; -------------------------------------------------------------------------


; Turn vector pattern into scalar variant.

(define_subst "vec_to_scalar"
  [(set (match_operand:VEC_REG_MODE 0)
	(vec_merge:VEC_REG_MODE
	   (match_operator:VEC_REG_MODE 1 ""
	       [(match_operand:VEC_REG_MODE 2)
	        (match_operand:VEC_REG_MODE 3)])
	   (match_operand:VEC_REG_MODE 4)
	   (match_operand:DI 5)))
   (clobber (reg:DI VCC_REG))]
  ""
  [(set (match_dup:<scalar_mode> 0)
	(match_op_dup:<scalar_mode> 1
	  [(match_dup:<scalar_mode> 1) (match_dup:<scalar_mode> 2)]))
   (use (match_dup:<scalar_mode> 5))
   (clobber (reg:DI VCC_REG))])

(define_subst "vec_to_scalar"
  [(set (match_operand:VEC_REG_MODE 0)
	(vec_merge:VEC_REG_MODE
	   (match_operator:VEC_REG_MODE 1 ""
	       [(match_operand:VEC_REG_MODE 2)
	        (match_operand:VEC_REG_MODE 3)])
	   (match_operand:VEC_REG_MODE 4)
	   (match_operand:DI 5)))]
  ""
  [(set (match_dup:<scalar_mode> 0)
	(match_op_dup:<scalar_mode> 1
	  [(match_dup:<scalar_mode> 1) (match_dup:<scalar_mode> 2)]))
   (use (match_dup:<scalar_mode> 5))])

(define_subst_attr "vec_suffix"
		   "vec_to_scalar" "vector" "scalar")

(define_insn "addv64si3_<vec_suffix>"
  [(set (match_operand:V64SI 0 "register_operand"         "=v")
	(vec_merge:V64SI
	  (plus:V64SI
		   (match_operand:V64SI 1 "register_operand" "%v")
		   (match_operand:V64SI 2 "gcn_alu_operand" " vSSB"))
	  (match_operand:V64SI 4 "gcn_register_or_unspec_operand" "0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e")))
   (clobber (reg:DI VCC_REG))]
  ""
  "v_add_u32\t%0, vcc, %2, %1"
  [(set_attr "type" "vop2")
   (set_attr "mode" "V64SI")])

(define_insn "addv64si3_vector_dup"
  [(set (match_operand:V64SI 0 "register_operand"         "=v")
	(vec_merge:V64SI
	  (plus:V64SI
		   (vec_duplicate:V64SI
			(match_operand:SI 2 "gcn_alu_operand" "SSB"))
		   (match_operand:V64SI 1 "register_operand" "v"))
	  (match_operand:V64SI 4 "gcn_register_or_unspec_operand" "0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e")))
   (clobber (reg:DI VCC_REG))]
  ""
  "v_add_u32\t%0, vcc, %1, %2"
  [(set_attr "type" "vop2")
   (set_attr "mode" "SI")])

(define_insn "addv64si3_vector_vcc"
  [(set (match_operand:V64SI 0 "register_operand" "=v,v")
	(vec_merge:V64SI
	  (plus:V64SI
		   (match_operand:V64SI 1 "register_operand" "%v,v")
		   (match_operand:V64SI 2 "gcn_alu_operand" " vSSB,vSSB"))
	  (match_operand:V64SI 4 "gcn_register_or_unspec_operand" "0U,0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e,e")))
   (set (match_operand:DI 5 "register_operand" "=cV,Sg")
	(ior:DI (and:DI (ltu:DI (plus:V64SI (match_dup 1)
					    (match_dup 2))
				(match_dup 1))
			(match_dup 3))
		(and:DI (not:DI (match_dup 3))
			(match_operand:DI 6 "gcn_register_or_unspec_operand" "5U,5U"))))]
  ""
  "v_add_u32\t%0, %5, %1, %2"
  [(set_attr "type" "vop2,vop3b")
   (set_attr "mode" "SI")])

;; Tom says that he thinks the previous value of VCC is unchanged when
;; execution lane is masked out.

(define_insn "addv64si3_vector_vcc_dup"
  [(set (match_operand:V64SI 0 "register_operand" "=v,v")
	(vec_merge:V64SI
	  (plus:V64SI
		   (vec_duplicate:V64SI (match_operand:SI 2 "gcn_alu_operand" "SSB,SSB"))
		   (match_operand:V64SI 1 "register_operand" "v,v"))
	  (match_operand:V64SI 4 "gcn_register_or_unspec_operand" "0U,0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e,e")))
   (set (match_operand:DI 5 "register_operand" "=cV,Sg")
	(ior:DI (and:DI (ltu:DI (plus:V64SI (vec_duplicate:V64SI (match_dup 2))
					    (match_dup 1))
				(vec_duplicate:V64SI (match_dup 2)))
			(match_dup 3))
		(and:DI (not:DI (match_dup 3))
			(match_operand:DI 6 "gcn_register_or_unspec_operand" "5U,5U"))))]
  ""
  "v_add_u32\t%0, %5, %1, %2"
  [(set_attr "type" "vop2,vop3b")
   (set_attr "mode" "SI")])

;; This pattern does not accept SGPR because VCC read already counts as a SGPR use
;; and number of SGPR operands is limited to 1.
(define_insn "addcv64si3_vec"
  [(set (match_operand:V64SI 0 "register_operand" "=v,v")
        (vec_merge:V64SI (plus:V64SI (plus:V64SI
					(vec_merge:V64SI
					   (match_operand:V64SI 7 "gcn_vec1_operand" "A,A")
					   (match_operand:V64SI 8 "gcn_vec0_operand" "A,A")
					   (match_operand:DI 5 "register_operand" "cV,cV"))
				        (match_operand:V64SI 1 "gcn_alu_operand" "%vA,vA"))
		                     (match_operand:V64SI 2 "gcn_alu_operand" "vB,vB"))
	         (match_operand:V64SI 4 "gcn_register_or_unspec_operand" "0U,0U")
	         (match_operand:DI 3 "gcn_exec_reg_operand" "e,e")))
   (set (match_operand:DI 6 "register_operand" "=cV,Sg")
	(ior:DI (and:DI (ior:DI (ltu:DI (plus:V64SI (plus:V64SI
						      (vec_merge:V64SI
							 (match_dup 7)
							 (match_dup 8)
							 (match_dup 5))
						      (match_dup 1))
						     (match_dup 2))
					 (match_dup 2))
				 (ltu:DI (plus:V64SI (vec_merge:V64SI
							 (match_dup 7)
							 (match_dup 8)
							 (match_dup 5))
						     (match_dup 1))
					 (match_dup 1)))
			  (match_dup 3))
		(and:DI (not:DI (match_dup 3))
			(match_operand:DI 9 "gcn_register_or_unspec_operand" "6U,6U"))))]
  ""
  "v_addc_u32\t%0, %6, %1, %2, vcc"
  [(set_attr "type" "vop2,vop3b")
   (set_attr "mode" "SI")])


(define_insn "subv64si3_<vec_suffix>"
  [(set (match_operand:V64SI 0 "register_operand"         "=v,v")
	(vec_merge:V64SI
	  (minus:V64SI
		   (match_operand:V64SI 1 "gcn_alu_operand" "vSSB,v")
		   (match_operand:V64SI 2 "gcn_alu_operand" "v,vSSB"))
	  (match_operand:V64SI 4 "gcn_register_or_unspec_operand" "0U,0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e,e")))
   (clobber (reg:DI VCC_REG))]
  "register_operand (operands[1], VOIDmode) || register_operand (operands[2], VOIDmode)"
  "@
   v_sub_u32\t%0, vcc, %2, %1
   v_subrev_u32\t%0, vcc, %2, %1"
  [(set_attr "type" "vop2")
   (set_attr "mode" "SI")])

(define_insn "subv64si3_vector_vcc"
  [(set (match_operand:V64SI 0 "register_operand"         "=v,v,v,v")
	(vec_merge:V64SI
	  (minus:V64SI
		   (match_operand:V64SI 1 "gcn_alu_operand" "vSSB,vSSB,v,v")
		   (match_operand:V64SI 2 "gcn_alu_operand" "v,v,vSSB,vSSB"))
	  (match_operand:V64SI 4 "gcn_register_or_unspec_operand" "0U,0U,0U,0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e,e,e,e")))
   (set (match_operand:DI 5 "register_operand" "=cV,Sg,cV,Sg")
	(ior:DI (and:DI (gtu:DI (minus:V64SI (match_dup 1)
					     (match_dup 2))
				(match_dup 1))
			(match_dup 3))
		(and:DI (not:DI (match_dup 3))
			(match_operand:DI 6 "gcn_register_or_unspec_operand" "5U,5U,5U,5U"))))]
  "register_operand (operands[1], VOIDmode) || register_operand (operands[2], VOIDmode)"
  "@
   v_sub_u32\t%0, %5, %2, %1
   v_sub_u32\t%0, %5, %2, %1
   v_subrev_u32\t%0, %5, %2, %1
   v_subrev_u32\t%0, %5, %2, %1"
  [(set_attr "type" "vop2,vop3b,vop2,vop3b")
   (set_attr "mode" "SI")])

;; This pattern does not accept SGPR because VCC read already counts
;; as a SGPR use and number of SGPR operands is limited to 1.
(define_insn "subcv64si3_vec"
  [(set (match_operand:V64SI 0 "register_operand" "=v,v,v,v")
        (vec_merge:V64SI (minus:V64SI (minus:V64SI
					(vec_merge:V64SI
					   (match_operand:V64SI 7 "gcn_vec1_operand" "A,A,A,A")
					   (match_operand:V64SI 8 "gcn_vec0_operand" "A,A,A,A")
					   (match_operand:DI 5 "gcn_alu_operand" "cV,cV,cV,cV"))
				        (match_operand:V64SI 1 "gcn_alu_operand" "vA,vA,vB,vB"))
		                     (match_operand:V64SI 2 "gcn_alu_operand" "vB,vB,vA,vA"))
	         (match_operand:V64SI 4 "gcn_register_or_unspec_operand" "0U,0U,0U,0U")
	         (match_operand:DI 3 "gcn_exec_reg_operand" "e,e,e,e")))
   (set (match_operand:DI 6 "register_operand" "=cV,Sg,cV,Sg")
	(ior:DI (and:DI (ior:DI (gtu:DI (minus:V64SI (minus:V64SI
						      (vec_merge:V64SI
							 (match_dup 7)
							 (match_dup 8)
							 (match_dup 5))
						      (match_dup 1))
						     (match_dup 2))
					 (match_dup 2))
				 (ltu:DI (minus:V64SI (vec_merge:V64SI
							 (match_dup 7)
							 (match_dup 8)
							 (match_dup 5))
						     (match_dup 1))
					 (match_dup 1)))
			  (match_dup 3))
		(and:DI (not:DI (match_dup 3))
			(match_operand:DI 9 "gcn_register_or_unspec_operand" "6U,6U,6U,6U"))))]
  "register_operand (operands[1], VOIDmode) || register_operand (operands[2], VOIDmode)"
  "@
   v_sub_u32\t%0, %5, %2, %1
   v_sub_u32\t%0, %5, %2, %1
   v_subrev_u32\t%0, %5, %2, %1
   v_subrev_u32\t%0, %5, %2, %1"
  [(set_attr "type" "vop2,vop3b,vop2,vop3b")
   (set_attr "mode" "SI")])

(define_insn_and_split "*addv64di3_vector"
  [(set (match_operand:V64DI 0 "register_operand"         "=v")
	(vec_merge:V64DI
	    (plus:V64DI
		     (match_operand:V64DI 1 "register_operand" "%v")
		     (match_operand:V64DI 2 "gcn_alu_operand" "vSSB"))
	    (match_operand:V64DI 4 "gcn_register_or_unspec_operand" "0U")
	    (match_operand:DI 3 "gcn_exec_reg_operand" "e")))
   (clobber (reg:DI VCC_REG))]
  ""
  "#"
  "gcn_can_split_p  (V64DImode, operands[0])
   && gcn_can_split_p (V64DImode, operands[1])
   && gcn_can_split_p (V64DImode, operands[2])
   && gcn_can_split_p (V64DImode, operands[4])"
  [(const_int 0)]
{
  rtx vcc = gen_rtx_REG (DImode, VCC_REG);
  emit_insn (gen_addv64si3_vector_vcc (gcn_operand_part (V64DImode, operands[0], 0),
				       gcn_operand_part (V64DImode, operands[1], 0),
				       gcn_operand_part (V64DImode, operands[2], 0),
				       operands[3],
				       gcn_operand_part (V64DImode, operands[4], 0),
				       vcc,
				       gcn_gen_undef (DImode)));
  emit_insn (gen_addcv64si3_vec (gcn_operand_part (V64DImode, operands[0], 1),
				 gcn_operand_part (V64DImode, operands[1], 1),
				 gcn_operand_part (V64DImode, operands[2], 1),
				 operands[3],
  				 gcn_operand_part (V64DImode, operands[4], 1),
				 vcc, vcc, gcn_vec_constant (V64SImode, 1),
				 gcn_vec_constant (V64SImode, 0),
				 gcn_gen_undef (DImode)));
  DONE;
})

(define_insn_and_split "subv64di3_vec"
  [(set (match_operand:V64DI 0 "register_operand"         "=v,v")
	(vec_merge:V64DI
	  (minus:V64DI
		   (match_operand:V64DI 1 "gcn_alu_operand" "vSSB,v")
		   (match_operand:V64DI 2 "gcn_alu_operand" "v,vSSB"))
	  (match_operand:V64DI 4 "gcn_register_or_unspec_operand" "0U,0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e,e")))
   (clobber (reg:DI VCC_REG))]
  "register_operand (operands[1], VOIDmode) || register_operand (operands[2], VOIDmode)"
  "#"
  "gcn_can_split_p  (V64DImode, operands[0])
   && gcn_can_split_p (V64DImode, operands[1])
   && gcn_can_split_p (V64DImode, operands[2])
   && gcn_can_split_p (V64DImode, operands[4])"
  [(const_int 0)]
{
  rtx vcc = gen_rtx_REG (DImode, VCC_REG);
  emit_insn (gen_subv64si3_vector_vcc (gcn_operand_part (V64DImode, operands[0], 0),
				       gcn_operand_part (V64DImode, operands[1], 0),
				       gcn_operand_part (V64DImode, operands[2], 0),
				       operands[3],
				       gcn_operand_part (V64DImode, operands[4], 0),
				       vcc,
				       gcn_gen_undef (DImode)));
  emit_insn (gen_subcv64si3_vec (gcn_operand_part (V64DImode, operands[0], 1),
				 gcn_operand_part (V64DImode, operands[1], 1),
				 gcn_operand_part (V64DImode, operands[2], 1),
				 operands[3],
  				 gcn_operand_part (V64DImode, operands[4], 1),
				 vcc, vcc, gcn_vec_constant (V64SImode, 1),
				 gcn_vec_constant (V64SImode, 0),
				 gcn_gen_undef (DImode)));
  DONE;
})

(define_insn_and_split "addv64di3_zext"
  [(set (match_operand:V64DI 0 "register_operand" "=&v,v")
	(vec_merge:V64DI
	    (plus:V64DI
		     (zero_extend:V64DI
			 (match_operand:V64SI 1 "gcn_alu_operand" "0vA,0vB"))
		     (match_operand:V64DI 2 "gcn_alu_operand" "0vB,0vA"))
	    (match_operand:V64DI 4 "gcn_register_or_unspec_operand" "0U,0U")
	    (match_operand:DI 3 "gcn_exec_reg_operand" "e,e")))
   (clobber (reg:DI VCC_REG))]
  ""
  "#"
  "gcn_can_split_p  (V64DImode, operands[0])
   && gcn_can_split_p (V64DImode, operands[2])
   && gcn_can_split_p (V64DImode, operands[4])"
  [(const_int 0)]
{
  rtx vcc = gen_rtx_REG (DImode, VCC_REG);
  emit_insn (gen_addv64si3_vector_vcc (gcn_operand_part (V64DImode, operands[0], 0),
				       operands[1],
				       gcn_operand_part (V64DImode, operands[2], 0),
				       operands[3],
				       gcn_operand_part (V64DImode, operands[4], 0),
				       vcc,
				       gcn_gen_undef (DImode)));
  emit_insn (gen_addcv64si3_vec (gcn_operand_part (V64DImode, operands[0], 1),
				 gcn_operand_part (V64DImode, operands[2], 1),
				 const0_rtx,
				 operands[3],
  				 gcn_operand_part (V64DImode, operands[4], 1),
				 vcc, vcc, gcn_vec_constant (V64SImode, 1),
				 gcn_vec_constant (V64SImode, 0),
				 gcn_gen_undef (DImode)));
  DONE;
})

(define_insn_and_split "addv64di3_zext_dup"
  [(set (match_operand:V64DI 0 "register_operand" "=&v")
	(vec_merge:V64DI
	    (plus:V64DI
		     (zero_extend:V64DI
			 (vec_duplicate:V64SI
			    (match_operand:SI 1 "gcn_alu_operand" "BSS")))
		     (match_operand:V64DI 2 "gcn_alu_operand" "0vA"))
	    (match_operand:V64DI 4 "gcn_register_or_unspec_operand" "0U")
	    (match_operand:DI 3 "gcn_exec_reg_operand" "e")))
   (clobber (reg:DI VCC_REG))]
  ""
  "#"
  "gcn_can_split_p  (V64DImode, operands[0])
   && gcn_can_split_p (V64DImode, operands[2])
   && gcn_can_split_p (V64DImode, operands[4])"
  [(const_int 0)]
{
  rtx vcc = gen_rtx_REG (DImode, VCC_REG);
  emit_insn (gen_addv64si3_vector_vcc_dup (gcn_operand_part (V64DImode, operands[0], 0),
				           gcn_operand_part (DImode, operands[1], 0),
				           gcn_operand_part (V64DImode, operands[2], 0),
				           operands[3],
				           gcn_operand_part (V64DImode, operands[4], 0),
					   vcc,
					   gcn_gen_undef (DImode)));
  emit_insn (gen_addcv64si3_vec (gcn_operand_part (V64DImode, operands[0], 1),
				 gcn_operand_part (V64DImode, operands[2], 1),
				 const0_rtx, operands[3],
  				 gcn_operand_part (V64DImode, operands[4], 1),
				 vcc, vcc, gcn_vec_constant (V64SImode, 1),
				 gcn_vec_constant (V64SImode, 0),
				 gcn_gen_undef (DImode)));
  DONE;
})

(define_insn_and_split "addv64di3_zext_dup2"
  [(set (match_operand:V64DI 0 "register_operand" "=v")
	(vec_merge:V64DI
	    (plus:V64DI
		     (zero_extend:V64DI
			 (match_operand:V64SI 1 "gcn_alu_operand" "vA"))
		     (vec_duplicate:V64DI
			 (match_operand:DI 2 "gcn_alu_operand" "BSS")))
	    (match_operand:V64DI 4 "gcn_register_or_unspec_operand" "0U")
	    (match_operand:DI 3 "gcn_exec_reg_operand" "e")))
   (clobber (reg:DI VCC_REG))]
  ""
  "#"
  "gcn_can_split_p  (V64DImode, operands[0])
   && gcn_can_split_p (V64DImode, operands[4])"
  [(const_int 0)]
{
  rtx vcc = gen_rtx_REG (DImode, VCC_REG);
  emit_insn (gen_addv64si3_vector_vcc_dup (gcn_operand_part (V64DImode, operands[0], 0),
				           operands[1],
				           gcn_operand_part (DImode, operands[2], 0),
				           operands[3],
				           gcn_operand_part (V64DImode, operands[4], 0),
					   vcc,
					   gcn_gen_undef (DImode)));
  rtx dsthi = gcn_operand_part (V64DImode, operands[0], 1);
  emit_insn (gen_vec_duplicatev64si (dsthi, gcn_operand_part (DImode, operands[2], 1),
				     operands[3],
				     gcn_gen_undef (V64SImode)));
  emit_insn (gen_addcv64si3_vec (dsthi, dsthi, const0_rtx, operands[3],
  				 gcn_operand_part (V64DImode, operands[4], 1),
				 vcc, vcc, gcn_vec_constant (V64SImode, 1),
				 gcn_vec_constant (V64SImode, 0),
				 gcn_gen_undef (DImode)));
  DONE;
})

(define_mode_iterator DS_ARITH_MODE [V64SI V64SF V64DI])

(define_insn "add<mode>3_ds_<vec_suffix>"
  [(set (match_operand:DS_ARITH_MODE 0 "gcn_ds_memory_operand" "=RD")
	(vec_merge:DS_ARITH_MODE
	  (plus:DS_ARITH_MODE
		   (match_operand:DS_ARITH_MODE 1 "gcn_ds_memory_operand" "%RD")
		   (match_operand:DS_ARITH_MODE 2 "register_operand" "v"))
	  (match_operand:DS_ARITH_MODE 4 "gcn_register_ds_or_unspec_operand" "0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e")))]
  "rtx_equal_p (operands[0], operands[1])"
  "ds_add%u0\t%A0, %2%O0"
  [(set_attr "type" "vop2")
   (set_attr "mode" "SI")])

(define_insn "sub<mode>3_ds_<vec_suffix>"
  [(set (match_operand:DS_ARITH_MODE 0 "gcn_ds_memory_operand" "=RD")
	(vec_merge:DS_ARITH_MODE
	  (minus:DS_ARITH_MODE
		   (match_operand:DS_ARITH_MODE 1 "gcn_ds_memory_operand" "RD")
		   (match_operand:DS_ARITH_MODE 2 "register_operand" "v"))
	  (match_operand:DS_ARITH_MODE 4 "gcn_register_ds_or_unspec_operand" "0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e")))]
  "rtx_equal_p (operands[0], operands[1])"
  "ds_sub%u0\t%A0, %2%O0"
  [(set_attr "type" "vop2")
   (set_attr "mode" "SI")])

(define_insn "subr<mode>3_ds_<vec_suffix>"
  [(set (match_operand:DS_ARITH_MODE 0 "gcn_ds_memory_operand" "=RD")
	(vec_merge:DS_ARITH_MODE
	  (minus:DS_ARITH_MODE
		   (match_operand:DS_ARITH_MODE 2 "register_operand" "v")
		   (match_operand:DS_ARITH_MODE 1 "gcn_ds_memory_operand" "RD"))
	  (match_operand:DS_ARITH_MODE 4 "gcn_register_ds_or_unspec_operand" "0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e")))]
  "rtx_equal_p (operands[0], operands[1])"
  "ds_rsub%u0\t%A0, %2%O0"
  [(set_attr "type" "vop2")
   (set_attr "mode" "SI")])

;; -------------------------------------------------------------------------
;; ALU: mult
;; -------------------------------------------------------------------------

(define_code_iterator any_extend [sign_extend zero_extend])
(define_code_attr sgnsuffix [(sign_extend "%i") (zero_extend "%u")])
(define_code_attr s [(sign_extend "s") (zero_extend "u")])

(define_insn "<s>mulsi3_highpart_vector"
  [(set (match_operand:V64SI 0 "register_operand" "=v")
	(vec_merge:V64SI
	  (truncate:V64SI
	    (lshiftrt:V64DI
	      (mult:V64DI
		(any_extend:V64DI
		  (match_operand:V64SI 1 "gcn_alu_operand" "%v"))
		(any_extend:V64DI
		  (match_operand:V64SI 2 "gcn_alu_operand" "vSSB")))
	      (const_int 32)))
	  (match_operand:V64SI 4 "gcn_register_ds_or_unspec_operand" "0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e")))]
  ""
  "v_mul_hi<sgnsuffix>0\t%0, %1, %2"
  [(set_attr "type" "vop3a")
   (set_attr "mode" "SI")])

(define_insn "<s>mulsi3_highpart_scalar"
  [(set (match_operand:SI 0 "register_operand" "=v")
	(truncate:SI
	  (lshiftrt:DI
	    (mult:DI
	      (any_extend:DI
		(match_operand:SI 1 "register_operand" "%v"))
	      (any_extend:DI
		(match_operand:SI 2 "gcn_alu_operand" "vSSB")))
	    (const_int 32))))
    (use (match_operand:DI 3 "gcn_exec_reg_operand" "e"))]
  ""
  "v_mul_hi<sgnsuffix>0\t%0, %1, %2"
  [(set_attr "type" "vop3a")
   (set_attr "mode" "SI")])

;; -------------------------------------------------------------------------
;; ALU the generic 32bit case
;; -------------------------------------------------------------------------

(define_insn "<expander><mode>3_<vec_suffix>"
  [(set (match_operand:VEC_1REG_INT_MODE 0 "gcn_valu_dst_operand" "=v,RD")
	(vec_merge:VEC_1REG_INT_MODE
	  (bitop:VEC_1REG_INT_MODE
	      (match_operand:VEC_1REG_INT_MODE 1 "gcn_valu_src0_operand" "%v,0")
	      (match_operand:VEC_1REG_INT_MODE 2 "gcn_valu_src1com_operand" "vSSB,v"))
	  (match_operand:VEC_1REG_INT_MODE 4 "gcn_register_ds_or_unspec_operand" "0U,0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e,e")))]
  "!memory_operand (operands[0], VOIDmode)
   || (rtx_equal_p (operands[0], operands[1]) && register_operand (operands[2], VOIDmode))"
  "@
   v_<mnemonic>0\t%0, %2, %1
   ds_<mnemonic>0\t%A0, %2%O0"
  [(set_attr "type" "vop2,dsmem")
   (set_attr "mode" "V64SI")])

; We add earlyclobber just because I am lazy to determine order of oeprations
; in output template.  VGPR register pairs are not aligned.
(define_insn "<expander>v64di3_<vec_suffix>"
  [(set (match_operand:V64DI 0 "gcn_valu_dst_operand" "=&v,RD")
	(vec_merge:V64DI
	  (bitop:V64DI
	      (match_operand:V64DI 1 "gcn_valu_src0_operand" "%0v,RD")
	      (match_operand:V64DI 2 "gcn_valu_src1com_operand" "v0SSB,v"))
	  (match_operand:V64DI 4 "gcn_register_ds_or_unspec_operand" "0U,0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e,e")))]
  "!memory_operand (operands[0], VOIDmode)
   || (rtx_equal_p (operands[0], operands[1]) && register_operand (operands[2], VOIDmode))"
  "@
   v_<mnemonic>0\t%L0, %L2, %L1\n\tv_<mnemonic>0\t%H0, %H2, %H1
   ds_<mnemonic>0\t%A0, %2%O0"
  [(set_attr "type" "vop2,dsmem")
   (set_attr "mode" "V64SI,V64DI")])

(define_insn "<expander><mode>3_<vec_suffix>"
  [(set (match_operand:V_INT_MODE 0 "register_operand" "=v")
	(vec_merge:V_INT_MODE
	   (shiftop:V_INT_MODE
	      (match_operand:V_INT_MODE 1 "gcn_alu_operand" "v")
	      (match_operand:V64SI 2 "gcn_alu_operand" "vSSB"))
	  (match_operand:V_INT_MODE 4 "gcn_register_or_unspec_operand" "0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e")))]
  ""
  "v_<revmnemonic>0\t%0, %2, %1"
  [(set_attr "type" "vop2")
   (set_attr "mode" "<MODE>")])

;; -------------------------------------------------------------------------
;; Generic FP binary operations
;; -------------------------------------------------------------------------

(define_mode_iterator VEC_FP_MODE
		      [V64HF V64SF V64DF])
(define_mode_iterator FP_MODE
		      [HF SF DF])

(define_code_iterator comm_fp [plus mult smin smax])
(define_code_iterator nocomm_fp [minus])
(define_code_iterator all_fp [plus mult minus smin smax])

(define_insn "<expander><mode>_<vec_suffix>"
  [(set (match_operand:VEC_FP_MODE 0 "register_operand"         "=v")
	(vec_merge:VEC_FP_MODE
	  (comm_fp:VEC_FP_MODE
		   (match_operand:VEC_FP_MODE 1 "register_operand" "%v")
		   (match_operand:VEC_FP_MODE 2 "gcn_alu_operand" " vSSB"))
	  (match_operand:VEC_FP_MODE 4 "gcn_register_or_unspec_operand" "0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e")))]
  ""
  "v_<mnemonic>0\t%0, %2, %1"
  [(set_attr "type" "vop2")
   (set_attr "mode" "<MODE>")])


(define_insn "<expander><mode>_<vec_suffix>"
  [(set (match_operand:VEC_FP_MODE 0 "register_operand"         "=v,v")
	(vec_merge:VEC_FP_MODE
	  (nocomm_fp:VEC_FP_MODE
		   (match_operand:VEC_FP_MODE 1 "register_operand" "vSSB,v")
		   (match_operand:VEC_FP_MODE 2 "gcn_alu_operand" "v,vSSB"))
	  (match_operand:VEC_FP_MODE 4 "gcn_register_or_unspec_operand" "0U,0U")
	  (match_operand:DI 3 "gcn_exec_reg_operand" "e,e")))]
  ""
  "@
   v_<mnemonic>0\t%0, %2, %1
   v_<revmnemonic>0\t%0, %2, %1"
  [(set_attr "type" "vop2")
   (set_attr "mode" "<MODE>")])

(define_expand "<expander><mode>3"
  [(set (match_operand:VEC_FP_MODE 0 "gcn_valu_dst_operand" "")
	(vec_merge:VEC_FP_MODE
	    (all_fp:VEC_FP_MODE
		     (match_operand:VEC_FP_MODE 1 "gcn_valu_src0_operand" "")
		     (match_operand:VEC_FP_MODE 2 "gcn_valu_src1_operand" ""))
	    (match_dup 4)
	    (match_dup 3)))]
  ""
{
  operands[3] = gcn_full_exec_reg ();
  operands[4] = gcn_gen_undef (<MODE>mode);
})

(define_expand "<expander><mode>3"
  [(parallel [(set (match_operand:FP_MODE 0 "gcn_valu_dst_operand" "")
		   (all_fp:FP_MODE
			    (match_operand:FP_MODE 1 "gcn_valu_src0_operand" "")
			    (match_operand:FP_MODE 2 "gcn_valu_src1_operand" "")))
	      (use (match_dup 3))])]
  ""
{
  operands[3] = gcn_scalar_exec ();
})
