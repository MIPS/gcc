;; ARM NEON coprocessor Machine Description
;; Copyright (C) 2006 Free Software Foundation, Inc.
;; Written by CodeSourcery.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful, but
;; WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING.  If not, write to the Free
;; Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
;; 02110-1301, USA.

;; Constants for unspecs.
(define_constants
  [(UNSPEC_VEXT 64)
   (UNSPEC_VPADD 65)
   (UNSPEC_VPSMIN 66)
   (UNSPEC_VPUMIN 67)
   (UNSPEC_VPSMAX 68)
   (UNSPEC_VPUMAX 69)
   (UNSPEC_ASHIFT_SIGNED 70)
   (UNSPEC_ASHIFT_UNSIGNED 71)])

;; Double-width vector modes.
(define_mode_macro VD [V8QI V4HI V2SI V2SF])

;; Same, without floating-point elements.
(define_mode_macro VDI [V8QI V4HI V2SI])

;; Quad-width vector modes.
(define_mode_macro VQ [V16QI V8HI V4SI V4SF])

;; Same, without floating-point elements.
(define_mode_macro VQI [V16QI V8HI V4SI])

;; Widenable modes.
(define_mode_macro VW [V8QI V4HI V2SI])

;; Narrowable modes.
(define_mode_macro VN [V8HI V4SI V2DI])

;; All supported vector modes.
(define_mode_macro VDQ [V8QI V16QI V4HI V8HI V2SI V4SI V2SF V4SF])

;; Supported integer vector modes.
(define_mode_macro VDQI [V8QI V16QI V4HI V8HI V2SI V4SI])

;; Define element mode for each vector mode.
(define_mode_attr V_elem [(V8QI "QI") (V16QI "QI")
			  (V4HI "HI") (V8HI "HI")
                          (V2SI "SI") (V4SI "SI")
                          (V2SF "SF") (V4SF "SF")])

;; Register width from element mode
(define_mode_attr V_reg [(V8QI "P") (V16QI "q")
                         (V4HI "P") (V8HI "q")
                         (V2SI "P") (V4SI "q")
                         (V2SF "P") (V4SF "q")])

;; Wider modes with the same number of elements.
(define_mode_attr V_widen [(V8QI "V8HI") (V4HI "V4SI") (V2SI "V2DI")])

;; Narrower modes with the same number of elements.
(define_mode_attr V_narrow [(V8HI "V8QI") (V4SI "V4HI") (V2DI "V2SI")])

;; Get element type from double-width mode, for operations where we don't care
;; about signedness.
(define_mode_attr V_if_elem [(V8QI "i8") (V16QI "i8")
			     (V4HI "i16") (V8HI "i16")
                             (V2SI "i32") (V4SI "i32")
			     (V2SF "f32") (V4SF "f32")])

;; Same, but for operations which work on signed values.
(define_mode_attr V_s_elem [(V8QI "s8") (V16QI "s8")
			    (V4HI "s16") (V8HI "s16")
                            (V2SI "s32") (V4SI "s32")
			    (V2SF "f32") (V4SF "f32")])

;; Same, but for operations which work on unsigned values.
(define_mode_attr V_u_elem [(V8QI "u8") (V16QI "u8")
			    (V4HI "u16") (V8HI "u16")
                            (V2SI "u32") (V4SI "u32")])

;; Element types for extraction of unsigned scalars.
(define_mode_attr VD_uf_sclr [(V8QI "u8") (V4HI "u16") (V2SI "32") (V2SF "32")])

;; Element sizes for duplicating ARM registers to all elements of a vector.
(define_mode_attr VD_dup [(V8QI "8") (V4HI "16") (V2SI "32") (V2SF "32")])

