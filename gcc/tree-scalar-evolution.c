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

/* This pass analyses the evolution of scalar variables in loop
   structures.  The algorithm is based on the SSA representation, and
   on the loop hierarchy tree.
   
   A short sketch of the algorithm is:
   
   - First step: gather the scalar variables that has to be analyzed
   in an imperative list.  The minimal imperative list contains the
   variables that are used in the exit condition of the analyzed loop.
   
   - Second step: for each variable of the imperative list, analyze
   the dependences on other scalar variables in the loop nest.  The
   result is a Scalar Dependence Graph (SDG).
   
   - Third step: compute a schedule that satisfies all the dependences
   of the scalar dependence graph.
   
   - Fourth step: for each variable in the schedule, determine its
   initial condition and then its evolution in the loop nest.
   
   At the end of this analysis, a chain of recurrence is associated to
   each variable of the imperative list.  If a variable cannot be
   statically analyzed, the answer of the analyzer is: "don't know",
   that corresponds to the conservative [-oo, +oo] element of the
   lattice of intervals.
   
   You can find a more detailed description of the algorithm in:
   http://icps.u-strasbg.fr/~pop/DEA_03_Pop.pdf
   http://icps.u-strasbg.fr/~pop/DEA_03_Pop.ps.gz.  But note that this
   is a preliminary report and some of the details of the algorithm
   might have changed.  I'm working on a research report that updates
   the description of the algorithms to reflect the design choices
   used in this implementation.
   
   FIXME: The following cases are not handled yet, and some of them
   produce rough approximations.  I suspect that these cases could be
   handled by a more precise analyzer.
   
   - FIXME wrap_around: The evolution is broken in a loop by an
   assignment under the form "a = x ..." with "x" beeing either a
   variable different from "a", or a constant.  Note that this case
   happens only in a loop that contains a loop-phi node for "a",
   otherwise the evolution is analyzed only in the inner loop avoiding
   the analysis of the evolution in this loop.  As an example,
   consider the following code:
   
   a_0 = 0
   b_0 = 0
   loop 
     a_1 = phi (a_0, a_4)
     b_1 = phi (b_0, b_2)
     a_2 = a_1 + 5
     a_3 = b_1 + 1
     a_4 = a_3 + 2
     b_2 = b_1 + 1
   endloop
   
   In this example the evolution of "a" is not {0, +, 3}, but {0, +,
   1}.  All the updating assignments "a = a + ..." should not update
   the evolution of "a" in this loop.
   
   The general solution for a wrap_around variable is: {x0 - y0, *, 0}
   + {y0, +, y1, +, y1}.  But this cannot be simplified into a single
   chrec for the moment: see Taylor's FIXME.
   
   - FIXME taylor: This FIXME concerns all the cases where we have to
   deal with additions of exponential functions: "exp + exp" or "poly
   + exp" or "cst + exp".  This could be handled by a Taylor
   decomposition of the exponential function, but is still under
   construction (not implemented yet) for the moment.
   
   - FIXME floating-point: search for "case REAL_CST:".  This is just
   another extension that has to be implemented.  There are no
   difficulties except for the folding.  Do not fold the floating
   point operations, but let the trees grow up.  Using this unfolded
   form it is possible to analyze the numerical stability of the code
   in the loop...  Ref: "Static Analysis of the Numerical Stability of
   Loops" by Matthieu Martel.
   
   - FIXME analysis of mixers: This is one of the most difficult cases
   the analyzer has to deal with.  For the moment being, the analyzer
   answers with a rough approximation.

   - FIXME arithmetic flip-flop: As a mixer but flip-flops the values
   using arithmetic operations.
   
   - FIXME does this happen?  Some of the constructs of the underlying
   representation are not handled.  These cases may or may not happen.
   These FIXMEs can be solved only after the SSA representation is
   completely specified (with a grammar).
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


/* Analyzers on the SSA representation.  */
static void analyze_scalars_from_schedule (varray_type);
static void analyze_initial_condition  (tree);
static void analyze_evolution          (tree);
static void analyze_evolution_scc      (varray_type);
static void analyze_evolution_in_loop  (tree);
static void scev_analyze_temporary_modify_expr (struct loop *, tree, tree);
static tree is_ssa_name_a_version_of_variable (tree *, int *, void *);
static inline bool expression_contains_variable_p (tree, tree);
static tree remove_variable_from_expression (tree, tree, enum tree_code *);
static void analyze_non_gimple_initial_condition (unsigned, tree, tree,
						  tree, tree, tree *, tree *);
static void matched_an_increment (unsigned, tree, tree, tree, tree, 
				  tree *, tree *);
static void matched_an_exponentiation (unsigned, tree, tree, tree, tree, 
				       tree *, tree *);
static void matched_a_wrap_around (enum tree_code, unsigned, tree, tree, tree, 
				   tree *, tree *);
static void matched_an_arithmetic_wrap_around (unsigned, tree, tree, 
					       tree *, tree *);
static inline bool evolution_of_phi_already_analyzed_p (tree);
static tree symbolically_analyze (tree);
static void scev_analyze_modify_expr (unsigned, tree, tree, tree, 
				      tree *, tree *);
static void scev_follow_ssa_edge (tree, tree);
static void scev_follow_ssa_edge_same_loop (tree, tree);
static void scev_analyze_inner_loop_phi (tree, tree);
static void scev_follow_ssa_edge_inner_loop (tree, tree);
static tree compute_overall_effect_of_inner_loop (tree, unsigned);
static inline bool no_ssa_name_in_arguments_of_phi_node (tree);
static tree get_loop_phi_node_for_variable (struct loop *, tree);

/* Chrec functions.  */
static struct scev_info_str *find_var_scev_info (tree);
static void set_scev                   (unsigned, tree, tree);
static void set_scev_inner_value       (tree, tree);
static void set_scev_outer_value       (tree, tree);
static tree get_scev                   (unsigned, tree);
static tree merge_evolutions           (tree, varray_type);
static void merge_branches_of_condition_phi_node_in_loop (tree, tree);
static void merge_branches_of_condition_phi_node (tree);
static tree evolution_at_version       (unsigned, tree);
static tree select_outer_and_current_evolutions (unsigned, tree);
static tree build_polynomial_evolution_in_loop (unsigned, tree, tree);
static tree build_exponential_evolution_in_loop (unsigned, tree, tree);
static tree add_to_evolution           (unsigned, tree, tree);
static tree multiply_evolution         (unsigned, tree, tree);
static inline tree cannot_analyze_loop_nb_iterations_yet (void);
static inline tree set_nb_iterations_in_loop (struct loop *, tree);
static tree homogenize_integer_cst_types_for_nb_iter (tree);
static tree nb_iterations_in_loop_until_eq (unsigned, tree, tree);

/* Scalar Dependence Graph functions.  */
static void construct_schedule (unsigned, tree, varray_type);

static bool record_dependences_for_version (unsigned, tree, varray_type, 
					    varray_type);
static void record_dependences_for_opnd (unsigned, tree, tree, 
					 varray_type, varray_type);
static void record_dependences_for_phi_node (unsigned, tree, 
					     varray_type, varray_type);
static void record_dependences_for_modify_expr (unsigned, tree, 
						varray_type, varray_type);
static void record_dependences_for_definition (unsigned, tree, 
					       varray_type, varray_type);
static void SDD_insert (tree, tree, varray_type);
static void wolfe_topological_sort_SDD_with_SCCs (varray_type, varray_type, 
						  varray_type);
static void wolfe_scc_recurse (unsigned int, varray_type, varray_type, 
			       varray_type, varray_type, varray_type, 
			       varray_type, varray_type, unsigned int *);

/* Selection functions.  */
static void get_exit_conditions_rec    (struct loop *, varray_type);
static bool analyzable_condition       (tree);

/* Debugging functions.  */
extern void dump_schedule_scc (FILE *, varray_type);
extern void dump_schedule_elt (FILE *, struct schedule_elt *);
extern void debug_schedule (varray_type);
extern void draw_SDD (varray_type);
extern void sdd2dot (FILE *, varray_type);

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

/* The following nodes are qualified as symbolic parameters:
   - the LHS of an assignment whose RHS is undecidable, 
   - the function's parameters.  */
tree chrec_symbolic_parameter;

static struct loops *scev_loops;
static varray_type scev_info;



/* Loop related functions.  */

static inline bool stmt_is_in_loop (tree, unsigned);

/* Determines whether STMT is in loop LOOP_NB.  */

static inline bool
stmt_is_in_loop (tree stmt, unsigned loop_nb)
{
  return flow_bb_inside_loop_p (loop_from_num (scev_loops, loop_nb),
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
  
  return flow_loop_nested_p (loop_from_num (scev_loops, b),
			     loop_from_num (scev_loops, a));
}

/* Determines whether loop A is strictly contained in loop B.  */

static bool
loop_is_strictly_included_in (unsigned a, unsigned b)
{
  if (a == b)
    return false;
  
  return flow_loop_nested_p (loop_from_num (scev_loops, b),
			     loop_from_num (scev_loops, a));
}

/* Observers for the EDGE struct.  */

static inline basic_block edge_src (edge);

/* Returns the block at the beginning of the edge.  */

static inline basic_block
edge_src (edge e)
{
  return e->src;
}

/* Imperative list functions.  */

static inline tree ssa_name_at_index_in_imperative_list (int, varray_type);
static inline tree var_at_index_in_imperative_list (int, varray_type);
static inline int index_of_variable_in_imperative_list (tree, varray_type);
static inline bool variable_is_in_imperative_list_p (tree, varray_type);
static inline void insert_ssa_name_in_imperative_list (tree, varray_type);

/* Returns the SSA_NAME at INDEX from the imperative list.  */

static inline tree 
ssa_name_at_index_in_imperative_list (int index, 
				      varray_type imperative_list)
{
  struct schedule_elt *sched_elt;
  
  sched_elt = (struct schedule_elt *) VARRAY_GENERIC_PTR (imperative_list, 
							  index);
  return VARRAY_TREE (SCHED_SCC (sched_elt), 0);
}

/* Returns the variable at INDEX from the imperative list.  */

static inline tree 
var_at_index_in_imperative_list (int index, 
				 varray_type imperative_list)
{
  return SSA_NAME_VAR (ssa_name_at_index_in_imperative_list (index, imperative_list));
}

/* Determines the index of VAR in the imperative list.  */

static inline int
index_of_variable_in_imperative_list (tree var, 
				      varray_type imperative_list)
{
  unsigned int j;
  
  for (j = 0; j < VARRAY_ACTIVE_SIZE (imperative_list); j++)
    if (var_at_index_in_imperative_list (j, imperative_list) == var)
      return j;
  
  return -1;
}

/* Determines whether the variable VAR is already in the list.  */

static inline bool
variable_is_in_imperative_list_p (tree var, 
				  varray_type imperative_list)
{
  if (index_of_variable_in_imperative_list (var, imperative_list) == -1)
    return false;
  else
    return true;
}

/* Insert the variable VAR in the imperative list.  */

static inline void
insert_ssa_name_in_imperative_list (tree ssa_name,
				    varray_type imperative_list)
{
  struct schedule_elt *sched_elt;
  
  DBG_S (fprintf (stderr, "(insert_ssa_name_in_imperative_list \n");
	 fprintf (stderr, "  (ssa_name = ");
	 debug_generic_expr (ssa_name);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, ")\n"));
  
  sched_elt = new_schedule_elt ();
  VARRAY_PUSH_TREE (SCHED_SCC (sched_elt), ssa_name);
  VARRAY_PUSH_GENERIC_PTR (imperative_list, sched_elt);
}


/* This section contains the interface to the SSA IR.  */

/* A quick hack for avoiding infinite recursion.  For example, in
   gcc/gcc/calls.c:expand_call, we have the following phi nodes:
   
   "normal_call_insns_8 = PHI <0B(380), normal_call_insns_9(782)>;
   normal_call_insns_9 = PHI <normal_call_insns_8(773), T.1529_2087(777), T.1529_2087(778)>;"
   
   This results in a loop in the PHI walking.  By the way, is this
   normally allowed in the SSA representation?  */

static varray_type already_visited;
static bool node_already_visited_by_ssa_path (tree node);

static bool
node_already_visited_by_ssa_path (tree node)
{
  if (tree_is_in_varray_tree_p (node, already_visited))
    return true;
  
  else
    {
      VARRAY_PUSH_TREE (already_visited, node);
      return false;
    }
}

/* Analyze the evolution of a SSA_NAME.  */

