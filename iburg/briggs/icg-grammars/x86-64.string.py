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
# memmove allows for overlapping strings; it isn't clear
#   how/when the gcc front end will convert memmove to memcpy
#   perhaps only under the strictest non-overlap detection.
#
# memcpy  allows for non-overlapping strings
#
# A translation of builtin_memcpy comes
# to us as a parallel tree that encodes all
# of the side effects on registers that are expected from
# executing a "rep movsq" (repeat move string quad) instruction sequence.
#
# Our strategy relies on DF being set to 0 (cleared),
#   so that autoincrement semantics are used; this is assumed
#   by the x86_64 ABI on function entry/exit.
#
# For example, this is the GCC IL that appears for memcpy:
#  (parallel [
#      (set (reg:DI 79)
#	  (const_int 0 [0x0]))
#      (set (reg/f:DI 70 [ D.89782 ])
#	  (plus:DI (ashift:DI (reg:DI 78)
#		  (const_int 3 [0x3]))
#	      (reg/f:DI 70 [ D.89782 ])))
#      (set (reg/f:DI 71 [ __beg ])
#	  (plus:DI (ashift:DI (reg:DI 78)
#		  (const_int 3 [0x3]))
#	      (reg/f:DI 71 [ __beg ])))
#      (set (mem:BLK (reg/f:DI 70 [ D.89782 ]) [0 A8])
#	    (mem:BLK (reg/f:DI 71 [ __beg ]) [0 A8]))
#      (use (reg:DI 78))
#  ]) 842 {*rep_movdi_rex64} (expr_list:REG_DEAD (reg:DI 78)
#	  (expr_list:REG_UNUSED (reg:DI 79)
#	      (nil)))
#
# (reg:DI 78) presumably holds the initial value of the C register, which is the count in uint64 words
# (reg:DI 79) maps to the rcx (count) register; when done, the C register is 0
# (reg:DI 70) maps to the rdi (dst  ) register; when done, it has value rdi += r78<<3
# (reg:DI 71) maps to the rsi (src  ) register; when done, it has value rsi += r78<<3
# and then there's a final use of r78
#
# The assignment (set (reg:DI 79) (const_int 0)) appears to be elided by something upstream of us
# possibly from icg-ssa.c?
#

plug.plugrule3("memcpy0", [
  ["rule", "cost"],
  ["""stmt:
      PARALLEL_ALL(
	  SET_ALL(lhs64.dst_rdi, PLUS_DI(ASHIFT_DI(r64.src1_rcx, CONST_P3), r64.src1_rdi)),
        PARALLEL_ALL(
	  SET_ALL(lhs64.dst_rsi, PLUS_DI(ASHIFT_DI(r64.src2_rcx, CONST_P3), r64.src1_rsi)),
	  PARALLEL_ALL(
	    SET_ALL(MEMB_DI(r64.src2_rdi), MEMB_DI(r64.src2_rsi)),
	    USE_ALL(r64.src3_rcx)
	  )
	)
      )
    """,
    [10, 10]	# TODO: bogus cost
  ],
], """
  $rule $cost
  supairs {
    /* TODO */
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src1_rcx->r, REG_RCX);
    coalesces += attempt_coalesce(pass, $src1_rdi->r, REG_RDI);
    coalesces += attempt_coalesce(pass, $src1_rsi->r, REG_RSI);
    if (!$dst_rdi->spilled)
      coalesces += attempt_coalesce(pass, $dst_rdi->r,  REG_RDI);
    if (!$dst_rsi->spilled)
      coalesces += attempt_coalesce(pass, $dst_rsi->r,  REG_RSI);
  },
  build {
    /* start at back, working forward (reverse order from things are emitted) */
    if ($dst_rsi->spilled) {
      sparseset_set_bit(live, REG_RSI);
      add_addr(live, $dst_rsi);
    }
    else
      add_copy_edges(/*dst*/$dst_rsi->r, /*src*/REG_RSI, live);
    if ($dst_rdi->spilled) {
      sparseset_set_bit(live, REG_RDI);
      add_addr(live, $dst_rdi);
    }
    else
      add_copy_edges(/*dst*/$dst_rdi->r, /*src*/REG_RDI, live);

    add_edges(REG_RCX, live);
    add_edges(REG_RDI, live);
    add_edges(REG_RSI, live);

    add_copy_edges(/*dst*/REG_RSI, /*src*/$src1_rsi->r, live);
    add_copy_edges(/*dst*/REG_RDI, /*src*/$src1_rdi->r, live);
    add_copy_edges(/*dst*/REG_RCX, /*src*/$src1_rcx->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    cost_copy($src1_rsi->r, REG_RSI);
    cost_copy($src1_rdi->r, REG_RDI);
    cost_copy($src1_rcx->r, REG_RCX);
    forgettable($src1_rsi->r);
    forgettable($src1_rdi->r);
    forgettable($src1_rcx->r);
    if (!$dst_rsi->spilled)
      cost_copy($dst_rsi->r, REG_RSI);
    if (!$dst_rdi->spilled)
      cost_copy($dst_rdi->r, REG_RDI);
  },
  debug {
    /*
     * by construction upstream of icg: $src1_rcx, $src2_rcx           should be identical
     * by construction upstream of icg: $dst_rdi, $src1_rdi, $src2_rdi should be identical
     * by construction upstream of icg: $dst_rsi, $src1_rsi, $src2_rsi should be identical
     */
    dump_copy("movq", $src1_rsi->r, REG_RSI, 'q');
    dump_copy("movq", $src1_rdi->r, REG_RDI, 'q');
    dump_copy("movq", $src1_rcx->r, REG_RCX, 'q');
    dump("rep movsq");
    if ($dst_rsi->spilled)
      dumpRM("movq", REG_RSI, 'q', $dst_rsi);
    else
      dump_copy("movq", REG_RSI, $dst_rsi->r, 'q');
    if ($dst_rdi->spilled)
      dumpRM("movq", REG_RDI, 'q', $dst_rdi);
    else
      dump_copy("movq", REG_RDI, $dst_rdi->r, 'q');
  },
  emit {
    const rtx src1_rcx = gen_rtx_REG(DImode, $src1_rcx->r);
    const rtx src1_rsi = gen_rtx_REG(DImode, $src1_rsi->r);
    const rtx src1_rdi = gen_rtx_REG(DImode, $src1_rdi->r);
    const rtx dst_rdi  = $dst_rdi->spilled
      ? gen_rtx_MEM(DImode, $dst_rdi->rtl)
      : gen_rtx_REG(DImode, $dst_rdi->r);
    const rtx dst_rsi  = $dst_rsi->spilled
      ? gen_rtx_MEM(DImode, $dst_rsi->rtl)
      : gen_rtx_REG(DImode, $dst_rsi->r);

    const rtx three    = gen_rtx_CONST_INT(DImode, 3);
    const rtx rsi      = gen_rtx_REG(DImode, REG_RSI);
    const rtx rdi      = gen_rtx_REG(DImode, REG_RDI);
    const rtx rcx      = gen_rtx_REG(DImode, REG_RCX);

    icg_emit_plain(gen_rtx_SET(VOIDmode, rcx, src1_rcx));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rsi, src1_rsi));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rdi, src1_rdi));
    icg_emit_plain(gen_rtx_PARALLEL(VOIDmode,
      gen_rtvec(5,
	gen_rtx_SET(VOIDmode, rcx, gen_rtx_CONST_INT(DImode, 0)),
	gen_rtx_SET(VOIDmode, rdi, gen_rtx_PLUS(DImode, gen_rtx_ASHIFT(DImode, rcx, three), rdi)),
	gen_rtx_SET(VOIDmode, rsi, gen_rtx_PLUS(DImode, gen_rtx_ASHIFT(DImode, rcx, three), rsi)),
	gen_rtx_SET(VOIDmode, gen_rtx_MEM(BLKmode, rdi), gen_rtx_MEM(BLKmode, rsi)),
	gen_rtx_USE(VOIDmode, rcx)
      )
    ));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst_rsi, rsi));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst_rdi, rdi));
  }
  ;
""")

