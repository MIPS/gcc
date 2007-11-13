/* Generic routines for manipulating SSA_NAME expressions
   Copyright (C) 2003, 2004, 2005, 2007 Free Software Foundation, Inc.
                                                                               
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
#include "tree.h"
#include "varray.h"
#include "ggc.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "diagnostic.h"

/* Rewriting a function into SSA form can create a huge number of SSA_NAMEs,
   many of which may be thrown away shortly after their creation if jumps
   were threaded through PHI nodes.  

   While our garbage collection mechanisms will handle this situation, it
   is extremely wasteful to create nodes and throw them away, especially
   when the nodes can be reused.

   For PR 8361, we can significantly reduce the number of nodes allocated
   and thus the total amount of memory allocated by managing SSA_NAMEs a
   little.  This additionally helps reduce the amount of work done by the
   garbage collector.  Similar results have been seen on a wider variety
   of tests (such as the compiler itself).

   Right now we maintain our free list on a per-function basis.  It may
   or may not make sense to maintain the free list for the duration of
   a compilation unit. 

   External code should rely solely upon HIGHEST_SSA_VERSION and the
   externally defined functions.  External code should not know about
   the details of the free list management.

   External code should also not assume the version number on nodes is
   monotonically increasing.  We reuse the version number when we
   reuse an SSA_NAME expression.  This helps keep arrays and bitmaps
   more compact.

   We could also use a zone allocator for these objects since they have
   a very well defined lifetime.  If someone wants to experiment with that
   this is the place to try it.  */

/* Version numbers with special meanings.  We start allocating new version
   numbers after the special ones.  */
#define UNUSED_NAME_VERSION 0

#ifdef GATHER_STATISTICS
unsigned int ssa_name_nodes_reused;
unsigned int ssa_name_nodes_created;
#endif

static void ssa_varmap_release (tree var);

/* Initialize management of SSA_NAMEs.  */

void
init_ssanames (void)
{
  SSANAMES (cfun) = VEC_alloc (tree, gc, 50);

  /* Version 0 is special, so reserve the first slot in the table.  Though
     currently unused, we may use version 0 in alias analysis as part of
     the heuristics used to group aliases when the alias sets are too
     large.

     We use VEC_quick_push here because we know that SSA_NAMES has at
     least 50 elements reserved in it.  */
  VEC_quick_push (tree, SSANAMES (cfun), NULL_TREE);
  FREE_SSANAMES (cfun) = NULL;
}

/* Finalize management of SSA_NAMEs.  */

void
fini_ssanames (void)
{
  VEC_free (tree, gc, SSANAMES (cfun));
  FREE_SSANAMES (cfun) = NULL;
}

/* Dump some simple statistics regarding the re-use of SSA_NAME nodes.  */

#ifdef GATHER_STATISTICS
void
ssanames_print_statistics (void)
{
  fprintf (stderr, "SSA_NAME nodes allocated: %u\n", ssa_name_nodes_created);
  fprintf (stderr, "SSA_NAME nodes reused: %u\n", ssa_name_nodes_reused);
}
#endif

/* Return an SSA_NAME node for variable VAR defined in statement STMT.
   STMT may be an empty statement for artificial references (e.g., default
   definitions created when a variable is used without a preceding
   definition).  */

tree
make_ssa_name (tree var, tree stmt)
{
  tree t;
  use_operand_p imm;

  gcc_assert (DECL_P (var)
	      || TREE_CODE (var) == INDIRECT_REF);

  gcc_assert (!stmt
	      || EXPR_P (stmt) || GIMPLE_STMT_P (stmt)
	      || TREE_CODE (stmt) == PHI_NODE);

  /* If our free list has an element, then use it.  */
  if (FREE_SSANAMES (cfun))
    {
      t = FREE_SSANAMES (cfun);
      FREE_SSANAMES (cfun) = TREE_CHAIN (FREE_SSANAMES (cfun));
#ifdef GATHER_STATISTICS
      ssa_name_nodes_reused++;
#endif

      /* The node was cleared out when we put it on the free list, so
	 there is no need to do so again here.  */
      gcc_assert (ssa_name (SSA_NAME_VERSION (t)) == NULL);
      VEC_replace (tree, SSANAMES (cfun), SSA_NAME_VERSION (t), t);
    }
  else
    {
      t = make_node (SSA_NAME);
      SSA_NAME_VERSION (t) = num_ssa_names;
      VEC_safe_push (tree, gc, SSANAMES (cfun), t);
#ifdef GATHER_STATISTICS
      ssa_name_nodes_created++;
#endif
    }

  TREE_TYPE (t) = TREE_TYPE (var);
  SSA_NAME_VAR (t) = var;
  SSA_NAME_DEF_STMT (t) = stmt;
  SSA_NAME_PTR_INFO (t) = NULL;
  SSA_NAME_IN_FREE_LIST (t) = 0;
  SSA_NAME_IS_DEFAULT_DEF (t) = 0;
  imm = &(SSA_NAME_IMM_USE_NODE (t));
  imm->use = NULL;
  imm->prev = imm;
  imm->next = imm;
  imm->stmt = t;

  return t;
}


