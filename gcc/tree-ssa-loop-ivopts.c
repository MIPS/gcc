/* Induction variable optimizations.
   Copyright (C) 2003 Free Software Foundation, Inc.
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* This pass tries to find the optimal set of induction variables for the loop.
   It optimizes just the basic linear induction variables (although adding
   support for other types should not be too hard).  It includes the
   optimizations commonly known as strength reduction, induction variable
   coalescing and induction variable elimination.  It does it in the
   following steps:

   1) The interesting uses of induction variables are found.  This includes

      -- uses of induction variables in non-linear expressions
      -- adresses of arrays
      -- comparisons of induction variables

   2) Candidates for the induction variables are found.  This includes

      -- old induction variables
      -- the variables defined by expressions derived from the "interesting
	 uses" above

   3) The optimal (w.r. to a cost function) set of variables is chosen.  The
      cost function assigns a cost to sets of induction variables and consists
      of three parts:

      -- The use costs.  Each of the interesting uses choses the best induction
	 variable in the set and adds its cost to the sum.  The cost reflects
	 the time spent on modifying the induction variables value to be usable
	 for the given purpose (adding base and offset for arrays, etc.).
      -- The variable costs.  Each of the variables has a cost assigned that
	 reflects the costs assoctiated with incrementing the value of the
	 variable.  The original variables are somewhat preferred.
      -- The set cost.  Depending on the size of the set, extra cost may be
	 added to reflect register pressure.

      All the costs are defined in a machine-specific way, using the target
      hooks and machine descriptions to determine them.

   4) The trees are transformed to use the new variables, the dead code is
      removed.
   
   All of this is done loop by loop.  Doing it globally is theoretically
   possible, it might give a better performance and it might enable us
   to decide costs more precisely, but getting all the interactions right
   would be complicated.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "timevar.h"
#include "cfgloop.h"
#include "varray.h"
#include "expr.h"
#include "tree-pass.h"
#include "ggc.h"
#include "insn-config.h"
#include "recog.h"
#include "hashtab.h"

/* The infinite cost.  */
#define INFTY 10000000

/* Just to shorten the ugly names.  */
#define EXEC_BINARY nondestructive_fold_binary_to_constant
#define EXEC_UNARY nondestructive_fold_unary_to_constant

/* Representation of the induction variable.  */
struct iv
{
  tree base;		/* Initial value of the iv.  */
  tree step;		/* Step of the iv (constant only).  */
  tree ssa_name;	/* The ssa name with the value.  */
  bool biv_p;		/* Is it a biv?  */
  bool have_use_for;	/* Do we already have a use for it?  */
};

/* The currently optimized loop.  ??? Perhaps it would be better to pass it
   everywhere in argument, so that the functions are easier to generalize
   if needed?  */
static struct loop *current_loop;

/* The size of version_info array allocated.  */
static unsigned version_info_size;

/* Per-ssa version information (induction variable descriptions, etc.).  */
struct version_info
{
  tree name;		/* The ssa name.  */
  struct iv *iv;	/* Induction variable description.  */
  bool has_nonlin_use;	/* For a loop-level invariant, whether it is used in
			   an expression that is not an induction variable.  */
  unsigned inv_id;	/* Id of an invariant.  */
  struct loop *outermost_usage;
			/* The outermost loop in that the variable is used.  */
};

/* The array of this information indexed by the ssa name version.  */
static struct version_info *version_info;

/* The bitmap of indices in version_info whose value was changed.  */
static bitmap relevant;

/* The maximum invariant id.  */
static unsigned max_inv_id;

/* Description of number of iterations of a loop.  */
struct tree_niter_desc
{
  tree assumptions;	/* Assumptions for the number of iterations be valid.  */
  tree may_be_zero;	/* Condition under that the loop exits in the first
			   iteration.  */
  tree niter;		/* Number of iterations.  */
};

/* Information attached to loop.  */
struct loop_data
{
  unsigned n_exits;	/* Number of exit edges.  */
  edge single_exit;	/* The exit edge in case there is exactly one and
			   its source dominates the loops latch.  */
  struct tree_niter_desc niter;
			/* Number of iterations.  */

  unsigned regs_used;	/* Number of registers used.  */
};

#define LOOP_DATA(LOOP) ((struct loop_data *) (LOOP)->aux)

/* Types of uses.  */
enum use_type
{
  USE_NONLINEAR_EXPR,	/* Use in a nonlinear expression.  */
  USE_ADDRESS,		/* Use in an address.  */
  USE_COMPARE		/* Use is a compare.  */
};

/* The candidate - cost pair.  */
struct cost_pair
{
  struct iv_cand *cand;	/* The candidate.  */
  unsigned cost;	/* The cost.  */
  bitmap depends_on;	/* The list of invariants that have to be
			   preserved.  */
};

/* Use.  */
struct iv_use
{
  unsigned id;		/* The id of the use.  */
  enum use_type type;	/* Type of the use.  */
  struct iv *iv;	/* The induction variable it is based on.  */
  tree stmt;		/* Statement in that it occurs.  */
  tree *op_p;		/* The place where it occurs.  */
  bitmap related_cands;	/* The set of "related" iv candidates.  */

  unsigned n_map_members; /* Number of candidates in the cost_map list.  */
  struct cost_pair *cost_map;
			/* The costs wrto the iv candidates.  */

  struct iv_cand *selected;
			/* The selected candidate.  */
};

/* The uses of induction variables.  */
static varray_type iv_uses;

/* Disabled -- the dominator optimizations tend to reuse the value from
   the previous iteration, which increases register pressure.  */
#define DISABLE_IP_START 0

/* The position where the iv is computed.  */
enum iv_position
{
#if DISABLE_IP_START
  IP_START,		/* At the very beginning of the loop body.  */
#endif
  IP_NORMAL,		/* At the end, just before the exit condition.  */
  IP_END		/* At the end of the latch block.  */
};

/* The induction variable candidate.  */
struct iv_cand
{
  unsigned id;		/* The number of the candidate.  */
  bool important;	/* Whether this is an "important" candidate, i.e. such
			   that it should be considered by all uses.  */
  enum iv_position pos;	/* Where it is computed.  */
  tree var_before;	/* The variable used for it before incrementation.  */
  tree var_after;	/* The variable used for it after incrementation.  */
  struct iv *iv;	/* The value of the candidate.  */
  unsigned cost;	/* Cost of the candidate.  */
};

/* The candidates.  */
static varray_type iv_candidates;

/* Whether to consider just related and important candidates when replacing an
   use.  */
static bool consider_all_candidates;

/* Bound on number of candidates below that all candidates are considered.  */

#define CONSIDER_ALL_CANDIDATES_BOUND 15

/* The properties of the target.  */

static unsigned avail_regs;	/* Number of available registers.  */
static unsigned res_regs;	/* Number of reserved registers.  */
static unsigned small_cost;	/* The cost for register when there is a free one.  */
static unsigned pres_cost;	/* The cost for register when there are not too many
				   free ones.  */
static unsigned spill_cost;	/* The cost for register when we need to spill.  */

/* The list of trees for that the decl_rtl field must be reset is stored
   here.  */

static varray_type decl_rtl_to_reset;

#define SWAP(X, Y) do { void *tmp = (X); (X) = (Y); (Y) = tmp; } while (0)

static tree force_gimple_operand (tree, tree *, tree, bool);

/* Dumps information about the induction variable IV to FILE.  */

extern void dump_iv (FILE *, struct iv *);
void
dump_iv (FILE *file, struct iv *iv)
{
  fprintf (file, "ssa name ");
  print_generic_expr (file, iv->ssa_name, TDF_SLIM);
  fprintf (file, "\n");

  if (iv->step)
    {
      fprintf (file, "  base ");
      print_generic_expr (file, iv->base, TDF_SLIM);
      fprintf (file, "\n");

      fprintf (file, "  step ");
      print_generic_expr (file, iv->step, TDF_SLIM);
      fprintf (file, "\n");
    }
  else
    {
      fprintf (file, "  invariant ");
      print_generic_expr (file, iv->base, TDF_SLIM);
      fprintf (file, "\n");
    }

  if (iv->biv_p)
    fprintf (file, "  is a biv\n");
}

/* Dumps information about the USE to FILE.  */

extern void dump_use (FILE *, struct iv_use *);
void
dump_use (FILE *file, struct iv_use *use)
{
  struct iv *iv = use->iv;

  fprintf (file, "use %d\n", use->id);

  switch (use->type)
    {
    case USE_NONLINEAR_EXPR:
      fprintf (file, "  generic\n");
      break;

    case USE_ADDRESS:
      fprintf (file, "  address\n");
      break;

    case USE_COMPARE:
      fprintf (file, "  compare\n");
      break;
    }

   fprintf (file, "  in statement ");
   print_generic_expr (file, use->stmt, TDF_SLIM);
   fprintf (file, "\n");

   fprintf (file, "  at position ");
   print_generic_expr (file, *use->op_p, TDF_SLIM);
   fprintf (file, "\n");

   if (iv->step)
     {
       fprintf (file, "  base ");
       print_generic_expr (file, iv->base, TDF_SLIM);
       fprintf (file, "\n");

       fprintf (file, "  step ");
       print_generic_expr (file, iv->step, TDF_SLIM);
       fprintf (file, "\n");
     }
   else
     {
       fprintf (file, "  invariant ");
       print_generic_expr (file, iv->base, TDF_SLIM);
       fprintf (file, "\n");
     }

   fprintf (file, "  related candidates ");
   dump_bitmap (file, use->related_cands);
}

/* Dumps information about the uses to FILE.  */

extern void dump_uses (FILE *);
void
dump_uses (FILE *file)
{
  unsigned i;
  struct iv_use *use;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_uses); i++)
    {
      use = VARRAY_GENERIC_PTR_NOGC (iv_uses, i);

      dump_use (file, use);
      fprintf (file, "\n");
    }
}

/* Dumps information about induction variable candidate CAND to FILE.  */

extern void dump_cand (FILE *, struct iv_cand *);
void
dump_cand (FILE *file, struct iv_cand *cand)
{
  struct iv *iv = cand->iv;

  fprintf (file, "candidate %d%s\n",
	   cand->id, cand->important ? " (important)" : "");

  switch (cand->pos)
    {
#if DISABLE_IP_START
    case IP_START:
      fprintf (file, "  incremented at start\n");
      break;
#endif

    case IP_NORMAL:
      fprintf (file, "  incremented before exit test\n");
      break;

    case IP_END:
      fprintf (file, "  incremented at end\n");
      break;
    }

   if (iv->step)
     {
       fprintf (file, "  base ");
       print_generic_expr (file, iv->base, TDF_SLIM);
       fprintf (file, "\n");

       fprintf (file, "  step ");
       print_generic_expr (file, iv->step, TDF_SLIM);
       fprintf (file, "\n");
     }
   else
     {
       fprintf (file, "  invariant ");
       print_generic_expr (file, iv->base, TDF_SLIM);
       fprintf (file, "\n");
     }
}

/* Returns the info for ssa version VER.  */

static inline struct version_info *
ver_info (unsigned ver)
{
  return version_info + ver;
}

/* Returns the info for ssa name NAME.  */

static inline struct version_info *
name_info (tree name)
{
  return ver_info (SSA_NAME_VERSION (name));
}

/* Checks whether ARG is either NULL_TREE or constant zero.  */

static bool
zero_p (tree arg)
{
  if (!arg)
    return true;

  return integer_zerop (arg);
}

/* Checks that X is integer constant that fits in unsigned HOST_WIDE_INT.
   Similar to host_integerp (x, 1), but does not fail if the value is
   negative.  */

static bool
cst_and_fits_in_hwi (tree x)
{
  if (TREE_CODE (x) != INTEGER_CST)
    return false;

  return (TREE_INT_CST_HIGH (x) == 0
	  || TREE_INT_CST_HIGH (x) == -1);
}

/* Return value of a constant X.  */

static HOST_WIDE_INT
int_cst_value (tree x)
{
  unsigned bits = TYPE_PRECISION (TREE_TYPE (x));
  unsigned HOST_WIDE_INT val = TREE_INT_CST_LOW (x);
  bool negative = ((val >> (bits - 1)) & 1) != 0;

  if (negative)
    val |= (~(unsigned HOST_WIDE_INT) 0) << (bits - 1) << 1;
  else
    val &= ~((~(unsigned HOST_WIDE_INT) 0) << (bits - 1) << 1);

  return val;
}

/* Builds integer constant of type TYPE and value VAL.  */

static tree
build_int_cst (tree type, unsigned HOST_WIDE_INT val)
{
  unsigned bits = TYPE_PRECISION (type);
  bool signed_p = !TREE_UNSIGNED (type);
  bool negative = ((val >> (bits - 1)) & 1) != 0;
  tree ival;

  if (signed_p && negative)
    {
      val = val | (~(unsigned HOST_WIDE_INT) 0 << (bits - 1) << 1);
      ival = build_int_2 (val, -1);
    }
  else
    {
      val = val & ~(~(unsigned HOST_WIDE_INT) 0 << (bits - 1) << 1);
      ival = build_int_2 (val, 0);
    }

  return convert (type, ival);
}

/* Checks whether there exists number X such that X * B = A, counting modulo
   2^BITS.  */

static bool
divide (unsigned bits, unsigned HOST_WIDE_INT a, unsigned HOST_WIDE_INT b,
	HOST_WIDE_INT *x)
{
  unsigned HOST_WIDE_INT mask = ~(~(unsigned HOST_WIDE_INT) 0 << (bits - 1) << 1);
  unsigned HOST_WIDE_INT inv, ex, val;
  unsigned i;

  a &= mask;
  b &= mask;

  /* First divide the whole equation by 2 as long as possible.  */
  while (!(a & 1) && !(b & 1))
    {
      a >>= 1;
      b >>= 1;
      bits--;
      mask >>= 1;
    }

  if (!(b & 1))
    {
      /* If b is still even, a is odd and there is no such x.  */
      return false;
    }

  /* Find the inverse of b.  We compute it as
     b^(2^(bits - 1) - 1) (mod 2^bits).  */
  inv = 1;
  ex = b;
  for (i = 0; i < bits - 1; i++)
    {
      inv = (inv * ex) & mask;
      ex = (ex * ex) & mask;
    }

  val = (a * inv) & mask;

  if (((val * b) & mask) != a)
    abort ();

  if ((val >> (bits - 1)) & 1)
    val |= ~mask;

  *x = val;

  return true;
}

/* Calls CBCK for each index in ADDR_P.  It passes the pointer to the index,
   the base if it is an array and DATA to the callback.  If the callback returns
   false, the whole search stops and false is returned.  */

bool
for_each_index (tree *addr_p, bool (*cbck) (tree, tree *, void *), void *data)
{
  tree *nxt;

  for (; ; addr_p = nxt)
    {
      switch (TREE_CODE (*addr_p))
	{
	case SSA_NAME:
	  return cbck (NULL, addr_p, data);

	case INDIRECT_REF:
	  nxt = &TREE_OPERAND (*addr_p, 0);
	  return cbck (NULL, nxt, data);

	case BIT_FIELD_REF:
	case COMPONENT_REF:
	  nxt = &TREE_OPERAND (*addr_p, 0);
	  break;

	case ARRAY_REF:
	  nxt = &TREE_OPERAND (*addr_p, 0);
	  if (!cbck (*nxt, &TREE_OPERAND (*addr_p, 1), data))
	    return false;
	  break;

	case VAR_DECL:
	case PARM_DECL:
	case STRING_CST:
	case RESULT_DECL:
	  return true;

	default:
    	  abort ();
	}
    }
}

/* Forces IDX to be either constant or ssa name.  Callback for
   for_each_index.  */

struct idx_fs_data
{
  tree stmts;
  tree var;
};

