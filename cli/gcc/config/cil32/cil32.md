;; GCC machine description for cil32.
;;
;;    Copyright (C) 2006 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify it under
;; the terms of the GNU General Public License as published by the Free
;; Software Foundation; either version 2, or (at your option) any later
;; version.
;;
;; GCC is distributed in the hope that it will be useful, but WITHOUT ANY
;; WARRANTY; without even the implied warranty of MERCHANTABILITY or
;; FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
;; for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING.  If not, write to the Free
;; Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
;; 02110-1301, USA.
;;
;; Authors:
;;    Andrea Bona
;;    Andrea Ornstein
;;    Erven Rohou
;;    Roberto Costa
;;
;; Contact information at STMicroelectronics:
;; Roberto Costa <roberto.costa@st.com>

;; See files "md.texi" and "rtl.def" for documentation on define_insn,
;; match_*, et. al.

;; Register numbers.
(define_constants
  [(CIL32_FAKE_REG 0)]
)


(define_mode_macro ALLMODES [SI HI QI DI SF DF])
(define_mode_macro ALLEVALMODES [SI DI SF DF])
(define_mode_macro ALLINTEVALMODES [SI DI])

(define_mode_macro VECMODES [V8QI V4HI V2SI V4QI V2HI])


;; Jump and branch insns.

(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  "1"
  ""
  [])

(define_insn "indirect_jump"
  [(set (pc) (match_operand:SI 0 "nonimmediate_operand" "rm"))]
  ""
  "")

;; Move insns.

(define_insn "mov<mode>"
  [(set (match_operand:ALLMODES 0 "nonimmediate_operand" "")
	(match_operand:ALLMODES 1 "general_operand" ""))]
  ""
  "")

;; Move for vectors
(define_insn "mov<mode>"
  [(set (match_operand:VECMODES 0 "nonimmediate_operand" "")
	(match_operand:VECMODES 1 "general_operand" ""))]
  ""
  "")


;; Add insns.

(define_insn "add<mode>3"
  [(set (match_operand:ALLEVALMODES 0 "nonimmediate_operand"  "")
	(plus:ALLEVALMODES
	 (match_operand:ALLEVALMODES 1 "general_operand" "")
	 (match_operand:ALLEVALMODES 2 "general_operand" "")))]
  ""
  "add")

(define_insn "sub<mode>3"
  [(set (match_operand:ALLEVALMODES 0 "nonimmediate_operand"  "")
	(minus:ALLEVALMODES
	 (match_operand:ALLEVALMODES 1 "general_operand" "")
	 (match_operand:ALLEVALMODES 2 "general_operand" "")))]
  ""
  "sub")

(define_insn "mul<mode>3"
  [(set (match_operand:ALLEVALMODES 0 "nonimmediate_operand"  "")
	(mult:ALLEVALMODES
	 (match_operand:ALLEVALMODES 1 "general_operand" "")
	 (match_operand:ALLEVALMODES 2 "general_operand" "")))]
  ""
  "mul")

(define_insn "div<mode>3"
  [(set (match_operand:ALLEVALMODES 0 "nonimmediate_operand"  "")
	(div:ALLEVALMODES
	 (match_operand:ALLEVALMODES 1 "general_operand" "")
	 (match_operand:ALLEVALMODES 2 "general_operand" "")))]
  ""
  "div")

(define_insn "udiv<mode>3"
  [(set (match_operand:ALLEVALMODES 0 "nonimmediate_operand"  "")
	(udiv:ALLEVALMODES
	 (match_operand:ALLEVALMODES 1 "general_operand" "")
	 (match_operand:ALLEVALMODES 2 "general_operand" "")))]
  ""
  "div.un")

(define_insn "mod<mode>3"
  [(set (match_operand:ALLEVALMODES 0 "nonimmediate_operand"  "")
	(mod:ALLEVALMODES
	 (match_operand:ALLEVALMODES 1 "general_operand" "")
	 (match_operand:ALLEVALMODES 2 "general_operand" "")))]
  ""
  "rem")

(define_insn "umod<mode>3"
  [(set (match_operand:ALLEVALMODES 0 "nonimmediate_operand"  "")
	(umod:ALLEVALMODES
	 (match_operand:ALLEVALMODES 1 "general_operand" "")
	 (match_operand:ALLEVALMODES 2 "general_operand" "")))]
  ""
  "rem.un")

(define_insn "and<mode>3"
  [(set (match_operand:ALLINTEVALMODES 0 "nonimmediate_operand"  "")
	(and:ALLINTEVALMODES
	 (match_operand:ALLINTEVALMODES 1 "general_operand" "")
	 (match_operand:ALLINTEVALMODES 2 "general_operand" "")))]
  ""
  "and")

;; and works in SIMD mode
(define_insn "and<mode>3"
  [(set (match_operand:VECMODES 0 "nonimmediate_operand"  "")
	(and:VECMODES
	 (match_operand:VECMODES 1 "general_operand" "")
	 (match_operand:VECMODES 2 "general_operand" "")))]
  ""
  "and")

(define_insn "ior<mode>3"
  [(set (match_operand:ALLINTEVALMODES 0 "nonimmediate_operand"  "")
	(ior:ALLINTEVALMODES
	 (match_operand:ALLINTEVALMODES 1 "general_operand" "")
	 (match_operand:ALLINTEVALMODES 2 "general_operand" "")))]
  ""
  "or")

;; or works in SIMD mode
(define_insn "ior<mode>3"
  [(set (match_operand:VECMODES 0 "nonimmediate_operand"  "")
	(ior:VECMODES
	 (match_operand:VECMODES 1 "general_operand" "")
	 (match_operand:VECMODES 2 "general_operand" "")))]
  ""
  "or")

(define_insn "xor<mode>3"
  [(set (match_operand:ALLINTEVALMODES 0 "nonimmediate_operand"  "")
	(xor:ALLINTEVALMODES
	 (match_operand:ALLINTEVALMODES 1 "general_operand" "")
	 (match_operand:ALLINTEVALMODES 2 "general_operand" "")))]
  ""
  "xor")

;; xor works in SIMD mode
(define_insn "xor<mode>3"
  [(set (match_operand:VECMODES 0 "nonimmediate_operand"  "")
	(xor:VECMODES
	 (match_operand:VECMODES 1 "general_operand" "")
	 (match_operand:VECMODES 2 "general_operand" "")))]
  ""
  "xor")


;; Call insns.

(define_insn "call"
  [(call (match_operand 0 "general_operand" "")
         (match_operand 1 "general_operand" ""))]
  ""
  "")

(define_insn "call_value"
  [(set (match_operand 0 "" "")
        (call (match_operand 1 "general_operand" "")
              (match_operand 2 "general_operand" "")))]
  ""
  "")

;; Local variables:
;; mode:emacs-lisp
;; comment-start: ";; "
;; eval: (set-syntax-table (copy-sequence (syntax-table)))
;; eval: (modify-syntax-entry ?[ "(]")
;; eval: (modify-syntax-entry ?] ")[")
;; eval: (modify-syntax-entry ?{ "(}")
;; eval: (modify-syntax-entry ?} "){")
;; eval: (setq indent-tabs-mode t)
;; End:
