/* Scalar evolution detector.
   Copyright (C) 2003, 2004 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <s.pop@laposte.net>

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* 
   Description: 
   
     This pass analyzes the evolution of scalar variables in loop
     structures.  The algorithm is based on the SSA representation,
     and on the loop hierarchy tree.  This algorithm is not based on
     the notion of versions of a variable, as it was the case for the
     previous implementations of the scalar evolution algorithm, but
     it assumes that each defined name is unique.
     
     A short sketch of the algorithm is:
     
     Given a scalar variable to be analyzed, follow the SSA edge to
     its definition:
     
     - When the definition is a MODIFY_EXPR: if the right hand side
     (RHS) of the definition cannot be statically analyzed, the answer
     of the analyzer is: "don't know", that corresponds to the
     conservative [-oo, +oo] element of the lattice of intervals.
     Otherwise, for all the variables that are not yet analyzed in the
     RHS, try to determine their evolution, and finally try to
     evaluate the operation of the RHS that gives the evolution
     function of the analyzed variable.

     - When the definition is a condition-phi-node: determine the
     evolution function for all the branches of the phi node, and
     finally merge these evolutions (see chrec_merge).

     - When the definition is a loop-phi-node: determine its initial
     condition, that is the SSA edge defined in an outer loop, and
     keep it symbolic.  Then determine the SSA edges that are defined
     in the body of the loop.  Follow the inner edges until ending on
     another loop-phi-node of the same analyzed loop.  If the reached
     loop-phi-node is not the starting loop-phi-node, then we keep
     this definition under a symbolic form.  If the reached
     loop-phi-node is the same as the starting one, then we compute a
     symbolic stride on the return path.  The result is then the
     symbolic chrec {initial_condition, +, symbolic_stride}_loop.

   Examples:
   
     Example 1: Illustration of the basic algorithm.
   
     | a = 3
     | loop_1
     |   b = phi (a, c)
     |   c = b + 1
     |   if (c > 10) exit_loop
     | endloop
   
     Suppose that we want to know the number of iterations of the
     loop_1.  The exit_loop is controlled by a COND_EXPR (c > 10).  We
     ask the scalar evolution analyzer two questions: what's the
     scalar evolution (scev) of "c", and what's the scev of "10".  For
     "10" the answer is "10" since it is a scalar constant.  For the
     scalar variable "c", it follows the SSA edge to its definition,
     "c = b + 1", and then asks again what's the scev of "b".
     Following the SSA edge, we end on a loop-phi-node "b = phi (a,
     c)", where the initial condition is "a", and the inner loop edge
     is "c".  The initial condition is kept under a symbolic form (it
     may be the case that the copy constant propagation has done its
     work and we end with the constant "3" as one of the edges of the
     loop-phi-node).  The update edge is followed to the end of the
     loop, and until reaching again the starting loop-phi-node: b -> c
     -> b.  At this point we have drawn a path from "b" to "b" from
     which we compute the stride in the loop: in this example it is
     "+1".  The resulting scev for "b" is "b -> {a, +, 1}_1".  Now
     that the scev for "b" is known, it is possible to compute the
     scev for "c", that is "c -> {a + 1, +, 1}_1".  In order to
     determine the number of iterations in the loop_1, we have to
     instantiate_parameters ({a + 1, +, 1}_1), that gives after some
     more analysis the scev {4, +, 1}_1, or in other words, this is
     the function "f (x) = x + 4", where x is the iteration count of
     the loop_1.  Now we have to solve the inequality "x + 4 > 10",
     and take the smallest iteration number for which the loop is
     exited: x = 7.  This loop runs from x = 0 to x = 7, and in total
     there are 8 iterations.  In terms of loop normalization, we have
     created a variable that is implicitly defined, "x" or just "_1",
     and all the other analyzed scalars of the loop are defined in
     function of this variable:
   
     a -> 3
     b -> {3, +, 1}_1
     c -> {4, +, 1}_1
     
     or in terms of a C program: 
     
     | a = 3
     | for (x = 0; x <= 7; x++)
     |   {
     |     b = x + 3
     |     c = x + 4
     |   }
     
     Example 2: Illustration of the algorithm on nested loops.
     
     | loop_1
     |   a = phi (1, b)
     |   c = a + 2
     |   loop_2  10 times
     |     b = phi (c, d)
     |     d = b + 3
     |   endloop
     | endloop
     
     For analyzing the scalar evolution of "a", the algorithm follows
     the SSA edge into the loop's body: "a -> b".  "b" is an inner
     loop-phi-node, and its analysis as in Example 1, gives: 
     
     b -> {c, +, 3}_2
     d -> {c + 3, +, 3}_2
     
     Following the SSA edge for the initial condition, we end on "c = a
     + 2", and then on the starting loop-phi-node "a".  From this point,
     the loop stride is computed: back on "c = a + 2" we get a "+2" in
     the loop_1, then on the loop-phi-node "b" we compute the overall
     effect of the inner loop that is "b = c + 30", and we get a "+30"
     in the loop_1.  That means that the overall stride in loop_1 is
     equal to "+32", and the result is: 
     
     a -> {1, +, 32}_1
     c -> {3, +, 32}_1
     
     Example 3: Higher degree polynomials.
     
     | loop_1
     |   a = phi (2, b)
     |   c = phi (5, d)
     |   b = a + 1
     |   d = c + a
     | endloop
     
     a -> {2, +, 1}_1
     b -> {3, +, 1}_1
     c -> {5, +, a}_1
     d -> {5 + a, +, a}_1
     
     instantiate_parameters ({5, +, a}_1) -> {5, +, 2, +, 1}_1
     instantiate_parameters ({5 + a, +, a}_1) -> {7, +, 3, +, 1}_1
     
     Example 4: Lucas, Fibonacci, or mixers in general.
     
     | loop_1
     |   a = phi (1, b)
     |   c = phi (3, d)
     |   b = c
     |   d = c + a
     | endloop
     
     a -> (1, c)_1
     c -> {3, +, a}_1
     
     The syntax "(1, c)_1" stands for a PEELED_CHREC that has the
     following semantics: during the first iteration of the loop_1, the
     variable contains the value 1, and then it contains the value "c".
     Note that this syntax is close to the syntax of the loop-phi-node:
     "a -> (1, c)_1" vs. "a = phi (1, c)".
     
     The symbolic chrec representation contains all the semantics of the
     original code.  What is more difficult is to use this information.
     
     Example 5: Flip-flops, or exchangers.
     
     | loop_1
     |   a = phi (1, b)
     |   c = phi (3, d)
     |   b = c
     |   d = a
     | endloop
     
     a -> (1, c)_1
     c -> (3, a)_1
     
     Based on these symbolic chrecs, it is possible to refine this
     information into the more precise PERIODIC_CHRECs: 
     
     a -> |1, 3|_1
     c -> |3, 1|_1
     
     This transformation is not yet implemented.
     
   Further readings:
   
     You can find a more detailed description of the algorithm in:
     http://icps.u-strasbg.fr/~pop/DEA_03_Pop.pdf
     http://icps.u-strasbg.fr/~pop/DEA_03_Pop.ps.gz.  But note that
     this is a preliminary report and some of the details of the
     algorithm have changed.  I'm working on a research report that
     updates the description of the algorithms to reflect the design
     choices used in this implementation.
     
   Fixmes:
   
     FIXME taylor: This FIXME concerns all the cases where we have to
     deal with additions of exponential functions: "exp + exp" or
     "poly + exp" or "cst + exp".  This could be handled by a Taylor
     decomposition of the exponential function, but this is still
     under construction (not implemented yet, or chrec_top).
     
     The idea is to represent the exponential evolution functions
     using infinite degree polynomials:
     
     | a -> {1, *, 2}_1 = {1, +, 1, +, 1, +, ...}_1 = {1, +, a}_1
     
     Proof:
     \begin{eqnarray*}
     \{1, *, t+1\} (x) &=& exp \left(log (1) + log (t+1) \binom{x}{1} \right) \\
                       &=& (t+1)^x \\
                       &=& \binom{x}{0} + \binom{x}{1}t + \binom{x}{2}t^2 + 
                           \ldots + \binom{x}{x}t^x \\
                       &=& \{1, +, t, +, t^2, +, \ldots, +, t^x\} \\
     \end{eqnarray*}
     
     While this equality is simple to prove for exponentials of degree
     1, it is still work in progress for higher degree exponentials.
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"

/* These RTL headers are needed for basic-block.h.  */
#include "rtl.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "timevar.h"
#include "cfgloop.h"
#include "tree-fold-const.h"
#include "tree-chrec.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "tree-pass.h"
#include "tree-vectorizer.h"
#include "flags.h"


static bool loop_phi_node_p (tree);
static tree compute_overall_effect_of_inner_loop (tree);


static struct scev_info_str *find_var_scev_info (tree);
static void set_scalar_evolution (tree, tree);
static void set_scalar_evolution_outer_value (tree, tree);
static tree get_scalar_evolution (unsigned, tree);

static tree select_outer_and_current_evolutions (unsigned, tree);
static tree build_polynomial_evolution_in_loop (unsigned, tree, tree);
static tree build_exponential_evolution_in_loop (unsigned, tree, tree);
static tree add_to_evolution (unsigned, tree, tree);
static tree multiply_evolution (unsigned, tree, tree);
static inline tree cannot_analyze_loop_nb_iterations_yet (void);
static inline tree set_nb_iterations_in_loop (struct loop *, tree);
static tree homogenize_integer_cst_types_for_nb_iter (tree);


static bool analyzable_condition       (tree);
static void get_exit_conditions_rec    (struct loop *, varray_type *);
static void select_loops_exit_conditions (struct loops *, varray_type *);

static bool follow_ssa_edge_in_rhs (unsigned, tree, tree, tree *);
static bool follow_ssa_edge_in_condition_phi (unsigned, tree, tree, tree *);
static bool follow_ssa_edge_inner_loop_phi (unsigned, tree, tree, tree *);
static bool follow_ssa_edge (unsigned, tree, tree, tree *);
static tree analyze_evolution_in_loop  (tree, tree);
static tree analyze_initial_condition  (tree);
static tree interpret_loop_phi (unsigned, tree);
static tree interpret_condition_phi (unsigned, tree);
static tree interpret_rhs_modify_expr (unsigned, tree);


/* The following trees are unique elements.  Thus the comparison of
   another element to these elements should be done on the pointer to
   these trees, and not on their value.  */

/* Qualifies all the AST nodes that are not yet analyzed.  */
tree chrec_not_analyzed_yet;

/* Reserved to the cases where the analyzer has detected an
   undecidable property at compile time.  */
tree chrec_top;

/* When the analyzer has detected that a property will never
   happen, then it qualifies it with chrec_bot.  */
tree chrec_bot;

static struct loops *current_loops;
static varray_type *scalar_evolution_info;
static varray_type *already_instantiated;
static varray_type scalar_evolution_info_st;
static varray_type already_instantiated_st;

/* Flag to indicate availability of dependency info.  */
bool dd_info_available;



/* Loop related functions.  */

static inline bool stmt_is_in_loop (tree, unsigned);

/* Determines whether STMT is in loop LOOP_NB.  */

static inline bool
stmt_is_in_loop (tree stmt, unsigned loop_nb)
{
  return flow_bb_inside_loop_p (loop_from_num (current_loops, loop_nb),
				bb_for_stmt (stmt));
}

