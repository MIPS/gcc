;; Machine Description for MIPS MSA ASE
;; Based on the MIPS MSA spec Revision 1.11 8/4/2014
;;
;; Copyright (C) 2015 Free Software Foundation, Inc.
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
;;

(define_c_enum "unspec" [
  UNSPEC_MSA_ASUB_S
  UNSPEC_MSA_ASUB_U
  UNSPEC_MSA_AVE_S
  UNSPEC_MSA_AVE_U
  UNSPEC_MSA_AVER_S
  UNSPEC_MSA_AVER_U
  UNSPEC_MSA_BCLR
  UNSPEC_MSA_BCLRI
  UNSPEC_MSA_BINSL
  UNSPEC_MSA_BINSLI
  UNSPEC_MSA_BINSR
  UNSPEC_MSA_BINSRI
  UNSPEC_MSA_BMNZ_V
  UNSPEC_MSA_BMNZI_B
  UNSPEC_MSA_BMZ_V
  UNSPEC_MSA_BMZI_B
  UNSPEC_MSA_BNEG
  UNSPEC_MSA_BNEGI
  UNSPEC_MSA_BSEL_V
  UNSPEC_MSA_BSELI_B
  UNSPEC_MSA_BSET
  UNSPEC_MSA_BSETI
  UNSPEC_MSA_BNZ_V
  UNSPEC_MSA_BZ_V
  UNSPEC_MSA_BNZ
  UNSPEC_MSA_BZ
  UNSPEC_MSA_CFCMSA
  UNSPEC_MSA_CMPI
  UNSPEC_MSA_CTCMSA
  UNSPEC_MSA_DOTP_S
  UNSPEC_MSA_DOTP_U
  UNSPEC_MSA_DPADD_S
  UNSPEC_MSA_DPADD_U
  UNSPEC_MSA_DPSUB_S
  UNSPEC_MSA_DPSUB_U
  UNSPEC_MSA_FCAF
  UNSPEC_MSA_FCLASS
  UNSPEC_MSA_FCUNE
  UNSPEC_MSA_FEXDO
  UNSPEC_MSA_FEXP2
  UNSPEC_MSA_FEXUPL
  UNSPEC_MSA_FEXUPR
  UNSPEC_MSA_FFQL
  UNSPEC_MSA_FFQR
  UNSPEC_MSA_FLOG2
  UNSPEC_MSA_FRCP
  UNSPEC_MSA_FRINT
  UNSPEC_MSA_FRSQRT
  UNSPEC_MSA_FSAF
  UNSPEC_MSA_FSEQ
  UNSPEC_MSA_FSLE
  UNSPEC_MSA_FSLT
  UNSPEC_MSA_FSNE
  UNSPEC_MSA_FSOR
  UNSPEC_MSA_FSUEQ
  UNSPEC_MSA_FSULE
  UNSPEC_MSA_FSULT
  UNSPEC_MSA_FSUN
  UNSPEC_MSA_FSUNE
  UNSPEC_MSA_FTINT_S
  UNSPEC_MSA_FTINT_U
  UNSPEC_MSA_FTRUNC_S
  UNSPEC_MSA_FTRUNC_U
  UNSPEC_MSA_FTQ
  UNSPEC_MSA_HADD_S
  UNSPEC_MSA_HADD_U
  UNSPEC_MSA_HSUB_S
  UNSPEC_MSA_HSUB_U
  UNSPEC_MSA_MADD_Q
  UNSPEC_MSA_MADDR_Q
  UNSPEC_MSA_MAX_A
  UNSPEC_MSA_MAXI_S
  UNSPEC_MSA_MAXI_U
  UNSPEC_MSA_MIN_A
  UNSPEC_MSA_MINI_S
  UNSPEC_MSA_MINI_U
  UNSPEC_MSA_MSUB_Q
  UNSPEC_MSA_MSUBR_Q
  UNSPEC_MSA_MUL_Q
  UNSPEC_MSA_MULR_Q
  UNSPEC_MSA_NLOC
  UNSPEC_MSA_NORI_B
  UNSPEC_MSA_SAT_S
  UNSPEC_MSA_SAT_U
  UNSPEC_MSA_SHF
  UNSPEC_MSA_SLD
  UNSPEC_MSA_SLDI
  UNSPEC_MSA_SLLI
  UNSPEC_MSA_SPLAT
  UNSPEC_MSA_SPLATI
  UNSPEC_MSA_SRAI
  UNSPEC_MSA_SRAR
  UNSPEC_MSA_SRARI
  UNSPEC_MSA_SRLI
  UNSPEC_MSA_SRLR
  UNSPEC_MSA_SRLRI
  UNSPEC_MSA_SUBS_S
  UNSPEC_MSA_SUBS_U
  UNSPEC_MSA_SUBSUU_S
  UNSPEC_MSA_SUBSUS_U
  UNSPEC_MSA_SUBVI
  UNSPEC_MSA_TSTNZ_V
  UNSPEC_MSA_TSTZ_V
  UNSPEC_MSA_TSTNZ
  UNSPEC_MSA_TSTZ
  UNSPEC_MSA_VSHF
  UNSPEC_MSA_XORI_B
  UNSPEC_MSA_CAST_TO_SCALAR
  UNSPEC_MSA_CAST_TO_VECTOR
])

;; All vector modes with 128 bits.
(define_mode_iterator MSA      [V2DF V4SF V2DI V4SI V8HI V16QI])

;; Same as MSA.  Used by vcond to iterate two modes.
(define_mode_iterator MSA_2    [V2DF V4SF V2DI V4SI V8HI V16QI])

;; Only used for splitting insert_d and copy_{u,s}.d.
(define_mode_iterator MSA_D    [V2DI V2DF])

;; Only used for copy_{u,s}.w.
(define_mode_iterator MSA_W    [V4SI V4SF])

;; Only integer modes.
(define_mode_iterator IMSA     [V2DI V4SI V8HI V16QI])

;; As IMSA but excludes V16QI.
(define_mode_iterator IMSA_DWH [V2DI V4SI V8HI])

;; As IMSA but excludes V2DI.
(define_mode_iterator IMSA_WHB [V4SI V8HI V16QI])

;; Only integer modes equal or larger than a word.
(define_mode_iterator IMSA_DW  [V2DI V4SI])

;; Only integer modes smaller than a word.
(define_mode_iterator IMSA_HB  [V8HI V16QI])

;; Only integer modes for fixed-point madd_q/maddr_q.
(define_mode_iterator IMSA_WH  [V4SI V8HI])

;; Only floating-point modes.
(define_mode_iterator FMSA     [V2DF V4SF])

;; The attribute gives the integer vector mode with same size.
(define_mode_attr VIMODE
  [(V2DF "V2DI")
   (V4SF "V4SI")
   (V2DI "V2DI")
   (V4SI "V4SI")
   (V8HI "V8HI")
   (V16QI "V16QI")])

;; The attribute gives half modes for vector modes.
(define_mode_attr VHMODE
  [(V8HI "V16QI")
   (V4SI "V8HI")
   (V2DI "V4SI")
   (V2DF "V4SF")])

;; The attribute gives double modes for vector modes.
(define_mode_attr VDMODE
  [(V4SI "V2DI")
   (V8HI "V4SI")
   (V16QI "V8HI")])

;; The attribute gives half modes with same number of elements for vector modes.
(define_mode_attr TRUNCMODE
  [(V8HI "V8QI")
   (V4SI "V4HI")
   (V2DI "V2SI")])

;; This attribute gives the mode of the result for "copy_s_b, copy_u_b" etc.
(define_mode_attr RES
  [(V2DF "DF")
   (V4SF "SF")
   (V2DI "DI")
   (V4SI "SI")
   (V8HI "SI")
   (V16QI "SI")])

;; Only used with MSA_D iterator.
(define_mode_attr msa_d
  [(V2DI "reg_or_0")
   (V2DF "register")])

;; This attribute gives the integer vector mode with same size.
(define_mode_attr mode_i
  [(V2DF "v2di")
   (V4SF "v4si")
   (V2DI "v2di")
   (V4SI "v4si")
   (V8HI "v8hi")
   (V16QI "v16qi")])

;; This attribute gives suffix for MSA instructions.
(define_mode_attr msafmt
  [(V2DF "d")
   (V4SF "w")
   (V2DI "d")
   (V4SI "w")
   (V8HI "h")
   (V16QI "b")])

;; This attribute gives suffix for integers in VHMODE.
(define_mode_attr hmsafmt
  [(V2DI "w")
   (V4SI "h")
   (V8HI "b")])

;; This attribute gives define_insn suffix for MSA instructions with need
;; distinction between integer and floating point.
(define_mode_attr msafmt_f
  [(V2DF "d_f")
   (V4SF "w_f")
   (V2DI "d")
   (V4SI "w")
   (V8HI "h")
   (V16QI "b")])

;; This is used in msa_cast* to output mov.s or mov.d.
(define_mode_attr unitfmt
  [(V2DF "d")
   (V4SF "s")])

;; The mask for shift amounts.
(define_mode_attr shift_mask
  [(V2DI "63")
   (V4SI "31")
   (V8HI "15")
   (V16QI "7")])

;; This is used to form an immediate operand constraint using
;; "const_<indeximm>_operand".
(define_mode_attr indeximm
  [(V2DF "0_or_1")
   (V4SF "0_to_3")
   (V2DI "0_or_1")
   (V4SI "0_to_3")
   (V8HI "uimm3")
   (V16QI "uimm4")])

;; To represent bitmask needed for vec_merge using "const_<bitmask>_operand".
(define_mode_attr bitmask
  [(V2DF "exp_2")
   (V4SF "exp_4")
   (V2DI "exp_2")
   (V4SI "exp_4")
   (V8HI "exp_8")
   (V16QI "exp_16")])

;; This attribute used to form an immediate operand constraint using
;; "const_<bitimm>_operand".
(define_mode_attr bitimm
  [(V16QI "uimm3")
   (V8HI  "uimm4")
   (V4SI  "uimm5")
   (V2DI  "uimm6")
  ])

(define_expand "vec_init<mode>"
  [(match_operand:MSA 0 "register_operand")
   (match_operand:MSA 1 "")]
  "ISA_HAS_MSA"
{
  mips_expand_vector_init (operands[0], operands[1]);
  DONE;
})

(define_expand "fixuns_trunc<FMSA:mode><mode_i>2"
  [(set (match_operand:<VIMODE> 0 "register_operand" "=f")
	(unsigned_fix:<VIMODE> (match_operand:FMSA 1 "register_operand" "f")))]
  "ISA_HAS_MSA"
{
  emit_insn (gen_msa_ftrunc_u_<msafmt> (operands[0], operands[1]));
  DONE;
})

(define_expand "fix_trunc<FMSA:mode><mode_i>2"
  [(set (match_operand:<VIMODE> 0 "register_operand" "=f")
	(fix:<VIMODE> (match_operand:FMSA 1 "register_operand" "f")))]
  "ISA_HAS_MSA"
{
  emit_insn (gen_msa_ftrunc_s_<msafmt> (operands[0], operands[1]));
  DONE;
})

(define_expand "vec_pack_trunc_v2df"
  [(set (match_operand:V4SF 0 "register_operand")
	(vec_concat:V4SF
	  (float_truncate:V2SF (match_operand:V2DF 1 "register_operand"))
	  (float_truncate:V2SF (match_operand:V2DF 2 "register_operand"))))]
  "ISA_HAS_MSA"
  "")

;; pckev pattern with implicit type conversion.
(define_insn "vec_pack_trunc_<mode>"
   [(set (match_operand:<VHMODE> 0 "register_operand" "=f")
	 (vec_concat:<VHMODE>
	   (truncate:<TRUNCMODE>
	     (match_operand:IMSA_DWH 1 "register_operand" "f"))
	   (truncate:<TRUNCMODE>
	     (match_operand:IMSA_DWH 2 "register_operand" "f"))))]
  "ISA_HAS_MSA"
  "pckev.<hmsafmt>\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "<MODE>")])

(define_expand "vec_unpacks_hi_v4sf"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(float_extend:V2DF
	  (vec_select:V2SF
	    (match_operand:V4SF 1 "register_operand" "f")
	    (parallel [(const_int 0) (const_int 1)])
	  )))]
  "ISA_HAS_MSA"
{
  if (BYTES_BIG_ENDIAN)
    emit_insn (gen_msa_fexupr_d (operands[0], operands[1]));
  else
    emit_insn (gen_msa_fexupl_d (operands[0], operands[1]));
  DONE;
})

