/* Subroutines used for macro/preprocessor support on the MIPS.
   Copyright (C) 2015 Free Software Foundation, Inc.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "machmode.h"
#include "vec.h"
#include "double-int.h"
#include "input.h"
#include "alias.h"
#include "symtab.h"
#include "tree.h"
#include "tm_p.h"
#include "flags.h"
#include "c-family/c-common.h"
#include "c-family/c-pragma.h"
#include "ggc.h"
#include "target.h"
#include "cpplib.h"

#define preprocessing_asm_p() (cpp_get_options (parse_in)->lang == CLK_ASM)
#define builtin_define(TXT) cpp_define (parse_in, TXT)
#define builtin_assert(TXT) cpp_assert (parse_in, TXT)

#define def_or_undef_macro_formatted(define_p, name, vargs...)		\
do {									\
  if (define_p)								\
    cpp_define_formatted (parse_in, name, vargs);			\
  else									\
    def_or_undef_macro (define_p, name);				\
} while (0)

#define def_or_undef_macro(define_p, name)				\
do {									\
  if (define_p)								\
    cpp_define (parse_in, name);					\
  else									\
    {									\
      char *p = xstrdup (name);						\
      char *q = strtok (p, "=");					\
      cpp_undef (parse_in, q);						\
    }									\
} while (0)

/* Define or undefine macros based on the current target.  */

void
mips_target_modify_macros (bool define_p,
			   HOST_WIDE_INT flags,
			   HOST_WIDE_INT ase_flags)
{
  if (TARGET_FLOAT64_P (flags))
    def_or_undef_macro (define_p, "__mips_fpr=64");
  else if (TARGET_FLOATXX_P (flags))
    def_or_undef_macro (define_p, "__mips_fpr=0");
  else
    def_or_undef_macro (define_p, "__mips_fpr=32");

  if (mips_base_compression_flags & MASK_MIPS16)
    def_or_undef_macro (define_p, "__mips16");

  if (TARGET_MIPS3D_P (ase_flags))
    def_or_undef_macro (define_p, "__mips3d");

  if (TARGET_SMARTMIPS_P (ase_flags))
    def_or_undef_macro (define_p, "__mips_smartmips");

  if (mips_base_compression_flags & MASK_MICROMIPS)
    def_or_undef_macro (define_p, "__mips_micromips");

  if (TARGET_MCU_P (ase_flags))
    def_or_undef_macro (define_p, "__mips_mcu");

  if (TARGET_EVA_P (ase_flags))
    def_or_undef_macro (define_p, "__mips_eva");

  if (TARGET_DSP_P (ase_flags))
    {
      def_or_undef_macro (define_p, "__mips_dsp");
      if (TARGET_DSPR2_P (ase_flags))
	{
	  def_or_undef_macro (define_p, "__mips_dspr2");
	  def_or_undef_macro (define_p, "__mips_dsp_rev=2");
	}
      else if (TARGET_DSPR3)
	{
	  def_or_undef_macro (define_p, "__mips_dspr3");
	  def_or_undef_macro (define_p, "__mips_dsp_rev=3");
	}
      else
	def_or_undef_macro (define_p, "__mips_dsp_rev=1");
    }

  if (TARGET_MSA_P (ase_flags))
    {
      def_or_undef_macro (define_p, "__mips_msa");
      def_or_undef_macro (define_p, "__mips_msa_width=128");
    }

  /* Historical Octeon macro.  */
  if (TARGET_OCTEON)
    def_or_undef_macro (define_p, "__OCTEON__");

  MIPS_CPP_SET_PROCESSOR ("_MIPS_ARCH", mips_arch_info);
  MIPS_CPP_SET_PROCESSOR ("_MIPS_TUNE", mips_tune_info);

  if (ISA_MIPS1)
    {
      def_or_undef_macro (define_p, "__mips=1");
      def_or_undef_macro (define_p, "_MIPS_ISA=_MIPS_ISA_MIPS1");
    }
  else if (ISA_MIPS2)
    {
      def_or_undef_macro (define_p, "__mips=2");
      def_or_undef_macro (define_p, "_MIPS_ISA=_MIPS_ISA_MIPS2");
    }
  else if (ISA_MIPS3)
    {
      def_or_undef_macro (define_p, "__mips=3");
      def_or_undef_macro (define_p, "_MIPS_ISA=_MIPS_ISA_MIPS3");
    }
  else if (ISA_MIPS4)
    {
      def_or_undef_macro (define_p, "__mips=4");
      def_or_undef_macro (define_p, "_MIPS_ISA=_MIPS_ISA_MIPS4");
    }
  else if (mips_isa >= 32 && mips_isa < 64)
    {
      def_or_undef_macro (define_p, "__mips=32");
      def_or_undef_macro (define_p, "_MIPS_ISA=_MIPS_ISA_MIPS32");
    }
  else if (mips_isa >= 64)
    {
      def_or_undef_macro (define_p, "__mips=64");
      def_or_undef_macro (define_p, "_MIPS_ISA=_MIPS_ISA_MIPS64");
    }
  if (mips_isa_rev > 0)
    def_or_undef_macro_formatted (define_p, "__mips_isa_rev=%d",
				  mips_isa_rev);

  def_or_undef_macro_formatted (define_p, "_MIPS_FPSET=%d",
				32 / MAX_FPRS_PER_FMT);
  def_or_undef_macro_formatted (define_p, "_MIPS_SPFPSET=%d",
				TARGET_ODD_SPREG ? 32 : 16);

}

