# {([
# -*- mode: python -*-
#
# Copyright (c) 2008 Google Inc. All rights reserved.
#
import plug

# global_print_plug_decls = 0; execfile("x86-64.gr.py")


#-------------------------------------------------------------
# lhs rules

plug.plugrule3("intlhsreg", [
  ["rule", "cost"],

  ["lhs64.dst : REGX_DI", [0, 0]],
  ["lhs32.dst : REGX_SI", [0, 0]],
  ["lhs16.dst : REGX_HI", [0, 0]],
  ["lhs8.dst  : REGX_QI", [0, 0]],
  ["lhs64.dst : REG_DI",  [0, 0]],
  ["lhs32.dst : REG_SI",  [0, 0]],
  ["lhs16.dst : REG_HI",  [0, 0]],
  ["lhs8.dst  : REG_QI",  [0, 0]],

], """
  $rule $cost
  names {
    $dst->spilled = false;
  },
  kinds {
    icg_reg_vector[$dst->r].kind = INT_REGISTER;
  },
  remat {
    flags |= LHS_REG;
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



plug.plugrule3("intlhsstore", [
  ["rule", "cost", "rtx_mode"],

  ["lhs64.lhs : MEM_DI(addr.dst)", [3, 0], "DImode"],
  ["lhs32.lhs : MEM_SI(addr.dst)", [3, 0], "SImode"],
  ["lhs16.lhs : MEM_HI(addr.dst)", [3, 0], "HImode"],
  ["lhs8.lhs  : MEM_QI(addr.dst)", [3, 0], "QImode"],

  #
  # Added 10Nov2008
  # in some cases we'll see IL where floating point values
  # are held in 64-bit GPRs.
  # This happens with the translation of CONST_DOUBLE,
  # where the 64-bit constant is materialized into a GPR
  # with a movabsq (say) and then treated as a double.
  # It isn't clear why/when the front end will give
  # us trees holding CONST_DOUBLE, which we turn into CONST_64P.
  #
  ["lhs64.lhs : MEM_DF(addr.dst)", [0, 0], "DImode"],
  ["lhs32.lhs : MEM_SF(addr.dst)", [0, 0], "SImode"],

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
  remat {
    flags = 0;
  },
  emit {
    $lhs->rtl = $dst->rtl;
  };
""")


