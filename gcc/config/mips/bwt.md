;; Copyright (C) 2014 BluWireless Technologies Ltd
;; Copyright (C) 2011 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.
;;
;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; Describe the resources.

;; ---------  HERE -------------
;; ?? inclusion of bwt.md implies a machine type of bwt, doesnt it ?
;; ?? ... or we somehow have to qualify the instructions to m14k at least
;; ?? ... or something like the #define we setup ?
;; ?? what effect does udi have on condition codes ?
;; ---------  HERE -------------

;;; Automaton for udi instructions.
;(define_automaton "bwtudi")
;
;(define_cpu_unit "udi" "bwtudi")
;
;(define_insn_reservation "udi_push" 1
;  (eq_attr "type" "mtc")
;  "udi")
;
;(define_insn_reservation "udi_pop" 10
;  (eq_attr "type" "mfc")
;  "udi")
;

;(define_constraint "qbwt_5bit"
;  "@internal
;   A bwt udi instruction unsigned constant field of 5 bits."
;  (match_operand 0 "bwt_5_operand"))
;
;(define_constraint "qbwt_is_0"
;  "@internal
;   A bwt udi instruction unsigned constant field with value 0"
;  (match_operand 0 "bwt_is_0_operand"))
;
;(define_predicate "bwt_5_operand"
;  (and      (match_code "const_int")
;      (match_test "mips_unsigned_immediate_p (INTVAL(op), 5, 0)")))
;
;(define_predicate "bwt_is_0_operand"
;  (and      (match_code "const_int")
;      (match_test "INTVAL(op) == 0")))
;

(define_c_enum "unspec" [
  UNSPEC_BWT_WRITE_REG_I
  UNSPEC_BWT_WRITE_REG_GPR
  UNSPEC_BWT_WRITE_REG_VAR

  UNSPEC_BWT_READ_REG_I
  UNSPEC_BWT_READ_REG_GPR
  UNSPEC_BWT_READ_REG_VAR

  UNSPEC_BWT_POP_RESULT
  UNSPEC_BWT_DROP_RESULT

  UNSPEC_BWT_MC_ADDR_Z
  UNSPEC_BWT_MC_ADDR_GPR
  UNSPEC_BWT_MC_ADDR

  UNSPEC_BWT_VN_IO_Z
  UNSPEC_BWT_VN_IO_GPR
  UNSPEC_BWT_VN_IO_VAR

  UNSPEC_BWT_TRANSFER_INST

  UNSPEC_BWT_LOAD_MC_Z
  UNSPEC_BWT_LOAD_MC_GPR
  UNSPEC_BWT_LOAD_MC

  UNSPEC_BWT_SELECT_C
  UNSPEC_BWT_SELECT_GPR
  UNSPEC_BWT_SELECT_VAR

  UNSPEC_BWT_SEMSET_C
  UNSPEC_BWT_SEMSET_GPR
  UNSPEC_BWT_SEMSET_VAR

  UNSPEC_BWT_PORT_IO_S
  UNSPEC_BWT_PORT_IO_GPR
  UNSPEC_BWT_PORT_IO_VAR

  UNSPEC_BWT_MC_MAIN_Z
  UNSPEC_BWT_MC_MAIN_GPR
  UNSPEC_BWT_MC_MAIN

  UNSPEC_BWT_FFT
])

;; TARGET_BWT_INTRINSICS ...

