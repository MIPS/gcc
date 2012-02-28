/* Definitions of target machine needed for option handling for GNU compiler,
   for IBM RS/6000.
   Copyright (C) 2010, 2011
   Free Software Foundation, Inc.
   Contributed by Michael Meissner (meissner@linux.vnet.ibm.com)

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef RS6000_OPTS_H
#define RS6000_OPTS_H

/* Processor type.  Order must match cpu attribute in MD file.  */
enum processor_type
 {
   PROCESSOR_RIOS1,
   PROCESSOR_RIOS2,
   PROCESSOR_RS64A,
   PROCESSOR_MPCCORE,
   PROCESSOR_PPC403,
   PROCESSOR_PPC405,
   PROCESSOR_PPC440,
   PROCESSOR_PPC476,
   PROCESSOR_PPC601,
   PROCESSOR_PPC603,
   PROCESSOR_PPC604,
   PROCESSOR_PPC604e,
   PROCESSOR_PPC620,
   PROCESSOR_PPC630,
   PROCESSOR_PPC750,
   PROCESSOR_PPC7400,
   PROCESSOR_PPC7450,
   PROCESSOR_PPC8540,
   PROCESSOR_PPCE300C2,
   PROCESSOR_PPCE300C3,
   PROCESSOR_PPCE500MC,
   PROCESSOR_PPCE500MC64,
   PROCESSOR_POWER4,
   PROCESSOR_POWER5,
   PROCESSOR_POWER6,
   PROCESSOR_POWER7,
   PROCESSOR_CELL,
   PROCESSOR_PPCA2,
   PROCESSOR_TITAN
};

/* FP processor type.  */
enum fpu_type_t
{
  FPU_NONE,			/* No FPU */
  FPU_SF_LITE,			/* Limited Single Precision FPU */
  FPU_DF_LITE,			/* Limited Double Precision FPU */
  FPU_SF_FULL,			/* Full Single Precision FPU */
  FPU_DF_FULL			/* Full Double Single Precision FPU */
};

/* Types of costly dependences.  */
enum rs6000_dependence_cost
{
  max_dep_latency = 1000,
  no_dep_costly,
  all_deps_costly,
  true_store_to_load_dep_costly,
  store_to_load_dep_costly
};

/* Types of nop insertion schemes in sched target hook sched_finish.  */
enum rs6000_nop_insertion
{
  sched_finish_regroup_exact = 1000,
  sched_finish_pad_groups,
  sched_finish_none
};

/* Dispatch group termination caused by an insn.  */
enum group_termination
{
  current_group,
  previous_group
};

/* Enumeration to give which calling sequence to use.  */
enum rs6000_abi {
  ABI_NONE,
  ABI_AIX,			/* IBM's AIX */
  ABI_V4,			/* System V.4/eabi */
  ABI_DARWIN			/* Apple's Darwin (OS X kernel) */
};

/* Small data support types.  */
enum rs6000_sdata_type {
  SDATA_NONE,			/* No small data support.  */
  SDATA_DATA,			/* Just put data in .sbss/.sdata, don't use relocs.  */
  SDATA_SYSV,			/* Use r13 to point to .sdata/.sbss.  */
  SDATA_EABI			/* Use r13 like above, r2 points to .sdata2/.sbss2.  */
};

/* Type of traceback to use.  */
enum  rs6000_traceback_type {
  traceback_default = 0,
  traceback_none,
  traceback_part,
  traceback_full
};

/* Code model for 64-bit linux.
   small: 16-bit toc offsets.
   medium: 32-bit toc offsets, static data and code within 2G of TOC pointer.
   large: 32-bit toc offsets, no limit on static data and code.  */
enum rs6000_cmodel {
  CMODEL_SMALL,
  CMODEL_MEDIUM,
  CMODEL_LARGE
};

/* Describe which vector unit to use for a given machine mode.  */
enum rs6000_vector {
  VECTOR_NONE,			/* Type is not  a vector or not supported */
  VECTOR_ALTIVEC,		/* Use altivec for vector processing */
  VECTOR_VSX,			/* Use VSX for vector processing */
  VECTOR_PAIRED,		/* Use paired floating point for vectors */
  VECTOR_SPE,			/* Use SPE for vector processing */
  VECTOR_OTHER			/* Some other vector unit */
};

/* Bitmasks used for all integer conditional mode operations (absolute value,
   minimum, maximu, conditional move, set condtional, etc.).  */
#define COND_MODE_NONE		0x0000	/* Use MI code.  */
#define COND_MODE_ISEL		0x0001	/* Use ISEL. */
#define COND_MODE_BCP8		0x0002	/* Use branch cond+8.   */
#define COND_MODE_SHIFT		0x0004	/* Use shift/xor/neg. */
#define COND_MODE_UNSET		-1	/* Preferences not yet set.  */

/* Describe how to do integer ABS and negative ABS.  */
enum rs6000_iabs_t {
  IABS_UNSET		= COND_MODE_UNSET,
  IABS_NONE		= COND_MODE_NONE,
  IABS_ISEL_ONLY	= COND_MODE_ISEL,
  IABS_BCP8_ONLY	= COND_MODE_BCP8,
  IABS_SHIFT_ONLY	= COND_MODE_SHIFT,

  IABS_ISEL		= (COND_MODE_SHIFT | COND_MODE_ISEL),
  IABS_BCP8		= (COND_MODE_SHIFT | COND_MODE_BCP8),
  IABS_SHIFT		= COND_MODE_SHIFT
};

#define IABS_BIT_P(MASK) (((unsigned)rs6000_iabs_method & (MASK)) != 0)

#define IABS_CLEAR_BIT(MASK)						\
  (rs6000_iabs_method							\
   = (enum rs6000_iabs_t)(((unsigned)rs6000_iabs_method) & ~(MASK)))

#define IABS_SET(VALUE) (rs6000_iabs_method = (enum rs6000_iabs_t)(VALUE))

/* Describe how to do integer minimum and maximum.  These are represented as
   bit masks, so that if a particular pattern is not valid (such as the
   presence of the ISEL instruction), other patterns can be tried.  */
enum rs6000_iminmax_t {
  IMINMAX_UNSET		= COND_MODE_UNSET,
  IMINMAX_NONE		= COND_MODE_NONE,
  IMINMAX_ISEL		= COND_MODE_ISEL,
  IMINMAX_BCP8		= COND_MODE_BCP8
};

#define IMINMAX_BIT_P(MASK) (((unsigned)rs6000_iminmax_method & (MASK)) != 0)

#define IMINMAX_CLEAR_BIT(MASK)						\
  (rs6000_iminmax_method						\
   = (enum rs6000_iminmax_t)(((unsigned)rs6000_iminmax_method) & ~(MASK)))

#define IMINMAX_SET(VALUE)						\
  (rs6000_iminmax_method = (enum rs6000_iminmax_t)(VALUE))

/* No enumeration is defined to index the -mcpu= values (entries in
   processor_target_table), with the type int being used instead, but
   we need to distinguish the special "native" value.  */
#define RS6000_CPU_OPTION_NATIVE -1

#endif