(define_expand "vec_unpacks_lo_v4sf"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(float_extend:V2DF
	  (vec_select:V2SF
	    (match_operand:V4SF 1 "register_operand" "f")
	    (parallel [(const_int 0) (const_int 1)])
	  )))]
  "ISA_HAS_MSA"
{
  if (BYTES_BIG_ENDIAN)
    emit_insn (gen_msa_fexupl_d (operands[0], operands[1]));
  else
    emit_insn (gen_msa_fexupr_d (operands[0], operands[1]));
  DONE;
})

(define_expand "vec_unpacks_hi_<mode>"
  [(set (match_operand:<VDMODE> 0 "register_operand")
	(match_operand:IMSA_WHB 1 "register_operand"))]
  "ISA_HAS_MSA"
{
  mips_expand_vec_unpack (operands, false/*unsigned_p*/, true/*high_p*/);
  DONE;
})

(define_expand "vec_unpacks_lo_<mode>"
  [(set (match_operand:<VDMODE> 0 "register_operand")
	(match_operand:IMSA_WHB 1 "register_operand"))]
  "ISA_HAS_MSA"
{
  mips_expand_vec_unpack (operands, false/*unsigned_p*/, false/*high_p*/);
  DONE;
})

(define_expand "vec_unpacku_hi_<mode>"
  [(set (match_operand:<VDMODE> 0 "register_operand")
	(match_operand:IMSA_WHB 1 "register_operand"))]
  "ISA_HAS_MSA"
{
  mips_expand_vec_unpack (operands, true/*unsigned_p*/, true/*high_p*/);
  DONE;
})

(define_expand "vec_unpacku_lo_<mode>"
  [(set (match_operand:<VDMODE> 0 "register_operand")
	(match_operand:IMSA_WHB 1 "register_operand"))]
  "ISA_HAS_MSA"
{
  mips_expand_vec_unpack (operands, true/*unsigned_p*/, false/*high_p*/);
  DONE;
})

(define_expand "vec_extract<mode>"
  [(match_operand:<UNITMODE> 0 "register_operand")
   (match_operand:IMSA 1 "register_operand")
   (match_operand 2 "const_<indeximm>_operand")]
  "ISA_HAS_MSA"
{
  if (<UNITMODE>mode == QImode || <UNITMODE>mode == HImode)
    {
      rtx dest1 = gen_reg_rtx (SImode);
      emit_insn (gen_msa_copy_s_<msafmt> (dest1, operands[1], operands[2]));
      emit_move_insn (operands[0],
		      gen_lowpart (<UNITMODE>mode, dest1));
    }
  else
    emit_insn (gen_msa_copy_s_<msafmt> (operands[0], operands[1], operands[2]));
  DONE;
})

(define_expand "vec_extract<mode>"
  [(match_operand:<UNITMODE> 0 "register_operand")
   (match_operand:FMSA 1 "register_operand")
   (match_operand 2 "const_<indeximm>_operand")]
  "ISA_HAS_MSA"
{
  rtx temp;
  HOST_WIDE_INT val = UINTVAL (operands[2]);

  if (val == 0)
    temp = operands[1];
  else
    {
      /* We need to do the SLDI operation in V16QImode and adjust
	 operand[2] accordingly.  */
      rtx tempb = gen_reg_rtx (V16QImode);
      rtx op1b = gen_reg_rtx (V16QImode);
      emit_move_insn (op1b, gen_rtx_SUBREG (V16QImode, operands[1], 0));
      rtx op2b = GEN_INT (val * GET_MODE_SIZE (<UNITMODE>mode));
      gcc_assert (UINTVAL (op2b) < GET_MODE_NUNITS (V16QImode));
      emit_insn (gen_msa_sldi_b (tempb, op1b, op1b, op2b));
      temp = gen_reg_rtx (<MODE>mode);
      emit_move_insn (temp, gen_rtx_SUBREG (<MODE>mode, tempb, 0));
    }
  emit_insn (gen_msa_cast_to_scalar_<msafmt_f> (operands[0], temp));
  DONE;
})

(define_expand "vec_set<mode>"
  [(match_operand:IMSA 0 "register_operand")
   (match_operand:<UNITMODE> 1 "reg_or_0_operand")
   (match_operand 2 "const_<indeximm>_operand")]
  "ISA_HAS_MSA"
{
  emit_insn (gen_msa_insert_<msafmt>_insn (operands[0], operands[1],
					   operands[0],
					   GEN_INT(1 << INTVAL (operands[2]))));
  DONE;
})

(define_expand "vec_set<mode>"
  [(match_operand:FMSA 0 "register_operand")
   (match_operand:<UNITMODE> 1 "register_operand")
   (match_operand 2 "const_<indeximm>_operand")]
  "ISA_HAS_MSA"
{
  emit_insn (gen_msa_insve_<msafmt_f>_s (operands[0], operands[0],
					 GEN_INT(1 << INTVAL (operands[2])),
					 operands[1]));
  DONE;
})

(define_expand "vcondu<MSA:mode><IMSA:mode>"
  [(set (match_operand:MSA 0 "register_operand")
	(if_then_else:MSA
	  (match_operator 3 ""
	    [(match_operand:IMSA 4 "register_operand")
	     (match_operand:IMSA 5 "register_operand")])
	  (match_operand:MSA 1 "reg_or_m1_operand")
	  (match_operand:MSA 2 "reg_or_0_operand")))]
  "ISA_HAS_MSA
   && (GET_MODE_NUNITS (<MSA:MODE>mode) == GET_MODE_NUNITS (<IMSA:MODE>mode))"
{
  mips_expand_vec_cond_expr (<MSA:MODE>mode, <MSA:VIMODE>mode, operands);
  DONE;
})

(define_expand "vcond<MSA:mode><MSA_2:mode>"
  [(set (match_operand:MSA 0 "register_operand")
	(if_then_else:MSA
	  (match_operator 3 ""
	    [(match_operand:MSA_2 4 "register_operand")
	     (match_operand:MSA_2 5 "register_operand")])
	  (match_operand:MSA 1 "reg_or_m1_operand")
	  (match_operand:MSA 2 "reg_or_0_operand")))]
  "ISA_HAS_MSA
   && (GET_MODE_NUNITS (<MSA:MODE>mode) == GET_MODE_NUNITS (<MSA_2:MODE>mode))"
{
  mips_expand_vec_cond_expr (<MSA:MODE>mode, <MSA:VIMODE>mode, operands);
  DONE;
})

;; Note used directly by builtins but via the following define_expand.
(define_insn "msa_insert_<msafmt>_insn"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(vec_merge:IMSA
	  (vec_duplicate:IMSA
	    (match_operand:<UNITMODE> 1 "reg_or_0_operand" "dJ"))
	  (match_operand:IMSA 2 "register_operand" "0")
	  (match_operand 3 "const_<bitmask>_operand" "")))]
  "ISA_HAS_MSA"
  "insert.<msafmt>\t%w0[%y3],%z1"
  [(set_attr "type" "simd_insert")
   (set_attr "mode" "<MODE>")])

;; Expand builtin for HImode and QImode which takes SImode.
(define_expand "msa_insert_<msafmt>"
  [(match_operand:IMSA 0 "register_operand")
   (match_operand:IMSA 1 "register_operand")
   (match_operand 2 "const_<indeximm>_operand")
   (match_operand:<RES> 3 "reg_or_0_operand")]
  "ISA_HAS_MSA"
{
  if ((GET_MODE_SIZE (<UNITMODE>mode) < GET_MODE_SIZE (<RES>mode))
      && (REG_P (operands[3]) || (GET_CODE (operands[3]) == SUBREG
				  && REG_P (SUBREG_REG (operands[3])))))
    {
      unsigned int offset = GET_MODE_SIZE (<RES>mode)
			    - GET_MODE_SIZE (<UNITMODE>mode);
      operands[3] = simplify_gen_subreg (<UNITMODE>mode, operands[3],
					 GET_MODE (operands[3]),
					 BYTES_BIG_ENDIAN ? offset : 0);
    }
  emit_insn (gen_msa_insert_<msafmt>_insn (operands[0], operands[3],
					   operands[1],
					   GEN_INT(1 << INTVAL (operands[2]))));
  DONE;
})

(define_expand "msa_insert_<msafmt_f>"
  [(match_operand:FMSA 0 "register_operand")
   (match_operand:FMSA 1 "register_operand")
   (match_operand 2 "const_<indeximm>_operand")
   (match_operand:<UNITMODE> 3 "reg_or_0_operand")]
  "ISA_HAS_MSA"
{
  emit_insn (gen_msa_insert_<msafmt_f>_insn (operands[0], operands[3],
					     operands[1],
					     GEN_INT(1 << INTVAL (operands[2]))));
  DONE;
})

(define_insn "msa_insert_<msafmt_f>_insn"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(vec_merge:FMSA
	  (vec_duplicate:FMSA
	    (match_operand:<UNITMODE> 1 "register_operand" "d"))
	  (match_operand:FMSA 2 "register_operand" "0")
	  (match_operand 3 "const_<bitmask>_operand" "")))]
  "ISA_HAS_MSA"
  "insert.<msafmt>\t%w0[%y3],%z1"
  [(set_attr "type" "simd_insert")
   (set_attr "mode" "<MODE>")])

(define_split
  [(set (match_operand:MSA_D 0 "register_operand")
	(vec_merge:MSA_D
	  (vec_duplicate:MSA_D
	    (match_operand:<UNITMODE> 1 "<MSA_D:msa_d>_operand"))
	  (match_operand:MSA_D 2 "register_operand")
	  (match_operand 3 "const_<bitmask>_operand")))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_insert_d (operands[0], operands[2], operands[3], operands[1]);
  DONE;
})

(define_expand "msa_insve_<msafmt_f>"
  [(set (match_operand:MSA 0 "register_operand")
	(vec_merge:MSA
	  (vec_duplicate:MSA
	    (vec_select:<UNITMODE>
	      (match_operand:MSA 3 "register_operand")
	      (parallel [(const_int 0)])))
	  (match_operand:MSA 1 "register_operand")
	  (match_operand 2 "const_<indeximm>_operand")))]
  "ISA_HAS_MSA"
{
  operands[2] = GEN_INT (1 << INTVAL (operands[2]));
})

(define_insn "msa_insve_<msafmt_f>_insn"
  [(set (match_operand:MSA 0 "register_operand" "=f")
	(vec_merge:MSA
	  (vec_duplicate:MSA
	    (vec_select:<UNITMODE>
	      (match_operand:MSA 3 "register_operand" "f")
	      (parallel [(const_int 0)])))
	  (match_operand:MSA 1 "register_operand" "0")
	  (match_operand 2 "const_<bitmask>_operand" "")))]
  "ISA_HAS_MSA"
  "insve.<msafmt>\t%w0[%y2],%w3[0]"
  [(set_attr "type" "simd_insert")
   (set_attr "mode" "<MODE>")])

;; Operand 3 is a scalar.
(define_insn "msa_insve_<msafmt>_f_s"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(vec_merge:FMSA
	  (vec_duplicate:FMSA
	    (match_operand:<UNITMODE> 3 "register_operand" "f"))
	  (match_operand:FMSA 1 "register_operand" "0")
	  (match_operand 2 "const_<bitmask>_operand" "")))]
  "ISA_HAS_MSA"
  "insve.<msafmt>\t%w0[%y2],%w3[0]"
  [(set_attr "type" "simd_insert")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_copy_<su>_<msafmt>"
  [(set (match_operand:<RES> 0 "register_operand" "=d")
	(any_extend:<RES>
	  (vec_select:<UNITMODE>
	    (match_operand:IMSA_HB 1 "register_operand" "f")
	    (parallel [(match_operand 2 "const_<indeximm>_operand" "")]))))]
  "ISA_HAS_MSA"
  "copy_<su>.<msafmt>\t%0,%w1[%2]"
  [(set_attr "type" "simd_copy")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_copy_u_w"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(zero_extend:DI
	  (vec_select:SI
	    (match_operand:V4SI 1 "register_operand" "f")
	    (parallel [(match_operand 2 "const_0_to_3_operand" "")]))))]
  "ISA_HAS_MSA && TARGET_64BIT"
  "copy_u.w\t%0,%w1[%2]"
  [(set_attr "type" "simd_copy")
   (set_attr "mode" "V4SI")])

(define_insn "msa_copy_s_<msafmt_f>"
  [(set (match_operand:<UNITMODE> 0 "register_operand" "=d")
	(vec_select:<UNITMODE>
	  (match_operand:MSA_W 1 "register_operand" "f")
	  (parallel [(match_operand 2 "const_<indeximm>_operand" "")])))]
  "ISA_HAS_MSA"
  "copy_s.<msafmt>\t%0,%w1[%2]"
  [(set_attr "type" "simd_copy")
   (set_attr "mode" "<MODE>")])

