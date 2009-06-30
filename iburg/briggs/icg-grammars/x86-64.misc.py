# {([

#
# Copyright (c) 2008 Google Inc. All rights reserved.
#
# $Header: $
#
# -*- mode: python -*-

import plug

# global_print_plug_decls = 0; execfile("x86-64.gr.py")

#------------------------------------------------------------------------
# addressing modes

plug.plugrule3("addrbid", [
  ["rule", "cost"],
  ["addr : PLUS_DI(base64.base | index64.index | disp.disp)", [0, 0]],
], """
  $rule $cost
  supairs {
    suOrder2($addr, $base, $index, kid, kids);
  },
  names {
    $addr->a.base   = $base->a.base;
    $addr->a.base_valid  = $base->a.base_valid;
    $addr->a.index  = $index->a.index;
    $addr->a.scale  = $index->a.scale;
    $addr->a.disp   = $disp->a.disp;
    $addr->a.string = $disp->a.string;
  },
  final {
    $addr->a.base  = $base->a.base;
    $addr->a.index = $index->a.index;
  },
  emit {
    $addr->rtl = gen_rtx_addr(DImode, $base->rtl, $index->rtl, $disp->rtl);
  };
""")

plug.plugrule3("addrbio", [
  ["rule", "cost", "scalefactor"],

  ["addr : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm5.offset)  | CONST_P8))", [0, 1], 8],
  ["addr : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm29.offset) | CONST_P8))", [0, 1], 8],
  ["addr : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm6.offset)  | CONST_P4))", [0, 1], 4],
  ["addr : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm30.offset) | CONST_P4))", [0, 1], 4],
  ["addr : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm7.offset)  | CONST_P2))", [0, 1], 2],
  ["addr : PLUS_DI(base64.base | MULT_DI(PLUS_DI(r64.index | imm31.offset) | CONST_P2))", [0, 1], 2],

  ["addr : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm5.offset),  CONST_P3))", [0, 1], 8],
  ["addr : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm29.offset), CONST_P3))", [0, 1], 8],
  ["addr : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm6.offset),  CONST_P2))", [0, 1], 4],
  ["addr : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm30.offset), CONST_P2))", [0, 1], 4],
  ["addr : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm7.offset),  CONST_P1))", [0, 1], 2],
  ["addr : PLUS_DI(base64.base | ASHIFT_DI(PLUS_DI(r64.index | imm31.offset), CONST_P1))", [0, 1], 2],

], """
  $rule $cost
  supairs {
    suOrder2($addr, $base, $index, kid, kids);
  },
  names {
    $addr->a.base   = $base->a.base;
    $addr->a.base_valid  = $base->a.base_valid;
    $addr->a.index  = $index->r;
    $addr->a.scale  = $scalefactor;
    $addr->a.disp   = $offset->val * $scalefactor;
    $addr->a.string = NULL;
  },
  costs {
    forgettable($index->r);
  },
  final {
    $addr->a.base  = $base->a.base;
    $addr->a.index = $index->r;
  },
  emit {
    rtx src2 = gen_rtx_REG(DImode, $index->r);
    $addr->rtl = gen_rtx_addr(DImode, $base->rtl,
			   gen_rtx_MULT(DImode, src2, GEN_INT($scalefactor)),
			   GEN_INT($offset->val * $scalefactor));
  };
""")


# The costs here are strange because there are 2 immediates feeding into each rule.
# Maybe I need to introduce negative costs.
plug.plugrule3("addrbio1", [
  ["rule", "cost", "scalefactor"],

  ["addr : PLUS_DI(base64.base | imm7.const1  | MULT_DI(PLUS_DI(r64.index | imm4.const2)  | CONST_P8))", [0, 0], 8],
  ["addr : PLUS_DI(base64.base | imm31.const1 | MULT_DI(PLUS_DI(r64.index | imm28.const2) | CONST_P8))", [0, 0], 8],
  ["addr : PLUS_DI(base64.base | imm7.const1  | MULT_DI(PLUS_DI(r64.index | imm5.const2)  | CONST_P4))", [0, 0], 4],
  ["addr : PLUS_DI(base64.base | imm31.const1 | MULT_DI(PLUS_DI(r64.index | imm29.const2) | CONST_P4))", [0, 0], 4],
  ["addr : PLUS_DI(base64.base | imm7.const1  | MULT_DI(PLUS_DI(r64.index | imm6.const2)  | CONST_P2))", [0, 0], 2],
  ["addr : PLUS_DI(base64.base | imm31.const1 | MULT_DI(PLUS_DI(r64.index | imm30.const2) | CONST_P2))", [0, 0], 2],

  ["addr : PLUS_DI(base64.base | imm7.const1  | ASHIFT_DI(PLUS_DI(r64.index | imm4.const2),  CONST_P3))", [0, 0], 8],
  ["addr : PLUS_DI(base64.base | imm31.const1 | ASHIFT_DI(PLUS_DI(r64.index | imm28.const2), CONST_P3))", [0, 0], 8],
  ["addr : PLUS_DI(base64.base | imm7.const1  | ASHIFT_DI(PLUS_DI(r64.index | imm5.const2),  CONST_P2))", [0, 0], 4],
  ["addr : PLUS_DI(base64.base | imm31.const1 | ASHIFT_DI(PLUS_DI(r64.index | imm29.const2), CONST_P2))", [0, 0], 4],
  ["addr : PLUS_DI(base64.base | imm7.const1  | ASHIFT_DI(PLUS_DI(r64.index | imm6.const2),  CONST_P1))", [0, 0], 2],
  ["addr : PLUS_DI(base64.base | imm31.const1 | ASHIFT_DI(PLUS_DI(r64.index | imm30.const2), CONST_P1))", [0, 0], 2],

], """
  $rule $cost
  supairs {
    suOrder2($addr, $base, $index, kid, kids);
  },
  names {
    $addr->a.base   = $base->a.base;
    $addr->a.base_valid  = $base->a.base_valid;
    $addr->a.index  = $index->r;
    $addr->a.scale  = $scalefactor;
    $addr->a.disp   = $const1->val + $const2->val * $scalefactor;
    $addr->a.string = NULL;
  },
  costs {
    forgettable($index->r);
  },
  final {
    $addr->a.base  = $base->a.base;
    $addr->a.index = $index->r;
  },
  emit {
    rtx src2 = gen_rtx_REG(DImode, $index->r);
    $addr->rtl = gen_rtx_addr(DImode, $base->rtl,
			   gen_rtx_MULT(DImode, src2, GEN_INT($scalefactor)),
			   GEN_INT($const1->val + $const2->val * $scalefactor));
  };
""")

plug.plugrule3("addrbi", [
  ["rule", "cost"],
  ["addr : PLUS_DI(base64.base | index64.index)", [0, 0]],
], """
  $rule $cost
  supairs {
    suOrder2($addr, $base, $index, kid, kids);
  },
  names {
    $addr->a.base   = $base->a.base;
    $addr->a.base_valid  = $base->a.base_valid;
    $addr->a.index  = $index->a.index;
    $addr->a.scale  = $index->a.scale;
    $addr->a.disp   = 0;
    $addr->a.string = NULL;
  },
  final {
    $addr->a.base  = $base->a.base;
    $addr->a.index = $index->a.index;
  },
  emit {
    $addr->rtl = gen_rtx_PLUS(DImode, $base->rtl, $index->rtl);
  };
""")

plug.plugrule3("addrbd", [
  ["rule", "cost"],
  ["addr : PLUS_DI(base64.base | disp.disp)", [0, 0]],
], """
  $rule $cost
  names {
    $addr->a.base   = $base->a.base;
    $addr->a.base_valid  = $base->a.base_valid;
    $addr->a.index  = 0;
    $addr->a.scale  = 0;
    $addr->a.disp   = $disp->a.disp;
    $addr->a.string = $disp->a.string;
  },
  final {
    $addr->a.base  = $base->a.base;
  },
  supairs {
    $addr->extra = $base->extra;
    $addr->freed = $base->freed;
  },
  emit {
    $addr->rtl = gen_rtx_PLUS(DImode, $base->rtl, $disp->rtl);
  };
""")

plug.plugrule3("addrid", [
  ["rule", "cost"],
  ["addr : PLUS_DI(index64.index | imm32.disp)", [0, 1]],
], """
  $rule $cost
  names {
    $addr->a.base_valid = 0;
    $addr->a.index  = $index->a.index;
    $addr->a.scale  = $index->a.scale;
    $addr->a.disp   = $disp->val;
    $addr->a.string = $disp->a.string;
  },
  final {
    $addr->a.index = $index->a.index;
  },
  supairs {
    $addr->extra = $index->extra;
    $addr->freed = $index->freed;
  },
  emit {
    $addr->rtl = gen_rtx_PLUS(DImode,
      $index->rtl,
      gen_rtx_imm_constant($disp->val, $disp->a.string, $disp->rtl));
  };
""")

plug.plugrule3("addrbase", [
  ["rule", "cost"],
  ["addr : base64.base", [0, 0]],
], """
  $rule $cost
  names {
    $addr->a.index  = 0;
    $addr->a.scale  = 0;
    $addr->a.disp   = 0;
    $addr->a.string = NULL;
  },
  emit {
    $addr->rtl = $base->rtl;
  };
""")

plug.plugrule3("addrindex", [
  ["rule", "cost"],
  ["addr : index64.index", [0, 5]],
], """
  $rule $cost
  names {
    $addr->a.base_valid  = 0;
    $addr->a.disp   = 0;
    $addr->a.string = NULL;
  },
  emit {
    $addr->rtl = $index->rtl;
  };
""")

#
# This chain rule "addr -> symbol" will hopefully be used preferentially
# to avoid a double chain rule application "addr -> imm32 -> symbol".
# The double chain rule will discard semantic information about the symbol.
#
plug.plugrule3("addrimm32", [
  ["rule", "cost"],
  ["addr.dst : symbol32.const",   [0, 1]],
  ["addr.dst : symbol64.const",   [0, 1]],
], """
  $rule $cost
  names {
    $dst->a = $const->a;	/* preserve all semantic info */
  },
  emit {
    rtx expr = $dst->rtl;	/* RTL from original tree (thin ice!) */
    $dst->rtl = gen_rtx_imm_constant($dst->a.disp, $dst->a.string, expr);
  };
""")

