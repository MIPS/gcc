/* Data flow functions for trees.
   Copyright (C) 2001, 2002, 2003 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "hashtab.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "timevar.h"
#include "expr.h"
#include "ggc.h"
#include "langhooks.h"
#include "flags.h"
#include "function.h"
#include "diagnostic.h"
#include "tree-dump.h"
#include "tree-simple.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-alias-common.h"
#include "tree-pass.h"
#include "convert.h"
#include "params.h"

/* Build and maintain data flow information for trees.  */

/* Counters used to display DFA and SSA statistics.  */
struct dfa_stats_d
{
  long num_stmt_anns;
  long num_var_anns;
  long num_defs;
  long num_uses;
  long num_phis;
  long num_phi_args;
  int max_num_phi_args;
  long num_vdefs;
  long num_vuses;
};

struct alias_stats_d
{
  unsigned int alias_queries;
  unsigned int alias_mayalias;
  unsigned int alias_noalias;
  unsigned int simple_queries;
  unsigned int simple_resolved;
  unsigned int tbaa_queries;
  unsigned int tbaa_resolved;
  unsigned int pta_queries;
  unsigned int pta_resolved;
};

/* Tuple to map a variable to its alias set.  Used to cache the results of
   calls to get_alias_set().  */
struct GTY(()) alias_map_d
{
  tree var;
  HOST_WIDE_INT set;
};


/* State information for find_vars_r.  */
struct walk_state
{
  /* Nonzero if the variables found under the current tree are written to.  */
  int is_store : 1;

  /* Nonzero if the walker is inside an INDIRECT_REF node.  */
  int is_indirect_ref : 1;

  /* Nonzero if the walker is inside an ASM_EXPR node.  */
  int is_asm_expr : 1;

  /* Hash table used to avoid adding the same variable more than once.  */
  htab_t vars_found;

  /* Number of CALL_EXPRs found.  Used to determine whether to group all
     call-clobbered variables into .GLOBAL_VAR.  */
  int num_calls;
};


/* Data and functions shared with tree-ssa.c.  */
struct dfa_stats_d dfa_stats;

struct alias_stats_d alias_stats;

/* Local functions.  */
static void collect_dfa_stats (struct dfa_stats_d *);
static tree collect_dfa_stats_r (tree *, int *, void *);
static void compute_alias_sets (void);
static void dump_alias_stats (FILE *);
static void create_memory_tags (bitmap);
static bool may_alias_p (tree, HOST_WIDE_INT, tree, HOST_WIDE_INT);
static bool may_access_global_mem_p (tree);
static void add_immediate_use (tree, tree);
static tree find_vars_r (tree *, int *, void *);
static void add_referenced_var (tree, struct walk_state *);
static tree get_memory_tag_for (tree);
static void compute_immediate_uses_for_phi (tree, bool (*)(tree));
static void compute_immediate_uses_for_stmt (tree, int, bool (*)(tree));
static void add_may_alias (tree, tree);
static void find_hidden_use_vars (tree);
static tree find_hidden_use_vars_r (tree *, int *, void *);
static void promote_call_clobbered_vars (bitmap);
static void find_addressable_vars (bitmap);

/* Global declarations.  */

/* Array of all variables referenced in the function.  */
varray_type referenced_vars;

/* Arrays for all the call clobbered variables in the function.  */
varray_type call_clobbered_vars;

/* Artificial variable used to model the effects of function calls on every
   variable that they may use and define.  Calls to non-const and non-pure
   functions are assumed to use and clobber this variable.

   Loads and stores to call clobbered variables will be considered aliased
   with this variable.  */
tree global_var;

/* 'true' after aliases have been computed (see compute_may_aliases).  This
   is used by get_stmt_operands and its helpers to determine what to do
   when scanning an operand for a variable that may be aliased.  If
   may-alias information is still not available, the statement is marked as
   having volatile operands.  */
bool aliases_computed_p;

/*---------------------------------------------------------------------------
			Dataflow analysis (DFA) routines
---------------------------------------------------------------------------*/
/* Find all the variables referenced in the function.  This function
   builds the global arrays REFERENCED_VARS and CALL_CLOBBERED_VARS.

   Note that this function does not look for statement operands, it simply
   determines what variables are referenced in the program and detects
   various attributes for each variable used by alias analysis and the
   optimizer.  */

static void
find_referenced_vars (void)
{
  static htab_t vars_found;
  basic_block bb;
  block_stmt_iterator si;
  struct walk_state walk_state;
  tree block;

  init_tree_ssa ();

  /* Walk the lexical blocks in the function looking for variables that may
     have been used to declare VLAs and for nested functions.  Both
     constructs create hidden uses of variables. 

     Note that at this point we may have multiple blocks hung off
     DECL_INITIAL chained through the BLOCK_CHAIN field due to
     how inlining works.  Egad.  */
  block = DECL_INITIAL (current_function_decl);
  while (block)
    {
      find_hidden_use_vars (block);
      block = BLOCK_CHAIN (block);
    }

  vars_found = htab_create (50, htab_hash_pointer, htab_eq_pointer, NULL);
  memset (&walk_state, 0, sizeof (walk_state));
  walk_state.vars_found = vars_found;

  FOR_EACH_BB (bb)
    for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
      {
	tree *stmt_p = bsi_stmt_ptr (si);
	walk_tree (stmt_p, find_vars_r, &walk_state, NULL);
      }

  /* Determine whether to use .GLOBAL_VAR to model call clobber semantics.
     At every call site, we need to emit VDEF expressions.
     
     One approach is to group all call-clobbered variables into a single
     representative that is used as an alias of every call-clobbered
     variable (.GLOBAL_VAR).  This works well, but it ties the optimizer
     hands because references to any call clobbered variable is a reference
     to .GLOBAL_VAR.

     The second approach is to emit a clobbering VDEF for every
     call-clobbered variable at call sites.  This is the preferred way in
     terms of optimization opportunities but it may create too many
     VDEF operands if there are many call clobbered variables and function
     calls in the function.

     To decide whether or not to use .GLOBAL_VAR we multiply the number of
     function calls found by the number of call-clobbered variables.  If
     that product is beyond a certain threshold, as determined by the
     parameterized values shown below, we use .GLOBAL_VAR.

     FIXME: This heuristic should be improved.  One idea is to use several
     .GLOBAL_VARs of different types instead of a single one.  The
     thresholds have been derived from a typical bootstrap cycle including
     all target libraries.  Compile times were found to take 1% more
     compared to using .GLOBAL_VAR.  */
  {
    if (walk_state.num_calls * num_call_clobbered_vars 
	 < MAX_CALLS_FOR_GLOBAL_VAR * (size_t)MAX_CLOBBERED_VARS_FOR_GLOBAL_VAR)
      global_var = NULL_TREE;
    else if (global_var)
      add_referenced_var (global_var, &walk_state);
  }

  htab_delete (vars_found);
}

struct tree_opt_pass pass_referenced_vars =
{
  NULL,					/* name */
  NULL,					/* gate */
  find_referenced_vars,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_gimple_leh | PROP_cfg,		/* properties_required */
  PROP_referenced_vars,			/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0,					/* todo_flags_finish */
};


/* Compute immediate uses.  The parameter calc_for is an option function 
   pointer which indicates whether immediate uses information should be
   calculated for a given SSA variable. If NULL, then information is computed
   for all variables.  */

void
compute_immediate_uses (int flags, bool (*calc_for)(tree))
{
  basic_block bb;
  block_stmt_iterator si;

  FOR_EACH_BB (bb)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	compute_immediate_uses_for_phi (phi, calc_for);

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
        {
	  tree stmt = bsi_stmt (si);
	  get_stmt_operands (stmt);
	  compute_immediate_uses_for_stmt (stmt, flags, calc_for);
	}
    }
}

/* Invalidates dataflow information for a statement STMT.  */

static void
free_df_for_stmt (tree stmt)
{
  stmt_ann_t ann = stmt_ann (stmt);

  if (ann)
    ann->df = NULL;
}

/* Invalidates dataflow information.  */

void
free_df (void)
{
  basic_block bb;
  block_stmt_iterator si;

  FOR_EACH_BB (bb)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	free_df_for_stmt (phi);

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
        {
	  tree stmt = bsi_stmt (si);
	  free_df_for_stmt (stmt);
	}
    }
}

/* Helper for compute_immediate_uses.  Check all the USE and/or VUSE
   operands in phi node PHI and add a def-use edge between their
   defining statement and PHI.

   PHI nodes are easy, we only need to look at its arguments.  */