(define_insn "bwt_write_reg_i"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "")
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "const_int_operand" "") 
                     (match_operand 5 "const_int_operand" "")] 
                                    UNSPEC_BWT_WRITE_REG_I)]
  ""
       { 
            return " # bwt_write_reg_i(%0,%1,%2,%3,%4,%5)\n"
                  _BWT_ASM_WRITE_REG(%0, %1, %2, %3, %4, %5) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_write_reg_gpr"
  [(unspec_volatile [(match_operand:SI 0 "const_int_operand" "") 
            (match_operand:SI 1 "const_int_operand" "") 
            (match_operand:SI 2 "const_int_operand" "") 
       (use (match_operand:SI 3 "register_operand" "r"))
            (match_operand:SI 4 "const_int_operand" "") 
            (match_operand:SI 5 "const_int_operand" "")]
                              UNSPEC_BWT_WRITE_REG_GPR)]
  ""
       { 
            return " # bwt_write_reg_var(%0,%1,%2,%3,%4,%5)\n"
                  _BWT_ASM_WRITE_REG(%0, %1, %2, %r3, %4, %5) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)


;[(unspec_volatile [(match_operand 0 "const_int_operand" "qbwt_5bit,qbwt_5bit") 
;                   (match_operand 1 "const_int_operand" "qbwt_5bit,qbwt_5bit") 
;                   (match_operand 2 "const_int_operand" "qbwt_5bit,qbwt_is_1") 
;                   (match_operand 3 "nonmemory_operand"   "r, qbwt_5bit") 
;                   (match_operand 4 "const_int_operand" "qbwt_5bit,qbwt_is_0") 
;                   (match_operand 5 "const_int_operand" "qbwt_5bit,qbwt_is_0")]

(define_expand "bwt_write_reg_var"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "nonmemory_operand" "")
                     (match_operand 4 "const_int_operand" "") 
                     (match_operand 5 "const_int_operand" "")]
                              UNSPEC_BWT_WRITE_REG_VAR)]
  ""
      {
            int done = 0 ;
            int op3_is_const = CONSTANT_P(operands[3]) ;

#if 0 /* use unspec_volatile */
            emit_insn(gen_blockage());
#endif
            if (op3_is_const && mips_unsigned_immediate_p(UINTVAL(operands[3]), 12, 0)) {
                  int op2_is_1 = (INTVAL(operands[2]) == 1) ? 1 : 0 ;
                  int op4_is_0 = (INTVAL(operands[4]) == 0) ? 1 : 0 ;
                  int op5_is_0 = (INTVAL(operands[5]) == 0) ? 1 : 0 ;
                  if (op2_is_1 && op4_is_0 && op5_is_0) {
                              // save the register load and embed the constant..
                        done = 1 ;
                        emit_insn(gen_bwt_write_reg_i(operands[0], operands[1],
                                        const0_rtx, const0_rtx, const0_rtx, operands[3]));
                  }
            }
            
            if (!done) {
                  if (op3_is_const) {
                        operands[3] = force_reg( SImode, operands[3]);
                  }
                  
                  emit_insn(gen_bwt_write_reg_gpr(operands[0],operands[1],
                               operands[2], operands[3], operands[4], operands[5]));
            }
            DONE ;
      }
)


(define_insn "bwt_read_reg_i"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "")
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "const_int_operand" "")] 
                                    UNSPEC_BWT_READ_REG_I)]
  ""
       { 
            return " # bwt_read_reg_i(%0,%1,%2,%3,%4)\n"
                  _BWT_ASM_READ_REG(%0, %1, %2, %3, %4) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_read_reg_gpr"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
            (match_operand 1 "const_int_operand" "")
            (match_operand 2 "const_int_operand" "")
            (match_operand 3 "const_int_operand" "")
       (use (match_operand 4 "register_operand" "r"))]
                              UNSPEC_BWT_READ_REG_GPR)]
  ""
       { 
            return " # bwt_read_reg_var(%0,%1,%2,%3,%4)\n"
                  _BWT_ASM_READ_REG(%0, %1, %2, %3, %r4) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)


(define_expand "bwt_read_reg_var"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "")
                     (match_operand 2 "const_int_operand" "")
                     (match_operand 3 "const_int_operand" "")
                     (match_operand 4 "nonmemory_operand" "")]
                              UNSPEC_BWT_READ_REG_VAR)]
  ""
      {
            int done = 0 ;
            int op4_is_const = CONSTANT_P(operands[4]) ;

#if 0 /* use unspec_volatile */
            emit_insn(gen_blockage());
#endif
            if (op4_is_const && mips_unsigned_immediate_p(UINTVAL(operands[4]), 7, 0)) {
                  int op3_is_1 = (INTVAL(operands[3]) == 1) ? 1 : 0 ;
                  int op1_is_0 = (INTVAL(operands[1]) == 0) ? 1 : 0 ;

                  if (op3_is_1 && op1_is_0) {
                              // save the register load and embed the constant..
                        done = 1 ;
                        emit_insn(gen_bwt_read_reg_i(operands[0], operands[4],
                                        operands[2], const0_rtx, const0_rtx));
                  }
            }
            
            if (!done) {
                  if (op4_is_const) {
                        operands[4] = force_reg( SImode, operands[4]);
                  }

                  emit_insn(gen_bwt_read_reg_gpr(operands[0],operands[1],
                                 operands[2], operands[3], operands[4]));
            }
            DONE ;
      }
)