static void 
analyze_evolution (tree version_to_analyze)
{
  tree definition_to_analyze = SSA_NAME_DEF_STMT (version_to_analyze);
  tree variable_to_analyze = SSA_NAME_VAR (version_to_analyze);
  tree loop_phi_node;
  
  DBG_S (fprintf (stderr, "(analyze_evolution \n");
	 fprintf (stderr, "  (version_to_analyze = ");
	 debug_generic_expr (version_to_analyze);
	 fprintf (stderr, "  )\n"));
  
  if (definition_to_analyze != NULL_TREE
      && TREE_CODE (definition_to_analyze) != NOP_EXPR)
    {
      loop_phi_node = get_loop_phi_node_for_variable 
	(loop_of_stmt (definition_to_analyze), variable_to_analyze);
      
      /* When the variable has a phi-node in the loop, the variable is
	 "updated" at every iteration of the loop.  */
      if (loop_phi_node)
	{
	  /* Avoid recursions when there are circular phi nodes, as in
	     gcc/gcc/calls.c:expand_call, where we have the following phis:
	     
	     "normal_call_insns_8 = PHI <0B(380), normal_call_insns_9(782)>;
	     ...
	     normal_call_insns_9 = PHI <normal_call_insns_8(773), T.1529_2087(777), T.1529_2087(778)>;"
	  */
	  if (PHI_MARKED (loop_phi_node) == 1)
	    return;
	  
	  else
	    PHI_MARKED (loop_phi_node) = 1;
	  
	  analyze_initial_condition (loop_phi_node);
	  analyze_evolution_in_loop (loop_phi_node);
	  
	  PHI_MARKED (loop_phi_node) = 0;
	}

      /* Otherwise, the variable is a temporary (such as those
	 introduced by the gimplification).  All the operands of the
	 definition are already analyzed.  The only thing that remains
	 is to determine an approximation of the definition.  */
      else
	{
	  /* Follow the edge to the definition.  */
	  tree rdef = definition_to_analyze;
	  
	  switch (TREE_CODE (rdef))
	    {
	    case PHI_NODE:
	      /* The only possible case is that this is a condition
		 phi-node.  */
	      merge_branches_of_condition_phi_node (rdef);
	      break;
	      
	    case MODIFY_EXPR:
	      scev_analyze_temporary_modify_expr (loop_of_stmt (rdef),
						  TREE_OPERAND (rdef, 0), 
						  TREE_OPERAND (rdef, 1));
	      break;
	      
	    default:
	      break;
	    }
	}
    }
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* Analyze the evolution of the variables in MIXER.  

   FIXME analysis of mixers:

   At this moment, we have all the information we need to analyze the
   mixer (ie. the evolution of all the other variables on which the
   components of the mixer depends on).
   
   However, for the moment, the analyzer associates to each variable 
   in the MIXER its initial condition, and an evolution of [-oo, +oo].  */

static void 
analyze_evolution_scc (varray_type mixer ATTRIBUTE_UNUSED)
{
  DBG_S (fprintf (stderr, "(analyze_evolution_scc \n");
	 dump_schedule_scc (stderr, mixer));

#if 0  
  unsigned k;
  
  for (k = 0; k < VARRAY_ACTIVE_SIZE (mixer); k++)
    {
      tree var_to_analyze, loop_phi_node;
      
      var_to_analyze = SSA_NAME_VAR (VARRAY_TREE (mixer, k));
      loop_phi_node = get_loop_phi_node_for_variable 
	(loop_nest, var_to_analyze);
      
      if (loop_phi_node)
	{
	  /* The variable has a phi node in this loop: the variable is
	     thus "updated" at every iteration of this loop.
	     
	     analyze_initial_condition (loop_nest, loop_phi_node);
	     
	     FIXME analysis of mixers: For the moment, set the
	     evolution in this loop to chrec_top.
	     
	     set_scev (loop_nest->num, var_to_analyze, 
	     reset_evolution_in_loop (loop_nest->num, 
	     get_scev (0, var_to_analyze),
	     chrec_top));  */
	  set_scev (loop_num (loop_nest), var_to_analyze, chrec_top);
	}
    }
#endif
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* Given a loop phi-node, this function determines the initial
   conditions of the variable on entry of the loop.  When the CCP has
   propagated constants into the loop-phi-node, the initial condition
   is instantiated, otherwise the initial condition is kept symbolic.
   This analyzer does not analyze the evolution outside the current
   loop, and leaves this task to the on-demand tree reconstructor.  */

static void 
analyze_initial_condition (tree loop_phi_node)
{
  int i;
  tree new_initial_condition = chrec_not_analyzed_yet;
  tree new_evolution = chrec_not_analyzed_yet;
  tree original_chrec;
  varray_type branch_chrecs;
  
#if defined ENABLE_CHECKING
  if (loop_phi_node == NULL_TREE)
    abort ();
#endif
  
  DBG_S (fprintf (stderr, "(analyze_initial_condition \n");
	 fprintf (stderr, "  (loop_phi_node = \n");
	 debug_generic_expr (loop_phi_node);
	 fprintf (stderr, "  )\n"));
  
  VARRAY_TREE_INIT (branch_chrecs, 2, "branch_chrecs");
  original_chrec = get_scev (0, SSA_NAME_VAR (PHI_RESULT (loop_phi_node)));
  
  for (i = 0; i < PHI_NUM_ARGS (loop_phi_node); i++)
    {
      tree upper_chain;
      tree branch = PHI_ARG_DEF (loop_phi_node, i);
      tree branch_effect = chrec_not_analyzed_yet;
      
      /* The arguments of a phi node can be classified into two cases:
	 
	 - the argument is constant (this is a side effect of the copy
           constant propagation),
	   
	 - the argument points on a variable defined outside the
           current loop.  */
      switch (TREE_CODE (branch))
	{
	case SSA_NAME:
	  upper_chain = SSA_NAME_DEF_STMT (branch);
	  
	  /* When the phi node has a NOP_EXPR or a NULL_TREE argument,
	     then this argument is the initial condition, and thus the
	     initial condition is not known.  No precise information
	     can be extracted from the program: the initial condition
	     is set by default at [-oo, +oo].
	     
	     FIXME: I'm not sure whether this is a correct
	     interpretation of the SSA representation, but setting the
	     branch_effect to chrec_top is a safe answer.  */
	  if (upper_chain == NULL_TREE 
	      || TREE_CODE (upper_chain) == NOP_EXPR)
	    branch_effect = chrec_top;
	  
	  /* When UPPER_CHAIN is a definition outside the current
	     loop nest, KEEP_IT_SYMBOLIC.  */
	  else if (loop_depth (loop_of_stmt (upper_chain))
		   < loop_depth (loop_of_stmt (loop_phi_node)))
	    branch_effect = evolution_at_version 
	      (loop_num (loop_of_stmt (loop_phi_node)), branch);
	  
	  /* When the branch is oriented to the loop's body, we don't
	     want to push the effect of this branch: it does not
	     contribute to the initial condition.  */
	  else
	    continue;
	  
	  break;
	  
	default:
	  /* In the default case fall all the scalars propagated in
	     the loop phi node by the CCP.  */
	  branch_effect = get_scev 
	    (loop_num (loop_of_stmt (loop_phi_node)), branch);
	  break;
	}
      
      VARRAY_PUSH_TREE (branch_chrecs, branch_effect);
    }

  if (VARRAY_ACTIVE_SIZE (branch_chrecs) == 1)
    new_initial_condition = initial_condition (VARRAY_TREE (branch_chrecs, 0));
  
  /* When there are multiple branches that go outside the loop, then
     the result is the merge of all these branches.  */
  else
    new_initial_condition = initial_condition 
      (merge_evolutions (original_chrec, branch_chrecs));
  
  new_evolution = replace_initial_condition (original_chrec, 
					     new_initial_condition);
  
  varray_clear (branch_chrecs);
  
  set_scev_inner_value (PHI_RESULT (loop_phi_node), 
			new_initial_condition);
  
  set_scev (0, SSA_NAME_VAR (PHI_RESULT (loop_phi_node)), new_evolution);
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* Given a loop phi-node LOOP_PHI_NODE for a variable VAR, and the
   outer loop phi-node HALTING_PHI, this function determines the
   evolution function for VAR in the current loop.  */

static void
analyze_evolution_in_loop (tree loop_phi_node)
{
  int i;
  
#if defined ENABLE_CHECKING
  if (loop_phi_node == NULL_TREE)
    abort ();
#endif
  
  DBG_S (fprintf (stderr, "(analyze_evolution_in_loop \n");
	 fprintf (stderr, "  (loop_phi_node = \n");
	 debug_generic_expr (loop_phi_node);
	 fprintf (stderr, "  )\n"));
  
  for (i = 0; i < PHI_NUM_ARGS (loop_phi_node); i++)
    {
      tree arg = PHI_ARG_DEF (loop_phi_node, i);
      
      /* The arguments that are not SSA_NAMEs don't come from the
	 loop's body.  */
      if (TREE_CODE (arg) == SSA_NAME)
	{
	  tree inner_chain = SSA_NAME_DEF_STMT (arg);
	  
	  /* Select the edges that enter the loop body.  */
	  if (inner_chain != NULL_TREE 
	      && TREE_CODE (inner_chain) != NOP_EXPR)
	    {
	      if (loop_of_stmt (inner_chain) == loop_of_stmt (loop_phi_node))
		{
		  if (SSA_NAME_VAR (arg) 
		      == SSA_NAME_VAR (PHI_RESULT (loop_phi_node)))
		    scev_follow_ssa_edge_same_loop (inner_chain, loop_phi_node);
		  
		  else
		    {
		      /* The phi node is under the form: 
			 "a = phi (..., t, ...)", where t has already 
			 been analyzed symbolically. 
			 testsuite/.../ssa-chrec-36.c  */
		      tree evolution_function, effect_after_execution;
		      unsigned loop_nb = loop_num (loop_of_stmt (loop_phi_node));
		      
		      scev_analyze_modify_expr 
			(loop_nb,
			 PHI_RESULT (loop_phi_node),
			 arg,
			 loop_phi_node, 
			 &evolution_function, 
			 &effect_after_execution);
		      
		      /* Set the evolution_function, but don't update the
			 effect_after_execution, because the loop-phi-node
			 has the right initial condition already set, and
			 its value on exit of the loop is computed after
			 the computation of the number of iterations.  */
		      set_scev (0, SSA_NAME_VAR (PHI_RESULT (loop_phi_node)), 
				evolution_function);
		    }
		}
	      
	      else if (loop_is_included_in 
		       (loop_num (loop_of_stmt (inner_chain)),
			loop_num (loop_of_stmt (loop_phi_node))))
		scev_follow_ssa_edge_inner_loop (inner_chain, loop_phi_node);
	    }
	}
    }

  DBG_S (fprintf (stderr, ")\n"));
}

/* Analyzes a modify expression whose result is a temporary name.
   This function is similar to the scev_follow_ssa_edge.  It does not
   recognize evolution patterns such as "i = i + ...", but only the
   expressions: "T = U op V", where all the operands have already
   analyzed evolutions.  The modify expression is under the form:
   "OPND0 = OPND1".  */

static void
scev_analyze_temporary_modify_expr (struct loop *loop,
				    tree opnd0,
				    tree opnd1)
{
  unsigned loop_nb = loop_num (loop);
  
  DBG_S (fprintf (stderr, "(scev_analyze_temporary_modify_expr \n");
	 fprintf (stderr, "  (opnd0 = \n");
	 debug_generic_expr (opnd0);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, "  (opnd1 = \n");
	 debug_generic_expr (opnd1);
	 fprintf (stderr, "  )\n"));
  
  /* The OPND1 is one of the following cases:
     - an SSA_NAME, 
     - an INTEGER_CST,
     - a PLUS_EXPR, 
     - a MINUS_EXPR,
     - other cases are not yet handled. 
  */
  switch (TREE_CODE (opnd1))
    {
    case INTEGER_CST:
      {
	tree res;
	
	/* This assignment is under the form "a_1 = 7".  */
	res = evolution_at_version (loop_nb, opnd1);
	set_scev (loop_nb, opnd0, res);
	set_scev (0, SSA_NAME_VAR (opnd0), res);
	break;
      }

    case SSA_NAME:
      if (SSA_NAME_VAR (opnd0) == SSA_NAME_VAR (opnd1))
	{
	  /* FIXME does this happen?
	     This is a strange case: "a_1 = a_2".  */
	  abort ();
	}
      else
	{
	  tree res;
	  
	  /* This assignment is under the form: "a_1 = b_2".  */
	  res = evolution_at_version (loop_nb, opnd1);
	  set_scev (loop_nb, opnd0, initial_condition (res));
	  set_scev (0, SSA_NAME_VAR (opnd0), res);
	}
      break;
      
    case PLUS_EXPR:
      {
	/* This case is under the form "opnd0 = opnd10 + opnd11", or
	   "T = x + y", where T is the temporary variable, and x, y
	   two scalars.  */
	tree opnd10, opnd11;
	tree chrec10, chrec11, chrec_sum;
	
	opnd10 = TREE_OPERAND (opnd1, 0);
	opnd11 = TREE_OPERAND (opnd1, 1);
	chrec10 = evolution_at_version (loop_nb, opnd10);
	chrec11 = evolution_at_version (loop_nb, opnd11);
	
	chrec_sum = chrec_fold_plus (chrec10, chrec11);
	set_scev (loop_nb, opnd0, initial_condition (chrec_sum));

	if (loop_depth (loop) == 0)
	  {
	    /* In the outermost loop strange things can happen...  */
	    tree evolution_part = evolution_part_in_loop_num
	      (chrec_sum, loop_nb);
	    
	    if (evolution_part != NULL_TREE)
	      set_scev (0, SSA_NAME_VAR (opnd0), reset_evolution_in_loop 
			(loop_nb, get_scev (0, SSA_NAME_VAR (opnd0)), 
			 evolution_part));
    	  }
	
	else
	  set_scev (0, SSA_NAME_VAR (opnd0), chrec_sum);
	
	break;
      }
      
    case MINUS_EXPR:
      {
	/* This case is under the form "opnd0 = opnd10 - opnd11", or
	   "T = x - y", where T is the temporary variable, and x, y
	   two scalars.  */
	tree opnd10, opnd11;
	tree chrec10, chrec11, chrec_diff;
	
	opnd10 = TREE_OPERAND (opnd1, 0);
	opnd11 = TREE_OPERAND (opnd1, 1);
	chrec10 = evolution_at_version (loop_nb, opnd10);
	chrec11 = evolution_at_version (loop_nb, opnd11);
	
	chrec_diff = chrec_fold_minus (chrec10, chrec11);
	set_scev (loop_nb, opnd0, initial_condition (chrec_diff));
	
	if (loop_depth (loop) == 0)
	  {
	    /* In the outermost loop strange things can happen...
	       since we don't have the "okay, once we're at the bottom
	       of the loop's body, we start over again from the
	       beginning of the loop" thing.  see
	       testsuite/.../ssa-chrec-46.c  */
	    tree evolution_part = evolution_part_in_loop_num
	      (chrec_diff, loop_nb);
	    
	    if (evolution_part != NULL_TREE)
	      set_scev (0, SSA_NAME_VAR (opnd0), reset_evolution_in_loop 
			(loop_nb, get_scev (0, SSA_NAME_VAR (opnd0)), 
			 evolution_part));
    	  }

	else
	  set_scev (0, SSA_NAME_VAR (opnd0), chrec_diff);
	
	break;
      }
      
    case MULT_EXPR:
      {
	/* This case is under the form "opnd0 = opnd10 * opnd11", or
	   "T = x * y", where T is the temporary variable, and x, y
	   two scalars.  */
	tree opnd10, opnd11;
	tree chrec10, chrec11, chrec_prod;
	
	opnd10 = TREE_OPERAND (opnd1, 0);
	opnd11 = TREE_OPERAND (opnd1, 1);
	chrec10 = evolution_at_version (loop_nb, opnd10);
	chrec11 = evolution_at_version (loop_nb, opnd11);
	
	chrec_prod = chrec_fold_multiply (chrec10, chrec11);
	set_scev (loop_nb, opnd0, initial_condition (chrec_prod));
	
	if (loop_depth (loop) == 0)
	  {
	    /* In the outermost loop strange things can happen...  */
	    tree evolution_part = evolution_part_in_loop_num
	      (chrec_prod, loop_nb);
	    
	    if (evolution_part != NULL_TREE)
	      set_scev (0, SSA_NAME_VAR (opnd0), reset_evolution_in_loop 
			(loop_nb, get_scev (0, SSA_NAME_VAR (opnd0)), 
			 evolution_part));
	  }
	
	else
	  set_scev (0, SSA_NAME_VAR (opnd0), chrec_prod);
	
	break;
      }
      
    default:
      set_scev (loop_nb, opnd0, chrec_top);
      set_scev (0, SSA_NAME_VAR (opnd0), chrec_top);
      break;
    }
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* Helper function for walk_tree.  Determines whether the tree pointer
   TP is a version of the variable DATA.  */

static tree
is_ssa_name_a_version_of_variable (tree *tp, 
				   int *walk_subtrees ATTRIBUTE_UNUSED,
				   void *data)
{
  tree *variable = (tree *) data;
  
  if (TREE_CODE (*tp) == SSA_NAME
      && SSA_NAME_VAR (*tp) == *variable)
    return *tp;
  
  else
    return NULL_TREE;
}

/* Determines whether EXPR contains versions of VARIABLE.  */

static inline bool
expression_contains_variable_p (tree expr, 
				tree variable)
{
  tree res = walk_tree (&expr, is_ssa_name_a_version_of_variable, &variable, NULL);
  
  return (res != NULL_TREE);
}

/* Strips the first occurrence of VARIABLE from EXPR, and return the
   tree_code of the subexpression that was deleted from RES.  */

static tree
remove_variable_from_expression (tree variable, 
				 tree expr, 
				 enum tree_code *code)
{
  switch (TREE_CODE (expr))
    {
    case PLUS_EXPR:
      if (TREE_CODE (TREE_OPERAND (expr, 0)) == SSA_NAME
	  && SSA_NAME_VAR (TREE_OPERAND (expr, 0)) == variable)
	{
	  *code = PLUS_EXPR;
	  return TREE_OPERAND (expr, 1);
	}
      
      else if (TREE_CODE (TREE_OPERAND (expr, 1)) == SSA_NAME
	       && SSA_NAME_VAR (TREE_OPERAND (expr, 1)) == variable)
	{
	  *code = PLUS_EXPR;
	  return TREE_OPERAND (expr, 0);
	}
      
      else 
	{
	  tree opnd0, opnd1;
	  
	  opnd0 = remove_variable_from_expression 
	    (variable, TREE_OPERAND (expr, 0), code);
	  
	  /* When the node has been found in opnd0, don't search for
	     it in opnd1.  */
	  if (*code != NOP_EXPR)
	    opnd1 = TREE_OPERAND (expr, 1);
	  
	  else
	    opnd1 = remove_variable_from_expression 
	      (variable, TREE_OPERAND (expr, 1), code);
	  
	  return build (PLUS_EXPR, TREE_TYPE (expr), opnd0, opnd1);
	}
      
    case MULT_EXPR:
      if (TREE_CODE (TREE_OPERAND (expr, 0)) == SSA_NAME
	  && SSA_NAME_VAR (TREE_OPERAND (expr, 0)) == variable)
	{
	  *code = MULT_EXPR;
	  return TREE_OPERAND (expr, 1);
	}
      
      else if (TREE_CODE (TREE_OPERAND (expr, 1)) == SSA_NAME
	       && SSA_NAME_VAR (TREE_OPERAND (expr, 1)) == variable)
	{
	  *code = MULT_EXPR;
	  return TREE_OPERAND (expr, 0);
	}
      
      else 
	{
	  tree opnd0, opnd1;
	  
	  opnd0 = remove_variable_from_expression 
	    (variable, TREE_OPERAND (expr, 0), code);
	  
	  /* When the node has been found in opnd0, don't search for
	     it in opnd1.  */
	  if (*code != NOP_EXPR)
	    opnd1 = TREE_OPERAND (expr, 1);
	  
	  else
	    opnd1 = remove_variable_from_expression 
	      (variable, TREE_OPERAND (expr, 1), code);
	  
	  return build (MULT_EXPR, TREE_TYPE (expr), opnd0, opnd1);
	}
      
    default:
      switch (TREE_CODE_LENGTH (TREE_CODE (expr)))
	{
	case 2:
	  {
	    tree opnd0, opnd1;
	    
	    opnd0 = remove_variable_from_expression 
	      (variable, TREE_OPERAND (expr, 0), code);
	    
	    /* When the node has been found in opnd0, don't search for
	       it in opnd1.  */
	    if (*code != NOP_EXPR)
	      opnd1 = TREE_OPERAND (expr, 1);
	    
	    else
	      opnd1 = remove_variable_from_expression 
		(variable, TREE_OPERAND (expr, 1), code);
	    return build (TREE_CODE (expr), TREE_TYPE (expr), opnd0, opnd1);
	  }
	  
	case 1:
	  {
	    tree opnd0 = remove_variable_from_expression 
	      (variable, TREE_OPERAND (expr, 0), code);
	    
	    return build1 (TREE_CODE (expr), TREE_TYPE (expr), opnd0);
	  }
	  
	default:
	  return expr;
	}
    }
}

/* When the gimplifier transforms an assignment into

   temp_var = i + j;
   i = temp_var + 2;
   
   the scalar evolution analyzer reconstructs the tree and ends to
   analyze the expression "i = i + j + 2".  This function deals with
   these reconstructed expressions.  */

static void
analyze_non_gimple_initial_condition (unsigned loop_nb, 
				      tree opnd0,
				      tree chrec_before,
				      tree chrec1,
				      tree init_cond,
				      tree *evolution_function, 
				      tree *effect_after_execution)
{
  tree stripped_expr;
  tree init_cond_chrec_before;
  enum tree_code code = NOP_EXPR;
  
  stripped_expr = remove_variable_from_expression 
    (SSA_NAME_VAR (opnd0), init_cond, &code);
  init_cond_chrec_before = initial_condition (chrec_before);
  
  switch (code)
    {
    case PLUS_EXPR:
      {
	tree to_be_added = replace_initial_condition 
	  (chrec1, stripped_expr);
	
	*evolution_function = add_to_evolution 
	  (loop_nb, init_cond_chrec_before, to_be_added);
	*effect_after_execution = chrec_eval_next_init_cond 
	  (loop_nb, *evolution_function);
	break;
      }
      
    case MULT_EXPR:
      {
	tree to_be_multiplied = replace_initial_condition 
	  (chrec1, stripped_expr);
	
	*evolution_function = multiply_evolution
	  (loop_nb, init_cond_chrec_before, to_be_multiplied);
	*effect_after_execution = chrec_eval_next_init_cond 
	  (loop_nb, *evolution_function);
	break;
      }
  
    default:
      *effect_after_execution = chrec1;
      *evolution_function = reset_evolution_in_loop 
	(loop_nb, chrec_before, chrec_top);
      break;
    }
}

/* The following pattern has been matched: "a_1 = a_2 + ...".
   OPND10 is "a_2", OPND11 is the rest "...".
   This function updates the EVOLUTION_FUNCTION and the
   EFFECT_AFTER_EXECUTION consequently.  */

static void
matched_an_increment (unsigned loop_nb, 
		      tree var, 
		      tree opnd10, 
		      tree opnd11,
		      tree halting_phi,
		      tree *evolution_function, 
		      tree *effect_after_execution)
{
  tree upper_chain = SSA_NAME_DEF_STMT (opnd10);
  tree chrec10, chrec11, chrec_before;
  tree to_be_added;
  
  /* Recursively construct the SSA path.  */
  scev_follow_ssa_edge (upper_chain, halting_phi);
    
  /* Analyze the assignment on the return walk.  */
  chrec_before = get_scev (0, var);
  if (chrec_should_remain_symbolic (chrec_before))
    /* KEEP_IT_SYMBOLIC.  */
    chrec_before = opnd10;
  
  chrec10 = evolution_at_version (loop_nb, opnd10);
  chrec11 = evolution_at_version (loop_nb, opnd11);
  
  if (chrec_should_remain_symbolic (chrec11))
    /* KEEP_IT_SYMBOLIC.  
       Don't propagate unknown values, but instead, 
       keep the evolution function under a symbolic 
       form.  Example: testsuite/.../ssa-chrec-17.c.  */
    to_be_added = opnd11;
  
  else
    to_be_added = chrec11;
  
  *effect_after_execution = chrec_fold_plus 
    (initial_condition (chrec10), 
     initial_condition (to_be_added));
  
  to_be_added = select_outer_and_current_evolutions 
    (loop_nb, to_be_added);
  *evolution_function = add_to_evolution 
    (loop_nb, chrec_before, to_be_added);
}

/* The following pattern has been matched: "a_1 = a_2 * ...".
   OPND10 is "a_2", OPND11 is the rest "...".
   This function updates the EVOLUTION_FUNCTION and the
   EFFECT_AFTER_EXECUTION consequently.  */

static void
matched_an_exponentiation (unsigned loop_nb, 
			   tree var, 
			   tree opnd10, 
			   tree opnd11,
			   tree halting_phi,
			   tree *evolution_function, 
			   tree *effect_after_execution)
{
  tree upper_chain = SSA_NAME_DEF_STMT (opnd10);
  tree chrec10, chrec11, chrec_before;
  tree to_be_multiplied;
  
  /* Recursively construct the SSA path.  */
  scev_follow_ssa_edge (upper_chain, halting_phi);
  
  /* Analyze the assignment on the return walk.  */
  chrec_before = get_scev (0, var);
  if (chrec_should_remain_symbolic (chrec_before))
    /* KEEP_IT_SYMBOLIC.  */
    chrec_before = opnd10;
  
  chrec10 = evolution_at_version (loop_nb, opnd10);
  chrec11 = evolution_at_version (loop_nb, opnd11);
  if (chrec_should_remain_symbolic (chrec11))
    /* KEEP_IT_SYMBOLIC.  */
    to_be_multiplied = opnd11;
  else
    to_be_multiplied = chrec11;
  
  *effect_after_execution = chrec_fold_multiply 
    (initial_condition (chrec10), 
     initial_condition (to_be_multiplied));
  
  *evolution_function = multiply_evolution 
    (loop_nb, chrec_before, to_be_multiplied);
}

/* The following pattern has been matched: "a = b code c".  

   This function updates the EVOLUTION_FUNCTION and the
   EFFECT_AFTER_EXECUTION consequently.  */

static void
matched_a_wrap_around (enum tree_code code,
		       unsigned loop_nb,
		       tree var,
		       tree opnd0, 
		       tree opnd1, 
		       tree *evolution_function, 
		       tree *effect_after_execution)
{
  tree opnd10, opnd11;
  tree chrec10, chrec11;
  tree init_cond, chrec1;
  tree chrec_before;
  
  opnd10 = TREE_OPERAND (opnd1, 0);
  opnd11 = TREE_OPERAND (opnd1, 1);
  
  chrec_before = get_scev (0, var);
  if (chrec_should_remain_symbolic (chrec_before))
    /* KEEP_IT_SYMBOLIC.  */
    chrec_before = opnd1;
  
  chrec10 = evolution_at_version (loop_nb, opnd10);
  chrec11 = evolution_at_version (loop_nb, opnd11);
  
  switch (code)
    {
    case PLUS_EXPR:
      chrec1 = chrec_fold_plus (chrec10, chrec11);
      break;
      
    case MINUS_EXPR:
      chrec1 = chrec_fold_minus (chrec10, chrec11);
      break;
      
    case MULT_EXPR:
      chrec1 = chrec_fold_multiply (chrec10, chrec11);
      break;
      
    default:
      chrec1 = chrec_top;
      break;
    }
  
  init_cond = initial_condition (chrec1);
  
  /* If VAR occurs in the initial condition of opnd1
     then the analyzed modify expression is not a
     wrap-around: it is just an expression that has
     been gimplified using temporary variables.  */
  if (expression_contains_variable_p 
      (init_cond, SSA_NAME_VAR (opnd0)))
    analyze_non_gimple_initial_condition 
      (loop_nb, opnd0, chrec_before, 
       chrec1, init_cond,
       evolution_function, effect_after_execution);
  
  else
    {
      /* FIXME wrap_around.  */
      *effect_after_execution = chrec1;
      *evolution_function = reset_evolution_in_loop 
	(loop_nb, chrec_before, chrec_top);
    }
}

/* The following pattern has been matched: "a_1 = b_2 - a_3".  
   
   This function updates the EVOLUTION_FUNCTION and the
   EFFECT_AFTER_EXECUTION consequently.  */

static void
matched_an_arithmetic_wrap_around (unsigned loop_nb,
				   tree var,
				   tree opnd1, 
				   tree *evolution_function, 
				   tree *effect_after_execution)
{
  /* FIXME arithmetic flip-flop.  */
  tree opnd10, opnd11;
  tree chrec10, chrec11;
  tree chrec_before = get_scev (0, var);
  if (chrec_should_remain_symbolic (chrec_before))
    /* KEEP_IT_SYMBOLIC.  */
    chrec_before = opnd1;
  
  opnd10 = TREE_OPERAND (opnd1, 0);
  opnd11 = TREE_OPERAND (opnd1, 1);
  
  chrec10 = evolution_at_version (loop_nb, opnd10);
  chrec11 = evolution_at_version (loop_nb, opnd11);
  
  *effect_after_execution = chrec_fold_minus (chrec10, chrec11);
  *evolution_function = reset_evolution_in_loop 
    (loop_nb, chrec_before, chrec_top);
}

/* Given a loop phi-node, determines whether its evolution has already
   been analyzed.  */

static inline bool
evolution_of_phi_already_analyzed_p (tree loop_phi)
{
  /* Another way to check this property would be: "if all the edges
     that point to definitions contained in the loop have been
     analyzed, then the loop phi-node has already been analyzed".  */
  
  return (evolution_part_in_loop_num 
	  (get_scev (0, SSA_NAME_VAR (PHI_RESULT (loop_phi))), 
	   loop_num (loop_of_stmt (loop_phi)))
	  == NULL_TREE);
}

/* Analyze VERSION and keep the reconstructed tree under a symbolic
   form.  */

static tree
symbolically_analyze (tree version)
{
  tree def = SSA_NAME_DEF_STMT (version);
  
  switch (TREE_CODE (def))
    {
    case MODIFY_EXPR:
      return TREE_OPERAND (def, 1);
      
    default:
      return version;
    }
}

/* Helper function for analyzing a modify expression "OPND0 = OPND1"
   in the context of loop LOOP_NB.  The EVOLUTION_FUNCTION is the new
   evolution function after having analyzed the statement, and the
   EFFECT_AFTER_EXECUTION is the initial condition of OPND1.  */

static void
scev_analyze_modify_expr (unsigned loop_nb,
			  tree opnd0, 
			  tree opnd1, 
			  tree halting_phi, 
			  tree *evolution_function, 
			  tree *effect_after_execution)
{
  tree var;
  
  DBG_S (fprintf (stderr, "(scev_analyze_modify_expr \n");
	 fprintf (stderr, "  (opnd0 = \n");
	 debug_generic_expr (opnd0);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, "  (opnd1 = \n");
	 debug_generic_expr (opnd1);
	 fprintf (stderr, "  )\n"));
  
  var = SSA_NAME_VAR (opnd0);
	
  /* The OPND1 is one of the following cases:
     - an SSA_NAME, 
     - an INTEGER_CST,
     - a PLUS_EXPR, 
     - a MINUS_EXPR,
     - other cases are not yet handled. 
  */
  switch (TREE_CODE (opnd1))
    {
    case INTEGER_CST:
      {
	/* FIXME wrap_around.
	   This assignment is under the form "a_1 = 7".  */
	tree chrec_before = get_scev (0, var);
	if (chrec_should_remain_symbolic (chrec_before))
	  /* KEEP_IT_SYMBOLIC.  */
	  chrec_before = opnd1;
	
	*effect_after_execution = evolution_at_version (loop_nb, opnd1);
	*evolution_function = reset_evolution_in_loop (loop_nb, chrec_before, chrec_top);
	break;
      }
      
    case SSA_NAME:
      if (var == SSA_NAME_VAR (opnd1))
	{
	  /* FIXME does this happen?
	     This is a strange case: "a_1 = a_2".  */
	  abort ();
	}
      
      /* This assignment is under the form: "a_1 = b_2".  */
      else
	{
	  tree init_cond, chrec1;
	  tree chrec_before;
	  
	  chrec_before = get_scev (0, var);
	  if (chrec_should_remain_symbolic (chrec_before))
	    /* KEEP_IT_SYMBOLIC.  */
	    chrec_before = opnd1;
	  
	  chrec1 = evolution_at_version (loop_nb, opnd1);
	  init_cond = initial_condition (chrec1);
	  
 	  /* When the evolution of opnd1 is not yet known, we have to
 	     analyze it before going further.  */
 	  if (chrec1 == opnd1)
 	    {
 	      /* testsuite/.../ssa-chrec-53.c.  */
 	      init_cond = symbolically_analyze (opnd1);
 	      chrec1 = replace_initial_condition (chrec_before, init_cond);
 	      
 	      if (expression_contains_variable_p 
 		  (init_cond, SSA_NAME_VAR (opnd0)))
 		analyze_non_gimple_initial_condition
 		  (loop_nb, opnd0, chrec_before, chrec1, init_cond, 
 		   evolution_function, effect_after_execution);
	      else
		{
		  /* FIXME wrap_around.  */
		  *effect_after_execution = chrec1;
		  *evolution_function = reset_evolution_in_loop 
		    (loop_nb, chrec_before, chrec_top);
		}
 	    }
 	  
 	  /* If VAR occurs in the initial condition of opnd1 then the
 	     analyzed modify expression is not a wrap-around: it is
 	     just an expression that has been gimplified using
 	     temporary variables.  */
 	  else if (expression_contains_variable_p 
 		   (init_cond, SSA_NAME_VAR (opnd0)))
  	    analyze_non_gimple_initial_condition 
 	      (loop_nb, opnd0, chrec_before, chrec1, init_cond,
  	       evolution_function, effect_after_execution);
  	  else
  	    {
  	      /* FIXME wrap_around.  */
	      *effect_after_execution = chrec1;
	      *evolution_function = reset_evolution_in_loop 
		(loop_nb, chrec_before, chrec_top);
	    }
	}
      break;
      
    case PLUS_EXPR:
      {
	/* This case is under the form "opnd0 = opnd10 + opnd11".  */
	tree opnd10, opnd11;
	      
	opnd10 = TREE_OPERAND (opnd1, 0);
	opnd11 = TREE_OPERAND (opnd1, 1);
	      	      
	if (TREE_CODE (opnd10) == SSA_NAME)
	  {

	    /* Match an assignment under the form: 
	       "a_1 = a_2 + ...".  */
	    if (SSA_NAME_VAR (opnd10) == var)
	      matched_an_increment 
		(loop_nb, var, opnd10, opnd11, halting_phi, 
		 evolution_function, effect_after_execution);
	      
	    /* Otherwise the assignment is under the form: 
	       "a_1 = b_2 + ...".  */
	    else
	      {

		/* Match an assignment under the form: 
		   "a_1 = b_2 + a_3".  */
		if (TREE_CODE (opnd11) == SSA_NAME
		    && SSA_NAME_VAR (opnd11) == var)
		  matched_an_increment
		    (loop_nb, var, opnd11, opnd10, halting_phi, 
		     evolution_function, effect_after_execution);
		
		/* Match an assignment under the form: 
		   "a_1 = b_2 + c_3".  */
		else
		  matched_a_wrap_around 
		    (PLUS_EXPR, loop_nb, var, opnd0, opnd1, 
		     evolution_function, effect_after_execution);
	      }
	  }
	
	else if (TREE_CODE (opnd11) == SSA_NAME)
	  {
	    if (SSA_NAME_VAR (opnd11) == var)
	      {
		/* FIXME does this happen? 
		   The assignment is under the form: 
		   "a_1 = 5 + a_2".  
		   From what I've seen, this case is turned into:
		   "a_1 = a_2 + 5".  */
		abort ();
	      }
	    
	    /* The assignment is under the form: "a_1 = 5 + b_2.  */
	    else
	      matched_a_wrap_around 
		(PLUS_EXPR, loop_nb, var, opnd0, opnd1, 
		 evolution_function, effect_after_execution);
	  }
	
	else
	  {
	    /* The arguments do not contain SSA_NAMEs.  For
	       example, this could be the computation of a
	       memory address as in:
		     
	       java/jcf-io.c: "buf_ptr_7 = &buf + 68B".  */
	    *effect_after_execution = opnd1;
	    *evolution_function = opnd1;
	  }
	      
	break;
      }
	    
    case MINUS_EXPR:
      {
	/* This case is under the form "opnd0 = opnd10 - opnd11".  */
	tree opnd10, opnd11;
	      
	opnd10 = TREE_OPERAND (opnd1, 0);
	opnd11 = TREE_OPERAND (opnd1, 1);
	      
	if (TREE_CODE (opnd10) == SSA_NAME)
	  {

	    /* Match an assignment under the form: 
	       "a_1 = a_2 - ...".  */
	    if (SSA_NAME_VAR (opnd10) == var)
	      {
		tree upper_chain = SSA_NAME_DEF_STMT (opnd10);
		tree chrec10, chrec11, chrec_before;
		tree to_be_added;
		      
		/* Recursively construct the SSA path.  */
		scev_follow_ssa_edge (upper_chain, halting_phi);
		
		/* Analyze the assignment on the return walk.  */
		chrec_before = get_scev (0, var);
		if (chrec_should_remain_symbolic (chrec_before))
		  /* KEEP_IT_SYMBOLIC.  */
		  chrec_before = opnd10;
		      
		chrec10 = evolution_at_version (loop_nb, opnd10);
		chrec11 = evolution_at_version (loop_nb, opnd11);
		if (chrec_should_remain_symbolic (chrec11))
		  /* KEEP_IT_SYMBOLIC.  */
		  to_be_added = chrec_fold_multiply (opnd11,
						     integer_minus_one_node);
		else
		  to_be_added = chrec_fold_multiply (chrec11,
						     integer_minus_one_node);
		      
		*effect_after_execution = chrec_fold_plus 
		  (initial_condition (chrec10), 
		   initial_condition (to_be_added));
		
		to_be_added = select_outer_and_current_evolutions 
		  (loop_nb, to_be_added);
		*evolution_function = add_to_evolution 
		  (loop_nb, chrec_before, to_be_added);
	      }
		  
	    /* Otherwise the assignment is under the form: 
	       "a_1 = b_2 - ...".  */
	    else
	      {
		
		/* Match an assignment under the form: 
		   "a_1 = b_2 - a_3".  */
		if (TREE_CODE (opnd11) == SSA_NAME
		    && SSA_NAME_VAR (opnd11) == var)
		  matched_an_arithmetic_wrap_around 
		    (loop_nb, var, opnd1, evolution_function, 
		     effect_after_execution);
		
		/* Otherwise the assignment is under the form: 
		   "a_1 = b_2 - c_3".  */
		else
		  matched_a_wrap_around 
		    (MINUS_EXPR, loop_nb, var, opnd0, opnd1, 
		     evolution_function, effect_after_execution);
	      }
	  }
	      
	else if (TREE_CODE (opnd11) == SSA_NAME)
	  {

	    /* Match an assignment under the form: 
	       "a_1 = 5 - a_2".  */
	    if (SSA_NAME_VAR (opnd11) == var)
	      matched_an_arithmetic_wrap_around 
		(loop_nb, var, opnd1, evolution_function, 
		 effect_after_execution);
	    
	    /* Otherwise the assignment is under the form: 
	       "a_1 = 5 - b_2.  */
	    else
	      matched_a_wrap_around
		(MINUS_EXPR, loop_nb, var, opnd0, opnd1, 
		 evolution_function, effect_after_execution);
	  }
	
	else
	  {
	    /* The arguments do not contain SSA_NAMEs.  */
	    *effect_after_execution = opnd1;
	    *evolution_function = opnd1;
	  }
	break;
      }
	    
    case MULT_EXPR:
      {
	/* This case is under the form "opnd0 = opnd10 * opnd11".  */
	tree opnd10, opnd11;
	      
	opnd10 = TREE_OPERAND (opnd1, 0);
	opnd11 = TREE_OPERAND (opnd1, 1);
	      	      
	if (TREE_CODE (opnd10) == SSA_NAME)
	  {
	    
	    /* Match an assignment under the form: 
	       "a_1 = a_2 * ...".  */
	    if (SSA_NAME_VAR (opnd10) == var)
	      matched_an_exponentiation 
		(loop_nb, var, opnd10, opnd11, halting_phi, 
		 evolution_function, effect_after_execution);
	    
	    /* Otherwise the assignment is under the form: 
	       "a_1 = b_2 * ...".  */
	    else
	      {
		/* Match an assignment under the form: 
		   "a_1 = b_2 * a_3".  */
		if (TREE_CODE (opnd11) == SSA_NAME
		    && SSA_NAME_VAR (opnd11) == var)
		  matched_an_exponentiation 
		    (loop_nb, var, opnd11, opnd10, halting_phi, 
		     evolution_function, effect_after_execution);
		
		/* Otherwise the assignment is under the form: 
		   "a_1 = b_2 * c_3".  */
		else
		  matched_a_wrap_around
		    (MULT_EXPR, loop_nb, var, opnd0, opnd1, 
		     evolution_function, effect_after_execution);
	      }
	  }
	      
	else if (TREE_CODE (opnd11) == SSA_NAME)
	  {

	    /* Match an assignment under the form: 
	       "a_1 = 5 * a_2".  */
	    if (SSA_NAME_VAR (opnd11) == var)
	      {
		/* FIXME does this happen? 
		   From what I've seen, this case is turned into:
		   "a_1 = a_2 * 5".  */
		abort ();
	      }
	    
	    /* Otherwise the assignment is under the form: 
	       "a_1 = 5 * b_2.  */
	    else
	      matched_a_wrap_around 
		(MULT_EXPR, loop_nb, var, opnd0, opnd1, 
		 evolution_function, effect_after_execution);
	  }
	
	else
	  {
	    /* FIXME does this happen? 
	       This is a strange case since the mult is not
	       folded: "a_1 = cst * cst".  */
	    abort ();
	  }
	break;
      }
	    
    default:
      {
	tree chrec_before = get_scev (0, var);
	if (chrec_should_remain_symbolic (chrec_before))
	  /* KEEP_IT_SYMBOLIC.  */
	  chrec_before = opnd1;
	
	*effect_after_execution = chrec_top;
	*evolution_function = reset_evolution_in_loop 
	  (loop_nb, chrec_before, chrec_top);
	break;
      }
    }
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* Helper function.  */

static void
scev_follow_ssa_edge (tree rdef, 
		      tree halting_phi)
{
  if (rdef == NULL_TREE 
      || TREE_CODE (rdef) == NOP_EXPR
      /* End the recursion when the halting_phi node is reached.  */
      || rdef == halting_phi)
    return;
  
  /* Dispatch the call to the right function handler.  */
  if (loop_of_stmt (rdef) == loop_of_stmt (halting_phi))
    scev_follow_ssa_edge_same_loop (rdef, halting_phi);
  
  else 
    scev_follow_ssa_edge_inner_loop (rdef, halting_phi);
}

/* This recursive function follows an SSA edge from a loop phi node to
   itself, constructing a path that is analyzed on the return walk.
   This function is called for each version of a variable in the
   current analyzed loop.  The number of calls to this function is
   equal to the number of versions of the variable in the loop of the
   halting_phi, excluding all the versions defined in inner loops.  */

static void
scev_follow_ssa_edge_same_loop (tree rdef, 
				tree halting_phi)
{
  unsigned rdef_num, rdef_depth;
  unsigned halting_num, halting_depth;
  
  rdef_num = loop_num (loop_of_stmt (rdef));
  rdef_depth = loop_depth (loop_of_stmt (rdef));
  halting_num = loop_num (loop_of_stmt (halting_phi));
  halting_depth = loop_depth (loop_of_stmt (halting_phi));
  
  /* We have to deal with only three cases in an update expression: 
     - an inner loop PHI_NODE, 
     - a conditional PHI_NODE, 
     - a MODIFY_EXPR.  */
  switch (TREE_CODE (rdef))
    {
    case PHI_NODE:
      /* RDEF is a CONDITION-phi-node.
	 
	 Follow the branches, and record their evolutions.  Finally,
	 merge the collected information and set the approximation to
	 the main variable.
	 
	 FIXME: It is possible to improve the analysis speed by not
	 following the whole chain to the loop-phi-node.  A better
	 solution is to analyze the evolution only on the portions of
	 paths that differ, ie. do not analyze n times outside the
	 if-body.  */
      merge_branches_of_condition_phi_node_in_loop (rdef, halting_phi);
      break;
      
    case MODIFY_EXPR:
      {
	tree evolution_function, effect_after_execution;
	tree opnd0, opnd1;
	
	opnd0 = TREE_OPERAND (rdef, 0);
	opnd1 = TREE_OPERAND (rdef, 1);
	scev_analyze_modify_expr (rdef_num, 
				  opnd0, 
				  opnd1, 
				  halting_phi, 
				  &evolution_function, 
				  &effect_after_execution);

	set_scev (0, SSA_NAME_VAR (opnd0), evolution_function);
	set_scev (rdef_num, opnd0, effect_after_execution);
	break;
      }
      
    default:
      /* At this level of abstraction, the program is just a set of
	 MODIFY_EXPRs and PHI_NODEs.  In principle there is no other
	 node to be handled.  */
      abort ();
      break;
    }
}

/* Given an inner loop phi-node, follow the upper chain for
   determining the initial conditions from the analysis of the outer
   loop.  Then analyze the evolution in the inner loop.  */

static void
scev_analyze_inner_loop_phi (tree inner_loop_phi, 
			     tree halting_phi)
{
  int i;
  
  if (inner_loop_phi == NULL_TREE)
    return;
  
  for (i = 0; i < PHI_NUM_ARGS (inner_loop_phi); i++)
    {
      tree arg = PHI_ARG_DEF (inner_loop_phi, i);
      tree upper_branch;
      
      if (TREE_CODE (arg) == INTEGER_CST)
	{
	  set_scev (loop_num (loop_of_stmt (inner_loop_phi)), 
		    PHI_RESULT (inner_loop_phi), arg);
	  break;
	}
      
      upper_branch = SSA_NAME_DEF_STMT (arg);
      if (TREE_CODE (upper_branch) == NOP_EXPR)
	continue;
      
      if (loop_is_strictly_included_in 
	  (loop_num (loop_of_stmt (inner_loop_phi)),
	   loop_num (loop_of_stmt (upper_branch))))
	{
	  tree res;
	  
	  switch (TREE_CODE (upper_branch))
	    {
	    case MODIFY_EXPR:
	      scev_follow_ssa_edge (upper_branch, halting_phi);
	      res = evolution_at_version 
		(loop_num (loop_of_stmt (upper_branch)), 
		 TREE_OPERAND (upper_branch, 0));
	      break;
	      
	    case PHI_NODE:
	      scev_follow_ssa_edge (upper_branch, halting_phi);
	      res = evolution_at_version 
		(loop_num (loop_of_stmt (upper_branch)), 
		 PHI_RESULT (upper_branch));
	      break;
	      
	    default:
	      debug_tree (inner_loop_phi);
	      res = chrec_top;
	    }
	  
	  set_scev (loop_num (loop_of_stmt (inner_loop_phi)),
		    PHI_RESULT (inner_loop_phi), 
		    initial_condition (res));
	}
    }
	  
  /* Avoid the analysis of the inner loop when it has already
     been analyzed: testsuite/.../ssa-chrec-01.c  */
  if (evolution_of_phi_already_analyzed_p (inner_loop_phi))
    analyze_evolution_in_loop (inner_loop_phi);
}

/* This recursive function follows an SSA edge from a loop phi node to
   itself, constructing a path that is analyzed on the return
   walk.  */

static void
scev_follow_ssa_edge_inner_loop (tree rdef, 
				 tree halting_phi)
{
  /* We have to deal with only three cases in an update expression: 
     - an inner loop PHI_NODE, 
     - a conditional PHI_NODE, 
     - a MODIFY_EXPR.  */
  switch (TREE_CODE (rdef))
    {
    case PHI_NODE:
      /* This is a inner-loop-phi-node.  */
      scev_analyze_inner_loop_phi (rdef, halting_phi);
      
      /* After having determined the evolution in the inner loop,
	 the analyzer computes the overall effect of the inner
	 loop on the analyzed variable.
	 
	 Example:  
	 
	 | loop 10 times
	 |   i = i + 2
	 | endloop
	 
	 This loop has the same effect as:
	 
	 | i = i + 20
      */
      compute_overall_effect_of_inner_loop 
	(PHI_RESULT (rdef), 
	 loop_num (loop_of_stmt (halting_phi)));
      break;
            
    case MODIFY_EXPR:
      {
	/* This is an inner loop modify-expression.  The first step
	   finds the inner loop phi-node and follows its initial
	   condition branch.  Then the evolution of the inner loop is
	   computed, and finally the overall effect of this inner loop
	   on the analyzed variable is computed.  */
	tree loop_phi_node = get_loop_phi_node_for_variable 
	  (loop_of_stmt (rdef), SSA_NAME_VAR (TREE_OPERAND (rdef, 0)));
	
	if (loop_phi_node != NULL_TREE)
	  {
	    scev_analyze_inner_loop_phi (loop_phi_node, halting_phi);
	    compute_overall_effect_of_inner_loop 
	      (TREE_OPERAND (rdef, 0), 
	       loop_num (loop_of_stmt (halting_phi)));
	  }
	
	else
	  {
	    /* This modify expression is in a loop, but does not have
	       a loop phi node.  It's a quite strange case, don't know
	       if this case occurs in the SSA IR, but if it happens, I
	       think that this have to be solved in the same time as
	       the FIXME wrap_around.  */
	    tree var = SSA_NAME_VAR (TREE_OPERAND (rdef, 0));
	    set_scev (0, var, 
		      reset_evolution_in_loop 
		      (loop_num (loop_of_stmt (halting_phi)), 
		       get_scev (0, var), chrec_top));
	  }
      	break;
      }
      
    default:
      /* At this level of abstraction, the program is just a set of
	 MODIFY_EXPRs and PHI_NODEs.  In principle there is no other
	 node to be handled.  */
      abort ();
      break;
    }
}

/* Compute the overall effect of a loop on a variable. 
   1. (TODO) compute the evolution if it is not yet analyzed in this loop, 
   2. compute the number of iterations in the loop,
   3. compute the value of the variable after crossing the loop.  */

static tree 
compute_overall_effect_of_inner_loop (tree version, 
				      unsigned halting_num)
{
  tree exit_value;
  tree nb_iter;
  struct loop *loop = loop_of_stmt (SSA_NAME_DEF_STMT (version));
  unsigned loop_nb = loop_num (loop);
  tree var = SSA_NAME_VAR (version);
  
  DBG_S (fprintf (stderr, "(compute_overall_effect_of_inner_loop \n"));
  
  nb_iter = number_of_iterations_in_loop (loop);
  if (nb_iter == chrec_top)
    {
      /* If the number of iterations is not known, set the
	 evolution to chrec_top.  As an example, consider
	 the following loop:
		     
	 | i = 5
	 | loop 
	 |   i = i + 1
	 |   loop chrec_top times
	 |     i = i + 3
	 |   endloop
	 | endloop
	 
	 since it is impossible to know the number of
	 iterations in the inner loop, the evolution of i
	 in the outer loop becomes unknown.
	 
	 FIXME: It is possible to say that the evolution
	 of i in the outer loop is:
	 
	 {5, +, [1, +oo]}_1 
	 
	 instead of the current rough approximation:
	 
	 {5, +, [-oo, +oo]}_1.
      */
      if (halting_num != 0)
	set_scev (0, var, 
		  reset_evolution_in_loop (halting_num, 
					   get_scev (0, var),
					   chrec_top));
      
      exit_value = chrec_top;
    }
  
  else
    {
      tree overall_effect;
      tree new_evol;
      tree evolution_fn;
      tree loop_phi_node;
      
      /* An example: given the evolution 
	 "{{22, +, 4}_1, +, [1,3]}_2", 
	 
	 and the fact that the loop 2 runs exactly 6 times, the
	 overall effect is obtained by evaluating:
	 
	 "({{22, +, 4}_1, +, [1,3]}_2 - initial_conditions) (2, 6)"
	 "({{22, +, 4}_1, +, [1,3]}_2 - 22) (2, 6)"
	 "{{0, +, 4}_1, +, [1,3]}_2 (2, 6)"
	 "[6,18]".
         
         That means that the effect of the inner loop 2 in the outer
	 loop 1 is that of adding an integer between 6 and 18 every
	 time the loop 1 is executed.  Consequently, the evolution
	 function is: "{{22, +, [10,22]}_1, +, [1,3]}_2".
      */
      
      evolution_fn = get_scev (0, var);
      
      overall_effect = chrec_apply
	(loop_nb, update_initial_condition_to_origin
	 (evolution_function_in_loop_num 
	  (evolution_fn, loop_nb)),
	 nb_iter);
      
      /* On exit of the loop, the value of the variable is equal to
	 the sum of its initial condition when entering the loop
	 (ie. the initial condition of the loop phi-node), with the
	 loop's overall effect.  */
      loop_phi_node = get_loop_phi_node_for_variable 
	(loop_of_stmt (SSA_NAME_DEF_STMT (version)), 
	 SSA_NAME_VAR (version));
      
      if (loop_phi_node == NULL_TREE)
	exit_value = chrec_top;
      
      else
	exit_value = chrec_fold_plus 
	  (get_scev (loop_nb, PHI_RESULT (loop_phi_node)), overall_effect);
      
      /* A VERSION has two values: 
	 
         - one exposed to the statements in the inner loop,
	 
	 - the other that is seen by the statements after the loop.
	 
	 This updates the exit value of the loop's phi node.  */
      set_scev_outer_value (version, exit_value);
      
      /* We're not interested in the evolution in the outermost scope:
	 loop 0 represents the analyzed function.  Maybe in an
	 interprocedural analysis this information could be useful,
	 but for interprocedural analysis we would need another type
	 of loop indexing.  */
      if (halting_num != 0)
	{
	  unsigned out_loop_nb = loop_num (outer_loop (loop));
	  
	  if (TREE_CODE (overall_effect) == EXPONENTIAL_CHREC)
	    /* testsuite/.../ssa-chrec-41.c.  */
	    new_evol = multiply_evolution (out_loop_nb, 
					   get_scev (loop_nb, var), 
					   overall_effect);
	  
	  else 
	    {
	      overall_effect = select_outer_and_current_evolutions 
		(out_loop_nb, overall_effect);
	      
	      new_evol = add_to_evolution (out_loop_nb,
					   get_scev (loop_nb, var), 
					   overall_effect);
	    }
	  
	  set_scev (0, var, new_evol);
	}
    }
  
  DBG_S (fprintf (stderr, ")\n"));
  return exit_value;
}

/* Determines whether the PHI_NODE has SSA_NAMEs in its arguments.  */

static inline bool
no_ssa_name_in_arguments_of_phi_node (tree phi_node)
{
  int i;
  
  for (i = 0; i < PHI_NUM_ARGS (phi_node); i++)
    {
      tree branch = PHI_ARG_DEF (phi_node, i);
      
      if (TREE_CODE (branch) == SSA_NAME)
	return false;
    }
  
  return true;
}

/* Retrieve the loop phi node for variable v.  */

static tree 
get_loop_phi_node_for_variable (struct loop *loop, 
				tree v)
{
  tree phi;
  
  for (phi = phi_nodes (loop_header (loop)); phi; phi = TREE_CHAIN (phi))
    if (SSA_NAME_VAR (PHI_RESULT (phi)) == v)
      {
	/* In testsuite/.../ssa-chrec-48.c the loop phi node of the
	   outermost loop is: 
	   
	   " # j_2 = PHI <10(0), 0(3)>;"
	   
	   This is the result of the Value Range Propagation pass.  In
	   order to analyze the evolution in the inner loops, we have
	   to discard such phi nodes.  */
	if (no_ssa_name_in_arguments_of_phi_node (phi))
	  continue;
	else
	  return phi;
      }
  
  return NULL_TREE;
}



/* The following section constitutes the interface with the chrecs.  */

/* Get the index corresponding to VAR in the current loop nest.  If
   it's the first time we ask for this VAR, then conservatively insert
   CHREC_TOP for this VAR and return its index.  */

static struct scev_info_str *
find_var_scev_info (tree var)
{
  unsigned int i;
  struct scev_info_str *res;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (scev_info); i++)
    {
      res = VARRAY_GENERIC_PTR (scev_info, i);
      if (MI_VAR (res) == var)
	return res;
    }
  
  /* The variable is not in the table, create a new entry for it.  */
  res = new_scev_info_str (var);
  VARRAY_PUSH_GENERIC_PTR (scev_info, res);
  
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
      /* chrec = chrec_symbolic_parameter; */
      chrec = var;
      break;
      
    case SSA_NAME:
      if (var == chrec)
	chrec = chrec_symbolic_parameter;
      break;
      
    default:
      break;
    }
  
  return chrec;
}

/* Associate CHREC to VAR.  */

static void
set_scev (unsigned loop_nb, 
	  tree var, 
	  tree chrec)
{
  struct scev_info_str *var_info;
  
  var_info = find_var_scev_info (var);
  chrec = set_scev_keep_symbolic (var, chrec);
  
  DBG_S (fprintf (stderr, "(set_scev \n");
	 fprintf (stderr, "  (loop_num = %d)\n", loop_nb);
	 fprintf (stderr, "  (sc = ");
	 debug_generic_expr (var);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, "  (scev = ");
	 debug_generic_expr (chrec);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, ")\n"));
  
  MI_LOOP_NUM (var_info) = loop_nb;
  MI_INNER_LOOPS_CHREC (var_info) = chrec;
}