#
# Contrast this with rule named memcpy0
# This pattern matches a subject perhaps from prologue/epilogue block moves, perhaps rep_movdi_rex64,
# to get a "rep movsq"; this from memory_region_map.ii
#
plug.plugrule3("memcpy1", [
  ["rule", "cost"],
  ["""stmt:
    PARALLEL_ALL(
      SET_ALL(MEMB_DI(r64.src_rdi), MEMB_DI(r64.src_rsi)),
      USE_ALL(r64.src_rcx)
    )
    """,
    [10, 10]	# TODO: bogus cost
  ],
], """
  $rule $cost
  supairs {
    /* TODO */
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src_rcx->r, REG_RCX);
    coalesces += attempt_coalesce(pass, $src_rdi->r, REG_RDI);
    coalesces += attempt_coalesce(pass, $src_rsi->r, REG_RSI);
  },
  build {
    /* start at back, working forward (reverse order from things are emitted) */
    add_edges(REG_RCX, live);
    add_edges(REG_RDI, live);
    add_edges(REG_RSI, live);

    add_copy_edges(/*dst*/REG_RSI, /*src*/$src_rsi->r, live);
    add_copy_edges(/*dst*/REG_RDI, /*src*/$src_rdi->r, live);
    add_copy_edges(/*dst*/REG_RCX, /*src*/$src_rcx->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    cost_copy($src_rsi->r, REG_RSI);
    cost_copy($src_rdi->r, REG_RDI);
    cost_copy($src_rcx->r, REG_RCX);
    forgettable($src_rsi->r);
    forgettable($src_rdi->r);
    forgettable($src_rcx->r);
  },
  debug {
    /*
     * by construction upstream of icg: $src1_rcx, $src2_rcx           should be identical
     * by construction upstream of icg: $dst_rdi, $src1_rdi, $src2_rdi should be identical
     * by construction upstream of icg: $dst_rsi, $src1_rsi, $src2_rsi should be identical
     */
    dump_copy("movq", $src_rsi->r, REG_RSI, 'q');
    dump_copy("movq", $src_rdi->r, REG_RDI, 'q');
    dump_copy("movq", $src_rcx->r, REG_RCX, 'q');
    dump("rep movsq");
  },
  emit {
    const rtx src_rsi = gen_rtx_REG(DImode, $src_rsi->r);
    const rtx src_rdi = gen_rtx_REG(DImode, $src_rdi->r);
    const rtx src_rcx = gen_rtx_REG(DImode, $src_rcx->r);

    const rtx three    = gen_rtx_CONST_INT(DImode, 3);
    const rtx rcx      = gen_rtx_REG(DImode, REG_RCX);
    const rtx rsi      = gen_rtx_REG(DImode, REG_RSI);
    const rtx rdi      = gen_rtx_REG(DImode, REG_RDI);

    icg_emit_plain(gen_rtx_SET(VOIDmode, rcx, src_rcx));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rsi, src_rsi));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rdi, src_rdi));
    /* here we take a short cut and generate the rtx for the memcpy0 pattern */
    icg_emit_plain(gen_rtx_PARALLEL(VOIDmode,
      gen_rtvec(5,
	gen_rtx_SET(VOIDmode, rcx, gen_rtx_CONST_INT(DImode, 0)),
	gen_rtx_SET(VOIDmode, rdi, gen_rtx_PLUS(DImode, gen_rtx_ASHIFT(DImode, rcx, three), rdi)),
	gen_rtx_SET(VOIDmode, rsi, gen_rtx_PLUS(DImode, gen_rtx_ASHIFT(DImode, rcx, three), rsi)),
	gen_rtx_SET(VOIDmode, gen_rtx_MEM(BLKmode, rdi), gen_rtx_MEM(BLKmode, rsi)),
	gen_rtx_USE(VOIDmode, rcx)
      )
    ));
    /*icg_emit_plain(gen_rtx_SET(VOIDmode, dst_rsi, rsi));*/
    /*icg_emit_plain(gen_rtx_SET(VOIDmode, dst_rdi, rdi));*/
  }
  ;
""")


