/* Export of alias/data dependency information to RTL.  
   Copyright (C) 2009 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tm_p.h"
#include "basic-block.h"
#include "timevar.h"
#include "diagnostic.h"
#include "ggc.h"
#include "output.h"
#include "function.h"
#include "tree-dump.h"
#include "gimple.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "tree-ssa-alias.h"
#include "vec.h"
#include "bitmap.h"
#include "alias-export.h"
#include "rtl.h"
#include "pointer-set.h"
#include "dbgcnt.h"
#include "cfgloop.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "statistics.h"

/* The final orig_expr -> pt set map used in RTL.  */
static struct pointer_map_t *exprs_to_ptas = NULL;

/* The map of decls to stack partitions.  */
static struct pointer_map_t *decls_to_stack = NULL;

/* The map of partition representative decls to bitmaps that are
   unified points-to sets for pointer decls.  */
static struct pointer_map_t *part_repr_to_pta = NULL;

/* The map of partition representative decls to bitmaps that have
   set bits corresponding to decl uids of a partition.  */
static struct pointer_map_t *part_repr_to_part = NULL;

/* Saved escaped points-to solution.  */
static struct pt_solution gimple_df_escaped;

/* Alias export statistics counter.  */
static unsigned int alias_export_disambiguations = 0;


/* This structure holds exported data references and relations.  */
struct ddg_info_def
{

  /* A hashtable for tree -> dataref mapping.  */
  htab_t tree_to_dataref;

  /* A hashtable for mapping dataref pairs to data dependence relation.  */
  htab_t datarefs_pair_to_ddr;

  /* TRUE for passes that perform code motion across loop branches, like SMS.
     For other passes we assume it is safe to disambiguate references that are
     dependent and distance vectors are known and non-zero.  */
  bool pipelining_completed;
};

/* A struct holding dataref information for a given tree.  */
typedef struct {
  const_tree ref;
  data_reference_p drf;
} tree_dataref;

/* A struct for holding data dependence relations for datarefs pairs.  */
typedef struct {
  data_reference_p a;
  data_reference_p b;
  ddr_p ddr;
} datarefs_pair_ddr;

/* Data references and data dependence relations exported from Tree-SSA
   level for use on RTL level.  */
static struct ddg_info_def *ddg_info;

/* DDG export statistics counter.  */
static unsigned int ddg_export_disambiguations = 0;


/* Replace *PBASE with its stack representative in EXPR, if any.  
   Return EXPR back.  */
static tree
maybe_replace_with_partition (tree expr, tree *pbase)
{
  tree *pdecl;
  
  if (!decls_to_stack)
    return expr;
  pdecl = (tree *) pointer_map_contains (decls_to_stack, *pbase);
  if (!pdecl
      || *pdecl == *pbase)
    return expr;
  if (*pbase == expr)
    return *pdecl;
  *pbase = *pdecl;
  return expr;
}

/* Change points-to set for POINTER in PID so that it would
   have all conflicting stack vars.  */
static void
mark_conflict_stack_vars (tree pointer, struct ptr_info_def *pid)
{
  bitmap_iterator bi;
  unsigned i;
  bitmap temp;
  tree *pdecl;

  if (!decls_to_stack)
    return;

  /* If pointer points to one of partition vars, make it point to all 
     of them.  */
  if (pid->pt.vars)
    {
      temp = BITMAP_ALLOC (NULL);
      EXECUTE_IF_SET_IN_BITMAP (pid->pt.vars, 0, i, bi)
        if ((pdecl = (tree *) pointer_map_contains (decls_to_stack,
                                                    referenced_var (i))))
          bitmap_ior_into (temp, 
                           *((bitmap *) pointer_map_contains (part_repr_to_part, 
                                                              *pdecl)));
      bitmap_ior_into (pid->pt.vars, temp);
      BITMAP_FREE (temp);
    }

  /* If pointer got paritioned itself, make its points-to set a union
     of all the partition vars' points-to sets.  */
  if ((pdecl = (tree *) pointer_map_contains (decls_to_stack, pointer)))
    {
      temp = *((bitmap *) pointer_map_contains (part_repr_to_pta, *pdecl));
      bitmap_ior_into (pid->pt.vars, temp);
    }
}