static bool
idx_force_simple (tree base ATTRIBUTE_UNUSED, tree *idx, void *data)
{
  struct idx_fs_data *d = data;
  tree stmts;

  *idx = force_gimple_operand (*idx, &stmts, d->var, true);

  if (stmts)
    {
      tree_stmt_iterator tsi = tsi_start (d->stmts);
      tsi_link_before (&tsi, stmts, TSI_SAME_STMT);
    }

  return true;
}

/* Updates TREE_ADDRESSABLE flag for the base variable of EXPR.  */

static void
update_addressable_flag (tree expr)
{
  if (TREE_CODE (expr) != ADDR_EXPR)
    abort ();

  expr = TREE_OPERAND (expr, 0);
  while (TREE_CODE (expr) == ARRAY_REF
	 || TREE_CODE (expr) == COMPONENT_REF
	 || TREE_CODE (expr) == REALPART_EXPR
	 || TREE_CODE (expr) == IMAGPART_EXPR)
    expr = TREE_OPERAND (expr, 0);
  if (TREE_CODE (expr) != VAR_DECL
      && TREE_CODE (expr) != PARM_DECL)
    return;

  TREE_ADDRESSABLE (expr) = 1;
}

/* Expands EXPR to list of gimple statements STMTS, forcing it to become
   a gimple operand that is returned.  Temporary variables (if needed)
   are based on VAR.  If SIMPLE is true, force the operand to be either
   ssa_name or integer constant.  */

static tree
force_gimple_operand (tree expr, tree *stmts, tree var, bool simple)
{
  enum tree_code code = TREE_CODE (expr);
  char class = TREE_CODE_CLASS (code);
  tree op0, op1, stmts0, stmts1, stmt, rhs, name;
  tree_stmt_iterator tsi;
  struct idx_fs_data d;

  if (is_gimple_val (expr)
      && (!simple
	  || TREE_CODE (expr) == SSA_NAME
	  || TREE_CODE (expr) == INTEGER_CST))
    {
      if (code == ADDR_EXPR)
	update_addressable_flag (expr);

      *stmts = NULL_TREE;
      return expr;
    }

  if (code == ADDR_EXPR)
    {
      op0 = TREE_OPERAND (expr, 0);
      if (TREE_CODE (op0) == INDIRECT_REF)
	return force_gimple_operand (TREE_OPERAND (op0, 0), stmts, var,
				     simple);
    }

  if (!var)
    {
      var = create_tmp_var (TREE_TYPE (expr), "fgotmp");
      add_referenced_tmp_var (var);
    }

  switch (class)
    {
    case '1':
    case '2':
      op0 = force_gimple_operand (TREE_OPERAND (expr, 0), &stmts0, var, false);
      if (class == '2')
	{
	  op1 = force_gimple_operand (TREE_OPERAND (expr, 1), &stmts1, var, false);
	  rhs = build (code, TREE_TYPE (expr), op0, op1);
	}
      else
	{
	  rhs = build1 (code, TREE_TYPE (expr), op0);
	  stmts1 = NULL_TREE;
	}

      stmt = build (MODIFY_EXPR, void_type_node, var, rhs);
      name = make_ssa_name (var, stmt);
      TREE_OPERAND (stmt, 0) = name;

      if (stmts0)
	{
	  *stmts = stmts0;
	  if (stmts1)
	    {
	      tsi = tsi_last (*stmts);
	      tsi_link_after (&tsi, stmts1, TSI_CONTINUE_LINKING);
	    }
	}
      else if (stmts1)
	*stmts = stmts1;
      else
	*stmts = alloc_stmt_list ();

      tsi = tsi_last (*stmts);
      tsi_link_after (&tsi, stmt, TSI_CONTINUE_LINKING);
      return name;

    default:
      break;
    }

  if (TREE_CODE (expr) == ADDR_EXPR)
    {
      stmt = build (MODIFY_EXPR, void_type_node, var, expr);
      name = make_ssa_name (var, stmt);
      TREE_OPERAND (stmt, 0) = name;

      *stmts = alloc_stmt_list ();
      tsi = tsi_last (*stmts);
      tsi_link_after (&tsi, stmt, TSI_CONTINUE_LINKING);

      d.stmts = *stmts;
      d.var = var;
      for_each_index (&TREE_OPERAND (expr, 0), idx_force_simple, &d);

      update_addressable_flag (TREE_OPERAND (stmt, 1));

      return name;
    }

  abort ();
}

/* If TYPE is an array type, corresponding pointer type is returned,
   otherwise the TYPE is returned unchanged.  */

static tree
array2ptr (tree type)
{
  if (TREE_CODE (type) != ARRAY_TYPE)
    return type;

  return build_pointer_type (TREE_TYPE (type));
}

/* Sets single_exit field for loops.  */

static void
find_exit_edges (void)
{
  basic_block bb;
  edge e;
  struct loop *src, *dest;;

  FOR_EACH_BB (bb)
    {
      for (e = bb->succ; e; e = e->succ_next)
	{
	  src = e->src->loop_father;
	  dest = find_common_loop (src, e->dest->loop_father);

	  for (; src != dest; src = src->outer)
	    {
	      LOOP_DATA (src)->n_exits++;
	      if (LOOP_DATA (src)->n_exits > 1)
		{
		  LOOP_DATA (src)->single_exit = NULL;
		  continue;
		}

	      if (!dominated_by_p (CDI_DOMINATORS, src->latch, e->src))
		continue;

	      LOOP_DATA (src)->single_exit = e;
	    }
	}
    }
}

/* Update usage information about OP using the fact that it is used in
   the LOOP.  */

static void
update_outermost_usage (tree op, struct loop *loop)
{
  struct version_info *info;

  if (TREE_CODE (op) != SSA_NAME)
    return;
  info = name_info (op);

  if (!info->outermost_usage)
    info->outermost_usage = loop;
  else
    info->outermost_usage = find_common_loop (loop, info->outermost_usage);
}

/* Finds the outermost loop in that each ssa name is used.  */

static void
find_outermost_usage (void)
{
  basic_block bb;
  block_stmt_iterator bsi;
  unsigned i;
  use_optype uses;
  tree use, stmt, phi;
  struct loop *loop, *aloop;

  FOR_EACH_BB (bb)
    {
      loop = bb->loop_father;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	for (i = 0; i < (unsigned) PHI_NUM_ARGS (phi); i++)
	  {
	    use = PHI_ARG_DEF (phi, i);

	    /* Use on the entry edge of a loop counts as use in the
	       superloop.  */
	    if (loop->header == bb
		&& PHI_ARG_EDGE (phi, i) == loop_preheader_edge (loop))
	      aloop = loop->outer;
	    else
	      aloop = loop;
	    update_outermost_usage (use, aloop);
	  }

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  stmt = bsi_stmt (bsi);
	  get_stmt_operands (stmt);

	  uses = STMT_USE_OPS (stmt);
	  for (i = 0; i < NUM_USES (uses); i++)
	    {
	      use = USE_OP (uses, i);
	      update_outermost_usage (use, loop);
	    }
	}
    }
}

/* Returns the basic block in that statements should be emitted for IP_END
   position.  */

static basic_block
ip_end_pos (void)
{
  tree last;
  basic_block bb;

  bb = current_loop->latch;
  last = last_stmt (bb);
#if DISABLE_IP_START
  /* If the latch is empty, preserve this (inserting the latch block might need
     extra jumps, which might spoil the code).  */
  if (!last || TREE_CODE (last) == LABEL_EXPR)
    return NULL;
#endif

  return bb;
}

/* Returns the basic block in that statements should be emitted for IP_NORMAL
   position.  */

static basic_block
ip_normal_pos (void)
{
  tree last;
  basic_block bb;

  if (current_loop->latch->pred->pred_next)
    return NULL;

  bb = current_loop->latch->pred->src;
  last = last_stmt (bb);
  if (TREE_CODE (last) != COND_EXPR)
    return NULL;

  return bb;
}

/* Returs true if STMT is after the place where the IP_NORMAL ivs will be
   emitted.  */

static bool
stmt_after_ip_normal_pos (tree stmt)
{
  basic_block bb = ip_normal_pos (), sbb = bb_for_stmt (stmt);

  if (!bb)
    abort ();

  if (sbb == current_loop->latch)
    return true;

  if (sbb != bb)
    return false;

  return stmt == last_stmt (bb);
}

/* Initializes data structures used by the iv optimization pass.  LOOPS is the
   loop tree.  */

static void
tree_ssa_iv_optimize_init (struct loops *loops)
{
  unsigned i;

  version_info_size = 2 * highest_ssa_version;
  version_info = xcalloc (version_info_size, sizeof (struct version_info));
  relevant = BITMAP_XMALLOC ();

  for (i = 1; i < loops->num; i++)
    if (loops->parray[i])
      loops->parray[i]->aux = xcalloc (1, sizeof (struct loop_data));

  find_exit_edges ();
  find_outermost_usage ();

  VARRAY_GENERIC_PTR_NOGC_INIT (iv_uses, 20, "iv_uses");
  VARRAY_GENERIC_PTR_NOGC_INIT (iv_candidates, 20, "iv_candidates");
  VARRAY_GENERIC_PTR_NOGC_INIT (decl_rtl_to_reset, 20, "decl_rtl_to_reset");
}

/* Allocates an induction variable with given initial value BASE and step STEP
   for loop LOOP.  */

static struct iv *
alloc_iv (tree base, tree step)
{
  struct iv *iv = xcalloc (1, sizeof (struct iv));

  if (step && integer_zerop (step))
    step = NULL_TREE;

  iv->base = base;
  iv->step = step;
  iv->biv_p = false;
  iv->have_use_for = false;
  iv->ssa_name = NULL_TREE;

  return iv;
}

/* Sets STEP and BASE for induction variable IV.  */

static void
set_iv (tree iv, tree base, tree step)
{
  struct version_info *info = name_info (iv);

  if (info->iv)
    abort ();

  bitmap_set_bit (relevant, SSA_NAME_VERSION (iv));
  info->iv = alloc_iv (base, step);
  info->iv->ssa_name = iv;
}

/* Finds induction variable declaration for VAR.  */

static struct iv *
get_iv (tree var)
{
  basic_block bb;
  
  if (!name_info (var)->iv)
    {
      bb = bb_for_stmt (SSA_NAME_DEF_STMT (var));

      if (!bb
	  || !flow_bb_inside_loop_p (current_loop, bb))
	set_iv (var, var, NULL_TREE);
    }

  return name_info (var)->iv;
}

/* Determines the step of a biv defined in PHI.  */

static tree
determine_biv_step (tree phi)
{
  tree var = phi_element_for_edge (phi, loop_latch_edge (current_loop))->def;
  tree type = TREE_TYPE (var);
  tree step, stmt, lhs, rhs, op0, op1;
  enum tree_code code;

  if (TREE_CODE (var) != SSA_NAME
      || !is_gimple_reg (var))
    return NULL_TREE;

  /* Just work for integers and pointers.  */
  if (TREE_CODE (type) != INTEGER_TYPE
      && TREE_CODE (type) != POINTER_TYPE)
    return NULL_TREE;

  step = convert (type, integer_zero_node);

  while (1)
    {
      stmt = SSA_NAME_DEF_STMT (var);
      if (stmt == phi)
	return step;

      if (TREE_CODE (stmt) != MODIFY_EXPR)
	return NULL_TREE;

      lhs = TREE_OPERAND (stmt, 0);
      rhs = TREE_OPERAND (stmt, 1);

      if (lhs != var)
	return NULL_TREE;

      code = TREE_CODE (rhs);
      switch (code)
	{
	case SSA_NAME:
	  var = rhs;
	  break;

	case PLUS_EXPR:
	case MINUS_EXPR:
	  op0 = TREE_OPERAND (rhs, 0);
	  op1 = TREE_OPERAND (rhs, 1);

	  if (TREE_CODE (op1) != INTEGER_CST)
	    {
	      if (code == MINUS_EXPR
		  || TREE_CODE (op0) != INTEGER_CST)
		return NULL_TREE;

	      SWAP (op0, op1);
	    }

	  if (TREE_CODE (op0) != SSA_NAME)
	    return NULL_TREE;

	  step = EXEC_BINARY (code, type, step, op1);
	  var = op0;
	  break;
	  
	default:
	  return NULL_TREE;
	}
    }
}

/* Finds basic ivs.  */

static bool
find_bivs (void)
{
  tree phi, step;
  bool found = false;

  for (phi = phi_nodes (current_loop->header); phi; phi = TREE_CHAIN (phi))
    {
      step = determine_biv_step (phi);
      if (!step)
	continue;

      set_iv (PHI_RESULT (phi),
	      phi_element_for_edge (phi, loop_preheader_edge (current_loop))->def,
	      step);
      found |= (integer_nonzerop (step) != 0);
    }

  return found;
}

/* Marks basic ivs.  */

static void
mark_bivs (void)
{
  tree phi, stmt, var, rhs, op0, op1;
  struct iv *iv;

  for (phi = phi_nodes (current_loop->header); phi; phi = TREE_CHAIN (phi))
    {
      iv = get_iv (PHI_RESULT (phi));
      if (!iv)
	continue;

      iv->biv_p = true;
      var = phi_element_for_edge (phi, loop_latch_edge (current_loop))->def;
      stmt = SSA_NAME_DEF_STMT (var);

      while (TREE_CODE (stmt) != PHI_NODE)
	{
	  var = TREE_OPERAND (stmt, 0);
	  iv = get_iv (var);
	  iv->biv_p = true;

	  rhs = TREE_OPERAND (stmt, 1);
	  switch (TREE_CODE (rhs))
	    {
	    case SSA_NAME:
	      var = rhs;
	      break;

	    case PLUS_EXPR:
	    case MINUS_EXPR:
	      op0 = TREE_OPERAND (rhs, 0);
	      op1 = TREE_OPERAND (rhs, 1);
	      var = TREE_CODE (op1) != INTEGER_CST ? op1 : op0;
	      break;

	    default:
	      abort ();
	    }

	  stmt = SSA_NAME_DEF_STMT (var);
	}
    }
}

/* Finds definition of VAR and fills in BASE and STEP accordingly.  */

static bool
get_var_def (tree var, tree *base, tree *step)
{
  struct iv *iv;
  
  if (is_gimple_min_invariant (var))
    {
      *base = var;
      *step = NULL_TREE;
      return true;
    }

  iv = get_iv (var);
  if (!iv)
    return false;

  *base = iv->base;
  *step = iv->step;

  return true;
}

/* Finds general ivs in statement STMT.  */

static void
find_givs_in_stmt (tree stmt)
{
  tree lhs, rhs, op0, op1, mul = NULL_TREE;
  bool negate = false;
  tree base = NULL_TREE, step = NULL_TREE, type;
  tree base0 = NULL_TREE, step0 = NULL_TREE;
  enum tree_code code;
  char class;

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return;

  lhs = TREE_OPERAND (stmt, 0);
  rhs = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (lhs) != SSA_NAME)
    return;

  type = TREE_TYPE (lhs);
  if (TREE_CODE (type) != INTEGER_TYPE
      && TREE_CODE (type) != POINTER_TYPE)
    return;

  code = TREE_CODE (rhs);
  class = TREE_CODE_CLASS (code);
  if (class == '1' || class == '2')
    {
      op0 = TREE_OPERAND (rhs, 0);
      if (!get_var_def (op0, &base0, &step0))
	return;
    }

  if (class == '2')
    {
      op1 = TREE_OPERAND (rhs, 1);
      if (!get_var_def (op1, &base, &step))
	return;
    }

  switch (code)
    {
    case SSA_NAME:
      if (!get_var_def (rhs, &base, &step))
	return;
      break;

    case PLUS_EXPR:
      break;

    case MINUS_EXPR:
      negate = true;
      break;

    case MULT_EXPR:
      if (step0)
	{
	  if (step)
	    return;

	  if (TREE_CODE (base) != INTEGER_CST)
	    return;

	  mul = base;
	  base = base0;
	  step = step0;
	  base0 = NULL_TREE;
	  step0 = NULL_TREE;
	}
      else if (step)
	{
	  if (TREE_CODE (base0) != INTEGER_CST)
	    return;

	  mul = base0;
	  base0 = NULL_TREE;
	}
      else
	return;

      break;
	  
    case NEGATE_EXPR:
      negate = true;
      base = base0;
      step = step0;
      base0 = NULL_TREE;
      step0 = NULL_TREE;
      break;

    default:
      return;
    }

  if (negate)
    {
      base = fold (build1 (NEGATE_EXPR, type, base));
      if (step)
	step = EXEC_UNARY (NEGATE_EXPR, type, step);
    }

  if (mul)
    {
      base = fold (build (MULT_EXPR, type, mul, base));
      if (step)
	step = EXEC_BINARY (MULT_EXPR, type, mul, step);
    }

  if (base0)
    base = fold (build (PLUS_EXPR, type, base, base0));
    
  if (step0)
    {
      if (step)
	step = EXEC_BINARY (PLUS_EXPR, type, step, step0);
      else
	step = step0;
    }
  
  set_iv (lhs, base, step);
}

