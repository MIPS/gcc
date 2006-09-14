/* Routines for performing Temporary Expression Replacement (TER) in SSA trees.
   Copyright (C) 2003, 2004, 2005, 2006 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod  <amacleod@redhat.com>

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
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "flags.h"
#include "rtl.h"
#include "tm_p.h"
#include "ggc.h"
#include "langhooks.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "expr.h"
#include "function.h"
#include "diagnostic.h"
#include "bitmap.h"
#include "tree-flow.h"
#include "tree-gimple.h"
#include "tree-inline.h"
#include "varray.h"
#include "timevar.h"
#include "hashtab.h"
#include "tree-dump.h"
#include "tree-ssa-live.h"
#include "tree-pass.h"
#include "toplev.h"
#include "vecprim.h"



/* Temporary Expression Replacement (TER)

   Replace SSA version variables during out-of-ssa with their defining
   expression if there is only one use of the variable.

   A pass is made through the function, one block at a time.  No cross block
   information is tracked.

   Variables which only have one use, and whose defining stmt is considered
   a replaceable expression (see check_replaceable) are entered into 
   consideration by adding a list of dependent partitions to the version_info
   vector for that ssa_name_version.  This information comes from the partition
   mapping for each USE.  At the same time, the partition_dep_list vector for 
   these partitions have this version number entered into their lists.

   When the use of a replaceable ssa_variable is encountered, the dependence
   list in version_info[] is moved to the "pending_dependence" list in case
   the current expression is also replaceable. (To be determined later in 
   processing this stmt.) version_info[] for the version is then updated to 
   point to the defining stmt and the 'replaceable' bit is set.

   Any partition which is defined by a statement 'kills' any expression which
   is dependent on this partition.  Every ssa version in the partitions' 
   dependence list is removed from future consideration.

   All virtual references are lumped together.  Any expression which is
   dependent on any virtual variable (via a VUSE) has a dependence added
   to the special partition defined by VIRTUAL_PARTITION.

   Whenever a V_MAY_DEF is seen, all expressions dependent this 
   VIRTUAL_PARTITION are removed from consideration.

   At the end of a basic block, all expression are removed from consideration
   in preparation for the next block.  
   
   The end result is a vector over SSA_NAME_VERSION which is passed back to
   rewrite_out_of_ssa.  As the SSA variables are being rewritten, instead of
   replacing the SSA_NAME tree element with the partition it was assigned, 
   it is replaced with the RHS of the defining expression.  */


/* Temporary Expression Replacement (TER) table information.  */

typedef struct temp_expr_table_d 
{
  var_map map;
  void **version_info;
  bitmap *expr_vars;
  bitmap *partition_dep_list;
  bitmap replaceable;
  bool saw_replaceable;
  int virtual_partition;
  bitmap partition_in_use;
  bitmap pending_dependence;
} *temp_expr_table_p;

/* Used to indicate a dependency on V_MAY_DEFs.  */
#define VIRTUAL_PARTITION(table)	(table->virtual_partition)


/* Create a new TER table for MAP.  */

static temp_expr_table_p
new_temp_expr_table (var_map map)
{
  temp_expr_table_p t;

  t = XNEW (struct temp_expr_table_d);
  t->map = map;

  t->version_info = XCNEWVEC (void *, num_ssa_names + 1);
  t->expr_vars = XCNEWVEC (bitmap, num_ssa_names + 1);
  t->partition_dep_list = XCNEWVEC (bitmap,
                                    num_var_partitions (map) + 1);

  t->replaceable = BITMAP_ALLOC (NULL);
  t->partition_in_use = BITMAP_ALLOC (NULL);

  t->saw_replaceable = false;
  t->virtual_partition = num_var_partitions (map);
  t->pending_dependence = BITMAP_ALLOC (NULL);

  return t;
}


/* Free TER table T.  If there are valid replacements, return the expression 
   vector.  */

static tree *
free_temp_expr_table (temp_expr_table_p t)
{
  tree *ret = NULL;
  unsigned i;

#ifdef ENABLE_CHECKING
  unsigned x;
  for (x = 0; x <= num_var_partitions (t->map); x++)
    gcc_assert (!t->partition_dep_list[x]);
#endif

  BITMAP_FREE (t->partition_in_use);
  BITMAP_FREE (t->replaceable);

  for (i = 0; i <= num_ssa_names; i++)
    if (t->expr_vars[i])
      BITMAP_FREE (t->expr_vars[i]);
  free (t->expr_vars);

  free (t->partition_dep_list);
  if (t->saw_replaceable)
    ret = (tree *)t->version_info;
  else
    free (t->version_info);
  
  free (t);
  return ret;
}


