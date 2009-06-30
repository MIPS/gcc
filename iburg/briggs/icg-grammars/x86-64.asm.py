# {([

#
# Copyright (c) 2008 Google Inc. All rights reserved.
#
# $Header: $
#
# -*- mode: python -*-

import plug

# global_print_plug_decls = 0; execfile("x86-64.gr.py")

#
# example:   __asm__ __volatile__("mfence" : : : "memory");
#
plug.plugrule3("asm_nulary", [
  ["rule", "cost"],
  ["stmt: ASM_NULLARY", [0, 0]],
], """
  $rule $cost
  debug {
    icg_analyze_asm(p->rtl, ICG_ASM_PHASE_DEBUG);
  },
  emit {
  }
  ;
""")

#
# Example: 
#  __asm__("bsr   %1, %0\n\t" "cmovz %2, %0" : "=&r" (bits) : "ro" (v), "r" (neg1) : "cc")
#
# For this rule, the destination register should be DIFFERENT from any of the source registers
#
plug.plugrule3("asm_binary_rrr", [
  ["rule", "cost", "schar", "rtx_mode"],
  ["r64x.dst: ASM_BINARY_RRR_DI(r64.src1, r64.src2)", [0, 0], 'q', "DImode"],
  ["r32x.dst: ASM_BINARY_RRR_SI(r32.src1, r32.src2)", [0, 0], 'l', "SImode"],
], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
    icg_reg_vector[$src1->rx].kind = INT_REGISTER;
    icg_reg_vector[$src2->rx].kind = INT_REGISTER;
  },
  supairs {
    /* TODO */
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($src1->r));
    sparseset_set_bit(live, find($src2->r));
    add_edges(rd, live);
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($src1->r);
    forgettable($src2->r);
  },
  debug {
    const rtx rtx = p->rtl;
    dumpasmRRR(ASM_OPERANDS_TEMPLATE(rtx),
      $src1->r, '$schar',
      $src2->r, '$schar',
      $dst->rx, '$schar');
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    static int doit = 1;
    if (doit) {
      const rtx expr = p->rtl;
      rtx src1 = gen_rtx_REG($rtx_mode, $src1->r);
      rtx src2 = gen_rtx_REG($rtx_mode, $src2->r);
      rtx dst  = gen_rtx_REG($rtx_mode, $dst->rx);
      /*
	0   ASM_OPERANDS_TEMPLATE
	1   ASM_OPERANDS_OUTPUT_CONSTRAINT
	2   ASM_OPERANDS_OUTPUT_IDX
	3   ASM_OPERANDS_INPUT_VEC
	4   ASM_OPERANDS_INPUT_CONSTRAINT_VEC
	3,N ASM_OPERANDS_INPUT
	3   ASM_OPERANDS_INPUT_LENGTH
	4,N ASM_OPERANDS_INPUT_CONSTRAINT_EXP
	4,N ASM_OPERANDS_INPUT_CONSTRAINT
	4,N ASM_OPERANDS_INPUT_MODE
	5,  ASM_OPERANDS_SOURCE_LOCATION
	1,  ASM_INPUT_SOURCE_LOCATION
      */
      /*
       * TODO: how is the output register $dst tied up into the asm rtx?
       */
      rtx asm_rtx  = gen_rtx_ASM_OPERANDS($rtx_mode,
	/*0*/ASM_OPERANDS_TEMPLATE(expr),
	/*1*/ASM_OPERANDS_OUTPUT_CONSTRAINT(expr),
	/*2*/ASM_OPERANDS_OUTPUT_IDX(expr),
	/*3*/gen_rtvec(2, src1, src2),
	/*4*/ASM_OPERANDS_INPUT_CONSTRAINT_VEC(expr),
	/*5*/ASM_OPERANDS_SOURCE_LOCATION(expr)
      );
      rtx set_rtx = gen_rtx_SET(VOIDmode, dst, asm_rtx);
      rtvec vect = gen_rtvec(3,
        set_rtx,
	gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(CCmode, FPSR_REG)),
	gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(CCmode, FLAGS_REG))
      );
      rtx parallel = gen_rtx_PARALLEL(VOIDmode, vect);
      icg_emit_plain(parallel);
    }
  }
  ;
""")