# matches strmovqi_rex_1, strmovhi_rex_1, strmovsi_rex_1
plug.plugrule3("memcpy2", [
  ["rule", "cost", "opcode", "rtx_mode", "scale"],
  ["""stmt:
      PARALLEL_ALL(
	SET_ALL(MEM_QI(r64.src2_rdi), MEM_QI(r64.src2_rsi)),
        PARALLEL_ALL(
	  SET_ALL(lhs64.dst_rsi, PLUS_DI(r64.src1_rsi, CONST_P1)),
	  SET_ALL(lhs64.dst_rdi, PLUS_DI(r64.src1_rdi, CONST_P1))
	)
      )
    """,
    [10, 10], "movsb", "QImode", 1,
  ],
  ["""stmt:
      PARALLEL_ALL(
	SET_ALL(MEM_HI(r64.src2_rdi), MEM_HI(r64.src2_rsi)),
        PARALLEL_ALL(
	  SET_ALL(lhs64.dst_rsi, PLUS_DI(r64.src1_rsi, CONST_P2)),
	  SET_ALL(lhs64.dst_rdi, PLUS_DI(r64.src1_rdi, CONST_P2))
	)
      )
    """,
    [10, 10], "movsw", "HImode", 2,
  ],
  ["""stmt:
      PARALLEL_ALL(
	SET_ALL(MEM_SI(r64.src2_rdi), MEM_SI(r64.src2_rsi)),
        PARALLEL_ALL(
	  SET_ALL(lhs64.dst_rsi, PLUS_DI(r64.src1_rsi, CONST_P4)),
	  SET_ALL(lhs64.dst_rdi, PLUS_DI(r64.src1_rdi, CONST_P4))
	)
      )
    """,
    [10, 10], "movsl", "SImode", 4,
  ]

], """
  $rule $cost
  supairs {
    /* TODO */
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src1_rdi->r, REG_RDI);
    coalesces += attempt_coalesce(pass, $src1_rsi->r, REG_RSI);
    if (!$dst_rdi->spilled)
      coalesces += attempt_coalesce(pass, $dst_rdi->r,  REG_RDI);
    if (!$dst_rsi->spilled)
      coalesces += attempt_coalesce(pass, $dst_rsi->r,  REG_RSI);
  },
  build {
    /* start at back, working forward (reverse order from things are emitted) */
    if ($dst_rsi->spilled) {
      sparseset_set_bit(live, REG_RSI);
      add_addr(live, $dst_rsi);
    }
    else
      add_copy_edges(/*dst*/$dst_rsi->r, /*src*/REG_RSI, live);
    if ($dst_rdi->spilled) {
      sparseset_set_bit(live, REG_RDI);
      add_addr(live, $dst_rdi);
    }
    else
      add_copy_edges(/*dst*/$dst_rdi->r, /*src*/REG_RDI, live);

    add_edges(REG_RDI, live);
    add_edges(REG_RSI, live);

    add_copy_edges(/*dst*/REG_RSI, /*src*/$src1_rsi->r, live);
    add_copy_edges(/*dst*/REG_RDI, /*src*/$src1_rdi->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    cost_copy($src1_rsi->r, REG_RSI);
    cost_copy($src1_rdi->r, REG_RDI);
    forgettable($src1_rsi->r);
    forgettable($src1_rdi->r);
    if (!$dst_rsi->spilled)
      cost_copy($dst_rsi->r, REG_RSI);
    if (!$dst_rdi->spilled)
      cost_copy($dst_rdi->r, REG_RDI);
  },
  debug {
    /*
     * by construction upstream of icg: $dst_rdi, $src1_rdi, $src2_rdi should be identical
     * by construction upstream of icg: $dst_rsi, $src1_rsi, $src2_rsi should be identical
     */
    dump_copy("movq", $src1_rsi->r, REG_RSI, 'q');
    dump_copy("movq", $src1_rdi->r, REG_RDI, 'q');
    dump("$opcode");
    if ($dst_rsi->spilled)
      dumpRM("movq", REG_RSI, 'q', $dst_rsi);
    else
      dump_copy("movq", REG_RSI, $dst_rsi->r, 'q');
    if ($dst_rdi->spilled)
      dumpRM("movq", REG_RDI, 'q', $dst_rdi);
    else
      dump_copy("movq", REG_RDI, $dst_rdi->r, 'q');
  },
  emit {
    const rtx src1_rsi = gen_rtx_REG(DImode, $src1_rsi->r);
    const rtx src1_rdi = gen_rtx_REG(DImode, $src1_rdi->r);
    const rtx dst_rdi  = $dst_rdi->spilled
      ? gen_rtx_MEM(DImode, $dst_rdi->rtl)
      : gen_rtx_REG(DImode, $dst_rdi->r);
    const rtx dst_rsi  = $dst_rsi->spilled
      ? gen_rtx_MEM(DImode, $dst_rsi->rtl)
      : gen_rtx_REG(DImode, $dst_rsi->r);

    const rtx incr     = gen_rtx_CONST_INT(DImode, $scale);
    const rtx rsi      = gen_rtx_REG(DImode, REG_RSI);
    const rtx rdi      = gen_rtx_REG(DImode, REG_RDI);

    icg_emit_plain(gen_rtx_SET(VOIDmode, rsi, src1_rsi));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rdi, src1_rdi));
    icg_emit_plain(gen_rtx_PARALLEL(VOIDmode,
      gen_rtvec(3,
	gen_rtx_SET(VOIDmode, gen_rtx_MEM($rtx_mode, rdi), gen_rtx_MEM($rtx_mode, rsi)),
	gen_rtx_SET(VOIDmode, rdi, gen_rtx_PLUS(DImode, rdi, incr)),
	gen_rtx_SET(VOIDmode, rsi, gen_rtx_PLUS(DImode, rsi, incr))
      )
    ));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst_rsi, rsi));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst_rdi, rdi));
  }
  ;
""")


