/* Definitions for GCC.  Part of the machine description for cil32.

   Copyright (C) 2006 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Bona
   Andrea Ornstein
   Erven Rohou
   Roberto Costa

Contact information at STMicroelectronics:
Roberto Costa <roberto.costa@st.com>   */

#define DISABLE_RTL_PASSES 1

#define IN_NAMED_SECTION(DECL) 0
#undef ASM_OUTPUT_ALIGN
#define ASM_OUTPUT_ALIGN(FILE,LOG) do { } while(0)
#undef ASM_DECLARE_OBJECT_NAME
#define ASM_DECLARE_OBJECT_NAME(FILE, NAME, DECL) do { } while(0)

/*  make_decl_cil (stream, decl); */
#define TARGET_DECLARE_VARIABLE(STREAM,DECL) make_decl_cil(STREAM,DECL)

/* Node: Driver */

#define CC1_SPEC "%{!mnoopensystemc:-mopensystemc}"
#define LIB_SPEC ""
#define LIBGCC_SPEC ""
#define LINK_SPEC "--no-stdlib %{!nostdlib:-l gcc4net.dll} %{shared:--format dll}"
#define STARTFILE_SPEC ""
#define ENDFILE_SPEC ""

/* Node: Run-time Target */

#define TARGET_CPU_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define_std ("__cil32__");		\
      builtin_define_std ("__GNU_CIL__");	\
      builtin_assert ("cpu=cil32");		\
      builtin_assert ("machine=cil32");		\
    }						\
  while(0)

/* This needs to be at least 32 bits.  */
extern int target_flags;

/* Print subsidiary information on the compiler version in use.
   Do not use VD.D syntax (D=digit), since this will cause confusion
   with the base gcc version among users, when we ask which version of
   gcc-cil32 they are using.  Please use some flavor of "R<number>" for
   the version (no need for major.minor versions, I believe).  */
#define TARGET_VERSION \
 fprintf (stderr, " [cil32]")

/* Node: Storage Layout */

#define BITS_BIG_ENDIAN 0
#define BYTES_BIG_ENDIAN 0
#define WORDS_BIG_ENDIAN 0

#define UNITS_PER_WORD 4
#define UNITS_PER_SIMD_WORD 4

/* Unused by cil32 machine */
#define PARM_BOUNDARY 32

/* Unused by cil32 machine */
#define STACK_BOUNDARY 64

/* Unused by cil32 machine */
#define FUNCTION_BOUNDARY 16

#define BIGGEST_ALIGNMENT 64

#define STRICT_ALIGNMENT 1

#define PCC_BITFIELD_TYPE_MATTERS 1

#define MAX_FIXED_MODE_SIZE 64


/* Node: Type Layout */

/* Note that DOUBLE_TYPE_SIZE is not defined anymore, since the default
   value gives a 64-bit double, which is what we now use.  */

/* For compatibility and historical reasons, a char should be signed.  */
#define DEFAULT_SIGNED_CHAR 1

/* Note that WCHAR_TYPE_SIZE is used in cexp.y,
   where TARGET_SHORT is not available.  */
#undef WCHAR_TYPE
#define WCHAR_TYPE "int"


/* Node: Register Basics */

#define FIRST_PSEUDO_REGISTER (1)
#define FIXED_REGISTERS {0}
#define CALL_USED_REGISTERS {1}

/* Node: Allocation Order */

#define REG_ALLOC_ORDER {0}


/* Node: Values in Registers */

/* The VOIDmode test is so we can omit mode on anonymous insns.  FIXME:
   Still needed in 2.9x, at least for Axis-20000319.  */
#define HARD_REGNO_NREGS(REGNO, MODE)	\
 (MODE == VOIDmode \
  ? 1 : ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD))

#define HARD_REGNO_MODE_OK(REGNO, MODE)	1

#define MODES_TIEABLE_P(MODE1, MODE2) 1


/* Node: Leaf Functions */
/* (no definitions) */

/* Node: Stack Registers */
/* (no definitions) */


/* Node: Register Classes */

enum reg_class
  {
    NO_REGS,
    GENERAL_REGS, ALL_REGS,
    LIM_REG_CLASSES
  };

#define N_REG_CLASSES (int) LIM_REG_CLASSES