/* Add VALUE to the version list for LIST.  TAB is the expression table */

static inline void
add_value_to_version_list (temp_expr_table_p tab, int list, int value)
{

  if (!tab->version_info[list])
    tab->version_info[list] = BITMAP_ALLOC (NULL);

  bitmap_set_bit (tab->version_info[list], value);
}


/* Add VALUE to the partition list for LIST.  TAB is the expression table */

static inline void
add_value_to_partition_list (temp_expr_table_p tab, int list, int value)
{

  if (!tab->partition_dep_list[list])
    tab->partition_dep_list[list] = BITMAP_ALLOC (NULL);

  bitmap_set_bit (tab->partition_dep_list[list], value);
}


/* Remove VALUE from the partition list for LIST.  TAB is the expression 
   table.  */

static inline void 
remove_value_from_partition_list (temp_expr_table_p tab, int list, int value)
{
  bitmap_clear_bit (tab->partition_dep_list[list], value);
  if (bitmap_empty_p (tab->partition_dep_list[list]))
    {
      BITMAP_FREE (tab->partition_dep_list[list]);
      tab->partition_dep_list[list] = NULL;
    }
}


/* Add a dependency between the def of ssa VERSION and VAR.  If VAR is 
   replaceable by an expression, add a dependence each of the elements of the 
   expression.  These are contained in the pending list.  TAB is the
   expression table.  */

static void
add_dependence (temp_expr_table_p tab, int version, tree var)
{
  int i;
  bitmap_iterator bi;
  unsigned x;

  i = SSA_NAME_VERSION (var);
  if (bitmap_bit_p (tab->replaceable, i))
    {
      /* This variable is being substituted, so use whatever dependences
         were queued up when we marked this as replaceable earlier.  */
      EXECUTE_IF_SET_IN_BITMAP (tab->pending_dependence, 0, x, bi)
        {
	  /* Version is now dependant on each pending dep partition.  */
	  add_value_to_partition_list (tab, x, version);
	}
      /* Rather than set version and inuse lists bit by bit, simply OR in
         the pending_dependence bits.  */
      if (!tab->version_info[version])
        tab->version_info[version] = BITMAP_ALLOC (NULL);
      bitmap_ior_into (tab->version_info[version], tab->pending_dependence);
      bitmap_ior_into (tab->partition_in_use, tab->pending_dependence);

      bitmap_clear (tab->pending_dependence);
    }
  else
    {
      i = var_to_partition (tab->map, var);
      gcc_assert (i != NO_PARTITION);
      add_value_to_partition_list (tab, i, version);
      add_value_to_version_list (tab, version, i);
      bitmap_set_bit (tab->partition_in_use, i);
    }
}


/* Check if expression STMT is suitable for replacement in table TAB.  If so, 
   create an expression entry.  Return true if this stmt is replaceable.  */

