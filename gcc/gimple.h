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

#ifndef GCC_GIMPLE_H
#define GCC_GIMPLE_H

#include "pointer-set.h"
#include "vec.h"
#include "ggc.h"
#include "tm.h"
#include "hard-reg-set.h"
#include "basic-block.h"

DEF_VEC_P(gimple);
DEF_VEC_ALLOC_P(gimple,heap);
DEF_VEC_ALLOC_P(gimple,gc);

DEF_VEC_P(gimple_seq);
DEF_VEC_ALLOC_P(gimple_seq,gc);
DEF_VEC_ALLOC_P(gimple_seq,heap);

enum gimple_code {
#define DEFGSCODE(SYM, STRING)	SYM,
#include "gimple.def"
#undef DEFGSCODE
    LAST_AND_UNUSED_GIMPLE_CODE
};


/* A sequence of gimple statements.  */
struct gimple_sequence GTY(())
{
  gimple first;
  gimple last;
};

static inline gimple
gimple_seq_first (gimple_seq s)
{
  return s->first;
}

static inline gimple
gimple_seq_last (gimple_seq s)
{
  return s->last;
}

static inline void
gimple_seq_set_last (gimple_seq s, gimple last)
{
  s->last = last;
}

static inline void
gimple_seq_set_first (gimple_seq s, gimple first)
{
  s->first = first;
}

static inline void
gimple_seq_init (gimple_seq s)
{
  s->first = NULL;
  s->last = NULL;
}

static inline gimple_seq
gimple_seq_alloc (void)
{
  return (gimple_seq) ggc_alloc_cleared (sizeof (struct gimple_sequence));
}

/* Copy the sequence SRC into the sequence DEST.  */

static inline void
gimple_seq_copy (gimple_seq dest, gimple_seq src)
{
  gimple_seq_set_first (dest, gimple_seq_first (src));
  gimple_seq_set_last (dest, gimple_seq_last (src));
}

static inline bool
gimple_seq_empty_p (gimple_seq s)
{
  return s->first == NULL;
}


/* Returns the sequence of statements in BB.  */

static inline gimple_seq
bb_seq (const_basic_block bb)
{
  gcc_assert (!(bb->flags & BB_RTL));
  return bb->il.gimple->seq;
}

/* Sets the sequence of statements in BB to SEQ.  */

static inline void
set_bb_seq (basic_block bb, gimple_seq seq)
{
  gcc_assert (!(bb->flags & BB_RTL));
  bb->il.gimple->seq = seq;
}


/* Data structure definitions for GIMPLE tuples.  */

struct gimple_statement_base GTY(())
{
  ENUM_BITFIELD(gimple_code) code : 16;
  unsigned int subcode : 8;
  unsigned int flags : 8;
  gimple next;
  gimple prev;
  struct basic_block_def *bb;
  location_t locus;
  tree block;
};

struct gimple_statement_with_ops GTY(())
{
  struct gimple_statement_base gsbase;
  unsigned modified : 1;

  /* FIXME tuples.  OP should be amalgamated with DEF_OPS and USE_OPS.
     This duplication is unnecessary.  */
  struct def_optype_d GTY((skip)) *def_ops;
  struct use_optype_d GTY((skip)) *use_ops;

  /* FIXME tuples.  For many tuples, the number of operands can
     be deduced from the code.  */
  size_t num_ops;
  tree * GTY((length ("%h.num_ops"))) op;
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
  struct gimple_statement_base gsbase;
  struct gimple_sequence body;
};

/* GIMPLE_BIND */
struct gimple_statement_bind GTY(())
{
  struct gimple_statement_base gsbase;
  tree vars;

  /* This is different than the ``block'' in gimple_statement_base, which
     is analogous to TREE_BLOCK.  This block is the equivalent of
     BIND_EXPR_BLOCK in tree land.  See gimple-low.c.  */
  tree block;

  struct gimple_sequence body;
};

/* GIMPLE_CATCH */
struct gimple_statement_catch GTY(())
{
  struct gimple_statement_base gsbase;
  tree types;
  gimple_seq handler;
};

/* GIMPLE_EH_FILTER */
struct gimple_statement_eh_filter GTY(())
{
  struct gimple_statement_base gsbase;
  /* Filter types.  */
  tree types;
  /* Failure actions.  */
  gimple_seq failure;
};

/* GIMPLE_PHI */
struct gimple_statement_phi GTY(())
{
  struct gimple_statement_base gsbase;
  size_t capacity;
  size_t nargs;
  tree result;
  struct phi_arg_d GTY ((length ("%h.nargs"))) args[1];
};

/* GIMPLE_RESX */
struct gimple_statement_resx GTY(())
{
  struct gimple_statement_base gsbase;

  /* Exception region number.  */
  int region;
};

/* GIMPLE_TRY */
struct gimple_statement_try GTY(())
{
  struct gimple_statement_base gsbase;

  /* Expression to evaluate.  */
  struct gimple_sequence eval;

  /* Cleanup expression.  */
  struct gimple_sequence cleanup;
};

/* Kind of GIMPLE_TRY statements.  Either try/catch or try/finally.  */
enum gimple_try_kind {
  GIMPLE_TRY_CATCH = 1,
  GIMPLE_TRY_FINALLY = 2
};


/* GIMPLE_ASM */
struct gimple_statement_asm GTY(())
{
  struct gimple_statement_with_memory_ops with_mem_ops;
  const char *string;
  unsigned ni;		/* Number of inputs.  */
  unsigned no;		/* Number of outputs.  */
  unsigned nc;		/* Number of clobbers.  */
};


/* GIMPLE_OMP_CRITICAL */
struct gimple_statement_omp_critical GTY(())
{
  struct gimple_statement_omp omp;
  /* Critical section name.  */
  tree name;
};

/* GIMPLE_OMP_FOR */
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
  struct gimple_sequence pre_body;
};


/* GIMPLE_OMP_PARALLEL */
struct gimple_statement_omp_parallel GTY(())
{
  struct gimple_statement_omp omp;
  tree clauses;
  tree child_fn;
  /* Shared data argument.  */
  tree data_arg;
};

/* GIMPLE_OMP_SECTION */
/* Uses struct gimple_statement_omp.  */

/* GIMPLE_OMP_SECTIONS */
struct gimple_statement_omp_sections GTY(())
{
  struct gimple_statement_omp omp;
  tree clauses;
};

/* GIMPLE_OMP_SINGLE */
struct gimple_statement_omp_single GTY(())
{
  struct gimple_statement_omp omp;
  tree clauses;
};

/* GIMPLE_OMP_RETURN */
/* Flags stored in GIMPLE_OMP_RETURN's flags.  */
#define OMP_RETURN_NOWAIT_FLAG 1 << 0