/* Function to tell the preprocessor about the defines for the current
   target.  */

void
mips_cpu_cpp_builtins (void)
{
  /* We defined unchangeable macros here and all function specific macros
     will be defined/undefined in mips_target_modify_macros.  */
  builtin_assert ("machine=mips");
  builtin_assert ("cpu=mips");
  builtin_define ("__mips__");
  builtin_define ("_mips");

  /* We do this here because __mips is defined below and so we
     can't use builtin_define_std.  We don't ever want to define
     "mips" for VxWorks because some of the VxWorks headers
     construct include filenames from a root directory macro,
     an architecture macro and a filename, where the architecture
     macro expands to 'mips'.  If we define 'mips' to 1, the
     architecture macro expands to 1 as well.  */
  if (!flag_iso && !TARGET_VXWORKS)
    builtin_define ("mips");

  if (TARGET_64BIT)
    builtin_define ("__mips64");

  /* Treat _R3000 and _R4000 like register-size
     defines, which is how they've historically
     been used.  */
  if (TARGET_64BIT)
    {
      builtin_define_std ("R4000");
      builtin_define ("_R4000");
    }
  else
    {
      builtin_define_std ("R3000");
      builtin_define ("_R3000");
    }

  if (TARGET_BIG_ENDIAN)
    {
      builtin_define_std ("MIPSEB");
      builtin_define ("_MIPSEB");
    }
  else
    {
      builtin_define_std ("MIPSEL");
      builtin_define ("_MIPSEL");
    }

  if (TARGET_SYNCI)
    builtin_define ("__mips_synci");

  /* Macros dependent on the C dialect.  */
  if (preprocessing_asm_p())
    {
      builtin_define_std ("LANGUAGE_ASSEMBLY");
      builtin_define ("_LANGUAGE_ASSEMBLY");
    }
  else if (c_dialect_cxx ())
    {
      builtin_define ("_LANGUAGE_C_PLUS_PLUS");
      builtin_define ("__LANGUAGE_C_PLUS_PLUS");
      builtin_define ("__LANGUAGE_C_PLUS_PLUS__");
    }
  else
    {
      builtin_define_std ("LANGUAGE_C");
      builtin_define ("_LANGUAGE_C");
    }
  if (c_dialect_objc ())
    {
      builtin_define ("_LANGUAGE_OBJECTIVE_C");
      builtin_define ("__LANGUAGE_OBJECTIVE_C");
      /* Bizarre, but retained for backwards compatibility.  */
      builtin_define_std ("LANGUAGE_C");
      builtin_define ("_LANGUAGE_C");
    }

  if (mips_abi == ABI_EABI)
    builtin_define ("__mips_eabi");

  if (TARGET_CACHE_BUILTIN)
    builtin_define ("__GCC_HAVE_BUILTIN_MIPS_CACHE");

  switch (mips_abi)
    {
    case ABI_32:
      builtin_define ("_ABIO32=1");
      builtin_define ("_MIPS_SIM=_ABIO32");
      break;

    case ABI_N32:
      builtin_define ("_ABIN32=2");
      builtin_define ("_MIPS_SIM=_ABIN32");
      break;

    case ABI_64:
      builtin_define ("_ABI64=3");
      builtin_define ("_MIPS_SIM=_ABI64");
      break;

    case ABI_O64:
      builtin_define ("_ABIO64=4");
      builtin_define ("_MIPS_SIM=_ABIO64");
      break;
    }

  /* These defines reflect the ABI in use, not whether the
     FPU is directly accessible.  */
  if (TARGET_NO_FLOAT)
    builtin_define ("__mips_no_float");
  else if (TARGET_HARD_FLOAT_ABI)
    builtin_define ("__mips_hard_float");
  else
    builtin_define ("__mips_soft_float");

  if (TARGET_SINGLE_FLOAT)
    builtin_define ("__mips_single_float");

  if (TARGET_PAIRED_SINGLE_FLOAT)
    builtin_define ("__mips_paired_single_float");

  if (mips_abs == MIPS_IEEE_754_2008)
    builtin_define ("__mips_abs2008");

  if (mips_nan == MIPS_IEEE_754_2008)
    builtin_define ("__mips_nan2008");

  if (mips_c_lib == MIPS_LIB_SMALL)
    builtin_define ("__mips_clib_small");

  if (mips_c_lib == MIPS_LIB_TINY)
    builtin_define ("__mips_clib_tiny");

  /* Whether calls should go through $25.  The separate __PIC__
     macro indicates whether abicalls code might use a GOT.  */
  if (TARGET_ABICALLS)
    builtin_define ("__mips_abicalls");

  /* Whether Loongson vector modes are enabled.  */
  if (TARGET_LOONGSON_VECTORS)
    builtin_define ("__mips_loongson_vector_rev");

  cpp_define_formatted (parse_in, "_MIPS_SZINT=%d", INT_TYPE_SIZE);
  cpp_define_formatted (parse_in, "_MIPS_SZLONG=%d", LONG_TYPE_SIZE);
  cpp_define_formatted (parse_in, "_MIPS_SZPTR=%d", POINTER_SIZE);

  mips_target_modify_macros (true, target_flags, mips_ase_flags);
}

