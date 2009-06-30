# {([
# -*- mode: python -*-
#
# Copyright (c) 2008 Google Inc. All rights reserved.
#
import plug


# To begin, minimally cover the RTL operations.
# Eventually, cover all the x86-64 instructions.
# There can be overlap, and burg will make the best choices for us.


# When I'm going to need to make a copy of a register
# (as in idiv), require that it be in an regx.  That way,
# if it's already there, we have a good chance of coalescing.
# If it's just in a reg form, the chain rule reg:regx will
# copy it for us, and we'll surely coalesce one of the copies.
# May be even more important for mult and plus, where we'd
# like to be able to reuse either of the inputs if it's available,
# only making a copy if necessary.

# Costs are from AMD's Barcelona Software Optimization Guide, Appendix C
# Since most everything is DirectPathSingle, I'm using latency as
# the primary cost and instruction size as the secondary cost.

#
# The opcodes' enumeration values have been carefully chosen
# For many of the "traditional" operators,
# the sizemode is encoded in the last base-(tb_align) digit
#  (eg, extractable using % tb_align).
# The operator itself is encoded in the remaining more sig digits.
# New operators always start off at values N*tb_align.
#
# See the encoding done in get_operator_mode.
# calling plug.term_align(tb_align) steps from operator to operator;
# calling plug.term_incr(5) gets us from the base operator (DI type) to the 1st float type..
#
# int64    DI: delta ==  0
# int32    SI: delta ==  1
# int16    HI: delta ==  2
# int8     QI: delta ==  3
# int128   TI: delta ==  4
# int256   OI: delta ==  5
# ieee64   DF: delta ==  6
# ieee32   SF: delta ==  7
# vint64  VDI: delta ==  8
# vint32  VSI: delta ==  9
# vint16  VHI: delta == 10
# vint8   VQI: delta == 11
# vieee64 VDF: delta == 12
# vieee32 VSF: delta == 13

tb_align = 14

#
# Some operators use suffix _ALL, which means "all types".
#

#
# start of operators that encode the operator and the result type.
# DO NOT USE opcode value 0.  We skip the first block of terminals.
#
plug.term_incr(1)	# skip opcode value 0

std_type_suffixes = ["DI", "SI", "HI", "QI", "TI", "OI", "DF", "SF"]
int_type_suffixes = ["DI", "SI", "HI", "QI", "TI", "OI"]
uns_type_suffixes = ["DI", "SI", "HI", "QI", "TI", "OI"]
mem_type_suffixes = ["DI", "SI", "HI", "QI"]
vec_type_suffixes = ["DI", "SI", "HI", "QI", "TI", "OI", "DF", "SF",
  "VDI", "VSI", "VHI", "VQI", "VDF", "VSF"]

plug.term_align(tb_align); plug.term_cross("REG",         std_type_suffixes)
plug.term_align(tb_align); plug.term_cross("REGX",        std_type_suffixes)

#
# We probably don't need the full cross product involving std_type_suffixes
# as the only type suffix used in practice is _DI.
#
plug.term_align(tb_align); plug.term_cross("REGCC",       std_type_suffixes)
plug.term_align(tb_align); plug.term_cross("REGCCX",      std_type_suffixes)
plug.term_align(tb_align); plug.term_cross("REGCCFPU",    std_type_suffixes)
plug.term_align(tb_align); plug.term_cross("REGCCFPUX",   std_type_suffixes)