#define REG_CLASS_NAMES							\
  {"NO_REGS",								\
   "GENERAL_REGS", "ALL_REGS"}

/* Count in the faked argument register in GENERAL_REGS.  Keep out SRP.  */
#define REG_CLASS_CONTENTS			\
  {						\
   {0},						\
   {1},                  			\
   {1}                                          \
  }

#define REGNO_REG_CLASS(REGNO) GENERAL_REGS

#define BASE_REG_CLASS GENERAL_REGS

#define INDEX_REG_CLASS GENERAL_REGS

#define REG_CLASS_FROM_LETTER(C) NO_REGS

#define REGNO_OK_FOR_BASE_P(REGNO)		\
  ((REGNO) == CIL32_FAKE_REG)

#define REGNO_OK_FOR_INDEX_P(REGNO) REGNO_OK_FOR_BASE_P(REGNO)

#define PREFERRED_RELOAD_CLASS(X, CLASS) GENERAL_REGS

#define SECONDARY_RELOAD_CLASS(CLASS, MODE, X) NO_REGS

#define CLASS_MAX_NREGS(CLASS, MODE)					\
 ((MODE) == VOIDmode							\
  ? 1                                                    		\
  : ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD))

#define CONST_OK_FOR_CONSTRAINT_P(VALUE, C, STR) 0

#define CONST_DOUBLE_OK_FOR_CONSTRAINT_P(VALUE, C, STR) 0

/* We want the unary operators visible for constraint checking.
   (will always be the case in 4.2).  */
#define KEEP_UNARY_OPERATORS_AT_CONSTRAINT_CHECKING


/* Node: Frame Layout */

#define STACK_GROWS_DOWNWARD
#define FRAME_GROWS_DOWNWARD 1

/* It seems to be indicated in the code (at least 2.1) that this is
   better a constant, and best 0.  */
#define STARTING_FRAME_OFFSET 0

#define FIRST_PARM_OFFSET(FNDECL) 0

#define INCOMING_RETURN_ADDR_RTX gen_rtx_REG (Pmode, CIL32_FAKE_REG)

#define EH_RETURN_DATA_REGNO(N) CIL32_FAKE_REG

#define EH_RETURN_STACKADJ_RTX gen_rtx_REG (SImode, CIL32_FAKE_REG)

#define DWARF_FRAME_RETURN_COLUMN DWARF_FRAME_REGNUM (CIL32_FAKE_REG)

/* FIXME: Move this to right node (it's not documented properly yet).
   FIXME: Check what alignment we can assume regarding
   TARGET_STACK_ALIGN and TARGET_ALIGN_BY_32.  */
#define DWARF_CIE_DATA_ALIGNMENT -1

/* If we would ever need an exact mapping between canonical register
   number and dwarf frame register, we would either need to include all
   registers in the gcc description (with some marked fixed of course), or
   an inverse mapping from dwarf register to gcc register.  There is one
   need in dwarf2out.c:expand_builtin_init_dwarf_reg_sizes.  Right now, I
   don't see that we need exact correspondence between DWARF *frame*
   registers and DBX_REGISTER_NUMBER, so map them onto GCC registers.  */
#define DWARF_FRAME_REGNUM(REG) (REG)

/* Node: Stack Checking */
/* (no definitions) */

/* Node: Frame Registers */

#define STACK_POINTER_REGNUM CIL32_FAKE_REG

/* Register used for frame pointer.  This is also the last of the saved
   registers, when a frame pointer is not used.  */
#define FRAME_POINTER_REGNUM CIL32_FAKE_REG

/* Faked register, is always eliminated.  We need it to eliminate
   allocating stack slots for the return address and the frame pointer.  */
#define ARG_POINTER_REGNUM CIL32_FAKE_REG

#define STATIC_CHAIN_REGNUM CIL32_FAKE_REG


/* Node: Elimination */

/* Really only needed if the stack frame has variable length (alloca
   or variable sized local arguments (GNU C extension).  */
#define FRAME_POINTER_REQUIRED 0

#define INITIAL_FRAME_POINTER_OFFSET(n) \
  do { n = 0; } while(0)

/* We need not worry about when the frame-pointer is required for other
   reasons.  */
#define CAN_ELIMINATE(FROM, TO) 1

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) (gcc_unreachable ())