/* Finds general ivs in basic block BB.  */

static void
find_givs_in_bb (basic_block bb)
{
  block_stmt_iterator bsi;

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    find_givs_in_stmt (bsi_stmt (bsi));
}

/* Finds general ivs.  */

static void
find_givs (void)
{
  basic_block *body = get_loop_body_in_dom_order (current_loop);
  unsigned i;

  for (i = 0; i < current_loop->num_nodes; i++)
    find_givs_in_bb (body[i]);
  free (body);
}

/* Computes inverse of X modulo 2^s, where MASK = 2^s-1.  */

static tree
inverse (tree x, tree mask)
{
  tree type = TREE_TYPE (x);
  tree ctr = EXEC_BINARY (RSHIFT_EXPR, type, mask, integer_one_node);
  tree rslt = convert (type, integer_one_node);

  while (integer_nonzerop (ctr))
    {
      rslt = EXEC_BINARY (MULT_EXPR, type, rslt, x);
      rslt = EXEC_BINARY (BIT_AND_EXPR, type, rslt, mask);
      x = EXEC_BINARY (MULT_EXPR, type, x, x);
      x = EXEC_BINARY (BIT_AND_EXPR, type, x, mask);
      ctr = EXEC_BINARY (RSHIFT_EXPR, type, ctr, integer_one_node);
    }

  return rslt;
}

/* Determine the number of iterations according to condition COND (for staying
   inside loop).  Store the results to NITER.  */

static void
number_of_iterations_cond (tree cond, struct tree_niter_desc *niter)
{
  tree type, op0, op1;
  enum tree_code code;
  tree base0, step0, base1, step1, step, delta, mmin, mmax;
  tree may_xform, bound, s, d, tmp;
  bool was_sharp = false;
  tree assumption;
  tree assumptions = boolean_true_node;
  tree noloop_assumptions = boolean_false_node;
  tree unsigned_step_type;

  /* The meaning of these assumptions is this:
     if !assumptions
       then the rest of information does not have to be valid
     if noloop_assumptions then the loop does not have to roll
       (but it is only conservative approximation, i.e. it only says that
       if !noloop_assumptions, then the loop does not end before the computed
       number of iterations)  */

  code = TREE_CODE (cond);
  switch (code)
    {
    case GT_EXPR:
    case GE_EXPR:
    case NE_EXPR:
    case LT_EXPR:
    case LE_EXPR:
      break;

    default:
      return;
    }
  
  op0 = TREE_OPERAND (cond, 0);
  op1 = TREE_OPERAND (cond, 1);
  type = TREE_TYPE (op0);

  if (TREE_CODE (type) != INTEGER_TYPE
    && TREE_CODE (type) != POINTER_TYPE)
    return;
      
  if (!get_var_def (op0, &base0, &step0))
    return;
  if (!get_var_def (op1, &base1, &step1))
    return;

  /* Make < comparison from > ones.  */
  if (code == GE_EXPR
      || code == GT_EXPR)
    {
      SWAP (base0, base1);
      SWAP (step0, step1);
      code = swap_tree_comparison (code);
    }

  /* We can take care of the case of two induction variables chasing each other
     if the test is NE. I have never seen a loop using it, but still it is
     cool.  */
  if (!zero_p (step0) && !zero_p (step1))
    {
      if (code != NE_EXPR)
	return;

      step0 = EXEC_BINARY (MINUS_EXPR, type, step0, step1);
      step1 = NULL_TREE;
    }

  /* If the result is a constant,  the loop is weird.  More precise handling
     would be possible, but the situation is not common enough to waste time
     on it.  */
  if (zero_p (step0) && zero_p (step1))
    return;

  /* Ignore loops of while (i-- < 10) type.  */
  if (code != NE_EXPR)
    {
      if (step0 && !tree_expr_nonnegative_p (step0))
	return;

      if (!zero_p (step1) && tree_expr_nonnegative_p (step1))
	return;
    }

  /* For pointers these are NULL.  We assume pointer arithmetics never
     overflows.  */
  mmin = TYPE_MIN_VALUE (type);
  mmax = TYPE_MAX_VALUE (type);

  /* Some more condition normalization.  We must record some assumptions
     due to overflows.  */

  if (code == LT_EXPR)
    {
      /* We want to take care only of <=; this is easy,
	 as in cases the overflow would make the transformation unsafe the loop
	 does not roll.  Seemingly it would make more sense to want to take
	 care of <, as NE is more simmilar to it, but the problem is that here
	 the transformation would be more difficult due to possibly infinite
	 loops.  */
      if (zero_p (step0))
	{
	  if (mmax)
	    assumption = fold (build (EQ_EXPR, boolean_type_node, base0, mmax));
	  else
	    assumption = boolean_true_node;
	  if (integer_nonzerop (assumption))
	    goto zero_iter;
	  base0 = fold (build (PLUS_EXPR, type, base0, integer_one_node));
	}
      else
	{
	  if (mmin)
	    assumption = fold (build (EQ_EXPR, boolean_type_node, base1, mmin));
	  else
	    assumption = boolean_true_node;
	  if (integer_nonzerop (assumption))
	    goto zero_iter;
	  base1 = fold (build (MINUS_EXPR, type, base1, integer_one_node));
	}
      noloop_assumptions = assumption;
      code = LE_EXPR;

      /* It will be useful to be able to tell the difference once more in
	 <= -> != reduction.  */
      was_sharp = true;
    }

  /* Take care of trivially infinite loops.  */
  if (code != NE_EXPR)
    {
      if (zero_p (step0)
	  && mmin
	  && operand_equal_p (base0, mmin, 0))
	return;
      if (zero_p (step1)
	  && mmax
	  && operand_equal_p (base1, mmax, 0))
	return;
    }

  /* If we can we want to take care of NE conditions instead of size
     comparisons, as they are much more friendly (most importantly
     this takes care of special handling of loops with step 1).  We can
     do it if we first check that upper bound is greater or equal to
     lower bound, their difference is constant c modulo step and that
     there is not an overflow.  */
  if (code != NE_EXPR)
    {
      if (zero_p (step0))
	step = EXEC_UNARY (NEGATE_EXPR, type, step1);
      else
	step = step0;
      delta = build (MINUS_EXPR, type, base1, base0);
      delta = fold (build (FLOOR_MOD_EXPR, type, delta, step));
      may_xform = boolean_false_node;

      if (TREE_CODE (delta) == INTEGER_CST)
	{
	  tmp = EXEC_BINARY (MINUS_EXPR, type, step, integer_zero_node);
	  if (was_sharp
	      && operand_equal_p (delta, tmp, 0))
	    {
	      /* A special case.  We have transformed condition of type
		 for (i = 0; i < 4; i += 4)
		 into
		 for (i = 0; i <= 3; i += 4)
		 obviously if the test for overflow during that transformation
		 passed, we cannot overflow here.  Most importantly any
		 loop with sharp end condition and step 1 falls into this
		 cathegory, so handling this case specially is definitely
		 worth the troubles.  */
	      may_xform = boolean_true_node;
	    }
	  else if (zero_p (step0))
	    {
	      if (!mmin)
		may_xform = boolean_true_node;
	      else
		{
		  bound = EXEC_BINARY (PLUS_EXPR, type, mmin, step);
		  bound = EXEC_BINARY (MINUS_EXPR, type, bound, delta);
		  may_xform = fold (build (LE_EXPR, boolean_type_node,
					   bound, base0));
		}
	    }
	  else
	    {
	      if (!mmax)
		may_xform = boolean_true_node;
	      else
		{
		  bound = EXEC_BINARY (MINUS_EXPR, type, mmax, step);
		  bound = EXEC_BINARY (PLUS_EXPR, type, bound, delta);
		  may_xform = fold (build (LE_EXPR, boolean_type_node,
					   base1, bound));
		}
	    }
	}

      if (!integer_zerop (may_xform))
	{
	  /* We perform the transformation always provided that it is not
	     completely senseless.  This is OK, as we would need this assumption
	     to determine the number of iterations anyway.  */
	  if (!integer_nonzerop (may_xform))
	    assumptions = may_xform;

	  if (zero_p (step0))
	    {
	      base0 = build (PLUS_EXPR, type, base0, delta);
	      base0 = fold (build (MINUS_EXPR, type, base0, step));
	    }
	  else
	    {
	      base1 = build (MINUS_EXPR, type, base1, delta);
	      base1 = fold (build (PLUS_EXPR, type, base1, step));
	    }

	  assumption = fold (build (GT_EXPR, boolean_type_node, base0, base1));
	  noloop_assumptions = fold (build (TRUTH_OR_EXPR, boolean_type_node,
					    noloop_assumptions, assumption));
	  code = NE_EXPR;
	}
    }

  /* Count the number of iterations.  */
  if (code == NE_EXPR)
    {
      /* Everything we do here is just arithmetics modulo size of mode.  This
	 makes us able to do more involved computations of number of iterations
	 than in other cases.  First transform the condition into shape
	 s * i <> c, with s positive.  */
      base1 = fold (build (MINUS_EXPR, type, base1, base0));
      base0 = NULL_TREE;
      if (!zero_p (step1))
  	step0 = EXEC_UNARY (NEGATE_EXPR, type, step1);
      step1 = NULL_TREE;
      if (!tree_expr_nonnegative_p (step0))
	{
	  step0 = EXEC_UNARY (NEGATE_EXPR, type, step0);
	  base1 = fold (build1 (NEGATE_EXPR, type, base1));
	}

      /* Let nsd (s, size of mode) = d.  If d does not divide c, the loop
	 is infinite.  Otherwise, the number of iterations is
	 (inverse(s/d) * (c/d)) mod (size of mode/d).  */
      s = step0;
      d = integer_one_node;
      unsigned_step_type = make_unsigned_type (TYPE_PRECISION (type));
      bound = convert (unsigned_step_type, build_int_2 (~0, ~0));
      while (1)
	{
	  tmp = EXEC_BINARY (BIT_AND_EXPR, type, s, integer_one_node);
	  if (integer_nonzerop (tmp))
	    break;
	  
	  s = EXEC_BINARY (RSHIFT_EXPR, type, s, integer_one_node);
	  d = EXEC_BINARY (LSHIFT_EXPR, type, d, integer_one_node);
	  bound = EXEC_BINARY (RSHIFT_EXPR, type, bound, integer_one_node);
	}

      tmp = fold (build (EXACT_DIV_EXPR, type, base1, d));
      tmp = fold (build (MULT_EXPR, type, tmp, inverse (s, bound)));
      niter->niter = fold (build (BIT_AND_EXPR, type, tmp, bound));
    }
  else
    {
      if (zero_p (step1))
	/* Condition in shape a + s * i <= b
	   We must know that b + s does not overflow and a <= b + s and then we
	   can compute number of iterations as (b + s - a) / s.  (It might
	   seem that we in fact could be more clever about testing the b + s
	   overflow condition using some information about b - a mod s,
	   but it was already taken into account during LE -> NE transform).  */
	{
	  if (mmax)
	    {
	      bound = EXEC_BINARY (MINUS_EXPR, type, mmax, step0);
	      assumption = fold (build (LE_EXPR, boolean_type_node,
					base1, bound));
	      assumptions = fold (build (TRUTH_AND_EXPR, boolean_type_node,
					 assumptions, assumption));
	    }
	  step = step0;
	  tmp = fold (build (PLUS_EXPR, type, base1, step0));
	  assumption = fold (build (GT_EXPR, boolean_type_node, base0, tmp));
	  delta = fold (build (PLUS_EXPR, type, base1, step));
	  delta = fold (build (MINUS_EXPR, type, delta, base0));
	}
      else
	{
	  /* Condition in shape a <= b - s * i
	     We must know that a - s does not overflow and a - s <= b and then
	     we can again compute number of iterations as (b - (a - s)) / s.  */
	  if (mmin)
	    {
	      bound = EXEC_BINARY (MINUS_EXPR, type, mmin, step1);
	      assumption = fold (build (LE_EXPR, boolean_type_node,
					bound, base0));
	      assumptions = fold (build (TRUTH_AND_EXPR, boolean_type_node,
					 assumptions, assumption));
	    }
	  step = fold (build1 (NEGATE_EXPR, type, step1));
	  tmp = fold (build (PLUS_EXPR, type, base0, step1));
	  assumption = fold (build (GT_EXPR, boolean_type_node, tmp, base1));
	  delta = fold (build (MINUS_EXPR, type, base0, step));
	  delta = fold (build (MINUS_EXPR, type, base1, delta));
	}
      noloop_assumptions = fold (build (TRUTH_OR_EXPR, boolean_type_node,
					noloop_assumptions, assumption));
      delta = fold (build (FLOOR_DIV_EXPR, type, delta, step));
      niter->niter = delta;
    }

  niter->assumptions = assumptions;
  niter->may_be_zero = noloop_assumptions;
  return;

zero_iter:
  niter->assumptions = boolean_true_node;
  niter->may_be_zero = boolean_true_node;
  niter->niter = convert (type, integer_zero_node);
  return;
}

/* Determine the number of iterations of the current loop.  */

static void
determine_number_of_iterations (void)
{
  tree stmt, cond;

  if (!LOOP_DATA (current_loop)->single_exit)
    return;

  stmt = last_stmt (LOOP_DATA (current_loop)->single_exit->src);
  if (!stmt || TREE_CODE (stmt) != COND_EXPR)
    return;

  /* We want the condition for staying inside loop.  */
  cond = COND_EXPR_COND (stmt);
  if (LOOP_DATA (current_loop)->single_exit->flags & EDGE_TRUE_VALUE)
    cond = invert_truthvalue (cond);

  number_of_iterations_cond (cond, &LOOP_DATA (current_loop)->niter);
}

/* For each ssa name defined in LOOP determines whether it is an induction
   variable and if so, its initial value and step.  */

static bool
find_induction_variables (void)
{
  unsigned i;

  /* TODO (FIXME?) Use scev for this.  ??? Perhaps the code could still be
     useful at lower optimization levels, since it might be faster.  */

  if (!find_bivs ())
    return false;

  find_givs ();
  mark_bivs ();
  determine_number_of_iterations ();

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      if (LOOP_DATA (current_loop)->niter.niter)
	{
	  fprintf (tree_dump_file, "  number of iterations ");
	  print_generic_expr (tree_dump_file,
			      LOOP_DATA (current_loop)->niter.niter,
			      TDF_SLIM);
	  fprintf (tree_dump_file, "\n");

    	  fprintf (tree_dump_file, "  may be zero if ");
    	  print_generic_expr (tree_dump_file,
			      LOOP_DATA (current_loop)->niter.may_be_zero,
    			      TDF_SLIM);
    	  fprintf (tree_dump_file, "\n");

    	  fprintf (tree_dump_file, "  bogus unless ");
    	  print_generic_expr (tree_dump_file,
			      LOOP_DATA (current_loop)->niter.assumptions,
    			      TDF_SLIM);
    	  fprintf (tree_dump_file, "\n");
    	  fprintf (tree_dump_file, "\n");
    	};
 
      fprintf (tree_dump_file, "Induction variables:\n\n");

      EXECUTE_IF_SET_IN_BITMAP (relevant, 0, i,
	{
	  if (ver_info (i)->iv)
	    dump_iv (tree_dump_file, ver_info (i)->iv);
	});
    }

  return true;
}

