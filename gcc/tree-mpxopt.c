#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tm_p.h"
#include "basic-block.h"
#include "flags.h"
#include "function.h"
#include "tree-inline.h"
#include "gimple.h"
#include "target.h"
#include "tree-iterator.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "tree-pass.h"
#include "hashtab.h"
#include "diagnostic.h"
#include "demangle.h"
#include "langhooks.h"
#include "ggc.h"
#include "cgraph.h"
#include "gimple.h"
#include "tree-pl.h"
#include "rtl.h"
#include "expr.h"
#include "tree-pretty-print.h"
#include "gimple-pretty-print.h"
#include "vec.h"

enum check_type
{
  CHECK_LOWER_BOUND,
  CHECK_UPPER_BOUND
};

struct pol_item
{
  tree cst;
  tree var;
};

typedef vec<struct pol_item> polynomial;

struct check_info
{
  check_type type;
  polynomial addr;
  tree bounds;
  gimple stmt;
};

struct bb_checks
{
  vec<struct check_info, va_heap, vl_ptr> checks;
};

static vec<struct bb_checks, va_heap, vl_ptr> check_infos;

static GTY (()) tree pl_checkl_fndecl;
static GTY (()) tree pl_checku_fndecl;

static int pol_item_compare (const void *i1, const void *i2);
static void add_pol_item (polynomial &pol, tree cst, tree var);
static void sub_pol_item (polynomial &pol, tree cst, tree var);
static void add_pol_pol (polynomial &pol, polynomial &delta);
static void sub_pol_pol (polynomial &pol, polynomial &delta);
static void mult_pol (polynomial &pol, tree mult);
static bool is_constant_pol (const polynomial &pol, int *sign);
static void print_pol (const polynomial &pol);
static void collect_value (tree ssa_name, polynomial &res);
static void init_check_info (void);
static void release_check_info (void);
static void mpxopt_init (void);
static void fill_check_info (gimple stmt, struct check_info *ci);
static void gather_checks_info (void);
static void remove_constant_checks (void);
static void compare_checks (struct check_info *ci1, struct check_info *ci2);
static void remove_redundant_checks (void);

int
pol_item_compare (const void *i1, const void *i2)
{
  const struct pol_item *p1 = (const struct pol_item *)i1;
  const struct pol_item *p2 = (const struct pol_item *)i2;

  if (p1->var == p2->var)
    return 0;
  else if (p1->var > p2->var)
    return 1;
  else
    return -1;
}

int
pol_find (polynomial &pol, tree var)
{
  int left = 0;
  int right = pol.length () - 1;
  int n;

  while (right >= left)
    {
      n = (left + right) / 2;

      if (pol[n].var == var)
	return n;
      else if (pol[n].var > var)
	right = n - 1;
      else
	left = n + 1;
    }

  return -1;
}

void
add_pol_item (polynomial &pol, tree cst, tree var)
{
  int n = pol_find (pol, var);
  if (n < 0)
    {
      struct pol_item item;
      item.cst = cst;
      item.var = var;
      
      pol.safe_push (item);
      pol.qsort (&pol_item_compare);
    }
  else
    {
      pol[n].cst = fold_build2 (PLUS_EXPR, TREE_TYPE (pol[n].cst),
				pol[n].cst, cst);
      if (TREE_CODE (pol[n].cst) == INTEGER_CST
	  && tree_int_cst_compare (pol[n].cst, integer_zero_node) == 0)
	pol.ordered_remove (n);
    }
}

void
sub_pol_item (polynomial &pol, tree cst, tree var)
{
  int n = pol_find (pol, var);
  if (n < 0)
    {
      struct pol_item item;
      item.cst = fold_build2 (MINUS_EXPR, TREE_TYPE (cst),
			      integer_zero_node, cst);
      item.var = var;
      
      pol.safe_push (item);
      pol.qsort (&pol_item_compare);
    }
  else
    {
      pol[n].cst = fold_build2 (MINUS_EXPR, TREE_TYPE (pol[n].cst),
				pol[n].cst, cst);
      if (TREE_CODE (pol[n].cst) == INTEGER_CST
	  && tree_int_cst_compare (pol[n].cst, integer_zero_node) == 0)
	pol.ordered_remove (n);
    }
}