/* Associate the value CHREC exposed to the statements in the inner
   loops to the VERSION.  */

static void
set_scev_inner_value (tree version, 
		      tree chrec)
{
  struct scev_info_str *version_info;
  unsigned loop_nb = loop_num (loop_of_stmt (SSA_NAME_DEF_STMT (version)));
   
  version_info = find_var_scev_info (version);
  chrec = set_scev_keep_symbolic (version, chrec);
  
  DBG_S (fprintf (stderr, "(set_scev_inner_value \n");
	 fprintf (stderr, "  (loop_num = %d)\n", loop_nb);
	 fprintf (stderr, "  (sc = ");
	 debug_generic_expr (version);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, "  (scev = ");
	 debug_generic_expr (chrec);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, ")\n"));
  
  MI_LOOP_NUM (version_info) = loop_nb;
  MI_INNER_LOOPS_CHREC (version_info) = chrec;
}

/* Associate the value CHREC, that is exposed to the statements after
   the loop, to the SSA_VERSION.  */

static void
set_scev_outer_value (tree version, 
		      tree chrec)
{
  struct scev_info_str *version_info;
  
  version_info = find_var_scev_info (version);
  chrec = set_scev_keep_symbolic (version, chrec);
  
  DBG_S (fprintf (stderr, "(set_scev_outer_value \n");
	 fprintf (stderr, "  (loop_num = %d)\n", 
		  loop_num (loop_of_stmt (SSA_NAME_DEF_STMT (version))));
	 fprintf (stderr, "  (sc = ");
	 debug_generic_expr (version);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, "  (scev = ");
	 debug_generic_expr (chrec);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, ")\n"));
  
  MI_OUTER_LOOPS_CHREC (version_info) = chrec;
}

/* Retrieve the chrec associated to VAR in the loop LOOP_NB.  */

static tree
get_scev (unsigned loop_nb, 
	  tree var)
{
  struct scev_info_str *var_info;
  tree res = NULL_TREE;
  
  DBG_S (fprintf (stderr, "(get_scev \n");
	 fprintf (stderr, "  (loop_nb = %d)\n", loop_nb));
  
  switch (TREE_CODE (var))
    {
    case REAL_CST:
    case INTEGER_CST:
      res = var;
      break;
      
    case VAR_DECL:
    case PARM_DECL:
      var_info = find_var_scev_info (var);
      res = MI_INNER_LOOPS_CHREC (var_info);
      break;
      
    case SSA_NAME:
      var_info = find_var_scev_info (var);
      
      if (loop_is_included_in (loop_nb, MI_LOOP_NUM (var_info)))
	res = MI_INNER_LOOPS_CHREC (var_info);
      
      else
	{
	  res = MI_OUTER_LOOPS_CHREC (var_info);
	  
	  if (res == chrec_not_analyzed_yet)
	    res = compute_overall_effect_of_inner_loop (var, 0);
	}
      
      break;
      
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
  
  DBG_S (fprintf (stderr, "  (sc = ");
	 debug_generic_expr (var);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, "  (scev = ");
	 debug_generic_expr (res);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, ")\n"));
  return res;
}