/* Record the final points-to set and returns orig expr.  */
tree
unshare_and_record_pta_info (tree expr)
{
  tree base, old_expr, *pbase;

  /* No point saving anything for unhandled stuff.  */
  if (! (SSA_VAR_P (expr)
         || handled_component_p (expr)
         || INDIRECT_REF_P (expr)))
    return NULL_TREE;

  /* Unshare the tree and do replacement in ddg info.  */
  old_expr = expr;
  expr = unshare_expr (expr);
  if (flag_ddg_export)
    replace_var_in_datarefs (old_expr, expr);
  
  base = expr, pbase = &expr;
  while (handled_component_p (base))
    {
      pbase = &TREE_OPERAND (base, 0);
      base = TREE_OPERAND (base, 0);
    }
  if (DECL_P (base)
      || TREE_CODE (base) == SSA_NAME)
    expr = maybe_replace_with_partition (expr, pbase);
  else if (INDIRECT_REF_P (base))
    {
      struct ptr_info_def **ppid, *pid;

      base = TREE_OPERAND (base, 0);
      if (TREE_CODE (base) == SSA_NAME
          && (pid = SSA_NAME_PTR_INFO (base)) != NULL)
        {
          if (!exprs_to_ptas)
            exprs_to_ptas = pointer_map_create ();
          ppid = (struct ptr_info_def **) pointer_map_insert (exprs_to_ptas,
                                                              expr);
          *ppid = pid; 
          
          mark_conflict_stack_vars (base, pid);
        }
    }
  
  return expr;
}

/* Record the DECL mapping to its PART_DECL representative.  */
static void
map_decl_to_representative (tree decl, tree part_decl)
{
  if (!decls_to_stack)
    decls_to_stack = pointer_map_create ();
  *((tree *) pointer_map_insert (decls_to_stack, decl)) = part_decl;
}

/* Create a bitmap in PMAP associated with DECL and return it.  */
static bitmap
map_decl_to_bitmap (struct pointer_map_t **pmap, tree decl)
{
  bitmap temp = NULL, *ptemp;

  if (!*pmap)
    *pmap = pointer_map_create ();
  ptemp = (bitmap *) pointer_map_contains (*pmap, decl);
  if (ptemp)
    {
      temp = *ptemp;
      gcc_assert (temp);
    }
  else
    {
      temp = BITMAP_ALLOC (NULL);
      *((bitmap *) pointer_map_insert (*pmap, decl)) = temp;
    }

  return temp;
}

/* Save stack partitions.  DECL has PART_DECL as a representative.  
   Also, create pta bitmaps so that alias analysis is not confused later.  */
void
record_stack_var_partition_for (tree decl, tree part_decl)
{
  bitmap temp;
  
  /* First, record that decl has part_decl as a representative.  */
  map_decl_to_representative (decl, part_decl);

  /* Second, create a bitmap that represents all partition.  */
  temp = map_decl_to_bitmap (&part_repr_to_part, part_decl);
  if (DECL_P (part_decl))
    bitmap_set_bit (temp, DECL_UID (part_decl));
  if (DECL_P (decl))
    bitmap_set_bit (temp, DECL_UID (decl));

  /* Third, when decl is a pointer, we need to do the same 
     for points-to sets.  */
  if (TREE_CODE (decl) == SSA_NAME
      && SSA_NAME_PTR_INFO (decl))
    {
      temp = map_decl_to_bitmap (&part_repr_to_pta, part_decl);
      if (TREE_CODE (part_decl) == SSA_NAME
          && SSA_NAME_PTR_INFO (part_decl))
        bitmap_ior_into (temp, SSA_NAME_PTR_INFO (part_decl)->pt.vars);
      bitmap_ior_into (temp, SSA_NAME_PTR_INFO (decl)->pt.vars);
    }
}

/* Return the ptr-info-def structure for given expression.  */
struct ptr_info_def *
get_exported_ptr_info (tree expr)
{
  struct ptr_info_def **ppid;

  if (! exprs_to_ptas)
    return NULL;
  ppid = (struct ptr_info_def **) pointer_map_contains (exprs_to_ptas, expr);
  if (ppid)
    return *ppid;
  return NULL;
}

