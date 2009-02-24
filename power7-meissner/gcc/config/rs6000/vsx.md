;; VSX patterns.
;; Copyright (C) 2009
;; Free Software Foundation, Inc.
;; Contributed by Michael Meissner <meissner@linux.vnet.ibm.com>

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

;; Iterator for vector floating point types supported by VSX
(define_mode_iterator VSX_F [V4SF V2DF])

;; Iterator for vector integer load types supported by VSX
(define_mode_iterator VSX_I [V16QI V8HI V4SI V2DI])

;; Iterator for logical types supported by VSX
(define_mode_iterator VSX_L [V16QI V8HI V4SI V2DI V4SF V2DF])

;; Map into the appropriate load/store name based on the type
(define_mode_attr VSm  [(V16QI "vw4")
			(V8HI  "vw4")
			(V4SI  "vw4")
			(V4SF  "vw4")
			(V2DF  "vd2")
			(V2DI  "vd2")])

;; Map into the appropriate suffix based on the type
(define_mode_attr VSs	[(V16QI "sp")
			 (V8HI  "sp")
			 (V4SI  "sp")
			 (V4SF  "sp")
			 (V2DF  "dp")
			 (V2DI  "dp")])

;; Map into the register class used
(define_mode_attr VSr	[(V16QI "wi")
			 (V8HI  "wi")
			 (V4SI  "wi")
			 (V4SF  "wf")
			 (V2DI  "wi")
			 (V2DF  "wd")])

;; VSX moves
(define_insn "*vsx_mov<mode>"
  [(set (match_operand:VSX_F 0 "nonimmediate_operand" "=Z,<VSr>,wa,?wa,?Z,*o,*r,*r,<VSr>,v")
	(match_operand:VSX_F 1 "input_operand" "<VSr>,Z,wa,Z,wa,r,o,r,j,W"))]
  "rs6000_vector_info[<MODE>mode].move == VECTOR_VSX
   && (register_operand (operands[0], <MODE>mode) 
       || register_operand (operands[1], <MODE>mode))"
{
  switch (which_alternative)
    {
    case 0: return "stx<VSm>x %x1,%y0";
    case 1: return "lx<VSm>x %x0,%y1";
    case 2: return "xvmov<VSs> %x0,%x1";
    case 3: return "stx<VSm>x %x1,%y0";
    case 4: return "lx<VSm>x %x0,%y1";
    case 5: return "#";
    case 6: return "#";
    case 7: return "#";
    case 8: return "xxlxor %x0,%x0,%x0";
    case 9: return output_vec_const_move (operands);
    default: gcc_unreachable ();
    }
}
  [(set_attr "type" "vecstore,vecload,vecsimple,vecstore,vecload,store,load,*,vecsimple,*")])

;; For integer vectors, favor the altivec registers, since altivec has the integer
;; vector instructions that VSX lacks.
(define_insn "*vsx_mov<mode>"
  [(set (match_operand:VSX_I 0 "nonimmediate_operand" "=Z,v,v,?Z,?f,?f,*o,*r,*r,<VSr>,v")
	(match_operand:VSX_I 1 "input_operand" "v,Z,v,f,Z,fz,r,o,r,j,W"))]
  "rs6000_vector_info[<MODE>mode].move == VECTOR_VSX
   && (register_operand (operands[0], <MODE>mode) 
       || register_operand (operands[1], <MODE>mode))"
{
  switch (which_alternative)
    {
    case  0: return "stx<VSm>x %x1,%y0";
    case  1: return "lx<VSm>x %x0,%y1";
    case  2: return "xvmov<VSs> %x0,%x1";
    case  3: return "stx<VSm>x %x1,%y0";
    case  4: return "lx<VSm>x %x0,%y1";
    case  5: return "xvmov<VSs> %x0,%x1";
    case  6: return "#";
    case  7: return "#";
    case  8: return "#";
    case  9: return "xxlxor %x0,%x0,%x0";
    case 10: return output_vec_const_move (operands);
    default: gcc_unreachable ();
    }
}
  [(set_attr "type" "vecstore,vecload,vecsimple,vecstore,vecload,vecsimple,store,load,*,vecsimple,*")])


