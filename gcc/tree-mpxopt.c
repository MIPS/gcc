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
#include "rtl.h"
#include "expr.h"
#include "tree-pretty-print.h"
#include "gimple-pretty-print.h"
#include "vec.h"
#include "tree-mpx.h"

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

/* Structure to hold check informtation.  */
struct check_info
{
  /* Type of the check.  */
  check_type type;
  /* Address used for the check.  */
  polynomial addr;
  /* Bounds used for the check.  */
  tree bounds;
  /* Check statement.  Can be NULL for removed checks.*/
  gimple stmt;
};

/* Structure to hold checks information for BB.  */
struct bb_checks
{
  vec<struct check_info, va_heap, vl_ptr> checks;
};

static vec<struct bb_checks, va_heap, vl_ptr> check_infos;

static GTY (()) tree mpx_checkl_fndecl;
static GTY (()) tree mpx_checku_fndecl;
static GTY (()) tree mpx_bndmk_fndecl;
static GTY (()) tree mpx_intersect_fndecl;

static int pol_item_compare (const void *i1, const void *i2);
static void add_pol_item (polynomial &pol, tree cst, tree var);
static void sub_pol_item (polynomial &pol, tree cst, tree var);
static void add_pol_pol (polynomial &pol, polynomial &delta);
static void sub_pol_pol (polynomial &pol, polynomial &delta);
static void mult_pol (polynomial &pol, tree mult);
static bool is_constant_pol (const polynomial &pol, int *sign);
static void print_pol (const polynomial &pol);
static void collect_addr_value (tree obj, polynomial &res);
static void collect_value (tree ssa_name, polynomial &res);
static void init_check_info (void);
static void release_check_info (void);
static void mpxopt_init (void);
static void mpxopt_fini (void);
static void fill_check_info (gimple stmt, struct check_info *ci);
static void gather_checks_info (void);
static void remove_check_if_pass (struct check_info *ci);
static void use_outer_bounds_if_possible (struct check_info *ci);
static void remove_excess_intersections (void);
static int get_check_result (struct check_info *ci, tree bounds);
static void remove_constant_checks (void);
static void compare_checks (struct check_info *ci1, struct check_info *ci2, bool postdom);
static void remove_redundant_checks (void);
static void reduce_bounds_lifetime (void);
static tree get_nobnd_fndecl (enum built_in_function fncode);
static void optimize_string_function_calls (void);

/* Comparator for pol_item structures I1 and I2 to be used
   to find items with equal var.  Also used for polynomial
   sorting.  */
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

/* Find plynomial item in POL with var equal to VAR
   and return its index.  Return -1 if item was not
   found.  */
int
pol_find (polynomial &pol, tree var)
{
  int left = 0;
  int right = pol.length () - 1;
  int n;

  while (right >= left)
    {
      n = (left + right) / 2;

      if (pol[n].var == var
	  || (var && pol[n].var
	      && TREE_CODE (var) == ADDR_EXPR
	      && TREE_CODE (pol[n].var) == ADDR_EXPR
	      && TREE_OPERAND (var, 0) == TREE_OPERAND (pol[n].var, 0)))
	return n;
      else if (pol[n].var > var)
	right = n - 1;
      else
	left = n + 1;
    }

  return -1;
}

/* Return constant CST extended to size type.  */
tree
extend_const (tree cst)
{
  if (TYPE_PRECISION (TREE_TYPE (cst)) < TYPE_PRECISION (size_type_node))
    return build_int_cst_type (size_type_node, tree_low_cst (cst, 0));

  return cst;
}

/* Add polynomial item CST * VAR to POL.  */
void
add_pol_item (polynomial &pol, tree cst, tree var)
{
  int n = pol_find (pol, var);

  cst = extend_const (cst);

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
	  && integer_zerop (pol[n].cst))
	pol.ordered_remove (n);
    }
}

/* Subtract polynomial item CST * VAR from POL.  */
void
sub_pol_item (polynomial &pol, tree cst, tree var)
{
  int n = pol_find (pol, var);

  cst = extend_const (cst);

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
	  && integer_zerop (pol[n].cst))
	pol.ordered_remove (n);
    }
}

/* Add polynomial DELTA to POL.  */
void
add_pol_pol (polynomial &pol, polynomial &delta)
{
  unsigned int i;
  for (i = 0; i < delta.length (); i++)
    add_pol_item (pol, delta[i].cst, delta[i].var);
}

