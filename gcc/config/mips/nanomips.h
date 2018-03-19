/* Target macros for mips*-img-elf targets.
   Copyright (C) 2018 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#undef MIPS_SUPPORT_PS_3D
#undef MIPS_SUPPORT_MSA
#undef MIPS_SUPPORT_LOONGSON
#undef MIPS_SUPPORT_MICROMIPS
#undef MIPS_SUPPORT_LEGACY
#undef MIPS_SUPPORT_FRAME_HEADER_OPT

/* Architecture target defines.  */
#undef TARGET_LOONGSON_2E
#define TARGET_LOONGSON_2E	    0
#undef TARGET_LOONGSON_2F
#define TARGET_LOONGSON_2F	    0
#undef TARGET_LOONGSON_2EF
#define TARGET_LOONGSON_2EF	    (TARGET_LOONGSON_2E || TARGET_LOONGSON_2F)
#undef TARGET_LOONGSON_3A
#define TARGET_LOONGSON_3A	    0
#undef TARGET_MIPS3900
#define TARGET_MIPS3900		    0
#undef TARGET_MIPS4000
#define TARGET_MIPS4000		    0
#undef TARGET_MIPS4120
#define TARGET_MIPS4120		    0
#undef TARGET_MIPS4130
#define TARGET_MIPS4130		    0
#undef TARGET_MIPS5400
#define TARGET_MIPS5400		    0
#undef TARGET_MIPS5500
#define TARGET_MIPS5500		    0
#undef TARGET_MIPS5900
#define TARGET_MIPS5900		    0
#undef TARGET_MIPS7000
#define TARGET_MIPS7000		    0
#undef TARGET_MIPS8000
#define TARGET_MIPS8000		    0
#undef TARGET_MIPS9000
#define TARGET_MIPS9000		    0
#undef TARGET_OCTEON
#define TARGET_OCTEON		    0
#undef TARGET_OCTEON2
#define TARGET_OCTEON2		    0
#undef TARGET_SB1
#define TARGET_SB1		    0
#undef TARGET_SR71K
#define TARGET_SR71K		    0
#undef TARGET_XLP
#define TARGET_XLP		    0
#undef TARGET_INTERAPTIV_MR2
#define TARGET_INTERAPTIV_MR2	    0

#undef TARGET_NANOMIPS
#define TARGET_NANOMIPS		    (mips_arch == PROCESSOR_NMS1	\
				     ? NANOMIPS_NMS			\
				     : mips_arch == PROCESSOR_I7200	\
				     ? NANOMIPS_NMF : 0)

/* Scheduling target defines.  */
#undef TUNE_20KC
#define TUNE_20KC		    0
#undef TUNE_24K
#define TUNE_24K		    0
#undef TUNE_74K
#define TUNE_74K		    0
#undef TUNE_LOONGSON_2EF
#define TUNE_LOONGSON_2EF	    0
#undef TUNE_LOONGSON_3A
#define TUNE_LOONGSON_3A	    0
#undef TUNE_MIPS3000
#define TUNE_MIPS3000		    0
#undef TUNE_MIPS3900
#define TUNE_MIPS3900		    0
#undef TUNE_MIPS4000
#define TUNE_MIPS4000		    0
#undef TUNE_MIPS4120
#define TUNE_MIPS4120		    0
#undef TUNE_MIPS4130
#define TUNE_MIPS4130		    0
#undef TUNE_MIPS5000
#define TUNE_MIPS5000		    0
#undef TUNE_MIPS5400
#define TUNE_MIPS5400		    0
#undef TUNE_MIPS5500
#define TUNE_MIPS5500		    0
#undef TUNE_MIPS6000
#define TUNE_MIPS6000		    0
#undef TUNE_MIPS7000
#define TUNE_MIPS7000		    0
#undef TUNE_MIPS9000
#define TUNE_MIPS9000		    0
#undef TUNE_OCTEON
#define TUNE_OCTEON		    0
#undef TUNE_SB1
#define TUNE_SB1		    0
#undef TUNE_P5600
#define TUNE_P5600		    0
#undef TUNE_I6400
#define TUNE_I6400		    0
#undef TUNE_P6600
#define TUNE_P6600		    0
#define TUNE_I7200                  (mips_tune == PROCESSOR_I7200)

/* Currently, querying of DFA is only needed for Loongson.  */
#undef CPU_UNITS_QUERY

