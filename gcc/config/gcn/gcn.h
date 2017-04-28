/* Copyright (C) 2016-2017 Free Software Foundation, Inc.

   This file is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This file is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config/gcn/gcn-opts.h"


/* FIXME */
#define TARGET_CPU_CPP_BUILTINS()

/* Temporarily disable libgcc until one actually exists.  */
#undef  LIBGCC_SPEC
#define LIBGCC_SPEC ""

/* Use LLVM assembler options.  */
#undef ASM_SPEC
#define ASM_SPEC "-triple=amdgcn--amdhsa %{march=*:-mcpu=%*} -filetype=obj"

#undef LINK_SPEC
#define LINK_SPEC ""

/* Support for a compile-time default architecture and tuning.  The rules are:
   --with-arch is ignored if -march is specified.
   --with-tune is ignored if -mtune is specified.  */
#define OPTION_DEFAULT_SPECS \
  {"arch", "%{!march=*:-march=%(VALUE)}" }, \
  {"tune", "%{!mtune=*:-mtune=%(VALUE)}" }

/* Default target_flags if no switches specified.  */
#ifndef TARGET_DEFAULT
#define TARGET_DEFAULT 0
#endif


/* Storage Layout */

#define BITS_BIG_ENDIAN 0

#define BYTES_BIG_ENDIAN 0

#define WORDS_BIG_ENDIAN 0

#define BITS_PER_WORD 32

#define UNITS_PER_WORD (BITS_PER_WORD/BITS_PER_UNIT)

#ifndef LIBGCC2_UNITS_PER_WORD
#define LIBGCC2_UNITS_PER_WORD 8
#endif

#define TARGET_64BIT 1

/* Scratch memory is addressed by buffered meomry accesses that are 32bit.  */
#define POINTER_SIZE 32

#define PARM_BOUNDARY 128

/* FIXME  */
#define STACK_BOUNDARY 128

#define FUNCTION_BOUNDARY 32

#define BIGGEST_ALIGNMENT 512

/* FIXME  */
#define DATA_ALIGNMENT(TYPE,ALIGN) ((ALIGN) > 128 ? (ALIGN) : 128)
#define CONSTANT_ALIGNMENT(TYPE,ALIGN) ((ALIGN) > 128 ? (ALIGN) : 128)
#define LOCAL_ALIGNMENT(TYPE,ALIGN) ((ALIGN) > 128 ? (ALIGN) : 128)

/* FIXME  */
#define EMPTY_FIELD_BOUNDARY 32

#define STRICT_ALIGNMENT 1

/* FIXME  */
#define PCC_BITFIELD_TYPE_MATTERS 1

/* FIXME  */
#define MAX_FIXED_MODE_SIZE 128

#define STACK_SIZE_MODE DImode


/* Type Layout: match what x86-64 does.  */

#define INT_TYPE_SIZE 32

#define LONG_TYPE_SIZE (TARGET_64BIT ? 64 : 32)

#define LONG_LONG_TYPE_SIZE (TARGET_64BIT ? 128 : 64)

#define FLOAT_TYPE_SIZE 32

#define DOUBLE_TYPE_SIZE 64

/* FIXME: software emulated?  */
#define LONG_DOUBLE_TYPE_SIZE 64

#define DEFAULT_SIGNED_CHAR 1

/* Register Basics */
#define FIRST_SGPR_REG		 0
#define LAST_SGPR_REG		 101

#define FLAT_SCRATCH_REG	 102
#define FLAT_SCRATCH_LO_REG	 102
#define FLAT_SCRATCH_HI_REG	 103
#define XNACK_MASK_REG		 104
#define XNACK_MASK_LO_REG	 104
#define XNACK_MASK_HI_REG	 105
#define VCC_LO_REG		 106
#define VCC_HI_REG		 107
#define VCCZ_REG		 108
#define TBA_REG			 109
#define TBA_LO_REG		 109
#define TBA_HI_REG		 110
#define TMA_REG			 111
#define TMA_LO_REG		 111
#define TMA_HI_REG		 112
#define TTMP0_REG		 113
#define TTMP11_REG		 124
#define M0_REG			 125
#define EXEC_REG		 126
#define EXEC_LO_REG		 126
#define EXEC_HI_REG		 127
#define EXECZ_REG		 128
#define SCC_REG			 129
/* 132-159 is reserved; I am lazy to produce masks.  */