static void
compute_immediate_uses_for_phi (tree phi, bool (*calc_for)(tree))
{
  int i;

#ifdef ENABLE_CHECKING
  if (TREE_CODE (phi) != PHI_NODE)
    abort ();
#endif

  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      tree arg = PHI_ARG_DEF (phi, i);

      if (TREE_CODE (arg) == SSA_NAME && (!calc_for || calc_for (arg)))
	{ 
	  tree imm_rdef_stmt = SSA_NAME_DEF_STMT (PHI_ARG_DEF (phi, i));
	  if (!IS_EMPTY_STMT (imm_rdef_stmt))
	    add_immediate_use (imm_rdef_stmt, phi);
	}
    }
}


/* Another helper for compute_immediate_uses.  Check all the USE and/or VUSE
   operands in STMT and add a def-use edge between their defining statement
   and STMT.  */

static void
compute_immediate_uses_for_stmt (tree stmt, int flags, bool (*calc_for)(tree))
{
  size_t i;
  use_optype uses;
  vuse_optype vuses;
  vdef_optype vdefs;
  stmt_ann_t ann;

  /* PHI nodes are handled elsewhere.  */
#ifdef ENABLE_CHECKING
  if (TREE_CODE (stmt) == PHI_NODE)
    abort ();
#endif

  /* Look at USE_OPS or VUSE_OPS according to FLAGS.  */
  ann = stmt_ann (stmt);
  if (flags & TDFA_USE_OPS)
    {
      uses = USE_OPS (ann);
      for (i = 0; i < NUM_USES (uses); i++)
	{
	  tree use = USE_OP (uses, i);
	  tree imm_stmt = SSA_NAME_DEF_STMT (use);
	  if (!IS_EMPTY_STMT (imm_stmt) && (!calc_for || calc_for (use)))
	    add_immediate_use (imm_stmt, stmt);
	}
    }

  if (flags & TDFA_USE_VOPS)
    {
      vuses = VUSE_OPS (ann);
      for (i = 0; i < NUM_VUSES (vuses); i++)
	{
	  tree vuse = VUSE_OP (vuses, i);
	  tree imm_rdef_stmt = SSA_NAME_DEF_STMT (vuse);
	  if (!IS_EMPTY_STMT (imm_rdef_stmt) && (!calc_for || calc_for (vuse)))
	    add_immediate_use (imm_rdef_stmt, stmt);
	}

      vdefs = VDEF_OPS (ann);
      for (i = 0; i < NUM_VDEFS (vdefs); i++)
	{
	  tree vuse = VDEF_OP (vdefs, i);
	  tree imm_rdef_stmt = SSA_NAME_DEF_STMT (vuse);
	  if (!IS_EMPTY_STMT (imm_rdef_stmt) && (!calc_for || calc_for (vuse)))
	    add_immediate_use (imm_rdef_stmt, stmt);
	}
    }
}


/* Compute reached uses.  */

void
compute_reached_uses (int flags ATTRIBUTE_UNUSED)
{
  abort ();
}


/* Compute reaching definitions.  */

void
compute_reaching_defs (int flags ATTRIBUTE_UNUSED)
{
  abort ();
}



/* Add statement USE_STMT to the list of statements that use definitions
    made by STMT.  */

static void
add_immediate_use (tree stmt, tree use_stmt)
{
  stmt_ann_t ann = get_stmt_ann (stmt);
  struct dataflow_d *df;

  df = ann->df;
  if (df == NULL)
    {
      df = ann->df = ggc_alloc (sizeof (struct dataflow_d));
      memset ((void *) df, 0, sizeof (struct dataflow_d));
      df->uses[0] = use_stmt;
      return;
    }

  if (!df->uses[1])
    {
      df->uses[1] = use_stmt;
      return;
    }

  if (ann->df->immediate_uses == NULL)
    VARRAY_TREE_INIT (ann->df->immediate_uses, 4, "immediate_uses");

  VARRAY_PUSH_TREE (ann->df->immediate_uses, use_stmt);
}


/*---------------------------------------------------------------------------
			    Manage annotations
---------------------------------------------------------------------------*/
/* Create a new annotation for a _DECL node T.  */

var_ann_t
create_var_ann (tree t)
{
  var_ann_t ann;

#if defined ENABLE_CHECKING
  if (t == NULL_TREE
      || !DECL_P (t)
      || (t->common.ann
	  && t->common.ann->common.type != VAR_ANN))
    abort ();
#endif

  ann = ggc_alloc (sizeof (*ann));
  memset ((void *) ann, 0, sizeof (*ann));

  ann->common.type = VAR_ANN;

  t->common.ann = (tree_ann) ann;

  return ann;
}


/* Create a new annotation for a statement node T.  */

stmt_ann_t
create_stmt_ann (tree t)
{
  stmt_ann_t ann;

#if defined ENABLE_CHECKING
  if ((!is_gimple_stmt (t) && !is_essa_node (t))
      || (t->common.ann
	  && t->common.ann->common.type != STMT_ANN))
    abort ();
#endif

  ann = ggc_alloc (sizeof (*ann));
  memset ((void *) ann, 0, sizeof (*ann));

  ann->common.type = STMT_ANN;

  /* Since we just created the annotation, mark the statement modified.  */
  ann->modified = true;

  t->common.ann = (tree_ann) ann;

  return ann;
}

/*---------------------------------------------------------------------------
			      Debugging functions
---------------------------------------------------------------------------*/
/* Dump the list of all the referenced variables in the current function to
   FILE.  */

void
dump_referenced_vars (FILE *file)
{
  size_t i;

  fprintf (file, "\nReferenced variables in %s: %u\n\n", 
	   get_name (current_function_decl), (unsigned) num_referenced_vars);

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      fprintf (file, "Variable: ");
      dump_variable (file, var);
      fprintf (file, "\n");
    }
}


/* Dump the list of all the referenced variables to stderr.  */

void
debug_referenced_vars (void)
{
  dump_referenced_vars (stderr);
}


/* Dump variable VAR and its may-aliases to FILE.  */

void
dump_variable (FILE *file, tree var)
{
  var_ann_t ann;

  if (var == NULL_TREE)
    {
      fprintf (file, "<nil>");
      return;
    }

  print_generic_expr (file, var, 0);
  
  if (TREE_CODE (var) == SSA_NAME)
    var = SSA_NAME_VAR (var);

  ann = var_ann (var);

  fprintf (file, ", UID %u", (unsigned) ann->uid);

  if (ann->mem_tag)
    {
      fprintf (file, ", memory tag: ");
      print_generic_expr (file, ann->mem_tag, 0);
    }

  if (ann->is_alias_tag)
    fprintf (file, ", is an alias tag");

  if (ann->may_alias_global_mem)
    fprintf (file, ", may alias global memory");

  if (ann->has_hidden_use)
    fprintf (file, ", has a hidden use");

  if (ann->may_point_to_global_mem)
    fprintf (file, ", may point to global memory");

  if (ann->is_call_clobbered)
    fprintf (file, ", call clobbered");

  if (ann->is_stored)
    fprintf (file, ", is written to");

  if (ann->is_dereferenced_store)
    fprintf (file, ", is dereferenced to store");

  if (ann->is_dereferenced_load)
    fprintf (file, ", is dereferenced to load");

  if (ann->default_def)
    {
      fprintf (file, ", default def: ");
      print_generic_expr (file, ann->default_def, 0);
    }

  if (ann->may_aliases)
    {
      fprintf (file, ", may aliases: ");
      dump_may_aliases_for (file, var);
    }

  fprintf (file, "\n");
}


/* Dump variable VAR and its may-aliases to stderr.  */

void
debug_variable (tree var)
{
  dump_variable (stderr, var);
}


/* Dump to FILE the list of variables that may be aliasing VAR.  */

void
dump_may_aliases_for (FILE *file, tree var)
{
  varray_type aliases;
  
  if (TREE_CODE (var) == SSA_NAME)
    var = SSA_NAME_VAR (var);

  aliases = var_ann (var)->may_aliases;
  if (aliases)
    {
      size_t i, num_aliases = VARRAY_ACTIVE_SIZE (aliases);

      fprintf (file, "{ ");
      for (i = 0; i < num_aliases; i++)
	{
	  print_generic_expr (file, VARRAY_TREE (aliases, i), 0);
	  fprintf (file, " ");
	}
      fprintf (file, "}");
    }

  fprintf (file, "\n");
}


/* Dump to stderr the list of variables that may be aliasing VAR.  */

void
debug_may_aliases_for (tree var)
{
  dump_may_aliases_for (stderr, var);
}