;; FIXME: Attributes are probably borked.
(define_insn "*neon_mov<mode>"
  [(set (match_operand:VD 0 "nonimmediate_operand"
	  "=w,Uv,w, w,  ?r,?w,?r,?r, ?Us")
	(match_operand:VD 1 "general_operand"
	  " w,w, Dn,Uvi, w, r, r, Usi,r"))]
  "TARGET_NEON"
{
  if (which_alternative == 2)
    {
      int width, is_valid;
      static char templ[40];

      is_valid = neon_immediate_valid_for_move (operands[1], <MODE>mode,
        &operands[1], &width);

      gcc_assert (is_valid != 0);

      if (width == 0)
        return "vmov.f32\t%P0, %1  @ <mode>";
      else
        sprintf (templ, "vmov.i%d\t%%P0, %%1  @ <mode>", width);

      return templ;
    }

  /* FIXME: If the memory layout is changed in big-endian mode, output_move_vfp
     below must be changed to output_move_neon (which will use the
     element/structure loads/stores), and the constraint changed to 'Un' instead
     of 'Uv'.  */

  switch (which_alternative)
    {
    case 0: return "vmov\t%P0, %P1  @ <mode>";
    case 1: case 3: return output_move_vfp (operands);
    case 2: gcc_unreachable ();
    case 4: return "vmov\t%Q0, %R0, %P1  @ <mode>";
    case 5: return "vmov\t%P0, %Q1, %R1  @ <mode>";
    default: return output_move_double (operands);
    }
}
  [(set_attr "type" "farith,f_stored,farith,f_loadd,f_2_r,r_2_f,*,load2,store2")
   (set_attr "length" "4,4,4,4,4,4,8,8,8")
   (set_attr "pool_range"     "*,*,*,1020,*,*,*,1020,*")
   (set_attr "neg_pool_range" "*,*,*,1008,*,*,*,1008,*")
   (set_attr "predicable" "no")])

(define_insn "*neon_mov<mode>"
  [(set (match_operand:VQ 0 "nonimmediate_operand"
  	  "=w,Un,w, w,  ?r,?w,?r,?r,  ?Us")
	(match_operand:VQ 1 "general_operand"
	  " w,w, Dn,Uni, w, r, r, Usi, r"))]
  "TARGET_NEON"
{
  if (which_alternative == 2)
    {
      int width, is_valid;
      static char templ[40];
      
      is_valid = neon_immediate_valid_for_move (operands[1], <MODE>mode,
        &operands[1], &width);
      
      gcc_assert (is_valid != 0);
      
      if (width == 0)
        return "vmov.f32\t%q0, %1  @ <mode>";
      else
        sprintf (templ, "vmov.i%d\t%%q0, %%1  @ <mode>", width);
      
      return templ;
    }
  
  switch (which_alternative)
    {
    case 0: return "vmov\t%q0, %q1  @ <mode>";
    case 1: case 3: return output_move_neon (operands);
    case 2: gcc_unreachable ();
    case 4: return "vmov\t%Q0, %R0, %e1  @ <mode>\;vmov\t%J0, %K0, %f1";
    case 5: return "vmov\t%e0, %Q1, %R1  @ <mode>\;vmov\t%f0, %J1, %K1";
    default: return output_move_quad (operands);
    }
}
  [(set_attr "type" "farith,f_stored,farith,f_loadd,f_2_r,r_2_f,*,load2,store2")
   (set_attr "length" "4,8,4,8,8,8,16,8,16")
   (set_attr "pool_range" "*,*,*,1020,*,*,*,1020,*")
   (set_attr "neg_pool_range" "*,*,*,1008,*,*,*,1008,*")
   (set_attr "predicable" "no")])

; FIXME: Set/extract/init quads.

(define_insn "vec_set<mode>"
  [(set (match_operand:VD 0 "s_register_operand" "+w")
        (vec_merge:VD
          (match_operand:VD 1 "s_register_operand" "0")
          (vec_duplicate:VD
            (match_operand:<V_elem> 2 "s_register_operand" "r"))
          (ashift:SI (const_int 1)
                     (match_operand:SI 3 "immediate_operand" "i"))))]
  "TARGET_NEON"
  "vmov.<VD_uf_sclr>\t%P0[%n3], %2"
  [(set_attr "predicable" "yes")])