/* Records a use of type USE_TYPE at *USE_P in STMT whose value is IV.  */

static void
record_use (tree *use_p, struct iv *iv, tree stmt, enum use_type use_type)
{
  struct iv_use *use = xcalloc (1, sizeof (struct iv_use));

  use->id = VARRAY_ACTIVE_SIZE (iv_uses);
  use->type = use_type;
  use->iv = iv;
  use->stmt = stmt;
  use->op_p = use_p;
  use->related_cands = BITMAP_XMALLOC ();

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    dump_use (tree_dump_file, use);

  VARRAY_PUSH_GENERIC_PTR_NOGC (iv_uses, use);
}

/* Checks whether OP is a loop-level invariant and if so, records it.
   NONLINEAR_USE is true if the invariant is used in a way we do not
   handle specially.  */

static void
record_invariant (tree op, bool nonlinear_use)
{
  basic_block bb;
  struct version_info *info;

  if (TREE_CODE (op) != SSA_NAME)
    return;

  bb = bb_for_stmt (SSA_NAME_DEF_STMT (op));
  if (bb
      && flow_bb_inside_loop_p (current_loop, bb))
    return;

  info = name_info (op);
  info->name = op;
  info->has_nonlin_use |= nonlinear_use;
  if (!info->inv_id)
    info->inv_id = ++max_inv_id;
  bitmap_set_bit (relevant, SSA_NAME_VERSION (op));
}

/* Checks whether the use *OP_P is interesting and if so, records it.  */

static void
find_interesting_uses_op (tree *op_p)
{
  struct iv *iv;
  struct iv *civ;

  if (TREE_CODE (*op_p) != SSA_NAME)
    return;

  iv = get_iv (*op_p);
  if (!iv || iv->have_use_for)
    return;
  if (zero_p (iv->step))
    {
      record_invariant (*op_p, true);
      return;
    }
  iv->have_use_for = true;

  civ = xmalloc (sizeof (struct iv));
  *civ = *iv;

  record_use (op_p, civ, SSA_NAME_DEF_STMT (*op_p), USE_NONLINEAR_EXPR);
}

/* Checks whether the condition *COND_P in STMT is interesting
   and if so, records it.  */

static void
find_interesting_uses_cond (tree stmt, tree *cond_p)
{
  tree *op0_p;
  tree *op1_p;
  struct iv *iv0 = NULL, *iv1 = NULL, *civ;
  struct iv const_iv;
  tree zero = integer_zero_node;

  const_iv.step = NULL_TREE;

  if (TREE_CODE (*cond_p) == SSA_NAME)
    {
      op0_p = cond_p;
      op1_p = &zero;
    }
  else
    {
      op0_p = &TREE_OPERAND (*cond_p, 0);
      op1_p = &TREE_OPERAND (*cond_p, 1);
    }

  if (TREE_CODE (*op0_p) == SSA_NAME)
    iv0 = get_iv (*op0_p);
  else
    iv0 = &const_iv;

  if (TREE_CODE (*op1_p) == SSA_NAME)
    iv1 = get_iv (*op1_p);
  else
    iv1 = &const_iv;

  if (/* When comparing with non-invariant value, we may not do any senseful
	 induction variable elimination.  */
      (!iv0 || !iv1)
      /* Eliminating condition based on two ivs would be nontrivial.
	 ??? TODO -- it is not really important to handle this case.  */
      || (!zero_p (iv0->step) && !zero_p (iv1->step)))
    {
      find_interesting_uses_op (op0_p);
      find_interesting_uses_op (op1_p);
      return;
    }

  if (zero_p (iv0->step) && zero_p (iv1->step))
    {
      /* If both are invariants, this is a work for unswitching.  */
      return;
    }

  civ = xmalloc (sizeof (struct iv));
  *civ = zero_p (iv0->step) ? *iv1: *iv0;
  record_use (cond_p, civ, stmt, USE_COMPARE);
}

/* Cumulates the steps of indices into DATA and replaces their values with the
   initial ones.  Returns false when the value of the index cannot be determined.
   Callback for for_each_index.  */

static bool
idx_find_step (tree base, tree *idx, void *data)
{
  tree *step_p = data;
  struct iv *iv;
  tree step, type;
  
  if (TREE_CODE (*idx) != SSA_NAME)
    return true;

  iv = get_iv (*idx);
  if (!iv)
    return false;

  *idx = iv->base;

  if (!iv->step)
    return true;

  if (base)
    {
      step = TYPE_SIZE_UNIT (TREE_TYPE (TREE_TYPE (base)));
      type = array2ptr (TREE_TYPE (base));
    }
  else
    {
      /* The step for pointer arithmetics already is 1 byte.  */
      step = integer_one_node;
      type = TREE_TYPE (*idx);
    }

  step = EXEC_BINARY (MULT_EXPR, type, step, iv->step);

  if (!*step_p)
    *step_p = step;
  else
    *step_p = EXEC_BINARY (PLUS_EXPR, type, *step_p, step);

  return true;
}

/* Records use in index IDX.  Callback for for_each_index.  */

static bool
idx_record_use (tree base ATTRIBUTE_UNUSED, tree *idx,
		void *data ATTRIBUTE_UNUSED)
{
  find_interesting_uses_op (idx);
  return true;
}

/* Finds addresses in *OP_P inside STMT.  */

static void
find_interesting_uses_address (tree stmt, tree *op_p)
{
  tree base = unshare_expr (*op_p), step = NULL;
  struct iv *civ;

  /* Ignore bitfields for now.  Not really something terribly complicated
     to handle.  TODO.  */
  if (TREE_CODE (base) == COMPONENT_REF
      && DECL_NONADDRESSABLE_P (TREE_OPERAND (base, 1)))
    goto fail;

  if (!for_each_index (&base, idx_find_step, &step)
      || zero_p (step))
    goto fail;

  if (TREE_CODE (base) == INDIRECT_REF)
    base = TREE_OPERAND (base, 0);
  else
    base = build1 (ADDR_EXPR,
		   build_pointer_type (TREE_TYPE (base)),
		   base);

  civ = alloc_iv (base, step);
  record_use (op_p, civ, stmt, USE_ADDRESS);
  return;

fail:
  for_each_index (op_p, idx_record_use, NULL);
}

/* Finds and records invariants used in STMT.  */

static void
find_invariants_stmt (tree stmt)
{
  use_optype uses = NULL;
  unsigned i, n;
  tree op;

  if (TREE_CODE (stmt) == PHI_NODE)
    n = PHI_NUM_ARGS (stmt);
  else
    {
      get_stmt_operands (stmt);
      uses = STMT_USE_OPS (stmt);
      n = NUM_USES (uses);
    }

  for (i = 0; i < n; i++)
    {
      if (TREE_CODE (stmt) == PHI_NODE)
	op = PHI_ARG_DEF (stmt, i);
      else
	op = USE_OP (uses, i);

      record_invariant (op, false);
    }
}

/* Finds interesting uses of induction variables in the statement STMT.  */

static void
find_interesting_uses_stmt (tree stmt)
{
  struct iv *iv;
  tree *op_p, lhs, rhs;
  use_optype uses = NULL;
  unsigned i, n;
  struct loop *loop;

  find_invariants_stmt (stmt);

  if (TREE_CODE (stmt) == COND_EXPR)
    {
      find_interesting_uses_cond (stmt, &COND_EXPR_COND (stmt));
      return;
    }

  if (TREE_CODE (stmt) == MODIFY_EXPR)
    {
      lhs = TREE_OPERAND (stmt, 0);
      rhs = TREE_OPERAND (stmt, 1);

      if (TREE_CODE (lhs) == SSA_NAME)
	{
	  /* If the statement defines an induction variable, uses of the induction
	     variables of the loop are not interesting.  */

	  iv = get_iv (lhs);

	  if (iv)
	    {
	      /* If the variable is used outside of the loop, we must preserve it.

		 TODO -- add posibility to replace it by the known final value,
		 or to express the final value using the other ivs.  */
	      loop = name_info (lhs)->outermost_usage;
	      if (loop
		  && loop != current_loop
		  && !flow_loop_nested_p (current_loop, loop))
		find_interesting_uses_op (&lhs);

	      return;
	    }
	}

      switch (TREE_CODE_CLASS (TREE_CODE (rhs)))
	{
	case '<':
	  find_interesting_uses_cond (stmt, &TREE_OPERAND (stmt, 1));
	  return;

	case 'r':
	  find_interesting_uses_address (stmt, &TREE_OPERAND (stmt, 1));
	  return;

	default: ;
	}

      if (TREE_CODE_CLASS (TREE_CODE (lhs)) == 'r')
	{
	  find_interesting_uses_address (stmt, &TREE_OPERAND (stmt, 0));
	  find_interesting_uses_op (&TREE_OPERAND (stmt, 1));
	  return;
	}
    }

  if (TREE_CODE (stmt) == PHI_NODE
      && bb_for_stmt (stmt) == current_loop->header)
    {
      lhs = PHI_RESULT (stmt);
      iv = get_iv (lhs);

      if (iv)
	{
	  /* If the variable is used outside of the loop, we must preserve it.
	     FIXME -- we do not handle this case correctly, since we just try
	     to replace the rhs of the phi.  */
	      
	  loop = name_info (lhs)->outermost_usage;
	  if (loop
	      && loop != current_loop
	      && !flow_loop_nested_p (current_loop, loop))
	    find_interesting_uses_op (&lhs);

	  return;
	}
    }

  if (TREE_CODE (stmt) == PHI_NODE)
    n = PHI_NUM_ARGS (stmt);
  else
    {
      uses = STMT_USE_OPS (stmt);
      n = NUM_USES (uses);
    }

  for (i = 0; i < n; i++)
    {
      if (TREE_CODE (stmt) == PHI_NODE)
	op_p = &PHI_ARG_DEF (stmt, i);
      else
	op_p = USE_OP_PTR (uses, i);

      if (TREE_CODE (*op_p) != SSA_NAME)
	continue;

      iv = get_iv (*op_p);
      if (!iv)
	continue;

      find_interesting_uses_op (op_p);
    }
}

/* Finds uses of the induction variables that are interesting.  */

static void
find_interesting_uses (void)
{
  basic_block bb;
  block_stmt_iterator bsi;
  tree phi;
  basic_block *body = get_loop_body (current_loop);
  unsigned i;
  struct version_info *info;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "Uses:\n\n");

  for (i = 0; i < current_loop->num_nodes; i++)
    {
      bb = body[i];

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	find_interesting_uses_stmt (phi);
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	find_interesting_uses_stmt (bsi_stmt (bsi));
    }
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "\n");

      EXECUTE_IF_SET_IN_BITMAP (relevant, 0, i,
	{
	  info = ver_info (i);
	  if (info->inv_id)
	    {
	      fprintf (tree_dump_file, "  ");
	      print_generic_expr (tree_dump_file, info->name, TDF_SLIM);
	      fprintf (tree_dump_file, " is invariant (%d)%s\n",
		       info->inv_id, info->has_nonlin_use ? "" : ", eliminable");
	    }
	});

      fprintf (tree_dump_file, "\n");
    }

  free (body);
}

/* Adds a candidate BASE + STEP * i.  Important field is set to IMPORTANT and
   position to POS.  If USE is not NULL, the candidate is set as related to
   it.  */

static void
add_candidate_1 (tree base, tree step, bool important, enum iv_position pos,
		 struct iv_use *use)
{
  unsigned i;
  struct iv_cand *cand = NULL;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_candidates); i++)
    {
      cand = VARRAY_GENERIC_PTR_NOGC (iv_candidates, i);

      if (cand->pos != pos)
	continue;

      if (!operand_equal_p (base, cand->iv->base, 0))
	continue;

      if (zero_p (cand->iv->step))
	{
	  if (zero_p (step))
	    break;
	}
      else
	{
	  if (step && operand_equal_p (step, cand->iv->step, 0))
	    break;
	}
    }

  if (i == VARRAY_ACTIVE_SIZE (iv_candidates))
    {
      cand = xcalloc (1, sizeof (struct iv_cand));
      cand->id = i;
      cand->iv = alloc_iv (base, step);
      cand->pos = pos;
      cand->var_before = create_tmp_var_raw (TREE_TYPE (base), "ivtmp");
      cand->var_after = cand->var_before;
      cand->important = important;
      VARRAY_PUSH_GENERIC_PTR_NOGC (iv_candidates, cand);

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	dump_cand (tree_dump_file, cand);
    }

  if (important && !cand->important)
    {
      cand->important = true;
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "Candidate %d is important\n", cand->id);
    }

  if (use)
    {
      bitmap_set_bit (use->related_cands, i);
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "Candidate %d is related to use %d\n",
		 cand->id, use->id);
    }
}

/* Adds a candidate BASE + STEP * i.  Important field is set to IMPORTANT and
   position to POS.  If USE is not NULL, the candidate is set as related to
   it.  The candidate computation is scheduled on all available positions.  */

static void
add_candidate (tree base, tree step, bool important, struct iv_use *use)
{
#if DISABLE_IP_START
  add_candidate_1 (base, step, important, IP_START, use);
#endif
  if (ip_normal_pos ())
    add_candidate_1 (base, step, important, IP_NORMAL, use);
  if (ip_end_pos ())
    add_candidate_1 (base, step, important, IP_END, use);
}

/* Adds standard iv candidates.  */

static void
add_standard_iv_candidates (void)
{
  /* Add 0 + 1 * iteration candidate.  */
  add_candidate (convert (integer_type_node, integer_zero_node),
      		 convert (integer_type_node, integer_one_node),
		 true, NULL);

  /* The same for a long type.  */
  add_candidate (convert (long_integer_type_node, integer_zero_node),
		 convert (long_integer_type_node, integer_one_node),
		 true, NULL);
}


/* Adds candidates bases on the old induction variable IV.  */

static void
add_old_iv_candidates (struct iv *iv)
{ 
  add_candidate (iv->base, iv->step, true, NULL);

  /* The same, but with initial value zero.  */
  add_candidate (convert (TREE_TYPE (iv->base), integer_zero_node),
		 iv->step, true, NULL);
}

/* Adds candidates based on the old induction variables.  */

static void
add_old_ivs_candidates (void)
{
  unsigned i;
  struct iv *iv;

  EXECUTE_IF_SET_IN_BITMAP (relevant, 0, i,
    {
      iv = ver_info (i)->iv;
      if (iv && iv->biv_p && !zero_p (iv->step))
	add_old_iv_candidates (iv);
    });
}

/* Adds candidates based on the value of the induction variable IV and USE.  */

static void
add_iv_value_candidates (struct iv *iv, struct iv_use *use)
{
  add_candidate (iv->base, iv->step, false, use);

  /* The same, but with initial value zero.  */
  add_candidate (convert (array2ptr (TREE_TYPE (iv->base)), integer_zero_node),
		 iv->step, false, use);
}

/* Adds candidates based on the address IV and USE.  */

