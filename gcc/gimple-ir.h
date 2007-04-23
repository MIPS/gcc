/* Gimple IR definitions.

   Copyright 2007 Free Software Foundation, Inc.
   Contributed by Aldy Hernandez <aldyh@redhat.com>

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
02110-1301, USA.  */

#ifndef GCC_GIMPLE_IR_H
#define GCC_GIMPLE_IR_H

enum gs_code {
#define DEFGSCODE(SYM, STRING)	SYM,
#include "gs.def"
#undef DEFGSCODE
    LAST_AND_UNUSED_GS_CODE
};

#define GS_CODE(G) ((G)->base.code)
#define GS_SUBCODE_FLAGS(G) ((G)->base.subcode_flags)
#define GS_NEXT(G) ((G)->base.next)
#define GS_PREV(G) ((G)->base.prev)

/* A sequences of gimple statements.  */
#define GS_SEQ_FIRST(S)	(S)->first
#define GS_SEQ_LAST(S)		(S)->last
#define GS_SEQ_INIT {0, 0}
struct gs_sequence
{
  gimple first;
  gimple last;
};
typedef struct gs_sequence *gs_seq;

struct gimple_statement_base GTY(())
{
  unsigned int code : 16;
  unsigned int subcode_flags : 16;
  gimple next;
  gimple prev;
  struct basic_block_def *bb;
  source_locus locus;
  tree block;
};

struct gimple_statement_with_ops GTY(())
{
  struct gimple_statement_base base;
  unsigned modified : 1;
  bitmap address_taken;
  struct def_optype_d GTY((skip)) *def_ops;
  struct use_optype_d GTY((skip)) *use_ops;
};

struct gimple_statement_with_memory_ops GTY(())
{
  struct gimple_statement_with_ops with_ops;
  unsigned has_volatile_ops : 1;
  struct voptype_d GTY((skip)) *vdef_ops;
  struct voptype_d GTY((skip)) *vuse_ops;
  bitmap stores;
  bitmap loads;
};

struct gimple_statement_omp GTY(())
{
     struct gimple_statement_base base;
     struct gs_sequence body;
};

/* Gimple tuples.  */

/* GS_BIND */
struct gimple_statement_bind GTY(())
{
  struct gimple_statement_base base;
  tree vars;
  struct gs_sequence body;
};

/* GS_CATCH */
struct gimple_statement_catch GTY(())
{
  struct gimple_statement_base base;
  tree types;
  gimple handler;
};

/* GS_EH_FILTER */
struct gimple_statement_eh_filter GTY(())
{
  struct gimple_statement_base base;
  /* Filter types.  */
  tree types;
  /* Failure actions.  */
  gimple failure;
};

/* GS_LABEL */
struct gimple_statement_label GTY(())
{
  struct gimple_statement_base base;
  tree label;
};

/* GS_PHI */
struct gimple_statement_phi GTY(())
{
  struct gimple_statement_base base;
  unsigned capacity;
  unsigned nargs;
  tree result;
  struct phi_arg_d GTY ((length ("%h.nargs"))) args[1];
};

/* GS_RESX */
struct gimple_statement_resx GTY(())
{
  struct gimple_statement_base base;
  /* Exception region number.  */
  int region;
};

/* GS_TRY */
struct gimple_statement_try GTY(())
{
  struct gimple_statement_base base;
  /* Expression to evaluate.  */
  gimple eval;
  /* Cleanup expression.  */
  gimple cleanup;
};

/* GS_ASSIGN */
struct gimple_statement_assign_binary GTY(())
{
  struct gimple_statement_with_ops with_ops;
  tree op[3];
};

struct gimple_statement_assign_unary_reg GTY(())
{
  struct gimple_statement_with_ops with_ops;
  tree op[2];
};

struct gimple_statement_assign_unary_mem GTY(())
{
  struct gimple_statement_with_memory_ops with_mem_ops;
  tree op[2];
};

/* GS_COND */
struct gimple_statement_cond GTY(())
{
  struct gimple_statement_with_ops with_ops;
  tree op[2];
  struct gimple_statement_label *label_true;
  struct gimple_statement_label *label_false;
};

/* GS_GOTO */
struct gimple_statement_goto GTY(())
{
  struct gimple_statement_with_ops with_ops;
  tree dest;
};