#undef MULTILIB_DEFAULTS

#undef MIPS_ISA_LEVEL_SPEC

/* A spec that injects the default multilib ISA if no architecture is
   specified.  */

#undef MIPS_DEFAULT_ISA_LEVEL_SPEC
#define MIPS_DEFAULT_ISA_LEVEL_SPEC \
  "%{!march=*: -" MULTILIB_ISA_DEFAULT "}"

#undef MIPS_32BIT_OPTION_SPEC
#define MIPS_32BIT_OPTION_SPEC \
  "march=32r6|march=32r6s|march=i7200|march=nms1"

#undef BASE_DRIVER_SELF_SPECS
#define BASE_DRIVER_SELF_SPECS \
  "%{march=32r6|march=32r6s|march=64r6|march=i7200|march=nms1: \
     %{!-fuse-ld=*: -fuse-ld=gold} \
     %{mdsp: -mdspr3}}" \
  "%{!mno-dsp: \
     %{!mdsp: %{march=i7200: -mdsp -mdspr3}}}"

/* A spec that infers a -mhard-float or -msoft-float setting from an
   -march argument.  Note that soft-float and hard-float code are not
   link-compatible.  */

#undef MIPS_ARCH_FLOAT_SPEC
#define MIPS_ARCH_FLOAT_SPEC \
  "%{mhard-float|msoft-float|march=32r6|march=32r6s:; \
     march=nms1|march=i7200: -msoft-float; \
     march=*: -mhard-float}"

#undef DRIVER_SELF_SPECS
#define DRIVER_SELF_SPECS						\
  /* Set the ISA for the default multilib.  */				\
  MIPS_DEFAULT_ISA_LEVEL_SPEC,						\
									\
  /* Infer the default float setting from -march.  */			\
  MIPS_ARCH_FLOAT_SPEC,							\
									\
  /* If no ABI option is specified, infer one from the ISA level	\
     or -mgp setting.  */						\
  "%{!m64:%{!m32:%{" MIPS_32BIT_OPTION_SPEC ": -m32;: -m64}}}",		\
									\
  /* Make sure that an endian option is always present.  This makes	\
     things like LINK_SPEC easier to write.  */				\
  "%{!EB:%{!EL:%(endian_spec)}}",					\
									\
  /* Configuration-independent MIPS rules.  */				\
  BASE_DRIVER_SELF_SPECS

#undef SUBTARGET_ASM_DEBUGGING_SPEC
#define SUBTARGET_ASM_DEBUGGING_SPEC "\
%{g} %{g0} %{g1} %{g2} %{g3} \
%{ggdb:-g} %{ggdb0:-g0} %{ggdb1:-g1} %{ggdb2:-g2} %{ggdb3:-g3} \
%{gstabs:-g} %{gstabs0:-g0} %{gstabs1:-g1} %{gstabs2:-g2} %{gstabs3:-g3} \
%{gstabs+:-g} %{gstabs+0:-g0} %{gstabs+1:-g1} %{gstabs+2:-g2} %{gstabs+3:-g3}"

#undef ASM_SPEC
#define ASM_SPEC "\
%(endian_spec) \
%{.S:-mlegacyregs} \
%{m32} %{m64} \
%{march=*} \
%{mdspr3} %{mno-dspr3} \
%{mmcu} %{mno-mcu} \
%{meva} %{mno-eva} \
%{mvirt} %{mno-virt} \
%{mxpa} %{mno-xpa} \
%{mmsa} %{mno-msa} \
%{mmt} %{mno-mt} \
%(subtarget_asm_debugging_spec) \
%{msym32} %{mno-sym32} \
%{mtune=*}" \
FP_ASM_SPEC "\
%{mbalc-stubs} %{mno-balc-stubs} %{!mno-balc-stubs: %{Os: -mbalc-stubs}} \
%(subtarget_asm_spec)"

#undef LINK_SPEC
#define LINK_SPEC "\
%(endian_spec) \
%{shared}"

#undef DEFAULT_SIGNED_CHAR
#define DEFAULT_SIGNED_CHAR 0

#undef MIPS_FRAME_BIAS
#define MIPS_FRAME_BIAS 4096

#define FRAME_ADDR_RTX(frame) \
   plus_constant (Pmode, frame, MIPS_FRAME_BIAS)

/* Trampolines are a block of code followed by two pointers.  */