;; VSX vector floating point arithmetic instructions
(define_insn "*vsx_add<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (plus:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvadd<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_sub<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (minus:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		     (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvsub<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_mul<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (mult:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvmul<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_div<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (div:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
	 	   (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvdiv<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfdiv")])

(define_insn "*vsx_neg<mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (neg:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvneg<VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_abs<mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (abs:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvabs<VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_nabs<mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (neg:VSX_F
	 (abs:VSX_F
	  (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>"))))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvnabs<VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_smax<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (smax:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvmax<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "*vsx_smin<mode>3"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (smin:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvmin<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "*vsx_sqrt<mode>2"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (sqrt:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvsqrt<VSs> %x0,%x1"
  [(set_attr "type" "vecfdiv")])

;; Fused vector multiply/add instructions
(define_insn "*vsx_fmadd<mode>4"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,<VSr>")
	(plus:VSX_F
	 (mult:VSX_F
	  (match_operand:VSX_F 1 "vsx_register_operand" "%<VSr>,<VSr>")
	  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,0"))
	 (match_operand:VSX_F 3 "vsx_register_operand" "0,<VSr>")))]
  "TARGET_VSX && TARGET_FUSED_MADD"
  "@
   xvmadda<VSs> %x0,%x1,%x2
   xvmaddm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_fmsub<mode>4"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,<VSr>")
	(minus:VSX_F
	 (mult:VSX_F
	  (match_operand:VSX_F 1 "vsx_register_operand" "%<VSr>,<VSr>")
	  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,0"))
	 (match_operand:VSX_F 3 "vsx_register_operand" "0,<VSr>")))]
  "TARGET_VSX && TARGET_FUSED_MADD"
  "@
   xvmsuba<VSs> %x0,%x1,%x2
   xvmsubm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_fmadd<mode>4"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,<VSr>")
	(neg:VSX_F
	 (plus:VSX_F
	  (mult:VSX_F
	   (match_operand:VSX_F 1 "vsx_register_operand" "%<VSr>,<VSr>")
	   (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,0"))
	  (match_operand:VSX_F 3 "vsx_register_operand" "0,<VSr>"))))]
  "TARGET_VSX && TARGET_FUSED_MADD && HONOR_SIGNED_ZEROS (<MODE>mode)"
  "@
   xvnmadda<VSs> %x0,%x1,%x2
   xvnmaddm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "vecfloat")])

(define_insn "*vsx_fmsub<mode>4"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>,<VSr>")
	(neg:VSX_F
	 (minus:VSX_F
	  (mult:VSX_F
	   (match_operand:VSX_F 1 "vsx_register_operand" "%<VSr>,<VSr>")
	   (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>,0"))
	  (match_operand:VSX_F 3 "vsx_register_operand" "0,<VSr>"))))]
  "TARGET_VSX && TARGET_FUSED_MADD && HONOR_SIGNED_ZEROS (<MODE>mode)"
  "@
   xvnmsuba<VSs> %x0,%x1,%x2
   xvnmsubm<VSs> %x0,%x1,%x3"
  [(set_attr "type" "vecfloat")])