void
add_pol_pol (polynomial &pol, polynomial &delta)
{
  unsigned int i;
  for (i = 0; i < delta.length (); i++)
    add_pol_item (pol, delta[i].cst, delta[i].var);
}

void
sub_pol_pol (polynomial &pol, polynomial &delta)
{
  unsigned int i;
  for (i = 0; i < delta.length (); i++)
    sub_pol_item (pol, delta[i].cst, delta[i].var); 
}

void
mult_pol (polynomial &pol, tree mult)
{
  unsigned int i;
  for (i = 0; i < pol.length (); i++)
    pol[i].cst = fold_build2 (MULT_EXPR, TREE_TYPE (pol[i].cst),
			      pol[i].cst, mult);
}

bool
is_constant_pol (const polynomial &pol, int *sign)
{
  *sign = 0;

  if (pol.length () > 1)
    return false;
  else if (pol[0].var)
    return false;
  else
    *sign = tree_int_cst_sgn (pol[0].cst);

  return true;
}

void
print_pol (const polynomial &pol)
{
  unsigned int n = 0;
  for (n = 0; n < pol.length (); n++)
    {
      if (n > 0)
	fprintf (dump_file, " + ");

      if (pol[n].var == NULL_TREE)
	print_generic_expr (dump_file, pol[n].cst, 0);
      else
	{
	  //if (TREE_CODE (pol[n].cst) != INTEGER_CST
	  //    || tree_int_cst_compare (pol[n].cst, integer_one_node) != 0)
	    {
	      print_generic_expr (dump_file, pol[n].cst, 0);
	      fprintf (dump_file, " * ");
	    }
	  print_generic_expr (dump_file, pol[n].var, 0);
	}
    }
}

void
collect_value (tree ssa_name, polynomial &res)
{
  gimple def_stmt;
  enum gimple_code code;
  enum tree_code rhs_code;
  polynomial addr;

  if (TREE_CODE (ssa_name) == INTEGER_CST)
    {
      add_pol_item (res, ssa_name, NULL);
      return;
    }
  
  if (TREE_CODE (ssa_name) != SSA_NAME)
    {
      add_pol_item (res, integer_one_node, ssa_name);
      return;
    }

  def_stmt = SSA_NAME_DEF_STMT (ssa_name);
  code = gimple_code (def_stmt);
	
  /* Do not walk through statements other than assignment.  */
  if (code != GIMPLE_ASSIGN)
    {
      add_pol_item (res, integer_one_node, ssa_name);
      return;
    }

  rhs_code = gimple_assign_rhs_code (def_stmt);

  switch (rhs_code)
    {
    case SSA_NAME:
      collect_value (gimple_assign_rhs1 (def_stmt), res);
      break;

    case INTEGER_CST:
      collect_value (gimple_assign_rhs1 (def_stmt), res);
      break;

    case PLUS_EXPR:
    case POINTER_PLUS_EXPR:
      collect_value (gimple_assign_rhs1 (def_stmt), res);
      addr.create (0);
      collect_value (gimple_assign_rhs2 (def_stmt), addr);
      add_pol_pol (res, addr);
      addr.release ();
      break;

    case MINUS_EXPR:
      collect_value (gimple_assign_rhs1 (def_stmt), res);
      addr.create (0);
      collect_value (gimple_assign_rhs2 (def_stmt), addr);
      sub_pol_pol (res, addr);
      addr.release ();
      break;

    case MULT_EXPR:
      if (TREE_CODE (gimple_assign_rhs1 (def_stmt)) == SSA_NAME
	  && TREE_CODE (gimple_assign_rhs2 (def_stmt)) == INTEGER_CST)
	{
	  collect_value (gimple_assign_rhs1 (def_stmt), res);
	  mult_pol (res, gimple_assign_rhs2 (def_stmt));
	}
      else if (TREE_CODE (gimple_assign_rhs2 (def_stmt)) == SSA_NAME
	       && TREE_CODE (gimple_assign_rhs1 (def_stmt)) == INTEGER_CST)
	{
	  collect_value (gimple_assign_rhs2 (def_stmt), res);
	  mult_pol (res, gimple_assign_rhs1 (def_stmt));
	}
      else
	add_pol_item (res, integer_one_node, ssa_name);
      break;

    default:
      add_pol_item (res, integer_one_node, ssa_name);
      break;
    }
}