/* Dump def-use edges on FILE.  */

void
dump_immediate_uses (FILE *file)
{
  basic_block bb;
  block_stmt_iterator si;
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);

  fprintf (file, "\nDef-use edges for function %s\n", funcname);

  FOR_EACH_BB (bb)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	dump_immediate_uses_for (file, phi);

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	dump_immediate_uses_for (file, bsi_stmt (si));
    }

  fprintf (file, "\n");
}


/* Dump def-use edges on stderr.  */

void
debug_immediate_uses (void)
{
  dump_immediate_uses (stderr);
}


/* Dump all immediate uses for STMT on FILE.  */

void
dump_immediate_uses_for (FILE *file, tree stmt)
{
  dataflow_t df = get_immediate_uses (stmt);
  int num_imm_uses = num_immediate_uses (df);

  if (num_imm_uses > 0)
    {
      int i;

      fprintf (file, "-> ");
      print_generic_stmt (file, stmt, TDF_SLIM);
      fprintf (file, "\n");

      for (i = 0; i < num_imm_uses; i++)
	{
	  fprintf (file, "\t");
	  print_generic_stmt (file, immediate_use (df, i), TDF_SLIM);
	  fprintf (file, "\n");
	}

      fprintf (file, "\n");
    }
}


/* Dump immediate uses for STMT on stderr.  */

void
debug_immediate_uses_for (tree stmt)
{
  dump_immediate_uses_for (stderr, stmt);
}


/* Dump various DFA statistics to FILE.  */