#
# A translation of builtin_memset comes
# to us as a parallel tree that encodes all
# of the side effects on registers that are expected from
# executing a "rep stosq" (repeat store string quad) instruction sequence.
#
# For example, this is the GCC IL that appears for memset:
#  (parallel [
#      (set (reg:DI 92)
#	  (const_int 0 [0x0]))
#      (set (reg/f:DI 84 [ D.83744 ])
#	  (plus:DI (ashift:DI (reg:DI 91)
#		  (const_int 3 [0x3]))
#	      (reg/f:DI 84 [ D.83744 ])))
#      (set (mem:BLK (reg/f:DI 84 [ D.83744 ]) [0 A8])
#	  (const_int 0 [0x0]))
#      (use (reg:DI 87))
#      (use (reg:DI 91))
#  ]) 854 {*rep_stosdi_rex64}
#  (expr_list:REG_DEAD (reg:DI 91)
#  (expr_list:REG_DEAD (reg:DI 87)
#  (expr_list:REG_UNUSED (reg:DI 92)
#  (nil))))

#
# (reg:DI 91) presumably holds the initial value of the C register, which is the count in uint64 words
# (reg:DI 92) maps to the rcx (count) register; when done, the C register is 0
# (reg:DI 84) maps to the rdi (dst  ) register; when done, it has value rdi += r91<<3
# and then there's a final use of r87 and r91
#
# We're not entirely sure about reg 91.
# My best guess is that it's set to 0 somewhere earlier
# so that we don't have to synthesize the 0 here.
# Instead, we hope that it's been usefully commoned.
#
# The stosq instruction does (here from the manual):
#  Copies a quadword from the RAX register to the memory locations pointed
#  to by RDI and increments/decrements RDI by 8 according to the value of DF.
#
# Alas, but the RTL backend to gcc has a pattern that only allows
# constant 0 to be the source operand to the block move,
# not some arbitrary value held in a register (rax).
#
# note that costs seem irrelevant here.
#

"""
PARALLEL_ALL
    SET_ALL
        REGX_DI:62
        CONST_0:0
    PARALLEL_ALL
        SET_ALL
            REGX_DI:63
            PLUS_DI
                ASHIFT_DI
                    REG_DI:61
                    CONST_P3:3
                REG_DI:64
        PARALLEL_ALL
            SET_ALL
                MEMB_DI
                    REG_DI:64
                CONST_0:0
            PARALLEL_ALL
                USE_ALL
                   REG_DI:60
                USE_ALL
                   REG_DI:61
"""