void
fill_check_info (gimple stmt, struct check_info *ci)
{
  ci->addr.create (0);
  ci->bounds = gimple_call_arg (stmt, 0);
  collect_value (gimple_call_arg (stmt, 1), ci->addr);
  ci->type = (gimple_call_fndecl (stmt) == pl_checkl_fndecl
	     ? CHECK_LOWER_BOUND
	     : CHECK_UPPER_BOUND);
  ci->stmt = stmt;
}

void
gather_checks_info (void)
{
  basic_block bb;
  gimple_stmt_iterator i;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Gathering information about checks...\n");

  init_check_info ();

  bb = ENTRY_BLOCK_PTR ->next_bb;
  FOR_EACH_BB (bb)
    {
      struct bb_checks *bbc = &check_infos[bb->index];

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Searching checks in BB%d...\n", bb->index);

      for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
        {
	  gimple stmt = gsi_stmt (i);

	  if (gimple_code (stmt) != GIMPLE_CALL)
	    continue;

	  if (gimple_call_fndecl (stmt) == pl_checkl_fndecl
	      || gimple_call_fndecl (stmt) == pl_checku_fndecl)
	    {
	      struct check_info ci;

	      fill_check_info (stmt, &ci);
	      bbc->checks.safe_push (ci);

	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "Adding check information:\n");
		  fprintf (dump_file, "  bounds: ");
		  print_generic_expr (dump_file, ci.bounds, 0);
		  fprintf (dump_file, "\n  address: ");
		  print_pol (ci.addr);
		  fprintf (dump_file, "\n  check: ");
		  print_gimple_stmt (dump_file, stmt, 0, 0);
		}
	    }
	}
    }
}

void
remove_constant_checks (void)
{
}

void
compare_checks (struct check_info *ci1, struct check_info *ci2)
{
  polynomial delta;
  int sign;

  if (ci1->type != ci2->type)
    return;

  if (ci1->bounds != ci2->bounds)
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "  Comparing checks...\n");
      fprintf (dump_file, "    First check: ");
      print_gimple_stmt (dump_file, ci1->stmt, 0, 0);
      fprintf (dump_file, "    Second check: ");
      print_gimple_stmt (dump_file, ci2->stmt, 0, 0);
    }

  delta = ci1->addr.copy ();
  sub_pol_pol (delta, ci2->addr);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "    Delta: ");
      print_pol (delta);
      fprintf (dump_file, "\n");
    }

  if (delta.length () == 0)
    {
      gimple_stmt_iterator i = gsi_for_stmt (ci2->stmt);

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "    Action: delete second check (same addresses)\n");

      gsi_remove (&i, true);
      ci2->stmt = NULL;
    }
  else if (!is_constant_pol (delta, &sign))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "    Action: skip (delta is not constant)\n");
    }
  else
    {
      if (sign)
	{
	  if ((sign > 0 && ci1->type == CHECK_UPPER_BOUND)
	      || (sign < 0 && ci1->type == CHECK_LOWER_BOUND))
	    {
	      gimple_stmt_iterator i = gsi_for_stmt (ci2->stmt);

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "    Action: delete second check\n");

	      gsi_remove (&i, true);
	      ci2->stmt = NULL;
	    }
	  else
	    {
	      gimple_stmt_iterator i = gsi_for_stmt (ci2->stmt);
	      gimple_seq seq = NULL;
	      tree addr = gimple_call_arg (ci1->stmt, 1);
	      unsigned int n;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "    Action: replace the first check with the second one\n");

	      gsi_remove (&i, true);
	      ci2->stmt = NULL;

	      for (n = 0; n < delta.length (); n++)
		if (delta[n].var == NULL)
		  {
		    tree tmp = fold_build2 (MINUS_EXPR,
					    TREE_TYPE (delta[n].cst),
					    integer_zero_node,
					    delta[n].cst);
		    addr = fold_build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
					addr, tmp);
		  }
		else if (tree_int_cst_compare (delta[n].cst,
					       integer_one_node) == 0)
		  {
		    tree tmp = fold_build2 (MINUS_EXPR,
					    TREE_TYPE (delta[n].var),
					    integer_zero_node,
					    delta[n].var);
		    addr = build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
				   addr, tmp);
		  }
		else if (tree_int_cst_compare (delta[n].cst,
					       integer_minus_one_node) == 0)
		  addr = build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
				 addr, delta[n].var);
		else
		  {
		    tree tmp = fold_build2 (MULT_EXPR,
					    TREE_TYPE (delta[n].var),
					    delta[n].var,
					    delta[n].cst);
		    tmp = fold_build2 (MINUS_EXPR, TREE_TYPE (tmp),
				       integer_zero_node, tmp);
		    addr = fold_build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
					addr, tmp);
		  }

	      addr = force_gimple_operand (unshare_expr (addr), &seq,
					   true, NULL_TREE);

	      i = gsi_for_stmt (ci1->stmt);
	      gsi_insert_seq_before (&i, seq, GSI_CONTINUE_LINKING);
	      gimple_call_set_arg (ci1->stmt, 1, addr);
	      update_stmt (ci1->stmt);

	      ci1->addr.release ();
	      fill_check_info (ci1->stmt, ci1);
	    }
	}
      else
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "    Action: skip (cannot compute sign of delta)\n");
	}
    }

  delta.release ();
}