/* GIMPLE_OMP_SECTION */
/* Flags stored in GIMPLE_OMP_SECTION's flags.  */
#define OMP_SECTION_LAST_FLAG 1 << 0

/* GIMPLE_OMP_PARALLEL */
/* Flags stored in GIMPLE_OMP_PARALLEL's flags.  */
#define OMP_PARALLEL_COMBINED_FLAG 1 << 0

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
  struct gimple_statement_base GTY ((tag ("GSS_BASE"))) gsbase;
  struct gimple_statement_with_ops GTY ((tag ("GSS_WITH_OPS"))) with_ops;
  struct gimple_statement_with_memory_ops GTY ((tag ("GSS_WITH_MEM_OPS"))) with_mem_ops;
  struct gimple_statement_omp GTY ((tag ("GSS_OMP"))) omp;
  struct gimple_statement_bind GTY ((tag ("GSS_BIND"))) gimple_bind;
  struct gimple_statement_catch GTY ((tag ("GSS_CATCH"))) gimple_catch;
  struct gimple_statement_eh_filter GTY ((tag ("GSS_EH_FILTER"))) gimple_eh_filter;
  struct gimple_statement_phi GTY ((tag ("GSS_PHI"))) gimple_phi;
  struct gimple_statement_resx GTY ((tag ("GSS_RESX"))) gimple_resx;
  struct gimple_statement_try GTY ((tag ("GSS_TRY"))) gimple_try;
  struct gimple_statement_asm GTY ((tag ("GSS_ASM"))) gimple_asm;
  struct gimple_statement_omp_critical GTY ((tag ("GSS_OMP_CRITICAL"))) gimple_omp_critical;
  struct gimple_statement_omp_for GTY ((tag ("GSS_OMP_FOR"))) gimple_omp_for;
  struct gimple_statement_omp_parallel GTY ((tag ("GSS_OMP_PARALLEL"))) gimple_omp_parallel;
  struct gimple_statement_omp_sections GTY ((tag ("GSS_OMP_SECTIONS"))) gimple_omp_sections;
  struct gimple_statement_omp_single GTY ((tag ("GSS_OMP_SINGLE"))) gimple_omp_single;
};

/* Set PREV to be the previous statement to G.  */

static inline void
set_gimple_prev (gimple g, gimple prev)
{
  g->gsbase.prev = prev;
}


/* Set NEXT to be the next statement to G.  */

static inline void
set_gimple_next (gimple g, gimple next)
{
  g->gsbase.next = next;
}


/* Common accessors for all GIMPLE statements.  */

static inline enum gimple_code
gimple_code (const_gimple g)
{
  return g->gsbase.code;
}

/* Common flags for every GIMPLE statement.  */
#define GIMPLE_NO_WARNING	(1 << 0)

static inline unsigned
gimple_flags (const_gimple g)
{
  return g->gsbase.flags;
}

static inline void
set_gimple_flags (gimple g, unsigned int flags)
{
  /* We only have 8 bits for flags.  Assert that we are not
     overflowing them.  */
  gcc_assert (flags < (1 << 8));
  g->gsbase.flags = flags;
}

static inline void
add_gimple_flag (gimple g, unsigned int flag)
{
  /* We only have 8 bits for flags.  Assert that we are not
     overflowing them.  */
  gcc_assert (flag < (1 << 8));
  g->gsbase.flags |= flag;
}

static inline void
set_gimple_subcode (gimple g, enum tree_code subcode)
{
  /* We only have 8 bits for the RHS code.  Assert that we are not
     overflowing it.  */
  gcc_assert (subcode < (1 << 8));
  g->gsbase.subcode = subcode;
}

static inline unsigned
gimple_subcode (const_gimple g)
{
  return g->gsbase.subcode;
}

static inline gimple
gimple_next (const_gimple g)
{
  return g->gsbase.next;
}

static inline gimple
gimple_prev (const_gimple g)
{
  return g->gsbase.prev;
}

static inline struct basic_block_def *
gimple_bb (const_gimple g)
{
  return g->gsbase.bb;
}

static inline tree
gimple_block (const_gimple g)
{
  return g->gsbase.block;
}

static inline void
set_gimple_block (gimple g, tree block)
{
  g->gsbase.block = block;
}

static inline location_t
gimple_locus (const_gimple g)
{
  return g->gsbase.locus;
}

static inline void
set_gimple_locus (gimple g, location_t locus)
{
  g->gsbase.locus = locus;
}

static inline bool
gimple_locus_empty_p (gimple g)
{
  return gimple_locus (g).file == NULL && gimple_locus (g).line == 0;
}


/* Return TRUE if a gimple statement has register or memory operands.  */

static inline bool
gimple_has_ops (gimple g)
{
  return gimple_code (g) >= GIMPLE_ASSIGN && gimple_code (g) <= GIMPLE_RETURN;
}

/* Return true if a gimple stmt has memory operands.  */

static inline bool
gimple_has_mem_ops (gimple g)
{
  return gimple_code (g) >= GIMPLE_ASM
         && gimple_code (g) <= GIMPLE_WITH_MEM_OPS;
}

/* Return true if no warnings should be emitted for statement STMT.  */

static inline bool
gimple_no_warning_p (gimple stmt)
{
  return gimple_flags (stmt) & GIMPLE_NO_WARNING;
}

/* SSA operand accesors.  */

static inline struct def_optype_d *
gimple_def_ops (gimple g)
{
  if (!gimple_has_ops (g))
    return NULL;
  return g->with_ops.def_ops;
}

static inline void
set_gimple_def_ops (gimple g, struct def_optype_d *def)
{
  gcc_assert (gimple_has_ops (g));
  g->with_ops.def_ops = def;
}

static inline struct use_optype_d *
gimple_use_ops (gimple g)
{
  if (!gimple_has_ops (g))
    return NULL;
  return g->with_ops.use_ops;
}

static inline void
set_gimple_use_ops (gimple g, struct use_optype_d *use)
{
  gcc_assert (gimple_has_ops (g));
  g->with_ops.use_ops = use;
}

static inline struct voptype_d *
gimple_vuse_ops (gimple g)
{
  if (!gimple_has_mem_ops (g))
    return NULL;
  return g->with_mem_ops.vuse_ops;
}

static inline void
set_gimple_vuse_ops (gimple g, struct voptype_d *ops)
{
  gcc_assert (gimple_has_mem_ops (g));
  g->with_mem_ops.vuse_ops = ops;
}

static inline struct voptype_d *
gimple_vdef_ops (gimple g)
{
  if (!gimple_has_mem_ops (g))
    return NULL;
  return g->with_mem_ops.vdef_ops;
}

static inline void
set_gimple_vdef_ops (gimple g, struct voptype_d *ops)
{
  gcc_assert (gimple_has_mem_ops (g));
  g->with_mem_ops.vdef_ops = ops;
}