/* Subtract polynomial DELTA from POL.  */
void
sub_pol_pol (polynomial &pol, polynomial &delta)
{
  unsigned int i;
  for (i = 0; i < delta.length (); i++)
    sub_pol_item (pol, delta[i].cst, delta[i].var); 
}

/* Mutiply polynomial POL by integer constant MULT.  */
void
mult_pol (polynomial &pol, tree mult)
{
  unsigned int i;
  for (i = 0; i < pol.length (); i++)
    pol[i].cst = fold_build2 (MULT_EXPR, TREE_TYPE (pol[i].cst),
			      pol[i].cst, mult);
}

/* Return 1 if we may prove POL has a constant value with
   determined sign, which is put into *SIGN.  Otherwise
   return 0.  */
bool
is_constant_pol (const polynomial &pol, int *sign)
{
  *sign = 0;

  if (pol.length () == 0)
    return true;
  else if (pol.length () > 1)
    return false;
  else if (pol[0].var)
    return false;
  else if (integer_zerop (pol[0].cst))
    *sign = 0;
  else if  (tree_int_cst_sign_bit (pol[0].cst))
    *sign = -1;
  else
    *sign = 1;

  return true;
}

/* Dump polynomial into dump_file.  */
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
	  if (TREE_CODE (pol[n].cst) != INTEGER_CST
	      || !integer_onep (pol[n].cst))
	    {
	      print_generic_expr (dump_file, pol[n].cst, 0);
	      fprintf (dump_file, " * ");
	    }
	  print_generic_expr (dump_file, pol[n].var, 0);
	}
    }
}

/* Compute value of PTR and put it into polynomial RES.
   PTR has to be ADDR_EXPR.  */
void
collect_addr_value (tree ptr, polynomial &res)
{
  tree obj = TREE_OPERAND (ptr, 0);
  polynomial addr;

  switch (TREE_CODE (obj))
    {
    case INDIRECT_REF:
      collect_value (TREE_OPERAND (obj, 0), res);
      break;

    case MEM_REF:
      collect_value (TREE_OPERAND (obj, 0), res);
      addr.create (0);
      collect_value (TREE_OPERAND (obj, 1), addr);
      add_pol_pol (res, addr);
      addr.release ();
      break;

    case ARRAY_REF:
      collect_value (build_fold_addr_expr (TREE_OPERAND (obj, 0)), res);
      addr.create (0);
      collect_value (TREE_OPERAND (obj, 1), addr);
      mult_pol (addr, array_ref_element_size (obj));
      add_pol_pol (res, addr);
      addr.release ();
      break;

    case COMPONENT_REF:
      {
	tree str = TREE_OPERAND (obj, 0);
	tree field = TREE_OPERAND (obj, 1);
	collect_value (build_fold_addr_expr (str), res);
	addr.create (0);
	collect_value (component_ref_field_offset (obj), addr);
	add_pol_pol (res, addr);
	addr.release ();
	if (DECL_FIELD_BIT_OFFSET (field))
	  {
	    addr.create (0);
	    collect_value (fold_build2 (TRUNC_DIV_EXPR, size_type_node,
					DECL_FIELD_BIT_OFFSET (field),
					size_int (BITS_PER_UNIT)),
			   addr);
	    add_pol_pol (res, addr);
	    addr.release ();
	  }
      }
      break;

    default:
      add_pol_item (res, integer_one_node, ptr);
      break;
    }
}

