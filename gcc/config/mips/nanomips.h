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

#undef MULTILIB_DEFAULTS

#undef DRIVER_SELF_SPECS
#define DRIVER_SELF_SPECS						\
  /* Set the ISA for the default multilib.  */				\
  MIPS_DEFAULT_ISA_LEVEL_SPEC,						\
									\
  /* Make sure a -mips option is present.  This helps us to pick	\
     the right multilib, and also makes the later specs easier		\
     to write.  */							\
  MIPS_ISA_LEVEL_SPEC,							\
									\
  /* Infer the default float setting from -march.  */			\
  MIPS_ARCH_FLOAT_SPEC,							\
									\
  /* Infer the -msynci setting from -march if not explicitly set.  */	\
  MIPS_ISA_SYNCI_SPEC,							\
									\
  /* If no ABI option is specified, infer one from the ISA level	\
     or -mgp setting.  */						\
  "%{!mabi=*: %{" MIPS_32BIT_OPTION_SPEC ": -mabi=p32;: -mabi=p64}}",	\
									\
  /* Make sure that an endian option is always present.  This makes	\
     things like LINK_SPEC easier to write.  */				\
  "%{!EB:%{!EL:%(endian_spec)}}",					\
									\
  /* Configuration-independent MIPS rules.  */				\
  BASE_DRIVER_SELF_SPECS

#undef LINK_SPEC
#define LINK_SPEC "\
%(endian_spec) \
%{!mips32r6: %{!mips64r6: %{G*}}} \
%{mips1} %{mips2} %{mips3} %{mips4} %{mips32*} %{mips64*} \
%{shared}"

#undef DEFAULT_SIGNED_CHAR
#define DEFAULT_SIGNED_CHAR 0

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

#define CTOR_LIST_BEGIN asm (CTORS_SECTION_ASM_OP)
#define CTOR_LIST_END	/* empty */
#define DTOR_LIST_BEGIN asm (DTORS_SECTION_ASM_OP)
#define DTOR_LIST_END	/* empty */