static inline bitmap
gimple_loaded_syms (gimple g)
{
  if (!gimple_has_mem_ops (g))
    return NULL;
  return g->with_mem_ops.loads;
}

static inline bitmap
gimple_stored_syms (gimple g)
{
  if (!gimple_has_mem_ops (g))
    return NULL;
  return g->with_mem_ops.stores;
}


/* Return TRUE if the given statement has operands and the modified
   field has been set.  */

static inline bool
gimple_modified (gimple g)
{
  if (gimple_has_ops (g))
    return (bool) g->with_ops.modified;
  else
    return false;
}


/* Set the MODIFIED flag to MODIFIEDP, iff the gimple stmt G has a
   MODIFIED field.  */

static inline void
set_gimple_modified (gimple g, bool modifiedp)
{
  if (gimple_has_ops (g))
    g->with_ops.modified = (unsigned) modifiedp;
}

/* Returns TRUE if a statement is a GIMPLE_OMP_RETURN and has the
   OMP_RETURN_NOWAIT_FLAG set.  */

static inline bool
gimple_omp_return_nowait_p (gimple g)
{
  gcc_assert (gimple_code (g) == GIMPLE_OMP_RETURN);
  return gimple_subcode (g) & OMP_RETURN_NOWAIT_FLAG;
}


/* Returns TRUE if a statement is a GIMPLE_OMP_SECTION and has the
   OMP_SECTION_LAST_FLAG set.  */

static inline bool
gimple_omp_section_last_p (gimple g)
{
  gcc_assert (gimple_code (g) == GIMPLE_OMP_SECTION);
  return gimple_subcode (g) & OMP_SECTION_LAST_FLAG;
}


/* Returns TRUE if a GIMPLE_OMP_PARALLEL has the OMP_PARALLEL_COMBINED_FLAG
   set.  */

static inline bool
gimple_omp_parallel_combined_p (gimple g)
{
  gcc_assert (gimple_code (g) == GIMPLE_OMP_PARALLEL);
  return gimple_subcode (g) & OMP_PARALLEL_COMBINED_FLAG;
}


/* In gimple.c.  */
gimple build_gimple_return (tree);
gimple build_gimple_assign (tree, tree);
gimple build_gimple_call_vec (tree, VEC(tree, gc) *);
gimple build_gimple_call (tree, size_t, ...);
gimple build_gimple_cond (enum tree_code, tree, tree, tree, tree);
void gimple_cond_invert (gimple);
gimple build_gimple_label (tree label);
gimple build_gimple_goto (tree dest);
gimple build_gimple_nop (void);
gimple build_gimple_bind (tree, gimple_seq);
gimple build_gimple_asm (const char *, unsigned, unsigned, unsigned, ...);
gimple build_gimple_asm_vec (const char *, VEC(tree,gc) *, VEC(tree,gc) *,
                             VEC(tree,gc) *);
gimple build_gimple_catch (tree, gimple_seq);
gimple build_gimple_eh_filter (tree, gimple_seq);
gimple build_gimple_try (gimple_seq, gimple_seq, unsigned int);
gimple build_gimple_phi (size_t, size_t, tree, ...);
gimple build_gimple_resx (int);
gimple build_gimple_switch (unsigned int, tree, tree, ...);
gimple build_gimple_switch_vec (tree, tree, VEC(tree,heap) *);
gimple build_gimple_omp_parallel (gimple_seq, tree, tree, tree);
gimple build_gimple_omp_for (gimple_seq, tree, tree, tree, tree, tree,
                             gimple_seq, enum tree_code);
gimple build_gimple_omp_critical (gimple_seq, tree);
gimple build_gimple_omp_section (gimple_seq);
gimple build_gimple_omp_continue (gimple_seq);
gimple build_gimple_omp_master (gimple_seq);
gimple build_gimple_omp_return (bool);
gimple build_gimple_omp_ordered (gimple_seq);
gimple build_gimple_omp_sections (gimple_seq, tree);
gimple build_gimple_omp_single (gimple_seq, tree);
enum gimple_statement_structure_enum gimple_statement_structure (gimple);
void gimple_seq_add (gimple_seq, gimple);
enum gimple_statement_structure_enum gss_for_assign (enum tree_code);
void sort_case_labels (VEC(tree,heap) *);
void set_gimple_body (tree, gimple_seq);
gimple_seq gimple_body (tree);
void gimple_seq_append (gimple_seq, gimple_seq);
int gimple_call_flags (gimple);
bool gimple_assign_copy_p (gimple);
void gimple_remove (gimple, gimple_seq, bool);
gimple_seq gimple_seq_reverse (gimple_seq);
void set_gimple_bb (gimple, struct basic_block_def *);

extern const char *const gimple_code_name[];


/* Error out if a gimple tuple is addressed incorrectly.  */
#if defined ENABLE_GIMPLE_CHECKING
extern void gimple_check_failed (const gimple, const char *, int,          \
                                 const char *, enum gimple_code,           \
				 enum tree_code) ATTRIBUTE_NORETURN;
extern void gimple_range_check_failed (const gimple, const char *, int,    \
                                       const char *, enum gimple_code,     \
				       enum gimple_code) ATTRIBUTE_NORETURN;

#define GIMPLE_CHECK(GS, CODE)						\
  do {									\
    const gimple __gs = (GS);						\
    if (gimple_code (__gs) != (CODE))					\
      gimple_check_failed (__gs, __FILE__, __LINE__, __FUNCTION__,	\
	  		   (CODE), 0);					\
  } while (0)

#define GIMPLE_CHECK2(GS, CODE1, CODE2)					\
  do {									\
    const gimple __gs = (GS);						\
    if (gimple_code (__gs) != (CODE1)					\
	|| gimple_subcode (__gs) != (CODE2))				\
      gimple_check_failed (__gs, __FILE__, __LINE__, __FUNCTION__,	\
	  		   (CODE1), (CODE2));				\
  } while (0)

#define GIMPLE_RANGE_CHECK(GS, CODE1, CODE2)				\
  do {									\
    const gimple __gs = (GS);						\
    if (gimple_code (__gs) < (CODE1) || gimple_code (__gs) > (CODE2))	\
      gimple_range_check_failed (__gs, __FILE__, __LINE__, __FUNCTION__,\
		                 (CODE1), (CODE2));			\
  } while (0)
#else  /* not ENABLE_GIMPLE_CHECKING  */
#define GIMPLE_CHECK(GS, CODE)			(void)0
#define GIMPLE_CHECK2(GS, C1, C2)		(void)0
#define GIMPLE_RANGE_CHECK(GS, CODE1, CODE2)	(void)0
#endif


/* GIMPLE IR accessor functions.  */

/* GIMPLE_WITH_OPS and GIMPLE_WITH_MEM_OPS accessors.  */