/* Merge the evolution functions from BRANCH_CHRECS with the
   ORIGINAL_CHREC.  */

static tree 
merge_evolutions (tree original_chrec, 
		  varray_type branch_chrecs)
{
  unsigned i;
  tree res = chrec_top;
  unsigned nb_branches = VARRAY_ACTIVE_SIZE (branch_chrecs);
  
  DBG_S (fprintf (stderr, "(merge_evolutions \n"));
  if (nb_branches > 0)
    {
      if (original_chrec == chrec_not_analyzed_yet)
	original_chrec = integer_zero_node;
      
      /* In the case where there is a single branch, there is no need
	 to merge evolution functions.  */
      if (nb_branches == 1) 
	res = chrec_merge (original_chrec, VARRAY_TREE (branch_chrecs, 0));
      
      /* Otherwise merge the different branches.  */
      else
	{
	  varray_type diff_chrecs;
	  VARRAY_TREE_INIT (diff_chrecs, 2, "diff_chrecs");
	  
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (branch_chrecs); i++)
	    {
	      tree diff_chrec;
	      tree branch_chrec = VARRAY_TREE (branch_chrecs, i);
	      
	      diff_chrec = chrec_fold_minus (branch_chrec, original_chrec);
	      DBG_S (fprintf (stderr, "  (branch = ");
		     debug_generic_expr (diff_chrec);
		     fprintf (stderr, "  )\n"));
	      VARRAY_PUSH_TREE (diff_chrecs, diff_chrec);
	    }
	  
	  res = VARRAY_TREE (diff_chrecs, 0);
	  if (res == NULL_TREE)
	    res = chrec_top;
	  
	  for (i = 1; i < VARRAY_ACTIVE_SIZE (diff_chrecs); i++)
	    res = chrec_merge (res, VARRAY_TREE (diff_chrecs, i));
	  
	  res = chrec_fold_plus (original_chrec, res);
	  varray_clear (diff_chrecs);
  	}
    }
  
  else
    res = original_chrec;
  
  DBG_S (fprintf (stderr, "  (evolution_after_merge = ");
	 debug_generic_expr (res);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, ")\n"));
  
  return res;
}

/* This function merges the branches of a condition phi node in a
   loop.  */