/* Implement TARGET_OPTION_PRAGMA.  The hook validates the current
   #pragma GCC target, sets the state, updates the macros based on what was
   changed.  */

bool
mips_pragma_target_parse (tree args, tree pop_target)
{
  tree prev_tree = build_target_option_node (&global_options);
  tree cur_tree;
  struct cl_target_option *prev_opt;
  struct cl_target_option *cur_opt;
  HOST_WIDE_INT prev_flags;
  HOST_WIDE_INT cur_flags;
  HOST_WIDE_INT diff_flags;
  HOST_WIDE_INT prev_ase_flags;
  HOST_WIDE_INT cur_ase_flags;
  HOST_WIDE_INT diff_ase_flags;

  if (! args)
    {
      cur_tree = (pop_target ? pop_target : target_option_default_node);
      cl_target_option_restore (&global_options,
				TREE_TARGET_OPTION (cur_tree));
    }
  else
    {
      cur_tree = mips_valid_attribute_tree (args, &global_options);

      if (!cur_tree)
       {
	 cl_target_option_restore (&global_options,
				   TREE_TARGET_OPTION (prev_tree));
	 return false;
       }
    }

  target_option_current_node = cur_tree;
  mips_reset_previous_fndecl ();

  /* Figure out the previous/current isa, arch, tune and the differences.  */
  prev_opt  = TREE_TARGET_OPTION (prev_tree);
  cur_opt   = TREE_TARGET_OPTION (cur_tree);
  prev_flags = prev_opt->x_target_flags;
  cur_flags  = cur_opt->x_target_flags;
  diff_flags = (prev_flags ^ cur_flags);
  prev_ase_flags = prev_opt->x_mips_ase_flags;
  cur_ase_flags  = cur_opt->x_mips_ase_flags;
  diff_ase_flags = (prev_ase_flags ^ cur_ase_flags);

  /* Undef all of the macros for that are no longer current.  */
  mips_target_modify_macros (false,
			     prev_flags & diff_flags,
			     prev_ase_flags & diff_ase_flags);

  /* Switch off warnings about the newly defined macros for -Wunused-macros.
     There is no need to warn about the compiler predefined macros.  */
  cpp_options *cpp_opts = cpp_get_options (parse_in);
  unsigned char saved_warn_unused_macros = cpp_opts->warn_unused_macros;
  cpp_opts->warn_unused_macros = 0;

  /* Define all of the macros for new options that were just turned on.  */
  mips_target_modify_macros (true,
			     cur_flags & diff_flags,
			     cur_ase_flags & diff_ase_flags);

  cpp_opts->warn_unused_macros = saved_warn_unused_macros;

  return true;
}

/* Register target pragmas.  */

void
mips_register_target_pragmas (void)
{
  targetm.target_option.pragma_parse = mips_pragma_target_parse;
}
