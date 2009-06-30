#{([

#
# Copyright (c) 2008 Google Inc. All rights reserved.
#
# $Header: $
#
# -*- mode: python -*-

import plug

# global_print_plug_decls = 0; execfile("x86-64.gr.py")

# rd   - a double-precision register that must be preserved
# rdx  - a double-precision register that need not be preserved
#
# rf   - a single-precision register that must be preserved
# rfx  - a single-precision register that need not be preserved
#
# lhsd - a double-precision left-hand side (either register or memory)
# lfsf - a single-precision left-hand side (either register or memory)

#-------------------------------------------------------------
# lhs rules

plug.plugrule3 ("floatlhsreg", [
  ["rule", "cost"],

  ["lhsd.dst : REGX_DF", [0, 0]],
  ["lhsf.dst : REGX_SF", [0, 0]],
  ["lhsd.dst : REG_DF",  [0, 0]],
  ["lhsf.dst : REG_SF",  [0, 0]],

], """
  $rule $cost
  names {
    $dst->spilled = false;
  },
  kinds {
    icg_reg_vector[$dst->r].kind = FLOAT_REGISTER;
  },
  costs {
    cost_store($dst->r);
  },
  spill {
    dirty |= make_spill_code(find($dst->r), $dst);
  },
  final {
    $dst->r = icg_reg_vector[find($dst->r)].color;
  };
""")




plug.plugrule3 ("floatlhsstore", [
  ["rule", "cost", "rtx_mode"],

  ["lhsd.lhs : MEM_DF(addr.dst)", [0, 0], "DFmode"],
  ["lhsf.lhs : MEM_SF(addr.dst)", [0, 0], "SFmode"],

], """
  $rule $cost
  names {
    $lhs->spilled  = true;
    $lhs->a.base   = $dst->a.base;
    $lhs->a.base_valid = $dst->a.base_valid;
    $lhs->a.index  = $dst->a.index;
    $lhs->a.scale  = $dst->a.scale;
    $lhs->a.disp   = $dst->a.disp;
    $lhs->a.string = $dst->a.string;
  },
  emit {
    $lhs->rtl = $dst->rtl;
  };
""")