void
dump_dfa_stats (FILE *file)
{
  struct dfa_stats_d dfa_stats;

  unsigned long size, total = 0;
  const char * const fmt_str   = "%-30s%-13s%12s\n";
  const char * const fmt_str_1 = "%-30s%13lu%11lu%c\n";
  const char * const fmt_str_3 = "%-43s%11lu%c\n";
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);

  collect_dfa_stats (&dfa_stats);

  fprintf (file, "\nDFA Statistics for %s\n\n", funcname);

  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, fmt_str, "", "  Number of  ", "Memory");
  fprintf (file, fmt_str, "", "  instances  ", "used ");
  fprintf (file, "---------------------------------------------------------\n");

  size = num_referenced_vars * sizeof (tree);
  total += size;
  fprintf (file, fmt_str_1, "Referenced variables", num_referenced_vars, 
	   SCALE (size), LABEL (size));

  size = num_call_clobbered_vars * sizeof (tree);
  total += size;
  fprintf (file, fmt_str_1, "Call clobbered variables", num_call_clobbered_vars,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_stmt_anns * sizeof (struct stmt_ann_d);
  total += size;
  fprintf (file, fmt_str_1, "Statements annotated", dfa_stats.num_stmt_anns,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_var_anns * sizeof (struct var_ann_d);
  total += size;
  fprintf (file, fmt_str_1, "Variables annotated", dfa_stats.num_var_anns,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_uses * sizeof (tree *);
  total += size;
  fprintf (file, fmt_str_1, "USE operands", dfa_stats.num_uses,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_defs * sizeof (tree *);
  total += size;
  fprintf (file, fmt_str_1, "DEF operands", dfa_stats.num_defs,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_vuses * sizeof (tree *);
  total += size;
  fprintf (file, fmt_str_1, "VUSE operands", dfa_stats.num_vuses,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_vdefs * sizeof (tree *);
  total += size;
  fprintf (file, fmt_str_1, "VDEF operands", dfa_stats.num_vdefs,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_phis * sizeof (struct tree_phi_node);
  total += size;
  fprintf (file, fmt_str_1, "PHI nodes", dfa_stats.num_phis,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_phi_args * sizeof (struct phi_arg_d);
  total += size;
  fprintf (file, fmt_str_1, "PHI arguments", dfa_stats.num_phi_args,
 	   SCALE (size), LABEL (size));

  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, fmt_str_3, "Total memory used by DFA/SSA data", SCALE (total),
	   LABEL (total));
  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, "\n");

  if (dfa_stats.num_phis)
    fprintf (file, "Average number of arguments per PHI node: %.1f (max: %d)\n",
	     (float) dfa_stats.num_phi_args / (float) dfa_stats.num_phis,
	     dfa_stats.max_num_phi_args);

  fprintf (file, "\n");
}


/* Dump DFA statistics on stderr.  */

void
debug_dfa_stats (void)
{
  dump_dfa_stats (stderr);
}


/* Collect DFA statistics and store them in the structure pointed by
   DFA_STATS_P.  */

static void
collect_dfa_stats (struct dfa_stats_d *dfa_stats_p)
{
  htab_t htab;
  basic_block bb;
  block_stmt_iterator i;

  if (dfa_stats_p == NULL)
    abort ();

  memset ((void *)dfa_stats_p, 0, sizeof (struct dfa_stats_d));

  /* Walk all the trees in the function counting references.  Start at
     basic block 0, but don't stop at block boundaries.  */
  htab = htab_create (30, htab_hash_pointer, htab_eq_pointer, NULL);

  for (i = bsi_start (BASIC_BLOCK (0)); !bsi_end_p (i); bsi_next (&i))
    walk_tree (bsi_stmt_ptr (i), collect_dfa_stats_r, (void *) dfa_stats_p,
	       (void *) htab);

  htab_delete (htab);

  FOR_EACH_BB (bb)
    {
      tree phi;
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  dfa_stats_p->num_phis++;
	  dfa_stats_p->num_phi_args += PHI_NUM_ARGS (phi);
	  if (PHI_NUM_ARGS (phi) > dfa_stats_p->max_num_phi_args)
	    dfa_stats_p->max_num_phi_args = PHI_NUM_ARGS (phi);
	}
    }
}


/* Callback for walk_tree to collect DFA statistics for a tree and its
   children.  */

static tree
collect_dfa_stats_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		     void *data)
{
  tree t = *tp;
  struct dfa_stats_d *dfa_stats_p = (struct dfa_stats_d *)data;

  if (t->common.ann)
    {
      switch (ann_type (t->common.ann))
	{
	case STMT_ANN:
	  {
	    stmt_ann_t ann = (stmt_ann_t) t->common.ann;
	    dfa_stats_p->num_stmt_anns++;
	    dfa_stats_p->num_defs += NUM_DEFS (DEF_OPS (ann));
	    dfa_stats_p->num_uses += NUM_USES (USE_OPS (ann));
	    dfa_stats_p->num_vdefs += NUM_VDEFS (VDEF_OPS (ann));
	    dfa_stats_p->num_vuses += NUM_VUSES (VUSE_OPS (ann));
	    break;
	  }

	case VAR_ANN:
	  dfa_stats_p->num_var_anns++;
	  break;

	default:
	  break;
	}
    }

  return NULL;
}


/*---------------------------------------------------------------------------
				    Aliasing
---------------------------------------------------------------------------*/

/* ADDRESSABLE_VARS contains all the global variables and locals that have
   had their address taken.  POINTERS contains all the pointers that have been
   referenced in the program.  Alias analysis will determine, for every two
   elements from each array whether they may alias each other or not.  */
static GTY(()) varray_type addressable_vars;
static GTY(()) varray_type pointers;


/* Compute may-alias information for every variable referenced in function
   FNDECL.  Note that in the absence of points-to analysis
   (-ftree-points-to), this may compute a much bigger set than necessary.  */

static void
compute_may_aliases (void)
{
  VARRAY_GENERIC_PTR_INIT (addressable_vars, 20, "addressable_vars");
  VARRAY_GENERIC_PTR_INIT (pointers, 20, "pointers");
  memset (&alias_stats, 0, sizeof (alias_stats));

  /* Remove the ADDRESSABLE flag from every call-clobbered variable whose
     address is not needed anymore.  This is caused by the propagation of
     ADDR_EXPR constants into INDIRECT_REF expressions and the removal of
     dead pointer assignments done by the early scalar cleanup passes.  */
  promote_call_clobbered_vars (vars_to_rename);

  /* Create memory tags for all the dereferenced pointers and build the
     ADDRESSABLE_VARS and POINTERS arrays used for building the may-alias
     sets.  */
  create_memory_tags (vars_to_rename);

  /* Compute alias sets.  */
  compute_alias_sets ();
  
  /* Debugging dumps.  */
  if (tree_dump_file)
    {
      if (tree_dump_flags & TDF_STATS)
	dump_alias_stats (tree_dump_file);
      dump_alias_info (tree_dump_file);
      dump_referenced_vars (tree_dump_file);
    }

  /* Deallocate memory used by aliasing data structures.  */
  addressable_vars = NULL;
  pointers = NULL;

  /* Indicate that may-alias information is now available.  */
  aliases_computed_p = true;
}

struct tree_opt_pass pass_may_alias = 
{
  "alias",				/* name */
  NULL,					/* gate */
  compute_may_aliases,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_MAY_ALIAS,			/* tv_id */
  PROP_cfg | PROP_ssa | PROP_pta,	/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_rename_vars
    | TODO_ggc_collect | TODO_verify_ssa  /* todo_flags_finish */
};


/* Create memory tags for every dereferenced pointer in the program.
   Pointers and the alias set of their pointed-to type are added to the
   POINTERS array.  Addressable variable and globals are added, together
   with their alias set, to the ADDRESSABLE_VARS array.
   
   Addressable variables and newly created memory tags are marked in
   VARS_TO_RENAME to be converted into SSA form afterwards.  */

static void
create_memory_tags (bitmap vars_to_rename)
{
  size_t i;

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      var_ann_t v_ann = var_ann (var);

      /* Global variables and addressable locals may be aliased.  Create an
	 entry in ADDRESSABLE_VARS for VAR.  */
      if (may_be_aliased (var))
	{
	  /* Create a new alias set entry for VAR.  */
	  struct alias_map_d *alias_map;
	  alias_map = ggc_alloc (sizeof (*alias_map));
	  alias_map->var = var;

	  if (TREE_CODE (TREE_TYPE (var)) == ARRAY_TYPE)
	    alias_map->set = get_alias_set (TREE_TYPE (TREE_TYPE (var)));
	  else
	    alias_map->set = get_alias_set (var);
	  VARRAY_PUSH_GENERIC_PTR (addressable_vars, alias_map);
	  bitmap_set_bit (vars_to_rename, var_ann (var)->uid);
	}

      /* Add pointer variables that have been dereferenced to the POINTERS
	 array and create a memory tag for them.  */
      if (POINTER_TYPE_P (TREE_TYPE (var))
	  && (v_ann->is_dereferenced_store
	      || v_ann->is_dereferenced_load))
	{
	  tree tag = v_ann->mem_tag;
	  var_ann_t t_ann;

	  /* If pointer VAR still doesn't have a memory tag associated with it,
	     create it now or re-use an existing one.  */
	  if (tag == NULL_TREE)
	    tag = get_memory_tag_for (var);

	  /* Mark the memory tag for renaming.  */
	  t_ann = var_ann (tag);
	  bitmap_set_bit (vars_to_rename, t_ann->uid);

	  /* Associate the tag with pointer VAR.  */
	  v_ann->mem_tag = tag;

	  /* If pointer VAR may point to global mem, then TAG may alias
	     global memory.  */
	  if (v_ann->may_point_to_global_mem)
	    t_ann->may_alias_global_mem = 1;

	  /* If pointer VAR has been used in a store operation, then its
	     memory tag must be marked as stored.  */
	  if (v_ann->is_dereferenced_store)
	    t_ann->is_stored = 1;
	}
    }
}


/* Compute alias sets.  Aliasing information is computed in two stages:

   1- Artificial variables called "memory tags" are created for each
      pointer used in the program.  Each memory tag (MT) represents the
      memory location pointed by its associated pointer.  Since pointers
      may point to each other, two or more pointers that may point to each
      other will be assigned the same memory tag.  These unique memory tags
      are computed by get_memory_tag_for and their associated pointers are
      added to the POINTERS array.

   2- All the addressable variables in ADDRESSABLE_VARS are compared against
      the pointers collected in step 1.  If a pointer P may point to
      variable V, then V is added to the list of may-aliases for P.

   For instance, consider the following function:

	    foo (int i)
	    {
	      int *p, a, b;
	    
	      if (i > 10)
	        p = &a;
	      else
	        p = &b;
	    
	      *p = 3;
	      a = b + 2;
	      return *p;
	    }

   After aliasing analysis has finished, the memory tag for pointer 'p'
   will have two aliases, namely variables 'a' and 'b'.  Every time pointer
   'p' is dereferenced, we want to mark the operation as a potential
   reference to 'a' and 'b'.  This is marked with virtual operands.
   Resulting in the following renamed program:

	    foo (int i)
	    {
	      int *p, a, b;

	      if (i_2 > 10)
		p_4 = &a;
	      else
		p_6 = &b;
	      # p_1 = PHI <p_4(1), p_6(2)>;

	      # a_7 = VDEF <a_3>;
	      # b_8 = VDEF <b_5>;
	      *p_1 = 3;
	      a_9 = b_8 + 2;

	      # VUSE <a_9>;
	      # VUSE <b_8>;
	      return *p_1;
	    }

   This method allows the compiler to optimize aliased variables when
   they're use directly and prevent optimizations when they are being
   accessed via aliased pointers.

   In certain cases, the list of may aliases for a pointer may grow too
   large.  This may cause an explosion in the number of virtual operands
   inserted in the code.  Resulting in increased memory consumption and
   compilation time.

   When the set of may aliases for a pointer grows beyond 5 elements
   (FIXME, this is currently an arbitrary limit), instead of adding new
   variables to the may-alias set, the new variables are made to share the
   same alias set as the original pointer.  For instance, suppose that
   pointer 'p' may point to variables 'a', 'b', 'c', 'd', 'e', 'f' and 'g'.
   After alias analysis, the alias sets will be as follows:

	may-alias(p) = { a, b, c, d, e }
	may-alias(f) = { a, b, c, d, e }
	may-alias(g) = { a, b, c, d, e }

   Notice that this grouping causes variables 'f' and 'g' to be aliased to
   variables they can't possibly alias to.  */

static void
compute_alias_sets (void)
{
  size_t i;


  /* For every pointer P, determine which addressable variables may alias
     with P.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (pointers); i++)
    {
      size_t j;
      struct alias_map_d *ptr = VARRAY_GENERIC_PTR (pointers, i);
      tree mem = var_ann (ptr->var)->mem_tag;
      var_ann_t mem_ann = var_ann (mem);

      for (j = 0; j < VARRAY_ACTIVE_SIZE (addressable_vars); j++)
	{
	  struct alias_map_d *var = VARRAY_GENERIC_PTR (addressable_vars, j);
	  var_ann_t v_ann = var_ann (var->var);

	  /* Skip memory tags and variables that have never been written to.  */
	  if (!mem_ann->is_stored && !v_ann->is_stored)
	    continue;
	     
	  /* Skip memory tags which are written if the variable is readonly.  */
	  if (mem_ann->is_stored && TREE_READONLY (var->var))
	    continue;

	  if (may_alias_p (ptr->var, ptr->set, var->var, var->set))
	    {
	      /* If MEM has less than 5 aliases in its alias set, add
		 VAR->VAR to the list of aliases for MEM.  Otherwise,
		 set the may-alias set for VAR->VAR to be the same alias
		 set as MEM.  This is to avoid the problem of having
		 large may-alias sets.  Large may-alias sets translate into
		 lots of virtual operands which can slow down the SSA pass
		 tremendously.  */
	      if (mem_ann->may_aliases
		  && VARRAY_ACTIVE_SIZE (mem_ann->may_aliases) >= 5)
		{
		  VARRAY_TREE_INIT (v_ann->may_aliases,
				    VARRAY_SIZE (mem_ann->may_aliases),
				    "aliases");
		  varray_copy (v_ann->may_aliases, mem_ann->may_aliases);
		}
	      else
		add_may_alias (mem, var->var);
	    }
	}
    }

  /* If the function has calls to clobbering functions, make GLOBAL_VAR
     an alias for all call-clobbered variables.  */
  if (global_var)
    for (i = 0; i < num_call_clobbered_vars; i++)
      {
	tree var = call_clobbered_var (i);
	if (var != global_var)
	  add_may_alias (var, global_var);
      }
}


/* Return TRUE if pointer PTR may point to variable VAR.
   
   MEM_ALIAS_SET is the alias set for the memory location pointed-to by PTR
	This is needed because when checking for type conflicts we are
	interested in the alias set of the memory location pointed-to by
	PTR.  The alias set of PTR itself is irrelevant.
   
   VAR_ALIAS_SET is the alias set for VAR.  */