/* We no longer need the SSA_NAME expression VAR, release it so that
   it may be reused. 

   Note it is assumed that no calls to make_ssa_name will be made
   until all uses of the ssa name are released and that the only
   use of the SSA_NAME expression is to check its SSA_NAME_VAR.  All
   other fields must be assumed clobbered.  */

void
release_ssa_name (tree var)
{
  if (!var)
    return;

  /* Remove variable mapping for this name.  */
  ssa_varmap_release (var);

  /* Never release the default definition for a symbol.  It's a
     special SSA name that should always exist once it's created.  */
  if (SSA_NAME_IS_DEFAULT_DEF (var))
    return;

  /* If VAR has been registered for SSA updating, don't remove it.
     After update_ssa has run, the name will be released.  */
  if (name_registered_for_update_p (var))
    {
      release_ssa_name_after_update_ssa (var);
      return;
    }

  /* release_ssa_name can be called multiple times on a single SSA_NAME.
     However, it should only end up on our free list one time.   We
     keep a status bit in the SSA_NAME node itself to indicate it has
     been put on the free list. 

     Note that once on the freelist you can not reference the SSA_NAME's
     defining statement.  */
  if (! SSA_NAME_IN_FREE_LIST (var))
    {
      tree saved_ssa_name_var = SSA_NAME_VAR (var);
      int saved_ssa_name_version = SSA_NAME_VERSION (var);
      use_operand_p imm = &(SSA_NAME_IMM_USE_NODE (var));

#ifdef ENABLE_CHECKING
      verify_imm_links (stderr, var);
#endif
      while (imm->next != imm)
	delink_imm_use (imm->next);

      VEC_replace (tree, SSANAMES (cfun),
		   SSA_NAME_VERSION (var), NULL_TREE);
      memset (var, 0, tree_size (var));

      imm->prev = imm;
      imm->next = imm;
      imm->stmt = var;
      /* First put back the right tree node so that the tree checking
	 macros do not complain.  */
      TREE_SET_CODE (var, SSA_NAME);

      /* Restore the version number.  */
      SSA_NAME_VERSION (var) = saved_ssa_name_version;

      /* Hopefully this can go away once we have the new incremental
         SSA updating code installed.  */
      SSA_NAME_VAR (var) = saved_ssa_name_var;

      /* Note this SSA_NAME is now in the first list.  */
      SSA_NAME_IN_FREE_LIST (var) = 1;

      /* And finally link it into the free list.  */
      TREE_CHAIN (var) = FREE_SSANAMES (cfun);
      FREE_SSANAMES (cfun) = var;
    }
}

/* Creates a duplicate of a ssa name NAME defined in statement STMT.  */

tree
duplicate_ssa_name (tree name, tree stmt)
{
  tree new_name = make_ssa_name (SSA_NAME_VAR (name), stmt);
  struct ptr_info_def *old_ptr_info = SSA_NAME_PTR_INFO (name);

  if (old_ptr_info)
    duplicate_ssa_name_ptr_info (new_name, old_ptr_info);

  return new_name;
}


/* Creates a duplicate of the ptr_info_def at PTR_INFO for use by
   the SSA name NAME.  */

void
duplicate_ssa_name_ptr_info (tree name, struct ptr_info_def *ptr_info)
{
  struct ptr_info_def *new_ptr_info;

  gcc_assert (POINTER_TYPE_P (TREE_TYPE (name)));
  gcc_assert (!SSA_NAME_PTR_INFO (name));

  if (!ptr_info)
    return;

  new_ptr_info = GGC_NEW (struct ptr_info_def);
  *new_ptr_info = *ptr_info;

  if (ptr_info->pt_vars)
    {
      new_ptr_info->pt_vars = BITMAP_GGC_ALLOC ();
      bitmap_copy (new_ptr_info->pt_vars, ptr_info->pt_vars);
    }

  SSA_NAME_PTR_INFO (name) = new_ptr_info;
}


