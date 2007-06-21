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
#include "gimple.def"
#undef DEFGSCODE
    LAST_AND_UNUSED_GS_CODE
};

#define GS_CODE(G) ((G)->base.code)
#define GS_SUBCODE_FLAGS(G) ((G)->base.subcode_flags)
#define GS_NEXT(G) ((G)->base.next)
#define GS_PREV(G) ((G)->base.prev)
#define GS_LOCUS(G) ((G)->base.locus)
#define GS_LOCUS_EMPTY_P(G)	(GS_LOCUS ((G)).file == NULL \
				 && GS_LOCUS ((G)).line == 0)

/* A sequences of gimple statements.  */
#define GS_SEQ_FIRST(S)	(S)->first
#define GS_SEQ_LAST(S)		(S)->last
#define GS_SEQ_INIT(SEQ)	((SEQ).first = NULL, (SEQ).last = NULL)
#define GS_SEQ_EMPTY_P(S) (!GS_SEQ_FIRST ((S)))

struct gs_sequence
{
  gimple first;
  gimple last;
};

struct gimple_statement_base GTY(())
{
  ENUM_BITFIELD(gs_code) code : 16;
  unsigned int subcode_flags : 16;
  gimple next;
  gimple prev;
  struct basic_block_def *bb;
  location_t locus;
  tree block;
};

struct gimple_statement_with_ops GTY(())
{
  struct gimple_statement_base base;
  unsigned modified : 1;
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

#define GS_TRY_CATCH 1 << 0
#define GS_TRY_FINALLY 1 << 1

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
  tree label_true;
  tree label_false;
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
  tree GTY ((length ("%h.ni+%h.no+%h.nc"))) op[1];
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

/* Predicate for conds. */
enum gs_cond {
  COND_LT, COND_GT, COND_LE, COND_GE, COND_EQ, COND_NE
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

/* Prototypes.  */

extern gimple gs_build_return (bool, tree);
extern gimple gs_build_assign (tree, tree);
extern gimple gs_build_call (tree, int, ...);
extern gimple gs_build_cond (enum gs_cond, tree, tree, tree, tree);
extern gimple gs_build_label (tree label);
extern gimple gs_build_goto (tree dest);
extern gimple gs_build_nop (void);
extern gimple gs_build_bind (tree, gs_seq);
extern gimple gs_build_asm (const char *, unsigned, unsigned, unsigned, ...);
extern gimple gs_build_catch (tree, gimple);
extern gimple gs_build_eh_filter (tree, gimple);
extern gimple gs_build_try (gimple, gimple, bool, bool);
extern gimple gs_build_phi (unsigned, unsigned, tree, ...);
extern gimple gs_build_resx (int);
extern gimple gs_build_switch (unsigned int, tree, tree, ...);
extern gimple gs_omp_build_parallel (struct gs_sequence, tree, tree, tree);
extern gimple gs_omp_build_for (struct gs_sequence, tree, tree, tree, tree,
    				tree, struct gs_sequence, enum gs_cond);
extern gimple gs_omp_build_critical (struct gs_sequence, tree);
extern gimple gs_omp_build_section (struct gs_sequence);
extern gimple gs_omp_build_continue (struct gs_sequence);
extern gimple gs_omp_build_master (struct gs_sequence);
extern gimple gs_omp_build_return (struct gs_sequence);
extern gimple gs_omp_build_ordered (struct gs_sequence);
extern gimple gs_omp_build_sections (struct gs_sequence, tree);
extern gimple gs_omp_build_single (struct gs_sequence, tree);
extern enum gimple_statement_structure_enum gimple_statement_structure (gimple);
extern void gs_add (gimple, gs_seq);
extern enum gimple_statement_structure_enum gss_for_assign (enum tree_code);

extern const char *const gs_code_name[];

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
  enum gimple_statement_structure_enum gss;

  GS_CHECK (gs, GS_ASSIGN);

  gss = gimple_statement_structure (gs);
  if (gss == GSS_ASSIGN_BINARY)
    return gs->gs_assign_binary.op[opno];
  else if (gss == GSS_ASSIGN_UNARY_REG)
    return gs->gs_assign_unary_reg.op[opno];
  else if (gss == GSS_ASSIGN_UNARY_MEM)
    return gs->gs_assign_unary_mem.op[opno];

