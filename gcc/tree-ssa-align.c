/* Alignment analysis for trees.
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Daniel Berlin <dberlin@dberlin.org>

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
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "timevar.h"
#include "expr.h"
#include "ggc.h"
#include "langhooks.h"
#include "flags.h"
#include "function.h"
#include "diagnostic.h"
#include "tree-dump.h"
#include "tree-gimple.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-pass.h"
#include "convert.h"
#include "params.h"
#include "tree-ssa-propagate.h"
 
/* Compute alignment/misalignment info for SSA pointers, using
   results of masks, guaranteed alignment properties, or whatever
   other information we can find around that tells us something about
   alignment.  */

/* Possible lattice values.  */
typedef enum
{
  UNINITIALIZED = 0,
  KNOWN,
  UNDEFINED
} latticevalue;


/* Main structure for CCP.  Contains the lattice value and, if it's a
    constant, the constant value.  */
typedef struct
{
  latticevalue lattice_val;
  struct
  {
    unsigned int n;
    unsigned int offset;
  } alignment;
} value;

/* This is used to track the current value of each variable.  */
static value *value_vector;

static void initialize (void);
static void finalize (void);
static enum ssa_prop_result visit_phi_node (tree);
static value cp_lattice_meet (value, value);
static enum ssa_prop_result visit_stmt (tree, edge *, tree *);
static enum ssa_prop_result visit_assignment (tree, tree *);
static void def_to_known_bpu_0 (tree);
static bool set_lattice_value (tree, value);
static value evaluate_stmt (tree);
static void dump_lattice_value (FILE *, const char *, value);
static value *get_value (tree);
static value get_default_value (tree);
 
/* Main entry point for SSA alignment analysis */

static void
compute_ptr_alignment (void)
{
  unsigned int i = 0;
  initialize ();
  ssa_propagate (visit_stmt, visit_phi_node);


  /* Set the alignments */
  for (i = 0; i < num_ssa_names; i++)
    {
      tree ssa_var = ssa_name (i);
      if (ssa_var && POINTER_TYPE_P (TREE_TYPE (ssa_var)))
	{
	  struct ptr_info_def *pi = get_ptr_info (ssa_var);
	  value *val = get_value (ssa_var);
	  if (val->lattice_val == KNOWN)
	    {
	      pi->alignment.n = val->alignment.n;
	      pi->alignment.offset = val->alignment.offset;
	    }
	}
    }
/* Free allocated memory.  */
  finalize ();
   
  /* Debugging dumps.  */
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      dump_referenced_vars (dump_file);
      dump_align_info (dump_file);
      fprintf (dump_file, "\n\n");
    }
}


/* Loop through the PHI_NODE's parameters for BLOCK and compare their
   lattice values to determine PHI_NODE's lattice value.  The value of a
   PHI node is determined calling cp_lattice_meet() with all the arguments
   of the PHI node */

static enum ssa_prop_result
visit_phi_node (tree phi)
{
  value phi_val, *curr_val;
  int i;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting PHI node: ");
      print_generic_expr (dump_file, phi, dump_flags);
    }

  curr_val = get_value (PHI_RESULT (phi));
  phi_val = *curr_val;
  if (phi_val.lattice_val == UNINITIALIZED)
    phi_val.lattice_val = UNDEFINED;

  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      /* Compute the meet operator over all the PHI arguments.  */
      edge e = PHI_ARG_EDGE (phi, i);      
      tree rdef = PHI_ARG_DEF (phi, i);
      value rdef_val;      
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file,
		   "\n    Argument #%d (%d -> %d)\n",
		   i, e->src->index, e->dest->index);
	}      
      if (TREE_CODE (rdef) == INTEGER_CST)
	{
	  rdef_val.lattice_val = KNOWN;
	  rdef_val.alignment.n = TREE_INT_CST_LOW (rdef) * BITS_PER_UNIT;
	  if (rdef_val.alignment.n == 0)
	    rdef_val.alignment.n = 1;
	  rdef_val.alignment.offset = 0;
	}
      else  if (TREE_CODE (rdef) == SSA_NAME)
	{
	  rdef_val = *(get_value (rdef));
	}
      else
	{
	  rdef_val.lattice_val = KNOWN;
	  rdef_val.alignment.n = BITS_PER_UNIT;
	  rdef_val.alignment.offset = 0;
	}
      phi_val = cp_lattice_meet (phi_val, rdef_val);
      
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\t");
	  print_generic_expr (dump_file, rdef, dump_flags);
	  dump_lattice_value (dump_file, "\tValue: ", rdef_val);
	  fprintf (dump_file, "\n");
	}
    }
  
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      dump_lattice_value (dump_file, "\n    PHI node value: ", phi_val);
      fprintf (dump_file, "\n\n");
    }
  
  if (set_lattice_value (PHI_RESULT (phi), phi_val))
    {
      if (phi_val.lattice_val == UNDEFINED)
	return SSA_PROP_VARYING;
      else 
	return SSA_PROP_INTERESTING;
    }
  else
    return SSA_PROP_NOT_INTERESTING;

}

