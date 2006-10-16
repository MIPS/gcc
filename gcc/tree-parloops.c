/* Loop autoparallelization.
   Copyright (C) 2006 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <pop@cri.ensmp.fr> and
   Zdenek Dvorak <dvorakz@suse.cz>.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tree-flow.h"
#include "cfgloop.h"
#include "ggc.h"
#include "tree-data-ref.h"
#include "diagnostic.h"
#include "tree-pass.h"
#include "tree-scalar-evolution.h"
#include "hashtab.h"
#include "langhooks.h"

/* This pass tries to distribute iterations of loops into several threads.
   The implementation is straightforward -- for each loop we test whether its
   iterations are independent, and if it is the case (and some additional
   conditions regarding profitability and correctness are satisfied), we
   split the loop to a separate function, and generate code to invoke the loop
   in different threads for different parts of the iteration space;  the most
   complexity is in the code generation part.

   TODO:
   -- if there are several parallelizable loops in a function, it may be
      possible to generate the threads just once (using synchronization to
      ensure that cross-loop dependences are obeyed).
   -- handling of common scalar dependence patterns (accumulation, ...)
   -- handling of non-innermost loops  */

/* Minimal number of iterations of a loop that should be executed in each
   thread.  */
#define MIN_PER_THREAD 100

/* Element of hashtable of names to copy.  */

struct name_to_copy_elt
{
  unsigned version;	/* The version of the name to copy.  */
  tree new_name;	/* The new name used in the copy.  */
  tree field;		/* The field of the structure used to pass the
			   value.  */
};

/* Equality and hash functions for hashtab code.  */

static int
name_to_copy_elt_eq (const void *aa, const void *bb)
{
  struct name_to_copy_elt *a = (struct name_to_copy_elt *) aa;
  struct name_to_copy_elt *b = (struct name_to_copy_elt *) bb;

  return a->version == b->version;
}

static hashval_t
name_to_copy_elt_hash (const void *aa)
{
  struct name_to_copy_elt *a = (struct name_to_copy_elt *) aa;

  return (hashval_t) a->version;
}

/* Returns true if the iterations of LOOP are independent on each other (that
   is, if we can execute them in parallel), and if LOOP satisfies other
   conditions that we need to be able to parallelize it.  Description of number
   of iterations is stored to NITER.  */

static bool
loop_parallel_p (struct loop *loop, struct tree_niter_desc *niter)
{
  edge exit = single_dom_exit (loop);
  VEC (ddr_p, heap) *dependence_relations;
  VEC (data_reference_p, heap) *datarefs;
  lambda_trans_matrix trans;
  bool ret = false;
  tree phi;

  /* Only consider innermost loops with just one exit.  The innermost-loop
     restriction is not necessary, but it makes things simpler.  */
  if (loop->inner || !exit)
    return false;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nConsidering loop %d\n", loop->num);

  /* We need to know # of iterations, and there should be no uses of values
     defined inside loop outside of it, unless the values are invariants of
     the loop.  */
  if (!number_of_iterations_exit (loop, exit, niter, false))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  FAILED: number of iterations not known\n");
      return false;
    }

  for (phi = phi_nodes (exit->dest); phi; phi = PHI_CHAIN (phi))
    {
      tree val = PHI_ARG_DEF_FROM_EDGE (phi, exit);

      if (is_gimple_reg (val)
	  && !expr_invariant_in_loop_p (loop, val))
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "  FAILED: value used outside loop\n");
	  return false;
	}
    }

  /* The iterations of the loop may comunicate only through bivs whose
     iteration space can be distributed efficiently.  */
  for (phi = phi_nodes (loop->header); phi; phi = PHI_CHAIN (phi))
    {
      tree def = PHI_RESULT (phi);
      affine_iv iv;

      if (is_gimple_reg (def)
	  && !simple_iv (loop, phi, def, &iv, true))
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "  FAILED: non-biv phi node\n");
	  return false;
	}
    }

  datarefs = VEC_alloc (data_reference_p, heap, 10);
  dependence_relations = VEC_alloc (ddr_p, heap, 10 * 10);

  /* Check for problems with dependences.  If the loop can be reversed,
     the iterations are indepent.  */
  compute_data_dependences_for_loop (loop, true, &datarefs,
				     &dependence_relations);
  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_data_dependence_relations (dump_file, dependence_relations);

  trans = lambda_trans_matrix_new (1, 1);
  LTM_MATRIX (trans)[0][0] = -1;

  if (lambda_transform_legal_p (trans, 1, dependence_relations))
    {
      ret = true;
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  SUCCESS: may be parallelized\n");
    }
  else if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "  FAILED: dependence check failed\n");

  free_dependence_relations (dependence_relations);
  free_data_refs (datarefs);

  return ret;
}

/* Calls mark_virtual_ops_for_renaming for all members of LIST.  */

static void
mark_virtual_ops_for_renaming_list (tree list)
{
  tree_stmt_iterator tsi;

  for (tsi = tsi_start (list); !tsi_end_p (tsi); tsi_next (&tsi))
    mark_virtual_ops_for_renaming (tsi_stmt (tsi));
}

/* Marks operands of calls for renaming.  */

void
mark_call_virtual_operands (void)
{
  basic_block bb;
  block_stmt_iterator bsi;
  tree stmt;

  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  stmt = bsi_stmt (bsi);
	  if (get_call_expr_in (stmt) != NULL_TREE)
	    mark_new_vars_to_rename (stmt);
	}
    }
}

/* Assigns the address of VAR in TYPE to an ssa name, and returns this name.
   The assignment statement is placed before LOOP.  DECL_ADDRESS maps decls
   to their addresses that can be reused.  */