#
# Floating point assignment
#
plug.plugrule3 ("floatassignment", [
  ["rule", "cost", "opcode", "rtxmode"],

  ["stmt : SET_ALL(lhsd.dst, rd.src)", [0, 0], "movsd", "DFmode"],
  ["stmt : SET_ALL(lhsf.dst, rf.src)", [0, 0], "movss", "SFmode"],

], """
  $rule $cost
  supairs {
    suOrder2($stmt, $dst, $src, kid, kids);
  },
  coalesce {
    if (!$dst->spilled)
      coalesces += attempt_coalesce(pass, $dst->r, $src->r);
  },
  build {
    if ($dst->spilled) {
      sparseset_set_bit(live, find($src->r));
      add_addr(live, $dst);
    }
    else
      add_copy_edges($dst->r, $src->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    if (!$dst->spilled)
      cost_copy($src->r, $dst->r);
  },
  debug {
    if ($dst->spilled)
      dumpRM("$opcode", $src->r, 'x', $dst);
    else
      dump_copy("$opcode", $src->r, $dst->r, 'x');
  },
  emit {
    rtx src = gen_rtx_REG($rtxmode, $src->r);
    rtx dst = $dst->spilled ? gen_rtx_MEM($rtxmode, $dst->rtl) : gen_rtx_REG($rtxmode, $dst->r);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


#-------------------------------------------------------------
# register-register chain rules

plug.plugrule3 ("floatdst", [
  ["rule", "cost",           "rfield"],

  ["rd.dst   : REG_DF",  [0, 0],  "r"],
  ["rf.dst   : REG_SF",  [0, 0],  "r"],
  ["rdx.dst  : REGX_DF", [0, 0],  "rx"],
  ["rfx.dst  : REGX_SF", [0, 0],  "rx"],

], """
  $rule $cost
  kinds {
    icg_reg_vector[$dst->$rfield].kind = FLOAT_REGISTER;
  },
  costs {
    cost_load($dst->$rfield);
  },
  supairs {
    $dst->extra = 0;	/* TODO? only for rdx: rules */
    $dst->freed = 0;	/* TODO? only for rdx: rules */
  },
  spill {
    dirty |= make_spill_code(find($dst->$rfield), $dst);
  },
  final {
    $dst->$rfield = icg_reg_vector[find($dst->$rfield)].color;
  };
""")


plug.plugrule3 ("floatchainxsrc", [
  ["rule", "cost"],

  ["rd.dst : rdx.src", [0, 0]],
  ["rf.dst : rfx.src", [0, 0]],

], """
  $rule $cost
  supairs {
    $dst->freed++; /* assuming that the register will eventually be freed */
  },
  names {
    $dst->r = $src->rx;
  },
  final {
    $dst->r = $src->rx;
  };
""")

# assume coalesce will fail (but try anyway)

plug.plugrule3 ("floatchainxdst", [
  ["rule",     "cost",   "opcode", "rtx_mode"],

  #["rdx.dst : rd.src", [2, 4], "movsd", "DFmode"],
  #["rfx.dst : rf.src", [2, 4], "movss", "SFmode"],
  ["rdx.dst : rd.src", [2, 4], "movapd", "DFmode"],	# movapd used by icg+rtl backend for intra-xmm moves
  ["rfx.dst : rf.src", [2, 4], "movaps", "SFmode"],	# movaps used by icg+rtl backend for intra-xmm moves

], """
  $rule $cost
  supairs {
    if ($dst->freed > 0)
      $dst->freed--;
    else
      $dst->extra++;
  },
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = FLOAT_REGISTER;
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $dst->rx, $src->r);
  },
  build {
    add_copy_edges($dst->rx, $src->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    cost_copy($dst->rx, $src->r);
  },
  debug {
    dump_copy("$opcode", $src->r, $dst->rx, 'x');
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    const rtx src = gen_rtx_REG($rtx_mode, $src->r);
    const rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")

#----------------------------------------------------------------


#
# floating point loads
#
plug.plugrule3 ("floatloadbasic", [
  ["rule", "cost", "opcode", "rtx_mode"],

  ["rdx.dst  : MEM_DF(addr)", [3, 2], "movsd", "DFmode"],
  ["rfx.dst  : MEM_SF(addr)", [3, 2], "movss", "SFmode"],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = FLOAT_REGISTER;
  },
  supairs {
    if ($addr->freed > 0) {
      $dst->extra = $addr->extra;
      $dst->freed = $addr->freed - 1;
    }
    else {
      $dst->extra = $addr->extra + 1;
      $dst->freed = 0;
    }
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_clear_bit(live, rd);
    add_edges(rd, live);
    add_addr(live, $addr);
  },
  remat {
    flags = 0;
  },
  debug {
    dumpMR("$opcode", $addr, $dst->rx, 'x');
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    rtx src = gen_rtx_MEM($rtx_mode, $addr->rtl);
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


#
# floating point converts (tar pit)
# ieee rounding modes and various conversions are ignored.
#

#
# convert from ieee64 to int64, int32, int16(?), or int8(?)
# convert from ieee32 to int64, int32, int16(?), or int8(?)
#
plug.plugrule3 ("floatmovcvt", [
  ["rule", "cost", "isRR", "dst_rtx_mode", "src_rtx_mode", "opcode"],

  ["r64x.dst  : FIX_DI(rd.src)",           [1, 1], 1, "DImode", "DFmode", "cvttsd2siq"], # LIE (TODO)
  ["r32x.dst  : FIX_SI(rd.src)",           [1, 1], 1, "SImode", "DFmode", "cvttsd2siq"], # LIE (TODO)
  ["r16x.dst  : FIX_HI(rd.src)",           [1, 1], 1, "HImode", "DFmode", "cvttsd2siq"], # LIE (TODO)
  ["r8x.dst   : FIX_QI(rd.src)",           [1, 1], 1, "QImode", "DFmode", "cvttsd2siq"], # LIE (TODO)

  ["r64x.dst  : FIX_DI(rf.src)",           [1, 1], 1, "DImode", "SFmode", "cvttsd2siq"], # LIE (TODO)
  ["r32x.dst  : FIX_SI(rf.src)",           [1, 1], 1, "SImode", "SFmode", "cvttsd2siq"], # LIE (TODO)
  ["r16x.dst  : FIX_HI(rf.src)",           [1, 1], 1, "HImode", "SFmode", "cvttsd2siq"], # LIE (TODO)
  ["r8x.dst   : FIX_QI(rf.src)",           [1, 1], 1, "QImode", "SFmode", "cvttsd2siq"], # LIE (TODO)

  ["r64x.dst  : FIX_DI(MEM_DF(addr.src))", [1, 1], 0, "DImode", "DFmode", "cvttsd2siq"], # LIE (TODO)
  ["r32x.dst  : FIX_SI(MEM_DF(addr.src))", [1, 1], 0, "SImode", "DFmode", "cvttsd2siq"], # LIE (TODO)
  ["r16x.dst  : FIX_HI(MEM_DF(addr.src))", [1, 1], 0, "HImode", "DFmode", "cvttsd2siq"], # LIE (TODO)
  ["r8x.dst   : FIX_QI(MEM_DF(addr.src))", [1, 1], 0, "QImode", "DFmode", "cvttsd2siq"], # LIE (TODO)

  ["r64x.dst  : FIX_DI(MEM_SF(addr.src))", [1, 1], 0, "DImode", "SFmode", "cvttsd2siq"], # LIE (TODO)
  ["r32x.dst  : FIX_SI(MEM_SF(addr.src))", [1, 1], 0, "SImode", "SFmode", "cvttsd2siq"], # LIE (TODO)
  ["r16x.dst  : FIX_HI(MEM_SF(addr.src))", [1, 1], 0, "HImode", "SFmode", "cvttsd2siq"], # LIE (TODO)
  ["r8x.dst   : FIX_QI(MEM_SF(addr.src))", [1, 1], 0, "QImode", "SFmode", "cvttsd2siq"], # LIE (TODO)

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    /* NYI */
  },
  build {
    add_edges(find($dst->rx), live);
    sparseset_clear_bit(live, find($dst->rx));
    if ($isRR) {
      sparseset_set_bit(live, find($src->r));	/* r, not rx, as rhs is rd */
    }
    else add_addr(live, $src);
  },
  remat {
    flags = 0;
  },
  costs {
    if ($isRR) {
      memorable($src->r);
    }
  },
  debug {
    if ($isRR) {
      dumpRR("$opcode", $src->r, 'x', $dst->rx, 'q');
    } else {
      dumpMR("$opcode", $src, $dst->rx, 'q');
    }
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    rtx src = $isRR
       ? gen_rtx_REG($src_rtx_mode, $src->r)
       : gen_rtx_MEM($src_rtx_mode, $src->rtl);
    rtx dst = gen_rtx_REG($dst_rtx_mode, $dst->rx);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, gen_rtx_FIX($dst_rtx_mode, src)));
  };
""")

#
# convert with truncation from ieee64 to ieee32
# this can be done by cvtsd2ss (scalar double to scalar single)
# or by the sequence unpcklpd (unpack low double precision fp); cvtpd2ps
# the 2 insn sequence is what gcc produces, and may be one cycle faster;
# it ends up with an ieee32 duplicated in both 32-bit halves
# of the result register.
# convert with extension from ieee32 to ieee64
#
plug.plugrule3("floatmovtruncate", [
  ["rule", "cost", "isRR", "src_mode", "dst_mode", "opcode", "rtx_builder"],

  ["rfx.dst : FLOAT_TRUNCATE_SF(rd.src)",           [1, 1], 1, "DFmode", "SFmode",
    "cvtsd2ss", "gen_rtx_FLOAT_TRUNCATE"],
  ["rfx.dst : FLOAT_TRUNCATE_SF(MEM_DF(addr.src))", [1, 1], 0, "DFmode", "SFmode",
    "cvtsd2ss", "gen_rtx_FLOAT_TRUNCATE"],
  ["rdx.dst : FLOAT_EXTEND_DF(rf.src)",             [1, 1], 1, "SFmode", "DFmode",
    "cvtss2sd", "gen_rtx_FLOAT_EXTEND"],
  ["rdx.dst : FLOAT_EXTEND_DF(MEM_SF(addr.src))",   [1, 1], 0, "SFmode", "DFmode",
    "cvtss2sd", "gen_rtx_FLOAT_EXTEND"],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = FLOAT_REGISTER;
  },
  supairs {
    /* NYI */
  },
  build {
    add_edges(find($dst->rx), live);
    sparseset_clear_bit(live, find($dst->rx));
    if ($isRR) {
      sparseset_set_bit(live, find($src->r));	/* r, not rx, as rhs is rd */
    }
    else add_addr(live, $src);
  },
  remat {
    flags = 0;
  },
  costs {
    if ($isRR) {
      memorable($src->r);
    }
  },
  debug {
    if ($isRR) {
      dumpRR("$opcode", $src->r, 'x', $dst->rx, 'x');
    } else {
      dumpMR("$opcode", $src, $dst->rx, 'x');
    }
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    rtx src = $isRR
      ? gen_rtx_REG($src_mode, $src->r)
      : gen_rtx_MEM($src_mode, $src->rtl);
    rtx dst = gen_rtx_REG($dst_mode, $dst->rx);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, $rtx_builder($dst_mode, src)));
  };
""")

#
# convert from int64 to ieee64
# convert from int64 to ieee32
#
plug.plugrule3("floatcvtfromint", [
  ["rule", "cost",                             "isRR", "src_mode", "dst_mode", "opcode", "rtx_builder"],

  ["rdx.dst : FLOAT_DF(r64.src)",          [1, 1], 1, "DImode", "DFmode", "cvtsi2sdq", "gen_rtx_FLOAT"],
  ["rdx.dst : FLOAT_DF(MEM_DI(addr.src))", [1, 1], 0, "DImode", "DFmode", "cvtsi2sdq", "gen_rtx_FLOAT"],
  ["rfx.dst : FLOAT_SF(r64.src)",          [1, 1], 1, "DImode", "SFmode", "cvtsi2ssq", "gen_rtx_FLOAT"],
  ["rfx.dst : FLOAT_SF(MEM_DI(addr.src))", [1, 1], 0, "DImode", "SFmode", "cvtsi2ssq", "gen_rtx_FLOAT"],

  ["rdx.dst : FLOAT_DF(r32.src)",          [1, 1], 1, "SImode", "DFmode", "cvtsi2sdl", "gen_rtx_FLOAT"],
  ["rdx.dst : FLOAT_DF(MEM_SI(addr.src))", [1, 1], 0, "SImode", "DFmode", "cvtsi2sdl", "gen_rtx_FLOAT"],
  ["rfx.dst : FLOAT_SF(r32.src)",          [1, 1], 1, "SImode", "SFmode", "cvtsi2ssl", "gen_rtx_FLOAT"],
  ["rfx.dst : FLOAT_SF(MEM_SI(addr.src))", [1, 1], 0, "SImode", "SFmode", "cvtsi2ssl", "gen_rtx_FLOAT"],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = FLOAT_REGISTER;
  },
  supairs {
    /* NYI */
  },
  build {
    add_edges(find($dst->rx), live);
    sparseset_clear_bit(live, find($dst->rx));
    if ($isRR) {
      sparseset_set_bit(live, find($src->r));	/* r, not rx, as rhs is r64 */
    }
    else add_addr(live, $src);
  },
  remat {
    flags = 0;
  },
  costs {
    if ($isRR) {
      memorable($src->r);
    }
  },
  debug {
    if ($isRR) {
      dumpRR("$opcode", $src->r, 'q', $dst->rx, 'x');
    } else {
      dumpMR("$opcode", $src, $dst->rx, 'x');
    }
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    rtx src = $isRR
      ? gen_rtx_REG($src_mode, $src->r)
      : gen_rtx_MEM($src_mode, $src->rtl);
    rtx dst = gen_rtx_REG($dst_mode, $dst->rx);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, $rtx_builder($dst_mode, src)));
  };
""")

plug.plugrule3("floatbasicrr", [
  ["rule", "cost", "isRR", "isCommute", "opcode", "rtx_mode", "rtx_builder"],

  ["rdx.dst: PLUS_DF(rdx.src1  | rd.src2)", [1, 1], 1, 1, "addsd", "DFmode", "gen_rtx_PLUS"],
  ["rfx.dst: PLUS_SF(rfx.src1  | rf.src2)", [1, 1], 1, 1, "addss", "SFmode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VDI(rdx.src1 | rd.src2)", [1, 1], 1, 1, "paddq", "V2DImode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VSI(rdx.src1 | rd.src2)", [1, 1], 1, 1, "paddd", "V4SImode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VHI(rdx.src1 | rd.src2)", [1, 1], 1, 1, "paddw", "V8HImode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VQI(rdx.src1 | rd.src2)", [1, 1], 1, 1, "paddb", "V16QImode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VDF(rdx.src1 | rd.src2)", [1, 1], 1, 1, "addpd", "V2DFmode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VSF(rdx.src1 | rd.src2)", [1, 1], 1, 1, "addps", "V4SFmode", "gen_rtx_PLUS"],

  ["rdx.dst: PLUS_DF(rdx.src1  | MEM_DF(addr.src2))",  [1, 1], 0, 0, "addsd", "DFmode", "gen_rtx_PLUS"],
  ["rfx.dst: PLUS_SF(rfx.src1  | MEM_SF(addr.src2))",  [1, 1], 0, 0, "addss", "SFmode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VDI(rdx.src1 | MEM_VDI(addr.src2))", [1, 1], 0, 0, "paddq", "V2DImode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VSI(rdx.src1 | MEM_VSI(addr.src2))", [1, 1], 0, 0, "paddd", "V4SImode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VHI(rdx.src1 | MEM_VHI(addr.src2))", [1, 1], 0, 0, "paddw", "V8HImode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VQI(rdx.src1 | MEM_VQI(addr.src2))", [1, 1], 0, 0, "paddb", "V16QImode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VDF(rdx.src1 | MEM_VDF(addr.src2))", [1, 1], 0, 0, "addpd", "V2DFmode", "gen_rtx_PLUS"],
  ["rdx.dst: PLUS_VSF(rdx.src1 | MEM_VSF(addr.src2))", [1, 1], 0, 0, "addps", "V4SFmode", "gen_rtx_PLUS"],

  ["rdx.dst: PLUS_DF(rdx.src1 ,  NEG_DF(rd.src2))",  [1, 1], 1, 0, "subsd", "DFmode", "gen_rtx_MINUS"],
  ["rfx.dst: PLUS_SF(rfx.src1 ,  NEG_SF(rf.src2))",  [1, 1], 1, 0, "subss", "SFmode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VDI(rdx.src1 , NEG_VDI(rd.src2))", [1, 1], 1, 0, "psubq", "V2DImode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VSI(rdx.src1 , NEG_VSI(rd.src2))", [1, 1], 1, 0, "psubd", "V4SImode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VHI(rdx.src1 , NEG_VHI(rd.src2))", [1, 1], 1, 0, "psubw", "V8HImode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VQI(rdx.src1 , NEG_VQI(rd.src2))", [1, 1], 1, 0, "psubb", "V16QImode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VDF(rdx.src1 , NEG_VDF(rd.src2))", [1, 1], 1, 0, "subpd", "V2DFmode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VSF(rdx.src1 , NEG_VSF(rd.src2))", [1, 1], 1, 0, "subps", "V4SFmode", "gen_rtx_MINUS"],

  ["rdx.dst: PLUS_DF(rdx.src1 ,  NEG_DF(MEM_DF(addr.src2)))",   [1, 1], 0, 0, "subsd", "DFmode", "gen_rtx_MINUS"],
  ["rfx.dst: PLUS_SF(rfx.src1 ,  NEG_SF(MEM_SF(addr.src2)))",   [1, 1], 0, 0, "subss", "SFmode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VDI(rdx.src1 , NEG_VDI(MEM_VDI(addr.src2)))", [1, 1], 0, 0, "psubq", "V2DImode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VSI(rdx.src1 , NEG_VSI(MEM_VSI(addr.src2)))", [1, 1], 0, 0, "psubd", "V4SImode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VHI(rdx.src1 , NEG_VHI(MEM_VHI(addr.src2)))", [1, 1], 0, 0, "psubw", "V8HImode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VQI(rdx.src1 , NEG_VQI(MEM_VQI(addr.src2)))", [1, 1], 0, 0, "psubb", "V16QImode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VDF(rdx.src1 , NEG_VDF(MEM_VDF(addr.src2)))", [1, 1], 0, 0, "subpd", "V2DFmode", "gen_rtx_MINUS"],
  ["rdx.dst: PLUS_VSF(rdx.src1 , NEG_VSF(MEM_VSF(addr.src2)))", [1, 1], 0, 0, "subps", "V4SFmode", "gen_rtx_MINUS"],

  ["rdx.dst: MINUS_DF(rdx.src1 ,  rd.src2)", [1, 1], 1, 0, "subsd", "DFmode", "gen_rtx_MINUS"],
  ["rfx.dst: MINUS_SF(rfx.src1 ,  rf.src2)", [1, 1], 1, 0, "subss", "SFmode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VDI(rdx.src1 , rd.src2)", [1, 1], 1, 0, "psubq", "V2DImode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VSI(rdx.src1 , rd.src2)", [1, 1], 1, 0, "psubd", "V4SImode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VHI(rdx.src1 , rd.src2)", [1, 1], 1, 0, "psubw", "V8HImode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VQI(rdx.src1 , rd.src2)", [1, 1], 1, 0, "psubb", "V16QImode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VDF(rdx.src1 , rd.src2)", [1, 1], 1, 0, "subpd", "V2DFmode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VSF(rdx.src1 , rd.src2)", [1, 1], 1, 0, "subps", "V4SFmode", "gen_rtx_MINUS"],

  ["rdx.dst: MINUS_DF(rdx.src1 ,  MEM_DF(addr.src2))",  [1, 1], 0, 0, "subsd", "DFmode", "gen_rtx_MINUS"],
  ["rfx.dst: MINUS_SF(rfx.src1 ,  MEM_SF(addr.src2))",  [1, 1], 0, 0, "subss", "SFmode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VDI(rdx.src1 , MEM_VDI(addr.src2))", [1, 1], 0, 0, "psubq", "V2DImode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VSI(rdx.src1 , MEM_VSI(addr.src2))", [1, 1], 0, 0, "psubd", "V4SImode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VHI(rdx.src1 , MEM_VHI(addr.src2))", [1, 1], 0, 0, "psubw", "V8HImode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VQI(rdx.src1 , MEM_VQI(addr.src2))", [1, 1], 0, 0, "psubb", "V16QImode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VDF(rdx.src1 , MEM_VDF(addr.src2))", [1, 1], 0, 0, "subpd", "V2DFmode", "gen_rtx_MINUS"],
  ["rdx.dst: MINUS_VSF(rdx.src1 , MEM_VSF(addr.src2))", [1, 1], 0, 0, "subps", "V4SFmode", "gen_rtx_MINUS"],

  ["rdx.dst: MULT_DF(rdx.src1  | rd.src2)", [1, 1], 1, 1, "mulsd", "DFmode", "gen_rtx_MULT"],
  ["rfx.dst: MULT_SF(rfx.src1  | rf.src2)", [1, 1], 1, 1, "mulss", "SFmode", "gen_rtx_MULT"],
  ["rdx.dst: MULT_VHI(rdx.src1 | rd.src2)", [1, 1], 1, 1, "pmullw", "V8HImode", "gen_rtx_MULT"],
  ["rdx.dst: MULT_VDF(rdx.src1 | rd.src2)", [1, 1], 1, 1, "mulpd", "V2DFmode", "gen_rtx_MULT"],
  ["rdx.dst: MULT_VSF(rdx.src1 | rd.src2)", [1, 1], 1, 1, "mulps", "V4SFmode", "gen_rtx_MULT"],

  ["rdx.dst: MULT_DF(rdx.src1  | MEM_DF(addr.src2))",  [1, 1], 0, 0, "mulsd", "DFmode",    "gen_rtx_MULT"],
  ["rfx.dst: MULT_SF(rfx.src1  | MEM_SF(addr.src2))",  [1, 1], 0, 0, "mulss", "SFmode",    "gen_rtx_MULT"],
  ["rdx.dst: MULT_VHI(rdx.src1 | MEM_VHI(addr.src2))", [1, 1], 0, 0, "pmullw", "V8HImode", "gen_rtx_MULT"],
  ["rdx.dst: MULT_VDF(rdx.src1 | MEM_VDF(addr.src2))", [1, 1], 0, 0, "mulpd", "V2DFmode",  "gen_rtx_MULT"],
  ["rdx.dst: MULT_VSF(rdx.src1 | MEM_VSF(addr.src2))", [1, 1], 0, 0, "mulps", "V4SFmode",  "gen_rtx_MULT"],

  ["rdx.dst: DIV_DF(rdx.src1 , rd.src2)",              [1, 1], 1, 0, "divsd", "DFmode",   "gen_rtx_DIV"],
  ["rfx.dst: DIV_SF(rfx.src1 , rf.src2)",              [1, 1], 1, 0, "divss", "SFmode",   "gen_rtx_DIV"],
  ["rdx.dst: DIV_VDF(rdx.src1 , rd.src2)",             [1, 1], 1, 0, "divpd", "V2DFmode", "gen_rtx_DIV"],
  ["rdx.dst: DIV_VSF(rdx.src1 , rd.src2)",             [1, 1], 1, 0, "divps", "V4SFmode", "gen_rtx_DIV"],

  ["rdx.dst: DIV_DF(rdx.src1 , MEM_DF(addr.src2))",    [1, 1], 0, 0, "divsd", "DFmode",   "gen_rtx_DIV"],
  ["rfx.dst: DIV_SF(rfx.src1 , MEM_SF(addr.src2))",    [1, 1], 0, 0, "divss", "SFmode",   "gen_rtx_DIV"],
  ["rdx.dst: DIV_VDF(rdx.src1 , MEM_VHI(addr.src2))",  [1, 1], 0, 0, "divpd", "V8HImode", "gen_rtx_DIV"],
  ["rdx.dst: DIV_VSF(rdx.src1 , MEM_VSF(addr.src2))",  [1, 1], 0, 0, "divps", "V4SFmode", "gen_rtx_DIV"],

  ["rdx.dst: IOR_DF(rdx.src1 | rd.src2)",              [1, 1], 1, 1, "orpd",   "DFmode", "gen_rtx_IOR"],
  ["rfx.dst: IOR_SF(rfx.src1 | rf.src2)",              [1, 1], 1, 1, "orps",   "SFmode", "gen_rtx_IOR"],
  ["rdx.dst: XOR_DF(rdx.src1 | rd.src2)",              [1, 1], 1, 1, "xorpd",  "DFmode", "gen_rtx_XOR"],
  ["rfx.dst: XOR_SF(rfx.src1 | rf.src2)",              [1, 1], 1, 1, "xorps",  "SFmode", "gen_rtx_XOR"],
  ["rdx.dst: AND_DF(rdx.src1 | rd.src2)",              [1, 1], 1, 1, "andpd",  "DFmode", "gen_rtx_AND"],
  ["rfx.dst: AND_SF(rfx.src1 | rf.src2)",              [1, 1], 1, 1, "andps",  "SFmode", "gen_rtx_AND"],
  ["rdx.dst: AND_DF(NOT_DF(rdx.src1) | rd.src2)",      [1, 1], 1, 1, "andnpd", "DFmode", "gen_rtx_ANDNOT"],
  ["rfx.dst: AND_SF(NOT_SF(rfx.src1) | rf.src2)",      [1, 1], 1, 1, "andnps", "SFmode", "gen_rtx_ANDNOT"],

  #
  # TODO: naked NOT_DF and NOT_SF done by xor with a mask of all 1's (similar to NEG_DF and NEG_SF)
  #

  #
  # TODO: vector (packed.src2) variants of vector Booleans

  #
  # TODO: For gcc 4.3.1, icg+rtl phase can't seem to consume RTL for AND/IOR/XOR to xmm registers from memory
  # gcc 4.4 appears to be able to do these memory operations from memory
  #
  #["rdx.dst: IOR_DF(rdx.src1 | MEM_DF(addr.src2))",  [1, 1], 0, "orpd", "DFmode", "gen_rtx_IOR"],
  #["rfx.dst: IOR_SF(rfx.src1 | MEM_SF(addr.src2))",  [1, 1], 0, "orps", "SFmode", "gen_rtx_IOR"],

  #["rdx.dst: XOR_DF(rdx.src1 | MEM_DF(addr.src2))",  [1, 1], 0, "xorpd", "DFmode", "gen_rtx_XOR"],
  #["rfx.dst: XOR_SF(rfx.src1 | MEM_SF(addr.src2))",  [1, 1], 0, "xorps", "SFmode", "gen_rtx_XOR"],

  #["rdx.dst: AND_DF(rdx.src1 | MEM_DF(addr.src2))",  [1, 1], 0, "andpd", "DFmode", "gen_rtx_AND"],
  #["rfx.dst: AND_SF(rfx.src1 | MEM_SF(addr.src2))",  [1, 1], 0, "andps", "SFmode", "gen_rtx_AND"],
  #["rdx.dst: AND_DF(NOT_DF(rdx.src1) | MEM_DF(addr.src2))",  [1, 1], 0, "andnpd", "DFmode", "gen_rtx_ANDNOT"],
  #["rfx.dst: AND_SF(NOT_SF(rfx.src1) | MEM_SF(addr.src2))",  [1, 1], 0, "andnps", "SFmode", "gen_rtx_ANDNOT"],

  #
  # TODO: it isn't clear when the FLT family (including FTE and FLE) come into the IL.
  #
  # FGE needs swapped operands from normal (eg, needs (rd, rdx.src2));
  # FGE uses wrapper function gen_rtx_LT_swap and gen_rtx_LE_swap
  # We also need to interchange uses of $src1 and $src2 in rules from normal.
  # Use a plug operator @ which is ``reverse ,'' (2nd | alternative.src2).
  #
  # This is a complete tarpit.
  #

  #
  # TODO vector (packed.src2) variants of the comparisons
  #

  ["rdx.dst: UNLT_ALL(rdx.src1 , rd.src2)", [1, 1], 1, 0, "cmpltsd",  "DFmode", "gen_rtx_UNLT"],
  ["rdx.dst: UNLE_ALL(rdx.src1 , rd.src2)", [1, 1], 1, 0, "cmplesd",  "DFmode", "gen_rtx_UNLE"],
  ["rdx.dst: UNEQ_ALL(rdx.src1 | rd.src2)", [1, 1], 1, 1, "cmpeqsd",  "DFmode", "gen_rtx_UNEQ"],
  ["rdx.dst: LTGT_ALL(rdx.src1 | rd.src2)", [1, 1], 1, 1, "cmpneqsd", "DFmode", "gen_rtx_LTGT"],
  ["rdx.dst: UNGE_ALL(rdx.src1 , rd.src2)", [1, 1], 1, 0, "cmpnltsd", "DFmode", "gen_rtx_UNGE"],
  ["rdx.dst: UNGT_ALL(rdx.src1 , rd.src2)", [1, 1], 1, 0, "cmpnlesd", "DFmode", "gen_rtx_UNGT"],

  ["rfx.dst: UNLT_ALL(rfx.src1 , rf.src2)", [1, 1], 1, 0, "cmpltss",  "SFmode", "gen_rtx_UNLT"],
  ["rfx.dst: UNLE_ALL(rfx.src1 , rf.src2)", [1, 1], 1, 0, "cmpless",  "SFmode", "gen_rtx_UNLE"],
  ["rfx.dst: UNEQ_ALL(rfx.src1 | rf.src2)", [1, 1], 1, 1, "cmpeqss",  "SFmode", "gen_rtx_UNEQ"],
  ["rfx.dst: LTGT_ALL(rfx.src1 | rf.src2)", [1, 1], 1, 1, "cmpneqss", "SFmode", "gen_rtx_LTGT"],
  ["rfx.dst: UNGE_ALL(rfx.src1 , rf.src2)", [1, 1], 1, 0, "cmpnltss", "SFmode", "gen_rtx_UNGE"],
  ["rfx.dst: UNGT_ALL(rfx.src1 , rf.src2)", [1, 1], 1, 0, "cmpnless", "SFmode", "gen_rtx_UNGT"],

  ["rdx.dst: UNLT_ALL(rdx.src1 , MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpltsd",  "DFmode", "gen_rtx_UNLT"],
  ["rdx.dst: UNLE_ALL(rdx.src1 , MEM_DF(addr.src2))", [1, 1], 0, 0, "cmplesd",  "DFmode", "gen_rtx_UNLE"],
  ["rdx.dst: UNEQ_ALL(rdx.src1 | MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpeqsd",  "DFmode", "gen_rtx_UNEQ"],
  ["rdx.dst: LTGT_ALL(rdx.src1 | MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpneqsd", "DFmode", "gen_rtx_LTGT"],
  ["rdx.dst: UNGE_ALL(rdx.src1 , MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpnltsd", "DFmode", "gen_rtx_UNGE"],
  ["rdx.dst: UNGT_ALL(rdx.src1 , MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpnlesd", "DFmode", "gen_rtx_UNGT"],

  ["rfx.dst: UNLT_ALL(rfx.src1 , MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpltss",  "SFmode", "gen_rtx_UNLT"],
  ["rfx.dst: UNLE_ALL(rfx.src1 , MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpless",  "SFmode", "gen_rtx_UNLE"],
  ["rfx.dst: UNEQ_ALL(rfx.src1 | MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpeqss",  "SFmode", "gen_rtx_UNEQ"],
  ["rfx.dst: LTGT_ALL(rfx.src1 | MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpneqss", "SFmode", "gen_rtx_LTGT"],
  ["rfx.dst: UNGE_ALL(rfx.src1 , MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpnltss", "SFmode", "gen_rtx_UNGE"],
  ["rfx.dst: UNGT_ALL(rfx.src1 , MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpnless", "SFmode", "gen_rtx_UNGT"],

  #
  # it isn't clear when/if the F..._DF and F..._SF operators will appear
  #
  ["rdx.dst: FLT_DF(rdx.src1 , rd.src2)", [1, 1], 1, 0, "cmpltsd", "DFmode", "gen_rtx_LT"],
  ["rdx.dst: FLE_DF(rdx.src1 , rd.src2)", [1, 1], 1, 0, "cmplesd", "DFmode", "gen_rtx_LE"],
  ["rdx.dst: FEQ_DF(rdx.src1 | rd.src2)", [1, 1], 1, 1, "cmpeqsd", "DFmode", "gen_rtx_EQ"],
  ["rdx.dst: FNEQ_DF(rdx.src1| rd.src2)", [1, 1], 1, 1, "cmpnesd", "DFmode", "gen_rtx_NE"],
  ["rdx.dst: FGE_DF(rdx.src1 @ rd.src2)", [1, 1], 1, 0, "cmpltsd", "DFmode", "gen_rtx_LT_swap"],
  ["rdx.dst: FGT_DF(rdx.src1 @ rd.src2)", [1, 1], 1, 0, "cmplesd", "DFmode", "gen_rtx_LE_swap"],

  ["rfx.dst: FLT_SF(rfx.src1 , rf.src2)", [1, 1], 1, 0, "cmpltss", "SFmode", "gen_rtx_LT"],
  ["rfx.dst: FLE_SF(rfx.src1 , rf.src2)", [1, 1], 1, 0, "cmpless", "SFmode", "gen_rtx_LE"],
  ["rfx.dst: FEQ_SF(rfx.src1 | rf.src2)", [1, 1], 1, 1, "cmpeqss", "SFmode", "gen_rtx_EQ"],
  ["rfx.dst: FNEQ_SF(rfx.src1| rf.src2)", [1, 1], 1, 1, "cmpneqss", "SFmode", "gen_rtx_NE"],
  ["rfx.dst: FGE_SF(rfx.src1 @ rf.src2)", [1, 1], 1, 0, "cmpltss", "SFmode", "gen_rtx_LT_swap"],
  ["rfx.dst: FGT_SF(rfx.src1 @ rf.src2)", [1, 1], 1, 0, "cmpless", "SFmode", "gen_rtx_LE_swap"],

  ["rdx.dst: FLT_DF(rdx.src1 , MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpltsd", "DFmode", "gen_rtx_LT"],
  ["rdx.dst: FLE_DF(rdx.src1 , MEM_DF(addr.src2))", [1, 1], 0, 0, "cmplesd", "DFmode", "gen_rtx_LE"],
  ["rdx.dst: FEQ_DF(rdx.src1 | MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpeqsd", "DFmode", "gen_rtx_EQ"],
  ["rdx.dst: FNEQ_DF(rdx.src1| MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpneqsd", "DFmode", "gen_rtx_NE"],
  ["rdx.dst: FGE_DF(rdx.src1 @ MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpltsd", "DFmode", "gen_rtx_LT_swap"],
  ["rdx.dst: FGT_DF(rdx.src1 @ MEM_DF(addr.src2))", [1, 1], 0, 0, "cmplesd", "DFmode", "gen_rtx_LE_swap"],

  ["rfx.dst: FLT_SF(rfx.src1 , MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpltss", "SFmode", "gen_rtx_LT"],
  ["rfx.dst: FLE_SF(rfx.src1 , MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpless", "SFmode", "gen_rtx_LE"],
  ["rfx.dst: FEQ_SF(rfx.src1 | MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpeqss", "SFmode", "gen_rtx_EQ"],
  ["rfx.dst: FNEQ_SF(rfx.src1| MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpneqss", "SFmode", "gen_rtx_NE"],
  ["rfx.dst: FGE_SF(rfx.src1 @ MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpltss", "SFmode", "gen_rtx_LT_swap"],
  ["rfx.dst: FGT_SF(rfx.src1 @ MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpless", "SFmode", "gen_rtx_LE_swap"],

  ["rdx.dst: LT_ALL(rdx.src1 , rd.src2)", [1, 1], 1, 0, "cmpltsd", "DFmode", "gen_rtx_LT"],
  ["rdx.dst: LE_ALL(rdx.src1 , rd.src2)", [1, 1], 1, 0, "cmplesd", "DFmode", "gen_rtx_LE"],
  ["rdx.dst: EQ_ALL(rdx.src1 | rd.src2)", [1, 1], 1, 1, "cmpeqsd", "DFmode", "gen_rtx_EQ"],
  ["rdx.dst: NE_ALL(rdx.src1 | rd.src2)", [1, 1], 1, 1, "cmpnesd", "DFmode", "gen_rtx_NE"],
  ["rdx.dst: GE_ALL(rdx.src1 @ rd.src2)", [1, 1], 1, 0, "cmpltsd", "DFmode", "gen_rtx_LT_swap"],
  ["rdx.dst: GT_ALL(rdx.src1 @ rd.src2)", [1, 1], 1, 0, "cmplesd", "DFmode", "gen_rtx_LE_swap"],

  ["rfx.dst: LT_ALL(rfx.src1 , rf.src2)", [1, 1], 1, 0, "cmpltss", "SFmode", "gen_rtx_LT"],
  ["rfx.dst: LE_ALL(rfx.src1 , rf.src2)", [1, 1], 1, 0, "cmpless", "SFmode", "gen_rtx_LE"],
  ["rfx.dst: EQ_ALL(rfx.src1 | rf.src2)", [1, 1], 1, 1, "cmpeqss", "SFmode", "gen_rtx_EQ"],
  ["rfx.dst: NE_ALL(rfx.src1 | rf.src2)", [1, 1], 1, 1, "cmpneqss","SFmode", "gen_rtx_NE"],
  ["rfx.dst: GE_ALL(rfx.src1 @ rf.src2)", [1, 1], 1, 0, "cmpltss", "SFmode", "gen_rtx_LT_swap"],
  ["rfx.dst: GT_ALL(rfx.src1 @ rf.src2)", [1, 1], 1, 0, "cmpless", "SFmode", "gen_rtx_LE_swap"],

  ["rdx.dst: LT_ALL(rdx.src1 , MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpltsd", "DFmode", "gen_rtx_LT"],
  ["rdx.dst: LE_ALL(rdx.src1 , MEM_DF(addr.src2))", [1, 1], 0, 0, "cmplesd", "DFmode", "gen_rtx_LE"],
  ["rdx.dst: EQ_ALL(rdx.src1 | MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpeqsd", "DFmode", "gen_rtx_EQ"],
  ["rdx.dst: NE_ALL(rdx.src1 | MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpneqsd", "DFmode", "gen_rtx_NE"],
  ["rdx.dst: GE_ALL(rdx.src1 @ MEM_DF(addr.src2))", [1, 1], 0, 0, "cmpltsd", "DFmode", "gen_rtx_LT_swap"],
  ["rdx.dst: GT_ALL(rdx.src1 @ MEM_DF(addr.src2))", [1, 1], 0, 0, "cmplesd", "DFmode", "gen_rtx_LE_swap"],

  ["rfx.dst: LT_ALL(rfx.src1 , MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpltss", "SFmode", "gen_rtx_LT"],
  ["rfx.dst: LE_ALL(rfx.src1 , MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpless", "SFmode", "gen_rtx_LE"],
  ["rfx.dst: EQ_ALL(rfx.src1 | MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpeqss", "SFmode", "gen_rtx_EQ"],
  ["rfx.dst: NE_ALL(rfx.src1 | MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpneqss", "SFmode", "gen_rtx_NE"],
  ["rfx.dst: GE_ALL(rfx.src1 @ MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpltss", "SFmode", "gen_rtx_LT_swap"],
  ["rfx.dst: GT_ALL(rfx.src1 @ MEM_SF(addr.src2))", [1, 1], 0, 0, "cmpless", "SFmode", "gen_rtx_LE_swap"],

  ["rdx.dst: ICG_UNSPEC_IEEE_MAX_DF(rdx.src1  | rd.src2)", [1, 1], 1, 1, "maxsd", "DFmode", "gen_rtx_IEEE_MAX_helper"],
  ["rfx.dst: ICG_UNSPEC_IEEE_MAX_SF(rfx.src1  | rf.src2)", [1, 1], 1, 1, "maxss", "SFmode", "gen_rtx_IEEE_MAX_helper"],
  ["rdx.dst: ICG_UNSPEC_IEEE_MAX_VDF(rdx.src1 | rd.src2)", [1, 1], 1, 1, "maxpd", "V2DFmode", "gen_rtx_IEEE_MAX_helper"],
  ["rdx.dst: ICG_UNSPEC_IEEE_MAX_VSF(rdx.src1 | rd.src2)", [1, 1], 1, 1, "maxps", "V4SFmode", "gen_rtx_IEEE_MAX_helper"],

  ["rdx.dst: ICG_UNSPEC_IEEE_MAX_DF(rdx.src1  | MEM_DF(addr.src2))",  [1, 1], 0, 0, "maxsd", "DFmode", "gen_rtx_IEEE_MAX_helper"],
  ["rfx.dst: ICG_UNSPEC_IEEE_MAX_SF(rfx.src1  | MEM_SF(addr.src2))",  [1, 1], 0, 0, "maxss", "SFmode", "gen_rtx_IEEE_MAX_helper"],
  ["rdx.dst: ICG_UNSPEC_IEEE_MAX_VDF(rdx.src1 | MEM_VDF(addr.src2))", [1, 1], 0, 0, "maxpd", "V2DFmode", "gen_rtx_IEEE_MAX_helper"],
  ["rdx.dst: ICG_UNSPEC_IEEE_MAX_VSF(rdx.src1 | MEM_VSF(addr.src2))", [1, 1], 0, 0, "maxps", "V4SFmode", "gen_rtx_IEEE_MAX_helper"],

  ["rdx.dst: ICG_UNSPEC_IEEE_MIN_DF(rdx.src1  | rd.src2)", [1, 1], 1, 1, "minsd", "DFmode", "gen_rtx_IEEE_MIN_helper"],
  ["rfx.dst: ICG_UNSPEC_IEEE_MIN_SF(rfx.src1  | rf.src2)", [1, 1], 1, 1, "minss", "SFmode", "gen_rtx_IEEE_MIN_helper"],
  ["rdx.dst: ICG_UNSPEC_IEEE_MIN_VDF(rdx.src1 | rd.src2)", [1, 1], 1, 1, "minpd", "V2DFmode", "gen_rtx_IEEE_MIN_helper"],
  ["rdx.dst: ICG_UNSPEC_IEEE_MIN_VSF(rdx.src1 | rd.src2)", [1, 1], 1, 1, "minps", "V4SFmode", "gen_rtx_IEEE_MIN_helper"],

  ["rdx.dst: ICG_UNSPEC_IEEE_MIN_DF(rdx.src1  | MEM_DF(addr.src2))",  [1, 1], 0, 0, "minsd", "DFmode", "gen_rtx_IEEE_MIN_helper"],
  ["rfx.dst: ICG_UNSPEC_IEEE_MIN_SF(rfx.src1  | MEM_SF(addr.src2))",  [1, 1], 0, 0, "minss", "SFmode", "gen_rtx_IEEE_MIN_helper"],
  ["rdx.dst: ICG_UNSPEC_IEEE_MIN_VDF(rdx.src1 | MEM_VDF(addr.src2))", [1, 1], 0, 0, "minpd", "V2DFmode", "gen_rtx_IEEE_MIN_helper"],
  ["rdx.dst: ICG_UNSPEC_IEEE_MIN_VSF(rdx.src1 | MEM_VSF(addr.src2))", [1, 1], 0, 0, "minps", "V4SFmode", "gen_rtx_IEEE_MIN_helper"],


], """
  $rule $cost
  supairs {
    suOrder2($dst, $src1, $src2, kid, kids);
  },
  names {
    $dst->rx = $src1->rx;
  },
  final {
    $dst->rx = $src1->rx;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
    if ($isRR) {
      sparseset_set_bit(live, find($src2->r));
    }
    else add_addr(live, $src2);
  },
  remat {
    flags = 0;
  },
  costs {
    if ($isRR)
      memorable($src2->r);
    if ($isCommute)
      memorable($src1->rx);
    else
      forgettable($src1->rx);
  },
  debug {
    if ($isRR) {
      dumpRR("$opcode", $src2->r, 'x', $dst->rx, 'x');
    } else {
      dumpMR("$opcode", $src2, $dst->rx, 'x');
    }
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $src1->rx);
    rtx src = $isRR
      ? $rtx_builder($rtx_mode, dst, gen_rtx_REG($rtx_mode, $src2->r))
      : $rtx_builder($rtx_mode, dst, gen_rtx_MEM($rtx_mode, $src2->rtl));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")

#})]