/* Determines whether STMT is not contained by the loop LOOP_NB.  */

static inline bool
stmt_is_not_in_loop (tree stmt, unsigned loop_nb)
{
  return !stmt_is_in_loop (stmt, loop_nb);
}

/* Determines whether loop A is contained in loop B.  */

static bool
loop_is_included_in (unsigned a, unsigned b)
{
  if (a == b)
    return true;
  
  return flow_loop_nested_p (loop_from_num (current_loops, b),
			     loop_from_num (current_loops, a));
}

/* Determines whether loop A is strictly contained in loop B.  */

static bool
loop_is_strictly_included_in (unsigned a, unsigned b)
{
  if (a == b)
    return false;
  
  return flow_loop_nested_p (loop_from_num (current_loops, b),
			     loop_from_num (current_loops, a));
}


/* This section contains the interface to the SSA IR.  */

/* This function determines whether PHI is a loop-phi-node.  Otherwise
   it is a condition-phi-node.  */

static bool
loop_phi_node_p (tree phi)
{
  /* The implementation of this function is based on the following
     property: "all the loop-phi-nodes of a loop are contained in the
     loop's header basic block".  */
  tree it;
  
  for (it = phi_nodes (loop_header (loop_of_stmt (phi))); 
       it != NULL_TREE; 
       it = TREE_CHAIN (it))
    if (it == phi)
      {
	int i;
	bool found_initial_condition = false;
	bool found_update = false;
	
	/* This is a loop-phi-node only if one of its edges is an
	   initial condition and one other edge points to the body of
	   the loop, or in other words, an updated definition. */
	for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	  {
	    tree upper_chain;
	    tree branch = PHI_ARG_DEF (phi, i);
	    
	    switch (TREE_CODE (branch))
	      {
	      case SSA_NAME:
		upper_chain = SSA_NAME_DEF_STMT (branch);
		
		/* When the phi node has a NOP_EXPR or a NULL_TREE
		   argument, or if it is not defined in a loop
		   contained in the loop where the phi is defined,
		   then this argument is the initial condition.  */
		if (upper_chain == NULL_TREE 
		    || TREE_CODE (upper_chain) == NOP_EXPR
		    || !loop_is_included_in 
		    (loop_num (loop_of_stmt (upper_chain)), 
		     loop_num (loop_of_stmt (phi))))
		  {
		    if (found_update)
		      return true;
		    else
		      found_initial_condition = true;
		  }
		
		/* Otherwise, the branch is oriented to the loop's
		   body.  */
		else
		  {
		    if (found_initial_condition)
		      return true;
		    else
		      found_update = true;
		  }
		break;
		
	      default:
		if (found_update)
		  return true;
		else
		  found_initial_condition = true;
		break;
	      }
	  }
      }
  
  return false;
}

/* Compute the overall effect of a loop on a variable. 
   1. compute the number of iterations in the loop,
   2. compute the value of the variable after crossing the loop.  

   Example:  
   
   | i_0 = ...
   | loop 10 times
   |   i_1 = phi (i_0, i_2)
   |   i_2 = i_1 + 2
   | endloop
   
   This loop has the same effect as:
   
   | i_1 = i_0 + 20
*/

static tree 
compute_overall_effect_of_inner_loop (tree version)
{
  tree res;
  tree nb_iter;
  struct loop *loop = loop_of_stmt (SSA_NAME_DEF_STMT (version));
  unsigned loop_nb;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "(compute_overall_effect_of_inner_loop \n");
  
  nb_iter = number_of_iterations_in_loop (loop);
  
  /* When the number of iterations is not known, set the evolution to
     chrec_top.  As an example, consider the following loop:
     
     | i = 5
     | loop 
     |   i = i + 1
     |   loop chrec_top times
     |     i = i + 3
     |   endloop
     | endloop
     
     since it is impossible to know the number of iterations in the
     inner loop, the evolution of i in the outer loop becomes unknown:
     
     | i = 5
     | loop 
     |   i = i + 1
     |   i = i + chrec_top
     | endloop
  */
  if (nb_iter == chrec_top)
    res = chrec_top;
  
  else
    {
      /* An example: given the evolution 
	 "{{22, +, 4}_1, +, [1,3]}_2", 
	 
	 and the fact that the loop 2 runs exactly 6 times, the
	 overall effect is obtained by evaluating:
	 
	 "({{22, +, 4}_1, +, [1,3]}_2 - initial_conditions) (2, 6)"
	 "{{0, +, 4}_1, +, [1,3]}_2 (2, 6)"
	 "[6,18]".
         
         That means that the effect of the inner loop 2 in the outer
	 loop 1 is that of adding an integer between 6 and 18 every
	 time the loop 1 is executed.  Consequently, the evolution
	 function is: "{{22, +, [10,22]}_1, +, [1,3]}_2".  */
      tree evolution_fn, overall_effect;
      
      /* Since the exit condition is on the end of the loop (after the
	 loop headers copy), we have to adjust the number of
	 iterations to "nb_iter - 1".  */
      nb_iter = chrec_fold_minus (nb_iter, integer_one_node);
      
      loop_nb = loop_num (loop);
      evolution_fn = analyze_scalar_evolution (loop_nb, version);
      overall_effect = chrec_apply
	(loop_nb, 
	 update_initial_condition_to_origin
	 (evolution_function_in_loop_num (evolution_fn, loop_nb)),
	 nb_iter);
      res = chrec_fold_plus (evolution_fn, overall_effect);
      
      /* Select only the evolution in the parent loop, since we're not
	 interested in having the details of the evolution in the
	 inner loops, when looking at the evolution in the outer
	 loop.  */
      res = select_outer_and_current_evolutions 
	(loop_num (outer_loop (loop)), res);
    }
  
  set_scalar_evolution_outer_value (version, res);
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
  return res;
}



/* The following section constitutes the interface with the chrecs.  */

/* Determine whether the CHREC is positive.  If the expression cannot
   be statically analyzed, return false, otherwise set the answer into
   VALUE.  */

bool
chrec_is_positive (tree chrec, bool *value)
{
  bool value0, value1;
  
  switch (TREE_CODE (chrec))
    {
    case INTERVAL_CHREC:
      return chrec_is_positive (CHREC_LOW (chrec), value);
      
    case POLYNOMIAL_CHREC:
    case EXPONENTIAL_CHREC:
      if (!chrec_is_positive (CHREC_LEFT (chrec), &value0)
	  || !chrec_is_positive (CHREC_RIGHT (chrec), &value1))
	return false;
      
      if (value0 == true && value1 == true)
	{
	  *value = true;
	  return true;
	}
      else if (value0 == false && value1 == false)
	{
	  *value = false;
	  return true;
	}
      else
	{
	  /* Otherwise the chrec is under the form: "{-197, +, 2}_1",
	     and the proof consists in showing that the sign never
	     changes during the execution of the loop, from 0 to
	     loop_nb_iterations ().  */
	  if (evolution_function_is_affine_p (chrec))
	    {
	      bool value2;
	      tree end_value;
	      tree nb_iter;
	      
	      nb_iter = number_of_iterations_in_loop
		(loop_from_num (current_loops, CHREC_VARIABLE (chrec)));
	      
	      nb_iter = chrec_fold_plus (nb_iter, integer_minus_one_node);
	      nb_iter = chrec_fold_plus (nb_iter, integer_minus_one_node);
	      
	      if (chrec_contains_undetermined (nb_iter))
		return false;
	      
	      end_value = chrec_apply (CHREC_VARIABLE (chrec), chrec, nb_iter);
	      
	      if (!chrec_is_positive (end_value, &value2))
		return false;
	      
	      if (value0 == true && value2 == true)
		{
		  *value = true;
		  return true;
		}
	      else if (value0 == false && value2 == false)
		{
		  *value = false;
		  return true;
		}
	      else
		return false;
	    }
	  
	  return false;
	}
      
    case INTEGER_CST:
      *value = (tree_int_cst_sgn (chrec) == 1);
      return true;
      
    case PLUS_EXPR:
    default:
      return false;
    }
}

/* Get the index corresponding to VAR in the current loop nest.  If
   it's the first time we ask for this VAR, then conservatively insert
   CHREC_TOP for this VAR and return its index.  */

static struct scev_info_str *
find_var_scev_info (tree var)
{
  unsigned int i;
  struct scev_info_str *res;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (*scalar_evolution_info); i++)
    {
      res = VARRAY_GENERIC_PTR (*scalar_evolution_info, i);
      if (MI_VAR (res) == var)
	return res;
    }
  
  /* The variable is not in the table, create a new entry for it.  */
  res = new_scev_info_str (var);
  VARRAY_PUSH_GENERIC_PTR (*scalar_evolution_info, res);
  
  return res;
}

/* Determine whether the set_chrec has to keep this expression
   symbolic. */

static tree 
set_scev_keep_symbolic (tree var,
			tree chrec)
{
  switch (TREE_CODE (chrec))
    {
    case ADDR_EXPR:
    case ARRAY_REF:
    case INDIRECT_REF:
    case COMPONENT_REF:
      /* KEEP_IT_SYMBOLIC.  */
      chrec = var;
      break;
      
    default:
      break;
    }
  
  return chrec;
}

/* Associate CHREC to SCALAR.  */

static void
set_scalar_evolution (tree scalar, 
		      tree chrec)
{
  struct scev_info_str *scalar_info;
  unsigned loop_nb = loop_num (loop_of_stmt (SSA_NAME_DEF_STMT (scalar)));
  
  scalar_info = find_var_scev_info (scalar);
  chrec = set_scev_keep_symbolic (scalar, chrec);
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(set_scalar_evolution \n");
      fprintf (tree_dump_file, "  (loop_nb = %d)\n", loop_nb);
      fprintf (tree_dump_file, "  (scalar = ");
      print_generic_expr (tree_dump_file, scalar, 0);
      fprintf (tree_dump_file, ")\n  (scalar_evolution = ");
      print_generic_expr (tree_dump_file, chrec, 0);
      fprintf (tree_dump_file, "))\n");
    }
  
  MI_LOOP_NUM (scalar_info) = loop_nb;
  MI_INNER_LOOPS_CHREC (scalar_info) = chrec;
}

/* Associate the value CHREC, that is exposed to the statements after
   the loop, to SCALAR.  */

static void
set_scalar_evolution_outer_value (tree scalar, 
				  tree chrec)
{
  struct scev_info_str *scalar_info;
  unsigned loop_nb = loop_num (loop_of_stmt (SSA_NAME_DEF_STMT (scalar)));
  
  scalar_info = find_var_scev_info (scalar);
  chrec = set_scev_keep_symbolic (scalar, chrec);
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(set_scalar_evolution_outer_value \n");
      fprintf (tree_dump_file, "  (loop_nb = %d)\n", loop_nb);
      fprintf (tree_dump_file, "  (scalar = ");
      print_generic_expr (tree_dump_file, scalar, 0);
      fprintf (tree_dump_file, ")\n  (scalar_evolution = ");
      print_generic_expr (tree_dump_file, chrec, 0);
      fprintf (tree_dump_file, "))\n");
    }
  
  MI_OUTER_LOOPS_CHREC (scalar_info) = chrec;
}