plug.plugrule3("memset0", [
  ["rule", "cost", "mode", "shift"],
  ["""stmt:
      PARALLEL_ALL(
          SET_ALL(lhs64.dst_rcx, CONST_0),
          PARALLEL_ALL(
	      SET_ALL(lhs64.dst_rdi, PLUS_DI(ASHIFT_DI(r64.src1_rcx, CONST_P3), r64.src1_rdi)),
	      PARALLEL_ALL(
	          SET_ALL(MEMB_DI(r64.src2_rdi), CONST_0),
	          PARALLEL_ALL(
	              USE_ALL(r64.src1_rax),
	              USE_ALL(r64.src2_rcx)))))""", [10, 10], "DImode", 3],
  ["""stmt:
      PARALLEL_ALL(
          SET_ALL(lhs64.dst_rcx, CONST_0),
          PARALLEL_ALL(
	      SET_ALL(lhs64.dst_rdi, PLUS_DI(ASHIFT_DI(r64.src1_rcx, CONST_P2), r64.src1_rdi)),
	      PARALLEL_ALL(
	          SET_ALL(MEMB_DI(r64.src2_rdi), CONST_0),
	          PARALLEL_ALL(
	              USE_ALL(r32.src1_rax),
	              USE_ALL(r64.src2_rcx)))))""", [10, 10], "SImode", 2],
], """
  $rule $cost
  supairs {
    /* TODO */
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src1_rax->r, REG_RAX);
    coalesces += attempt_coalesce(pass, $src1_rcx->r, REG_RCX);
    coalesces += attempt_coalesce(pass, $src1_rdi->r, REG_RDI);
    if (!$dst_rdi->spilled)
      coalesces += attempt_coalesce(pass, $dst_rdi->r,  REG_RDI);
    if (!$dst_rcx->spilled)
      coalesces += attempt_coalesce(pass, $dst_rcx->r,  REG_RCX);
  },
  build {
    /* start at back, working forward (reverse order from things are emitted) */
    if ($dst_rdi->spilled) {
      sparseset_set_bit(live, REG_RDI);
      add_addr(live, $dst_rdi);
    }
    else
      add_copy_edges(/*dst*/$dst_rdi->r, /*src*/REG_RDI, live);
    if ($dst_rcx->spilled) {
      sparseset_set_bit(live, REG_RCX);
      add_addr(live, $dst_rcx);
    }
    else
      add_copy_edges(/*dst*/$dst_rcx->r, /*src*/REG_RCX, live);

    add_edges(REG_RCX, live);
    add_edges(REG_RDI, live);

    add_copy_edges(/*dst*/REG_RDI, /*src*/$src1_rdi->r, live);
    add_copy_edges(/*dst*/REG_RCX, /*src*/$src1_rcx->r, live);
    add_copy_edges(/*dst*/REG_RAX, /*src*/$src1_rax->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    cost_copy($src1_rax->r, REG_RAX);
    cost_copy($src1_rcx->r, REG_RCX);
    cost_copy($src1_rdi->r, REG_RDI);
    forgettable($src1_rax->r);
    forgettable($src1_rdi->r);
    forgettable($src1_rcx->r);
    if (!$dst_rdi->spilled)
      cost_copy($dst_rdi->r, REG_RDI);
    if (!$dst_rcx->spilled)
      cost_copy($dst_rcx->r, REG_RCX);
  },
  debug {
    dump_copy("movq", $src1_rax->r, REG_RAX, 'q'); /* get zero into rax    */
    dump_copy("movq", $src1_rcx->r, REG_RCX, 'q'); /* get count into rcx   */
    dump_copy("movq", $src1_rdi->r, REG_RDI, 'q'); /* get address into rdi */
    dump("rep stosq");
    if ($dst_rcx->spilled)
      dumpRM("movq", REG_RCX, 'q', $dst_rcx);
    else
      dump_copy("movq", REG_RCX, $dst_rcx->r, 'q');
    if ($dst_rdi->spilled)
      dumpRM("movq", REG_RDI, 'q', $dst_rdi);
    else
      dump_copy("movq", REG_RDI, $dst_rdi->r, 'q');
  },
  emit {
    #if 0
      We want something like this; ordering of parallel kids is evidently important:
	 (parallel [
            (set (reg:DI 2 cx [92])
                (const_int 0 [0x0]))
            (set (reg/f:DI 5 di [orig:84 D.83744 ] [84])
                (plus:DI (ashift:DI (reg:DI 2 cx [91])
                        (const_int 3 [0x3]))
                    (reg/f:DI 5 di [orig:84 D.83744 ] [84])))
            (set (mem:BLK (reg/f:DI 5 di [orig:84 D.83744 ] [84]) [0 A8])
                (const_int 0 [0x0]))
            (use (reg:DI 0 ax [87]))
            (use (reg:DI 2 cx [91]))
	  ]) 854 {*rep_stosdi_rex64} (nil)
    #endif

    const rtx src1_rcx = gen_rtx_REG(DImode, $src1_rcx->r);
    const rtx src1_rax = gen_rtx_REG($mode, $src1_rax->r);
    const rtx src1_rdi = gen_rtx_REG(DImode, $src1_rdi->r);
    const rtx dst_rdi  = $dst_rdi->spilled
      ? gen_rtx_MEM(DImode, $dst_rdi->rtl)
      : gen_rtx_REG(DImode, $dst_rdi->r);
    const rtx dst_rcx  = $dst_rcx->spilled
      ? gen_rtx_MEM(DImode, $dst_rcx->rtl)
      : gen_rtx_REG(DImode, $dst_rcx->r);

    const rtx shift = gen_rtx_CONST_INT(DImode, $shift);
    const rtx rax   = gen_rtx_REG($mode, REG_RAX);
    const rtx rdi   = gen_rtx_REG(DImode, REG_RDI);
    const rtx rcx   = gen_rtx_REG(DImode, REG_RCX);

    icg_emit_plain(gen_rtx_SET(VOIDmode, rax, src1_rax));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rcx, src1_rcx));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rdi, src1_rdi));
    icg_emit_plain(gen_rtx_PARALLEL(VOIDmode,
      gen_rtvec(5,
	gen_rtx_SET(VOIDmode, rcx, gen_rtx_CONST_INT(DImode, 0)),
	gen_rtx_SET(VOIDmode, rdi, gen_rtx_PLUS(DImode, gen_rtx_ASHIFT(DImode, rcx, shift), rdi)),
	gen_rtx_SET(VOIDmode, gen_rtx_MEM(BLKmode, rdi), gen_rtx_CONST_INT(DImode, 0)),
	gen_rtx_USE(VOIDmode, rax),	/* must be 1st use */
	gen_rtx_USE(VOIDmode, rcx)	/* must be 2nd use */
      )
    ));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst_rcx, rcx));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst_rdi, rdi));
  };
""")


# this next one was surprising.
# it's apparently an encoding of the STOSD instruction,
# without the REP prefix.  Seems to be used to get alignment,
# sometimes, before using a REP STOSQ.  I'll make it as much
# as possible like the memset sequences above.
# costs here are again irrelevant.