/* Release all the SSA_NAMEs created by STMT.  */

void
release_defs (tree stmt)
{
  tree def;
  ssa_op_iter iter;

  /* Make sure that we are in SSA.  Otherwise, operand cache may point
     to garbage.  */
  gcc_assert (gimple_in_ssa_p (cfun));

  /* If this was a copy stmt, save what the lhs knows about variables.  */
  if (TREE_CODE (stmt) == GIMPLE_MODIFY_STMT
      && TREE_CODE (GIMPLE_STMT_OPERAND (stmt, 0)) == SSA_NAME
      && TREE_CODE (GIMPLE_STMT_OPERAND (stmt, 1)) == SSA_NAME)
    ssa_varmap_process_copy (stmt);

  FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_ALL_DEFS)
    if (TREE_CODE (def) == SSA_NAME)
      release_ssa_name (def);
}


/* Replace the symbol associated with SSA_NAME with SYM.  */

void
replace_ssa_name_symbol (tree ssa_name, tree sym)
{
  SSA_NAME_VAR (ssa_name) = sym;
  TREE_TYPE (ssa_name) = TREE_TYPE (sym);
}

bitmap
ssa_varmap_lookup (tree name)
{
  struct uid_bitmap_map x;
  struct uid_bitmap_map **loc;

  x.uid = SSA_NAME_VERSION (name);
  loc = (struct uid_bitmap_map **)
	htab_find_slot_with_hash (cfun->varmap_hash, &x,
 				  SSA_NAME_VERSION (name), NO_INSERT);
  if (!loc)
    return NULL;
  return (*loc)->map;
}

static void
ssa_varmap_set (tree name, bitmap map)
{
  struct uid_bitmap_map x;
  struct uid_bitmap_map **loc;

  x.uid = SSA_NAME_VERSION (name);
  loc = (struct uid_bitmap_map **)
	htab_find_slot_with_hash (cfun->varmap_hash, &x,
 				  SSA_NAME_VERSION (name), INSERT);
  if (!*loc)
    {
      *loc = GGC_NEW (struct uid_bitmap_map);
      (*loc)->uid = SSA_NAME_VERSION (name);
    }
  (*loc)->map = map;
}

static void
ssa_varmap_replace (tree name, bitmap map)
{
  struct uid_bitmap_map x;
  struct uid_bitmap_map **loc;

  x.uid = SSA_NAME_VERSION (name);
  loc = (struct uid_bitmap_map **)
	htab_find_slot_with_hash (cfun->varmap_hash, &x,
 				  SSA_NAME_VERSION (name), INSERT);
  if (!*loc)
    {
      *loc = GGC_NEW (struct uid_bitmap_map);
      (*loc)->uid = SSA_NAME_VERSION (name);
      (*loc)->map = BITMAP_GGC_ALLOC ();
    }
  bitmap_copy ((*loc)->map, map);
}

/* Releases the variable mapping information for SSA_NAME var.  */

static void
ssa_varmap_release (tree var)
{
  struct uid_bitmap_map x;
  struct uid_bitmap_map **loc;

  /* After going out of SSA we don't care for released SSA_NAMES.  */
  if (!cfun->varmap_hash)
    return;

  x.uid = SSA_NAME_VERSION (var);
  loc = (struct uid_bitmap_map **)
	htab_find_slot_with_hash (cfun->varmap_hash, &x,
 				  SSA_NAME_VERSION (var), NO_INSERT);
  if (loc)
    htab_remove_elt_with_hash (cfun->varmap_hash, &x, SSA_NAME_VERSION (var));
}

/* Reference the variable VAR used in variable mappings.
   ???  Do this only if creating debug information.  */

void
ssa_varmap_add_ref (tree var)
{
  DECL_NOGC_P (var) = true;
}

/* Update the two SSA_NAMEs variable map appearing in the copy
   relation LHS = RHS.  */