/* Retrieve the chrec associated to VAR in the loop LOOP_NB.  LOOP_NB
   determines the returned value, following the loop inclusion: if
   LOOP_NB is contained in loop_of_stmt (var), then the inner value is
   returned, otherwise it's the value that is viewed by a statement
   after the loop.  */

static tree
get_scalar_evolution (unsigned loop_nb, 
		      tree var)
{
  struct scev_info_str *var_info;
  tree res = NULL_TREE;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(get_scalar_evolution \n");
      fprintf (tree_dump_file, "  (loop_nb = %d)\n", loop_nb);
      fprintf (tree_dump_file, "  (scalar = ");
      print_generic_expr (tree_dump_file, var, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  switch (TREE_CODE (var))
    {
    case SSA_NAME:
      var_info = find_var_scev_info (var);
      
      if (MI_INNER_LOOPS_CHREC (var_info) == chrec_not_analyzed_yet)
	res = chrec_not_analyzed_yet;
      
      else if (MI_INNER_LOOPS_CHREC (var_info) == chrec_top)
	res = chrec_top;
      
      else if (loop_is_included_in (loop_nb, MI_LOOP_NUM (var_info)))
	res = MI_INNER_LOOPS_CHREC (var_info);
      
      else
	{
	  res = MI_OUTER_LOOPS_CHREC (var_info);
	  
	  if (res == chrec_not_analyzed_yet)
	    res = compute_overall_effect_of_inner_loop (var);
	}
      break;

    case VAR_DECL:
    case PARM_DECL:
    case REAL_CST:
    case INTEGER_CST:
    case FLOAT_EXPR:
    case NEGATE_EXPR:
    case ABS_EXPR:
    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
    case LROTATE_EXPR:
    case RROTATE_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
    case BIT_AND_EXPR:
    case BIT_NOT_EXPR:
    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
    case TRUTH_NOT_EXPR:
    case ADDR_EXPR:
    case ARRAY_REF:
    case INDIRECT_REF:
    case COMPONENT_REF:
      /* KEEP_IT_SYMBOLIC. These nodes are kept in "symbolic" form. */
      res = var;
      break;
      
      /*
	case VAR_DECL:
	case PARM_DECL:
	var_info = find_var_scev_info (var);
	res = MI_INNER_LOOPS_CHREC (var_info);
	break;
      */  
      
    case CONVERT_EXPR:
    case NOP_EXPR:
      {
      /* KEEP_IT_SYMBOLIC.  In the case of a cast, keep it symbolic,
	 otherwise just answer chrec_top.  */
	tree opnd0 = TREE_OPERAND (var, 0);
	
	if (opnd0 && TREE_CODE (opnd0) == SSA_NAME)
	  res = var;
	else
	  res = chrec_top;
	break;
      }
      
    case CALL_EXPR:
    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    default:
      /* We really don't want to do symbolic computations on these
	 nodes!  */
      res = chrec_top;
      break;
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "  (scalar_evolution = ");
      print_generic_expr (tree_dump_file, res, 0);
      fprintf (tree_dump_file, "))\n");
    }
  
  return res;
}

/* Select the evolution function in the current LOOP_NB and in the
   outer containing loops.  */

static tree
select_outer_and_current_evolutions (unsigned loop_nb, 
				     tree chrec)
{
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
      if (loop_is_included_in (loop_nb, CHREC_VARIABLE (chrec)))
	return build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec), 
	   select_outer_and_current_evolutions (loop_nb, CHREC_LEFT (chrec)),
	   select_outer_and_current_evolutions (loop_nb, CHREC_RIGHT (chrec)));
      
      else
	return select_outer_and_current_evolutions 
	  (loop_nb, CHREC_LEFT (chrec));
      
    case EXPONENTIAL_CHREC:
      if (loop_is_included_in (loop_nb, CHREC_VARIABLE (chrec)))
	return build_exponential_chrec 
	  (CHREC_VARIABLE (chrec), 
	   select_outer_and_current_evolutions (loop_nb, CHREC_LEFT (chrec)),
	   select_outer_and_current_evolutions (loop_nb, CHREC_RIGHT (chrec)));
      
      else
	return select_outer_and_current_evolutions 
	  (loop_nb, CHREC_LEFT (chrec));
      
    default:
      return chrec;
    }
}

/* The expression CHREC_BEFORE has no evolution part in LOOP_NB.  
   This function constructs a new polynomial evolution function for this 
   loop.  The evolution part is TO_ADD.  */

static tree
build_polynomial_evolution_in_loop (unsigned loop_nb, 
				    tree chrec_before, 
				    tree to_add)
{
  switch (TREE_CODE (chrec_before))
    {
    case POLYNOMIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) < loop_nb)
	return build_polynomial_chrec 
	  (loop_nb, chrec_before, to_add);
      else
	return build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   build_polynomial_evolution_in_loop 
	   (loop_nb, CHREC_LEFT (chrec_before), to_add),
	   CHREC_RIGHT (chrec_before));
      
    case EXPONENTIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) < loop_nb)
	return build_polynomial_chrec 
	  (loop_nb, chrec_before, to_add);
      else
	return build_exponential_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   build_polynomial_evolution_in_loop 
	   (loop_nb, CHREC_LEFT (chrec_before), to_add),
	   CHREC_RIGHT (chrec_before));
      
    case VAR_DECL:
    case PARM_DECL:
    case INTEGER_CST:
    case INTERVAL_CHREC:
    default:
      /* These nodes do not depend on a loop.  */
      return build_polynomial_chrec 
	(loop_nb, chrec_before, to_add);
    }
}

/* The expression CHREC_BEFORE has no evolution part in LOOP_NUM.  
   This function constructs a new exponential evolution function for this 
   loop.  The evolution part is TO_MULT.  */

static tree
build_exponential_evolution_in_loop (unsigned loop_num, 
				     tree chrec_before, 
				     tree to_mult)
{
  switch (TREE_CODE (chrec_before))
    {
    case POLYNOMIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) < loop_num)
	return build_exponential_chrec 
	  (loop_num, chrec_before, to_mult);
      else
	return build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   build_exponential_evolution_in_loop 
	   (loop_num, CHREC_LEFT (chrec_before), to_mult),
	   CHREC_RIGHT (chrec_before));
      
    case EXPONENTIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) < loop_num)
	return build_exponential_chrec 
	  (loop_num, chrec_before, to_mult);
      else
	return build_exponential_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   build_exponential_evolution_in_loop 
	   (loop_num, CHREC_LEFT (chrec_before), to_mult),
	   CHREC_RIGHT (chrec_before));
      
    case VAR_DECL:
    case PARM_DECL:
    case INTEGER_CST:
    case INTERVAL_CHREC:
    default:
      /* These nodes do not depend on a loop.  */
      return build_exponential_chrec 
	(loop_num, chrec_before, to_mult);
    }
}

/* Add TO_ADD to the evolution part of CHREC_BEFORE in the dimension
   of LOOP_NB.  
   
   Description (provided for completeness, for those who read code in
   a plane, and for my poor 62 bytes brain that would have forgotten
   all this in the next two or three months):
   
   The algorithm of translation of programs from the SSA representation
   into the chrecs syntax is based on a pattern matching.  After having
   reconstructed the overall tree expression for a loop, there are only
   two cases that can arise:
   
   1. a = loop-phi (init, a + expr)
   2. a = loop-phi (init, expr)
   
   where EXPR is either a scalar constant with respect to the analyzed
   loop (this is a degree 0 polynomial), or an expression containing
   other loop-phi definitions (these are higher degree polynomials).
   
   Examples:
   
   1. 
   | init = ...
   | loop_1
   |   a = phi (init, a + 5)
   | endloop
   
   2. 
   | inita = ...
   | initb = ...
   | loop_1
   |   a = phi (inita, 2 * b + 3)
   |   b = phi (initb, b + 1)
   | endloop
   
   For the first case, the semantics of the SSA representation is: 
   
   | a (x) = init + \sum_{j = 0}^{x - 1} expr (j)
   
   that is, there is a loop index "x" that determines the scalar value
   of the variable during the loop execution.  During the first
   iteration, the value is that of the initial condition INIT, while
   during the subsequent iterations, it is the sum of the initial
   condition with the sum of all the values of EXPR from the initial
   iteration to the before last considered iteration.  
   
   For the second case, the semantics of the SSA program is:
   
   | a (x) = init, if x = 0;
   |         expr (x - 1), otherwise.
   
   The second case corresponds to the PEELED_CHREC, whose syntax is
   close to the syntax of a loop-phi-node: 
   
   | phi (init, expr)  vs.  (init, expr)_x
   
   The proof of the translation algorithm for the first case is a
   proof by structural induction based on the degree of EXPR.  
   
   Degree 0:
   When EXPR is a constant with respect to the analyzed loop, or in
   other words when EXPR is a polynomial of degree 0, the evolution of
   the variable A in the loop is an affine function with an initial
   condition INIT, and a step EXPR.  In order to show this, we start
   from the semantics of the SSA representation:
   
   f (x) = init + \sum_{j = 0}^{x - 1} expr (j)
   
   and since "expr (j)" is a constant with respect to "j",
   
   f (x) = init + x * expr 
   
   Finally, based on the semantics of the pure sum chrecs, by
   identification we get the corresponding chrecs syntax:
   
   f (x) = init * \binom{x}{0} + expr * \binom{x}{1} 
   f (x) -> {init, +, expr}_x
   
   Higher degree:
   Suppose that EXPR is a polynomial of degree N with respect to the
   analyzed loop_x for which we have already determined that it is
   written under the chrecs syntax:
   
   | expr (x)  ->  {b_0, +, b_1, +, ..., +, b_{n-1}} (x)
   
   We start from the semantics of the SSA program:
   
   | f (x) = init + \sum_{j = 0}^{x - 1} expr (j)
   |
   | f (x) = init + \sum_{j = 0}^{x - 1} 
   |                (b_0 * \binom{j}{0} + ... + b_{n-1} * \binom{j}{n-1})
   |
   | f (x) = init + \sum_{j = 0}^{x - 1} 
   |                \sum_{k = 0}^{n - 1} (b_k * \binom{j}{k}) 
   |
   | f (x) = init + \sum_{k = 0}^{n - 1} 
   |                (b_k * \sum_{j = 0}^{x - 1} \binom{j}{k}) 
   |
   | f (x) = init + \sum_{k = 0}^{n - 1} 
   |                (b_k * \binom{x}{k + 1}) 
   |
   | f (x) = init + b_0 * \binom{x}{1} + ... 
   |              + b_{n-1} * \binom{x}{n} 
   |
   | f (x) = init * \binom{x}{0} + b_0 * \binom{x}{1} + ... 
   |                             + b_{n-1} * \binom{x}{n} 
   |
   
   And finally from the definition of the chrecs syntax, we identify:
   | f (x)  ->  {init, +, b_0, +, ..., +, b_{n-1}}_x 
   
   This shows the mechanism that stands behind the add_to_evolution
   function.  In some cases, EXPR is not built at once, but step by
   step on the returning path, and consequently it is simpler to build
   the evolution functions starting from a partially built evolution
   CHREC_BEFORE, adding a step of evolution TO_ADD.  Another important
   point is that the use of symbolic parameters avoids the need of an
   analysis schedule.
   
   Example:
   
   | inita = ...
   | initb = ...
   | loop_1 
   |   a = phi (inita, a + 2 + b)
   |   b = phi (initb, b + 1)
   | endloop
   
   When analyzing "a", the algorithm keeps "b" symbolically:
   
   | a  ->  {inita, +, 2 + b}_1
   
   Then, after instantiation, the analyzer ends on the evolution:
   
   | a  ->  {inita, +, 2 + initb, +, 1}_1
*/

static tree 
add_to_evolution (unsigned loop_nb, 
		  tree chrec_before,
		  tree to_add)
{
  tree res = NULL_TREE;
  
  if (to_add == NULL_TREE)
    return chrec_before;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(add_to_evolution \n");
      fprintf (tree_dump_file, "  (loop_nb = %d)\n", loop_nb);
      fprintf (tree_dump_file, "  (chrec_before = ");
      print_generic_expr (tree_dump_file, chrec_before, 0);
      fprintf (tree_dump_file, ")\n  (to_add = ");
      print_generic_expr (tree_dump_file, to_add, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  switch (TREE_CODE (to_add))
    {
    case POLYNOMIAL_CHREC:
      if (no_evolution_in_loop_p (chrec_before, loop_nb))
	{
	  switch (TREE_CODE (chrec_before))
	    {
	    case POLYNOMIAL_CHREC:
	      if (loop_nb != CHREC_VARIABLE (to_add)
		  && loop_is_included_in (loop_nb, CHREC_VARIABLE (to_add)))
		/* testsuite/.../ssa-chrec-38.c.  */
		res = build_polynomial_evolution_in_loop 
		  (loop_nb, chrec_before, to_add);
	      else
		/* testsuite/.../ssa-chrec-43.c.  */
		res = build_polynomial_chrec 
		  (CHREC_VARIABLE (chrec_before), 
		   add_to_evolution 
		   (loop_nb, CHREC_LEFT (chrec_before), to_add), 
		   CHREC_RIGHT (chrec_before));
	      
	      break;
	      
	    case EXPONENTIAL_CHREC:
	      if (loop_nb != CHREC_VARIABLE (to_add)
		  && loop_is_included_in (loop_nb, CHREC_VARIABLE (to_add)))
		/* FIXME taylor.  */
		res = reset_evolution_in_loop (loop_nb, 
					       chrec_before, 
					       chrec_top);
	      else
		res = build_exponential_chrec 
		  (CHREC_VARIABLE (chrec_before), 
		   add_to_evolution 
		   (loop_nb, CHREC_LEFT (chrec_before), to_add), 
		   CHREC_RIGHT (chrec_before));
	      
	      break;
	      
	    default:
	      if (loop_nb != CHREC_VARIABLE (to_add)
		  && loop_is_included_in (loop_nb, CHREC_VARIABLE (to_add)))
		/* If TO_ADD has an evolution in an outer containing
		   loop, don't restructure the tree.  Example: 
		   {2, +, {0, +, 1}_1}_2  is not equivalent to 
		   {{2, +, 1}_1, +, 0}_2.
		   testsuite/.../ssa-chrec-43.c  */
		res = build_polynomial_evolution_in_loop 
		  (loop_nb, chrec_before, to_add);
	      else
		{
		  /* When CHREC_BEFORE is constant, it becomes the new
		     initial condition of the evolution, while the
		     initial condition of TO_ADD is added to the
		     evolution in LOOP_NB. Example:
		     (add_to_evolution 
		     (loop_nb = 1)
		     (chrec_before = 2)
		     (to_add = {3, +, 4}_1)
		     (res = {{2, +, 3}_1, +, 4}_1))
		     testsuite/.../ssa-chrec-08.c  */
		  tree init_cond_to_add = initial_condition (to_add);
		  
		  chrec_before = chrec_replace_initial_condition 
		    (to_add, chrec_before);
		  res = build_polynomial_evolution_in_loop 
		    (loop_nb, chrec_before, init_cond_to_add);
		}
	      break;
	    }
	}
      
      else
	/* testsuite/.../ssa-chrec-37.c.  */
	res = add_expr_to_loop_evolution (loop_nb, chrec_before, to_add);
      break;
      
    case EXPONENTIAL_CHREC:
      if (no_evolution_in_loop_p (chrec_before, loop_nb))
	/* testsuite/.../ssa-chrec-40.c  */
	res = build_polynomial_evolution_in_loop 
	  (loop_nb, chrec_before, to_add);
      else
	/* FIXME Taylor.  */
	res = chrec_top;
      break;
      
    case VAR_DECL:
    case PARM_DECL:
    case INTERVAL_CHREC:
    default:
      if (no_evolution_in_loop_p (chrec_before, loop_nb))
	/* testsuite/.../ssa-chrec-39.c.  */
	res = build_polynomial_evolution_in_loop 
	  (loop_nb, chrec_before, to_add);
      else
	/* testsuite/.../ssa-chrec-20.c.  */
	res = add_expr_to_loop_evolution (loop_nb, chrec_before, to_add);
      break;
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "  (res = ");
      print_generic_expr (tree_dump_file, res, 0);
      fprintf (tree_dump_file, "))\n");
    }
  
  return res;
}

/* Add TO_MULT to the evolution part of CHREC_BEFORE in the dimension
   of LOOP_NB.  */

static tree 
multiply_evolution (unsigned loop_nb, 
		    tree chrec_before,
		    tree to_mult)
{
  tree res = NULL_TREE;
  
  if (to_mult == NULL_TREE)
    return chrec_before;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(multiply_evolution \n");
      fprintf (tree_dump_file, "  (loop_nb = %d)\n", loop_nb);
      fprintf (tree_dump_file, "  (chrec_before = ");
      print_generic_expr (tree_dump_file, chrec_before, 0);
      fprintf (tree_dump_file, ")\n  (to_mult = ");
      print_generic_expr (tree_dump_file, to_mult, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  switch (TREE_CODE (to_mult))
    {
    case EXPONENTIAL_CHREC:
      if (no_evolution_in_loop_p (chrec_before, loop_nb))
	/* testsuite/.../ssa-chrec-41.c.  */
	{
	  switch (TREE_CODE (chrec_before))
	    {
	    case POLYNOMIAL_CHREC:

	      if (loop_nb != CHREC_VARIABLE (to_mult)
		  && loop_is_included_in (loop_nb, CHREC_VARIABLE (to_mult)))
		/* FIXME taylor.  */
		res = reset_evolution_in_loop 
		  (loop_nb, chrec_before, chrec_top);
	      else
		res = build_polynomial_chrec 
		  (CHREC_VARIABLE (chrec_before), 
		   multiply_evolution 
		   (loop_nb, CHREC_LEFT (chrec_before), to_mult), 
		   CHREC_RIGHT (chrec_before));
	      
	      break;
	      
	    case EXPONENTIAL_CHREC:
	      if (loop_nb != CHREC_VARIABLE (to_mult)
		  && loop_is_included_in (loop_nb, CHREC_VARIABLE (to_mult)))
		res = build_exponential_evolution_in_loop 
		  (loop_nb, chrec_before, to_mult);
	      else
		res = build_exponential_chrec 
		  (CHREC_VARIABLE (chrec_before), 
		   multiply_evolution 
		   (loop_nb, CHREC_LEFT (chrec_before), to_mult), 
		   CHREC_RIGHT (chrec_before));
	      
	      break;
	      
	    default:
	      if (loop_nb != CHREC_VARIABLE (to_mult)
		  && loop_is_included_in (loop_nb, CHREC_VARIABLE (to_mult)))
		res = build_exponential_evolution_in_loop 
		  (loop_nb, chrec_before, to_mult);
	      else
		{
		  tree init_cond_to_mult = initial_condition (to_mult);
		  
		  chrec_before = chrec_replace_initial_condition 
		    (to_mult, chrec_before);
		  res = build_exponential_evolution_in_loop 
		    (loop_nb, chrec_before, init_cond_to_mult);
		}
	      break;
	    }
	}
      
      else
	/* testsuite/.../ssa-chrec-22.c.  */
	res = multiply_by_expr_the_loop_evolution 
	  (loop_nb, chrec_before, to_mult);
      break;
      
    case POLYNOMIAL_CHREC:
      if (no_evolution_in_loop_p (chrec_before, loop_nb))
	/* testsuite/.../ssa-chrec-23.c.  */
	res = build_exponential_evolution_in_loop 
	  (loop_nb, chrec_before, to_mult);
      else
	/* FIXME Taylor.  */
	res = chrec_top;
      break;
      
    case VAR_DECL:
    case PARM_DECL:
    case INTERVAL_CHREC:
    default:
      if (no_evolution_in_loop_p (chrec_before, loop_nb))
	/* testsuite/.../ssa-chrec-22.c.  */
	res = build_exponential_evolution_in_loop 
	  (loop_nb, chrec_before, to_mult);
      else
	res = multiply_by_expr_the_loop_evolution 
	  (loop_nb, chrec_before, to_mult);
      break;
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "  (res = ");
      print_generic_expr (tree_dump_file, res, 0);
      fprintf (tree_dump_file, "))\n");
    }
  
  return res;
}



/* This section deals with the approximation of the number of
   iterations a loop will run.  */

/* Helper function.  */

static inline tree
cannot_analyze_loop_nb_iterations_yet (void)
{
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "  (nb_iterations cannot be determined))\n");
  
  /* Do not update the loop->nb_iterations.  */
  return chrec_top;
}

/* Helper function.  */

static inline tree
set_nb_iterations_in_loop (struct loop *loop, 
			   tree res)
{
  /* After the loop copy headers has transformed the code, each loop
     runs at least once.  */
  res = chrec_fold_plus (res, integer_one_node);
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "  (set_nb_iterations_in_loop = ");
      print_generic_expr (tree_dump_file, res, 0);
      fprintf (tree_dump_file, "))\n");
    }
  
  loop->nb_iterations = res;
  return res;
}

/* In order to determine the number of iterations in a loop, we have
   to perform some computations on integers.  If the evolution
   functions on which we work contains INTEGER_CSTs with different
   types, ie. size_t, unsigned, ..., then the computation of the
   number of iterations may fail.  Example:

   fold_minus (op0, op1) 
   op0 = {0, +, 1}_1
   op1 = 168
   res = {0, +, 1}_1
   
   The result is erroneous due to the fact that the folder performs
   unsigned arithmetic.  */

static tree
homogenize_integer_cst_types_for_nb_iter (tree chrec)
{
  tree res;

  if (automatically_generated_chrec_p (chrec))
    res = chrec;
  
  else if (TREE_CODE (chrec) == INTEGER_CST
	   && TREE_TYPE (chrec) != integer_type_node)
    {
      res = copy_node (chrec);
      TREE_TYPE (res) = integer_type_node;
    }
  
  else
    switch (TREE_CODE_LENGTH (TREE_CODE (chrec)))
      {
      case 3:
	res = build 
	  (TREE_CODE (chrec), 
	   TREE_TYPE (chrec),
	   homogenize_integer_cst_types_for_nb_iter (TREE_OPERAND (chrec, 0)),
	   homogenize_integer_cst_types_for_nb_iter (TREE_OPERAND (chrec, 1)),
	   homogenize_integer_cst_types_for_nb_iter (TREE_OPERAND (chrec, 2)));
	break;
	
      case 2:
	res = build 
	  (TREE_CODE (chrec), 
	   TREE_TYPE (chrec),
	   homogenize_integer_cst_types_for_nb_iter (TREE_OPERAND (chrec, 0)),
	   homogenize_integer_cst_types_for_nb_iter (TREE_OPERAND (chrec, 1)));
	break;
	
      default:
	res = chrec;
	break;
      }
  
  return res;
}



/* This section selects the loops that will be good candidates for the
   scalar evolution analysis.
   
   Note: This section will be rewritten to expose a better interface
   to other client passes.  For the moment, greedily select all the
   loop nests we could analyze.  */

/* Determine whether it is possible to analyze this condition
   expression.  */

static bool
analyzable_condition (tree expr)
{
  tree condition;
  
  if (TREE_CODE (expr) != COND_EXPR)
    return false;
  
  condition = TREE_OPERAND (expr, 0);
  
  switch (TREE_CODE (condition))
    {
    case SSA_NAME:
      /* Volatile expressions are not analyzable.  */
      if (TREE_THIS_VOLATILE (SSA_NAME_VAR (condition)))
	return false;
      return true;
      
    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
      {
	tree opnd0, opnd1;
	
	opnd0 = TREE_OPERAND (condition, 0);
	opnd1 = TREE_OPERAND (condition, 1);
	
	if (TREE_CODE (opnd0) == SSA_NAME
	    && TREE_THIS_VOLATILE (SSA_NAME_VAR (opnd0)))
	  return false;
	
	if (TREE_CODE (opnd1) == SSA_NAME
	    && TREE_THIS_VOLATILE (SSA_NAME_VAR (opnd1)))
	  return false;
	
	return true;
      }
      
    default:
      return false;
    }
  
  return false;
}

/* For a loop with a single exit edge, determine the COND_EXPR that
   guards the exit edge.  If the expression is too difficult to
   analyze, then give up.  */