plug.plugrule3("memset1", [
  ["rule", "cost", "mode", "stride"],
  ["""stmt: PARALLEL_ALL(
              SET_ALL(MEM_SI(r64.src1_rdi), SUBREG_SI(r64.src_rax, CONST_0)),
              SET_ALL(lhs64.dst_rdi, PLUS_DI(r64.src2_rdi, CONST_P4)))""", [1, 1], "SImode", 4],
  ["""stmt: PARALLEL_ALL(
              SET_ALL(MEM_HI(r64.src1_rdi), SUBREG_HI(r64.src_rax, CONST_0)),
              SET_ALL(lhs64.dst_rdi, PLUS_DI(r64.src2_rdi, CONST_P2)))""", [1, 1], "HImode", 2],
  ["""stmt: PARALLEL_ALL(
              SET_ALL(MEM_QI(r64.src1_rdi), SUBREG_QI(r64.src_rax, CONST_0)),
              SET_ALL(lhs64.dst_rdi, PLUS_DI(r64.src2_rdi, CONST_P1)))""", [1, 1], "QImode", 1],
  ], """
  $rule $cost
  supairs {
    /* TODO */
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src_rax->r, REG_RAX);
    coalesces += attempt_coalesce(pass, $src1_rdi->r, REG_RDI);
    if (!$dst_rdi->spilled)
      coalesces += attempt_coalesce(pass, $dst_rdi->r,  REG_RDI);
  },
  build {
    /* start at back, working forward (reverse order from things are emitted) */
    if ($dst_rdi->spilled) {
      sparseset_set_bit(live, REG_RDI);
      add_addr(live, $dst_rdi);
    }
    else
      add_copy_edges(/*dst*/$dst_rdi->r, /*src*/REG_RDI, live);

    add_edges(REG_RDI, live);

    add_copy_edges(/*dst*/REG_RDI, /*src*/$src1_rdi->r, live);
    add_copy_edges(/*dst*/REG_RAX, /*src*/$src_rax->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    cost_copy($src_rax->r, REG_RAX);
    cost_copy($src1_rdi->r, REG_RDI);
    forgettable($src_rax->r);
    forgettable($src1_rdi->r);
    if (!$dst_rdi->spilled)
      cost_copy($dst_rdi->r, REG_RDI);
  },
  debug {
    dump_copy("movq", $src_rax->r, REG_RAX, 'q');
    dump_copy("movq", $src1_rdi->r, REG_RDI, 'q');
    dump("stosl");
    if ($dst_rdi->spilled)
      dumpRM("movq", REG_RDI, 'q', $dst_rdi);
    else
      dump_copy("movq", REG_RDI, $dst_rdi->r, 'q');
  },
  emit {
    const rtx src_rax = gen_rtx_REG(DImode, $src_rax->r);
    const rtx src_rdi = gen_rtx_REG(DImode, $src1_rdi->r);
    const rtx dst_rdi  = $dst_rdi->spilled
      ? gen_rtx_MEM(DImode, $dst_rdi->rtl)
      : gen_rtx_REG(DImode, $dst_rdi->r);

    const rtx stride = gen_rtx_CONST_INT(DImode, $stride);
    const rtx rax    = gen_rtx_REG(DImode, REG_RAX);
    const rtx rdi    = gen_rtx_REG(DImode, REG_RDI);

    icg_emit_plain(gen_rtx_SET(VOIDmode, rax, src_rax));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rdi, src_rdi));
    icg_emit_plain(gen_rtx_PARALLEL(VOIDmode,
      gen_rtvec(2,
	gen_rtx_SET(VOIDmode, gen_rtx_MEM($mode, rdi), gen_rtx_SUBREG($mode, rax, 0)),
	gen_rtx_SET(VOIDmode, rdi, gen_rtx_PLUS(DImode, rdi, stride))
      )
    ));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst_rdi, rdi));
  };
""")

#
# A translation of __builtin_strlen comes to us as
# UNSPEC code 30 (UNSPEC_SCAS)
#
# For example, this is the GCC IL that appears:

# (parallel [
#  (set (reg:DI 61 [ D.94029 ])
#  (plus:DI (not:DI (unspec:DI [
#		    (mem:BLK (reg/v/f:DI 71 [ names ]) [0 A8])	; src
#		    (const_int 0 [0x0])				; eos
#		    (const_int 1 [0x1])				; align (unused?); also see CONST_P8
#		    (const_int -1 [0xffffffffffffffff])		; scratch RCX, UINTMAX; initial RCX value
#		    ] 30))
#   (const_int -1 [0xffffffffffffffff])))
# (clobber (reg:CC 17 flags))
# ])