static tree
take_address_of (tree var, tree type, struct loop *loop, htab_t decl_address)
{
  int uid = DECL_UID (var);
  void **dslot;
  struct int_tree_map ielt, *nielt;
  tree name, bvar, stmt;
  edge entry = loop_preheader_edge (loop);

  ielt.uid = uid;
  dslot = htab_find_slot_with_hash (decl_address, &ielt, uid, INSERT);
  if (!*dslot)
    {
      bvar = create_tmp_var (type, get_name (var));
      add_referenced_var (bvar);
      stmt = build2 (MODIFY_EXPR, void_type_node, bvar,
		     fold_convert (type,
				   build_addr (var, current_function_decl)));
      name = make_ssa_name (bvar, stmt);
      TREE_OPERAND (stmt, 0) = name;
      bsi_insert_on_edge_immediate_loop (entry, stmt);

      nielt = XNEW (struct int_tree_map);
      nielt->uid = uid;
      nielt->to = name;
      *dslot = nielt;

      return name;
    }

  name = ((struct int_tree_map *) *dslot)->to;
  if (TREE_TYPE (name) == type)
    return name;

  bvar = SSA_NAME_VAR (name);
  stmt = build2 (MODIFY_EXPR, void_type_node, bvar,
		 fold_convert (type, name));
  name = make_ssa_name (bvar, stmt);
  TREE_OPERAND (stmt, 0) = name;
  bsi_insert_on_edge_immediate_loop (entry, stmt);

  return name;
}

/* Eliminates references to local variables in *TP from LOOP.  DECL_ADDRESS
   contains addresses for the references for that we have already taken
   them.  If the expression is changed, CHANGED is set to true.  Callback for
   walk_tree.  */

struct elv_data
{
  struct loop *loop;
  htab_t decl_address;
  bool changed;
};

static tree
eliminate_local_variables_1 (tree *tp, int *walk_subtrees, void *data)
{
  struct elv_data *dta = data;
  tree t = *tp, var, addr, addr_type, type;

  if (DECL_P (t))
    {
      *walk_subtrees = 0;

      if (!SSA_VAR_P (t) || DECL_EXTERNAL (t))
	return NULL_TREE;

      type = TREE_TYPE (t);
      addr_type = build_pointer_type (type);
      addr = take_address_of (t, addr_type, dta->loop, dta->decl_address);
      *tp = build1 (INDIRECT_REF, TREE_TYPE (*tp), addr);

      dta->changed = true;
      return NULL_TREE;
    }

  if (TREE_CODE (t) == ADDR_EXPR)
    {
      var = TREE_OPERAND (t, 0);
      if (!DECL_P (var))
	return NULL_TREE;

      *walk_subtrees = 0;
      if (!SSA_VAR_P (var) || DECL_EXTERNAL (var))
	return NULL_TREE;

      addr_type = TREE_TYPE (t);
      addr = take_address_of (var, addr_type, dta->loop, dta->decl_address);
      *tp = addr;

      dta->changed = true;
      return NULL_TREE;
    }

  if (!EXPR_P (t))
    *walk_subtrees = 0;

  return NULL_TREE;
}

/* Moves the references to local variables in STMT from LOOP.  DECL_ADDRESS
   contains addresses for the references for that we have already taken
   them.  */

static void
eliminate_local_variables_stmt (struct loop *loop, tree stmt,
				htab_t decl_address)
{
  struct elv_data dta;

  dta.loop = loop;
  dta.decl_address = decl_address;
  dta.changed = false;

  walk_tree (&stmt, eliminate_local_variables_1, &dta, NULL);

  if (dta.changed)
    update_stmt (stmt);
}

/* Eliminates the references to local variables from LOOP.  This includes:

   1) Taking address of a local variable -- these are moved out of the loop
      (and temporary variable is created to hold the address if necessary).
   2) Dereferencing a local variable -- these are replaced with indirect
      references.  */

static void
eliminate_local_variables (struct loop *loop)
{
  basic_block bb, *body = get_loop_body (loop);
  unsigned i;
  block_stmt_iterator bsi;
  htab_t decl_address = htab_create (10, int_tree_map_hash, int_tree_map_eq,
				     free);

  /* Find and rename the ssa names defined outside of loop.  */
  for (i = 0; i < loop->num_nodes; i++)
    {
      bb = body[i];

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	eliminate_local_variables_stmt (loop, bsi_stmt (bsi), decl_address);
    }

  htab_delete (decl_address);
}

/* If COPY_NAME_P is true, creates and returns a duplicate of NAME.
   The copies are stored to NAME_COPIES, if NAME was already duplicated,
   its duplicate stored in NAME_COPIES is returned.
   
   Regardless of COPY_NAME_P, the decl used as a base of the ssa name is also
   duplicated, storing the copies in DECL_COPIES.  */