tree 
get_loop_exit_condition (struct loop *loop)
{
  tree res = NULL_TREE;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "(get_loop_exit_condition \n  ");
  
  if (loop_exit_edges (loop))
    {
      edge exit_edge;
      tree expr;
      
      exit_edge = loop_exit_edge (loop, 0);
      expr = last_stmt (edge_source (exit_edge));
      
      if (analyzable_condition (expr))
	res = expr;
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      print_generic_expr (tree_dump_file, res, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  return res;
}

/* Recursively determine and enqueue the exit conditions for a loop.  */

static void 
get_exit_conditions_rec (struct loop *loop, 
			 varray_type *exit_conditions)
{
  if (!loop)
    return;
  
  /* Recurse on the inner loops, then on the next (sibling) loops.  */
  get_exit_conditions_rec (inner_loop (loop), exit_conditions);
  get_exit_conditions_rec (next_loop (loop), exit_conditions);
  
  flow_loop_scan (loop, LOOP_EXIT_EDGES);
  if (loop_num_exits (loop) == 1)
    {
      tree loop_condition = get_loop_exit_condition (loop);
      
      if (loop_condition)
	VARRAY_PUSH_TREE (*exit_conditions, loop_condition);
    }
}

/* Select the candidate loop nests for the analysis.  This function
   initializes the EXIT_CONDITIONS array.  The vector EXIT_CONDITIONS is
   initialized in a loop-depth-first order, ie. the inner loops
   conditions appear before the outer.  This property of the
   EXIT_CONDITIONS list is exploited by the evolution analyzer.  */

static void
select_loops_exit_conditions (struct loops *loops, 
			      varray_type *exit_conditions)
{
  struct loop *function_body = loops->parray[0];
  
  get_exit_conditions_rec (inner_loop (function_body), exit_conditions);
}



/* Debugging functions section.  */

extern void draw_tree_cfg (void);

/* Draw the flow graph.  */

void
draw_tree_cfg (void)
{
  FILE *dump_file;
  if (n_basic_blocks > 0)
    {
      dump_file = fopen ("tree_cfg.dot", "w");
      if (dump_file)
        {
          tree_cfg2dot (dump_file);
          fclose (dump_file);
          system ("dotty tree_cfg.dot");
        }
    }
}
     


/* Follow the ssa edge into the right hand side of an assignment.  */

static bool
follow_ssa_edge_in_rhs (unsigned loop_nb, 
			tree rhs, 
			tree halting_phi, 
			tree *evolution_of_loop)
{
  bool res = false;
  tree rhs0, rhs1;

  /* The RHS is one of the following cases:
     - an SSA_NAME, 
     - an INTEGER_CST,
     - a PLUS_EXPR, 
     - a MINUS_EXPR,
     - other cases are not yet handled. 
  */
  switch (TREE_CODE (rhs))
    {
    case INTEGER_CST:
      /* This assignment is under the form "a_1 = 7".  */
      res = false;
      break;
      
    case SSA_NAME:
      /* This assignment is under the form: "a_1 = b_2".  */
      res = follow_ssa_edge 
	(loop_nb, SSA_NAME_DEF_STMT (rhs), halting_phi, evolution_of_loop);
      break;
      
    case PLUS_EXPR:
      /* This case is under the form "rhs0 + rhs1".  */
      rhs0 = TREE_OPERAND (rhs, 0);
      rhs1 = TREE_OPERAND (rhs, 1);
	      	      
      if (TREE_CODE (rhs0) == SSA_NAME)
	{
	  if (TREE_CODE (rhs1) == SSA_NAME)
	    {
	      /* Match an assignment under the form: 
		 "a = b + c".  */
	      res = follow_ssa_edge 
		(loop_nb, SSA_NAME_DEF_STMT (rhs0), halting_phi, 
		 evolution_of_loop);

	      if (res)
		*evolution_of_loop = add_to_evolution 
		  (loop_nb, *evolution_of_loop, rhs1);
	      
	      else
		{
		  res = follow_ssa_edge 
		    (loop_nb, SSA_NAME_DEF_STMT (rhs1), halting_phi, 
		     evolution_of_loop);
		  
		  if (res)
		    *evolution_of_loop = add_to_evolution 
		      (loop_nb, *evolution_of_loop, rhs0);
		}
	    }
	  
	  else
	    {
	      /* Match an assignment under the form: 
		 "a = b + ...".  */
	      res = follow_ssa_edge 
		(loop_nb, SSA_NAME_DEF_STMT (rhs0), halting_phi, 
		 evolution_of_loop);
	      if (res)
		*evolution_of_loop = add_to_evolution 
		  (loop_nb, *evolution_of_loop, rhs1);
	    }
	}
      
      else if (TREE_CODE (rhs1) == SSA_NAME)
	{
	  /* Match an assignment under the form: 
	     "a = ... + c".  */
	  res = follow_ssa_edge 
	    (loop_nb, SSA_NAME_DEF_STMT (rhs1), halting_phi, 
	     evolution_of_loop);
	  if (res)
	    *evolution_of_loop = add_to_evolution 
	      (loop_nb, *evolution_of_loop, rhs0);
	}

      else
	/* Otherwise, match an assignment under the form: 
	   "a = ... + ...".  */
	/* And there is nothing to do.  */
	res = false;
      
      break;
      
    case MINUS_EXPR:
      /* This case is under the form "opnd0 = rhs0 - rhs1".  */
      rhs0 = TREE_OPERAND (rhs, 0);
      rhs1 = TREE_OPERAND (rhs, 1);
      if (TREE_CODE (rhs0) == SSA_NAME)
	{
	  if (TREE_CODE (rhs1) == SSA_NAME)
	    {
	      /* Match an assignment under the form: 
		 "a = b - c".  */
	      res = follow_ssa_edge 
		(loop_nb, SSA_NAME_DEF_STMT (rhs0), halting_phi, 
		 evolution_of_loop);
	      
	      if (res)
		*evolution_of_loop = add_to_evolution 
		  (loop_nb, *evolution_of_loop, 
		   chrec_fold_multiply (rhs1, integer_minus_one_node));
	      
	      else
		{
		  res = follow_ssa_edge 
		    (loop_nb, SSA_NAME_DEF_STMT (rhs1), halting_phi, 
		     evolution_of_loop);
		  
		  if (res)
		    *evolution_of_loop = add_to_evolution 
		      (loop_nb, *evolution_of_loop, 
		       chrec_fold_multiply (rhs0, integer_minus_one_node));
		}
	    }
	  
	  else
	    {
	      /* Match an assignment under the form: 
		 "a = b - ...".  */
	      res = follow_ssa_edge 
		(loop_nb, SSA_NAME_DEF_STMT (rhs0), halting_phi, 
		 evolution_of_loop);
	      if (res)
		*evolution_of_loop = add_to_evolution 
		  (loop_nb, *evolution_of_loop, 
		   chrec_fold_multiply (rhs1, integer_minus_one_node));
	    }
	}
      
      else if (TREE_CODE (rhs1) == SSA_NAME)
	{
	  /* Match an assignment under the form: 
	     "a = ... - c".  */
	  res = follow_ssa_edge 
	    (loop_nb, SSA_NAME_DEF_STMT (rhs1), halting_phi, 
	     evolution_of_loop);
	  if (res)
	    *evolution_of_loop = add_to_evolution 
	      (loop_nb, *evolution_of_loop, 
	       chrec_fold_multiply (rhs0, integer_minus_one_node));
	}
      
      else
	/* Otherwise, match an assignment under the form: 
	   "a = ... - ...".  */
	/* And there is nothing to do.  */
	res = false;
      
      break;
    
    case MULT_EXPR:
      /* This case is under the form "opnd0 = rhs0 * rhs1".  */
      rhs0 = TREE_OPERAND (rhs, 0);
      rhs1 = TREE_OPERAND (rhs, 1);
      if (TREE_CODE (rhs0) == SSA_NAME)
	{
	  if (TREE_CODE (rhs1) == SSA_NAME)
	    {
	      /* Match an assignment under the form: 
		 "a = b * c".  */
	      res = follow_ssa_edge 
		(loop_nb, SSA_NAME_DEF_STMT (rhs0), halting_phi, 
		 evolution_of_loop);
	      
	      if (res)
		*evolution_of_loop = multiply_evolution 
		  (loop_nb, *evolution_of_loop, rhs1);
	      
	      else
		{
		  res = follow_ssa_edge 
		    (loop_nb, SSA_NAME_DEF_STMT (rhs1), halting_phi, 
		     evolution_of_loop);
		  
		  if (res)
		    *evolution_of_loop = multiply_evolution 
		      (loop_nb, *evolution_of_loop, rhs0);
		}
	    }
	  
	  else
	    {
	      /* Match an assignment under the form: 
		 "a = b * ...".  */
	      res = follow_ssa_edge 
		(loop_nb, SSA_NAME_DEF_STMT (rhs0), halting_phi, 
		 evolution_of_loop);
	      if (res)
		*evolution_of_loop = multiply_evolution 
		  (loop_nb, *evolution_of_loop, rhs1);
	    }
	}
      
      else if (TREE_CODE (rhs1) == SSA_NAME)
	{
	  /* Match an assignment under the form: 
	     "a = ... * c".  */
	  res = follow_ssa_edge 
	    (loop_nb, SSA_NAME_DEF_STMT (rhs1), halting_phi, 
	     evolution_of_loop);
	  if (res)
	    *evolution_of_loop = multiply_evolution 
	      (loop_nb, *evolution_of_loop, rhs0);
	}
      
      else
	/* Otherwise, match an assignment under the form: 
	   "a = ... * ...".  */
	/* And there is nothing to do.  */
	res = false;
      
      break;

    default:
      res = false;
      break;
    }
  
  return res;
}

/* Helper function for one branch of the condition-phi-node.  */

static inline bool
follow_ssa_edge_in_condition_phi_branch (int i,
					 unsigned loop_nb, 
					 tree condition_phi, 
					 tree halting_phi,
					 tree *evolution_of_branch)
{
  bool found_path_to_halting_phi = false;
  tree branch = PHI_ARG_DEF (condition_phi, i);
  
  switch (TREE_CODE (branch))
    {
    case INTEGER_CST:
      /* This case occurs when one of the condition branches sets 
	 the variable to a constant: ie. a phi-node like
	 "a_2 = PHI <a_7(5), 2(6)>;".  
	 The testsuite/.../ssa-chrec-17.c exercises this code.  
	 
	 FIXME:  This case have to be refined correctly: 
	 in some cases it is possible to say something better than
	 chrec_top, for example using a wrap-around notation.  */
      *evolution_of_branch = chrec_top;
      found_path_to_halting_phi = false;
      break;
      
    case SSA_NAME:
      *evolution_of_branch = chrec_not_analyzed_yet;
      found_path_to_halting_phi = follow_ssa_edge 
	(loop_nb, SSA_NAME_DEF_STMT (branch), halting_phi, 
	 evolution_of_branch);
      break;
      
    default:
      *evolution_of_branch = chrec_top;
      found_path_to_halting_phi = false;
      break;
    }
  
  return found_path_to_halting_phi;
}

/* This function merges the branches of a condition-phi-node in a
   loop.  */

static bool
follow_ssa_edge_in_condition_phi (unsigned loop_nb, 
				  tree condition_phi, 
				  tree halting_phi, 
				  tree *evolution_of_loop)
{
  bool res = false;
  int i;
  tree evolution_of_branch;
  
  res = follow_ssa_edge_in_condition_phi_branch 
    (0, loop_nb, condition_phi, halting_phi, &evolution_of_branch);
  /* We pass in the evolution_of_loop the initial condition of the
     variable in the loop, from the analyze_evolution_in_loop.  */
  *evolution_of_loop = chrec_replace_initial_condition 
    (evolution_of_branch, *evolution_of_loop);
  
  for (i = 1; i < PHI_NUM_ARGS (condition_phi); i++)
    {
      bool found_path_to_halting_phi = follow_ssa_edge_in_condition_phi_branch 
	(i, loop_nb, condition_phi, halting_phi, &evolution_of_branch);
      
      res = res || found_path_to_halting_phi;
      *evolution_of_loop = chrec_merge 
	(*evolution_of_loop, evolution_of_branch);
    }
  
  return res;
}

/* Follow an SSA edge in an inner loop.  It computes the overall
   effect of the loop, and following the symbolic initial conditions,
   it follows the edges in the parent loop.  The inner loop is
   considered as a single statement.  */

static bool
follow_ssa_edge_inner_loop_phi (unsigned outer_loop_nb,
				tree loop_phi_node, 
				tree halting_phi,
				tree *evolution_of_loop)
{
  tree overall_effect = get_scalar_evolution 
    (outer_loop_nb, PHI_RESULT (loop_phi_node));
  
  if (overall_effect == chrec_not_analyzed_yet)
    overall_effect = compute_overall_effect_of_inner_loop 
      (PHI_RESULT (loop_phi_node));
  
  return follow_ssa_edge_in_rhs 
    (outer_loop_nb, overall_effect, halting_phi, evolution_of_loop);
}

/* Follow an SSA edge from a loop-phi-node to itself, constructing a
   path that is analyzed on the return walk.  */

static bool
follow_ssa_edge (unsigned loop_nb, 
		 tree def, 
		 tree halting_phi,
		 tree *evolution_of_loop)
{
  unsigned def_loop_nb;
  
  if (def == NULL_TREE 
      || TREE_CODE (def) == NOP_EXPR)
    return false;
  
  def_loop_nb = loop_num (loop_of_stmt (def));
  
  switch (TREE_CODE (def))
    {
    case PHI_NODE:
      if (loop_phi_node_p (def))
	{
	  /* When the analyzed phi is the halting_phi, the
	     depth-first search is over: we have found a path from
	     the halting_phi to itself in the loop.  */
	  if (def == halting_phi)
	    return true;
	  
	  /* Otherwise, the evolution of the HALTING_PHI depends
	     on the evolution of another loop-phi-node, ie. the
	     evolution function is a higher degree polynomial.  */
	  else if (def_loop_nb == loop_nb)
	    return false;
	  
	  /* Inner loop.  */
	  else if (loop_is_strictly_included_in (def_loop_nb, loop_nb))
	    return follow_ssa_edge_inner_loop_phi 
	      (loop_nb, def, halting_phi, evolution_of_loop);
	  
	  else
	    /* Outer loop.  */
	    return false;
	}
      
      /* DEF is a condition-phi-node.  Follow the branches, and
	 record their evolutions.  Finally, merge the collected
	 information and set the approximation to the main
	 variable.  */
      else
	{
	  if (PHI_MARKED (def))
	    {
	      *evolution_of_loop = chrec_top;
	      return false;
	    }
	  else 
	    {
	      bool res;
	      PHI_MARKED (def) = 1;
	      res = follow_ssa_edge_in_condition_phi 
		(loop_nb, def, halting_phi, evolution_of_loop);
	      PHI_MARKED (def) = 0;
	      return res;
	    }
	}
      
    case MODIFY_EXPR:
      return follow_ssa_edge_in_rhs (loop_nb,
				     TREE_OPERAND (def, 1), 
				     halting_phi, 
				     evolution_of_loop);
      
    default:
      /* At this level of abstraction, the program is just a set
	 of MODIFY_EXPRs and PHI_NODEs.  In principle there is no
	 other node to be handled.  */
      abort ();
      return false;
    }
}

/* Given a LOOP_PHI_NODE, this function determines the evolution
   function from LOOP_PHI_NODE to LOOP_PHI_NODE in the loop.  */

static tree
analyze_evolution_in_loop (tree loop_phi_node, 
			   tree init_cond)
{
  int i;
  tree evolution_function = chrec_not_analyzed_yet;
  unsigned loop_nb = loop_num (loop_of_stmt (loop_phi_node));
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(analyze_evolution_in_loop \n");
      fprintf (tree_dump_file, "  (loop_phi_node = ");
      print_generic_expr (tree_dump_file, loop_phi_node, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  for (i = 0; i < PHI_NUM_ARGS (loop_phi_node); i++)
    {
      tree arg = PHI_ARG_DEF (loop_phi_node, i);
      
      /* The arguments that are not SSA_NAMEs don't come from the
	 loop's body.  */
      if (TREE_CODE (arg) == SSA_NAME)
	{
	  tree ssa_chain = SSA_NAME_DEF_STMT (arg);
	  
	  /* Select the edges that enter the loop body.  */
	  if (ssa_chain != NULL_TREE 
	      && TREE_CODE (ssa_chain) != NOP_EXPR
	      && (loop_depth (loop_of_stmt (ssa_chain)) 
		  >= loop_depth (loop_of_stmt (loop_phi_node))))
	    {
	      bool res;
	      /* Pass in the initial condition to the follow edge
		 function.  */
	      tree ev_fn = init_cond;
	      res = follow_ssa_edge 
		(loop_nb, ssa_chain, loop_phi_node, &ev_fn);
	      
	      /* When it is impossible to go back on the same
		 loop_phi_node by following the ssa edges, the
		 evolution is represented by a peeled chrec, ie. the
		 first iteration, EV_FN has the value INIT_COND, then
		 all the other iterations it has the value of ARG.  */
	      if (res == false)
		{
		  /* FIXME: when dealing with periodic scalars, the
		     analysis of the scalar evolution of ARG would
		     create an infinite recurrence.  Solution: don't
		     try to simplify the peeled chrec at this time,
		     but wait until having more information.   */
		  tree arg_chrec = arg;
		  ev_fn = build_peeled_chrec 
		    (loop_nb, init_cond, arg_chrec);
		  
		  /* Try to simplify the peeled chrec.  */
		  ev_fn = simplify_peeled_chrec (ev_fn);
		}
	      
	      /* When there are multiple edges that enter the loop,
		 merge their evolutions. */
	      evolution_function = chrec_merge (evolution_function, ev_fn);
	    }
	}
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "  (evolution_function = ");
      print_generic_expr (tree_dump_file, evolution_function, 0);
      fprintf (tree_dump_file, "))\n");
    }
  
  return evolution_function;
}

/* Given a loop-phi-node, this function determines the initial
   conditions of the variable on entry of the loop.  When the CCP has
   propagated constants into the loop-phi-node, the initial condition
   is instantiated, otherwise the initial condition is kept symbolic.
   This analyzer does not analyze the evolution outside the current
   loop, and leaves this task to the on-demand tree reconstructor.  */

static tree 
analyze_initial_condition (tree loop_phi_node)
{
  int i;
  tree init_cond = chrec_not_analyzed_yet;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(analyze_initial_condition \n");
      fprintf (tree_dump_file, "  (loop_phi_node = \n");
      print_generic_expr (tree_dump_file, loop_phi_node, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  for (i = 0; i < PHI_NUM_ARGS (loop_phi_node); i++)
    {
      tree branch = PHI_ARG_DEF (loop_phi_node, i);
      
      switch (TREE_CODE (branch))
	{
	case SSA_NAME:
	  {
	    tree ssa_chain = SSA_NAME_DEF_STMT (branch);
	    
	    if (ssa_chain == NULL_TREE)
	      init_cond = chrec_top;
	    
	    else if (TREE_CODE (ssa_chain) == NOP_EXPR)
	      init_cond = branch;
	    
	    else if (loop_depth (loop_of_stmt (ssa_chain)) 
		     < loop_depth (loop_of_stmt (loop_phi_node)))
	      {
		/* When there are several branches to the outside of
		   the loop, pointing to the initial conditions, and
		   one of the initial conditions has a symbolic value
		   (that's the current one: branch) the result of
		   chrec_merge is chrec_top.  */
		if (init_cond != chrec_not_analyzed_yet)
		  init_cond = chrec_top;
		
		/* When SSA_CHAIN is a definition outside the current
		   loop nest, KEEP_IT_SYMBOLIC.  */
		else
		  init_cond = branch;
	      }
	    /* When the branch is oriented to the loop's body, it does
	       not contribute to the initial condition.  */
	    break;
	  }
	  
	default:
	  /* In the default case fall all the scalars propagated in
	     the loop-phi-node by the CCP.  */
	  if (init_cond == chrec_not_analyzed_yet)
	    init_cond = branch;
	  
	  else
	    /* It is possible that the previous upper branch was a
	       constant.  Try to merge with the previous information.  */
	    init_cond = chrec_merge (init_cond, branch);
	  
	  break;
	}
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "  (init_cond = ");
      print_generic_expr (tree_dump_file, init_cond, 0);
      fprintf (tree_dump_file, "))\n");
    }
  
  return init_cond;
}

/* Analyze the scalar evolution for the loop-phi-node DEF.  LOOP_NB
   determines the returned value, following the loop inclusion: if
   LOOP_NB is contained in loop_of_stmt (def), then the inner value is
   returned, otherwise it's the value that is viewed by a statement
   after the loop.  */

static tree 
interpret_loop_phi (unsigned loop_nb, 
		    tree loop_phi)
{
  tree res = get_scalar_evolution (loop_nb, PHI_RESULT (loop_phi));
  
  if (res == chrec_not_analyzed_yet)
    {
      unsigned loop_phi_nb = loop_num (loop_of_stmt (loop_phi));
      tree init_cond = analyze_initial_condition (loop_phi);
      res = analyze_evolution_in_loop (loop_phi, init_cond);
      set_scalar_evolution (PHI_RESULT (loop_phi), res);
      
      if (loop_is_strictly_included_in (loop_phi_nb, loop_nb))
	{
	  /* As in testsuite/.../ssa-chrec-{03, 06}.c we end on the
	     loop-phi-node of an inner loop.  */
	  res = compute_overall_effect_of_inner_loop 
	    (PHI_RESULT (loop_phi));
	  res = interpret_rhs_modify_expr (loop_nb, res);
	}
    }
  
  return res;
}

/* This function merges the branches of a condition-phi-node,
   contained in the outermost loop, and whose arguments are already
   analyzed.  */

static tree
interpret_condition_phi (unsigned loop_nb, 
			 tree condition_phi)
{
  int i;
  tree res = chrec_not_analyzed_yet;
  
  for (i = 0; i < PHI_NUM_ARGS (condition_phi); i++)
    {
      tree branch_chrec = analyze_scalar_evolution 
	(loop_nb, PHI_ARG_DEF (condition_phi, i));
      
      res = chrec_merge (res, branch_chrec);
    }

  set_scalar_evolution (PHI_RESULT (condition_phi), res);
  return res;
}

/* Interpret the right hand side of a modify_expr OPND1.  If we didn't
   analyzed this node before, follow the definitions until ending
   either on an analyzed modify_expr, or on a loop-phi-node.  On the
   return path, this function propagates evolutions (ala constant copy
   propagation).  */

static tree
interpret_rhs_modify_expr (unsigned loop_nb,
			   tree opnd1)
{
  tree res, opnd10, opnd11, chrec10, chrec11;
  
  switch (TREE_CODE (opnd1))
    {
    case PLUS_EXPR:
      opnd10 = TREE_OPERAND (opnd1, 0);
      opnd11 = TREE_OPERAND (opnd1, 1);
      chrec10 = analyze_scalar_evolution (loop_nb, opnd10);
      chrec11 = analyze_scalar_evolution (loop_nb, opnd11);
      res = chrec_fold_plus (chrec10, chrec11);
      break;
      
    case MINUS_EXPR:
      opnd10 = TREE_OPERAND (opnd1, 0);
      opnd11 = TREE_OPERAND (opnd1, 1);
      chrec10 = analyze_scalar_evolution (loop_nb, opnd10);
      chrec11 = analyze_scalar_evolution (loop_nb, opnd11);
      res = chrec_fold_minus (chrec10, chrec11);
      break;
      
    case MULT_EXPR:
      opnd10 = TREE_OPERAND (opnd1, 0);
      opnd11 = TREE_OPERAND (opnd1, 1);
      chrec10 = analyze_scalar_evolution (loop_nb, opnd10);
      chrec11 = analyze_scalar_evolution (loop_nb, opnd11);
      res = chrec_fold_multiply (chrec10, chrec11);
      break;
      
    default:
      res = chrec_top;
      break;
    }
  
  return res;
}



/* This section contains all the entry points: 
   - number_of_iterations_in_loop,
   - number_of_iterations_for_all_loops,
   - analyze_scalar_evolution,
   - instantiate_parameters.
*/

/* Entry point for the scalar evolution analyzer.
   
   Analyzes and returns the scalar evolution of the ssa_name VERSION.
   LOOP_NB is the identifier number of the loop in which the version
   is used.  
   
   Example of use: having a pointer VERSION to a SSA_NAME node, STMT
                   a pointer to the statement that uses this version,
                   in order to determine the evolution function of the
                   version, use the following calls:
   
   unsigned loop_nb = loop_num (loop_of_stmt (stmt));
   tree chrec_with_symbols = analyze_scalar_evolution (loop_nb, version);
   tree chrec_instantiated = instantiate_parameters 
     (loop_nb, chrec_with_symbols);
*/

tree 
analyze_scalar_evolution (unsigned loop_nb, 
			  tree version)
{
  tree res, def;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(analyze_scalar_evolution \n");
      fprintf (tree_dump_file, "  (loop_nb = %d)\n", loop_nb);
      fprintf (tree_dump_file, "  (scalar = ");
      print_generic_expr (tree_dump_file, version, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  res = get_scalar_evolution (loop_nb, version);
  if (res == chrec_not_analyzed_yet)
    {
      def = SSA_NAME_DEF_STMT (version);
      if (def == NULL_TREE)
	res = chrec_top;
      
      else
	{
	  switch (TREE_CODE (def))
	    {
	    case MODIFY_EXPR:
	      res = interpret_rhs_modify_expr 
		(loop_nb, TREE_OPERAND (def, 1));
	      
	      /* Following the LOOP_NB from where we're looking at
		 this definition, we have to set the inner or outer
		 visible value.  Examples: ssa-chrec-{01, 06}.  */
	      if (loop_is_included_in (loop_nb, loop_num (loop_of_stmt (def))))
		set_scalar_evolution (version, res);
	      else
		set_scalar_evolution_outer_value (version, res);
	      
	      break;
	      
	    case PHI_NODE:
	      if (PHI_MARKED (def))
		res = chrec_top;
	      else 
		{
		  PHI_MARKED (def) = 1;
		  if (loop_phi_node_p (def))
		    res = interpret_loop_phi (loop_nb, def);
		  else
		    res = interpret_condition_phi (loop_nb, def);
		  PHI_MARKED (def) = 0;
		}
	      break;
	  
	    default:
	      res = chrec_top;
	      if (TREE_CODE (def) != NOP_EXPR)
		set_scalar_evolution (version, res);
	      break;
	    }
      	}
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
  
  return res;
}

/* Analyze all the parameters of the chrec that were left under a
   symbolic form, except for version.  */

tree
instantiate_parameters (unsigned loop_nb, 
			tree chrec)
{
  tree res, op0, op1, op2;
  
  if (chrec == NULL_TREE
      || automatically_generated_chrec_p (chrec))
    res = chrec;
  
  else if (TREE_CODE (chrec) == SSA_NAME
	   || TREE_CODE (chrec) == VAR_DECL
	   || TREE_CODE (chrec) == PARM_DECL)
    {
      if (tree_is_in_varray_tree_p (chrec, *already_instantiated))
	/* Don't instantiate the SSA_NAME if it is in a mixer
	   structure.  This is used for avoiding the instantiation of
	   recursively defined functions, such as: 

	   | a_2 -> {0, +, 1, +, a_2}_1
	   
	   Note: the size of already_instantiated is proportional to
	   the degree of the evolution function.  This is the number
	   of parameters that have to be instantiated, and is almost
	   all the time less than 2.  */
	res = chrec;
      
      else
	{
	  res = analyze_scalar_evolution (loop_nb, chrec);
	  
	  /* If the analysis yields a parametric chrec, instantiate
	     the result again.  Enqueue the SSA_NAME such that it will
	     never be instantiated twice, avoiding the cyclic
	     instantiation in mixers.  */
	  if (chrec_contains_symbols (res))
	    {
	      VARRAY_PUSH_TREE (*already_instantiated, chrec);
	      res = instantiate_parameters (loop_nb, res);
	      VARRAY_POP (*already_instantiated);
	    }
	}
    }
  
  else
    switch (TREE_CODE (chrec))
      {
      case PLUS_EXPR:
	op0 = instantiate_parameters (loop_nb, TREE_OPERAND (chrec, 0));
	op1 = instantiate_parameters (loop_nb, TREE_OPERAND (chrec, 1));
	res = chrec_fold_plus (op0, op1);
	break;
	
      case MINUS_EXPR:
	op0 = instantiate_parameters (loop_nb, TREE_OPERAND (chrec, 0));
	op1 = instantiate_parameters (loop_nb, TREE_OPERAND (chrec, 1));
	res = chrec_fold_minus (op0, op1);
	break;
	
      case MULT_EXPR:
	op0 = instantiate_parameters (loop_nb, TREE_OPERAND (chrec, 0));
	op1 = instantiate_parameters (loop_nb, TREE_OPERAND (chrec, 1));
	res = chrec_fold_multiply (op0, op1);
	break;
	
      case ABS_EXPR:
	/* In general these nodes come from the symbolic computation
	   of the number of iterations.  These nodes are too difficult
	   to instantiate for the moment.  */
	res = chrec;
	break;
	
      default:
	switch (TREE_CODE_LENGTH (TREE_CODE (chrec)))
	  {
	  case 3:
	    op0 = instantiate_parameters 
	      (loop_nb, TREE_OPERAND (chrec, 0));
	    op1 = instantiate_parameters 
	      (loop_nb, TREE_OPERAND (chrec, 1));
	    op2 = instantiate_parameters 
	      (loop_nb, TREE_OPERAND (chrec, 2));
	    res = build (TREE_CODE (chrec), TREE_TYPE (chrec), op0, op1, op2);
	    break;

	  case 2:
	    op0 = instantiate_parameters 
	      (loop_nb, TREE_OPERAND (chrec, 0));
	    op1 = instantiate_parameters 
	      (loop_nb, TREE_OPERAND (chrec, 1));
	    res = build (TREE_CODE (chrec), TREE_TYPE (chrec), op0, op1);
	    break;
	    
	  case 1:
	    res = instantiate_parameters 
	      (loop_nb, TREE_OPERAND (chrec, 0));
	    if (!automatically_generated_chrec_p (res))
	      res = build1 (TREE_CODE (chrec), TREE_TYPE (chrec), res);
	    break;
	    
	  default:
	    res = chrec;
	    break;
	  }
	break;
      }
  
  return res;
}

/* Entry point for the analysis of the number of iterations pass.  
   
   This function tries to safely approximate the number of iterations
   the loop will run.  When this property is not decidable at compile
   time, the result is chrec_top: [-oo, +oo].  Otherwise the result is
   a scalar, an interval, or a symbolic parameter.
   
   Example of analysis: suppose that the loop has an exit condition:
   
   "if (b > 49) goto end_loop;"
   
   and that in a previous analysis we have determined that the
   variable 'b' has an evolution function:
   
   "EF = {23, +, 5}_2".  
   
   When we evaluate the function at the point 5, i.e. the value of the
   variable 'b' after 5 iterations in the loop, we have EF (5) = 48,
   and EF (6) = 53.  In this case the value of 'b' on exit is '53' and
   the loop body has been executed 6 times.  */

tree 
number_of_iterations_in_loop (struct loop *loop)
{
  int loop_nb = loop_num (loop);
  tree res;
  tree cond, test, opnd0, opnd1;
  tree chrec0, chrec1, chrec_cond;

  /* Determine whether the number_of_iterations_in_loop has already
     been computed.  */
  res = loop_nb_iterations (loop);
  if (res)
    return res;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "(number_of_iterations_in_loop \n");
  
  cond = get_loop_exit_condition (loop);
  if (cond == NULL_TREE)
    return set_nb_iterations_in_loop (loop, chrec_top);
  
  test = TREE_OPERAND (cond, 0);
  switch (TREE_CODE (test))
    {
    case SSA_NAME:
      /* "while (opnd0 != 0)", or in other terms, 
	 "exit when (chrec0 == 0)".  */
      opnd0 = test;
      chrec0 = analyze_scalar_evolution (loop_nb, opnd0);
      
      if (chrec0 == chrec_top)
	/* KEEP_IT_SYMBOLIC.  */
	chrec0 = opnd0;
      
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "  (loop_nb = %d)\n", loop_nb);
	  fprintf (tree_dump_file, "  (loop_while_expr_is_true: ");
	  print_generic_expr (tree_dump_file, test, 0);
	  fprintf (tree_dump_file, ")\n  (chrec0 = ");
	  print_generic_expr (tree_dump_file, chrec0, 0);
	  fprintf (tree_dump_file, ")\n");
	}
      
      if (chrec_contains_undetermined (chrec0))
	return cannot_analyze_loop_nb_iterations_yet ();
      
      else
	return set_nb_iterations_in_loop 
	  (loop, how_far_to_zero (loop_nb, chrec0));

    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
      opnd0 = TREE_OPERAND (test, 0);
      opnd1 = TREE_OPERAND (test, 1);
      chrec0 = analyze_scalar_evolution (loop_nb, opnd0);
      chrec1 = analyze_scalar_evolution (loop_nb, opnd1);
      
      chrec0 = instantiate_parameters (loop_nb, chrec0);
      chrec1 = instantiate_parameters (loop_nb, chrec1);
      
      chrec0 = homogenize_integer_cst_types_for_nb_iter (chrec0);
      chrec1 = homogenize_integer_cst_types_for_nb_iter (chrec1);
      
      if (chrec0 == chrec_top)
	/* KEEP_IT_SYMBOLIC.  */
	chrec0 = opnd0;
      
      if (chrec1 == chrec_top)
	/* KEEP_IT_SYMBOLIC.  */
	chrec1 = opnd1;
      
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "  (loop_nb = %d)\n", loop_nb);
	  fprintf (tree_dump_file, "  (loop_while_expr_is_true: ");
	  print_generic_expr (tree_dump_file, test, 0);
	  fprintf (tree_dump_file, ")\n  (chrec0 = ");
	  print_generic_expr (tree_dump_file, chrec0, 0);
	  fprintf (tree_dump_file, ")\n  (chrec1 = ");
	  print_generic_expr (tree_dump_file, chrec1, 0);
	  fprintf (tree_dump_file, ")\n");
	}
	
      if (chrec_contains_undetermined (chrec0)
	  || chrec_contains_undetermined (chrec1))
	return cannot_analyze_loop_nb_iterations_yet ();
	
      switch (TREE_CODE (test))
	{
	case LT_EXPR:
	  /* "while (opnd0 < opnd1)", or in other terms, 
	     "exit when (chrec0 - chrec1 + 1 > 0)".  */
	  chrec_cond = chrec_fold_plus 
	    (chrec_fold_minus (chrec0, chrec1), integer_one_node);
	  return set_nb_iterations_in_loop 
	    (loop, how_far_to_positive (loop_nb, chrec_cond));
	  
	case LE_EXPR:
	  /* "while (opnd0 <= opnd1)", or, 
	     "exit when (chrec0 - chrec1 > 0)".  */
	  chrec_cond = chrec_fold_minus (chrec0, chrec1);
	  return set_nb_iterations_in_loop 
	    (loop, how_far_to_positive (loop_nb, chrec_cond));
	  
	case GT_EXPR:
	  /* "while (opnd0 > opnd1)", or, 
	     "exit when (chrec1 - chrec0 + 1 > 0)".  */
	  chrec_cond = chrec_fold_plus 
	    (chrec_fold_minus (chrec1, chrec0), integer_one_node);
	  return set_nb_iterations_in_loop 
	    (loop, how_far_to_positive (loop_nb, chrec_cond));
	  
	case GE_EXPR:
	  /* "while (opnd0 >= opnd1)", or, 
	     "exit when (chrec1 - chrec0 > 0)".  */
	  chrec_cond = chrec_fold_minus (chrec1, chrec0);
	  return set_nb_iterations_in_loop 
	    (loop, how_far_to_positive (loop_nb, chrec_cond));
	    
	case EQ_EXPR:
	  /* "while (opnd0 == opnd1)", or,
	     "exit when (chrec0 - chrec1) != 0.  */
	  chrec_cond = chrec_fold_minus (chrec0, chrec1);
	  return set_nb_iterations_in_loop 
	    (loop, how_far_to_non_zero (loop_nb, chrec_cond));
	    
	case NE_EXPR:
	  /* "while (opnd0 != opnd1)", or, 
	     "exit when (chrec0 == chrec1)".  */
	  return set_nb_iterations_in_loop 
	    (loop, how_far_to_zero (loop_nb, chrec_fold_minus (chrec0, 
							       chrec1)));
	  
	default:
	  return set_nb_iterations_in_loop (loop, chrec_top);
	}
      
    default:
      return set_nb_iterations_in_loop (loop, chrec_top);
    }
}

/* Entry point for the analysis of the number of iterations pass.
   
   This function computes the number of iterations for all the loops
   from the EXIT_CONDITIONS array.  */

void 
number_of_iterations_for_all_loops (varray_type *exit_conditions)
{
  unsigned int i;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (*exit_conditions); i++)
    {
      tree exit_condition, nb_iter;
      
      exit_condition = VARRAY_TREE (*exit_conditions, i);
      nb_iter = number_of_iterations_in_loop (loop_of_stmt (exit_condition));
    }
  
  if (tree_dump_file)
    print_loop_ir (tree_dump_file);
}