/* GS_SWITCH */
struct gimple_statement_switch GTY(())
{
  struct gimple_statement_with_ops with_ops;
  unsigned int nlabels;
  tree index;
  tree default_label;
  tree GTY ((length ("%h.nlabels + 1"))) labels[1];
};

/* GS_ASM */
struct gimple_statement_asm GTY(())
{
  struct gimple_statement_with_memory_ops with_mem_ops;
  const char *string;
  /* Number of inputs.  */
  unsigned ni;
  /* Number of outputs.  */
  unsigned no;
  /* Number of clobbers.  */
  unsigned nc;
  tree GTY ((length ("%h.ni"))) op[1];
};

/* GS_CALL */
struct gimple_statement_call GTY(())
{
  struct gimple_statement_with_memory_ops with_mem_ops;
  tree lhs;
  tree fn;
  tree chain;
  unsigned long nargs;
  tree GTY ((length ("%h.nargs"))) args[1];
};

/* GS_RETURN */
struct gimple_statement_return GTY(())
{
  struct gimple_statement_with_memory_ops with_mem_ops;
  tree retval;
};

/* GS_OMP_CRITICAL */
struct gimple_statement_omp_critical GTY(())
{
  struct gimple_statement_omp omp;
  /* Critical section name.  */
  tree name;
};

/* GS_OMP_FOR */
struct gimple_statement_omp_for GTY(())
{
  struct gimple_statement_omp omp;
  tree clauses;
  /* Index variable.  */
  tree index;
  /* Initial value.  */
  tree initial;
  /* Final value.  */
  tree final;
  /* Increment.  */
  tree incr;
  /* Pre-body evaluated before the loop body begins.  */
  struct gs_sequence pre_body;
};

/* GS_OMP_PARALLEL */
struct gimple_statement_omp_parallel GTY(())
{
  struct gimple_statement_omp omp;
  tree clauses;
  tree child_fn;
  /* Shared data argument.  */
  tree data_arg;
};

/* GS_OMP_SECTION */
/* Uses struct gimple_statement_omp.  */

/* GS_OMP_SECTIONS */
struct gimple_statement_omp_sections GTY(())
{
  struct gimple_statement_omp omp;
  tree clauses;
};

/* GS_OMP_SINGLE */
struct gimple_statement_omp_single GTY(())
{
  struct gimple_statement_omp omp;
  tree clauses;
};

enum gimple_statement_structure_enum {
#define DEFGSSTRUCT(SYM, STRING)	SYM,
#include "gsstruct.def"
#undef DEFGSSTRUCT
    LAST_GSS_ENUM
};

/* Define the overall contents of a gimple tuple.  It may be any of the
   structures declared above for various types of tuples.  */

union gimple_statement_d GTY ((desc ("gimple_statement_structure (&%h)")))
{
  struct gimple_statement_base GTY ((tag ("GSS_BASE"))) base;
  /* We never really return GSS_WITH_OPS or GSS_WITH_MEM_OPS in
     gimple_statement_structure() since they're subsumed by the other
     tuples.  We only include the tags for completeness.  */
  struct gimple_statement_with_ops GTY ((tag ("GSS_WITH_OPS"))) with_ops;
  struct gimple_statement_with_memory_ops GTY ((tag ("GSS_WITH_MEM_OPS"))) with_mem_ops;
  struct gimple_statement_omp GTY ((tag ("GSS_OMP"))) omp;