#
# integer assignment
#
plug.plugrule3 ("intassignment", [
  ["rule", "cost", "mopcode", "mmode", "mchar", "ropcode", "rmode", "rchar"],

  ["stmt : SET_ALL(lhs64.dst, r64.src)", [0, 0], "movq", "DImode", 'q', "movq", "DImode", 'q'],
  ["stmt : SET_ALL(lhs32.dst, r32.src)", [0, 0], "movl", "SImode", 'l', "movl", "SImode", 'l'],
  ["stmt : SET_ALL(lhs16.dst, r16.src)", [0, 0], "movw", "HImode", 'w', "movl", "SImode", 'l'],
  ["stmt : SET_ALL(lhs8.dst,  r8.src)",  [0, 0], "movb", "QImode", 'b', "movl", "SImode", 'l'],

  # paradoxical subregs in the lval context, easy
  ["stmt : SET_ALL(SUBREG_DI(lhs32.dst, CONST_0), r64.src)", [0, 0], "movl", "SImode", 'l', "movl", "SImode", 'l'],
  ["stmt : SET_ALL(SUBREG_DI(lhs16.dst, CONST_0), r64.src)", [0, 0], "movw", "HImode", 'w', "movl", "SImode", 'l'],
  ["stmt : SET_ALL(SUBREG_DI(lhs8.dst,  CONST_0), r64.src)", [0, 0], "movb", "QImode", 'b', "movl", "SImode", 'l'],
  ["stmt : SET_ALL(SUBREG_SI(lhs16.dst, CONST_0), r32.src)", [0, 0], "movw", "HImode", 'w', "movl", "SImode", 'l'],
  ["stmt : SET_ALL(SUBREG_SI(lhs8.dst,  CONST_0), r32.src)", [0, 0], "movb", "QImode", 'b', "movl", "SImode", 'l'],
  ["stmt : SET_ALL(SUBREG_HI(lhs8.dst,  CONST_0), r16.src)", [0, 0], "movb", "QImode", 'b', "movl", "SImode", 'l'],

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
  costs {
    if (!$dst->spilled)
      cost_copy($src->r, $dst->r);
  },
  debug {
    if ($dst->spilled)
      dumpRM("$mopcode", $src->r, '$mchar', $dst);
    else
      dump_copy("$ropcode", $src->r, $dst->r, '$rchar');
  },
  emit {
    if ($dst->spilled) {
      rtx src = gen_rtx_REG($mmode, $src->r);
      rtx dst = gen_rtx_MEM($mmode, $dst->rtl);
      icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
    }
    else {
      rtx src = gen_rtx_REG($rmode, $src->r);
      rtx dst = gen_rtx_REG($rmode, $dst->r);
      icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
    }
  };
""")


plug.plugrule3("immstore", [
  ["rule", "cost", "opcode", "rtx_mode"],

  ["stmt : SET_ALL(MEM_DI(addr), imm32.const)", [3, 2], "movq", "DImode"],
  ["stmt : SET_ALL(MEM_SI(addr), imm32.const)", [3, 1], "movl", "SImode"],
  ["stmt : SET_ALL(MEM_HI(addr), imm16.const)", [3, 2], "movw", "HImode"],
  ["stmt : SET_ALL(MEM_QI(addr), imm8.const)",  [3, 1], "movb", "QImode"],

], """
  $rule $cost
  build {
    add_addr(live, $addr);
  },
  remat {
    flags = 0;
  },
  debug {
    dumpIM("$opcode", $const, $addr);
  },
  emit {
    rtx src = gen_rtx_imm_constant($const->val, $const->a.string, $const->rtl);
    rtx dst = gen_rtx_MEM($rtx_mode, $addr->rtl);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")




#-------------------------------------------------------------
# reg-reg chain rules

plug.plugrule3("regdst", [
  ["rule", "cost"],

  ["r64.dst : REG_DI",  [0, 0]],
  ["r32.dst : REG_SI",  [0, 0]],
  ["r16.dst : REG_HI",  [0, 0]],
  ["r8.dst  : REG_QI",  [0, 0]],

], """
  $rule $cost
  kinds {
    icg_reg_vector[$dst->r].kind = INT_REGISTER;
  },
  costs {
    cost_load($dst->r);
  },
  spill {
    dirty |= make_spill_code(find($dst->r), $dst);
  },
  final {
    $dst->r = icg_reg_vector[find($dst->r)].color;
  };
""")

#
# This block differs from the rule above in that it has an supairs field.
#
plug.plugrule3("regsrc", [
  ["rule", "cost"],

  ["r64x.dst : REGX_DI", [0, 0]],
  ["r32x.dst : REGX_SI", [0, 0]],
  ["r16x.dst : REGX_HI", [0, 0]],
  ["r8x.dst  : REGX_QI", [0, 0]],

], """
  $rule $cost
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  costs {
    cost_load($dst->rx);
  },
  supairs {
    $dst->extra = 0;
    $dst->freed = 0;
  },
  spill {
    dirty |= make_spill_code(find($dst->rx), $dst);
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  };
""")

#-------------------------------------------------------------
# register-register chain rules

# this set of rules acts to rename, throwing away a bit of
# the information carried in the name of the non-terminal

plug.plugrule3 ("regchaindiscard", [
  ["rule", "cost"],

# here we throw away the fact that the RHS is zero or sign extended

  ["r32.dst : rz32.src", [0, 0]],
  ["r16.dst : rz16.src", [0, 0]],
  ["r16.dst : rs16.src", [0, 0]],
  ["r8.dst  : rz8.src", [0, 0]],
  ["r8.dst  : rs8.src", [0, 0]],

  ["r32x.dst : rz32x.src", [0, 0]],
  ["r16x.dst : rz16x.src", [0, 0]],
  ["r16x.dst : rs16x.src", [0, 0]],
  ["r8x.dst  : rz8x.src", [0, 0]],
  ["r8x.dst  : rs8x.src", [0, 0]],

  ["ccr32x.dst : ccrz32x.src", [0, 0]],
  ["ccr16x.dst : ccrz16x.src", [0, 0]],
  ["ccr16x.dst : ccrs16x.src", [0, 0]],
  ["ccr8x.dst  : ccrz8x.src", [0, 0]],
  ["ccr8x.dst  : ccrs8x.src", [0, 0]],

# here we throw away the fact that the condition code is set

  ["r64x.dst  : ccr64x.src", [0, 0]],
  ["r32x.dst  : ccr32x.src", [0, 0]],
  ["rz32x.dst : ccrz32x.src", [0, 0]],
  ["r16x.dst  : ccr16x.src", [0, 0]],
  ["rz16x.dst : ccrz16x.src", [0, 0]],
  ["rs16x.dst : ccrs16x.src", [0, 0]],
  ["r8x.dst   : ccr8x.src", [0, 0]],
  ["rz8x.dst  : ccrz8x.src", [0, 0]],
  ["rs8x.dst  : ccrs8x.src", [0, 0]],

], """
  $rule $cost
    ;
""")

#
# Throw away the upper part of a register.
# Contrast TRUNCATE with SUBREG.  It isn't clear why TRUNCATE is used
# in one context and SUBREG in another.
#

plug.plugrule3 ("regtruncate0", [
  ["rule", "cost"],

  ["r32.dst:  TRUNCATE_SI(r64.src)",  [0, 0]],
  ["r16.dst:  TRUNCATE_HI(r64.src)",  [0, 0]],
  ["r8.dst:   TRUNCATE_QI(r64.src)",  [0, 0]],
  ["r16.dst:  TRUNCATE_HI(r32.src)",  [0, 0]],
  ["r8.dst:   TRUNCATE_QI(r32.src)",  [0, 0]],
  ["r8.dst:   TRUNCATE_QI(r16.src)",  [0, 0]],

  # normal subregs in rval context
  ["r32.dst : SUBREG_SI(r64.src, CONST_0)", [0, 0]],
  ["r16.dst : SUBREG_HI(r64.src, CONST_0)", [0, 0]],
  ["r8.dst  : SUBREG_QI(r64.src, CONST_0)", [0, 0]],
  ["r16.dst : SUBREG_HI(r32.src, CONST_0)", [0, 0]],
  ["r8.dst  : SUBREG_QI(r32.src, CONST_0)", [0, 0]],
  ["r8.dst  : SUBREG_QI(r16.src, CONST_0)", [0, 0]],

  # paradoxical subregs in rval context
  ["r64.dst : SUBREG_DI(r32.src, CONST_0)", [0, 0]],
  ["r64.dst : SUBREG_DI(r16.src, CONST_0)", [0, 0]],
  ["r64.dst : SUBREG_DI(r8.src,  CONST_0)", [0, 0]],
  ["r32.dst : SUBREG_SI(r16.src, CONST_0)", [0, 0]],
  ["r32.dst : SUBREG_SI(r8.src,  CONST_0)", [0, 0]],
  ["r16.dst : SUBREG_HI(r8.src,  CONST_0)", [0, 0]],

], """
  $rule $cost
  names {
    $dst->r = $src->r;
  },
  final {
    $dst->r = $src->r;
  },
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  };
""")

plug.plugrule3 ("regtruncate1", [
  ["rule", "cost"],
  ["r32x.dst: TRUNCATE_SI(r64x.src)", [0, 0]],
  ["r16x.dst: TRUNCATE_HI(r64x.src)", [0, 0]],
  ["r8x.dst:  TRUNCATE_QI(r64x.src)", [0, 0]],
  ["r16x.dst: TRUNCATE_HI(r32x.src)", [0, 0]],
  ["r8x.dst:  TRUNCATE_QI(r32x.src)", [0, 0]],
  ["r8x.dst:  TRUNCATE_QI(r16x.src)", [0, 0]],

  ["r32x.dst : SUBREG_SI(r64x.src, CONST_0)", [0, 0]],
  ["r16x.dst : SUBREG_HI(r64x.src, CONST_0)", [0, 0]],
  ["r8x.dst  : SUBREG_QI(r64x.src, CONST_0)", [0, 0]],
  ["r16x.dst : SUBREG_HI(r32x.src, CONST_0)", [0, 0]],
  ["r8x.dst  : SUBREG_QI(r32x.src, CONST_0)", [0, 0]],
  ["r8x.dst  : SUBREG_QI(r16x.src, CONST_0)", [0, 0]],

  ["r64x.dst : SUBREG_DI(r32x.src, CONST_0)", [0, 0]],
  ["r64x.dst : SUBREG_DI(r16x.src, CONST_0)", [0, 0]],
  ["r64x.dst : SUBREG_DI(r8x.src,  CONST_0)", [0, 0]],
  ["r32x.dst : SUBREG_SI(r16x.src, CONST_0)", [0, 0]],
  ["r32x.dst : SUBREG_SI(r8x.src,  CONST_0)", [0, 0]],
  ["r16x.dst : SUBREG_HI(r8x.src,  CONST_0)", [0, 0]],

], """
  $rule $cost
  names {
    $dst->rx = $src->rx;
  },
  final {
    $dst->rx = $src->rx;
  },
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  };
""")


# this set of rules lets us use an RX as if it were an R.

plug.plugrule3 ("regchainxsrc", [
  ["rule", "cost"],

  ["r64.dst  : r64x.src",  [0, 0]],
  ["r32.dst  : r32x.src",  [0, 0]],
  ["rz32.dst : rz32x.src", [0, 0]],
  ["r16.dst  : r16x.src",  [0, 0]],
  ["rz16.dst : rz16x.src", [0, 0]],
  ["rs16.dst : rs16x.src", [0, 0]],
  ["r8.dst   : r8x.src",   [0, 0]],
  ["rz8.dst  : rz8x.src",  [0, 0]],
  ["rs8.dst  : rs8x.src",  [0, 0]],

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
plug.plugrule3("regchainxdst", [
  ["rule", "cost", "opcode", "schar", "rtx_mode"],

  ["r64x.dst  : r64.src",  [1, 3], "movq", 'q', "DImode"],
  ["rz32x.dst : r32.src",  [1, 2], "movl", 'l', "SImode"],
  ["r16x.dst  : r16.src",  [1, 2], "movl", 'l', "SImode"],
  ["rz16x.dst : rz16.src", [1, 2], "movl", 'l', "SImode"],
  ["rs16x.dst : rs16.src", [1, 2], "movl", 'l', "SImode"],
  ["r8x.dst   : r8.src",   [1, 2], "movl", 'l', "SImode"],
  ["rz8x.dst  : rz8.src",  [1, 2], "movl", 'l', "SImode"],
  ["rs8x.dst  : rs8.src",  [1, 2], "movl", 'l', "SImode"],

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
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $dst->rx, $src->r);
  },
  build {
    add_copy_edges($dst->rx, $src->r, live);
  },
  costs {
    cost_copy($dst->rx, $src->r);
  },
  debug {
    dump_copy("$opcode", $src->r, $dst->rx, '$schar');
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    rtx src = gen_rtx_REG($rtx_mode, $src->r);
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")




# these are various NOPs that might be introduced when messing with spill code
plug.plugrule3("regchainnoprr", [
  ["rule", "cost"],

  ["r64.dst    : PLUS_DI(r64.src     | CONST_0)", [0, 0]],
  ["r32.dst    : PLUS_SI(r32.src     | CONST_0)", [0, 0]],
  ["r16.dst    : PLUS_HI(r16.src     | CONST_0)", [0, 0]],
  ["r8.dst     : PLUS_QI(r8.src      | CONST_0)", [0, 0]],
  ["rz32.dst   : PLUS_SI(rz32.src    | CONST_0)", [0, 0]],
  ["rz16.dst   : PLUS_HI(rz16.src    | CONST_0)", [0, 0]],
  ["rz8.dst    : PLUS_QI(rz8.src     | CONST_0)", [0, 0]],
  ["rs16.dst   : PLUS_HI(rs16.src    | CONST_0)", [0, 0]],
  ["rs8.dst    : PLUS_QI(rs8.src     | CONST_0)", [0, 0]],
  
  ["r64.dst    : MINUS_DI(r64.src    | CONST_0)", [0, 0]],
  ["r32.dst    : MINUS_SI(r32.src    | CONST_0)", [0, 0]],
  ["r16.dst    : MINUS_HI(r16.src    | CONST_0)", [0, 0]],
  ["r8.dst     : MINUS_QI(r8.src     | CONST_0)", [0, 0]],
  ["rz32.dst   : MINUS_SI(rz32.src   | CONST_0)", [0, 0]],
  ["rz16.dst   : MINUS_HI(rz16.src   | CONST_0)", [0, 0]],
  ["rz8.dst    : MINUS_QI(rz8.src    | CONST_0)", [0, 0]],
  ["rs16.dst   : MINUS_HI(rs16.src   | CONST_0)", [0, 0]],
  ["rs8.dst    : MINUS_QI(rs8.src    | CONST_0)", [0, 0]],
  
], """
  $rule $cost
  names {
    $dst->r = $src->r;
  },
  final {
    $dst->r = $src->r;
  },
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  };
""")




plug.plugrule3("regchainnoprxrx", [
  ["rule", "cost"],

  ["r64x.dst    : PLUS_DI(r64x.src     | CONST_0)", [0, 0]],
  ["r32x.dst    : PLUS_SI(r32x.src     | CONST_0)", [0, 0]],
  ["r16x.dst    : PLUS_HI(r16x.src     | CONST_0)", [0, 0]],
  ["r8x.dst     : PLUS_QI(r8x.src      | CONST_0)", [0, 0]],
  ["rz32x.dst   : PLUS_SI(rz32x.src    | CONST_0)", [0, 0]],
  ["rz16x.dst   : PLUS_HI(rz16x.src    | CONST_0)", [0, 0]],
  ["rz8x.dst    : PLUS_QI(rz8x.src     | CONST_0)", [0, 0]],
  ["rs16x.dst   : PLUS_HI(rs16x.src    | CONST_0)", [0, 0]],
  ["rs8x.dst    : PLUS_QI(rs8x.src     | CONST_0)", [0, 0]],
  
  ["ccr64x.dst  : PLUS_DI(ccr64x.src   | CONST_0)", [0, 0]],
  ["ccr32x.dst  : PLUS_SI(ccr32x.src   | CONST_0)", [0, 0]],
  ["ccr16x.dst  : PLUS_HI(ccr16x.src   | CONST_0)", [0, 0]],
  ["ccr8x.dst   : PLUS_QI(ccr8x.src    | CONST_0)", [0, 0]],
  ["ccrz32x.dst : PLUS_SI(ccrz32x.src  | CONST_0)", [0, 0]],
  ["ccrz16x.dst : PLUS_HI(ccrz16x.src  | CONST_0)", [0, 0]],
  ["ccrz8x.dst  : PLUS_QI(ccrz8x.src   | CONST_0)", [0, 0]],
  ["ccrs16x.dst : PLUS_HI(ccrs16x.src  | CONST_0)", [0, 0]],
  ["ccrs8x.dst  : PLUS_QI(ccrs8x.src   | CONST_0)", [0, 0]],

  ["r64x.dst    : MINUS_DI(r64x.src    | CONST_0)", [0, 0]],
  ["r32x.dst    : MINUS_SI(r32x.src    | CONST_0)", [0, 0]],
  ["r16x.dst    : MINUS_HI(r16x.src    | CONST_0)", [0, 0]],
  ["r8x.dst     : MINUS_QI(r8x.src     | CONST_0)", [0, 0]],
  ["rz32x.dst   : MINUS_SI(rz32x.src   | CONST_0)", [0, 0]],
  ["rz16x.dst   : MINUS_HI(rz16x.src   | CONST_0)", [0, 0]],
  ["rz8x.dst    : MINUS_QI(rz8x.src    | CONST_0)", [0, 0]],
  ["rs16x.dst   : MINUS_HI(rs16x.src   | CONST_0)", [0, 0]],
  ["rs8x.dst    : MINUS_QI(rs8x.src    | CONST_0)", [0, 0]],
  
  ["ccr64x.dst  : MINUS_DI(ccr64x.src  | CONST_0)", [0, 0]],
  ["ccr32x.dst  : MINUS_SI(ccr32x.src  | CONST_0)", [0, 0]],
  ["ccr16x.dst  : MINUS_HI(ccr16x.src  | CONST_0)", [0, 0]],
  ["ccr8x.dst   : MINUS_QI(ccr8x.src   | CONST_0)", [0, 0]],
  ["ccrz32x.dst : MINUS_SI(ccrz32x.src | CONST_0)", [0, 0]],
  ["ccrz16x.dst : MINUS_HI(ccrz16x.src | CONST_0)", [0, 0]],
  ["ccrz8x.dst  : MINUS_QI(ccrz8x.src  | CONST_0)", [0, 0]],
  ["ccrs16x.dst : MINUS_HI(ccrs16x.src | CONST_0)", [0, 0]],
  ["ccrs8x.dst  : MINUS_QI(ccrs8x.src  | CONST_0)", [0, 0]],

], """
  $rule $cost
  names {
    $dst->rx = $src->rx;
  },
  final {
    $dst->rx = $src->rx;
  },
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  };
""")



#-----------------------------------------------------------
# loading constants

# use XOR to zero register.
# Note that upper-half of word is automatically
# cleared with a 32-bit operation, saving a byte.
plug.plugrule3("zeroload", [
  ["rule", "cost", "opcode", "rtx_mode", "reg_kind", 'schar'],

  ["ccr64x.dst  : CONST_0", [1, 2], "xorl",  "SImode", "INT_REGISTER",   'l'],
  ["ccrz32x.dst : CONST_0", [1, 2], "xorl",  "SImode", "INT_REGISTER",   'l'],
  ["ccrz16x.dst : CONST_0", [1, 2], "xorl",  "SImode", "INT_REGISTER",   'l'],
  ["ccrs16x.dst : CONST_0", [1, 2], "xorl",  "SImode", "INT_REGISTER",   'l'],
  ["ccrz8x.dst  : CONST_0", [1, 2], "xorl",  "SImode", "INT_REGISTER",   'l'],
  ["ccrs8x.dst  : CONST_0", [1, 2], "xorl",  "SImode", "INT_REGISTER",   'l'],
  [   "rdx.dst  : CONST_0", [2, 4], "xorpd", "DFmode", "FLOAT_REGISTER", 'x'],
  [   "rfx.dst  : CONST_0", [2, 4], "xorps", "SFmode", "FLOAT_REGISTER", 'x'],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = $reg_kind;
  },
  supairs {
    $dst->extra = 1;
    $dst->freed = 0;
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
  },
  remat {
    flags |= RHS_REMAT;
  },
  debug {
    dumpRR("$opcode", $dst->rx, '$schar', $dst->rx, '$schar');
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src = gen_rtx_XOR($rtx_mode, dst, dst);
    icg_emit_clobber(gen_rtx_SET($rtx_mode, dst, src));
  };
""")


plug.plugrule3("immload", [
  ["rule", "cost", "opcode", "schar", "rtx_mode"],

  ["r64x.dst  : imm64.const", [1, 2], "movabsq", 'q', "DImode"],
  ["r64x.dst  : pos32.const", [1, 1], "movl",    'd', "SImode"],
  ["rz32x.dst : imm32.const", [1, 1], "movl",    'd', "SImode"],
  #
  # the latency on the next two rules is indeed 2 cycles
  # (because they imply a register merge)
  #
  ["r16x.dst : imm16.const",  [2, 2], "movw",    'w', "HImode"],
  ["r8x.dst  : imm8.const",   [2, 1], "movb",    'b', "QImode"],
  #
  # using imm32 here to ensure instruction size computes correctly
  #
  ["r16x.dst  : imm32.const", [1, 5-4], "movl",  'd', "SImode"],
  ["rz16x.dst : pos16.const", [1, 5-2], "movl",  'd', "SImode"],
  ["r8x.dst   : imm32.const", [1, 5-4], "movl",  'd', "SImode"],
  ["rz8x.dst  : pos8.const",  [1, 5-1], "movl",  'd', "SImode"],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    $dst->extra = 1;
    $dst->freed = 0;
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
  },
  remat {
    flags |= RHS_REMAT;
  },
  debug {
    dumpIR("$opcode", $const, $dst->rx, '$schar');
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    rtx src = gen_rtx_imm_constant($const->val, $const->a.string, $const->rtl);
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


#--------------------------------------------------------------
# plus


#
# The inc and dec instructions do not set the C (carry) flag.
# Consequently, they are not necessarily equivalent to an add instruction with literal 1.
# However, it seems that for the 6 signed comparisons that the C flag is not consulted.
# For the 6 unsigned compares, the C flag is consulted.
# TODO: there's no way to distinguish if the inc/dec instruction is used in an unsigned context.
#
plug.plugrule3("reginc", [
  ["rule", "cost", "opcode", "schar", "mode_slot", "rtx_builder"],

  ["r64x.dst  : PLUS_DI(r64x.src   | CONST_P1)", [1, 3], "incq", 'q', "DImode", "gen_rtx_PLUS"],
  ["rz32x.dst : PLUS_SI(r32x.src   | CONST_P1)", [1, 2], "incl", 'd', "SImode", "gen_rtx_PLUS"],
  ["r16x.dst  : PLUS_HI(r16x.src   | CONST_P1)", [1, 3], "incw", 'w', "HImode", "gen_rtx_PLUS"],
  ["r8x.dst   : PLUS_QI(r8x.src    | CONST_P1)", [1, 2], "incb", 'b', "QImode", "gen_rtx_PLUS"],

  ["r64x.dst  : PLUS_DI(r64x.src   | CONST_N1)", [1, 3], "decq", 'q', "DImode", "gen_rtx_MINUS"],
  ["rz32x.dst : PLUS_SI(r32x.src   | CONST_N1)", [1, 2], "decl", 'd', "SImode", "gen_rtx_MINUS"],
  ["r16x.dst  : PLUS_HI(r16x.src   | CONST_N1)", [1, 3], "decw", 'w', "HImode", "gen_rtx_MINUS"],
  ["r8x.dst   : PLUS_QI(r8x.src    | CONST_N1)", [1, 2], "decb", 'b', "QImode", "gen_rtx_MINUS"],

  # we can propagate the zero extension info
  ["rz16x.dst : PLUS_HI(rz16x.src  | CONST_P1)", [1, 3], "incw", 'w', "HImode", "gen_rtx_PLUS"],
  ["rz8x.dst  : PLUS_QI(rz8x.src   | CONST_P1)", [1, 2], "incb", 'b', "QImode", "gen_rtx_PLUS"],
  ["rz16x.dst : PLUS_HI(rz16x.src  | CONST_N1)", [1, 3], "decw", 'w', "HImode", "gen_rtx_MINUS"],
  ["rz8x.dst  : PLUS_QI(rz8x.src   | CONST_N1)", [1, 2], "decb", 'b', "QImode", "gen_rtx_MINUS"],

  # we can save a byte for 16-bit values (but we lose the zero extension)
  ["r16x.dst  : PLUS_HI(r16x.src   | CONST_P1)", [1, 2], "incl", 'd', "SImode", "gen_rtx_PLUS"],
  ["r16x.dst  : PLUS_HI(r16x.src   | CONST_N1)", [1, 2], "decl", 'd', "SImode", "gen_rtx_MINUS"],

  # repeat for MINUS
  ["r64x.dst  : MINUS_DI(r64x.src  | CONST_N1)", [1, 3], "incq", 'q', "DImode", "gen_rtx_PLUS"],
  ["rz32x.dst : MINUS_SI(r32x.src  | CONST_N1)", [1, 2], "incl", 'd', "SImode", "gen_rtx_PLUS"],
  ["r16x.dst  : MINUS_HI(r16x.src  | CONST_N1)", [1, 3], "incw", 'w', "HImode", "gen_rtx_PLUS"],
  ["r8x.dst   : MINUS_QI(r8x.src   | CONST_N1)", [1, 2], "incb", 'b', "QImode", "gen_rtx_PLUS"],

  ["r64x.dst  : MINUS_DI(r64x.src  | CONST_P1)", [1, 3], "decq", 'q', "DImode", "gen_rtx_MINUS"],
  ["rz32x.dst : MINUS_SI(r32x.src  | CONST_P1)", [1, 2], "decl", 'd', "SImode", "gen_rtx_MINUS"],
  ["r16x.dst  : MINUS_HI(r16x.src  | CONST_P1)", [1, 3], "decw", 'w', "HImode", "gen_rtx_MINUS"],
  ["r8x.dst   : MINUS_QI(r8x.src   | CONST_P1)", [1, 2], "decb", 'b', "QImode", "gen_rtx_MINUS"],

  ["rz16x.dst : MINUS_HI(rz16x.src | CONST_N1)", [1, 3], "incw", 'w', "HImode", "gen_rtx_PLUS"],
  ["rz8x.dst  : MINUS_QI(rz8x.src  | CONST_N1)", [1, 2], "incb", 'b', "QImode", "gen_rtx_PLUS"],
  ["rz16x.dst : MINUS_HI(rz16x.src | CONST_P1)", [1, 3], "decw", 'w', "HImode", "gen_rtx_MINUS"],
  ["rz8x.dst  : MINUS_QI(rz8x.src  | CONST_P1)", [1, 2], "decb", 'b', "QImode", "gen_rtx_MINUS"],

  ["r16x.dst  : MINUS_HI(r16x.src  | CONST_N1)", [1, 2], "incl", 'd', "SImode", "gen_rtx_PLUS"],
  ["r16x.dst  : MINUS_HI(r16x.src  | CONST_P1)", [1, 2], "decl", 'd', "SImode", "gen_rtx_MINUS"],

], """
  $rule $cost
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  },
  names {
    $dst->rx = $src->rx;
  },
  final {
    $dst->rx = $src->rx;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
  },
  costs {
    forgettable($src->rx);
  },
  debug {
    dumpR("$opcode", $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($mode_slot, $src->rx);
    rtx src = $rtx_builder($mode_slot, dst, GEN_INT(1));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")

plug.plugrule3("basicir", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder"],

  ["ccr64x.dst  : PLUS_DI(r64x.src | imm8.const)",  [1, 3], "addq", 'q', "DImode", "gen_rtx_PLUS"],
  ["ccr64x.dst  : PLUS_DI(r64x.src | imm32.const)", [1, 3], "addq", 'q', "DImode", "gen_rtx_PLUS"],
  ["ccrz32x.dst : PLUS_SI(r32x.src | imm8.const)",  [1, 2], "addl", 'l', "SImode", "gen_rtx_PLUS"],
  ["ccrz32x.dst : PLUS_SI(r32x.src | imm32.const)", [1, 2], "addl", 'l', "SImode", "gen_rtx_PLUS"],
  ["ccr16x.dst  : PLUS_HI(r16x.src | imm8.const)",  [1, 3], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccr16x.dst  : PLUS_HI(r16x.src | imm16.const)", [1, 3], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccr8x.dst   : PLUS_QI(r8x.src  | imm8.const)",  [1, 2], "addb", 'b', "QImode", "gen_rtx_PLUS"],

  # we can propagate the zero extension info
  ["ccrz16x.dst : PLUS_HI(rz16x.src | imm8.const)", [1, 3], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccrz16x.dst : PLUS_HI(rz16x.src | imm16.const)",[1, 3], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src  | imm8.const)", [1, 2], "addb", 'b', "QImode", "gen_rtx_PLUS"],

  # we can save a byte for 16-bit values (but we lose the condition code and zero extension)
  ["r16x.dst    : PLUS_HI(r16x.src | imm8.const)",  [1, 2], "addl", 'l', "SImode", "gen_rtx_PLUS"],

  # repeat for MINUS
  ["ccr64x.dst  : MINUS_DI(r64x.src, imm8.const)",  [1, 3], "subq", 'q', "DImode", "gen_rtx_MINUS"],
  ["ccr64x.dst  : MINUS_DI(r64x.src, imm32.const)", [1, 3], "subq", 'q', "DImode", "gen_rtx_MINUS"],
  ["ccrz32x.dst : MINUS_SI(r32x.src, imm8.const)",  [1, 2], "subl", 'l', "SImode", "gen_rtx_MINUS"],
  ["ccrz32x.dst : MINUS_SI(r32x.src, imm32.const)", [1, 2], "subl", 'l', "SImode", "gen_rtx_MINUS"],
  ["ccr16x.dst  : MINUS_HI(r16x.src, imm8.const)",  [1, 3], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccr16x.dst  : MINUS_HI(r16x.src, imm16.const)", [1, 3], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccr8x.dst   : MINUS_QI(r8x.src,  imm8.const)",  [1, 2], "subb", 'b', "QImode", "gen_rtx_MINUS"],

  ["ccrz16x.dst : MINUS_HI(rz16x.src, imm8.const)", [1, 3], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccrz16x.dst : MINUS_HI(rz16x.src, imm16.const)",[1, 3], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src,  imm8.const)", [1, 2], "subb", 'b', "QImode", "gen_rtx_MINUS"],

  ["  r16x.dst  : MINUS_HI(r16x.src, imm8.const)",  [1, 2], "subl", 'l', "SImode", "gen_rtx_MINUS"],

  # simple versions, optimizations will come later, to be automatically generated
  ["ccr64x.dst  : AND_DI(r64x.src | imm8.const)",  [1, 3], "andq", 'q', "DImode", "gen_rtx_AND"],
  ["ccr64x.dst  : AND_DI(r64x.src | imm32.const)", [1, 3], "andq", 'q', "DImode", "gen_rtx_AND"],
  ["ccrz32x.dst : AND_SI(r32x.src | imm8.const)",  [1, 2], "andl", 'l', "SImode", "gen_rtx_AND"],
  ["ccrz32x.dst : AND_SI(r32x.src | imm32.const)", [1, 2], "andl", 'l', "SImode", "gen_rtx_AND"],
  ["ccr16x.dst  : AND_HI(r16x.src | imm8.const)",  [1, 3], "andw", 'w', "HImode", "gen_rtx_AND"],
  ["ccr16x.dst  : AND_HI(r16x.src | imm16.const)", [1, 3], "andw", 'w', "HImode", "gen_rtx_AND"],
  ["ccr8x.dst   : AND_QI(r8x.src  | imm8.const)",  [1, 2], "andb", 'b', "QImode", "gen_rtx_AND"],

  # we can save a byte for 16-bit values (but we lose the condition code)
  ["r16x.dst    : AND_HI(r16x.src | imm8.const)",  [1, 2], "andl", 'l', "SImode", "gen_rtx_AND"],

  # repeat for XOR
  ["ccr64x.dst  : XOR_DI(r64x.src | imm8.const)",  [1, 3], "xorq", 'q', "DImode", "gen_rtx_XOR"],
  ["ccr64x.dst  : XOR_DI(r64x.src | imm32.const)", [1, 3], "xorq", 'q', "DImode", "gen_rtx_XOR"],
  ["ccrz32x.dst : XOR_SI(r32x.src | imm8.const)",  [1, 2], "xorl", 'l', "SImode", "gen_rtx_XOR"],
  ["ccrz32x.dst : XOR_SI(r32x.src | imm32.const)", [1, 2], "xorl", 'l', "SImode", "gen_rtx_XOR"],
  ["ccr16x.dst  : XOR_HI(r16x.src | imm8.const)",  [1, 3], "xorw", 'w', "HImode", "gen_rtx_XOR"],
  ["ccr16x.dst  : XOR_HI(r16x.src | imm16.const)", [1, 3], "xorw", 'w', "HImode", "gen_rtx_XOR"],
  ["ccr8x.dst   : XOR_QI(r8x.src  | imm8.const)",  [1, 2], "xorb", 'b', "QImode", "gen_rtx_XOR"],

  ["r16x.dst    : XOR_HI(r16x.src | imm8.const)",  [1, 2], "xorl", 'l', "SImode", "gen_rtx_XOR"],

  # repeat for IOR
  ["ccr64x.dst  : IOR_DI(r64x.src | imm8.const)",  [1, 3],  "orq", 'q', "DImode", "gen_rtx_IOR"],
  ["ccr64x.dst  : IOR_DI(r64x.src | imm32.const)", [1, 3],  "orq", 'q', "DImode", "gen_rtx_IOR"],
  ["ccrz32x.dst : IOR_SI(r32x.src | imm8.const)",  [1, 2],  "orl", 'l', "SImode", "gen_rtx_IOR"],
  ["ccrz32x.dst : IOR_SI(r32x.src | imm32.const)", [1, 2],  "orl", 'l', "SImode", "gen_rtx_IOR"],
  ["ccr16x.dst  : IOR_HI(r16x.src | imm8.const)",  [1, 3],  "orw", 'w', "HImode", "gen_rtx_IOR"],
  ["ccr16x.dst  : IOR_HI(r16x.src | imm16.const)", [1, 3],  "orw", 'w', "HImode", "gen_rtx_IOR"],
  ["ccr8x.dst   : IOR_QI(r8x.src  | imm8.const)",  [1, 2],  "orb", 'b', "QImode", "gen_rtx_IOR"],

  ["r16x.dst    : IOR_HI(r16x.src | imm8.const)",  [1, 2],  "orl", 'l', "SImode", "gen_rtx_IOR"],

], """
  $rule $cost
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  },
  names {
    $dst->rx = $src->rx;
  },
  final {
    $dst->rx = $src->rx;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
  },
  costs {
    forgettable($src->rx);
  },
  debug {
    dumpIR("$opcode", $const, $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $src->rx);
    rtx src = $rtx_builder($rtx_mode, dst, gen_rtx_imm_constant($const->val, $const->a.string, $const->rtl));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")

plug.plugrule3("basicmr", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder"],

  ["ccr64x.dst  : PLUS_DI(r64x.src  | MEM_DI(addr))", [4, 2], "addq", 'q', "DImode", "gen_rtx_PLUS"],
  ["ccrz32x.dst : PLUS_SI(r32x.src  | MEM_SI(addr))", [4, 1], "addl", 'l', "SImode", "gen_rtx_PLUS"],
  ["ccr16x.dst  : PLUS_HI(r16x.src  | MEM_HI(addr))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccr8x.dst   : PLUS_QI(r8x.src   | MEM_QI(addr))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],

  ["ccrz16x.dst : PLUS_HI(rz16x.src | MEM_HI(addr))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src  | MEM_QI(addr))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],


  ["ccrz32x.dst : PLUS_SI(r32x.src  | SUBREG_SI(MEM_DI(addr), CONST_0))", [4, 1], "addl", 'l', "SImode", "gen_rtx_PLUS"],
  ["ccr16x.dst  : PLUS_HI(r16x.src  | SUBREG_HI(MEM_DI(addr), CONST_0))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccr16x.dst  : PLUS_HI(r16x.src  | SUBREG_HI(MEM_SI(addr), CONST_0))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccr8x.dst   : PLUS_QI(r8x.src   | SUBREG_QI(MEM_DI(addr), CONST_0))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],
  ["ccr8x.dst   : PLUS_QI(r8x.src   | SUBREG_QI(MEM_SI(addr), CONST_0))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],
  ["ccr8x.dst   : PLUS_QI(r8x.src   | SUBREG_QI(MEM_HI(addr), CONST_0))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],

  ["ccrz16x.dst : PLUS_HI(rz16x.src | SUBREG_HI(MEM_DI(addr), CONST_0))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccrz16x.dst : PLUS_HI(rz16x.src | SUBREG_HI(MEM_SI(addr), CONST_0))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src  | SUBREG_QI(MEM_DI(addr), CONST_0))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src  | SUBREG_QI(MEM_SI(addr), CONST_0))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src  | SUBREG_QI(MEM_HI(addr), CONST_0))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],



  ["ccr64x.dst  : MINUS_DI(r64x.src,  NEG_DI(MEM_DI(addr)))", [4, 2], "addq", 'q', "DImode", "gen_rtx_PLUS"],
  ["ccrz32x.dst : MINUS_SI(r32x.src,  NEG_SI(MEM_SI(addr)))", [4, 1], "addl", 'l', "SImode", "gen_rtx_PLUS"],
  ["ccr16x.dst  : MINUS_HI(r16x.src,  NEG_HI(MEM_HI(addr)))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccr8x.dst   : MINUS_QI(r8x.src,   NEG_QI(MEM_QI(addr)))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],

  ["ccrz16x.dst : MINUS_HI(rz16x.src, NEG_HI(MEM_HI(addr)))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src,  NEG_QI(MEM_QI(addr)))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],


  ["ccrz32x.dst : MINUS_SI(r32x.src,  NEG_SI(SUBREG_SI(MEM_DI(addr), CONST_0)))", [4, 1], "addl", 'l', "SImode", "gen_rtx_PLUS"],
  ["ccr16x.dst  : MINUS_HI(r16x.src,  NEG_HI(SUBREG_HI(MEM_DI(addr), CONST_0)))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccr16x.dst  : MINUS_HI(r16x.src,  NEG_HI(SUBREG_HI(MEM_SI(addr), CONST_0)))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccr8x.dst   : MINUS_QI(r8x.src,   NEG_QI(SUBREG_QI(MEM_DI(addr), CONST_0)))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],
  ["ccr8x.dst   : MINUS_QI(r8x.src,   NEG_QI(SUBREG_QI(MEM_SI(addr), CONST_0)))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],
  ["ccr8x.dst   : MINUS_QI(r8x.src,   NEG_QI(SUBREG_QI(MEM_HI(addr), CONST_0)))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],

  ["ccrz16x.dst : MINUS_HI(rz16x.src, NEG_HI(SUBREG_HI(MEM_DI(addr), CONST_0)))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccrz16x.dst : MINUS_HI(rz16x.src, NEG_HI(SUBREG_HI(MEM_SI(addr), CONST_0)))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS"],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src,  NEG_QI(SUBREG_QI(MEM_DI(addr), CONST_0)))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src,  NEG_QI(SUBREG_QI(MEM_SI(addr), CONST_0)))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src,  NEG_QI(SUBREG_QI(MEM_HI(addr), CONST_0)))", [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS"],



  ["ccr64x.dst  : MINUS_DI(r64x.src,  MEM_DI(addr))", [4, 2], "subq", 'q', "DImode", "gen_rtx_MINUS"],
  ["ccrz32x.dst : MINUS_SI(r32x.src,  MEM_SI(addr))", [4, 1], "subl", 'l', "SImode", "gen_rtx_MINUS"],
  ["ccr16x.dst  : MINUS_HI(r16x.src,  MEM_HI(addr))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccr8x.dst   : MINUS_QI(r8x.src,   MEM_QI(addr))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],

  ["ccrz16x.dst : MINUS_HI(rz16x.src, MEM_HI(addr))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src,  MEM_QI(addr))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],


  ["ccrz32x.dst : MINUS_SI(r32x.src,  SUBREG_SI(MEM_DI(addr), CONST_0))", [4, 1], "subl", 'l', "SImode", "gen_rtx_MINUS"],
  ["ccr16x.dst  : MINUS_HI(r16x.src,  SUBREG_HI(MEM_DI(addr), CONST_0))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccr16x.dst  : MINUS_HI(r16x.src,  SUBREG_HI(MEM_SI(addr), CONST_0))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccr8x.dst   : MINUS_QI(r8x.src,   SUBREG_QI(MEM_DI(addr), CONST_0))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],
  ["ccr8x.dst   : MINUS_QI(r8x.src,   SUBREG_QI(MEM_SI(addr), CONST_0))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],
  ["ccr8x.dst   : MINUS_QI(r8x.src,   SUBREG_QI(MEM_HI(addr), CONST_0))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],

  ["ccrz16x.dst : MINUS_HI(rz16x.src, SUBREG_HI(MEM_DI(addr), CONST_0))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccrz16x.dst : MINUS_HI(rz16x.src, SUBREG_HI(MEM_SI(addr), CONST_0))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src,  SUBREG_QI(MEM_DI(addr), CONST_0))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src,  SUBREG_QI(MEM_SI(addr), CONST_0))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src,  SUBREG_QI(MEM_HI(addr), CONST_0))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],



  ["ccr64x.dst  : PLUS_DI(r64x.src  | NEG_DI(MEM_DI(addr)))", [4, 2], "subq", 'q', "DImode", "gen_rtx_MINUS"],
  ["ccrz32x.dst : PLUS_SI(r32x.src  | NEG_SI(MEM_SI(addr)))", [4, 1], "subl", 'l', "SImode", "gen_rtx_MINUS"],
  ["ccr16x.dst  : PLUS_HI(r16x.src  | NEG_HI(MEM_HI(addr)))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccr8x.dst   : PLUS_QI(r8x.src   | NEG_QI(MEM_QI(addr)))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],

  ["ccrz16x.dst : PLUS_HI(rz16x.src | NEG_HI(MEM_HI(addr)))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src  | NEG_QI(MEM_QI(addr)))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],


  ["ccrz32x.dst : PLUS_SI(r32x.src  | NEG_SI(SUBREG_SI(MEM_DI(addr), CONST_0)))", [4, 1], "subl", 'l', "SImode", "gen_rtx_MINUS"],
  ["ccr16x.dst  : PLUS_HI(r16x.src  | NEG_HI(SUBREG_HI(MEM_DI(addr), CONST_0)))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccr16x.dst  : PLUS_HI(r16x.src  | NEG_HI(SUBREG_HI(MEM_SI(addr), CONST_0)))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccr8x.dst   : PLUS_QI(r8x.src   | NEG_QI(SUBREG_QI(MEM_DI(addr), CONST_0)))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],
  ["ccr8x.dst   : PLUS_QI(r8x.src   | NEG_QI(SUBREG_QI(MEM_SI(addr), CONST_0)))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],
  ["ccr8x.dst   : PLUS_QI(r8x.src   | NEG_QI(SUBREG_QI(MEM_HI(addr), CONST_0)))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],

  ["ccrz16x.dst : PLUS_HI(rz16x.src | NEG_HI(SUBREG_HI(MEM_DI(addr), CONST_0)))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccrz16x.dst : PLUS_HI(rz16x.src | NEG_HI(SUBREG_HI(MEM_SI(addr), CONST_0)))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS"],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src  | NEG_QI(SUBREG_QI(MEM_DI(addr), CONST_0)))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src  | NEG_QI(SUBREG_QI(MEM_SI(addr), CONST_0)))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src  | NEG_QI(SUBREG_QI(MEM_HI(addr), CONST_0)))", [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS"],



  # imul does not set the condition codes in a useful way
  ["r64x.dst    : MULT_DI(r64x.src  | MEM_DI(addr))", [7, 2], "imulq", 'q', "DImode", "gen_rtx_MULT"],
  ["rz32x.dst   : MULT_SI(r32x.src  | MEM_SI(addr))", [6, 1], "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["r16x.dst    : MULT_HI(r16x.src  | MEM_HI(addr))", [6, 2], "imulw", 'w', "HImode", "gen_rtx_MULT"],

  ["rz16x.dst   : MULT_HI(rz16x.src | MEM_HI(addr))", [6, 2], "imulw", 'w', "HImode", "gen_rtx_MULT"],


  ["rz32x.dst   : MULT_SI(r32x.src  | SUBREG_SI(MEM_DI(addr), CONST_0))", [6, 1], "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["r16x.dst    : MULT_HI(r16x.src  | SUBREG_HI(MEM_DI(addr), CONST_0))", [6, 2], "imulw", 'w', "HImode", "gen_rtx_MULT"],
  ["r16x.dst    : MULT_HI(r16x.src  | SUBREG_HI(MEM_SI(addr), CONST_0))", [6, 2], "imulw", 'w', "HImode", "gen_rtx_MULT"],

  ["rz16x.dst   : MULT_HI(rz16x.src | SUBREG_HI(MEM_DI(addr), CONST_0))", [6, 2], "imulw", 'w', "HImode", "gen_rtx_MULT"],
  ["rz16x.dst   : MULT_HI(rz16x.src | SUBREG_HI(MEM_SI(addr), CONST_0))", [6, 2], "imulw", 'w', "HImode", "gen_rtx_MULT"],



  ["ccr64x.dst  : AND_DI(r64x.src  | MEM_DI(addr))", [4, 2], "andq", 'q', "DImode", "gen_rtx_AND"],
  ["ccrz32x.dst : AND_SI(r32x.src  | MEM_SI(addr))", [4, 1], "andl", 'l', "SImode", "gen_rtx_AND"],
  ["ccr16x.dst  : AND_HI(r16x.src  | MEM_HI(addr))", [4, 2], "andw", 'w', "HImode", "gen_rtx_AND"],
  ["ccr8x.dst   : AND_QI(r8x.src   | MEM_QI(addr))", [4, 1], "andb", 'b', "QImode", "gen_rtx_AND"],

  ["ccrz16x.dst : AND_HI(rz16x.src | MEM_HI(addr))", [4, 2], "andw", 'w', "HImode", "gen_rtx_AND"],
  ["ccrz8x.dst  : AND_QI(rz8x.src  | MEM_QI(addr))", [4, 1], "andb", 'b', "QImode", "gen_rtx_AND"],


  ["ccrz32x.dst : AND_SI(r32x.src  | SUBREG_SI(MEM_DI(addr), CONST_0))", [4, 1], "andl", 'l', "SImode", "gen_rtx_AND"],
  ["ccr16x.dst  : AND_HI(r16x.src  | SUBREG_HI(MEM_DI(addr), CONST_0))", [4, 2], "andw", 'w', "HImode", "gen_rtx_AND"],
  ["ccr16x.dst  : AND_HI(r16x.src  | SUBREG_HI(MEM_SI(addr), CONST_0))", [4, 2], "andw", 'w', "HImode", "gen_rtx_AND"],
  ["ccr8x.dst   : AND_QI(r8x.src   | SUBREG_QI(MEM_DI(addr), CONST_0))", [4, 1], "andb", 'b', "QImode", "gen_rtx_AND"],
  ["ccr8x.dst   : AND_QI(r8x.src   | SUBREG_QI(MEM_SI(addr), CONST_0))", [4, 1], "andb", 'b', "QImode", "gen_rtx_AND"],
  ["ccr8x.dst   : AND_QI(r8x.src   | SUBREG_QI(MEM_HI(addr), CONST_0))", [4, 1], "andb", 'b', "QImode", "gen_rtx_AND"],

  ["ccrz16x.dst : AND_HI(rz16x.src | SUBREG_HI(MEM_DI(addr), CONST_0))", [4, 2], "andw", 'w', "HImode", "gen_rtx_AND"],
  ["ccrz16x.dst : AND_HI(rz16x.src | SUBREG_HI(MEM_SI(addr), CONST_0))", [4, 2], "andw", 'w', "HImode", "gen_rtx_AND"],
  ["ccrz8x.dst  : AND_QI(rz8x.src  | SUBREG_QI(MEM_DI(addr), CONST_0))", [4, 1], "andb", 'b', "QImode", "gen_rtx_AND"],
  ["ccrz8x.dst  : AND_QI(rz8x.src  | SUBREG_QI(MEM_SI(addr), CONST_0))", [4, 1], "andb", 'b', "QImode", "gen_rtx_AND"],
  ["ccrz8x.dst  : AND_QI(rz8x.src  | SUBREG_QI(MEM_HI(addr), CONST_0))", [4, 1], "andb", 'b', "QImode", "gen_rtx_AND"],



  ["ccr64x.dst  : IOR_DI(r64x.src  | MEM_DI(addr))", [4, 2], "orq", 'q', "DImode", "gen_rtx_IOR"],
  ["ccrz32x.dst : IOR_SI(r32x.src  | MEM_SI(addr))", [4, 1], "orl", 'l', "SImode", "gen_rtx_IOR"],
  ["ccr16x.dst  : IOR_HI(r16x.src  | MEM_HI(addr))", [4, 2], "orw", 'w', "HImode", "gen_rtx_IOR"],
  ["ccr8x.dst   : IOR_QI(r8x.src   | MEM_QI(addr))", [4, 1], "orb", 'b', "QImode", "gen_rtx_IOR"],

  ["ccrz16x.dst : IOR_HI(rz16x.src | MEM_HI(addr))", [4, 2], "orw", 'w', "HImode", "gen_rtx_IOR"],
  ["ccrz8x.dst  : IOR_QI(rz8x.src  | MEM_QI(addr))", [4, 1], "orb", 'b', "QImode", "gen_rtx_IOR"],


  ["ccrz32x.dst : IOR_SI(r32x.src  | SUBREG_SI(MEM_DI(addr), CONST_0))", [4, 1], "orl", 'l', "SImode", "gen_rtx_IOR"],
  ["ccr16x.dst  : IOR_HI(r16x.src  | SUBREG_HI(MEM_DI(addr), CONST_0))", [4, 2], "orw", 'w', "HImode", "gen_rtx_IOR"],
  ["ccr16x.dst  : IOR_HI(r16x.src  | SUBREG_HI(MEM_SI(addr), CONST_0))", [4, 2], "orw", 'w', "HImode", "gen_rtx_IOR"],
  ["ccr8x.dst   : IOR_QI(r8x.src   | SUBREG_QI(MEM_DI(addr), CONST_0))", [4, 1], "orb", 'b', "QImode", "gen_rtx_IOR"],
  ["ccr8x.dst   : IOR_QI(r8x.src   | SUBREG_QI(MEM_SI(addr), CONST_0))", [4, 1], "orb", 'b', "QImode", "gen_rtx_IOR"],
  ["ccr8x.dst   : IOR_QI(r8x.src   | SUBREG_QI(MEM_HI(addr), CONST_0))", [4, 1], "orb", 'b', "QImode", "gen_rtx_IOR"],

  ["ccrz16x.dst : IOR_HI(rz16x.src | SUBREG_HI(MEM_DI(addr), CONST_0))", [4, 2], "orw", 'w', "HImode", "gen_rtx_IOR"],
  ["ccrz16x.dst : IOR_HI(rz16x.src | SUBREG_HI(MEM_SI(addr), CONST_0))", [4, 2], "orw", 'w', "HImode", "gen_rtx_IOR"],
  ["ccrz8x.dst  : IOR_QI(rz8x.src  | SUBREG_QI(MEM_DI(addr), CONST_0))", [4, 1], "orb", 'b', "QImode", "gen_rtx_IOR"],
  ["ccrz8x.dst  : IOR_QI(rz8x.src  | SUBREG_QI(MEM_SI(addr), CONST_0))", [4, 1], "orb", 'b', "QImode", "gen_rtx_IOR"],
  ["ccrz8x.dst  : IOR_QI(rz8x.src  | SUBREG_QI(MEM_HI(addr), CONST_0))", [4, 1], "orb", 'b', "QImode", "gen_rtx_IOR"],



  ["ccr64x.dst  : XOR_DI(r64x.src  | MEM_DI(addr))", [4, 2], "xorq", 'q', "DImode", "gen_rtx_XOR"],
  ["ccrz32x.dst : XOR_SI(r32x.src  | MEM_SI(addr))", [4, 1], "xorl", 'l', "SImode", "gen_rtx_XOR"],
  ["ccr16x.dst  : XOR_HI(r16x.src  | MEM_HI(addr))", [4, 2], "xorw", 'w', "HImode", "gen_rtx_XOR"],
  ["ccr8x.dst   : XOR_QI(r8x.src   | MEM_QI(addr))", [4, 1], "xorb", 'b', "QImode", "gen_rtx_XOR"],

  ["ccrz16x.dst : XOR_HI(rz16x.src | MEM_HI(addr))", [4, 2], "xorw", 'w', "HImode", "gen_rtx_XOR"],
  ["ccrz8x.dst  : XOR_QI(rz8x.src  | MEM_QI(addr))", [4, 1], "xorb", 'b', "QImode", "gen_rtx_XOR"],


  ["ccrz32x.dst : XOR_SI(r32x.src  | SUBREG_SI(MEM_DI(addr), CONST_0))", [4, 1], "xorl", 'l', "SImode", "gen_rtx_XOR"],
  ["ccr16x.dst  : XOR_HI(r16x.src  | SUBREG_HI(MEM_DI(addr), CONST_0))", [4, 2], "xorw", 'w', "HImode", "gen_rtx_XOR"],
  ["ccr16x.dst  : XOR_HI(r16x.src  | SUBREG_HI(MEM_SI(addr), CONST_0))", [4, 2], "xorw", 'w', "HImode", "gen_rtx_XOR"],
  ["ccr8x.dst   : XOR_QI(r8x.src   | SUBREG_QI(MEM_DI(addr), CONST_0))", [4, 1], "xorb", 'b', "QImode", "gen_rtx_XOR"],
  ["ccr8x.dst   : XOR_QI(r8x.src   | SUBREG_QI(MEM_SI(addr), CONST_0))", [4, 1], "xorb", 'b', "QImode", "gen_rtx_XOR"],
  ["ccr8x.dst   : XOR_QI(r8x.src   | SUBREG_QI(MEM_HI(addr), CONST_0))", [4, 1], "xorb", 'b', "QImode", "gen_rtx_XOR"],

  ["ccrz16x.dst : XOR_HI(rz16x.src | SUBREG_HI(MEM_DI(addr), CONST_0))", [4, 2], "xorw", 'w', "HImode", "gen_rtx_XOR"],
  ["ccrz16x.dst : XOR_HI(rz16x.src | SUBREG_HI(MEM_SI(addr), CONST_0))", [4, 2], "xorw", 'w', "HImode", "gen_rtx_XOR"],
  ["ccrz8x.dst  : XOR_QI(rz8x.src  | SUBREG_QI(MEM_DI(addr), CONST_0))", [4, 1], "xorb", 'b', "QImode", "gen_rtx_XOR"],
  ["ccrz8x.dst  : XOR_QI(rz8x.src  | SUBREG_QI(MEM_SI(addr), CONST_0))", [4, 1], "xorb", 'b', "QImode", "gen_rtx_XOR"],
  ["ccrz8x.dst  : XOR_QI(rz8x.src  | SUBREG_QI(MEM_HI(addr), CONST_0))", [4, 1], "xorb", 'b', "QImode", "gen_rtx_XOR"],

], """
  $rule $cost
  supairs {
    suOrder2($dst, $src, $addr, kid, kids);
  },
  names {
    $dst->rx = $src->rx;
  },
  final {
    $dst->rx = $src->rx;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
    add_addr(live, $addr);
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($src->rx);
  },
  debug {
    dumpMR("$opcode", $addr, $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $src->rx);
    rtx src = $rtx_builder($rtx_mode, dst, gen_rtx_MEM($rtx_mode, $addr->rtl));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


# We're penalizing most of the r-r add instructions because we'd prefer to use the 3-address LEA alternative.
# We'd remove the add variants entirely, but sometimes it's worth having them for the condition code, etc.
# TODO (maybe): have a separate penalty field/flag that adds a tiny penalty (say 1/10).

plug.plugrule3("basicrr", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder", "commute"],

  ["ccr64x.dst  : PLUS_DI(r64x.src1  | r64.src2)", [1, 3+1], "addq", 'q', "DImode", "gen_rtx_PLUS", 1],
  ["ccrz32x.dst : PLUS_SI(r32x.src1  | r32.src2)", [1, 2+1], "addl", 'l', "SImode", "gen_rtx_PLUS", 1],
  ["ccr16x.dst  : PLUS_HI(r16x.src1  | r16.src2)", [1, 3], "addw", 'w', "HImode", "gen_rtx_PLUS", 1],
  ["ccr8x.dst   : PLUS_QI(r8x.src1   | r8.src2)",  [1, 2+1], "addb", 'b', "QImode", "gen_rtx_PLUS", 1],

  # we can propagate the zero extension info
  ["ccrz16x.dst : PLUS_HI(rz16x.src1 | r16.src2)", [1, 3], "addw", 'w', "HImode", "gen_rtx_PLUS", 1],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src1  | r8.src2)",  [1, 2+1], "addb", 'b', "QImode", "gen_rtx_PLUS", 1],

  # we can save a byte for 16-bit values (but we lose the condition code and zero extension)
  ["r16x.dst    : PLUS_HI(r16x.src1  | r16.src2)", [1, 2+1], "addl", 'l', "SImode", "gen_rtx_PLUS", 1],


  ["ccr64x.dst  : MINUS_DI(r64x.src1,  NEG_DI(r64.src2))", [1, 3+1], "addq", 'q', "DImode", "gen_rtx_PLUS", 0],
  ["ccrz32x.dst : MINUS_SI(r32x.src1,  NEG_SI(r32.src2))", [1, 2+1], "addl", 'l', "SImode", "gen_rtx_PLUS", 0],
  ["ccr16x.dst  : MINUS_HI(r16x.src1,  NEG_HI(r16.src2))", [1, 3], "addw", 'w', "HImode", "gen_rtx_PLUS", 0],
  ["ccr8x.dst   : MINUS_QI(r8x.src1,   NEG_QI(r8.src2))",  [1, 2+1], "addb", 'b', "QImode", "gen_rtx_PLUS", 0],

  # we can propagate the zero extension info
  ["ccrz16x.dst : MINUS_HI(rz16x.src1, NEG_HI(r16.src2))", [1, 3], "addw", 'w', "HImode", "gen_rtx_PLUS", 0],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src1,  NEG_HI(r8.src2))",  [1, 2+1], "addb", 'b', "QImode", "gen_rtx_PLUS", 0],

  # we can save a byte for 16-bit values (but we lose the condition code and zero extension)
  ["r16x.dst    : MINUS_HI(r16x.src1,  NEG_HI(r16.src2))", [1, 2+1], "addl", 'l', "SImode", "gen_rtx_PLUS", 0],


  ["ccr64x.dst  : PLUS_DI(r64x.src1 | NEG_DI(r64.src2))", [1, 3], "subq", 'q', "DImode", "gen_rtx_MINUS", 1],
  ["ccrz32x.dst : PLUS_SI(r32x.src1 | NEG_SI(r32.src2))", [1, 2], "subl", 'l', "SImode", "gen_rtx_MINUS", 1],
  ["ccr16x.dst  : PLUS_HI(r16x.src1 | NEG_HI(r16.src2))", [1, 3], "subw", 'w', "HImode", "gen_rtx_MINUS", 1],
  ["ccr8x.dst   : PLUS_QI(r8x.src1  | NEG_QI(r8.src2))",  [1, 2], "subb", 'b', "QImode", "gen_rtx_MINUS", 1],

  # we can propagate the zero extension info
  ["ccrz16x.dst : PLUS_HI(rz16x.src1 | NEG_HI(r16.src2))", [1, 3], "subw", 'w', "HImode", "gen_rtx_MINUS", 1],
  ["ccrz8x.dst  : PLUS_QI(rz8x.src1  | NEG_QI(r8.src2))",  [1, 2], "subb", 'b', "QImode", "gen_rtx_MINUS", 1],

  # we can save a byte for 16-bit values (but we lose the condition code and zero extension)
  ["r16x.dst    : PLUS_HI(r16x.src1 | NEG_HI(r16.src2))", [1, 2], "subl", 'l', "SImode", "gen_rtx_MINUS", 1],


  ["ccr64x.dst  : MINUS_DI(r64x.src1,  r64.src2)", [1, 3], "subq", 'q', "DImode", "gen_rtx_MINUS", 0],
  ["ccrz32x.dst : MINUS_SI(r32x.src1,  r32.src2)", [1, 2], "subl", 'l', "SImode", "gen_rtx_MINUS", 0],
  ["ccr16x.dst  : MINUS_HI(r16x.src1,  r16.src2)", [1, 3], "subw", 'w', "HImode", "gen_rtx_MINUS", 0],
  ["ccr8x.dst   : MINUS_QI(r8x.src1,   r8.src2)",  [1, 2], "subb", 'b', "QImode", "gen_rtx_MINUS", 0],

  # we can propagate the zero extension info
  ["ccrz16x.dst : MINUS_HI(rz16x.src1, r16.src2)", [1, 3], "subw", 'w', "HImode", "gen_rtx_MINUS", 0],
  ["ccrz8x.dst  : MINUS_QI(rz8x.src1,  r8.src2)",  [1, 2], "subb", 'b', "QImode", "gen_rtx_MINUS", 0],

  # we can save a byte for 16-bit values (but we lose the condition code and zero extension)
  ["r16x.dst    : MINUS_HI(r16x.src1,  r16.src2)", [1, 2], "subl", 'l', "SImode", "gen_rtx_MINUS", 0],


  # imul doesn't set the CC usefully.  We use 32-bit multiply to accomplish 8 and 16-bit multiplication
  ["r64x.dst  : MULT_DI(r64x.src1  | r64.src2)", [4, 4], "imulq", 'q', "DImode", "gen_rtx_MULT", 1],
  ["rz32x.dst : MULT_SI(r32x.src1  | r32.src2)", [3, 3], "imull", 'l', "SImode", "gen_rtx_MULT", 1],
  ["r16x.dst  : MULT_HI(r16x.src1  | r16.src2)", [3, 3], "imull", 'l', "SImode", "gen_rtx_MULT", 1],
  ["r8x.dst   : MULT_QI(r8x.src1   | r8.src2)",  [3, 3], "imull", 'l', "SImode", "gen_rtx_MULT", 1],

  # we can propagate the zero extension info if we use the 16-bit multiplication
  ["rz16x.dst : MULT_HI(rz16x.src1 | r16.src2)", [3, 4], "imulw", 'w', "HImode",  "gen_rtx_MULT", 1],


  ["ccr64x.dst  : AND_DI(r64x.src1  | r64.src2)",  [1, 3], "andq", 'q', "DImode", "gen_rtx_AND", 1],
  ["ccrz32x.dst : AND_SI(r32x.src1  | r32.src2)",  [1, 2], "andl", 'l', "SImode", "gen_rtx_AND", 1],
  ["ccr16x.dst  : AND_HI(r16x.src1  | r16.src2)",  [1, 3], "andw", 'w', "HImode", "gen_rtx_AND", 1],
  ["ccr8x.dst   : AND_QI(r8x.src1   | r8.src2)",   [1, 2], "andb", 'b', "QImode", "gen_rtx_AND", 1],

  # we can propagate the zero extension info
  ["ccrz16x.dst : AND_HI(rz16x.src1 | r16.src2)",  [1, 3], "andw", 'w', "HImode", "gen_rtx_AND", 1],
  ["ccrz8x.dst  : AND_QI(rz8x.src1  | r8.src2)",   [1, 2], "andb", 'b', "QImode", "gen_rtx_AND", 1],

  # sometimes it's worth doing these with a 32-bit AND (but we lose the accurate CC)
  ["r16x.dst    : AND_HI(r16x.src1  | r16.src2)",  [1, 2], "andl", 'l', "SImode", "gen_rtx_AND", 1],
  ["rz16x.dst   : AND_HI(rz16x.src1 | r16.src2)",  [1, 2], "andl", 'l', "SImode", "gen_rtx_AND", 1],
  ["rz16x.dst   : AND_HI(r16x.src1  | rz16.src2)", [1, 2], "andl", 'l', "SImode", "gen_rtx_AND", 1],
  ["rs16x.dst   : AND_HI(rs16x.src1 | rs16.src2)", [1, 2], "andl", 'l', "SImode", "gen_rtx_AND", 1],
  ["r8x.dst     : AND_QI(r8x.src1   | r8.src2)",   [1, 2], "andl", 'l', "SImode", "gen_rtx_AND", 1],
  ["rz8x.dst    : AND_QI(rz8x.src1  | r8.src2)",   [1, 2], "andl", 'l', "SImode", "gen_rtx_AND", 1],
  ["rz8x.dst    : AND_QI(r8x.src1   | rz8.src2)",  [1, 2], "andl", 'l', "SImode", "gen_rtx_AND", 1],
  ["rs8x.dst    : AND_QI(rs8x.src1  | rs8.src2)",  [1, 2], "andl", 'l', "SImode", "gen_rtx_AND", 1],


  ["ccr64x.dst  : IOR_DI(r64x.src1  | r64.src2)",  [1, 3], "orq", 'q', "DImode", "gen_rtx_IOR", 1],
  ["ccrz32x.dst : IOR_SI(r32x.src1  | r32.src2)",  [1, 2], "orl", 'l', "SImode", "gen_rtx_IOR", 1],
  ["ccr16x.dst  : IOR_HI(r16x.src1  | r16.src2)",  [1, 3], "orw", 'w', "HImode", "gen_rtx_IOR", 1],
  ["ccr8x.dst   : IOR_QI(r8x.src1   | r8.src2)",   [1, 2], "orb", 'b', "QImode", "gen_rtx_IOR", 1],

  # we can propagate the zero extension info
  ["ccrz16x.dst : IOR_HI(rz16x.src1 | r16.src2)",  [1, 3], "orw", 'w', "HImode", "gen_rtx_IOR", 1],
  ["ccrz8x.dst  : IOR_QI(rz8x.src1  | r8.src2)",   [1, 2], "orb", 'b', "QImode", "gen_rtx_IOR", 1],


  # sometimes it's worth doing these with a 32-bit OR (but we lose the accurate CC)
  ["r16x.dst    : IOR_HI(r16x.src1  | r16.src2)",  [1, 2], "orl", 'l', "SImode", "gen_rtx_IOR", 1],
  ["rz16x.dst   : IOR_HI(rz16x.src1 | rz16.src2)", [1, 2], "orl", 'l', "SImode", "gen_rtx_IOR", 1],
  ["rs16x.dst   : IOR_HI(rs16x.src1 | rs16.src2)", [1, 2], "orl", 'l', "SImode", "gen_rtx_IOR", 1],
  ["rz8x.dst    : IOR_QI(rz8x.src1  | rz8.src2)",  [1, 2], "orl", 'l', "SImode", "gen_rtx_IOR", 1],
  ["rs8x.dst    : IOR_QI(rs8x.src1  | rs8.src2)",  [1, 2], "orl", 'l', "SImode", "gen_rtx_IOR", 1],



  ["ccr64x.dst  : XOR_DI(r64x.src1  | r64.src2)",  [1, 3], "xorq", 'q', "DImode", "gen_rtx_XOR", 1],
  ["ccrz32x.dst : XOR_SI(r32x.src1  | r32.src2)",  [1, 2], "xorl", 'l', "SImode", "gen_rtx_XOR", 1],
  ["ccr16x.dst  : XOR_HI(r16x.src1  | r16.src2)",  [1, 3], "xorw", 'w', "HImode", "gen_rtx_XOR", 1],
  ["ccr8x.dst   : XOR_QI(r8x.src1   | r8.src2)",   [1, 2], "xorb", 'b', "QImode", "gen_rtx_XOR", 1],

  # we can propagate the zero extension info
  ["ccrz16x.dst : XOR_HI(rz16x.src1 | r16.src2)", [1, 3], "xorw", 'w', "HImode", "gen_rtx_XOR", 1],
  ["ccrz8x.dst  : XOR_QI(rz8x.src1  | r8.src2)",  [1, 2], "xorb", 'b', "QImode", "gen_rtx_XOR", 1],

  # sometimes it's worth doing these with a 32-bit XOR (but we lose the accurate CC)
  ["r16x.dst    : XOR_HI(r16x.src1  | r16.src2)",  [1, 2], "xorl", 'l', "SImode", "gen_rtx_XOR", 1],
  ["rz16x.dst   : XOR_HI(rz16x.src1 | rz16.src2)", [1, 2], "xorl", 'l', "SImode", "gen_rtx_XOR", 1],
  ["rs16x.dst   : XOR_HI(rs16x.src1 | rs16.src2)", [1, 2], "xorl", 'l', "SImode", "gen_rtx_XOR", 1],
  ["rz8x.dst    : XOR_QI(rz8x.src1  | rz8.src2)",  [1, 2], "xorl", 'l', "SImode", "gen_rtx_XOR", 1],
  ["rs8x.dst    : XOR_QI(rs8x.src1  | rs8.src2)",  [1, 2], "xorl", 'l', "SImode", "gen_rtx_XOR", 1],


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
    sparseset_set_bit(live, find($src2->r));
  },
  remat {
    flags = 0;
  },
  costs {
    if ($commute)
      memorable($src1->r);
    else
      forgettable($src1->rx);
    memorable($src2->r);
  },
  debug {
    dumpRR("$opcode", $src2->r, '$schar', $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $src1->rx);
    rtx src = $rtx_builder($rtx_mode, dst, gen_rtx_REG($rtx_mode, $src2->r));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


plug.plugrule3("basicneg", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder", "rcc_clobber"],

  ["ccr64x.dst  : NEG_DI(r64x.src)",  [1, 3], "negq", 'q', "DImode", "gen_rtx_NEG", 1],
  ["ccrz32x.dst : NEG_SI(r32x.src)",  [1, 2], "negl", 'l', "SImode", "gen_rtx_NEG", 1],
  ["ccr16x.dst  : NEG_HI(r16x.src)",  [1, 3], "negw", 'w', "HImode", "gen_rtx_NEG", 1],
  ["ccr8x.dst   : NEG_QI(r8x.src)",   [1, 2], "negb", 'b', "QImode", "gen_rtx_NEG", 1],

  # we can propagate the zero extension info
  ["ccrz16x.dst : NEG_HI(rz16x.src)", [1, 3], "negw", 'w', "HImode", "gen_rtx_NEG", 1],
  ["ccrz8x.dst  : NEG_QI(rz8x.src)",  [1, 2], "negb", 'b', "QImode", "gen_rtx_NEG", 1],

  # sometimes it's worth doing this with a 32-bit NEGATE (but we lose the accurate CC)
  ["r16x.dst    : NEG_HI(r16x.src)",  [1, 2], "negl", 'l', "SImode", "gen_rtx_NEG", 1],

  # sometimes it's worth doing this with a 32-bit NEGATE (and we get an accurate CC)
  ["ccrs16x.dst : NEG_HI(rs16x.src)", [1, 2], "negl", 'l', "SImode", "gen_rtx_NEG", 1],
  ["ccrs8x.dst  : NEG_HI(rs8x.src)",  [1, 2], "negl", 'l', "SImode", "gen_rtx_NEG", 1],


  # Note that "not" does not affect any flags, but "neg" does.
  ["r64x.dst  : NOT_DI(r64x.src)",  [1, 3], "notq", 'q', "DImode", "gen_rtx_NOT", 0],
  ["rz32x.dst : NOT_SI(r32x.src)",  [1, 2], "notl", 'l', "SImode", "gen_rtx_NOT", 0],
  ["r16x.dst  : NOT_HI(r16x.src)",  [1, 3], "notw", 'w', "HImode", "gen_rtx_NOT", 0],
  ["r8x.dst   : NOT_QI(r8x.src)",   [1, 2], "notb", 'b', "QImode", "gen_rtx_NOT", 0],

  # we can propagate the zero extension info
  ["rz16x.dst : NOT_HI(rz16x.src)", [1, 3], "notw", 'w', "HImode", "gen_rtx_NOT", 0],
  ["rz8x.dst  : NOT_QI(rz8x.src)",  [1, 2], "notb", 'b', "QImode", "gen_rtx_NOT", 0],

  # sometimes it's worth doing this with a 32-bit NOT
  ["r16x.dst  : NOT_HI(r16x.src)",  [1, 2], "notl", 'l', "SImode", "gen_rtx_NOT", 0],
  ["rs16x.dst : NOT_HI(rs16x.src)", [1, 2], "notl", 'l', "SImode", "gen_rtx_NOT", 0],
  ["rs8x.dst  : NOT_QI(rs8x.src)",  [1, 2], "notl", 'l', "SImode", "gen_rtx_NOT", 0],

], """
  $rule $cost
  names {
    $dst->rx = $src->rx;
  },
  final {
    $dst->rx = $src->rx;
  },
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
  },
  costs {
    forgettable($src->rx);
  },
  debug {
    dumpR("$opcode", $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $src->rx);
    rtx src = $rtx_builder($rtx_mode, gen_rtx_REG($rtx_mode, $src->rx));
    ($rcc_clobber ? icg_emit_clobber : icg_emit_plain)(gen_rtx_SET(VOIDmode, dst, src));
  };

""")

plug.plugrule3("extenders", [
  ["rule", "cost", "field"],

  ["r64x.dst  : NEG_DI(NEG_DI(r64x.src))",  [0, 0], "rx"],
  ["r32x.dst  : NEG_SI(NEG_SI(r32x.src))",  [0, 0], "rx"],
  ["rz32x.dst : NEG_SI(NEG_SI(rz32x.src))", [0, 0], "rx"],
  ["r16x.dst  : NEG_HI(NEG_HI(r16x.src))",  [0, 0], "rx"],
  ["rz16x.dst : NEG_HI(NEG_HI(rz16x.src))", [0, 0], "rx"],
  ["rs16x.dst : NEG_HI(NEG_HI(rs16x.src))", [0, 0], "rx"],
  ["r8x.dst   : NEG_QI(NEG_QI(r8x.src))",   [0, 0], "rx"],
  ["rz8x.dst  : NEG_QI(NEG_QI(rz8x.src))",  [0, 0], "rx"],
  ["rs8x.dst  : NEG_QI(NEG_QI(rs8x.src))",  [0, 0], "rx"],
  ["rdx.dst   : NEG_DF(NEG_DF(rdx.src))",   [0, 0], "rx"],
  ["rfx.dst   : NEG_SF(NEG_SF(rfx.src))",   [0, 0], "rx"],

  ["rz32.dst  : SIGN_EXTEND_SI(rs16.src)",  [0, 0], "r"],
  ["rz32.dst  : SIGN_EXTEND_SI(rs8.src)",   [0, 0], "r"],
  ["rs16.dst  : SIGN_EXTEND_HI(rs8.src)",   [0, 0], "r"],
  ["rz32x.dst : SIGN_EXTEND_SI(rs16x.src)", [0, 0], "rx"],
  ["rz32x.dst : SIGN_EXTEND_SI(rs8x.src)",  [0, 0], "rx"],
  ["rs16x.dst : SIGN_EXTEND_HI(rs8x.src)",  [0, 0], "rx"],

  ["rz32.dst  : ZERO_EXTEND_SI(rz16.src)",  [0, 0], "r"],
  ["rz32.dst  : ZERO_EXTEND_SI(rz8.src)",   [0, 0], "r"],
  ["rz16.dst  : ZERO_EXTEND_HI(rz8.src)",   [0, 0], "r"],
  ["rz32x.dst : ZERO_EXTEND_SI(rz16x.src)", [0, 0], "rx"],
  ["rz32x.dst : ZERO_EXTEND_SI(rz8x.src)",  [0, 0], "rx"],
  ["rz16x.dst : ZERO_EXTEND_HI(rz8x.src)",  [0, 0], "rx"],

  #
  # This is a little subtle.
  # By requiring a regx, I force a chain rule to  make
  # a copy (if required) and it'll use a 32-bit copy
  # (versus the more expensive 64-bit copy).
  #
  ["r64x.dst : ZERO_EXTEND_DI(rz32x.src)", [0, 0], "rx"],
  ["r64x.dst : ZERO_EXTEND_DI(rz16x.src)", [0, 0], "rx"],
  ["r64x.dst : ZERO_EXTEND_DI(rz8x.src)",  [0, 0], "rx"],
  ["r64.dst  : ZERO_EXTEND_DI(rz32.src)",  [0, 0], "r"],
  ["r64.dst  : ZERO_EXTEND_DI(rz16.src)",  [0, 0], "r"],
  ["r64.dst  : ZERO_EXTEND_DI(rz8.src)",   [0, 0], "r"],

], """
  $rule $cost
  names {
    $dst->$field = $src->$field;
  },
  final {
    $dst->$field = $src->$field;
  },
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  };
""")


#----------------------------------------------------------------
# mult
#
# Note that imul doesn't set the CC usefully.
#

plug.plugrule3("multrri", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder"],

  ["r64x.dst  : MULT_DI(r64.src  | imm8.const)",  [4, 3], "imulq", 'q', "DImode", "gen_rtx_MULT"],
  ["r64x.dst  : MULT_DI(r64.src  | imm32.const)", [4, 3], "imulq", 'q', "DImode", "gen_rtx_MULT"],
  ["rz32x.dst : MULT_SI(r32.src  | imm8.const)",  [3, 2], "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["rz32x.dst : MULT_SI(r32.src  | imm32.const)", [3, 2], "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["r16x.dst  : MULT_HI(r16.src  | imm8.const)",  [4, 3], "imulw", 'w', "HImode", "gen_rtx_MULT"],
  ["r16x.dst  : MULT_HI(r16.src  | imm16.const)", [4, 3], "imulw", 'w', "HImode", "gen_rtx_MULT"],

  # we can propagate the zero extension info
  ["rz16x.dst : MULT_HI(rz16.src | imm8.const)",  [4, 3], "imulw", 'w', "HImode", "gen_rtx_MULT"],
  ["rz16x.dst : MULT_HI(rz16.src | imm16.const)", [4, 3], "imulw", 'w', "HImode", "gen_rtx_MULT"],

  # often it will be worth doing these with a 32-bit multiply
  ["r16x.dst  : MULT_HI(r16.src  | imm8.const)",  [3, 2], "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["r16x.dst  : MULT_HI(r16.src  | imm32.const)", [3, 2], "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["r8x.dst   : MULT_QI(r8.src   | imm8.const)",  [3, 2], "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["r8x.dst   : MULT_QI(r8.src   | imm32.const)", [3, 2], "imull", 'l', "SImode", "gen_rtx_MULT"],

], """
  $rule $cost
  supairs {
    $dst->extra = $src->extra;            /* think about these */
    $dst->freed = $src->freed;
  },
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  build {
    add_edges(find($dst->rx), live);
    sparseset_clear_bit(live, find($dst->rx));
    sparseset_set_bit(live, find($src->r));
  },
  remat {
    flags = 0;
  },
  costs {
    memorable($src->r);
  },
  debug {
    dumpIRR("$opcode", $const, $src->r, $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src = $rtx_builder($rtx_mode,
			   gen_rtx_REG($rtx_mode, $src->r),
			   gen_rtx_imm_constant($const->val, $const->a.string, $const->rtl));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")



plug.plugrule3("multimr", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder"],

  ["r64x.dst  : MULT_DI(MEM_DI(addr) | imm8.const)",  [8, 3],  "imulq", 'q', "DImode", "gen_rtx_MULT"],
  ["r64x.dst  : MULT_DI(MEM_DI(addr) | imm32.const)", [8, 3],  "imulq", 'q', "DImode", "gen_rtx_MULT"],
  ["rz32x.dst : MULT_SI(MEM_SI(addr) | imm8.const)",  [7, 2],  "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["rz32x.dst : MULT_SI(MEM_SI(addr) | imm32.const)", [7, 2],  "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["r16x.dst  : MULT_HI(MEM_HI(addr) | imm8.const)",  [7, 3],  "imulw", 'w', "HImode", "gen_rtx_MULT"],
  ["r16x.dst  : MULT_HI(MEM_HI(addr) | imm16.const)", [7, 3],  "imulw", 'w', "HImode", "gen_rtx_MULT"],

  ["rz32x.dst : MULT_SI(SUBREG_SI(MEM_DI(addr), CONST_0) | imm8.const)",  [7, 2], "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["rz32x.dst : MULT_SI(SUBREG_SI(MEM_DI(addr), CONST_0) | imm32.const)", [7, 2], "imull", 'l', "SImode", "gen_rtx_MULT"],
  ["r16x.dst  : MULT_HI(SUBREG_HI(MEM_DI(addr), CONST_0) | imm8.const)",  [7, 3], "imulw", 'w', "HImode", "gen_rtx_MULT"],
  ["r16x.dst  : MULT_HI(SUBREG_HI(MEM_SI(addr), CONST_0) | imm8.const)",  [7, 3], "imulw", 'w', "HImode", "gen_rtx_MULT"],
  ["r16x.dst  : MULT_HI(SUBREG_HI(MEM_DI(addr), CONST_0) | imm16.const)", [7, 3], "imulw", 'w', "HImode", "gen_rtx_MULT"],
  ["r16x.dst  : MULT_HI(SUBREG_HI(MEM_SI(addr), CONST_0) | imm16.const)", [7, 3], "imulw", 'w', "HImode", "gen_rtx_MULT"],

], """
  $rule $cost
  supairs {
    suOrder2($dst, $addr, $const, kid, kids);
  },
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  build {
    add_edges(find($dst->rx), live);
    sparseset_clear_bit(live, find($dst->rx));
    add_addr(live, $addr);
  },
  debug {
    dumpIMR("$opcode", $const, $addr, $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src = $rtx_builder($rtx_mode,
			   gen_rtx_MEM($rtx_mode, $addr->rtl),
			   gen_rtx_imm_constant($const->val, $const->a.string, $const->rtl));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


# div and mod
# assume first coalesce will succeed.
# What about second?  I guess we'll count it.
#
# Note that gcc doesn't use "cqo" for pentium, just the opteron.
#
# time is 2 + div, where div is, say, 50
#

plug.plugrule3("div", [
  ["rule", "cost", "opcode", "schar", "dst_reg_name", "rtx_mode", "mov_opcode", "extend_opcode", "isRR", "issigned"],

  ["r64x.dst  :  DIV_DI(r64x.src1, r64.src2)",                              [52, 8], "idivq", 'q', "REG_RAX", "DImode", "movq", "cqo",  1, 1],
  ["rz32x.dst :  DIV_SI(r32x.src1, r32.src2)",                              [52, 6], "idivl", 'd', "REG_RAX", "SImode", "movl", "cdq",  1, 1],

  ["r64x.dst  :  MOD_DI(r64x.src1, r64.src2)",                              [52, 8], "idivq", 'q', "REG_RDX", "DImode", "movq", "cqo",  1, 1],
  ["rz32x.dst :  MOD_SI(r32x.src1, r32.src2)",                              [52, 5], "idivl", 'd', "REG_RDX", "SImode", "movl", "cdq",  1, 1],

  ["r64x.dst :   DIV_DI(r64x.src1, MEM_DI(addr.src2))",                     [52, 8], "idivq", 'q', "REG_RAX", "DImode", "movq", "cqo",  0, 1],
  ["rz32x.dst :  DIV_SI(r32x.src1, MEM_SI(addr.src2))",                     [52, 4], "idivl", 'd', "REG_RAX", "SImode", "movl", "cdq",  0, 1],
  ["rz32x.dst :  DIV_SI(r32x.src1, SUBREG_SI(MEM_DI(addr.src2), CONST_0))", [52, 4], "idivl", 'd', "REG_RAX", "SImode", "movl", "cdq",  0, 1],

  ["r64x.dst :   MOD_DI(r64x.src1, MEM_DI(addr.src2))",                     [52, 8], "idivq", 'q', "REG_RDX", "DImode", "movq", "cqo",  0, 1],
  ["rz32x.dst :  MOD_SI(r32x.src1, MEM_SI(addr.src2))",                     [52, 4], "idivl", 'd', "REG_RDX", "SImode", "movl", "cdq",  0, 1],
  ["rz32x.dst :  MOD_SI(r32x.src1, SUBREG_SI(MEM_DI(addr.src2), CONST_0))", [52, 4], "idivl", 'd', "REG_RDX", "SImode", "movl", "cdq",  0, 1],
  #
  # TODO: DIV_HI
  # TODO: MOD_HI
  # TODO:  MOD_QI (hard, as mod comes back in register %ah, eg (rax>>8)&0xff) (dst_reg_name must be REG_RAX)
  # TODO: UMOD_QI (hard, as mod comes back in register %ah, eg (rax>>8)&0xff) (dst_reg_name must be REG_RAX)
  #

  ["r64x.dst  : UDIV_DI(r64x.src1, r64.src2)",                              [52, 8],  "divq", 'q', "REG_RAX", "DImode", "movq", "xorl", 1, 0],
  ["rz32x.dst : UDIV_SI(r32x.src1, r32.src2)",                              [52, 6],  "divl", 'd', "REG_RAX", "SImode", "movl", "xorl", 1, 0],
  ["r16x.dst  : UDIV_HI(r16x.src1, r16.src2)",                              [52, 6],  "divw", 'w', "REG_RAX", "HImode", "movl", "xorl", 1, 0],
  ["r8x.dst   : UDIV_QI(r16x.src1, r8.src2)",                               [52, 6],  "divb", 'b', "REG_RAX", "QImode", "movl", "xor?", 1, 0],

  ["r64x.dst  : UMOD_DI(r64x.src1, r64.src2)",                              [52, 8],  "divq", 'q', "REG_RDX", "DImode", "movq", "xorl", 1, 0],
  ["rz32x.dst : UMOD_SI(r32x.src1, r32.src2)",                              [52, 6],  "divl", 'd', "REG_RDX", "SImode", "movl", "xorl", 1, 0],
  ["r16x.dst  : UMOD_HI(r16x.src1, r16.src2)",                              [52, 6],  "divw", 'w', "REG_RDX", "HImode", "movl", "xorl", 1, 0],

  ["r64x.dst  : UDIV_DI(r64x.src1,           MEM_DI(addr.src2))",           [52, 8],  "divq", 'q', "REG_RAX", "DImode", "movq", "xorl", 0, 0],
  ["rz32x.dst : UDIV_SI(r32x.src1,           MEM_SI(addr.src2))",           [52, 5],  "divl", 'd', "REG_RAX", "SImode", "movl", "xorl", 0, 0],
  ["rz32x.dst : UDIV_SI(r32x.src1, SUBREG_SI(MEM_DI(addr.src2), CONST_0))", [52, 5],  "divl", 'd', "REG_RAX", "SImode", "movl", "xorl", 0, 0],
  ["r16x.dst  : UDIV_HI(r16x.src1,           MEM_HI(addr.src2))",           [52, 6],  "divw", 'w', "REG_RAX", "HImode", "movl", "xorl", 0, 0],
  ["r16x.dst  : UDIV_HI(r16x.src1, SUBREG_HI(MEM_DI(addr.src2), CONST_0))", [52, 6],  "divw", 'w', "REG_RAX", "HImode", "movl", "xorl", 0, 0],
  ["r16x.dst  : UDIV_HI(r16x.src1, SUBREG_HI(MEM_SI(addr.src2), CONST_0))", [52, 6],  "divw", 'w', "REG_RAX", "HImode", "movl", "xorl", 0, 0],
  ["r8x.dst   : UDIV_QI(r16x.src1, SUBREG_QI(MEM_DI(addr.src2), CONST_0))", [52, 6],  "divb", 'b', "REG_RAX", "QImode", "movl", "xor?", 0, 0],
  ["r8x.dst   : UDIV_QI(r16x.src1, SUBREG_QI(MEM_SI(addr.src2), CONST_0))", [52, 6],  "divb", 'b', "REG_RAX", "QImode", "movl", "xor?", 0, 0],
  ["r8x.dst   : UDIV_QI(r16x.src1, SUBREG_QI(MEM_HI(addr.src2), CONST_0))", [52, 6],  "divb", 'b', "REG_RAX", "QImode", "movl", "xor?", 0, 0],

  ["r64x.dst  : UMOD_DI(r64x.src1,           MEM_DI(addr.src2))",           [52, 8],  "divq", 'q', "REG_RDX", "DImode", "movq", "xorl", 0, 0],
  ["rz32x.dst : UMOD_SI(r32x.src1,           MEM_SI(addr.src2))",           [52, 5],  "divl", 'd', "REG_RDX", "SImode", "movl", "xorl", 0, 0],
  ["rz32x.dst : UMOD_SI(r32x.src1, SUBREG_SI(MEM_DI(addr.src2), CONST_0))", [52, 5],  "divl", 'd', "REG_RDX", "SImode", "movl", "xorl", 0, 0],
  ["r16x.dst  : UMOD_HI(r16x.src1,           MEM_HI(addr.src2))",           [52, 6],  "divw", 'w', "REG_RDX", "HImode", "movl", "xorl", 0, 0],
  ["r16x.dst  : UMOD_HI(r16x.src1, SUBREG_HI(MEM_DI(addr.src2), CONST_0))", [52, 6],  "divw", 'w', "REG_RDX", "HImode", "movl", "xorl", 0, 0],
  ["r16x.dst  : UMOD_HI(r16x.src1, SUBREG_HI(MEM_SI(addr.src2), CONST_0))", [52, 6],  "divw", 'w', "REG_RDX", "HImode", "movl", "xorl", 0, 0],

], """
  $rule $cost
  supairs {
    if ($isRR) {
      suOrder2($dst, $src1, $src2, kid, kids);
    } else {
      suOrder2($dst, $src1, $src2, kid, kids);
    }
  },
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src1->rx, REG_RAX);
    coalesces += attempt_coalesce(pass, $dst->rx, $dst_reg_name);
  },
  build {
    if ($dst_reg_name == REG_RAX)
      add_copy_edges($dst->rx, REG_RAX, live);

    else
      add_copy_edges($dst->rx, REG_RDX, live);

    add_edges(REG_RAX, live);
    if ($isRR)
      sparseset_set_bit(live, find($src2->r));
    else
      add_addr(live, $src2);

    if ('$schar' != 'b') {
      sparseset_clear_bit(live, REG_RDX);
      add_edges(REG_RDX, live);
    }

    add_copy_edges(REG_RAX, $src1->rx, live);
  },
  remat {
    flags = 0;
  },
  costs {
    cost_copy($src1->rx, REG_RAX);
    memorable($src1->rx);
    if ($isRR)
      memorable($src2->r);
    cost_copy($dst_reg_name, $dst->rx);
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dump_copy("$mov_opcode", $src1->rx, REG_RAX, '$schar');
    if ('$schar' != 'b') {
      if ($issigned) {
	dump("$extend_opcode");					/* such as cqo or cdq */
      } else {
	dumpRR("$extend_opcode", REG_RDX, 'd', REG_RDX, 'd');	/* xorl */
      }
    }
    if ($isRR) {
      dumpR("$opcode", $src2->r, '$schar');
    } else {
      dumpM("$opcode", $src2);
    }
    dump_copy("$mov_opcode", $dst_reg_name, $dst->rx, '$schar');
  },
  emit {
    rtx dst  = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src1 = gen_rtx_REG($rtx_mode, $src1->rx);
    rtx src2 = $isRR
      ? gen_rtx_REG($rtx_mode, $src2->r)
      : gen_rtx_MEM($rtx_mode, $src2->rtl);
    /*
     * TODO: raxm, which is the wider mode
     * TODO: rdxm, which is the wider mode
     */
    rtx rax = gen_rtx_REG($rtx_mode, REG_RAX);
    rtx rax1 = gen_rtx_REG(HImode, REG_RAX);
    rtx rdx = ('$schar' != 'b') ? gen_rtx_REG($rtx_mode, REG_RDX) : 0;
    rtvec parset = 0;
    icg_emit_plain(gen_rtx_SET(VOIDmode, rax/*raxm?*/, src1));
    /*
     * TODO? emit RTL for the sign_extend (true sign extend, or zero fill with xor) using rdx/rdxm
     */
    parset = ('$schar' == 'b')
      ? gen_rtvec(2,
	  gen_rtx_SET(VOIDmode, rax, gen_rtx_DIV_helper($issigned, $rtx_mode, rax1, src2)),
	  gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(CCmode, FLAGS_REG)))
      : gen_rtvec(3,
	  gen_rtx_SET(VOIDmode, rax, gen_rtx_DIV_helper($issigned, $rtx_mode, rax, src2)),
	  gen_rtx_SET(VOIDmode, rdx, gen_rtx_MOD_helper($issigned, $rtx_mode, rax, src2)),
	  gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(CCmode, FLAGS_REG)))
    ;
    icg_emit_plain(gen_rtx_PARALLEL(VOIDmode, parset));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, ($dst_reg_name == REG_RAX) ? rax/*raxm?*/ : rdx/*rdxm*/));
  };
""")


#---------------------------------------------------------------
# signed div/mod in parallel
# TODO: merge with previous rule
# be careful with rax vs raxm
plug.plugrule3("pardivmod", [
  ["rule", "cost", "rtx_mode", "mov_opcode", "schar", "extend_opcode", "div_opcode", "isRR", "issigned"],

  ["""stmt : PARALLEL_ALL(SET_ALL(lhs64.dst1, DIV_DI(r64.src1a, r64.src1b)),
                          SET_ALL(lhs64.dst2, MOD_DI(r64.src2a, r64.src2b)))""",
    [52, 8], "DImode", "movq", 'q', "cqo", "idivq", 1, 1],
  ["""stmt : PARALLEL_ALL(SET_ALL(lhs32.dst1, DIV_SI(r32.src1a, r32.src1b)),
                          SET_ALL(lhs32.dst2, MOD_SI(r32.src2a, r32.src2b)))""",
    [52, 8], "SImode", "movl", 'd', "cdq", "idivl", 1, 1],

  ["""stmt : PARALLEL_ALL(SET_ALL(lhs64.dst1, UDIV_DI(r64.src1a, r64.src1b)),
                          SET_ALL(lhs64.dst2, UMOD_DI(r64.src2a, r64.src2b)))""",
    [52, 8], "DImode", "movq", 'q', "xorl", "divq", 1, 0],
  ["""stmt : PARALLEL_ALL(SET_ALL(lhs32.dst1, UDIV_SI(r32.src1a, r32.src1b)),
                          SET_ALL(lhs32.dst2, UMOD_SI(r32.src2a, r32.src2b)))""",
    [52, 8], "SImode", "movl", 'd', "xorl", "divl", 1, 0],

  ["""stmt : PARALLEL_ALL(SET_ALL(lhs64.dst1, DIV_DI(r64.src1a, MEM_DI(addr.src1b))),
                          SET_ALL(lhs64.dst2, MOD_DI(r64.src2a, MEM_DI(addr.src2b))))""",
    [52, 8], "DImode", "movq", 'q', "cqo", "idivq", 0, 1],
  ["""stmt : PARALLEL_ALL(SET_ALL(lhs32.dst1, DIV_SI(r32.src1a, MEM_SI(addr.src1b))),
                          SET_ALL(lhs32.dst2, MOD_SI(r32.src2a, MEM_SI(addr.src2b))))""",
    [52, 8], "SImode", "movl", 'd', "cltd", "idivl", 0, 1],
  ["""stmt : PARALLEL_ALL(SET_ALL(lhs32.dst1, DIV_SI(r32.src1a, SUBREG_SI(MEM_DI(addr.src1b), CONST_0))),
                          SET_ALL(lhs32.dst2, MOD_SI(r32.src2a, SUBREG_SI(MEM_DI(addr.src2b), CONST_0))))""",
    [52, 8], "SImode", "movl", 'd', "cltd", "idivl", 0, 1],

  ["""stmt : PARALLEL_ALL(SET_ALL(lhs64.dst1, UDIV_DI(r64.src1a, MEM_DI(addr.src1b))),
                          SET_ALL(lhs64.dst2, UMOD_DI(r64.src2a, MEM_DI(addr.src2b))))""",
    [52, 8], "DImode", "movq", 'q', "xorl", "divq", 0, 0],
  ["""stmt : PARALLEL_ALL(SET_ALL(lhs32.dst1, UDIV_SI(r32.src1a, MEM_SI(addr.src1b))),
                          SET_ALL(lhs32.dst2, UMOD_SI(r32.src2a, MEM_SI(addr.src2b))))""",
    [52, 8], "SImode", "movl", 'd', "xorl", "divl", 0, 0],
  ["""stmt : PARALLEL_ALL(SET_ALL(lhs32.dst1, UDIV_SI(r32.src1a, SUBREG_SI(MEM_DI(addr.src1b), CONST_0))),
                          SET_ALL(lhs32.dst2, UMOD_SI(r32.src2a, SUBREG_SI(MEM_DI(addr.src2b), CONST_0))))""",
    [52, 8], "SImode", "movl", 'd', "xorl", "divl", 0, 0],

], """
  $rule $cost
  supairs {
    /* suOrder2($stmt, $dst1, $src1a, kid, kids); */
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src1a->r, REG_RAX);
    if (!$dst1->spilled)
      coalesces += attempt_coalesce(pass, $dst1->r, REG_RAX);
    if (!$dst2->spilled)
      coalesces += attempt_coalesce(pass, $dst2->r, REG_RDX);
  },
  build {
    /* final copy instruction */
    if ($dst2->spilled) {
      sparseset_set_bit(live, REG_RDX);
      add_addr(live, $dst2);
    }
    else
      add_copy_edges($dst2->r, REG_RDX, live);

    /* penultimate copy instruction */
    if ($dst1->spilled) {
      sparseset_set_bit(live, REG_RAX);
      add_addr(live, $dst1);
    }
    else
      add_copy_edges($dst1->r, REG_RAX, live);

    /* divide instruction */
    add_edges(REG_RAX, live);
    if ($isRR)
      sparseset_set_bit(live, find($src1b->r));
    else
      add_addr(live, $src1b);

    /* extension instruction */
    sparseset_clear_bit(live, REG_RDX);
    add_edges(REG_RDX, live);

    /* final copy instruction */
    add_copy_edges(REG_RAX, $src1a->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($src1a->r);
    cost_copy($src1a->r, REG_RAX);
    if ($isRR)
      memorable($src1b->r);
    if (!$dst1->spilled)
      cost_copy($dst1->r, REG_RAX);
    if (!$dst2->spilled)
      cost_copy(REG_RDX, $dst2->r);
  },
  debug {
    dump_copy("$mov_opcode", $src1a->r, REG_RAX, '$schar');

    if ($issigned)
      dump("$extend_opcode");
    else
      dumpRR("$extend_opcode", REG_RDX, '$schar', REG_RDX, '$schar');

    if ($isRR)
      dumpR("$div_opcode", $src1b->r, '$schar');
    else
      dumpM("$div_opcode", $src1b);

    if ($dst1->spilled)
      dumpRM("$mov_opcode", REG_RAX, '$schar', $dst1);
    else
      dump_copy("$mov_opcode", REG_RAX, $dst1->r, '$schar');

    if ($dst1->spilled)
      dumpRM("$mov_opcode", REG_RDX, '$schar', $dst2);
    else
      dump_copy("$mov_opcode", REG_RDX, $dst2->r, '$schar');
  },
  emit {
    rtx src1 = gen_rtx_REG($rtx_mode, $src1a->r);
    rtx src2 = $isRR
      ? gen_rtx_REG($rtx_mode, $src1b->r)
      : gen_rtx_MEM($rtx_mode, $src1b->rtl);
    rtx dst1 = $dst1->spilled
      ? gen_rtx_MEM($rtx_mode, $dst1->rtl)
      : gen_rtx_REG($rtx_mode, $dst1->r);
    rtx dst2 = $dst2->spilled
      ? gen_rtx_MEM($rtx_mode, $dst2->rtl)
      : gen_rtx_REG($rtx_mode, $dst2->r);
    rtx rax  = gen_rtx_REG($rtx_mode, REG_RAX);
    rtx rdx  = gen_rtx_REG($rtx_mode, REG_RDX);
    icg_emit_plain(gen_rtx_SET(VOIDmode, rax, src1));
    icg_emit_plain(gen_rtx_PARALLEL(VOIDmode,
      gen_rtvec(3,
	gen_rtx_SET(VOIDmode, rax, gen_rtx_DIV_helper($issigned, $rtx_mode, rax, src2)),
	gen_rtx_SET(VOIDmode, rdx, gen_rtx_MOD_helper($issigned, $rtx_mode, rax, src2)),
	gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(CCmode, FLAGS_REG)))));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst1, rax));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst2, rdx));
  };
""")


plug.plugrule3("pardivmod_hi", [
  ["rule", "cost"],

  ["""stmt : PARALLEL_ALL(SET_ALL(lhs16.dst1, UDIV_HI(r16.src1a, r16.src1b)),
		          PARALLEL_ALL(SET_ALL(lhs16.dst2, UMOD_HI(r16.src2a, r16.src2b)),
				       USE_ALL(CONST_0)))""", [52, 8]],
], """
  $rule $cost
  supairs {
    /* suOrder2($stmt, $dst1, $src1a, kid, kids); */
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src1a->r, REG_RAX);
    if (!$dst1->spilled)
      coalesces += attempt_coalesce(pass, $dst1->r, REG_RAX);
    if (!$dst2->spilled)
      coalesces += attempt_coalesce(pass, $dst2->r, REG_RDX);
  },
  build {
    /* final copy instruction */
    if ($dst2->spilled) {
      sparseset_set_bit(live, REG_RDX);
      add_addr(live, $dst2);
    }
    else
      add_copy_edges($dst2->r, REG_RDX, live);

    /* penultimate copy instruction */
    if ($dst1->spilled) {
      sparseset_set_bit(live, REG_RAX);
      add_addr(live, $dst1);
    }
    else
      add_copy_edges($dst1->r, REG_RAX, live);

    /* divide instruction */
    add_edges(REG_RAX, live);
    sparseset_set_bit(live, find($src1b->r));

    /* xor instruction */
    sparseset_clear_bit(live, REG_RDX);
    add_edges(REG_RDX, live);

    /* final copy instruction */
    add_copy_edges(REG_RAX, $src1a->r, live);
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($src1a->r);
    cost_copy($src1a->r, REG_RAX);
    memorable($src1b->r);
    if (!$dst1->spilled)
      cost_copy($dst1->r, REG_RAX);
    if (!$dst2->spilled)
      cost_copy(REG_RDX, $dst2->r);
  },
  debug {
    dump_copy("movl", $src1a->r, REG_RAX, 'd'); /* use movw? no, movl is preferred */
    dumpRR("xorl", REG_RDX, 'd', REG_RDX, 'd'); /* use xorw? no, xorl is preferred */
    dumpR("divw", $src1b->r, 'w');
    dump_copy("movl", REG_RAX, $dst1->r, 'd'); /* use movw? no, movl is preferred */
    dump_copy("movl", REG_RDX, $dst2->r, 'd'); /* use movw? no, movl is preferred */
  },
  emit {
    rtx dst1 = $dst1->spilled
      ? gen_rtx_MEM(HImode, $dst1->rtl)
      : gen_rtx_REG(HImode, $dst1->r);
    rtx dst2 = $dst2->spilled
      ? gen_rtx_MEM(HImode, $dst2->rtl)
      : gen_rtx_REG(HImode, $dst2->r);

    rtx src1 = gen_rtx_REG(HImode, $src1a->r);
    rtx src2 = gen_rtx_REG(HImode, $src1b->r);
    rtx rax  = gen_rtx_REG(HImode, REG_RAX);
    rtx rdx  = gen_rtx_REG(HImode, REG_RDX);
    rtx rdxm = gen_rtx_REG(SImode, REG_RDX);
    icg_emit_plain(gen_rtx_SET(VOIDmode, rax, src1));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, rdxm,
			     gen_rtx_XOR(SImode, rdxm, rdxm)));
    icg_emit_plain(gen_rtx_PARALLEL(VOIDmode,
      gen_rtvec(4,
	gen_rtx_SET(VOIDmode, rax, gen_rtx_UDIV(HImode, rax, src2)),
	gen_rtx_SET(VOIDmode, rdx, gen_rtx_UMOD(HImode, rax, src2)),
	gen_rtx_USE(VOIDmode, rdx),
	gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(CCmode, FLAGS_REG)))));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst1, rax));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst2, rdx));
  };
""")


#--------------------------------------------------------------
#
# Shifts by a variable count (versus a constant) require that the shift
# count be in register %cl.  When we generate code, we'll have a copy
# from some symbolic register containing the count into %cl.
# With luck, the copy will be coalesced away during register allocation.
# For purposes of burg costs, we'll assume the coalesce is going to succeed.
#
# To help make sure the coalesce succeeds, we require that the register
# holding the count be an RX.  Will this really help?  Someday an experiment
# should be conducted.
#
# There are 3 varieties of shift in RTL
#   ASHIFT   - shift left, implemented via sal
#   ASHIFTRT - arithmetic shift right, implemented via sar
#   LSHIFTRT - logical shift right, implemented via shr
#   ROTATE   - rotate left, implemented via rol
#   ROTATERT - rotate right, implemented via ror
#
# The shift instructions do not set the condition code flags if the shift count is 0.
# Consequently, we do not use the "ccr...x" non-terminal as the destination.
# Indeed, it seems that we may never rely on the condition codes set by a shift of any sort.
# Note that shift by 1 sets the OF flag; other shifts don't.  Is implementation dependent.
#
plug.plugrule3("shiftrr", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder"],

  # we start with ASHIFT
  ["r64x.dst  : ASHIFT_DI(r64x.src1,  r8x.src2)", [1, 3], "salq", 'q', "DImode", "gen_rtx_ASHIFT"],
  ["rz32x.dst : ASHIFT_SI(r32x.src1,  r8x.src2)", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],
  ["r16x.dst  : ASHIFT_HI(r16x.src1,  r8x.src2)", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],
  ["r8x.dst   : ASHIFT_QI(r8x.src1,   r8x.src2)", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],

  # we can preserve the zero-extension
  ["rz16x.dst : ASHIFT_HI(rz16x.src1, r8x.src2)", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],
  ["rz8x.dst  : ASHIFT_QI(rz8x.src1,  r8x.src2)", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],

  # it's worth doing this with a 32-bit op
  ["r16x.dst  : ASHIFT_HI(r16x.src1, r8x.src2)",  [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],

  # usually, the shift count is subreg'd to 8-bits
  ["r64x.dst  : ASHIFT_DI(r64x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 3], "salq", 'q', "DImode", "gen_rtx_ASHIFT"],
  ["r64x.dst  : ASHIFT_DI(r64x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 3], "salq", 'q', "DImode", "gen_rtx_ASHIFT"],
  ["r64x.dst  : ASHIFT_DI(r64x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 3], "salq", 'q', "DImode", "gen_rtx_ASHIFT"],

  ["rz32x.dst : ASHIFT_SI(r32x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],
  ["rz32x.dst : ASHIFT_SI(r32x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],
  ["rz32x.dst : ASHIFT_SI(r32x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],

  ["r16x.dst  : ASHIFT_HI(r16x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],
  ["r16x.dst  : ASHIFT_HI(r16x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],
  ["r16x.dst  : ASHIFT_HI(r16x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],

  ["r8x.dst   : ASHIFT_QI(r8x.src1,   SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],
  ["r8x.dst   : ASHIFT_QI(r8x.src1,   SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],
  ["r8x.dst   : ASHIFT_QI(r8x.src1,   SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],

  # we can preserve the zero-extension
  ["rz16x.dst : ASHIFT_HI(rz16x.src1, SUBREG_QI(r64x.src2, CONST_0))", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],
  ["rz16x.dst : ASHIFT_HI(rz16x.src1, SUBREG_QI(r32x.src2, CONST_0))", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],
  ["rz16x.dst : ASHIFT_HI(rz16x.src1, SUBREG_QI(r16x.src2, CONST_0))", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],

  ["rz8x.dst  : ASHIFT_QI(rz8x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],
  ["rz8x.dst  : ASHIFT_QI(rz8x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],
  ["rz8x.dst  : ASHIFT_QI(rz8x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],

  # it's worth doing these with a 32-bit op
  ["r16x.dst  : ASHIFT_HI(r16x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],
  ["r16x.dst  : ASHIFT_HI(r16x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],
  ["r16x.dst  : ASHIFT_HI(r16x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],



  # ASHIFTRT is organized the same way
  ["r64x.dst  : ASHIFTRT_DI(r64x.src1,  r8x.src2)", [1, 3], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT"],
  ["rz32x.dst : ASHIFTRT_SI(r32x.src1,  r8x.src2)", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],
  ["r16x.dst  : ASHIFTRT_HI(r16x.src1,  r8x.src2)", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],
  ["r8x.dst   : ASHIFTRT_QI(r8x.src1,   r8x.src2)", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],

  # we can preserve the zero-extension
  ["rz16x.dst : ASHIFTRT_HI(rz16x.src1, r8x.src2)", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],
  ["rz8x.dst  : ASHIFTRT_QI(rz8x.src1,  r8x.src2)", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],

  # we can preserve the sign-extension, using 32-bit op to save a byte on the 16-bit ops
  ["rs16x.dst : ASHIFTRT_HI(rs16x.src1, r8x.src2)", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],
  ["rs8x.dst  : ASHIFTRT_QI(rs8x.src1,  r8x.src2)", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],

  # usually, the shift count is subreg'd to 8-bits
  ["r64x.dst  : ASHIFTRT_DI(r64x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 3], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT"],
  ["r64x.dst  : ASHIFTRT_DI(r64x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 3], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT"],
  ["r64x.dst  : ASHIFTRT_DI(r64x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 3], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT"],

  ["rz32x.dst : ASHIFTRT_SI(r32x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],
  ["rz32x.dst : ASHIFTRT_SI(r32x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],
  ["rz32x.dst : ASHIFTRT_SI(r32x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],

  ["r16x.dst  : ASHIFTRT_HI(r16x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],
  ["r16x.dst  : ASHIFTRT_HI(r16x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],
  ["r16x.dst  : ASHIFTRT_HI(r16x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],

  ["r8x.dst   : ASHIFTRT_QI(r8x.src1,   SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],
  ["r8x.dst   : ASHIFTRT_QI(r8x.src1,   SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],
  ["r8x.dst   : ASHIFTRT_QI(r8x.src1,   SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],

  # we can preserve the zero-extension
  ["rz16x.dst : ASHIFTRT_HI(rz16x.src1, SUBREG_QI(r64x.src2, CONST_0))", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],
  ["rz16x.dst : ASHIFTRT_HI(rz16x.src1, SUBREG_QI(r32x.src2, CONST_0))", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],
  ["rz16x.dst : ASHIFTRT_HI(rz16x.src1, SUBREG_QI(r16x.src2, CONST_0))", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],

  ["rz8x.dst  : ASHIFTRT_QI(rz8x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],
  ["rz8x.dst  : ASHIFTRT_QI(rz8x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],
  ["rz8x.dst  : ASHIFTRT_QI(rz8x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],

  # we can preserve the sign-extension, using 32-bit op to save a byte on the 16-bit ops
  ["rs16x.dst : ASHIFTRT_HI(rs16x.src1, SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],
  ["rs16x.dst : ASHIFTRT_HI(rs16x.src1, SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],
  ["rs16x.dst : ASHIFTRT_HI(rs16x.src1, SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],

  ["rs8x.dst  : ASHIFTRT_QI(rs8x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],
  ["rs8x.dst  : ASHIFTRT_QI(rs8x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],
  ["rs8x.dst  : ASHIFTRT_QI(rs8x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],


  # LSHIFTRT is organized the same way
  ["r64x.dst  : LSHIFTRT_DI(r64x.src1,  r8x.src2)", [1, 3], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT"],
  ["rz32x.dst : LSHIFTRT_SI(r32x.src1,  r8x.src2)", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],
  ["r16x.dst  : LSHIFTRT_HI(r16x.src1,  r8x.src2)", [1, 3], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT"],
  ["r8x.dst   : LSHIFTRT_QI(r8x.src1,   r8x.src2)", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],

  # we can preserve the zero-extension, using 32-bit op to save a byte on the 16-bit ops
  ["rz16x.dst : LSHIFTRT_HI(rz16x.src1, r8x.src2)", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],
  ["rz8x.dst  : LSHIFTRT_QI(rz8x.src1,  r8x.src2)", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],

  # usually, the shift count is subreg'd to 8-bits
  ["r64x.dst  : LSHIFTRT_DI(r64x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 3], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT"],
  ["r64x.dst  : LSHIFTRT_DI(r64x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 3], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT"],
  ["r64x.dst  : LSHIFTRT_DI(r64x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 3], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT"],

  ["rz32x.dst : LSHIFTRT_SI(r32x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],
  ["rz32x.dst : LSHIFTRT_SI(r32x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],
  ["rz32x.dst : LSHIFTRT_SI(r32x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],

  ["r16x.dst  : LSHIFTRT_HI(r16x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 3], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT"],
  ["r16x.dst  : LSHIFTRT_HI(r16x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 3], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT"],
  ["r16x.dst  : LSHIFTRT_HI(r16x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 3], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT"],

  ["r8x.dst   : LSHIFTRT_QI(r8x.src1,   SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],
  ["r8x.dst   : LSHIFTRT_QI(r8x.src1,   SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],
  ["r8x.dst   : LSHIFTRT_QI(r8x.src1,   SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],

  # we can preserve the zero-extension, using 32-bit op to save a byte on the 16-bit ops
  ["rz16x.dst : LSHIFTRT_HI(rz16x.src1, SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],
  ["rz16x.dst : LSHIFTRT_HI(rz16x.src1, SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],
  ["rz16x.dst : LSHIFTRT_HI(rz16x.src1, SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],

  ["rz8x.dst  : LSHIFTRT_QI(rz8x.src1,  SUBREG_QI(r64x.src2, CONST_0))", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],
  ["rz8x.dst  : LSHIFTRT_QI(rz8x.src1,  SUBREG_QI(r32x.src2, CONST_0))", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],
  ["rz8x.dst  : LSHIFTRT_QI(rz8x.src1,  SUBREG_QI(r16x.src2, CONST_0))", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],

  # ROTATE is organized the same way, but without any sign/zero extension tricks
  ["r64x.dst  :   ROTATE_DI(r64x.src1,  r8x.src2)", [1, 3], "rolq", 'q', "DImode", "gen_rtx_ROTATE"],
  ["r32x.dst  :   ROTATE_SI(r32x.src1,  r8x.src2)", [1, 2], "roll", 'l', "SImode", "gen_rtx_ROTATE"],
  ["r16x.dst  :   ROTATE_HI(r16x.src1,  r8x.src2)", [1, 3], "rolw", 'w', "HImode", "gen_rtx_ROTATE"],
  ["r8x.dst   :   ROTATE_QI(r8x.src1,   r8x.src2)", [1, 2], "rolb", 'b', "QImode", "gen_rtx_ROTATE"],

  # ROTATE_RT is organized the same way, but without any sign/zero extension tricks
  ["r64x.dst  : ROTATERT_DI(r64x.src1,  r8x.src2)", [1, 3], "rorq", 'q', "DImode", "gen_rtx_ROTATERT"],
  ["r32x.dst  : ROTATERT_SI(r32x.src1,  r8x.src2)", [1, 2], "rorl", 'l', "SImode", "gen_rtx_ROTATERT"],
  ["r16x.dst  : ROTATERT_HI(r16x.src1,  r8x.src2)", [1, 3], "rorw", 'w', "HImode", "gen_rtx_ROTATERT"],
  ["r8x.dst   : ROTATERT_QI(r8x.src1,   r8x.src2)", [1, 2], "rorb", 'b', "QImode", "gen_rtx_ROTATERT"],

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
  coalesce {
    coalesces += attempt_coalesce(pass, $src2->rx, REG_RCX);
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
    add_copy_edges(REG_RCX, $src2->rx, live);
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($src1->rx);
    forgettable($src2->rx);
    cost_copy(REG_RCX, $src2->rx);
  },
  debug {
    dump_copy("movl", $src2->rx, REG_RCX, 'd');
    dumpRR("$opcode", REG_RCX, 'b', $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $src1->rx);
    rtx rcx = gen_rtx_REG(SImode, REG_RCX);
    rtx src = gen_rtx_REG(SImode, $src2->rx);
    icg_emit_plain(gen_rtx_SET(VOIDmode, rcx, src));
    src = $rtx_builder($rtx_mode, dst, gen_rtx_REG(QImode, REG_RCX));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


#
# The shift instructions do not set the condition code flags if the shift count is 0.
# Consequently, we do not use the "ccr...x" non-terminal as the destination.
#
plug.plugrule3("shiftir", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder"],

  # we start with ASHIFT
  ["r64x.dst  : ASHIFT_DI(r64x.src,    imm8.const)", [1, 3], "salq", 'q', "DImode", "gen_rtx_ASHIFT"],
  ["rz32x.dst : ASHIFT_SI(r32x.src,    imm8.const)", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],
  ["r16x.dst  : ASHIFT_HI(r16x.src,    imm8.const)", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],
  ["r8x.dst   : ASHIFT_QI(r8x.src,     imm8.const)", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],

  # we can preserve the zero extension
  ["rz16x.dst : ASHIFT_HI(rz16x.src,   imm8.const)", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],
  ["rz8x.dst  : ASHIFT_QI(rz8x.src,    imm8.const)", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],

  # it's worth doing this with a 32-bit op
  ["r16x.dst  : ASHIFT_HI(r16x.src,    imm8.const)", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],


  # repeat for ASHIFTRT
  ["r64x.dst  : ASHIFTRT_DI(r64x.src,  imm8.const)", [1, 3], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT"],
  ["rz32x.dst : ASHIFTRT_SI(r32x.src,  imm8.const)", [1, 2], "sarw", 'l', "SImode", "gen_rtx_ASHIFTRT"],
  ["r16x.dst  : ASHIFTRT_HI(r16x.src,  imm8.const)", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],
  ["r8x.dst   : ASHIFTRT_QI(r8x.src,   imm8.const)", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],

  # we can preserve the zero extension
  ["rz16x.dst : ASHIFTRT_HI(rz16x.src, imm8.const)", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],
  ["rz8x.dst  : ASHIFTRT_QI(rz8x.src,  imm8.const)", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],

  # we can preserve the sign extension, using a 32-bit op to save a byte over the 16-bit op
  ["rs16x.dst : ASHIFTRT_HI(rs16x.src, imm8.const)", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],
  ["rs8x.dst  : ASHIFTRT_QI(rs8x.src,  imm8.const)", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],


  # repeat for LSHIFTRT
  ["r64x.dst  : LSHIFTRT_DI(r64x.src,  imm8.const)", [1, 3], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT"],
  ["rz32x.dst : LSHIFTRT_SI(r32x.src,  imm8.const)", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],
  ["r16x.dst  : LSHIFTRT_HI(r16x.src,  imm8.const)", [1, 3], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT"],
  ["r8x.dst   : LSHIFTRT_QI(r8x.src,   imm8.const)", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],

  # we can preserve the zero extension, using a 32-bit op to save a byte over the 16-bit op
  ["rz16x.dst : LSHIFTRT_HI(rz16x.src, imm8.const)", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],
  ["rz8x.dst  : LSHIFTRT_QI(rz8x.src,  imm8.const)", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],

  #
  # repeat for ROTATE, but without any tracking of sign/zero extension
  #
  ["r64x.dst  :   ROTATE_DI(r64x.src,    imm8.const)", [1, 3], "rolq", 'q', "DImode", "gen_rtx_ROTATE"],
  ["r32x.dst  :   ROTATE_SI(r32x.src,    imm8.const)", [1, 2], "roll", 'l', "SImode", "gen_rtx_ROTATE"],
  ["r16x.dst  :   ROTATE_HI(r16x.src,    imm8.const)", [1, 3], "rolw", 'w', "HImode", "gen_rtx_ROTATE"],
  ["r8x.dst   :   ROTATE_QI(r8x.src,     imm8.const)", [1, 2], "rolb", 'b', "QImode", "gen_rtx_ROTATE"],

  ["r64x.dst  : ROTATERT_DI(r64x.src,    imm8.const)", [1, 3], "rorq", 'q', "DImode", "gen_rtx_ROTATERT"],
  ["r32x.dst  : ROTATERT_SI(r32x.src,    imm8.const)", [1, 2], "rorl", 'l', "SImode", "gen_rtx_ROTATERT"],
  ["r16x.dst  : ROTATERT_HI(r16x.src,    imm8.const)", [1, 3], "rorw", 'w', "HImode", "gen_rtx_ROTATERT"],
  ["r8x.dst   : ROTATERT_QI(r8x.src,     imm8.const)", [1, 2], "rorb", 'b', "QImode", "gen_rtx_ROTATERT"],

], """
  $rule $cost
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  },
  names {
    $dst->rx = $src->rx;
  },
  final {
    $dst->rx = $src->rx;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
  },
  costs {
    forgettable($src->rx);
  },
  debug {
    dumpIR("$opcode", $const, $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src = $rtx_builder($rtx_mode,
			     gen_rtx_REG($rtx_mode, $src->rx),
			     gen_rtx_imm_constant($const->val, $const->a.string, $const->rtl));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


#
# Since we're shifting by 1 here, we can be confident that the CC is set correctly.
# Or can we?  I don't understand this yet, so I'm going to leave out the ccr cases.
#

plug.plugrule3("shift1r", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder"],

  # we start with ASHIFT
  ["r64x.dst  : ASHIFT_DI(r64x.src,    CONST_P1)", [1, 3], "salq", 'q', "DImode", "gen_rtx_ASHIFT"],
  ["rz32x.dst : ASHIFT_SI(r32x.src,    CONST_P1)", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],
  ["r16x.dst  : ASHIFT_HI(r16x.src,    CONST_P1)", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],
  ["r8x.dst   : ASHIFT_QI(r8x.src,     CONST_P1)", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],

  # we can preserve the zero extension
  ["rz16x.dst : ASHIFT_HI(rz16x.src,   CONST_P1)", [1, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT"],
  ["rz8x.dst  : ASHIFT_QI(rz8x.src,    CONST_P1)", [1, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT"],

  # it's sometimes worth doing this with a 32-bit op, though we lose the CC
  ["r16x.dst  : ASHIFT_HI(r16x.src,    CONST_P1)", [1, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT"],


  # ASHIFTRT is organized the same way
  ["r64x.dst  : ASHIFTRT_DI(r64x.src,  CONST_P1)", [1, 3], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT"],
  ["rz32x.dst : ASHIFTRT_SI(r32x.src,  CONST_P1)", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],
  ["r16x.dst  : ASHIFTRT_HI(r16x.src,  CONST_P1)", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],
  ["r8x.dst   : ASHIFTRT_QI(r8x.src,   CONST_P1)", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],

  # we can preserve the zero extension
  ["rz16x.dst : ASHIFTRT_HI(rz16x.src, CONST_P1)", [1, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT"],
  ["rz8x.dst  : ASHIFTRT_QI(rz8x.src,  CONST_P1)", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],

  # we can preserve the sign extension, using 32-bit op to save a byte on the 16-bit ops
  ["rs16x.dst : ASHIFTRT_HI(rs16x.src, CONST_P1)", [1, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT"],
  ["rs8x.dst  : ASHIFTRT_QI(rs8x.src,  CONST_P1)", [1, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT"],


  # LSHIFTRT is organized the same way
  ["r64x.dst  : LSHIFTRT_DI(r64x.src,  CONST_P1)", [1, 3], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT"],
  ["rz32x.dst : LSHIFTRT_SI(r32x.src,  CONST_P1)", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],
  ["r16x.dst  : LSHIFTRT_HI(r16x.src,  CONST_P1)", [1, 3], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT"],
  ["r8x.dst   : LSHIFTRT_QI(r8x.src,   CONST_P1)", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],

  # we can preserve the zero-extension, using 32-bit op to save a byte on the 16-bit ops
  ["rz16x.dst : LSHIFTRT_HI(rz16x.src, CONST_P1)", [1, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT"],
  ["rz8x.dst  : LSHIFTRT_QI(rz8x.src,  CONST_P1)", [1, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT"],

  ["r64x.dst  :   ROTATE_DI(r64x.src,    CONST_P1)", [1, 3], "rolq", 'q', "DImode", "gen_rtx_ROTATE"],
  ["r32x.dst  :   ROTATE_SI(r32x.src,    CONST_P1)", [1, 2], "roll", 'l', "SImode", "gen_rtx_ROTATE"],
  ["r16x.dst  :   ROTATE_HI(r16x.src,    CONST_P1)", [1, 3], "rolw", 'w', "HImode", "gen_rtx_ROTATE"],
  ["r8x.dst   :   ROTATE_QI(r8x.src,     CONST_P1)", [1, 2], "rolb", 'b', "QImode", "gen_rtx_ROTATE"],
  ["r64x.dst  : ROTATERT_DI(r64x.src,    CONST_P1)", [1, 3], "rorq", 'q', "DImode", "gen_rtx_ROTATERT"],
  ["r32x.dst  : ROTATERT_SI(r32x.src,    CONST_P1)", [1, 2], "rorl", 'l', "SImode", "gen_rtx_ROTATERT"],
  ["r16x.dst  : ROTATERT_HI(r16x.src,    CONST_P1)", [1, 3], "rorw", 'w', "HImode", "gen_rtx_ROTATERT"],
  ["r8x.dst   : ROTATERT_QI(r8x.src,     CONST_P1)", [1, 2], "rorb", 'b', "QImode", "gen_rtx_ROTATERT"],

], """
  $rule $cost
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  },
  names {
    $dst->rx = $src->rx;
  },
  final {
    $dst->rx = $src->rx;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
  },
  costs {
    forgettable($src->rx);
  },
  debug {
    dumpIR1("$opcode", $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src = $rtx_builder($rtx_mode,
			       gen_rtx_REG($rtx_mode, $src->rx),
			       GEN_INT(1));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")



#
#------------------------------------------------------------
# load effective address
#
# We can't use the obvious chain rule
#
#    r64x : addr
#
# because we seem to get into some inconvenient cycle
# and some of the Node's fields get overwritten.
#
# Note that LEA doesn't set the condition code, so we
# always define an rx rather than a ccrx
#
# Since LEA is a 3-address instruction, there's no question
# of preserving zero or sign extensions.
#
# We use the 32-bit LEA for the 16-bit form, to save a byte,
# and for the 8-bit form, because there is no 8-bit LEA.
#
plug.plugrule3("leabd", [
  ["rule", "cost", "opcode", "schar", "rtx_mode"],

  ["r64x.dst  : PLUS_DI(base64.base | disp.disp)", [1, 2], "leaq", 'q', "DImode"],
  ["rz32x.dst : PLUS_SI(base32.base | disp.disp)", [1, 1], "leal", 'l', "SImode"],
  ["r16x.dst  : PLUS_HI(base16.base | disp.disp)", [1, 1], "leal", 'l', "SImode"],
  ["r8x.dst   : PLUS_QI(base8.base  | disp.disp)", [1, 1], "leal", 'l', "SImode"],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    if ($base->freed > 0) {
      $dst->extra = $base->extra;
      $dst->freed = $base->freed - 1;
    }
    else {
      $dst->extra = $base->extra + 1;
      $dst->freed = 0;
    }
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($base->a.base));
  },
  remat {
    unsigned r = find($base->a.base);
    if (r == REG_FP || r == REG_ARGP)
      flags |= RHS_REMAT;
    else
      flags = 0;
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dumpMbidR("$opcode",
      $base->a.base, true, 0, 0, $disp->a.disp, $disp->a.string,
      $dst->rx, '$schar');
  },
  emit {
    rtx src = gen_rtx_PLUS($rtx_mode, $base->rtl, $disp->rtl);
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    icg_emit_clobber_tagged(gen_rtx_SET(VOIDmode, dst, src), __FILE__, __LINE__);
    /* the above line must be "clobber", not "plain" */
  };
""")

plug.plugrule3("leabi", [
  ["rule", "cost", "opcode", "schar", "rtx_mode"],

  ["r64x.dst  : PLUS_DI(r64.base | r64.index)", [1, 3], "leaq", 'q', "DImode"],
  ["rz32x.dst : PLUS_SI(r32.base | r32.index)", [1, 2], "leal", 'l', "SImode"],
  ["r16x.dst  : PLUS_HI(r16.base | r16.index)", [1, 2], "leal", 'l', "SImode"],
  ["r8x.dst   : PLUS_QI(r8.base  | r8.index)",  [1, 2], "leal", 'l', "SImode"],

  ["r64x.dst  : MINUS_DI(r64.base, NEG_DI(r64.index))", [1, 3], "leaq", 'q', "DImode"],
  ["rz32x.dst : MINUS_SI(r32.base, NEG_SI(r32.index))", [1, 2], "leal", 'l', "SImode"],
  ["r16x.dst  : MINUS_HI(r16.base, NEG_HI(r16.index))", [1, 2], "leal", 'l', "SImode"],
  ["r8x.dst   : MINUS_QI(r8.base,  NEG_QI(r8.index))",  [1, 2], "leal", 'l', "SImode"],

], """
  $rule $cost
  supairs {
    suOrder2($dst, $base, $index, kid, kids);
  },
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($base->r));
    sparseset_set_bit(live, find($index->r));
  },
  remat {
    flags = 0;
  },
  costs {
    memorable($base->r);
    memorable($index->r);
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dumpMbidR("$opcode",
      $base->r, true, $index->r, 1, 0, NULL,
      $dst->rx, '$schar');
  },
  emit {
    rtx src;
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src1 = gen_rtx_REG($rtx_mode, $base->r);
    rtx src2 = gen_rtx_REG($rtx_mode, $index->r);
    if ($base->a.base == $dst->rx)
      src = gen_rtx_PLUS($rtx_mode, src1, src2);
    else
      src = gen_rtx_PLUS($rtx_mode, src2, src1);
    icg_emit_clobber_tagged(gen_rtx_SET(VOIDmode, dst, src), __FILE__, __LINE__);
    /* the above line must be "clobber", not "plain" */
  };
""")

plug.plugrule3("leabid", [
  ["rule", "cost", "opcode", "schar", "rtx_mode"],

  ["r64x.dst  : PLUS_DI(base64.base | index64.index | disp.disp)", [2, 2], "leaq", 'q', "DImode"],
  ["rz32x.dst : PLUS_SI(base32.base | index32.index | disp.disp)", [2, 1], "leal", 'l', "SImode"],
  ["r16x.dst  : PLUS_HI(base16.base | index16.index | disp.disp)", [2, 1], "leal", 'l', "SImode"],
  ["r8x.dst   : PLUS_QI(base8.base  | index8.index  | disp.disp)", [2, 1], "leal", 'l', "SImode"],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    suOrder2($dst, $base, $index, kid, kids);
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($base->a.base));
    sparseset_set_bit(live, find($index->a.index));
  },
  remat {
    flags = 0;
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dumpMbidR("$opcode",
      $base->a.base, true, $index->a.index, $index->a.scale, $disp->a.disp, $disp->a.string,
      $dst->rx, '$schar');
  },
  emit {
    /*
     * for some reason, 64 bit leaq must have the gcc tree
     * built slightly differently, or the back end of gcc will
     * not be able to match it.  gen_rtx_addr will build a tree
     * from two gen_rtx_PLUS, but with a different permutation
     * than we use for the other kinds of lea instruction.
     */
    const rtx src = ('$schar' == 'q')
    ? gen_rtx_addr($rtx_mode, $base->rtl, $index->rtl, $disp->rtl)
    : gen_rtx_PLUS(SImode, gen_rtx_PLUS($rtx_mode, $index->rtl, $base->rtl), $disp->rtl)
    ;
    const rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    icg_emit_plain_tagged(gen_rtx_SET(VOIDmode, dst, src), __FILE__, __LINE__);
    /* the above line must be "plain", not "clobber" */
  };
""")


#
# We need to exercise some care here to be sure that
# mcon*disp doesn't overflow it's allotted size.
#

plug.plugrule3("leabis", [
  ["rule", "cost", "opcode", "schar", "scale"],

  ["r64x.dst  : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm5.const)  | CONST_P8))", [2, 2], "leaq", 'q', 8],
  ["r64x.dst  : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm29.const) | CONST_P8))", [2, 2], "leaq", 'q', 8],
  ["r64x.dst  : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm6.const)  | CONST_P4))", [2, 2], "leaq", 'q', 4],
  ["r64x.dst  : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm30.const) | CONST_P4))", [2, 2], "leaq", 'q', 4],
  ["r64x.dst  : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm7.const)  | CONST_P2))", [2, 2], "leaq", 'q', 2],
  ["r64x.dst  : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm31.const) | CONST_P2))", [2, 2], "leaq", 'q', 2],

  ["r64x.dst  : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm5.const),  CONST_P3))", [2, 2], "leaq", 'q', 8],
  ["r64x.dst  : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm29.const), CONST_P3))", [2, 2], "leaq", 'q', 8],
  ["r64x.dst  : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm6.const),  CONST_P2))", [2, 2], "leaq", 'q', 4],
  ["r64x.dst  : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm30.const), CONST_P2))", [2, 2], "leaq", 'q', 4],
  ["r64x.dst  : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm7.const),  CONST_P1))", [2, 2], "leaq", 'q', 2],
  ["r64x.dst  : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm31.const), CONST_P1))", [2, 2], "leaq", 'q', 2],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    suOrder2($dst, $base, $index, kid, kids);
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($base->a.base));
    sparseset_set_bit(live, find($index->r));
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($index->r);
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dumpMbidR("$opcode",
      $base->a.base, true, $index->r, $scale, $const->val*$scale, NULL,
      $dst->rx, '$schar');
  },
  emit {
    rtx src2 = gen_rtx_REG(DImode, $index->r);
    rtx src = gen_rtx_addr(DImode, $base->rtl,
			   gen_rtx_MULT(DImode, src2, GEN_INT($scale)),
			   GEN_INT($const->val*$scale));
    rtx dst = gen_rtx_REG(DImode, $dst->rx);
    icg_emit_plain_tagged(gen_rtx_SET(VOIDmode, dst, src), __FILE__, __LINE__);
    /* the above line must be "plain", not "clobber" */
  };
""")

plug.plugrule3("leabis1", [
  ["rule", "cost", "opcode", "schar", "scale"],

  ["rz32x.dst : PLUS_SI(base32.base | MULT_SI(PLUS_SI(r32.index | imm5.const)  | CONST_P8))", [2, 1], "leal", 'l', 8],
  ["rz32x.dst : PLUS_SI(base32.base | MULT_SI(PLUS_SI(r32.index | imm29.const) | CONST_P8))", [2, 1], "leal", 'l', 8],
  ["r16x.dst  : PLUS_HI(base16.base | MULT_HI(PLUS_HI(r16.index | imm5.const)  | CONST_P8))", [2, 1], "leal", 'l', 8],
  ["r16x.dst  : PLUS_HI(base16.base | MULT_HI(PLUS_HI(r16.index | imm13.const) | CONST_P8))", [2, 1], "leal", 'l', 8],
  ["r8x.dst   : PLUS_QI(base8.base  | MULT_QI(PLUS_QI(r8.index  | imm5.const)  | CONST_P8))", [2, 1], "leal", 'l', 8],
  ["rz32x.dst : PLUS_SI(base32.base | MULT_SI(PLUS_SI(r32.index | imm6.const)  | CONST_P4))", [2, 1], "leal", 'l', 4],
  ["rz32x.dst : PLUS_SI(base32.base | MULT_SI(PLUS_SI(r32.index | imm30.const) | CONST_P4))", [2, 1], "leal", 'l', 4],
  ["r16x.dst  : PLUS_HI(base16.base | MULT_HI(PLUS_HI(r16.index | imm6.const)  | CONST_P4))", [2, 1], "leal", 'l', 4],
  ["r16x.dst  : PLUS_HI(base16.base | MULT_HI(PLUS_HI(r16.index | imm14.const) | CONST_P4))", [2, 1], "leal", 'l', 4],
  ["r8x.dst   : PLUS_QI(base8.base  | MULT_QI(PLUS_QI(r8.index  | imm6.const)  | CONST_P4))", [2, 1], "leal", 'l', 4],
  ["rz32x.dst : PLUS_SI(base32.base | MULT_SI(PLUS_SI(r32.index | imm7.const)  | CONST_P2))", [2, 1], "leal", 'l', 2],
  ["rz32x.dst : PLUS_SI(base32.base | MULT_SI(PLUS_SI(r32.index | imm31.const) | CONST_P2))", [2, 1], "leal", 'l', 2],
  ["r16x.dst  : PLUS_HI(base16.base | MULT_HI(PLUS_HI(r16.index | imm7.const)  | CONST_P2))", [2, 1], "leal", 'l', 2],
  ["r16x.dst  : PLUS_HI(base16.base | MULT_HI(PLUS_HI(r16.index | imm15.const) | CONST_P2))", [2, 1], "leal", 'l', 2],
  ["r8x.dst   : PLUS_QI(base8.base  | MULT_QI(PLUS_QI(r8.index  | imm7.const)  | CONST_P2))", [2, 1], "leal", 'l', 2],

  ["rz32x.dst : PLUS_SI(base32.base | ASHIFT_SI(PLUS_SI(r32.index | imm5.const),  CONST_P3))", [2, 1], "leal", 'l', 8],
  ["rz32x.dst : PLUS_SI(base32.base | ASHIFT_SI(PLUS_SI(r32.index | imm29.const), CONST_P3))", [2, 1], "leal", 'l', 8],
  ["r16x.dst  : PLUS_HI(base16.base | ASHIFT_HI(PLUS_HI(r16.index | imm5.const),  CONST_P3))", [2, 1], "leal", 'l', 8],
  ["r16x.dst  : PLUS_HI(base16.base | ASHIFT_HI(PLUS_HI(r16.index | imm13.const), CONST_P3))", [2, 1], "leal", 'l', 8],
  ["r8x.dst   : PLUS_QI(base8.base  | ASHIFT_QI(PLUS_QI(r8.index  | imm5.const),  CONST_P3))", [2, 1], "leal", 'l', 8],
  ["rz32x.dst : PLUS_SI(base32.base | ASHIFT_SI(PLUS_SI(r32.index | imm6.const),  CONST_P2))", [2, 1], "leal", 'l', 4],
  ["rz32x.dst : PLUS_SI(base32.base | ASHIFT_SI(PLUS_SI(r32.index | imm30.const), CONST_P2))", [2, 1], "leal", 'l', 4],
  ["r16x.dst  : PLUS_HI(base16.base | ASHIFT_HI(PLUS_HI(r16.index | imm6.const),  CONST_P2))", [2, 1], "leal", 'l', 4],
  ["r16x.dst  : PLUS_HI(base16.base | ASHIFT_HI(PLUS_HI(r16.index | imm14.const), CONST_P2))", [2, 1], "leal", 'l', 4],
  ["r8x.dst   : PLUS_QI(base8.base  | ASHIFT_QI(PLUS_QI(r8.index  | imm6.const),  CONST_P2))", [2, 1], "leal", 'l', 4],
  ["rz32x.dst : PLUS_SI(base32.base | ASHIFT_SI(PLUS_SI(r32.index | imm7.const),  CONST_P1))", [2, 1], "leal", 'l', 2],
  ["rz32x.dst : PLUS_SI(base32.base | ASHIFT_SI(PLUS_SI(r32.index | imm31.const), CONST_P1))", [2, 1], "leal", 'l', 2],
  ["r16x.dst  : PLUS_HI(base16.base | ASHIFT_HI(PLUS_HI(r16.index | imm7.const),  CONST_P1))", [2, 1], "leal", 'l', 2],
  ["r16x.dst  : PLUS_HI(base16.base | ASHIFT_HI(PLUS_HI(r16.index | imm15.const), CONST_P1))", [2, 1], "leal", 'l', 2],
  ["r8x.dst   : PLUS_QI(base8.base  | ASHIFT_QI(PLUS_QI(r8.index  | imm7.const),  CONST_P1))", [2, 1], "leal", 'l', 2],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    suOrder2($dst, $base, $index, kid, kids);
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($base->a.base));
    sparseset_set_bit(live, find($index->r));
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($index->r);
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dumpMbidR("$opcode",
      $base->a.base, true, $index->r, $scale, $const->val*$scale, NULL,
      $dst->rx, '$schar');
  },
  emit {
    rtx src2 = gen_rtx_REG(SImode, $index->r);
    rtx src = gen_rtx_PLUS(SImode,
			   gen_rtx_PLUS(SImode,
					gen_rtx_MULT(SImode, src2, GEN_INT($scale)),
					$base->rtl),
			   GEN_INT($const->val*$scale));
    rtx dst = gen_rtx_REG(SImode, $dst->rx);
    icg_emit_plain_tagged(gen_rtx_SET(VOIDmode, dst, src), __FILE__, __LINE__);
    /* the above line must be "plain", not "clobber" */
  };
""")


# The costs here are strange because there are 2 immediates feeding into each rule
plug.plugrule3("leabis2", [
  ["rule", "cost", "opcode", "schar", "scale"],

  ["r64x.dst : PLUS_DI(base64.base | imm7.const1  |   MULT_DI(PLUS_DI(r64.index | imm4.const2)  | CONST_P8))", [2, 1], "leaq", 'q', 2],
  ["r64x.dst : PLUS_DI(base64.base | imm31.const1 |   MULT_DI(PLUS_DI(r64.index | imm28.const2) | CONST_P8))", [2, 0], "leaq", 'q', 2],
  ["r64x.dst : PLUS_DI(base64.base | imm7.const1  |   MULT_DI(PLUS_DI(r64.index | imm5.const2)  | CONST_P4))", [2, 1], "leaq", 'q', 2],
  ["r64x.dst : PLUS_DI(base64.base | imm31.const1 |   MULT_DI(PLUS_DI(r64.index | imm29.const2) | CONST_P4))", [2, 0], "leaq", 'q', 2],
  ["r64x.dst : PLUS_DI(base64.base | imm7.const1  |   MULT_DI(PLUS_DI(r64.index | imm6.const2)  | CONST_P2))", [2, 1], "leaq", 'q', 2],
  ["r64x.dst : PLUS_DI(base64.base | imm31.const1 |   MULT_DI(PLUS_DI(r64.index | imm30.const2) | CONST_P2))", [2, 0], "leaq", 'q', 2],

  ["r64x.dst : PLUS_DI(base64.base | imm7.const1  | ASHIFT_DI(PLUS_DI(r64.index | imm4.const2),  CONST_P3))", [2, 1], "leaq", 'q', 2],
  ["r64x.dst : PLUS_DI(base64.base | imm31.const1 | ASHIFT_DI(PLUS_DI(r64.index | imm28.const2), CONST_P3))", [2, 0], "leaq", 'q', 2],
  ["r64x.dst : PLUS_DI(base64.base | imm7.const1  | ASHIFT_DI(PLUS_DI(r64.index | imm5.const2),  CONST_P2))", [2, 1], "leaq", 'q', 2],
  ["r64x.dst : PLUS_DI(base64.base | imm31.const1 | ASHIFT_DI(PLUS_DI(r64.index | imm29.const2), CONST_P2))", [2, 0], "leaq", 'q', 2],
  ["r64x.dst : PLUS_DI(base64.base | imm7.const1  | ASHIFT_DI(PLUS_DI(r64.index | imm6.const2),  CONST_P1))", [2, 1], "leaq", 'q', 2],
  ["r64x.dst : PLUS_DI(base64.base | imm31.const1 | ASHIFT_DI(PLUS_DI(r64.index | imm30.const2), CONST_P1))", [2, 0], "leaq", 'q', 2],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    suOrder2($dst, $base, $index, kid, kids);
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($base->a.base));
    sparseset_set_bit(live, find($index->r));
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($index->r);
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dumpMbidR("$opcode",
      $base->a.base, true, $index->r, $scale, $const1->val + $const2->val*$scale, NULL,
      $dst->rx, '$schar');
  },
  emit {
    rtx src2 = gen_rtx_REG(DImode, $index->r);
    rtx src = gen_rtx_addr(DImode, $base->rtl,
			   gen_rtx_MULT(DImode, src2, GEN_INT($scale)),
			   GEN_INT($const1->val + $const2->val*$scale));
    rtx dst = gen_rtx_REG(DImode, $dst->rx);
    icg_emit_plain_tagged(gen_rtx_SET(VOIDmode, dst, src), __FILE__, __LINE__);
    /* the above line must be "plain", not "clobber" */
  };
""")


# The costs here are strange because there are 2 immediates feeding into each rule
plug.plugrule3("leabis1", [
  ["rule", "cost", "opcode", "schar", "scale"],

  ["rz32x.dst : PLUS_SI(base32.base | imm7.const1  | MULT_SI(PLUS_SI(r32.index | imm4.const2)  | CONST_P8))", [2, 0], "leal", 'l', 8],
  ["rz32x.dst : PLUS_SI(base32.base | imm31.const1 | MULT_SI(PLUS_SI(r32.index | imm28.const2) | CONST_P8))", [2, 0], "leal", 'l', 8],
  ["r16x.dst  : PLUS_HI(base16.base | imm7.const1  | MULT_HI(PLUS_HI(r16.index | imm5.const2)  | CONST_P8))", [2, 0], "leal", 'l', 8],
  ["r16x.dst  : PLUS_HI(base16.base | imm15.const1 | MULT_HI(PLUS_HI(r16.index | imm13.const2) | CONST_P8))", [2, 0], "leal", 'l', 8],
  ["r8x.dst   : PLUS_QI(base8.base  | imm7.const1  | MULT_QI(PLUS_QI(r8.index  | imm5.const2)  | CONST_P8))", [2, 0], "leal", 'l', 8],
  ["rz32x.dst : PLUS_SI(base32.base | imm7.const1  | MULT_SI(PLUS_SI(r32.index | imm6.const2)  | CONST_P4))", [2, 0], "leal", 'l', 4],
  ["rz32x.dst : PLUS_SI(base32.base | imm31.const1 | MULT_SI(PLUS_SI(r32.index | imm30.const2) | CONST_P4))", [2, 0], "leal", 'l', 4],
  ["r16x.dst  : PLUS_HI(base16.base | imm7.const1  | MULT_HI(PLUS_HI(r16.index | imm6.const2)  | CONST_P4))", [2, 0], "leal", 'l', 4],
  ["r16x.dst  : PLUS_HI(base16.base | imm15.const1 | MULT_HI(PLUS_HI(r16.index | imm14.const2) | CONST_P4))", [2, 0], "leal", 'l', 4],
  ["r8x.dst   : PLUS_QI(base8.base  | imm7.const1  | MULT_QI(PLUS_QI(r8.index  | imm6.const2)  | CONST_P4))", [2, 0], "leal", 'l', 4],
  ["rz32x.dst : PLUS_SI(base32.base | imm7.const1  | MULT_SI(PLUS_SI(r32.index | imm7.const2)  | CONST_P2))", [2, 0], "leal", 'l', 2],
  ["rz32x.dst : PLUS_SI(base32.base | imm31.const1 | MULT_SI(PLUS_SI(r32.index | imm31.const2) | CONST_P2))", [2, 0], "leal", 'l', 2],
  ["r16x.dst  : PLUS_HI(base16.base | imm7.const1  | MULT_HI(PLUS_HI(r16.index | imm7.const2)  | CONST_P2))", [2, 0], "leal", 'l', 2],
  ["r16x.dst  : PLUS_HI(base16.base | imm15.const1 | MULT_HI(PLUS_HI(r16.index | imm15.const2) | CONST_P2))", [2, 0], "leal", 'l', 2],
  ["r8x.dst   : PLUS_QI(base8.base  | imm7.const1  | MULT_QI(PLUS_QI(r8.index  | imm7.const2)  | CONST_P2))", [2, 0], "leal", 'l', 2],

  ["rz32x.dst : PLUS_SI(base32.base | imm7.const1  | ASHIFT_SI(PLUS_SI(r32.index | imm5.const2),  CONST_P3))", [2, 0], "leal", 'l', 8],
  ["rz32x.dst : PLUS_SI(base32.base | imm31.const1 | ASHIFT_SI(PLUS_SI(r32.index | imm29.const2), CONST_P3))", [2, 0], "leal", 'l', 8],
  ["r16x.dst  : PLUS_HI(base16.base | imm7.const1  | ASHIFT_HI(PLUS_HI(r16.index | imm5.const2),  CONST_P3))", [2, 0], "leal", 'l', 8],
  ["r16x.dst  : PLUS_HI(base16.base | imm15.const1 | ASHIFT_HI(PLUS_HI(r16.index | imm13.const2), CONST_P3))", [2, 0], "leal", 'l', 8],
  ["r8x.dst   : PLUS_QI(base8.base  | imm7.const1  | ASHIFT_QI(PLUS_QI(r8.index  | imm5.const2),  CONST_P3))", [2, 0], "leal", 'l', 8],
  ["rz32x.dst : PLUS_SI(base32.base | imm7.const1  | ASHIFT_SI(PLUS_SI(r32.index | imm6.const2),  CONST_P2))", [2, 0], "leal", 'l', 4],
  ["rz32x.dst : PLUS_SI(base32.base | imm31.const1 | ASHIFT_SI(PLUS_SI(r32.index | imm30.const2), CONST_P2))", [2, 0], "leal", 'l', 4],
  ["r16x.dst  : PLUS_HI(base16.base | imm7.const1  | ASHIFT_HI(PLUS_HI(r16.index | imm6.const2),  CONST_P2))", [2, 0], "leal", 'l', 4],
  ["r16x.dst  : PLUS_HI(base16.base | imm15.const1 | ASHIFT_HI(PLUS_HI(r16.index | imm14.const2), CONST_P2))", [2, 0], "leal", 'l', 4],
  ["r8x.dst   : PLUS_QI(base8.base  | imm7.const1  | ASHIFT_QI(PLUS_QI(r8.index  | imm6.const2),  CONST_P2))", [2, 0], "leal", 'l', 4],
  ["rz32x.dst : PLUS_SI(base32.base | imm7.const1  | ASHIFT_SI(PLUS_SI(r32.index | imm7.const2),  CONST_P1))", [2, 0], "leal", 'l', 2],
  ["rz32x.dst : PLUS_SI(base32.base | imm31.const1 | ASHIFT_SI(PLUS_SI(r32.index | imm31.const2), CONST_P1))", [2, 0], "leal", 'l', 2],
  ["r16x.dst  : PLUS_HI(base16.base | imm7.const1  | ASHIFT_HI(PLUS_HI(r16.index | imm7.const2),  CONST_P1))", [2, 0], "leal", 'l', 2],
  ["r16x.dst  : PLUS_HI(base16.base | imm15.const1 | ASHIFT_HI(PLUS_HI(r16.index | imm15.const2), CONST_P1))", [2, 0], "leal", 'l', 2],
  ["r8x.dst   : PLUS_QI(base8.base  | imm7.const1  | ASHIFT_QI(PLUS_QI(r8.index  | imm7.const2),  CONST_P1))", [2, 0], "leal", 'l', 2],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    suOrder2($dst, $base, $index, kid, kids);
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($base->a.base));
    sparseset_set_bit(live, find($index->r));
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($index->r);
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dumpMbidR("$opcode",
      $base->a.base, true, $index->r, $scale, $const1->val + $const2->val*$scale, NULL,
      $dst->rx, '$schar');
  },
  emit {
    rtx src2 = gen_rtx_REG(SImode, $index->r);
    rtx src = gen_rtx_PLUS(SImode,
			   gen_rtx_PLUS(SImode,
					gen_rtx_MULT(SImode, src2, GEN_INT($scale)),
					$base->rtl),
			   GEN_INT($const1->val + $const2->val*$scale));
    rtx dst = gen_rtx_REG(SImode, $dst->rx);
    icg_emit_plain_tagged(gen_rtx_SET(VOIDmode, dst, src), __FILE__, __LINE__);
    /* the above line must be "plain", not "clobber" */
  };
""")




plug.plugrule3("leabii", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "scale_factor", "order_key"],

  ["r64x.dst  : PLUS_DI(base64.base | index64.index)", [2, 2], "leaq", 'q', "DImode", 0, 1],
  ["rz32x.dst : PLUS_SI(base32.base | index32.index)", [2, 1], "leal", 'l', "SImode", 0, 0],
  ["r16x.dst  : PLUS_HI(base16.base | index16.index)", [2, 1], "leal", 'l', "SImode", 0, 0],
  ["r8x.dst   : PLUS_QI(base8.base  | index8.index)",  [2, 1], "leal", 'l', "SImode", 0, 0],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    suOrder2($dst, $base, $index, kid, kids);
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($base->a.base));
    sparseset_set_bit(live, find($index->a.index));
  },
  remat {
    flags = 0;
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dumpMbidR("$opcode",
      $base->a.base, true, $index->a.index, $index->a.scale, 0, NULL,
      $dst->rx, '$schar');
  },
  emit {
    rtx src = $order_key	/* 64-bit RTL shape weirdness */
      ? gen_rtx_PLUS($rtx_mode, $base->rtl, $index->rtl)
      : gen_rtx_PLUS($rtx_mode, $index->rtl, $base->rtl)
    ;
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    icg_emit_plain_tagged(gen_rtx_SET(VOIDmode, dst, src), __FILE__, __LINE__);
    /* the above line must be "plain", not "clobber" */
  };
""")

plug.plugrule3("leabic", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "scale_factor"],

  ["r64x.dst  : PLUS_DI(index64.index | imm32.const)", [2, 3], "leaq", 'q', "DImode", 0],
  ["rz32x.dst : PLUS_SI(index32.index | imm32.const)", [2, 2], "leal", 'l', "SImode", 0],
  ["r16x.dst  : PLUS_HI(index16.index | imm32.const)", [2, 2], "leal", 'l', "SImode", 0],
  ["r8x.dst   : PLUS_QI(index8.index  | imm32.const)", [2, 2], "leal", 'l', "SImode", 0],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    if ($index->freed > 0) {
      $dst->extra = $index->extra;
      $dst->freed = $index->freed - 1;
    }
    else {
      $dst->extra = $index->extra + 1;
      $dst->freed = 0;
    }
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($index->a.index));
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dumpMbidR("$opcode",
      0, false, $index->a.index, $index->a.scale, $const->val, NULL,
      $dst->rx, '$schar');
  },
  emit {
    rtx src = gen_rtx_PLUS($rtx_mode, $index->rtl, $const->rtl);
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    icg_emit_plain_tagged(gen_rtx_SET(VOIDmode, dst, src), __FILE__, __LINE__);
    /* the above line must be "plain", not "clobber" */
  };
""")

plug.plugrule3("leai", [
  ["rule", "cost", "opcode", "schar", "scale_factor", "rtx_mode"],

  ["r64x.dst  : index64.index", [2, 7], "leaq", 'q', 0, "DImode"],
  ["rz32x.dst : index32.index", [2, 6], "leal", 'l', 0, "SImode"],
  ["r16x.dst  : index16.index", [2, 6], "leal", 'l', 0, "SImode"],
  ["r8x.dst   : index8.index",  [2, 6], "leal", 'l', 0, "SImode"],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  supairs {
    if ($index->freed > 0) {
      $dst->extra = $index->extra;
      $dst->freed = $index->freed - 1;
    }
    else {
      $dst->extra = $index->extra + 1;
      $dst->freed = 0;
    }
  },
  build {
    unsigned rd = find($dst->rx);
    add_edges(rd, live);
    sparseset_clear_bit(live, rd);
    sparseset_set_bit(live, find($index->a.index));
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dumpMbidR("$opcode",
      0, false, $index->a.index, $index->a.scale, 0, NULL,
      $dst->rx, '$schar');
  },
  emit {
    rtx src = $index->rtl;
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    icg_emit_plain_tagged(gen_rtx_SET(VOIDmode, dst, src), __FILE__, __LINE__);
    /* the above line must be "plain", not "clobber" */
  };
""")


#----------------------------------------------------------------
# Sign and zero extension instructions.
# These do not affect the condition codes.
#

plug.plugrule3("loadcvt", [
  ["rule", "cost", "opcode", "dst_schar", "src_schar", "dst_mode", "src_mode", "rtx_builder", "move_flag"],

  ["r64x.dst  : SIGN_EXTEND_DI(r32.src)", [1, 3], "movslq", 'q', 'l', "DImode", "SImode", "gen_rtx_SIGN_EXTEND", 0],
  ["r64x.dst  : SIGN_EXTEND_DI(r16.src)", [1, 4], "movswq", 'q', 'w', "DImode", "HImode", "gen_rtx_SIGN_EXTEND", 0],
  ["r64x.dst  : SIGN_EXTEND_DI(r8.src)",  [1, 4], "movsbq", 'q', 'b', "DImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],
  ["rz32x.dst : SIGN_EXTEND_SI(r16.src)", [1, 3], "movswl", 'l', 'w', "SImode", "HImode", "gen_rtx_SIGN_EXTEND", 0],
  ["rz32x.dst : SIGN_EXTEND_SI(r8.src)",  [1, 3], "movsbl", 'l', 'b', "SImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],
  ["r16x.dst  : SIGN_EXTEND_HI(r8.src)",  [1, 4], "movsbw", 'w', 'b', "HImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],

  # we can save a byte for the 16-bit case
  ["rs16x.dst : SIGN_EXTEND_HI(r8.src)",  [1, 3], "movsbl", 'l', 'b', "SImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],

  # Note the special handling of the rule below.
  # There is no movzlq instruction, but the 32-bit instruction automatically zero extends to 64 bits.
  ["r64x.dst  : ZERO_EXTEND_DI(r32.src)", [1, 2], "movl",   'l', 'l', "SImode", "SImode", "gen_rtx_ZERO_EXTEND", 1],
  ["r64x.dst  : ZERO_EXTEND_DI(r16.src)", [1, 4], "movzwq", 'q', 'w', "DImode", "HImode", "gen_rtx_ZERO_EXTEND", 0],
  ["r64x.dst  : ZERO_EXTEND_DI(r8.src)",  [1, 4], "movzbq", 'q', 'b', "DImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],
  ["rz32x.dst : ZERO_EXTEND_SI(r16.src)", [1, 3], "movzwl", 'l', 'w', "SImode", "HImode", "gen_rtx_ZERO_EXTEND", 0],
  ["rz32x.dst : ZERO_EXTEND_SI(r8.src)",  [1, 3], "movzbl", 'l', 'b', "SImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],
  ["r16x.dst  : ZERO_EXTEND_HI(r8.src)",  [1, 4], "movzbw", 'w', 'b', "HImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],

  # we can save a byte for the 16-bit case
  ["rz16x.dst : ZERO_EXTEND_HI(r8.src)",  [1, 3], "movzbl", 'l', 'b', "SImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],

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
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  build {
    add_edges(find($dst->rx), live);
    sparseset_clear_bit(live, find($dst->rx));
    sparseset_set_bit(live, find($src->r));
  },
  costs {
    memorable($src->r);
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    rtx dst = gen_rtx_REG($dst_mode, $dst->rx);
    rtx src;
    if ($move_flag)
      src = gen_rtx_REG($src_mode, $src->r);
    else
      src = $rtx_builder($dst_mode, gen_rtx_REG($src_mode, $src->r));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  },
  debug {
    dumpRR("$opcode", $src->r, '$src_schar', $dst->rx, '$dst_schar');
  };

""")


# The thought behind these next few rules was that we could do an extension
# while making a copy, if it was helpful.  Unfortunately, it
# violates conventions for handling r and rx in the same node.
#
# rz32x : r32 [1, 2]
# rz16x : r16 [1, 3]
# rz8x  : r8  [1, 3]
# rs16x : r16 [1, 3]
# rs8x  : r8  [1, 3]



plug.plugrule3("movcvt", [
  ["rule", "cost", "opcode", "schar", "dst_mode", "src_mode", "rtx_builder", "move_flag"],

  ["r64x.dst  : SIGN_EXTEND_DI(MEM_SI(addr))", [4, 2], "movslq", 'q', "DImode", "SImode", "gen_rtx_SIGN_EXTEND", 0],
  ["r64x.dst  : SIGN_EXTEND_DI(MEM_HI(addr))", [4, 3], "movswq", 'q', "DImode", "HImode", "gen_rtx_SIGN_EXTEND", 0],
  ["r64x.dst  : SIGN_EXTEND_DI(MEM_QI(addr))", [4, 3], "movsbq", 'q', "DImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],
  ["rz32x.dst : SIGN_EXTEND_SI(MEM_HI(addr))", [4, 2], "movswl", 'l', "SImode", "HImode", "gen_rtx_SIGN_EXTEND", 0],
  ["rz32x.dst : SIGN_EXTEND_SI(MEM_QI(addr))", [4, 2], "movsbl", 'l', "SImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],
  ["r16x.dst  : SIGN_EXTEND_HI(MEM_QI(addr))", [4, 3], "movsbw", 'w', "HImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],

  # we can save a byte for the 16-bit case
  ["rs16x.dst : SIGN_EXTEND_HI(MEM_QI(addr))", [4, 2], "movsbl", 'l', "SImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],

  # Note the special handling of the rule below.
  # There is no movzlq instruction, but the 32-bit instruction automatically zero extends to 64 bits.
  ["r64x.dst  : ZERO_EXTEND_DI(MEM_SI(addr))", [4, 1], "movl",   'l', "SImode", "SImode", "gen_rtx_ZERO_EXTEND", 1],
  ["r64x.dst  : ZERO_EXTEND_DI(MEM_HI(addr))", [4, 3], "movzwq", 'q', "DImode", "HImode", "gen_rtx_ZERO_EXTEND", 0],
  ["r64x.dst  : ZERO_EXTEND_DI(MEM_QI(addr))", [4, 3], "movzbq", 'q', "DImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],
  ["rz32x.dst : ZERO_EXTEND_SI(MEM_HI(addr))", [4, 2], "movzwl", 'l', "SImode", "HImode", "gen_rtx_ZERO_EXTEND", 0],
  ["rz32x.dst : ZERO_EXTEND_SI(MEM_QI(addr))", [4, 2], "movzbl", 'l', "SImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],
  ["r16x.dst  : ZERO_EXTEND_HI(MEM_QI(addr))", [4, 3], "movzbw", 'w', "HImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],

  # we can save a byte for the 16-bit case
  ["rz16x.dst : ZERO_EXTEND_HI(MEM_QI(addr))", [4, 2], "movzbl", 'l', "SImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],


  # the rules below are implemented using the sign-extension instructions
  ["rs16x.dst : MEM_HI(addr)",                     [4, 2], "movswl", 'l', "SImode", "HImode", "gen_rtx_SIGN_EXTEND", 0],
  ["rs8x.dst  : MEM_QI(addr)",                     [4, 2], "movsbl", 'l', "SImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],
  ["rs16x.dst : SUBREG_HI(MEM_DI(addr), CONST_0)", [4, 2], "movswl", 'l', "SImode", "HImode", "gen_rtx_SIGN_EXTEND", 0],
  ["rs16x.dst : SUBREG_HI(MEM_SI(addr), CONST_0)", [4, 2], "movswl", 'l', "SImode", "HImode", "gen_rtx_SIGN_EXTEND", 0],
  ["rs8x.dst  : SUBREG_QI(MEM_DI(addr), CONST_0)", [4, 2], "movsbl", 'l', "SImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],
  ["rs8x.dst  : SUBREG_QI(MEM_SI(addr), CONST_0)", [4, 2], "movsbl", 'l', "SImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],
  ["rs8x.dst  : SUBREG_QI(MEM_HI(addr), CONST_0)", [4, 2], "movsbl", 'l', "SImode", "QImode", "gen_rtx_SIGN_EXTEND", 0],

  # the rules below are implemented using the zero-extension instructions
  ["rz16x.dst : MEM_HI(addr)",                     [4, 2], "movzwl", 'l', "SImode", "HImode", "gen_rtx_ZERO_EXTEND", 0],
  ["rz8x.dst  : MEM_QI(addr)",                     [4, 2], "movzbl", 'l', "SImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],
  ["rz16x.dst : SUBREG_HI(MEM_DI(addr), CONST_0)", [4, 2], "movzwl", 'l', "SImode", "HImode", "gen_rtx_ZERO_EXTEND", 0],
  ["rz16x.dst : SUBREG_HI(MEM_SI(addr), CONST_0)", [4, 2], "movzwl", 'l', "SImode", "HImode", "gen_rtx_ZERO_EXTEND", 0],
  ["rz8x.dst  : SUBREG_QI(MEM_DI(addr), CONST_0)", [4, 2], "movzbl", 'l', "SImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],
  ["rz8x.dst  : SUBREG_QI(MEM_SI(addr), CONST_0)", [4, 2], "movzbl", 'l', "SImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],
  ["rz8x.dst  : SUBREG_QI(MEM_HI(addr), CONST_0)", [4, 2], "movzbl", 'l', "SImode", "QImode", "gen_rtx_ZERO_EXTEND", 0],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
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
    dumpMR("$opcode", $addr, $dst->rx, '$schar');
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    rtx dst = gen_rtx_REG($dst_mode, $dst->rx);
    rtx src;
    if ($move_flag)
      src = gen_rtx_MEM($src_mode, $addr->rtl);
    else
      src = $rtx_builder($dst_mode, gen_rtx_MEM($src_mode, $addr->rtl));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


#-----------------------------------------------------------
# loads, and load-like instructions
#
# note that the 8 and 16-bit loads are handled above,
# since we may as well sign or zero-extend while loading
#

plug.plugrule3("loadbasic", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_code"],

  ["r64x.dst  :                  MEM_DI(addr)",            [3, 2], "movq", 'q', "DImode", 0],
  ["rz32x.dst :                  MEM_SI(addr)",            [3, 1], "movl", 'l', "SImode", 0],
  ["rz32x.dst :        SUBREG_SI(MEM_DI(addr), CONST_0)",  [3, 1], "movl", 'l', "SImode", 0],

  ["r64x.dst  : BSF_DI(          MEM_DI(addr))",           [3, 2], "bsfq", 'q', "DImode", 1],
  ["rz32x.dst : BSF_SI(          MEM_SI(addr))",           [3, 1], "bsfl", 'l', "SImode", 1],
  ["rz32x.dst : BSF_SI(SUBREG_SI(MEM_DI(addr), CONST_0))", [3, 1], "bsfl", 'l', "SImode", 1],

  ["r64x.dst  : BSR_DI(          MEM_DI(addr))",           [3, 2], "bsrq", 'q', "DImode", 1],
  ["rz32x.dst : BSR_SI(          MEM_SI(addr))",           [3, 1], "bsrl", 'l', "SImode", 1],
  ["rz32x.dst : BSR_SI(SUBREG_SI(MEM_DI(addr), CONST_0))", [3, 1], "bsrl", 'l', "SImode", 1],

  #
  # TODO: add CLZ and POPCOUNT from memory
  #

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
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
    dumpMR("$opcode", $addr, $dst->rx, '$schar');
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    /* TODO: handle BSF and BSR in the rtx IL, perhaps as asm */
    const rtx src = gen_rtx_MEM($rtx_mode, $addr->rtl);
    const rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


#
# TODO: this block of rules will need to be broken up into other smaller blocks
# in order to correctly do the semantics of immediate vs register vs memory operands
# and also to handle different arities of the children.
#
plug.plugrule3("memop_unary", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder", "is_nocc"],

  [" rcc.dst : MEM_NEG_DI(MEM_DI(addr))", [4, 2], "negq", 'q', "DImode", "gen_rtx_NEG", 0],
  [" rcc.dst : MEM_NEG_SI(MEM_SI(addr))", [4, 1], "negl", 'l', "SImode", "gen_rtx_NEG", 0],
  [" rcc.dst : MEM_NEG_HI(MEM_HI(addr))", [4, 2], "negw", 'w', "HImode", "gen_rtx_NEG", 0],
  [" rcc.dst : MEM_NEG_QI(MEM_QI(addr))", [4, 1], "negb", 'b', "QImode", "gen_rtx_NEG", 0],

  ["nocc.dst : MEM_NOT_DI(MEM_DI(addr))", [4, 2], "notq", 'q', "DImode", "gen_rtx_NOT", 1],
  ["nocc.dst : MEM_NOT_SI(MEM_SI(addr))", [4, 1], "notl", 'l', "SImode", "gen_rtx_NOT", 1],
  ["nocc.dst : MEM_NOT_HI(MEM_HI(addr))", [4, 2], "notw", 'w', "HImode", "gen_rtx_NOT", 1],
  ["nocc.dst : MEM_NOT_QI(MEM_QI(addr))", [4, 1], "notb", 'b', "QImode", "gen_rtx_NOT", 1],

], """
  $rule $cost
  build {
    add_addr(live, $addr);
  },
  remat {
    flags = 0;
  },
  debug {
    dumpM("$opcode", $addr);
  },
  emit {
    const rtx mem = gen_rtx_MEM($rtx_mode, $addr->rtl);
    ($is_nocc ? icg_emit_plain : icg_emit_clobber)(gen_rtx_SET(VOIDmode,
      mem,	/* NOTE: dag */
      $rtx_builder($rtx_mode,
	mem	/* NOTE: dag */
      )
    ));
  };
""")

plug.plugrule3("mem_mathop_reg", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder", "is_nocc"],

  ["rcc.dst : MEM_PLUS_DI(MEM_DI(addr),  NEG_DI(r64.src))", [4, 2], "subq", 'q', "DImode", "gen_rtx_MINUS", 0],
  ["rcc.dst : MEM_PLUS_SI(MEM_SI(addr),  NEG_DI(r32.src))", [4, 1], "subl", 'l', "SImode", "gen_rtx_MINUS", 0],
  ["rcc.dst : MEM_PLUS_HI(MEM_HI(addr),  NEG_DI(r16.src))", [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS", 0],
  ["rcc.dst : MEM_PLUS_QI(MEM_QI(addr),  NEG_DI(r8.src))",  [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS", 0],

  ["rcc.dst : MEM_MINUS_DI(MEM_DI(addr), NEG_DI(r64.src))", [4, 2], "addq", 'q', "DImode", "gen_rtx_PLUS", 0],
  ["rcc.dst : MEM_MINUS_SI(MEM_SI(addr), NEG_DI(r32.src))", [4, 1], "addl", 'l', "SImode", "gen_rtx_PLUS", 0],
  ["rcc.dst : MEM_MINUS_HI(MEM_HI(addr), NEG_DI(r16.src))", [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS", 0],
  ["rcc.dst : MEM_MINUS_QI(MEM_QI(addr), NEG_DI(r8.src))",  [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS", 0],

  ["rcc.dst : MEM_IOR_DI     (MEM_DI(addr), r64.src)",  [4, 2], "orq",  'q', "DImode", "gen_rtx_IOR", 0],
  ["rcc.dst : MEM_IOR_SI     (MEM_SI(addr), r32.src)",  [4, 1], "orl",  'l', "SImode", "gen_rtx_IOR", 0],
  ["rcc.dst : MEM_IOR_HI     (MEM_HI(addr), r16.src)",  [4, 2], "orw",  'w', "HImode", "gen_rtx_IOR", 0],
  ["rcc.dst : MEM_IOR_QI     (MEM_QI(addr), r8.src)",   [4, 1], "orb",  'b', "QImode", "gen_rtx_IOR", 0],

  ["rcc.dst : MEM_XOR_DI     (MEM_DI(addr), r64.src)",  [4, 2], "xorq", 'q', "DImode", "gen_rtx_XOR", 0],
  ["rcc.dst : MEM_XOR_SI     (MEM_SI(addr), r32.src)",  [4, 1], "xorl", 'l', "SImode", "gen_rtx_XOR", 0],
  ["rcc.dst : MEM_XOR_HI     (MEM_HI(addr), r16.src)",  [4, 2], "xorw", 'w', "HImode", "gen_rtx_XOR", 0],
  ["rcc.dst : MEM_XOR_QI     (MEM_QI(addr), r8.src)",   [4, 1], "xorb", 'b', "QImode", "gen_rtx_XOR", 0],

  ["rcc.dst : MEM_PLUS_DI    (MEM_DI(addr), r64.src)",  [4, 2], "addq", 'q', "DImode", "gen_rtx_PLUS", 0],
  ["rcc.dst : MEM_PLUS_SI    (MEM_SI(addr), r32.src)",  [4, 1], "addl", 'l', "SImode", "gen_rtx_PLUS", 0],
  ["rcc.dst : MEM_PLUS_HI    (MEM_HI(addr), r16.src)",  [4, 2], "addw", 'w', "HImode", "gen_rtx_PLUS", 0],
  ["rcc.dst : MEM_PLUS_QI    (MEM_QI(addr), r8.src)",   [4, 1], "addb", 'b', "QImode", "gen_rtx_PLUS", 0],

  ["rcc.dst : MEM_MINUS_DI   (MEM_DI(addr), r64.src)",  [4, 2], "subq", 'q', "DImode", "gen_rtx_MINUS", 0],
  ["rcc.dst : MEM_MINUS_SI   (MEM_SI(addr), r32.src)",  [4, 1], "subl", 'l', "SImode", "gen_rtx_MINUS", 0],
  ["rcc.dst : MEM_MINUS_HI   (MEM_HI(addr), r16.src)",  [4, 2], "subw", 'w', "HImode", "gen_rtx_MINUS", 0],
  ["rcc.dst : MEM_MINUS_QI   (MEM_QI(addr), r8.src)",   [4, 1], "subb", 'b', "QImode", "gen_rtx_MINUS", 0],

  ["rcc.dst : MEM_AND_DI     (MEM_DI(addr), r64.src)",  [4, 2], "andq", 'q', "DImode", "gen_rtx_AND", 0],
  ["rcc.dst : MEM_AND_SI     (MEM_SI(addr), r32.src)",  [4, 1], "andl", 'l', "SImode", "gen_rtx_AND", 0],
  ["rcc.dst : MEM_AND_HI     (MEM_HI(addr), r16.src)",  [4, 2], "andw", 'w', "HImode", "gen_rtx_AND", 0],
  ["rcc.dst : MEM_AND_QI     (MEM_QI(addr), r8.src)",   [4, 1], "andb", 'b', "QImode", "gen_rtx_AND", 0],

], """
  $rule $cost
  supairs {
    /*TODO*/
  },
  costs {
    /*
     * the src register can not be directly replaced
     * with a spill memory reference, so this is "forgettable".
     */
    forgettable($src->r);
  },
  build {
    sparseset_set_bit(live, find($src->r));
    add_addr(live, $addr);
  },
  remat {
    flags = 0;
  },
  debug {
    dumpRM("$opcode", $src->r, '$schar', $addr);
  },
  emit {
    const rtx mem = gen_rtx_MEM($rtx_mode, $addr->rtl);
    ($is_nocc ? icg_emit_plain : icg_emit_clobber)(gen_rtx_SET(VOIDmode,
      mem,	/* NOTE: dag */
      $rtx_builder($rtx_mode,
	mem,	/* NOTE: dag */
	gen_rtx_REG($rtx_mode, $src->r)
      )
    ));
  };
""")

plug.plugrule3("mem_shiftop_reg", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder", "is_nocc"],

  #
  # If there must be a copy, then we want to be sure to copy with
  # a movl instruction where we can; 8- and 16-bit moves are expensive.
  # The 16-bit move is bigger; both are slower.
  #
  # By saying "r8x", we think it's more likely that the initial copy into RCX
  # will be coalesced.  Given this, we'll assume the coalesce will succeed
  # and set the costs accordingly.
  #

  #
  # The shifts and rotates are shown here as clobbering the condition codes.
  # even though the condition codes will not get clobbered if the shift count is 0.
  # We need to produce RTX that encodes condition codes being clobbered
  # so that the downstream gcc+RTL consumer will be happy.
  #
  ["rcc.dst : MEM_ASHIFT_DI  (MEM_DI(addr), r8x.src)", [4, 2], "salq", 'q', "DImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_SI  (MEM_SI(addr), r8x.src)", [4, 1], "sall", 'l', "SImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_HI  (MEM_HI(addr), r8x.src)", [4, 2], "salw", 'w', "HImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_QI  (MEM_QI(addr), r8x.src)", [4, 1], "salb", 'b', "QImode", "gen_rtx_ASHIFT", 0],

  ["rcc.dst : MEM_ROTATE_DI  (MEM_DI(addr), r8x.src)", [4, 2], "rolq", 'q', "DImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_SI  (MEM_SI(addr), r8x.src)", [4, 1], "roll", 'l', "SImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_HI  (MEM_HI(addr), r8x.src)", [4, 2], "rolw", 'w', "HImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_QI  (MEM_QI(addr), r8x.src)", [4, 1], "rolb", 'b', "QImode", "gen_rtx_ROTATE", 0],

  ["rcc.dst : MEM_ASHIFTRT_DI(MEM_DI(addr), r8x.src)", [4, 2], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_SI(MEM_SI(addr), r8x.src)", [4, 1], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_HI(MEM_HI(addr), r8x.src)", [4, 2], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_QI(MEM_QI(addr), r8x.src)", [4, 1], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT", 0],

  ["rcc.dst : MEM_LSHIFTRT_DI(MEM_DI(addr), r8x.src)", [4, 2], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_SI(MEM_SI(addr), r8x.src)", [4, 1], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_HI(MEM_HI(addr), r8x.src)", [4, 2], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_QI(MEM_QI(addr), r8x.src)", [4, 1], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT", 0],

  ["rcc.dst : MEM_ROTATERT_DI(MEM_DI(addr), r8x.src)", [4, 2], "rorq", 'q', "DImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_SI(MEM_SI(addr), r8x.src)", [4, 1], "rorl", 'l', "SImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_HI(MEM_HI(addr), r8x.src)", [4, 2], "rorw", 'w', "HImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_QI(MEM_QI(addr), r8x.src)", [4, 1], "rorb", 'b', "QImode", "gen_rtx_ROTATERT", 0],


  ["rcc.dst : MEM_ASHIFT_DI  (MEM_DI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 2], "salq", 'q', "DImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_SI  (MEM_SI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 1], "sall", 'l', "SImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_HI  (MEM_HI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 2], "salw", 'w', "HImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_QI  (MEM_QI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 1], "salb", 'b', "QImode", "gen_rtx_ASHIFT", 0],

  ["rcc.dst : MEM_ROTATE_DI  (MEM_DI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 2], "rolq", 'q', "DImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_SI  (MEM_SI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 1], "roll", 'l', "SImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_HI  (MEM_HI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 2], "rolw", 'w', "HImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_QI  (MEM_QI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 1], "rolb", 'b', "QImode", "gen_rtx_ROTATE", 0],

  ["rcc.dst : MEM_ASHIFTRT_DI(MEM_DI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 2], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_SI(MEM_SI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 1], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_HI(MEM_HI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 2], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_QI(MEM_QI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 1], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT", 0],

  ["rcc.dst : MEM_LSHIFTRT_DI(MEM_DI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 2], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_SI(MEM_SI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 1], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_HI(MEM_HI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 2], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_QI(MEM_QI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 1], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT", 0],

  ["rcc.dst : MEM_ROTATERT_DI(MEM_DI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 2], "rorq", 'q', "DImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_SI(MEM_SI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 1], "rorl", 'l', "SImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_HI(MEM_HI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 2], "rorw", 'w', "HImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_QI(MEM_QI(addr), SUBREG_QI(r64x.src, CONST_0))", [4, 1], "rorb", 'b', "QImode", "gen_rtx_ROTATERT", 0],


  ["rcc.dst : MEM_ASHIFT_DI  (MEM_DI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 2], "salq", 'q', "DImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_SI  (MEM_SI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 1], "sall", 'l', "SImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_HI  (MEM_HI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 2], "salw", 'w', "HImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_QI  (MEM_QI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 1], "salb", 'b', "QImode", "gen_rtx_ASHIFT", 0],

  ["rcc.dst : MEM_ROTATE_DI  (MEM_DI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 2], "rolq", 'q', "DImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_SI  (MEM_SI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 1], "roll", 'l', "SImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_HI  (MEM_HI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 2], "rolw", 'w', "HImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_QI  (MEM_QI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 1], "rolb", 'b', "QImode", "gen_rtx_ROTATE", 0],

  ["rcc.dst : MEM_ASHIFTRT_DI(MEM_DI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 2], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_SI(MEM_SI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 1], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_HI(MEM_HI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 2], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_QI(MEM_QI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 1], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT", 0],

  ["rcc.dst : MEM_LSHIFTRT_DI(MEM_DI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 2], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_SI(MEM_SI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 1], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_HI(MEM_HI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 2], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_QI(MEM_QI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 1], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT", 0],

  ["rcc.dst : MEM_ROTATERT_DI(MEM_DI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 2], "rorq", 'q', "DImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_SI(MEM_SI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 1], "rorl", 'l', "SImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_HI(MEM_HI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 2], "rorw", 'w', "HImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_QI(MEM_QI(addr), SUBREG_QI(r32x.src, CONST_0))", [4, 1], "rorb", 'b', "QImode", "gen_rtx_ROTATERT", 0],


  ["rcc.dst : MEM_ASHIFT_DI  (MEM_DI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 2], "salq", 'q', "DImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_SI  (MEM_SI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 1], "sall", 'l', "SImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_HI  (MEM_HI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 2], "salw", 'w', "HImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_QI  (MEM_QI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 1], "salb", 'b', "QImode", "gen_rtx_ASHIFT", 0],

  ["rcc.dst : MEM_ROTATE_DI  (MEM_DI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 2], "rolq", 'q', "DImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_SI  (MEM_SI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 1], "roll", 'l', "SImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_HI  (MEM_HI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 2], "rolw", 'w', "HImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_QI  (MEM_QI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 1], "rolb", 'b', "QImode", "gen_rtx_ROTATE", 0],

  ["rcc.dst : MEM_ASHIFTRT_DI(MEM_DI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 2], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_SI(MEM_SI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 1], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_HI(MEM_HI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 2], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_QI(MEM_QI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 1], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT", 0],

  ["rcc.dst : MEM_LSHIFTRT_DI(MEM_DI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 2], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_SI(MEM_SI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 1], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_HI(MEM_HI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 2], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_QI(MEM_QI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 1], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT", 0],

  ["rcc.dst : MEM_ROTATERT_DI(MEM_DI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 2], "rorq", 'q', "DImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_SI(MEM_SI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 1], "rorl", 'l', "SImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_HI(MEM_HI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 2], "rorw", 'w', "HImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_QI(MEM_QI(addr), SUBREG_QI(r16x.src, CONST_0))", [4, 1], "rorb", 'b', "QImode", "gen_rtx_ROTATERT", 0],

], """
  $rule $cost
  supairs {
    /*TODO*/
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src->rx, REG_RCX);
  },
  build {
    add_addr(live, $addr);
    add_copy_edges(REG_RCX, $src->rx, live);
  },
  remat {
    flags = 0;
  },
  costs {
    /*
     * the src register can not be directly replaced
     * with a spill memory reference, so this is "forgettable".
     */
    forgettable($src->rx);	/* must be in a register */
    cost_copy(REG_RCX, $src->rx);
  },
  debug {
    dumpRM("$opcode", $src->rx, '$schar', $addr);
  },
  emit {
    const rtx mem = gen_rtx_MEM($rtx_mode, $addr->rtl);
    const rtx rcx = gen_rtx_REG(SImode, REG_RCX);
    const rtx src = gen_rtx_REG(SImode, $src->rx);
    icg_emit_plain(gen_rtx_SET(VOIDmode, rcx, src));
    ($is_nocc ? icg_emit_plain : icg_emit_clobber)(gen_rtx_SET(VOIDmode,
      mem,	/* NOTE: dag */
      $rtx_builder($rtx_mode,
	mem,	/* NOTE: dag */
	gen_rtx_REG(QImode, REG_RCX)
      )
    ));
  };
""")

plug.plugrule3("mem_arithop_imm", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder", "is_nocc"],

  ["rcc.dst  : MEM_PLUS_DI(MEM_DI(addr),     imm32.const)", [4, 3], "addq", 'q', "DImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_PLUS_SI(MEM_SI(addr),     imm32.const)", [4, 2], "addl", 'l', "SImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_PLUS_HI(MEM_HI(addr),     imm16.const)", [4, 3], "addw", 'w', "HImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_PLUS_QI(MEM_QI(addr),     imm8.const)",  [4, 2], "addb", 'b', "QImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_PLUS_DI(MEM_DI(addr),     imm8.const)",  [4, 3], "addq", 'q', "DImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_PLUS_SI(MEM_SI(addr),     imm8.const)",  [4, 2], "addl", 'l', "SImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_PLUS_HI(MEM_HI(addr),     imm8.const)",  [4, 3], "addw", 'w', "HImode", "gen_rtx_PLUS", 0],

  ["rcc.dst  : MEM_MINUS_DI(MEM_DI(addr),    imm32.const)", [4, 3], "subq", 'q', "DImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_MINUS_SI(MEM_SI(addr),    imm32.const)", [4, 2], "subl", 'l', "SImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_MINUS_HI(MEM_HI(addr),    imm16.const)", [4, 3], "subw", 'w', "HImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_MINUS_QI(MEM_QI(addr),    imm8.const)",  [4, 2], "subb", 'b', "QImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_MINUS_DI(MEM_DI(addr),    imm8.const)",  [4, 3], "subq", 'q', "DImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_MINUS_SI(MEM_SI(addr),    imm8.const)",  [4, 2], "subl", 'l', "SImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_MINUS_HI(MEM_HI(addr),    imm8.const)",  [4, 3], "subw", 'w', "HImode", "gen_rtx_MINUS", 0],

  ["rcc.dst  : MEM_AND_DI(MEM_DI(addr),      imm32.const)", [4, 3], "andq", 'q', "DImode", "gen_rtx_AND", 0],
  ["rcc.dst  : MEM_AND_SI(MEM_SI(addr),      imm32.const)", [4, 2], "andl", 'l', "SImode", "gen_rtx_AND", 0],
  ["rcc.dst  : MEM_AND_HI(MEM_HI(addr),      imm16.const)", [4, 3], "andw", 'w', "HImode", "gen_rtx_AND", 0],
  ["rcc.dst  : MEM_AND_QI(MEM_QI(addr),      imm8.const)",  [4, 2], "andb", 'b', "QImode", "gen_rtx_AND", 0],
  ["rcc.dst  : MEM_AND_DI(MEM_DI(addr),      imm8.const)",  [4, 3], "andq", 'q', "DImode", "gen_rtx_AND", 0],
  ["rcc.dst  : MEM_AND_SI(MEM_SI(addr),      imm8.const)",  [4, 2], "andl", 'l', "SImode", "gen_rtx_AND", 0],
  ["rcc.dst  : MEM_AND_HI(MEM_HI(addr),      imm8.const)",  [4, 3], "andw", 'w', "HImode", "gen_rtx_AND", 0],

  ["rcc.dst  : MEM_IOR_DI(MEM_DI(addr),      imm32.const)", [4, 3], "orq",  'q', "DImode", "gen_rtx_IOR", 0],
  ["rcc.dst  : MEM_IOR_SI(MEM_SI(addr),      imm32.const)", [4, 2], "orl",  'l', "SImode", "gen_rtx_IOR", 0],
  ["rcc.dst  : MEM_IOR_HI(MEM_HI(addr),      imm16.const)", [4, 3], "orw",  'w', "HImode", "gen_rtx_IOR", 0],
  ["rcc.dst  : MEM_IOR_QI(MEM_QI(addr),      imm8.const)",  [4, 2], "orb",  'b', "QImode", "gen_rtx_IOR", 0],
  ["rcc.dst  : MEM_IOR_DI(MEM_DI(addr),      imm8.const)",  [4, 3], "orq",  'q', "DImode", "gen_rtx_IOR", 0],
  ["rcc.dst  : MEM_IOR_SI(MEM_SI(addr),      imm8.const)",  [4, 2], "orl",  'l', "SImode", "gen_rtx_IOR", 0],
  ["rcc.dst  : MEM_IOR_HI(MEM_HI(addr),      imm8.const)",  [4, 3], "orw",  'w', "HImode", "gen_rtx_IOR", 0],

  ["rcc.dst  : MEM_XOR_DI(MEM_DI(addr),      imm32.const)", [4, 3], "xorq", 'q', "DImode", "gen_rtx_XOR", 0],
  ["rcc.dst  : MEM_XOR_SI(MEM_SI(addr),      imm32.const)", [4, 2], "xorl", 'l', "SImode", "gen_rtx_XOR", 0],
  ["rcc.dst  : MEM_XOR_HI(MEM_HI(addr),      imm16.const)", [4, 3], "xorw", 'w', "HImode", "gen_rtx_XOR", 0],
  ["rcc.dst  : MEM_XOR_QI(MEM_QI(addr),      imm8.const)",  [4, 2], "xorb", 'b', "QImode", "gen_rtx_XOR", 0],
  ["rcc.dst  : MEM_XOR_DI(MEM_DI(addr),      imm8.const)",  [4, 3], "xorq", 'q', "DImode", "gen_rtx_XOR", 0],
  ["rcc.dst  : MEM_XOR_SI(MEM_SI(addr),      imm8.const)",  [4, 2], "xorl", 'l', "SImode", "gen_rtx_XOR", 0],
  ["rcc.dst  : MEM_XOR_HI(MEM_HI(addr),      imm8.const)",  [4, 3], "xorw", 'w', "HImode", "gen_rtx_XOR", 0],

  #
  # shifts and rotates of memory are marked as clobbering the condition codes
  # contrast this with shifts and rotates of registers,
  # which are marked as NOT clobbering the condition codes
  # we must generate RTL that encodes clobbering the condition codes,
  # otherwise the gcc+rtl back end will choke.
  #
  ["rcc.dst : MEM_ASHIFT_DI(MEM_DI(addr),   imm8.const)",  [4, 3], "salq", 'q', "DImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_SI(MEM_SI(addr),   imm8.const)",  [4, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_HI(MEM_HI(addr),   imm8.const)",  [4, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_QI(MEM_QI(addr),   imm8.const)",  [4, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT", 0],

  ["rcc.dst : MEM_ROTATE_DI(MEM_DI(addr),   imm8.const)",  [4, 3], "rolq", 'q', "DImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_SI(MEM_SI(addr),   imm8.const)",  [4, 2], "roll", 'l', "SImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_HI(MEM_HI(addr),   imm8.const)",  [4, 3], "rolw", 'w', "HImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_QI(MEM_QI(addr),   imm8.const)",  [4, 2], "rolb", 'b', "QImode", "gen_rtx_ROTATE", 0],

  ["rcc.dst : MEM_LSHIFTRT_DI(MEM_DI(addr), imm8.const)",  [4, 3], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_SI(MEM_SI(addr), imm8.const)",  [4, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_HI(MEM_HI(addr), imm8.const)",  [4, 3], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_QI(MEM_QI(addr), imm8.const)",  [4, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT", 0],

  ["rcc.dst : MEM_ASHIFTRT_DI(MEM_DI(addr), imm8.const)",  [4, 3], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_SI(MEM_SI(addr), imm8.const)",  [4, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_HI(MEM_HI(addr), imm8.const)",  [4, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_QI(MEM_QI(addr), imm8.const)",  [4, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT", 0],

  ["rcc.dst : MEM_ROTATERT_DI(MEM_DI(addr), imm8.const)",  [4, 3], "rorq", 'q', "DImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_SI(MEM_SI(addr), imm8.const)",  [4, 2], "rorl", 'l', "SImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_HI(MEM_HI(addr), imm8.const)",  [4, 3], "rorw", 'w', "HImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_QI(MEM_QI(addr), imm8.const)",  [4, 2], "rorb", 'b', "QImode", "gen_rtx_ROTATERT", 0],

], """
  $rule $cost
  supairs {
    /*TODO*/
  },
  build {
    add_addr(live, $addr);
  },
  remat {
    flags = 0;
  },
  debug {
    dumpIM("$opcode", $const, $addr);
  },
  emit {
    const rtx mem = gen_rtx_MEM($rtx_mode, $addr->rtl);
    ($is_nocc ? icg_emit_plain : icg_emit_clobber)(gen_rtx_SET(VOIDmode,
      mem,	/* NOTE: dag */
      $rtx_builder($rtx_mode,
	mem,	/* NOTE: dag */
	gen_rtx_imm_constant($const->val, $const->a.string, $const->rtl)
      )
    ));
  };
""")

#
# increment/decrement memory
# the condition codes are marked as clobbered, and we generate RTX that reflects that.
# Contrast that with shift/rotate by 1.
#
plug.plugrule3("mem_arithop_imm_1", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder", "is_nocc"],

  ["rcc.dst  : MEM_PLUS_DI(MEM_DI(addr),     CONST_P1.const)", [4, 3], "incq", 'q', "DImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_PLUS_SI(MEM_SI(addr),     CONST_P1.const)", [4, 2], "incl", 'l', "SImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_PLUS_HI(MEM_HI(addr),     CONST_P1.const)", [4, 3], "incw", 'w', "HImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_PLUS_QI(MEM_QI(addr),     CONST_P1.const)", [4, 2], "incb", 'b', "QImode", "gen_rtx_PLUS", 0],

  ["rcc.dst  : MEM_PLUS_DI(MEM_DI(addr),     CONST_N1.const)", [4, 3], "decq", 'q', "DImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_PLUS_SI(MEM_SI(addr),     CONST_N1.const)", [4, 2], "decl", 'l', "SImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_PLUS_HI(MEM_HI(addr),     CONST_N1.const)", [4, 3], "decw", 'w', "HImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_PLUS_QI(MEM_QI(addr),     CONST_N1.const)", [4, 2], "decb", 'b', "QImode", "gen_rtx_MINUS", 0],

  ["rcc.dst  : MEM_MINUS_DI(MEM_DI(addr),    CONST_P1.const)", [4, 3], "decq", 'q', "DImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_MINUS_SI(MEM_SI(addr),    CONST_P1.const)", [4, 2], "decl", 'l', "SImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_MINUS_HI(MEM_HI(addr),    CONST_P1.const)", [4, 3], "decw", 'w', "HImode", "gen_rtx_MINUS", 0],
  ["rcc.dst  : MEM_MINUS_QI(MEM_QI(addr),    CONST_P1.const)", [4, 2], "decb", 'b', "QImode", "gen_rtx_MINUS", 0],

  ["rcc.dst  : MEM_MINUS_DI(MEM_DI(addr),    CONST_N1.const)", [4, 3], "incq", 'q', "DImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_MINUS_SI(MEM_SI(addr),    CONST_N1.const)", [4, 2], "incl", 'l', "SImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_MINUS_HI(MEM_HI(addr),    CONST_N1.const)", [4, 3], "incw", 'w', "HImode", "gen_rtx_PLUS", 0],
  ["rcc.dst  : MEM_MINUS_QI(MEM_QI(addr),    CONST_N1.const)", [4, 2], "incb", 'b', "QImode", "gen_rtx_PLUS", 0],

], """
  $rule $cost
  supairs {
    /*TODO*/
  },
  build {
    add_addr(live, $addr);
  },
  remat {
    flags = 0;
  },
  debug {
    dumpM("$opcode", $addr);
  },
  emit {
    const rtx mem = gen_rtx_MEM($rtx_mode, $addr->rtl);
    ($is_nocc ? icg_emit_plain : icg_emit_clobber)(gen_rtx_SET(VOIDmode,
      mem,	/* NOTE: dag */
      $rtx_builder($rtx_mode,
	mem,	/* NOTE: dag */
	GEN_INT(1)
      )
    ));
  };
""")

#
# Shift or rotate memory by one, which is a special case in the ISA.
# The condition codes are clobbered by the hardware,
# and we have to express that in the RTX we produce.
# Note that for shifts with a register destination the condition codes
# are also clobbered, BUT the rtx we produce doesn't encode clobbered condition codes.
#
plug.plugrule3("mem_shiftop_imm_1", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder", "is_nocc"],

  ["rcc.dst : MEM_ASHIFT_DI(MEM_DI(addr),   CONST_P1.const)", [4, 3], "salq", 'q', "DImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_SI(MEM_SI(addr),   CONST_P1.const)", [4, 2], "sall", 'l', "SImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_HI(MEM_HI(addr),   CONST_P1.const)", [4, 3], "salw", 'w', "HImode", "gen_rtx_ASHIFT", 0],
  ["rcc.dst : MEM_ASHIFT_QI(MEM_QI(addr),   CONST_P1.const)", [4, 2], "salb", 'b', "QImode", "gen_rtx_ASHIFT", 0],

  ["rcc.dst : MEM_ROTATE_DI(MEM_DI(addr),   CONST_P1.const)", [4, 3], "rolq", 'q', "DImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_SI(MEM_SI(addr),   CONST_P1.const)", [4, 2], "roll", 'l', "SImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_HI(MEM_HI(addr),   CONST_P1.const)", [4, 3], "rolw", 'w', "HImode", "gen_rtx_ROTATE", 0],
  ["rcc.dst : MEM_ROTATE_QI(MEM_QI(addr),   CONST_P1.const)", [4, 2], "rolb", 'b', "QImode", "gen_rtx_ROTATE", 0],

  ["rcc.dst : MEM_LSHIFTRT_DI(MEM_DI(addr), CONST_P1.const)", [4, 3], "shrq", 'q', "DImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_SI(MEM_SI(addr), CONST_P1.const)", [4, 2], "shrl", 'l', "SImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_HI(MEM_HI(addr), CONST_P1.const)", [4, 3], "shrw", 'w', "HImode", "gen_rtx_LSHIFTRT", 0],
  ["rcc.dst : MEM_LSHIFTRT_QI(MEM_QI(addr), CONST_P1.const)", [4, 2], "shrb", 'b', "QImode", "gen_rtx_LSHIFTRT", 0],

  ["rcc.dst : MEM_ASHIFTRT_DI(MEM_DI(addr), CONST_P1.const)", [4, 3], "sarq", 'q', "DImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_SI(MEM_SI(addr), CONST_P1.const)", [4, 2], "sarl", 'l', "SImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_HI(MEM_HI(addr), CONST_P1.const)", [4, 3], "sarw", 'w', "HImode", "gen_rtx_ASHIFTRT", 0],
  ["rcc.dst : MEM_ASHIFTRT_QI(MEM_QI(addr), CONST_P1.const)", [4, 2], "sarb", 'b', "QImode", "gen_rtx_ASHIFTRT", 0],

  ["rcc.dst : MEM_ROTATERT_DI(MEM_DI(addr), CONST_P1.const)", [4, 3], "sarq", 'q', "DImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_SI(MEM_SI(addr), CONST_P1.const)", [4, 2], "sarl", 'l', "SImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_HI(MEM_HI(addr), CONST_P1.const)", [4, 3], "sarw", 'w', "HImode", "gen_rtx_ROTATERT", 0],
  ["rcc.dst : MEM_ROTATERT_QI(MEM_QI(addr), CONST_P1.const)", [4, 2], "sarb", 'b', "QImode", "gen_rtx_ROTATERT", 0],

], """
  $rule $cost
  supairs {
    /*TODO*/
  },
  build {
    add_addr(live, $addr);
  },
  remat {
    flags = 0;
  },
  debug {
    dumpM("$opcode", $addr);
  },
  emit {
    const rtx mem = gen_rtx_MEM($rtx_mode, $addr->rtl);
    ($is_nocc ? icg_emit_plain : icg_emit_clobber)(gen_rtx_SET(VOIDmode,
      mem,	/* NOTE: dag */
      $rtx_builder($rtx_mode,
	mem,	/* NOTE: dag */
	GEN_INT(1)
      )
    ));
  };
""")

#
# Chain rule to discard top level occurances of MEM_OP
# productions that do not modify the condition code
#
plug.plugrule3("nocc", [
  ["rule", "cost"],
  ["stmt : nocc", [100, 100]],	# discard top level MEM_OP
  ["stmt : rcc",  [100, 100]],	# discard top level MEM_OP that have set the cc
], """
  $rule $cost
  ;
""")

#
# This rule is used to implement 64-bit division by small constants
# by multiplying by reciprocals.
# The semantics here are more or less identical to the case for "divq".
#
# From the manual page for div (unsigned divide):
# Divides the unsigned value in RDX:RAX by the unsigned value in the specified
# register or memory location, leaves the quotient in RAX and the remainder in RDX.
#
# From the manual page for mul (unsigned multiply):
#   Multiplies the quadword value in the specified register or memory location
#   by the value in RAX, and stores the result in RDX:RAX.
#
# From the manual page for the most constrained use of imul (signed multiply):
#   Multiplies the contents of RAX by the contents of a 64-bit memory
#   or register operand, and put the signed result in RDX:RAX
#
# For our purposes, we'll pick up the value in RDX.
#
plug.plugrule3("mulTI", [
  ["rule", "cost", "opcode", "schar", "dst_reg_name", "rtx_mode", "mov_opcode", "extend_opcode", "isRR", "issigned"],
  #
  # That's right, the destination is reg_rdx: we pluck out the upper 64 bits directly from rdx
  # TODO: cost is made up (carried forward from division)
  #
  ["r64x.dst  : TRUNCATE_DI(LSHIFTRT_TI(MULT_TI(ZERO_EXTEND_TI(r64x.src1) | ZERO_EXTEND_TI(r64.src2)), CONST8P))",
    [4, 4], "mulq", 'q', "REG_RDX", "DImode", "movq", "?xorq?", 1, 0],
  ["r64x.dst  : TRUNCATE_DI(LSHIFTRT_TI(MULT_TI(ZERO_EXTEND_TI(r64x.src1) | ZERO_EXTEND_TI(MEM_QI(addr.src2))), CONST8P))",
    [7, 2], "mulq", 'q', "REG_RDX", "DImode", "movq", "?xorq?", 0, 0],
  #
  # TODO: for the signed case, we just use the imulq insn that does ax *= argument,
  #   following what was done above with mulq.
  # However, imulq is much less constrained regarding the operand locations.
  #
  ["r64x.dst  : TRUNCATE_DI(LSHIFTRT_TI(MULT_TI(SIGN_EXTEND_TI(r64x.src1) | SIGN_EXTEND_TI(r64.src2)), CONST8P))",
    [4, 4], "imulq", 'q', "REG_RDX", "DImode", "movq", "?xorq?", 1, 1],
  ["r64x.dst  : TRUNCATE_DI(LSHIFTRT_TI(MULT_TI(SIGN_EXTEND_TI(r64x.src1) | SIGN_EXTEND_TI(MEM_QI(addr.src2))), CONST8P))",
    [7, 2], "imulq", 'q', "REG_RDX", "DImode", "movq", "?xorq?", 0, 1],
], """
  $rule $cost
  supairs {
    if ($isRR) {
      suOrder2($dst, $src1, $src2, kid, kids);
    } else {
      suOrder2($dst, $src1, $src2, kid, kids);
    }
  },
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  coalesce {
    coalesces += attempt_coalesce(pass, $src1->rx, REG_RAX);
    coalesces += attempt_coalesce(pass, $dst->rx, $dst_reg_name);
  },
  build {
    if ('$schar' != 'b') {
      add_copy_edges($dst->rx, REG_RDX, live);
    }
    if ($isRR) {
      sparseset_set_bit(live, find($src2->r));
    } else {
      add_addr(live, $src2);
    }
    if ('$schar' != 'b') {
      sparseset_clear_bit(live, REG_RDX);
      add_edges(REG_RDX, live);	/* RDX is always killed on 64,32,16 bit variants */
    }
    add_copy_edges(REG_RAX, $src1->rx, live);	/* we want these to coalesce */
  },
  remat {
    flags = 0;
  },
  costs {
    cost_copy(REG_RAX, $src1->rx);
    if ($isRR) {
      memorable($src1->rx);
      memorable($src2->r);
    }
    else
      forgettable($src1->rx);
    cost_copy($dst_reg_name, $dst->rx);
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    dump_copy("$mov_opcode", $src1->rx, REG_RAX, '$schar');
    if ($isRR)
      dumpR("$opcode", $src2->r, '$schar');
    else
      dumpM("$opcode", $src2);
    dump_copy("$mov_opcode", $dst_reg_name, $dst->rx, '$schar');
  },
  emit {
    rtx dst  = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src1 = gen_rtx_REG($rtx_mode, $src1->rx);
    rtx src2 = $isRR
      ? gen_rtx_REG($rtx_mode, $src2->r)
      : gen_rtx_MEM($rtx_mode, $src2->rtl);
    /*
     * TODO: raxm, which is the wider mode
     * TODO: rdxm, which is the wider mode
     */
    rtx rax = gen_rtx_REG($rtx_mode, REG_RAX);
    rtx rdx = ('$schar' != 'b') ? gen_rtx_REG($rtx_mode, REG_RDX) : 0;
    rtvec parset = 0;
    icg_emit_plain(gen_rtx_SET(VOIDmode, rax/*raxm?*/, src1));
    /*
     * unsigned multiply modifes the OF and CF flags,
     * so we must use icg_emit_clobber (gen_rtx_CLOBBER)
     */
    parset = gen_rtvec(3,
      gen_rtx_SET(VOIDmode,
	rdx,	/* value comes back in rdx, no shifts needed */
	gen_rtx_TRUNCATE(DImode,
	  gen_rtx_LSHIFTRT(TImode,
	    gen_rtx_MULT(TImode,
	      $issigned
		? gen_rtx_SIGN_EXTEND(TImode, rax)
		: gen_rtx_ZERO_EXTEND(TImode, rax),
	      $issigned
	        ?  gen_rtx_SIGN_EXTEND(TImode, src2)
	        :  gen_rtx_ZERO_EXTEND(TImode, src2)
	      ),
	    gen_rtx_CONST_INT(QImode, 64)
	))),
      gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(DImode, REG_RAX)),	/* perhaps clobber the scratch reg? */
      gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(CCmode, FLAGS_REG))
    );

    icg_emit_plain(gen_rtx_PARALLEL(VOIDmode, parset));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, ($dst_reg_name == REG_RAX) ? rax/*raxm?*/ : rdx/*rdxm*/));
  };
""")

#
# Two address unary operators that do not clobber the destination register;
# See the rule block for multrri for an analog
#
plug.plugrule3("twooperand", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_builder"],

  #["r64x.dst  : BSF_DI(r64.src)",  [4, 3], "bsfq", 'q', "DImode", "/*BOGUS*/gen_rtx_PLUS"],	# TODO
  #["rz32x.dst : BSF_SI(r32.src)",  [3, 2], "bsfl", 'l', "SImode", "/*BOGUS*/gen_rtx_PLUS"],	# TODO
  #["r16x.dst  : BSF_HI(r16.src)",  [4, 3], "bsfw", 'w', "HImode", "/*BOGUS*/gen_rtx_PLUS"],	# TODO

  #["r64x.dst  : BSR_DI(r64.src)",  [4, 3], "bsrq", 'q', "DImode", "/*BOGUS*/gen_rtx_PLUS"],	# TODO
  #["rz32x.dst : BSR_SI(r32.src)",  [3, 2], "bsrl", 'l', "SImode", "/*BOGUS*/gen_rtx_PLUS"],	# TODO
  #["r16x.dst  : BSR_HI(r16.src)",  [4, 3], "bsrw", 'w', "HImode", "/*BOGUS*/gen_rtx_PLUS"],	# TODO

  # CLZ is an x86_64 ABM (advanced bit manipulation) instruction
  ["r64x.dst  : CLZ_DI(r64.src)",  [4, 3], "lzcntq", 'q', "DImode", "gen_rtx_CLZ"],
  ["rz32x.dst : CLZ_SI(r32.src)",  [3, 2], "lzcntl", 'l', "SImode", "gen_rtx_CLZ"],
  ["r16x.dst  : CLZ_HI(r16.src)",  [4, 3], "lzcntw", 'w', "HImode", "gen_rtx_CLZ"],

  ["r64x.dst  : PLUS_DI(CONST_P63, NEG_DI(CLZ_DI(r64.src)))",  [4, 3], "bsrq", 'q', "DImode", "gen_rtx_BSR_DI"],
  ["rz32x.dst : PLUS_DI(CONST_P31, NEG_DI(CLZ_SI(r32.src)))",  [3, 2], "bsrl", 'l', "SImode", "gen_rtx_BSR_SI"],
  ["r16x.dst  : PLUS_DI(CONST_P15, NEG_DI(CLZ_HI(r16.src)))",  [4, 3], "bsrw", 'w', "HImode", "gen_rtx_BSR_HI"],

  # POPCOUNT is an x86_64 ABM (advanced bit manipulation) instruction
  ["r64x.dst  : POPCOUNT_DI(r64.src)",  [4, 3], "popcntq", 'q', "DImode", "gen_rtx_POPCOUNT"],
  ["rz32x.dst : POPCOUNT_SI(r32.src)",  [3, 2], "popcntl", 'l', "SImode", "gen_rtx_POPCOUNT"],
  ["r16x.dst  : POPCOUNT_HI(r16.src)",  [4, 3], "popcntw", 'w', "HImode", "gen_rtx_POPCOUNT"],

], """
  $rule $cost
  supairs {
    $dst->extra = $src->extra;            /* think about these */
    $dst->freed = $src->freed;
  },
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  build {
    add_edges(find($dst->rx), live);
    sparseset_clear_bit(live, find($dst->rx));
    sparseset_set_bit(live, find($src->r));
  },
  costs {
    forgettable($src->r);
  },
  debug {
    dumpRR("$opcode", $src->r, '$schar', $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src = $rtx_builder($rtx_mode,
      gen_rtx_REG($rtx_mode, $src->r)
    );
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")

# })]
plug.plugrule3("prefetch", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "rtx_code"],
  ["stmt  :  PREFETCH_DI(addr, imm32.coded_value)", [3, 2], "prefetchnta", 'q', "DImode", 0],
], """
  $rule $cost
  build {
    add_addr(live, $addr);
  },
  debug {
    dumpM("$opcode", $addr);
  },
  emit {
    const int k1 = $coded_value->val/10;
    const int k2 = $coded_value->val%10;
    icg_emit_plain(gen_rtx_PREFETCH($rtx_mode, $addr->rtl, GEN_INT(k1), GEN_INT(k2)));
  };
""")




#-------------------------------------------------------------
# various peepholes


# A peephole optimization discovered during code comparisons.
# This is an incomplete implementation, driven by the limited constants I had lying around
# (1 through 4 and 8). It'll be easy to make it more complete.
# Could also think about opportunities to propagate zero and sign extension info.
#

plug.plugrule3("peepshifts1", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "andval", "rtx_builder"],

  ["ccr64x.dst  : ASHIFT_DI(LSHIFTRT_DI(r64x.src, CONST_P1), CONST_P1)", [1, 8], "andq", 'q', "DImode", -2,   "gen_rtx_AND"],
  ["ccr64x.dst  : ASHIFT_DI(LSHIFTRT_DI(r64x.src, CONST_P2), CONST_P2)", [1, 8], "andq", 'q', "DImode", -4,   "gen_rtx_AND"],
  ["ccr64x.dst  : ASHIFT_DI(LSHIFTRT_DI(r64x.src, CONST_P3), CONST_P3)", [1, 8], "andq", 'q', "DImode", -8,   "gen_rtx_AND"],
  ["ccr64x.dst  : ASHIFT_DI(LSHIFTRT_DI(r64x.src, CONST_P4), CONST_P4)", [1, 8], "andq", 'q', "DImode", -16,  "gen_rtx_AND"],
  ["ccr64x.dst  : ASHIFT_DI(LSHIFTRT_DI(r64x.src, CONST_P8), CONST_P8)", [1, 8], "andq", 'q', "DImode", -256, "gen_rtx_AND"],

  ["ccrz32x.dst : ASHIFT_SI(LSHIFTRT_SI(r32x.src, CONST_P1), CONST_P1)", [1, 7], "andl", 'l', "SImode", -2,   "gen_rtx_AND"],
  ["ccrz32x.dst : ASHIFT_SI(LSHIFTRT_SI(r32x.src, CONST_P2), CONST_P2)", [1, 7], "andl", 'l', "SImode", -4,   "gen_rtx_AND"],
  ["ccrz32x.dst : ASHIFT_SI(LSHIFTRT_SI(r32x.src, CONST_P3), CONST_P3)", [1, 7], "andl", 'l', "SImode", -8,   "gen_rtx_AND"],
  ["ccrz32x.dst : ASHIFT_SI(LSHIFTRT_SI(r32x.src, CONST_P4), CONST_P4)", [1, 7], "andl", 'l', "SImode", -16,  "gen_rtx_AND"],
  ["ccrz32x.dst : ASHIFT_SI(LSHIFTRT_SI(r32x.src, CONST_P8), CONST_P8)", [1, 7], "andl", 'l', "SImode", -256, "gen_rtx_AND"],

  ["ccr16x.dst  : ASHIFT_HI(LSHIFTRT_HI(r16x.src, CONST_P1), CONST_P1)", [1, 8], "andw", 'w', "HImode", -2,   "gen_rtx_AND"],
  ["ccr16x.dst  : ASHIFT_HI(LSHIFTRT_HI(r16x.src, CONST_P2), CONST_P2)", [1, 8], "andw", 'w', "HImode", -4,   "gen_rtx_AND"],
  ["ccr16x.dst  : ASHIFT_HI(LSHIFTRT_HI(r16x.src, CONST_P3), CONST_P3)", [1, 8], "andw", 'w', "HImode", -8,   "gen_rtx_AND"],
  ["ccr16x.dst  : ASHIFT_HI(LSHIFTRT_HI(r16x.src, CONST_P4), CONST_P4)", [1, 8], "andw", 'w', "HImode", -16,  "gen_rtx_AND"],
  ["ccr16x.dst  : ASHIFT_HI(LSHIFTRT_HI(r16x.src, CONST_P8), CONST_P8)", [1, 8], "andw", 'w', "HImode", -256, "gen_rtx_AND"],

  ["ccr8x.dst   : ASHIFT_QI(LSHIFTRT_QI(r8x.src,  CONST_P1), CONST_P1)", [1, 7], "andb", 'b', "QImode", -2,   "gen_rtx_AND"],
  ["ccr8x.dst   : ASHIFT_QI(LSHIFTRT_QI(r8x.src,  CONST_P2), CONST_P2)", [1, 7], "andb", 'b', "QImode", -4,   "gen_rtx_AND"],
  ["ccr8x.dst   : ASHIFT_QI(LSHIFTRT_QI(r8x.src,  CONST_P3), CONST_P3)", [1, 7], "andb", 'b', "QImode", -8,   "gen_rtx_AND"],
  ["ccr8x.dst   : ASHIFT_QI(LSHIFTRT_QI(r8x.src,  CONST_P4), CONST_P4)", [1, 7], "andb", 'b', "QImode", -16,  "gen_rtx_AND"],

], """
  $rule $cost
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  },
  names {
    $dst->rx = $src->rx;
  },
  final {
    $dst->rx = $src->rx;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
  },
  costs {
    forgettable($src->rx);
  },
  debug {
    dumpIRn("$opcode", $andval, $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src = $rtx_builder($rtx_mode,
			       gen_rtx_REG($rtx_mode, $src->rx),
			       GEN_INT($andval));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")



#
# A peephole optimization discovered during code comparisons.
#
plug.plugrule3("peepshifts2", [
  ["rule", "cost", "opcode", "schar", "rtx_mode", "shift"],

  ["r64x.dst : ASHIFT_DI(ZERO_EXTEND_DI(r32x.src), CONST_P32)", [1, 4], "shlq", 'q', "DImode", 32],
  ["r64x.dst : ASHIFT_DI(ZERO_EXTEND_DI(r16x.src), CONST_P48)", [1, 4], "shlq", 'q', "DImode", 48],
  ["r64x.dst : ASHIFT_DI(ZERO_EXTEND_DI(r8x.src),  CONST_P56)", [1, 4], "shlq", 'q', "DImode", 56],
  ["r32x.dst : ASHIFT_SI(ZERO_EXTEND_SI(r16x.src), CONST_P16)", [1, 3], "shll", 'l', "SImode", 16],
  ["r32x.dst : ASHIFT_SI(ZERO_EXTEND_SI(r8x.src),  CONST_P24)", [1, 3], "shll", 'l', "SImode", 24],
  ["r16x.dst : ASHIFT_HI(ZERO_EXTEND_HI(r8x.src),  CONST_P8)",  [1, 3], "shll", 'l', "SImode",  8],

  ["r64x.dst : ASHIFT_DI(SIGN_EXTEND_DI(r32x.src), CONST_P32)", [1, 4], "shlq", 'q', "DImode", 32],
  ["r64x.dst : ASHIFT_DI(SIGN_EXTEND_DI(r16x.src), CONST_P48)", [1, 4], "shlq", 'q', "DImode", 48],
  ["r64x.dst : ASHIFT_DI(SIGN_EXTEND_DI(r8x.src),  CONST_P56)", [1, 4], "shlq", 'q', "DImode", 56],
  ["r32x.dst : ASHIFT_SI(SIGN_EXTEND_SI(r16x.src), CONST_P16)", [1, 3], "shll", 'l', "SImode", 16],
  ["r32x.dst : ASHIFT_SI(SIGN_EXTEND_SI(r8x.src),  CONST_P24)", [1, 3], "shll", 'l', "SImode", 24],
  ["r16x.dst : ASHIFT_HI(SIGN_EXTEND_HI(r8x.src),  CONST_P8)",  [1, 3], "shll", 'l', "SImode",  8],

], """
  $rule $cost
  supairs {
    $dst->extra = $src->extra;
    $dst->freed = $src->freed;
  },
  names {
    $dst->rx = $src->rx;
  },
  final {
    $dst->rx = $src->rx;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
  },
  costs {
    forgettable($src->rx);
  },
  debug {
    dumpIRn("$opcode", $shift, $dst->rx, '$schar');
  },
  emit {
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    rtx src = gen_rtx_ASHIFT($rtx_mode,
                             gen_rtx_REG($rtx_mode, $src->rx),
			     GEN_INT($shift));
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, src));
  };
""")




#
# A peephole optimization discovered during code comparisons.
#
plug.plugrule3("peepand1", [
  ["rule", "cost"],

  ["r64x.dst  : AND_DI(               r64.src  | CONST_P255)", [1, 3]],
  ["r64x.dst  : AND_DI(ZERO_EXTEND_DI(r32.src) | CONST_P255)", [1, 3]],
  ["r64x.dst  : AND_DI(SIGN_EXTEND_DI(r32.src) | CONST_P255)", [1, 3]],
  ["r64x.dst  : AND_DI(ZERO_EXTEND_DI(r16.src) | CONST_P255)", [1, 3]],
  ["r64x.dst  : AND_DI(SIGN_EXTEND_DI(r16.src) | CONST_P255)", [1, 3]],
  ["r64x.dst  : AND_DI(ZERO_EXTEND_DI(r8.src)  | CONST_P255)", [1, 3]],
  ["r64x.dst  : AND_DI(SIGN_EXTEND_DI(r8.src)  | CONST_P255)", [1, 3]],

  ["rz32x.dst : AND_SI(               r32.src  | CONST_P255)", [1, 3]],
  ["rz32x.dst : AND_SI(ZERO_EXTEND_SI(r16.src) | CONST_P255)", [1, 3]],
  ["rz32x.dst : AND_SI(SIGN_EXTEND_SI(r16.src) | CONST_P255)", [1, 3]],
  ["rz32x.dst : AND_SI(ZERO_EXTEND_SI(r8.src)  | CONST_P255)", [1, 3]],
  ["rz32x.dst : AND_SI(SIGN_EXTEND_SI(r8.src)  | CONST_P255)", [1, 3]],

  ["rz16x.dst : AND_HI(               r16.src  | CONST_P255)", [1, 3]],
  ["rz16x.dst : AND_HI(ZERO_EXTEND_HI(r8.src)  | CONST_P255)", [1, 3]],
  ["rz16x.dst : AND_HI(SIGN_EXTEND_HI(r8.src)  | CONST_P255)", [1, 3]],

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
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  build {
    add_edges(find($dst->rx), live);
    sparseset_clear_bit(live, find($dst->rx));
    sparseset_set_bit(live, find($src->r));
  },
  costs {
    memorable($src->r);
  },
  debug {
    dumpRR("movzbl", $src->r, 'b', $dst->rx, 'l');
  },
  emit {
    rtx dst = gen_rtx_REG(SImode, $dst->rx);
    rtx src = gen_rtx_ZERO_EXTEND(SImode, gen_rtx_REG(QImode, $src->r));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


plug.plugrule3("peepand2", [
  ["rule", "cost"],

  ["r64x.dst  : AND_DI(               MEM_DI(addr)  | CONST_P255)", [4, 2]],
  ["r64x.dst  : AND_DI(ZERO_EXTEND_DI(MEM_SI(addr)) | CONST_P255)", [4, 2]],
  ["r64x.dst  : AND_DI(SIGN_EXTEND_DI(MEM_SI(addr)) | CONST_P255)", [4, 2]],
  ["r64x.dst  : AND_DI(ZERO_EXTEND_DI(MEM_HI(addr)) | CONST_P255)", [4, 2]],
  ["r64x.dst  : AND_DI(SIGN_EXTEND_DI(MEM_HI(addr)) | CONST_P255)", [4, 2]],
  ["r64x.dst  : AND_DI(ZERO_EXTEND_DI(MEM_QI(addr)) | CONST_P255)", [4, 2]],
  ["r64x.dst  : AND_DI(SIGN_EXTEND_DI(MEM_QI(addr)) | CONST_P255)", [4, 2]],

  ["rz32x.dst : AND_SI(               MEM_SI(addr)  | CONST_P255)", [4, 2]],
  ["rz32x.dst : AND_SI(ZERO_EXTEND_DI(MEM_HI(addr)) | CONST_P255)", [4, 2]],
  ["rz32x.dst : AND_SI(SIGN_EXTEND_DI(MEM_HI(addr)) | CONST_P255)", [4, 2]],
  ["rz32x.dst : AND_SI(ZERO_EXTEND_DI(MEM_QI(addr)) | CONST_P255)", [4, 2]],
  ["rz32x.dst : AND_SI(SIGN_EXTEND_DI(MEM_QI(addr)) | CONST_P255)", [4, 2]],

  ["rz16x.dst : AND_HI(               MEM_HI(addr)  | CONST_P255)", [4, 2]],
  ["rz16x.dst : AND_HI(ZERO_EXTEND_DI(MEM_QI(addr)) | CONST_P255)", [4, 2]],
  ["rz16x.dst : AND_HI(SIGN_EXTEND_DI(MEM_QI(addr)) | CONST_P255)", [4, 2]],

], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
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
    dumpMR("movzbl", $addr, $dst->rx, 'l');
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  emit {
    rtx dst = gen_rtx_REG(SImode, $dst->rx);
    rtx src = gen_rtx_ZERO_EXTEND(SImode, gen_rtx_MEM(QImode, $addr->rtl));
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