(define_insn_and_split "msa_copy_s_<msafmt_f>"
  [(set (match_operand:<UNITMODE> 0 "register_operand" "=d")
	(vec_select:<UNITMODE>
	  (match_operand:MSA_D 1 "register_operand" "f")
	  (parallel [(match_operand 2 "const_<indeximm>_operand" "")])))]
  "ISA_HAS_MSA"
{
  if (TARGET_64BIT)
    return "copy_s.<msafmt>\t%0,%w1[%2]";
  else
    return "#";
}
  "reload_completed && ISA_HAS_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_copy_d (operands[0], operands[1], operands[2],
			 gen_msa_copy_s_w);
  DONE;
}
  [(set_attr "type" "simd_copy")
   (set_attr "mode" "<MODE>")])

(define_expand "vec_perm<mode>"
  [(match_operand:MSA 0 "register_operand")
   (match_operand:MSA 1 "register_operand")
   (match_operand:MSA 2 "register_operand")
   (match_operand:<VIMODE> 3 "register_operand")]
  "ISA_HAS_MSA"
{
  /* The optab semantics are that index 0 selects the first element
     of operands[1] and the highest index selects the last element
     of operands[2].  This is the oppossite order from "vshf.df wd,rs,wt"
     where index 0 selects the first element of wt and the highest index
     selects the last element of ws.  We therefore swap the operands here.  */
  emit_insn (gen_msa_vshf<mode> (operands[0], operands[3], operands[2],
				 operands[1]));
  DONE;
})

(define_expand "vec_perm_const<mode>"
  [(match_operand:MSA 0 "register_operand")
   (match_operand:MSA 1 "register_operand")
   (match_operand:MSA 2 "register_operand")
   (match_operand:<VIMODE> 3 "")]
  "ISA_HAS_MSA"
{
  if (mips_expand_vec_perm_const (operands))
    DONE;
  else
    FAIL;
})

(define_expand "abs<mode>2"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(abs:IMSA (match_operand:IMSA 1 "register_operand" "f")))]
  "ISA_HAS_MSA"
{
  rtx reg = gen_reg_rtx (<MODE>mode);
  emit_insn (gen_msa_ldi<mode> (reg, const0_rtx));
  emit_insn (gen_msa_add_a_<msafmt> (operands[0], operands[1], reg));
  DONE;
})

(define_expand "neg<mode>2"
  [(match_operand:IMSA 0 "register_operand")
   (match_operand:IMSA 1 "register_operand")]
  "ISA_HAS_MSA"
{
  rtx reg = gen_reg_rtx (<MODE>mode);
  emit_insn (gen_msa_ldi<mode> (reg, const0_rtx));
  emit_insn (gen_sub<mode>3 (operands[0], reg, operands[1]));
  DONE;
})

(define_expand "neg<mode>2"
  [(match_operand:FMSA 0 "register_operand")
   (match_operand:FMSA 1 "register_operand")]
  "ISA_HAS_MSA"
{
  rtx reg = gen_reg_rtx (<MODE>mode);
  emit_move_insn (reg, CONST0_RTX (<MODE>mode));
  emit_insn (gen_sub<mode>3 (operands[0], reg, operands[1]));
  DONE;
})

(define_expand "msa_ldi<mode>"
  [(match_operand:IMSA 0 "register_operand")
   (match_operand 1 "const_imm10_operand")]
  "ISA_HAS_MSA"
{
  unsigned n_elts = GET_MODE_NUNITS (<MODE>mode);
  rtvec v = rtvec_alloc (n_elts);
  HOST_WIDE_INT val = INTVAL (operands[1]);
  unsigned int i;

  if (<MODE>mode != V16QImode)
    {
      unsigned shift = HOST_BITS_PER_WIDE_INT - 10;
      val = trunc_int_for_mode ((val << shift) >> shift, <UNITMODE>mode);
    }
  else
    val = trunc_int_for_mode (val, <UNITMODE>mode);

  for (i = 0; i < n_elts; i++)
    RTVEC_ELT (v, i) = GEN_INT (val);
  emit_move_insn (operands[0],
		  gen_rtx_CONST_VECTOR (<MODE>mode, v));
  DONE;
})

(define_insn "msa_vshf<mode>"
  [(set (match_operand:MSA 0 "register_operand" "=f")
	(unspec:MSA [(match_operand:<VIMODE> 1 "register_operand" "0")
		     (match_operand:MSA 2 "register_operand" "f")
		     (match_operand:MSA 3 "register_operand" "f")]
		    UNSPEC_MSA_VSHF))]
  "ISA_HAS_MSA"
  "vshf.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_sld")
   (set_attr "mode" "<MODE>")])

(define_expand "mov<mode>"
  [(set (match_operand:MSA 0)
	(match_operand:MSA 1))]
  "TARGET_MSA"
{
  if (mips_legitimize_move (<MODE>mode, operands[0], operands[1]))
    DONE;
})

(define_expand "movmisalign<mode>"
  [(set (match_operand:MSA 0)
	(match_operand:MSA 1))]
  "TARGET_MSA"
{
  if (mips_legitimize_move (<MODE>mode, operands[0], operands[1]))
    DONE;
})

;; 128bit MSA modes only in msa registers or memory.  An exception is allowing
;; MSA modes for GP registers for arguments and return values.
(define_insn "mov<mode>_msa"
  [(set (match_operand:MSA 0 "nonimmediate_operand" "=f,f,R,*d,*f")
	(match_operand:MSA 1 "move_operand" "fYGYI,R,f,*f,*d"))]
  "TARGET_MSA"
  { return mips_output_move (insn, operands[0], operands[1]); }
  [(set_attr "type" "simd_move,simd_load,simd_store,simd_copy,simd_insert")
   (set_attr "mode" "<MODE>")])

(define_split
  [(set (match_operand:MSA 0 "nonimmediate_operand")
	(match_operand:MSA 1 "move_operand"))]
  "reload_completed && TARGET_MSA
   && mips_split_move_insn_p (operands[0], operands[1], insn)"
  [(const_int 0)]
{
  mips_split_move_insn (operands[0], operands[1], curr_insn);
  DONE;
})

;; Offset load
(define_expand "msa_ld_<msafmt_f>"
  [(match_operand:MSA 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand 2 "aq10<msafmt>_operand")]
  "ISA_HAS_MSA"
{
  rtx addr = plus_constant (GET_MODE (operands[1]), operands[1],
				      INTVAL (operands[2]));
  mips_emit_move (operands[0], gen_rtx_MEM (<MODE>mode, addr));
  DONE;
})

;; Offset store
(define_expand "msa_st_<msafmt_f>"
  [(match_operand:MSA 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand 2 "aq10<msafmt>_operand")]
  "ISA_HAS_MSA"
{
  rtx addr = plus_constant (GET_MODE (operands[1]), operands[1],
			    INTVAL (operands[2]));
  mips_emit_move (gen_rtx_MEM (<MODE>mode, addr), operands[0]);
  DONE;
})

;; Integer operations
(define_insn "add<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f,f,f")
	(plus:IMSA
	  (match_operand:IMSA 1 "register_operand" "f,f,f")
	  (match_operand:IMSA 2 "reg_or_vector_same_ximm5_operand" "f,Unv5,Uuv5")))]
  "ISA_HAS_MSA"
{
  switch (which_alternative)
    {
    case 0:
      return "addv.<msafmt>\t%w0,%w1,%w2";
    case 1:
      {
	HOST_WIDE_INT val = INTVAL (CONST_VECTOR_ELT (operands[2], 0));

	operands[2] = GEN_INT (-val);
	return "subvi.<msafmt>\t%w0,%w1,%d2";
      }
    case 2:
      {
	HOST_WIDE_INT val = INTVAL (CONST_VECTOR_ELT (operands[2], 0));

	operands[2] = GEN_INT (val);
	return "addvi.<msafmt>\t%w0,%w1,%d2";
      }
    default:
      gcc_unreachable ();
    }
}
  [(set_attr "alu_type" "simd_add")
   (set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "sub<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f,f,f")
	(minus:IMSA
	  (match_operand:IMSA 1 "register_operand" "f,f,f")
	  (match_operand:IMSA 2 "reg_or_vector_same_ximm5_operand" "f,Unv5,Uuv5")))]
  "ISA_HAS_MSA"
{
  switch (which_alternative)
    {
    case 0:
      return "subv.<msafmt>\t%w0,%w1,%w2";
    case 1:
      {
	HOST_WIDE_INT val = INTVAL (CONST_VECTOR_ELT (operands[2], 0));

	operands[2] = GEN_INT (-val);
	return "addvi.<msafmt>\t%w0,%w1,%d2";
      }
    case 2:
      {
	HOST_WIDE_INT val = INTVAL (CONST_VECTOR_ELT (operands[2], 0));

	operands[2] = GEN_INT (val);
	return "subvi.<msafmt>\t%w0,%w1,%d2";
      }
    default:
      gcc_unreachable ();
    }
}
  [(set_attr "alu_type" "simd_add")
   (set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "mul<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(mult:IMSA (match_operand:IMSA 1 "register_operand" "f")
		   (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "mulv.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_maddv_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(plus:IMSA (mult:IMSA (match_operand:IMSA 2 "register_operand" "f")
			      (match_operand:IMSA 3 "register_operand" "f"))
		   (match_operand:IMSA 1 "register_operand" "0")))]
  "ISA_HAS_MSA"
  "maddv.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_msubv_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(minus:IMSA (match_operand:IMSA 1 "register_operand" "0")
		    (mult:IMSA (match_operand:IMSA 2 "register_operand" "f")
			       (match_operand:IMSA 3 "register_operand" "f"))))]
  "ISA_HAS_MSA"
  "msubv.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "div<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(div:IMSA (match_operand:IMSA 1 "register_operand" "f")
		  (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  { return mips_msa_output_division ("div_s.<msafmt>\t%w0,%w1,%w2", operands); }
  [(set_attr "type" "simd_div")
   (set_attr "mode" "<MODE>")])

(define_insn "udiv<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(udiv:IMSA (match_operand:IMSA 1 "register_operand" "f")
		   (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  { return mips_msa_output_division ("div_u.<msafmt>\t%w0,%w1,%w2", operands); }
  [(set_attr "type" "simd_div")
   (set_attr "mode" "<MODE>")])

(define_insn "mod<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(mod:IMSA (match_operand:IMSA 1 "register_operand" "f")
		  (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  { return mips_msa_output_division ("mod_s.<msafmt>\t%w0,%w1,%w2", operands); }
  [(set_attr "type" "simd_div")
   (set_attr "mode" "<MODE>")])

(define_insn "umod<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(umod:IMSA (match_operand:IMSA 1 "register_operand" "f")
		   (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  { return mips_msa_output_division ("mod_u.<msafmt>\t%w0,%w1,%w2", operands); }
  [(set_attr "type" "simd_div")
   (set_attr "mode" "<MODE>")])

(define_insn "xorv16qi3"
  [(set (match_operand:V16QI 0 "register_operand" "=f,f")
	(xor:V16QI
	  (match_operand:V16QI 1 "register_operand" "f,f")
	  (match_operand:V16QI 2 "reg_or_vector_same_byte_operand" "f,Ubv8")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 1)
    {
      operands[2] = CONST_VECTOR_ELT (operands[2], 0);
      return "xori.b\t%w0,%w1,%B2";
    }
  else
    return "xor.v\t%w0,%w1,%w2";
}
  [(set_attr "type" "simd_logic")
   (set_attr "mode" "TI,V16QI")])

(define_insn "xor<mode>3"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f,f")
	(xor:IMSA_DWH
	  (match_operand:IMSA_DWH 1 "register_operand" "f,f")
	  (match_operand:IMSA_DWH 2 "reg_or_vector_same_<mode>_set_operand" "f,YC")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 1)
    {
      HOST_WIDE_INT val = INTVAL (CONST_VECTOR_ELT (operands[2], 0));
      int vlog2 = exact_log2 (val);
      gcc_assert (vlog2 != -1);
      operands[2] = GEN_INT (vlog2);
      return "bnegi.%v0\t%w0,%w1,%2";
    }
  else
    return "xor.v\t%w0,%w1,%w2";
}
  [(set_attr "type" "simd_logic,simd_bit")
   (set_attr "mode" "TI,<MODE>")])

(define_insn "iorv16qi3"
  [(set (match_operand:V16QI 0 "register_operand" "=f,f")
	(ior:V16QI
	  (match_operand:V16QI 1 "register_operand" "f,f")
	  (match_operand:V16QI 2 "reg_or_vector_same_byte_operand" "f,Ubv8")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 1)
    {
      operands[2] = CONST_VECTOR_ELT (operands[2], 0);
      return "ori.b\t%w0,%w1,%B2";
    }
  else
    return "or.v\t%w0,%w1,%w2";
}
  [(set_attr "type" "simd_logic")
   (set_attr "mode" "TI,V16QI")])

(define_insn "ior<mode>3"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f,f")
	(ior:IMSA_DWH
	  (match_operand:IMSA_DWH 1 "register_operand" "f,f")
	  (match_operand:IMSA_DWH 2 "reg_or_vector_same_<mode>_set_operand" "f,YC")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 1)
    {
      HOST_WIDE_INT val = INTVAL (CONST_VECTOR_ELT (operands[2], 0));
      int vlog2 = exact_log2 (val);
      gcc_assert (vlog2 != -1);
      operands[2] = GEN_INT (vlog2);
      return "bseti.%v0\t%w0,%w1,%2";
    }
  else
    return "or.v\t%w0,%w1,%w2";
}
  [(set_attr "type" "simd_logic,simd_bit")
   (set_attr "mode" "TI,<MODE>")])

(define_insn "andv16qi3"
  [(set (match_operand:V16QI 0 "register_operand" "=f,f")
	(and:V16QI
	  (match_operand:V16QI 1 "register_operand" "f,f")
	  (match_operand:V16QI 2 "reg_or_vector_same_byte_operand" "f,Ubv8")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 1)
    {
      operands[2] = CONST_VECTOR_ELT (operands[2], 0);
      return "andi.b\t%w0,%w1,%B2";
    }
  else
    return "and.v\t%w0,%w1,%w2";
}
  [(set_attr "type" "simd_logic")
   (set_attr "mode" "TI,V16QI")])

(define_insn "and<mode>3"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f,f")
	(and:IMSA_DWH
	  (match_operand:IMSA_DWH 1 "register_operand" "f,f")
	  (match_operand:IMSA_DWH 2 "reg_or_vector_same_<mode>_clr_operand" "f,YZ")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 1)
    {
      HOST_WIDE_INT val = INTVAL (CONST_VECTOR_ELT (operands[2], 0));
      int vlog2 = exact_log2 (~val);
      gcc_assert (vlog2 != -1);
      operands[2] = GEN_INT (vlog2);
      return "bclri.%v0\t%w0,%w1,%2";
    }
   else
     return "and.v\t%w0,%w1,%w2";
}
  [(set_attr "type" "simd_logic,simd_bit")
   (set_attr "mode" "TI,<MODE>")])

(define_insn "one_cmpl<mode>2"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(not:IMSA (match_operand:IMSA 1 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "nor.v\t%w0,%w1,%w1"
  [(set_attr "type" "simd_logic")
   (set_attr "mode" "TI")])

(define_insn "vlshr<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f,f")
	(lshiftrt:IMSA
	  (match_operand:IMSA 1 "register_operand" "f,f")
	  (match_operand:IMSA 2 "reg_or_vector_same_uimm6_operand" "f,Uuv6")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 0)
    return "srl.<msafmt>\t%w0,%w1,%w2";

  operands[2] = GEN_INT (INTVAL (CONST_VECTOR_ELT (operands[2], 0))
			 & <shift_mask>);
  return "srli.<msafmt>\t%w0,%w1,%2";
}
  [(set_attr "type" "simd_shift")
   (set_attr "mode" "<MODE>")])

(define_insn "vashr<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f,f")
	(ashiftrt:IMSA
	  (match_operand:IMSA 1 "register_operand" "f,f")
	  (match_operand:IMSA 2 "reg_or_vector_same_uimm6_operand" "f,Uuv6")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 0)
    return "sra.<msafmt>\t%w0,%w1,%w2";

  operands[2] = GEN_INT (INTVAL (CONST_VECTOR_ELT (operands[2], 0))
			 & <shift_mask>);
  return "srai.<msafmt>\t%w0,%w1,%2";
}
  [(set_attr "type" "simd_shift")
   (set_attr "mode" "<MODE>")])

(define_insn "vashl<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f,f")
	(ashift:IMSA
	  (match_operand:IMSA 1 "register_operand" "f,f")
	  (match_operand:IMSA 2 "reg_or_vector_same_uimm6_operand" "f,Uuv6")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 0)
    return "sll.<msafmt>\t%w0,%w1,%w2";

  operands[2] = GEN_INT (INTVAL (CONST_VECTOR_ELT (operands[2], 0))
			 & <shift_mask>);
  return "slli.<msafmt>\t%w0,%w1,%2";
}
  [(set_attr "type" "simd_shift")
   (set_attr "mode" "<MODE>")])