static bool
may_alias_p (tree ptr, HOST_WIDE_INT mem_alias_set,
	     tree var, HOST_WIDE_INT var_alias_set)
{
  tree mem;
  var_ann_t v_ann, m_ann;

  mem = var_ann (ptr)->mem_tag;
  alias_stats.alias_queries++;
  alias_stats.simple_queries++;
  /* By convention, a variable cannot alias itself.  */
  if (mem == var)
    {
      alias_stats.alias_noalias++;
      alias_stats.simple_resolved++;
      return false;
    }

  v_ann = var_ann (var);
  m_ann = var_ann (mem);

#if defined ENABLE_CHECKING
  if (!m_ann->is_mem_tag)
    abort ();
#endif
  alias_stats.tbaa_queries++;
  /* If VAR is a pointer with the same alias set as PTR, then dereferencing
     PTR can't possibly affect VAR.  Note, that we are specifically testing
     for PTR's alias set here, not its pointed-to type.  We also can't
     do this check with relaxed aliasing enabled.  */
  if (POINTER_TYPE_P (TREE_TYPE (var))
      && var_alias_set != 0)
    {
      HOST_WIDE_INT ptr_alias_set = get_alias_set (ptr);
      if (ptr_alias_set == var_alias_set)
	{
	  alias_stats.alias_noalias++;
	  alias_stats.tbaa_resolved++;
	  return false;
	}
    }
  /* If the alias sets don't conflict then MEM cannot alias VAR.  */
  if (!alias_sets_conflict_p (mem_alias_set, var_alias_set))
    {
      /* Handle aliases to structure fields.  If either VAR or MEM are
	 aggregate types, they may not have conflicting types, but one of
	 the structures could contain a pointer to the other one.

	 For instance, given

		MEM -> struct P *p;
		VAR -> struct Q *q;

	 It may happen that '*p' and '*q' can't alias because 'struct P'
	 and 'struct Q' have non-conflicting alias sets.  However, it could
	 happen that one of the fields in 'struct P' is a 'struct Q *' or
	 vice-versa.

	 Therefore, we also need to check if 'struct P' aliases 'struct Q *'
	 or 'struct Q' aliases 'struct P *'.  Notice, that since GIMPLE
	 does not have more than one-level pointers, we don't need to
	 recurse into the structures.  */
      if (AGGREGATE_TYPE_P (TREE_TYPE (mem))
	  || AGGREGATE_TYPE_P (TREE_TYPE (var)))
	{
	  tree ptr_to_var = TYPE_POINTER_TO (TREE_TYPE (var));

	  /* If no pointer-to VAR exists, then MEM can't alias VAR.  */
	  if (ptr_to_var == NULL_TREE)
	    {
	      alias_stats.alias_noalias++;
	      alias_stats.tbaa_resolved++;
	      return false;
	    }

	  /* If MEM doesn't alias a pointer to VAR and VAR doesn't alias
	     PTR, then PTR can't alias VAR.  */
	  if (!alias_sets_conflict_p (mem_alias_set, get_alias_set (ptr_to_var))
	      && !alias_sets_conflict_p (var_alias_set, get_alias_set (ptr)))
	    {
	      alias_stats.alias_noalias++;
	      alias_stats.tbaa_resolved++;
	      return false;
	    }
	}
      else
	{
	  alias_stats.alias_noalias++;
	  alias_stats.tbaa_resolved++;
	  return false;
	}
    }

  if (flag_tree_points_to != PTA_NONE)
      alias_stats.pta_queries++;

  /* If -ftree-points-to is given, check if PTR may point to VAR.  */
  if (flag_tree_points_to == PTA_ANDERSEN
      && !ptr_may_alias_var (ptr, var))
    {
      alias_stats.alias_noalias++;
      alias_stats.pta_resolved++;
      return false;
    }

  alias_stats.alias_mayalias++;
  return true;
}


/* Add ALIAS to the set of variables that may alias VAR.  */

static void
add_may_alias (tree var, tree alias)
{
  size_t i;
  var_ann_t v_ann = get_var_ann (var);
  var_ann_t a_ann = get_var_ann (alias);

#if defined ENABLE_CHECKING
  if (var == alias)
    abort ();
#endif

  if (v_ann->may_aliases == NULL)
    VARRAY_TREE_INIT (v_ann->may_aliases, 2, "aliases");

  /* Avoid adding duplicates.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (v_ann->may_aliases); i++)
    if (alias == VARRAY_TREE (v_ann->may_aliases, i))
      return;

  /* If either VAR or ALIAS may access global memory, then mark the other
     one as a global memory alias.  */
  if (may_access_global_mem_p (var))
    a_ann->may_alias_global_mem = 1;

  if (may_access_global_mem_p (alias))
    v_ann->may_alias_global_mem = 1;

  VARRAY_PUSH_TREE (v_ann->may_aliases, alias);
  a_ann->is_alias_tag = 1;
}


/* Find variable VAR in ARRAY.  Return -1 if VAR doesn't exist.  */

static inline int
find_variable_in (varray_type array, tree var)
{
  size_t i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (array); i++)
    if (var == VARRAY_TREE (array, i))
      return (int) i;

  return -1;
}


/* Remove element I from ARRAY by swapping element I with the last element
   of ARRAY.  */

static inline void
remove_element_from (varray_type array, size_t i)
{
  size_t len;

  len = VARRAY_ACTIVE_SIZE (array);
  if (i < len - 1)
    VARRAY_TREE (array, i) = VARRAY_TREE (array, len - 1);
  VARRAY_POP (array);
}


/* Find call-clobbered variables whose address is not needed anymore.  */

static void
promote_call_clobbered_vars (bitmap vars_to_rename)
{
  size_t i;
  bitmap addresses_needed;
  
  addresses_needed = BITMAP_XMALLOC ();
  bitmap_clear (addresses_needed);

  /* Find variables that still need to have their address taken.  */
  find_addressable_vars (addresses_needed);

  /* Promote call-clobbered variables whose addresses are not needed
     anymore.  */
  for (i = 0; i < num_call_clobbered_vars; i++)
    {
      tree var = call_clobbered_var (i);
      var_ann_t ann = var_ann (var);

      if (TREE_ADDRESSABLE (var)
	  && !bitmap_bit_p (addresses_needed, ann->uid)
	  && !ann->has_hidden_use
	  && !needs_to_live_in_memory (var))
	{
	  ann->is_call_clobbered = 0;
	  ann->may_alias_global_mem = 0;
	  TREE_ADDRESSABLE (var) = 0;

	  /* Add VAR to the list of variables to rename.  */
	  bitmap_set_bit (vars_to_rename, var_ann (var)->uid);

	  /* Debugging dumps.  */
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_dump_file, "Removing ADDRESSABLE flag from variable ");
	      print_generic_expr (tree_dump_file, var, 0);
	    }
	}
    }

  /* Remove promoted variables from CALL_CLOBBERED_VARS.  */
  for (i = 0; i < num_call_clobbered_vars; i++)
    if (!is_gimple_call_clobbered (call_clobbered_var (i)))
      {
	remove_element_from (call_clobbered_vars, i);
	i--;	/* Re-set the iterator to account for the removed element.  */
      }

  /* Free allocated memory.  */
  BITMAP_FREE (addresses_needed);
}


/* Find all the variables that need to remain marked addressable and
   variables that need to be renamed in a second SSA pass because of the
   propagation of ADDR_EXPR values into INDIRECT_REF expressions.

   Variables that need to remain addressable are marked in the
   ADDRESSES_NEEDED bitmap.  */

static void
find_addressable_vars (bitmap addresses_needed)
{
  basic_block bb;

  /* Since ADDR_EXPRs have been propagated into INDIRECT_REF expressions
     and folded from '*&VAR' into 'VAR', some statements will have operands
     that are not in SSA form.  Those variables are added to a list of
     variables to be renamed in a second SSA pass.  */
  FOR_EACH_BB (bb)
    {
      block_stmt_iterator si;
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  int i;
	  for (i = 0; i < PHI_NUM_ARGS (phi); ++i)
	    {
	      tree t = PHI_ARG_DEF (phi, i);

	      if (TREE_CODE (t) != ADDR_EXPR)
		continue;

	      t = get_base_symbol (TREE_OPERAND (t, 0));
	      if (t && (TREE_CODE (t) == VAR_DECL
		        || TREE_CODE (t) == PARM_DECL))
		bitmap_set_bit (addresses_needed, var_ann (t)->uid);
	    }
	}

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  varray_type ops;
	  size_t i;
	  tree stmt = bsi_stmt (si);

	  get_stmt_operands (stmt);

	  ops = addresses_taken (stmt);
	  if (ops)
	    {
	      for (i = 0; i < VARRAY_ACTIVE_SIZE (ops); i++)
		{
		  tree var = VARRAY_TREE (ops, i);
		  bitmap_set_bit (addresses_needed, var_ann (var)->uid);
		}
	    }

	  /* FIXME: We should only modify statements that have pointer
	     de-references or addressable variables in them.  */
	  modify_stmt (stmt);
	}
    }
}