  struct gimple_statement_bind GTY ((tag ("GSS_BIND"))) gs_bind;
  struct gimple_statement_catch GTY ((tag ("GSS_CATCH"))) gs_catch;
  struct gimple_statement_eh_filter GTY ((tag ("GSS_EH_FILTER"))) gs_eh_filter;
  struct gimple_statement_label GTY ((tag ("GSS_LABEL"))) gs_label;
  struct gimple_statement_phi GTY ((tag ("GSS_PHI"))) gs_phi;
  struct gimple_statement_resx GTY ((tag ("GSS_RESX"))) gs_resx;
  struct gimple_statement_try GTY ((tag ("GSS_TRY"))) gs_try;
  struct gimple_statement_assign_binary GTY ((tag ("GSS_ASSIGN_BINARY"))) gs_assign_binary;
  struct gimple_statement_assign_unary_reg GTY ((tag ("GSS_ASSIGN_UNARY_REG"))) gs_assign_unary_reg;
  struct gimple_statement_assign_unary_mem GTY ((tag ("GSS_ASSIGN_UNARY_MEM"))) gs_assign_unary_mem;
  struct gimple_statement_cond GTY ((tag ("GSS_COND"))) gs_cond;
  struct gimple_statement_goto GTY ((tag ("GSS_GOTO"))) gs_goto;
  struct gimple_statement_switch GTY ((tag ("GSS_SWITCH"))) gs_switch;
  struct gimple_statement_asm GTY ((tag ("GSS_ASM"))) gs_asm;
  struct gimple_statement_call GTY ((tag ("GSS_CALL"))) gs_call;
  struct gimple_statement_return GTY ((tag ("GSS_RETURN"))) gs_return;
  struct gimple_statement_omp_critical GTY ((tag ("GSS_OMP_CRITICAL"))) gs_omp_critical;
  struct gimple_statement_omp_for GTY ((tag ("GSS_OMP_FOR"))) gs_omp_for;
  struct gimple_statement_omp_parallel GTY ((tag ("GSS_OMP_PARALLEL"))) gs_omp_parallel;
  struct gimple_statement_omp_sections GTY ((tag ("GSS_OMP_SECTIONS"))) gs_omp_sections;
  struct gimple_statement_omp_single GTY ((tag ("GSS_OMP_SINGLE"))) gs_omp_single;
};

/* Error out if a gimple tuple is addressed incorrectly.  */
#if defined ENABLE_TREE_CHECKING && (GCC_VERSION >= 2007)

extern void gs_check_failed (const gimple, const char *, int, const char *, \
    			     unsigned int, unsigned int) ATTRIBUTE_NORETURN;

#define GS_CHECK(GS, CODE) __extension__				\
  ({  const gimple __gs = (GS);						\
      if (GS_CODE (__gs) != (CODE))					\
        gs_check_failed (__gs, __FILE__, __LINE__, __FUNCTION__,	\
	  		 (CODE), 0);					\
      __gs; })

#define GS_CHECK2(GS, CODE1, CODE2) __extension__			\
  ({  const gimple __gs = (GS);						\
      if (GS_CODE (__gs) != (CODE1)					\
	  || GS_SUBCODE_FLAGS (__gs) != (CODE2))			\
        gs_check_failed (__gs, __FILE__, __LINE__, __FUNCTION__,	\
	  		 (CODE1), (CODE2));				\
      __gs; })
#else  /* not ENABLE_TREE_CHECKING, or not gcc */
#define GS_CHECK(GS, CODE)	(GS)
#define GS_CHECK2(GS, C1, C2)	(GS)
#endif

/* GIMPLE IR accessor functions.  */

/* GS_ASSIGN accessors.  */
static inline tree
gs_assign_operand (gimple gs, int opno)
{
  enum gimple_statement_structure gss;

  GS_CHECK (gs, GS_ASSIGN);

  gss = gimple_statement_structure (gs);
  if (gss == GSS_ASSIGN_BINARY)
    return gs->gs_assign_binary.op[opno];
  else if (gss == GSS_ASSIGN_UNARY_REG)
    return gs->gs_assign_unary_reg[opno];
  else if (gss == GSS_ASSIGN_UNARY_MEM)
    return gs->gs_assign_unary_mem[opno];

  gcc_unreachable ();
  return NULL;
}

static inline tree
gs_assign_operand_lhs (gimple gs)
{
  return gs_assign_operand (gs, 0);
}

static inline tree
gs_assign_operand_rhs (gimple gs)
{
  return gs_assign_operand (gs, 1);
}

/* GS_RETURN accessors.  */
static inline tree *
gs_return_operand_retval (gimple gs)
{
  return &GS_CHECK (gs, GS_RETURN)->gs_return.retval;
}

#define GS_RETURN_OPERAND_RETVAL(G) (*gs_return_operand_retval ((G)))

#endif  /* GCC_GIMPLE_IR_H */

extern gimple gs_build_return (bool, tree);
extern enum gimple_statement_structure_enum gimple_statement_structure (gimple);
extern void gs_add (gimple, gs_seq);