static void
add_address_candidates (struct iv *iv, struct iv_use *use)
{
  tree base, type;

  /* First, the trivial choices.  */
  add_iv_value_candidates (iv, use);

  /* Second, try removing the COMPONENT_REFs.  */
  if (TREE_CODE (iv->base) == ADDR_EXPR)
    {
      base = TREE_OPERAND (iv->base, 0);
      type = TREE_TYPE (iv->base);
      while (TREE_CODE (base) == COMPONENT_REF
	     || (TREE_CODE (base) == ARRAY_REF
		 && TREE_CODE (TREE_OPERAND (base, 1)) == INTEGER_CST))
	base = TREE_OPERAND (base, 0);

      if (base != TREE_OPERAND (iv->base, 0))
	{ 
	  if (TREE_CODE (base) == INDIRECT_REF)
	    base = TREE_OPERAND (base, 0);
	  else
	    base = build1 (ADDR_EXPR, type, base);
	  add_candidate (base, iv->step, false, use);
	}
    }
}

/* Adds candidates based on the uses.  */

static void
add_derived_ivs_candidates (void)
{
  unsigned i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_uses); i++)
    {
      struct iv_use *use = VARRAY_GENERIC_PTR_NOGC (iv_uses, i);

      if (!use)
	continue;

      switch (use->type)
	{
	case USE_NONLINEAR_EXPR:
	case USE_COMPARE:
	  /* Just add the ivs based on the value of the iv used here.  */
	  add_iv_value_candidates (use->iv, use);
	  break;

	case USE_ADDRESS:
	  add_address_candidates (use->iv, use);
	  break;
	}
    }
}

/* Finds the candidates for the induction variables.  */

static void
find_iv_candidates (void)
{
  /* Add commonly used ivs.  */
  add_standard_iv_candidates ();

  /* Add old induction variables.  */
  add_old_ivs_candidates ();

  /* Add induction variables derived from uses.  */
  add_derived_ivs_candidates ();
}

/* Allocates the data structure mapping the (use, candidate) pairs to costs.
   If consider_all_candidates is true, we use a two-dimensional array, otherwise
   we allocate a simple list to every use.  */

static void
alloc_use_cost_map (void)
{
  unsigned i, n_imp = 0, size, j;

  if (!consider_all_candidates)
    {
      for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_candidates); i++)
	{
	  struct iv_cand *cand = VARRAY_GENERIC_PTR_NOGC (iv_candidates, i);
	  if (cand->important)
	    n_imp++;
	}
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_uses); i++)
    {
      struct iv_use *use = VARRAY_GENERIC_PTR_NOGC (iv_uses, i);

      if (consider_all_candidates)
	{
	  size = VARRAY_ACTIVE_SIZE (iv_candidates);
	  use->n_map_members = size;
	}
      else
	{
	  size = n_imp;
	  EXECUTE_IF_SET_IN_BITMAP (use->related_cands, 0, j, size++);
	  use->n_map_members = 0;
	}

      use->cost_map = xcalloc (size, sizeof (struct cost_pair));
    }
}

/* Sets cost of (USE, CANDIDATE) pair to COST and record that it depends
   on invariants DEPENDS_ON.  */

static void
set_use_iv_cost (struct iv_use *use, struct iv_cand *cand, unsigned cost,
		 bitmap depends_on)
{
  if (cost == INFTY
      && depends_on)
    {
      BITMAP_XFREE (depends_on);
      depends_on = NULL;
    }

  if (consider_all_candidates)
    {
      use->cost_map[cand->id].cand = cand;
      use->cost_map[cand->id].cost = cost;
      use->cost_map[cand->id].depends_on = depends_on;
      return;
    }

  if (cost == INFTY)
    return;

  use->cost_map[use->n_map_members].cand = cand;
  use->cost_map[use->n_map_members].cost = cost;
  use->cost_map[use->n_map_members].depends_on = depends_on;
  use->n_map_members++;
}

/* Gets cost of (USE, CANDIDATE) pair.  Stores the bitmap of dependencies to
   DEPENDS_ON.  */

static unsigned
get_use_iv_cost (struct iv_use *use, struct iv_cand *cand, bitmap *depends_on)
{
  unsigned i;

  if (!cand)
    return INFTY;

  if (consider_all_candidates)
    i = cand->id;
  else
    {
      for (i = 0; i < use->n_map_members; i++)
	if (use->cost_map[i].cand == cand)
	  break;

      if (i == use->n_map_members)
	return INFTY;
    }

  if (depends_on)
    *depends_on = use->cost_map[i].depends_on;
  return use->cost_map[i].cost;
}

/* Returns estimate on cost of computing SEQ.  */

static unsigned
seq_cost (rtx seq)
{
  unsigned cost = 0;
  rtx set;

  for (; seq; seq = NEXT_INSN (seq))
    {
      set = single_set (seq);
      if (set)
	cost += rtx_cost (set, SET);
      else
	cost++;
    }

  return cost;
}

/* Prepares decl_rtl for variables referred in *EXPR_P.  Callback for
   walk_tree.  DATA contains the actual fake register number.  */

static tree
prepare_decl_rtl (tree *expr_p, int *ws, void *data)
{
  tree obj = NULL_TREE;
  rtx x = NULL_RTX;
  int *regno = data;

  switch (TREE_CODE (*expr_p))
    {
    case SSA_NAME:
      *ws = 0;
      obj = SSA_NAME_VAR (*expr_p);
      if (!DECL_RTL_SET_P (obj))
	x = gen_raw_REG (DECL_MODE (obj), (*regno)++);
      break;

    case VAR_DECL:
    case PARM_DECL:
    case RESULT_DECL:
      *ws = 0;
      obj = *expr_p;

      if (DECL_RTL_SET_P (obj))
	break;

      if (DECL_MODE (obj) == BLKmode)
	{
	  if (TREE_STATIC (obj)
	      || DECL_EXTERNAL (obj))
	    {
	      const char *name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (obj));
	      x = gen_rtx_SYMBOL_REF (Pmode, name);
	    }
	  else
	    x = gen_raw_REG (Pmode, (*regno)++);

	  x = gen_rtx_MEM (DECL_MODE (obj), x);
	}
      else
	x = gen_raw_REG (DECL_MODE (obj), (*regno)++);

      break;

    default:
      break;
    }

  if (x)
    {
      VARRAY_PUSH_GENERIC_PTR_NOGC (decl_rtl_to_reset, obj);
      SET_DECL_RTL (obj, x);
    }

  return NULL_TREE;
}

/* Determines cost of the computation of EXPR.  */

static unsigned
computation_cost (tree expr)
{
  rtx seq, rslt;
  tree type = TREE_TYPE (expr);
  unsigned cost;
  int regno = 0;

  walk_tree (&expr, prepare_decl_rtl, &regno, NULL);
  start_sequence ();
  rslt = expand_expr (expr, NULL_RTX, TYPE_MODE (type), EXPAND_NORMAL);
  seq = get_insns ();
  end_sequence ();

  cost = seq_cost (seq);
  if (GET_CODE (rslt) == MEM)
    cost += address_cost (XEXP (rslt, 0), TYPE_MODE (type));

  return cost;
}

/* Returns variable containing the value of candidate CAND at position
   of USE.  */

static tree
var_at_use (struct iv_use *use, struct iv_cand *cand)
{
  switch (cand->pos)
    {
#if DISABLE_IP_START
    case IP_START:
      return cand->var_after;
#endif

    case IP_NORMAL:
      if (stmt_after_ip_normal_pos (use->stmt))
	return cand->var_after;
      else
	return cand->var_before;

    case IP_END:
      return cand->var_before;

    default:
      abort ();
    }
}

/* Determines the expression by that USE is expressed from induction variable
   CAND.  */

static tree
get_computation (struct iv_use *use, struct iv_cand *cand)
{
  tree ubase = use->iv->base, ustep = use->iv->step;
  tree cbase = cand->iv->base, cstep = cand->iv->step;
  tree utype = TREE_TYPE (ubase), ctype = TREE_TYPE (cbase);
  tree expr, delta;
  tree ratio;
  unsigned HOST_WIDE_INT ustepi, cstepi;
  HOST_WIDE_INT ratioi;

  expr = var_at_use (use, cand);

  if (TYPE_PRECISION (utype) > TYPE_PRECISION (ctype))
    {
      /* We do not have a precision to express the values of use.  */
      return NULL_TREE;
    }

  if (utype != ctype)
    {
      expr = convert (utype, expr);
      cbase = convert (utype, cbase);
      cstep = convert (utype, cstep);
    }

  if (!cst_and_fits_in_hwi (cstep)
      || !cst_and_fits_in_hwi (ustep))
    return NULL_TREE;

  ustepi = int_cst_value (ustep);
  cstepi = int_cst_value (cstep);

  if (!divide (TYPE_PRECISION (utype), ustepi, cstepi, &ratioi))
    {
      /* TODO maybe consider case when ustep divides cstep and the ratio is
	 a power of 2 (so that the division is fast to execute)?  We would
	 need to be much more careful with overflows etc. then.  */
      return NULL_TREE;
    }

  /* If we are after the "normal" position, the value of the candidate is
     higher by one iteration.  */
  if (cand->pos == IP_NORMAL
      && stmt_after_ip_normal_pos (use->stmt))
    cbase = fold (build (PLUS_EXPR, utype, cbase, cstep));

  /* use = ubase + ratio * (var - cbase).  If either cbase is a constant
     or |ratio| == 1, it is better to handle this like
     
     ubase - ratio * cbase + ratio * var.  */

  if (ratioi == 1)
    {
      delta = fold (build (MINUS_EXPR, utype, ubase, cbase));
      expr = fold (build (PLUS_EXPR, utype, expr, delta));
    }
  else if (ratioi == -1)
    {
      delta = fold (build (PLUS_EXPR, utype, ubase, cbase));
      expr = fold (build (MINUS_EXPR, utype, delta, expr));
    }
  else if (TREE_CODE (cbase) == INTEGER_CST)
    {
      ratio = build_int_cst (utype, ratioi);
      delta = fold (build (MULT_EXPR, utype, ratio, cbase));
      delta = fold (build (MINUS_EXPR, utype, ubase, delta));
      expr = fold (build (MULT_EXPR, utype, ratio, expr));
      expr = fold (build (PLUS_EXPR, utype, delta, expr));
    }
  else
    {
      expr = fold (build (MINUS_EXPR, utype, expr, cbase));
      ratio = build_int_cst (utype, ratioi);
      expr = fold (build (MULT_EXPR, utype, ratio, expr));
      expr = fold (build (PLUS_EXPR, utype, ubase, expr));
    }

  return expr;
}

/* Strips constant offsets from EXPR and adds them to OFFSET.  */

static void
strip_offset (tree *expr, unsigned HOST_WIDE_INT *offset)
{
  tree op0, op1;
  enum tree_code code;
  
  while (1)
    {
      if (cst_and_fits_in_hwi (*expr))
	{
	  *offset += int_cst_value (*expr);
	  *expr = integer_zero_node;
	  return;
	}

      code = TREE_CODE (*expr);
     
      if (code != PLUS_EXPR && code != MINUS_EXPR)
	return;

      op0 = TREE_OPERAND (*expr, 0);
      op1 = TREE_OPERAND (*expr, 1);

      if (cst_and_fits_in_hwi (op1))
	{
	  if (code == PLUS_EXPR)
	    *offset += int_cst_value (op1);
	  else
	    *offset -= int_cst_value (op1);

	  *expr = op0;
	  continue;
	}

      if (code != PLUS_EXPR)
	return;

      if (!cst_and_fits_in_hwi (op0))
	return;

      *offset += int_cst_value (op0);
      *expr = op1;
    }
}

/* Returns cost of addition in MODE.  */

static unsigned
add_cost (enum machine_mode mode)
{
  static unsigned costs[NUM_MACHINE_MODES];
  rtx seq;
  unsigned cost;

  if (costs[mode])
    return costs[mode];

  start_sequence ();
  force_operand (gen_rtx_fmt_ee (PLUS, mode,
				 gen_raw_REG (mode, FIRST_PSEUDO_REGISTER),
				 gen_raw_REG (mode, FIRST_PSEUDO_REGISTER + 1)),
		 NULL_RTX);
  seq = get_insns ();
  end_sequence ();

  cost = seq_cost (seq);
  if (!cost)
    cost = 1;

  costs[mode] = cost;
      
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "Addition in %s costs %d\n",
	     GET_MODE_NAME (mode), cost);
  return cost;
}

/* Entry in a hashtable of already known costs for multiplication.  */
struct mbc_entry
{
  HOST_WIDE_INT cst;		/* The constant to multiply by.  */
  enum machine_mode mode;	/* In mode.  */
  unsigned cost;		/* The cost.  */
};

/* Counts hash value for the ENTRY.  */

static hashval_t
mbc_entry_hash (const void *entry)
{
  const struct mbc_entry *e = entry;

  return 57 * (hashval_t) e->mode + (hashval_t) (e->cst % 877);
}

/* Compares the hash table entries ENTRY1 and ENTRY2.  */

static int
mbc_entry_eq (const void *entry1, const void *entry2)
{
  const struct mbc_entry *e1 = entry1;
  const struct mbc_entry *e2 = entry2;

  return (e1->mode == e2->mode
	  && e1->cst == e2->cst);
}

/* Returns cost of multiplication by constant CST in MODE.  */

static unsigned
multiply_by_cost (HOST_WIDE_INT cst, enum machine_mode mode)
{
  static htab_t costs;
  struct mbc_entry **cached, act;
  rtx seq;
  unsigned cost;

  if (!costs)
    costs = htab_create (100, mbc_entry_hash, mbc_entry_eq, free);

  act.mode = mode;
  act.cst = cst;
  cached = (struct mbc_entry **) htab_find_slot (costs, &act, INSERT);
  if (*cached)
    return (*cached)->cost;

  *cached = xmalloc (sizeof (struct mbc_entry));
  (*cached)->mode = mode;
  (*cached)->cst = cst;

  start_sequence ();
  expand_mult (mode, gen_raw_REG (mode, FIRST_PSEUDO_REGISTER), GEN_INT (cst),
	       NULL_RTX, 0);
  seq = get_insns ();
  end_sequence ();
  
  cost = seq_cost (seq);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "Multiplication by %d in %s costs %d\n",
	     (int) cst, GET_MODE_NAME (mode), cost);

  (*cached)->cost = cost;

  return cost;
}

/* Returns cost of address in shape symbol + var + OFFSET + RATIO * index.
   If SYMBOL_PRESENT is false, symbol is omitted.  If VAR_PRESENT is false,
   variable is omitted.  The created memory accesses MODE.
   
   TODO -- there must be some better way.  This all is quite crude.  */