#undef TRAMPOLINE_SIZE
#define TRAMPOLINE_SIZE ((8 * 2) + GET_MODE_SIZE (ptr_mode) * 2)

/* 32-bit alignment is sufficient for the nanoMIPS trampoline.  */

#undef TRAMPOLINE_ALIGNMENT
#define TRAMPOLINE_ALIGNMENT 32

/* Describe how we implement __builtin_eh_return.  */

/* At the moment, nothing appears to use more than 2 EH data registers.
   The chosen registers must not clash with the return register ($2),
   EH_RETURN_STACKADJ ($3), or MIPS_EPILOGUE_TEMP ($5), and they must
   be general MIPS16 registers.  Pick $6 and $7.  */
#undef EH_RETURN_DATA_REGNO
#define EH_RETURN_DATA_REGNO(N) \
  ((N) < 2 ? 7 - (N) : INVALID_REGNUM)

/* Use $5 as a temporary for both MIPS16 and non-MIPS16.  */
#undef MIPS_EPILOGUE_TEMP_REGNUM
#define MIPS_EPILOGUE_TEMP_REGNUM \
  (cfun->machine->interrupt_handler_p ? K0_REG_NUM : GP_REG_FIRST + 5)

/* Using long will always be right for size_t and ptrdiff_t, since
   sizeof(long) must equal sizeof(void *), following from the setting
   of the -mlong64 option.  */
#undef SIZE_TYPE
#define SIZE_TYPE "unsigned int"
#undef PTRDIFF_TYPE
#define PTRDIFF_TYPE "int"

/* We are using array initializers and don't want calls in the INIT
   and FINI sections.  */
#undef CRT_CALL_STATIC_FUNCTION
#define CRT_CALL_STATIC_FUNCTION(SECTION_OP, FUNC)

#define CTORS_SECTION_ASM_OP "\t.section\t.init_array,\"aw\",@init_array"
#define DTORS_SECTION_ASM_OP "\t.section\t.fini_array,\"aw\",@fini_array"

#undef INIT_SECTION_ASM_OP
#undef FINI_SECTION_ASM_OP
#define INIT_ARRAY_SECTION_ASM_OP CTORS_SECTION_ASM_OP
#define FINI_ARRAY_SECTION_ASM_OP DTORS_SECTION_ASM_OP

#undef TARGET_ASM_CONSTRUCTOR
#define TARGET_ASM_CONSTRUCTOR default_elf_init_array_asm_out_constructor
#undef TARGET_ASM_DESTRUCTOR
#define TARGET_ASM_DESTRUCTOR default_elf_fini_array_asm_out_destructor

#define CTOR_LIST_BEGIN asm (CTORS_SECTION_ASM_OP)
#define CTOR_LIST_END	/* empty */
#define DTOR_LIST_BEGIN asm (DTORS_SECTION_ASM_OP)
#define DTOR_LIST_END	/* empty */