static void 
dump_alias_stats (FILE *file)
{
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);
  fprintf (file, "\nAlias statistics for %s\n\n", funcname);
  fprintf (file, "Total alias queries:\t%u\n", alias_stats.alias_queries);
  fprintf (file, "Total alias mayalias results:\t%u\n", 
	   alias_stats.alias_mayalias);
  fprintf (file, "Total alias noalias results:\t%u\n",
	   alias_stats.alias_noalias);
  fprintf (file, "Total simple queries:\t%u\n",
	   alias_stats.simple_queries);
  fprintf (file, "Total simple resolved:\t%u\n",
	   alias_stats.simple_resolved);
  fprintf (file, "Total TBAA queries:\t%u\n",
	   alias_stats.tbaa_queries);
  fprintf (file, "Total TBAA resolved:\t%u\n",
	   alias_stats.tbaa_resolved);
  fprintf (file, "Total PTA queries:\t%u\n",
	   alias_stats.pta_queries);
  fprintf (file, "Total PTA resolved:\t%u\n",
	   alias_stats.pta_resolved);
}
  
/* Dump alias information on FILE.  */

void
dump_alias_info (FILE *file)
{
  size_t i;
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);

  if (addressable_vars == NULL)
    return;

  fprintf (file, "\nAlias information for %s\n\n", funcname);
  fprintf (file, "%u addressable variables\n",
           (unsigned) VARRAY_ACTIVE_SIZE (addressable_vars));
  fprintf (file, "%u memory tags\n\n",
           (unsigned) VARRAY_ACTIVE_SIZE (pointers));

  for (i = 0; i < VARRAY_ACTIVE_SIZE (pointers); i++)
    {
      struct alias_map_d *map = VARRAY_GENERIC_PTR (pointers, i);
      tree mem = var_ann (map->var)->mem_tag;
      varray_type aliases = may_aliases (mem);
      if (aliases)
	{
	  fprintf (file, "Memory tag ");
	  print_generic_expr (file, mem, 0);
	  fprintf (file, " aliases ");
	  dump_may_aliases_for (file, mem);
	}
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (addressable_vars); i++)
    {
      struct alias_map_d *map = VARRAY_GENERIC_PTR (addressable_vars, i);
      varray_type aliases = may_aliases (map->var);
      if (aliases)
	{
	  fprintf (file, "Addressable var ");
	  print_generic_expr (file, map->var, 0);
	  fprintf (file, " aliases ");
	  dump_may_aliases_for (file, map->var);
	}
    }

  fprintf (file, "\n");
}


/* Dump alias information on stderr.  */

void
debug_alias_info (void)
{
  dump_alias_info (stderr);
}



/*---------------------------------------------------------------------------
			     Miscellaneous helpers
---------------------------------------------------------------------------*/
/* Return TRUE if expression EXPR may reference memory outside the current
   function scope.  */

static bool
may_access_global_mem_p (tree expr)
{
  char class;

  if (expr == NULL_TREE)
    return false;

  /* Function arguments and global variables may reference global memory.  */
  if (DECL_P (expr)
      && (TREE_CODE (expr) == PARM_DECL
	  || decl_function_context (expr) != current_function_decl))
    return true;

  /* If the expression is a variable that may point to or alias global memory,
     return true.  */
  if (SSA_VAR_P (expr))
    {
      var_ann_t ann;
      ann = var_ann (TREE_CODE (expr) == SSA_NAME ? SSA_NAME_VAR (expr) : expr);
      if (ann->may_point_to_global_mem || ann->may_alias_global_mem)
	return true;
    }

  /* Otherwise, the expression must be of pointer type.  */
  if (TREE_TYPE (expr) == NULL_TREE
      || !POINTER_TYPE_P (TREE_TYPE (expr)))
    return false;

  /* Call expressions that return pointers may point to global memory.  */
  if (TREE_CODE (expr) == CALL_EXPR)
    return true;

  /* A non-NULL constant used as a pointer points to global memory.  */
  if (TREE_CONSTANT (expr) && !integer_zerop (expr))
    return true;

  class = TREE_CODE_CLASS (TREE_CODE (expr));

  /* If EXPR is a reference, see if its base address may access global
     memory.  */
  if (class == 'r')
    {
      while (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (TREE_CODE (expr))))
	expr = TREE_OPERAND (expr, 0);
      return may_access_global_mem_p (expr);
    }

  /* Recursively check the expression's operands.  */
  if (IS_EXPR_CODE_CLASS (class))
    {
      unsigned char i;

      for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (expr)); i++)
	if (may_access_global_mem_p (TREE_OPERAND (expr, i)))
	  return true;
    }

  return false;
}


/* Remove variable DECL from the block that declares it.  */

void
remove_decl (tree decl, tree block)
{
  tree *loc;
  
  loc = find_decl_location (decl, block);
  if (loc)
    *loc = TREE_CHAIN (decl);
}


/* Find the location for declaration DECL in lexical block BLOCK.  All the
   subblocks of BLOCK are searched as well if BLOCK does not declare DECL.
   Return an address LOC such that *LOC == DECL or NULL if DECL couldn't be
   located.  */

tree *
find_decl_location (tree decl, tree block)
{
  tree d, sub;

  /* Special case.  If DECL is the first symbol in the block, return its
     location directly.  */
  if (BLOCK_VARS (block) == decl)
    return &(BLOCK_VARS (block));

  for (d = BLOCK_VARS (block); d; d = TREE_CHAIN (d))
    if (TREE_CHAIN (d) == decl)
      return &(TREE_CHAIN (d));

  for (sub = BLOCK_SUBBLOCKS (block); sub; sub = TREE_CHAIN (sub))
    {
      tree *loc = find_decl_location (decl, sub);
      if (loc)
	return loc;
    }

  return NULL;
}


/* Callback for walk_tree.  Used to collect variables referenced in
   the function.  */

static tree
find_vars_r (tree *tp, int *walk_subtrees, void *data)
{
  tree t = *tp;
  struct walk_state *walk_state = (struct walk_state *)data;

  /* Type and constant nodes have no interesting children.  Ignore them.  */
  if (TYPE_P (t) || TREE_CODE_CLASS (TREE_CODE (t)) == 'c')
    {
      *walk_subtrees = 0;
      return NULL_TREE;
    }

  /* DECL nodes have no interesting children.  */
  if (DECL_P (t))
    {
      *walk_subtrees = 0;

      /* If this _DECL node is not interesting to the SSA builder,
         then we can just return now.  */
      if (! SSA_VAR_P (t)) 
	return NULL_TREE;
    }

  if (TREE_CODE (t) == MODIFY_EXPR)
    {
      tree *lhs_p = &TREE_OPERAND (t, 0);
      tree *rhs_p = &TREE_OPERAND (t, 1);

      walk_state->is_store = 1;
      walk_tree (lhs_p, find_vars_r, walk_state, NULL);
      walk_state->is_store = 0;
      walk_tree (rhs_p, find_vars_r, walk_state, NULL);

      /* If this is an assignment to a pointer and the RHS may point to
	 global memory, mark the pointer on the LHS.  */
      if (SSA_VAR_P (*lhs_p)
	  && POINTER_TYPE_P (TREE_TYPE (*lhs_p))
	  && may_access_global_mem_p (*rhs_p))
	set_may_point_to_global_mem (*lhs_p);

      /* If either side makes volatile references, mark the statement.  */
      if (TREE_THIS_VOLATILE (*lhs_p)
	  || TREE_THIS_VOLATILE (*rhs_p))
	get_stmt_ann (t)->has_volatile_ops = 1;

      return t;
    }
  else if (TREE_CODE (t) == ASM_EXPR)
    {
      walk_state->is_asm_expr = 1;
      walk_state->is_store = 1;
      walk_tree (&ASM_OUTPUTS (t), find_vars_r, walk_state, NULL);
      walk_tree (&ASM_CLOBBERS (t), find_vars_r, walk_state, NULL);
      walk_state->is_store = 0;
      walk_tree (&ASM_INPUTS (t), find_vars_r, walk_state, NULL);
      walk_state->is_asm_expr = 0;
      return t;
    }
  else if (TREE_CODE (t) == INDIRECT_REF)
    {
      walk_state->is_indirect_ref = 1;
      walk_tree (&TREE_OPERAND (t, 0), find_vars_r, walk_state, NULL);

      /* INDIRECT_REF nodes cannot be nested in GIMPLE, so there is no need
	 of saving/restoring the state.  */
      walk_state->is_indirect_ref = 0;

      /* Keep iterating, because an INDIRECT_REF node may have more
	 references inside (structures and arrays).  */
      return NULL_TREE;
    }

  if (SSA_VAR_P (t))
    {
      add_referenced_var (t, walk_state);
      return NULL_TREE;
    }

  /* A function call that receives pointer arguments may dereference them.
     For every pointer 'p' in the argument to the function call, add a
     reference to '*p'.  */
  if (TREE_CODE (t) == CALL_EXPR)
    {
      tree op;

      walk_state->num_calls++;

      for (op = TREE_OPERAND (t, 1); op; op = TREE_CHAIN (op))
	{
	  tree arg = TREE_VALUE (op);
	  if (SSA_VAR_P (arg)
	      && POINTER_TYPE_P (TREE_TYPE (arg))
	      && !VOID_TYPE_P (TREE_TYPE (TREE_TYPE (arg))))
	    {
	      walk_state->is_indirect_ref = 1;
	      add_referenced_var (arg, walk_state);
	      walk_state->is_indirect_ref = 0;
	    }
	}

      /* Note that we may undo this creation after all the variables and
	 call sites have been found.  See find_referenced_vars.  */
      if (global_var == NULL_TREE)
	create_global_var ();
    }

  return NULL_TREE;
}


