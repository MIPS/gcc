;; Machine Description for MIPS MSA ASE
;; Based on the MIPS MSA spec Revision 0.??? 11/5/2012
;; Contributed by Chao-ying Fu (fu@mips.com), MIPS Technologies, Inc.
;;
;; Copyright (C) 2012 Free Software Foundation, Inc.
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
  UNSPEC_MSA_ADD_A
  UNSPEC_MSA_ADDS_A
  UNSPEC_MSA_ADDS_S
  UNSPEC_MSA_ADDS_U
  UNSPEC_MSA_ADDVI
  UNSPEC_MSA_ANDI_B
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
  UNSPEC_MSA_BN_V
  UNSPEC_MSA_BZ_V
  UNSPEC_MSA_BN
  UNSPEC_MSA_BZ
  UNSPEC_MSA_CEQ
  UNSPEC_MSA_CFCMSA
  UNSPEC_MSA_CEQI
  UNSPEC_MSA_CLE_S
  UNSPEC_MSA_CLE_U
  UNSPEC_MSA_CLEI_S
  UNSPEC_MSA_CLEI_U
  UNSPEC_MSA_CLT_S
  UNSPEC_MSA_CLT_U
  UNSPEC_MSA_CLTI_S
  UNSPEC_MSA_CLTI_U
  UNSPEC_MSA_COPY_S
  UNSPEC_MSA_COPY_U
  UNSPEC_MSA_CTCMSA
  UNSPEC_MSA_DOTP_S
  UNSPEC_MSA_DOTP_U
  UNSPEC_MSA_DPADD_S
  UNSPEC_MSA_DPADD_U
  UNSPEC_MSA_DPSUB_S
  UNSPEC_MSA_DPSUB_U
  UNSPEC_MSA_FCAF
  UNSPEC_MSA_FCEQ
  UNSPEC_MSA_FCLASS
  UNSPEC_MSA_FCLE
  UNSPEC_MSA_FCLT
  UNSPEC_MSA_FCNE
  UNSPEC_MSA_FCOR
  UNSPEC_MSA_FCUEQ
  UNSPEC_MSA_FCULE
  UNSPEC_MSA_FCULT
  UNSPEC_MSA_FCUN
  UNSPEC_MSA_FCUNE
  UNSPEC_MSA_FEXDO
  UNSPEC_MSA_FEXP2
  UNSPEC_MSA_FEXUPL
  UNSPEC_MSA_FEXUPR
  UNSPEC_MSA_FFINT_S
  UNSPEC_MSA_FFINT_U
  UNSPEC_MSA_FFQL
  UNSPEC_MSA_FFQR
  UNSPEC_MSA_FILL
  UNSPEC_MSA_FLOG2
  UNSPEC_MSA_FMAX
  UNSPEC_MSA_FMAX_A
  UNSPEC_MSA_FMIN
  UNSPEC_MSA_FMIN_A
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
  UNSPEC_MSA_ILVEV
  UNSPEC_MSA_ILVL
  UNSPEC_MSA_ILVOD
  UNSPEC_MSA_ILVR
  UNSPEC_MSA_INSERT
  UNSPEC_MSA_INSVE
  UNSPEC_MSA_LDI
  UNSPEC_MSA_MADD_Q
  UNSPEC_MSA_MADDR_Q
  UNSPEC_MSA_MAX_A
  UNSPEC_MSA_MAX_S
  UNSPEC_MSA_MAX_U
  UNSPEC_MSA_MAXI_S
  UNSPEC_MSA_MAXI_U
  UNSPEC_MSA_MIN_A
  UNSPEC_MSA_MIN_S
  UNSPEC_MSA_MIN_U
  UNSPEC_MSA_MINI_S
  UNSPEC_MSA_MINI_U
  UNSPEC_MSA_MSUB_Q
  UNSPEC_MSA_MSUBR_Q
  UNSPEC_MSA_MUL_Q
  UNSPEC_MSA_MULR_Q
  UNSPEC_MSA_NLOC
  UNSPEC_MSA_NLZC
  UNSPEC_MSA_NORI_B
  UNSPEC_MSA_ORI_B
  UNSPEC_MSA_PCKEV
  UNSPEC_MSA_PCKOD
  UNSPEC_MSA_PCNT
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
  UNSPEC_MSA_VSHF
  UNSPEC_MSA_XORI_B
  UNSPEC_MSA_CAST_TO_SCALAR
  UNSPEC_MSA_CAST_TO_VECTOR
])

;; All vector modes with 128 bits.
(define_mode_iterator MODE128 [V2DF V4SF V2DI V4SI V8HI V16QI])

;; Same as MODE128.  Used by vcond to iterate two modes.
(define_mode_iterator MODE128_2 [V2DF V4SF V2DI V4SI V8HI V16QI])

;; Only integer modes.
(define_mode_iterator IMODE128 [V2DI V4SI V8HI V16QI])

;; Only integer modes for fixed-point madd_q/maddr_q.
(define_mode_iterator QMODE128 [V4SI V8HI])

;; Only floating-point modes.
(define_mode_iterator FMODE128 [V2DF V4SF])

;; This attribute gives the integer mode for selection mask in vec_perm.
;; vcond also uses MODE128_I for operand 0, 1, and 2.
(define_mode_attr MODE128_I
  [(V2DF "V2DI")
   (V4SF "V4SI")
   (V2DI "V2DI")
   (V4SI "V4SI")
   (V8HI "V8HI")
   (V16QI "V16QI")])

;; This attributes gives the element mode for vector mode.
(define_mode_attr ELEMMODE
  [(V2DF "DF")
   (V4SF "SF")
   (V2DI "DI")
   (V4SI "SI")
   (V8HI "HI")
   (V16QI "QI")])

(define_mode_attr msafmt
  [(V2DF "d")
   (V4SF "w")
   (V2DI "d")
   (V4SI "w")
   (V8HI "h")
   (V16QI "b")])

;; This is used in msa_cast* to output mov.s or mov.d.
(define_mode_attr msafmt2
  [(V2DF "d")
   (V4SF "s")])

;; The maximum index inside a vector.
(define_mode_attr max_elem_index
  [(V2DF "1")
   (V4SF "3")
   (V2DI "1")
   (V4SI "3")
   (V8HI "7")
   (V16QI "15")])

;; The mask for shift amounts.
(define_mode_attr shift_mask
  [(V2DI "63")
   (V4SI "31")
   (V8HI "15")
   (V16QI "7")])

(define_expand "vec_init<mode>"
  [(match_operand:MODE128 0 "register_operand")
   (match_operand:MODE128 1 "")]
  "ISA_HAS_MSA"
{
  mips_expand_vector_init (operands[0], operands[1]);
  DONE;
})

(define_expand "vec_extract<mode>"
  [(match_operand:<ELEMMODE> 0 "register_operand")
   (match_operand:IMODE128 1 "register_operand")
   (match_operand 2 "const_int_operand")]
  "ISA_HAS_MSA"
{
  gcc_assert (INTVAL (operands[2]) <= <max_elem_index>);
  enum machine_mode mode0 = GET_MODE (operands[0]);
  if (mode0 == QImode || mode0 == HImode)
    {
      unsigned int byte = 0;
      rtx dest1 = gen_reg_rtx (SImode);
      emit_insn (gen_msa_copy_s_<msafmt> (dest1, operands[1], operands[2]));
      if (TARGET_BIG_ENDIAN)
	{
	  if (mode0 == QImode)
	    byte = 3;
	  else if (mode0 == HImode)
	    byte = 2;
	}
      rtx dest2 = simplify_gen_subreg (mode0, dest1, SImode, byte);
      emit_move_insn (operands[0], dest2);
    }
  else
    emit_insn (gen_msa_copy_s_<msafmt> (operands[0], operands[1], operands[2]));
  DONE;
})

(define_expand "vec_extract<mode>"
  [(match_operand:<ELEMMODE> 0 "register_operand")
   (match_operand:FMODE128 1 "register_operand")
   (match_operand 2 "const_int_operand")]
  "ISA_HAS_MSA"
{
  rtx temp;
  gcc_assert (INTVAL (operands[2]) <= <max_elem_index>);

  if (INTVAL (operands[2]) == 0)
    temp = operands[1];
  else
    {
      temp = gen_reg_rtx (<MODE>mode);
      emit_insn (gen_msa_sldi_<msafmt>_f (temp, operands[1], operands[1],
					  operands[2]));
    }
  emit_insn (gen_msa_cast_to_scalar_<msafmt>_f (operands[0], temp));
  DONE;
})

(define_expand "vec_set<mode>"
  [(match_operand:IMODE128 0 "register_operand")
   (match_operand:<ELEMMODE> 1 "register_operand")
   (match_operand 2 "const_int_operand")]
  "ISA_HAS_MSA"
{
  gcc_assert (INTVAL (operands[2]) <= <max_elem_index>);
  emit_insn (gen_msa_insert_<msafmt> (operands[0], operands[0], operands[2],
				      operands[1]));
  DONE;
})

(define_expand "vec_set<mode>"
  [(match_operand:FMODE128 0 "register_operand")
   (match_operand:<ELEMMODE> 1 "register_operand")
   (match_operand 2 "const_int_operand")]
  "ISA_HAS_MSA"
{
  int index = INTVAL (operands[2]);
  gcc_assert (index <= <max_elem_index>);
  emit_insn (gen_msa_insve_<msafmt>_f_s (operands[0], operands[0], operands[2],
					 operands[1]));
  DONE;
})

(define_expand "vcondu<MODE128_2:mode><IMODE128:mode>"
  [(set (match_operand:MODE128_2 0 "register_operand")
        (if_then_else:MODE128_2
          (match_operator 3 ""
            [(match_operand:IMODE128 4 "register_operand")
             (match_operand:IMODE128 5 "register_operand")])
          (match_operand:MODE128_2 1 "reg_or_m1_operand")
          (match_operand:MODE128_2 2 "reg_or_0_operand")))]
  "ISA_HAS_MSA
   && (GET_MODE_NUNITS (<MODE128_2:MODE>mode)
       == GET_MODE_NUNITS (<IMODE128:MODE>mode))"
{
  mips_expand_msa_vcond (operands[0], operands[1], operands[2],
			 GET_CODE (operands[3]), operands[4], operands[5]);
  DONE;
})

(define_expand "vcond<MODE128_2:mode><MODE128:mode>"
  [(set (match_operand:MODE128_2 0 "register_operand")
        (if_then_else:MODE128_2
          (match_operator 3 ""
            [(match_operand:MODE128 4 "register_operand")
             (match_operand:MODE128 5 "register_operand")])
          (match_operand:MODE128_2 1 "reg_or_m1_operand")
          (match_operand:MODE128_2 2 "reg_or_0_operand")))]
  "ISA_HAS_MSA
   && (GET_MODE_NUNITS (<MODE128_2:MODE>mode)
       == GET_MODE_NUNITS (<MODE128:MODE>mode))"
{
  mips_expand_msa_vcond (operands[0], operands[1], operands[2],
			 GET_CODE (operands[3]), operands[4], operands[5]);
  DONE;
})

(define_insn "msa_insert_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand 2 "const_uimm4_operand" "")
		       (match_operand:SI 3 "reg_or_0_operand" "dJ")]
		      UNSPEC_MSA_INSERT))]
  "ISA_HAS_MSA"
  "insert.b\t%w0[%2],%z3"
  [(set_attr "type"	"mtc")
   (set_attr "mode"	"TI")])