/* Find the greatest common denominator of A and B.  */

static int
gcd (int a, int b)
{ 
  
  int x, y, z;
  
  x = a;
  y = b;
  
  while (x>0)
    {
      z = y % x;
      y = x;
      x = z;
    }
  
  return (y);
}

/* Find the largest power of two alignment such that off1 % newalign
   == off2 % newalign.  */

static unsigned int 
find_largest_common_alignment (unsigned int off1, unsigned int off2)
{
  unsigned int mask;
  mask = ((unsigned HOST_WIDE_INT)1 << ceil_log2 (MIN (off1, off2))) - 1;
  while ((off1 & mask) != (off2 & mask))
    mask >>= 1;
  return mask + 1;
}
/* Compute the meet operator between VAL1 and VAL2:

   any M UNDEFINED = any
   n1, off1 M n2, off2 = n1, off1 if n1 == n2 && off1 == off2
   n1, off1 M n2, off2 = largest_common_alignment (off1 % gcd (n1, n2), off2 % gcd (n1 ,n2))
*/		
		
static value
cp_lattice_meet (value val1, value val2)
{
  value result;

  /* any M UNDEFINED = any.  */
  if (val1.lattice_val == UNDEFINED)
    return val2;
  else if (val2.lattice_val == UNDEFINED)
    return val1;

  if (val1.alignment.n == val2.alignment.n 
      && val1.alignment.offset == val2.alignment.offset)
    {
      result.lattice_val = KNOWN;
      result.alignment.n = val2.alignment.n;
      result.alignment.offset = val2.alignment.offset;
    }
  else
    {
      unsigned int newalign;
      unsigned int newoff1;
      unsigned int newoff2;
      newalign = gcd (val1.alignment.n, val2.alignment.n);
      newoff1 = val1.alignment.offset % newalign;
      newoff2 = val2.alignment.offset % newalign;
      if (newoff1 != newoff2)
	{
	  newalign = find_largest_common_alignment (newoff1, newoff2);
	  newoff1 = newoff1 % newalign;
	  newoff2 = newoff2 % newalign;
	}
      result.lattice_val = KNOWN;
      result.alignment.n = newalign;
      result.alignment.offset = newoff1;
    }

  return result;
}


/* Evaluate statement STMT.  If the statement produces an alignment value and
   its evaluation changes the lattice value of its output, do the following:

   - If the statement is an assignment, add all the SSA edges starting at
   this definition.  */

static enum ssa_prop_result
visit_stmt (tree stmt, edge *taken_edge_p ATTRIBUTE_UNUSED, tree *output_p)
{
  stmt_ann_t ann;
  tree def;
  ssa_op_iter iter;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting statement: ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  ann = stmt_ann (stmt);

  /* Now examine the statement.  If the statement is an assignment that
     produces a single output value, evaluate its RHS to see if the lattice
     value of its output has changed.  */
  if (TREE_CODE (stmt) == MODIFY_EXPR
      && TREE_CODE (TREE_OPERAND (stmt, 0)) == SSA_NAME)
    return visit_assignment (stmt, output_p);
  
  /* Definitions made by statements other than assignments to SSA_NAMEs
     represent unknown modifications to their outputs.  Mark them KNOWN.  */
  FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_DEF)
    def_to_known_bpu_0 (def);
  
  return SSA_PROP_VARYING;
}


/* Visit the assignment statement STMT.  Set the value of its LHS to the
   value computed by the RHS.  */

static enum ssa_prop_result
visit_assignment (tree stmt, tree *output_p)
{
  value val;
  tree lhs, rhs;

  lhs = TREE_OPERAND (stmt, 0);
  rhs = TREE_OPERAND (stmt, 1);
  STRIP_NOPS (rhs);
  
  if (TREE_CODE (rhs) == SSA_NAME)
    {
      /* For a simple copy operation, we copy the lattice values.  */
      value *nval = get_value (rhs);
      val = *nval;
    }
  else
    {
      /* Evaluate the statement.  */
      val = evaluate_stmt (stmt);
    }

  /* Set the lattice value of the statement's output.  */
  if (set_lattice_value (lhs, val))
    {
      *output_p = lhs;
      if (val.lattice_val == UNDEFINED)
	return SSA_PROP_VARYING;
      else
	return SSA_PROP_INTERESTING;
    }
  else
    return SSA_PROP_NOT_INTERESTING;
      
}


/* Evaluate statement STMT.  */

static value
evaluate_stmt (tree stmt)
{
  value val;
  tree rhs;
  rhs = get_rhs (stmt);
  
  if (TREE_CODE (rhs) == INTEGER_CST)
    {
      val.lattice_val = KNOWN;
      val.alignment.n = TREE_INT_CST_LOW (rhs) * BITS_PER_UNIT;
      if (val.alignment.n == 0)
        val.alignment.n = 1;
      val.alignment.offset = 0;
    }
  else if (TREE_CODE (rhs) == PLUS_EXPR || TREE_CODE (rhs) == MINUS_EXPR)
    {
      tree op1 = TREE_OPERAND (rhs, 0);
      tree op2 = TREE_OPERAND (rhs, 1);
      value *op1val = NULL;
      int newoffset = 0;
      if (TREE_CODE (op1) == SSA_NAME)
	op1val = get_value (op1);
      if (TREE_CODE (op2) == INTEGER_CST)
	newoffset = TREE_INT_CST_LOW (op2);
      if (op1val == NULL 
	  || newoffset == 0 
	  || (op1val && op1val->lattice_val == UNDEFINED))
	{
	   val.lattice_val = KNOWN;
	   val.alignment.n = BITS_PER_UNIT;
	   val.alignment.offset = 0;
	}
      else
	{
	  val = *op1val;
	  val.alignment.offset += (BITS_PER_UNIT * newoffset) % val.alignment.n;
	}
    } 
  else
    {
      val.lattice_val = KNOWN;
      val.alignment.n = BITS_PER_UNIT;
      val.alignment.offset = 0;
    }
  return val;
}


/* Debugging dumps.  */

static void
dump_lattice_value (FILE *outf, const char *prefix, value val)
{
  switch (val.lattice_val)
    {
    case UNDEFINED:
      fprintf (outf, "%sUNDEFINED", prefix);
      break;
    case KNOWN:
      fprintf (outf, "%sKNOWN (%d, %d)", prefix, 
	       val.alignment.n, val.alignment.offset);
      break;
    default:
      abort ();
    }
}

/* Initialize local data structures and worklists for CCP.  */

static void
initialize (void)
{
  basic_block bb;

  value_vector = (value *) xmalloc (num_ssa_names * sizeof (value));
  memset (value_vector, 0, num_ssa_names * sizeof (value));

  FOR_EACH_BB (bb)
    {
      tree phi;
      block_stmt_iterator bsi;
      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	DONT_SIMULATE_AGAIN (phi) = false;
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);
	  DONT_SIMULATE_AGAIN (stmt) = false;
	}
    }
      
  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_immediate_uses (dump_file);
}


/* Free allocated storage.  */

static void
finalize (void)
{

  free (value_vector);
}

/* Set the lattice value for the variable VAR to KNOWN, {BITS PER
   UNIT, 0}.  */

static void
def_to_known_bpu_0(tree var)
{
  value val;
  val.lattice_val = KNOWN;
  val.alignment.n = BITS_PER_UNIT;
  val.alignment.offset = 0;
  set_lattice_value (var, val);
}

/* Set the lattice value for variable VAR to VAL.  */

static bool
set_lattice_value (tree var, value val)
{
  value *old = get_value (var);

#ifdef ENABLE_CHECKING
  if (val.lattice_val == UNDEFINED)
    {
      /* (KNOWN->UNDEFINED) is never a valid state transition, unless
	 the default value f the var was known.  */
      if (old->lattice_val == KNOWN 
	  && get_default_value (var).lattice_val  != KNOWN)
	abort ();
    }
#endif  

  if (old->lattice_val != val.lattice_val)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  dump_lattice_value (dump_file,
			      "Lattice value changed to ", val);
	  fprintf (dump_file, ".  Adding definition to SSA edges.\n");
	}
      *old = val;
      return true;
    }
  return false;
}