static inline size_t
gimple_num_ops (gimple gs)
{
  GIMPLE_RANGE_CHECK (gs, GIMPLE_ASSIGN, GIMPLE_RETURN);
  return gs->with_ops.num_ops;
}

static inline tree
gimple_op (gimple gs, size_t i)
{
  GIMPLE_RANGE_CHECK (gs, GIMPLE_ASSIGN, GIMPLE_RETURN);
  gcc_assert (i < gs->with_ops.num_ops);
  return gs->with_ops.op[i];
}

static inline tree *
gimple_op_ptr (gimple gs, size_t i)
{
  GIMPLE_RANGE_CHECK (gs, GIMPLE_ASSIGN, GIMPLE_RETURN);
  gcc_assert (i < gs->with_ops.num_ops);
  return &gs->with_ops.op[i];
}

static inline void
gimple_set_op (gimple gs, size_t i, tree op)
{
  GIMPLE_RANGE_CHECK (gs, GIMPLE_ASSIGN, GIMPLE_RETURN);
  gcc_assert (i < gs->with_ops.num_ops);
  gs->with_ops.op[i] = op;
}


/* GIMPLE_ASSIGN accessors.  */

static inline enum tree_code
gimple_assign_subcode (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ASSIGN);
  return gimple_subcode (gs);
}

static inline tree
gimple_assign_operand (gimple gs, size_t opno)
{
  GIMPLE_CHECK (gs, GIMPLE_ASSIGN);
  gcc_assert (gs->with_ops.num_ops > opno);
  return gs->with_ops.op[opno];
}

static inline void
gimple_assign_set_operand (gimple gs, size_t opno, tree op)
{
  GIMPLE_CHECK (gs, GIMPLE_ASSIGN);
  gcc_assert (gs->with_ops.num_ops > opno);
  gs->with_ops.op[opno] = op;
}

static inline tree
gimple_assign_lhs (gimple gs)
{
  return gimple_assign_operand (gs, 0);
}

static inline void
gimple_assign_set_lhs (gimple gs, tree lhs)
{
  gimple_assign_set_operand (gs, 0, lhs);
}

static inline tree
gimple_assign_rhs1 (gimple gs)
{
  return gimple_assign_operand (gs, 1);
}

static inline void
gimple_assign_set_rhs1 (gimple gs, tree rhs)
{
  gimple_assign_set_operand (gs, 1, rhs);
}

static inline tree
gimple_assign_rhs2 (gimple gs)
{
  return gimple_assign_operand (gs, 2);
}

static inline void
gimple_assign_set_rhs2 (gimple gs, tree rhs)
{
  gimple_assign_set_operand (gs, 2, rhs);
}

/* GIMPLE_CALL accessors. */

static inline tree
gimple_call_lhs (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_CALL);
  return gs->with_ops.op[0];
}

static inline void
gimple_call_set_lhs (gimple gs, tree lhs)
{
  GIMPLE_CHECK (gs, GIMPLE_CALL);
  gs->with_ops.op[0] = lhs;
}

static inline tree
gimple_call_fn (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_CALL);
  gcc_assert (gs->with_ops.num_ops > 1);
  return gs->with_ops.op[1];
}

/* If a given GIMPLE_CALL's callee is a FUNCTION_DECL, return it.
   Otherwise return NULL.  This function is analogous to
   get_callee_fndecl in tree land.  */

static inline tree
gimple_call_fndecl (gimple gs)
{
  tree decl;

  GIMPLE_CHECK (gs, GIMPLE_CALL);
  gcc_assert (gs->with_ops.num_ops > 1);
  decl = gs->with_ops.op[1];
  if (TREE_CODE (decl) == FUNCTION_DECL)
    return decl;
  else
    return NULL;
}

static inline tree
gimple_call_return_type (gimple gs)
{
  tree fn = gimple_call_fn (gs);
  tree type = TREE_TYPE (fn);

  /* See through pointer to functions.  */
  if (TREE_CODE (type) == POINTER_TYPE)
    type = TREE_TYPE (type);

  gcc_assert (TREE_CODE (type) == FUNCTION_TYPE);

  /* The type returned by a FUNCTION_DECL is the type of its
     function type.  */
  return TREE_TYPE (type);
}

static inline tree
gimple_call_chain (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_CALL);
  gcc_assert (gs->with_ops.num_ops > 2);
  return gs->with_ops.op[2];
}

static inline void
gimple_call_set_chain (gimple gs, tree chain)
{
  GIMPLE_CHECK (gs, GIMPLE_CALL);
  gcc_assert (gs->with_ops.num_ops > 2);
  gs->with_ops.op[2] = chain;
}

static inline unsigned long
gimple_call_nargs (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_CALL);
  gcc_assert (gs->with_ops.num_ops >= 3);
  return gs->with_ops.num_ops - 3;
}

static inline tree
gimple_call_arg (gimple gs, size_t index)
{
  GIMPLE_CHECK (gs, GIMPLE_CALL);
  gcc_assert (gs->with_ops.num_ops > index + 3);
  return gs->with_ops.op[index + 3];
}

static inline tree *
gimple_call_arg_ptr (gimple gs, size_t index)
{
  GIMPLE_CHECK (gs, GIMPLE_CALL);
  gcc_assert (gs->with_ops.num_ops > index + 3);
  return &gs->with_ops.op[index + 3];
}

static inline void
gimple_call_set_arg (gimple gs, size_t index, tree arg)
{
  GIMPLE_CHECK (gs, GIMPLE_CALL);
  gcc_assert (gs->with_ops.num_ops > index + 3);
  gs->with_ops.op[index + 3] = arg;
}


/* GIMPLE_COND accessors. */

static inline enum tree_code
gimple_cond_code (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_COND);
  return gimple_subcode (gs);
}

static inline void
gimple_cond_set_code (gimple gs, enum tree_code code)
{
  GIMPLE_CHECK (gs, GIMPLE_COND);
  gcc_assert (TREE_CODE_CLASS (code) == tcc_comparison);
  set_gimple_subcode (gs, code);
}

static inline tree
gimple_cond_lhs (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_COND);
  gcc_assert (gs->with_ops.num_ops == 4);
  return gs->with_ops.op[0];
}

static inline void
gimple_cond_set_lhs (gimple gs, tree lhs)
{
  GIMPLE_CHECK (gs, GIMPLE_COND);
  gcc_assert (gs->with_ops.num_ops == 4);
  gs->with_ops.op[0] = lhs;
}

static inline tree
gimple_cond_rhs (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_COND);
  gcc_assert (gs->with_ops.num_ops == 4);
  return gs->with_ops.op[1];
}

static inline void
gimple_cond_set_rhs (gimple gs, tree rhs)
{
  GIMPLE_CHECK (gs, GIMPLE_COND);
  gcc_assert (gs->with_ops.num_ops == 4);
  gs->with_ops.op[1] = rhs;
}