#
# child 0 is the source pointer (reg:DI 71)
# child 1 is the end of string character (always const 0?)
# child 3 holds the value UINT_MAX, and may always be const?
#
# The scasb instruction does (here from the manual):
#  Compares the al register with the byte pointed to by RDI,
#   sets the status flags according to result,
#   increments RDI by 1.
# The repnz instruction does:
#  Repeat associated string instruction the number of times
#  spec in RCX; the repetition terminates when RCX is 0
#  or when the ZF is set to 1.
#
plug.plugrule3("strlen", [
  ["rule", "cost"],
  ["""r64.dst1_rcx:
      ICG_UNSPEC_SCAS(
        MEMB_DI(r64.src1_rdi),
	ICG_UNSPEC_SCAS(
	  r8.src1_rax,
	  ICG_UNSPEC_SCAS(
	    imm5.align,
	    r64.src1_rcx
	  )
	)
      )
    """,
    [10, 10]	# TODO: bogus cost
  ],
], """
  $rule $cost
  supairs {
    /* TODO */
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src1_rcx->r, REG_RCX);
    coalesces += attempt_coalesce(pass, $src1_rdi->r, REG_RDI);
    coalesces += attempt_coalesce(pass, $src1_rax->r, REG_RAX);
    if (!$dst1_rcx->spilled)
      coalesces += attempt_coalesce(pass, $dst1_rcx->r, REG_RCX);
  },
  build {
    /* start at back, working forward (reverse order from things are emitted) */
    if ($dst1_rcx->spilled) {
      sparseset_set_bit(live, REG_RCX);
      add_addr(live, $dst1_rcx);
    }
    else
      add_copy_edges(/*dst*/$dst1_rcx->r, /*src*/REG_RCX, live);

    add_edges(REG_RCX, live);
    add_edges(REG_RDI, live);
    add_edges(REG_RAX, live);

    add_copy_edges(/*dst*/REG_RAX, /*src*/$src1_rax->r, live);
    add_copy_edges(/*dst*/REG_RDI, /*src*/$src1_rdi->r, live);
    add_copy_edges(/*dst*/REG_RCX, /*src*/$src1_rcx->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    cost_copy($src1_rcx->r, REG_RCX);
    cost_copy($src1_rdi->r, REG_RDI);
    cost_copy($src1_rax->r, REG_RAX);
    forgettable($src1_rax->r);
    forgettable($src1_rdi->r);
    forgettable($src1_rcx->r);
    if (!$dst1_rcx->spilled)
      cost_copy($dst1_rcx->r, REG_RCX);
  },
  debug {
    dump_copy("movq", $src1_rcx->r, REG_RCX, 'q');
    dump_copy("movq", $src1_rdi->r, REG_RDI, 'q');
    dump_copy("movb", $src1_rax->r, REG_RAX, 'b');	/* comparand */
    dump("repnz scasb");
    if ($dst1_rcx->spilled)
      dumpRM("movq", REG_RCX, 'q', $dst1_rcx);
    else
      dump_copy("movq", REG_RCX, $dst1_rcx->r, 'q');
  },
  emit {
    #if 0
      We want something like this
       (unspec:DI [
	  (mem:BLK (reg/v/f:DI 71 [ names ]) [0 A8])
	  (const_int 0 [0x0])
	  (const_int 1 [0x1])
	  (const_int -1 [0xffffffffffffffff])
       ] 30 )

    The i386.md file contains this pattern; we have to make a tree that exactly matches this:
	(define_insn "*strlenqi_rex_1"
	  [(set (match_operand:DI 0 "register_operand" "=&c")
		(unspec:DI [(mem:BLK (match_operand:DI 5 "register_operand" "1"))
			    (match_operand:QI 2 "register_operand" "a")
			    (match_operand:DI 3 "immediate_operand" "i")
			    (match_operand:DI 4 "register_operand" "0")] UNSPEC_SCAS))
	   (clobber (match_operand:DI 1 "register_operand" "=D"))
	   (clobber (reg:CC FLAGS_REG))]
	  "TARGET_64BIT"
	  "repnz scasb"
	  [(set_attr "type" "str")
	   (set_attr "mode" "QI")
	   (set_attr "prefix_rep" "1")])

    #endif

    const rtx src1_rcx = gen_rtx_REG(DImode, $src1_rcx->r);
    const rtx src1_rax = gen_rtx_REG(QImode, $src1_rax->r);
    const rtx src1_rdi = gen_rtx_REG(DImode, $src1_rdi->r);
    const rtx dst1_rcx = $dst1_rcx->spilled
      ? gen_rtx_MEM(DImode, $dst1_rcx->rtl)
      : gen_rtx_REG(DImode, $dst1_rcx->r);

    const rtx rax      = gen_rtx_REG(QImode, REG_RAX);
    const rtx rdi      = gen_rtx_REG(DImode, REG_RDI);
    const rtx rcx      = gen_rtx_REG(DImode, REG_RCX);

    icg_emit_plain(gen_rtx_SET(VOIDmode, rcx, src1_rcx));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rax, src1_rax));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rdi, src1_rdi));
    icg_emit_plain(
      gen_rtx_PARALLEL(VOIDmode,
	gen_rtvec(3,
	  gen_rtx_SET(VOIDmode,
	    rcx,
	    gen_rtx_UNSPEC(DImode,
	      gen_rtvec(4,
		gen_rtx_MEM(BLKmode, rdi),
		1 ? rax : gen_rtx_CONST_INT(DImode, 0),
		gen_rtx_CONST_INT(DImode, 1),
		1 ? rcx : gen_rtx_CONST_INT(DImode, ~0ULL)
	      ),
	    UNSPEC_SCAS
	    )),
	  gen_rtx_CLOBBER(VOIDmode, rdi),
	  gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(CCmode, FLAGS_REG))
	)
      )
    );
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst1_rcx, rcx));
  }
  ;
""")