plug.plugrule3("addrimm32", [
  ["rule", "cost"],
  ["addr.dst : imm32.const", [0, 1]],
], """
  $rule $cost
  names {
    $dst->a.base_valid = 0;
    $dst->a.index = 0;
    $dst->a.scale = 0;
    $dst->a.disp  = $const->val;
  },
  emit {
    rtx expr = $dst->rtl;	/* RTL from original tree (thin ice!) */
    $dst->rtl = gen_rtx_imm_constant($dst->a.disp, $dst->a.string, expr);
  };
""")

plug.plugrule3("dispimm8", [
  ["rule", "cost"],
  ["disp : imm8.const", [0, 0]],
  ["disp : imm32.const", [0, 0]],
], """
  $rule $cost
  names {
    $disp->a.disp = $const->val;
  },
  emit {
    $disp->rtl = gen_rtx_imm_constant($disp->a.disp, $disp->a.string, $disp->rtl);
  };
""")

plug.plugrule3("basereg", [
  ["rule", "cost", "rtx_mode"],
  ["base64.base : r64.src", [0, 1], "DImode"],
  ["base32.base : r32.src", [0, 1], "SImode"], # address reg accessed as a SI
  ["base16.base : r16.src", [0, 1], "SImode"], # address reg accessed as a SI
  ["base8.base  : r8.src",  [0, 1], "SImode"], # address reg accessed as a SI
], """
  $rule $cost
  names {
    $base->a.base = $src->r;
    $base->a.base_valid = 1;
    $base->a.base_rip = 0;
  },
  costs {
    forgettable($src->r);
  },
  final {
    $base->a.base = $src->r;
  },
  emit {
    $base->rtl = gen_rtx_REG($rtx_mode, $src->r);
  };
""")

plug.plugrule3("indexreg", [
  ["rule", "cost", "rtx_mode"],
  ["index64.index : r64.src", [0, 1], "DImode"],
  ["index32.index : r32.src", [0, 1], "SImode"],
  ["index16.index : r16.src", [0, 1], "SImode"],
  ["index8.index  : r8.src",  [0, 1], "SImode"],
], """
  $rule $cost
  names {
    $index->a.index = $src->r;
    $index->a.scale = 1;
  },
  costs {
    forgettable($src->r);
  },
  final {
    $index->a.index = $src->r;
  },
  emit {
    $index->rtl = gen_rtx_REG($rtx_mode, $src->r);
  };
""")

plug.plugrule3("indexscalereg", [
  ["rule", "cost", "rtx_mode", "is_shift"],

  ["index64.index : ASHIFT_DI(r64.src, scon.const)",                                     [0, 1], "DImode", 1],
  ["index32.index : ASHIFT_SI(r32.src, scon.const)",                                     [0, 1], "SImode", 1],
  ["index16.index : ASHIFT_HI(r16.src, scon.const)",                                     [0, 1], "HImode", 1],
  ["index8.index  : ASHIFT_QI(r8.src,  scon.const)",                                     [0, 1], "QImode", 1],

  ["index64.index : MULT_DI(r64.src | mcon.const)",                                      [0, 1], "DImode", 0],
  ["index32.index : MULT_SI(r32.src | mcon.const)",                                      [0, 1], "SImode", 0],
  ["index16.index : MULT_HI(r16.src | mcon.const)",                                      [0, 1], "HImode", 0],
  ["index8.index  : MULT_QI(r8.src  | mcon.const)",                                      [0, 1], "QImode", 0],

  ["index16.index : SUBREG_HI(ASHIFT_SI(SIGN_EXTEND_SI(r16.src), scon.const), CONST_0)", [0, 1], "HImode", 1],
  ["index16.index : SUBREG_HI(ASHIFT_SI(ZERO_EXTEND_SI(r16.src), scon.const), CONST_0)", [0, 1], "HImode", 1],
  ["index8.index  : SUBREG_QI(ASHIFT_SI(SIGN_EXTEND_SI(r8.src),  scon.const), CONST_0)", [0, 1], "QImode", 1],
  ["index8.index  : SUBREG_QI(ASHIFT_SI(ZERO_EXTEND_SI(r8.src),  scon.const), CONST_0)", [0, 1], "QImode", 1],

], """
  $rule $cost
  names {
    $index->a.index = $src->r;
    $index->a.scale = $is_shift ? (1 << $const->val) : $const->val;
  },
  final {
    $index->a.index = $src->r;
  },
  costs {
    forgettable($src->r);
  },
  supairs {
    $index->extra = $src->extra;
    $index->freed = $src->freed;
  },
  emit {
    $index->rtl = gen_rtx_MULT($rtx_mode,
      gen_rtx_REG($rtx_mode, $src->r),
      GEN_INT($is_shift ? (1 << $const->val) : $const->val));
  };
""")


#--------------------------------------------------------------
# constants
plug.plugrule3("constants", [
  ["rule", "cost"],

  ["scon.dst : CONST_0",   [0, 0]],
  ["scon.dst : CONST_P1",  [0, 0]],
  ["scon.dst : CONST_P2",  [0, 0]],
  ["scon.dst : CONST_P3",  [0, 0]],

  ["mcon.dst : CONST_P1",  [0, 0]],
  ["mcon.dst : CONST_P2",  [0, 0]],
  ["mcon.dst : CONST_P4",  [0, 0]],
  ["mcon.dst : CONST_P8",  [0, 0]],

  ["imm4.dst : CONST_N1",  [0, 1]],
  ["imm4.dst : CONST_0",   [0, 1]],
  ["imm4.dst : CONST_P1",  [0, 1]],
  ["imm4.dst : CONST_P2",  [0, 1]],
  ["imm4.dst : CONST_P3",  [0, 1]],
  ["imm4.dst : CONST_P4",  [0, 0]],
  ["imm4.dst : CONST4P",   [0, 1]],
  ["imm4.dst : CONST4N",   [0, 1]],

  ["imm5.dst : imm4.src",  [0, 0]],
  ["imm5.dst : CONST_P8",  [0, 1]],
  ["imm5.dst : CONST_P15", [0, 1]],
  ["imm5.dst : CONST5P",   [0, 1]],
  ["imm5.dst : CONST5N",   [0, 1]],

  ["imm6.dst : imm5.src",  [0, 0]],
  ["imm5.dst : CONST_P16", [0, 1]],
  ["imm5.dst : CONST_P24", [0, 1]],
  ["imm5.dst : CONST_P31", [0, 1]],
  ["imm6.dst : CONST_P32", [0, 1]],
  ["imm6.dst : CONST6P",   [0, 1]],
  ["imm6.dst : CONST6N",   [0, 1]],

  ["imm7.dst : imm6.src",  [0, 0]],
  ["imm6.dst : CONST_P48", [0, 1]],
  ["imm6.dst : CONST_P56", [0, 1]],
  ["imm6.dst : CONST_P63", [0, 1]],
  ["imm7.dst : CONST7P",   [0, 1]],
  ["imm7.dst : CONST7N",   [0, 1]],

  ["imm8.dst : imm7.src",  [0, 0]],
  ["imm8.dst : CONST8P",   [0, 1]],
  ["imm8.dst : CONST8N",   [0, 1]],

  ["imm12.dst : imm8.src",   [0, 1]],
  ["imm12.dst : CONST_P255", [0, 2]],
  ["imm12.dst : CONST12P",   [0, 2]],
  ["imm12.dst : CONST12N",   [0, 2]],

  ["imm13.dst : imm12.src",  [0, 0]],
  ["imm13.dst : CONST13P",   [0, 2]],
  ["imm13.dst : CONST13N",   [0, 2]],

  ["imm14.dst : imm13.src", [0, 0]],
  ["imm14.dst : CONST14P",  [0, 2]],
  ["imm14.dst : CONST14N",  [0, 2]],

  ["imm15.dst : imm14.src", [0, 0]],
  ["imm15.dst : CONST15P",  [0, 2]],
  ["imm15.dst : CONST15N",  [0, 2]],

  ["imm16.dst : imm15.src", [0, 0]],
  ["imm16.dst : CONST16P",  [0, 2]],
  ["imm16.dst : CONST16N",  [0, 2]],

  ["imm28.dst : imm16.src", [0, 2]],
  ["imm28.dst : CONST28P",  [0, 4]],
  ["imm28.dst : CONST28N",  [0, 4]],

  ["imm29.dst : imm28.src", [0, 2]],
  ["imm29.dst : CONST29P",  [0, 4]],
  ["imm29.dst : CONST29N",  [0, 4]],

  ["imm30.dst : imm29.src", [0, 0]],
  ["imm30.dst : CONST30P",  [0, 4]],
  ["imm30.dst : CONST30N",  [0, 4]],

  ["imm31.dst : imm30.src", [0, 0]],
  ["imm31.dst : CONST31P",  [0, 4]],
  ["imm31.dst : CONST31N",  [0, 4]],

  ["imm32.dst : pureimm32.src",   [0, 0]],
  ["pureimm32.dst : imm31.src",   [0, 0]],
  ["pureimm32.dst : CONST32P",    [0, 4]],
  ["pureimm32.dst : CONST32N",    [0, 4]],
  ["pureimm32.dst : CONSTIEEE32", [0, 8]],

  ["imm64.dst : imm32.src",   [0, 4]],
  ["imm64.dst : CONST64P",    [0, 8]],
  ["imm64.dst : CONST64N",    [0, 8]],
  ["imm64.dst : CONSTIEEE64", [0, 8]],

  ["pos8.dst  : mcon.src",  [0, 1]],
  ["pos8.dst  : scon.src",  [0, 1]],
  ["pos8.dst  : CONST4P",   [0, 1]],
  ["pos8.dst  : CONST5P",   [0, 1]],
  ["pos8.dst  : CONST6P",   [0, 1]],
  ["pos8.dst  : CONST7P",   [0, 1]],
  ["pos8.dst  : CONST8P",   [0, 1]],
  ["pos16.dst : pos8.src",  [0, 1]],
  ["pos16.dst : CONST12P",  [0, 2]],
  ["pos16.dst : CONST13P",  [0, 2]],
  ["pos16.dst : CONST14P",  [0, 2]],
  ["pos16.dst : CONST15P",  [0, 2]],
  ["pos16.dst : CONST16P",  [0, 2]],
  ["pos32.dst : pos16.src", [0, 2]],
  ["pos32.dst : CONST28P",  [0, 4]],
  ["pos32.dst : CONST29P",  [0, 4]],
  ["pos32.dst : CONST30P",  [0, 4]],
  ["pos32.dst : CONST31P",  [0, 4]],
  ["pos32.dst : CONST32P",  [0, 4]],

], """
  $rule $cost
  ;
""")