static inline tree
gimple_cond_true_label (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_COND);
  gcc_assert (gs->with_ops.num_ops == 4);
  return gs->with_ops.op[2];
}

static inline void
gimple_cond_set_true_label (gimple gs, tree label)
{
  GIMPLE_CHECK (gs, GIMPLE_COND);
  gcc_assert (gs->with_ops.num_ops == 4);
  gs->with_ops.op[2] = label;
}

static inline void
gimple_cond_set_false_label (gimple gs, tree label)
{
  GIMPLE_CHECK (gs, GIMPLE_COND);
  gcc_assert (gs->with_ops.num_ops == 4);
  gs->with_ops.op[3] = label;
}

static inline tree
gimple_cond_false_label (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_COND);
  gcc_assert (gs->with_ops.num_ops == 4);
  return gs->with_ops.op[3];
}

/* Set the conditional COND_STMT to be of the form 'if (1 == 0)'.  */

static inline void
gimple_cond_make_false (gimple gs)
{
  set_gimple_subcode (gs, EQ_EXPR);
  gimple_cond_set_lhs (gs, boolean_true_node);
  gimple_cond_set_rhs (gs, boolean_false_node);
}

/* Set the conditional COND_STMT to be of the form 'if (1 == 1)'.  */

static inline void
gimple_cond_make_true (gimple gs)
{
  set_gimple_subcode (gs, EQ_EXPR);
  gimple_cond_set_lhs (gs, boolean_true_node);
  gimple_cond_set_lhs (gs, boolean_true_node);
}

/* GIMPLE_LABEL accessors. */

static inline tree
gimple_label_label (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_LABEL);
  gcc_assert (gs->with_ops.num_ops == 1);
  return gs->with_ops.op[0];
}

static inline void
gimple_label_set_label (gimple gs, tree label)
{
  GIMPLE_CHECK (gs, GIMPLE_LABEL);
  gcc_assert (gs->with_ops.num_ops == 1);
  gcc_assert (TREE_CODE (label) == LABEL_DECL);
  gs->with_ops.op[0] = label;
}


/* GIMPLE_GOTO accessors. */

static inline tree
gimple_goto_dest (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_GOTO);
  gcc_assert (gs->with_ops.num_ops == 1);
  return gs->with_ops.op[0];
}

static inline void 
gimple_goto_set_dest (gimple gs, tree dest)
{
  GIMPLE_CHECK (gs, GIMPLE_GOTO);
  gcc_assert (gs->with_ops.num_ops == 1);
  gs->with_ops.op[0] = dest;
}


/* GIMPLE_BIND accessors. */

static inline tree
gimple_bind_vars (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_BIND);
  return gs->gimple_bind.vars;
}

static inline void
gimple_bind_set_vars (gimple gs, tree vars)
{
  GIMPLE_CHECK (gs, GIMPLE_BIND);
  gs->gimple_bind.vars = vars;
}

static inline gimple_seq
gimple_bind_body (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_BIND);
  return &(gs->gimple_bind.body);
}

static inline void
gimple_bind_set_body (gimple gs, gimple_seq seq)
{
  GIMPLE_CHECK (gs, GIMPLE_BIND);
  gimple_seq_copy (&(gs->gimple_bind.body), seq);
}

static inline tree
gimple_bind_block (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_BIND);
  return gs->gimple_bind.block;
}

static inline void
gimple_bind_set_block (gimple gs, tree block)
{
  GIMPLE_CHECK (gs, GIMPLE_BIND);
  gs->gimple_bind.block = block;
}


/* GIMPLE_ASM accessors. */

static inline unsigned int
gimple_asm_ninputs (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ASM);
  return gs->gimple_asm.ni;
}

static inline unsigned int
gimple_asm_noutputs (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ASM);
  return gs->gimple_asm.no;
}

static inline unsigned int
gimple_asm_nclobbered (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ASM);
  return gs->gimple_asm.nc;
}

static inline tree
gimple_asm_input_op (gimple gs, unsigned int index)
{
  GIMPLE_CHECK (gs, GIMPLE_ASM);
  gcc_assert (index <= gs->gimple_asm.ni);
  return gs->with_ops.op[index];
}

static inline void
gimple_asm_set_input_op (gimple gs, unsigned int index, tree in_op)
{
  GIMPLE_CHECK (gs, GIMPLE_ASM);
  gcc_assert (index <= gs->gimple_asm.ni);
  gs->with_ops.op[index] = in_op;
}

static inline tree
gimple_asm_output_op (gimple gs, unsigned int index)
{
  GIMPLE_CHECK (gs, GIMPLE_ASM);
  gcc_assert (index <= gs->gimple_asm.no);
  return gs->with_ops.op[index + gs->gimple_asm.ni];
}

static inline void
gimple_asm_set_output_op (gimple gs, unsigned int index, tree out_op)
{
  GIMPLE_CHECK (gs, GIMPLE_ASM);
  gcc_assert (index <= gs->gimple_asm.no);
  gs->with_ops.op[index + gs->gimple_asm.ni] = out_op;
}

static inline tree
gimple_asm_clobber_op (gimple gs, unsigned int index)
{
  GIMPLE_CHECK (gs, GIMPLE_ASM);
  gcc_assert (index <= gs->gimple_asm.nc);
  return gs->with_ops.op[index + gs->gimple_asm.ni + gs->gimple_asm.no];
}

static inline void
gimple_asm_set_clobber_op (gimple gs, unsigned int index, tree clobber_op)
{
  GIMPLE_CHECK (gs, GIMPLE_ASM);
  gcc_assert (index <= gs->gimple_asm.nc);
  gs->with_ops.op[index + gs->gimple_asm.ni + gs->gimple_asm.no] = clobber_op;
}

static inline const char *
gimple_asm_string (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ASM);
  return gs->gimple_asm.string;
}


/* GIMPLE_CATCH accessors. */

static inline tree
gimple_catch_types (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_CATCH);
  return gs->gimple_catch.types;
}

static inline tree *
gimple_catch_types_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_CATCH);
  return &gs->gimple_catch.types;
}

static inline gimple_seq
gimple_catch_handler (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_CATCH);
  return gs->gimple_catch.handler;
}

static inline void
gimple_catch_set_types (gimple gs, tree t)
{
  GIMPLE_CHECK (gs, GIMPLE_CATCH);
  gs->gimple_catch.types = t;
}

static inline void
gimple_catch_set_handler (gimple gs, gimple_seq handler)
{
  GIMPLE_CHECK (gs, GIMPLE_CATCH);
  gs->gimple_catch.handler = handler;
}


/* GIMPLE_EH_FILTER accessors. */

static inline tree
gimple_eh_filter_types (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_EH_FILTER);
  return gs->gimple_eh_filter.types;
}

static inline tree *
gimple_eh_filter_types_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_EH_FILTER);
  return &gs->gimple_eh_filter.types;
}