static void
ssa_varmap_process_copy_1 (tree lhs, tree rhs)
{
  bitmap lhs_vars, rhs_vars;

  if (dump_file && dump_flags & TDF_VARS)
    {
      fprintf (dump_file, "Processing copy relation ");
      print_generic_expr (dump_file, lhs, dump_flags);
      fprintf (dump_file, " = ");
      print_generic_expr (dump_file, rhs, dump_flags);
      fprintf (dump_file, "\n");
    }

  /* We cannot blindly exchange an artificial vars ssa_name variable
     with another as this may create overlapping life-ranges.  So we
     also cannot assume that artificial vars do not have a variable map.  */

  /* Instead we merge variable maps if available or add the variable
     uids of non-artificial variables to the maps.  */
  lhs_vars = ssa_varmap_lookup (lhs);
  rhs_vars = ssa_varmap_lookup (rhs);
  if (lhs_vars && rhs_vars
      && lhs_vars != rhs_vars)
    {
      /* We IOR both ways to reach shared bitmaps on both sides of
	 the copy relation.  There's no point in changing what SSA_NAME
	 references what bitmap though.  */
      bitmap_ior_into (lhs_vars, rhs_vars);
      bitmap_ior_into (rhs_vars, lhs_vars);
    }
  else if (lhs_vars && !rhs_vars)
    {
      if (!DECL_ARTIFICIAL (SSA_NAME_VAR (rhs)))
	bitmap_set_bit (lhs_vars, DECL_UID (SSA_NAME_VAR (rhs)));
      ssa_varmap_set (rhs, lhs_vars);
    }
  else if (!lhs_vars && rhs_vars)
    {
      if (!DECL_ARTIFICIAL (SSA_NAME_VAR (lhs)))
        bitmap_set_bit (rhs_vars, DECL_UID (SSA_NAME_VAR (lhs)));
      ssa_varmap_set (lhs, rhs_vars);
    }
  else if (!lhs_vars && !rhs_vars
	   && SSA_NAME_VAR (lhs) != SSA_NAME_VAR (rhs)
	   && (!DECL_ARTIFICIAL (SSA_NAME_VAR (rhs))
	       || !DECL_ARTIFICIAL (SSA_NAME_VAR (lhs))))
    {
      bitmap tmp = BITMAP_GGC_ALLOC ();
      if (!DECL_ARTIFICIAL (SSA_NAME_VAR (lhs)))
        bitmap_set_bit (tmp, DECL_UID (SSA_NAME_VAR (lhs)));
      if (!DECL_ARTIFICIAL (SSA_NAME_VAR (rhs)))
        bitmap_set_bit (tmp, DECL_UID (SSA_NAME_VAR (rhs)));
      ssa_varmap_set (lhs, tmp);
      ssa_varmap_set (rhs, tmp);
    }
}

/* Update the two SSA_NAMEs variable map appearing in the copy
   relation STMT.  */

void
ssa_varmap_process_copy (tree stmt)
{
  gcc_assert (gimple_in_ssa_p (cfun)
	      && TREE_CODE (stmt) == GIMPLE_MODIFY_STMT);

  ssa_varmap_process_copy_1 (GIMPLE_STMT_OPERAND (stmt, 0),
			     GIMPLE_STMT_OPERAND (stmt, 1));
}

/* Add a single variable VAR to the mapping of SSA_NAME NAME.  */

void
ssa_varmap_add_var (tree name, tree var)
{
  bitmap lhs_vars;

  if (dump_file && dump_flags & TDF_VARS)
    {
      fprintf (dump_file, "Adding variable ");
      print_generic_expr (dump_file, var, dump_flags);
      fprintf (dump_file, " to SSA_NAME ");
      print_generic_expr (dump_file, name, dump_flags);
      fprintf (dump_file, " map\n");
    }

  if (DECL_ARTIFICIAL (var))
    return;

  lhs_vars = ssa_varmap_lookup (name);
  if (lhs_vars)
    bitmap_set_bit (lhs_vars, DECL_UID (var));
  else if (SSA_NAME_VAR (name) != var)
    {
      bitmap tmp = BITMAP_GGC_ALLOC ();
      if (!DECL_ARTIFICIAL (SSA_NAME_VAR (name)))
        bitmap_set_bit (tmp, DECL_UID (SSA_NAME_VAR (name)));
      bitmap_set_bit (tmp, DECL_UID (var));
      ssa_varmap_set (name, tmp);
    }

  ssa_varmap_add_ref (var);
}

/* Update the PHI_RESULTs variable map of STMT based on the PHI arguments.  */