;; Floating-point operations
(define_insn "add<mode>3"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(plus:FMSA (match_operand:FMSA 1 "register_operand" "f")
		   (match_operand:FMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fadd.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fadd")
   (set_attr "mode" "<MODE>")])

(define_insn "sub<mode>3"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(minus:FMSA (match_operand:FMSA 1 "register_operand" "f")
		    (match_operand:FMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fsub.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fadd")
   (set_attr "mode" "<MODE>")])

(define_insn "mul<mode>3"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(mult:FMSA (match_operand:FMSA 1 "register_operand" "f")
		   (match_operand:FMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fmul.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fmul")
   (set_attr "mode" "<MODE>")])

(define_insn "div<mode>3"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(div:FMSA (match_operand:FMSA 1 "register_operand" "f")
		  (match_operand:FMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fdiv.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fdiv")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_fmadd_<msafmt>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(plus:FMSA (mult:FMSA (match_operand:FMSA 2 "register_operand" "f")
			      (match_operand:FMSA 3 "register_operand" "f"))
		   (match_operand:FMSA 1 "register_operand" "0")))]
  "ISA_HAS_MSA"
  "fmadd.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_fmadd")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_fmsub_<msafmt>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(minus:FMSA (match_operand:FMSA 1 "register_operand" "0")
		    (mult:FMSA (match_operand:FMSA 2 "register_operand" "f")
			       (match_operand:FMSA 3 "register_operand" "f"))))]
  "ISA_HAS_MSA"
  "fmsub.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_fmadd")
   (set_attr "mode" "<MODE>")])

(define_insn "sqrt<mode>2"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(sqrt:FMSA (match_operand:FMSA 1 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fsqrt.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fdiv")
   (set_attr "mode" "<MODE>")])

;; Built-in functions
(define_insn "msa_add_a_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(plus:IMSA (abs:IMSA (match_operand:IMSA 1 "register_operand" "f"))
		   (abs:IMSA (match_operand:IMSA 2 "register_operand" "f"))))]
  "ISA_HAS_MSA"
  "add_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_adds_a_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(ss_plus:IMSA
	  (abs:IMSA (match_operand:IMSA 1 "register_operand" "f"))
	  (abs:IMSA (match_operand:IMSA 2 "register_operand" "f"))))]
  "ISA_HAS_MSA"
  "adds_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "ssadd<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(ss_plus:IMSA (match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "adds_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "usadd<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(us_plus:IMSA (match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "adds_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_expand "msa_addvi_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand")
	(plus:IMSA (match_operand:IMSA 1 "register_operand")
		   (match_operand 2 "const_uimm5_operand")))]
  "ISA_HAS_MSA"
{
  unsigned n_elts = GET_MODE_NUNITS (<MODE>mode);
  rtvec v = rtvec_alloc (n_elts);
  HOST_WIDE_INT val = INTVAL (operands[2]);
  unsigned int i;

  for (i = 0; i < n_elts; i++)
    RTVEC_ELT (v, i) = GEN_INT (val);

  emit_insn (gen_msa_addvi_<msafmt>_insn (operands[0], operands[1],
					  gen_rtx_CONST_VECTOR (<MODE>mode, v)));
  DONE;
})

(define_expand "msa_andi_b"
  [(set (match_operand:V16QI 0 "register_operand")
	(and:V16QI (match_operand:V16QI 1 "register_operand")
		   (match_operand:QI 2 "const_uimm8_operand")))]
  "ISA_HAS_MSA"
{
  rtvec v = rtvec_alloc (16);
  HOST_WIDE_INT val = INTVAL (operands[2]);
  unsigned int i;

  for (i = 0; i < 16; i++)
    RTVEC_ELT (v, i) = GEN_INT (val);

  emit_insn (gen_msa_andi_b_insn (operands[0], operands[1],
				  gen_rtx_CONST_VECTOR (V16QImode, v)));
  DONE;
})

(define_insn "msa_addvi_<msafmt>_insn"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(plus:IMSA
	  (match_operand:IMSA 1 "register_operand" "f")
	  (match_operand:IMSA 2 "const_vector_same_uimm5_operand" "")))]
  "ISA_HAS_MSA"
  "addvi.<msafmt>\t%w0,%w1,%E2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_andi_b_insn"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(and:V16QI
	  (match_operand:V16QI 1 "register_operand" "f")
	  (match_operand:V16QI 2 "const_vector_same_uimm8_operand" "")))]
  "ISA_HAS_MSA"
  "andi.b\t%w0,%w1,%E2"
  [(set_attr "type" "simd_logic")
   (set_attr "mode" "V16QI")])

(define_insn "msa_asub_s_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_ASUB_S))]
  "ISA_HAS_MSA"
  "asub_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_asub_u_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_ASUB_U))]
  "ISA_HAS_MSA"
  "asub_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_ave_s_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_AVE_S))]
  "ISA_HAS_MSA"
  "ave_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_ave_u_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_AVE_U))]
  "ISA_HAS_MSA"
  "ave_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_aver_s_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_AVER_S))]
  "ISA_HAS_MSA"
  "aver_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_aver_u_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_AVER_U))]
  "ISA_HAS_MSA"
  "aver_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_bclr_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_BCLR))]
  "ISA_HAS_MSA"
  "bclr.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_bit")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_bclri_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_BCLRI))]
  "ISA_HAS_MSA"
  "bclri.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_bit")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_binsl_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "0")
		      (match_operand:IMSA 2 "register_operand" "f")
		      (match_operand:IMSA 3 "register_operand" "f")]
		     UNSPEC_MSA_BINSL))]
  "ISA_HAS_MSA"
  "binsl.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_bitins")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_binsli_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "0")
		      (match_operand:IMSA 2 "register_operand" "f")
		      (match_operand 3 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_BINSLI))]
  "ISA_HAS_MSA"
  "binsli.<msafmt>\t%w0,%w2,%3"
  [(set_attr "type" "simd_bitins")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_binsr_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "0")
		      (match_operand:IMSA 2 "register_operand" "f")
		      (match_operand:IMSA 3 "register_operand" "f")]
		     UNSPEC_MSA_BINSR))]
  "ISA_HAS_MSA"
  "binsr.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_bitins")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_binsri_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "0")
		      (match_operand:IMSA 2 "register_operand" "f")
		      (match_operand 3 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_BINSRI))]
  "ISA_HAS_MSA"
  "binsri.<msafmt>\t%w0,%w2,%3"
  [(set_attr "type" "simd_bitins")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_bmnz_v_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "0")
		      (match_operand:IMSA 2 "register_operand" "f")
		      (match_operand:IMSA 3 "register_operand" "f")]
		     UNSPEC_MSA_BMNZ_V))]
  "ISA_HAS_MSA"
  "bmnz.v\t%w0,%w2,%w3"
  [(set_attr "type" "simd_bitmov")
   (set_attr "mode" "TI")])

(define_insn "msa_bmnzi_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand:V16QI 2 "register_operand" "f")
		       (match_operand 3 "const_uimm8_operand" "")]
		      UNSPEC_MSA_BMNZI_B))]
  "ISA_HAS_MSA"
  "bmnzi.b\t%w0,%w2,%3"
  [(set_attr "type" "simd_bitmov")
   (set_attr "mode" "V16QI")])

(define_insn "msa_bmz_v_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "0")
		      (match_operand:IMSA 2 "register_operand" "f")
		      (match_operand:IMSA 3 "register_operand" "f")]
		     UNSPEC_MSA_BMZ_V))]
  "ISA_HAS_MSA"
  "bmz.v\t%w0,%w2,%w3"
  [(set_attr "type" "simd_bitmov")
   (set_attr "mode" "TI")])

