/* Target macros for mips*-img-elf targets.
   Copyright (C) 2017 Free Software Foundation, Inc.

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
#define TARGET_NANOMIPS		    (mips_arch == PROCESSOR_M7000	\
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
  "march=32r6|march=32r6s|march=i7200|march=m7000"

#undef BASE_DRIVER_SELF_SPECS
#define BASE_DRIVER_SELF_SPECS \
  "%{march=32r6|march=32r6s|march=64r6|march=i7200|march=m7000: \
     %{!-fuse-ld=*: -fuse-ld=gold} \
     %{mdsp: -mdspr3}}"

/* A spec that infers a -mhard-float or -msoft-float setting from an
   -march argument.  Note that soft-float and hard-float code are not
   link-compatible.  */

#undef MIPS_ARCH_FLOAT_SPEC
#define MIPS_ARCH_FLOAT_SPEC \
  "%{mhard-float|msoft-float|march=32r6|march=32r6s:; \
     march=m7000|march=i7200: -msoft-float; \
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
