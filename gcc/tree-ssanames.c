/* Generic routines for manipulating SSA_NAME expressions
   Copyright (C) 2003-2013 Free Software Foundation, Inc.

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
#include "tree-flow.h"
#include "tree-pass.h"
#include "gimple-tree.h"

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
   more compact.  */


/* Version numbers with special meanings.  We start allocating new version
   numbers after the special ones.  */
#define UNUSED_NAME_VERSION 0

unsigned int ssa_name_nodes_reused;
unsigned int ssa_name_nodes_created;

/* Initialize management of SSA_NAMEs to default SIZE.  If SIZE is
   zero use default.  */

void
init_ssanames (struct function *fn, int size)
{
  if (size < 50)
    size = 50;

  vec_alloc (SSANAMES (fn), size);

  /* Version 0 is special, so reserve the first slot in the table.  Though
     currently unused, we may use version 0 in alias analysis as part of
     the heuristics used to group aliases when the alias sets are too
     large.

     We use vec::quick_push here because we know that SSA_NAMES has at
     least 50 elements reserved in it.  */
  SSANAMES (fn)->quick_push (NULL_TREE);
  FREE_SSANAMES (fn) = NULL;

  fn->gimple_df->ssa_renaming_needed = 0;
  fn->gimple_df->rename_vops = 0;
}

/* Finalize management of SSA_NAMEs.  */

void
fini_ssanames (void)
{
  vec_free (SSANAMES (cfun));
  vec_free (FREE_SSANAMES (cfun));
}

/* Dump some simple statistics regarding the re-use of SSA_NAME nodes.  */

void
ssanames_print_statistics (void)
{
  fprintf (stderr, "SSA_NAME nodes allocated: %u\n", ssa_name_nodes_created);
  fprintf (stderr, "SSA_NAME nodes reused: %u\n", ssa_name_nodes_reused);
}

/* Return an SSA_NAME node for variable VAR defined in statement STMT
   in function FN.  STMT may be an empty statement for artificial
   references (e.g., default definitions created when a variable is
   used without a preceding definition).  */
SSADecl
make_ssa_name_fn (struct function *fn, GimpleType type, gimple stmt)
{
  SSADecl t;
  use_operand_p imm;

  gcc_assert (is_gimple_reg_type (type));

  /* If our free list has an element, then use it.  */
  if (!vec_safe_is_empty (FREE_SSANAMES (fn)))
    {
      t = FREE_SSANAMES (fn)->pop ();
      if (GATHER_STATISTICS)
	ssa_name_nodes_reused++;

      /* The node was cleared out when we put it on the free list, so
	 there is no need to do so again here.  */
      gcc_assert (ssa_name (t.ssa_name_version ()) == NULL);
      (*SSANAMES (fn))[t.ssa_name_version ()] = t;
    }
  else
    {
      t = make_node (SSA_NAME);
      t.set_ssa_name_version (SSANAMES (fn)->length ());
      vec_safe_push (SSANAMES (fn), (tree)t);
      if (GATHER_STATISTICS)
	ssa_name_nodes_created++;
    }

  t.set_type (type);
  t.set_ssa_name_var (NULL_TREE);

  t.set_ssa_name_def_stmt (stmt);
  t.set_ssa_name_ptr_info (NULL);
  t.set_ssa_name_in_free_list (false);
  t.set_ssa_name_is_default_def (false);
  imm = t.ssa_name_imm_use_node_ptr ();
  imm->use = NULL;
  imm->prev = imm;
  imm->next = imm;
  imm->loc.ssa_name = t;

  return t;

}

SSADecl
make_ssa_name_fn (struct function *fn, GimpleDecl var, gimple stmt)
{
  SSADecl t;

  gcc_assert (var.code() == VAR_DECL
	      || var.code() == PARM_DECL
	      || var.code() == RESULT_DECL);

  t = make_ssa_name_fn (fn, var.type(), stmt);
  t.set_ssa_name_var (var);
  return t;
}