static void
merge_branches_of_condition_phi_node_in_loop (tree condition_phi, 
					      tree halting_phi)
{
  int i;
  tree original_chrec, res;
  varray_type branch_chrecs;

  if (PHI_MARKED (condition_phi) == 1)
    return;
  
  else
    PHI_MARKED (condition_phi) = 1;
  
  VARRAY_TREE_INIT (branch_chrecs, 2, "branch_chrecs");
  original_chrec = get_scev (0, SSA_NAME_VAR (PHI_RESULT (condition_phi)));
  
  for (i = 0; i < PHI_NUM_ARGS (condition_phi); i++)
    {
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
	  res = chrec_top;
	  goto register_res;
	  
	case SSA_NAME:
	  scev_follow_ssa_edge (SSA_NAME_DEF_STMT (branch), 
				halting_phi);
	  VARRAY_PUSH_TREE (branch_chrecs, get_scev 
			    (0, SSA_NAME_VAR (PHI_RESULT (condition_phi))));
	  set_scev (0, SSA_NAME_VAR (PHI_RESULT (condition_phi)), 
		    original_chrec);
	  break;
	  
	default:
	  VARRAY_PUSH_TREE (branch_chrecs, chrec_top);
	  break;
	}
    }
  
  res = merge_evolutions (original_chrec, branch_chrecs);
  
 register_res:;
  set_scev (0, SSA_NAME_VAR (PHI_RESULT (condition_phi)), res);
  set_scev (loop_num (loop_of_stmt (condition_phi)),
	    PHI_RESULT (condition_phi), 
	    chrec_eval_next_init_cond (loop_num (loop_of_stmt (condition_phi)), 
				       res));
  
  varray_clear (branch_chrecs);
  PHI_MARKED (condition_phi) = 0;
}

/* This function merges the branches of a condition phi node,
   contained in the outermost loop, and whose arguments are already
   analyzed.  */

static void
merge_branches_of_condition_phi_node (tree condition_phi)
{
  int i;
  tree res, var, chrec;
  varray_type branch_chrecs;
  int loop_nb = loop_num (loop_of_stmt (condition_phi));
  
  VARRAY_TREE_INIT (branch_chrecs, 2, "branch_chrecs");
  
  for (i = 1; i < PHI_NUM_ARGS (condition_phi); i++)
    {
      chrec = get_scev (loop_nb, PHI_ARG_DEF (condition_phi, i));
      if (chrec != chrec_not_analyzed_yet)
	VARRAY_PUSH_TREE (branch_chrecs, chrec);
      else
	VARRAY_PUSH_TREE (branch_chrecs, PHI_ARG_DEF (condition_phi, i));
    }
  
  chrec = get_scev (loop_nb, PHI_ARG_DEF (condition_phi, 0));
  if (chrec != chrec_not_analyzed_yet)
    res = merge_evolutions (chrec, branch_chrecs);
  else
    res = merge_evolutions (PHI_ARG_DEF (condition_phi, 0), branch_chrecs);
  
  var = SSA_NAME_VAR (PHI_RESULT (condition_phi));
  set_scev (0, var, replace_initial_condition (get_scev (0, var), res));
  set_scev (loop_nb, PHI_RESULT (condition_phi), 
	    chrec_eval_next_init_cond (loop_nb, res));
  
  varray_clear (branch_chrecs);
}

/* Merge the information of the evolution of a variable with the
   initial condition at the given point/version.  The initial
   condition is an interval, while the evolution of the variable is
   the evolution at the origin.  
   
   When the evolution of a variable is not analyzed yet, this function
   returns the version itself.  */

static tree 
evolution_at_version (unsigned loop_nb, 
		      tree version)
{
  struct scev_info_str *var_info;
  tree init_cond, evolution, res;
  
  DBG_S (fprintf (stderr, "(evolution_at_version \n"));
  
  switch (TREE_CODE (version))
    {
    case PLUS_EXPR:
    case MINUS_EXPR:
    case MULT_EXPR:
    case ADDR_EXPR:
    case INTEGER_CST:
    case REAL_CST:
    case VAR_DECL:
    case PARM_DECL:
      res = version;
      break;
      
    case SSA_NAME:
      var_info = find_var_scev_info (version);
      if (1 || loop_is_included_in (loop_nb, MI_LOOP_NUM (var_info)))
	{
	  /* Retrieve the initial condition from the version.  */
	  init_cond = get_scev (loop_nb, version);
	  
	  /* Instead of propagating chrec_top elements incognito, it
	     is more precise to keep a pointer to the version (from
	     which it is still possible to get_scev and say that it is
	     chrec_top).  */
	  if (init_cond == chrec_top
	      || init_cond == chrec_not_analyzed_yet)
	    init_cond = version;
	  
	  /* The evolution in a loop is stored as an attribute of the
	     main variable.  */
	  evolution = get_scev (0, SSA_NAME_VAR (version));
	  
	  /* When the main variable has an unknown evolution, and the
	     current version has not been analyzed, the result is
	     unknown.  */
	  if (evolution == chrec_top 
	      && init_cond == version)
	    res = chrec_top;
	  
	  /* The evolution function for a version is equal to the
	     evolution function in the loop instantiated with the
	     initial condition of the version.  */
	  else
	    res = replace_initial_condition (evolution, init_cond);
	}
      
      /* Get the overall effect of the loop on the variable.  At this
	 point get_scev returns a NULL_TREE if the condition after the
	 crossing of a loop has not been determined yet.  If the
	 outside visible value is chrec_top, then that means that the
	 analyzer has failed to compute the number of iterations in
	 the loop.  */
      else 
	res = get_scev (loop_nb, version);
      
      break;
      
    default:
      res = chrec_top;
      break;
    }
  
  DBG_S (fprintf (stderr, "  (version = ");
	 debug_generic_expr (version);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, "  (loop_nb = %d)\n", loop_nb);
	 fprintf (stderr, "  (scev = ");
	 debug_generic_expr (res);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, ")\n"));
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
#if defined ENABLE_CHECKING 
  if (to_add == NULL_TREE
      || chrec_before == NULL_TREE)
    abort ();
#endif
  
  switch (TREE_CODE (chrec_before))
    {
    case POLYNOMIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) < loop_nb)
	return build_polynomial_chrec (loop_nb, chrec_before, to_add);
      else
	return build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   build_polynomial_evolution_in_loop (loop_nb, 
					       CHREC_LEFT (chrec_before), 
					       to_add),
	   CHREC_RIGHT (chrec_before));
      
    case EXPONENTIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) < loop_nb)
	return build_polynomial_chrec (loop_nb, chrec_before, to_add);
      else
	return build_exponential_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   build_polynomial_evolution_in_loop (loop_nb, 
					       CHREC_LEFT (chrec_before), 
					       to_add),
	   CHREC_RIGHT (chrec_before));
      
    case VAR_DECL:
    case PARM_DECL:
    case INTEGER_CST:
    case INTERVAL_CHREC:
    default:
      /* These nodes do not depend on a loop.  */
      return build_polynomial_chrec (loop_nb, chrec_before, to_add);
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
#if defined ENABLE_CHECKING 
  if (to_mult == NULL_TREE
      || chrec_before == NULL_TREE)
    abort ();
#endif
  
  switch (TREE_CODE (chrec_before))
    {
    case POLYNOMIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) < loop_num)
	return build_exponential_chrec (loop_num, chrec_before, to_mult);
      else
	return build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   build_exponential_evolution_in_loop (loop_num, 
						CHREC_LEFT (chrec_before), 
						to_mult),
	   CHREC_RIGHT (chrec_before));
      
    case EXPONENTIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) < loop_num)
	return build_exponential_chrec (loop_num, chrec_before, to_mult);
      else
	return build_exponential_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   build_exponential_evolution_in_loop (loop_num, 
						CHREC_LEFT (chrec_before), 
						to_mult),
	   CHREC_RIGHT (chrec_before));
      
    case VAR_DECL:
    case PARM_DECL:
    case INTEGER_CST:
    case INTERVAL_CHREC:
    default:
      /* These nodes do not depend on a loop.  */
      return build_exponential_chrec (loop_num, chrec_before, to_mult);
    }
}

/* Add TO_ADD to the evolution part of CHREC_BEFORE in the dimension
   of LOOP_NB.  */

static tree 
add_to_evolution (unsigned loop_nb, 
		  tree chrec_before,
		  tree to_add)
{
  tree res = NULL_TREE;
  
  if (to_add == NULL_TREE)
    return chrec_before;
  
#if defined ENABLE_CHECKING
  if (chrec_before == NULL_TREE)
    abort ();
#endif
  
  DBG_S (fprintf (stderr, "(add_to_evolution \n");
	 fprintf (stderr, "  (loop_nb = %d)\n", loop_nb);
	 fprintf (stderr, "  (chrec_before = ");
	 debug_generic_expr (chrec_before);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, "  (to_add = ");
	 debug_generic_expr (to_add);
	 fprintf (stderr, "  )\n");
	 if (TREE_CODE (chrec_before) == POLYNOMIAL_CHREC
	     && CHREC_VARIABLE (chrec_before) == loop_nb)
	 fprintf (stderr, "  constructing a polynomial of degree > 1 \n"));
  
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
		res = build_polynomial_evolution_in_loop (loop_nb, 
							  chrec_before, 
							  to_add);
	      else
		/* testsuite/.../ssa-chrec-43.c.  */
		res = build_polynomial_chrec 
		  (CHREC_VARIABLE (chrec_before), 
		   add_to_evolution (loop_nb, CHREC_LEFT (chrec_before), to_add), 
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
		   add_to_evolution (loop_nb, CHREC_LEFT (chrec_before), to_add), 
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
		res = build_polynomial_evolution_in_loop (loop_nb, 
							  chrec_before, 
							  to_add);
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
		  
		  chrec_before = replace_initial_condition 
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
	res = build_polynomial_evolution_in_loop (loop_nb, 
						  chrec_before, 
						  to_add);
      else
	/* FIXME Taylor.  */
	res = chrec_top;
      break;
      
    case PERIODIC_CHREC:
      res = chrec_top;
      break;
      
    case VAR_DECL:
    case PARM_DECL:
    case INTERVAL_CHREC:
    default:
      if (no_evolution_in_loop_p (chrec_before, loop_nb))
	/* testsuite/.../ssa-chrec-39.c.  */
	res = build_polynomial_evolution_in_loop (loop_nb, chrec_before, 
						  to_add);
      else
	/* testsuite/.../ssa-chrec-20.c.  */
	res = add_expr_to_loop_evolution (loop_nb, chrec_before, to_add);
      break;
    }
  
  DBG_S (fprintf (stderr, "  (res = ");
	 debug_generic_expr (res);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, ")\n"));
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
  
#if defined ENABLE_CHECKING
  if (chrec_before == NULL_TREE)
    abort ();
#endif
  
  DBG_S (fprintf (stderr, "(multiply_evolution \n");
	 fprintf (stderr, "  (loop_nb = %d)\n", loop_nb);
	 fprintf (stderr, "  (chrec_before = ");
	 debug_generic_expr (chrec_before);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, "  (to_mult = ");
	 debug_generic_expr (to_mult);
	 fprintf (stderr, "  )\n"));

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
		res = reset_evolution_in_loop (loop_nb, 
					       chrec_before, 
					       chrec_top);
	      else
		res = build_polynomial_chrec 
		  (CHREC_VARIABLE (chrec_before), 
		   multiply_evolution (loop_nb, CHREC_LEFT (chrec_before), to_mult), 
		   CHREC_RIGHT (chrec_before));
	      
	      break;
	      
	    case EXPONENTIAL_CHREC:
	      if (loop_nb != CHREC_VARIABLE (to_mult)
		  && loop_is_included_in (loop_nb, CHREC_VARIABLE (to_mult)))
		res = build_exponential_evolution_in_loop (loop_nb, 
							   chrec_before, 
							   to_mult);
	      else
		res = build_exponential_chrec 
		  (CHREC_VARIABLE (chrec_before), 
		   multiply_evolution (loop_nb, CHREC_LEFT (chrec_before), to_mult), 
		   CHREC_RIGHT (chrec_before));
	      
	      break;
	      
	    default:
	      if (loop_nb != CHREC_VARIABLE (to_mult)
		  && loop_is_included_in (loop_nb, CHREC_VARIABLE (to_mult)))
		res = build_exponential_evolution_in_loop (loop_nb, 
							   chrec_before, 
							   to_mult);
	      else
		{
		  tree init_cond_to_mult = initial_condition (to_mult);
		  
		  chrec_before = replace_initial_condition 
		    (to_mult, chrec_before);
		  res = build_exponential_evolution_in_loop 
		    (loop_nb, chrec_before, init_cond_to_mult);
		}
	      break;
	    }
	}
      
      else
	/* testsuite/.../ssa-chrec-22.c.  */
	res = multiply_by_expr_the_loop_evolution (loop_nb, 
						   chrec_before, 
						   to_mult);
      break;
      
    case POLYNOMIAL_CHREC:
      if (no_evolution_in_loop_p (chrec_before, loop_nb))
	/* testsuite/.../ssa-chrec-23.c.  */
	res = build_exponential_evolution_in_loop (loop_nb, 
						   chrec_before, 
						   to_mult);
      else
	/* FIXME Taylor.  */
	res = chrec_top;
      break;
      
    case PERIODIC_CHREC:
      res = chrec_top;
      break;
      
    case VAR_DECL:
    case PARM_DECL:
    case INTERVAL_CHREC:
    default:
      if (no_evolution_in_loop_p (chrec_before, loop_nb))
	/* testsuite/.../ssa-chrec-22.c.  */
	res = build_exponential_evolution_in_loop (loop_nb, 
						   chrec_before, 
						   to_mult);
      else
	res = multiply_by_expr_the_loop_evolution (loop_nb, 
						   chrec_before, 
						   to_mult);
      break;
    }
  
  DBG_S (fprintf (stderr, "  (res = ");
	 debug_generic_expr (res);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, ")\n"));
  return res;
}



/* This section deals with the approximation of the number of
   iterations a loop will run.  */

/* Helper function.  */

static inline tree
cannot_analyze_loop_nb_iterations_yet (void)
{
  /* Do not update the loop->nb_iterations.  */
  DBG_S (fprintf (stderr, "  (nb_iterations cannot be determined)\n");
	 fprintf (stderr, ")\n"));
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
  DBG_S (fprintf (stderr, "  (set_nb_iterations_in_loop = ");
	 debug_generic_expr (res);
	 fprintf (stderr, "  )\n");
	 fprintf (stderr, ")\n"));

  loop->nb_iterations = res;
  return res;
}

/* In order to determine the number of iterations in a loop, we have
   to perform some computations on integers.  If the evolution
   functions on which we work contain INTEGER_CSTs with different
   types, ie. size_t, unsigned, ..., then the computation of the
   number of iterations may fail.  Example:  

   fold_minus 
   op 0 = {0, +, 1}_1
   op 1 = 168
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

/* Determines the number of iterations RES in the loop LOOP_NUM that
   verify the equality CHREC_A (RES) == CHREC_B (RES).  */

static tree 
nb_iterations_in_loop_until_eq (unsigned loop_num ATTRIBUTE_UNUSED, 
				tree chrec_a, 
				tree chrec_b)
{
  tree overlap_iterations_a, overlap_iterations_b;
  /* FIXME: for the moment, this ugly function is disabled.  */
  return chrec_top;

  if (chrec_contains_undetermined (chrec_a)
      || chrec_contains_undetermined (chrec_b))
    return chrec_top;
  
  if (chrec_eq (initial_condition (chrec_a),
		initial_condition (chrec_b)))
    return integer_zero_node;
  
  analyze_overlapping_iterations (chrec_a, chrec_b, 
				  &overlap_iterations_a, 
				  &overlap_iterations_b);
  
  if (chrec_eq (initial_condition (overlap_iterations_a),
		initial_condition (overlap_iterations_b)))
    return initial_condition (overlap_iterations_a);
  
  return chrec_top;
}


/* The following section contains Scalar Dependence Graph (SDG)
   related functions.  The SDG is used for directing the analysis of
   the induction variables.  */

/* Helper function for the pattern "LHS op OPND".  */