void
remove_redundant_checks (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for redundant checks...\n");

  bb = ENTRY_BLOCK_PTR ->next_bb;
  FOR_EACH_BB (bb)
    {
      struct bb_checks *bbc = &check_infos[bb->index];
      unsigned int no, other;

      for (no = 0; no < bbc->checks.length (); no++)
	if (bbc->checks[no].stmt)
	  for (other = no + 1; other < bbc->checks.length (); other++)
	    if (bbc->checks[other].stmt)
	      compare_checks (&bbc->checks[no], &bbc->checks[other]);
    }
}

void
init_check_info (void)
{
  struct bb_checks empty_bbc;
  int n;

  release_check_info ();
  
  check_infos.create (n_basic_blocks);
  for (n = 0; n < n_basic_blocks; n++)
    {
      check_infos.safe_push (empty_bbc);
      check_infos.last ().checks.create (0);
    }
}

void
release_check_info (void)
{
  unsigned int n, m;

  if (check_infos.exists ())
    {
      for (n = 0; n < check_infos.length (); n++)
	{
	  for (m = 0; m < check_infos[n].checks.length (); m++)
	    if (check_infos[n].checks[m].addr.exists ())
	      check_infos[n].checks[m].addr.release ();
	  check_infos[n].checks.release ();
	}
      check_infos.release ();
    }
}

void
mpxopt_init ()
{
  check_infos.create (0);
}

unsigned int
mpxopt_execute (void)
{
  pl_checkl_fndecl = targetm.builtin_pl_function (BUILT_IN_PL_BNDCL);
  pl_checku_fndecl = targetm.builtin_pl_function (BUILT_IN_PL_BNDCU);

  mpxopt_init();

  gather_checks_info ();

  remove_constant_checks ();

  remove_redundant_checks ();

  release_check_info ();

  return 0;
}

bool
mpxopt_gate (void)
{
  return flag_pl != 0 && optimize;
}


struct gimple_opt_pass pass_mpxopt =
{
 {
  GIMPLE_PASS,
  "mpxopt",                             /* name */
  OPTGROUP_NONE,                        /* optinfo_flags */
  mpxopt_gate,                          /* gate */
  mpxopt_execute,                       /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_NONE,                              /* tv_id */
  PROP_ssa | PROP_cfg,                  /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_verify_flow | TODO_verify_stmts
  | TODO_update_ssa                     /* todo_flags_finish */
 }
};