static tree
separate_decls_in_loop_name (tree name,
			     htab_t name_copies, htab_t decl_copies,
			     bool copy_name_p)
{
  tree copy, var, var_copy;
  unsigned idx, uid, nuid;
  struct int_tree_map ielt, *nielt;
  struct name_to_copy_elt elt, *nelt;
  void **slot, **dslot;

  idx = SSA_NAME_VERSION (name);
  elt.version = idx;
  slot = htab_find_slot_with_hash (name_copies, &elt, idx,
				   copy_name_p ? INSERT : NO_INSERT);
  if (slot && *slot)
    return ((struct name_to_copy_elt *) *slot)->new_name;

  var = SSA_NAME_VAR (name);
  uid = DECL_UID (var);
  ielt.uid = uid;
  dslot = htab_find_slot_with_hash (decl_copies, &ielt, uid, INSERT);
  if (!*dslot)
    {
      var_copy = create_tmp_var (TREE_TYPE (var), get_name (var));
      add_referenced_var (var_copy);
      nielt = XNEW (struct int_tree_map);
      nielt->uid = uid;
      nielt->to = var_copy;
      *dslot = nielt;

      /* Ensure that when we meet this decl next time, we won't duplicate
	 it again.  */
      nuid = DECL_UID (var_copy);
      ielt.uid = nuid;
      dslot = htab_find_slot_with_hash (decl_copies, &ielt, nuid, INSERT);
      gcc_assert (!*dslot);
      nielt = XNEW (struct int_tree_map);
      nielt->uid = nuid;
      nielt->to = var_copy;
      *dslot = nielt;
    }
  else
    var_copy = ((struct int_tree_map *) *dslot)->to;

  if (copy_name_p)
    {
      copy = duplicate_ssa_name (name, NULL_TREE);
      nelt = XNEW (struct name_to_copy_elt);
      nelt->version = idx;
      nelt->new_name = copy;
      nelt->field = NULL_TREE;
      *slot = nelt;
    }
  else
    {
      gcc_assert (!slot);
      copy = name;
    }

  SSA_NAME_VAR (copy) = var_copy;
  return copy;
}

/* Finds the ssa names used in STMT that are defined outside of LOOP and
   replaces such ssa names with their duplicates.  The duplicates are stored to
   NAME_COPIES.  Base decls of all ssa names used in STMT
   (including those defined in LOOP) are replaced with the new temporary
   variables; the replacement decls are stored in DECL_COPIES.  */

static void
separate_decls_in_loop_stmt (struct loop *loop, tree stmt,
			     htab_t name_copies, htab_t decl_copies)
{
  use_operand_p use;
  def_operand_p def;
  ssa_op_iter oi;
  tree name, copy;
  bool copy_name_p;

  mark_virtual_ops_for_renaming (stmt);

  FOR_EACH_PHI_OR_STMT_DEF (def, stmt, oi, SSA_OP_DEF)
    {
      name = DEF_FROM_PTR (def);
      gcc_assert (TREE_CODE (name) == SSA_NAME);
      copy = separate_decls_in_loop_name (name, name_copies, decl_copies,
					  false);
      gcc_assert (copy == name);
    }

  FOR_EACH_PHI_OR_STMT_USE (use, stmt, oi, SSA_OP_USE)
    {
      name = USE_FROM_PTR (use);
      if (TREE_CODE (name) != SSA_NAME)
	continue;

      copy_name_p = expr_invariant_in_loop_p (loop, name);
      copy = separate_decls_in_loop_name (name, name_copies, decl_copies,
					  copy_name_p);
      SET_USE (use, copy);
    }
}

/* Callback for htab_traverse.  Adds a field corresponding to a ssa name
   described in SLOT to the type passed in DATA.  */

static int
add_field_for_name (void **slot, void *data)
{
  struct name_to_copy_elt *elt = *slot;
  tree type = data;
  tree name = ssa_name (elt->version);
  tree var = SSA_NAME_VAR (name);
  tree field = build_decl (FIELD_DECL, DECL_NAME (var), TREE_TYPE (var));

  insert_field_into_struct (type, field);
  elt->field = field;
  return 1;
}

/* Callback for htab_traverse.  Creates loads to a field of LOAD in LOAD_BB and
   store to a field of STORE in STORE_BB for the ssa name and its duplicate
   specified in SLOT.  */

struct clsn_data
{
  tree store;
  tree load;

  basic_block store_bb;
  basic_block load_bb;
};

static int
create_loads_and_stores_for_name (void **slot, void *data)
{
  struct name_to_copy_elt *elt = *slot;
  struct clsn_data *clsn_data = data;
  tree stmt;
  block_stmt_iterator bsi;
  tree type = TREE_TYPE (elt->new_name);
  tree struct_type = TREE_TYPE (TREE_TYPE (clsn_data->load));
  tree load_struct;

  bsi = bsi_last (clsn_data->store_bb);
  stmt = build2 (MODIFY_EXPR, void_type_node,
		 build3 (COMPONENT_REF, type, clsn_data->store, elt->field,
			 NULL_TREE),
		 ssa_name (elt->version));
  mark_virtual_ops_for_renaming (stmt);
  bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);

  bsi = bsi_last (clsn_data->load_bb);
  load_struct = fold_build1 (INDIRECT_REF, struct_type, clsn_data->load);
  stmt = build2 (MODIFY_EXPR, void_type_node,
		 elt->new_name,
		 build3 (COMPONENT_REF, type, load_struct, elt->field,
			 NULL_TREE));
  SSA_NAME_DEF_STMT (elt->new_name) = stmt;
  bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);

  return 1;
}

/* Moves all the variables used in LOOP and defined outside of it (including
   the initial values of loop phi nodes, and *PER_THREAD if it is a ssa
   name) to a structure created for this purpose.  The code
 
   while (1)
     {
       use (a);
       use (b);
     }

   is transformed this way:

bb0:
   old.a = a;
   old.b = b;

bb1:
   a' = new->a;
   b' = new->b;
   while (1)
     {
       use (a');
       use (b');
     }

   `old' is stored to *ARG_STRUCT and `new' is stored to NEW_ARG_STRUCT.  The
   pointer `new' is intentionally not initialized (the loop will be split to a
   separate function later, and `new' will be initialized from its arguments).
   *PER_THREAD is updated to the ssa name to that the value is loaded in
   bb1.  A mapping of old to new copies is stored to *NAME_COPIES.  */