/* Save the above solution.  */
void
record_escaped_solution (struct pt_solution *escaped)
{
  gimple_df_escaped = *escaped;
  if (escaped->vars)
    {
      gimple_df_escaped.vars = BITMAP_ALLOC (NULL);
      bitmap_copy (gimple_df_escaped.vars, escaped->vars);
    }
}

/* Functions to be called when needed to use exported information.  */

/* Main function to ask saved information about if REF1 and REF2 may
   alias or not.  */
bool
alias_export_may_alias_p (tree ref1, tree ref2)
{
  struct ptr_info_def *pid1, *pid2;
  
  if (! dbg_cnt (alias_export))
    return true;
  
  pid1 = get_exported_ptr_info (ref1);
  pid2 = get_exported_ptr_info (ref2);
  if (! refs_may_alias_p_1 (ref1, ref2, pid1, pid2, &gimple_df_escaped))
    {
      alias_export_disambiguations++;
      return false;
    }

  return true;
}

/* Free exported info.  */
void
free_alias_export_info (void)
{
  statistics_counter_event (cfun, "Alias export disambiguations", 
                            alias_export_disambiguations);
  alias_export_disambiguations = 0;

  if (exprs_to_ptas)
    {
      pointer_map_destroy (exprs_to_ptas);
      exprs_to_ptas = NULL;
    }
  if (decls_to_stack)
    {
      pointer_map_destroy (decls_to_stack);
      decls_to_stack = NULL;
    }
  if (part_repr_to_pta)
    {
      pointer_map_destroy (part_repr_to_pta);
      part_repr_to_pta = NULL;
    }
  if (part_repr_to_part)
    {
      pointer_map_destroy (part_repr_to_part);
      part_repr_to_part = NULL;
    }
  if (gimple_df_escaped.vars)
    {
      BITMAP_FREE (gimple_df_escaped.vars);
      memset (&gimple_df_escaped, 0, sizeof gimple_df_escaped);
    }
}


/* Data dependence export.  */

/* Hash a dataref T.  */
static hashval_t
htab_hash_tree (const tree_dataref *t)
{
  return htab_hash_pointer (t->ref);
}

/* Equality function for struct tree_dataref.  */
static int
htab_eq_tree (const tree_dataref *t1, const tree_dataref *t2)
{
  return t1->ref == t2->ref;
}

/* When freeing hashtable, free the dataref memory.  */
static void
htab_del_tree_dataref (tree_dataref *t)
{
  if (t->drf)
    free_data_ref (t->drf);
}

/* Hash function for the above.  */
static hashval_t
htab_hash_datarefs_pair (const datarefs_pair_ddr *dp)
{
  return iterative_hash (&dp->a, sizeof (data_reference_p),
			 htab_hash_pointer (dp->b));
}

/* Equality function for the above.  */
static int
htab_eq_datarefs_pair (const datarefs_pair_ddr *dp1,
		       const datarefs_pair_ddr *dp2)
{
  return dp1->a == dp2->a && dp1->b == dp2->b;
}

/* Free function for the above.  */
static void
htab_del_datarefs_pair (datarefs_pair_ddr *dp)
{
  free_dependence_relation (dp->ddr);
}

/* Init the ddg_info structure holding the saved data.  */
static void
init_ddg_info (void)
{
  gcc_assert (!ddg_info);

  ddg_info = XCNEW (struct ddg_info_def);
  ddg_info->tree_to_dataref
   = htab_create (1, (htab_hash) htab_hash_tree, (htab_eq) htab_eq_tree,
		  (htab_del) htab_del_tree_dataref);
  ddg_info->datarefs_pair_to_ddr
   = htab_create (1, (htab_hash) htab_hash_datarefs_pair,
		  (htab_eq) htab_eq_datarefs_pair,
		  (htab_del) htab_del_datarefs_pair);
  ddg_info->pipelining_completed = false;
}

/* Save the data reference DRF in the ddg_info structure.  */
static void
record_data_reference (data_reference_p drf)
{
  void **slot;
  tree_dataref *td;

  td = XNEW (tree_dataref);
  td->ref = drf->ref;
  td->drf = drf;

  slot = htab_find_slot (ddg_info->tree_to_dataref, td, INSERT);
  gcc_assert (!*slot);

  *slot = td;
}