/* Add VAR to the list of dereferenced variables.  If VAR is a candidate
   for aliasing, add it to the ADDRESSABLE_VAR array.  If VAR is a memory
   tag, add it to the POINTERS array.  These two arrays are used for
   alias analysis (See compute_alias_sets).

   WALK_STATE is an array with a hash table used to avoid adding the
   same variable more than once to its corresponding set as well as flags
   indicating if we're processing a load or store.  Note that this function
   assumes that VAR is a valid SSA variable.  */

static void
add_referenced_var (tree var, struct walk_state *walk_state)
{
  void **slot;
  var_ann_t v_ann;

  v_ann = get_var_ann (var);

  if (walk_state)
    slot = htab_find_slot (walk_state->vars_found, (void *) var, INSERT);
  else
    slot = NULL;

  if (slot == NULL || *slot == NULL)
    {
      /* This is the first time we find this variable, add it to the
         REFERENCED_VARS array and annotate it with attributes that are
	 intrinsic to the variable.  */
      if (slot)
	*slot = (void *) var;
      v_ann->uid = num_referenced_vars;
      VARRAY_PUSH_TREE (referenced_vars, var);

      /* Arguments or global variable pointers may point to memory outside
	 the current function.  */
      if (POINTER_TYPE_P (TREE_TYPE (var))
	  && (TREE_CODE (var) == PARM_DECL
	      || decl_function_context (var) != current_function_decl))
	v_ann->may_point_to_global_mem = 1;

      /* Mark local statics and global variables as global memory aliases
	 to avoid DCE killing seemingly dead stores to them.  */
      if (decl_function_context (var) != current_function_decl
	  || TREE_STATIC (var))
	v_ann->may_alias_global_mem = 1;

      /* Add call clobbered variables to a separate array.  */
      if (is_gimple_call_clobbered (var))
	{
	  VARRAY_PUSH_TREE (call_clobbered_vars, var);
	  v_ann->is_call_clobbered = 1;
	  if (POINTER_TYPE_P (TREE_TYPE (var)))
	    v_ann->may_point_to_global_mem = 1;
	}

      /* DECL_NONLOCAL variables should not be removed, as they are needed
	 to emit nested functions.  */
      if (DECL_NONLOCAL (var))
	v_ann->used = 1;
    }

  /* Now, set attributes that depend on WALK_STATE.  */
  if (walk_state == NULL)
    return;

  /* Remember if the variable has been written to.  This is important for
     alias analysis.  If a variable and its aliases are never modified, it
     is not interesting for the optimizers because there are no aliased
     stores to keep track of.  */
  if (walk_state->is_store)
    v_ann->is_stored = 1;

  /* If the variable is a pointer being clobbered by an ASM_EXPR, the
     pointer may end up pointing to global memory.  */
  if (POINTER_TYPE_P (TREE_TYPE (var))
      && walk_state->is_store
      && walk_state->is_asm_expr)
    v_ann->may_point_to_global_mem = 1;

  /* If VAR is a pointer referenced in an INDIRECT_REF node, mark it so
     that alias analysis creates a memory tag representing the location
     pointed-to by pointer VAR.  */
  if (walk_state->is_indirect_ref)
    {
      if (walk_state->is_store)
	v_ann->is_dereferenced_store = 1;
      else
	v_ann->is_dereferenced_load = 1;
    }
}


/* Return the memory tag associated to pointer PTR.  A memory tag is an
   artificial variable that represents the memory location pointed-to by
   PTR.  It is used to model the effects of pointer de-references on
   addressable variables.  */

static tree
get_memory_tag_for (tree ptr)
{
  size_t i;
  tree tag;
  tree tag_type = TREE_TYPE (TREE_TYPE (ptr));
  HOST_WIDE_INT tag_set = get_alias_set (tag_type);

  /* To avoid creating unnecessary memory tags, only create one memory tag
     per alias set class.  */
  for (i = 0, tag = NULL_TREE; i < VARRAY_ACTIVE_SIZE (pointers); i++)
    {
      struct alias_map_d *curr = VARRAY_GENERIC_PTR (pointers, i);
      if (tag_set == curr->set 
	  && (flag_tree_points_to == PTA_NONE 
	      || same_points_to_set (curr->var, ptr)))
	{
	  tag = var_ann (curr->var)->mem_tag;
	  break;
	}
    }

  /* If VAR cannot alias with any of the existing memory tags, create a new
     tag for PTR and add it to the POINTERS array.  */
  if (tag == NULL_TREE)
    {
      struct alias_map_d *alias_map;
      var_ann_t tag_ann;

      /* Create a new MT.* artificial variable representing the memory
	 location pointed-to by PTR.  */
      tag = create_tmp_var_raw (tag_type, "MT");

      /* If the pointed-to type is volatile, so is the tag.  */
      TREE_THIS_VOLATILE (tag) = TREE_THIS_VOLATILE (tag_type);

      tag_ann = get_var_ann (tag);
      tag_ann->is_mem_tag = 1;
      tag_ann->mem_tag = NULL_TREE;
      add_referenced_tmp_var (tag);

      /* Memory tags are by definition addressable.  This also avoids
	 is_gimple_ref for confusing memory tags with optimizable
	 variables.  FIXME: The 'is_mem_tag' attribute should probably be
	 moved into the VAR_DECL node.  */
      TREE_ADDRESSABLE (tag) = 1;

      /* Add PTR to the POINTERS array.  Note that we are not interested in
	 PTR's alias set.  Instead, we cache the alias set for the memory that
	 PTR points to.  */
      alias_map = ggc_alloc (sizeof (*alias_map));
      alias_map->var = ptr;
      alias_map->set = tag_set;
      VARRAY_PUSH_GENERIC_PTR (pointers, alias_map);
    }

  return tag;
}


/* Return the virtual variable associated to the non-scalar variable VAR.  */

tree
get_virtual_var (tree var)
{
  enum tree_code code;

  STRIP_NOPS (var);

  if (TREE_CODE (var) == SSA_NAME)
    var = SSA_NAME_VAR (var);

  code = TREE_CODE (var);

  while (code == ARRAY_REF
         || code == COMPONENT_REF
	 || code == REALPART_EXPR
	 || code == IMAGPART_EXPR)
    {
      var = TREE_OPERAND (var, 0);
      code = TREE_CODE (var);
    }

  return var;
}

/* Mark variables that have hidden uses.

   A hidden use can occur due to VLA declarations or nested functions.   */

static void
find_hidden_use_vars (tree block)
{
  tree sub, decl;

  /* Check all the arrays declared in the block for VLAs.

     While scanning the block's variables, also see if there is
     a nested function at this scope.  */
  for (decl = BLOCK_VARS (block); decl; decl = TREE_CHAIN (decl))
    {
      int inside_vla = 0;
      walk_tree (&decl, find_hidden_use_vars_r, &inside_vla, NULL);
    }

  /* Now repeat the search in any sub-blocks.  */
  for (sub = BLOCK_SUBBLOCKS (block); sub; sub = TREE_CHAIN (sub))
    find_hidden_use_vars (sub);
}