(define_insn "msa_bmzi_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand:V16QI 2 "register_operand" "f")
		       (match_operand 3 "const_uimm8_operand" "")]
		      UNSPEC_MSA_BMZI_B))]
  "ISA_HAS_MSA"
  "bmzi.b\t%w0,%w2,%3"
  [(set_attr "type" "simd_bitmov")
   (set_attr "mode" "V16QI")])

(define_insn "msa_bneg_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_BNEG))]
  "ISA_HAS_MSA"
  "bneg.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_bit")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_bnegi_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		       (match_operand 2 "const_msa_branch_operand" "")]
		     UNSPEC_MSA_BNEGI))]
  "ISA_HAS_MSA"
  "bnegi.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_bit")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_bsel_v_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "0")
		      (match_operand:IMSA 2 "register_operand" "f")
		      (match_operand:IMSA 3 "register_operand" "f")]
		     UNSPEC_MSA_BSEL_V))]
  "ISA_HAS_MSA"
  "bsel.v\t%w0,%w2,%w3"
  [(set_attr "type" "simd_bitmov")
   (set_attr "mode" "TI")])

(define_insn "msa_bseli_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand:V16QI 2 "register_operand" "f")
		       (match_operand 3 "const_uimm8_operand" "")]
		      UNSPEC_MSA_BSELI_B))]
  "ISA_HAS_MSA"
  "bseli.b\t%w0,%w2,%3"
  [(set_attr "type" "simd_bitmov")
   (set_attr "mode" "V16QI")])

(define_insn "msa_bset_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_BSET))]
  "ISA_HAS_MSA"
  "bset.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_bit")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_bseti_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_BSETI))]
  "ISA_HAS_MSA"
  "bseti.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_bit")
   (set_attr "mode" "<MODE>")])

(define_code_iterator ICC [eq le leu lt ltu])

(define_code_attr icc
    [(eq  "eq")
     (le  "le_s")
     (leu "le_u")
     (lt  "lt_s")
     (ltu "lt_u")])

(define_code_attr icci
    [(eq  "eqi")
     (le  "lei_s")
     (leu "lei_u")
     (lt  "lti_s")
     (ltu "lti_u")])

(define_code_attr cmpi
    [(eq   "s")
     (le   "s")
     (leu  "u")
     (lt   "s")
     (ltu  "u")])

(define_insn "msa_c<ICC:icc>_<IMSA:msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(ICC:IMSA (match_operand:IMSA 1 "register_operand" "f")
		  (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "c<ICC:icc>.<IMSA:msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_c<ICC:icci>i_<IMSA:msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(ICC:IMSA
	  (match_operand:IMSA 1 "register_operand" "f")
	  (match_operand:IMSA 2 "const_vector_same_cmp<ICC:cmpi>imm4_operand" "")))]
  "ISA_HAS_MSA"
  "c<ICC:icci>.<IMSA:msafmt>\t%w0,%w1,%E2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_c<ICC:icci>_<IMSA:msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(ICC:IMSA (match_operand:IMSA 1 "register_operand" "f")
				(match_operand 2 "const_imm5_operand" ""))]
		     UNSPEC_MSA_CMPI))]
  "ISA_HAS_MSA"
  "c<ICC:icci>.<IMSA:msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_dotp_s_<msafmt>"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f")
	(unspec:IMSA_DWH [(match_operand:<VHMODE> 1 "register_operand" "f")
			  (match_operand:<VHMODE> 2 "register_operand" "f")]
			 UNSPEC_MSA_DOTP_S))]
  "ISA_HAS_MSA"
  "dotp_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_dotp_u_<msafmt>"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f")
	(unspec:IMSA_DWH [(match_operand:<VHMODE> 1 "register_operand" "f")
			  (match_operand:<VHMODE> 2 "register_operand" "f")]
			 UNSPEC_MSA_DOTP_U))]
  "ISA_HAS_MSA"
  "dotp_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_dpadd_s_<msafmt>"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f")
	(unspec:IMSA_DWH [(match_operand:IMSA_DWH 1 "register_operand" "0")
			  (match_operand:<VHMODE> 2 "register_operand" "f")
			  (match_operand:<VHMODE> 3 "register_operand" "f")]
			 UNSPEC_MSA_DPADD_S))]
  "ISA_HAS_MSA"
  "dpadd_s.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_dpadd_u_<msafmt>"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f")
	(unspec:IMSA_DWH [(match_operand:IMSA_DWH 1 "register_operand" "0")
			  (match_operand:<VHMODE> 2 "register_operand" "f")
			  (match_operand:<VHMODE> 3 "register_operand" "f")]
			 UNSPEC_MSA_DPADD_U))]
  "ISA_HAS_MSA"
  "dpadd_u.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_dpsub_s_<msafmt>"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f")
	(unspec:IMSA_DWH [(match_operand:IMSA_DWH 1 "register_operand" "0")
			  (match_operand:<VHMODE> 2 "register_operand" "f")
			  (match_operand:<VHMODE> 3 "register_operand" "f")]
			 UNSPEC_MSA_DPSUB_S))]
  "ISA_HAS_MSA"
  "dpsub_s.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_dpsub_u_<msafmt>"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f")
	(unspec:IMSA_DWH [(match_operand:IMSA_DWH 1 "register_operand" "0")
			  (match_operand:<VHMODE> 2 "register_operand" "f")
			  (match_operand:<VHMODE> 3 "register_operand" "f")]
			 UNSPEC_MSA_DPSUB_U))]
  "ISA_HAS_MSA"
  "dpsub_u.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_fclass_<msafmt>"
  [(set (match_operand:<VIMODE> 0 "register_operand" "=f")
	(unspec:<VIMODE> [(match_operand:FMSA 1 "register_operand" "f")]
			 UNSPEC_MSA_FCLASS))]
  "ISA_HAS_MSA"
  "fclass.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fclass")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_fcaf_<msafmt>"
  [(set (match_operand:<VIMODE> 0 "register_operand" "=f")
	(unspec:<VIMODE> [(match_operand:FMSA 1 "register_operand" "f")
			  (match_operand:FMSA 2 "register_operand" "f")]
			 UNSPEC_MSA_FCAF))]
  "ISA_HAS_MSA"
  "fcaf.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fcmp")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_fcune_<FMSA:msafmt>"
  [(set (match_operand:<VIMODE> 0 "register_operand" "=f")
	(unspec:<VIMODE> [(match_operand:FMSA 1 "register_operand" "f")
			  (match_operand:FMSA 2 "register_operand" "f")]
			 UNSPEC_MSA_FCUNE))]
  "ISA_HAS_MSA"
  "fcune.<FMSA:msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fcmp")
   (set_attr "mode" "<MODE>")])

(define_code_iterator FCC [unordered ordered eq ne le lt uneq unle unlt])

(define_code_attr fcc
    [(unordered "fcun")
     (ordered   "fcor")
     (eq        "fceq")
     (ne        "fcne")
     (uneq      "fcueq")
     (unle      "fcule")
     (unlt      "fcult")
     (le        "fcle")
     (lt        "fclt")])

(define_int_iterator FSC_UNS [UNSPEC_MSA_FSAF UNSPEC_MSA_FSUN UNSPEC_MSA_FSOR
			      UNSPEC_MSA_FSEQ UNSPEC_MSA_FSNE UNSPEC_MSA_FSUEQ
			      UNSPEC_MSA_FSUNE UNSPEC_MSA_FSULE UNSPEC_MSA_FSULT
			      UNSPEC_MSA_FSLE UNSPEC_MSA_FSLT])

(define_int_attr fsc
    [(UNSPEC_MSA_FSAF  "fsaf")
     (UNSPEC_MSA_FSUN  "fsun")
     (UNSPEC_MSA_FSOR  "fsor")
     (UNSPEC_MSA_FSEQ  "fseq")
     (UNSPEC_MSA_FSNE  "fsne")
     (UNSPEC_MSA_FSUEQ "fsueq")
     (UNSPEC_MSA_FSUNE "fsune")
     (UNSPEC_MSA_FSULE "fsule")
     (UNSPEC_MSA_FSULT "fsult")
     (UNSPEC_MSA_FSLE  "fsle")
     (UNSPEC_MSA_FSLT  "fslt")])

(define_insn "msa_<FCC:fcc>_<FMSA:msafmt>"
  [(set (match_operand:<VIMODE> 0 "register_operand" "=f")
	(FCC:<VIMODE> (match_operand:FMSA 1 "register_operand" "f")
		      (match_operand:FMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "<FCC:fcc>.<FMSA:msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fcmp")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_<fsc>_<FMSA:msafmt>"
  [(set (match_operand:<VIMODE> 0 "register_operand" "=f")
	(unspec:<VIMODE> [(match_operand:FMSA 1 "register_operand" "f")
			   (match_operand:FMSA 2 "register_operand" "f")]
			 FSC_UNS))]
  "ISA_HAS_MSA"
  "<fsc>.<FMSA:msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fcmp")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_fexp2_<msafmt>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(unspec:FMSA [(match_operand:FMSA 1 "register_operand" "f")
		      (match_operand:<VIMODE> 2 "register_operand" "f")]
		     UNSPEC_MSA_FEXP2))]
  "ISA_HAS_MSA"
  "fexp2.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fexp2")
   (set_attr "mode" "<MODE>")])

(define_mode_attr FINT
   [(V4SF "V4SI")
    (V2DF "V2DI")])

(define_mode_attr fint
   [(V4SF "v4si")
    (V2DF "v2di")])

(define_mode_attr FQ
   [(V4SF "V8HI")
    (V2DF "V4SI")])

(define_mode_attr FINTCNV
  [(V4SF "I2S")
   (V2DF "I2D")])

(define_mode_attr FINTCNV_2
  [(V4SF "S2I")
   (V2DF "D2I")])

(define_insn "float<fint><FMSA:mode>2"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(float:FMSA (match_operand:<FINT> 1 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "ffint_s.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "cnv_mode" "<FINTCNV>")
   (set_attr "mode" "<MODE>")])

(define_insn "floatuns<fint><FMSA:mode>2"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(unsigned_float:FMSA (match_operand:<FINT> 1 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "ffint_u.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "cnv_mode" "<FINTCNV>")
   (set_attr "mode" "<MODE>")])

(define_mode_attr FFQ
  [(V4SF "V8HI")
   (V2DF "V4SI")])

(define_insn "msa_ffql_<msafmt>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(unspec:FMSA [(match_operand:<FQ> 1 "register_operand" "f")]
		     UNSPEC_MSA_FFQL))]
  "ISA_HAS_MSA"
  "ffql.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "cnv_mode" "<FINTCNV>")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_ffqr_<msafmt>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(unspec:FMSA [(match_operand:<FQ> 1 "register_operand" "f")]
		     UNSPEC_MSA_FFQR))]
  "ISA_HAS_MSA"
  "ffqr.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "cnv_mode" "<FINTCNV>")
   (set_attr "mode" "<MODE>")])

;; Note used directly by builtins but via the following define_expand.
(define_insn "msa_fill_<msafmt>_insn"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(vec_duplicate:IMSA
	  (match_operand:<UNITMODE> 1 "reg_or_0_operand" "dJ")))]
  "ISA_HAS_MSA"
  "fill.<msafmt>\t%w0,%z1"
  [(set_attr "type" "simd_fill")
   (set_attr "mode" "<MODE>")])

;; Expand builtin for HImode and QImode which takes SImode.
(define_expand "msa_fill_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand")
	(vec_duplicate:IMSA
	  (match_operand:<RES> 1 "reg_or_0_operand")))]
  "ISA_HAS_MSA"
{
  if ((GET_MODE_SIZE (<UNITMODE>mode) < GET_MODE_SIZE (<RES>mode))
      && (REG_P (operands[1]) || (GET_CODE (operands[1]) == SUBREG
				  && REG_P (SUBREG_REG (operands[1])))))
    {
      unsigned int offset = GET_MODE_SIZE (<RES>mode)
			    - GET_MODE_SIZE (<UNITMODE>mode);
      operands[1] = simplify_gen_subreg (<UNITMODE>mode, operands[1],
					 GET_MODE (operands[1]),
					 BYTES_BIG_ENDIAN ? offset : 0);
    }
  emit_insn (gen_msa_fill_<msafmt>_insn (operands[0], operands[1]));
  DONE;
})

(define_insn "msa_fill_<msafmt_f>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(vec_duplicate:FMSA
	  (match_operand:<UNITMODE> 1 "reg_or_0_operand" "dJ")))]
  "ISA_HAS_MSA"
  "fill.<msafmt>\t%w0,%z1"
  [(set_attr "type" "simd_fill")
   (set_attr "mode" "<MODE>")])