static void initialize_scalar_evolutions_analyzer (void);
static void scev_init (void);
static void scev_analysis (void);
static void scev_depend (void);
static void scev_elim_checks (void);
static void scev_vectorize (void);
static void scev_done (void);
static bool gate_scev (void);
static bool gate_scev_analysis (void);
static bool gate_scev_depend (void);
static bool gate_scev_elim_checks (void);
static bool gate_scev_vectorize (void);

/* Initializer.  */

static void
initialize_scalar_evolutions_analyzer (void)
{
  /* The elements below are unique.  The values contained in these
     intervals are not used.  */
  chrec_not_analyzed_yet = build_interval_chrec 
    (build_int_2 (1111, 0), build_int_2 (2111, 0));
  chrec_top = build_interval_chrec 
    (build_int_2 (2222, 0), build_int_2 (3222, 0));
  chrec_bot = build_interval_chrec 
    (build_int_2 (3333, 0), build_int_2 (4333, 0));
}

/* Initialize the analysis of scalar evolutions.  */

static void
scev_init (void)
{
  current_loops = loop_optimizer_init (NULL);
  if (!current_loops)
    return;
  
  scalar_evolution_info_st = NULL;
  already_instantiated_st = NULL;
  VARRAY_GENERIC_PTR_INIT (scalar_evolution_info_st, 100, 
			   "scalar_evolution_info");
  VARRAY_TREE_INIT (already_instantiated_st, 37, 
		    "already_instantiated");
  scalar_evolution_info = &scalar_evolution_info_st;
  already_instantiated = &already_instantiated_st;
  
  initialize_scalar_evolutions_analyzer ();
}