#define FIRST_VGPR_REG		 160
#define LAST_VGPR_REG		 415

#define VGPR_REGNO(N) ((N)+FIRST_VGPR_REG)
#define SGPR_REGNO(N) ((N)+FIRST_SGPR_REG)

#define SGPR_OR_VGPR_REGNO_P(N) ((N)>=FIRST_VGPR_REG && (N) <= LAST_SGPR_REG)
#define SGPR_REGNO_P(N) ((N) <= LAST_SGPR_REG)
#define VGPR_REGNO_P(N) ((N)>=FIRST_VGPR_REG && (N) <= LAST_VGPR_REG)
#define CC_REG_P(X) (REG_P (X) && CC_REGNO_P (REGNO (X)))
#define CC_REGNO_P(X) ((X) == SCC_REG || (X) == VCC_REG)

#define FIRST_PSEUDO_REGISTER 416

/* s[16:17] is fixed for exec hack moves.
   Both registers can be probably eliminated and passed to regalloc for general
   purpose with some magic.  */
#define FIXED_REGISTERS {			    \
    /* Scalars.  */				    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		    \
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,		    \
    /* Special regs and padding.  */		    \
/*  flat  xnack vcc      tba   tma   ttmp */        \
    1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
/*                       m0 exec     scc */	    \
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,		    \
    /* VGRPs */					    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
}

#define CALL_USED_REGISTERS {			    \
    /* Scalars.  */				    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 		    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 		    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 		    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 		    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 		    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 		    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 		    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 		    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 		    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,		    \
    /* Special regs and padding.  */		    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,		    \
    /* VGRPs */					    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
}


/* Values in Registers */

#define HARD_REGNO_NREGS(REGNO, MODE) gcn_hard_regno_nregs (REGNO, MODE)
#define HARD_REGNO_MODE_OK(REGNO, MODE) gcn_hard_regno_mode_ok (REGNO, MODE)
#define CANNOT_CHANGE_MODE_CLASS(FROM,TO,CLASS) gcn_cannot_change_mode_class (FROM, TO, CLASS)

#define MODES_TIEABLE_P(MODE1, MODE2) \
  (GET_MODE_BITSIZE (MODE1) <= MAX_FIXED_MODE_SIZE \
   && GET_MODE_BITSIZE (MODE2) <= MAX_FIXED_MODE_SIZE)

/* Register Classes */

enum reg_class {
   NO_REGS,

   /* SCC */
   SCC_CONDITIONAL_REG,

   /* VCCZ */
   VCCZ_CONDITIONAL_REG,

   /* VCC */
   VCC_CONDITIONAL_REG,

   /* EXECZ */
   EXECZ_CONDITIONAL_REG,

   /* SCC VCCZ EXECZ */
   ALL_CONDITIONAL_REGS,

   /* EXEC */
   EXEC_MASK_REG,

   /* SGPR0-101 */
   SGPR_REGS,

   /* SGPR0-101 EXEC_LO/EXEC_HI */
   SGPR_EXEC_REGS,

   /* SGPR0-101, VCC LO/HI, TBA LO/HI, TMA LO/HI, TTMP0-11, M0, EXEC LO/HI,
      VCCZ, EXECZ, SCC
      FIXME: Maybe manual has bug and FLAT_SCRATCH is OK.  */
   SGPR_VOP3A_SRC_REGS,

   /* SGPR0-101, FLAT_SCRATCH_LO/HI, XNACK_MASK_LO/HI, VCC LO/HI, TBA LO/HI
      TMA LO/HI, TTMP0-11 */
   SGPR_MEM_SRC_REGS,

   /* SGPR0-101, FLAT_SCRATCH_LO/HI, XNACK_MASK_LO/HI, VCC LO/HI, TBA LO/HI
      TMA LO/HI, TTMP0-11, M0, EXEC LO/HI */
   SGPR_DST_REGS,

