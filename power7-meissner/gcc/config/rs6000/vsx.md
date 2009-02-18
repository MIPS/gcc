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

;; Iterator for logical types supported by VSX
(define_mode_iterator VSX_L [V4SI V8HI V16QI V4SF V2DF V2DI])

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

;; Map into the register class used for the move instructions
(define_mode_attr VSr	[(V16QI "v")
			 (V8HI  "v")
			 (V4SI  "v")
			 (V4SF  "w")
			 (V2DI  "v")
			 (V2DF  "w")])

;; Generic LX load instruction.
(define_insn "*lx_<mode>_vsx"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>")
	(match_operand:VSX_L 1 "memory_operand" "Z"))]
  "TARGET_VSX"
  "lx<VSm>x %x0,%y1"
  [(set_attr "type" "vecload")])

;; Generic STVX store instruction.
(define_insn "*stx_<mode>_vsx"
  [(set (match_operand:VSX_L 0 "memory_operand" "=Z")
	(match_operand:VSX_L 1 "vsx_register_operand" "<VSr>"))]
  "TARGET_VSX"
  "stx<VSm>x %x1,%y0"
  [(set_attr "type" "vecstore")])

;; VSX moves
(define_insn "*mov<mode>_vsx"
  [(set (match_operand:VSX_L 0 "nonimmediate_operand" "=Z,<VSr>,<VSr>,o,r,r,<VSr>,v")
	(match_operand:VSX_L 1 "input_operand" "<VSr>,Z,<VSr>,r,o,r,j,W"))]
  "TARGET_VSX
   && (register_operand (operands[0], <MODE>mode) 
       || register_operand (operands[1], <MODE>mode))"
{
  switch (which_alternative)
    {
    case 0: return "stx<VSm>x %x1,%y0";
    case 1: return "lx<VSm>x %x0,%y1";
    case 2: return "xvmov<VSs> %x0,%x1";
    case 3: return "#";
    case 4: return "#";
    case 5: return "#";
    case 6: return "xxlxor %x0,%x0,%x0";
    case 7: return output_vec_const_move (operands);
    default: gcc_unreachable ();
    }
}
  [(set_attr "type" "vecstore,vecload,vecsimple,store,load,*,vecsimple,*")])