(define_insn "bwt_pop_result"
   [(set (match_operand:SI 0 "register_operand" "=r")
      (unspec_volatile :SI [
         (match_operand 1 "const_int_operand" "")
         (match_operand 2 "const_int_operand" "")
         (match_operand 3 "const_int_operand" "")
         (match_operand 4 "const_int_operand" "")] UNSPEC_BWT_POP_RESULT)
    )]
  ""
       { 
            if (TARGET_BWT_POPFIX)
                  return  "nop\n "
                        " # bwt_pop_result(%0,%1,%2,%3,%4)\n"
                        _BWT_ASM_POP_RESULT(%r0, %1, %2, %3, %4) ;
            else
                  return  " # bwt_pop_result(%0,%1,%2,%3,%4)\n"
                        _BWT_ASM_POP_RESULT(%r0, %1, %2, %3, %4) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mfc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_drop_result"
  [(unspec_volatile [
                     (match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "")] UNSPEC_BWT_DROP_RESULT)]
  ""
       { 
            return " # bwt_drop_result(0, %0,%1,%2,%3)\n"
                  _BWT_ASM_POP_RESULT(0, %0, %1, %2, %3) ; 
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mfc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_mc_addr_z"
  [(unspec_volatile [
                     (match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "const_int_operand" "") 
                     (match_operand 5 "const_int_operand" "")] UNSPEC_BWT_MC_ADDR_Z)]
  ""
       { 
            return " # bwt_mc_addr(%0,%1,%2,%3,%4,%5)\n"
                  _BWT_ASM_MC_ADDR(%0, %1, %2, %3, %4, %5) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)


(define_insn "bwt_mc_addr_gpr"
  [(unspec_volatile [ (match_operand 0 "const_int_operand" "") 
        (use (match_operand 1 "register_operand" "r")) 
             (match_operand 2 "const_int_operand" "") 
             (match_operand 3 "const_int_operand" "") 
             (match_operand 4 "const_int_operand" "") 
             (match_operand 5 "const_int_operand" "")] UNSPEC_BWT_MC_ADDR_GPR)]
  ""
       { 
            return " # bwt_mc_addr(%0,%1,%2,%3,%4,%5)\n"
                  _BWT_ASM_MC_ADDR(%0, %r1, %2, %3, %4, %5) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_expand "bwt_mc_addr"
  [(unspec_volatile [
                     (match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "nonmemory_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "const_int_operand" "") 
                     (match_operand 5 "const_int_operand" "")] UNSPEC_BWT_MC_ADDR)]
  ""
       { 
            int done = 0 ;
#if 0 /* use unspec_volatile */
            emit_insn(gen_blockage());
#endif
            if (CONSTANT_P(operands[1])) {
                  if (UINTVAL(operands[1]) == 0) {
                        emit_insn(gen_bwt_mc_addr_z(operands[0], const0_rtx, operands[2],
                                             operands[3], operands[4], operands[5]));
                        done = 1 ;
                  }
                  else {
                        operands[1] = force_reg( SImode, operands[1] );
                  }
            }
            if (!done)
                  emit_insn(gen_bwt_mc_addr_gpr(operands[0], operands[1], operands[2], 
                                                   operands[3], operands[4], operands[5]));
            DONE;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_vn_io_gpr"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
       (use (match_operand 1 "register_operand" "r")) 
            (match_operand 2 "const_int_operand" "") 
            (match_operand 3 "const_int_operand" "") 
            (match_operand 4 "const_int_operand" "")
            (match_operand 5 "const_int_operand" "")
            (match_operand 6 "const_int_operand" "")
                                    ] UNSPEC_BWT_VN_IO_GPR)]
  ""
       { 
            return " # bwt_vn_io_var(%0,%1,%2,%3,%4,%5,%6)\n"
                  _BWT_ASM_VN_IO_VAR(%0, %r1, %2, %3, %4, %5, %6) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)


(define_insn "bwt_vn_io_z"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "const_int_operand" "")
                     (match_operand 5 "const_int_operand" "")
                     (match_operand 6 "const_int_operand" "")
                                    ] UNSPEC_BWT_VN_IO_Z)]
  ""
       { 
            return " # bwt_vn_io_var(%0,%1,%2,%3,%4,%5,%6)\n"
                  _BWT_ASM_VN_IO_VAR(%0, %1, %2, %3, %4, %5, %6) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)


(define_expand "bwt_vn_io_var"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "nonmemory_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "const_int_operand" "")
                     (match_operand 5 "const_int_operand" "")
                     (match_operand 6 "const_int_operand" "")
                                    ] UNSPEC_BWT_VN_IO_VAR)]
  ""
       { 
            int done = 0 ;
#if 0 /* use unspec_volatile */
            emit_insn(gen_blockage());
#endif
            if (CONSTANT_P(operands[1])) {
                  if (UINTVAL(operands[1]) == 0) {
                        emit_insn(gen_bwt_vn_io_z(operands[0], const0_rtx, 
                                            operands[2], operands[3], 
                                            operands[4], operands[5], operands[6]));
                        done = 1 ;
                  }
                  else {
                        operands[1] = force_reg( SImode, operands[1] );
                  }
            }
            if (!done) 
                  emit_insn(gen_bwt_vn_io_gpr(operands[0], operands[1], 
                                        operands[2], operands[3], 
                                        operands[4], operands[5], operands[6]));
            DONE;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)