/* Compute value of PTR and put it into polynomial RES.  */
void
collect_value (tree ptr, polynomial &res)
{
  gimple def_stmt;
  enum gimple_code code;
  enum tree_code rhs_code;
  polynomial addr;
  tree rhs1;

  if (TREE_CODE (ptr) == INTEGER_CST)
    {
      add_pol_item (res, ptr, NULL);
      return;
    }
  else if (TREE_CODE (ptr) == ADDR_EXPR)
    {
      collect_addr_value (ptr, res);
      return;
    }
  else if (TREE_CODE (ptr) != SSA_NAME)
    {
      add_pol_item (res, integer_one_node, ptr);
      return;
    }

  /* Now we handle the case when polynomial is computed
     for SSA NAME.  */
  def_stmt = SSA_NAME_DEF_STMT (ptr);
  code = gimple_code (def_stmt);
	
  /* Currently we do not walk through statements other
     than assignment.  */
  if (code != GIMPLE_ASSIGN)
    {
      add_pol_item (res, integer_one_node, ptr);
      return;
    }

  rhs_code = gimple_assign_rhs_code (def_stmt);
  rhs1 = gimple_assign_rhs1 (def_stmt);

  switch (rhs_code)
    {
    case SSA_NAME:
    case INTEGER_CST:
    case ADDR_EXPR:
      collect_value (rhs1, res);
      break;

    case PLUS_EXPR:
    case POINTER_PLUS_EXPR:
      collect_value (rhs1, res);
      addr.create (0);
      collect_value (gimple_assign_rhs2 (def_stmt), addr);
      add_pol_pol (res, addr);
      addr.release ();
      break;

    case MINUS_EXPR:
      collect_value (rhs1, res);
      addr.create (0);
      collect_value (gimple_assign_rhs2 (def_stmt), addr);
      sub_pol_pol (res, addr);
      addr.release ();
      break;

    case MULT_EXPR:
      if (TREE_CODE (rhs1) == SSA_NAME
	  && TREE_CODE (gimple_assign_rhs2 (def_stmt)) == INTEGER_CST)
	{
	  collect_value (rhs1, res);
	  mult_pol (res, gimple_assign_rhs2 (def_stmt));
	}
      else if (TREE_CODE (gimple_assign_rhs2 (def_stmt)) == SSA_NAME
	       && TREE_CODE (rhs1) == INTEGER_CST)
	{
	  collect_value (gimple_assign_rhs2 (def_stmt), res);
	  mult_pol (res, rhs1);
	}
      else
	add_pol_item (res, integer_one_node, ptr);
      break;

    default:
      add_pol_item (res, integer_one_node, ptr);
      break;
    }
}

/* Fill check_info structure *CI with information about
   check STMT.  */
void
fill_check_info (gimple stmt, struct check_info *ci)
{
  ci->addr.create (0);
  ci->bounds = gimple_call_arg (stmt, 0);
  collect_value (gimple_call_arg (stmt, 1), ci->addr);
  ci->type = (gimple_call_fndecl (stmt) == mpx_checkl_fndecl
	     ? CHECK_LOWER_BOUND
	     : CHECK_UPPER_BOUND);
  ci->stmt = stmt;
}

/* Find all checks in current function and store info about them
   in check_infos.  */
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

	  if (gimple_call_fndecl (stmt) == mpx_checkl_fndecl
	      || gimple_call_fndecl (stmt) == mpx_checku_fndecl)
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

/* Return 1 if check CI against BOUNDS always pass,
   -1 if check CI against BOUNDS always fails and
   0 if we cannot compute check result.  */
int
get_check_result (struct check_info *ci, tree bounds)
{
  gimple bnd_def;
  polynomial bound_val;
  int sign, res = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Trying to compute result of the check\n");
      fprintf (dump_file, "  check: ");
      print_gimple_stmt (dump_file, ci->stmt, 0, 0);
      fprintf (dump_file, "  address: ");
      print_pol (ci->addr);
      fprintf (dump_file, "\n  bounds: ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, "\n");
    }

  if (TREE_CODE (bounds) != SSA_NAME)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: bounds tree code is not ssa_name\n");
      return 0;
    }

  bnd_def = SSA_NAME_DEF_STMT (bounds);
  if (gimple_code (bnd_def) != GIMPLE_CALL
      || gimple_call_fndecl (bnd_def) != mpx_bndmk_fndecl)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: cannot compute bounds value\n");
      return 0;
    }

  bound_val.create (0);
  collect_value (gimple_call_arg (bnd_def, 0), bound_val);
  if (ci->type == CHECK_UPPER_BOUND)
    {
      polynomial size_val;
      size_val.create (0);
      collect_value (gimple_call_arg (bnd_def, 1), size_val);
      add_pol_pol (bound_val, size_val);
      size_val.release ();
      add_pol_item (bound_val, integer_minus_one_node, NULL);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "  bound value: ");
      print_pol (bound_val);
      fprintf (dump_file, "\n");
    }

  sub_pol_pol (bound_val, ci->addr);

  if (!is_constant_pol (bound_val, &sign))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: cannot compute result\n");

      res = 0;
    }
  else if (sign == 0
	   || (ci->type == CHECK_UPPER_BOUND && sign > 0)
	   || (ci->type == CHECK_LOWER_BOUND && sign < 0))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always pass\n");

      res = 1;
    }
  else
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always fail\n");

      res = -1;
    }

  bound_val.release ();

  return res;
}