static inline gimple_seq
gimple_eh_filter_failure (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_EH_FILTER);
  return gs->gimple_eh_filter.failure;
}

static inline void
gimple_eh_filter_set_types (gimple gs, tree types)
{
  GIMPLE_CHECK (gs, GIMPLE_EH_FILTER);
  gs->gimple_eh_filter.types = types;
}

static inline void
gimple_eh_filter_set_failure (gimple gs, gimple_seq failure)
{
  GIMPLE_CHECK (gs, GIMPLE_EH_FILTER);
  gs->gimple_eh_filter.failure = failure;
}


/* GIMPLE_TRY accessors. */

static inline enum gimple_try_kind
gimple_try_kind (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_TRY);
  return (enum gimple_try_kind) gimple_subcode (gs);
}

static inline gimple_seq
gimple_try_eval (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_TRY);
  return &gs->gimple_try.eval;
}

static inline gimple_seq
gimple_try_cleanup (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_TRY);
  return &gs->gimple_try.cleanup;
}

static inline void
gimple_try_set_eval (gimple gs, gimple_seq eval)
{
  GIMPLE_CHECK (gs, GIMPLE_TRY);
  gimple_seq_copy (gimple_try_eval (gs), eval);
}

static inline void
gimple_try_set_cleanup (gimple gs, gimple_seq cleanup)
{
  GIMPLE_CHECK (gs, GIMPLE_TRY);
  gimple_seq_copy (gimple_try_cleanup (gs), cleanup);
}


/* GIMPLE_PHI accessors. */

static inline size_t
gimple_phi_capacity (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_PHI);
  return gs->gimple_phi.capacity;
}

static inline void
gimple_phi_set_capacity (gimple gs, size_t capacity)
{
  GIMPLE_CHECK (gs, GIMPLE_PHI);
  gs->gimple_phi.capacity = capacity;
}

static inline size_t
gimple_phi_num_args (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_PHI);
  return gs->gimple_phi.nargs;
}

static inline void
gimple_phi_set_nargs (gimple gs, size_t nargs)
{
  GIMPLE_CHECK (gs, GIMPLE_PHI);
  gs->gimple_phi.nargs = nargs;
}

static inline tree
gimple_phi_result (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_PHI);
  return gs->gimple_phi.result;
}

static inline void
gimple_phi_set_result (gimple gs, tree result)
{
  GIMPLE_CHECK (gs, GIMPLE_PHI);
  gs->gimple_phi.result = result;
}

static inline struct phi_arg_d *
gimple_phi_arg (gimple gs, size_t index)
{
  GIMPLE_CHECK (gs, GIMPLE_PHI);
  gcc_assert (index <= gs->gimple_phi.nargs);
  return &(gs->gimple_phi.args[index]);
}

static inline void
gimple_phi_set_arg (gimple gs, size_t index, struct phi_arg_d * phiarg)
{
  GIMPLE_CHECK (gs, GIMPLE_PHI);
  gcc_assert (index <= gs->gimple_phi.nargs);
  memcpy (gs->gimple_phi.args + index, phiarg, sizeof (struct phi_arg_d));
}


/* GIMPLE_RESX accessors. */

static inline int
gimple_resx_region (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_RESX);
  return gs->gimple_resx.region;
}

static inline void
gimple_resx_set_region (gimple gs, int region)
{
  GIMPLE_CHECK (gs, GIMPLE_RESX);
  gs->gimple_resx.region = region;
}


/* GIMPLE_SWITCH accessors. */

static inline size_t
gimple_switch_num_labels (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_SWITCH);
  gcc_assert (gs->with_ops.num_ops > 1);
  return gs->with_ops.num_ops - 1;
}

static inline void
gimple_switch_set_num_labels (gimple g, size_t nlabels)
{
  GIMPLE_CHECK (g, GIMPLE_SWITCH);
  g->with_ops.num_ops = nlabels + 1;
}

static inline tree
gimple_switch_index (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_SWITCH);
  return gs->with_ops.op[0];
}

static inline void
gimple_switch_set_index (gimple gs, tree index)
{
  GIMPLE_CHECK (gs, GIMPLE_SWITCH);
  gs->with_ops.op[0] = index;
}

/* Return the label numbered INDEX.  The default label is 0, followed by any
   labels in a switch statement.  */

static inline tree
gimple_switch_label (gimple gs, size_t index)
{
  GIMPLE_CHECK (gs, GIMPLE_SWITCH);
  gcc_assert (gs->with_ops.num_ops > index + 1);
  return gs->with_ops.op[index + 1];
}

/* Set the label number INDEX to LABEL.  0 is always the default label.  */

static inline void
gimple_switch_set_label (gimple gs, size_t index, tree label)
{
  GIMPLE_CHECK (gs, GIMPLE_SWITCH);
  gcc_assert (gs->with_ops.num_ops > index + 1);
  gs->with_ops.op[index + 1] = label;
}

/* Return the default label for a switch statement.  */

static inline tree
gimple_switch_default_label (gimple gs)
{
  return gimple_switch_label (gs, 0);
}

/* Set the default label for a switch statement.  */

static inline void
gimple_switch_set_default_label (gimple gs, tree label)
{
  gimple_switch_set_label (gs, 0, label);
}


/* GIMPLE_OMP_* accessors. */

static inline gimple_seq 
gimple_omp_body (gimple gs)
{
  return &(gs->omp.body);
}

static inline void
gimple_omp_set_body (gimple gs, gimple_seq body)
{
  gimple_seq_copy (&(gs->omp.body), body);
}

/* GIMPLE_OMP_CRITICAL accessors. */

static inline tree
gimple_omp_critical_name (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_CRITICAL);
  return gs->gimple_omp_critical.name;
}

static inline tree *
gimple_omp_critical_name_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_CRITICAL);
  return &gs->gimple_omp_critical.name;
}

static inline void
gimple_omp_critical_set_name (gimple gs, tree name)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_CRITICAL);
  gs->gimple_omp_critical.name = name;
}

/* GIMPLE_OMP_FOR accessors. */

static inline tree
gimple_omp_for_clauses (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return gs->gimple_omp_for.clauses;
}

static inline tree *
gimple_omp_for_clauses_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return &gs->gimple_omp_for.clauses;
}

static inline void
gimple_omp_for_set_clauses (gimple gs, tree clauses)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  gs->gimple_omp_for.clauses = clauses;
}

static inline tree
gimple_omp_for_index (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return gs->gimple_omp_for.index;
}

static inline tree *
gimple_omp_for_index_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return &gs->gimple_omp_for.index;
}

static inline void
gimple_omp_for_set_index (gimple gs, tree index)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  gs->gimple_omp_for.index = index;
}

static inline tree
gimple_omp_for_initial (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return gs->gimple_omp_for.initial;
}

static inline tree *
gimple_omp_for_initial_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return &gs->gimple_omp_for.initial;
}