  gcc_unreachable ();
}

static inline void
gs_assign_set_operand (gimple gs, size_t opno, tree op)
{
  enum gimple_statement_structure_enum gss;

  GS_CHECK (gs, GS_ASSIGN);

  gss = gimple_statement_structure (gs);
  if (gss == GSS_ASSIGN_BINARY) 
    {
      gcc_assert (opno <= 2);
      gs->gs_assign_binary.op[opno] = op;
      return;
    }
  else if (gss == GSS_ASSIGN_UNARY_REG)
    {
      gcc_assert (opno <= 1);
      gs->gs_assign_unary_reg.op[opno] = op;
      return;
    }
  else if (gss == GSS_ASSIGN_UNARY_MEM)
    {
      gcc_assert (opno <= 1);
      gs->gs_assign_unary_mem.op[opno] = op;
      return;
    }

  gcc_unreachable ();
}

static inline tree
gs_assign_lhs (gimple gs)
{
  return gs_assign_operand (gs, 0);
}

static inline void
gs_assign_set_lhs (gimple gs, tree lhs)
{
  gs_assign_set_operand (gs, 0, lhs);
}

static inline tree
gs_assign_binary_rhs1 (gimple gs)
{
  return gs_assign_operand (gs, 1);
}

static inline void
gs_assign_binary_set_rhs1 (gimple gs, tree rhs)
{
  gs_assign_set_operand (gs, 1, rhs);
}

static inline tree
gs_assign_binary_rhs2 (gimple gs)
{
  return gs_assign_operand (gs, 2);
}

static inline void
gs_assign_binary_set_rhs2 (gimple gs, tree rhs)
{
  gs_assign_set_operand (gs, 2, rhs);
}

static inline tree
gs_assign_unary_rhs (gimple gs)
{
#if defined ENABLE_CHECKING
  enum gimple_statement_structure_enum gss;
  gss = gss_for_assign (GS_SUBCODE_FLAGS (gs));
#endif
  return gs_assign_operand (gs, 1);
}

static inline void
gs_assign_unary_set_rhs (gimple gs, tree rhs)
{
  gs_assign_set_operand (gs, 1, rhs);
}


/* GS_CALL accessors. */

static inline tree
gs_call_fn (gimple gs)
{
  GS_CHECK (gs, GS_CALL);
  return gs->gs_call.fn;
}

static inline void
gs_call_set_fn (gimple gs, tree fn)
{
  GS_CHECK (gs, GS_CALL);
  gs->gs_call.fn = fn;
}

static inline tree
gs_call_lhs (gimple gs)
{
  GS_CHECK (gs, GS_CALL);
  return gs->gs_call.lhs;
}

static inline void
gs_call_set_lhs (gimple gs, tree lhs)
{
  GS_CHECK (gs, GS_CALL);
  gs->gs_call.lhs = lhs;
}

static inline tree
gs_call_chain (gimple gs)
{
  GS_CHECK (gs, GS_CALL);
  return gs->gs_call.chain;
}

static inline void
gs_call_set_chain (gimple gs, tree chain)
{
  GS_CHECK (gs, GS_CALL);
  gs->gs_call.chain = chain;
}

static inline unsigned long
gs_call_nargs (gimple gs)
{
  GS_CHECK (gs, GS_CALL);
  return gs->gs_call.nargs;
}

static inline void
gs_call_set_nargs (gimple gs, unsigned long nargs)
{
  GS_CHECK (gs, GS_CALL);
  gs->gs_call.nargs = nargs;
}

static inline tree
gs_call_arg (gimple gs, int index)
{
  GS_CHECK (gs, GS_CALL);
  return gs->gs_call.args[index];
}

static inline void
gs_call_set_arg (gimple gs, int index, tree arg)
{
  GS_CHECK (gs, GS_CALL);
  gs->gs_call.args[index] = arg;
}


/* GS_COND accessors. */

static inline tree
gs_cond_lhs (gimple gs)
{
  GS_CHECK (gs, GS_COND);
  return gs->gs_cond.op[0];
}

static inline void
gs_cond_set_lhs (gimple gs, tree lhs)
{
  GS_CHECK (gs, GS_COND);
  gs->gs_cond.op[0] = lhs;
}

static inline tree
gs_cond_rhs (gimple gs)
{
  GS_CHECK (gs, GS_COND);
  return gs->gs_cond.op[1];
}

static inline void
gs_cond_set_rhs (gimple gs, tree rhs)
{
  GS_CHECK (gs, GS_COND);
  gs->gs_cond.op[1] = rhs;
}

static inline tree
gs_cond_true_label (gimple gs)
{
  GS_CHECK (gs, GS_COND);
  return gs->gs_cond.label_true;
}

static inline void
gs_cond_set_true_label (gimple gs, tree label)
{
  GS_CHECK (gs, GS_COND);
  gs->gs_cond.label_true = label;
}

static inline void
gs_cond_set_false_label (gimple gs, tree label)
{
  GS_CHECK (gs, GS_COND);
  gs->gs_cond.label_false = label;
}

static inline tree
gs_cond_false_label (gimple gs)
{
  GS_CHECK (gs, GS_COND);
  return gs->gs_cond.label_false;
}

/* GS_LABEL accessors. */

static inline tree
gs_label_label (gimple gs)
{
  GS_CHECK (gs, GS_LABEL);
  return gs->gs_label.label;
}
static inline void
gs_label_set_label (gimple gs, tree label)
{
  GS_CHECK (gs, GS_LABEL);
  gs->gs_label.label = label;
}

/* GS_GOTO accessors. */

static inline tree
gs_goto_dest (gimple gs)
{
  GS_CHECK (gs, GS_GOTO);
  return gs->gs_goto.dest;
}
static inline void 
gs_goto_set_dest (gimple gs, tree dest)
{
  GS_CHECK (gs, GS_GOTO);
  gs->gs_goto.dest =  dest;
}

/* GS_BIND accessors. */

static inline tree
gs_bind_vars (gimple gs)
{
  GS_CHECK (gs, GS_BIND);
  return gs->gs_bind.vars;
}

static inline void
gs_bind_set_vars (gimple gs, tree vars)
{
  GS_CHECK (gs, GS_BIND);
  gs->gs_bind.vars = vars;
}

static inline struct gs_sequence *
gs_bind_body (gimple gs) {
  GS_CHECK (gs, GS_BIND);
  return &(gs->gs_bind.body);
}

static inline void
gs_bind_set_body (gimple gs, struct gs_sequence * gss)
{
  GS_CHECK (gs, GS_BIND);
  gs->gs_bind.body.first = gss->first;
  gs->gs_bind.body.last = gss->last;
}

/* GS_ASM accessors. */
static inline unsigned int
gs_asm_ni (gimple gs)
{
  GS_CHECK (gs, GS_ASM);
  return gs->gs_asm.ni;
}

static inline void
gs_asm_set_ni (gimple gs, unsigned ni)
{
  GS_CHECK (gs, GS_ASM);
  gs->gs_asm.ni = ni;
}

static inline unsigned int
gs_asm_no (gimple gs)
{
  GS_CHECK (gs, GS_ASM);
  return gs->gs_asm.no;
}

static inline void
gs_asm_set_no (gimple gs, unsigned no)
{
  GS_CHECK (gs, GS_ASM);
  gs->gs_asm.no = no;
}

static inline unsigned int
gs_asm_nc (gimple gs)
{
  GS_CHECK (gs, GS_ASM);
  return gs->gs_asm.nc;
}

static inline void
gs_asm_set_nc (gimple gs, unsigned nc)
{
  GS_CHECK (gs, GS_ASM);
  gs->gs_asm.nc = nc;
}

static inline tree
gs_asm_in_op (gimple gs, unsigned int index)
{
  GS_CHECK (gs, GS_ASM);
  gcc_assert(index <= gs->gs_asm.ni);
  return gs->gs_asm.op[index];
}

static inline void
gs_asm_set_in_op (gimple gs, unsigned int index, tree in_op)
{
  GS_CHECK (gs, GS_ASM);
  gcc_assert(index <= gs->gs_asm.ni);
  gs->gs_asm.op[index] = in_op;
}

static inline tree
gs_asm_out_op (gimple gs, unsigned int index)
{
  GS_CHECK (gs, GS_ASM);
  gcc_assert(index <= gs->gs_asm.no);
  return gs->gs_asm.op[index + gs->gs_asm.ni];
}

static inline void
gs_asm_set_out_op (gimple gs, unsigned int index, tree out_op)
{
  GS_CHECK (gs, GS_ASM);
  gcc_assert (index <= gs->gs_asm.no);
  gs->gs_asm.op[index + gs->gs_asm.ni] = out_op;
}

static inline tree
gs_asm_clobber_op (gimple gs, unsigned int index)
{
  GS_CHECK (gs, GS_ASM);
  gcc_assert (index <= gs->gs_asm.nc);
  return gs->gs_asm.op[index + gs->gs_asm.ni + gs->gs_asm.no];
}

static inline void
gs_asm_set_clobber_op (gimple gs, unsigned int index, tree clobber_op)
{
  GS_CHECK (gs, GS_ASM);
  gcc_assert(index <= gs->gs_asm.nc);
  gs->gs_asm.op[index + gs->gs_asm.ni + gs->gs_asm.no] = clobber_op;
}

static inline const char *
gs_asm_string (gimple gs)
{
  GS_CHECK (gs, GS_ASM);
  return gs->gs_asm.string;
}

static inline void
gs_asm_set_string (gimple gs, const char* string) 
{
  GS_CHECK (gs, GS_ASM);
  gs->gs_asm.string = string;
}

/* GS_CATCH accessors. */

static inline tree
gs_catch_types (gimple gs)
{
 GS_CHECK (gs, GS_CATCH);
 return gs->gs_catch.types;
}

static inline gimple
gs_catch_handler (gimple gs)
{
  GS_CHECK (gs, GS_CATCH);
  return gs->gs_catch.handler;
}

static inline void
gs_catch_set_types (gimple gs, tree t)
{
  GS_CHECK (gs, GS_CATCH);
  gs->gs_catch.types = t;
}

static inline void
gs_catch_set_handler (gimple gs, gimple handler)
{
  GS_CHECK (gs, GS_CATCH);
  gs->gs_catch.handler = handler;
}

/* GS_EH_FILTER accessors. */

static inline tree
gs_eh_filter_types (gimple gs)
{
  GS_CHECK (gs, GS_EH_FILTER);
  return gs->gs_eh_filter.types;
}

static inline gimple
gs_eh_filter_failure (gimple gs)
{
  GS_CHECK (gs, GS_EH_FILTER);
  return gs->gs_eh_filter.failure;
}

static inline void
gs_eh_filter_set_types (gimple gs, tree types)
{
  GS_CHECK (gs, GS_EH_FILTER);
  gs->gs_eh_filter.types = types;
}

static inline void
gs_eh_filter_set_failure (gimple gs, gimple failure)
{
  GS_CHECK (gs, GS_EH_FILTER);
  gs->gs_eh_filter.failure = failure;
}


/* GS_TRY accessors. */

static inline gimple
gs_try_eval (gimple gs)
{
  GS_CHECK (gs, GS_TRY);
  return gs->gs_try.eval;
}

static inline gimple
gs_try_cleanup (gimple gs)
{
  GS_CHECK (gs, GS_TRY);
  return gs->gs_try.cleanup;
}

static inline void
gs_try_set_eval (gimple gs, gimple eval)
{
  GS_CHECK (gs, GS_TRY);
  gs->gs_try.eval = eval;
}

static inline void
gs_try_set_cleanup (gimple gs, gimple cleanup)
{
    GS_CHECK (gs, GS_TRY);
    gs->gs_try.cleanup = cleanup;
}

/* GS_PHI accessors. */

static inline unsigned int
gs_phi_capacity (gimple gs)
{
    GS_CHECK (gs, GS_PHI);
    return gs->gs_phi.capacity;
}

static inline void
gs_phi_set_capacity (gimple gs, unsigned int capacity)
{
  GS_CHECK (gs, GS_PHI);
  gs->gs_phi.capacity = capacity;
}

static inline unsigned int
gs_phi_nargs (gimple gs)
{
  GS_CHECK (gs, GS_PHI);
  return gs->gs_phi.nargs;
}

static inline void
gs_phi_set_nargs (gimple gs, unsigned int nargs)
{
  GS_CHECK (gs, GS_PHI);
  gs->gs_phi.nargs = nargs;
}

static inline tree
gs_phi_result (gimple gs)
{
  GS_CHECK (gs, GS_PHI);
  return gs->gs_phi.result;
}

static inline void
gs_phi_set_result (gimple gs, tree result)
{
  GS_CHECK (gs, GS_PHI);
  gs->gs_phi.result = result;
}

static inline struct phi_arg_d *
gs_phi_arg (gimple gs, unsigned int index)
{
  GS_CHECK (gs, GS_PHI);
  gcc_assert (index <= gs->gs_phi.nargs);
  return &(gs->gs_phi.args[index]);
}

static inline void
gs_phi_set_arg (gimple gs, unsigned int index, struct phi_arg_d * phiarg)
{
  GS_CHECK (gs, GS_PHI);
  gcc_assert (index <= gs->gs_phi.nargs);
  memcpy (gs->gs_phi.args + index, phiarg, sizeof(struct phi_arg_d));
}

/* GS_RESX accessors. */

static inline int
gs_resx_region (gimple gs)
{
  GS_CHECK (gs, GS_RESX);
  return gs->gs_resx.region;
}

static inline void
gs_resx_set_region (gimple gs, int region)
{
  GS_CHECK (gs, GS_RESX);
  gs->gs_resx.region = region;
}

/* GS_SWITCH accessors. */

static inline unsigned int
gs_switch_nlabels (gimple gs)
{
  GS_CHECK (gs, GS_SWITCH);
  return gs->gs_switch.nlabels;
}

static inline void
gs_switch_set_nlabels (gimple gs, unsigned int nlabels)
{
  GS_CHECK (gs, GS_SWITCH);
  gs->gs_switch.nlabels = nlabels;
}

static inline tree
gs_switch_index (gimple gs)
{
  GS_CHECK (gs, GS_SWITCH);
  return gs->gs_switch.index;
}

static inline void
gs_switch_set_index (gimple gs, tree index)
{
  GS_CHECK (gs, GS_SWITCH);
  gs->gs_switch.index = index;
}

static inline tree
gs_switch_default_label (gimple gs)
{
  GS_CHECK (gs, GS_SWITCH);
  return gs->gs_switch.labels[0];
}

static inline void
gs_switch_set_default_label (gimple gs, tree label)
{
  GS_CHECK (gs, GS_SWITCH);
  gs->gs_switch.labels[0] = label;
}

static inline tree
gs_switch_label (gimple gs, int index)
{
  GS_CHECK (gs, GS_SWITCH);
  gcc_assert ((unsigned)index <= gs->gs_switch.nlabels);
  return gs->gs_switch.labels[index + 1];
}

static inline void
gs_switch_set_label (gimple gs, int index, tree label)
{
  GS_CHECK (gs, GS_SWITCH);
  gcc_assert ((unsigned)index <= gs->gs_switch.nlabels);
  gs->gs_switch.labels[index + 1] = label;
}

/* GS_OMP_CRITICAL accessors. */

static inline struct gs_sequence * 
gs_omp_body (gimple gs)
{
  return &(gs->omp.body);
}

static inline void
gs_omp_set_body (gimple gs, struct gs_sequence body)
{
  gs->omp.body.first = body.first;
  gs->omp.body.last = body.last;
}

static inline tree
gs_omp_critical_name (gimple gs)
{
  GS_CHECK (gs, GS_OMP_CRITICAL);
  return gs->gs_omp_critical.name;
}

static inline void
gs_omp_critical_set_name (gimple gs, tree name)
{
  GS_CHECK (gs, GS_OMP_CRITICAL);
  gs->gs_omp_critical.name = name;
}

/* GS_OMP_FOR accessors. */

static inline tree
gs_omp_for_clauses (gimple gs)
{
  GS_CHECK (gs, GS_OMP_FOR);
  return gs->gs_omp_for.clauses;
}

static inline void
gs_omp_for_set_clauses (gimple gs, tree clauses)
{
  GS_CHECK (gs, GS_OMP_FOR);
  gs->gs_omp_for.clauses = clauses;
}

static inline tree
gs_omp_for_index (gimple gs)
{
  GS_CHECK (gs, GS_OMP_FOR);
  return gs->gs_omp_for.index;
}

static inline void
gs_omp_for_set_index (gimple gs, tree index)
{
  GS_CHECK (gs, GS_OMP_FOR);
  gs->gs_omp_for.index = index;
}

static inline tree
gs_omp_for_initial (gimple gs)
{
  GS_CHECK (gs, GS_OMP_FOR);
  return gs->gs_omp_for.initial;
}

static inline void
gs_omp_for_set_initial (gimple gs, tree initial)
{
  GS_CHECK (gs, GS_OMP_FOR);
  gs->gs_omp_for.initial = initial;
}

static inline tree
gs_omp_for_final (gimple gs)
{
  GS_CHECK (gs, GS_OMP_FOR);
  return gs->gs_omp_for.final;
}

static inline void
gs_omp_for_set_final (gimple gs, tree final)
{
  GS_CHECK (gs, GS_OMP_FOR);
  gs->gs_omp_for.final = final;
}

static inline tree
gs_omp_for_incr (gimple gs)
{
  GS_CHECK (gs, GS_OMP_FOR);
  return gs->gs_omp_for.incr;
}

static inline void
gs_omp_for_set_incr (gimple gs, tree incr)
{
  GS_CHECK (gs, GS_OMP_FOR);
  gs->gs_omp_for.incr = incr;
}

static inline struct gs_sequence *
gs_omp_for_pre_body (gimple gs)
{
  GS_CHECK (gs, GS_OMP_FOR);
  return &(gs->gs_omp_for.pre_body);
}

static inline void
gs_omp_for_set_pre_body (gimple gs, struct gs_sequence pre_body)
{
  GS_CHECK (gs, GS_OMP_FOR);
  gs->gs_omp_for.pre_body.first = pre_body.first;
  gs->gs_omp_for.pre_body.last = pre_body.last;
}

/* GS_OMP_PARALLEL accessors. */

static inline tree
gs_omp_parallel_clauses (gimple gs)
{
  GS_CHECK (gs, GS_OMP_PARALLEL);
  return gs->gs_omp_parallel.clauses;
}

static inline void
gs_omp_parallel_set_clauses (gimple gs, tree clauses)
{
  GS_CHECK (gs, GS_OMP_PARALLEL);
  gs->gs_omp_parallel.clauses = clauses;
}

static inline tree
gs_omp_parallel_child_fn (gimple gs)
{
  GS_CHECK (gs, GS_OMP_PARALLEL);
  return gs->gs_omp_parallel.child_fn;
}

static inline void
gs_omp_parallel_set_child_fn (gimple gs, tree child_fn)
{
  GS_CHECK (gs, GS_OMP_PARALLEL);
  gs->gs_omp_parallel.child_fn = child_fn;
}

static inline tree
gs_omp_parallel_data_arg (gimple gs)
{
  GS_CHECK (gs, GS_OMP_PARALLEL);
  return gs->gs_omp_parallel.data_arg;
}

static inline void
gs_omp_parallel_set_data_arg (gimple gs, tree data_arg)
{
  GS_CHECK (gs, GS_OMP_PARALLEL);
  gs->gs_omp_parallel.data_arg = data_arg;
}

/* GS_OMP_SECTION accessors. */

/* GS_OMP_SINGLE accessors. */

static inline tree
gs_omp_single_clauses (gimple gs)
{
  GS_CHECK (gs, GS_OMP_SINGLE);
  return gs->gs_omp_single.clauses;
}

static inline void
gs_omp_single_set_clauses (gimple gs, tree clauses)
{
  GS_CHECK (gs, GS_OMP_SINGLE);
  gs->gs_omp_single.clauses = clauses;
}

static inline tree
gs_omp_sections_clauses (gimple gs)
{
  GS_CHECK (gs, GS_OMP_SECTIONS);
  return gs->gs_omp_sections.clauses;
}

static inline void
gs_omp_sections_set_clauses (gimple gs, tree clauses)
{
  GS_CHECK (gs, GS_OMP_SECTIONS);
  gs->gs_omp_sections.clauses = clauses;
}



/* get or set the OMP_FOR_COND stored in the subcode flags */
static inline void
gs_assign_omp_for_cond (gimple gs, enum gs_cond cond)
{
  GS_CHECK(gs, GS_OMP_FOR);
  GS_SUBCODE_FLAGS(gs) =  cond;
}

static inline enum gs_cond
gs_omp_for_cond (gimple gs)
{
  GS_CHECK(gs, GS_OMP_FOR);
  return (enum gs_cond)GS_SUBCODE_FLAGS(gs);
}

/* GS_RETURN accessors.  */

static inline tree
gs_return_retval (gimple gs)
{
  GS_CHECK (gs, GS_RETURN);
  return gs->gs_return.retval;
}

static inline void
gs_return_set_retval (gimple gs, tree retval)
{
  GS_CHECK (gs, GS_RETURN);
  gs->gs_return.retval = retval;
}

/* Append sequence SRC to the end of sequence DST.  */

static inline void
gs_seq_append (gs_seq src, gs_seq dst)
{
  if (!GS_SEQ_EMPTY_P (src))
    gs_add (GS_SEQ_FIRST (src), dst);
}

static inline void
gs_add_subcode_flag(gimple g, unsigned int flag)
{
  GS_SUBCODE_FLAGS(g) |= flag;
}
#include "gimple-iterator.h"

#endif  /* GCC_GIMPLE_IR_H */