static void 
record_dependences_for_opnd (unsigned loop_nb, 
			     tree lhs,
			     tree opnd, 
			     varray_type sdd_graph, 
			     varray_type complete_imperative_list)
{
  if (opnd != NULL_TREE
      && TREE_CODE (opnd) == SSA_NAME
      
      /* Verify that the operand is not a function parameter.  */
      && SSA_NAME_DEF_STMT (opnd) != NULL_TREE
      && TREE_CODE (SSA_NAME_DEF_STMT (opnd)) != NOP_EXPR
      
      /* Don't follow the edges to the outer loops.  */
      && stmt_is_in_loop (SSA_NAME_DEF_STMT (opnd), loop_nb))
    {
      /* If this variable has already been visited by the SDG
	 algorithm, don't analyze it again.  */
      if (!node_already_visited_by_ssa_path (opnd))
	{
	  if (record_dependences_for_version (loop_nb, opnd, sdd_graph, 
					      complete_imperative_list))
	    SDD_insert (lhs, opnd, sdd_graph);
	  
	  /* Analyze dependences only on the current loop.  */
	  else if (loop_num (loop_of_stmt (SSA_NAME_DEF_STMT (opnd))) 
		   == loop_nb)
	    record_dependences_for_definition 
	      (loop_nb, SSA_NAME_DEF_STMT (opnd), sdd_graph, 
	       complete_imperative_list);
	}
    }
}

/* Helper function for phi nodes.  */

static void
record_dependences_for_phi_node (unsigned loop_nb, 
				 tree phi_node, 
				 varray_type sdd_graph, 
				 varray_type complete_imperative_list)
{
  int i;
  
  if (PHI_MARKED (phi_node) == 1)
    return;
  
  PHI_MARKED (phi_node) = 1;
  for (i = 0; i < PHI_NUM_ARGS (phi_node); i++)
    record_dependences_for_opnd 
      (loop_nb, PHI_RESULT (phi_node), PHI_ARG_DEF (phi_node, i), 
       sdd_graph, complete_imperative_list);
  PHI_MARKED (phi_node) = 0;
}

/* Helper function for modify_exprs.  */

static void
record_dependences_for_modify_expr (unsigned loop_nb, 
				    tree modify_expr, 
				    varray_type sdd_graph, 
				    varray_type complete_imperative_list)
{
  tree lhs, rhs;
  
  lhs = TREE_OPERAND (modify_expr, 0);
  rhs = TREE_OPERAND (modify_expr, 1);
  
  if (rhs == NULL_TREE
      || lhs == NULL_TREE)
    return;
  
  if (VUSE_OPS (stmt_ann (modify_expr)))
    return;
  
  switch (TREE_CODE_LENGTH (TREE_CODE (rhs)))
    {
    case 2:
      record_dependences_for_opnd
	(loop_nb, lhs, TREE_OPERAND (rhs, 1), sdd_graph, 
	 complete_imperative_list);
      
    case 1:
      record_dependences_for_opnd
	(loop_nb, lhs, TREE_OPERAND (rhs, 0), sdd_graph, 
	 complete_imperative_list);
      
    case 0:
      return;
      
    default:
      abort ();
    }
}

/* Records the dependences for the definition RDEF.  */

static void
record_dependences_for_definition (unsigned loop_nb, 
				   tree rdef, 
				   varray_type sdd_graph,
				   varray_type complete_imperative_list)
{
  if (rdef == NULL_TREE)
    return;
  
  switch (TREE_CODE (rdef))
    {
    case PHI_NODE:
      /* Analyze only the definitions in the current loop.  */
      if (stmt_is_in_loop (rdef, loop_nb))
	record_dependences_for_phi_node
	  (loop_nb, rdef, sdd_graph, complete_imperative_list);
      break;
      
    case MODIFY_EXPR:
      /* Analyze only the definitions in the current loop.  */
      if (stmt_is_in_loop (rdef, loop_nb))
	record_dependences_for_modify_expr 
	  (loop_nb, rdef, sdd_graph, complete_imperative_list);
      break;
      
    case ASM_EXPR:
    case NOP_EXPR:
      break;
     
    default:
      abort ();
      break;
    }
}

/* Entry point in the record_dependences_* module.  Compute the scalar
   dependences for the ssa_name VERSION, and record these dependence
   relations into the SDD_GRAPH.  Construct the
   COMPLETE_IMPERATIVE_LIST that contains all the nodes of the
   SDD_GRAPH.  */

static bool
record_dependences_for_version (unsigned loop_nb, 
				tree version, 
				varray_type sdd_graph, 
				varray_type complete_imperative_list)
{
  tree scev;
  
  /* Avoid multiple insertions of a same variable in the imperative
     list.  */
  if (variable_is_in_imperative_list_p (SSA_NAME_VAR (version),
					complete_imperative_list))
    return false;

  /* When the evolution function of the version is already known,
     there is no need to reanalyze it.  */
  scev = get_scev (loop_nb, version);
  if (scev != chrec_not_analyzed_yet)
    return false;

  insert_ssa_name_in_imperative_list (version, complete_imperative_list);
  record_dependences_for_definition 
    (loop_nb, SSA_NAME_DEF_STMT (version), sdd_graph, 
     complete_imperative_list);
  return true;
}

/* Construct the schedule of the analysis.  The schedule keeps track
   of the dependences coming from assignments: "a = b + c" adds the
   dependences "a->b" and "a->c", such that the evolution of "a" will
   be analyzed after the evolution of "b" and "c".  */

static void
construct_schedule (unsigned loop_nb, 
		    tree version, 
		    varray_type schedule)
{
  varray_type sdd_graph, complete_imperative_list;
  
  if (TREE_CODE (version) != SSA_NAME)
    return;
  
  DBG_S (fprintf (stderr, "(construct_schedule \n"));
  
  VARRAY_TREE_INIT (sdd_graph, 100, "sdd_graph");
  VARRAY_GENERIC_PTR_INIT (complete_imperative_list, 100, 
			   "complete_imperative_list");
  
  /* Construct the SDG = SDD = Scalar Data Dependence Graph.  */
  VARRAY_TREE_INIT (already_visited, 37, "already_visited");
  record_dependences_for_version 
    (loop_nb, version, sdd_graph, complete_imperative_list);
  varray_clear (already_visited);
  DBG_SCHEDULE_S (debug_schedule (complete_imperative_list));
  
  /* Then establish a schedule for this analysis.  */
  wolfe_topological_sort_SDD_with_SCCs (complete_imperative_list, 
					schedule, sdd_graph);
  DBG_SCHEDULE_S (debug_schedule (schedule));
  
  varray_clear (sdd_graph);
  varray_clear (complete_imperative_list);
  DBG_S (fprintf (stderr, ")\n"));
}

/* Insert a tuple (A, B), ie. "A depends on B", in the SDD_GRAPH.  */

static void
SDD_insert (tree a, 
	    tree b,
	    varray_type sdd_graph)
{
  if (TREE_CODE (a) == SSA_NAME 
      && TREE_CODE (b) == SSA_NAME
      && TREE_CODE (SSA_NAME_VAR (a)) != INDIRECT_REF
      && TREE_CODE (SSA_NAME_VAR (b)) != INDIRECT_REF
      && SSA_NAME_VAR (a) != SSA_NAME_VAR (b))
    {
      tree name_a, name_b;
      
      name_a = SSA_NAME_VAR (a);
      name_b = SSA_NAME_VAR (b);
      
      DBG_SDD_S (fprintf (stderr, "SDD_insert \n");
		 debug_generic_expr (name_a);
		 debug_generic_expr (name_b));
      
      /* Insert the tuple (a, b).  */
      VARRAY_PUSH_TREE (sdd_graph, name_a);
      VARRAY_PUSH_TREE (sdd_graph, name_b);
    }
}

/* Topological sorting of the scalar dependence graph.  
   
   For reference, see "High Performance Compilers for Parallel Computing"
   by Michael Wolfe ("3.2.4 Finding Cycles in Directed Graphs" pages
   60 to 64).  */

/* Sort the COMPLETE_IMPERATIVE_LIST in a topological order, and
   output the strongly connected components into ANALYSIS_SCHEDULE.
   
   SDD_GRAPH is a varray of tuples of nodes:
   {a, b, d, a, c, b, ...} means  
   - a depends on b 
   - d depends on a 
   - c depends on b 
   ... 
   
   ANALYSIS_SCHEDULE is a varray of varray of nodes. 
   
   COMPLETE_IMPERATIVE_LIST is an array that contains all the nodes of
   the graph, ie. {a, b, c, d, ...}.  */

static void
wolfe_topological_sort_SDD_with_SCCs (varray_type complete_imperative_list, 
				      varray_type analysis_schedule, 
				      varray_type sdd_graph)
{
  unsigned int i;
  unsigned int wolfe_n;
  varray_type wolfe_npre, wolfe_instack, wolfe_stack, wolfe_lowlink;
  
  VARRAY_UINT_INIT (wolfe_npre, 37, "wolfe_npre");
  VARRAY_UINT_INIT (wolfe_lowlink, 37, "wolfe_lowlink");
  VARRAY_UINT_INIT (wolfe_instack, 37, "wolfe_instack");
  VARRAY_UINT_INIT (wolfe_stack, 37, "wolfe_stack");
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (complete_imperative_list); i++)
    {
      VARRAY_PUSH_UINT (wolfe_npre, 0);
      VARRAY_PUSH_UINT (wolfe_lowlink, 0);
      VARRAY_PUSH_UINT (wolfe_instack, 0);
    }
  
  wolfe_n = 0;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (complete_imperative_list); i++)
    if (VARRAY_UINT (wolfe_npre, i) == 0)
      wolfe_scc_recurse 
	(i, complete_imperative_list, analysis_schedule, sdd_graph, 
	 wolfe_npre, wolfe_instack, wolfe_stack, wolfe_lowlink, &wolfe_n);
  
  varray_clear (wolfe_npre);
  varray_clear (wolfe_lowlink);
  varray_clear (wolfe_instack);
  varray_clear (wolfe_stack);
}

/* Recursive helper function.  */

static void
wolfe_scc_recurse (unsigned int x, 
		   varray_type complete_imperative_list,
		   varray_type analysis_schedule, 
		   varray_type sdd_graph, 
		   varray_type wolfe_npre, 
		   varray_type wolfe_instack, 
		   varray_type wolfe_stack, 
		   varray_type wolfe_lowlink,
		   unsigned int *wolfe_n)
{
  unsigned int i;
  tree var = var_at_index_in_imperative_list (x, complete_imperative_list);
  
  ++(*wolfe_n);
  VARRAY_UINT (wolfe_npre, x) = *wolfe_n;
  VARRAY_UINT (wolfe_lowlink, x) = *wolfe_n;
  VARRAY_PUSH_UINT (wolfe_stack, x);
  VARRAY_UINT (wolfe_instack, x) = 1;
  
  /* For Y in the successors of X.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (sdd_graph); i+=2)
    {
      tree a, b;
      
      a = VARRAY_TREE (sdd_graph, i);
      b = VARRAY_TREE (sdd_graph, i+1);
      
      if (a == var)
	{
	  int y;
	  
	  /* Find the index Y in COMPLETE_IMPERATIVE_LIST corresponding to 
	     the successor of VAR.  */
	  y = index_of_variable_in_imperative_list 
	    (b, complete_imperative_list);
	  
#if defined ENABLE_CHECKING
	  if (y == -1)
	    abort ();
#endif
	  
	  if (VARRAY_UINT (wolfe_npre, y) == 0)
	    {
	      unsigned int llx, lly;
	      
	      wolfe_scc_recurse (y, complete_imperative_list, 
				 analysis_schedule, sdd_graph, 
				 wolfe_npre, wolfe_instack, wolfe_stack, 
				 wolfe_lowlink, wolfe_n);
	      
	      llx = VARRAY_UINT (wolfe_lowlink, x);
	      lly = VARRAY_UINT (wolfe_lowlink, y);
	      VARRAY_UINT (wolfe_lowlink, x) = (llx < lly) ? llx : lly;
	    }
	  else if (VARRAY_UINT (wolfe_npre, y) < VARRAY_UINT (wolfe_npre, x)
		   && VARRAY_UINT (wolfe_instack, y))
	    {
	      unsigned int llx, lly;
	      
	      llx = VARRAY_UINT (wolfe_lowlink, x);
	      lly = VARRAY_UINT (wolfe_npre, y);
	      VARRAY_UINT (wolfe_lowlink, x) = (llx < lly) ? llx : lly;
	    }
	}
    }
  
  if (VARRAY_UINT (wolfe_npre, x) == VARRAY_UINT (wolfe_lowlink, x))
    {
      struct schedule_elt *sched_elt;
      
      sched_elt = new_schedule_elt ();
      
      DBG_SCHEDULE_S (fprintf (stderr, "(Wolfe_SCC \n"));
      for (;;)
	{
	  unsigned int w;
	  tree w_tree;
	  
	  w = VARRAY_TOP_UINT (wolfe_stack);
	  VARRAY_POP (wolfe_stack);
	  VARRAY_UINT (wolfe_instack, w) = 0;
	  
	  w_tree = 
	    ssa_name_at_index_in_imperative_list (w, complete_imperative_list);
	  DBG_SCHEDULE_S (debug_generic_expr (w_tree));
	  
	  /* Enqueue W_TREE into the SCC.  */
	  VARRAY_PUSH_TREE (SCHED_SCC (sched_elt), w_tree);
	  
	  if (w == x)
	    break;
	}
      DBG_SCHEDULE_S (fprintf (stderr, ")\n"));
      
      VARRAY_PUSH_GENERIC_PTR (analysis_schedule, sched_elt);
    }
}

/* Given a schedule that contains SCCs of induction variables to be
   analyzed, this calls the appropriate analyzer (either the single IV
   analyzer, or the mixers analyzer).  */

static void
analyze_scalars_from_schedule (varray_type schedule)
{
  unsigned int j;
  
  DBG_S (fprintf (stderr, "(analyze_scalars_from_schedule \n"));
  
  /* For each SCC in the schedule.  */
  for (j = 0; j < VARRAY_ACTIVE_SIZE (schedule); j++)
    {
      struct schedule_elt *sched_elt;
      
      sched_elt = (struct schedule_elt *) VARRAY_GENERIC_PTR (schedule, j);
      
      if (VARRAY_ACTIVE_SIZE (SCHED_SCC (sched_elt)) == 1)
	/* The simple case where the SCC contains a single variable.  */
	analyze_evolution (VARRAY_TREE (SCHED_SCC (sched_elt), 0));
      
      else
	analyze_evolution_scc (SCHED_SCC (sched_elt));
    }
  
  DBG_S (fprintf (stderr, ")\n"));
}



/* This section selects the loops that will be good candidates for the
   scalar evolution analysis.
   
   Note: This section will be rewritten to expose a better interface
   to other client passes.  For the moment, greedily select all the
   loop nests we could analyze.  */

/* Select the candidate loop nests for the analysis.  This function
   initializes the EXIT_CONDITIONS array.  The vector EXIT_CONDITIONS is
   initialized in a loop-depth-first order, ie. the inner loops
   conditions appear before the outer.  This property of the
   EXIT_CONDITIONS list is exploited by the evolution analyzer.  */