;; Vector conditional expressions
(define_insn "*vsx_eq<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(eq:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvcmpeq<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "*vsx_gt<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(gt:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvcmpgt<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "*vsx_ge<mode>"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
	(ge:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		  (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xvcmpge<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "vsx_vsel<mode>"
  [(set (match_operand:VSX_L 0 "register_operand" "=<VSr>")
	(if_then_else:VSX_L (ne (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>")
				(const_int 0))
			    (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>")
			    (match_operand:VSX_L 3 "vsx_register_operand" "<VSr>")))]
  "rs6000_vector_info[<MODE>mode].arith == VECTOR_VSX"
  "xxsel %x0,%x3,%x2,%x1"
  [(set_attr "type" "vecperm")])


;; VSX scalar double precision floating point operations
(define_insn"*vsx_adddf3"
  [(set (match_operand:DF 0 "register_operand" "=ws")
	(plus:DF (match_operand:DF 1 "register_operand" "ws")
		     (match_operand:DF 2 "register_operand" "ws")))]
  "TARGET_VSX && TARGET_VSX_SCALAR_DOUBLE"
  "xsadddp %x0,%x1,%x2"
  [(set_attr "type" "fp")
   (set_attr "fp_type" "fp_addsub_d")])

(define_insn"*vsx_subdf3"
  [(set (match_operand:DF 0 "register_operand" "=ws")
	(minus:DF (match_operand:DF 1 "register_operand" "ws")
		      (match_operand:DF 2 "register_operand" "ws")))]
  "TARGET_VSX && TARGET_VSX_SCALAR_DOUBLE"
  "xssubdp %x0,%x1,%x2"
  [(set_attr "type" "fp")
   (set_attr "fp_type" "fp_addsub_d")])

(define_insn"*vsx_muldf3"
  [(set (match_operand:DF 0 "register_operand" "=ws")
	(mult:DF (match_operand:DF 1 "register_operand" "ws")
		     (match_operand:DF 2 "register_operand" "ws")))]
  "TARGET_VSX && TARGET_VSX_SCALAR_DOUBLE"
  "xsmuldp %x0,%x1,%x2"
  [(set_attr "type" "dmul")
   (set_attr "fp_type" "fp_mul_d")])

(define_insn"*vsx_divdf3"
  [(set (match_operand:DF 0 "register_operand" "=ws")
	(div:DF (match_operand:DF 1 "register_operand" "ws")
		    (match_operand:DF 2 "register_operand" "ws")))]
  "TARGET_VSX && TARGET_VSX_SCALAR_DOUBLE"
  "xsdivdp %x0,%x1,%x2"
  [(set_attr "type" "ddiv")])

(define_insn "*vsx_sqrtdf2"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
        (sqrt:DF (match_operand:DF 1 "vsx_register_operand" "ws")))]
  "TARGET_VSX && TARGET_VSX_SCALAR_DOUBLE"
  "xssqrtdp %x0,%x1"
  [(set_attr "type" "dsqrt")])

(define_insn"*vsx_negdf2"
  [(set (match_operand:DF 0 "register_operand" "=ws")
	(neg:DF (match_operand:DF 1 "register_operand" "ws")))]
  "TARGET_VSX && TARGET_VSX_SCALAR_DOUBLE"
  "xsnegdp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn"vsx_absdf2"
  [(set (match_operand:DF 0 "register_operand" "=ws")
	(abs:DF (match_operand:DF 1 "register_operand" "ws")))]
  "TARGET_VSX && TARGET_VSX_SCALAR_DOUBLE"
  "xsabsdp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn"*vsx_nabsdf2"
  [(set (match_operand:DF 0 "register_operand" "=ws")
	(neg:DF (abs:DF (match_operand:DF 1 "register_operand" "ws"))))]
  "TARGET_VSX && TARGET_VSX_SCALAR_DOUBLE"
  "xsnabsdp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*vsx_smaxdf3"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
        (smax:DF (match_operand:DF 1 "vsx_register_operand" "ws")
		 (match_operand:DF 2 "vsx_register_operand" "ws")))]
  "TARGET_VSX && TARGET_VSX_SCALAR_DOUBLE"
  "xsmaxdp %x0,%x1,%x2"
  [(set_attr "type" "fp")])


(define_insn "*vsx_smindf3"
  [(set (match_operand:DF 0 "vsx_register_operand" "=ws")
        (smin:DF (match_operand:DF 1 "vsx_register_operand" "ws")
		 (match_operand:DF 2 "vsx_register_operand" "ws")))]
  "TARGET_VSX && TARGET_VSX_SCALAR_DOUBLE"
  "xsmindp %x0,%x1,%x2"
  [(set_attr "type" "fp")])

;; Fused vector multiply/add instructions
(define_insn "*vsx_fmadddf4"
  [(set (match_operand:DF 0 "vsx_register_operand" "=f,f")
	(plus:DF
	 (mult:DF
	  (match_operand:DF 1 "vsx_register_operand" "%f,f")
	  (match_operand:DF 2 "vsx_register_operand" "f,0"))
	 (match_operand:DF 3 "vsx_register_operand" "0,f")))]
  "TARGET_VSX && TARGET_FUSED_MADD"
  "@
   xsmaddadp %x0,%x1,%x2
   xsmaddmdp %x0,%x1,%x3"
  [(set_attr "type" "fp")
   (set_attr "fp_type" "fp_maddsub_d")])

(define_insn "*vsx_fmsubdf4"
  [(set (match_operand:DF 0 "vsx_register_operand" "=f,f")
	(minus:DF
	 (mult:DF
	  (match_operand:DF 1 "vsx_register_operand" "%f,f")
	  (match_operand:DF 2 "vsx_register_operand" "f,0"))
	 (match_operand:DF 3 "vsx_register_operand" "0,f")))]
  "TARGET_VSX && TARGET_FUSED_MADD"
  "@
   xsmsubadp %x0,%x1,%x2
   xsmsubmdp %x0,%x1,%x3"
  [(set_attr "type" "fp")
   (set_attr "fp_type" "fp_maddsub_d")])

(define_insn "*vsx_fnmadddf4"
  [(set (match_operand:DF 0 "vsx_register_operand" "=f,f")
	(neg:DF
	 (plus:DF
	  (mult:DF
	   (match_operand:DF 1 "vsx_register_operand" "%f,f")
	   (match_operand:DF 2 "vsx_register_operand" "f,0"))
	  (match_operand:DF 3 "vsx_register_operand" "0,f"))))]
  "TARGET_VSX && TARGET_FUSED_MADD && HONOR_SIGNED_ZEROS (DFmode)"
  "@
   xsnmaddadp %x0,%x1,%x2
   xsnmaddmdp %x0,%x1,%x3"
  [(set_attr "type" "fp")
   (set_attr "fp_type" "fp_maddsub_d")])

(define_insn "*vsx_fnmsubdf4"
  [(set (match_operand:DF 0 "vsx_register_operand" "=f,f")
	(neg:DF
	 (minus:DF
	  (mult:DF
	   (match_operand:DF 1 "vsx_register_operand" "%f,f")
	   (match_operand:DF 2 "vsx_register_operand" "f,0"))
	  (match_operand:DF 3 "vsx_register_operand" "0,f"))))]
  "TARGET_VSX && TARGET_FUSED_MADD && HONOR_SIGNED_ZEROS (DFmode)"
  "@
   xsnmsubadp %x0,%x1,%x2
   xsnmsubmdp %x0,%x1,%x3"
  [(set_attr "type" "fp")
   (set_attr "fp_type" "fp_maddsub_d")])


;; Logical and permute operations
(define_insn "*vsx_and<mode>3"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=wl")
        (and:VSX_L
	 (match_operand:VSX_L 1 "vsx_register_operand" "wl")
	 (match_operand:VSX_L 2 "vsx_register_operand" "wl")))]
  "rs6000_vector_info[<MODE>mode].logical == VECTOR_VSX"
  "xxland %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*vsx_ior<mode>3"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=wl")
        (ior:VSX_L (match_operand:VSX_L 1 "vsx_register_operand" "wl")
		   (match_operand:VSX_L 2 "vsx_register_operand" "wl")))]
  "rs6000_vector_info[<MODE>mode].logical == VECTOR_VSX"
  "xxlor %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*vsx_xor<mode>3"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=wl")
        (xor:VSX_L
	 (match_operand:VSX_L 1 "vsx_register_operand" "wl")
	 (match_operand:VSX_L 2 "vsx_register_operand" "wl")))]
  "rs6000_vector_info[<MODE>mode].logical == VECTOR_VSX"
  "xxlxor %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*vsx_one_cmpl<mode>2"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=wl")
        (not:VSX_L
	 (match_operand:VSX_L 1 "vsx_register_operand" "wl")))]
  "rs6000_vector_info[<MODE>mode].logical == VECTOR_VSX"
  "xxlnor %x0,%x1,%x1"
  [(set_attr "type" "vecsimple")])
  