/* Try to compare bounds value and address value
   used in the check CI.  If we can prove that check
   always pass then remove it.  */
void
remove_check_if_pass (struct check_info *ci)
{
  int result = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Trying to remove check: ");
      print_gimple_stmt (dump_file, ci->stmt, 0, 0);
    }

  result = get_check_result (ci, ci->bounds);

  if (result == 1)
    {
      gimple_stmt_iterator i = gsi_for_stmt (ci->stmt);

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  action: delete check (always pass)\n");

      gsi_remove (&i, true);
      unlink_stmt_vdef (ci->stmt);
      release_defs (ci->stmt);
      ci->stmt = NULL;
    }
  else if (result == -1)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  action: keep check (always fail)\n");
    }
  else if (result == 0)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  action: keep check (cannot compute result)\n");
    }
}

/* For bounds used in CI check if bounds are produced by
   intersection and we may use outer bounds instead.  If
   transformation is possible then fix check statement and
   recompute its info.  */
void
use_outer_bounds_if_possible (struct check_info *ci)
{
  gimple bnd_def;
  tree bnd1, bnd2, bnd_res = NULL;
  int check_res1, check_res2;

  if (TREE_CODE (ci->bounds) != SSA_NAME)
    return;

  bnd_def = SSA_NAME_DEF_STMT (ci->bounds);
  if (gimple_code (bnd_def) != GIMPLE_CALL
      || gimple_call_fndecl (bnd_def) != mpx_intersect_fndecl)
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Check if bounds intersection is redundant: \n");
      fprintf (dump_file, "  check: ");
      print_gimple_stmt (dump_file, ci->stmt, 0, 0);
      fprintf (dump_file, "  intersection: ");
      print_gimple_stmt (dump_file, bnd_def, 0, 0);
      fprintf (dump_file, "\n");
    }

  bnd1 = gimple_call_arg (bnd_def, 0);
  bnd2 = gimple_call_arg (bnd_def, 1);

  check_res1 = get_check_result (ci, bnd1);
  check_res2 = get_check_result (ci, bnd2);
  if (check_res1 == 1)
    bnd_res = bnd2;
  else if (check_res1 == -1)
    bnd_res = bnd1;
  else if (check_res2 == 1)
    bnd_res = bnd1;
  else if (check_res2 == -1)
    bnd_res = bnd2;

  if (bnd_res)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "  action: use ");
	  print_generic_expr (dump_file, bnd2, 0);
	  fprintf (dump_file, " instead of ");
	  print_generic_expr (dump_file, ci->bounds, 0);
	}

      ci->bounds = bnd_res;
      gimple_call_set_arg (ci->stmt, 0, bnd_res);
      update_stmt (ci->stmt);
    }
}

/*  Try to find checks whose bounds were produced by intersection
    which does not affect check result.  In such check outer bounds
    are used instead.  It allows to remove excess intersections
    and helps to compare checks.  */
void
remove_excess_intersections (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for redundant bounds intersections...\n");

  bb = ENTRY_BLOCK_PTR ->next_bb;
  FOR_EACH_BB (bb)
    {
      struct bb_checks *bbc = &check_infos[bb->index];
      unsigned int no;

      /* Iterate throw all found checks in BB.  */
      for (no = 0; no < bbc->checks.length (); no++)
	if (bbc->checks[no].stmt)
	  use_outer_bounds_if_possible (&bbc->checks[no]);
    }
}

/*  Try to remove all checks which are known to alwyas pass.  */
void
remove_constant_checks (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for redundant checks...\n");

  bb = ENTRY_BLOCK_PTR ->next_bb;
  FOR_EACH_BB (bb)
    {
      struct bb_checks *bbc = &check_infos[bb->index];
      unsigned int no;

      /* Iterate throw all found checks in BB.  */
      for (no = 0; no < bbc->checks.length (); no++)
	if (bbc->checks[no].stmt)
	  remove_check_if_pass (&bbc->checks[no]);
    }
}

