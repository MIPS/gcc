/* Definitions of target machine for GNU compiler,
   for IBM RS/6000 POWER running AIX.
   Copyright (C) 2000 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* Yes!  We are AIX!  */
#define DEFAULT_ABI ABI_AIX
#define TARGET_OBJECT_FORMAT OBJECT_XCOFF

/* The RS/6000 uses the XCOFF format.  */

#define XCOFF_DEBUGGING_INFO

/* Define if the object format being used is COFF or a superset.  */
#define OBJECT_FORMAT_COFF

/* Define the magic numbers that we recognize as COFF.
 
    AIX 4.3 adds U803XTOCMAGIC (0757) for 64-bit objects, but collect2.c
    does not include files in the correct order to conditionally define
    the symbolic name in this macro.
 
    The AIX linker accepts import/export files as object files,
    so accept "#!" (0x2321) magic number.  */
#define MY_ISCOFF(magic) \
  ((magic) == U802WRMAGIC || (magic) == U802ROMAGIC \
   || (magic) == U802TOCMAGIC || (magic) == 0757 || (magic) == 0x2321)

/* This is the only version of nm that collect2 can work with.  */
#define REAL_NM_FILE_NAME "/usr/ucb/nm"

/* We don't have GAS for the RS/6000 yet, so don't write out special
    .stabs in cc1plus.  */

#define FASCIST_ASSEMBLER

/* AIX does not have any init/fini or ctor/dtor sections, so create
    static constructors and destructors as normal functions.  */
/* #define ASM_OUTPUT_CONSTRUCTOR(file, name) */
/* #define ASM_OUTPUT_DESTRUCTOR(file, name) */

/* The prefix to add to user-visible assembler symbols. */
#define USER_LABEL_PREFIX "."

/* Don't turn -B into -L if the argument specifies a relative file name.  */
#define RELATIVE_PREFIX_NOT_LINKDIR

/* Names to predefine in the preprocessor for this target machine.  */

#define CPP_PREDEFINES "-D_IBMR2 -D_POWER -D_AIX -D_AIX32 -D_LONG_LONG \
-Asystem(unix) -Asystem(aix) -Acpu(rs6000) -Amachine(rs6000)"

/* Tell the assembler to assume that all undefined names are external.

   Don't do this until the fixed IBM assembler is more generally available.
   When this becomes permanently defined, the ASM_OUTPUT_EXTERNAL,
   ASM_OUTPUT_EXTERNAL_LIBCALL, and RS6000_OUTPUT_BASENAME macros will no
   longer be needed.  Also, the extern declaration of mcount in ASM_FILE_START
   will no longer be needed.  */

/* #define ASM_SPEC "-u %(asm_cpu)" */

/* Default location of syscalls.exp under AIX */
#ifndef CROSS_COMPILE
#define LINK_SYSCALLS_SPEC "-bI:/lib/syscalls.exp"
#else
#define LINK_SYSCALLS_SPEC ""
#endif

/* Default location of libg.exp under AIX */
#ifndef CROSS_COMPILE
#define LINK_LIBG_SPEC "-bexport:/usr/lib/libg.exp"
#else
#define LINK_LIBG_SPEC ""
#endif

/* Define the options for the binder: Start text at 512, align all segments
   to 512 bytes, and warn if there is text relocation.

 The -bhalt:4 option supposedly changes the level at which ld will abort,
   but it also suppresses warnings about multiply defined symbols and is
   used by the AIX cc command.  So we use it here.

   -bnodelcsect undoes a poor choice of default relating to multiply-defined
   csects.  See AIX documentation for more information about this.

 -bM:SRE tells the linker that the output file is Shared REusable.  Note
   that to actually build a shared library you will also need to specify an
   export list with the -Wl,-bE option.  */

#define LINK_SPEC "-T512 -H512 %{!r:-btextro} -bhalt:4 -bnodelcsect\
%{static:-bnso %(link_syscalls) } \
%{!shared:%{g*: %(link_libg) }} %{shared:-bM:SRE}"

/* Profiled library versions are used by linking with special directories.  */
#define LIB_SPEC "%{pg:-L/lib/profiled -L/usr/lib/profiled}\
%{p:-L/lib/profiled -L/usr/lib/profiled} %{!shared:%{g*:-lg}} -lc"

#define ASM_OUTPUT_EXTERNAL(FILE, DECL, NAME)	\
{ rtx _symref = XEXP (DECL_RTL (DECL), 0);	\
  if ((TREE_CODE (DECL) == VAR_DECL		\
       || TREE_CODE (DECL) == FUNCTION_DECL)	\
      && (NAME)[strlen (NAME) - 1] != ']')	\
    {						\
      char *_name = (char *) permalloc (strlen (XSTR (_symref, 0)) + 5); \
      strcpy (_name, XSTR (_symref, 0));	\
      strcat (_name, TREE_CODE (DECL) == FUNCTION_DECL ? "[DS]" : "[RW]"); \
      XSTR (_symref, 0) = _name;		\
    }						\
}

/* Output at end of assembler file.

   On the RS/6000, referencing data should automatically pull in text.  */