   /* SGPR0-101, FLAT_SCRATCH_LO/HI, XNACK_MASK_LO/HI, VCC LO/HI, TBA LO/HI
      TMA LO/HI, TTMP0-11 */
   SGPR_SRC_REGS,
   GENERAL_REGS,
   VGPR_REGS,
   SRCDST_REGS,
   ALL_REGS,
   LIM_REG_CLASSES
};

#define N_REG_CLASSES (int) LIM_REG_CLASSES

#define REG_CLASS_NAMES \
{  "NO_REGS", \
   "SCC_CONDITIONAL_REG", \
   "VCCZ_CONDITIONAL_REG", \
   "VCC_CONDITIONAL_REG", \
   "EXECZ_CONDITIONAL_REG", \
   "ALL_CONDITIONAL_REGS", \
   "EXEC_MASK_REG", \
   "SGPR_REGS", \
   "SGPR_EXEC_REGS", \
   "SGPR_VOP3A_SRC_REGS", \
   "SGPR_MEM_SRC_REGS", \
   "SGPR_SRC_REGS", \
   "SGPR_DST_REGS", \
   "GENERAL_REGS", \
   "VGPR_REGS", \
   "SRCDST_REGS", \
   "ALL_REGS" \
}

#define NAMED_REG_MASK(N) (1<<((N)-3*32))
#define NAMED_REG_MASK2(N) (1<<((N)-4*32))

#define REG_CLASS_CONTENTS { \
    {0, 0, 0, 0,						\
     0, 0, 0, 0,						\
     0, 0, 0, 0, 0}, /* no regs */ 				\
    {0, 0, 0, 0,						\
     NAMED_REG_MASK2 (SCC_REG), 0, 0, 0,			\
     0, 0, 0, 0}, /* scc reg */					\
    {0, 0, 0, 							\
     NAMED_REG_MASK (VCCZ_REG), 0, 0, 0, 0,			\
     0, 0, 0, 0, 0}, /* vccz reg */				\
    {0, 0, 0, 							\
     NAMED_REG_MASK (VCC_LO_REG)|NAMED_REG_MASK (VCC_HI_REG), 0, 0, 0, 0,\
     0, 0, 0, 0, 0}, /* vccz reg */				\
    {0, 0, 0, 0, 						\
     NAMED_REG_MASK2 (EXECZ_REG),  0, 0, 0,			\
     0, 0, 0, 0}, /* execz reg */				\
    {0, 0, 0, 							\
     NAMED_REG_MASK (VCCZ_REG),    				\
     NAMED_REG_MASK (EXECZ_REG) | NAMED_REG_MASK2 (SCC_REG),	\
     0, 0, 0,							\
     0, 0, 0, 0, 0}, /* all conditional regs */			\
    {0, 0, 0, 							\
     NAMED_REG_MASK (EXEC_LO_REG) | NAMED_REG_MASK (EXEC_HI_REG),\
     0,								\
     0, 0, 0,							\
     0, 0, 0, 0, 0}, /* exec mask reg */       			\
    {0xffffffff, 0xffffffff, 0xffffffff, 0xf1,			\
     0,          0,          0,          0,			\
     0,          0,          0,          0,			\
     0}, 				      /* sgpr regs*/	\
    {0xffffffff, 0xffffffff, 0xffffffff, 0xf1 			\
     | NAMED_REG_MASK (EXEC_LO_REG) | NAMED_REG_MASK (EXEC_HI_REG),\
     0,          0,          0,          0,			\
     0,          0,          0,          0,			\
     0}, 				    /* sgpr exec regs*/	\
    {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff		\
     -NAMED_REG_MASK (FLAT_SCRATCH_LO_REG) 			\
     -NAMED_REG_MASK (FLAT_SCRATCH_HI_REG) 			\
     -NAMED_REG_MASK (XNACK_MASK_LO_REG) 			\
     -NAMED_REG_MASK (XNACK_MASK_HI_REG), 			\
     NAMED_REG_MASK (EXECZ_REG) | NAMED_REG_MASK2 (SCC_REG),	\
     0,          0,          0,					\
     0,          0,          0,          0,			\
     0}, 				    /* sgpr vopra regs*/\
    {0xffffffff, 0xffffffff, 0xffffffff,			\
     0xffffffff-NAMED_REG_MASK (VCCZ_REG)-NAMED_REG_MASK (M0_REG)\
     -NAMED_REG_MASK (EXEC_LO_REG)-NAMED_REG_MASK (EXEC_HI_REG),\
     0,          0,          0,          0,			\
     0,          0,          0,          0,			\
     0}, 				  /* sgpr src mem regs*/\
    {0xffffffff, 0xffffffff, 0xffffffff, 			\
     0xffffffff-NAMED_REG_MASK (VCCZ_REG),			\
     0,		 0,          0,          0,			\
     0,          0,          0,          0,			\
     0}, 				      /* sgpr dst regs*/\
    {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,		\
     NAMED_REG_MASK2 (EXECZ_REG) | NAMED_REG_MASK2 (SCC_REG),   \
     0,          0,          0,					\
     0,          0,          0,          0,			\
     0}, 				      /* sgpr src regs*/\
    {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,		\
     0xffffffff, 0,          0,          0,			\
     0,          0,          0,          0,			\
     0}, 				      /* general regs*/ \
    {0,		 0,	     0,		 0, 			\
     0, 	 0xffffffff, 0xffffffff, 0xffffffff,		\
     0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,		\
     0xffffffff}, 			      /* vector regs */ \
    {0xffffffff, 0xffffffff, 0xffffffff, 			\
     0xffffffff-NAMED_REG_MASK (VCCZ_REG),			\
     0, 	 0xffffffff, 0xffffffff, 0xffffffff,		\
     0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,		\
     0xffffffff}, 				/* srcdst regs*/\
    {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,		\
     0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,		\
     0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,		\
     0xffffffff}} /* all regs*/