; Similar to msa_insert_b but with QImode for operand 3.
(define_insn "*msa_insert_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand 2 "const_uimm4_operand" "")
		       (match_operand:QI 3 "reg_or_0_operand" "dJ")]
		      UNSPEC_MSA_INSERT))]
  "ISA_HAS_MSA"
  "insert.b\t%w0[%2],%z3"
  [(set_attr "type"	"mtc")
   (set_attr "mode"	"TI")])

(define_insn "msa_insert_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "0")
		      (match_operand 2 "const_uimm3_operand" "")
		      (match_operand:SI 3 "reg_or_0_operand" "dJ")]
		     UNSPEC_MSA_INSERT))]
  "ISA_HAS_MSA"
  "insert.h\t%w0[%2],%z3"
  [(set_attr "type"	"mtc")
   (set_attr "mode"	"TI")])

; Similar to msa_insert_h but with HImode for operand 3.
(define_insn "*msa_insert_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "0")
		      (match_operand 2 "const_uimm3_operand" "")
		      (match_operand:HI 3 "reg_or_0_operand" "dJ")]
		     UNSPEC_MSA_INSERT))]
  "ISA_HAS_MSA"
  "insert.h\t%w0[%2],%z3"
  [(set_attr "type"	"mtc")
   (set_attr "mode"	"TI")])

(define_insn "msa_insert_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "0")
		      (match_operand 2 "const_0_to_3_operand" "")
		      (match_operand:SI 3 "reg_or_0_operand" "dJ")]
		     UNSPEC_MSA_INSERT))]
  "ISA_HAS_MSA"
  "insert.w\t%w0[%2],%z3"
  [(set_attr "type"	"mtc")
   (set_attr "mode"	"TI")])

(define_insn "msa_insert_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V4SF 1 "register_operand" "0")
		      (match_operand 2 "const_0_to_3_operand" "")
		      (match_operand:SF 3 "register_operand" "d")]
		     UNSPEC_MSA_INSERT))]
  "ISA_HAS_MSA"
  "insert.w\t%w0[%2],%3"
  [(set_attr "type"	"mtc")
   (set_attr "mode"	"TI")])

;; Note that insert.d will be splitted later if !TARGET_64BIT.
(define_insn "msa_insert_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "0")
		      (match_operand 2 "const_0_or_1_operand" "")
		      (match_operand:DI 3 "reg_or_0_operand" "dJ")]
		     UNSPEC_MSA_INSERT))]
  "ISA_HAS_MSA"
  "insert.d\t%w0[%2],%z3"
  [(set_attr "type"	"mtc")
   (set_attr "mode"	"TI")])

;; Note that insert.d will be splitted later if !TARGET_64BIT.
(define_insn "msa_insert_d_f"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V2DF 1 "register_operand" "0")
		      (match_operand 2 "const_0_or_1_operand" "")
		      (match_operand:DF 3 "register_operand" "d")]
		     UNSPEC_MSA_INSERT))]
  "ISA_HAS_MSA"
  "insert.d\t%w0[%2],%3"
  [(set_attr "type"	"mtc")
   (set_attr "mode"	"TI")])

(define_split
  [(set (match_operand:V2DI 0 "register_operand")
	(unspec: V2DI [(match_operand:V2DI 1 "register_operand")
		       (match_operand 2 "const_0_or_1_operand")
		       (match_operand:DI 3 "reg_or_0_operand" "")]
		      UNSPEC_MSA_INSERT))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_insert_d (operands[0], operands[1], operands[2], operands[3]);
  DONE;
})

(define_split
  [(set (match_operand:V2DF 0 "register_operand")
	(unspec: V2DF [(match_operand:V2DF 1 "register_operand")
		       (match_operand 2 "const_0_or_1_operand")
		       (match_operand:DF 3 "register_operand" "")]
		      UNSPEC_MSA_INSERT))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_insert_d (operands[0], operands[1], operands[2], operands[3]);
  DONE;
})

(define_insn "msa_insve_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand 2 "const_uimm4_operand" "")
		       (match_operand:V16QI 3 "register_operand" "f")]
		      UNSPEC_MSA_INSVE))]
  "ISA_HAS_MSA"
  "insve.b\t%w0[%2],%w3[0]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_insve_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "0")
		      (match_operand 2 "const_uimm3_operand" "")
		      (match_operand:V8HI 3 "register_operand" "f")]
		      UNSPEC_MSA_INSVE))]
  "ISA_HAS_MSA"
  "insve.h\t%w0[%2],%w3[0]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_insve_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "0")
		      (match_operand 2 "const_0_to_3_operand" "")
		      (match_operand:V4SI 3 "register_operand" "f")]
		      UNSPEC_MSA_INSVE))]
  "ISA_HAS_MSA"
  "insve.w\t%w0[%2],%w3[0]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_insve_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V4SF 1 "register_operand" "0")
		      (match_operand 2 "const_0_to_3_operand" "")
		      (match_operand:V4SF 3 "register_operand" "f")]
		      UNSPEC_MSA_INSVE))]
  "ISA_HAS_MSA"
  "insve.w\t%w0[%2],%w3[0]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

;; operand 3 is a scalar
(define_insn "msa_insve_w_f_s"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V4SF 1 "register_operand" "0")
		      (match_operand 2 "const_0_to_3_operand" "")
		      (match_operand:SF 3 "register_operand" "f")]
		      UNSPEC_MSA_INSVE))]
  "ISA_HAS_MSA"
  "insve.w\t%w0[%2],%w3[0]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_insve_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "0")
		      (match_operand 2 "const_0_or_1_operand" "")
		      (match_operand:V2DI 3 "register_operand" "f")]
		      UNSPEC_MSA_INSVE))]
  "ISA_HAS_MSA"
  "insve.d\t%w0[%2],%w3[0]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_insve_d_f"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V2DF 1 "register_operand" "0")
		      (match_operand 2 "const_0_or_1_operand" "")
		      (match_operand:V2DF 3 "register_operand" "f")]
		      UNSPEC_MSA_INSVE))]
  "ISA_HAS_MSA"
  "insve.d\t%w0[%2],%w3[0]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

;; operand 3 is a scalar
(define_insn "msa_insve_d_f_s"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V2DF 1 "register_operand" "0")
		      (match_operand 2 "const_0_or_1_operand" "")
		      (match_operand:DF 3 "register_operand" "f")]
		      UNSPEC_MSA_INSVE))]
  "ISA_HAS_MSA"
  "insve.d\t%w0[%2],%w3[0]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_copy_s_b"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V16QI 1 "register_operand" "f")
		    (match_operand 2 "const_uimm4_operand" "")]
		   UNSPEC_MSA_COPY_S))]
  "ISA_HAS_MSA"
  "copy_s.b\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

(define_insn "msa_copy_s_h"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V8HI 1 "register_operand" "f")
		    (match_operand 2 "const_uimm3_operand" "")]
		   UNSPEC_MSA_COPY_S))]
  "ISA_HAS_MSA"
  "copy_s.h\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

(define_insn "msa_copy_s_w"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V4SI 1 "register_operand" "f")
		    (match_operand 2 "const_0_to_3_operand" "")]
		   UNSPEC_MSA_COPY_S))]
  "ISA_HAS_MSA"
  "copy_s.w\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

(define_insn "msa_copy_s_w_f"
  [(set (match_operand:SF 0 "register_operand" "=d")
	(unspec:SF [(match_operand:V4SF 1 "register_operand" "f")
		    (match_operand 2 "const_0_to_3_operand" "")]
		   UNSPEC_MSA_COPY_S))]
  "ISA_HAS_MSA"
  "copy_s.w\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

;; Note that copy_s.d will be splitted later if !TARGET_64BIT.
(define_insn "msa_copy_s_d"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(unspec:DI [(match_operand:V2DI 1 "register_operand" "f")
		    (match_operand 2 "const_0_or_1_operand" "")]
		   UNSPEC_MSA_COPY_S))]
  "ISA_HAS_MSA"
  "copy_s.d\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

;; Note that copy_s.d will be splitted later if !TARGET_64BIT.
(define_insn "msa_copy_s_d_f"
  [(set (match_operand:DF 0 "register_operand" "=d")
	(unspec:DF [(match_operand:V2DF 1 "register_operand" "f")
		    (match_operand 2 "const_0_or_1_operand" "")]
		   UNSPEC_MSA_COPY_S))]
  "ISA_HAS_MSA"
  "copy_s.d\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

(define_split
  [(set (match_operand:DI 0 "register_operand")
	(unspec: DI [(match_operand:V2DI 1 "register_operand")
		     (match_operand 2 "const_0_or_1_operand")]
		    UNSPEC_MSA_COPY_S))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_copy_d (operands[0], operands[1], operands[2], false);
  DONE;
})

(define_split
  [(set (match_operand:DF 0 "register_operand")
	(unspec: DF [(match_operand:V2DF 1 "register_operand")
		     (match_operand 2 "const_0_or_1_operand")]
		    UNSPEC_MSA_COPY_S))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_copy_d (operands[0], operands[1], operands[2], false);
  DONE;
})

(define_insn "msa_copy_u_b"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V16QI 1 "register_operand" "f")
		    (match_operand 2 "const_uimm4_operand" "")]
		   UNSPEC_MSA_COPY_U))]
  "ISA_HAS_MSA"
  "copy_u.b\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

(define_insn "msa_copy_u_h"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V8HI 1 "register_operand" "f")
		    (match_operand 2 "const_uimm3_operand" "")]
		   UNSPEC_MSA_COPY_U))]
  "ISA_HAS_MSA"
  "copy_u.h\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

(define_insn "msa_copy_u_w"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V4SI 1 "register_operand" "f")
		    (match_operand 2 "const_0_to_3_operand" "")]
		   UNSPEC_MSA_COPY_U))]
  "ISA_HAS_MSA"
  "copy_u.w\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

(define_insn "msa_copy_u_w_f"
  [(set (match_operand:SF 0 "register_operand" "=d")
	(unspec:SF [(match_operand:V4SF 1 "register_operand" "f")
		    (match_operand 2 "const_0_to_3_operand" "")]
		   UNSPEC_MSA_COPY_U))]
  "ISA_HAS_MSA"
  "copy_u.w\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

;; Note that copy_u.d will be splitted later if !TARGET_64BIT.
(define_insn "msa_copy_u_d"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(unspec:DI [(match_operand:V2DI 1 "register_operand" "f")
		    (match_operand 2 "const_0_or_1_operand" "")]
		   UNSPEC_MSA_COPY_U))]
  "ISA_HAS_MSA"
  "copy_u.d\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

;; Note that copy_u.d will be splitted later if !TARGET_64BIT.
(define_insn "msa_copy_u_d_f"
  [(set (match_operand:DF 0 "register_operand" "=d")
	(unspec:DF [(match_operand:V2DF 1 "register_operand" "f")
		    (match_operand 2 "const_0_or_1_operand" "")]
		   UNSPEC_MSA_COPY_U))]
  "ISA_HAS_MSA"
  "copy_u.d\t%0,%w1[%2]"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"TI")])

(define_split
  [(set (match_operand:DI 0 "register_operand")
	(unspec: DI [(match_operand:V2DI 1 "register_operand")
		     (match_operand 2 "const_0_or_1_operand")]
		    UNSPEC_MSA_COPY_U))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_copy_d (operands[0], operands[1], operands[2], true);
  DONE;
})