#--------------------------------------------------------------
# relocatable constants


plug.plugrule3("relocconstants", [
  ["rule", "cost"],
  ["symbol32.dst : SYMBOL_REF_32", [0, 0]],
  ["symbol64.dst : SYMBOL_REF_64", [0, 0]],
], """
  $rule $cost
  names {
    $dst->a.base_rip = 1;		/* %rip usage */
  };
""")

plug.plugrule3("relocconstants", [
  ["rule", "cost"],
  ["symbol32.dst : LABEL_REF_DI", [0, 0]],
], """
  $rule $cost
  names {
    $dst->a.base_rip = 1;		/* %rip usage(?) */
  };
""")


plug.plugrule3("symbolmix",[
  ["rule", "cost"],
  ["symbol.dst : symbol32.src", [0, 0]],
  ["symbol.dst : symbol64.src", [0, 0]],
], """
  $rule $cost
  ;
""")

plug.plugrule3("symbolconst1",[
  ["rule", "cost"],
  ["symbol32.dst : pureimm32.src", [0, 0]],
  ["symbol64.dst : pureimm32.src", [0, 0]],
], """
  $rule $cost
  ;
""")

#
# The upstream part gcc sometimes has CONST_DI, and sometimes they don't.
# So we let them have as many as they want; it all seems somewhat random.
#
plug.plugrule3("symbolconst2", [
  ["rule", "cost"],
  ["symbol32.dst : CONST_DI(symbol32.src)", [0, 0]],
  ["symbol64.dst : CONST_DI(symbol64.src)", [0, 0]],
], """
  $rule $cost
  names {
    $dst->a.string = $src->a.string;
    $dst->val = $src->val;
    $dst->a.base_rip = 1;	/* %rip usage */
  };
""")

plug.plugrule3("symbolsymconstplus", [
  ["rule", "cost"],
  ["symbol32.dst : PLUS_DI(symbol32.src | pureimm32.const)", [0, 0]],
  ["symbol64.dst : PLUS_DI(symbol64.src | pureimm32.const)", [0, 0]],
], """
  $rule $cost
  names {
    $dst->a.string = $src->a.string;
    $dst->val = $src->val + $const->val;
    $dst->a.base_rip = 1;	/* %rip usage */
  };
""")

plug.plugrule3("symbolsymconstminus", [
  ["rule", "cost"],
  ["symbol32.dst : MINUS_DI(symbol32.src, pureimm32.const)", [0, 0]],
  ["symbol64.dst : MINUS_DI(symbol64.src, pureimm32.const)", [0, 0]],
], """
  $rule $cost
  names {
    $dst->a.string = $src->a.string;
    $dst->val = $src->val - $const->val;
    $dst->a.base_rip = 1;	/* %rip usage */
  };
""")

plug.plugrule3("immsym", [
  ["rule", "cost"],
  ["imm32.dst : symbol32.src", [0, 4]],
  ["imm64.dst : symbol64.src", [0, 8]],
], """
  $rule $cost
  names {
    /* this transfers the "a" attribute from src to dst */
  }
  ;
""")


#--------------------------------------------------------------
# control flow

# we don't do anything for jmps and branches;
# just use the old instruction.

# TODO: need some odd expressions to deal with case statements.

# Need to do tests to be sure these are accurate.
# How are floats compared?
#
# Later, when we compare r64 to immediate,
# remember to try reverse tests too.
#
# May want to do these inline with IF_THEN_ELSE
# so that SU numbering can take advantage of all registers.


plug.plugrule3("label", [
  ["rule", "cost"],
  ["label : LABEL_REF_DI", [0, 0]],
], """
  $rule $cost
  ;
""")

#--------------------------------------------------------------
# non-computed unconditional branch (simple)
#
plug.plugrule3("jmp", [
  ["rule", "cost"],
  # TODO: bogus cost
  ["stmt : SET_ALL(PC_ALL, label.target)", [0, 0]],
], """
  $rule $cost
  debug {
    dump_jump("jmp", $target);
  },
  emit {
    rtx label = XEXP ($target->rtl, 0);
    rtx new_jump = emit_jump_insn (gen_jump (label));
    LABEL_NUSES (label) += 1;
    JUMP_LABEL (new_jump) = label;
  };
""")

#--------------------------------------------------------------
# computed unconditional branch (indirect goto)
#
plug.plugrule3("jmp", [
  ["rule", "cost"],
  # TODO: bogus cost
  ["stmt : PARALLEL_ALL(SET_ALL(PC_ALL, r64.src1), USE_ALL(label.target))", [0, 0]],
], """
  $rule $cost
  build {
    sparseset_set_bit(live, find($src1->r));
  },
  remat {
    flags = 0;
  },
  costs {
    memorable($src1->r);
  },
  debug {
    dump_jumpR("jmp", $src1->r, 'q');
  },
  emit {
    rtx label = XEXP ($target->rtl, 0);
    rtx new_jump = emit_jump_insn(
      gen_rtx_PARALLEL(VOIDmode,
                       gen_rtvec(2,
                                 gen_rtx_SET(VOIDmode,
                                             gen_rtx_PC(VOIDmode),
                                             gen_rtx_REG(DImode, $src1->r)),
                                 gen_rtx_USE(VOIDmode,
                                             $target->rtl))));
    LABEL_NUSES (label) += 1;
    JUMP_LABEL (new_jump) = label;
  };
""")



"""
A note on the x86 conditionals.
This information was lifted directly from the pdf version
of the AMD64 Programmer's Reference Manual, Volume 3.
jo	overflow		(of = 1).

jno	not overflow		(of = 0).

jb	below			(cf = 1).		unsigned <
jc	carry			(cf = 1).		unsigned <
jnae	not above or equal	(cf = 1).		unsigned <

jnb	not below		(cf = 0).		unsigned >=
jnc	not carry		(cf = 0).		unsigned >=
jae	above or equal		(cf = 0).		unsigned >=

jz	zero			(zf = 1).
je	equal			(zf = 1).

jnz	not zero		(zf = 0).
jne	not equal		(zf = 0).

jbe	below or equal		(cf = 1 or zf = 1).	unsigned <=
jna	not above		(cf = 1 or zf = 1).	unsigned <=

jnbe	not below or equal	(cf = 0 and zf = 0).	unsigned >
ja	above			(cf = 0 and zf = 0).	unsigned >

js	sign			(sf = 1).

jns	not sign		(sf = 0).

jp	parity			(pf = 1).
jpe	parity even		(pf = 1).

jnp	not parity		(pf = 0).
jpo	parity odd		(pf = 0).

jl	less			(sf <> of).		signed <
jnge	not greater or equal	(sf <> of).		signed <

jnl	not less		(sf = of).		signed >=
jge	greater or equal	(sf = of).		signed >=

jle	less or equal		(zf = 1 or sf <> of).	signed <=
jng	not greater		(zf = 1 or sf <> of).	signed <=

jnle	not less or equal	(zf = 0 and sf = of).	signed >
jg	greater			(zf = 0 and sf = of).	signed >

"""

#--------------------------------------------------------------
# conditional branches
#

plug.plugrule3("jcc", [
  ["rule", "cost"],
  ["stmt : SET_ALL(PC_ALL, COND_MOVE(condition.cond, PAIR_ALL(label.target, PC_ALL)))", [0, 0]],
], """
  $rule $cost
  debug {
    const enum rtx_code code = $cond->code;
    switch (code) {
    case UNEQ:			/* unordered equality testing */
      dump_jump("je", $target);
      dump_jump("jp", $target);	/* jump if unordered */
      break;
    case LTGT:			/* unordered inequality testing */
      dump_jump("jne", $target);
      dump_jump("jp", $target);	/* jump if unordered */
      break;
    case UNGE:      dump_jump("jae", $target); break;
    case UNGT:      dump_jump("ja",  $target); break;
    case UNLE:      dump_jump("jbe", $target); break;
    case UNLT:      dump_jump("jb",  $target); break;
    case UNORDERED: dump_jump("jp",  $target); break;
    case   ORDERED: dump_jump("jnp", $target); break;

    case EQ:        dump_jump("je",  $target); break;
    case NE:        dump_jump("jne", $target); break;
    case GE:        dump_jump("jge", $target); break;
    case GT:        dump_jump("jg",  $target); break;
    case LE:        dump_jump("jle", $target); break;
    case LT:        dump_jump("jl",  $target); break;

    case GEU:       dump_jump("jae", $target); break;
    case GTU:       dump_jump("ja",  $target); break;
    case LEU:       dump_jump("jbe", $target); break;
    case LTU:       dump_jump("jb",  $target); break;

    default:
      icg_nyi("unrecognized condition code %d for jump", code);
    }
  };
""")


#--------------------------------------------------------------
# conditional moves in the GPR set.
# These are implemented via CMOVcc.
# Aook at attribute indicating kind of condition.