/* Compare two checks CI1 and CI2 to find redundant one.
   CI1 is known to dominate CI2.  POSTDOM indicated if
   CI2 postdominateds CI1.

   Few conditions are checked to find redundant check:
     1. Checks has the same type
     2. Checks use the same bounds
     3. One check fail means other check fail
     4. Stronger check is always executed if weaker one is executed

   If redundant check is found it is removed. If removed check is CI1
   then CI2 is moved to CI1's position to avoid bound violation happened
   before check is executed.  */
void
compare_checks (struct check_info *ci1, struct check_info *ci2, bool postdom)
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
	fprintf (dump_file, "    Action: delete the second check (same addresses)\n");

      gsi_remove (&i, true);
      unlink_stmt_vdef (ci2->stmt);
      release_defs (ci2->stmt);
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
		fprintf (dump_file, "    Action: delete the second check\n");

	      gsi_remove (&i, true);
	      unlink_stmt_vdef (ci2->stmt);
	      release_defs (ci2->stmt);
	      ci2->stmt = NULL;
	    }
	  else if (postdom)
	    {
	      gimple_stmt_iterator i = gsi_for_stmt (ci2->stmt);
	      gimple_seq seq = NULL;
	      tree addr = gimple_call_arg (ci1->stmt, 1);
	      unsigned int n;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "    Action: replace the first check with the second one\n");

	      gsi_remove (&i, true);
	      unlink_stmt_vdef (ci2->stmt);
	      release_defs (ci2->stmt);
	      ci2->stmt = NULL;

	      for (n = 0; n < delta.length (); n++)
		if (delta[n].var == NULL)
		  {
		    tree tmp = fold_build2 (MINUS_EXPR,
					    TREE_TYPE (delta[n].cst),
					    integer_zero_node,
					    delta[n].cst);
		    addr = fold_build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
					addr, convert_to_ptrofftype (tmp));
		  }
		else if (integer_onep (delta[n].cst))
		  {
		    tree tmp = fold_build2 (MINUS_EXPR,
					    TREE_TYPE (delta[n].var),
					    integer_zero_node,
					    delta[n].var);
		    addr = build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
				   addr, convert_to_ptrofftype (tmp));
		  }
		else if (tree_int_cst_compare (delta[n].cst,
					       integer_minus_one_node) == 0)
		  addr = build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
				 addr, convert_to_ptrofftype (delta[n].var));
		else
		  {
		    tree tmp = fold_build2 (MULT_EXPR,
					    TREE_TYPE (delta[n].var),
					    delta[n].var,
					    delta[n].cst);
		    tmp = fold_build2 (MINUS_EXPR, TREE_TYPE (tmp),
				       integer_zero_node, tmp);
		    addr = fold_build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
					addr, convert_to_ptrofftype (tmp));
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
	  else
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "    Action: skip (the first check is not post-dominanted by the second check)\n");
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

/* Find all pairs of checks where the first check dominates the
   second one and call compare_checks to find and remove redundant
   checks.  */
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
      unsigned int no;

      /* Iterate throw all found checks in BB.  */
      for (no = 0; no < bbc->checks.length (); no++)
	if (bbc->checks[no].stmt)
	  {
	    vec<basic_block> dom_bbs;
	    unsigned bb_no, other;

	    /* Compare check with all other following checks in this BB.  */
	    for (other = no + 1; other < bbc->checks.length (); other++)
	      if (bbc->checks[other].stmt)
		compare_checks (&bbc->checks[no], &bbc->checks[other], true);

	    /* Now compare with checks in BBs dominated by current one.  */
	    dom_bbs = get_all_dominated_blocks (CDI_DOMINATORS, bb);
	    for (bb_no = 0; bb_no < dom_bbs.length (); bb_no++)
	      {
		struct bb_checks *dom_bbc = &check_infos[dom_bbs[bb_no]->index];

		if (dom_bbs[bb_no] == bb)
		  continue;

		for (other = 0; other < dom_bbc->checks.length (); other++)
		  if (dom_bbc->checks[other].stmt)
		    compare_checks (&bbc->checks[no], &dom_bbc->checks[other],
				    dominated_by_p (CDI_POST_DOMINATORS, bb, dom_bbs[bb_no]));
	      }
	  }
    }
}