#define REGNO_REG_CLASS(REGNO) gcn_regno_reg_class (REGNO)
#define MODE_CODE_BASE_REG_CLASS(MODE, AS, OUTER, INDEX) \
	 gcn_mode_code_base_reg_class (MODE, AS, OUTER, INDEX)
#define REGNO_MODE_CODE_OK_FOR_BASE_P(NUM, MODE, AS, OUTER, INDEX) \
	 gcn_regno_mode_code_ok_for_base_p (NUM, MODE, AS, OUTER, INDEX)
#define INDEX_REG_CLASS VGPR_REGS
#define REGNO_OK_FOR_INDEX_P(regno) regno_ok_for_index_p (regno)

/* Forward declaration so the following compiles.  */
extern short *reg_renumber;

#define INT_REG_OK_FOR_INDEX_P(X, STRICT) \
	((!(STRICT) || REGNO_OK_FOR_INDEX_P (REGNO (X))))
#define INT_REG_OK_FOR_BASE_P(X, STRICT) \
	((!(STRICT) || REGNO_OK_FOR_BASE_P (REGNO (X))))

/* Address spaces.  */
enum gcn_address_spaces {
  ADDR_SPACE_SCRATCH = 0,
  ADDR_SPACE_FLAT,
  ADDR_SPACE_SCALAR_FLAT,
  ADDR_SPACE_LDS,
  ADDR_SPACE_GDS
};
#define REGISTER_TARGET_PRAGMAS() do {					\
c_register_addr_space ("__flat", ADDR_SPACE_FLAT);			\
c_register_addr_space ("__scalar_flat", ADDR_SPACE_SCALAR_FLAT);	\
c_register_addr_space ("__lds", ADDR_SPACE_LDS);			\
c_register_addr_space ("__gds", ADDR_SPACE_LDS);			\
}while (0);

/* Sections */
#define GLOBAL_ASM_OP "\t.globl\t"

/* File Framework */
#undef ASM_DECLARE_FUNCTION_NAME
#define ASM_DECLARE_FUNCTION_NAME(FILE, NAME, DECL)			\
  gcn_hsa_declare_function_name ((FILE), (NAME), (DECL))