plug.plugrule3("cmovreg", [
  ["rule", "cost", "schar", "rtx_mode", "is_normal"],

  ["r64x.dst : COND_MOVE(condition.cond, PAIR_ALL(r64x.src1, r64.src2))", [1, 4], 'q', "DImode", 1],
  ["r32x.dst : COND_MOVE(condition.cond, PAIR_ALL(r32x.src1, r32.src2))", [1, 3], 'd', "SImode", 1],
  ["r16x.dst : COND_MOVE(condition.cond, PAIR_ALL(r16x.src1, r16.src2))", [1, 3], 'd', "SImode", 1],
  ["r8x.dst  : COND_MOVE(condition.cond, PAIR_ALL(r8x.src1,  r8.src2))",  [1, 3], 'd', "SImode", 1],

], """
  $rule $cost
  names {
    $dst->rx = $src1->rx;
    if ($is_normal) {
      /*
       * set the order the children are visited;
       * when su ordering is working, this will force a Sethi-Ullman traversal.
       */
      p->perm[0] = 1;
      p->perm[1] = 2;
      p->perm[2] = 0; /* ensuring that condition is evaluated last */
    }
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
    forgettable($src1->rx);
    memorable($src2->r);
  },
  debug {
    const enum rtx_code code = $cond->code;
    switch (code) {
    /*
     * NOTE A:
     * There is empirical evidence suggesting that the conditions and the cmov opcodes
     * must be inverted relative to each other.  This will make the icg+asm
     * path produce identical asm code to the icg+rtl path.
     * 03Nov08: After 4 hours of searching, rrh does not have an explanation for this.
     * Alternatively, try swapping the fsrc and tsrc args to gen_rtx_IF_THEN_ELSE
     * perhaps this is because of the evaluation permutation that is induced?
     * By swapping fsrc and tsrc to the gen_rtx_IF_THEN_ELSE
     * we get identical code icg+asm and icg+rtl.
     */
    case EQ:   dumpRR("cmove",  $src2->r, '$schar', $dst->rx, '$schar'); break;
    case NE:   dumpRR("cmovne", $src2->r, '$schar', $dst->rx, '$schar'); break;
    case GE:   dumpRR("cmovge", $src2->r, '$schar', $dst->rx, '$schar'); break;
    case GT:   dumpRR("cmovg",  $src2->r, '$schar', $dst->rx, '$schar'); break;
    case LE:   dumpRR("cmovle", $src2->r, '$schar', $dst->rx, '$schar'); break;
    case LT:   dumpRR("cmovl",  $src2->r, '$schar', $dst->rx, '$schar'); break;
    case GEU:  dumpRR("cmovae", $src2->r, '$schar', $dst->rx, '$schar'); break;
    case GTU:  dumpRR("cmova",  $src2->r, '$schar', $dst->rx, '$schar'); break;
    case LEU:  dumpRR("cmovbe", $src2->r, '$schar', $dst->rx, '$schar'); break;
    case LTU:  dumpRR("cmovb",  $src2->r, '$schar', $dst->rx, '$schar'); break;
    case UNLE: dumpRR("cmovbe", $src2->r, '$schar', $dst->rx, '$schar'); break;
    default:
      icg_nyi("unrecognized condition code %d for cmov", code);
    }
  },
  emit {
    rtx cond, src;
    rtx tsrc = gen_rtx_REG($rtx_mode, $src1->rx);
    rtx fsrc = gen_rtx_REG($rtx_mode, $src2->r);
    rtx dst = tsrc;
    const enum rtx_code code = $cond->code;
    switch (code) {
    case EQ:   cond = gen_rtx_EQ  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case NE:   cond = gen_rtx_NE  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GE:   cond = gen_rtx_GE  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GT:   cond = gen_rtx_GT  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LE:   cond = gen_rtx_LE  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LT:   cond = gen_rtx_LT  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GEU:  cond = gen_rtx_GEU (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GTU:  cond = gen_rtx_GTU (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LEU:  cond = gen_rtx_LEU (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LTU:  cond = gen_rtx_LTU (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNLE: cond = gen_rtx_UNLE(VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    default:
      icg_nyi("unrecognized condition code %d for cmov", code);
    }
    src = gen_rtx_IF_THEN_ELSE($rtx_mode, cond, tsrc, fsrc);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")


#
# TODO preston, extend these cases
# are there compare from memory for the integral types?

plug.plugrule3("cmovmem", [
  ["rule", "cost", "schar", "rtx_mode"],
  ["r64x.dst : COND_MOVE(condition.cond, PAIR_ALL(r64x.src1, MEM_DI(addr.src2)))", [4, 3], 'q', "DImode"],
  #
  # TODO: compare from memory with other data types; see above specification
  #
], """
  $rule $cost
  names {
    $dst->rx = $src1->rx;
    p->perm[0] = 1;
    p->perm[1] = 2;
    p->perm[2] = 0; /* ensuring that condition is evaluated last */
  },
  final {
    $dst->rx = $src1->rx;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_set_bit(live, rd);
    add_edges(rd, live);
    add_addr(live, $src2);
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($src1->rx);
  },
  debug {
    const enum rtx_code code = $cond->code;
    switch (code) {
    /*
     * See
     * NOTE A
     * above
     */
    case EQ:   dumpMR("cmove",  $src2, $dst->rx, '$schar'); break;
    case NE:   dumpMR("cmovne", $src2, $dst->rx, '$schar'); break;
    case GE:   dumpMR("cmovge", $src2, $dst->rx, '$schar'); break;
    case GT:   dumpMR("cmovg",  $src2, $dst->rx, '$schar'); break;
    case LE:   dumpMR("cmovle", $src2, $dst->rx, '$schar'); break;
    case LT:   dumpMR("cmovl",  $src2, $dst->rx, '$schar'); break;
    case GEU:  dumpMR("cmovae", $src2, $dst->rx, '$schar'); break;
    case GTU:  dumpMR("cmova",  $src2, $dst->rx, '$schar'); break;
    case LEU:  dumpMR("cmovbe", $src2, $dst->rx, '$schar'); break;
    case LTU:  dumpMR("cmovb",  $src2, $dst->rx, '$schar'); break;
    case UNLE: dumpMR("cmovbe",  $src2, $dst->rx, '$schar'); break;
    default:
      icg_nyi("unrecognized condition code %d for cmov", code);
    }
  },
  emit {
    rtx cond, src;
    rtx tsrc = gen_rtx_REG($rtx_mode, $src1->rx);
    rtx fsrc = gen_rtx_MEM($rtx_mode, $src2->rtl);
    rtx dst = tsrc;
    const enum rtx_code code = $cond->code;
    switch (code) {
    case EQ:   cond = gen_rtx_EQ  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case NE:   cond = gen_rtx_NE  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GE:   cond = gen_rtx_GE  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GT:   cond = gen_rtx_GT  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LE:   cond = gen_rtx_LE  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LT:   cond = gen_rtx_LT  (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GEU:  cond = gen_rtx_GEU (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GTU:  cond = gen_rtx_GTU (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LEU:  cond = gen_rtx_LEU (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LTU:  cond = gen_rtx_LTU (VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNLE: cond = gen_rtx_UNLE(VOIDmode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    default:
      icg_nyi("unrecognized condition code %d for cmov", code);
    }
    src = gen_rtx_IF_THEN_ELSE($rtx_mode, cond, tsrc, fsrc);
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")



#--------------------------------------------------------------
# conditional value

# implemented via SETcc

plug.plugrule3("setccreg", [
  ["rule", "cost", "schar", "rtx_mode"],
  ["r8x.dst : condition.cond", [1, 3], 'b', "QImode"],
], """
  $rule $cost
  names {
    $dst->rx = new_reg();
  },
  kinds {
    icg_reg_vector[$dst->rx].kind = INT_REGISTER;
  },
  build {
    unsigned rd = find($dst->rx);
    sparseset_clear_bit(live, rd);
    add_edges(rd, live);
  },
  remat {
    flags = 0;
  },
  final {
    $dst->rx = icg_reg_vector[find($dst->rx)].color;
  },
  debug {
    const enum rtx_code code = $cond->code;
    const enum machine_mode cc_code = $cond->cc_code;
    switch (code) {
    case EQ:  dumpR("sete",  $dst->rx, '$schar'); break;
    case NE:  dumpR("setne", $dst->rx, '$schar'); break;
    case GE:  dumpR(cc_code == CCFPUmode ? "setae" : "setge", $dst->rx, '$schar'); break;
    case GT:  dumpR(cc_code == CCFPUmode ? "seta"  : "setg",  $dst->rx, '$schar'); break;
    case LE:  dumpR(cc_code == CCFPUmode ? "setbe" : "setle", $dst->rx, '$schar'); break;
    case LT:  dumpR(cc_code == CCFPUmode ? "setb"  : "setl",  $dst->rx, '$schar'); break;

    case GEU: dumpR("setae", $dst->rx, '$schar'); break;
    case GTU: dumpR("seta",  $dst->rx, '$schar'); break;
    case LEU: dumpR("setbe", $dst->rx, '$schar'); break;
    case LTU: dumpR("setb",  $dst->rx, '$schar'); break;

    /*
     * UNEQ (unordered ==) and LTGT (unordered !=) are problematic
     * LTGT in flow context will generate 2 conditional jumps,
     * one for jne and the other jp (jump unordered)
     * Empirically, when we see LTGT in value context it
     * is as a child of an IOR, where the other child
     * is a test of UNORDERED, so we can just handle the jne case here.
     */
    case UNEQ:      dumpR("sete",  $dst->rx, '$schar'); break;
    case LTGT:      dumpR("setne", $dst->rx, '$schar'); break;

    case UNGE:      dumpR("setae", $dst->rx, '$schar'); break;
    case UNGT:      dumpR("seta",  $dst->rx, '$schar'); break;
    case UNLE:      dumpR("setbe", $dst->rx, '$schar'); break;
    case UNLT:      dumpR("setb",  $dst->rx, '$schar'); break;
    case UNORDERED: dumpR("setp",  $dst->rx, '$schar'); break;
    case ORDERED:   dumpR("setnp", $dst->rx, '$schar'); break;

    default:
      icg_nyi("unrecognized condition code %d for set", code);
    }
  },
  emit {
    rtx src;
    rtx dst = gen_rtx_REG($rtx_mode, $dst->rx);
    const enum rtx_code code = $cond->code;
    switch (code) {
    case EQ:  src = gen_rtx_EQ ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case NE:  src = gen_rtx_NE ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GE:  src = gen_rtx_GE ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GT:  src = gen_rtx_GT ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LE:  src = gen_rtx_LE ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LT:  src = gen_rtx_LT ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;

    case GEU: src = gen_rtx_GEU($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GTU: src = gen_rtx_GTU($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LEU: src = gen_rtx_LEU($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LTU: src = gen_rtx_LTU($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;

    case UNEQ: src = gen_rtx_UNEQ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LTGT: src = gen_rtx_LTGT($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNGE: src = gen_rtx_UNGE($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNGT: src = gen_rtx_UNGT($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNLE: src = gen_rtx_UNLE($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNLT: src = gen_rtx_UNLT($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNORDERED: src = gen_rtx_UNORDERED($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case   ORDERED: src =   gen_rtx_ORDERED($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;

    default:
      icg_nyi("unrecognized condition code %d for set", code);
    }
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")

plug.plugrule3("setccmem", [
  ["rule", "cost", "schar", "rtx_mode"],
  ["stmt : SET_ALL(MEM_QI(addr.dst), condition.cond)", [3, 2], 'b', "QImode"],
], """
  $rule $cost
  build {
    add_addr(live, $dst);
  },
  remat {
    flags = 0;
  },
  debug {
    const enum rtx_code code = $cond->code;
    const enum machine_mode cc_code = $cond->cc_code;
    switch (code) {
    case EQ:  dumpM("sete",  $dst); break;
    case NE:  dumpM("setne", $dst); break;
    case GE:  dumpM(cc_code == CCFPUmode ? "setae" : "setge", $dst); break;
    case GT:  dumpM(cc_code == CCFPUmode ? "seta"  : "setg",  $dst); break;
    case LE:  dumpM(cc_code == CCFPUmode ? "setbe" : "setle", $dst); break;
    case LT:  dumpM(cc_code == CCFPUmode ? "setb"  : "setl",  $dst); break;

    case GEU: dumpM("setae", $dst); break;
    case GTU: dumpM("seta",  $dst); break;
    case LEU: dumpM("setbe", $dst); break;
    case LTU: dumpM("setb",  $dst); break;

    case UNEQ:      dumpM("sete",  $dst); break;
    case LTGT:      dumpM("setne", $dst); break;
    case UNGE:      dumpM("setae", $dst); break;
    case UNGT:      dumpM("seta",  $dst); break;
    case UNLE:      dumpM("setbe", $dst); break;
    case UNLT:      dumpM("setb",  $dst); break;
    case UNORDERED: dumpM("setp",  $dst); break;
    case ORDERED:   dumpM("setnp", $dst); break;

    default:
      icg_nyi("unrecognized condition code %d for set", code);
    }
  },
  emit {
    rtx src;
    rtx dst = gen_rtx_MEM($rtx_mode, $dst->rtl);
    const enum rtx_code code = $cond->code;
    switch (code) {
    case EQ:  src = gen_rtx_EQ ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case NE:  src = gen_rtx_NE ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GE:  src = gen_rtx_GE ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GT:  src = gen_rtx_GT ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LE:  src = gen_rtx_LE ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LT:  src = gen_rtx_LT ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;

    case GEU: src = gen_rtx_GEU($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case GTU: src = gen_rtx_GTU($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LEU: src = gen_rtx_LEU($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LTU: src = gen_rtx_LTU($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;

    case UNEQ: src = gen_rtx_UNEQ($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case LTGT: src = gen_rtx_LTGT($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNGE: src = gen_rtx_UNGE($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNGT: src = gen_rtx_UNGT($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNLE: src = gen_rtx_UNLE($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNLT: src = gen_rtx_UNLT($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case UNORDERED: src = gen_rtx_UNORDERED($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;
    case   ORDERED: src =   gen_rtx_ORDERED($rtx_mode, gen_rtx_REG($cond->cc_code, REG_CC), GEN_INT(0)); break;

    default:
      icg_nyi("unrecognized condition code %d for set", code);
    }
    icg_emit_plain(gen_rtx_SET(VOIDmode, dst, src));
  };
""")




#--------------------------------------------------------------
# a peephole optimization involving the carry bit
# somebody upstream of us is finding opportunities to conditionally modify a variable by 1
# by using sbb (sub borrow) or adc (add carry)
#

plug.plugrule3("sbb", [
  ["rule", "cost", "op", "mode", "schar", "bias"],

  ["r64x.dst : PLUS_DI(r64x.src | NEG_DI(PLUS_DI(LTU_ALL(rcc.cc, CONST_0), CONST_N1)))", [1, 4], "sbbq", "DImode", 'q', -1],
  ["r32x.dst : PLUS_SI(r32x.src | NEG_SI(PLUS_SI(LTU_ALL(rcc.cc, CONST_0), CONST_N1)))", [1, 3], "sbbl", "SImode", 'd', -1],
  ["r16x.dst : PLUS_HI(r16x.src | NEG_HI(PLUS_HI(LTU_ALL(rcc.cc, CONST_0), CONST_N1)))", [1, 4], "sbbw", "HImode", 'w', -1],
  ["r8x.dst  : PLUS_QI(r8x.src  | NEG_QI(PLUS_QI(LTU_ALL(rcc.cc, CONST_0), CONST_N1)))", [1, 4], "sbbb", "QImode", 'b', -1],

  ["r64x.dst : PLUS_DI(r64x.src | NEG_DI(PLUS_DI(LTU_ALL(rcc.cc, CONST_0),  CONST_0)))", [1, 4], "sbbq", "DImode", 'q',  0],
  ["r32x.dst : PLUS_SI(r32x.src | NEG_SI(PLUS_SI(LTU_ALL(rcc.cc, CONST_0),  CONST_0)))", [1, 3], "sbbl", "SImode", 'd',  0],
  ["r16x.dst : PLUS_HI(r16x.src | NEG_HI(PLUS_HI(LTU_ALL(rcc.cc, CONST_0),  CONST_0)))", [1, 4], "sbbw", "HImode", 'w',  0],
  ["r8x.dst  : PLUS_QI(r8x.src  | NEG_QI(PLUS_QI(LTU_ALL(rcc.cc, CONST_0),  CONST_0)))", [1, 4], "sbbb", "QImode", 'b',  0],

], """
  $rule $cost
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
  remat {
    flags = 0;
  },
  costs {
    forgettable($src->rx);
  },
  debug {
    dumpIRm1("$op", $dst->rx, '$schar');
  },
  emit {
    rtx ltu = gen_rtx_LTU($mode, gen_rtx_REG(CCmode, REG_CC), GEN_INT(0));
    rtx plus = gen_rtx_PLUS($mode, ltu, GEN_INT($bias));
    rtx minus = gen_rtx_MINUS($mode, gen_rtx_REG($mode, $src->rx), plus);
    rtx dst = gen_rtx_REG($mode, $dst->rx);
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, minus));
  };
""")

plug.plugrule3("adc", [
  ["rule", "cost", "op", "mode", "schar", "bias"],

  ["r64x.dst : PLUS_DI(r64x.src | LTU_ALL(rcc.cc, CONST_0) |  CONST_0)", [1, 4], "adcq", "DImode", 'q',  0],
  ["r32x.dst : PLUS_SI(r32x.src | LTU_ALL(rcc.cc, CONST_0) |  CONST_0)", [1, 3], "adcl", "SImode", 'd',  0],
  ["r16x.dst : PLUS_HI(r16x.src | LTU_ALL(rcc.cc, CONST_0) |  CONST_0)", [1, 4], "adcw", "HImode", 'w',  0],
  ["r8x.dst  : PLUS_QI(r8x.src  | LTU_ALL(rcc.cc, CONST_0) |  CONST_0)", [1, 4], "adcb", "QImode", 'b',  0],

  ["r64x.dst : PLUS_DI(r64x.src | LTU_ALL(rcc.cc, CONST_0) | CONST_N1)", [1, 4], "adcq", "DImode", 'q', -1],
  ["r32x.dst : PLUS_SI(r32x.src | LTU_ALL(rcc.cc, CONST_0) | CONST_N1)", [1, 3], "adcl", "SImode", 'd', -1],
  ["r16x.dst : PLUS_HI(r16x.src | LTU_ALL(rcc.cc, CONST_0) | CONST_N1)", [1, 4], "adcw", "HImode", 'w', -1],
  ["r8x.dst  : PLUS_QI(r8x.src  | LTU_ALL(rcc.cc, CONST_0) | CONST_N1)", [1, 4], "adcb", "QImode", 'b', -1],

], """
  $rule $cost
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
  remat {
    flags = 0;
  },
  costs {
    forgettable($src->rx);
  },
  debug {
    dumpIRm1("$op", $dst->rx, '$schar');
  },
  emit {
    /* careflly reassemble a tree canonicalized for what the downstream ICG+RTL wants */
    rtx ltu = gen_rtx_LTU($mode, gen_rtx_REG(CCmode, REG_CC), GEN_INT(0));
    rtx plus0 = gen_rtx_PLUS($mode, ltu, gen_rtx_REG($mode, $src->rx));
    rtx plus1 = gen_rtx_PLUS($mode, plus0, GEN_INT($bias));
    rtx dst = gen_rtx_REG($mode, $dst->rx);
    icg_emit_clobber(gen_rtx_SET(VOIDmode, dst, plus1));
  };
""")


#--------------------------------------------------------------
# conditions

# set attribute indicating kind of condition

plug.plugrule3("condition", [
  ["rule", "cost", "op", "cc_mode"],

  #
  # ordered signed and unsigned comparisions
  #
  ["condition.dst : EQ_ALL ( rcc.src, CONST_0)", [0, 0], "EQ",  "CCmode"],
  ["condition.dst : NE_ALL ( rcc.src, CONST_0)", [0, 0], "NE",  "CCmode"],
  ["condition.dst : GE_ALL ( rcc.src, CONST_0)", [0, 0], "GE",  "CCmode"],
  ["condition.dst : GT_ALL ( rcc.src, CONST_0)", [0, 0], "GT",  "CCmode"],
  ["condition.dst : LE_ALL ( rcc.src, CONST_0)", [0, 0], "LE",  "CCmode"],
  ["condition.dst : LT_ALL ( rcc.src, CONST_0)", [0, 0], "LT",  "CCmode"],
  ["condition.dst : GEU_ALL( rcc.src, CONST_0)", [0, 0], "GEU", "CCmode"],  # unsigned
  ["condition.dst : GTU_ALL( rcc.src, CONST_0)", [0, 0], "GTU", "CCmode"],  # unsigned
  ["condition.dst : LEU_ALL( rcc.src, CONST_0)", [0, 0], "LEU", "CCmode"],  # unsigned
  ["condition.dst : LTU_ALL( rcc.src, CONST_0)", [0, 0], "LTU", "CCmode"],  # unsigned

  #
  # ordered floating point comparisions
  # Watchout: this is a tarpit.
  # These rule is here by analog with the signed/unsigned cases.
  # These may never happen in practice.
  # We have seen:
  #   COND_MOVE((NE_ALL REG_SF REG_SF) (PAIR_ALL LABEL PC))
  # to implement conditional branches based on floating point compares.

  #
  # under what circumstances do we put the "UN" (unordered) modifier on the requested test?
  #
  ["condition.dst : EQ_ALL(urcc.src, CONST_0)", [0, 0], "EQ",  "CCFPUmode"],
  ["condition.dst : NE_ALL(urcc.src, CONST_0)", [0, 0], "NE",  "CCFPUmode"],
  ["condition.dst : GE_ALL(urcc.src, CONST_0)", [0, 0], "GE",  "CCFPUmode"],
  ["condition.dst : GT_ALL(urcc.src, CONST_0)", [0, 0], "GT",  "CCFPUmode"],
  ["condition.dst : LE_ALL(urcc.src, CONST_0)", [0, 0], "LE",  "CCFPUmode"],
  ["condition.dst : LT_ALL(urcc.src, CONST_0)", [0, 0], "LT",  "CCFPUmode"],

  #
  # TODO: these unordered floating comparisions not fully flensed out.
  # Watchout: this is a tarpit.
  #
  ["condition.dst :      UNEQ_ALL(urcc.src, CONST_0)", [0, 0], "UNEQ", "CCFPUmode"],  # unordered FP
  ["condition.dst :      UNGE_ALL(urcc.src, CONST_0)", [0, 0], "UNGE", "CCFPUmode"],  # unordered FP
  ["condition.dst :      UNGT_ALL(urcc.src, CONST_0)", [0, 0], "UNGT", "CCFPUmode"],  # unordered FP
  ["condition.dst :      UNLE_ALL(urcc.src, CONST_0)", [0, 0], "UNLE", "CCFPUmode"],  # unordered FP
  ["condition.dst :      UNLT_ALL(urcc.src, CONST_0)", [0, 0], "UNLT", "CCFPUmode"],  # unordered FP
  ["condition.dst :      LTGT_ALL(urcc.src, CONST_0)", [0, 0], "LTGT", "CCFPUmode"],  # unordered FP
  ["condition.dst : UNORDERED_ALL(urcc.src, CONST_0)", [0, 0], "UNORDERED", "CCFPUmode"],  # unordered FP
  ["condition.dst :   ORDERED_ALL(urcc.src, CONST_0)", [0, 0], "ORDERED", "CCFPUmode"],  # unordered FP

], """
  $rule $cost
  names {
    $dst->code = $op;
    $dst->cc_code = $cc_mode;
  };
""")

#--------------------------------------------------------------
# condition codes

# The next two sets of special rules
# are used to overcome some weaknesses in gcc.
#
# The 1st problem is that gcc sometimes provides us with dead code
# of the form (SET_ALL (REGX_DI r17) (COMPARE_CC ...))
# Later passes of gcc will clean it up, though it'd be better if
# if it were removed before allocation.
# Keeping the first set of rules allows the dead code to be
# recognized by the grammar.
#
# The 2nd problem is that we don't know how to provoke gcc into
# generating conditional branches.  Therefore, we sometimes see
# things like
#
# insn 7: SET_ALL
#            REGX_DI:17[flags]
#            COMPARE_CC
#                REG_SI:60
#                REG_SI:61
# insn 8: SET_ALL
#            PC_ALL
#            COND_MOVE
#                GE_ALL
#                    REGX_DI:17[flags]
#                    CONST_0:0
#                PAIR_ALL
#                    LABEL_REF_DI
#                    PC_ALL
#
# Someday, it'd be great to have the COMPARE forward
# propagated into the conditional branch.  For today,
# we leave it alone.

plug.plugrule3("stmtcc", [
  ["rule", "cost"],
  #
  # The high cost on these rules is to prevent these from
  # being used unless there's no alternative.
  #
  ["stmt : SET_ALL(REGCCX_DI.dst, rcc.src)",  [20, 20]],
  ["stmt : SET_ALL(REGCCFPUX_DI.dst, urcc.src)", [20, 20]],
], """
  $rule $cost
  ;
""")


plug.plugrule3("rccr64", [
  ["rule", "cost"],
  #
  # TODO: what does this/these rule really mean?
  # Apparently the flags register comes to us as the terminal symbol
  #  REGX_DI:17[flags] the reduction chain REGX_DI:17[flags] --> r64
  #
  ["rcc.dst  : REGCCX_DI", [0, 0]],
  ["rcc.dst  : REGCC_DI", [0, 0]],	# TODO: perhaps (needed for some -O3 code)
  #
  # By analogy with the previous rule,
  # It is tempting to allow r64 to reduce to a urcc,
  # but that is not correct, as a computation of a r64
  # doesn't implicitly do any unordered floating point comparisons;
  # all floating point compares must be done explicitly.
  #
  ["urcc.dst  : REGCCFPUX_DI", [0, 0]],
  ["urcc.dst  : REGCCFPU_DI", [0, 0]],	# TODO: perhaps (needed for some -O3 code)
], """
  $rule $cost
  costs {
    /*forgettable($src->rx);*/
  };
""")

# End of sets of special rules




plug.plugrule3("rcc_compare0", [
  ["rule", "cost"],
  ["rcc.dst  : COMPARE_CC(ccr64x,  CONST_0)", [0, 0]],
  ["rcc.dst  : COMPARE_CC(ccr32x,  CONST_0)", [0, 0]],
  ["rcc.dst  : COMPARE_CC(ccr16x,  CONST_0)", [0, 0]],
  ["rcc.dst  : COMPARE_CC(ccr8x,   CONST_0)", [0, 0]],
], """
  $rule $cost
  debug {
    /* fprintf(dump_file, "; IMPLICIT USE OF PREVIOUSLY SET CONDITION CODE\\n"); */
  };
""")

plug.plugrule3("urcc_comparereg", [
  ["rule", "cost", "opcode", "schar", "cc_mode", "rtx_mode", "is_test_variant"],

  ["urcc.dst : COMPARE_CC(rd.src1, rd.src2)",   [1, 3], "ucomisd", 'x', "CCFPUmode", "DFmode", 0],
  ["urcc.dst : COMPARE_CC(rf.src1, rf.src2)",   [1, 3], "ucomiss", 'x', "CCFPUmode", "SFmode", 0],

  ["rcc.dst  : COMPARE_CC(r64.src1, r64.src2)", [1, 3], "cmpq",    'q', "CCmode",    "DImode", 0],
  ["rcc.dst  : COMPARE_CC(r32.src1, r32.src2)", [1, 2], "cmpl",    'd', "CCmode",    "SImode", 0],
  ["rcc.dst  : COMPARE_CC(r16.src1, r16.src2)", [1, 2], "cmpw",    'w', "CCmode",    "HImode", 0],
  ["rcc.dst  : COMPARE_CC(r8.src1, r8.src2)",   [1, 2], "cmpb",    'b', "CCmode",    "QImode", 0],

  #
  # I don't understand why this has to be CCZmode, except that's what the stock gcc RTL uses.
  #
  ["rcc.dst  : COMPARE_CC(AND_DI(r64.src1, r64.src2), CONST_0)", [1, 3], "testq", 'q', "CCZmode", "DImode", 1],
  ["rcc.dst  : COMPARE_CC(AND_SI(r32.src1, r32.src2), CONST_0)", [1, 2], "testq", 'd', "CCZmode", "SImode", 1],
  ["rcc.dst  : COMPARE_CC(AND_HI(r16.src1, r16.src2), CONST_0)", [1, 2], "testq", 'w', "CCZmode", "HImode", 1],
  ["rcc.dst  : COMPARE_CC(AND_QI(r8.src1,  r8.src2),  CONST_0)", [1, 2], "testq", 'b', "CCZmode", "QImode", 1],

], """
  $rule $cost
  build {
    sparseset_set_bit(live, find($src1->r));
    sparseset_set_bit(live, find($src2->r));
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($src1->r);
    memorable($src2->r);
  },
  debug {
    dumpRR("$opcode", $src2->r, '$schar', $src1->r, '$schar');
  },
  emit {
    emit_insn(gen_rtx_SET(VOIDmode,
      gen_rtx_REG($cc_mode, REG_CC),
      gen_rtx_COMPARE_assist($is_test_variant, $cc_mode, $rtx_mode,
	gen_rtx_REG($rtx_mode, $src1->r),
	gen_rtx_REG($rtx_mode, $src2->r))));
  };
""")

plug.plugrule3("rcc_compare_regimm", [
  ["rule", "cost", "opcode", "schar", "cc_mode", "rtx_mode", "is_test_variant"],

  ["rcc.dst : COMPARE_CC(r64.src1, imm32.src2)", [1, 3], "cmpq", 'q', "CCmode", "DImode", 0],
  ["rcc.dst : COMPARE_CC(r64.src1, imm8.src2)",  [1, 3], "cmpq", 'q', "CCmode", "DImode", 0],
  ["rcc.dst : COMPARE_CC(r32.src1, imm32.src2)", [1, 2], "cmpl", 'd', "CCmode", "SImode", 0],
  ["rcc.dst : COMPARE_CC(r32.src1, imm8.src2)",  [1, 2], "cmpl", 'd', "CCmode", "SImode", 0],
  ["rcc.dst : COMPARE_CC(r16.src1, imm16.src2)", [1, 3], "cmpw", 'w', "CCmode", "HImode", 0],
  ["rcc.dst : COMPARE_CC(r16.src1, imm8.src2)",  [1, 3], "cmpw", 'w', "CCmode", "HImode", 0],
  ["rcc.dst : COMPARE_CC(r8.src1,  imm8.src2)",  [1, 2], "cmpb", 'b', "CCmode", "QImode", 0],

  #
  # I don't understand why this has to be CCZmode, except that's what the stock gcc RTL uses.
  #
  ["rcc.dst : COMPARE_CC(AND_DI(r64.src1, imm32.src2), CONST_0)", [1, 3], "testq", 'q', "CCZmode", "DImode", 1],
  ["rcc.dst : COMPARE_CC(AND_SI(r32.src1, imm32.src2), CONST_0)", [1, 2], "testl", 'd', "CCZmode", "SImode", 1],
  ["rcc.dst : COMPARE_CC(AND_HI(r16.src1, imm16.src2), CONST_0)", [1, 3], "testw", 'w', "CCZmode", "HImode", 1],
  ["rcc.dst : COMPARE_CC(AND_QI(r8.src1,  imm8.src2), CONST_0)",  [1, 2], "testb", 'b', "CCZmode", "QImode", 1],

], """
  $rule $cost
  build {
    sparseset_set_bit(live, find($src1->r));
  },
  remat {
    flags = 0;
  },
  costs {
    memorable($src1->r);
  },
  debug {
    dumpIR("$opcode", $src2, $src1->r, '$schar');
  },
  emit {
    emit_insn(gen_rtx_SET(VOIDmode,
			  gen_rtx_REG($cc_mode, REG_CC),
			  gen_rtx_COMPARE_assist($is_test_variant, $cc_mode, $rtx_mode,
					  gen_rtx_REG($rtx_mode, $src1->r),
					  GEN_INT($src2->val))));
  };
""")




# we can save a byte by using "test r,r" in place of "cmp $0,r"

plug.plugrule3("rcc_compare_reg_zero", [
  ["rule", "cost", "opcode", "schar", "rtx_mode"],

  ["rcc.dst : COMPARE_CC(r64.src1, CONST_0)",  [1, 3], "testq", 'q', "DImode"],
  ["rcc.dst : COMPARE_CC(r32.src1, CONST_0)",  [1, 2], "testl", 'd', "SImode"],
  ["rcc.dst : COMPARE_CC(r16.src1, CONST_0)",  [1, 3], "testw", 'w', "HImode"],
  ["rcc.dst : COMPARE_CC(r8.src1,  CONST_0)",  [1, 2], "testb", 'b', "QImode"],

], """
  $rule $cost
  build {
    sparseset_set_bit(live, find($src1->r));
  },
  remat {
    flags = 0;
  },
  costs {
    memorable($src1->r);
  },
  debug {
    dumpRR("$opcode", $src1->r, '$schar', $src1->r, '$schar');
  },
  emit {
    emit_insn(gen_rtx_SET(VOIDmode,
			  gen_rtx_REG(CCNOmode, REG_CC),
			  gen_rtx_COMPARE(CCNOmode,
                                          gen_rtx_REG($rtx_mode, $src1->r),
                                          GEN_INT(0))));
  };
""")





plug.plugrule3("rcc_compare_memimm", [
  ["rule", "cost", "opcode", "schar", "cc_mode", "rtx_mode", "is_test_variant"],

  ["rcc.dst : COMPARE_CC(MEM_DI(addr.src1), imm32.src2)", [4, 3], "cmpq", 'q', "CCmode", "DImode", 0],
  ["rcc.dst : COMPARE_CC(MEM_DI(addr.src1), imm8.src2)",  [4, 3], "cmpq", 'q', "CCmode", "DImode", 0],
  ["rcc.dst : COMPARE_CC(MEM_SI(addr.src1), imm32.src2)", [4, 2], "cmpl", 'l', "CCmode", "SImode", 0],
  ["rcc.dst : COMPARE_CC(MEM_SI(addr.src1), imm8.src2)",  [4, 2], "cmpl", 'l', "CCmode", "SImode", 0],
  ["rcc.dst : COMPARE_CC(MEM_HI(addr.src1), imm16.src2)", [4, 3], "cmpw", 'w', "CCmode", "HImode", 0],
  ["rcc.dst : COMPARE_CC(MEM_HI(addr.src1), imm8.src2)",  [4, 3], "cmpw", 'w', "CCmode", "HImode", 0],
  ["rcc.dst : COMPARE_CC(MEM_QI(addr.src1), imm8.src2)",  [4, 2], "cmpb", 'b', "CCmode", "QImode", 0],

  #
  # I don't understand why this has to be CCZmode, except that's what the stock gcc RTL uses.
  #
  ["rcc.dst : COMPARE_CC(AND_DI(MEM_DI(addr.src1), imm32.src2), CONST_0)", [4, 3], "testq", 'q', "CCZmode", "DImode", 0],
  ["rcc.dst : COMPARE_CC(AND_SI(MEM_SI(addr.src1), imm32.src2), CONST_0)", [4, 2], "testl", 'l', "CCZmode", "SImode", 0],
  ["rcc.dst : COMPARE_CC(AND_HI(MEM_HI(addr.src1), imm16.src2), CONST_0)", [4, 3], "testw", 'w', "CCZmode", "HImode", 0],
  ["rcc.dst : COMPARE_CC(AND_QI(MEM_QI(addr.src1), imm8.src2),  CONST_0)", [4, 2], "testb", 'b', "CCZmode", "QImode", 0],

], """
  $rule $cost
  build {
    add_addr(live, $src1);
  },
  remat {
    flags = 0;
  },
  debug {
    dumpIM("$opcode", $src2, $src1);
  },
  emit {
    emit_insn(gen_rtx_SET(VOIDmode,
			  gen_rtx_REG($cc_mode, REG_CC),
			  gen_rtx_COMPARE_assist($is_test_variant, $cc_mode, $rtx_mode,
					  gen_rtx_MEM($rtx_mode, $src1->rtl),
					  GEN_INT($src2->val))));
  };
""")

plug.plugrule3("rcc_compare_regmem", [
  ["rule", "cost", "opcode", "schar", "cc_mode", "rtx_mode", "is_test_variant"],

  ["urcc.dst : COMPARE_CC(rd.src,  MEM_DF(addr.mem))", [4, 2], "ucomisd", 'x', "CCFPUmode", "DFmode", 0],
  ["urcc.dst : COMPARE_CC(rf.src,  MEM_SF(addr.mem))", [4, 2], "ucomiss", 'x', "CCFPUmode", "SFmode", 0],

  ["rcc.dst :  COMPARE_CC(r64.src, MEM_DI(addr.mem))", [4, 2], "cmpq",    'q', "CCmode",    "DImode", 0],
  ["rcc.dst :  COMPARE_CC(r32.src, MEM_SI(addr.mem))", [4, 1], "cmpl",    'd', "CCmode",    "SImode", 0],
  ["rcc.dst :  COMPARE_CC(r16.src, MEM_HI(addr.mem))", [4, 2], "cmpw",    'w', "CCmode",    "HImode", 0],
  ["rcc.dst :  COMPARE_CC(r8.src,  MEM_QI(addr.mem))", [4, 1], "cmpb",    'b', "CCmode",    "QImode", 0],

], """
  $rule $cost
  build {
    sparseset_set_bit(live, find($src->r));
    add_addr(live, $mem);
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($src->r);
  },
  debug {
    dumpMR("$opcode", $mem, $src->r, '$schar');
  },
  emit {
    emit_insn(gen_rtx_SET(VOIDmode,
			  gen_rtx_REG($cc_mode, REG_CC),
			  gen_rtx_COMPARE_assist($is_test_variant, $cc_mode, $rtx_mode,
					  gen_rtx_REG($rtx_mode, $src->r),
					  gen_rtx_MEM($rtx_mode, $mem->rtl))));
  };
""")

plug.plugrule3("rcc_compare_memreg", [
  ["rule", "cost", "opcode", "schar", "cc_mode", "rtx_mode", "is_test_variant"],

  ["rcc.dst : COMPARE_CC(MEM_DI(addr.mem), r64.src)", [4, 2], "cmpq", 'q', "CCmode", "DImode", 0],
  ["rcc.dst : COMPARE_CC(MEM_SI(addr.mem), r32.src)", [4, 1], "cmpl", 'd', "CCmode", "SImode", 0],
  ["rcc.dst : COMPARE_CC(MEM_HI(addr.mem), r16.src)", [4, 2], "cmpw", 'w', "CCmode", "HImode", 0],
  ["rcc.dst : COMPARE_CC(MEM_QI(addr.mem),  r8.src)", [4, 1], "cmpb", 'b', "CCmode", "QImode", 0],

  #
  # I don't understand why this has to be CCZmode, except that's what the stock gcc RTL uses.
  #
  ["rcc.dst : COMPARE_CC(AND_DI(MEM_DI(addr.mem), r64.src), CONST_0)", [4, 2], "testq", 'q', "CCZmode", "DImode", 1],
  ["rcc.dst : COMPARE_CC(AND_SI(MEM_SI(addr.mem), r32.src), CONST_0)", [4, 1], "testl", 'd', "CCZmode", "SImode", 1],
  ["rcc.dst : COMPARE_CC(AND_HI(MEM_HI(addr.mem), r16.src), CONST_0)", [4, 2], "testw", 'w', "CCZmode", "HImode", 1],
  ["rcc.dst : COMPARE_CC(AND_QI(MEM_QI(addr.mem),  r8.src), CONST_0)", [4, 1], "testb", 'b', "CCZmode", "QImode", 1],

], """
  $rule $cost

  build {
    sparseset_set_bit(live, find($src->r));
    add_addr(live, $mem);
  },
  remat {
    flags = 0;
  },
  costs {
    forgettable($src->r);
  },
  debug {
    dumpRM("$opcode", $src->r, '$schar', $mem);
  },
  emit {
    emit_insn(gen_rtx_SET(VOIDmode,
			  gen_rtx_REG($cc_mode, REG_CC),
			  gen_rtx_COMPARE_assist($is_test_variant, $cc_mode, $rtx_mode,
					  gen_rtx_MEM($rtx_mode, $mem->rtl),
					  gen_rtx_REG($rtx_mode, $src->r))));
  };
""")



#
# calls
# Calls always call to a byte address, hence their left child is ALWAYS a MEM_QI.
#
#   addrtype == 0  ==> function address held in register (general indirect call)
#   addrtype == 1  ==> function address held in memory   (simple indirect call)
#   addrtype == 2  ==> function address is literal in istream (normal case)
#
#   valuetype == 0 ==>     no return value
#   valuetype == 1 ==> scalar return value
#   valuetype == 2 ==> struct return value
#
#
plug.plugrule3("callvalueRM", [
  ["rule", "cost", "rtx_mode", "addrtype", "valuetype"],

  #
  # These CALL types occur when a scalar is returned eg, valuetype==1.
  #!
  ["stmt : SET_ALL(r64.dst,      CALL_ALL(PAIR_ALL(MEM_QI(r64.addr),          imm64.const), use_list))", [0, 0], "DImode", 0, 1],
  ["stmt : SET_ALL(r64.dst,      CALL_ALL(PAIR_ALL(MEM_QI(MEM_DI(addr.addr)), imm64.const), use_list))", [0, 0], "DImode", 1, 1],
  ["stmt : SET_ALL(r64.dst,      CALL_ALL(PAIR_ALL(MEM_QI(symbol.addr),       imm64.const), use_list))", [0, 0], "DImode", 2, 1],

  ["stmt : SET_ALL(r32.dst,      CALL_ALL(PAIR_ALL(MEM_QI(r64.addr),          imm64.const), use_list))", [0, 0], "DImode", 0, 1],
  ["stmt : SET_ALL(r32.dst,      CALL_ALL(PAIR_ALL(MEM_QI(MEM_DI(addr.addr)), imm64.const), use_list))", [0, 0], "DImode", 1, 1],
  ["stmt : SET_ALL(r32.dst,      CALL_ALL(PAIR_ALL(MEM_QI(symbol.addr),       imm64.const), use_list))", [0, 0], "DImode", 2, 1],

  ["stmt : SET_ALL(r16.dst,      CALL_ALL(PAIR_ALL(MEM_QI(r64.addr),          imm64.const), use_list))", [0, 0], "DImode", 0, 1],
  ["stmt : SET_ALL(r16.dst,      CALL_ALL(PAIR_ALL(MEM_QI(MEM_DI(addr.addr)), imm64.const), use_list))", [0, 0], "DImode", 1, 1],
  ["stmt : SET_ALL(r16.dst,      CALL_ALL(PAIR_ALL(MEM_QI(symbol.addr),       imm64.const), use_list))", [0, 0], "DImode", 2, 1],

  ["stmt : SET_ALL(r8.dst,       CALL_ALL(PAIR_ALL(MEM_QI(r64.addr),          imm64.const), use_list))", [0, 0], "DImode", 0, 1],
  ["stmt : SET_ALL(r8.dst,       CALL_ALL(PAIR_ALL(MEM_QI(MEM_DI(addr.addr)), imm64.const), use_list))", [0, 0], "DImode", 1, 1],
  ["stmt : SET_ALL(r8.dst,       CALL_ALL(PAIR_ALL(MEM_QI(symbol.addr),       imm64.const), use_list))", [0, 0], "DImode", 2, 1],

  ["stmt : SET_ALL(rf.dst,       CALL_ALL(PAIR_ALL(MEM_QI(r64.addr),          imm64.const), use_list))", [0, 0], "SFmode", 0, 1],
  ["stmt : SET_ALL(rf.dst,       CALL_ALL(PAIR_ALL(MEM_QI(MEM_DI(addr.addr)), imm64.const), use_list))", [0, 0], "SFmode", 1, 1],
  ["stmt : SET_ALL(rf.dst,       CALL_ALL(PAIR_ALL(MEM_QI(symbol.addr),       imm64.const), use_list))", [0, 0], "DFmode", 2, 1],

  ["stmt : SET_ALL(rd.dst,       CALL_ALL(PAIR_ALL(MEM_QI(r64.addr),          imm64.const), use_list))", [0, 0], "DFmode", 0, 1],
  ["stmt : SET_ALL(rd.dst,       CALL_ALL(PAIR_ALL(MEM_QI(MEM_DI(addr.addr)), imm64.const), use_list))", [0, 0], "DFmode", 1, 1],
  ["stmt : SET_ALL(rd.dst,       CALL_ALL(PAIR_ALL(MEM_QI(symbol.addr),       imm64.const), use_list))", [0, 0], "SFmode", 2, 1],

  #
  # The CALL form occurs when returning a structure, eg valuetype==2.
  # This implementation is potentially fragile because it relies on the set
  # destination which is a PARALLEL of expr_lists (register defs)
  # to be exactly what is desired at the end.
  # Likely it is OK because we would never be changing what registers the CALL defs.
  #
  ["stmt : SET_ALL(def_list.dst, CALL_ALL(PAIR_ALL(MEM_QI(r64.addr),          imm64.const), use_list))", [0, 0], "DFmode", 0, 2],
  ["stmt : SET_ALL(def_list.dst, CALL_ALL(PAIR_ALL(MEM_QI(MEM_DI(addr.addr)), imm64.const), use_list))", [0, 0], "DFmode", 1, 2],
  ["stmt : SET_ALL(def_list.dst, CALL_ALL(PAIR_ALL(MEM_QI(symbol.addr),       imm64.const), use_list))", [0, 0], "SFmode", 2, 2],

  #
  # This CALL form happens when nothing is returned, eg valuetype==0.
  # There is no $dst to reference.
  #
  ["stmt :                       CALL_ALL(PAIR_ALL(MEM_QI(r64.addr),          imm64.const), use_list)",  [0, 0], "DFmode", 0, 0],
  ["stmt :                       CALL_ALL(PAIR_ALL(MEM_QI(MEM_DI(addr.addr)), imm64.const), use_list)",  [0, 0], "DFmode", 1, 0],
  ["stmt :                       CALL_ALL(PAIR_ALL(MEM_QI(symbol.addr),       imm64.const), use_list)",  [0, 0], "SFmode", 2, 0],

], """
  $rule $cost
  build {
    handle_call_kills(add_edges, live);
    if ($addrtype == 0) {
      /* indirect call to an address held in a register */
      sparseset_set_bit(live, find($addr->r));
    } else if ($addrtype == 1) {
      /* indirect call to an address held in a symbol */
      add_addr(live, $addr);
    } else if ($addrtype == 2) {
      /* direct call to an immediate address (normal) */
    } else {
      gcc_assert(0);
    }
  },
  remat {
    flags = 0;
  },
  debug {
    if ($addrtype == 0) {
      dump_callR($addr->r);
    } else if ($addrtype == 1) {
      dump_callM($addr->a);
    } else if ($addrtype == 2) {
      dump_call($addr->a.string);
    } else {
      gcc_assert(0);
    }
  },
  emit {
    rtx call_fusage = CALL_INSN_FUNCTION_USAGE($stmt->insn);
    rtx addr = 0;
    rtx dst = 0;
    if ($addrtype == 0) {
      addr = gen_rtx_REG(DImode, $addr->r);
    } else if ($addrtype == 1) {
      addr = gen_rtx_MEM(DImode, $addr->rtl);
    } else if ($addrtype == 2) {
      addr = $addr->rtl;
    } else {
      gcc_assert(0);
    }
    if ($valuetype == 0) {		/* no value return */
      dst = 0;				/* unused */
    } else if ($valuetype == 1) {	/* scalar return */
      # if ($valuetype == 1)		/* $dst otherwise undefined */
      dst = gen_rtx_REG($rtx_mode, $dst->r);
      # endif
    } else if ($valuetype == 2) {	/* structure return */
      dst = SET_DEST($stmt->rtl);
    } else {
      gcc_assert(0);
    }
    {
      rtx pure_call = gen_rtx_CALL(VOIDmode,
	gen_rtx_MEM(QImode, addr),		/* call to a byte address */
	GEN_INT($const->val));
      rtx dst_call = ($valuetype == 0)
	? pure_call				/* void return value */
	: gen_rtx_SET(VOIDmode, dst, pure_call)	/* scalar or structure */
      ;
      rtx call_insn = emit_call_insn(dst_call);
      add_function_usage_to(call_insn, call_fusage);
    }
  };
""")


#
# defs and uses
#

plug.plugrule3("userecur", [
  ["rule", "cost"],
  ["use_list.dst : LIST_ALL(use.src1, use_list.src2)", [0, 0]],
  ["def_list.dst : LIST_ALL(def.src1, def_list.src2)", [0, 0]],
], """
  $rule $cost
  ;
""")

plug.plugrule3("usebasis", [
  ["rule", "cost"],
  ["use_list.dst : END_OF_LIST", [0, 0]],
  ["def_list.dst : END_OF_LIST", [0, 0]],
], """
  $rule $cost
  ;
""")

plug.plugrule3("stmtuse", [
  ["rule", "cost"],
  ["stmt.dst : use.src", [0, 0]],
  ["stmt.dst : def.src", [0, 0]],
], """
  $rule $cost
  ;
""")

plug.plugrule3("use", [
  ["rule", "cost"],
  ["use.dst : USE_ALL(r64.src)", [0, 0]],
  ["use.dst : USE_ALL(r32.src)", [0, 0]],
  ["use.dst : USE_ALL(r16.src)", [0, 0]],
  ["use.dst : USE_ALL(r8.src)",  [0, 0]],
  ["use.dst : USE_ALL(rd.src)",  [0, 0]],
  ["use.dst : USE_ALL(rf.src)",  [0, 0]],
], """
  $rule $cost
  build {
    sparseset_set_bit(live, find($src->r));
  },
  remat {
    flags = 0;
  },
  emit {
    $dst->rtl = gen_rtx_USE(VOIDmode, gen_rtx_REG(DImode, $src->r));
  };
""")

#
# Preston changed these from, e.g. r64d to r64.
# He wonders if they can ever be spilled.
# That would require some thought.
#
plug.plugrule3("def", [
  ["rule", "cost"],
  ["def.dst : DEF_ALL(r64.src)", [0, 0]],
  ["def.dst : DEF_ALL(r32.src)", [0, 0]],
  ["def.dst : DEF_ALL(r16.src)", [0, 0]],
  ["def.dst : DEF_ALL(r8.src)",  [0, 0]],
  ["def.dst : DEF_ALL(rd.src)",  [0, 0]],
  ["def.dst : DEF_ALL(rf.src)",  [0, 0]],
], """
  $rule $cost
  ;
""")

#
# These rules are added to support condition branches on floating point values.
# the tree we are presented with is:
#
#   SET_ALL(PC_ALL,COND_MOVE(NE_ALL(REG_SF, REG_SF), PAIR_ALL(LABEL_REF_DI, PC_ALL)))
#
# TODO: add memory variants
# TODO: The RTL tree out the back end is missing the SET_ALL and more, so icg+rtl output is mangled
# TODO: add missing comparision operators (such as LT_ALL, etc)
# TODO: For NE_ALL, stock gcc will produce both a jne and a jp (if unordered) to the same target
#
plug.plugrule3("condition_float", [
  ["rule", "cost", "opcode", "schar", "op", "cc_mode", "rtx_mode"],
  ["condition.dst : EQ_ALL(rd.src1, rd.src2)",   [1, 3], "ucomisd", 'x', "UNEQ", "CCFPUmode", "DFmode"],
  ["condition.dst : NE_ALL(rd.src1, rd.src2)",   [1, 3], "ucomisd", 'x', "LTGT", "CCFPUmode", "DFmode"],
  ["condition.dst : EQ_ALL(rf.src1, rf.src2)",   [1, 3], "ucomiss", 'x', "UNEQ", "CCFPUmode", "SFmode"],
  ["condition.dst : NE_ALL(rf.src1, rf.src2)",   [1, 3], "ucomiss", 'x', "LTGT", "CCFPUmode", "SFmode"],
], """
  $rule $cost
  names {
    $dst->code = $op;
    $dst->cc_code = $cc_mode;
  },
  build {
    sparseset_set_bit(live, find($src1->r));
    sparseset_set_bit(live, find($src2->r));
  },
  remat {
    flags = 0;
  },
  costs {
    memorable($src1->r);
    memorable($src2->r);
  },
  debug {
    dumpRR("$opcode", $src2->r, '$schar', $src1->r, '$schar');
  },
  emit {
    emit_insn(gen_rtx_SET(VOIDmode,
			  gen_rtx_REG($cc_mode, REG_CC),
			  gen_rtx_COMPARE_assist(/*is_test_variant*/0, $cc_mode, $rtx_mode,
					  gen_rtx_REG($rtx_mode, $src1->r),
					  gen_rtx_REG($rtx_mode, $src2->r))));
  };
""")

# })]