/* Return fast version of string function FNCODE.  */
tree
get_nobnd_fndecl (enum built_in_function fncode)
{
  switch (fncode)
    {
    case BUILT_IN_MEMCPY:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMCPY_NOBND);

    case BUILT_IN_MEMPCPY:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMPCPY_NOBND);

    case BUILT_IN_MEMMOVE:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMMOVE_NOBND);

    case BUILT_IN_MEMSET:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMSET_NOBND);

    default:
      return NULL_TREE;
    }
}

/* Try to find memcpy, mempcpy, memmove and memset calls which
   are known to not write pointers to memory and use faster
   function versions for them.  */
void
optimize_string_function_calls (void)
{
  basic_block bb;

  /* Check if we are allowed to use fast string functions.  */
  if (!flag_mpx_use_fast_string_functions)
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for replacable string function calls...\n");

  bb = ENTRY_BLOCK_PTR ->next_bb;
  FOR_EACH_BB (bb)
    {
      gimple_stmt_iterator i;

      for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
        {
	  gimple stmt = gsi_stmt (i);
	  tree fndecl;

	  if (gimple_code (stmt) != GIMPLE_CALL)
	    continue;

	  fndecl = gimple_call_fndecl (stmt);

	  if (!fndecl || DECL_BUILT_IN_CLASS (fndecl) != BUILT_IN_NORMAL)
	    continue;

	  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMCPY
	      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMPCPY
	      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMMOVE
	      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMSET)
	    {
	      tree dst = gimple_call_arg (stmt, 0);

	      /* We may replace call corresponding __mpx_*_nobnd call
		 in case destination pointer base type is not
		 void or pointer.  */
	      if (POINTER_TYPE_P (TREE_TYPE (dst))
		  && !VOID_TYPE_P (TREE_TYPE (TREE_TYPE (dst)))
		  && !mpx_type_has_pointer (TREE_TYPE (TREE_TYPE (dst))))
		{
		  tree fndecl_nobnd
		    = get_nobnd_fndecl (DECL_FUNCTION_CODE (fndecl));

		  if (fndecl_nobnd)
		    gimple_call_set_fndecl (stmt, fndecl_nobnd);
		}
	    }
	}
    }
}

/* MPX pass inserts most of bounds creation code in
   the header of the function.  We want to move bounds
   creation closer to bounds usage to reduce bounds
   lifetime.  We also do not want to have bounds creation
   code on paths which do not use them.  */