(define_insn "bwt_transfer_inst"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "const_int_operand" "")
                     (match_operand 5 "const_int_operand" "")
                     (match_operand 6 "const_int_operand" "")
                     (match_operand 7 "const_int_operand" "")
                                    ] UNSPEC_BWT_TRANSFER_INST)]
  ""
       { 
            return " # bwt_transfer_inst(%0,%1,%2,%3,%4,%5,%6,%7)\n"
                  _BWT_ASM_TRANSFER_INST(%0, %1, %2, %3, %4, %5, %6, %7) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)


(define_insn "bwt_load_mc_z"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                                    ] UNSPEC_BWT_LOAD_MC_Z)]
  ""
       { 
            return " # bwt_load_mc(%0,%1,%2)\n"
                  _BWT_ASM_LOAD_MC(%0, %1, %2) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_load_mc_gpr"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
       (use (match_operand 1 "register_operand" "r")) 
            (match_operand 2 "const_int_operand" "") 
                                    ] UNSPEC_BWT_LOAD_MC_GPR)]
  ""
       { 
            return " # bwt_load_mc(%0,%1,%2)\n"
                  _BWT_ASM_LOAD_MC(%0, %r1, %2) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_expand "bwt_load_mc"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "nonmemory_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                                    ] UNSPEC_BWT_LOAD_MC)]
  ""
       { 
            int done = 0 ;
#if 0 /* use unspec_volatile */
            emit_insn(gen_blockage());
#endif
            if (CONSTANT_P(operands[1])) {
                  if (UINTVAL(operands[1]) == 0) {
                        emit_insn(gen_bwt_load_mc_z(operands[0], const0_rtx, operands[2]));
                        done = 1 ;
                  }
                  else {
                        operands[1] = force_reg( SImode, operands[1] );
                  }
            }
            if (!done)
                  emit_insn(gen_bwt_load_mc_gpr(operands[0], operands[1], operands[2]));
            DONE;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_select_c"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "const_int_operand" "") 
                                    ] UNSPEC_BWT_SELECT_C)]
  ""
       { 
            return " # bwt_select_c(%0,%1,%2,%3,%4)\n"
                  _BWT_ASM_SELECT(%0, %1, %2, %3, %4) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)