#
# A translation of __builtin_memcmp comes to us as
# a COMPARE_CC involving MEMB_DI.
#
#  The cmpsb instruction does (here from the manual):
#    Compares the bytes pointed bo by the RSI and RDI
#    registers, sets/clears the status flags of rFLAGS,
#    increments RSI and RDI according to DF.
#
# The repz instruction does:
#  Repeat associated string instruction the number of times
#  spec in RCX; the repetition terminates when RCX is 0
#  or when the ZF is set to 0.
#
plug.plugrule3("memcmp", [
  ["rule", "cost"],
  ["""stmt:
      PARALLEL_ALL(
	SET_ALL(rcc.dst_rcc,
	  COND_MOVE(
	    NE_ALL(r64.src1_rcx, CONST_0),
	    PAIR_ALL(
	      COMPARE_CC(MEMB_DI(r64.src1_rdi), MEMB_DI(r64.src1_rsi)),
	      CONST_0
	    )
	  )),
	PARALLEL_ALL(
	  USE_ALL(CONST_P1),
	  USE_ALL(COMPARE_CC(r64.src2_rcx, r64.src3_rcx))
	)
      )
    """,
    [10, 10]	# TODO: bogus cost
  ],
], """
  $rule $cost
  supairs {
    /* TODO */
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src1_rcx->r, REG_RCX);
    coalesces += attempt_coalesce(pass, $src2_rcx->r, REG_RCX); /* TODO: redundant? */
    coalesces += attempt_coalesce(pass, $src3_rcx->r, REG_RCX); /* TODO: redundant? */
    coalesces += attempt_coalesce(pass, $src1_rdi->r, REG_RDI);
    coalesces += attempt_coalesce(pass, $src1_rsi->r, REG_RSI);
    #if 0
    if (!$dst1_rcx->spilled)
      coalesces += attempt_coalesce(pass, $dst1_rcx->r, REG_RCX);
    #endif
  },
  build {
    /* start at back, working forward (reverse order from things are emitted) */
    #if 0
    if ($dst1_rcx->spilled) {
      sparseset_set_bit(live, REG_RCX);
      add_addr(live, $dst1_rcx);
    }
    else
      add_copy_edges(/*dst*/$dst1_rcx->r, /*src*/REG_RCX, live);
    #endif

    add_edges(REG_RCX, live);
    add_edges(REG_RDI, live);
    add_edges(REG_RSI, live);

    add_copy_edges(/*dst*/REG_RSI, /*src*/$src1_rsi->r, live);
    add_copy_edges(/*dst*/REG_RDI, /*src*/$src1_rdi->r, live);
    add_copy_edges(/*dst*/REG_RCX, /*src*/$src1_rcx->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    cost_copy($src1_rcx->r, REG_RCX);
    cost_copy($src1_rdi->r, REG_RDI);
    cost_copy($src1_rsi->r, REG_RSI);
    forgettable($src1_rsi->r);
    forgettable($src1_rdi->r);
    forgettable($src1_rcx->r);
    #if 0
    if (!$dst1_rcx->spilled)
      cost_copy($dst1_rcx->r, REG_RCX);
    #endif
  },
  debug {
    dump_copy("movq", $src1_rcx->r, REG_RCX, 'q');
    dump_copy("movq", $src1_rdi->r, REG_RDI, 'q');
    dump_copy("movq", $src1_rsi->r, REG_RSI, 'q');
    dump("repz cmpsb");
    #if 0
    if ($dst1_rcx->spilled)
      dumpRM("movq", REG_RCX, 'q', $dst1_rcx);
    else
      dump_copy("movq", REG_RCX, $dst1_rcx->r, 'q');
    #endif
  },
  emit {
    #if 0
      We want something like this
       (unspec:DI [
	  (mem:BLK (reg/v/f:DI 71 [ names ]) [0 A8])
	  (const_int 0 [0x0])
	  (const_int 1 [0x1])
	  (const_int -1 [0xffffffffffffffff])
       ] 30 )

    The i386.md file contains this pattern; we have to make a tree that exactly matches this:

	(define_insn "*cmpstrnqi_rex_1"
	  [(set (reg:CC FLAGS_REG)
		(if_then_else:CC (ne (match_operand:DI 6 "register_operand" "2")
				     (const_int 0))
		  (compare:CC (mem:BLK (match_operand:DI 4 "register_operand" "0"))
			      (mem:BLK (match_operand:DI 5 "register_operand" "1")))
		  (const_int 0)))
	   (use (match_operand:SI 3 "immediate_operand" "i"))
	   (use (reg:CC FLAGS_REG))
	   (clobber (match_operand:DI 0 "register_operand" "=S"))
	   (clobber (match_operand:DI 1 "register_operand" "=D"))
	   (clobber (match_operand:DI 2 "register_operand" "=c"))]
	  "TARGET_64BIT"
	  "repz cmpsb"
	  [(set_attr "type" "str")
	   (set_attr "mode" "QI")
	   (set_attr "prefix_rep" "1")])

    #endif

    const rtx src1_rcx = gen_rtx_REG(DImode, $src1_rcx->r);
    const rtx src1_rsi = gen_rtx_REG(DImode, $src1_rsi->r);
    const rtx src1_rdi = gen_rtx_REG(DImode, $src1_rdi->r);
    #if 0
    const rtx dst1_rcx = $dst1_rcx->spilled
      ? gen_rtx_MEM(DImode, $dst1_rcx->rtl)
      : gen_rtx_REG(DImode, $dst1_rcx->r);
    #endif

    const rtx rcx      = gen_rtx_REG(DImode, REG_RCX);
    const rtx rdi      = gen_rtx_REG(DImode, REG_RDI);
    const rtx rsi      = gen_rtx_REG(DImode, REG_RSI);

    icg_emit_plain(gen_rtx_SET(VOIDmode, rcx, src1_rcx));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rsi, src1_rsi));
    icg_emit_plain(gen_rtx_SET(VOIDmode, rdi, src1_rdi));
    #if 0
     TODO
    icg_emit_plain(
      gen_rtx_PARALLEL(VOIDmode,
	gen_rtvec(3,
	  gen_rtx_SET(VOIDmode,
	    rcx,
	    gen_rtx_UNSPEC(DImode,
	      gen_rtvec(4,
		gen_rtx_MEM(BLKmode, rdi),
		1 ? rax : gen_rtx_CONST_INT(DImode, 0),
		gen_rtx_CONST_INT(DImode, 1),
		1 ? rcx : gen_rtx_CONST_INT(DImode, ~0ULL)
	      ),
	    UNSPEC_SCAS
	    )),
	  gen_rtx_CLOBBER(VOIDmode, rdi),
	  gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(CCmode, FLAGS_REG))
	)
      )
    );
    #endif
    #if 0
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst1_rcx, rcx));
    #endif
  }
  ;
""")

#
# We also see trees like this for some form of string compare instruction
#
"""
PARALLEL_ALL <iburg handle 0x94faca8> {node 0x94ca4c0}
    SET_ALL <iburg handle 0x94fa6e8> {node 0x94ca544}
        REGCCX_DI <iburg handle 0x907e380> {node 0x94ca5c8}
        COMPARE_CC <iburg handle 0x94fa578> {node 0x94ca64c}
            MEMB_DI <iburg handle 0x94fa298> {node 0x94ca6d0}
                PLUS_DI <iburg handle 0x94fa128> {node 0x94ca754}
                    REG_DI:66 <iburg handle 0x9080780> {node 0x94ca7d8}
                    REG_DI:76 <iburg handle 0x9080780> {node 0x94ca85c}
            MEMB_DI <iburg handle 0x94fa408> {node 0x94ca8e0}
                REG_DI:77 <iburg handle 0x9080780> {node 0x94ca964}
    PARALLEL_ALL <iburg handle 0x94fab38> {node 0x94ca2bc}
        USE_ALL <iburg handle 0x94fa858> {node 0x94ca340}
            REG_DI:78 <iburg handle 0x9080780> {node 0x94ca3c4}
        USE_ALL <iburg handle 0x94fa9c8> {node 0x94ca1b4}
            CONST_P1:1 <iburg handle 0x906dd00> {node 0x94ca238}
"""

# })]