void
reduce_bounds_lifetime (void)
{
  basic_block bb = FALLTHRU_EDGE (ENTRY_BLOCK_PTR)->dest;
  gimple_stmt_iterator i;

  for (i = gsi_start_bb (bb); !gsi_end_p (i); )
    {
      gimple dom_use, use_stmt, stmt = gsi_stmt (i);
      basic_block dom_bb;
      ssa_op_iter iter;
      imm_use_iterator use_iter;
      use_operand_p use_p;
      tree op;
      bool want_move = false;
      bool deps = false;

      if (gimple_code (stmt) == GIMPLE_CALL
	  && gimple_call_fndecl (stmt) == mpx_bndmk_fndecl)
	want_move = true;

      if (gimple_code (stmt) == GIMPLE_ASSIGN
	  && BOUND_TYPE_P (TREE_TYPE (gimple_assign_lhs (stmt)))
	  && gimple_assign_rhs_code (stmt) == VAR_DECL)
	want_move = true;

      if (!want_move)
	{
	  gsi_next (&i);
	  continue;
	}

      /* Check we do not increase other values lifetime.  */
      FOR_EACH_PHI_OR_STMT_USE (use_p, stmt, iter, SSA_OP_USE)
	{
	  op = USE_FROM_PTR (use_p);

	  if (TREE_CODE (op) == SSA_NAME
	      && gimple_code (SSA_NAME_DEF_STMT (op)) != GIMPLE_NOP)
	    deps = true;
	}

      if (deps)
	{
	  gsi_next (&i);
	  continue;
	}

      /* Check all usages of bounds.  */
      if (gimple_code (stmt) == GIMPLE_CALL)
	op = gimple_call_lhs (stmt);
      else
	{
	  gcc_assert (gimple_code (stmt) == GIMPLE_ASSIGN);
	  op = gimple_assign_lhs (stmt);
	}

      dom_use = NULL;
      dom_bb = NULL;

      FOR_EACH_IMM_USE_STMT (use_stmt, use_iter, op)
	{
	  if (dom_bb &&
	      dominated_by_p (CDI_DOMINATORS,
			      dom_bb, gimple_bb (use_stmt)))
	    {
	      dom_use = use_stmt;
	      dom_bb = NULL;
	    }
	  else if (dom_bb)
	    dom_bb = nearest_common_dominator (CDI_DOMINATORS, dom_bb,
					       gimple_bb (use_stmt));
	  else if (!dom_use)
	    dom_use = use_stmt;
	  else if (stmt_dominates_stmt_p (use_stmt, dom_use))
	    dom_use = use_stmt;
	  else if (!stmt_dominates_stmt_p (dom_use, use_stmt)
		   /* If dom_use and use_stmt are PHI nodes in one BB
		      then it is OK to keep any of them as dom_use.
		      stmt_dominates_stmt_p returns 0 for such
		      combination, so check it here manually.  */
		   && (gimple_code (dom_use) != GIMPLE_PHI
		       || gimple_code (use_stmt) != GIMPLE_PHI
		       || gimple_bb (use_stmt) != gimple_bb (dom_use))
		   )
	    {
	      dom_bb = nearest_common_dominator (CDI_DOMINATORS,
						 gimple_bb (use_stmt),
						 gimple_bb (dom_use));
	      dom_use = NULL;
	    }
	}

      /* In case there is a single use, just move bounds
	 creation to the use.  */
      if (dom_use || dom_bb)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Moving creation of ");
	      print_generic_expr (dump_file, op, 0);
	      fprintf (dump_file, " down to its use.\n");
	    }

	  if (dom_use && gimple_code (dom_use) == GIMPLE_PHI)
	    {
	      dom_bb = get_immediate_dominator (CDI_DOMINATORS,
						gimple_bb (dom_use));
	      dom_use = NULL;
	    }

	  if (dom_bb == bb
	      || (dom_use && gimple_bb (dom_use) == bb))
	    {
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    fprintf (dump_file, "Cannot move statement bacause there is no "
			     "suitable dominator block other than entry block.");

		  gsi_next (&i);
	    }
	  else 
	    {
	      if (dom_bb)
		{
		  gimple_stmt_iterator last = gsi_last_bb (dom_bb);
		  if (!gsi_end_p (last) && stmt_ends_bb_p (gsi_stmt (last)))
		    gsi_move_before (&i, &last);
		  else
		    gsi_move_after (&i, &last);
		}
	      else
		{
		  gimple_stmt_iterator gsi = gsi_for_stmt (dom_use);
		  gsi_move_before (&i, &gsi);
		}

	      update_stmt (stmt);
	    }
	}
      else
	gsi_next (&i);
    }
}

/* Create structures to hold check information
   for current function.  */
void
init_check_info (void)
{
  struct bb_checks empty_bbc;
  int n;

  empty_bbc.checks.create (0);
  empty_bbc.checks.release ();

  release_check_info ();
  
  check_infos.create (n_basic_blocks);
  for (n = 0; n < n_basic_blocks; n++)
    {
      check_infos.safe_push (empty_bbc);
      check_infos.last ().checks.create (0);
    }
}

/* Release structures holding check information
   for current function.  */
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

/* Initilize pass.  */
void
mpxopt_init (void)
{
  check_infos.create (0);

  calculate_dominance_info (CDI_DOMINATORS);
  calculate_dominance_info (CDI_POST_DOMINATORS);
}

/* Finalise pass.  */
void
mpxopt_fini (void)
{
  mpx_fix_cfg ();

  free_dominance_info (CDI_DOMINATORS);
  free_dominance_info (CDI_POST_DOMINATORS);
}

/* Main pass function.  */
unsigned int
mpxopt_execute (void)
{
  mpx_checkl_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_BNDCL);
  mpx_checku_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_BNDCU);
  mpx_bndmk_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_BNDMK);
  mpx_intersect_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_INTERSECT);

  mpxopt_init();

  gather_checks_info ();

  remove_excess_intersections ();

  remove_constant_checks ();

  remove_redundant_checks ();

  optimize_string_function_calls ();

  reduce_bounds_lifetime ();

  release_check_info ();

  mpxopt_fini ();

  return 0;
}

/* Pass gate.  */
bool
mpxopt_gate (void)
{
  return flag_mpx != 0 && (flag_mpxopt > 0
			   || (flag_mpxopt == -1 && optimize > 0));
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