void
ssa_varmap_process_phi (tree stmt)
{
  tree lhs;
  bitmap arg_vars;
  bitmap tmp;
  ssa_op_iter i;
  use_operand_p use;
  bool first_p;

  gcc_assert (gimple_in_ssa_p (cfun)
	      && TREE_CODE (stmt) == PHI_NODE);

  if (MTAG_P (SSA_NAME_VAR (PHI_RESULT (stmt))))
    return;

  if (dump_file && dump_flags & TDF_VARS)
    {
      fprintf (dump_file, "Processing merge relation ");
      print_generic_expr (dump_file, stmt, dump_flags);
      fprintf (dump_file, "\n");
    }

  /* A PHI nodes result represents the intersection of all decls of
     the PHI arguments.  */
  tmp = BITMAP_GGC_ALLOC ();
  first_p = true;
  FOR_EACH_PHI_ARG (use, stmt, i, SSA_OP_USE)
    {
      tree op = USE_FROM_PTR (use);

      /* Just ignore constants.  */
      if (!op
	  || is_gimple_min_invariant (op))
	continue;

      /* Look up vars for the argument.  If it is the first variable
	 we visit, initialize the temporary map with its map.  */
      arg_vars = ssa_varmap_lookup (op);
      if (arg_vars)
        {
	  if (first_p)
	    bitmap_copy (tmp, arg_vars);
	  else
	    bitmap_and_into (tmp, arg_vars);
	}
      else
	{
	  if (first_p
	      && !DECL_ARTIFICIAL (SSA_NAME_VAR (op)))
	    bitmap_set_bit (tmp, DECL_UID (SSA_NAME_VAR (op)));
	  else if (!bitmap_bit_p (tmp, DECL_UID (SSA_NAME_VAR (op))))
	    {
	      bitmap_clear (tmp);
	      break;
	    }
	}

      if (bitmap_empty_p (tmp))
        break;
      first_p = false;
    }

  /* Clearing the PHI results bit first makes the following easier.  */
  lhs = PHI_RESULT (stmt);
  bitmap_clear_bit (tmp, DECL_UID (SSA_NAME_VAR (lhs)));

  /* The result is now the argument intersection.  If that is empty
     there is nothing to do.  */
  if (bitmap_empty_p (tmp))
    return;

  /* Otherwise re-add the UID from the PHI result var and replace the
     map.  Note this by purpose affects all shared copies.  */
  if (!DECL_ARTIFICIAL (SSA_NAME_VAR (lhs)))
    bitmap_set_bit (tmp, DECL_UID (SSA_NAME_VAR (lhs)));
  ssa_varmap_replace (lhs, tmp);
}

/* Find the variable bitmap associated with EXPR.  Returns NULL if there
   is no such association.  */

bitmap
ssa_varmap_exprmap_lookup (tree expr)
{
  struct tree_bitmap_map id;
  struct tree_bitmap_map *h;

  if (!cfun
      /* We might never have gone into and out of SSA.  */
      || !cfun->varmap_exprmap)
    return NULL;

  id.base.from = expr;
  h = (struct tree_bitmap_map *)
      htab_find_with_hash (cfun->varmap_exprmap, &id, htab_hash_pointer (expr));
  if (h)
    return h->map;
  return NULL;
}

/* Insert a mapping from EXPR to VARS.  */

void
ssa_varmap_exprmap_insert (tree expr, bitmap vars)
{
  struct tree_bitmap_map id;
  struct tree_bitmap_map **loc;

  id.base.from = expr;
  loc = (struct tree_bitmap_map **)
	htab_find_slot_with_hash (cfun->varmap_exprmap, &id,
				  htab_hash_pointer (expr), INSERT);
  if (*loc)
    return;
  *loc = GGC_NEW (struct tree_bitmap_map);
  (*loc)->base.from = expr;
  (*loc)->map = vars;
}

/* Merge vars from SSA_NAME NAME to EXPR.  */

void
ssa_varmap_exprmap_merge_name (tree expr, tree name)
{
  bitmap name_vars = ssa_varmap_lookup (name);
  bitmap expr_vars = ssa_varmap_exprmap_lookup (expr);
  tree var = SSA_NAME_VAR (name);

  if (expr_vars && name_vars)
    bitmap_ior_into (expr_vars, name_vars);
  else if (expr_vars && !name_vars)
    {
      if (!DECL_ARTIFICIAL (var))
	bitmap_set_bit (expr_vars, DECL_UID (var));
    }
  else if (!expr_vars && name_vars)
    {
      expr_vars = BITMAP_GGC_ALLOC ();
      bitmap_copy (expr_vars, name_vars);
      ssa_varmap_exprmap_insert (expr, expr_vars);
    }
  else if (expr != var
	   && !DECL_ARTIFICIAL (var))
    {
      expr_vars = BITMAP_GGC_ALLOC ();
      bitmap_set_bit (expr_vars, DECL_UID (var));
      ssa_varmap_exprmap_insert (expr, expr_vars);
    }
}