static inline void
gimple_omp_for_set_initial (gimple gs, tree initial)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  gs->gimple_omp_for.initial = initial;
}

static inline tree
gimple_omp_for_final (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return gs->gimple_omp_for.final;
}

static inline tree *
gimple_omp_for_final_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return &gs->gimple_omp_for.final;
}

static inline void
gimple_omp_for_set_final (gimple gs, tree final)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  gs->gimple_omp_for.final = final;
}

static inline tree
gimple_omp_for_incr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return gs->gimple_omp_for.incr;
}

static inline tree *
gimple_omp_for_incr_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return &gs->gimple_omp_for.incr;
}

static inline void
gimple_omp_for_set_incr (gimple gs, tree incr)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  gs->gimple_omp_for.incr = incr;
}

static inline gimple_seq
gimple_omp_for_pre_body (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return &(gs->gimple_omp_for.pre_body);
}

static inline void
gimple_omp_for_set_pre_body (gimple gs, gimple_seq pre_body)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  gimple_seq_copy (&(gs->gimple_omp_for.pre_body),  pre_body);
}

/* GIMPLE_OMP_PARALLEL accessors. */

static inline tree
gimple_omp_parallel_clauses (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_PARALLEL);
  return gs->gimple_omp_parallel.clauses;
}

static inline tree *
gimple_omp_parallel_clauses_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_PARALLEL);
  return &gs->gimple_omp_parallel.clauses;
}

static inline void
gimple_omp_parallel_set_clauses (gimple gs, tree clauses)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_PARALLEL);
  gs->gimple_omp_parallel.clauses = clauses;
}

static inline tree
gimple_omp_parallel_child_fn (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_PARALLEL);
  return gs->gimple_omp_parallel.child_fn;
}

static inline tree *
gimple_omp_parallel_child_fn_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_PARALLEL);
  return &gs->gimple_omp_parallel.child_fn;
}

static inline void
gimple_omp_parallel_set_child_fn (gimple gs, tree child_fn)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_PARALLEL);
  gs->gimple_omp_parallel.child_fn = child_fn;
}

static inline tree
gimple_omp_parallel_data_arg (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_PARALLEL);
  return gs->gimple_omp_parallel.data_arg;
}

static inline tree *
gimple_omp_parallel_data_arg_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_PARALLEL);
  return &gs->gimple_omp_parallel.data_arg;
}

static inline void
gimple_omp_parallel_set_data_arg (gimple gs, tree data_arg)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_PARALLEL);
  gs->gimple_omp_parallel.data_arg = data_arg;
}

/* GIMPLE_OMP_SECTION accessors. */

/* GIMPLE_OMP_SINGLE accessors. */

static inline tree
gimple_omp_single_clauses (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_SINGLE);
  return gs->gimple_omp_single.clauses;
}

static inline tree *
gimple_omp_single_clauses_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_SINGLE);
  return &gs->gimple_omp_single.clauses;
}

static inline void
gimple_omp_single_set_clauses (gimple gs, tree clauses)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_SINGLE);
  gs->gimple_omp_single.clauses = clauses;
}

static inline tree
gimple_omp_sections_clauses (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_SECTIONS);
  return gs->gimple_omp_sections.clauses;
}

static inline tree *
gimple_omp_sections_clauses_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_SECTIONS);
  return &gs->gimple_omp_sections.clauses;
}

static inline void
gimple_omp_sections_set_clauses (gimple gs, tree clauses)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_SECTIONS);
  gs->gimple_omp_sections.clauses = clauses;
}


/* Get or set the OMP_FOR_COND stored in the subcode flags.  */

static inline void
gimple_omp_for_set_cond (gimple gs, enum tree_code cond)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  gcc_assert (TREE_CODE_CLASS (cond) == tcc_comparison);
  set_gimple_subcode (gs, cond);
}

static inline enum tree_code
gimple_omp_for_cond (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_OMP_FOR);
  return gimple_subcode (gs);
}

/* GIMPLE_RETURN accessors.  */

static inline tree
gimple_return_retval (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_RETURN);
  gcc_assert (gs->with_ops.num_ops == 1);
  return gs->with_ops.op[0];
}

static inline void
gimple_return_set_retval (gimple gs, tree retval)
{
  GIMPLE_CHECK (gs, GIMPLE_RETURN);
  gcc_assert (gs->with_ops.num_ops == 1);
  gs->with_ops.op[0] = retval;
}


/* Returns true when the gimple statment STMT is any of the OpenMP types.  */

static inline bool
is_gimple_omp (gimple stmt)
{
  return (gimple_code (stmt) == GIMPLE_OMP_PARALLEL
	  || gimple_code (stmt) == GIMPLE_OMP_FOR
	  || gimple_code (stmt) == GIMPLE_OMP_SECTIONS
	  || gimple_code (stmt) == GIMPLE_OMP_SINGLE
	  || gimple_code (stmt) == GIMPLE_OMP_SECTION
	  || gimple_code (stmt) == GIMPLE_OMP_MASTER
	  || gimple_code (stmt) == GIMPLE_OMP_ORDERED
	  || gimple_code (stmt) == GIMPLE_OMP_CRITICAL
	  || gimple_code (stmt) == GIMPLE_OMP_RETURN
	  || gimple_code (stmt) == GIMPLE_OMP_CONTINUE);
}


/* GIMPLE_NOP.  */

/* Returns TRUE if statement G is a GIMPLE_NOP.  */

static inline bool
gimple_nop_p (gimple g)
{
  return gimple_code (g) == GIMPLE_NOP;
}


/* Iterator object for GIMPLE statement sequences.  */

typedef struct {
    /* Current statement.  */
    gimple stmt;

    /* Sequence and basic block holding the statement.  These fields
       are necessary to handle edge cases such as one a statement is
       added to an empty basic block or when the last statement of a
       block/sequence is removed.  */
    gimple_seq seq;
    basic_block bb;
} gimple_stmt_iterator;


/* Return a new iterator pointing to GIMPLE_SEQ's first statement.  */

static inline gimple_stmt_iterator *
gsi_start (gimple_seq seq)
{
  gimple_stmt_iterator *i = ggc_alloc_cleared (sizeof (gimple_stmt_iterator));

  if (seq)
    {
      i->stmt = gimple_seq_first (seq);
      i->seq = seq;
    }

  if (i->stmt)
    i->bb = gimple_bb (i->stmt);

  return i;
}


/* Return a new iterator pointing to the first statement in basic block BB.  */

static inline gimple_stmt_iterator *
gsi_start_bb (basic_block bb)
{
  gimple_stmt_iterator *i = ggc_alloc_cleared (sizeof (gimple_stmt_iterator));
  gimple_seq seq = bb_seq (bb);

  if (seq)
    {
      i->stmt = gimple_seq_first (seq);
      i->seq = seq;
    }

  i->bb = bb;

  return i;
}