static void
separate_decls_in_loop (struct loop *loop, tree *per_thread,
			tree *arg_struct, tree *new_arg_struct,
			htab_t *name_copies)
{
  basic_block bb1 = loop_split_edge_with (loop_preheader_edge (loop), NULL);
  basic_block bb0 = single_pred (bb1);
  htab_t decl_copies = htab_create (10, int_tree_map_hash, int_tree_map_eq,
				    free);
  basic_block bb, *body = get_loop_body (loop);
  unsigned i;
  tree phi, type, type_name, nvar;
  block_stmt_iterator bsi;
  struct clsn_data clsn_data;

  *name_copies = htab_create (10, name_to_copy_elt_hash,
			      name_to_copy_elt_eq, free);

  /* Find and rename the ssa names defined outside of loop.  */
  for (i = 0; i < loop->num_nodes; i++)
    {
      bb = body[i];

      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	separate_decls_in_loop_stmt (loop, phi, *name_copies, decl_copies);

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	separate_decls_in_loop_stmt (loop, bsi_stmt (bsi), *name_copies,
				     decl_copies);
    }

  if (TREE_CODE (*per_thread) == SSA_NAME)
    *per_thread
       = separate_decls_in_loop_name (*per_thread, *name_copies, decl_copies, 
				      true);
  free (body);

  if (htab_elements (*name_copies) == 0)
    {
      /* It may happen that there is nothing to copy (if there are only
	 loop carried and external variables in the loop).  */
      *arg_struct = NULL;
      *new_arg_struct = NULL;
    }
  else
    {
      /* Create the type for the structure to store the ssa names to.  */
      type = lang_hooks.types.make_type (RECORD_TYPE);
      type_name = build_decl (TYPE_DECL, create_tmp_var_name (".paral_data"),
			      type);
      TYPE_NAME (type) = type_name;

      htab_traverse (*name_copies, add_field_for_name, type);
      layout_type (type);

      /* Create the loads and stores.  */
      *arg_struct = create_tmp_var (type, ".paral_data_store");
      add_referenced_var(*arg_struct);
      nvar = create_tmp_var (build_pointer_type (type), ".paral_data_load");
      add_referenced_var(nvar);
      *new_arg_struct = make_ssa_name (nvar, NULL_TREE);

      /* We should mark *arg_struct call clobbered.  However, this means
	 we would need to update virtual operands for every function call.
	 To avoid this, we pretend this variable is volatile.  */
      TREE_THIS_VOLATILE (*arg_struct) = 1;

      clsn_data.store = *arg_struct;
      clsn_data.load = *new_arg_struct;
      clsn_data.store_bb = bb0;
      clsn_data.load_bb = bb1;
      htab_traverse (*name_copies, create_loads_and_stores_for_name,
		     &clsn_data);
    }

  htab_delete (decl_copies);
}

/* Replaces initial values of induction variables in LOOP to the start of the
   PER_THREAD * (thread number - 1)-th iteration.  STEPS is the hashtable that
   contains steps of each induction variable.  */

static void
shift_ivs_for_iteration (struct loop *loop, tree per_thread, htab_t steps)
{
  tree phi, name, step, ncopy, stmts, call, cdecl, type, init, delta, ninit;
  edge entry = loop_preheader_edge (loop);
  struct int_tree_map tem, *elt;
  unsigned ver;
  block_stmt_iterator bsi = bsi_last (entry->src);
  use_operand_p init_p;

  for (phi = phi_nodes (loop->header); phi; phi = PHI_CHAIN (phi))
    {
      name = PHI_RESULT (phi);
      if (!is_gimple_reg (name))
	continue;

      ver = SSA_NAME_VERSION (name);
      tem.uid = ver;
      elt = htab_find_with_hash (steps, &tem, ver);
      step = elt->to;

      init_p = PHI_ARG_DEF_PTR_FROM_EDGE (phi, entry);
      init = USE_FROM_PTR (init_p);
      type = TREE_TYPE (init);
      cdecl = built_in_decls[BUILT_IN_OMP_GET_THREAD_NUM];
      call = fold_convert (type, build_function_call_expr (cdecl, NULL));
      ncopy = build2 (MINUS_EXPR, type, call, build_int_cst (type, 1));
      delta = fold_build2 (MULT_EXPR, type, unshare_expr (step), per_thread);
      delta = fold_build2 (MULT_EXPR, type, delta, ncopy);
      ninit = fold_build2 (PLUS_EXPR, type, init, delta);

      ninit = force_gimple_operand (ninit, &stmts, true, NULL_TREE);
      if (stmts)
	bsi_insert_after (&bsi, stmts, BSI_CONTINUE_LINKING);

      SET_USE (init_p, ninit);
    }
}

/* Records steps of induction variables of LOOP to STEPS.  */

static void
record_steps (struct loop *loop, htab_t *steps)
{
  tree phi, name;
  unsigned ver;
  affine_iv iv;
  bool ok;
  void **slot;
  struct int_tree_map tem, *elt;

  *steps = htab_create (10, int_tree_map_hash, int_tree_map_eq, free);
  for (phi = phi_nodes (loop->header); phi; phi = PHI_CHAIN (phi))
    {
      name = PHI_RESULT (phi);
      if (!is_gimple_reg (name))
	continue;

      ver = SSA_NAME_VERSION (name);
      ok = simple_iv (loop, phi, name, &iv, true);
      gcc_assert (ok);

      tem.uid = ver;
      slot = htab_find_slot_with_hash (*steps, &tem, ver, INSERT);
      elt = XNEW (struct int_tree_map);
      elt->uid = ver;
      elt->to = iv.step;
      *slot = elt;
    }
}