(define_insn "bwt_select_gpr"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
            (match_operand 1 "const_int_operand" "") 
            (match_operand 2 "const_int_operand" "") 
       (use (match_operand 3 "register_operand" "r")) 
            (match_operand 4 "const_int_operand" "") 
                                    ] UNSPEC_BWT_SELECT_GPR)]
  ""
       { 
            return " # bwt_select_var(%0,%1,%2,%3,%4)\n"
                  _BWT_ASM_SELECT(%0, %1, %2, %r3, %4) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)


(define_expand "bwt_select_var"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "nonmemory_operand" "") 
                     (match_operand 4 "const_int_operand" "") 
                                    ] UNSPEC_BWT_SELECT_VAR)]
  ""
      {
            int done = 0 ;
            int op3_is_const = CONSTANT_P(operands[3]) ;

#if 0 /* use unspec_volatile */
            emit_insn(gen_blockage());
#endif
            if (op3_is_const && mips_unsigned_immediate_p(UINTVAL(operands[3]), 8, 0)) {
                  int op2_is_1 = (INTVAL(operands[2]) == 1) ? 1 : 0 ;
                  int op4_is_0 = (INTVAL(operands[4]) == 0) ? 1 : 0 ;

                  if (op2_is_1 && op4_is_0) {
                              // save the register load and embed the constant..
                        done = 1 ;
                        emit_insn(gen_bwt_select_c(operands[0], operands[1],
                                        const0_rtx, const0_rtx, operands[3]));
                  }
            }
            
            if (!done) {
                  if (op3_is_const) {
                        operands[3] = force_reg( SImode, operands[3]);
                  }

                  emit_insn(gen_bwt_select_gpr(operands[0],operands[1],
                                 operands[2], operands[3], operands[4]));
            }
            DONE ;
      }
)

(define_insn "bwt_semset_c"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "const_int_operand" "") 
                                    ] UNSPEC_BWT_SEMSET_C)]
  ""
       { 
            return " # bwt_semset_c(%0,%1,%2,%3,%4)\n"
                  _BWT_ASM_SEMSET(%0, %1, %2, %3, %4) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_semset_gpr"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
            (match_operand 1 "const_int_operand" "") 
            (match_operand 2 "const_int_operand" "") 
       (use (match_operand 3 "register_operand" "r")) 
            (match_operand 4 "const_int_operand" "") 
                                    ] UNSPEC_BWT_SEMSET_GPR)]
  ""
       { 
            return " # bwt_semset_var(%0,%1,%2,%3,%4)\n"
                  _BWT_ASM_SEMSET(%0, %1, %2, %r3, %4) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_expand "bwt_semset_var"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "nonmemory_operand" "") 
                     (match_operand 4 "const_int_operand" "") 
                                    ] UNSPEC_BWT_SEMSET_VAR)]
  ""
      {
            int done = 0 ;
            int op3_is_const = CONSTANT_P(operands[3]) ;

#if 0 /* use unspec_volatile */
            emit_insn(gen_blockage());
#endif
            if (op3_is_const && mips_unsigned_immediate_p(UINTVAL(operands[3]), 8, 0)) {
                  int op2_is_1 = (INTVAL(operands[2]) == 1) ? 1 : 0 ;
                  int op4_is_0 = (INTVAL(operands[4]) == 0) ? 1 : 0 ;

                  if (op2_is_1 && op4_is_0) {
                              // save the register load and embed the constant..
                        done = 1 ;
                        emit_insn(gen_bwt_semset_c(operands[0], operands[1],
                                        const0_rtx, const0_rtx, operands[3]));
                  }
            }
            
            if (!done) {
                  if (op3_is_const) {
                        operands[3] = force_reg( SImode, operands[3]);      
                  }

                  emit_insn(gen_bwt_semset_gpr(operands[0],operands[1],
                                 operands[2], operands[3], operands[4]));
            }
            DONE ;
      }
)