(define_insn "vec_extract<mode>"
  [(set (match_operand:<V_elem> 0 "s_register_operand" "=r")
        (vec_select:<V_elem>
          (match_operand:VD 1 "s_register_operand" "w")
          (parallel [(match_operand:SI 2 "immediate_operand" "i")])))]
  "TARGET_NEON"
  "vmov.<VD_uf_sclr>\t%0, %P1[%n2]"
  [(set_attr "predicable" "yes")])

(define_expand "vec_init<mode>"
  [(match_operand:VD 0 "s_register_operand" "")
   (match_operand 1 "" "")]
  "TARGET_NEON"
{
  neon_expand_vector_init (operands[0], operands[1]);
  DONE;
})

;; Doubleword and quadword arithmetic.

;; NOTE: vadd/vsub and some other instructions also support 64-bit integer
;; element size, which we could potentially use for "long long" operations. We
;; don't want to do this at present though, because moving values from the
;; vector unit to the ARM core is currently slow and 64-bit addition (etc.) is
;; easy to do with ARM instructions anyway.

(define_insn "*add<mode>3_neon"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
        (plus:VDQ (match_operand:VDQ 1 "s_register_operand" "w")
		  (match_operand:VDQ 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vadd.<V_if_elem>\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_insn "*sub<mode>3_neon"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
        (minus:VDQ (match_operand:VDQ 1 "s_register_operand" "w")
                   (match_operand:VDQ 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vsub.<V_if_elem>\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_insn "*mul<mode>3_neon"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
        (mult:VDQ (match_operand:VDQ 1 "s_register_operand" "w")
                  (match_operand:VDQ 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vmul.<V_if_elem>\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_insn "ior<mode>3"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w,w")
	(ior:VDQ (match_operand:VDQ 1 "s_register_operand" "w,0")
		 (match_operand:VDQ 2 "neon_logic_op2" "w,Dl")))]
  "TARGET_NEON"
{
  switch (which_alternative)
    {
    case 0: return "vorr\t%<V_reg>0, %<V_reg>1, %<V_reg>2";
    case 1: return neon_output_logic_immediate ("vorr", &operands[2],
		     <MODE>mode, 0, VALID_NEON_QREG_MODE (<MODE>mode));
    default: gcc_unreachable ();
    }
}
  [(set_attr "predicable" "no")])

;; The concrete forms of the Neon immediate-logic instructions are vbic and
;; vorr. We support the pseudo-instruction vand instead, because that
;; corresponds to the canonical form the middle-end expects to use for
;; immediate bitwise-ANDs.

(define_insn "and<mode>3"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w,w")
	(and:VDQ (match_operand:VDQ 1 "s_register_operand" "w,0")
		 (match_operand:VDQ 2 "neon_inv_logic_op2" "w,DL")))]
  "TARGET_NEON"
{
  switch (which_alternative)
    {
    case 0: return "vand\t%<V_reg>0, %<V_reg>1, %<V_reg>2";
    case 1: return neon_output_logic_immediate ("vand", &operands[2],
    		     <MODE>mode, 1, VALID_NEON_QREG_MODE (<MODE>mode));
    default: gcc_unreachable ();
    }
}
  [(set_attr "predicable" "no")])

(define_insn "*orn<mode>3_neon"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
	(ior:VDQ (match_operand:VDQ 1 "s_register_operand" "w")
		 (not:VDQ (match_operand:VDQ 2 "s_register_operand" "w"))))]
  "TARGET_NEON"
  "vorn\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_insn "*bic<mode>3_neon"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
	(and:VDQ (match_operand:VDQ 1 "s_register_operand" "w")
		 (not:VDQ (match_operand:VDQ 2 "s_register_operand" "w"))))]
  "TARGET_NEON"
  "vbic\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_insn "xor<mode>3"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
	(xor:VDQ (match_operand:VDQ 1 "s_register_operand" "w")
		 (match_operand:VDQ 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "veor\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_insn "one_cmpl<mode>2"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
        (not:VDQ (match_operand:VDQ 1 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vmvn\t%<V_reg>0, %<V_reg>1"
  [(set_attr "predicable" "no")])

(define_insn "abs<mode>2"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
	(abs:VDQ (match_operand:VDQ 1 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vabs.<V_s_elem>\t%<V_reg>0, %<V_reg>1"
  [(set_attr "predicable" "no")])

(define_insn "neg<mode>2"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
	(neg:VDQ (match_operand:VDQ 1 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vneg.<V_s_elem>\t%<V_reg>0, %<V_reg>1"
  [(set_attr "predicable" "no")])

(define_insn "*umin<mode>3_neon"
  [(set (match_operand:VDQI 0 "s_register_operand" "=w")
	(umin:VDQI (match_operand:VDQI 1 "s_register_operand" "w")
		   (match_operand:VDQI 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vmin.<V_u_elem>\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_insn "*umax<mode>3_neon"
  [(set (match_operand:VDQI 0 "s_register_operand" "=w")
	(umax:VDQI (match_operand:VDQI 1 "s_register_operand" "w")
		   (match_operand:VDQI 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vmax.<V_u_elem>\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_insn "*smin<mode>3_neon"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
	(smin:VDQ (match_operand:VDQ 1 "s_register_operand" "w")
		  (match_operand:VDQ 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vmin.<V_s_elem>\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_insn "*smax<mode>3_neon"
  [(set (match_operand:VDQ 0 "s_register_operand" "=w")
	(smax:VDQ (match_operand:VDQ 1 "s_register_operand" "w")
		  (match_operand:VDQ 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vmax.<V_s_elem>\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_insn "ashl<mode>3"
  [(set (match_operand:VDQI 0 "s_register_operand" "=w")
       (ashift:VDQI (match_operand:VDQI 1 "s_register_operand" "w")
                    (match_operand:VDQI 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vshl.<V_s_elem>\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

; Used for implementing logical shift-right, which is a left-shift by a negative
; amount, with signed operands. This is essentially the same as ashl<mode>3
; above, but using an unspec in case GCC tries anything tricky with negative
; shift amounts.

(define_insn "ashl<mode>3_signed"
  [(set (match_operand:VDQI 0 "s_register_operand" "=w")
	(unspec:VDQI [(match_operand:VDQI 1 "s_register_operand" "w")
		      (match_operand:VDQI 2 "s_register_operand" "w")]
		     UNSPEC_ASHIFT_SIGNED))]
  "TARGET_NEON"
  "vshl.<V_s_elem>\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

; Used for implementing logical shift-right, which is a left-shift by a negative
; amount, with unsigned operands.

(define_insn "ashl<mode>3_unsigned"
  [(set (match_operand:VDQI 0 "s_register_operand" "=w")
	(unspec:VDQI [(match_operand:VDQI 1 "s_register_operand" "w")
		      (match_operand:VDQI 2 "s_register_operand" "w")]
		     UNSPEC_ASHIFT_UNSIGNED))]
  "TARGET_NEON"
  "vshl.<V_u_elem>\t%<V_reg>0, %<V_reg>1, %<V_reg>2"
  [(set_attr "predicable" "no")])

(define_expand "ashr<mode>3"
  [(set (match_operand:VDQI 0 "s_register_operand" "")
	(ashiftrt:VDQI (match_operand:VDQI 1 "s_register_operand" "")
		       (match_operand:VDQI 2 "s_register_operand" "")))]
  "TARGET_NEON"
{
  rtx neg = gen_reg_rtx (<MODE>mode);

  emit_insn (gen_neg<mode>2 (neg, operands[2]));
  emit_insn (gen_ashl<mode>3_signed (operands[0], operands[1], neg));

  DONE;
})

(define_expand "lshr<mode>3"
  [(set (match_operand:VDQI 0 "s_register_operand" "")
	(lshiftrt:VDQI (match_operand:VDQI 1 "s_register_operand" "")
		       (match_operand:VDQI 2 "s_register_operand" "")))]
  "TARGET_NEON"
{
  rtx neg = gen_reg_rtx (<MODE>mode);

  emit_insn (gen_neg<mode>2 (neg, operands[2]));
  emit_insn (gen_ashl<mode>3_unsigned (operands[0], operands[1], neg));

  DONE;
})

;; Widening operations

;; FIXME: I'm not sure if sign/zero_extend are legal to use on vector modes.

(define_insn "widen_ssum<mode>3"
  [(set (match_operand:<V_widen> 0 "s_register_operand" "=w")
	(plus:<V_widen> (sign_extend:<V_widen>
			  (match_operand:VW 1 "s_register_operand" "%w"))
		        (match_operand:<V_widen> 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vaddw.<V_s_elem>\t%q0, %q2, %P1"
  [(set_attr "predicable" "no")])

(define_insn "widen_usum<mode>3"
  [(set (match_operand:<V_widen> 0 "s_register_operand" "=w")
	(plus:<V_widen> (zero_extend:<V_widen>
			  (match_operand:VW 1 "s_register_operand" "%w"))
		        (match_operand:<V_widen> 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vaddw.<V_u_elem>\t%q0, %q2, %P1"
  [(set_attr "predicable" "no")])

;; VEXT can be used to synthesize coarse whole-vector shifts with 8-bit
;; shift-count granularity. That's good enough for the middle-end's current
;; needs.

(define_expand "vec_shr_<mode>"
  [(match_operand:VDQ 0 "s_register_operand" "")
   (match_operand:VDQ 1 "s_register_operand" "")
   (match_operand:SI 2 "const_multiple_of_8_operand" "")]
  "TARGET_NEON"
{
  rtx zero_reg;
  HOST_WIDE_INT num_bits = INTVAL (operands[2]);
  const int width = GET_MODE_BITSIZE (<MODE>mode);
  const enum machine_mode bvecmode = (width == 128) ? V16QImode : V8QImode;
  rtx (*gen_ext) (rtx, rtx, rtx, rtx) =
    (width == 128) ? gen_neon_vextv16qi : gen_neon_vextv8qi;

  if (num_bits == width)
    {
      emit_move_insn (operands[0], operands[1]);
      DONE;
    }
  
  zero_reg = force_reg (bvecmode, CONST0_RTX (bvecmode));
  operands[0] = gen_lowpart (bvecmode, operands[0]);
  operands[1] = gen_lowpart (bvecmode, operands[1]);
  
  emit_insn (gen_ext (operands[0], operands[1], zero_reg,
		      GEN_INT (num_bits / BITS_PER_UNIT)));
  DONE;
})

(define_expand "vec_shl_<mode>"
  [(match_operand:VDQ 0 "s_register_operand" "")
   (match_operand:VDQ 1 "s_register_operand" "")
   (match_operand:SI 2 "const_multiple_of_8_operand" "")]
  "TARGET_NEON"
{
  rtx zero_reg;
  HOST_WIDE_INT num_bits = INTVAL (operands[2]);
  const int width = GET_MODE_BITSIZE (<MODE>mode);
  const enum machine_mode bvecmode = (width == 128) ? V16QImode : V8QImode;
  rtx (*gen_ext) (rtx, rtx, rtx, rtx) =
    (width == 128) ? gen_neon_vextv16qi : gen_neon_vextv8qi;
  
  if (num_bits == 0)
    {
      emit_move_insn (operands[0], CONST0_RTX (<MODE>mode));
      DONE;
    }
  
  num_bits = width - num_bits;
  
  zero_reg = force_reg (bvecmode, CONST0_RTX (bvecmode));
  operands[0] = gen_lowpart (bvecmode, operands[0]);
  operands[1] = gen_lowpart (bvecmode, operands[1]);
  
  emit_insn (gen_ext (operands[0], zero_reg, operands[1],
		      GEN_INT (num_bits / BITS_PER_UNIT)));
  DONE;
})

(define_insn "neon_vextv8qi"
  [(set (match_operand:V8QI 0 "s_register_operand" "=w")
        (unspec:V8QI [(match_operand:V8QI 1 "s_register_operand" "w")
		      (match_operand:V8QI 2 "s_register_operand" "w")
		      (match_operand:SI 3 "immediate_operand" "i")]
                     UNSPEC_VEXT))]
  "TARGET_NEON"
  "vext.8\t%P0, %P1, %P2, %3"
  [(set_attr "predicable" "no")])

(define_insn "neon_vextv16qi"
  [(set (match_operand:V16QI 0 "s_register_operand" "=w")
        (unspec:V16QI [(match_operand:V16QI 1 "s_register_operand" "w")
		       (match_operand:V16QI 2 "s_register_operand" "w")
		       (match_operand:SI 3 "immediate_operand" "i")]
                      UNSPEC_VEXT))]
  "TARGET_NEON"
  "vext.8\t%q0, %q1, %q2, %3"
  [(set_attr "predicable" "no")])

;; Reduction operations

; We have pairwise addition with wraparound semantics: we don't need to define
; reduc_splus_<mode> too.

(define_expand "reduc_uplus_<mode>"
  [(match_operand:VD 0 "s_register_operand" "")
   (match_operand:VD 1 "s_register_operand" "")]
  "TARGET_NEON"
{
  neon_pairwise_reduce (operands[0], operands[1], <MODE>mode,
			&gen_neon_vpadd<mode>);
  DONE;
})

(define_expand "reduc_smin_<mode>"
  [(match_operand:VD 0 "s_register_operand" "")
   (match_operand:VD 1 "s_register_operand" "")]
  "TARGET_NEON"
{
  neon_pairwise_reduce (operands[0], operands[1], <MODE>mode,
			&gen_neon_vpsmin<mode>);
  DONE;
})

(define_expand "reduc_smax_<mode>"
  [(match_operand:VD 0 "s_register_operand" "")
   (match_operand:VD 1 "s_register_operand" "")]
  "TARGET_NEON"
{
  neon_pairwise_reduce (operands[0], operands[1], <MODE>mode,
			&gen_neon_vpsmax<mode>);
  DONE;
})

(define_expand "reduc_umin_<mode>"
  [(match_operand:VDI 0 "s_register_operand" "")
   (match_operand:VDI 1 "s_register_operand" "")]
  "TARGET_NEON"
{
  neon_pairwise_reduce (operands[0], operands[1], <MODE>mode,
			&gen_neon_vpumin<mode>);
  DONE;
})

(define_expand "reduc_umax_<mode>"
  [(match_operand:VDI 0 "s_register_operand" "")
   (match_operand:VDI 1 "s_register_operand" "")]
  "TARGET_NEON"
{
  neon_pairwise_reduce (operands[0], operands[1], <MODE>mode,
			&gen_neon_vpumax<mode>);
  DONE;
})

(define_insn "neon_vpadd<mode>"
  [(set (match_operand:VD 0 "s_register_operand" "=w")
	(unspec:VD [(match_operand:VD 1 "s_register_operand" "w")
		    (match_operand:VD 2 "s_register_operand" "w")]
                   UNSPEC_VPADD))]
  "TARGET_NEON"
  "vpadd.<V_if_elem>\t%P0, %P1, %P2"
  [(set_attr "predicable" "no")])

(define_insn "neon_vpsmin<mode>"
  [(set (match_operand:VD 0 "s_register_operand" "=w")
	(unspec:VD [(match_operand:VD 1 "s_register_operand" "w")
		    (match_operand:VD 2 "s_register_operand" "w")]
                   UNSPEC_VPSMIN))]
  "TARGET_NEON"
  "vpmin.<V_s_elem>\t%P0, %P1, %P2"
  [(set_attr "predicable" "no")])

(define_insn "neon_vpsmax<mode>"
  [(set (match_operand:VD 0 "s_register_operand" "=w")
	(unspec:VD [(match_operand:VD 1 "s_register_operand" "w")
		    (match_operand:VD 2 "s_register_operand" "w")]
                   UNSPEC_VPSMAX))]
  "TARGET_NEON"
  "vpmax.<V_s_elem>\t%P0, %P1, %P2"
  [(set_attr "predicable" "no")])

(define_insn "neon_vpumin<mode>"
  [(set (match_operand:VDI 0 "s_register_operand" "=w")
	(unspec:VDI [(match_operand:VDI 1 "s_register_operand" "w")
		     (match_operand:VDI 2 "s_register_operand" "w")]
                   UNSPEC_VPUMIN))]
  "TARGET_NEON"
  "vpmin.<V_u_elem>\t%P0, %P1, %P2"
  [(set_attr "predicable" "no")])

(define_insn "neon_vpumax<mode>"
  [(set (match_operand:VDI 0 "s_register_operand" "=w")
	(unspec:VDI [(match_operand:VDI 1 "s_register_operand" "w")
		     (match_operand:VDI 2 "s_register_operand" "w")]
                   UNSPEC_VPUMAX))]
  "TARGET_NEON"
  "vpmax.<V_u_elem>\t%P0, %P1, %P2"
  [(set_attr "predicable" "no")])

;; Saturating arithmetic

; NOTE: Neon supports many more saturating variants of instructions than the
; following, but these are all GCC currently understands.
; FIXME: Actually, GCC doesn't know how to create saturating add/sub by itself
; yet either, although these patterns may be used by intrinsics when they're
; added.

(define_insn "*ss_add<mode>_neon"
  [(set (match_operand:VD 0 "s_register_operand" "=w")
       (ss_plus:VD (match_operand:VD 1 "s_register_operand" "w")
                   (match_operand:VD 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vqadd.<V_s_elem>\t%P0, %P1, %P2"
  [(set_attr "predicable" "no")])

(define_insn "*us_add<mode>_neon"
  [(set (match_operand:VD 0 "s_register_operand" "=w")
       (us_plus:VD (match_operand:VD 1 "s_register_operand" "w")
                   (match_operand:VD 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vqadd.<V_u_elem>\t%P0, %P1, %P2"
  [(set_attr "predicable" "no")])

(define_insn "*ss_sub<mode>_neon"
  [(set (match_operand:VD 0 "s_register_operand" "=w")
       (ss_minus:VD (match_operand:VD 1 "s_register_operand" "w")
                    (match_operand:VD 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vqsub.<V_s_elem>\t%P0, %P1, %P2"
  [(set_attr "predicable" "no")])

(define_insn "*us_sub<mode>_neon"
  [(set (match_operand:VD 0 "s_register_operand" "=w")
       (us_minus:VD (match_operand:VD 1 "s_register_operand" "w")
                    (match_operand:VD 2 "s_register_operand" "w")))]
  "TARGET_NEON"
  "vqsub.<V_u_elem>\t%P0, %P1, %P2"
  [(set_attr "predicable" "no")])

; FIXME: These instructions aren't supported in GCC 4.1, but are documented
; for the current trunk. Uncomment when this code is merged to a GCC version
; which supports them.

;(define_insn "*ss_neg<mode>_neon"
;  [(set (match_operand:VD 0 "s_register_operand" "=w")
;      (ss_neg:VD 1 (match_operand:VD 1 "s_register_operand" "w")))]
;  "TARGET_NEON"
;  "vqneg.<V_s_elem>\t%P0, %P1"
;  [(set_attr "predicable" "no")])

;(define_insn "*ss_ashift<mode>_neon"
;  [(set (match_operand:VD 0 "s_register_operand" "=w")
;      (ss_ashift:VD (match_operand:VD 1 "s_register_operand" "w")
;                    (match_operand:VD 2 "s_register_operand" "w")))]
;  "TARGET_NEON"
;  "vqshl.<V_s_elem>\t%P0, %P1, %P2"
;  [(set_attr "predicable" "no")])