;; VSX vector floating point arithmetic instructions
(define_insn "*add<mode>3_vsx"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (plus:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xvadd<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfloat")])

(define_insn "*sub<mode>3_vsx"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (minus:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		     (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xvsub<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfloat")])

(define_insn "*mul<mode>3_vsx"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (mult:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xvmul<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfloat")])

(define_insn "*div<mode>3_vsx"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (div:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
	 	   (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xvdiv<VSs> %x0,%x1,%x2"
  [(set_attr "type" "vecfdiv")])

(define_insn "*neg<mode>2_vsx"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (neg:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xvneg<VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*abs<mode>2_vsx"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (abs:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xvabs<VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*nabs<mode>2_vsx"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (neg:VSX_F
	 (abs:VSX_F
	  (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>"))))]
  "TARGET_VSX"
  "xvnabs<VSs> %x0,%x1"
  [(set_attr "type" "vecfloat")])

(define_insn "*smax<mode>3_vsx"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (smax:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xvmax<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "*smin<mode>3_vsx"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (smin:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")
		    (match_operand:VSX_F 2 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xvmin<VSs> %x0,%x1,%x2"
  [(set_attr "type" "veccmp")])

(define_insn "*sqrt<mode>2_vsx"
  [(set (match_operand:VSX_F 0 "vsx_register_operand" "=<VSr>")
        (sqrt:VSX_F (match_operand:VSX_F 1 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xvsqrt<VSs> %x0,%x1"
  [(set_attr "type" "vecfdiv")])

;; Fused vector multiply/add instructions
(define_insn "*fmadd<type>4_vsx"
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

(define_insn "*fmsub<type>4_vsx"
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

(define_insn "*fmadd<type>4_vsx"
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

(define_insn "*fmsub<type>4_vsx"
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


;; VSX scalar double precision floating point operations
(define_insn"*adddf3_vsx"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(plus:DF (match_operand:DF 1 "register_operand" "f")
		     (match_operand:DF 2 "register_operand" "f")))]
  "TARGET_VSX"
  "xsadddp %x0,%x1,%x2"
  [(set_attr "type" "fp")
   (set_attr "fp_type" "fp_addsub_d")])

(define_insn"*subdf3_vsx"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(minus:DF (match_operand:DF 1 "register_operand" "f")
		      (match_operand:DF 2 "register_operand" "f")))]
  "TARGET_VSX"
  "xssubdp %x0,%x1,%x2"
  [(set_attr "type" "fp")
   (set_attr "fp_type" "fp_addsub_d")])

(define_insn"*muldf3_vsx"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(mult:DF (match_operand:DF 1 "register_operand" "f")
		     (match_operand:DF 2 "register_operand" "f")))]
  "TARGET_VSX"
  "xsmuldp %x0,%x1,%x2"
  [(set_attr "type" "dmul")
   (set_attr "fp_type" "fp_mul_d")])

(define_insn"*divdf3_vsx"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(div:DF (match_operand:DF 1 "register_operand" "f")
		    (match_operand:DF 2 "register_operand" "f")))]
  "TARGET_VSX"
  "xsdivdp %x0,%x1,%x2"
  [(set_attr "type" "ddiv")])

(define_insn"*negdf2_vsx"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(neg:DF (match_operand:DF 1 "register_operand" "f")))]
  "TARGET_VSX"
  "xsnegdp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn"*absdf2_vsx"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(abs:DF (match_operand:DF 1 "register_operand" "f")))]
  "TARGET_VSX"
  "xsabsdp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn"*nabsdf2_vsx"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(neg:DF (abs:DF (match_operand:DF 1 "register_operand" "f"))))]
  "TARGET_VSX"
  "xsnabsdp %x0,%x1"
  [(set_attr "type" "fp")])

(define_insn "*smaxdf3_vsx"
  [(set (match_operand:DF 0 "vsx_register_operand" "=f")
        (smax:DF (match_operand:DF 1 "vsx_register_operand" "f")
		 (match_operand:DF 2 "vsx_register_operand" "f")))]
  "TARGET_VSX"
  "xsmaxdp %x0,%x1,%x2"
  [(set_attr "type" "fp")])


(define_insn "*smindf3_vsx"
  [(set (match_operand:DF 0 "vsx_register_operand" "=f")
        (smin:DF (match_operand:DF 1 "vsx_register_operand" "f")
		 (match_operand:DF 2 "vsx_register_operand" "f")))]
  "TARGET_VSX"
  "xsmindp %x0,%x1,%x2"
  [(set_attr "type" "fp")])

;; Fused vector multiply/add instructions
(define_insn "*fmadddf4_vsx"
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

(define_insn "*fmsubdf4_vsx"
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

(define_insn "*fnmadddf4_vsx"
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

(define_insn "*fnmsubdf4_vsx"
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


;; Logical operations
(define_insn "*and<mode>3_vsx"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>")
        (and:VSX_L
	 (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>")
	 (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xxland %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*ior<mode>3_vsx"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>")
        (ior:VSX_L (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>")
		   (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xxlor %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*xor<mode>3_vsx"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>")
        (xor:VSX_L
	 (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>")
	 (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xxlxor %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*one_cmpl<mode>2_vsx"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>")
        (not:VSX_L
	 (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xxlnor %x0,%x1,%x1"
  [(set_attr "type" "vecsimple")])
  
(define_insn "*nor<mode>3_vsx"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>")
        (not:VSX_L
	 (ior:VSX_L
	  (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>")
	  (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>"))))]
  "TARGET_VSX"
  "xxlnor %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

(define_insn "*andc<mode>3_vsx"
  [(set (match_operand:VSX_L 0 "vsx_register_operand" "=<VSr>")
        (and:VSX_L
	 (not:VSX_L
	  (match_operand:VSX_L 2 "vsx_register_operand" "<VSr>"))
	 (match_operand:VSX_L 1 "vsx_register_operand" "<VSr>")))]
  "TARGET_VSX"
  "xxlandc %x0,%x1,%x2"
  [(set_attr "type" "vecsimple")])