(define_split
  [(set (match_operand:DF 0 "register_operand")
	(unspec: DF [(match_operand:V2DF 1 "register_operand")
		     (match_operand 2 "const_0_or_1_operand")]
		    UNSPEC_MSA_COPY_U))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_copy_d (operands[0], operands[1], operands[2], true);
  DONE;
})

(define_expand "vec_perm<mode>"
  [(match_operand:MODE128 0 "register_operand" "")
   (match_operand:MODE128 1 "register_operand" "")
   (match_operand:MODE128 2 "register_operand" "")
   (match_operand:<MODE128_I> 3 "register_operand" "")]
  "ISA_HAS_MSA"
{
  /* Note that GCC always uses memory order (as big-endian) in indexing,
     and layouts operands[1] frist and then operands[2] next.
     However, vshf starts indexes from wt to ws, so so we need to swap
     two operands.  MSA loads or stores elements to or from the rightmost
     position of vector registers, for both big-endian and little-endian CPUs.
     No need to change any index numbers.  */
  emit_insn (gen_msa_vshf<mode> (operands[0], operands[3], operands[2],
				 operands[1]));
  DONE;
})

(define_expand "neg<mode>2"
  [(match_operand:MODE128 0 "register_operand" "")
   (match_operand:MODE128 1 "register_operand" "")]
  "ISA_HAS_MSA"
{
  rtx reg = gen_reg_rtx (<MODE>mode);
  emit_insn (gen_msa_ldi<mode> (reg, GEN_INT (0)));
  emit_insn (gen_sub<mode>3 (operands[0], reg, operands[1]));
  DONE;
})

(define_insn "msa_ldi<mode>"
  [(set (match_operand:MODE128 0 "register_operand" "=f")
	(unspec:MODE128 [(match_operand 1 "const_imm10_operand" "")]
			UNSPEC_MSA_LDI))]
  "ISA_HAS_MSA"
  "ldi.<msafmt>\t%w0,%1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_lsa"
 [(set (match_operand:SI 0 "register_operand" "=f")
       (plus:SI (match_operand:SI 1 "register_operand" "f")
 		(ashiftrt:SI (match_operand:SI 2 "register_operand" "f")
			     (match_operand    3 "const_immlsa_operand" ""))))]
 "ISA_HAS_MSA"
 "lsa\t%0,%1,%2,(%3)-1"
 [(set_attr "type"      "arith")
  (set_attr "mode"      "SI")])

(define_insn "msa_vshf<mode>"
  [(set (match_operand:MODE128 0 "register_operand" "=f")
	(unspec:MODE128 [(match_operand:<MODE128_I> 1 "register_operand" "0")
			 (match_operand:MODE128 2 "register_operand" "f")
			 (match_operand:MODE128 3 "register_operand" "f")]
			UNSPEC_MSA_VSHF))]
  "ISA_HAS_MSA"
  "vshf.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

;; 128-bit integer/MSA vector registers moves
;; Note that we prefer floating-point loads, stores, and moves by adding * to
;; other register preferences.
;; Note that we combine f and J, so that move_type for J is fmove and its
;; instruction length can be 1.
(define_insn "movti_msa"
  [(set (match_operand:TI 0 "nonimmediate_operand" "=*d,*d,*d,*R,*d,*f,f,R,f")
        (match_operand:TI 1 "move_operand" "*d,*i,*R,*d*J,*f,*d,R,f,fJ"))]
  "ISA_HAS_MSA
   && !TARGET_64BIT
   && (register_operand (operands[0], TImode)
       || reg_or_0_operand (operands[1], TImode))"
  { return mips_output_move (operands[0], operands[1]); }
  [(set_attr "move_type"        "move,const,load,store,mfc,mtc,fpload,fpstore,fmove")
   (set_attr "mode"     "TI")])

;; Note that we prefer floating-point loads, stores, and moves by adding * to
;; other register preferences.
;; Note that we combine f and J, so that move_type for J is fmove and its
;; instruction length can be 1.
(define_insn "movti_msa_64bit"
  [(set (match_operand:TI 0 "nonimmediate_operand" "=*d,*d,*d,*R,*a,*d,*d,*f,f,R,f")
	(match_operand:TI 1 "move_operand" "*d,*i,*R,*d*J,*d*J,*a,*f,*d,R,f,fJ"))]
  "ISA_HAS_MSA
   && TARGET_64BIT
   && (register_operand (operands[0], TImode)
       || reg_or_0_operand (operands[1], TImode))"
  { return mips_output_move (operands[0], operands[1]); }
  [(set_attr "move_type" "move,const,load,store,mtlo,mflo,mfc,mtc,fpload,fpstore,fmove")
   (set_attr "mode" "TI")])

(define_expand "mov<mode>"
  [(set (match_operand:MODE128 0)
        (match_operand:MODE128 1))]
  "TARGET_64BIT || TARGET_MSA"
{
  if (mips_legitimize_move (<MODE>mode, operands[0], operands[1]))
    DONE;
})

;; Note that we prefer floating-point loads, stores, and moves by adding * to
;; other register preferences.
;; Note that we combine f and YG, so that move_type for YG is fmove and its
;; instruction length can be 1.
(define_insn "mov<mode>_msa"
  [(set (match_operand:MODE128 0 "nonimmediate_operand" "=f,f,R,R,*f,*d,*d,*d,*R")
        (match_operand:MODE128 1 "move_operand" "fYG,R,f,YG,*d,*f,*d*YG,*R,*d"))]
  "ISA_HAS_MSA
   && (register_operand (operands[0], <MODE>mode)
       || reg_or_0_operand (operands[1], <MODE>mode))"
{ return mips_output_move (operands[0], operands[1]); }
  [(set_attr "move_type"        "fmove,fpload,fpstore,store,mtc,mfc,move,load,store")
   (set_attr "mode"     "TI")])

(define_split
  [(set (match_operand:TI 0 "nonimmediate_operand")
        (match_operand:TI 1 "move_operand"))]
  "reload_completed && TARGET_MSA
   && mips_split_128bit_move_p (operands[0], operands[1])"
  [(const_int 0)]
{
  mips_split_128bit_move (operands[0], operands[1]);
  DONE;
})

(define_split
  [(set (match_operand:MODE128 0 "nonimmediate_operand")
        (match_operand:MODE128 1 "move_operand"))]
  "reload_completed && TARGET_MSA
   && mips_split_128bit_move_p (operands[0], operands[1])"
  [(const_int 0)]
{
  mips_split_128bit_move (operands[0], operands[1]);
  DONE;
})

;; Offset load
(define_expand "msa_ld_<msafmt>"
  [(match_operand:IMODE128 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand 2 "const_msa_addr_offset_operand")]
  "ISA_HAS_MSA"
{
  rtx addr = plus_constant (GET_MODE (operands[1]), operands[1], INTVAL (operands[2]));
  mips_emit_move (operands[0], gen_rtx_MEM (<MODE>mode, addr));
  DONE;
})

(define_expand "msa_ld_<msafmt>_f"
  [(match_operand:FMODE128 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand 2 "const_msa_addr_offset_operand")]
  "ISA_HAS_MSA"
{
  rtx addr = plus_constant (GET_MODE (operands[1]), operands[1], INTVAL (operands[2]));
  mips_emit_move (operands[0], gen_rtx_MEM (<MODE>mode, addr));
  DONE;
})

;; Indexed load
(define_expand "msa_ldx_<msafmt>"
  [(match_operand:IMODE128 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand:SI 2 "register_operand")]
  "ISA_HAS_MSA"
{
  operands[2] = convert_to_mode (Pmode, operands[2], false);
  emit_insn (PMODE_INSN (gen_msa_ldx<mode>,
			 (operands[0], operands[1], operands[2])));
  DONE;
})

(define_expand "msa_ldx_<msafmt>_f"
  [(match_operand:FMODE128 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand:SI 2 "register_operand")]
  "ISA_HAS_MSA"
{
  operands[2] = convert_to_mode (Pmode, operands[2], false);
  emit_insn (PMODE_INSN (gen_msa_ldx<mode>,
			 (operands[0], operands[1], operands[2])));
  DONE;
})

(define_insn "msa_ldx<mode>_<P:mode>"
  [(set (match_operand:MODE128 0 "register_operand" "=f")
	(mem:MODE128 (plus:P (match_operand:P 1 "register_operand" "d")
			     (match_operand:P 2 "register_operand" "d"))))]
  "ISA_HAS_MSA"
  "ldx.<msafmt>\t%w0,%2(%1)"
  [(set_attr "type"	"fpload")
   (set_attr "mode"	"TI")
   (set_attr "length"	"4")])

;; Offset store
(define_expand "msa_st_<msafmt>"
  [(match_operand:IMODE128 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand 2 "const_msa_addr_offset_operand")]
  "ISA_HAS_MSA"
{
  rtx addr = plus_constant (GET_MODE (operands[1]), operands[1], INTVAL (operands[2]));
  mips_emit_move (gen_rtx_MEM (<MODE>mode, addr), operands[0]);
  DONE;
})

(define_expand "msa_st_<msafmt>_f"
  [(match_operand:FMODE128 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand 2 "const_msa_addr_offset_operand")]
  "ISA_HAS_MSA"
{
  rtx addr = plus_constant (GET_MODE (operands[1]), operands[1], INTVAL (operands[2]));
  mips_emit_move (gen_rtx_MEM (<MODE>mode, addr), operands[0]);
  DONE;
})

;; Indexed store
(define_expand "msa_stx_<msafmt>"
  [(match_operand:IMODE128 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand:SI 2 "register_operand")]
  "ISA_HAS_MSA"
{
  operands[2] = convert_to_mode (Pmode, operands[2], false);
  emit_insn (PMODE_INSN (gen_msa_stx<mode>,
			 (operands[1], operands[2], operands[0])));
  DONE;
})

(define_expand "msa_stx_<msafmt>_f"
  [(match_operand:FMODE128 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand:SI 2 "register_operand")]
  "ISA_HAS_MSA"
{
  operands[2] = convert_to_mode (Pmode, operands[2], false);
  emit_insn (PMODE_INSN (gen_msa_stx<mode>,
			 (operands[1], operands[2], operands[0])));
  DONE;
})

(define_insn "msa_stx<mode>_<P:mode>"
  [(set (mem:MODE128 (plus:P (match_operand:P 0 "register_operand" "d")
			     (match_operand:P 1 "register_operand" "d")))
	(match_operand:MODE128 2 "register_operand" "f"))]
  "ISA_HAS_MSA"
  "stx.<msafmt>\t%w2,%1(%0)"
  [(set_attr "type"	"fpstore")
   (set_attr "mode"	"TI")
   (set_attr "length"	"4")])

;; Integer operations
(define_insn "add<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f,f")
	(plus:IMODE128 (match_operand:IMODE128 1 "register_operand" "f,f")
		       (match_operand:IMODE128 2 "reg_or_vector_uimm5_operand" "f,YC")))]
  "ISA_HAS_MSA"
  "@
   addv.<msafmt>\t%w0,%w1,%w2
   addvi.<msafmt>\t%w0,%w1,%D2"
  [(set_attr "alu_type"	"add")
   (set_attr "mode"	"TI")])

(define_insn "sub<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f,f")
	(minus:IMODE128 (match_operand:IMODE128 1 "register_operand" "f,f")
		        (match_operand:IMODE128 2 "reg_or_vector_uimm5_operand" "f,YC")))]
  "ISA_HAS_MSA"
  "@
   subv.<msafmt>\t%w0,%w1,%w2
   subvi.<msafmt>\t%w0,%w1,%D2"
  [(set_attr "alu_type"	"sub")
   (set_attr "mode"	"TI")])