/* Save the data relation DDR in the ddg info structure.  */
static void
record_data_dependence_relation (ddr_p ddr)
{
  void **slot;
  datarefs_pair_ddr *dp;

  dp = XNEW (datarefs_pair_ddr);
  dp->a = ddr->a;
  dp->b = ddr->b;
  dp->ddr = ddr;

  slot = htab_find_slot (ddg_info->datarefs_pair_to_ddr, dp, INSERT);
  gcc_assert (!*slot);

  *slot = dp;
}

/* Export ddg info for loop LOOP.  */
static void
record_ddg_info_for (struct loop *loop)
{
  VEC (data_reference_p, heap) *datarefs = NULL;
  VEC (ddr_p, heap) *ddrs = NULL;
  unsigned int i;
  data_reference_p drf;
  ddr_p ddr;

  compute_data_dependences_for_loop (loop, false, &datarefs, &ddrs);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      dump_data_references (dump_file, datarefs);
      dump_ddrs (dump_file, ddrs);
    }

  for (i = 0; VEC_iterate (data_reference_p, datarefs, i, drf); i++)
    {
      /* We want to save only those data references that correspond to
         iteration of innermost loop containing the reference.  */
      if (!drf->ref || loop != loop_containing_stmt (drf->stmt))
        continue;
      record_data_reference (drf);
    }

  for (i = 0; VEC_iterate (ddr_p, ddrs, i, ddr); i++)
    {
      /* As above, we want to save only those DDRs that describe
         relation of references for innermost loop containing them.  */
      if (!(ddr->a && ddr->b)
          || loop != loop_containing_stmt (ddr->a->stmt))
        continue;
      record_data_dependence_relation (ddr);
    }

  VEC_free (data_reference_p, heap, datarefs);
  VEC_free (ddr_p, heap, ddrs);
}

/* For each loop in function, save datarefs and ddrs obtained via
   compute_dependencies_for_loop into ddg_info.  */
static unsigned int
run_ddg_export (void)
{
  bool inside_tree_loop_opt_p = !!current_loops;
  bool dom_info_was_avail_p = dom_info_available_p (CDI_DOMINATORS);
  struct loop *loop;
  loop_iterator li;

  if (!inside_tree_loop_opt_p)
    loop_optimizer_init (AVOID_CFG_MODIFICATIONS);

  /* This can be more than actual number of loops, because number_of_loops ()
     includes deleted loops.  */
  if (number_of_loops () > 1)
    {
      if (!inside_tree_loop_opt_p)
	scev_initialize ();
      init_ddg_info ();

      FOR_EACH_LOOP (li, loop, 0)
        record_ddg_info_for (loop);

      if (!inside_tree_loop_opt_p)
	scev_finalize ();
    }

  if (!inside_tree_loop_opt_p)
    loop_optimizer_finalize ();

  if (!dom_info_was_avail_p)
    free_dominance_info (CDI_DOMINATORS);

  return 0;
}

/* Replace FROM tree to TO in ref fields of saved datarefs.  */
void
replace_var_in_datarefs (tree from, tree to)
{
  void **slot;
  tree_dataref td, *ptd;

  if (!ddg_info)
    return;

  td.ref = from;
  slot = htab_find_slot (ddg_info->tree_to_dataref, &td, NO_INSERT);

  /* IVOPTS might want to change a memory reference for which no dataref was
     produced.  However, it would be nice to enable this assert and see in
     what cases it happens.  */
  /* gcc_assert (slot); */
  if (!slot)
    return;

  ptd = XNEW (tree_dataref);
  ptd->ref = to;
  ptd->drf = ((tree_dataref *) (*slot))->drf;
  ((tree_dataref *) (*slot))->drf = NULL;
  htab_clear_slot (ddg_info->tree_to_dataref, slot);
  
  slot = htab_find_slot (ddg_info->tree_to_dataref, ptd, INSERT);
  gcc_assert (!*slot);
  *slot = ptd;
}

/* Search for the dataref for T and return it if found, otherwise return
   NULL.  */

static data_reference_p
find_dataref (const_tree t)
{
  tree_dataref td, *ptd;

  td.ref = t;
  ptd = (tree_dataref *) htab_find (ddg_info->tree_to_dataref, &td);
  return ptd ? ptd->drf : NULL;
}

/* Search for data dependence relation for DR1 and DR2, return it if found;
   otherwise return NULL.  */