static unsigned
get_address_cost (bool symbol_present, bool var_present,
		  unsigned HOST_WIDE_INT offset, HOST_WIDE_INT ratio)
{
#define MAX_RATIO 128
  static sbitmap valid_mult;
  static HOST_WIDE_INT rat, off;
  static HOST_WIDE_INT min_offset, max_offset;
  static unsigned costs[2][2][2][2];
  unsigned cost, acost;
  rtx seq, addr, base;
  bool offset_p, ratio_p;
  rtx reg1;
  HOST_WIDE_INT s_offset;
  unsigned HOST_WIDE_INT mask;
  unsigned bits;

  if (!valid_mult)
    {
      HOST_WIDE_INT i;

      reg1 = gen_raw_REG (Pmode, FIRST_PSEUDO_REGISTER);

      addr = gen_rtx_fmt_ee (PLUS, Pmode, reg1, NULL_RTX);
      for (i = 1; i <= 1 << 20; i <<= 1)
	{
	  XEXP (addr, 1) = GEN_INT (i);
	  if (!memory_address_p (Pmode, addr))
	    break;
	}
      max_offset = i >> 1;
      off = max_offset;

      for (i = 1; i <= 1 << 20; i <<= 1)
	{
	  XEXP (addr, 1) = GEN_INT (-i);
	  if (!memory_address_p (Pmode, addr))
	    break;
	}
      min_offset = -(i >> 1);

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "get_address_cost:\n");
	  fprintf (tree_dump_file, "  min offset %d\n", (int) min_offset);
	  fprintf (tree_dump_file, "  max offset %d\n", (int) max_offset);
	}

      valid_mult = sbitmap_alloc (2 * MAX_RATIO + 1);
      sbitmap_zero (valid_mult);
      rat = 1;
      addr = gen_rtx_fmt_ee (MULT, Pmode, reg1, NULL_RTX);
      for (i = -MAX_RATIO; i <= MAX_RATIO; i++)
	{
	  XEXP (addr, 1) = GEN_INT (i);
	  if (memory_address_p (Pmode, addr))
	    {
	      SET_BIT (valid_mult, i + MAX_RATIO);
	      rat = i;
	    }
	}

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "  allowed multipliers:");
	  for (i = -MAX_RATIO; i <= MAX_RATIO; i++)
	    if (TEST_BIT (valid_mult, i + MAX_RATIO))
	      fprintf (tree_dump_file, " %d", (int) i);
	  fprintf (tree_dump_file, "\n");
	  fprintf (tree_dump_file, "\n");
	}
    }

  bits = GET_MODE_BITSIZE (Pmode);
  mask = ~(~(unsigned HOST_WIDE_INT) 0 << (bits - 1) << 1);
  offset &= mask;
  if ((offset >> (bits - 1) & 1))
    offset |= ~mask;
  s_offset = offset;

  cost = 0;
  offset_p = (min_offset <= s_offset && s_offset <= max_offset);
  ratio_p = (ratio != 1
	     && -MAX_RATIO <= ratio && ratio <= MAX_RATIO
	     && TEST_BIT (valid_mult, ratio + MAX_RATIO));

  if (ratio != 1 && !ratio_p)
    cost += multiply_by_cost (ratio, Pmode);

  if (s_offset && !offset_p && !symbol_present)
    {
      cost += add_cost (Pmode);
      var_present = true;
    }

  acost = costs[symbol_present][var_present][offset_p][ratio_p];
  if (!acost)
    {
      acost = 0;
      
      addr = gen_raw_REG (Pmode, FIRST_PSEUDO_REGISTER);
      reg1 = gen_raw_REG (Pmode, FIRST_PSEUDO_REGISTER + 1);
      if (ratio_p)
	addr = gen_rtx_fmt_ee (MULT, Pmode, addr, GEN_INT (rat));

      if (symbol_present)
	{
	  base = gen_rtx_SYMBOL_REF (Pmode, ggc_strdup (""));
	  if (offset_p)
	    base = gen_rtx_fmt_e (CONST, Pmode,
				  gen_rtx_fmt_ee (PLUS, Pmode,
						  base,
						  GEN_INT (off)));
	  if (var_present)
	    base = gen_rtx_fmt_ee (PLUS, Pmode, reg1, base);
	}

      else if (var_present)
	{
	  base = reg1;
	  if (offset_p)
	    base = gen_rtx_fmt_ee (PLUS, Pmode, base, GEN_INT (off));
	}
      else if (offset_p)
	base = GEN_INT (off);
      else
	base = NULL_RTX;
    
      if (base)
	addr = gen_rtx_fmt_ee (PLUS, Pmode, base, addr);
  
      start_sequence ();
      addr = memory_address (Pmode, addr);
      seq = get_insns ();
      end_sequence ();
  
      acost = seq_cost (seq);
      acost += address_cost (addr, Pmode);

      if (!acost)
	acost = 1;
      costs[symbol_present][var_present][offset_p][ratio_p] = acost;
    }

  return cost + acost;
}

/* Records invariants in *EXPR_P.  Callback for walk_tree.  DATA contains
   the bitmap to that we should store it.  */

static tree
find_depends (tree *expr_p, int *ws ATTRIBUTE_UNUSED, void *data)
{
  bitmap *depends_on = data;
  struct version_info *info;

  if (TREE_CODE (*expr_p) != SSA_NAME)
    return NULL_TREE;
  info = name_info (*expr_p);

  if (!info->inv_id || info->has_nonlin_use)
    return NULL_TREE;

  if (!*depends_on)
    *depends_on = BITMAP_XMALLOC ();
  bitmap_set_bit (*depends_on, info->inv_id);

  return NULL_TREE;
}

/* Estimates cost of forcing EXPR into variable.  DEPENDS_ON is a set of the
   invariants the computation depends on.*/

static unsigned
force_var_cost (tree expr, bitmap *depends_on)
{
  if (depends_on)
    walk_tree (&expr, find_depends, depends_on, NULL);

  if (is_gimple_min_invariant (expr)
      || SSA_VAR_P (expr))
    return 0;

  return spill_cost;
}

/* Peels a single layer of ADDR.  If DIFF is not NULL, do it only if the
   offset is constant and add the offset to DIFF.  */

static tree
peel_address (tree addr, unsigned HOST_WIDE_INT *diff)
{
  tree off, size;
  HOST_WIDE_INT bit_offset;

  switch (TREE_CODE (addr))
    {
    case SSA_NAME:
    case INDIRECT_REF:
    case BIT_FIELD_REF:
    case VAR_DECL:
    case PARM_DECL:
    case RESULT_DECL:
    case STRING_CST:
      return NULL_TREE;

    case COMPONENT_REF:
      off = DECL_FIELD_BIT_OFFSET (TREE_OPERAND (addr, 1));
      bit_offset = TREE_INT_CST_LOW (off);

      if (bit_offset % BITS_PER_UNIT)
	abort ();
      
      if (diff)
	*diff += bit_offset / BITS_PER_UNIT;

      return TREE_OPERAND (addr, 0);

    case ARRAY_REF:
      off = TREE_OPERAND (addr, 1);

      if (diff)
	{
	  if (!cst_and_fits_in_hwi (off))
	    return NULL_TREE;

	  size = TYPE_SIZE_UNIT (TREE_TYPE (addr));
	  if (!cst_and_fits_in_hwi (size))
	    return NULL_TREE;

	  *diff += TREE_INT_CST_LOW (off) * TREE_INT_CST_LOW (size);
	}

      return TREE_OPERAND (addr, 0);

    default:
      abort ();
    }
}

/* Checks whether E1 and E2 have constant difference, and if they do,
   store it in *DIFF.  */

static bool
ptr_difference_const (tree e1, tree e2, unsigned HOST_WIDE_INT *diff)
{
  int d1 = 0, d2 = 0;
  tree x;
  unsigned HOST_WIDE_INT delta1 = 0, delta2 = 0;

  /* Find depths of E1 and E2.  */
  for (x = e1; x; x = peel_address (x, NULL))
    d1++;
  for (x = e2; x; x = peel_address (x, NULL))
    d2++;

  for (; e1 && d1 > d2; e1 = peel_address (e1, &delta1))
    d1--;
  for (; e2 && d2 > d1; e2 = peel_address (e2, &delta2))
    d2--;

  while (e1 && e2 && !operand_equal_p (e1, e2, 0))
    {
      e1 = peel_address (e1, &delta1);
      e2 = peel_address (e2, &delta1);
    }

  if (!e1 || !e2)
    return false;

  *diff = delta1 - delta2;
  return true;
}

/* Estimates cost of expressing address ADDR  as var + symbol + offset.  The
   value of offset is added to OFFSET, SYMBOL_PRESENT and VAR_PRESENT are set
   to false if the corresponding part is missing.  DEPENDS_ON is a set of the
   invariants the computation depends on.  */

static unsigned
split_address_cost (tree addr, bool *symbol_present, bool *var_present,
		    unsigned HOST_WIDE_INT *offset, bitmap *depends_on)
{
  tree core = addr;

  while (core
	 && TREE_CODE (core) != VAR_DECL)
    core = peel_address (core, offset);

  if (!core)
    {
      *symbol_present = false;
      *var_present = true;
      walk_tree (&addr, find_depends, depends_on, NULL);
      return spill_cost;
    }  
	  
  if (TREE_STATIC (core)
      || DECL_EXTERNAL (core))
    {
      *symbol_present = true;
      *var_present = false;
      return 0;
    }
      
  *symbol_present = false;
  *var_present = true;
  return 0;
}

/* Estimates cost of expressing difference of addresses E1 - E2 as
   var + symbol + offset.  The value of offset is added to OFFSET,
   SYMBOL_PRESENT and VAR_PRESENT are set to false if the corresponding
   part is missing.  DEPENDS_ON is a set of the invariants the computation
   depends on.  */

static unsigned
ptr_difference_cost (tree e1, tree e2, bool *symbol_present, bool *var_present,
		     unsigned HOST_WIDE_INT *offset, bitmap *depends_on)
{
  unsigned HOST_WIDE_INT diff = 0;
  unsigned cost;

  if (TREE_CODE (e1) != ADDR_EXPR)
    abort ();

  if (TREE_CODE (e2) == ADDR_EXPR
      && ptr_difference_const (TREE_OPERAND (e1, 0),
			       TREE_OPERAND (e2, 0), &diff))
    {
      *offset += diff;
      *symbol_present = false;
      *var_present = false;
      return 0;
    }

  if (e2 == integer_zero_node)
    return split_address_cost (TREE_OPERAND (e1, 0),
			       symbol_present, var_present, offset, depends_on);

  *symbol_present = false;
  *var_present = true;
  
  cost = force_var_cost (e1, depends_on);
  cost += force_var_cost (e2, depends_on);
  cost += add_cost (Pmode);

  return cost;
}

/* Estimates cost of expressing difference E1 - E2 as
   var + symbol + offset.  The value of offset is added to OFFSET,
   SYMBOL_PRESENT and VAR_PRESENT are set to false if the corresponding
   part is missing.  DEPENDS_ON is a set of the invariants the computation
   depends on.  */

static unsigned
difference_cost (tree e1, tree e2, bool *symbol_present, bool *var_present,
		 unsigned HOST_WIDE_INT *offset, bitmap *depends_on)
{
  unsigned cost;
  enum machine_mode mode = TYPE_MODE (TREE_TYPE (e1));

  strip_offset (&e1, offset);
  *offset = -*offset;
  strip_offset (&e2, offset);
  *offset = -*offset;

  if (TREE_CODE (e1) == ADDR_EXPR)
    return ptr_difference_cost (e1, e2, symbol_present, var_present, offset,
				depends_on);
  *symbol_present = false;

  if (operand_equal_p (e1, e2, 0))
    {
      *var_present = false;
      return 0;
    }
  *var_present = true;
  if (zero_p (e2))
    return force_var_cost (e1, depends_on);

  if (zero_p (e1))
    {
      cost = force_var_cost (e2, depends_on);
      cost += multiply_by_cost (-1, mode);

      return cost;
    }

  cost = force_var_cost (e1, depends_on);
  cost += force_var_cost (e2, depends_on);
  cost += add_cost (mode);

  return cost;
}

/* Determines the cost of the computation by that USE is expressed
   from induction variable CAND.  If ADDRESS_P is true, we just need
   to create an address from it, otherwise we want to get it into
   register.  A set of invariants we depend on is stored in
   DEPENDS_ON.  */

static unsigned
get_computation_cost (struct iv_use *use, struct iv_cand *cand,
		      bool address_p, bitmap *depends_on)
{
  tree ubase = use->iv->base, ustep = use->iv->step;
  tree cbase = cand->iv->base, cstep = cand->iv->step;
  tree utype = TREE_TYPE (ubase), ctype = TREE_TYPE (cbase);
  unsigned HOST_WIDE_INT ustepi, cstepi, offset = 0;
  HOST_WIDE_INT ratio, aratio;
  bool var_present, symbol_present;
  unsigned cost = 0, n_sums;

  *depends_on = NULL;

  if (TYPE_PRECISION (utype) > TYPE_PRECISION (ctype))
    {
      /* We do not have a precision to express the values of use.  */
      return INFTY;
    }

  if (!cst_and_fits_in_hwi (ustep)
      || !cst_and_fits_in_hwi (cstep))
    return INFTY;

  if (TREE_CODE (ubase) == INTEGER_CST
      && !cst_and_fits_in_hwi (ubase))
    goto fallback;

  if (TREE_CODE (cbase) == INTEGER_CST
      && !cst_and_fits_in_hwi (cbase))
    goto fallback;
    
  ustepi = int_cst_value (ustep);
  cstepi = int_cst_value (cstep);

  if (TYPE_PRECISION (utype) != TYPE_PRECISION (ctype))
    {
      /* TODO -- add direct handling of this case.  */
      goto fallback;
    }

  if (!divide (TYPE_PRECISION (utype), ustepi, cstepi, &ratio))
    return INFTY;

  /* use = ubase + ratio * (var - cbase).  If either cbase is a constant
     or ratio == 1, it is better to handle this like
     
     ubase - ratio * cbase + ratio * var
     
     (also holds in the case ratio == -1, TODO.  */

  if (TREE_CODE (cbase) == INTEGER_CST)
    {
      offset = - ratio * int_cst_value (cbase); 
      cost += difference_cost (ubase, integer_zero_node,
			       &symbol_present, &var_present, &offset,
			       depends_on);
    }
  else if (ratio == 1)
    {
      cost += difference_cost (ubase, cbase,
			       &symbol_present, &var_present, &offset,
			       depends_on);
    }
  else
    {
      cost += force_var_cost (cbase, depends_on);
      cost += add_cost (TYPE_MODE (ctype));
      cost += difference_cost (ubase, integer_zero_node,
			       &symbol_present, &var_present, &offset,
			       depends_on);
    }

  /* If we are after the "normal" position, the value of the candidate is
     higher by one iteration.  */
  if (cand->pos == IP_NORMAL
      && stmt_after_ip_normal_pos (use->stmt))
    offset -= ratio * cstepi;

  /* Now the computation is in shape symbol + var1 + const + ratio * var2.
     (symbol/var/const parts may be omitted).  If we are looking for an address,
     find the cost of addressing this.  */
  if (address_p)
    return get_address_cost (symbol_present, var_present, offset, ratio);

  /* Otherwise estimate the costs for computing the expression.  */
  aratio = ratio > 0 ? ratio : -ratio;
  if (!symbol_present && !var_present && !offset)
    {
      if (ratio != 1)
	cost += multiply_by_cost (ratio, TYPE_MODE (ctype));

      return cost;
    }

  if (aratio != 1)
    cost += multiply_by_cost (aratio, TYPE_MODE (ctype));

  n_sums = 1;
  if (var_present
      /* Symbol + offset should be compile-time computable.  */
      && (symbol_present || offset))
    n_sums++;

  return cost + n_sums * add_cost (TYPE_MODE (ctype));

fallback:
  {
    /* Just get the expression, expand it and measure the cost.  */
    tree comp = get_computation (use, cand);

    if (!comp)
      return INFTY;

    if (address_p)
      comp = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (comp)), comp);

    return computation_cost (comp);
  }
}

/* Determines cost of basing replacement of USE on CAND in a generic
   expression.  */

static void
determine_use_iv_cost_generic (struct iv_use *use, struct iv_cand *cand)
{
  bitmap depends_on;
  unsigned cost = get_computation_cost (use, cand, false, &depends_on);

  set_use_iv_cost (use, cand, cost, depends_on);
}

/* Determines cost of basing replacement of USE on CAND in an address.  */

static void
determine_use_iv_cost_address (struct iv_use *use, struct iv_cand *cand)
{
  bitmap depends_on;
  unsigned cost = get_computation_cost (use, cand, true, &depends_on);

  set_use_iv_cost (use, cand, cost, depends_on);
}

/* Computes value of candidate CAND at position USE in iteration NITER.  */

static tree
cand_value_at (struct iv_cand *cand, struct iv_use *use, tree niter)
{
  tree val;
  tree type = TREE_TYPE (niter);

  if (cand->pos == IP_NORMAL
      && stmt_after_ip_normal_pos (use->stmt))
    niter = fold (build (PLUS_EXPR, type, niter, integer_one_node));

  val = fold (build (MULT_EXPR, type, cand->iv->step, niter));

  return fold (build (PLUS_EXPR, type, cand->iv->base, val));
}