(define_insn "mul<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(mult:IMODE128 (match_operand:IMODE128 1 "register_operand" "f")
		       (match_operand:IMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "mulv.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"imul3")
   (set_attr "mode"	"TI")])

(define_insn "msa_maddv_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(plus:IMODE128 (mult:IMODE128 (match_operand:IMODE128 2 "register_operand" "f")
				      (match_operand:IMODE128 3 "register_operand" "f"))
		       (match_operand:IMODE128 1 "register_operand" "0")))]
  "ISA_HAS_MSA"
  "maddv.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"     "imadd")
   (set_attr "mode"     "TI")])

(define_insn "msa_msubv_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(minus:IMODE128 (match_operand:IMODE128 1 "register_operand" "0")
		        (mult:IMODE128 (match_operand:IMODE128 2 "register_operand" "f")
				       (match_operand:IMODE128 3 "register_operand" "f"))))]
  "ISA_HAS_MSA"
  "msubv.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"     "imadd")
   (set_attr "mode"     "TI")])

(define_insn "div<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(div:IMODE128 (match_operand:IMODE128 1 "register_operand" "f")
		      (match_operand:IMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  { return mips_msa_output_division ("div_s.<msafmt>\t%w0,%w1,%w2", operands); }
  [(set_attr "type"	"idiv3")
   (set_attr "mode"	"TI")])

(define_insn "udiv<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(udiv:IMODE128 (match_operand:IMODE128 1 "register_operand" "f")
		       (match_operand:IMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  { return mips_msa_output_division ("div_u.<msafmt>\t%w0,%w1,%w2", operands); }
  [(set_attr "type"	"idiv3")
   (set_attr "mode"	"TI")])

(define_insn "mod<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(mod:IMODE128 (match_operand:IMODE128 1 "register_operand" "f")
		      (match_operand:IMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  { return mips_msa_output_division ("mod_s.<msafmt>\t%w0,%w1,%w2", operands); }
  [(set_attr "type"	"idiv3")
   (set_attr "mode"	"TI")])

(define_insn "umod<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(umod:IMODE128 (match_operand:IMODE128 1 "register_operand" "f")
		       (match_operand:IMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  { return mips_msa_output_division ("mod_u.<msafmt>\t%w0,%w1,%w2", operands); }
  [(set_attr "type"	"idiv3")
   (set_attr "mode"	"TI")])

(define_insn "xor<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(xor:IMODE128 (match_operand:IMODE128 1 "register_operand" "f")
		      (match_operand:IMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "xor.v\t%w0,%w1,%w2"
  [(set_attr "alu_type"	"xor")
   (set_attr "mode"	"TI")])

(define_insn "ior<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(ior:IMODE128 (match_operand:IMODE128 1 "register_operand" "f")
		      (match_operand:IMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "or.v\t%w0,%w1,%w2"
  [(set_attr "alu_type"	"or")
   (set_attr "mode"	"TI")])

(define_insn "and<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(and:IMODE128 (match_operand:IMODE128 1 "register_operand" "f")
		      (match_operand:IMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "and.v\t%w0,%w1,%w2"
  [(set_attr "alu_type"	"and")
   (set_attr "mode"	"TI")])

(define_expand "one_cmpl<mode>2"
  [(match_operand:IMODE128 0 "register_operand" "")
   (match_operand:IMODE128 1 "register_operand" "")]
  "ISA_HAS_MSA"
{
  rtx reg = gen_reg_rtx (<MODE>mode);
  emit_insn (gen_msa_ldi<mode> (reg, GEN_INT (0)));
  emit_insn (gen_msa_nor_v_<msafmt> (operands[0], reg, operands[1]));
  DONE;
})

(define_insn "vlshr<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f,f")
	(lshiftrt:IMODE128 (match_operand:IMODE128 1 "register_operand" "f,f")
			   (match_operand:IMODE128 2 "reg_or_vector_uimm6_operand" "f,YC")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 0)
    return "srl.<msafmt>\t%w0,%w1,%w2";

  operands[2] = GEN_INT (INTVAL (CONST_VECTOR_ELT (operands[2], 0))
			 & <shift_mask>);
  return "srli.<msafmt>\t%w0,%w1,%2";
}
  [(set_attr "type"	"shift")
   (set_attr "mode"	"TI")])

(define_insn "vashr<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f,f")
	(ashiftrt:IMODE128 (match_operand:IMODE128 1 "register_operand" "f,f")
			   (match_operand:IMODE128 2 "reg_or_vector_uimm6_operand" "f,YC")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 0)
    return "sra.<msafmt>\t%w0,%w1,%w2";

  operands[2] = GEN_INT (INTVAL (CONST_VECTOR_ELT (operands[2], 0))
			 & <shift_mask>);
  return "srai.<msafmt>\t%w0,%w1,%2";
}
  [(set_attr "type"	"shift")
   (set_attr "mode"	"TI")])

(define_insn "vashl<mode>3"
  [(set (match_operand:IMODE128 0 "register_operand" "=f,f")
	(ashift:IMODE128 (match_operand:IMODE128 1 "register_operand" "f,f")
			 (match_operand:IMODE128 2 "reg_or_vector_uimm6_operand" "f,YC")))]
  "ISA_HAS_MSA"
{
  if (which_alternative == 0)
    return "sll.<msafmt>\t%w0,%w1,%w2";

  operands[2] = GEN_INT (INTVAL (CONST_VECTOR_ELT (operands[2], 0))
			 & <shift_mask>);
  return "slli.<msafmt>\t%w0,%w1,%2";
}
  [(set_attr "type"	"shift")
   (set_attr "mode"	"TI")])

;; Floating-point operations
(define_insn "add<mode>3"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(plus:FMODE128 (match_operand:FMODE128 1 "register_operand" "f")
		       (match_operand:FMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fadd.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fadd")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "sub<mode>3"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(minus:FMODE128 (match_operand:FMODE128 1 "register_operand" "f")
		        (match_operand:FMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fsub.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fadd")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "mul<mode>3"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(mult:FMODE128 (match_operand:FMODE128 1 "register_operand" "f")
		       (match_operand:FMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fmul.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fmul")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "div<mode>3"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(div:FMODE128 (match_operand:FMODE128 1 "register_operand" "f")
		      (match_operand:FMODE128 2 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fdiv.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fdiv")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fmadd_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(plus:FMODE128 (mult:FMODE128 (match_operand:FMODE128 2 "register_operand" "f")
				      (match_operand:FMODE128 3 "register_operand" "f"))
		       (match_operand:FMODE128 1 "register_operand" "0")))]
  "ISA_HAS_MSA"
  "fmadd.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"     "fmadd")
   (set_attr "mode"     "<UNITMODE>")])

(define_insn "msa_fmsub_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(minus:FMODE128 (match_operand:FMODE128 1 "register_operand" "0")
		        (mult:FMODE128 (match_operand:FMODE128 2 "register_operand" "f")
				       (match_operand:FMODE128 3 "register_operand" "f"))))]
  "ISA_HAS_MSA"
  "fmsub.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"     "fmadd")
   (set_attr "mode"     "<UNITMODE>")])

(define_insn "sqrt<mode>2"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(sqrt:FMODE128 (match_operand:FMODE128 1 "register_operand" "f")))]
  "ISA_HAS_MSA"
  "fsqrt.<msafmt>\t%w0,%w1"
  [(set_attr "type"	"fsqrt")
   (set_attr "mode"	"<UNITMODE>")])

;; Built-in functions
(define_insn "msa_add_a_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_ADD_A))]
  "ISA_HAS_MSA"
  "add_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_adds_a_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_ADDS_A))]
  "ISA_HAS_MSA"
  "adds_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_adds_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_ADDS_S))]
  "ISA_HAS_MSA"
  "adds_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_adds_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_ADDS_U))]
  "ISA_HAS_MSA"
  "adds_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_addvi_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_uimm5_operand" "")]
			 UNSPEC_MSA_ADDVI))]
  "ISA_HAS_MSA"
  "addvi.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_andi_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm8_operand" "")]
		      UNSPEC_MSA_ANDI_B))]
  "ISA_HAS_MSA"
  "andi.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_asub_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_ASUB_S))]
  "ISA_HAS_MSA"
  "asub_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_asub_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_ASUB_U))]
  "ISA_HAS_MSA"
  "asub_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_ave_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_AVE_S))]
  "ISA_HAS_MSA"
  "ave_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_ave_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_AVE_U))]
  "ISA_HAS_MSA"
  "ave_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_aver_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_AVER_S))]
  "ISA_HAS_MSA"
  "aver_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_aver_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_AVER_U))]
  "ISA_HAS_MSA"
  "aver_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bclr_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_BCLR))]
  "ISA_HAS_MSA"
  "bclr.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bclri_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm3_operand" "")]
		      UNSPEC_MSA_BCLRI))]
  "ISA_HAS_MSA"
  "bclri.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bclri_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm4_operand" "")]
		     UNSPEC_MSA_BCLRI))]
  "ISA_HAS_MSA"
  "bclri.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bclri_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_BCLRI))]
  "ISA_HAS_MSA"
  "bclri.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bclri_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm6_operand" "")]
		     UNSPEC_MSA_BCLRI))]
  "ISA_HAS_MSA"
  "bclri.d\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_binsl_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "0")
			  (match_operand:IMODE128 2 "register_operand" "f")
			  (match_operand:IMODE128 3 "register_operand" "f")]
			 UNSPEC_MSA_BINSL))]
  "ISA_HAS_MSA"
  "binsl.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_binsli_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand:V16QI 2 "register_operand" "f")
		       (match_operand 3 "const_uimm3_operand" "")]
		      UNSPEC_MSA_BINSLI))]
  "ISA_HAS_MSA"
  "binsli.b\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_binsli_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "0")
		      (match_operand:V8HI 2 "register_operand" "f")
		      (match_operand 3 "const_uimm4_operand" "")]
		     UNSPEC_MSA_BINSLI))]
  "ISA_HAS_MSA"
  "binsli.h\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_binsli_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "0")
		      (match_operand:V4SI 2 "register_operand" "f")
		      (match_operand 3 "const_uimm5_operand" "")]
		     UNSPEC_MSA_BINSLI))]
  "ISA_HAS_MSA"
  "binsli.w\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_binsli_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "0")
		      (match_operand:V2DI 2 "register_operand" "f")
		      (match_operand 3 "const_uimm6_operand" "")]
		     UNSPEC_MSA_BINSLI))]
  "ISA_HAS_MSA"
  "binsli.d\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_binsr_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "0")
			  (match_operand:IMODE128 2 "register_operand" "f")
			  (match_operand:IMODE128 3 "register_operand" "f")]
			 UNSPEC_MSA_BINSR))]
  "ISA_HAS_MSA"
  "binsr.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_binsri_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand:V16QI 2 "register_operand" "f")
		       (match_operand 3 "const_uimm3_operand" "")]
		      UNSPEC_MSA_BINSRI))]
  "ISA_HAS_MSA"
  "binsri.b\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_binsri_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "0")
		      (match_operand:V8HI 2 "register_operand" "f")
		      (match_operand 3 "const_uimm4_operand" "")]
		     UNSPEC_MSA_BINSRI))]
  "ISA_HAS_MSA"
  "binsri.h\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_binsri_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "0")
		      (match_operand:V4SI 2 "register_operand" "f")
		      (match_operand 3 "const_uimm5_operand" "")]
		     UNSPEC_MSA_BINSRI))]
  "ISA_HAS_MSA"
  "binsri.w\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_binsri_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "0")
		      (match_operand:V2DI 2 "register_operand" "f")
		      (match_operand 3 "const_uimm6_operand" "")]
		     UNSPEC_MSA_BINSRI))]
  "ISA_HAS_MSA"
  "binsri.d\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bmnz_v_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "0")
			  (match_operand:IMODE128 2 "register_operand" "f")
			  (match_operand:IMODE128 3 "register_operand" "f")]
			 UNSPEC_MSA_BMNZ_V))]
  "ISA_HAS_MSA"
  "bmnz.v\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bmnzi_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand:V16QI 2 "register_operand" "f")
		       (match_operand 3 "const_uimm8_operand" "")]
		      UNSPEC_MSA_BMNZI_B))]
  "ISA_HAS_MSA"
  "bmnzi.b\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bmz_v_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "0")
		          (match_operand:IMODE128 2 "register_operand" "f")
			  (match_operand:IMODE128 3 "register_operand" "f")]
			 UNSPEC_MSA_BMZ_V))]
  "ISA_HAS_MSA"
  "bmz.v\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bmzi_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand:V16QI 2 "register_operand" "f")
		       (match_operand 3 "const_uimm8_operand" "")]
		      UNSPEC_MSA_BMZI_B))]
  "ISA_HAS_MSA"
  "bmzi.b\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bneg_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_BNEG))]
  "ISA_HAS_MSA"
  "bneg.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bnegi_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_msa_branch_operand" "")]
		      UNSPEC_MSA_BNEGI))]
  "ISA_HAS_MSA"
  "bnegi.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bnegi_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_msa_branch_operand" "")]
		     UNSPEC_MSA_BNEGI))]
  "ISA_HAS_MSA"
  "bnegi.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bnegi_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_msa_branch_operand" "")]
		     UNSPEC_MSA_BNEGI))]
  "ISA_HAS_MSA"
  "bnegi.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bnegi_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_msa_branch_operand" "")]
		     UNSPEC_MSA_BNEGI))]
  "ISA_HAS_MSA"
  "bnegi.d\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bsel_v_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "0")
			  (match_operand:IMODE128 2 "register_operand" "f")
			  (match_operand:IMODE128 3 "register_operand" "f")]
			 UNSPEC_MSA_BSEL_V))]
  "ISA_HAS_MSA"
  "bsel.v\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bseli_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand:V16QI 2 "register_operand" "f")
		       (match_operand 3 "const_uimm8_operand" "")]
		      UNSPEC_MSA_BSELI_B))]
  "ISA_HAS_MSA"
  "bseli.b\t%w0,%w2,%3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bset_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_BSET))]
  "ISA_HAS_MSA"
  "bset.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bseti_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm3_operand" "")]
		      UNSPEC_MSA_BSETI))]
  "ISA_HAS_MSA"
  "bseti.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bseti_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm4_operand" "")]
		     UNSPEC_MSA_BSETI))]
  "ISA_HAS_MSA"
  "bseti.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bseti_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_BSETI))]
  "ISA_HAS_MSA"
  "bseti.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bseti_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm6_operand" "")]
		     UNSPEC_MSA_BSETI))]
  "ISA_HAS_MSA"
  "bseti.d\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_ceq_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_CEQ))]
  "ISA_HAS_MSA"
  "ceq.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_ceqi_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_imm5_operand" "")]
			 UNSPEC_MSA_CEQI))]
  "ISA_HAS_MSA"
  "ceqi.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_cle_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_CLE_S))]
  "ISA_HAS_MSA"
  "cle_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_cle_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_CLE_U))]
  "ISA_HAS_MSA"
  "cle_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_clei_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_imm5_operand" "")]
			 UNSPEC_MSA_CLEI_S))]
  "ISA_HAS_MSA"
  "clei_s.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_clei_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_uimm5_operand" "")]
			 UNSPEC_MSA_CLEI_U))]
  "ISA_HAS_MSA"
  "clei_u.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_clt_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_CLT_S))]
  "ISA_HAS_MSA"
  "clt_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_clt_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_CLT_U))]
  "ISA_HAS_MSA"
  "clt_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_clti_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_imm5_operand" "")]
			 UNSPEC_MSA_CLTI_S))]
  "ISA_HAS_MSA"
  "clti_s.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_clti_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_uimm5_operand" "")]
			 UNSPEC_MSA_CLTI_U))]
  "ISA_HAS_MSA"
  "clti_u.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dotp_s_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V16QI 1 "register_operand" "f")
		      (match_operand:V16QI 2 "register_operand" "f")]
		     UNSPEC_MSA_DOTP_S))]
  "ISA_HAS_MSA"
  "dotp_s.h\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dotp_s_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand:V8HI 2 "register_operand" "f")]
		     UNSPEC_MSA_DOTP_S))]
  "ISA_HAS_MSA"
  "dotp_s.w\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dotp_s_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand:V4SI 2 "register_operand" "f")]
		     UNSPEC_MSA_DOTP_S))]
  "ISA_HAS_MSA"
  "dotp_s.d\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dotp_u_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V16QI 1 "register_operand" "f")
		      (match_operand:V16QI 2 "register_operand" "f")]
		     UNSPEC_MSA_DOTP_U))]
  "ISA_HAS_MSA"
  "dotp_u.h\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dotp_u_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand:V8HI 2 "register_operand" "f")]
		     UNSPEC_MSA_DOTP_U))]
  "ISA_HAS_MSA"
  "dotp_u.w\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dotp_u_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand:V4SI 2 "register_operand" "f")]
		     UNSPEC_MSA_DOTP_U))]
  "ISA_HAS_MSA"
  "dotp_u.d\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpadd_s_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "0")
		      (match_operand:V16QI 2 "register_operand" "f")
		      (match_operand:V16QI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPADD_S))]
  "ISA_HAS_MSA"
  "dpadd_s.h\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpadd_s_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "0")
		      (match_operand:V8HI 2 "register_operand" "f")
		      (match_operand:V8HI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPADD_S))]
  "ISA_HAS_MSA"
  "dpadd_s.w\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpadd_s_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "0")
		      (match_operand:V4SI 2 "register_operand" "f")
		      (match_operand:V4SI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPADD_S))]
  "ISA_HAS_MSA"
  "dpadd_s.d\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpadd_u_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "0")
		      (match_operand:V16QI 2 "register_operand" "f")
		      (match_operand:V16QI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPADD_U))]
  "ISA_HAS_MSA"
  "dpadd_u.h\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpadd_u_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "0")
		      (match_operand:V8HI 2 "register_operand" "f")
		      (match_operand:V8HI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPADD_U))]
  "ISA_HAS_MSA"
  "dpadd_u.w\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpadd_u_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "0")
		      (match_operand:V4SI 2 "register_operand" "f")
		      (match_operand:V4SI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPADD_U))]
  "ISA_HAS_MSA"
  "dpadd_u.d\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpsub_s_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "0")
		      (match_operand:V16QI 2 "register_operand" "f")
		      (match_operand:V16QI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPSUB_S))]
  "ISA_HAS_MSA"
  "dpsub_s.h\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpsub_s_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "0")
		      (match_operand:V8HI 2 "register_operand" "f")
		      (match_operand:V8HI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPSUB_S))]
  "ISA_HAS_MSA"
  "dpsub_s.w\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpsub_s_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "0")
		      (match_operand:V4SI 2 "register_operand" "f")
		      (match_operand:V4SI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPSUB_S))]
  "ISA_HAS_MSA"
  "dpsub_s.d\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpsub_u_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "0")
		      (match_operand:V16QI 2 "register_operand" "f")
		      (match_operand:V16QI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPSUB_U))]
  "ISA_HAS_MSA"
  "dpsub_u.h\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpsub_u_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "0")
		      (match_operand:V8HI 2 "register_operand" "f")
		      (match_operand:V8HI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPSUB_U))]
  "ISA_HAS_MSA"
  "dpsub_u.w\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_dpsub_u_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "0")
		      (match_operand:V4SI 2 "register_operand" "f")
		      (match_operand:V4SI 3 "register_operand" "f")]
		     UNSPEC_MSA_DPSUB_U))]
  "ISA_HAS_MSA"
  "dpsub_u.d\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_fclass_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")]
			    UNSPEC_MSA_FCLASS))]
  "ISA_HAS_MSA"
  "fclass.<msafmt>\t%w0,%w1"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fcaf_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCAF))]
  "ISA_HAS_MSA"
  "fcaf.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fcor_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCOR))]
  "ISA_HAS_MSA"
  "fcor.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fcun_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCUN))]
  "ISA_HAS_MSA"
  "fcun.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fcune_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCUNE))]
  "ISA_HAS_MSA"
  "fcune.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fcueq_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCUEQ))]
  "ISA_HAS_MSA"
  "fcueq.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fceq_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCEQ))]
  "ISA_HAS_MSA"
  "fceq.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fcne_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCNE))]
  "ISA_HAS_MSA"
  "fcne.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fclt_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCLT))]
  "ISA_HAS_MSA"
  "fclt.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fcult_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCULT))]
  "ISA_HAS_MSA"
  "fcult.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fcle_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCLE))]
  "ISA_HAS_MSA"
  "fcle.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fcule_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FCULE))]
  "ISA_HAS_MSA"
  "fcule.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fsaf_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSAF))]
  "ISA_HAS_MSA"
  "fsaf.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fsor_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSOR))]
  "ISA_HAS_MSA"
  "fsor.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fsun_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSUN))]
  "ISA_HAS_MSA"
  "fsun.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fsune_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSUNE))]
  "ISA_HAS_MSA"
  "fsune.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fsueq_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSUEQ))]
  "ISA_HAS_MSA"
  "fsueq.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fseq_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSEQ))]
  "ISA_HAS_MSA"
  "fseq.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fsne_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSNE))]
  "ISA_HAS_MSA"
  "fsne.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fslt_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSLT))]
  "ISA_HAS_MSA"
  "fslt.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fsult_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSULT))]
  "ISA_HAS_MSA"
  "fsult.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fsle_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSLE))]
  "ISA_HAS_MSA"
  "fsle.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fsule_<msafmt>"
  [(set (match_operand:<MODE128_I> 0 "register_operand" "=f")
	(unspec:<MODE128_I> [(match_operand:FMODE128 1 "register_operand" "f")
			     (match_operand:FMODE128 2 "register_operand" "f")]
			    UNSPEC_MSA_FSULE))]
  "ISA_HAS_MSA"
  "fsule.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fcmp")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fexp2_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "f")
			  (match_operand:<MODE128_I> 2 "register_operand" "f")]
			 UNSPEC_MSA_FEXP2))]
  "ISA_HAS_MSA"
  "fexp2.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fmul")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_ffint_s_w"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V4SI 1 "register_operand" "f")]
		     UNSPEC_MSA_FFINT_S))]
  "ISA_HAS_MSA"
  "ffint_s.w\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"I2S")
   (set_attr "mode"	"SF")])

(define_insn "msa_ffint_s_d"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V2DI 1 "register_operand" "f")]
		     UNSPEC_MSA_FFINT_S))]
  "ISA_HAS_MSA"
  "ffint_s.d\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"I2D")
   (set_attr "mode"	"DF")])