/* We no longer need the SSA_NAME expression VAR, release it so that
   it may be reused.

   Note it is assumed that no calls to make_ssa_name will be made
   until all uses of the ssa name are released and that the only
   use of the SSA_NAME expression is to check its SSA_NAME_VAR.  All
   other fields must be assumed clobbered.  */

void
release_ssa_name (SSADecl var)
{
  if (!var)
    return;

  /* Never release the default definition for a symbol.  It's a
     special SSA name that should always exist once it's created.  */
  if (var.ssa_name_is_default_def ())
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
  if (! var.ssa_name_in_free_list ())
    {
      GimpleDecl saved_ssa_name_var = var.ssa_name_var ();
      GimpleIdentifier saved_ssa_name_ident = var.ssa_name_identifier ();
      int saved_ssa_name_version = var.ssa_name_version ();
      use_operand_p imm = var.ssa_name_imm_use_node_ptr ();

      if (MAY_HAVE_DEBUG_STMTS)
	insert_debug_temp_for_var_def (NULL, var);

#ifdef ENABLE_CHECKING
      verify_imm_links (stderr, var);
#endif
      while (imm->next != imm)
	delink_imm_use (imm->next);

      (*SSANAMES (cfun))[var.ssa_name_version ()] = NULL_TREE;
      memset (var, 0, tree_size (var));

      imm->prev = imm;
      imm->next = imm;
      imm->loc.ssa_name = var;

      /* First put back the right tree node so that the tree checking
	 macros do not complain.  */
      var.set_code (SSA_NAME);

      /* Restore the version number.  */
      var.set_ssa_name_version (saved_ssa_name_version);

      /* Hopefully this can go away once we have the new incremental
         SSA updating code installed.  */
      if (saved_ssa_name_var)
	var.set_ssa_name_var (saved_ssa_name_var);
      else
	var.set_ssa_name_identifier (saved_ssa_name_ident);

      /* Note this SSA_NAME is now in the first list.  */
      var.set_ssa_name_in_free_list (true);

      /* And finally put it on the free list.  */
      vec_safe_push (FREE_SSANAMES (cfun), (tree)var);
    }
}

/* If the alignment of the pointer described by PI is known, return true and
   store the alignment and the deviation from it into *ALIGNP and *MISALIGNP
   respectively.  Otherwise return false.  */

bool
get_ptr_info_alignment (struct ptr_info_def *pi, unsigned int *alignp,
			unsigned int *misalignp)
{
  if (pi->align)
    {
      *alignp = pi->align;
      *misalignp = pi->misalign;
      return true;
    }
  else
    return false;
}

/* State that the pointer described by PI has unknown alignment.  */

void
mark_ptr_info_alignment_unknown (struct ptr_info_def *pi)
{
  pi->align = 0;
  pi->misalign = 0;
}

/* Store the the power-of-two byte alignment and the deviation from that
   alignment of pointer described by PI to ALIOGN and MISALIGN
   respectively.  */

void
set_ptr_info_alignment (struct ptr_info_def *pi, unsigned int align,
			    unsigned int misalign)
{
  gcc_checking_assert (align != 0);
  gcc_assert ((align & (align - 1)) == 0);
  gcc_assert ((misalign & ~(align - 1)) == 0);

  pi->align = align;
  pi->misalign = misalign;
}

/* If pointer described by PI has known alignment, increase its known
   misalignment by INCREMENT modulo its current alignment.  */

void
adjust_ptr_info_misalignment (struct ptr_info_def *pi,
			      unsigned int increment)
{
  if (pi->align != 0)
    {
      pi->misalign += increment;
      pi->misalign &= (pi->align - 1);
    }
}

/* Return the alias information associated with pointer T.  It creates a
   new instance if none existed.  */

struct ptr_info_def *
get_ptr_info (SSADecl decl)
{
  struct ptr_info_def *pi;

  gcc_assert (decl.type().pointer_type_p());

  pi = decl.ssa_name_ptr_info ();
  if (pi == NULL)
    {
      pi = ggc_alloc_cleared_ptr_info_def ();
      pt_solution_reset (&pi->pt);
      mark_ptr_info_alignment_unknown (pi);
      decl.set_ssa_name_ptr_info (pi);
    }

  return pi;
}


