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

(define_insn "move_balc_call_value"
  [(parallel [(set (match_operand 4 "" "")
		   (call (mem:SI (match_operand 2 "" ""))
			 (match_operand 3 "" "")))
	      (set (match_operand 0 "register_operand" "")
		   (match_operand 1 "register_operand" ""))
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
		   (match_operand 1 "register_operand" ""))
	      (use (match_dup 0))
	      (clobber (reg:SI RETURN_ADDR_REGNUM))])]
  "TARGET_NANOMIPS
   && nanomips_move_balc_p (operands)"
  { return mips_output_jump (operands, 2, -1, true, true); }
  [(set_attr "jal" "direct")])