;; Note that fill.d and fill.d_f will be split later if !TARGET_64BIT.
(define_split
  [(set (match_operand:V2DI 0 "register_operand")
	(vec_duplicate:V2DI
	  (match_operand:DI 1 "reg_or_0_operand")))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_fill_d (operands[0], operands[1]);
  DONE;
})

(define_split
  [(set (match_operand:V2DF 0 "register_operand")
	(vec_duplicate:V2DF
	  (match_operand:DF 1 "register_operand")))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_fill_d (operands[0], operands[1]);
  DONE;
})

(define_insn "msa_flog2_<msafmt>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(unspec:FMSA [(match_operand:FMSA 1 "register_operand" "f")]
		     UNSPEC_MSA_FLOG2))]
  "ISA_HAS_MSA"
  "flog2.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_flog2")
   (set_attr "mode" "<MODE>")])

(define_insn "smax<mode>3"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(smax:FMSA (match_operand:FMSA 1 "register_operand" "f")
		   (match_operand:FMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fmax.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fminmax")
   (set_attr "mode" "<MODE>")])

(define_insn "umax<mode>3"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(umax:FMSA (match_operand:FMSA 1 "register_operand" "f")
		   (match_operand:FMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fmax_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fminmax")
   (set_attr "mode" "<MODE>")])

(define_insn "smin<mode>3"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(smin:FMSA (match_operand:FMSA 1 "register_operand" "f")
		   (match_operand:FMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fmin.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fminmax")
   (set_attr "mode" "<MODE>")])

(define_insn "umin<mode>3"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(umin:FMSA (match_operand:FMSA 1 "register_operand" "f")
		   (match_operand:FMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fmin_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fminmax")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_frcp_<msafmt>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(unspec:FMSA [(match_operand:FMSA 1 "register_operand" "f")]
		     UNSPEC_MSA_FRCP))]
  "ISA_HAS_MSA"
  "frcp.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fdiv")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_frint_<msafmt>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(unspec:FMSA [(match_operand:FMSA 1 "register_operand" "f")]
		     UNSPEC_MSA_FRINT))]
  "ISA_HAS_MSA"
  "frint.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_frsqrt_<msafmt>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(unspec:FMSA [(match_operand:FMSA 1 "register_operand" "f")]
		     UNSPEC_MSA_FRSQRT))]
  "ISA_HAS_MSA"
  "frsqrt.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fdiv")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_ftint_s_<msafmt>"
  [(set (match_operand:<FINT> 0 "register_operand" "=f")
	(unspec:<FINT> [(match_operand:FMSA 1 "register_operand" "f")]
		       UNSPEC_MSA_FTINT_S))]
  "ISA_HAS_MSA"
  "ftint_s.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "cnv_mode" "<FINTCNV_2>")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_ftint_u_<msafmt>"
  [(set (match_operand:<FINT> 0 "register_operand" "=f")
	(unspec:<FINT> [(match_operand:FMSA 1 "register_operand" "f")]
		       UNSPEC_MSA_FTINT_U))]
  "ISA_HAS_MSA"
  "ftint_u.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "cnv_mode" "<FINTCNV_2>")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_ftrunc_s_<msafmt>"
  [(set (match_operand:<FINT> 0 "register_operand" "=f")
	(unspec:<FINT> [(match_operand:FMSA 1 "register_operand" "f")]
		       UNSPEC_MSA_FTRUNC_S))]
  "ISA_HAS_MSA"
  "ftrunc_s.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "cnv_mode" "<FINTCNV_2>")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_ftrunc_u_<msafmt>"
  [(set (match_operand:<FINT> 0 "register_operand" "=f")
	(unspec:<FINT> [(match_operand:FMSA 1 "register_operand" "f")]
		       UNSPEC_MSA_FTRUNC_U))]
  "ISA_HAS_MSA"
  "ftrunc_u.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "cnv_mode" "<FINTCNV_2>")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_ftq_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V4SF 1 "register_operand" "f")
		      (match_operand:V4SF 2 "register_operand" "f")]
		     UNSPEC_MSA_FTQ))]
  "ISA_HAS_MSA"
  "ftq.h\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fcvt")
   (set_attr "cnv_mode" "S2I")
   (set_attr "mode" "V4SF")])

(define_insn "msa_ftq_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V2DF 1 "register_operand" "f")
		      (match_operand:V2DF 2 "register_operand" "f")]
		     UNSPEC_MSA_FTQ))]
  "ISA_HAS_MSA"
  "ftq.w\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fcvt")
   (set_attr "cnv_mode" "D2I")
   (set_attr "mode" "V2DF")])

(define_insn "msa_hadd_s_<msafmt>"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f")
	(unspec:IMSA_DWH [(match_operand:<VHMODE> 1 "register_operand" "f")
			  (match_operand:<VHMODE> 2 "register_operand" "f")]
			 UNSPEC_MSA_HADD_S))]
  "ISA_HAS_MSA"
  "hadd_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_hadd_u_<msafmt>"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f")
	(unspec:IMSA_DWH [(match_operand:<VHMODE> 1 "register_operand" "f")
			  (match_operand:<VHMODE> 2 "register_operand" "f")]
			 UNSPEC_MSA_HADD_U))]
  "ISA_HAS_MSA"
  "hadd_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_hsub_s_<msafmt>"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f")
	(unspec:IMSA_DWH [(match_operand:<VHMODE> 1 "register_operand" "f")
			  (match_operand:<VHMODE> 2 "register_operand" "f")]
			 UNSPEC_MSA_HSUB_S))]
  "ISA_HAS_MSA"
  "hsub_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_hsub_u_<msafmt>"
  [(set (match_operand:IMSA_DWH 0 "register_operand" "=f")
	(unspec:IMSA_DWH [(match_operand:<VHMODE> 1 "register_operand" "f")
			  (match_operand:<VHMODE> 2 "register_operand" "f")]
			 UNSPEC_MSA_HSUB_U))]
  "ISA_HAS_MSA"
  "hsub_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_ilvev_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(vec_select:V16QI
	  (vec_concat:V32QI
	    (match_operand:V16QI 1 "register_operand" "f")
	    (match_operand:V16QI 2 "register_operand" "f"))
	  (parallel [(const_int 0)  (const_int 16)
		     (const_int 2)  (const_int 18)
		     (const_int 4)  (const_int 20)
		     (const_int 6)  (const_int 22)
		     (const_int 8)  (const_int 24)
		     (const_int 10) (const_int 26)
		     (const_int 12) (const_int 28)
		     (const_int 14) (const_int 30)])))]
  "ISA_HAS_MSA"
  "ilvev.b\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V16QI")])

(define_insn "msa_ilvev_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(vec_select:V8HI
	  (vec_concat:V16HI
	    (match_operand:V8HI 1 "register_operand" "f")
	    (match_operand:V8HI 2 "register_operand" "f"))
	  (parallel [(const_int 0) (const_int 8)
		     (const_int 2) (const_int 10)
		     (const_int 4) (const_int 12)
		     (const_int 6) (const_int 14)])))]
  "ISA_HAS_MSA"
  "ilvev.h\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V8HI")])

(define_insn "msa_ilvev_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(vec_select:V4SI
	  (vec_concat:V8SI
	    (match_operand:V4SI 1 "register_operand" "f")
	    (match_operand:V4SI 2 "register_operand" "f"))
	  (parallel [(const_int 0) (const_int 4)
		     (const_int 2) (const_int 6)])))]
  "ISA_HAS_MSA"
  "ilvev.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SI")])

(define_insn "msa_ilvev_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(vec_select:V4SF
	  (vec_concat:V8SF
	    (match_operand:V4SF 1 "register_operand" "f")
	    (match_operand:V4SF 2 "register_operand" "f"))
	  (parallel [(const_int 0) (const_int 4)
		     (const_int 2) (const_int 6)])))]
  "ISA_HAS_MSA"
  "ilvev.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SF")])

(define_insn "msa_ilvl_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(vec_select:V16QI
	  (vec_concat:V32QI
	    (match_operand:V16QI 1 "register_operand" "f")
	    (match_operand:V16QI 2 "register_operand" "f"))
	  (parallel [(const_int 8)  (const_int 24)
		     (const_int 9)  (const_int 25)
		     (const_int 10) (const_int 26)
		     (const_int 11) (const_int 27)
		     (const_int 12) (const_int 28)
		     (const_int 13) (const_int 29)
		     (const_int 14) (const_int 30)
		     (const_int 15) (const_int 31)])))]
  "ISA_HAS_MSA"
  "ilvl.b\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V16QI")])

(define_insn "msa_ilvl_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(vec_select:V8HI
	  (vec_concat:V16HI
	    (match_operand:V8HI 1 "register_operand" "f")
	    (match_operand:V8HI 2 "register_operand" "f"))
	  (parallel [(const_int 4) (const_int 12)
		     (const_int 5) (const_int 13)
		     (const_int 6) (const_int 14)
		     (const_int 7) (const_int 15)])))]
  "ISA_HAS_MSA"
  "ilvl.h\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V8HI")])

(define_insn "msa_ilvl_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(vec_select:V4SI
	  (vec_concat:V8SI
	    (match_operand:V4SI 1 "register_operand" "f")
	    (match_operand:V4SI 2 "register_operand" "f"))
	  (parallel [(const_int 2) (const_int 6)
		     (const_int 3) (const_int 7)])))]
  "ISA_HAS_MSA"
  "ilvl.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SI")])

(define_insn "msa_ilvl_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(vec_select:V4SF
	  (vec_concat:V8SF
	    (match_operand:V4SF 1 "register_operand" "f")
	    (match_operand:V4SF 2 "register_operand" "f"))
	  (parallel [(const_int 2) (const_int 6)
		     (const_int 3) (const_int 7)])))]
  "ISA_HAS_MSA"
  "ilvl.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SF")])

(define_insn "msa_ilvl_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(vec_select:V2DI
	  (vec_concat:V4DI
	    (match_operand:V2DI 1 "register_operand" "f")
	    (match_operand:V2DI 2 "register_operand" "f"))
	  (parallel [(const_int 1) (const_int 3)])))]
  "ISA_HAS_MSA"
  "ilvl.d\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V2DI")])

(define_insn "msa_ilvl_d_f"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(vec_select:V2DF
	  (vec_concat:V4DF
	    (match_operand:V2DF 1 "register_operand" "f")
	    (match_operand:V2DF 2 "register_operand" "f"))
	  (parallel [(const_int 1) (const_int 3)])))]
  "ISA_HAS_MSA"
  "ilvl.d\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V2DF")])

(define_insn "msa_ilvod_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(vec_select:V16QI
	  (vec_concat:V32QI
	    (match_operand:V16QI 1 "register_operand" "f")
	    (match_operand:V16QI 2 "register_operand" "f"))
	  (parallel [(const_int 1)  (const_int 17)
		     (const_int 3)  (const_int 19)
		     (const_int 5)  (const_int 21)
		     (const_int 7)  (const_int 23)
		     (const_int 9)  (const_int 25)
		     (const_int 11) (const_int 27)
		     (const_int 13) (const_int 29)
		     (const_int 15) (const_int 31)])))]
  "ISA_HAS_MSA"
  "ilvod.b\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V16QI")])

(define_insn "msa_ilvod_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(vec_select:V8HI
	  (vec_concat:V16HI
	    (match_operand:V8HI 1 "register_operand" "f")
	    (match_operand:V8HI 2 "register_operand" "f"))
	  (parallel [(const_int 1) (const_int 9)
		     (const_int 3) (const_int 11)
		     (const_int 5) (const_int 13)
		     (const_int 7) (const_int 15)])))]
  "ISA_HAS_MSA"
  "ilvod.h\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V8HI")])

(define_insn "msa_ilvod_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(vec_select:V4SI
	  (vec_concat:V8SI
	    (match_operand:V4SI 1 "register_operand" "f")
	    (match_operand:V4SI 2 "register_operand" "f"))
	  (parallel [(const_int 1) (const_int 5)
		     (const_int 3) (const_int 7)])))]
  "ISA_HAS_MSA"
  "ilvod.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SI")])

(define_insn "msa_ilvod_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(vec_select:V4SF
	  (vec_concat:V8SF
	    (match_operand:V4SF 1 "register_operand" "f")
	    (match_operand:V4SF 2 "register_operand" "f"))
	  (parallel [(const_int 1) (const_int 5)
		     (const_int 3) (const_int 7)])))]
  "ISA_HAS_MSA"
  "ilvod.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SF")])