static ddr_p
find_ddr (data_reference_p dr1, data_reference_p dr2)
{
  datarefs_pair_ddr dp, *pdp;

  dp.a = dr1;
  dp.b = dr2;
  pdp = (datarefs_pair_ddr *) htab_find (ddg_info->datarefs_pair_to_ddr, &dp);
  if (pdp)
    return pdp->ddr;

  dp.a = dr2;
  dp.b = dr1;
  pdp = (datarefs_pair_ddr *) htab_find (ddg_info->datarefs_pair_to_ddr, &dp);
  return pdp ? pdp->ddr : NULL;
}

/* A callback for for_each_rtx to remove exported data for mems.  */
static int
walk_mems (rtx *x, void *data ATTRIBUTE_UNUSED)
{
  if (MEM_P (*x) && MEM_ORIG_EXPR (*x) && find_dataref (MEM_ORIG_EXPR (*x)))
    {
      tree_dataref td;

      td.ref = MEM_ORIG_EXPR (*x);
      htab_remove_elt (ddg_info->tree_to_dataref, &td);

      return -1;
    }
  
  return 0;
}

/* Remove saved dependency data for INSN.  Used when INSN is in the loop that
   was unrolled.  */
void
remove_exported_ddg_data (rtx insn)
{
  if (!ddg_info)
    return;

  for_each_rtx (&PATTERN (insn), walk_mems, NULL);
}

/* Set pipelining-completed to B.  */
void
ddg_export_set_pipelining_completed (bool b)
{
  if (ddg_info)
    ddg_info->pipelining_completed = b;
}

/* Return TRUE if any of DIST_VECTS is non-zero.  */
static bool
nonzero_dist_vects (VEC (lambda_vector, heap) *dist_vects, int loops_count)
{
  lambda_vector dist_v;
  int i, j;

  for (i = 0; VEC_iterate (lambda_vector, dist_vects, i, dist_v); i++)
    for (j = 0; j < loops_count; j++)
      if (dist_v[j])
	return true;

  return false;
}

/* Return TRUE if we cannot prove from exported DDG info that MEM1 and MEM2
   are independent memory references.  When FOR_PIPELINING is true, do not
   disambiguate references with non-zero distances.  */
bool
ddg_export_may_alias_p (tree t1, tree t2, bool for_pipelining)
{
  data_reference_p drf1, drf2;
  ddr_p ddr;

  if (! dbg_cnt (ddg_export))
    return true;

  if (!ddg_info || !t1 || !t2)
    return true;

  drf1 = find_dataref (t1);
  drf2 = find_dataref (t2);
  if (!drf1 || !drf2)
    return true;

  ddr = find_ddr (drf1, drf2);
  if (!ddr)
    return true;

  if (DDR_ARE_DEPENDENT (ddr) == chrec_known)
    {
      ddg_export_disambiguations++;
      return false;
    }
  
  if (DDR_ARE_DEPENDENT (ddr) == NULL_TREE 
      && DDR_NUM_DIST_VECTS (ddr) > 0
      && ! for_pipelining
      && ! ddg_info->pipelining_completed
      && nonzero_dist_vects (DDR_DIST_VECTS (ddr), DDR_NB_LOOPS (ddr)))
    {
      ddg_export_disambiguations++;
      return false;
    }
  
  return true;
}

/* Free ddg export info.  */
void
free_ddg_export_info (void)
{
  if (!ddg_info)
    return;

  statistics_counter_event (cfun, "DDG export disambiguations", 
                            ddg_export_disambiguations);
  ddg_export_disambiguations = 0;

  /* TODO: DDR_LOOP_NESTs are not free'd.  */
  htab_delete (ddg_info->datarefs_pair_to_ddr);
  htab_delete (ddg_info->tree_to_dataref);

  free (ddg_info);
  ddg_info = NULL;
}

/* Gate for ddg export pass.  */
static bool
gate_ddg_export (void)
{
  return flag_ddg_export != 0;
}

struct gimple_opt_pass pass_gather_ddg_info =
{
 {
  GIMPLE_PASS,
  "ddg-export",				/* name */
  gate_ddg_export,	                /* gate */
  run_ddg_export,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_NONE,				/* tv_id */
  0,					/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func                        /* todo_flags_finish */
 }
};