/* Remaps ssa names in EXPR according to NAME_COPIES, and returns the updated
   expression.  */

static tree
remap_ssa_names (tree expr, htab_t name_copies)
{
  struct name_to_copy_elt tem, *elt;
  unsigned ver;
  tree copied;
  unsigned i, n;
  tree nop;

  if (TREE_CODE (expr) == SSA_NAME)
    {
      ver = SSA_NAME_VERSION (expr);
      tem.version = ver;
      elt = htab_find_with_hash (name_copies, &tem, ver);

      return elt->new_name;
    }

  if (!EXPR_P (expr))
    return unshare_expr (expr);

  copied = copy_node (expr);
  n = TREE_CODE_LENGTH (TREE_CODE (expr));

  for (i = 0; i < n; i++)
    {
      nop = remap_ssa_names (TREE_OPERAND (expr, i), name_copies);
      TREE_OPERAND (copied, i) = nop;
    }

  return copied;
}

/* Records steps for induction variables of NLOOP in STEPS, and remap the steps
   of induction variables in LOOP according to NAME_COPIES.  */

static void
record_and_update_steps (struct loop *loop, struct loop *nloop, htab_t steps,
			 htab_t name_copies)
{
  tree phi, nphi, name, nname;
  unsigned ver, nver;
  void **slot;
  struct int_tree_map tem, *elt, *nelt;

  /* The corresponding phi nodes in LOOP and in NLOOP should be in the same
     order.  */
  for (phi = phi_nodes (loop->header), nphi = phi_nodes (nloop->header); phi;
       phi = PHI_CHAIN (phi), nphi = PHI_CHAIN (nphi))
    {
      name = PHI_RESULT (phi);
      nname = PHI_RESULT (nphi);

      if (!is_gimple_reg (name))
	{
	  gcc_assert (!is_gimple_reg (nname));
	  continue;
	}
      gcc_assert (is_gimple_reg (nname));
      ver = SSA_NAME_VERSION (name);
      nver = SSA_NAME_VERSION (nname);

      /* First copy the values from LOOP to NLOOP.  */
      tem.uid = ver;
      elt = htab_find_with_hash (steps, &tem, ver);
      tem.uid = nver;
      slot = htab_find_slot_with_hash (steps, &tem, nver, INSERT);
      nelt = XNEW (struct int_tree_map);
      nelt->uid = nver;
      nelt->to = unshare_expr (elt->to);
      *slot = nelt;

      /* Then rewrite the old ones.  */
      elt->to = remap_ssa_names (elt->to, name_copies);
    }
  gcc_assert (nphi == NULL_TREE);
}

/* Change the exit condition of LOOP so that it exits after PER_THREAD
   iterations, and remove the old exit.  */

static void
change_exit_condition (struct loop *loop, tree per_thread)
{
  basic_block ex_bb = loop->single_exit->src;
  basic_block ex_to = loop->single_exit->dest;
  block_stmt_iterator bsi = bsi_last (ex_bb);
  edge in_loop, e;
  tree exit_stmt = bsi_stmt (bsi);
  tree type = TREE_TYPE (per_thread);
  tree iv, cond;

  gcc_assert (TREE_CODE (exit_stmt) == COND_EXPR);
  bsi_remove (&bsi, true);

  in_loop = EDGE_SUCC (ex_bb, 0);
  if (in_loop == loop->single_exit)
    in_loop = EDGE_SUCC (ex_bb, 1);

  remove_edge (loop->single_exit);
  in_loop->flags &= ~(EDGE_TRUE_VALUE | EDGE_FALSE_VALUE);
  in_loop->flags |= EDGE_FALLTHRU;

  e = single_pred_edge (loop_split_edge_with (loop_latch_edge (loop), NULL));
  bsi = bsi_last (e->src);
  create_iv (build_int_cst (type, 0), build_int_cst (type, 1), NULL_TREE,
	     loop, &bsi, true, NULL, &iv);
  cond = build3 (COND_EXPR, void_type_node,
		 fold_build2 (LT_EXPR, boolean_type_node, iv, per_thread),
		 build1 (GOTO_EXPR, void_type_node,
			 tree_block_label (e->dest)),
		 build1 (GOTO_EXPR, void_type_node,
			 tree_block_label (ex_to)));
  bsi_insert_after (&bsi, cond, BSI_NEW_STMT);

  e->flags &= ~EDGE_FALLTHRU;
  e->flags |= EDGE_TRUE_VALUE;
  loop->single_exit = make_edge (e->src, ex_to, EDGE_FALSE_VALUE);
}

/* Bitmap containing uids of functions created by parallelization.  We cannot
   allocate it from the default obstack, as it must live across compilation
   of several functions; we make it gc allocated instead.  */

static GTY(()) bitmap parallelized_functions;

/* Returns true if FN was created by create_loop_fn.  */

static bool
parallelized_function_p (tree fn)
{
  if (!parallelized_functions || !DECL_ARTIFICIAL (fn))
    return false;

  return bitmap_bit_p (parallelized_functions, DECL_UID (fn));
}

/* Creates and returns an empty function to that the body of the loop
   is later split.  */