(define_insn "*vsx_nor<mode>3"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=wl")
        (not:VSX_L
	 (ior:VSX_L
	  (match_operand:VSX_L 1 "vsx_register_operand" "wl")
	  (match_operand:VSX_L 2 "vsx_register_operand" "wl"))))]
  "rs6000_vector_info[<MODE>mode].logical == VECTOR_VSX"
  "xxlnor %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*vsx_andc<mode>3"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=wl")
        (and:VSX_L
	 (not:VSX_L
	  (match_operand:VSX_L 2 "vsx_register_operand" "wl"))
	 (match_operand:VSX_L 1 "vsx_register_operand" "wl")))]
  "rs6000_vector_info[<MODE>mode].logical == VECTOR_VSX"
  "xxlandc %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

;; Convert two doubles into one V2DF
(define_insn "vsx_concat_v2df"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd")
	(vec_concat:V2DF
	 (match_operand:DF 1 "vsx_register_operand" "wd")
	 (match_operand:DF 2 "vsx_register_operand" "wd")))]
  "rs6000_vector_info[V2DFmode].logical == VECTOR_VSX"
  "xxpermdi %x0,%x1,%x2,0"
  [(set_attr "type" "vecperm")])

;; Set a double into one element
(define_insn "vsx_set_v2df"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd")
	(vec_merge:V2DF
	 (match_operand:V2DF 1 "vsx_register_operand" "wd")
	 (vec_duplicate:V2DF (match_operand:DF 2 "vsx_register_operand" "wd"))
	 (match_operand:QI 3 "u5bit_cint_operand" "i")))]
  "rs6000_vector_info[V2DFmode].logical == VECTOR_VSX"
{
  operands[3] = GEN_INT (INTVAL (operands[3]) & 1);
  return \"xxpermdi %x0,%x1,%x2,%3\";
}
  [(set_attr "type" "vecperm")])