/* Build a map from expressions that define ssa names *VALUES to
   the variable bitmap.  */

void
ssa_varmap_build_exprmap (tree *values)
{
  int i;

  for (i = 0; i < (int)num_ssa_names; ++i)
    if (values[i])
      {
	tree ssa_name = GIMPLE_STMT_OPERAND (values[i], 0);
	tree expr = GIMPLE_STMT_OPERAND (values[i], 1);
	bitmap map = ssa_varmap_lookup (ssa_name);
	if (!map)
	  continue;

	ssa_varmap_exprmap_insert (expr, map);

	/* Hold on the variable name.
	   ???  Maybe as a fallback we could also reference the
	   first bit set in the map.  */
	if (!DECL_ARTIFICIAL (SSA_NAME_VAR (ssa_name)))
	  ssa_varmap_add_ref (SSA_NAME_VAR (ssa_name));
      }
}

/* Print variable maps to FILE.  */

void
print_ssa_varmap (FILE *file)
{
  int live_ssa_names = 0, with_map = 0, max_vars = 0, total_vars = 0;
  unsigned int i;

  fprintf (file, "\nSSA variable maps:\n");
  for (i = 0; i < num_ssa_names; ++i)
    {
      tree name = ssa_name (i);
      int n = 0;
      bitmap_iterator bi;
      unsigned int i;
      bitmap vars;
      if (!name)
        continue;
      live_ssa_names++;
      vars = ssa_varmap_lookup (name);
      if (!vars)
        continue;
      with_map++;
      fprintf (file, "  ");
      print_generic_expr (file, name, dump_flags);
      fprintf (file, ": ");
      EXECUTE_IF_SET_IN_BITMAP (vars, 0, i, bi)
	{
	  tree var = ssa_varmap_get_ref (i);
	  n++;
	  if (var)
	    print_generic_expr (file, var, dump_flags);
	  else
	    fprintf (file, "%u", i);
	  fprintf (file, " ");
	}
      total_vars += n;
      if (n > max_vars)
	max_vars = n;
      fprintf (file, "\n");
    }
  fprintf (file, "%d SSA_NAMEs, %d with variable map\n"
	   "with average %.3f variables (%d max)\n\n", live_ssa_names,
	   with_map, with_map > 0 ? total_vars / (float)with_map : 0, max_vars);
}

/* Return SSA names that are unused to GGC memory.  This is used to keep
   footprint of compiler during interprocedural optimization.
   As a side effect the SSA_NAME_VERSION number reuse is reduced
   so this function should not be used too often.  */
static unsigned int
release_dead_ssa_names (void)
{
  tree t, next;
  int n = 0;
  referenced_var_iterator rvi;

  /* Current defs point to various dead SSA names that in turn points to dead
     statements so bunch of dead memory is held from releasing.  */
  FOR_EACH_REFERENCED_VAR (t, rvi)
    set_current_def (t, NULL);
  /* Now release the freelist.  */
  for (t = FREE_SSANAMES (cfun); t; t = next)
    {
      next = TREE_CHAIN (t);
      /* Dangling pointers might make GGC to still see dead SSA names, so it is
 	 important to unlink the list and avoid GGC from seeing all subsequent
	 SSA names.  In longer run we want to have all dangling pointers here
	 removed (since they usually go through dead statements that consume
	 considerable amounts of memory).  */
      TREE_CHAIN (t) = NULL_TREE;
      n++;
    }
  FREE_SSANAMES (cfun) = NULL;

  /* Cgraph edges has been invalidated and point to dead statement.  We need to
     remove them now and will rebuild it before next IPA pass.  */
  cgraph_node_remove_callees (cgraph_node (current_function_decl));

  if (dump_file)
    fprintf (dump_file, "Released %i names, %.2f%%\n", n, n * 100.0 / num_ssa_names);
  return 0;
}

struct tree_opt_pass pass_release_ssa_names =
{
  "release_ssa",			/* name */
  NULL,					/* gate */
  release_dead_ssa_names,		/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_ssa,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0,					/* todo_flags_finish */
  0					/* letter */
};