static tree
create_loop_fn (void)
{
  char buf[100];
  char *tname;
  tree decl, type, name, t;
  struct function *act_cfun = cfun;
  static unsigned loopfn_num;

  snprintf (buf, 100, "%s.$loopfn", current_function_name ());
  ASM_FORMAT_PRIVATE_NAME (tname, buf, loopfn_num++);
  clean_symbol_name (tname);
  name = get_identifier (tname);
  type = build_function_type_list (void_type_node, ptr_type_node, NULL_TREE);

  decl = build_decl (FUNCTION_DECL, name, type);
  if (!parallelized_functions)
    parallelized_functions = BITMAP_GGC_ALLOC ();
  bitmap_set_bit (parallelized_functions, DECL_UID (decl));

  TREE_STATIC (decl) = 1;
  TREE_USED (decl) = 1;
  DECL_ARTIFICIAL (decl) = 1;
  DECL_IGNORED_P (decl) = 0;
  TREE_PUBLIC (decl) = 0;
  DECL_UNINLINABLE (decl) = 1;
  DECL_EXTERNAL (decl) = 0;
  DECL_CONTEXT (decl) = NULL_TREE;
  DECL_INITIAL (decl) = make_node (BLOCK);

  t = build_decl (RESULT_DECL, NULL_TREE, void_type_node);
  DECL_ARTIFICIAL (t) = 1;
  DECL_IGNORED_P (t) = 1;
  DECL_RESULT (decl) = t;

  t = build_decl (PARM_DECL, get_identifier (".paral_data_param"),
		  ptr_type_node);
  DECL_ARTIFICIAL (t) = 1;
  DECL_ARG_TYPE (t) = ptr_type_node;
  DECL_CONTEXT (t) = decl;
  TREE_USED (t) = 1;
  DECL_ARGUMENTS (decl) = t;

  allocate_struct_function (decl);

  /* The call to allocate_struct_function clobbers CFUN, so we need to restore
     it.  */
  cfun = act_cfun;

  return decl;
}

/* Extracts LOOP and its preheader to a separate function.  This function is
   returned in LOOP_FN.  ARG_STRUCT is initialized in the new function from
   an argument of the function.  The single basic block that replaces LOOP is
   returned.  */

static basic_block
extract_loop_to_function (struct loop *loop, tree arg_struct, tree *loop_fn)
{
  basic_block bb_to = loop_split_edge_with (loop->single_exit, NULL);
  basic_block bb_from = loop_preheader_edge (loop)->src;
  basic_block repl_bb;
  tree arg, narg, stmt;
  struct function *act_cfun = cfun;
  tree act_decl = current_function_decl;
  block_stmt_iterator bsi;
  basic_block *body = get_loop_body (loop);
  struct loop *outer = loop->outer;
  unsigned i, n = loop->num_nodes;

  cancel_loop_tree (current_loops, loop);
  for (i = 0; i < n; i++)
    remove_bb_from_loops (body[i]);
  free (body);
  remove_bb_from_loops (bb_from);
  remove_bb_from_loops (bb_to);

  bsi = bsi_last (bb_to);
  bsi_insert_after (&bsi, build1 (RETURN_EXPR, void_type_node, NULL),
		    BSI_NEW_STMT);

  *loop_fn = create_loop_fn ();
  repl_bb = move_sese_region_to_fn (DECL_STRUCT_FUNCTION (*loop_fn),
				    bb_from, bb_to);
  add_bb_to_loop (repl_bb, outer);

  cfun = DECL_STRUCT_FUNCTION (*loop_fn);
  current_function_decl = *loop_fn;

  /* Initialize the arg_struct.  */
  if (arg_struct)
    {
      arg = DECL_ARGUMENTS (*loop_fn);
      add_referenced_var (arg);
      narg = make_ssa_name (arg, build_empty_stmt ());
      set_default_def (arg, narg);

      bsi = bsi_after_labels (single_succ (ENTRY_BLOCK_PTR));
      stmt = build2 (MODIFY_EXPR, void_type_node, arg_struct,
		     fold_convert (TREE_TYPE (arg_struct), narg));
      SSA_NAME_DEF_STMT (arg_struct) = stmt;
      bsi_insert_before (&bsi, stmt, BSI_NEW_STMT);
    }
  cfun = act_cfun;
  current_function_decl = act_decl;

  go_out_of_ssa (*loop_fn);

  return repl_bb;
}

/* Emits the code to call LOOP_FN with argument ARG in N_THREADS threads
   after BSI.  */

static void
call_in_parallel (block_stmt_iterator bsi, tree loop_fn, tree arg,
		  unsigned n_threads)
{
  tree start_decl = built_in_decls[BUILT_IN_GOMP_PARALLEL_START];
  tree args, stmts, adata;

  args = tree_cons (NULL, build_int_cst (unsigned_type_node, n_threads),
		    NULL);
  if (arg)
    {
      adata = build_fold_addr_expr (arg);
      mark_call_clobbered (arg, ESCAPE_TO_CALL);
    }
  else
    adata = null_pointer_node;
  args = tree_cons (NULL, adata, args);
  args = tree_cons (NULL, build_fold_addr_expr (loop_fn), args);
  force_gimple_operand (build_function_call_expr (start_decl, args),
			&stmts, false, NULL_TREE);
  mark_virtual_ops_for_renaming_list (stmts);
  bsi_insert_after (&bsi, stmts, BSI_CONTINUE_LINKING);
}

/* Makes the induction variables in LOOP start at
   (N_THREADS - 1) * PER_THREAD-th iteration when the LOOP is entered through
   NEW_ENTRY.  STEPS describe the steps of induction variables in LOOP.  */