(define_insn "msa_ffint_u_w"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V4SI 1 "register_operand" "f")]
		     UNSPEC_MSA_FFINT_U))]
  "ISA_HAS_MSA"
  "ffint_u.w\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"I2S")
   (set_attr "mode"	"SF")])

(define_insn "msa_ffint_u_d"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V2DI 1 "register_operand" "f")]
		     UNSPEC_MSA_FFINT_U))]
  "ISA_HAS_MSA"
  "ffint_u.d\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"I2D")
   (set_attr "mode"	"DF")])

(define_insn "msa_ffql_w"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V8HI 1 "register_operand" "f")]
		     UNSPEC_MSA_FFQL))]
  "ISA_HAS_MSA"
  "ffql.w\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"I2S")
   (set_attr "mode"	"SF")])

(define_insn "msa_ffql_d"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V4SI 1 "register_operand" "f")]
		     UNSPEC_MSA_FFQL))]
  "ISA_HAS_MSA"
  "ffql.d\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"I2D")
   (set_attr "mode"	"DF")])

(define_insn "msa_ffqr_w"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V8HI 1 "register_operand" "f")]
		     UNSPEC_MSA_FFQR))]
  "ISA_HAS_MSA"
  "ffqr.w\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"I2S")
   (set_attr "mode"	"SF")])

(define_insn "msa_ffqr_d"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V4SI 1 "register_operand" "f")]
		     UNSPEC_MSA_FFQR))]
  "ISA_HAS_MSA"
  "ffqr.d\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"I2D")
   (set_attr "mode"	"DF")])

(define_insn "msa_fill_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:SI 1 "reg_or_0_operand" "dJ")]
		      UNSPEC_MSA_FILL))]
  "ISA_HAS_MSA"
  "fill.b\t%w0,%z1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_fill_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:SI 1 "reg_or_0_operand" "dJ")]
		     UNSPEC_MSA_FILL))]
  "ISA_HAS_MSA"
  "fill.h\t%w0,%z1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_fill_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:SI 1 "reg_or_0_operand" "dJ")]
		     UNSPEC_MSA_FILL))]
  "ISA_HAS_MSA"
  "fill.w\t%w0,%z1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_fill_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:SF 1 "register_operand" "d")]
		     UNSPEC_MSA_FILL))]
  "ISA_HAS_MSA"
  "fill.w\t%w0,%1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

;; Note that fill.d will be splitted later if !TARGET_64BIT.
(define_insn "msa_fill_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:DI 1 "reg_or_0_operand" "dJ")]
		     UNSPEC_MSA_FILL))]
  "ISA_HAS_MSA"
  "fill.d\t%w0,%z1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

;; Note that fill.d will be splitted later if !TARGET_64BIT.
(define_insn "msa_fill_d_f"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:DF 1 "register_operand" "d")]
		     UNSPEC_MSA_FILL))]
  "ISA_HAS_MSA"
  "fill.d\t%w0,%1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_split
  [(set (match_operand:V2DI 0 "register_operand")
	(unspec: V2DI [(match_operand:DI 1 "reg_or_0_operand")]
		      UNSPEC_MSA_FILL))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_fill_d (operands[0], operands[1]);
  DONE;
})

(define_split
  [(set (match_operand:V2DF 0 "register_operand")
	(unspec: V2DF [(match_operand:DF 1 "register_operand")]
		      UNSPEC_MSA_FILL))]
  "reload_completed && TARGET_MSA && !TARGET_64BIT"
  [(const_int 0)]
{
  mips_split_msa_fill_d (operands[0], operands[1]);
  DONE;
})

(define_insn "msa_flog2_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "f")]
			 UNSPEC_MSA_FLOG2))]
  "ISA_HAS_MSA"
  "flog2.<msafmt>\t%w0,%w1"
  [(set_attr "type"	"fmul")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fmax_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "f")
			  (match_operand:FMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_FMAX))]
  "ISA_HAS_MSA"
  "fmax.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fadd")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fmax_a_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "f")
			  (match_operand:FMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_FMAX_A))]
  "ISA_HAS_MSA"
  "fmax_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fadd")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fmin_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "f")
			  (match_operand:FMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_FMIN))]
  "ISA_HAS_MSA"
  "fmin.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fadd")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_fmin_a_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "f")
			  (match_operand:FMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_FMIN_A))]
  "ISA_HAS_MSA"
  "fmin_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"fadd")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_frcp_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "f")]
			 UNSPEC_MSA_FRCP))]
  "ISA_HAS_MSA"
  "frcp.<msafmt>\t%w0,%w1"
  [(set_attr "type"	"frdiv")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_frint_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "f")]
			 UNSPEC_MSA_FRINT))]
  "ISA_HAS_MSA"
  "frint.<msafmt>\t%w0,%w1"
  [(set_attr "type"	"fmul")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_frsqrt_<msafmt>"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "f")]
			 UNSPEC_MSA_FRSQRT))]
  "ISA_HAS_MSA"
  "frsqrt.<msafmt>\t%w0,%w1"
  [(set_attr "type"	"frsqrt")
   (set_attr "mode"	"<UNITMODE>")])

(define_insn "msa_ftint_s_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SF 1 "register_operand" "f")]
		     UNSPEC_MSA_FTINT_S))]
  "ISA_HAS_MSA"
  "ftint_s.w\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"S2I")
   (set_attr "mode"	"SF")])

(define_insn "msa_ftint_s_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DF 1 "register_operand" "f")]
		     UNSPEC_MSA_FTINT_S))]
  "ISA_HAS_MSA"
  "ftint_s.d\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"D2I")
   (set_attr "mode"	"DF")])

(define_insn "msa_ftint_u_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SF 1 "register_operand" "f")]
		     UNSPEC_MSA_FTINT_U))]
  "ISA_HAS_MSA"
  "ftint_u.w\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"S2I")
   (set_attr "mode"	"SF")])

(define_insn "msa_ftint_u_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DF 1 "register_operand" "f")]
		     UNSPEC_MSA_FTINT_U))]
  "ISA_HAS_MSA"
  "ftint_u.d\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"D2I")
   (set_attr "mode"	"DF")])

(define_insn "msa_ftrunc_s_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SF 1 "register_operand" "f")]
		     UNSPEC_MSA_FTRUNC_S))]
  "ISA_HAS_MSA"
  "ftrunc_s.w\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"S2I")
   (set_attr "mode"	"SF")])

(define_insn "msa_ftrunc_s_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DF 1 "register_operand" "f")]
		     UNSPEC_MSA_FTRUNC_S))]
  "ISA_HAS_MSA"
  "ftrunc_s.d\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"D2I")
   (set_attr "mode"	"DF")])

(define_insn "msa_ftrunc_u_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SF 1 "register_operand" "f")]
		     UNSPEC_MSA_FTRUNC_U))]
  "ISA_HAS_MSA"
  "ftrunc_u.w\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"S2I")
   (set_attr "mode"	"SF")])

(define_insn "msa_ftrunc_u_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DF 1 "register_operand" "f")]
		     UNSPEC_MSA_FTRUNC_U))]
  "ISA_HAS_MSA"
  "ftrunc_u.d\t%w0,%w1"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"D2I")
   (set_attr "mode"	"DF")])

(define_insn "msa_ftq_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V4SF 1 "register_operand" "f")
		      (match_operand:V4SF 2 "register_operand" "f")]
		     UNSPEC_MSA_FTQ))]
  "ISA_HAS_MSA"
  "ftq.h\t%w0,%w1,%w2"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"S2I")
   (set_attr "mode"	"SF")])

(define_insn "msa_ftq_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V2DF 1 "register_operand" "f")
		      (match_operand:V2DF 2 "register_operand" "f")]
		     UNSPEC_MSA_FTQ))]
  "ISA_HAS_MSA"
  "ftq.w\t%w0,%w1,%w2"
  [(set_attr "type"	"fcvt")
   (set_attr "cnv_mode"	"D2I")
   (set_attr "mode"	"DF")])

(define_insn "msa_hadd_s_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V16QI 1 "register_operand" "f")
		      (match_operand:V16QI 2 "register_operand" "f")]
		     UNSPEC_MSA_HADD_S))]
  "ISA_HAS_MSA"
  "hadd_s.h\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hadd_s_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand:V8HI 2 "register_operand" "f")]
		     UNSPEC_MSA_HADD_S))]
  "ISA_HAS_MSA"
  "hadd_s.w\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hadd_s_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand:V4SI 2 "register_operand" "f")]
		     UNSPEC_MSA_HADD_S))]
  "ISA_HAS_MSA"
  "hadd_s.d\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hadd_u_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V16QI 1 "register_operand" "f")
		      (match_operand:V16QI 2 "register_operand" "f")]
		     UNSPEC_MSA_HADD_U))]
  "ISA_HAS_MSA"
  "hadd_u.h\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hadd_u_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand:V8HI 2 "register_operand" "f")]
		     UNSPEC_MSA_HADD_U))]
  "ISA_HAS_MSA"
  "hadd_u.w\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hadd_u_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand:V4SI 2 "register_operand" "f")]
		     UNSPEC_MSA_HADD_U))]
  "ISA_HAS_MSA"
  "hadd_u.d\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hsub_s_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V16QI 1 "register_operand" "f")
		      (match_operand:V16QI 2 "register_operand" "f")]
		     UNSPEC_MSA_HSUB_S))]
  "ISA_HAS_MSA"
  "hsub_s.h\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hsub_s_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand:V8HI 2 "register_operand" "f")]
		     UNSPEC_MSA_HSUB_S))]
  "ISA_HAS_MSA"
  "hsub_s.w\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hsub_s_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand:V4SI 2 "register_operand" "f")]
		     UNSPEC_MSA_HSUB_S))]
  "ISA_HAS_MSA"
  "hsub_s.d\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hsub_u_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V16QI 1 "register_operand" "f")
		      (match_operand:V16QI 2 "register_operand" "f")]
		     UNSPEC_MSA_HSUB_U))]
  "ISA_HAS_MSA"
  "hsub_u.h\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hsub_u_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand:V8HI 2 "register_operand" "f")]
		     UNSPEC_MSA_HSUB_U))]
  "ISA_HAS_MSA"
  "hsub_u.w\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_hsub_u_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand:V4SI 2 "register_operand" "f")]
		     UNSPEC_MSA_HSUB_U))]
  "ISA_HAS_MSA"
  "hsub_u.d\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_ilvev_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_ILVEV))]
  "ISA_HAS_MSA"
  "ilvev.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "alu_type"	"add")
   (set_attr "mode"	"TI")])

(define_insn "msa_ilvl_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_ILVL))]
  "ISA_HAS_MSA"
  "ilvl.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "alu_type"	"add")
   (set_attr "mode"	"TI")])

(define_insn "msa_ilvod_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_ILVOD))]
  "ISA_HAS_MSA"
  "ilvod.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "alu_type"	"add")
   (set_attr "mode"	"TI")])

(define_insn "msa_ilvr_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_ILVR))]
  "ISA_HAS_MSA"
  "ilvr.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "alu_type"	"add")
   (set_attr "mode"	"TI")])