void
select_loops_exit_conditions (struct loops *loops, 
			      varray_type exit_conditions)
{
  struct loop *function_body = loops->parray[0];
  
  get_exit_conditions_rec (inner_loop (function_body), exit_conditions);
  
  /* This is an extension for an interprocedural analysis: the
     function_body is a loop, and the outer loop is the context of the
     caller.  The interprocedural analysis is one more reason to
     compute the chrecs symbolically: the functions parameters are
     considered as symbols defined in the outer context.  */
  if (0)
    {
      flow_loop_scan (function_body, LOOP_EXIT_EDGES);
      if (loop_num_exits (function_body) == 1)
	{
	  tree loop_condition = get_loop_exit_condition (function_body);

	  if (loop_condition)
	    VARRAY_PUSH_TREE (exit_conditions, loop_condition);
	}
    }
}

/* Recursively determine and enqueue the exit conditions for a loop.  */

static void 
get_exit_conditions_rec (struct loop *loop, 
			 varray_type exit_conditions)
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
	VARRAY_PUSH_TREE (exit_conditions, loop_condition);
    }
}

/* For a loop with a single exit edge, determine the COND_EXPR that
   guards the exit edge.  If the expression is too difficult to
   analyze, then give up.  */

tree 
get_loop_exit_condition (struct loop *loop)
{
  tree res = NULL_TREE;
  
  DBG_S (fprintf (stderr, "(get_loop_exit_condition \n  "));
  
  if (loop_exit_edges (loop))
    {
      edge exit_edge;
      tree expr;
      
      exit_edge = loop_exit_edge (loop, 0);
      expr = last_stmt (edge_src (exit_edge));
      
      if (analyzable_condition (expr))
	res = expr;
    }
  
  DBG_S (debug_generic_expr (res);
	 fprintf (stderr, ")\n"));
  return res;
}

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



/* Debugging functions section.  */

/* Dump all the evolution functions of the variables contained in
   SCHEDULE.  */

static void 
dump_evolution_functions (FILE *file, 
			  varray_type schedule)
{
  unsigned int j;

  /* print_generic_stmt (file, 
     DECL_SAVED_TREE (current_function_decl), 0);  */
  fprintf (file, "\n\n  Scalar evolution functions: \n\n");
  
  /* For each SCC in the schedule.  */
  for (j = 0; j < VARRAY_ACTIVE_SIZE (schedule); j++)
    {
      unsigned k;
      struct schedule_elt *sched_elt;

      sched_elt = (struct schedule_elt *) 
	VARRAY_GENERIC_PTR (schedule, j);
      
      /* For each variable in the SCC.  */
      for (k = 0; k < VARRAY_ACTIVE_SIZE (SCHED_SCC (sched_elt)); k++)
	{
	  tree var, chrec;
	  
	  var = SSA_NAME_VAR (VARRAY_TREE (SCHED_SCC (sched_elt), k));
	  chrec = get_scev (0, var);
	  
	  print_generic_stmt (file, var, 0);
	  fprintf (file, "  ->  ");
	  print_generic_stmt (file, chrec, 0);
	  fprintf (file, "\n");
	}
    }
}

/* Debug a strongly connected component of the schedule.  */

void
dump_schedule_scc (FILE *file, 
		   varray_type sched_scc)
{
  unsigned int i;

  fprintf (file, "\n  (scc = \n");
  for (i = 0; i < VARRAY_ACTIVE_SIZE (sched_scc); i++)
    {
      fprintf (file, "    elt %d = ", i);
      print_generic_stmt (file, VARRAY_TREE (sched_scc, i), 0);
    }
  fprintf (file, "  )\n");
}

/* Debug an element of the schedule.  */

void
dump_schedule_elt (FILE *file, 
		   struct schedule_elt *sched_elt)
{
  fprintf (file, "(schedule_elt \n");
  dump_schedule_scc (file, SCHED_SCC (sched_elt));
  fprintf (file, ")\n");
}

/* Debug the schedule.  */

void 
debug_schedule (varray_type schedule)
{
  unsigned int i;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (schedule); i++)
    dump_schedule_elt (stderr, VARRAY_GENERIC_PTR (schedule, i));
}

/* Draw using DOTTY the scalar dependence graph.  */

void 
draw_SDD (varray_type sdd_graph)
{
  FILE *df;
  df = fopen ("tree_sdd.dot", "w");
  if (df)
    {
      sdd2dot (df, sdd_graph);
      fclose (df);
      system ("dotty tree_sdd.dot");
    }
}

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
     
/* The Scalar Data Dependence (SDD) graph is stored in an array by
   tuples with the following convention:
   
   - the first element in the tuple is the origin of the edge,
   
   - and the second element is the destination.
   
   Some researchers represent the dependence relations with the edges
   in the opposite direction: for "A depends on B" they draw "A <- B".
*/

void 
sdd2dot (FILE *file, 
	 varray_type sdd_graph) 
{
  unsigned int idx = 0;
  tree a, b;
  
  fprintf (file, "digraph foo_bar_baz\n{\n");
  
  if (sdd_graph)
    {
      while (idx < VARRAY_ACTIVE_SIZE (sdd_graph))
	{
	  a = VARRAY_TREE (sdd_graph, idx++);
	  b = VARRAY_TREE (sdd_graph, idx++);

	  fprintf (file, "  \"%s\" -> \"%s\"\n", 
		   IDENTIFIER_POINTER (DECL_NAME (a)),
		   IDENTIFIER_POINTER (DECL_NAME (b)));
	}
    }
  fputs ("}\n\n", file);
}



/* Initializer.  */

void
initialize_scalar_evolutions_analyzer (struct loops *loops, 
				       varray_type ev_info)
{
  scev_loops = loops;
  scev_info = ev_info;
  
#if 0
  DBG_S (fprintf (stderr, "Loop and SSA IRs:\n");
	 debug_loop_ir ());
#endif
  
  /* The elements below are unique.  The values contained in these
     intervals are not used.  */
  chrec_not_analyzed_yet = build_interval_chrec (build_int_2 (1111, 0), 
						 build_int_2 (2111, 0));
  chrec_top = build_interval_chrec (build_int_2 (2222, 0), 
				    build_int_2 (3222, 0));
  chrec_bot = build_interval_chrec (build_int_2 (3333, 0), 
				    build_int_2 (4333, 0));
  chrec_symbolic_parameter = build_interval_chrec (build_int_2 (4444, 0),
						   build_int_2 (5444, 0));
}

/* Finalize.  */

void 
finalize_scalar_evolutions_analyzer (void)
{
  if (tree_dump_file)
    fprintf (tree_dump_file, "\n\n");
}

/* Helper function.  */

static tree
analyze_scalar_evolution_1 (unsigned loop_nb,
			    tree version)
{
  /* ANALYSIS_SCHEDULE is a list of strongly connected components
     (SCC).  The list contains the order in which the SCCs have to
     be analyzed.  An element of the ANALYSIS_SCHEDULE contains a
     list of variables that constitute an SCC.  */
  varray_type analysis_schedule;

  VARRAY_GENERIC_PTR_INIT (analysis_schedule, 37, "analysis_schedule");
  construct_schedule (loop_nb, version, analysis_schedule);
  analyze_scalars_from_schedule (analysis_schedule);
  
  if (tree_dump_file)
    dump_evolution_functions (tree_dump_file, analysis_schedule);
  
  varray_clear (analysis_schedule);

  return evolution_at_version (loop_nb, version);
}



/* This section contains all the entry points: 
   - number_of_iterations_in_loop,
   - number_of_iterations_for_all_loops,
   - analyze_scalar_evolution,
   - instantiate_parameters.
*/

/* Entry point for the scalar evolution analyzer.
   
   Analyzes and returns the scalar evolution of the ssa_name VERSION.
   LOOP_NB is the number of the loop that has to be analyzed: the
   definitions in the outer loops are considered as constants, and
   left under a symbolical form.  
   
   Example of use: having a pointer VERSION to a SSA_NAME node, in
                   order to determine the evolution function of
                   VERSION, use the following calls:
   
   unsigned loop_nb = loop_num (loop_of_stmt (SSA_NAME_DEF_STMT (version)));
   tree chrec_with_symbols = analyze_scalar_evolution (loop_nb, version);
   tree chrec_instantiated = instantiate_parameters 
     (loop_nb, chrec_with_symbols, version);
*/

tree 
analyze_scalar_evolution (unsigned loop_nb, 
			  tree version)
{
  tree res;
  DBG_S (fprintf (stderr, "(analyze_scalar_evolution \n");
	 fprintf (stderr, "  (loop_nb = %d)\n", loop_nb);
	 fprintf (stderr, "  (version = ");
	 debug_generic_expr (version);
	 fprintf (stderr, "  )\n"));
  
  res = evolution_at_version (loop_nb, version);
  
  /* When the evolution is not in the knowledge base, analyze it.  */
  if (TREE_CODE (version) == SSA_NAME 
      && initial_condition (res) == version)
    res = analyze_scalar_evolution_1 (loop_nb, version);
  
  /* Don't expose to the client passes all the information from the
     evolution knowledge base: select only the evolution parts in the
     current LOOP_NB and in the outer containing loops.  */
  res = select_outer_and_current_evolutions (loop_nb, res);
  
  DBG_S (fprintf (stderr, ")\n"));
  return res;
}

/* Analyze all the parameters of the chrec that were left under a
   symbolic form, except for version.  */

tree
instantiate_parameters (unsigned loop_nb,
			tree chrec, 
			tree version)
{
  tree res;
  
  if (chrec == NULL_TREE
      || automatically_generated_chrec_p (chrec))
    res = chrec;
  
  else if (TREE_CODE (chrec) == SSA_NAME
	   || TREE_CODE (chrec) == VAR_DECL
	   || TREE_CODE (chrec) == PARM_DECL)
    {
      /* Don't instantiate VERSION.  This is used for avoiding the
	 instantiation of recursively defined functions, such as:
	 a_2  ->  {0, +, 1, +, a_2}_1  
	 
	 FIXME: Maybe we'll need to pass in a vector of versions that
	 we don't want to instantiate.  */
      if (chrec == version)
	res = chrec;
      
      else
	res = analyze_scalar_evolution (loop_nb, chrec);
    }
  
  else
    switch (TREE_CODE (chrec))
      {
      case PLUS_EXPR:
	res = chrec_fold_plus 
	  (instantiate_parameters (loop_nb, CHREC_LEFT (chrec), version),
	   instantiate_parameters (loop_nb, CHREC_RIGHT (chrec), version));
	break;
	
      case MINUS_EXPR:
	res = chrec_fold_minus 
	  (instantiate_parameters (loop_nb, CHREC_LEFT (chrec), version),
	   instantiate_parameters (loop_nb, CHREC_RIGHT (chrec), version));
	break;
	
      case MULT_EXPR:
	res = chrec_fold_multiply 
	  (instantiate_parameters (loop_nb, CHREC_LEFT (chrec), version),
	   instantiate_parameters (loop_nb, CHREC_RIGHT (chrec), version));
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
	  case 2:
	    res = build 
	      (TREE_CODE (chrec), 
	       TREE_TYPE (chrec),
	       instantiate_parameters (loop_nb, TREE_OPERAND (chrec, 0), version),
	       instantiate_parameters (loop_nb, TREE_OPERAND (chrec, 1), version));
	    break;
	    
	  case 1:
	    res = instantiate_parameters (loop_nb, TREE_OPERAND (chrec, 0), version);
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
  
  DBG_S (fprintf (stderr, "(number_of_iterations_in_loop \n"));
  
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
      
      if (chrec0 == chrec_top
	  || chrec0 == chrec_symbolic_parameter)
	/* KEEP_IT_SYMBOLIC.  */
	chrec0 = opnd0;
      
      DBG_S (fprintf (stderr, "  (loop_nb = %d)\n", loop_nb);
	     fprintf (stderr, "  (loop_while_expr_is_true: ");
	     debug_generic_expr (test);
	     fprintf (stderr, "  )\n");
	     fprintf (stderr, "  (chrec0 = ");
	     debug_generic_expr (chrec0);
	     fprintf (stderr, "  )\n"));
      
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
      
      chrec0 = instantiate_parameters (loop_nb, chrec0, opnd0);
      chrec1 = instantiate_parameters (loop_nb, chrec1, opnd1);
      
      chrec0 = homogenize_integer_cst_types_for_nb_iter (chrec0);
      chrec1 = homogenize_integer_cst_types_for_nb_iter (chrec1);
      
      if (chrec0 == chrec_top
	  || chrec0 == chrec_symbolic_parameter)
	/* KEEP_IT_SYMBOLIC.  */
	chrec0 = opnd0;
      
      if (chrec1 == chrec_top
	  || chrec1 == chrec_symbolic_parameter)
	/* KEEP_IT_SYMBOLIC.  */
	chrec1 = opnd1;
      
      DBG_S (fprintf (stderr, "  (loop_nb = %d)\n", loop_nb);
	     fprintf (stderr, "  (loop_while_expr_is_true: ");
	     debug_generic_expr (test);
	     fprintf (stderr, "  )\n");
	     fprintf (stderr, "  (chrec0 = ");
	     debug_generic_expr (chrec0);
	     fprintf (stderr, "  )\n");
	     fprintf (stderr, "  (chrec1 = ");
	     debug_generic_expr (chrec1);
	     fprintf (stderr, "  )\n"));
	
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
	    (loop, nb_iterations_in_loop_until_eq (loop_nb, 
						   chrec0, chrec1));
	  
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
number_of_iterations_for_all_loops (varray_type exit_conditions)
{
  unsigned int i;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (exit_conditions); i++)
    {
      tree exit_condition, nb_iter;
      
      exit_condition = VARRAY_TREE (exit_conditions, i);
      nb_iter = number_of_iterations_in_loop (loop_of_stmt (exit_condition));
    }
}

static struct loops *current_loops;
static varray_type ev_info;

/* Runs the analysis of scalar evolutions.  */

static void
scev_init (void)
{
  current_loops = loop_optimizer_init (NULL);
	  
  if (!current_loops)
    return;

  VARRAY_GENERIC_PTR_INIT (ev_info, 37, "ev_info");
  initialize_scalar_evolutions_analyzer (current_loops, ev_info);
}

static void
scev_anal (void)
{
  varray_type exit_conditions;

  VARRAY_GENERIC_PTR_INIT (exit_conditions, 37, "exit_conditions");
  select_loops_exit_conditions (current_loops, exit_conditions);
  number_of_iterations_for_all_loops (exit_conditions);
  varray_clear (exit_conditions);
}

static void
scev_depend (void)
{
  analyze_all_data_dependences ();
}

static void
scev_vectorize (void)
{
  bitmap_clear (vars_to_rename);

  vectorize_loops (current_loops, ev_info);
}

static void
scev_done (void)
{
  if (current_loops)
    {
      varray_clear (ev_info);
      finalize_scalar_evolutions_analyzer ();
      loop_optimizer_finalize (current_loops, NULL);
      current_loops = NULL;
    }
}

static bool
gate_scev (void)
{
  return flag_scalar_evolutions || flag_tree_vectorize;
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

static bool
gate_scev_anal (void)
{
  return current_loops && flag_scalar_evolutions != 0;
}

struct tree_opt_pass pass_scev_anal = 
{
  "scev",				/* name */
  gate_scev_anal,			/* gate */
  scev_anal,				/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_SCALAR_EVOLUTIONS,			/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  PROP_scev,				/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0					/* todo_flags_finish */
};

static bool
gate_scev_depend (void)
{
  return current_loops && flag_all_data_deps != 0;
}

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
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0					/* todo_flags_finish */
};

static bool
gate_scev_vectorize (void)
{
  return current_loops && flag_tree_vectorize != 0;
}

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