/* Return a new iterator initially pointing to GIMPLE_SEQ's last statement.  */

static inline gimple_stmt_iterator *
gsi_last (gimple_seq seq)
{
  gimple_stmt_iterator *i = ggc_alloc_cleared (sizeof (gimple_stmt_iterator));

  if (seq)
    {
      i->stmt = gimple_seq_last (seq);
      i->seq = seq;
    }

  if (i->stmt)
    i->bb = gimple_bb (i->stmt);

  return i;
}


/* Return a new iterator pointing to the first statement in basic block BB.  */

static inline gimple_stmt_iterator *
gsi_last_bb (basic_block bb)
{
  gimple_stmt_iterator *i = ggc_alloc_cleared (sizeof (gimple_stmt_iterator));
  gimple_seq seq = bb_seq (bb);

  if (seq)
    {
      i->stmt = gimple_seq_last (seq);
      i->seq = seq;
    }

  i->bb = bb;

  return i;
}


/* Return TRUE if at the end of I.  */

static inline bool
gsi_end_p (gimple_stmt_iterator *i)
{
  return i->stmt == NULL;
}

/* Return TRUE if we're one statement before the end of I.  */

static inline bool
gsi_one_before_end_p (gimple_stmt_iterator *i)
{
  return i->stmt == gimple_seq_last (i->seq);
}

/* Return the next gimple statement in I.  */

static inline void
gsi_next (gimple_stmt_iterator *i)
{
#if defined ENABLE_GIMPLE_CHECKING
  /* The last statement of the sequence should not have anything
     chained after it.  */
  gimple next = gimple_next (i->stmt);
  if (i->stmt == gimple_seq_last (i->seq))
    gcc_assert (next == NULL);
#endif
  i->stmt = gimple_next (i->stmt);
}

/* Return the previous gimple statement in I.  */

static inline void
gsi_prev (gimple_stmt_iterator *i)
{
#if defined ENABLE_GIMPLE_CHECKING
  /* The first statement of the sequence should not have anything
     chained before it.  */
  gimple prev = gimple_prev (i->stmt);
  if (i->stmt == gimple_seq_first (i->seq))
    gcc_assert (prev == NULL);
#endif
  i->stmt = gimple_prev (i->stmt);
}

/* Return the current stmt.  */

static inline gimple
gsi_stmt (gimple_stmt_iterator *i)
{
  return i->stmt;
}


/* Remove the current stmt from the sequence.  The iterator is updated to
   point to the next statement.

   When REMOVE_EH_INFO is true we remove the statement pointed to by
   iterator I from the EH tables.  Otherwise we do not modify the EH
   tables.

   Generally, REMOVE_EH_INFO should be true when the statement is going to
   be removed from the IL and not reinserted elsewhere.  */

static inline void
gsi_remove (gimple_stmt_iterator *i, bool remove_eh_info)
{
  gimple stmt = i->stmt;
  gimple_seq seq = i->seq;
  gsi_next (i);
  gimple_remove (stmt, seq, remove_eh_info);
}

enum gsi_iterator_update
{
  GSI_NEW_STMT,		/* Only valid when single statement is added, move
			   iterator to it.  */
  GSI_SAME_STMT,	/* Leave the iterator at the same statement.  */
  GSI_CONTINUE_LINKING	/* Move iterator to whatever position is suitable
			   for linking other statements in the same
			   direction.  */
};

void gsi_link_seq_before (gimple_stmt_iterator *, gimple_seq,
			  enum gsi_iterator_update);
void gsi_link_before (gimple_stmt_iterator *, gimple,
    		      enum gsi_iterator_update);
void gsi_link_seq_after (gimple_stmt_iterator *, gimple_seq,
			 enum gsi_iterator_update);
void gsi_link_after (gimple_stmt_iterator *, gimple, enum gsi_iterator_update);
gimple_seq gsi_split_seq_after (const gimple_stmt_iterator *);
gimple_seq gsi_split_seq_before (gimple_stmt_iterator *);
void gsi_replace (gimple_stmt_iterator *, gimple, bool);
void gsi_insert_before (gimple_stmt_iterator *, gimple,
			enum gsi_iterator_update);
void gsi_insert_seq_before (gimple_stmt_iterator *, gimple_seq,
		       enum gsi_iterator_update);
void gsi_insert_after (gimple_stmt_iterator *, gimple,
		       enum gsi_iterator_update);
void gsi_insert_seq_after (gimple_stmt_iterator *, gimple_seq,
			   enum gsi_iterator_update);
gimple_stmt_iterator *gsi_for_stmt (gimple);
void gsi_move_after (gimple_stmt_iterator *, gimple_stmt_iterator *);
void gsi_move_before (gimple_stmt_iterator *, gimple_stmt_iterator *);
void gsi_move_to_bb_end (gimple_stmt_iterator *, struct basic_block_def *);


/* Callback for walk_gimple_stmt.  Called for every statement found
   during traversal.  */
typedef bool (*walk_stmt_fn) (gimple, void *);

/* Convenience routines to walk all statements of a gimple function.
   Note that this is useful exclusively before the code is converted
   into SSA form.  Once the program is in SSA form, the standard
   operand interface should be used to analyze/modify statements.  */
struct walk_stmt_info
{
  /* Points to the current statement being walked.  */
  gimple_stmt_iterator *gsi;

  /* Additional data that CALLBACK may want to carry through the
     recursion.  */
  void *info;

  /* Indicates whether the *TP being examined may be replaced
     with something that matches is_gimple_val (if true) or something
     slightly more complicated (if false).  "Something" technically
     means the common subset of is_gimple_lvalue and is_gimple_rhs,
     but we never try to form anything more complicated than that, so
     we don't bother checking.

     Also note that CALLBACK should update this flag while walking the
     sub-expressions of a statement.  For instance, when walking the
     statement 'foo (&var)', the flag VAL_ONLY will initially be set
     to true, however, when walking &var, the operand of that
     ADDR_EXPR does not need to be a GIMPLE value.  */
  bool val_only;

  /* True if we are currently walking the LHS of an assignment.  */
  bool is_lhs;

  /* Optional.  Set to true by CALLBACK if it made any changes.  */
  bool changed;

  /* True if we're interested in seeing BIND_EXPRs.  */
  bool want_bind_expr;

  /* True if we're interested in seeing RETURN_EXPRs.  */
  bool want_return_expr;

  /* True if we're interested in location information.  */
  bool want_locations;
};

void walk_gimple_seq (gimple_seq, walk_stmt_fn, walk_tree_fn,
		      struct walk_stmt_info *);
void walk_gimple_stmt (gimple, walk_stmt_fn, walk_tree_fn,
		       struct walk_stmt_info *);

#endif  /* GCC_GIMPLE_H */