static void
shift_ivs_for_remaining_iterations (struct loop *loop, tree per_thread,
				    unsigned n_threads, edge new_entry,
				    htab_t steps)
{
  tree phi, name, nphi;
  unsigned ver;
  struct int_tree_map tem, *elt;
  basic_block bb = new_entry->dest;
  edge old_entry, entry;
  tree stmts, new_init, init, pass, type, delta;
  use_operand_p init_p;

  old_entry = EDGE_PRED (bb, 0);
  if (old_entry == new_entry)
    old_entry = EDGE_PRED (bb, 1);
  entry = loop_preheader_edge (loop);

  for (phi = phi_nodes (loop->header); phi; phi = PHI_CHAIN (phi))
    {
      name = PHI_RESULT (phi);
      if (!is_gimple_reg (name))
	continue;

      ver = SSA_NAME_VERSION (name);
      tem.uid = ver;
      elt = htab_find_with_hash (steps, &tem, ver);

      nphi = create_phi_node (SSA_NAME_VAR (name), bb);
      pass = PHI_RESULT (nphi);

      init_p = PHI_ARG_DEF_PTR_FROM_EDGE (phi, entry);
      init = USE_FROM_PTR (init_p);
      SET_USE (init_p, pass);

      add_phi_arg (nphi, init, old_entry);
      type = TREE_TYPE (name);
      delta = fold_build2 (MULT_EXPR, type,
			   build_int_cst (type, n_threads -  1),
			   per_thread);
      delta = fold_build2 (MULT_EXPR, type, delta, unshare_expr (elt->to));
      new_init = fold_build2 (PLUS_EXPR, type, init, delta);

      new_init = force_gimple_operand (new_init, &stmts, true, NULL_TREE);
      if (stmts)
	bsi_insert_on_edge_immediate_loop (new_entry, stmts);
      add_phi_arg (nphi, new_init, new_entry);
    }
}

/* If PAR is true, emit the code on E to finalize the threads.  */

static void
finalize_threads (edge e, tree par)
{
  basic_block cond_bb = loop_split_edge_with (e, NULL);
  basic_block fin_bb = loop_split_edge_with (single_succ_edge (cond_bb), NULL);
  basic_block cont_bb = single_succ (fin_bb);
  block_stmt_iterator bsi = bsi_last (cond_bb);
  tree decl = built_in_decls[BUILT_IN_GOMP_PARALLEL_END];
  tree stmt = build_function_call_expr (decl, NULL);
  edge te, fe, be;
  tree phi;

  bsi_insert_after (&bsi,
		    build3 (COND_EXPR, void_type_node,
			    par,
			    build1 (GOTO_EXPR, void_type_node,
				    tree_block_label (fin_bb)),
			    build1 (GOTO_EXPR, void_type_node,
				    tree_block_label (cont_bb))),
		    BSI_NEW_STMT);
  te = single_succ_edge (cond_bb);
  te->flags = EDGE_TRUE_VALUE;
  be = single_succ_edge (fin_bb);
  fe = make_edge (cond_bb, cont_bb, EDGE_FALSE_VALUE);

  for (phi = phi_nodes (cont_bb); phi != NULL_TREE; phi = PHI_CHAIN (phi))
    add_phi_arg (phi, PHI_ARG_DEF_FROM_EDGE (phi, be), fe);

  set_immediate_dominator (CDI_DOMINATORS, cont_bb,
			   recount_dominator (CDI_DOMINATORS, cont_bb));
  bsi = bsi_last (fin_bb);
  mark_virtual_ops_for_renaming (stmt);
  bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);
}

/* Generates code to execute the iterations of LOOP in N_THREADS threads in
   parallel.  NITER describes number of iterations of LOOP.  */

static void
gen_parallel_loop (struct loop *loop, unsigned n_threads,
		   struct tree_niter_desc *niter)
{
  struct loop *nloop;
  tree many_iterations_cond, type, per_thread, new_per_thread;
  tree stmts, arg_struct, new_arg_struct, loop_fn, par_flag, phi;
  basic_block repl_bb, npre;
  htab_t steps, name_copies;
  edge orig_entry;

  /* From

     ---------------------------------------------------------------------
     loop
       {
	 IV = phi (INIT, IV + STEP)
	 BODY1;
	 if (COND)
	   break;
	 BODY2;
       }
     ---------------------------------------------------------------------

     with # of iterations NITER (possibly with MAY_BE_ZERO assumption),
     we generate the following code:

     ---------------------------------------------------------------------
     parallelized = false;

     if (MAY_BE_ZERO
	 || NITER < MIN_PER_THREAD * N_THREADS)
       goto rest;

     per_thread = NITER / N_THREADS;
     store all local loop-invariant variables (including per_thread in case it
       is not a constant) used in body of the loop to DATA.
     __builtin_GOMP_parallel_start (loop_fn, &DATA, N_THREADS);
     INIT += STEP * per_thread * (N_THREADS - 1);
     parallelized = true;

     rest:
     loop
       {
	 IV = phi (INIT, IV + STEP)
	 BODY1;
	 if (COND)
	   break;
	 BODY2;
       }

     if (parallelized)
       __builtin_GOMP_parallel_end ();
     ---------------------------------------------------------------------

     With the function

     ---------------------------------------------------------------------
     void loop_fn (void *data)
       {
	 load all local loop-invariant variables used in body of the loop
	   from data
	 INIT += STEP * per_thread * (thread_id - 1);

	 loop
	   {
	     IV = phi (INIT, IV + STEP)
	     NEW_CTR = phi (0, NEW_CTR');
	     BODY1;
	     BODY2;
	     NEW_CTR' = NEW_CTR + 1;
	     if (NEW_CTR' == per_thread)
	       return;
	   }
       }
     ---------------------------------------------------------------------
   */

  /* Record the steps of the induction variables.  */
  record_steps (loop, &steps);