/* Callback for walk_tree used by find_hidden_use_vars to analyze each 
   variable in a lexical block.  If the variable's size has a variable
   size, then mark all objects needed to compute the variable's size
   as having hidden uses.  */

static tree
find_hidden_use_vars_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
			void *data ATTRIBUTE_UNUSED)
{
  int *inside_vla = (int *) data;

  /* We need to look for hidden uses due to VLAs in variable
     definitions.  We originally used to look for these hidden
     uses in the variable's type, but that's unreliable if the
     type's size contains a SAVE_EXPR for a different function
     context than the variable is used within.  */
  if (SSA_VAR_P (*tp)
      && ((DECL_SIZE (*tp)
	   && ! really_constant_p (DECL_SIZE (*tp)))
	  || (DECL_SIZE_UNIT (*tp)
	      && ! really_constant_p (DECL_SIZE_UNIT (*tp)))))
    {
      int save = *inside_vla;

      *inside_vla = 1;
      walk_tree (&DECL_SIZE (*tp), find_hidden_use_vars_r, inside_vla, NULL);
      walk_tree (&DECL_SIZE_UNIT (*tp), find_hidden_use_vars_r,
		 inside_vla, NULL);
      *inside_vla = save;
    }
  else if (*inside_vla && SSA_VAR_P (*tp))
    set_has_hidden_use (*tp);

  return NULL_TREE;
}


/* Create GLOBAL_VAR, an artificial global variable to act as a
   representative of all the variables that may be clobbered by function
   calls.  */

void
create_global_var (void)
{
  global_var = build_decl (VAR_DECL, get_identifier (".GLOBAL_VAR"),
                           size_type_node);
  DECL_ARTIFICIAL (global_var) = 1;
  TREE_READONLY (global_var) = 1;
  DECL_EXTERNAL (global_var) = 0;
  TREE_STATIC (global_var) = 1;
  TREE_USED (global_var) = 1;
  DECL_CONTEXT (global_var) = current_function_decl;
  TREE_THIS_VOLATILE (global_var) = 1;
  TREE_ADDRESSABLE (global_var) = 0;
}


/* Add a temporary variable to REFERENCED_VARS.  This is similar to
   add_referenced_var, but is used by passes that need to add new temps to
   the REFERENCED_VARS array after the program has been scanned for
   variables.  In particular, none of the annotations that depend on struct
   walk_state will be set.  The variable will just receive a new UID and be
   added to the REFERENCED_VARS array without checking for duplicates.  */

void
add_referenced_tmp_var (tree var)
{
  add_referenced_var (var, NULL);
}

/* Return true if VDEFS_AFTER contains fewer entries than VDEFS_BEFORE.
   Note that this assumes that both varrays are VDEF operands for the same
   statement.  */

static inline bool
vdefs_disappeared_p (vdef_optype vdefs_before, vdef_optype vdefs_after)
{
  /* If there was nothing before, nothing could've disappeared.  */
  if (vdefs_before == NULL)
    return false;
     
  /* All/some of them gone.  */
  if (vdefs_after == NULL
      || NUM_VDEFS (vdefs_before) > NUM_VDEFS (vdefs_after))
    return true;

  return false;
}

/* Add all the non-SSA variables found in STMT's operands to the bitmap
   VARS_TO_RENAME.  */

void
mark_new_vars_to_rename (tree stmt, bitmap vars_to_rename)
{
  def_optype defs;
  use_optype uses;
  vdef_optype vdefs;
  vuse_optype vuses;
  size_t i;
  bitmap vars_in_vops_to_rename;
  bool found_exposed_symbol = false;
  vdef_optype vdefs_before, vdefs_after;
  stmt_ann_t ann;

  vars_in_vops_to_rename = BITMAP_XMALLOC ();

  /* Before re-scanning the statement for operands, mark the existing
     virtual operands to be renamed again.  We do this because when new
     symbols are exposed, the virtual operands that were here before due to
     aliasing will probably be removed by the call to get_stmt_operand.
     Therefore, we need to flag them to be renamed beforehand.

     We flag them in a separate bitmap because we don't really want to
     rename them if there are not any newly exposed symbols in the
     statement operands.  */
  ann = stmt_ann (stmt);
  vdefs_before = vdefs = VDEF_OPS (ann);
  for (i = 0; i < NUM_VDEFS (vdefs); i++)
    {
      tree var = VDEF_RESULT (vdefs, i);
      if (!DECL_P (var))
	var = SSA_NAME_VAR (var);
      bitmap_set_bit (vars_in_vops_to_rename, var_ann (var)->uid);
    }

  vuses = VUSE_OPS (ann);
  for (i = 0; i < NUM_VUSES (vuses); i++)
    {
      tree var = VUSE_OP (vuses, i);
      if (!DECL_P (var))
	var = SSA_NAME_VAR (var);
      bitmap_set_bit (vars_in_vops_to_rename, var_ann (var)->uid);
    }

  /* Now force an operand re-scan on the statement and mark any newly
     exposed variables.  */
  modify_stmt (stmt);
  get_stmt_operands (stmt);

  defs = DEF_OPS (ann);
  for (i = 0; i < NUM_DEFS (defs); i++)
    {
      tree var = DEF_OP (defs, i);
      if (DECL_P (var))
	{
	  found_exposed_symbol = true;
	  bitmap_set_bit (vars_to_rename, var_ann (var)->uid);
	}
    }

  uses = USE_OPS (ann);
  for (i = 0; i < NUM_USES (uses); i++)
    {
      tree var = USE_OP (uses, i);
      if (DECL_P (var))
	{
	  found_exposed_symbol = true;
	  bitmap_set_bit (vars_to_rename, var_ann (var)->uid);
	}
    }

  vdefs_after = vdefs = VDEF_OPS (ann);
  for (i = 0; i < NUM_VDEFS (vdefs); i++)
    {
      tree var = VDEF_RESULT (vdefs, i);
      if (DECL_P (var))
	{
	  found_exposed_symbol = true;
	  bitmap_set_bit (vars_to_rename, var_ann (var)->uid);
	}
    }

  vuses = VUSE_OPS (ann);
  for (i = 0; i < NUM_VUSES (vuses); i++)
    {
      tree var = VUSE_OP (vuses, i);
      if (DECL_P (var))
	{
	  found_exposed_symbol = true;
	  bitmap_set_bit (vars_to_rename, var_ann (var)->uid);
	}
    }

  /* If we found any newly exposed symbols, or if there are fewer VDEF
     operands in the statement, add the variables we had set in
     VARS_IN_VOPS_TO_RENAME to VARS_TO_RENAME.  We need to check for
     vanishing VDEFs because in those cases, the names that were formerly
     generated by this statement are not going to be available anymore.  */
  if (found_exposed_symbol
      || vdefs_disappeared_p (vdefs_before, vdefs_after))
    bitmap_a_or_b (vars_to_rename, vars_to_rename, vars_in_vops_to_rename);

  BITMAP_XFREE (vars_in_vops_to_rename);
}

/* Helper function for discover_nonconstant_array_refs. 
   Look for variables inside ARRAY_REF with non-constant operand
   and mark them as addressable.  */
static tree
discover_nonconstant_array_refs_r (tree * tp, int *walk_subtrees,
				   void *data ATTRIBUTE_UNUSED)
{
  tree t = *tp;
  if (TYPE_P (t) || DECL_P (t))
    *walk_subtrees = 0;
  else if (TREE_CODE (t) == ARRAY_REF)
    {
      while ((TREE_CODE (t) == ARRAY_REF
	      && is_gimple_min_invariant (TREE_OPERAND (t, 1)))
	     || (TREE_CODE (t) == COMPONENT_REF
		 || TREE_CODE (t) == REALPART_EXPR
		 || TREE_CODE (t) == IMAGPART_EXPR))
	t = TREE_OPERAND (t, 0);
      if (TREE_CODE (t) == ARRAY_REF)
	{
	  t = get_base_symbol (t);
	  if (t && DECL_P (t))
	    TREE_ADDRESSABLE (t) = 1;
	}
      *walk_subtrees = 0;
    }
  return NULL_TREE;
}

/* RTL expansion code is not able to compile array references with variable
   offsets for arrays stored in single register.
   Discover such expressions and mark variables as addressable to avoid
   this scenario.  */

void
discover_nonconstant_array_refs (void)
{
  basic_block bb;
  block_stmt_iterator bsi;

  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	walk_tree (bsi_stmt_ptr (bsi), discover_nonconstant_array_refs_r,
		   NULL , NULL);
    }
}

#include "gt-tree-dfa.h"