#define ASM_APP_ON ""

#define ASM_APP_OFF ""


/* Uninitialized Data */
#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".comm ", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ",%d\n", (ROUNDED)))

#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".lcomm ", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ",%d\n", (ROUNDED)))


/* Label Output */
#define ASM_OUTPUT_LABEL(FILE,NAME)	\
  do { assemble_name (FILE, NAME); fputs (":\n", FILE); } while (0)

#define ASM_OUTPUT_LABELREF(FILE, NAME) \
  asm_fprintf (FILE, "%U%s", default_strip_name_encoding (NAME))

#define ASM_OUTPUT_SYMBOL_REF(FILE, X) \
  do							\
    {							\
      tree decl;					\
      assemble_name (FILE, XSTR ((X), 0));		\
      if ((decl = SYMBOL_REF_DECL ((X))) != 0		\
	  && TREE_CODE (decl) == VAR_DECL		\
	  && TYPE_ADDR_SPACE (TREE_TYPE (decl)))	\
	fputs ("@ppu", FILE);				\
    } while (0)


/* Instruction Output */
#define REGISTER_NAMES \
   {"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9",           \
    "s10", "s11", "s12", "s13", "s14", "s15", "s16", "s17", "s18", "s19", \
    "s20", "s21", "s22", "s23", "s24", "s25", "s26", "s27", "s28", "s29", \
    "s30", "s31", "s32", "s33", "s34", "s35", "s36", "s37", "s38", "s39", \
    "s40", "s41", "s42", "s43", "s44", "s45", "s46", "s47", "s48", "s49", \
    "s50", "s51", "s52", "s53", "s54", "s55", "s56", "s57", "s58", "s59", \
    "s60", "s61", "s62", "s63", "s64", "s65", "s66", "s67", "s68", "s69", \
    "s70", "s71", "s72", "s73", "s74", "s75", "s76", "s77", "s78", "s79", \
    "s80", "s81", "s82", "s83", "s84", "s85", "s86", "s87", "s88", "s89", \
    "s90", "s91", "s92", "s93", "s94", "s95", "s96", "s97", "s98", "s99", \
    "s100", "s101",							  \
    "flat_scratch_lo", "flat_scratch_hi", "xnack_mask_lo", "xnack_mask_hi", \
    "vcc_lo", "vcc_hi", "vccz", "tba_lo", "tba_hi", "tma_lo", "tma_hi",   \
    "ttmp0", "ttmp1", "ttmp2", "ttmp3", "ttmp4", "ttmp5", "ttmp6", "ttmp7", \
    "ttmp8", "ttmp9", "ttmp10", "ttmp11", "m0", "exec_lo", "exec_hi", "execz", \
    "scc", "res130", "res131", "res132", "res133", "res134", "res135",    \
    "res136", "res137", "res138", "res139",				  \
    "res140", "res141", "res142", "res143", "res144", "res145", "res146", \
    "res147", "res148", "res149",					  \
    "res150", "res151", "res152", "res153", "res154", "res155", "res156", \
    "res157", "res158", "res159",					  \
    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9",		  \
    "v10", "v11", "v12", "v13", "v14", "v15", "v16", "v17", "v18", "v19", \
    "v20", "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29", \
    "v30", "v31", "v32", "v33", "v34", "v35", "v36", "v37", "v38", "v39", \
    "v40", "v41", "v42", "v43", "v44", "v45", "v46", "v47", "v48", "v49", \
    "v50", "v51", "v52", "v53", "v54", "v55", "v56", "v57", "v58", "v59", \
    "v60", "v61", "v62", "v63", "v64", "v65", "v66", "v67", "v68", "v69", \
    "v70", "v71", "v72", "v73", "v74", "v75", "v76", "v77", "v78", "v79", \
    "v80", "v81", "v82", "v83", "v84", "v85", "v86", "v87", "v88", "v89", \
    "v90", "v91", "v92", "v93", "v94", "v95", "v96", "v97", "v98", "v99", \
    "v100", "v101", "v102", "v103", "v104", "v105", "v106", "v107", "v108", \
    "v109",								  \
    "v110", "v111", "v112", "v113", "v114", "v115", "v116", "v117", "v118", \
    "v119",								  \
    "v120", "v121", "v122", "v123", "v124", "v125", "v126", "v127", "v128", \
    "v129",								  \
    "v130", "v131", "v132", "v133", "v134", "v135", "v136", "v137", "v138", \
    "v139",								  \
    "v140", "v141", "v142", "v143", "v144", "v145", "v146", "v147", "v148", \
    "v149",								  \
    "v150", "v151", "v152", "v153", "v154", "v155", "v156", "v157", "v158", \
    "v159",								  \
    "v160", "v161", "v162", "v163", "v164", "v165", "v166", "v167", "v168", \
    "v169",								  \
    "v170", "v171", "v172", "v173", "v174", "v175", "v176", "v177", "v178", \
    "v179",								  \
    "v180", "v181", "v182", "v183", "v184", "v185", "v186", "v187", "v188", \
    "v189",								  \
    "v190", "v191", "v192", "v193", "v194", "v195", "v196", "v197", "v198", \
    "v199",								  \
    "v200", "v201", "v202", "v203", "v204", "v205", "v206", "v207", "v208", \
    "v209",								  \
    "v210", "v211", "v212", "v213", "v214", "v215", "v216", "v217", "v218", \
    "v219",								  \
    "v220", "v221", "v222", "v223", "v224", "v225", "v226", "v227", "v228", \
    "v229",								  \
    "v230", "v231", "v232", "v233", "v234", "v235", "v236", "v237", "v238", \
    "v239",								  \
    "v240", "v241", "v242", "v243", "v244", "v245", "v246", "v247", "v248", \
    "v249",								  \
    "v250", "v251", "v252", "v253", "v254", "v255",			  \
    }

#define PRINT_OPERAND(FILE, X, CODE)  print_operand(FILE, X, CODE)

#define PRINT_OPERAND_ADDRESS(FILE, ADDR)  print_operand_address (FILE, ADDR)

#define LOCAL_LABEL_PREFIX "."

#define USER_LABEL_PREFIX ""

#define ASM_COMMENT_START "#"


/* Dispatch Tables */

#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL)  \
  fprintf (FILE, "\t.word .L%d-.L%d\n", VALUE, REL)

#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE)  \
  fprintf (FILE, "\t.word .L%d\n", VALUE)


/* Alignment Output */

#define ASM_OUTPUT_ALIGN(FILE,LOG)  \
  do { if (LOG!=0) fprintf (FILE, "\t.align\t%d\n", 1<<(LOG)); } while (0)


/* Misc */

#define CASE_VECTOR_MODE DImode

#define Pmode SImode

#define FUNCTION_MODE QImode

/* Frame Registers, and other registers */

#define STACK_POINTER_REGNUM FLAT_SCRATCH_REG

/* FIXME: The following declaration is also in gen-protos.h.  Either remove it
   from there or from here if it is not necessary in the following macro.  */
extern unsigned gcn_frame_pointer_regnum ();
/* FIXME.  */
#define HARD_FRAME_POINTER_REGNUM gcn_frame_pointer_regnum ()

#define FRAME_POINTER_REGNUM FLAT_SCRATCH_REG

#define HARD_FRAME_POINTER_IS_ARG_POINTER false
#define HARD_FRAME_POINTER_IS_FRAME_POINTER false
/* There is no arg pointer.  Just choose random fixed register that does
   not intefere with anything.  */
#define ARG_POINTER_REGNUM FLAT_SCRATCH_REG
/* FIXME.  */
#define FUNCTION_ARG_REGNO_P(N) 0

/* Frame Layout all FIXME */

#define STACK_GROWS_DOWNWARD 0

#define FRAME_GROWS_DOWNWARD 0

#define STARTING_FRAME_OFFSET (0)

#define STACK_POINTER_OFFSET 32

#define FIRST_PARM_OFFSET(FNDECL) (0)

#define DYNAMIC_CHAIN_ADDRESS(FP) plus_constant (Pmode, (FP), -16)

#define INITIAL_FRAME_POINTER_OFFSET(N) (N)

/* Register Arguments */

#define GCN_KERNEL_ARG_TYPES 19

struct GTY(()) gcn_kernel_args
{
  long requested;
  int reg[GCN_KERNEL_ARG_TYPES];
  int order[GCN_KERNEL_ARG_TYPES];
  int nargs, nsgprs;
};

typedef struct gcn_args {
  struct gcn_kernel_args args;
  int num;
  int offset;
  int alignment;
} CUMULATIVE_ARGS;

#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,FNDECL,N_NAMED_ARGS) \
  gcn_init_cumulative_args (&(CUM), (FNTYPE), (LIBNAME), (FNDECL), \
			    (N_NAMED_ARGS) != -1)

/* Address spaces.  */

#define MAX_REGS_PER_ADDRESS 2

/* Profiling */

#define FUNCTION_PROFILER(FILE, LABELNO)

#define NO_PROFILE_COUNTERS 1

#define PROFILE_BEFORE_PROLOGUE 0

/* Trampolines */
/*FIXME*/
#define TRAMPOLINE_SIZE (65)
/*FIXME*/
#define TRAMPOLINE_ALIGNMENT 64

/* Misc */

#define MOVE_MAX 16

#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) ((INPREC) <= 32 \
						&& (OUTPREC) <= (INPREC))