;; Extract an element
(define_insn "vsx_extract_v2df"
  [(set (match_operand:DF 0 "vsx_register_operand" "=wd")
	(vec_select:DF (match_operand:V2DF 1 "vsx_register_operand" "wd")
		       (parallel
			[(match_operand:QI 2 "u5bit_cint_operand" "i")])))]
  "rs6000_vector_info[V2DFmode].logical == VECTOR_VSX"
{
  operands[3] = GEN_INT (INTVAL (operands[2]) & 1);
  return \"xxpermdi %x0,%x1,%x1,%3\";
}
  [(set_attr "type" "vecperm")])

;; General permute
(define_insn "vsx_xxpermdi"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd")
	(vec_concat:V2DF
	 (vec_select:DF (match_operand:V2DF 1 "vsx_register_operand" "wd")
			(parallel
			 [(match_operand:QI 2 "u5bit_cint_operand" "i")]))
	 (vec_select:DF (match_operand:V2DF 3 "vsx_register_operand" "wd")
			(parallel
			 [(match_operand:QI 4 "u5bit_cint_operand" "i")]))))]
  "rs6000_vector_info[V2DFmode].logical == VECTOR_VSX"
{
  operands[5] = GEN_INT (((INTVAL (operands[2]) & 1) << 1)
			 | (INTVAL (operands[4]) & 1));
  return \"xxpermdi %x0,%x1,%x3,%5\";
}
  [(set_attr "type" "vecperm")])

;; Double splat
(define_insn "vsx_splatv2df"
  [(set (match_operand:V2DF 0 "vsx_register_operand" "=wd,wd")
	(vec_duplicate:V2DF
	 (match_operand:DF 1 "input_operand" "wd,Z")))]
  "rs6000_vector_info[V2DFmode].logical == VECTOR_VSX"
  "@
   xxpermdi %x0,%x1,%x1,0
   lxvdsx %x0,%y1"
  [(set_attr "type" "vecperm,vecload")])

;; V4SF interleave
(define_insn "vsx_xxmrglw"
  [(set (match_operand:V4SF 0 "vsx_register_operand" "=wf")
        (vec_merge:V4SF (vec_select:V4SF (match_operand:V4SF 1 "vsx_register_operand" "wf")
					 (parallel [(const_int 2)
					 	    (const_int 0)
						    (const_int 3)
						    (const_int 1)]))
                        (vec_select:V4SF (match_operand:V4SI 2 "vsx_register_operand" "wf")
					 (parallel [(const_int 0)
					 	    (const_int 2)
						    (const_int 1)
						    (const_int 3)]))
			(const_int 5)))]
  "rs6000_vector_info[V4SFmode].logical == VECTOR_VSX"
  "xxmrglw %x0,%x1,%x2"
  [(set_attr "type" "vecperm")])

(define_insn "vsx_xxmrghw"
  [(set (match_operand:V4SF 0 "vsx_register_operand" "=wf")
        (vec_merge:V4SF (vec_select:V4SF (match_operand:V4SF 1 "vsx_register_operand" "wf")
                                         (parallel [(const_int 0)
                                                    (const_int 2)
                                                    (const_int 1)
                                                    (const_int 3)]))
                        (vec_select:V4SF (match_operand:V4SF 2 "vsx_register_operand" "wf")
                                         (parallel [(const_int 2)
                                                    (const_int 0)
                                                    (const_int 3)
                                                    (const_int 1)]))
			(const_int 5)))]
  "rs6000_vector_info[V4SFmode].logical == VECTOR_VSX"
  "xxmrghw %x0,%x1,%x2"
  [(set_attr "type" "vecperm")])