static bool
check_replaceable (temp_expr_table_p tab, tree stmt)
{
  tree var, def, basevar;
  int version;
  ssa_op_iter iter;
  tree call_expr;
  bitmap def_vars = BITMAP_ALLOC (NULL), use_vars;

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return false;
  
  /* Punt if there is more than 1 def, or more than 1 use.  */
  def = SINGLE_SSA_TREE_OPERAND (stmt, SSA_OP_DEF);
  if (!def)
    return false;

  if (num_imm_uses (def) != 1)
    return false;

  /* There must be no V_MAY_DEFS or V_MUST_DEFS.  */
  if (!(ZERO_SSA_OPERANDS (stmt, (SSA_OP_VMAYDEF | SSA_OP_VMUSTDEF))))
    return false;

  /* Float expressions must go through memory if float-store is on.  */
  if (flag_float_store && FLOAT_TYPE_P (TREE_TYPE (TREE_OPERAND (stmt, 1))))
    return false;

  /* Calls to functions with side-effects cannot be replaced.  */
  if ((call_expr = get_call_expr_in (stmt)) != NULL_TREE)
    {
      int call_flags = call_expr_flags (call_expr);
      if (TREE_SIDE_EFFECTS (call_expr)
	  && !(call_flags & (ECF_PURE | ECF_CONST | ECF_NORETURN)))
	return false;
    }

  version = SSA_NAME_VERSION (def);
  basevar = SSA_NAME_VAR (def);
  bitmap_set_bit (def_vars, DECL_UID (basevar));

  /* Add this expression to the dependency list for each use partition.  */
  FOR_EACH_SSA_TREE_OPERAND (var, stmt, iter, SSA_OP_USE)
    {
      add_dependence (tab, version, var);
      use_vars = tab->expr_vars[SSA_NAME_VERSION (var)];
      if (use_vars)
	bitmap_ior_into (def_vars, use_vars);
    }
  tab->expr_vars[version] = def_vars;

  /* If there are VUSES, add a dependence on virtual defs.  */
  if (!ZERO_SSA_OPERANDS (stmt, SSA_OP_VUSE))
    {
      add_value_to_version_list (tab, version, VIRTUAL_PARTITION (tab));
      add_value_to_partition_list (tab, VIRTUAL_PARTITION (tab), version);
      bitmap_set_bit (tab->partition_in_use, VIRTUAL_PARTITION (tab));
    }

  return true;
}


/* This function will remove the expression for VERSION from replacement 
   consideration.n table TAB  If 'replace' is true, it is marked as 
   replaceable, otherwise not.  */

static void
finish_expr (temp_expr_table_p tab, int version, bool replace)
{
  int partition;
  unsigned i;
  bitmap_iterator bi;

  gcc_assert (tab->version_info[version]);
  /* Remove this expression from its dependent lists.  The partition dependence
     list is retained and transfered later to whomever uses this version.  */
  EXECUTE_IF_SET_IN_BITMAP (tab->version_info[version], 0, i, bi)
    {
      partition = i;
      gcc_assert (tab->partition_dep_list[partition]);
      remove_value_from_partition_list (tab, partition, version);
      /* Only clear the bit when the dependency list is emptied via 
         a replacement. Otherwise kill_expr will take care of it.  */
      if (!tab->partition_dep_list[partition] && replace)
	bitmap_clear_bit (tab->partition_in_use, partition);
    }
  if (replace)
    {
      tab->saw_replaceable = true;
      bitmap_set_bit (tab->replaceable, version);
    }
  else
    {
      gcc_assert (!bitmap_bit_p (tab->replaceable, version));
      BITMAP_FREE (tab->version_info[version]);
    }
}


/* Mark the expression associated with VAR as replaceable, and enter
   the defining stmt into the version_info table TAB.  */

static void
mark_replaceable (temp_expr_table_p tab, tree var)
{
  int version = SSA_NAME_VERSION (var);
  finish_expr (tab, version, true);

  /* Move the dependence list to the pending list.  */
  if (tab->version_info[version])
    {
      bitmap_ior_into (tab->pending_dependence, tab->version_info[version]);
      BITMAP_FREE (tab->version_info[version]);
    }
  
  /* Set it to the replaceable expression.  */
  tab->version_info[version] = SSA_NAME_DEF_STMT (var);
}


/* This function marks any expression in TAB which is dependent on PARTITION
   as NOT replaceable.  CLEAR_BIT is used to determine whether partition_in_use
   should have its bit cleared.  Since this routine can be called within an
   EXECUTE_IF_SET_IN_BITMAP, the bit can't always be cleared.  */

static inline void
kill_expr (temp_expr_table_p tab, int partition, bool clear_bit)
{
  unsigned i;

  /* Mark every active expr dependent on this var as not replaceable.  
     finish_expr can modify the bitmap, so we can't execute over it.  */
  while (tab->partition_dep_list[partition])
    {
      i = bitmap_first_set_bit (tab->partition_dep_list[partition]);
      finish_expr (tab, i, false);
    }

  gcc_assert (!tab->partition_dep_list[partition]);

  if (clear_bit)
    bitmap_clear_bit (tab->partition_in_use, partition);
}


/* This function kills all expressions in TAB which are dependent on virtual 
   DEFs.  CLEAR_BIT determines whether partition_in_use gets cleared.  */

static inline void
kill_virtual_exprs (temp_expr_table_p tab, bool clear_bit)
{
  kill_expr (tab, VIRTUAL_PARTITION (tab), clear_bit);
}