/* Node: Stack Arguments */

#define ACCUMULATE_OUTGOING_ARGS 1

#define RETURN_POPS_ARGS(FUNDECL, FUNTYPE, STACKSIZE) 0


/* Node: Register Arguments */

/* The void_type_node is sent as a "closing" call.  */
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED)			\
 (NULL_RTX)

/* This no longer *needs* to be a structure; but keeping it as such should
   not hurt (and hacking the ABI is simpler).  */
#define CUMULATIVE_ARGS struct cum_args
struct cum_args {int regs;};

/* The regs member is an integer, the number of arguments got into
   registers so far.  */
#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, FNDECL, N_NAMED_ARGS) \
 ((CUM).regs = 0)

#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)		\
 ((CUM).regs += 1)

#define FUNCTION_ARG_REGNO_P(REGNO) 0


/* Node: Scalar Return */

#define FUNCTION_VALUE(VALTYPE, FUNC)  \
 gen_rtx_REG (TYPE_MODE (VALTYPE), CIL32_FAKE_REG)

#define LIBCALL_VALUE(MODE) gen_rtx_REG (MODE, CIL32_FAKE_REG)

#define FUNCTION_VALUE_REGNO_P(N) ((N) == CIL32_FAKE_REG)


/* Node: Aggregate Return */
/* (no definitions) */

/* Node: Caller Saves */
/* (no definitions) */

/* Node: Function entry */
/* (no definitions) */

/* Node: Profiling */

#define FUNCTION_PROFILER(FILE, LABELNO)  \
 error ("no FUNCTION_PROFILER for CIL32 yet")

/* FIXME: Some of the undefined macros might be mandatory.  If so, fix
   documentation.  */


/* Node: Trampolines */
/* TODO */

#define TRAMPOLINE_SIZE 0

#define TRAMPOLINE_ALIGNMENT 0

#define INITIALIZE_TRAMPOLINE(TRAMP, FNADDR, CXT) \
   do {} while(0)

/* Node: Library Calls */

/* If you change this, you have to check whatever libraries and systems
   that use it.  */
#define TARGET_EDOM 33


/* Node: Addressing Modes */

#define CONSTANT_ADDRESS_P(X) CONSTANT_P (X)

#define MAX_REGS_PER_ADDRESS 2

#define CONSTANT_INDEX_P(X) \
 (CONSTANT_P (X))

#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)                 \
 {								\
   if (REG_P (X))			                        \
     goto ADDR;							\
   if (CONSTANT_INDEX_P (X))					\
     goto ADDR;							\
   if (GET_CODE (X) == PLUS)					\
     {								\
       rtx x1 = XEXP (X, 0);					\
       rtx x2 = XEXP (X, 1);					\
       /* BDAP o, Rd.  */					\
       if ((REG_P (x1) || CONSTANT_INDEX_P (x1))		\
	   && (REG_P (x2) && CONSTANT_INDEX_P (x2)))		\
	 goto ADDR;						\
     }								\
 }

#ifndef REG_OK_STRICT
 /* Nonzero if X is a hard reg that can be used as a base reg
    or if it is a pseudo reg.  */
# define REG_OK_FOR_BASE_P(X) 1
#else
 /* Nonzero if X is a hard reg that can be used as a base reg.  */
# define REG_OK_FOR_BASE_P(X) REGNO_OK_FOR_BASE_P (REGNO (X))
#endif

#ifndef REG_OK_STRICT
 /* Nonzero if X is a hard reg that can be used as an index
    or if it is a pseudo reg.  */
# define REG_OK_FOR_INDEX_P(X) REG_OK_FOR_BASE_P (X)
#else
 /* Nonzero if X is a hard reg that can be used as an index.  */
# define REG_OK_FOR_INDEX_P(X) REGNO_OK_FOR_INDEX_P (REGNO (X))
#endif

#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR, LABEL)	\
  do {} while(0)

#define LEGITIMATE_CONSTANT_P(X) 1


/* Node: Condition Code */

#define NOTICE_UPDATE_CC(EXP, INSN) (gcc_unreachable ())


/* Node: Costs */

#define SLOW_BYTE_ACCESS 0


/* Node: Sections */