#undef REGISTER_NAMES
#define REGISTER_NAMES							   \
{ /* General registers.  */						   \
  "$zero", "$at",   "$t4",   "$t5",   "$a0",   "$a1",   "$a2",   "$a3",	   \
  "$a4",   "$a5",   "$a6",   "$a7",   "$t0",   "$t1",   "$t2",   "$t3",	   \
  "$s0",   "$s1",   "$s2",   "$s3",   "$s4",   "$s5",   "$s6",   "$s7",	   \
  "$t8",   "$t9",   "$k0",   "$k1",   "$gp",   "$sp",   "$fp",   "$ra",	   \
  /* Floating-point registers.  */					   \
  "$f0",   "$f1",   "$f2",   "$f3",   "$f4",   "$f5",   "$f6",   "$f7",	   \
  "$f8",   "$f9",   "$f10",  "$f11",  "$f12",  "$f13",  "$f14",  "$f15",   \
  "$f16",  "$f17",  "$f18",  "$f19",  "$f20",  "$f21",  "$f22",  "$f23",   \
  "$f24",  "$f25",  "$f26",  "$f27",  "$f28",  "$f29",  "$f30",  "$f31",   \
  /* Others.  */							   \
  "$hi",   "$lo",   "",      "$fcc0", "$fcc1", "$fcc2", "$fcc3", "$fcc4",  \
  "$fcc5","$fcc6",  "$fcc7",  "", "$cprestore","$arg",  "$frame","$fakec", \
  /* COP0 registers */							   \
  "$c0r0", "$c0r1", "$c0r2", "$c0r3", "$c0r4", "$c0r5", "$c0r6", "$c0r7",  \
  "$c0r8", "$c0r9", "$c0r10","$c0r11","$c0r12","$c0r13","$c0r14","$c0r15", \
  "$c0r16","$c0r17","$c0r18","$c0r19","$c0r20","$c0r21","$c0r22","$c0r23", \
  "$c0r24","$c0r25","$c0r26","$c0r27","$c0r28","$c0r29","$c0r30","$c0r31", \
  /* COP2 registers */							   \
  "$c2r0", "$c2r1", "$c2r2", "$c2r3", "$c2r4", "$c2r5", "$c2r6", "$c2r7",  \
  "$c2r8", "$c2r9", "$c2r10","$c2r11","$c2r12","$c2r13","$c2r14","$c2r15", \
  "$c2r16","$c2r17","$c2r18","$c2r19","$c2r20","$c2r21","$c2r22","$c2r23", \
  "$c2r24","$c2r25","$c2r26","$c2r27","$c2r28","$c2r29","$c2r30","$c2r31", \
  /* COP3 registers */							   \
  "$c3r0", "$c3r1", "$c3r2", "$c3r3", "$c3r4", "$c3r5", "$c3r6", "$c3r7",  \
  "$c3r8", "$c3r9", "$c3r10","$c3r11","$c3r12","$c3r13","$c3r14","$c3r15", \
  "$c3r16","$c3r17","$c3r18","$c3r19","$c3r20","$c3r21","$c3r22","$c3r23", \
  "$c3r24","$c3r25","$c3r26","$c3r27","$c3r28","$c3r29","$c3r30","$c3r31", \
  /* 6 DSP accumulator registers & 6 control registers */		   \
  "$ac1hi","$ac1lo","$ac2hi","$ac2lo","$ac3hi","$ac3lo","$dsp_po","$dsp_sc",\
  "$dsp_ca","$dsp_ou","$dsp_cc","$dsp_ef" }

#undef ADDITIONAL_REGISTER_NAMES
#define ADDITIONAL_REGISTER_NAMES					\
{									\
  { "$r0",	 0 + GP_REG_FIRST },					\
  { "$AT",	 1 + GP_REG_FIRST },					\
  { "$r1",	 1 + GP_REG_FIRST },					\
  { "$r2",	 2 + GP_REG_FIRST },					\
  { "$r3",	 3 + GP_REG_FIRST },					\
  { "$r4",	 4 + GP_REG_FIRST },					\
  { "$r5",	 5 + GP_REG_FIRST },					\
  { "$r6",	 6 + GP_REG_FIRST },					\
  { "$r7",	 7 + GP_REG_FIRST },					\
  { "$r8",	 8 + GP_REG_FIRST },					\
  { "$r9",	 9 + GP_REG_FIRST },					\
  { "$r10",	10 + GP_REG_FIRST },					\
  { "$r11",	11 + GP_REG_FIRST },					\
  { "$r12",	12 + GP_REG_FIRST },					\
  { "$r13",	13 + GP_REG_FIRST },					\
  { "$r14",	14 + GP_REG_FIRST },					\
  { "$r15",	15 + GP_REG_FIRST },					\
  { "$r16",	16 + GP_REG_FIRST },					\
  { "$r17",	17 + GP_REG_FIRST },					\
  { "$r18",	18 + GP_REG_FIRST },					\
  { "$r19",	19 + GP_REG_FIRST },					\
  { "$r20",	20 + GP_REG_FIRST },					\
  { "$r21",	21 + GP_REG_FIRST },					\
  { "$r22",	22 + GP_REG_FIRST },					\
  { "$r23",	23 + GP_REG_FIRST },					\
  { "$r24",	24 + GP_REG_FIRST },					\
  { "$r25",	25 + GP_REG_FIRST },					\
  { "$r26",	26 + GP_REG_FIRST },					\
  { "$r27",	27 + GP_REG_FIRST },					\
  { "$r28",	28 + GP_REG_FIRST },					\
  { "$r29",	29 + GP_REG_FIRST },					\
  { "$r30",	30 + GP_REG_FIRST },					\
  { "$r31",	31 + GP_REG_FIRST },					\
}