/* Check whether it is possible to express the condition in USE by comparison
   of candidate CAND.  If so, store the comparison code to COMPARE and the
   value compared with to BOUND.  */

static bool
may_eliminate_iv (struct iv_use *use, struct iv_cand *cand,
		  enum tree_code *compare, tree *bound)
{
  edge exit;
  struct tree_niter_desc *niter;

  /* For now just very primitive -- we work just for the single exit condition,
     and are quite conservative about the possible overflows.  TODO -- both of
     these can be improved.  */
  exit = LOOP_DATA (current_loop)->single_exit;
  if (!exit)
    return false;
  if (use->stmt != last_stmt (exit->src))
    return false;

  niter = &LOOP_DATA (current_loop)->niter;
  if (!niter->niter
      || !operand_equal_p (niter->assumptions, boolean_true_node, 0)
      || !operand_equal_p (niter->may_be_zero, boolean_false_node, 0))
    return false;

  if (exit->flags & EDGE_TRUE_VALUE)
    *compare = EQ_EXPR;
  else
    *compare = NE_EXPR;

  *bound = cand_value_at (cand, use, niter->niter);

  return true;
}

/* Determines cost of basing replacement of USE on CAND in a condition.  */

static void
determine_use_iv_cost_condition (struct iv_use *use, struct iv_cand *cand)
{
  tree bound;
  enum tree_code compare;

  if (may_eliminate_iv (use, cand, &compare, &bound))
    {
      bitmap depends_on = BITMAP_XMALLOC ();
      unsigned cost = force_var_cost (bound, &depends_on);

      set_use_iv_cost (use, cand, cost, depends_on);
      return;
    }

  /* The induction variable elimination failed; just express the original
     giv.  If it is compared with an invariant, note that we cannot get
     rid of it.  */
  if (TREE_CODE (*use->op_p) == SSA_NAME)
    record_invariant (*use->op_p, true);
  else
    {
      record_invariant (TREE_OPERAND (*use->op_p, 0), true);
      record_invariant (TREE_OPERAND (*use->op_p, 1), true);
    }

  determine_use_iv_cost_generic (use, cand);
}

/* Determines cost of basing replacement of USE on CAND.  */

static void
determine_use_iv_cost (struct iv_use *use, struct iv_cand *cand)
{
  switch (use->type)
    {
    case USE_NONLINEAR_EXPR:
      determine_use_iv_cost_generic (use, cand);
      break;

    case USE_ADDRESS:
      determine_use_iv_cost_address (use, cand);
      break;

    case USE_COMPARE:
      determine_use_iv_cost_condition (use, cand);
      break;
    }
}

/* Determines costs of basing the use of the iv on an iv candidate.  */

static void
determine_use_iv_costs (void)
{
  unsigned i, j;
  struct iv_use *use;
  struct iv_cand *cand;

  consider_all_candidates = (VARRAY_ACTIVE_SIZE (iv_candidates)
			     <= CONSIDER_ALL_CANDIDATES_BOUND);

  alloc_use_cost_map ();

  if (!consider_all_candidates)
    {
      /* Add the important candidate entries.  */
      for (j = 0; j < VARRAY_ACTIVE_SIZE (iv_candidates); j++)
	{
	  cand = VARRAY_GENERIC_PTR_NOGC (iv_candidates, j);
	  if (!cand->important)
	    continue;
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_uses); i++)
	    {
	      use = VARRAY_GENERIC_PTR_NOGC (iv_uses, i);
	      determine_use_iv_cost (use, cand);
	    }
	}
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_uses); i++)
    {
      use = VARRAY_GENERIC_PTR_NOGC (iv_uses, i);

      if (consider_all_candidates)
	{
	  for (j = 0; j < VARRAY_ACTIVE_SIZE (iv_candidates); j++)
	    {
	      cand = VARRAY_GENERIC_PTR_NOGC (iv_candidates, j);
	      determine_use_iv_cost (use, cand);
	    }
	}
      else
	{
	  EXECUTE_IF_SET_IN_BITMAP (use->related_cands, 0, j,
	    {
	      cand = VARRAY_GENERIC_PTR_NOGC (iv_candidates, j);
	      if (!cand->important)
	        determine_use_iv_cost (use, cand);
	    });
	}
    }

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "Use-candidate costs:\n");

      for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_uses); i++)
	{
	  use = VARRAY_GENERIC_PTR_NOGC (iv_uses, i);

	  fprintf (tree_dump_file, "Use %d:\n", i);
	  fprintf (tree_dump_file, "  cand\tcost\tdepends on\n");
	  for (j = 0; j < use->n_map_members; j++)
	    {
	      if (!use->cost_map[j].cand
		  || use->cost_map[j].cost == INFTY)
		continue;

	      fprintf (tree_dump_file, "  %d\t%d\t",
		       use->cost_map[j].cand->id,
		       use->cost_map[j].cost);
	      if (use->cost_map[j].depends_on)
		bitmap_print (tree_dump_file,
			      use->cost_map[j].depends_on, "","");
	      fprintf (tree_dump_file, "\n");
	    }

	  fprintf (tree_dump_file, "\n");
	}
      fprintf (tree_dump_file, "\n");
    }
}

/* Determines cost of the candidate CAND.  */

static void
determine_iv_cost (struct iv_cand *cand)
{
  unsigned cost_base, cost_step;
  tree base = cand->iv->base;

  /* There are two costs associated with the candidate -- its incrementation
     and its initialization.  The second is almost negligible for any loop
     that rolls enough, so we take it just very little into account.  */

#if DISABLE_IP_START
  tree type = TREE_TYPE (base);

  if (cand->pos == IP_START)
    {
      /* We must decrease the base, because it will get increased just at the
	 start of the loop body.  */
      base = fold (build (MINUS_EXPR, type, base, cand->iv->step));
    }
#endif

  cost_base = force_var_cost (base, NULL);
  cost_step = add_cost (TYPE_MODE (TREE_TYPE (base)));

  /* TODO use profile to determine the ratio here.  */
  cand->cost = cost_step + cost_base / 5;
}

/* Determines costs of computation of the candidates.  */

static void
determine_iv_costs (void)
{
  unsigned i;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "Candidate costs:\n");
      fprintf (tree_dump_file, "  cand\tcost\n");
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_candidates); i++)
    {
      struct iv_cand *cand = VARRAY_GENERIC_PTR_NOGC (iv_candidates, i);

      determine_iv_cost (cand);

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "  %d\t%d\n", i, cand->cost);
    }
  
if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
      fprintf (tree_dump_file, "\n");
}

/* Calculates cost for having SIZE new loop global variables.  REGS_USED is the
   number of global registers used in loop.  N_USES is the number of relevant
   variable uses.  */

unsigned
global_cost_for_size (unsigned size, unsigned regs_used, unsigned n_uses)
{
  unsigned regs_needed = regs_used + size;
  unsigned cost = 0;

  if (regs_needed + res_regs <= avail_regs)
    cost += small_cost * size;
  else if (regs_needed <= avail_regs)
    cost += pres_cost * size;
  else
    {
      cost += pres_cost * size;
      cost += spill_cost * n_uses * (regs_needed - avail_regs) / regs_needed;
    }

  return cost;
}

/* Calculates cost for having SIZE induction variables.  */

static unsigned
ivopts_global_cost_for_size (unsigned size)
{
  return global_cost_for_size (size, LOOP_DATA (current_loop)->regs_used,
			       VARRAY_ACTIVE_SIZE (iv_uses));
}

/* Initialize the constants for computing set costs.  */

void
init_set_costs (void)
{
  rtx seq;
  rtx reg1 = gen_raw_REG (SImode, FIRST_PSEUDO_REGISTER);
  rtx reg2 = gen_raw_REG (SImode, FIRST_PSEUDO_REGISTER + 1);
  rtx addr = gen_raw_REG (Pmode, FIRST_PSEUDO_REGISTER + 2);
  rtx mem = validize_mem (gen_rtx_MEM (SImode, addr));
  unsigned i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (reg_class_contents[GENERAL_REGS], i)
	&& !fixed_regs[i])
      avail_regs++;

  res_regs = 3;

  /* These are really just heuristic values.  */
  
  start_sequence ();
  emit_move_insn (reg1, reg2);
  seq = get_insns ();
  end_sequence ();
  small_cost = seq_cost (seq);
  pres_cost = 2 * small_cost;

  start_sequence ();
  emit_move_insn (mem, reg1);
  emit_move_insn (reg2, mem);
  seq = get_insns ();
  end_sequence ();
  spill_cost = seq_cost (seq);
}

/* For each size of the induction variable set determine the penalty.  */

static void
determine_set_costs (void)
{
  unsigned j, n;
  tree phi, op;

  /* We use the following model (definitely improvable, especially the
     cost function -- TODO):

     We estimate the number of registers available (using MD data), name it A.

     We estimate the number of registers used by the loop, name it U.  This
     number is obtained as the number of loop phi nodes (not counting virtual
     registers and bivs) + the number of variables from outside of the loop.

     We set a reserve R (free regs that are used for temporary computations,
     etc.).  For now the reserve a constant 3.

     Let I be the number of induction variables.
     
     -- if U + I + R <= A, the cost is I * SMALL_COST (just not to encourage
	make a lot of ivs without a reason).
     -- if A - R < U + I <= A, the cost is I * PRES_COST
     -- if U + I > A, the cost is I * PRES_COST and
        number of uses * SPILL_COST * (U + I - A) / (U + I) is added.  */

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "Global costs:\n");
      fprintf (tree_dump_file, "  avail_regs %d\n", avail_regs);
      fprintf (tree_dump_file, "  small_cost %d\n", small_cost);
      fprintf (tree_dump_file, "  pres_cost %d\n", pres_cost);
      fprintf (tree_dump_file, "  spill_cost %d\n", spill_cost);
    }

  n = 0;
  for (phi = phi_nodes (current_loop->header); phi; phi = TREE_CHAIN (phi))
    {
      op = PHI_RESULT (phi);

      if (!is_gimple_reg (op))
	continue;

      if (get_iv (op))
	continue;

      n++;
    }

  EXECUTE_IF_SET_IN_BITMAP (relevant, 0, j,
    {
      struct version_info *info = ver_info (j);

      if (info->inv_id && info->has_nonlin_use)
	n++;
    });

  LOOP_DATA (current_loop)->regs_used = n;
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "  regs_used %d\n", n);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "  cost for size:\n");
      fprintf (tree_dump_file, "  ivs\tcost\n");
      for (j = 0; j <= 2 * avail_regs; j++)
	fprintf (tree_dump_file, "  %d\t%d\n", j,
		 ivopts_global_cost_for_size (j));
      fprintf (tree_dump_file, "\n");
    }
}

/* Finds a best candidate for USE and stores it to CAND.  The candidates are
   taken from the set SOL and they may depend on invariants in the set INV.
   The really used candidate and invariants are noted to USED_IVS and
   USED_INV.  */

static unsigned
find_best_candidate (struct iv_use *use, bitmap sol, bitmap inv,
		     bitmap used_ivs, bitmap used_inv, struct iv_cand **cand)
{
  unsigned c, d;
  unsigned best_cost = INFTY, cost;
  struct iv_cand *cnd = NULL, *acnd;
  bitmap depends_on = NULL;

  EXECUTE_IF_SET_IN_BITMAP (sol, 0, c,
    {
      acnd = VARRAY_GENERIC_PTR_NOGC (iv_candidates, c);
      cost = get_use_iv_cost (use, acnd, &depends_on);

      if (cost >= best_cost)
	goto next_cand;

      if (depends_on)
	{
	  EXECUTE_IF_AND_COMPL_IN_BITMAP (depends_on, inv, 0, d,
					  goto next_cand);
	  if (used_inv)
	    bitmap_a_or_b (used_inv, used_inv, depends_on);
	}

      cnd = acnd;
      best_cost = cost;
next_cand: ;
    });

  if (cnd && used_ivs)
    bitmap_set_bit (used_ivs, cnd->id);

  if (cand)
    *cand = cnd;

  return best_cost;
}

/* Computes cost of set of ivs SOL + invariants INV.  Removes unnecessary
   induction variable candidates and invariants from the sets.  */

static unsigned
set_cost (bitmap sol, bitmap inv)
{
  unsigned i;
  unsigned cost = 0, size = 0, acost;
  struct iv_use *use;
  struct iv_cand *cand;
  bitmap used_ivs = BITMAP_XMALLOC (), used_inv = BITMAP_XMALLOC ();

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_uses); i++)
    {
      use = VARRAY_GENERIC_PTR_NOGC (iv_uses, i);
      acost = find_best_candidate (use, sol, inv, used_ivs, used_inv, NULL);
      if (acost == INFTY)
	{
	  BITMAP_XFREE (used_ivs);
	  BITMAP_XFREE (used_inv);
	  return INFTY;
	}
      cost += acost;
    }

  EXECUTE_IF_SET_IN_BITMAP (used_ivs, 0, i,
    {
      size++;
      cand = VARRAY_GENERIC_PTR_NOGC (iv_candidates, i);
      cost += cand->cost;
    });
  EXECUTE_IF_SET_IN_BITMAP (used_inv, 0, i, size++);
  cost += ivopts_global_cost_for_size (size);

  bitmap_copy (sol, used_ivs);
  bitmap_copy (inv, used_inv);

  BITMAP_XFREE (used_ivs);
  BITMAP_XFREE (used_inv);

  return cost;
}

/* Finds an initial set of IVS and invariants INV.  We do this by simply
   chosing the best candidate for each use.  */

static unsigned
get_initial_solution (bitmap ivs, bitmap inv)
{
  unsigned i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_candidates); i++)
    bitmap_set_bit (ivs, i);
  for (i = 1; i <= max_inv_id; i++)
    if (!ver_info (i)->has_nonlin_use)
      bitmap_set_bit (inv, i);

  return set_cost (ivs, inv);
}

/* Tries to improve set of induction variables IVS and invariants INV to get
   it better than COST.  */

static bool
try_improve_iv_set (bitmap ivs, bitmap inv, unsigned *cost)
{
  unsigned i, acost;
  bitmap new_ivs = BITMAP_XMALLOC (), new_inv = BITMAP_XMALLOC ();
  bitmap best_new_ivs = NULL, best_new_inv = NULL;

  /* Try altering the set of induction variables by one.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_candidates); i++)
    {
      bitmap_copy (new_ivs, ivs);
      bitmap_copy (new_inv, inv);

      if (bitmap_bit_p (ivs, i))
	bitmap_clear_bit (new_ivs, i);
      else
	bitmap_set_bit (new_ivs, i);

      acost = set_cost (new_ivs, new_inv);
      if (acost >= *cost)
	continue;

      if (!best_new_ivs)
	{
	  best_new_ivs = BITMAP_XMALLOC ();
	  best_new_inv = BITMAP_XMALLOC ();
	}

      *cost = acost;
      bitmap_copy (best_new_ivs, new_ivs);
      bitmap_copy (best_new_inv, new_inv);
    }

  /* Ditto for invariants.  */
  for (i = 1; i <= max_inv_id; i++)
    {
      if (ver_info (i)->has_nonlin_use)
	continue;

      bitmap_copy (new_ivs, ivs);
      bitmap_copy (new_inv, inv);

      if (bitmap_bit_p (inv, i))
	bitmap_clear_bit (new_inv, i);
      else
	bitmap_set_bit (new_inv, i);

      acost = set_cost (new_ivs, new_inv);
      if (acost >= *cost)
	continue;

      if (!best_new_ivs)
	{
	  best_new_ivs = BITMAP_XMALLOC ();
	  best_new_inv = BITMAP_XMALLOC ();
	}

      *cost = acost;
      bitmap_copy (best_new_ivs, new_ivs);
      bitmap_copy (best_new_inv, new_inv);
    }

  BITMAP_XFREE (new_ivs);
  BITMAP_XFREE (new_inv);

  if (!best_new_ivs)
    return false;

  bitmap_copy (ivs, best_new_ivs);
  bitmap_copy (inv, best_new_inv);
  BITMAP_XFREE (best_new_ivs);
  BITMAP_XFREE (best_new_inv);
  return true;
}