(define_insn "msa_ilvr_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(vec_select:V16QI
	  (vec_concat:V32QI
	    (match_operand:V16QI 1 "register_operand" "f")
	    (match_operand:V16QI 2 "register_operand" "f"))
	  (parallel [(const_int 0) (const_int 16)
		     (const_int 1) (const_int 17)
		     (const_int 2) (const_int 18)
		     (const_int 3) (const_int 19)
		     (const_int 4) (const_int 20)
		     (const_int 5) (const_int 21)
		     (const_int 6) (const_int 22)
		     (const_int 7) (const_int 23)])))]
  "ISA_HAS_MSA"
  "ilvr.b\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V16QI")])

(define_insn "msa_ilvr_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(vec_select:V8HI
	  (vec_concat:V16HI
	    (match_operand:V8HI 1 "register_operand" "f")
	    (match_operand:V8HI 2 "register_operand" "f"))
	  (parallel [(const_int 0) (const_int 8)
		     (const_int 1) (const_int 9)
		     (const_int 2) (const_int 10)
		     (const_int 3) (const_int 11)])))]
  "ISA_HAS_MSA"
  "ilvr.h\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V8HI")])

(define_insn "msa_ilvr_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(vec_select:V4SI
	  (vec_concat:V8SI
	    (match_operand:V4SI 1 "register_operand" "f")
	    (match_operand:V4SI 2 "register_operand" "f"))
	  (parallel [(const_int 0) (const_int 4)
		     (const_int 1) (const_int 5)])))]
  "ISA_HAS_MSA"
  "ilvr.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SI")])

(define_insn "msa_ilvr_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(vec_select:V4SF
	  (vec_concat:V8SF
	    (match_operand:V4SF 1 "register_operand" "f")
	    (match_operand:V4SF 2 "register_operand" "f"))
	  (parallel [(const_int 0) (const_int 4)
		     (const_int 1) (const_int 5)])))]
  "ISA_HAS_MSA"
  "ilvr.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SF")])

(define_insn "msa_ilvr_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(vec_select:V2DI
	  (vec_concat:V4DI
	    (match_operand:V2DI 1 "register_operand" "f")
	    (match_operand:V2DI 2 "register_operand" "f"))
	  (parallel [(const_int 0) (const_int 2)])))]
  "ISA_HAS_MSA"
  "ilvr.d\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V2DI")])

(define_insn "msa_ilvr_d_f"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(vec_select:V2DF
	  (vec_concat:V4DF
	    (match_operand:V2DF 1 "register_operand" "f")
	    (match_operand:V2DF 2 "register_operand" "f"))
	  (parallel [(const_int 0) (const_int 2)])))]
  "ISA_HAS_MSA"
  "ilvr.d\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V2DF")])

(define_insn "msa_madd_q_<msafmt>"
  [(set (match_operand:IMSA_WH 0 "register_operand" "=f")
	(unspec:IMSA_WH [(match_operand:IMSA_WH 1 "register_operand" "0")
			 (match_operand:IMSA_WH 2 "register_operand" "f")
			 (match_operand:IMSA_WH 3 "register_operand" "f")]
			UNSPEC_MSA_MADD_Q))]
  "ISA_HAS_MSA"
  "madd_q.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_maddr_q_<msafmt>"
  [(set (match_operand:IMSA_WH 0 "register_operand" "=f")
	(unspec:IMSA_WH [(match_operand:IMSA_WH 1 "register_operand" "0")
			 (match_operand:IMSA_WH 2 "register_operand" "f")
			 (match_operand:IMSA_WH 3 "register_operand" "f")]
			UNSPEC_MSA_MADDR_Q))]
  "ISA_HAS_MSA"
  "maddr_q.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_max_a_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_MAX_A))]
  "ISA_HAS_MSA"
  "max_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "smax<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(smax:IMSA (match_operand:IMSA 1 "register_operand" "f")
		   (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "max_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "umax<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(umax:IMSA (match_operand:IMSA 1 "register_operand" "f")
		   (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "max_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_maxi_s_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_imm5_operand" "")]
		     UNSPEC_MSA_MAXI_S))]
  "ISA_HAS_MSA"
  "maxi_s.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_maxi_u_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_MAXI_U))]
  "ISA_HAS_MSA"
  "maxi_u.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_min_a_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_MIN_A))]
  "ISA_HAS_MSA"
  "min_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "smin<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(smin:IMSA (match_operand:IMSA 1 "register_operand" "f")
		   (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "min_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "umin<mode>3"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(umin:IMSA (match_operand:IMSA 1 "register_operand" "f")
		   (match_operand:IMSA 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "min_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_mini_s_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_imm5_operand" "")]
		     UNSPEC_MSA_MINI_S))]
  "ISA_HAS_MSA"
  "mini_s.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_mini_u_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_MINI_U))]
  "ISA_HAS_MSA"
  "mini_u.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_msub_q_<msafmt>"
  [(set (match_operand:IMSA_WH 0 "register_operand" "=f")
	(unspec:IMSA_WH [(match_operand:IMSA_WH 1 "register_operand" "0")
			 (match_operand:IMSA_WH 2 "register_operand" "f")
			 (match_operand:IMSA_WH 3 "register_operand" "f")]
			UNSPEC_MSA_MSUB_Q))]
  "ISA_HAS_MSA"
  "msub_q.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_msubr_q_<msafmt>"
  [(set (match_operand:IMSA_WH 0 "register_operand" "=f")
	(unspec:IMSA_WH [(match_operand:IMSA_WH 1 "register_operand" "0")
			 (match_operand:IMSA_WH 2 "register_operand" "f")
			 (match_operand:IMSA_WH 3 "register_operand" "f")]
			UNSPEC_MSA_MSUBR_Q))]
  "ISA_HAS_MSA"
  "msubr_q.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_mul_q_<msafmt>"
  [(set (match_operand:IMSA_WH 0 "register_operand" "=f")
	(unspec:IMSA_WH [(match_operand:IMSA_WH 1 "register_operand" "f")
			 (match_operand:IMSA_WH 2 "register_operand" "f")]
			UNSPEC_MSA_MUL_Q))]
  "ISA_HAS_MSA"
  "mul_q.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_mulr_q_<msafmt>"
  [(set (match_operand:IMSA_WH 0 "register_operand" "=f")
	(unspec:IMSA_WH [(match_operand:IMSA_WH 1 "register_operand" "f")
			 (match_operand:IMSA_WH 2 "register_operand" "f")]
			UNSPEC_MSA_MULR_Q))]
  "ISA_HAS_MSA"
  "mulr_q.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_mul")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_nloc_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")]
		     UNSPEC_MSA_NLOC))]
  "ISA_HAS_MSA"
  "nloc.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_bit")
   (set_attr "mode" "<MODE>")])

(define_insn "clz<mode>2"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(clz:IMSA (match_operand:IMSA 1 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "nlzc.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_bit")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_nor_v_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(and:IMSA (not:IMSA (match_operand:IMSA 1 "register_operand" "f"))
		  (not:IMSA (match_operand:IMSA 2 "register_operand" "f"))))]
  "ISA_HAS_MSA"
  "nor.v\t%w0,%w1,%w2"
  [(set_attr "type" "simd_logic")
   (set_attr "mode" "TI")])

(define_insn "msa_nori_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm8_operand" "")]
		      UNSPEC_MSA_NORI_B))]
  "ISA_HAS_MSA"
  "nori.b\t%w0,%w1,%2"
  [(set_attr "type" "simd_logic")
   (set_attr "mode" "V16QI")])

(define_insn "msa_ori_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(ior:V16QI (match_operand:V16QI 1 "register_operand" "f")
		   (match_operand 2 "const_uimm8_operand" "")))]
  "ISA_HAS_MSA"
  "ori.b\t%w0,%w1,%2"
  [(set_attr "type" "simd_logic")
   (set_attr "mode" "V16QI")])

(define_insn "msa_pckev_b"
[(set (match_operand:V16QI 0 "register_operand" "=f")
      (vec_select:V16QI
	(vec_concat:V32QI
	  (match_operand:V16QI 1 "register_operand" "f")
	  (match_operand:V16QI 2 "register_operand" "f"))
	(parallel [(const_int 0) (const_int 2)
		   (const_int 4) (const_int 6)
		   (const_int 8) (const_int 10)
		   (const_int 12) (const_int 14)
		   (const_int 16) (const_int 18)
		   (const_int 20) (const_int 22)
		   (const_int 24) (const_int 26)
		   (const_int 28) (const_int 30)])))]
  "ISA_HAS_MSA"
  "pckev.b\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V16QI")])

(define_insn "msa_pckev_h"
[(set (match_operand:V8HI 0 "register_operand" "=f")
      (vec_select:V8HI
	(vec_concat:V16HI
	  (match_operand:V8HI 1 "register_operand" "f")
	  (match_operand:V8HI 2 "register_operand" "f"))
	(parallel [(const_int 0) (const_int 2)
		   (const_int 4) (const_int 6)
		   (const_int 8) (const_int 10)
		   (const_int 12) (const_int 14)])))]
  "ISA_HAS_MSA"
  "pckev.h\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V8HI")])

(define_insn "msa_pckev_w"
[(set (match_operand:V4SI 0 "register_operand" "=f")
      (vec_select:V4SI
	(vec_concat:V8SI
	  (match_operand:V4SI 1 "register_operand" "f")
	  (match_operand:V4SI 2 "register_operand" "f"))
	(parallel [(const_int 0) (const_int 2)
		   (const_int 4) (const_int 6)])))]
  "ISA_HAS_MSA"
  "pckev.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SI")])

(define_insn "msa_pckev_w_f"
[(set (match_operand:V4SF 0 "register_operand" "=f")
      (vec_select:V4SF
	(vec_concat:V8SF
	  (match_operand:V4SF 1 "register_operand" "f")
	  (match_operand:V4SF 2 "register_operand" "f"))
	(parallel [(const_int 0) (const_int 2)
		   (const_int 4) (const_int 6)])))]
  "ISA_HAS_MSA"
  "pckev.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SF")])

(define_insn "msa_pckod_b"
[(set (match_operand:V16QI 0 "register_operand" "=f")
      (vec_select:V16QI
	(vec_concat:V32QI
	  (match_operand:V16QI 1 "register_operand" "f")
	  (match_operand:V16QI 2 "register_operand" "f"))
	(parallel [(const_int 1) (const_int 3)
		   (const_int 5) (const_int 7)
		   (const_int 9) (const_int 11)
		   (const_int 13) (const_int 15)
		   (const_int 17) (const_int 19)
		   (const_int 21) (const_int 23)
		   (const_int 25) (const_int 27)
		   (const_int 29) (const_int 31)])))]
  "ISA_HAS_MSA"
  "pckod.b\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V16QI")])

(define_insn "msa_pckod_h"
[(set (match_operand:V8HI 0 "register_operand" "=f")
      (vec_select:V8HI
	(vec_concat:V16HI
	  (match_operand:V8HI 1 "register_operand" "f")
	  (match_operand:V8HI 2 "register_operand" "f"))
	(parallel [(const_int 1) (const_int 3)
		   (const_int 5) (const_int 7)
		   (const_int 9) (const_int 11)
		   (const_int 13) (const_int 15)])))]
  "ISA_HAS_MSA"
  "pckod.h\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V8HI")])

(define_insn "msa_pckod_w"
[(set (match_operand:V4SI 0 "register_operand" "=f")
      (vec_select:V4SI
	(vec_concat:V8SI
	  (match_operand:V4SI 1 "register_operand" "f")
	  (match_operand:V4SI 2 "register_operand" "f"))
	(parallel [(const_int 1) (const_int 3)
		   (const_int 5) (const_int 7)])))]
  "ISA_HAS_MSA"
  "pckod.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SI")])

(define_insn "msa_pckod_w_f"
[(set (match_operand:V4SF 0 "register_operand" "=f")
      (vec_select:V4SF
	(vec_concat:V8SF
	  (match_operand:V4SF 1 "register_operand" "f")
	  (match_operand:V4SF 2 "register_operand" "f"))
	(parallel [(const_int 1) (const_int 3)
		   (const_int 5) (const_int 7)])))]
  "ISA_HAS_MSA"
  "pckod.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_permute")
   (set_attr "mode" "V4SF")])

(define_insn "popcount<mode>2"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(popcount:IMSA (match_operand:IMSA 1 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "pcnt.<msafmt>\t%w0,%w1"
  [(set_attr "type" "simd_pcnt")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_sat_s_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_SAT_S))]
  "ISA_HAS_MSA"
  "sat_s.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_sat")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_sat_u_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_SAT_U))]
  "ISA_HAS_MSA"
  "sat_u.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_sat")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_shf_<msafmt>"
  [(set (match_operand:IMSA_WHB 0 "register_operand" "=f")
	(unspec:IMSA_WHB [(match_operand:IMSA_WHB 1 "register_operand" "f")
			  (match_operand 2 "const_uimm8_operand" "")]
			 UNSPEC_MSA_SHF))]
  "ISA_HAS_MSA"
  "shf.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_shf")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_shf_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V4SF 1 "register_operand" "f")
		      (match_operand 2 "const_uimm8_operand" "")]
		     UNSPEC_MSA_SHF))]
  "ISA_HAS_MSA"
  "shf.w\t%w0,%w1,%2"
  [(set_attr "type" "simd_shf")
   (set_attr "mode" "V4SF")])

(define_insn "msa_slli_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_SLLI))]
  "ISA_HAS_MSA"
  "slli.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_shift")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_srai_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_SRAI))]
  "ISA_HAS_MSA"
  "srai.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_shift")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_srar_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_SRAR))]
  "ISA_HAS_MSA"
  "srar.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_shift")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_srari_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_SRARI))]
  "ISA_HAS_MSA"
  "srari.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_shift")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_srli_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_SRLI))]
  "ISA_HAS_MSA"
  "srli.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_shift")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_srlr_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_SRLR))]
  "ISA_HAS_MSA"
  "srlr.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_shift")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_srlri_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_<bitimm>_operand" "")]
		     UNSPEC_MSA_SRLRI))]
  "ISA_HAS_MSA"
  "srlri.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_shift")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_subs_s_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_SUBS_S))]
  "ISA_HAS_MSA"
  "subs_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_subs_u_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_SUBS_U))]
  "ISA_HAS_MSA"
  "subs_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_subsuu_s_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_SUBSUU_S))]
  "ISA_HAS_MSA"
  "subsuu_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_subsus_u_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand:IMSA 2 "register_operand" "f")]
		     UNSPEC_MSA_SUBSUS_U))]
  "ISA_HAS_MSA"
  "subsus_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_subvi_<msafmt>"
  [(set (match_operand:IMSA 0 "register_operand" "=f")
	(unspec:IMSA [(match_operand:IMSA 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_SUBVI))]
  "ISA_HAS_MSA"
  "subvi.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type" "simd_int_arith")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_xori_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm8_operand" "")]
		      UNSPEC_MSA_XORI_B))]
  "ISA_HAS_MSA"
  "xori.b\t%w0,%w1,%2"
  [(set_attr "type" "simd_logic")
   (set_attr "mode" "V16QI")])

(define_insn "msa_sld_<msafmt_f>"
  [(set (match_operand:MSA 0 "register_operand" "=f")
	(unspec:MSA [(match_operand:MSA 1 "register_operand" "0")
		     (match_operand:MSA 2 "register_operand" "f")
		     (match_operand:SI 3 "reg_or_0_operand" "dJ")]
		    UNSPEC_MSA_SLD))]
  "ISA_HAS_MSA"
  "sld.<msafmt>\t%w0,%w2[%z3]"
  [(set_attr "type" "simd_sld")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_sldi_<msafmt_f>"
  [(set (match_operand:MSA 0 "register_operand" "=f")
	(unspec:MSA [(match_operand:MSA 1 "register_operand" "0")
		     (match_operand:MSA 2 "register_operand" "f")
		     (match_operand 3 "const_<indeximm>_operand" "")]
		    UNSPEC_MSA_SLDI))]
  "ISA_HAS_MSA"
  "sldi.<msafmt>\t%w0,%w2[%3]"
  [(set_attr "type" "simd_sld")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_splat_<msafmt_f>"
  [(set (match_operand:MSA 0 "register_operand" "=f")
	(unspec:MSA [(match_operand:MSA 1 "register_operand" "f")
		     (match_operand:SI 2 "reg_or_0_operand" "dJ")]
		    UNSPEC_MSA_SPLAT))]
  "ISA_HAS_MSA"
  "splat.<msafmt>\t%w0,%w1[%z2]"
  [(set_attr "type" "simd_splat")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_splati_<msafmt_f>"
  [(set (match_operand:MSA 0 "register_operand" "=f")
	(unspec:MSA [(match_operand:MSA 1 "register_operand" "f")
		     (match_operand 2 "const_<indeximm>_operand" "")]
		    UNSPEC_MSA_SPLATI))]
  "ISA_HAS_MSA"
  "splati.<msafmt>\t%w0,%w1[%2]"
  [(set_attr "type" "simd_splat")
   (set_attr "mode" "<MODE>")])

;; Operand 1 is a scalar.
(define_insn "msa_splati_<msafmt_f>_s"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(unspec:FMSA [(match_operand:<UNITMODE> 1 "register_operand" "f")
		      (match_operand 2 "const_<indeximm>_operand" "")]
		     UNSPEC_MSA_SPLATI))]
  "ISA_HAS_MSA"
  "splati.<msafmt>\t%w0,%w1[%2]"
  [(set_attr "type" "simd_splat")
   (set_attr "mode" "<MODE>")])

(define_insn "msa_cfcmsa"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec_volatile:SI [(match_operand 1 "const_uimm5_operand" "")]
			    UNSPEC_MSA_CFCMSA))]
  "ISA_HAS_MSA"
  "cfcmsa\t%0,$%1"
  [(set_attr "type" "simd_cmsa")
   (set_attr "mode" "SI")])

(define_insn "msa_ctcmsa"
  [(unspec_volatile [(match_operand 0 "const_uimm5_operand" "")
		     (match_operand:SI 1 "register_operand" "d")]
		    UNSPEC_MSA_CTCMSA)]
  "ISA_HAS_MSA"
  "ctcmsa\t$%0,%1"
  [(set_attr "type" "simd_cmsa")
   (set_attr "mode" "SI")])

(define_insn "msa_fexdo_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V4SF 1 "register_operand" "f")
		      (match_operand:V4SF 2 "register_operand" "f")]
		     UNSPEC_MSA_FEXDO))]
  "ISA_HAS_MSA"
  "fexdo.h\t%w0,%w1,%w2"
  [(set_attr "type" "simd_fcvt")
   (set_attr "mode" "V8HI")])

(define_insn "msa_fexdo_w"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(vec_concat:V4SF
	  (float_truncate:V2SF (match_operand:V2DF 1 "register_operand" "f"))
	  (float_truncate:V2SF (match_operand:V2DF 2 "register_operand" "f"))))]
  "ISA_HAS_MSA"
  "fexdo.w\t%w0,%w2,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "mode" "V4SF")])

(define_insn "msa_fexupl_w"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V8HI 1 "register_operand" "f")]
		     UNSPEC_MSA_FEXUPL))]
  "ISA_HAS_MSA"
  "fexupl.w\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "mode" "V4SF")])

(define_insn "msa_fexupl_d"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V4SF 1 "register_operand" "f")]
		     UNSPEC_MSA_FEXUPL))]
  "ISA_HAS_MSA"
  "fexupl.d\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "mode" "V2DF")])

(define_insn "msa_fexupr_w"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
       (unspec:V4SF [(match_operand:V8HI 1 "register_operand" "f")]
		    UNSPEC_MSA_FEXUPR))]
  "ISA_HAS_MSA"
  "fexupr.w\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "mode" "V4SF")])

(define_insn "msa_fexupr_d"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
       (unspec:V2DF [(match_operand:V4SF 1 "register_operand" "f")]
		    UNSPEC_MSA_FEXUPR))]
  "ISA_HAS_MSA"
  "fexupr.d\t%w0,%w1"
  [(set_attr "type" "simd_fcvt")
   (set_attr "mode" "V2DF")])

(define_insn "msa_branch_nz_v_<msafmt_f>"
 [(set (pc) (if_then_else
	      (ne (unspec:SI [(match_operand:MSA 1 "register_operand" "f")]
			     UNSPEC_MSA_BNZ_V)
		  (match_operand:SI 2 "const_0_operand"))
		  (label_ref (match_operand 0))
		  (pc)))]
 "ISA_HAS_MSA"
{
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("bnz.v", "%w1,%0"),
					 MIPS_BRANCH ("bz.v", "%w1,%0"));
}
 [(set_attr "type" "simd_branch")
  (set_attr "mode" "TI")])

(define_expand "msa_bnz_v_<msafmt_f>"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:MSA 1 "register_operand" "f")]
		   UNSPEC_MSA_TSTNZ_V))]
  "ISA_HAS_MSA"
{
  mips_expand_msa_branch (operands, gen_msa_branch_nz_v_<MSA:msafmt_f>);
  DONE;
})

(define_insn "msa_branchz_v_<msafmt_f>"
 [(set (pc) (if_then_else
	      (eq (unspec:SI [(match_operand:MSA 1 "register_operand" "f")]
			     UNSPEC_MSA_BZ_V)
		  (match_operand:SI 2 "const_0_operand"))
		  (label_ref (match_operand 0))
		  (pc)))]
 "ISA_HAS_MSA"
{
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("bz.v", "%w1,%0"),
					 MIPS_BRANCH ("bnz.v", "%w1,%0"));
}
 [(set_attr "type" "simd_branch")
  (set_attr "mode" "TI")])

(define_expand "msa_bz_v_<msafmt_f>"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:MSA 1 "register_operand" "f")]
		   UNSPEC_MSA_TSTZ_V))]
  "ISA_HAS_MSA"
{
  mips_expand_msa_branch (operands, gen_msa_branchz_v_<MSA:msafmt_f>);
  DONE;
})

(define_insn "msa_branchnz_<msafmt_f>"
 [(set (pc) (if_then_else
	      (ne (unspec:SI [(match_operand:MSA 1 "register_operand" "f")]
			     UNSPEC_MSA_BNZ)
		  (match_operand:SI 2 "const_0_operand"))
		  (label_ref (match_operand 0))
		  (pc)))]
 "ISA_HAS_MSA"
{
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("bnz.<msafmt>", "%w1,%0"),
					 MIPS_BRANCH ("bz.<msafmt>", "%w1,%0"));

}

 [(set_attr "type" "simd_branch")
  (set_attr "mode" "<MODE>")])

(define_expand "msa_bnz_<msafmt>"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:IMSA 1 "register_operand" "f")]
		   UNSPEC_MSA_TSTNZ))]
  "ISA_HAS_MSA"
{
  mips_expand_msa_branch (operands, gen_msa_branchnz_<IMSA:msafmt>);
  DONE;
})

(define_insn "msa_branchz_<msafmt>"
 [(set (pc) (if_then_else
	      (eq (unspec:SI [(match_operand:IMSA 1 "register_operand" "f")]
			     UNSPEC_MSA_BZ)
		   (match_operand:IMSA 2 "const_0_operand"))
		  (label_ref (match_operand 0))
		  (pc)))]
 "ISA_HAS_MSA"
{
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("bz.<msafmt>", "%w1,%0"),
					 MIPS_BRANCH ("bnz.<msafmt>","%w1,%0"));
}
 [(set_attr "type" "simd_branch")
  (set_attr "mode" "<MODE>")])

(define_expand "msa_bz_<msafmt>"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:IMSA 1 "register_operand" "f")]
		   UNSPEC_MSA_TSTZ))]
  "ISA_HAS_MSA"
{
  mips_expand_msa_branch (operands, gen_msa_branchz_<IMSA:msafmt>);
  DONE;
})

;; Note that this instruction treats scalar as vector registers freely.
(define_insn "msa_cast_to_vector_<msafmt_f>"
  [(set (match_operand:FMSA 0 "register_operand" "=f")
	(unspec:FMSA [(match_operand:<UNITMODE> 1 "register_operand" "f")]
		     UNSPEC_MSA_CAST_TO_VECTOR))]
  "ISA_HAS_MSA"
{
  if (REGNO (operands[0]) == REGNO (operands[1]))
    return "nop\t# Cast %1 to %w0";
  else
    return "mov.<unitfmt>\t%0,%1\t# Cast %1 to %w0";
}
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])

;; Note that this instruction treats vector as scalar registers freely.
(define_insn "msa_cast_to_scalar_<msafmt_f>"
  [(set (match_operand:<UNITMODE> 0 "register_operand" "=f")
	(unspec:<UNITMODE> [(match_operand:FMSA 1 "register_operand" "f")]
			   UNSPEC_MSA_CAST_TO_SCALAR))]
  "ISA_HAS_MSA"
{
  if (REGNO (operands[0]) == REGNO (operands[1]))
    return "nop\t# Cast %w1 to %0";
  else
    return "mov.<unitfmt>\t%0,%1\t# Cast %w1 to %0";
}
  [(set_attr "type" "arith")
   (set_attr "mode" "TI")])