#define ASM_FILE_END(FILE)					\
{								\
  text_section ();						\
  fputs ("_section_.text:\n", FILE);				\
  data_section ();						\
  fputs (TARGET_32BIT						\
	 ? "\t.long _section_.text\n" : "\t.llong _section_.text\n", FILE); \
}

/* Define the extra sections we need.  We define three: one is the read-only
   data section which is used for constants.  This is a csect whose name is
   derived from the name of the input file.  The second is for initialized
   global variables.  This is a csect whose name is that of the variable.
   The third is the TOC.  */

#define EXTRA_SECTIONS \
   read_only_data, private_data, read_only_private_data, toc, bss

/* Define the routines to implement these extra sections.
   BIGGEST_ALIGNMENT is 64, so align the sections that much.  */

#define EXTRA_SECTION_FUNCTIONS				\
							\
void							\
read_only_data_section ()				\
{							\
  if (in_section != read_only_data)			\
    {							\
      fprintf (asm_out_file, "\t.csect %s[RO],3\n",	\
	       xcoff_read_only_section_name);		\
      in_section = read_only_data;			\
    }							\
}							\
							\
void							\
private_data_section ()					\
{							\
  if (in_section != private_data)			\
    {							\
      fprintf (asm_out_file, "\t.csect %s[RW],3\n",	\
	       xcoff_private_data_section_name);	\
      in_section = private_data;			\
    }							\
}							\
							\
void							\
read_only_private_data_section ()			\
{							\
  if (in_section != read_only_private_data)		\
    {							\
      fprintf (asm_out_file, "\t.csect %s[RO],3\n",	\
	       xcoff_private_data_section_name);	\
      in_section = read_only_private_data;		\
    }							\
}							\
							\
void							\
toc_section ()						\
{							\
  if (TARGET_MINIMAL_TOC)				\
    {							\
      /* toc_section is always called at least once from ASM_FILE_START, \
	 so this is guaranteed to always be defined once and only once   \
	 in each file.  */						 \
      if (! toc_initialized)				\
	{						\
	  fputs ("\t.toc\nLCTOC..0:\n", asm_out_file);	\
	  fputs ("\t.tc toc_table[TC],toc_table[RW]\n", asm_out_file); \
	  toc_initialized = 1;				\
	}						\
							\
      if (in_section != toc)				\
	fprintf (asm_out_file, "\t.csect toc_table[RW]%s\n",	\
		 (TARGET_32BIT ? "" : ",3"));		\
    }							\
  else							\
    {							\
      if (in_section != toc)				\
        fputs ("\t.toc\n", asm_out_file);		\
    }							\
  in_section = toc;					\
}

/* Define the name of our readonly data section.  */

#define READONLY_DATA_SECTION read_only_data_section

/* Select the section for an initialized data object.

   On the RS/6000, we have a special section for all variables except those
   that are static.  */

#define SELECT_SECTION(EXP,RELOC)			\
{							\
  if ((TREE_CODE (EXP) == STRING_CST			\
       && ! flag_writable_strings)			\
      || (TREE_CODE_CLASS (TREE_CODE (EXP)) == 'd'	\
	  && TREE_READONLY (EXP) && ! TREE_THIS_VOLATILE (EXP) \
	  && DECL_INITIAL (EXP)				\
	  && (DECL_INITIAL (EXP) == error_mark_node	\
	      || TREE_CONSTANT (DECL_INITIAL (EXP)))	\
	  && ! (RELOC)))				\
    {							\
      if (TREE_PUBLIC (EXP))				\
        read_only_data_section ();			\
      else						\
        read_only_private_data_section ();		\
    }							\
  else							\
    {							\
      if (TREE_PUBLIC (EXP))				\
        data_section ();				\
      else						\
        private_data_section ();			\
    }							\
}

/* Indicate that jump tables go in the text section.  */

#define JUMP_TABLES_IN_TEXT_SECTION 1

/* Enable AIX XL compiler calling convention breakage compatibility.  */
#undef TARGET_XL_CALL
#define MASK_XL_CALL		0x40000000
#define	TARGET_XL_CALL		(target_flags & MASK_XL_CALL)
#undef  SUBTARGET_SWITCHES
#define SUBTARGET_SWITCHES		\
  {"xl-call", 		MASK_XL_CALL,					\
   "Always pass floating-point arguments in memory" },			\
  {"no-xl-call",	- MASK_XL_CALL,					\
   "Don't always pass floating-point arguments in memory" },		\
  SUBSUBTARGET_SWITCHES
#define SUBSUBTARGET_SWITCHES 

/* Define any extra SPECS that the compiler needs to generate.  */
#undef  SUBTARGET_EXTRA_SPECS
#define SUBTARGET_EXTRA_SPECS						\
  { "link_syscalls",            LINK_SYSCALLS_SPEC },			\
  { "link_libg",                LINK_LIBG_SPEC }

/* FP save and restore routines.  */
#define	SAVE_FP_PREFIX "._savef"
#define SAVE_FP_SUFFIX ""
#define	RESTORE_FP_PREFIX "._restf"
#define RESTORE_FP_SUFFIX ""

/* Function name to call to do profiling.  */
#define RS6000_MCOUNT ".__mcount"

/* AIX always has a TOC.  */
#define TARGET_NO_TOC		0
#define	TARGET_TOC		1

/* AIX allows r13 to be used.  */
#define FIXED_R13 0