plug.term_align(tb_align); plug.term_cross("ZERO_EXTEND", std_type_suffixes)
plug.term_align(tb_align); plug.term_cross("SIGN_EXTEND", std_type_suffixes)
plug.term_align(tb_align); plug.term_cross("SUBREG",      std_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM",         vec_type_suffixes)
plug.term_align(tb_align); plug.term_cross("PREFETCH",    vec_type_suffixes)

plug.term_align(tb_align); plug.term_cross("PLUS",        vec_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_PLUS",    mem_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MINUS",       vec_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_MINUS",   mem_type_suffixes)
plug.term_align(tb_align); plug.term_cross("NEG",         vec_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_NEG",     mem_type_suffixes)

plug.term_align(tb_align); plug.term_cross("MULT",        vec_type_suffixes)
plug.term_align(tb_align); plug.term_cross("DIV",         vec_type_suffixes)
plug.term_align(tb_align); plug.term_cross("UDIV",        uns_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MOD",         int_type_suffixes)
plug.term_align(tb_align); plug.term_cross("UMOD",        int_type_suffixes)

plug.term_align(tb_align); plug.term_cross("SMIN",        int_type_suffixes)
plug.term_align(tb_align); plug.term_cross("SMAX",        int_type_suffixes)
plug.term_align(tb_align); plug.term_cross("UMIN",        int_type_suffixes)
plug.term_align(tb_align); plug.term_cross("UMAX",        int_type_suffixes)

plug.term_align(tb_align); plug.term_cross("CLZ",         int_type_suffixes)	# x86_64 advanced bit manipulations
plug.term_align(tb_align); plug.term_cross("POPCOUNT",    int_type_suffixes)	# x86_64 advanced bit manipulations
plug.term_align(tb_align); plug.term_cross("BSF",         ["HI", "SI", "DI"])	# no 8 bit variant
plug.term_align(tb_align); plug.term_cross("BSR",         ["HI", "SI", "DI"])	# no 8 bit variant

#
# MEMB is a binary operator.
# The left child is the address,
# and the right child is a constant carrying
# the size of the memory block, in units of size_t.
# We only(?) deal with DImode types.
#
plug.term_align(tb_align); plug.term_cross("MEMB",        std_type_suffixes)

#
# We see bitwise boolean operators on DF and SF registers for some kinds of
# comparison operators that want to yield 1.0 or 0.0
# by doing bit manipulations.
#
plug.term_align(tb_align); plug.term_cross("NOT",         vec_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_NOT",     mem_type_suffixes)
plug.term_align(tb_align); plug.term_cross("AND",         vec_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_AND",     mem_type_suffixes)
plug.term_align(tb_align); plug.term_cross("IOR",         vec_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_IOR",     mem_type_suffixes)
plug.term_align(tb_align); plug.term_cross("XOR",         vec_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_XOR",     mem_type_suffixes)

#
# this structure of names is inherited from the gcc RTL namespace.  Too bad the directionality
# of ASHIFT isn't encoded as ASHIFTL or something similar.
#   ASHIFT:    arithmetic shift left
#   LSHIFTRT:  logical right shift
#   ASHIFTRT:  arithmetic right shift
#   ROTATE:    rotate left
#   ROTATERT:  rotate right
#
plug.term_align(tb_align); plug.term_cross("ASHIFT",       int_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_ASHIFT",   mem_type_suffixes)
plug.term_align(tb_align); plug.term_cross("LSHIFTRT",     int_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_LSHIFTRT", mem_type_suffixes)
plug.term_align(tb_align); plug.term_cross("ASHIFTRT",     int_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_ASHIFTRT", mem_type_suffixes)
plug.term_align(tb_align); plug.term_cross("ROTATE",       int_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_ROTATE",   mem_type_suffixes)
plug.term_align(tb_align); plug.term_cross("ROTATERT",     int_type_suffixes)
plug.term_align(tb_align); plug.term_cross("MEM_ROTATERT", mem_type_suffixes)

plug.term_align(tb_align); plug.term_cross("FIX",          int_type_suffixes)
plug.term_align(tb_align); plug.term_cross("TRUNCATE",     int_type_suffixes)

plug.term_align(tb_align); plug.term_cross("ASM_BINARY_RRR", int_type_suffixes);

plug.term("ASM_NULLARY");

float_only = ["DF", "SF"];
def term_base_float(name):
  plug.term(name + "_BASE");  plug.term_incr(5); plug.term_cross(name, float_only)

plug.term_align(tb_align); term_base_float("FLOAT")

#
# FLOAT_TRUNCATE uses an ieee64 and yields an ieee32
#
plug.term_align(tb_align); term_base_float("FLOAT_TRUNCATE")

#
# FLOAT_EXTEND uses an ieee32 and yields an ieee64
#
plug.term_align(tb_align); term_base_float("FLOAT_EXTEND")

plug.term_align(tb_align); plug.term_cross("UPDATE", mem_type_suffixes)

#
# This was an attempt by rrh on Oct 9, 2008 to use cmpsd instruction
# for compares that must yield a double value (in C++), compares normally
# yield a Boolean but some upstream phase recognizes
# (double compare double) ==> double
# as a special case, and transforms it into some other tree.
# The transformation is correct, but inefficient;
# see gcc.gnu.org/bugzilla bug/issue 37784
#
# See CMPSD page in AMD manual
#
# TODO: it isn't clear when FLT_DF is produced vs UNLT_ALL vs LT_ALL
# when comparing doubles/floats.
#
plug.term_align(tb_align); term_base_float(   "FEQ")
plug.term_align(tb_align); term_base_float(   "FLT")
plug.term_align(tb_align); term_base_float(   "FLE")
plug.term_align(tb_align); term_base_float("FUNORD")
plug.term_align(tb_align); term_base_float(  "FNEQ")
plug.term_align(tb_align); term_base_float(  "FNLT")
plug.term_align(tb_align); term_base_float(  "FNLE")
plug.term_align(tb_align); term_base_float(  "FORD")
plug.term_align(tb_align); term_base_float(   "FGE")	# synthetic
plug.term_align(tb_align); term_base_float(   "FGT")	# synthetic



#
# end of operators that encode the operator and result type.
#

plug.term_align(tb_align); plug.term("SYMBOL_REF_DI")
plug.term("SYMBOL_REF_32")
plug.term("SYMBOL_REF_64")
plug.term("LABEL_REF_DI")

#
# For ordered scalar compares yielding a condition code.
# However sometimes we see these operators in the context double + (double < double).
#
plug.term("LT_ALL")	#   signed
plug.term("LE_ALL")	#   signed
plug.term("EQ_ALL")	# unsigned and signed
plug.term("NE_ALL")	# unsigned and signed
plug.term("GE_ALL")	#   signed
plug.term("GT_ALL")	#   signed

plug.term("LTS_ALL")	#   signed (when mode is CCGOC: CC garbage overflow carry, generate "js")

plug.term("LTU_ALL")	# unsigned
plug.term("LEU_ALL")	# unsigned
plug.term("GEU_ALL")	# unsigned
plug.term("GTU_ALL")	# unsigned

#
# For unordered floating point compares yielding a condition code.
#
plug.term("UNEQ_ALL")		# unordered floating point
plug.term("UNGE_ALL")		# unordered floating point
plug.term("UNGT_ALL")		# unordered floating point
plug.term("UNLE_ALL")		# unordered floating point
plug.term("UNLT_ALL")		# unordered floating point
plug.term("LTGT_ALL")		# unordered floating point
plug.term("UNORDERED_ALL")	# unordered floating point
plug.term("ORDERED_ALL")	# unordered floating point


plug.term_align(tb_align); plug.term("PC_ALL");	# program counter

plug.term("COMPARE_CC")

plug.term("COND_MOVE"); plug.term("COND_MOVE_XX")
plug.term("PAIR_ALL")

#
# do I need all the types for CONST,
# or is CONST only used for relocatable addresses?
#
plug.term("CONST_DI")

plug.term("PARALLEL_ALL")
plug.term("SET_ALL")
plug.term("USE_ALL")
plug.term("END_OF_LIST")
plug.term("CALL_ALL")
plug.term("LIST_ALL")
plug.term("DEF_ALL")

plug.term("CONST_0")	# the constant +0
plug.term("CONST_P1")	# the constant +1
plug.term("CONST_P2")	# the constant +2
plug.term("CONST_P3")	# the constant +3
plug.term("CONST_P4")	# the constant +4
plug.term("CONST_P8")	# the constant +8
plug.term("CONST_P15")	# the constant +15	(for BSR idiom)
plug.term("CONST_P16")	# the constant +16
plug.term("CONST_P24")	# the constant +24
plug.term("CONST_P31")	# the constant +31	(for BSR idiom)
plug.term("CONST_P32")	# the constant +32
plug.term("CONST_P48")	# the constant +48
plug.term("CONST_P56")	# the constant +56
plug.term("CONST_P63")	# the constant +63	(for BSR idiom)
plug.term("CONST_P255")	# the constant +255
plug.term("CONST_N1")	# the constant -1

plug.term("CONST4P")	# a 4 bit positive constant, including the sign bit, [0..7]
plug.term("CONST5P")
plug.term("CONST6P")
plug.term("CONST7P")
plug.term("CONST8P")
plug.term("CONST12P")
plug.term("CONST13P")
plug.term("CONST14P")
plug.term("CONST15P")
plug.term("CONST16P")
plug.term("CONST28P")
plug.term("CONST29P")
plug.term("CONST30P")
plug.term("CONST31P")
plug.term("CONST32P")
plug.term("CONST64P")

plug.term("CONST4N")	# a 4-bit negative constant, [-8..-1]
plug.term("CONST5N")
plug.term("CONST6N")
plug.term("CONST7N")
plug.term("CONST8N")
plug.term("CONST12N")
plug.term("CONST13N")
plug.term("CONST14N")
plug.term("CONST15N")
plug.term("CONST16N")
plug.term("CONST28N")
plug.term("CONST29N")
plug.term("CONST30N")
plug.term("CONST31N")
plug.term("CONST32N")
plug.term("CONST64N")

plug.term("CONSTIEEE32")	# an IEEE-32 constant
plug.term("CONSTIEEE64")	# an IEEE-32 constant

#
# Add terminal symbols for all of the UNSPECs.
# We'll only add rules for the UNSPECs as we encounter a need.
#
plug.term("ICG_UNSPEC_GOT") # 0
plug.term("ICG_UNSPEC_GOTOFF") # 1
plug.term("ICG_UNSPEC_GOTPCREL") # 2
plug.term("ICG_UNSPEC_GOTTPOFF") # 3
plug.term("ICG_UNSPEC_TPOFF") # 4
plug.term("ICG_UNSPEC_NTPOFF") # 5
plug.term("ICG_UNSPEC_DTPOFF") # 6
plug.term("ICG_UNSPEC_GOTNTPOFF") # 7
plug.term("ICG_UNSPEC_INDNTPOFF") # 8
plug.term("ICG_UNSPEC_PLTOFF") # 9
plug.term("ICG_UNSPEC_MACHOPIC_OFFSET") # 10
plug.term("ICG_UNSPEC_STACK_ALLOC") # 11
plug.term("ICG_UNSPEC_SET_GOT") # 12
plug.term("ICG_UNSPEC_SSE_PROLOGUE_SAVE") # 13
plug.term("ICG_UNSPEC_REG_SAVE") # 14
plug.term("ICG_UNSPEC_DEF_CFA") # 15
plug.term("ICG_UNSPEC_SET_RIP") # 16
plug.term("ICG_UNSPEC_SET_GOT_OFFSET") # 17
plug.term("ICG_UNSPEC_TP") # 18
plug.term("ICG_UNSPEC_TLS_GD") # 19
plug.term("ICG_UNSPEC_TLS_LD_BASE") # 20
plug.term("ICG_UNSPEC_TLSDESC") # 21
plug.term("ICG_UNSPEC_SCAS") # 30
plug.term("ICG_UNSPEC_FNSTSW") # 31
plug.term("ICG_UNSPEC_SAHF") # 32
plug.term("ICG_UNSPEC_FSTCW") # 33
plug.term_align(tb_align); plug.term_cross("ICG_UNSPEC_ADD_CARRY", vec_type_suffixes) # 34
plug.term("ICG_UNSPEC_FLDCW") # 35
plug.term("ICG_UNSPEC_REP") # 36
plug.term("ICG_UNSPEC_EH_RETURN") # 37
plug.term("ICG_UNSPEC_LD_MPIC") # 38
plug.term("ICG_UNSPEC_TRUNC_NOOP") # 39
plug.term("ICG_UNSPEC_FIX_NOTRUNC") # 40
plug.term("ICG_UNSPEC_PFRCPIT1") # 40
plug.term("ICG_UNSPEC_MASKMOV") # 41
plug.term("ICG_UNSPEC_PFRCPIT2") # 41
plug.term("ICG_UNSPEC_MOVMSK") # 42
plug.term("ICG_UNSPEC_PFRSQRT") # 42
plug.term("ICG_UNSPEC_MOVNT") # 43
plug.term("ICG_UNSPEC_PFRSQIT1") # 43
plug.term("ICG_UNSPEC_MFENCE") # 44
plug.term("ICG_UNSPEC_MOVU") # 44
plug.term("ICG_UNSPEC_LFENCE") # 45
plug.term("ICG_UNSPEC_RCP") # 45
plug.term("ICG_UNSPEC_PSADBW") # 46
plug.term("ICG_UNSPEC_RSQRT") # 46
plug.term("ICG_UNSPEC_LDDQU") # 47
plug.term("ICG_UNSPEC_SFENCE") # 47
plug.term("ICG_UNSPEC_PFRCP") # 49
plug.term("ICG_UNSPEC_COPYSIGN") # 50
plug.term_align(tb_align); plug.term_cross("ICG_UNSPEC_IEEE_MIN", vec_type_suffixes) # 51
plug.term_align(tb_align); plug.term_cross("ICG_UNSPEC_IEEE_MAX", vec_type_suffixes) # 52
plug.term("ICG_UNSPEC_SIN") # 60
plug.term("ICG_UNSPEC_COS") # 61
plug.term("ICG_UNSPEC_FPATAN") # 62
plug.term("ICG_UNSPEC_FYL2X") # 63
plug.term("ICG_UNSPEC_FYL2XP1") # 64
plug.term("ICG_UNSPEC_FRNDINT") # 65
plug.term("ICG_UNSPEC_FIST") # 66
plug.term("ICG_UNSPEC_F2XM1") # 67
plug.term("ICG_UNSPEC_TAN") # 68
plug.term("ICG_UNSPEC_FXAM") # 69
plug.term("ICG_UNSPEC_FRNDINT_FLOOR") # 70
plug.term("ICG_UNSPEC_FRNDINT_CEIL") # 71
plug.term("ICG_UNSPEC_FRNDINT_TRUNC") # 72
plug.term("ICG_UNSPEC_FRNDINT_MASK_PM") # 73
plug.term("ICG_UNSPEC_FIST_FLOOR") # 74
plug.term("ICG_UNSPEC_FIST_CEIL") # 75
plug.term("ICG_UNSPEC_SINCOS_COS") # 80
plug.term("ICG_UNSPEC_SINCOS_SIN") # 81
plug.term("ICG_UNSPEC_XTRACT_FRACT") # 84
plug.term("ICG_UNSPEC_XTRACT_EXP") # 85
plug.term("ICG_UNSPEC_FSCALE_FRACT") # 86
plug.term("ICG_UNSPEC_FSCALE_EXP") # 87
plug.term("ICG_UNSPEC_FPREM_F") # 88
plug.term("ICG_UNSPEC_FPREM_U") # 89
plug.term("ICG_UNSPEC_FPREM1_F") # 90
plug.term("ICG_UNSPEC_FPREM1_U") # 91
plug.term("ICG_UNSPEC_C2_FLAG") # 95
plug.term("ICG_UNSPEC_SP_SET") # 100
plug.term("ICG_UNSPEC_SP_TEST") # 101
plug.term("ICG_UNSPEC_SP_TLS_SET") # 102
plug.term("ICG_UNSPEC_SP_TLS_TEST") # 103
plug.term("ICG_UNSPEC_PSHUFB") # 120
plug.term("ICG_UNSPEC_PSIGN") # 121
plug.term("ICG_UNSPEC_PALIGNR") # 122
plug.term("ICG_UNSPEC_EXTRQI") # 130
plug.term("ICG_UNSPEC_EXTRQ") # 131
plug.term("ICG_UNSPEC_INSERTQI") # 132
plug.term("ICG_UNSPEC_INSERTQ") # 133
plug.term("ICG_UNSPEC_BLENDV") # 134
plug.term("ICG_UNSPEC_INSERTPS") # 135
plug.term("ICG_UNSPEC_DP") # 136
plug.term("ICG_UNSPEC_MOVNTDQA") # 137
plug.term("ICG_UNSPEC_MPSADBW") # 138
plug.term("ICG_UNSPEC_PHMINPOSUW") # 139
plug.term("ICG_UNSPEC_PTEST") # 140
plug.term("ICG_UNSPEC_ROUND") # 141
plug.term("ICG_UNSPEC_CRC32") # 143
plug.term("ICG_UNSPEC_PCMPESTR") # 144
plug.term("ICG_UNSPEC_PCMPISTR") # 145
plug.term("ICG_UNSPEC_SSE5_INTRINSIC") # 150
plug.term("ICG_UNSPEC_SSE5_UNSIGNED_CMP") # 151
plug.term("ICG_UNSPEC_SSE5_TRUEFALSE") # 152
plug.term("ICG_UNSPEC_SSE5_PERMUTE") # 153
plug.term("ICG_UNSPEC_FRCZ") # 154
plug.term("ICG_UNSPEC_CVTPH2PS") # 155
plug.term("ICG_UNSPEC_CVTPS2PH") # 156
plug.term("ICG_UNSPEC_AESENC") # 159
plug.term("ICG_UNSPEC_AESENCLAST") # 160
plug.term("ICG_UNSPEC_AESDEC") # 161
plug.term("ICG_UNSPEC_AESDECLAST") # 162
plug.term("ICG_UNSPEC_AESIMC") # 163
plug.term("ICG_UNSPEC_AESKEYGENASSIST") # 164
plug.term("ICG_UNSPEC_PCLMUL") # 165
plug.term("ICG_UNSPEC_PCMP") # 166
plug.term("ICG_UNSPEC_VPERMIL") # 167
plug.term("ICG_UNSPEC_VPERMIL2") # 168
plug.term("ICG_UNSPEC_VPERMIL2F128") # 169
plug.term("ICG_UNSPEC_MASKLOAD") # 170
plug.term("ICG_UNSPEC_MASKSTORE") # 171
plug.term("ICG_UNSPEC_CAST") # 172
plug.term("ICG_UNSPEC_VTESTP") # 173

plug.nonterm("stmt")
plug.nonterm("nocc")	# computation to memory, without any side effects on condition codes

#
# Preston writes (02Jul2008):
# All the integer registers are 64 bits,
# but lots of times they hold smaller values.  I differentiate between these cases:
#
#   r64
#   r32  - don't know the value of the upper 32 bits
#   rz32 - the upper 32 bits are 0
#   r16  - don't know the value of the upper 48 bits
#   rz16 - the upper 48 bits are all zero
#   rs16 - the upper 32 are zero, the next 16 are a sign extension of the lower 16
#   r8   - don't know the value of the upper 56 bits
#   rz8  - the upper 56 bits are all zero
#   rs8  - the upper 32 bits are 0, the next 24 are a sign extension of the lower 8
#
#
# There are also the "x" variants of each of these, r64x, r32x, rz32x, etc.
# The 'x'     variant means that the register will NOT be reused.
# The non-'x' variant continues to be live, so is read only.
#
# non terminals prefixed with "cc" means that the condition code is also set
# and is live and is to be used immediately.  For example "if (a+b > 0)"
# would be done as an add of rn:=a+b, and then a branch; no intermediate test is needed.
#
# TODO (05Nov2008): the register allocator may spill and reload and the condition codes will not
# be spilled/reloaded.  This perhaps needs to be marked as a special case in the allocator?
#
# All of these oddball cases come up because of the oddball way the x86-64
# works.  It all started when I had to load an 8-bit value from memory to
# a register.  I had a choice: either sign extend it to 32 bit (zeroing the
# upper 32 bits), or zero extend it to 32 bits (still zeroing the upper
# 32 bits).  How to choose between them?  So I made all these names and
# added many, many grammar rules to force iburg to choose between them.
# Worked a treat.
#
# Last night, I had the idea of condensing the set of non-terminal
# names, combining r8, r16, r32, and r64 into a single class called r64.
# After all, it's basically a register right?  And we'd differentiate
# between things using the mode of the operation.  Plus, I'd be able to
# take advantage of a few more tricky situations.
#
# It almost worked.  Where it fails is with SIGN_EXTEND and ZERO_EXTEND.
# The problem is that the mode of these specifies what the result should be
# (e.g., DI) but not what the source was.  I need to know both so I can
# know to extend from 8 to 64 bits, or 16 to 64, etc.
#

legal_lhs_attrs = ["r", "rtl", "spilled"]

plug.nonterm("lhs64", legal_lhs_attrs)
plug.nonterm("lhs32", legal_lhs_attrs)
plug.nonterm("lhs16", legal_lhs_attrs)
plug.nonterm("lhs8",  legal_lhs_attrs)
plug.nonterm("lhsd",  legal_lhs_attrs)
plug.nonterm("lhsf",  legal_lhs_attrs)

legal_rx_attrs = ["rx", "extra", "freed"]
legal_r_attrs =  ["r", "extra", "freed"]

plug.nonterm("rd", legal_r_attrs)
plug.nonterm("rdx", legal_rx_attrs)

plug.nonterm("rf", legal_r_attrs)
plug.nonterm("rfx", legal_rx_attrs)

plug.nonterm("r64", legal_r_attrs)
plug.nonterm("r64x", legal_rx_attrs)

plug.nonterm("r32", legal_r_attrs)
plug.nonterm("r32x", legal_rx_attrs)

plug.nonterm("rz32")  # rzN is zero-extended to 64 bits
plug.nonterm("rz32x", legal_rx_attrs) # rzN is zero-extended to 64 bits

plug.nonterm("r16", legal_r_attrs)
plug.nonterm("r16x", legal_rx_attrs)
plug.nonterm("rz16", legal_r_attrs)
plug.nonterm("rz16x", legal_rx_attrs)
# rsN is sign-extended to 32 bits
plug.nonterm("rs16", legal_r_attrs )
plug.nonterm("rs16x", legal_rx_attrs) # rsN is sign-extended to 32 bits

plug.nonterm("r8", legal_r_attrs)
plug.nonterm("r8x", legal_rx_attrs)
plug.nonterm("rz8", legal_r_attrs)
plug.nonterm("rz8x", legal_rx_attrs)
plug.nonterm("rs8", legal_r_attrs)
plug.nonterm("rs8x", legal_rx_attrs)

plug.nonterm("ccr64x", legal_rx_attrs)
plug.nonterm("ccr32x", legal_rx_attrs)
plug.nonterm("ccrz32x", legal_rx_attrs)
plug.nonterm("ccr16x", legal_rx_attrs)
plug.nonterm("ccrz16x", legal_rx_attrs)
plug.nonterm("ccrs16x", legal_rx_attrs)

plug.nonterm("ccr8x", legal_rx_attrs)
plug.nonterm("ccrz8x", legal_rx_attrs)
plug.nonterm("ccrs8x", legal_rx_attrs)

bogus_attrs = ["foo"]
plug.nonterm("rcc", bogus_attrs)		#           condition code
plug.nonterm("urcc", bogus_attrs)		# unordered condition code (floating point)
plug.nonterm("condition", ["code", "cc_code"])

plug.nonterm("disp", ["a", "rtl"])
plug.nonterm("addr", ["a", "extra", "freed", "rtl"])

plug.nonterm("base64", ["extra", "freed", "a", "rtl"])
plug.nonterm("base32", ["extra", "freed", "a", "rtl"])
plug.nonterm("base16", ["extra", "freed", "a", "rtl"])
plug.nonterm("base8",  ["extra", "freed", "a", "rtl"])

plug.nonterm("index64", ["extra", "freed", "a", "rtl"])
plug.nonterm("index32", ["extra", "freed", "a", "rtl"])
plug.nonterm("index16", ["extra", "freed", "a", "rtl"])
plug.nonterm("index8",  ["extra", "freed", "a", "rtl"])

plug.nonterm("scon", ["val"])
plug.nonterm("mcon", ["val"])
plug.nonterm("imm4", ["val"])
plug.nonterm("imm5", ["val"])
plug.nonterm("imm6", ["val"])
plug.nonterm("imm7", ["val"])
plug.nonterm("imm8", ["val"])
plug.nonterm("imm12", ["val"])
plug.nonterm("imm13", ["val"])
plug.nonterm("imm14", ["val"])
plug.nonterm("imm15", ["val"])
plug.nonterm("imm16", ["val"])
plug.nonterm("imm28", ["val"])
plug.nonterm("imm29", ["val"])
plug.nonterm("imm30", ["val"])
plug.nonterm("imm31", ["val"])
plug.nonterm("imm32", ["val", "a", "rtl"])
plug.nonterm("imm64", ["val"])

plug.nonterm("pos8", ["val"])
plug.nonterm("pos16", ["val"])
plug.nonterm("pos32", ["val"])
plug.nonterm("pureimm32", ["val"])	# only constants; no symbols involved

plug.nonterm("label")
plug.nonterm("compare")
plug.nonterm("use")
plug.nonterm("def")
plug.nonterm("use_list")
plug.nonterm("def_list")

plug.nonterm("symbol")
plug.nonterm("symbol32")
plug.nonterm("symbol64")

plug.reduce("names"     , "icg-names.cases")     # a pass for numbering all the temporary registers
plug.reduce("kinds"     , "icg-kinds.cases")     # a pass for setting the kind of each register
plug.reduce("supairs"   , "icg-supairs.cases")   # a pass for computing SU pairs
plug.reduce("build"     , "icg-build.cases")     # a pass for building the inteference graph
plug.reduce("coalesce"  , "icg-coalesce.cases")  # a pass for coalescing
plug.reduce("remat"     , "icg-remat.cases")     # a pass for finding rematerialization opportunities
plug.reduce("costs"     , "icg-costs.cases")     # a pass for computing spill costs
plug.reduce("spill"     , "icg-spill.cases")     # a pass for adding spill code
plug.reduce("debug"     , "icg-debug.cases")     # a pass for dumping debuggable stuff
plug.reduce("final"     , "icg-final.cases")     # final rewrite of code with colored registers
plug.reduce("targeting" , "icg-targeting.cases") # a pass for coalescing for copy above commutative expressions
plug.reduce("emit"      , "icg-emit.cases")      # a pass for generating RTL

plug.start("stmt")


# costs
#
# I'm using two parts, [latency, bytes], where latency is more significant.
# Most everything on the Opteron has a 1-cycle latency, so it's not too interesting.
# The encoding size, in bytes, will often be the differentiator.
# I can't be perfectly accurate, because the final size will sometimes
# depend on the registers assigned.
#
# Here's a summary of what I've seen in experiments for addressing modes
# (this is only the part the addressing mode contributes to the total cost),
# so something like
#
#     movq 5(%rax,%rbx,4),%rcx
#
# has requires 2 bytes for the basic movq + whatever the addressing mode
# contributes (in this case, 3 more bytes for a total of 5).
#
# I test the size of things by typing instructions into a .s file
# and running them through the assembler, like this
#
#     gcc -c -Wa,-alh test.s
#
# which spits out a listing.  Be sure to use a 64-bit version of gcc!
#
# addressing
#    mode		cost		comments
#--------------------------------------------------------------------
#    (base)		 1		a base register contributes 1
#    (base,index)	 2		an index register add 1
#    (base,index,4)	 2		even if it is shifted
#    1(base,index,4)	 3		a displacement adds 1 byte, if it fits
#    130(base,index,4)	 6		or 4 bytes
#    (,index)		 6		an index by itself is expensive
#    1(,index,4)         6		same cost
#    130(,index,4)       6		same cost
#    130		 6		direct (use RIP instead)
#    130(%RIP)           5		position independent

# })]