(define_insn "msa_madd_q_<msafmt>"
  [(set (match_operand:QMODE128 0 "register_operand" "=f")
	(unspec:QMODE128 [(match_operand:QMODE128 1 "register_operand" "0")
			  (match_operand:QMODE128 2 "register_operand" "f")
			  (match_operand:QMODE128 3 "register_operand" "f")]
			 UNSPEC_MSA_MADD_Q))]
  "ISA_HAS_MSA"
  "madd_q.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_maddr_q_<msafmt>"
  [(set (match_operand:QMODE128 0 "register_operand" "=f")
	(unspec:QMODE128 [(match_operand:QMODE128 1 "register_operand" "0")
			  (match_operand:QMODE128 2 "register_operand" "f")
			  (match_operand:QMODE128 3 "register_operand" "f")]
			 UNSPEC_MSA_MADDR_Q))]
  "ISA_HAS_MSA"
  "maddr_q.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_max_a_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_MAX_A))]
  "ISA_HAS_MSA"
  "max_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_max_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_MAX_S))]
  "ISA_HAS_MSA"
  "max_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_max_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_MAX_U))]
  "ISA_HAS_MSA"
  "max_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_maxi_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_imm5_operand" "")]
			 UNSPEC_MSA_MAXI_S))]
  "ISA_HAS_MSA"
  "maxi_s.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_maxi_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_uimm5_operand" "")]
			 UNSPEC_MSA_MAXI_U))]
  "ISA_HAS_MSA"
  "maxi_u.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_min_a_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_MIN_A))]
  "ISA_HAS_MSA"
  "min_a.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_min_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_MIN_S))]
  "ISA_HAS_MSA"
  "min_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_min_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_MIN_U))]
  "ISA_HAS_MSA"
  "min_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_mini_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_imm5_operand" "")]
			 UNSPEC_MSA_MINI_S))]
  "ISA_HAS_MSA"
  "mini_s.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_mini_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_uimm5_operand" "")]
			 UNSPEC_MSA_MINI_U))]
  "ISA_HAS_MSA"
  "mini_u.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_msub_q_<msafmt>"
  [(set (match_operand:QMODE128 0 "register_operand" "=f")
	(unspec:QMODE128 [(match_operand:QMODE128 1 "register_operand" "0")
			  (match_operand:QMODE128 2 "register_operand" "f")
			  (match_operand:QMODE128 3 "register_operand" "f")]
			 UNSPEC_MSA_MSUB_Q))]
  "ISA_HAS_MSA"
  "msub_q.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_msubr_q_<msafmt>"
  [(set (match_operand:QMODE128 0 "register_operand" "=f")
	(unspec:QMODE128 [(match_operand:QMODE128 1 "register_operand" "0")
			  (match_operand:QMODE128 2 "register_operand" "f")
			  (match_operand:QMODE128 3 "register_operand" "f")]
			 UNSPEC_MSA_MSUBR_Q))]
  "ISA_HAS_MSA"
  "msubr_q.<msafmt>\t%w0,%w2,%w3"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_mul_q_<msafmt>"
  [(set (match_operand:QMODE128 0 "register_operand" "=f")
	(unspec:QMODE128 [(match_operand:QMODE128 1 "register_operand" "f")
			  (match_operand:QMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_MUL_Q))]
  "ISA_HAS_MSA"
  "mul_q.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_mulr_q_<msafmt>"
  [(set (match_operand:QMODE128 0 "register_operand" "=f")
	(unspec:QMODE128 [(match_operand:QMODE128 1 "register_operand" "f")
			  (match_operand:QMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_MULR_Q))]
  "ISA_HAS_MSA"
  "mulr_q.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_nloc_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")]
			 UNSPEC_MSA_NLOC))]
  "ISA_HAS_MSA"
  "nloc.<msafmt>\t%w0,%w1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_nlzc_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")]
			 UNSPEC_MSA_NLZC))]
  "ISA_HAS_MSA"
  "nlzc.<msafmt>\t%w0,%w1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_nor_v_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
        (and:IMODE128 (not:IMODE128 (match_operand:IMODE128 1 "register_operand" "f"))
		      (not:IMODE128 (match_operand:IMODE128 2 "register_operand" "f"))))]
  "ISA_HAS_MSA"
  "nor.v\t%w0,%w1,%w2"
  [(set_attr "alu_type"	"nor")
   (set_attr "mode"	"TI")])

(define_insn "msa_nori_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm8_operand" "")]
		      UNSPEC_MSA_NORI_B))]
  "ISA_HAS_MSA"
  "nori.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_ori_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm8_operand" "")]
		      UNSPEC_MSA_ORI_B))]
  "ISA_HAS_MSA"
  "ori.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_pckev_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_PCKEV))]
  "ISA_HAS_MSA"
  "pckev.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "alu_type"	"add")
   (set_attr "mode"	"TI")])

(define_insn "msa_pckod_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_PCKOD))]
  "ISA_HAS_MSA"
  "pckod.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "alu_type"	"add")
   (set_attr "mode"	"TI")])

(define_insn "msa_pcnt_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")]
			 UNSPEC_MSA_PCNT))]
  "ISA_HAS_MSA"
  "pcnt.<msafmt>\t%w0,%w1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sat_s_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm3_operand" "")]
		      UNSPEC_MSA_SAT_S))]
  "ISA_HAS_MSA"
  "sat_s.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sat_s_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm4_operand" "")]
		     UNSPEC_MSA_SAT_S))]
  "ISA_HAS_MSA"
  "sat_s.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sat_s_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_SAT_S))]
  "ISA_HAS_MSA"
  "sat_s.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sat_s_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm6_operand" "")]
		     UNSPEC_MSA_SAT_S))]
  "ISA_HAS_MSA"
  "sat_s.d\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sat_u_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm3_operand" "")]
		      UNSPEC_MSA_SAT_U))]
  "ISA_HAS_MSA"
  "sat_u.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sat_u_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm4_operand" "")]
		     UNSPEC_MSA_SAT_U))]
  "ISA_HAS_MSA"
  "sat_u.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sat_u_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_SAT_U))]
  "ISA_HAS_MSA"
  "sat_u.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sat_u_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm6_operand" "")]
		     UNSPEC_MSA_SAT_U))]
  "ISA_HAS_MSA"
  "sat_u.d\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_shf_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm8_operand" "")]
		      UNSPEC_MSA_SHF))]
  "ISA_HAS_MSA"
  "shf.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_shf_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm8_operand" "")]
		     UNSPEC_MSA_SHF))]
  "ISA_HAS_MSA"
  "shf.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_shf_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm8_operand" "")]
		     UNSPEC_MSA_SHF))]
  "ISA_HAS_MSA"
  "shf.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_shf_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V4SF 1 "register_operand" "f")
		      (match_operand 2 "const_uimm8_operand" "")]
		     UNSPEC_MSA_SHF))]
  "ISA_HAS_MSA"
  "shf.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_slli_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm3_operand" "")]
		      UNSPEC_MSA_SLLI))]
  "ISA_HAS_MSA"
  "slli.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_slli_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm4_operand" "")]
		     UNSPEC_MSA_SLLI))]
  "ISA_HAS_MSA"
  "slli.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_slli_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_SLLI))]
  "ISA_HAS_MSA"
  "slli.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_slli_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm6_operand" "")]
		     UNSPEC_MSA_SLLI))]
  "ISA_HAS_MSA"
  "slli.d\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srai_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm3_operand" "")]
		      UNSPEC_MSA_SRAI))]
  "ISA_HAS_MSA"
  "srai.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srai_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm4_operand" "")]
		     UNSPEC_MSA_SRAI))]
  "ISA_HAS_MSA"
  "srai.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srai_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_SRAI))]
  "ISA_HAS_MSA"
  "srai.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srai_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm6_operand" "")]
		     UNSPEC_MSA_SRAI))]
  "ISA_HAS_MSA"
  "srai.d\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srar_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_SRAR))]
  "ISA_HAS_MSA"
  "srar.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srari_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm3_operand" "")]
		      UNSPEC_MSA_SRARI))]
  "ISA_HAS_MSA"
  "srari.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srari_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm4_operand" "")]
		     UNSPEC_MSA_SRARI))]
  "ISA_HAS_MSA"
  "srari.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srari_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_SRARI))]
  "ISA_HAS_MSA"
  "srari.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srari_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm6_operand" "")]
		     UNSPEC_MSA_SRARI))]
  "ISA_HAS_MSA"
  "srari.d\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srli_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm3_operand" "")]
		      UNSPEC_MSA_SRLI))]
  "ISA_HAS_MSA"
  "srli.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srli_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm4_operand" "")]
		     UNSPEC_MSA_SRLI))]
  "ISA_HAS_MSA"
  "srli.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srli_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_SRLI))]
  "ISA_HAS_MSA"
  "srli.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srli_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm6_operand" "")]
		     UNSPEC_MSA_SRLI))]
  "ISA_HAS_MSA"
  "srli.d\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srlr_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_SRLR))]
  "ISA_HAS_MSA"
  "srlr.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srlri_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm3_operand" "")]
		      UNSPEC_MSA_SRLRI))]
  "ISA_HAS_MSA"
  "srlri.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srlri_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm4_operand" "")]
		     UNSPEC_MSA_SRLRI))]
  "ISA_HAS_MSA"
  "srlri.h\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srlri_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm5_operand" "")]
		     UNSPEC_MSA_SRLRI))]
  "ISA_HAS_MSA"
  "srlri.w\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_srlri_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm6_operand" "")]
		     UNSPEC_MSA_SRLRI))]
  "ISA_HAS_MSA"
  "srlri.d\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_subs_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_SUBS_S))]
  "ISA_HAS_MSA"
  "subs_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_subs_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_SUBS_U))]
  "ISA_HAS_MSA"
  "subs_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_subsuu_s_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_SUBSUU_S))]
  "ISA_HAS_MSA"
  "subsuu_s.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_subsus_u_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:IMODE128 2 "register_operand" "f")]
			 UNSPEC_MSA_SUBSUS_U))]
  "ISA_HAS_MSA"
  "subsus_u.<msafmt>\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_subvi_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand 2 "const_uimm5_operand" "")]
			 UNSPEC_MSA_SUBVI))]
  "ISA_HAS_MSA"
  "subvi.<msafmt>\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_xori_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm8_operand" "")]
		      UNSPEC_MSA_XORI_B))]
  "ISA_HAS_MSA"
  "xori.b\t%w0,%w1,%2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sld_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "0")
			  (match_operand:IMODE128 2 "register_operand" "f")
			  (match_operand:SI 3 "reg_or_0_operand" "dJ")]
			 UNSPEC_MSA_SLD))]
  "ISA_HAS_MSA"
  "sld.<msafmt>\t%w0,%w2[%z3]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sld_<msafmt>_f"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "0")
			  (match_operand:FMODE128 2 "register_operand" "f")
			  (match_operand:SI 3 "reg_or_0_operand" "dJ")]
			 UNSPEC_MSA_SLD))]
  "ISA_HAS_MSA"
  "sld.<msafmt>\t%w0,%w2[%z3]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sldi_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "0")
		       (match_operand:V16QI 2 "register_operand" "f")
		       (match_operand 3 "const_uimm4_operand" "")]
		      UNSPEC_MSA_SLDI))]
  "ISA_HAS_MSA"
  "sldi.b\t%w0,%w2[%3]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sldi_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "0")
		      (match_operand:V8HI 2 "register_operand" "f")
		      (match_operand 3 "const_uimm3_operand" "")]
		     UNSPEC_MSA_SLDI))]
  "ISA_HAS_MSA"
  "sldi.h\t%w0,%w2[%3]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sldi_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "0")
		      (match_operand:V4SI 2 "register_operand" "f")
		      (match_operand 3 "const_0_to_3_operand" "")]
		     UNSPEC_MSA_SLDI))]
  "ISA_HAS_MSA"
  "sldi.w\t%w0,%w2[%3]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sldi_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V4SF 1 "register_operand" "0")
		      (match_operand:V4SF 2 "register_operand" "f")
		      (match_operand 3 "const_0_to_3_operand" "")]
		     UNSPEC_MSA_SLDI))]
  "ISA_HAS_MSA"
  "sldi.w\t%w0,%w2[%3]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sldi_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "0")
		      (match_operand:V2DI 2 "register_operand" "f")
		      (match_operand 3 "const_0_or_1_operand" "")]
		     UNSPEC_MSA_SLDI))]
  "ISA_HAS_MSA"
  "sldi.d\t%w0,%w2[%3]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_sldi_d_f"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V2DF 1 "register_operand" "0")
		      (match_operand:V2DF 2 "register_operand" "f")
		      (match_operand 3 "const_0_or_1_operand" "")]
		     UNSPEC_MSA_SLDI))]
  "ISA_HAS_MSA"
  "sldi.d\t%w0,%w2[%3]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_splat_<msafmt>"
  [(set (match_operand:IMODE128 0 "register_operand" "=f")
	(unspec:IMODE128 [(match_operand:IMODE128 1 "register_operand" "f")
			  (match_operand:SI 2 "reg_or_0_operand" "dJ")]
			 UNSPEC_MSA_SPLAT))]
  "ISA_HAS_MSA"
  "splat.<msafmt>\t%w0,%w1[%z2]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_splat_<msafmt>_f"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:FMODE128 1 "register_operand" "f")
			  (match_operand:SI 2 "reg_or_0_operand" "dJ")]
			 UNSPEC_MSA_SPLAT))]
  "ISA_HAS_MSA"
  "splat.<msafmt>\t%w0,%w1[%z2]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_splati_b"
  [(set (match_operand:V16QI 0 "register_operand" "=f")
	(unspec:V16QI [(match_operand:V16QI 1 "register_operand" "f")
		       (match_operand 2 "const_uimm4_operand" "")]
		      UNSPEC_MSA_SPLATI))]
  "ISA_HAS_MSA"
  "splati.b\t%w0,%w1[%2]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_splati_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V8HI 1 "register_operand" "f")
		      (match_operand 2 "const_uimm3_operand" "")]
		     UNSPEC_MSA_SPLATI))]
  "ISA_HAS_MSA"
  "splati.h\t%w0,%w1[%2]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_splati_w"
  [(set (match_operand:V4SI 0 "register_operand" "=f")
	(unspec:V4SI [(match_operand:V4SI 1 "register_operand" "f")
		      (match_operand 2 "const_0_to_3_operand" "")]
		     UNSPEC_MSA_SPLATI))]
  "ISA_HAS_MSA"
  "splati.w\t%w0,%w1[%2]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_splati_w_f"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V4SF 1 "register_operand" "f")
		      (match_operand 2 "const_0_to_3_operand" "")]
		     UNSPEC_MSA_SPLATI))]
  "ISA_HAS_MSA"
  "splati.w\t%w0,%w1[%2]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