/* Attempts to find the optimal set of induction variables.  TODO document
   the algorithm, once it converges to the final form.  For now we do simple
   greedy heuristic (trying to replace at most one candidate in the selected
   solution).  */

static bitmap
find_optimal_iv_set (void)
{
  unsigned cost, i;
  bitmap set = BITMAP_XMALLOC ();
  bitmap inv = BITMAP_XMALLOC ();
  struct iv_use *use;

  /* Set the upper bound.  */
  cost = get_initial_solution (set, inv);
  if (cost == INFTY)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "Unable to substitute for ivs, failed.\n");
      BITMAP_XFREE (inv);
      BITMAP_XFREE (set);
      return NULL;
    }

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "Initial set of candidates (cost %d): ", cost);
      bitmap_print (tree_dump_file, set, "", "");
      fprintf (tree_dump_file, " invariants ");
      bitmap_print (tree_dump_file, inv, "", "");
      fprintf (tree_dump_file, "\n");
    }

  while (try_improve_iv_set (set, inv, &cost))
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "Improved to (cost %d): ", cost);
	  bitmap_print (tree_dump_file, set, "", "");
	  fprintf (tree_dump_file, " invariants ");
	  bitmap_print (tree_dump_file, inv, "", "");
	  fprintf (tree_dump_file, "\n");
	}
    }

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "Final cost %d\n\n", cost);

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_uses); i++)
    {
      use = VARRAY_GENERIC_PTR_NOGC (iv_uses, i);
      find_best_candidate (use, set, inv, NULL, NULL, &use->selected);
    }

  BITMAP_XFREE (inv);

  return set;
}

/* Creates an induction variable with value BASE + STEP * iteration in LOOP.
   It is expected that neither BASE nor STEP are shared with other expressions
   (unless the sharing rules allow this).  Use VAR as a base var_decl for it
   (if NULL, a new temporary will be created).  The increment will occur at
   INCR_POS (after it if AFTER is true, before it otherwise).  The ssa versions
   of the variable before and after increment will be stored in VAR_BEFORE and
   VAR_AFTER (unless they are NULL).  */

void
create_iv (tree base, tree step, tree var, struct loop *loop,
	   block_stmt_iterator *incr_pos, bool after,
	   tree *var_before, tree *var_after)
{
  tree stmt, stmts, initial;
  tree vb, va;

  if (!var)
    {
      var = create_tmp_var (TREE_TYPE (base), "ivtmp");
      add_referenced_tmp_var (var);
    }

  vb = make_ssa_name (var, NULL_TREE);
  if (var_before)
    *var_before = vb;
  va = make_ssa_name (var, NULL_TREE);
  if (var_after)
    *var_after = va;

  stmt = build (MODIFY_EXPR, void_type_node, va,
		build (PLUS_EXPR, TREE_TYPE (base),
		       vb, step));
  SSA_NAME_DEF_STMT (va) = stmt;
  if (after)
    bsi_insert_after (incr_pos, stmt, BSI_NEW_STMT);
  else
    bsi_insert_before (incr_pos, stmt, BSI_NEW_STMT);

  initial = force_gimple_operand (base, &stmts, var, false);
  if (stmts)
    {
      basic_block new_bb;
      edge pe = loop_preheader_edge (loop);
      
      new_bb = bsi_insert_on_edge_immediate (pe, stmts);
      if (new_bb)
	add_bb_to_loop (new_bb, new_bb->pred->src->loop_father);
    }

  stmt = create_phi_node (vb, loop->header);
  SSA_NAME_DEF_STMT (vb) = stmt;
  add_phi_arg (&stmt, initial, loop_preheader_edge (loop));
  add_phi_arg (&stmt, va, loop_latch_edge (loop));
}

/* Creates a new induction variable corresponding to CAND.  */

static void
create_new_iv (struct iv_cand *cand)
{
  block_stmt_iterator incr_pos;
  tree base;
  bool after = false;

  switch (cand->pos)
    {
#if DISABLE_IP_START
    case IP_START:
      incr_pos = bsi_start (current_loop->header);
      while (!bsi_end_p (incr_pos)
	     && TREE_CODE (bsi_stmt (incr_pos)) == LABEL_EXPR)
	bsi_next (&incr_pos);
      if (bsi_end_p (incr_pos))
	{
	  incr_pos = bsi_last (current_loop->header);
	  after = true;
	}
      break;
#endif

    case IP_NORMAL:
      incr_pos = bsi_last (ip_normal_pos ());
      break;

    case IP_END:
      incr_pos = bsi_last (ip_end_pos ());
      after = true;
      break;
    }
 
  gimple_add_tmp_var (cand->var_before);
  add_referenced_tmp_var (cand->var_before);

  base = unshare_expr (cand->iv->base);
#if DISABLE_IP_START
  if (cand->pos == IP_START)
    {
      /* We must decrease the base, because it will get increased just at the
	 start of the loop body.  */
      base = fold (build (MINUS_EXPR, TREE_TYPE (base), base, cand->iv->step));
    }
#endif

  create_iv (base, cand->iv->step, cand->var_before, current_loop,
	     &incr_pos, after, &cand->var_before, &cand->var_after);
}

/* Creates new induction variables described in SET.  */

static void
create_new_ivs (bitmap set)
{
  unsigned i;
  struct iv_cand *cand;

  EXECUTE_IF_SET_IN_BITMAP (set, 0, i,
    {
      cand = VARRAY_GENERIC_PTR_NOGC (iv_candidates, i);
      create_new_iv (cand);
    });
}

/* Rewrites USE (definition of iv used in a nonlinear expression)
   using candidate CAND.  */

static void
rewrite_use_nonlinear_expr (struct iv_use *use, struct iv_cand *cand)
{
  tree comp = unshare_expr (get_computation (use, cand));
  tree op, stmts;
  block_stmt_iterator bsi;
 
  if (TREE_CODE (use->stmt) == PHI_NODE)
    {
      /* TODO -- handle rewriting of the phi node.  This may only occur when
	 the result of the phi node is used outside of the loop.  */
      return;
    }

  bsi = stmt_bsi (use->stmt);
  op = force_gimple_operand (comp, &stmts,
			     SSA_NAME_VAR (TREE_OPERAND (use->stmt, 0)),
			     false);
  if (stmts)
    bsi_insert_before (&bsi, stmts, BSI_SAME_STMT);
  TREE_OPERAND (use->stmt, 1) = op;
}

/* Rewrites USE (address that is an iv) using candidate CAND.  */

static void
rewrite_use_address (struct iv_use *use, struct iv_cand *cand)
{
  tree comp = unshare_expr (get_computation (use, cand));
  block_stmt_iterator bsi = stmt_bsi (use->stmt);
  tree stmts;
  tree op = force_gimple_operand (comp, &stmts, NULL_TREE, false);
  tree var, tmp_var, name;

  if (stmts)
    bsi_insert_before (&bsi, stmts, BSI_SAME_STMT);

  if (TREE_CODE (op) == SSA_NAME)
    {
      /* We need to add a memory tag for the variable.  But we do not want
	 to add it to the temporary used for the computations, since this leads
	 to problems in redundancy elimination when there are common parts
	 in two computations refering to the different arrays.  So we rewrite
	 the base variable of the ssa name to a new temporary.  */
      tmp_var = create_tmp_var (TREE_TYPE (op), "ruatmp");
      add_referenced_tmp_var (tmp_var);
      SSA_NAME_VAR (op) = tmp_var;

      var = get_base_decl (*use->op_p);
      if (var_ann (var)->type_mem_tag)
	var = var_ann (var)->type_mem_tag;
      var_ann (tmp_var)->type_mem_tag = var;

      name = get_base_var (*use->op_p);
      if (name && TREE_CODE (name) == SSA_NAME)
	{
	  ssa_name_ann_t ann = ssa_name_ann (name), new_ann;

	  if (ann && ann->name_mem_tag)
	    {
	      new_ann = get_ssa_name_ann (op);
	      new_ann->name_mem_tag = ann->name_mem_tag;
	    }
	}
    }

  *use->op_p = build1 (INDIRECT_REF, TREE_TYPE (*use->op_p), op);
}

/* Rewrites USE (the condition such that one of the arguments is an iv) using
   candidate CAND.  */

static void
rewrite_use_compare (struct iv_use *use, struct iv_cand *cand)
{
  tree comp;
  tree *op_p, cond, op, stmts, bound;
  block_stmt_iterator bsi = stmt_bsi (use->stmt);
  enum tree_code compare;
  
  if (may_eliminate_iv (use, cand, &compare, &bound))
    {
      op = force_gimple_operand (unshare_expr (bound), &stmts,
				 NULL_TREE, false);

      if (stmts)
	bsi_insert_before (&bsi, stmts, BSI_SAME_STMT);

      *use->op_p = build (compare, boolean_type_node,
			  var_at_use (use, cand), op);
      modify_stmt (use->stmt);
      return;
    }

  /* The induction variable elimination failed; just express the original
     giv.  */
  comp = unshare_expr (get_computation (use, cand));

  cond = *use->op_p;
  op_p = &TREE_OPERAND (cond, 0);
  if (TREE_CODE (*op_p) != SSA_NAME
      || zero_p (get_iv (*op_p)->step))
    op_p = &TREE_OPERAND (cond, 1);

  op = force_gimple_operand (comp, &stmts, SSA_NAME_VAR (*op_p), false);
  if (stmts)
    bsi_insert_before (&bsi, stmts, BSI_SAME_STMT);

  *op_p = op;
}

/* Rewrites USE using candidate CAND.  */

static void
rewrite_use (struct iv_use *use, struct iv_cand *cand)
{
  switch (use->type)
    {
      case USE_NONLINEAR_EXPR:
	rewrite_use_nonlinear_expr (use, cand);
	break;

      case USE_ADDRESS:
	rewrite_use_address (use, cand);
	break;

      case USE_COMPARE:
	rewrite_use_compare (use, cand);
	break;
    }
  modify_stmt (use->stmt);
}

/* Rewrite the uses using the selected induction variables.  */

static void
rewrite_uses (void)
{
  unsigned i;
  struct iv_cand *cand;
  struct iv_use *use;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_uses); i++)
    {
      use = VARRAY_GENERIC_PTR_NOGC (iv_uses, i);
      cand = use->selected;
      if (!cand)
	abort ();

      rewrite_use (use, cand);
    }
}

/* Frees data allocated by the optimization of a single loop.  */

static void
free_loop_data (void)
{
  unsigned i, j;

  EXECUTE_IF_SET_IN_BITMAP (relevant, 0, i,
    {
      struct version_info *info;

      info = ver_info (i);
      if (info->iv)
	free (info->iv);
      info->iv = NULL;
      info->has_nonlin_use = false;
      info->inv_id = 0;
    });
  bitmap_clear (relevant);

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_uses); i++)
    {
      struct iv_use *use = VARRAY_GENERIC_PTR_NOGC (iv_uses, i);

      free (use->iv);
      BITMAP_XFREE (use->related_cands);
      for (j = 0; j < use->n_map_members; j++)
	if (use->cost_map[j].depends_on)
	  BITMAP_XFREE (use->cost_map[j].depends_on);
      free (use->cost_map);
      free (use);
    }
  VARRAY_POP_ALL (iv_uses);

  for (i = 0; i < VARRAY_ACTIVE_SIZE (iv_candidates); i++)
    {
      struct iv_cand *cand = VARRAY_GENERIC_PTR_NOGC (iv_candidates, i);

      free (cand->iv);
      free (cand);
    }
  VARRAY_POP_ALL (iv_candidates);

  if (version_info_size < highest_ssa_version)
    {
      version_info_size = 2 * highest_ssa_version;
      free (version_info);
      version_info = xcalloc (version_info_size, sizeof (struct version_info));
    }

  max_inv_id = 0;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (decl_rtl_to_reset); i++)
    {
      tree obj = VARRAY_GENERIC_PTR_NOGC (decl_rtl_to_reset, i);

      SET_DECL_RTL (obj, NULL_RTX);
    }
  VARRAY_POP_ALL (decl_rtl_to_reset);
}

/* Finalizes data structures used by the iv optimization pass.  LOOPS is the
   loop tree.  */

static void
tree_ssa_iv_optimize_finalize (struct loops *loops)
{
  unsigned i;

  for (i = 1; i < loops->num; i++)
    if (loops->parray[i])
      {
	free (loops->parray[i]->aux);
	loops->parray[i]->aux = NULL;
      }

  free_loop_data ();
  free (version_info);
  BITMAP_XFREE (relevant);

  VARRAY_FREE (decl_rtl_to_reset);
  VARRAY_FREE (iv_uses);
  VARRAY_FREE (iv_candidates);
}

/* Optimizes the LOOP.  Returns true if anything changed.  */

static bool
tree_ssa_iv_optimize_loop (struct loop *loop)
{
  bool changed = false;
  bitmap iv_set;

  current_loop = loop;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "Processing loop %d\n", loop->num);
      fprintf (tree_dump_file, "  %d exits\n", LOOP_DATA (loop)->n_exits);
      if (LOOP_DATA (loop)->single_exit)
	{
	  edge ex = LOOP_DATA (loop)->single_exit;

	  fprintf (tree_dump_file, "  single exit %d -> %d, exit condition ",
		   ex->src->index, ex->dest->index);
	  print_generic_expr (tree_dump_file, last_stmt (ex->src), TDF_SLIM);
	  fprintf (tree_dump_file, "\n");
	}
      fprintf (tree_dump_file, "\n");
    }

  /* For each ssa name determines whether it behaves as an induction variable
     in some loop.  */
  if (!find_induction_variables ())
    goto finish;

  /* Finds interesting uses (item 1).  */
  find_interesting_uses ();

  /* Finds candidates for the induction variables (item 2).  */
  find_iv_candidates ();

  /* Calculates the costs (item 3, part 1).  */
  determine_use_iv_costs ();
  determine_iv_costs ();
  determine_set_costs ();

  /* Find the optimal set of induction variables (item 3, part 2).  */
  iv_set = find_optimal_iv_set ();
  if (!iv_set)
    goto finish;
  changed = true;

  /* Create the new induction variables (item 4, part 1).  */
  create_new_ivs (iv_set);
  
  /* Rewrite the uses (item 4, part 2).  */
  rewrite_uses ();

  BITMAP_XFREE (iv_set);
finish:
  free_loop_data ();

  return changed;
}

/* Main entry point.  Optimizes induction variables in LOOPS.  */

void
tree_ssa_iv_optimize (struct loops *loops)
{
  bool run_dce = false;
  struct loop *loop;

  timevar_push (TV_TREE_LOOP_IVOPTS);
  tree_ssa_iv_optimize_init (loops);

  /* Optimize the loops starting with the innermost ones.  */
  loop = loops->tree_root;
  while (loop->inner)
    loop = loop->inner;

  /* Scan the loops, inner ones first.  */
  while (loop != loops->tree_root)
    {
      run_dce |= tree_ssa_iv_optimize_loop (loop);

      if (loop->next)
	{
	  loop = loop->next;
	  while (loop->inner)
	    loop = loop->inner;
	}
      else
	loop = loop->outer;
    }

  tree_ssa_iv_optimize_finalize (loops);

  /* Cleanup the now unused variables.  */
  if (run_dce)
    tree_ssa_dce_no_cfg_changes ();

  timevar_pop (TV_TREE_LOOP_IVOPTS);
}