#define TEXT_SECTION_ASM_OP ""

#define DATA_SECTION_ASM_OP ""


/* Node: PIC */
/* (no definitions) */

/* Node: File Framework */

#undef  ASM_OUTPUT_SKIP
#define ASM_OUTPUT_SKIP(FILE, SIZE) \
  do {} while(0)

#define ASM_APP_ON ""
#define ASM_APP_OFF ""


/* Node: Data Output */

#define IS_ASM_LOGICAL_LINE_SEPARATOR(C) (C) == '@'

/* Node: Uninitialized Data */

#define ASM_OUTPUT_ALIGNED_DECL_COMMON(FILE, DECL, NAME, SIZE, ALIGN) \
 do {} while(0)

#define ASM_OUTPUT_ALIGNED_DECL_LOCAL(FILE, DECL, NAME, SIZE, ALIGN) \
 do {} while(0)

/* Node: Label Output */

/* Globalizing directive for a label.  */
#define GLOBAL_ASM_OP "\t.global "

#define SUPPORTS_WEAK 1

/* Remove any previous definition (elfos.h).  */
#undef ASM_GENERATE_INTERNAL_LABEL
#define ASM_GENERATE_INTERNAL_LABEL(LABEL, PREFIX, NUM)	\
  sprintf (LABEL, "*%s%s%ld", LOCAL_LABEL_PREFIX, PREFIX, (long) NUM)

/* Node: Initialization */
/* (no definitions) */

/* Node: Macros for Initialization */
/* (no definitions) */

/* Node: Instruction Output */

#define REGISTER_NAMES {"fake"}

#define PRINT_OPERAND(FILE, X, CODE) (gcc_unreachable ())

/* For delay-slot handling.  */
#define PRINT_OPERAND_PUNCT_VALID_P(CODE) 0

#define PRINT_OPERAND_ADDRESS(FILE, ADDR) (gcc_unreachable ())

#define LOCAL_LABEL_PREFIX "."

#define REGISTER_PREFIX "$"

#define ASM_OUTPUT_REG_PUSH(FILE, REGNO) (gcc_unreachable ())
#define ASM_OUTPUT_REG_POP(FILE, REGNO) (gcc_unreachable ())


/* Node: Dispatch Tables */
/* (no definitions) */

/* Node: Exception Region Output */
/* (no definitions) */

/* Node: Alignment Output */

/* Node: All Debuggers */

#define DBX_REGISTER_NUMBER(REGNO) (REGNO)


/* Node: DBX Options */
/* (no definitions) */

/* Node: DBX Hooks */
/* (no definitions) */

/* Node: File names and DBX */
/* (no definitions) */


/* Node: SDB and DWARF */
/* (no definitions) */

/* Node: Misc */

#define CASE_VECTOR_MODE HImode

/* The maximum number of bytes that a single instruction can move quickly
   between memory and registers or between two memory locations.  */
#define MOVE_MAX 8
#define MOVE_MAX_PIECES 4

/* Determine whether to use move_by_pieces or block move insn.  */
#define MOVE_BY_PIECES_P(SIZE, ALIGN)		\
  ( (SIZE) == 1 || (SIZE) == 2 || (SIZE) == 4 || (SIZE) == 8 )

/* Determine whether to use clear_by_pieces or block clear insn.  */
#define CLEAR_BY_PIECES_P(SIZE, ALIGN) MOVE_BY_PIECES_P (SIZE, ALIGN)

/* This macro is used to determine whether store_by_pieces should be
   called to "memset" storage with byte values other than zero, or
   to "memcpy" storage when the source is a constant string.  */
#define STORE_BY_PIECES_P(SIZE, ALIGN) MOVE_BY_PIECES_P (SIZE, ALIGN)

#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) 1

#define Pmode SImode

#define FUNCTION_MODE QImode

#define NO_IMPLICIT_EXTERN_C

extern struct tree_opt_pass pass_gen_cil;
extern struct tree_opt_pass pass_simp_cil;

/* cil32 builtin ID */
enum cil32_builtin
{
  CIL32_BUILTIN_VA_ARG,
  CIL32_BUILTIN_IS_LITTLE_ENDIAN
};

extern tree cil32_is_LE_decl;

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * indent-tabs-mode: t
 * End:
 */