  /* Create two versions of the loop -- in the old one, we know that the
     number of iterations is large enough, and we will transform it into the
     loop that will be split to loop_fn, the new one will be used for the
     remaining iterations.  */
  
  type = TREE_TYPE (niter->niter);
  many_iterations_cond =
	  fold_build2 (GE_EXPR, boolean_type_node,
		       niter->niter,
		       build_int_cst (type, MIN_PER_THREAD * n_threads));
  many_iterations_cond
	  = fold_build2 (TRUTH_AND_EXPR, boolean_type_node,
			 invert_truthvalue (niter->may_be_zero),
			 many_iterations_cond);
  many_iterations_cond
	  = force_gimple_operand (unshare_expr (many_iterations_cond),
				  &stmts, false, NULL_TREE);
  if (stmts)
    bsi_insert_on_edge_immediate_loop (loop_preheader_edge (loop), stmts);
  if (!is_gimple_condexpr (many_iterations_cond))
    {
      many_iterations_cond
	      = force_gimple_operand (many_iterations_cond, &stmts,
				      true, NULL_TREE);
      if (stmts)
	bsi_insert_on_edge_immediate_loop (loop_preheader_edge (loop), stmts);
    }

  initialize_original_copy_tables ();
  nloop = loop_version (current_loops, loop, many_iterations_cond, NULL, true);
  update_ssa (TODO_update_ssa);
  free_original_copy_tables ();

  /* Compute number of iterations per thread.  */
  per_thread = fold_build2 (FLOOR_DIV_EXPR, type, niter->niter,
			    build_int_cst (type, n_threads));
  per_thread = force_gimple_operand (unshare_expr (per_thread), &stmts, true,
				     NULL_TREE);
  if (stmts)
    bsi_insert_on_edge_immediate_loop (loop_preheader_edge (loop), stmts);

  /* Eliminate the references to local variables from the loop.  */
  eliminate_local_variables (loop);

  /* In the old loop, move all variables non-local to the loop to a structure
     and back, and create separate decls for the variables used in loop.  */
  new_per_thread = per_thread;
  separate_decls_in_loop (loop, &new_per_thread, &arg_struct, &new_arg_struct,
			  &name_copies);

  /* Record the steps of induction variables in the copy, and remap those
     in the original loop.  */
  record_and_update_steps (loop, nloop, steps, name_copies);

  /* Update the initial values of induction variables.  */
  shift_ivs_for_iteration (loop, new_per_thread, steps);

  /* Add the new exit condition.  */
  change_exit_condition (loop, new_per_thread);

  /* Split the loop to the new function.  */
  repl_bb = extract_loop_to_function (loop, new_arg_struct, &loop_fn);
  cgraph_add_new_function (loop_fn);

  /* Call the builtin to run the function in several threads.  */
  call_in_parallel (bsi_last (repl_bb), loop_fn, arg_struct, n_threads);

  /* Redirect the exit edge to the versioned copy, and set the parallelized
     flag.  */
  par_flag = create_tmp_var (boolean_type_node, "parallelized");
  add_referenced_var (par_flag);

  npre = loop_preheader_edge (nloop)->src;
  gcc_assert (phi_nodes (npre) == NULL);

  orig_entry = single_pred_edge (npre);
  redirect_edge_and_branch (single_succ_edge (repl_bb),
			    loop_preheader_edge (nloop)->src);
  phi = create_phi_node (par_flag, npre);
  add_phi_arg (phi, boolean_false_node, orig_entry);
  add_phi_arg (phi, boolean_true_node, single_succ_edge (repl_bb));
  par_flag = PHI_RESULT (phi);

  /* Update the initial values of induction variables in the versioned
     copy.  */
  shift_ivs_for_remaining_iterations (nloop, per_thread, n_threads,
				      single_succ_edge (repl_bb), steps);

  /* Finalize the threads after the loop if necessary.  */
  finalize_threads (nloop->single_exit, par_flag);

  htab_delete (name_copies);
  htab_delete (steps);
  scev_reset ();

  /* We created a new call clobbered variable.  This means every call in the
     function gets a new virtual operand.  However, we cannot rerun alias
     analysis after vectorizer (or at least, passes.c claims so), thus we
     must mark them for renaming manually.  */
  mark_call_virtual_operands ();
  update_ssa (TODO_update_ssa_only_virtuals);
}

/* Detect parallel loops and generate parallel code using libgomp
   primitives.  */

bool
parallelize_loops (void)
{
  unsigned i, n_threads = flag_tree_parallelize_loops;
  unsigned n = current_loops->num;
  bool changed = false;
  struct loop *loop;
  struct tree_niter_desc niter_desc;

  /* Do not parallelize loops in the functions created by parallelization.  */
  if (parallelized_function_p (cfun->decl))
    return false;

  for (i = 1; i < n; i++)
    {
      loop = current_loops->parray[i];
      if (!loop
	  /* Do not bother with loops in cold areas.  */
	  || !maybe_hot_bb_p (loop->header)
	  /* Or loops that roll too little.  */
	  || expected_loop_iterations (loop) <= n_threads
	  /* And of course, the loop must be parallelizable.  */
	  || !can_duplicate_loop_p (loop)
	  || !loop_parallel_p (loop, &niter_desc))
	continue;

      changed = true;
      gen_parallel_loop (loop, n_threads, &niter_desc);
      verify_flow_info ();
      verify_dominators (CDI_DOMINATORS);
      verify_loop_structure (current_loops);
      verify_loop_closed_ssa ();
    }

  return changed;
}

#include "gt-tree-parloops.h"