struct GTY(()) machine_function
{
  /* Register holding default value of EXEC.  */
  rtx exec_reg;
  bool exec_reg_init;
  struct gcn_kernel_args args;
  int kernarg_segment_alignment;
  int kernarg_segment_byte_size;
};

/* Definitions for register eliminations.

   This is an array of structures.  Each structure initializes one pair
   of eliminable registers.  The "from" register number is given first,
   followed by "to".  Eliminations of the same "from" register are listed
   in order of preference.

   There are two registers that can always be eliminated on the i386.
   The frame pointer and the arg pointer can be replaced by either the
   hard frame pointer or to the stack pointer, depending upon the
   circumstances.  The hard frame pointer is not used before reload and
   so it is not eligible for elimination.  */

#define ELIMINABLE_REGS					\
{{ FRAME_POINTER_REGNUM, 0},				\
 { FRAME_POINTER_REGNUM, 1},				\
 { FRAME_POINTER_REGNUM, 2},				\
 { FRAME_POINTER_REGNUM, 3},				\
 { FRAME_POINTER_REGNUM, 4},				\
 { FRAME_POINTER_REGNUM, 5},				\
 { FRAME_POINTER_REGNUM, 6},				\
 { FRAME_POINTER_REGNUM, 7},				\
 { FRAME_POINTER_REGNUM, 8},				\
 { FRAME_POINTER_REGNUM, 9},				\
 { FRAME_POINTER_REGNUM, 10},				\
 { FRAME_POINTER_REGNUM, 11},				\
 { FRAME_POINTER_REGNUM, 12},				\
 { FRAME_POINTER_REGNUM, 13},				\
 { FRAME_POINTER_REGNUM, 14},				\
 { FRAME_POINTER_REGNUM, 15}}				\

/* Define the offset between two registers, one to be eliminated, and the other
   its replacement, at the start of a routine.  */

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) ((OFFSET) = 0)

#define SLOW_BYTE_ACCESS 0

/* Define this macro if it is advisable to hold scalars in registers
   in a wider mode than that declared by the program.  In such cases,
   the value is constrained to be within the bounds of the declared
   type, but kept valid in the wider mode.  The signedness of the
   extension may differ from that of the type.  */

#define PROMOTE_MODE(MODE,UNSIGNEDP,TYPE)			\
  if (GET_MODE_CLASS (MODE) == MODE_INT				\
      && (TYPE == NULL || TREE_CODE (TYPE) != VECTOR_TYPE)	\
      && GET_MODE_SIZE (MODE) < UNITS_PER_WORD)			\
    {								\
      (MODE) = SImode;						\
    }