/* Return a default value for variable VAR using the following rules:

   1- Global and static variables are considered KNOWN,
   {BITS_PER_UNIT, 0}, or the minimum alignment required by their
   underlying type, for pointers.

   2- Function arguments are considered KNOWN, {BITS_PER_UNIT, 0}, or
   the minimum alignment required by their underlying type, for pointers.

   3- Any other value is considered UNDEFINED.  This is useful when
      considering PHI nodes.  PHI arguments that are undefined do not
      change the alignment of the phi.  */

static value
get_default_value (tree var)
{
  value val;
  tree sym;

  if (TREE_CODE (var) == SSA_NAME)
    sym = SSA_NAME_VAR (var);
  else
    {
#ifdef ENABLE_CHECKING
      if (!DECL_P (var))
	abort ();
#endif
      sym = var;
    }

  val.lattice_val = UNDEFINED;
  val.alignment.n = 0;
  val.alignment.offset = 0;
  if (TREE_CODE (sym) == PARM_DECL 
      || (decl_function_context (sym) != current_function_decl
	  || TREE_STATIC (sym)))
    {
      val.lattice_val = KNOWN;
      /* Pointer types are assumed to have the minimum alignment
	 required by their underlying type.  */
      if (POINTER_TYPE_P (TREE_TYPE (sym)))
	val.alignment.n = TYPE_ALIGN (TREE_TYPE (TREE_TYPE (sym)));
      else
	val.alignment.n = BITS_PER_UNIT;
      val.alignment.offset = 0;
    }
  else
    {
      enum tree_code code;
      tree stmt = SSA_NAME_DEF_STMT (var);

      if (stmt && !IS_EMPTY_STMT (stmt))
        {
	  code = TREE_CODE (stmt);
	  if (code != MODIFY_EXPR && code != PHI_NODE)
	    {
	      val.lattice_val = KNOWN;
	      val.alignment.n = BITS_PER_UNIT;
	      val.alignment.offset = 0;
	    }
	}
    }
  return val;
}



struct tree_opt_pass pass_align_analysis = 
{
  "align",				/* name */
  NULL,					/* gate */
  compute_ptr_alignment,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,			/* tv_id */
  PROP_cfg | PROP_ssa,	                /* properties_required */
  PROP_align,			/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0,                                    /* todo_flags_finish */
  0					/* letter */
};
/* Get the lattice + alignment  info associated with var */

static value *
get_value (tree var)
{
  value *val;
  if (TREE_CODE (var) != SSA_NAME)
    abort ();
  val = &value_vector [SSA_NAME_VERSION (var)];
  if (val->lattice_val == UNINITIALIZED)
    *val = get_default_value (var);
  return val;
}

/* Dump alignment information for SSA_NAME PTR into FILE.  */

static void
dump_align_info_for (FILE *file, tree ptr)
{
  struct ptr_info_def *pi = SSA_NAME_PTR_INFO (ptr);

  fprintf (file, "Pointer ");
  print_generic_expr (file, ptr, dump_flags);

  if (pi == NULL)
    return;

  fprintf (file, " alignment n, offset = (%d, %d)\n",
	   pi->alignment.n, pi->alignment.offset);
  
}

/* Dump alignment information into FILE.  NOTE: This function is slow, as
   it needs to traverse the whole CFG looking for pointer SSA_NAMEs.  */

void
dump_align_info (FILE *file)
{
  basic_block bb;
  block_stmt_iterator si;
  size_t i;
  const char *fname =
    lang_hooks.decl_printable_name (current_function_decl, 2);
  ssa_op_iter iter;

  fprintf (file, "\nAlignment info for pointers in %s\n\n", fname);

  /* First dump points-to information for the default definitions of
     pointer variables.  This is necessary because default definitions are
     not part of the code.  */
  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      if (POINTER_TYPE_P (TREE_TYPE (var)))
	{
	  var_ann_t ann = var_ann (var);
	  if (ann->default_def)
	    dump_align_info_for (file, ann->default_def);
	}
    }

  /* Dump points-to information for every pointer defined in the program.  */  
  FOR_EACH_BB (bb)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  tree ptr = PHI_RESULT (phi);
	  if (POINTER_TYPE_P (TREE_TYPE (ptr)))
	    dump_align_info_for (file, ptr);
	}

	for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	  {
            tree stmt = bsi_stmt (si);
            def_operand_p def_p;
 
            FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_DEF)
	      if (POINTER_TYPE_P (TREE_TYPE (DEF_FROM_PTR (def_p))))
	        dump_align_info_for (file, DEF_FROM_PTR (def_p));
	  }
    } 

  fprintf (file, "\n");
}

/* Dump out alignment info for pointers to stderr */
void 
debug_align_info (void)
{
  dump_align_info (stderr);
}