/* Runs the analysis of scalar evolutions.  */

static void
scev_analysis (void)
{
  varray_type exit_conditions;

  VARRAY_GENERIC_PTR_INIT (exit_conditions, 37, "exit_conditions");
  select_loops_exit_conditions (current_loops, &exit_conditions);
  number_of_iterations_for_all_loops (&exit_conditions);
  VARRAY_CLEAR (exit_conditions);
}

/* Runs the analysis of all the data dependences.  */

static void
scev_depend (void)
{
  analyze_all_data_dependences ();
  dd_info_available = true;
}

static void
scev_elim_checks (void)
{
  eliminate_redundant_checks ();
}

/* Runs the linear loop transformations.  */

static void
scev_linear_transform (void)
{
  linear_transform_loops (current_loops, *scalar_evolution_info);
}

/* Runs the vectorization pass.  */

static void
scev_vectorize (void)
{
  bitmap_clear (vars_to_rename);

  vectorize_loops (current_loops, *scalar_evolution_info);
}

/* Finalize the scalar evolution passes.  */

static void
scev_done (void)
{
  if (current_loops)
    {
      VARRAY_CLEAR (*scalar_evolution_info);
      VARRAY_CLEAR (*already_instantiated);
      loop_optimizer_finalize (current_loops, NULL);
      current_loops = NULL;
    }

  dd_info_available = false;
}

static bool
gate_scev (void)
{
  return (flag_scalar_evolutions != 0
	  || flag_tree_vectorize != 0
	  || flag_all_data_deps != 0
	  || flag_tree_elim_checks != 0
	  || flag_tree_loop_linear != 0);
}

static bool
gate_scev_analysis (void)
{
  return current_loops && flag_scalar_evolutions != 0;
}

static bool
gate_scev_depend (void)
{
  return current_loops && flag_all_data_deps != 0;
}

static bool 
gate_scev_elim_checks (void)
{
  return current_loops && flag_tree_elim_checks != 0;
}

static bool
gate_scev_linear_transform (void)
{
  return current_loops && flag_tree_loop_linear != 0;
}


static bool
gate_scev_vectorize (void)
{
  return current_loops && flag_tree_vectorize != 0;
}

struct tree_opt_pass pass_scev = 
{
  NULL,                                 /* name */
  gate_scev,				/* gate */
  NULL,					/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func			/* todo_flags_finish */
};

struct tree_opt_pass pass_scev_init = 
{
  NULL,					/* name */
  NULL,					/* gate */
  scev_init,				/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0					/* todo_flags_finish */
};

struct tree_opt_pass pass_scev_anal = 
{
  "scev",				/* name */
  gate_scev_analysis,			/* gate */
  scev_analysis,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_SCALAR_EVOLUTIONS,			/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,        				/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0					/* todo_flags_finish */
};

struct tree_opt_pass pass_scev_depend = 
{
  "ddall",				/* name */
  gate_scev_depend,			/* gate */
  scev_depend,				/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_ALL_DATA_DEPS,			/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  PROP_scev,				/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0					/* todo_flags_finish */
};

struct tree_opt_pass pass_scev_vectorize = 
{
  "vect",				/* name */
  gate_scev_vectorize,			/* gate */
  scev_vectorize,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_VECTORIZATION,		/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_rename_vars	/* todo_flags_finish */
};

struct tree_opt_pass pass_scev_linear_transform =
{
  "ltrans",				/* name */
  gate_scev_linear_transform,		/* gate */
  scev_linear_transform,       		/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_LINEAR_TRANSFORM,  		/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func                	/* todo_flags_finish */
};


struct tree_opt_pass pass_scev_elim_checks = 
{
  "elck",				/* name */
  gate_scev_elim_checks,		/* gate */
  scev_elim_checks,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_ELIM_CHECKS,  		/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func                	/* todo_flags_finish */
};

struct tree_opt_pass pass_scev_done = 
{
  NULL,					/* name */
  NULL,					/* gate */
  scev_done,				/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0					/* todo_flags_finish */
};