(define_insn "bwt_port_io_s"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "const_int_operand" "") 
                     (match_operand 5 "const_int_operand" "") 
                                    ] UNSPEC_BWT_PORT_IO_S)]
  ""
       { 
            return " # bwt_port_io_s(%0,%1,%2,%3,%4,%5)\n"
                  _BWT_ASM_PORT_IO(%0, %1, %2, %3, %4, %5) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_port_io_gpr"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
            (match_operand 1 "const_int_operand" "") 
            (match_operand 2 "const_int_operand" "") 
            (match_operand 3 "const_int_operand" "") 
       (use (match_operand 4 "register_operand" "r"))
            (match_operand 5 "const_int_operand" "") 
                        ] UNSPEC_BWT_PORT_IO_GPR)]
  ""
       { 
            return " # bwt_port_io_var(%0,%1,%2,%3,%4,%5)\n"
                  _BWT_ASM_PORT_IO(%0, %1, %2, %3, %r4, %5) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_expand "bwt_port_io_var"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "") 
                     (match_operand 4 "nonmemory_operand" "") 
                     (match_operand 5 "const_int_operand" "") 
                                    ] UNSPEC_BWT_PORT_IO_VAR)]
  ""
      {
            int done = 0 ;
            int op4_is_const = CONSTANT_P(operands[4]) ;

#if 0 /* use unspec_volatile */
            emit_insn(gen_blockage());
#endif
            if (op4_is_const && mips_unsigned_immediate_p(UINTVAL(operands[4]), 10, 0)) {
                  int op3_is_1 = (INTVAL(operands[3]) == 1) ? 1 : 0 ;
                  int op2_is_0 = (INTVAL(operands[2]) == 0) ? 1 : 0 ;

                  if (op3_is_1 && op2_is_0) {
                              // save the register load and embed the constant..
                        done = 1 ;
                        emit_insn(gen_bwt_port_io_s(operands[0], operands[1],
                              operands[4], const0_rtx, const0_rtx, operands[5]));
                  }
            }
            
            if (!done) {
                  if (op4_is_const) {
                        operands[4] = force_reg( SImode, operands[4]);
                  }

                  emit_insn(gen_bwt_port_io_gpr(operands[0],operands[1],
                                 operands[2], operands[3], operands[4], operands[5]));
            }
            DONE ;
      }
)

(define_insn "bwt_mc_main_gpr"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
            (match_operand 1 "const_int_operand" "") 
            (match_operand 2 "const_int_operand" "") 
       (use (match_operand 3 "register_operand" "r"))
                                    ] UNSPEC_BWT_MC_MAIN_GPR)]
  ""
       { 
            return " # bwt_mc_main(%0,%1,%2,%3)\n"
                  _BWT_ASM_MC_MAIN(%0, %1, %2, %r3) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_mc_main_z"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "const_int_operand" "") 
                                    ] UNSPEC_BWT_MC_MAIN_Z)]
  ""
       { 
            return " # bwt_mc_main(%0,%1,%2,%3)\n"
                  _BWT_ASM_MC_MAIN(%0, %1, %2, %3) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_expand "bwt_mc_main"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "") 
                     (match_operand 2 "const_int_operand" "") 
                     (match_operand 3 "nonmemory_operand" "") 
                                    ] UNSPEC_BWT_MC_MAIN)]
  ""
       { 
            int done = 0 ;
#if 0 /* use unspec_volatile */
            emit_insn(gen_blockage());
#endif
            if (CONSTANT_P(operands[3])) {
                  if (UINTVAL(operands[3]) == 0) {
                        emit_insn(gen_bwt_mc_main_z(operands[0],operands[1],
                                                   operands[2], const0_rtx));
                        done = 1 ;
                  }
                  else {
                        operands[3] = force_reg( SImode, operands[3] );
                  }
            }
            if (!done)
                  emit_insn(gen_bwt_mc_main_gpr(operands[0],operands[1],
                                       operands[2], operands[3]));
            DONE;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

(define_insn "bwt_fft"
  [(unspec_volatile [(match_operand 0 "const_int_operand" "") 
                     (match_operand 1 "const_int_operand" "")] UNSPEC_BWT_FFT)]
  ""
       { 
            return " # bwt_fft(%0,%1)\n" 
                  _BWT_ASM_FFT(%0, %1) ;
      }
  [
   (set_attr "mode" "SI")
   (set_attr "type" "mtc")
   (set_attr "hazard" "none")
  ]
)