;; operand 1 is a scalar
(define_insn "msa_splati_w_f_s"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:SF 1 "register_operand" "f")
		      (match_operand 2 "const_0_to_3_operand" "")]
		     UNSPEC_MSA_SPLATI))]
  "ISA_HAS_MSA"
  "splati.w\t%w0,%w1[%2]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_splati_d"
  [(set (match_operand:V2DI 0 "register_operand" "=f")
	(unspec:V2DI [(match_operand:V2DI 1 "register_operand" "f")
		      (match_operand 2 "const_0_or_1_operand" "")]
		     UNSPEC_MSA_SPLATI))]
  "ISA_HAS_MSA"
  "splati.d\t%w0,%w1[%2]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_splati_d_f"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V2DF 1 "register_operand" "f")
		      (match_operand 2 "const_0_or_1_operand" "")]
		     UNSPEC_MSA_SPLATI))]
  "ISA_HAS_MSA"
  "splati.d\t%w0,%w1[%2]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

;; operand 1 is a scalar
(define_insn "msa_splati_d_f_s"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:DF 1 "register_operand" "f")
		      (match_operand 2 "const_0_or_1_operand" "")]
		     UNSPEC_MSA_SPLATI))]
  "ISA_HAS_MSA"
  "splati.d\t%w0,%w1[%2]"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_cfcmsa"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec_volatile:SI [(match_operand 1 "const_uimm5_operand" "")]
			    UNSPEC_MSA_CFCMSA))]
  "ISA_HAS_MSA"
  "cfcmsa\t%0,$%1"
  [(set_attr "type"	"mfc")
   (set_attr "mode"	"SI")])

(define_insn "msa_ctcmsa"
  [(unspec_volatile [(match_operand 0 "const_uimm5_operand" "")
		     (match_operand:SI 1 "register_operand" "d")]
		    UNSPEC_MSA_CTCMSA)]
  "ISA_HAS_MSA"
  "ctcmsa\t$%0,%1"
  [(set_attr "type"	"mtc")
   (set_attr "mode"	"SI")])

(define_insn "msa_fexdo_h"
  [(set (match_operand:V8HI 0 "register_operand" "=f")
	(unspec:V8HI [(match_operand:V4SF 1 "register_operand" "f")
		      (match_operand:V4SF 2 "register_operand" "f")]
		     UNSPEC_MSA_FEXDO))]
  "ISA_HAS_MSA"
  "fexdo.h\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_fexdo_w"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V2DF 1 "register_operand" "f")
		      (match_operand:V2DF 2 "register_operand" "f")]
		     UNSPEC_MSA_FEXDO))]
  "ISA_HAS_MSA"
  "fexdo.w\t%w0,%w1,%w2"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_fexupl_w"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V8HI 1 "register_operand" "f")]
		     UNSPEC_MSA_FEXUPL))]
  "ISA_HAS_MSA"
  "fexupl.w\t%w0,%w1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_fexupl_d"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V4SF 1 "register_operand" "f")]
		     UNSPEC_MSA_FEXUPL))]
  "ISA_HAS_MSA"
  "fexupl.d\t%w0,%w1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_fexupr_w"
  [(set (match_operand:V4SF 0 "register_operand" "=f")
	(unspec:V4SF [(match_operand:V8HI 1 "register_operand" "f")]
		     UNSPEC_MSA_FEXUPR))]
  "ISA_HAS_MSA"
  "fexupr.w\t%w0,%w1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_fexupr_d"
  [(set (match_operand:V2DF 0 "register_operand" "=f")
	(unspec:V2DF [(match_operand:V4SF 1 "register_operand" "f")]
		     UNSPEC_MSA_FEXUPR))]
  "ISA_HAS_MSA"
  "fexupr.d\t%w0,%w1"
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

(define_insn "msa_bn_v_<msafmt>"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:IMODE128 1 "register_operand" "f")]
		   UNSPEC_MSA_BN_V))]
  "ISA_HAS_MSA"
  "%(bn.v\t%w1,1f; li\t%0,1; li\t%0,0%)\n1:"
  [(set_attr "type"	"arith")
   (set_attr "length"	"12")
   (set_attr "mode"	"TI")])

(define_insn "msa_bn_v_<msafmt>_f"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:FMODE128 1 "register_operand" "f")]
		   UNSPEC_MSA_BN_V))]
  "ISA_HAS_MSA"
  "%(bn.v\t%w1,1f; li\t%0,1; li\t%0,0%)\n1:"
  [(set_attr "type"	"arith")
   (set_attr "length"	"12")
   (set_attr "mode"	"TI")])

(define_insn "msa_bz_v_<msafmt>"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:IMODE128 1 "register_operand" "f")]
		   UNSPEC_MSA_BZ_V))]
  "ISA_HAS_MSA"
  "%(bz.v\t%w1,1f; li\t%0,1; li\t%0,0%)\n1:"
  [(set_attr "type"	"arith")
   (set_attr "length"	"12")
   (set_attr "mode"	"TI")])

(define_insn "msa_bz_v_<msafmt>_f"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:FMODE128 1 "register_operand" "f")]
		   UNSPEC_MSA_BZ_V))]
  "ISA_HAS_MSA"
  "%(bz.v\t%w1,1f; li\t%0,1; li\t%0,0%)\n1:"
  [(set_attr "type"	"arith")
   (set_attr "length"	"12")
   (set_attr "mode"	"TI")])

(define_insn "msa_bn_<msafmt>"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:IMODE128 1 "register_operand" "f")]
		   UNSPEC_MSA_BN))]
  "ISA_HAS_MSA"
  "%(bn.<msafmt>\t%w1,1f; li\t%0,1; li\t%0,0%)\n1:"
  [(set_attr "type"	"arith")
   (set_attr "length"	"12")
   (set_attr "mode"	"TI")])

(define_insn "msa_bn_<msafmt>_f"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:FMODE128 1 "register_operand" "f")]
		   UNSPEC_MSA_BN))]
  "ISA_HAS_MSA"
  "%(bn.<msafmt>\t%w1,1f; li\t%0,1; li\t%0,0%)\n1:"
  [(set_attr "type"	"arith")
   (set_attr "length"	"12")
   (set_attr "mode"	"TI")])

(define_insn "msa_bz_<msafmt>"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:IMODE128 1 "register_operand" "f")]
		   UNSPEC_MSA_BZ))]
  "ISA_HAS_MSA"
  "%(bz.<msafmt>\t%w1,1f; li\t%0,1; li\t%0,0%)\n1:"
  [(set_attr "type"	"arith")
   (set_attr "length"	"12")
   (set_attr "mode"	"TI")])

(define_insn "msa_bz_<msafmt>_f"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:FMODE128 1 "register_operand" "f")]
		   UNSPEC_MSA_BZ))]
  "ISA_HAS_MSA"
  "%(bz.<msafmt>\t%w1,1f; li\t%0,1; li\t%0,0%)\n1:"
  [(set_attr "type"	"arith")
   (set_attr "length"	"12")
   (set_attr "mode"	"TI")])

;; Note that this instruction treats scalar as vector registers freely.
(define_insn "msa_cast_to_vector_<msafmt>_f"
  [(set (match_operand:FMODE128 0 "register_operand" "=f")
	(unspec:FMODE128 [(match_operand:<ELEMMODE> 1 "register_operand" "f")]
			 UNSPEC_MSA_CAST_TO_VECTOR))]
  "ISA_HAS_MSA"
{
  if (REGNO (operands[0]) == REGNO (operands[1]))
    return "nop\t# Cast %1 to %w0";
  else
    return "mov.<msafmt2>\t%w0,%1\t# Cast %1 to %w0";
}
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])

;; Note that this instruction treats vector as scalar registers freely.
(define_insn "msa_cast_to_scalar_<msafmt>_f"
  [(set (match_operand:<ELEMMODE> 0 "register_operand" "=f")
	(unspec:<ELEMMODE> [(match_operand:FMODE128 1 "register_operand" "f")]
			   UNSPEC_MSA_CAST_TO_SCALAR))]
  "ISA_HAS_MSA"
{
  if (REGNO (operands[0]) == REGNO (operands[1]))
    return "nop\t# Cast %w1 to %0";
  else
    return "mov.<msafmt2>\t%0,%w1\t# Cast %w1 to %0";
}
  [(set_attr "type"	"arith")
   (set_attr "mode"	"TI")])