/* Creates a new SSA name using the template NAME tobe defined by
   statement STMT in function FN.  */

SSADecl
copy_ssa_name_fn (struct function *fn, SSADecl name, gimple stmt)
{
  SSADecl new_name;

  if (name.ssa_name_var ())
    new_name = make_ssa_name_fn (fn, name.ssa_name_var (), stmt);
  else
    {
      new_name = make_ssa_name_fn (fn, name.type(), stmt);
      new_name.set_ssa_name_identifier (name.ssa_name_identifier ());
    }

  return new_name;
}


/* Creates a duplicate of the ptr_info_def at PTR_INFO for use by
   the SSA name NAME.  */

void
duplicate_ssa_name_ptr_info (SSADecl name, struct ptr_info_def *ptr_info)
{
  struct ptr_info_def *new_ptr_info;

  gcc_assert (name.type().pointer_type_p ());
  gcc_assert (!name.ssa_name_ptr_info ());

  if (!ptr_info)
    return;

  new_ptr_info = ggc_alloc_ptr_info_def ();
  *new_ptr_info = *ptr_info;

  name.set_ssa_name_ptr_info (new_ptr_info);
}


/* Creates a duplicate of a ssa name NAME tobe defined by statement STMT
   in function FN.  */

SSADecl
duplicate_ssa_name_fn (struct function *fn, SSADecl name, gimple stmt)
{
  SSADecl new_name = copy_ssa_name_fn (fn, name, stmt);
  struct ptr_info_def *old_ptr_info = name.ssa_name_ptr_info ();

  if (old_ptr_info)
    duplicate_ssa_name_ptr_info (new_name, old_ptr_info);

  return new_name;
}


/* Release all the SSA_NAMEs created by STMT.  */

void
release_defs (gimple stmt)
{
  GimpleValue def;
  ssa_op_iter iter;

  /* Make sure that we are in SSA.  Otherwise, operand cache may point
     to garbage.  */
  gcc_assert (gimple_in_ssa_p (cfun));

  FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_ALL_DEFS)
    if (SSADecl name = def.as_a<SSADecl>())
      release_ssa_name (name);
}


/* Replace the symbol associated with SSA_NAME with SYM.  */

void
replace_ssa_name_symbol (SSADecl ssa_name, GimpleDecl sym)
{
  ssa_name.set_ssa_name_var (sym);
  ssa_name.set_type (sym.type ());
}

/* Return SSA names that are unused to GGC memory and compact the SSA
   version namespace.  This is used to keep footprint of compiler during
   interprocedural optimization.  */
static unsigned int
release_dead_ssa_names (void)
{
  unsigned i, j;
  int n = vec_safe_length (FREE_SSANAMES (cfun));

  /* Now release the freelist.  */
  vec_free (FREE_SSANAMES (cfun));

  /* And compact the SSA number space.  We make sure to not change the
     relative order of SSA versions.  */
  for (i = 1, j = 1; i < cfun->gimple_df->ssa_names->length (); ++i)
    {
      SSADecl name = ssa_name (i);
      if (name)
	{
	  if (i != j)
	    {
	      name.set_ssa_name_version (j);
	      (*cfun->gimple_df->ssa_names)[j] = name;
	    }
	  j++;
	}
    }
  cfun->gimple_df->ssa_names->truncate (j);

  statistics_counter_event (cfun, "SSA names released", n);
  statistics_counter_event (cfun, "SSA name holes removed", i - j);
  if (dump_file)
    fprintf (dump_file, "Released %i names, %.2f%%, removed %i holes\n",
	     n, n * 100.0 / num_ssa_names, i - j);
  return 0;
}

struct gimple_opt_pass pass_release_ssa_names =
{
 {
  GIMPLE_PASS,
  "release_ssa",			/* name */
  OPTGROUP_NONE,                        /* optinfo_flags */
  NULL,					/* gate */
  release_dead_ssa_names,		/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_SSA_OTHER,			/* tv_id */
  PROP_ssa,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  TODO_remove_unused_locals,		/* todo_flags_start */
  0					/* todo_flags_finish */
 }
};