/* This function processes basic block BB, and looks for variables which can
   be replaced by their expressions.  Results are stored in TAB.  */

static void
find_replaceable_in_bb (temp_expr_table_p tab, basic_block bb)
{
  block_stmt_iterator bsi;
  tree stmt, def, use;
  stmt_ann_t ann;
  int partition;
  var_map map = tab->map;
  ssa_op_iter iter;

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      stmt = bsi_stmt (bsi);
      ann = stmt_ann (stmt);

      /* Determine if this stmt finishes an existing expression.  */
      FOR_EACH_SSA_TREE_OPERAND (use, stmt, iter, SSA_OP_USE)
	{
	  unsigned ver = SSA_NAME_VERSION (use);

	  if (tab->version_info[ver])
	    {
	      bool same_root_var = false;
	      ssa_op_iter iter2;
	      bitmap vars = tab->expr_vars[ver];

	      /* See if the root variables are the same.  If they are, we
		 do not want to do the replacement to avoid problems with
		 code size, see PR tree-optimization/17549.  */
	      FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter2, SSA_OP_DEF)
		{
		  if (bitmap_bit_p (vars, DECL_UID (SSA_NAME_VAR (def))))
		    {
		      same_root_var = true;
		      break;
		    }
		}

	      /* Mark expression as replaceable unless stmt is volatile
		 or DEF sets the same root variable as STMT.  */
	      if (!ann->has_volatile_ops && !same_root_var)
		mark_replaceable (tab, use);
	      else
		finish_expr (tab, ver, false);
	    }
	}
      
      /* Next, see if this stmt kills off an active expression.  */
      FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_DEF)
	{
	  partition = var_to_partition (map, def);
	  if (partition != NO_PARTITION && tab->partition_dep_list[partition])
	    kill_expr (tab, partition, true);
	}

      /* Now see if we are creating a new expression or not.  */
      if (!ann->has_volatile_ops)
	check_replaceable (tab, stmt);

      /* Free any unused dependency lists.  */
      bitmap_clear (tab->pending_dependence);

      /* A V_{MAY,MUST}_DEF kills any expression using a virtual operand.  */
      if (!ZERO_SSA_OPERANDS (stmt, SSA_OP_VIRTUAL_DEFS))
        kill_virtual_exprs (tab, true);
    }
}


/* This function is the driver routine for replacement of temporary expressions
   in the SSA->normal phase, operating on MAP.  If there are replaceable 
   expressions, a table is returned which maps SSA versions to the 
   expressions they should be replaced with.  A NULL_TREE indicates no 
   replacement should take place.  If there are no replacements at all, 
   NULL is returned by the function, otherwise an expression vector indexed
   by SSA_NAME version numbers.  */

extern tree *
find_replaceable_exprs (var_map map)
{
  basic_block bb;
  unsigned i;
  temp_expr_table_p table;
  tree *ret;

  table = new_temp_expr_table (map);
  FOR_EACH_BB (bb)
    {
      bitmap_iterator bi;

      find_replaceable_in_bb (table, bb);
      EXECUTE_IF_SET_IN_BITMAP ((table->partition_in_use), 0, i, bi)
	kill_expr (table, i, false);
      bitmap_clear (table->partition_in_use);

#ifdef ENABLE_CHECKING
  for (i = 0; i < num_ssa_names + 1; i++)
    if (bitmap_bit_p (table->replaceable, i))
      gcc_assert (table->version_info[i] != NULL);
    else
      gcc_assert (table->version_info[i] == NULL);
#endif
    }

  ret = free_temp_expr_table (table);
  return ret;
}


/* Dump TER expression table EXPR to file F.  */

extern void
dump_replaceable_exprs (FILE *f, tree *expr)
{
  tree stmt, var;
  int x;
  fprintf (f, "\nReplacing Expressions\n");
  for (x = 0; x < (int)num_ssa_names + 1; x++)
    if (expr[x])
      {
        stmt = expr[x];
	var = SINGLE_SSA_TREE_OPERAND (stmt, SSA_OP_DEF);
	gcc_assert (var != NULL_TREE);
	print_generic_expr (f, var, TDF_SLIM);
	fprintf (f, " replace with --> ");
	print_generic_expr (f, TREE_OPERAND (stmt, 1), TDF_SLIM);
	fprintf (f, "\n");
      }
  fprintf (f, "\n");
}
