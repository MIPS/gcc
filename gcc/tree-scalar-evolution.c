/* Scalar evolution detector.
   Copyright (C) 2003 Free Software Foundation, Inc.
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

/* This pass analyses the evolution of induction variables in a loop.
   The algorithm is based on the SSA representation, and on the loop
   hierarchy tree.
   
   A short sketch of the algorithm is:
   
   - First step: gather the scalar variables that has to be analyzed.
   This is called the imperative list.  The minimal imperative list
   contains all the variables that are used in the exit conditions of
   the loop nest.  Other variables such as those used in array
   accesses are then enqueued on this list.
   
   - Second step: for each variable of the imperative list, analyze
   the dependences on other scalar variables in the loop nest.  The
   result is a scalar dependence graph.
   
   - Third step: establish a schedule for the IV analysis that
   satisfies all the dependences of the scalar dependence graph.
   
   - Fourth step: for each variable in the schedule, determine its
   initial condition and then its evolution in the loop nest.
   
   At the end of this analysis, an envelope chain of recurrence is
   associated to each variable of the imperative list.  If a variable
   has not been analyzed, the default answer of the analyzer is:
   "don't know", that corresponds to the [-oo, +oo] element of the
   lattice of intervals.
   
   FIXME: If on the first pass of the analyzer we have not scheduled
   some variables, it could be possible to further refine the
   knowledge base by calling again the analyzer with another
   imperative list.
   
   You can find a more detailed description of the algorithm in:
   http://icps.u-strasbg.fr/~pop/DEA_03_Pop.pdf
   http://icps.u-strasbg.fr/~pop/DEA_03_Pop.ps.gz.  But note that this
   is a preliminary report and some of the details of the algorithm
   might have changed.  I'm working on a research report that updates
   the description of the algorithm to reflect the design choices used
   in this implementation.
   
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
   
   - FIXME zero-trip-problem: This problem consists in determining the
   number of iterations in a loop.  Without such an information some
   cases could not be analyzed correctly.  As an example, consider the
   code:
   
   a = 0
   loop 
     a = a + 1
     loop i = 1, N, 1
       a = a + 2
     endloop
   endloop
   
   The evolution in the first loop is determined by the number of
   iterations in the inner loop.  In this case the inner loop runs N
   times, and the overall effect of the assignment "a = a + 2" from
   the inner loop is "a = a + 2*N".  Thus the evolution is: "a -> {{0,
   +, 2*N+1}_1, +, 2}_2".
   
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


/* Analyzers on the SSA representation.  */
static void analyze_evolution          (tree, struct loop *);
static void analyze_evolution_rec      (tree, struct loop *, struct loop *);
static void analyze_evolution_scc      (varray_type, struct loop*);
static void analyze_evolution_scc_rec  (tree, struct loop *);
static void analyze_initial_condition_depth_zero (struct loop *, tree);
static void analyze_initial_condition  (struct loop *, tree);
static void analyze_evolution_in_loop  (tree, unsigned);
static void monev_follow_ssa_edge      (tree, tree);
static void determine_whether_ssa_name_is_symbolic_parameter (tree, 
							      struct loop *);
static inline bool no_ssa_name_in_arguments_of_phi_node (tree);
static tree get_loop_phi_node_for_variable (struct loop *, tree);

/* Chrec functions.  */
static struct monev_info_str *find_var_monev_info (tree);
static void set_scev                   (unsigned, tree, tree);
static void set_scev_inner_value       (unsigned, tree, tree);
static void set_scev_outer_value       (tree, tree);
static tree get_scev                   (unsigned, tree);
static bool loop_is_included_in        (unsigned, unsigned);
static bool loop_is_included_in_rec    (unsigned, struct loop *);
static tree merge_evolutions           (tree, varray_type);
static void merge_branches_of_condition_phi_node_in_loop (tree, tree);
static void merge_branches_of_condition_phi_node (tree);
static tree evolution_at_version       (unsigned, tree);
static tree add_to_evolution           (unsigned, tree, tree);
static tree multiply_evolution         (unsigned, tree, tree);
static inline tree cannot_analyze_loop_nb_iterations_yet (void);
static inline tree set_nb_iterations_in_loop (struct loop *, tree);
static tree number_of_iterations_in_loop (struct loop *);

/* Scalar Dependence Graph functions.  */
static void schedule_exit_conditions   (struct loop *, varray_type, 
					varray_type);
static void construct_schedule         (varray_type, varray_type);

static void record_dependences_for_ssa_name (struct loop *, tree, 
					     varray_type, varray_type);
static void record_dependences_for_var_rec (struct loop *, struct loop *, 
					    tree, varray_type, varray_type);
static inline void 
record_dependences_for_opnd (struct loop *, tree, tree, varray_type, 
			     varray_type);
static inline void 
record_dependences_for_opnd_rec (struct loop *, tree, tree, tree, 
				 varray_type, varray_type);
static void 
record_dependences_for_modify_expr (struct loop *, tree, varray_type, 
				    varray_type);
static void 
record_dependences_for_modify_expr_rec (struct loop *, tree, tree,
					varray_type, varray_type);
static void 
follow_ssa_edge_and_record_dependences (struct loop *, tree, 
					varray_type, varray_type);
static void 
follow_ssa_edge_and_record_dependences_rec (struct loop *, tree, tree, 
					    varray_type, varray_type);
static void 
SDD_insert_deps_on_loop_exit_vars (struct loop *, tree, struct loop *, 
				   varray_type, varray_type);
static void SDD_insert (tree, tree, varray_type);
static void 
wolfe_topological_sort_SDD_with_SCCs (varray_type, varray_type, varray_type);
static void wolfe_scc_recurse (unsigned int, varray_type, varray_type, 
			       varray_type, varray_type, varray_type, 
			       varray_type, varray_type, unsigned int *);

static void analyze_scalars_from_schedule (varray_type);

/* Selection functions.  */
static bool get_exit_conditions_for_nest (struct loop *, varray_type);
static bool get_exit_conditions_rec    (struct loop *, varray_type);
static tree get_loop_exit_condition    (struct loop *);
static bool analyzable_condition       (tree);

/* ICCP.  */
static tree iccp_follow_ssa_edge_and_compute_scev (int, tree, varray_type);
static void iccp_what_scalars_have_to_be_analyzed_before (struct loop *, tree, 
							  varray_type);
static bool iccp_record_dependences_for_ssa_name (struct loop *, tree, 
						  varray_type, varray_type);
static void iccp_record_dependences_for_opnd (struct loop *, tree, tree,
					      varray_type, varray_type);
static void iccp_record_dependences_for_modify_expr (struct loop *, tree, 
						     varray_type, varray_type);
static void iccp_record_dependences (struct loop *, tree, varray_type,
				     varray_type);
/* Debugging functions.  */
static void dump_evolution_functions (varray_type);

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

static struct loops *monev_loops;
static varray_type monev_info;

/* Debugging dumps.  */
static FILE *dump_file;
static int dump_flags;



/* Imperative list functions.  */

static inline tree ssa_name_at_index_in_imperative_list (int, varray_type);
static inline tree var_at_index_in_imperative_list (int, varray_type);
static inline struct loop *loop_nest_at_index_in_imperative_list (int, 
								  varray_type);
static inline int index_of_variable_in_imperative_list (tree, varray_type);
static inline bool variable_is_in_imperative_list_p (tree, varray_type);
static inline void insert_ssa_name_in_imperative_list (tree, struct loop *, 
						       varray_type);

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

/* Returns the loop nest at INDEX from the imperative list.  */

static inline struct loop *
loop_nest_at_index_in_imperative_list (int index, 
				       varray_type imperative_list)
{
  struct schedule_elt *sched_elt;
  
  sched_elt = (struct schedule_elt *) VARRAY_GENERIC_PTR (imperative_list, 
							  index);
  return SCHED_LOOP_NEST (sched_elt);
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
				    struct loop *loop_nest,
				    varray_type imperative_list)
{
  struct schedule_elt *sched_elt;
  
  if (variable_is_in_imperative_list_p (SSA_NAME_VAR (ssa_name),
					imperative_list))
    return;
  
  DBG_S (fprintf (stderr, "(insert_ssa_name_in_imperative_list \n");
	 fprintf (stderr, "  loop = %d\n", loop_nest->num);
	 fprintf (stderr, "  ssa_name = ");
	 debug_generic_expr (ssa_name);
	 fprintf (stderr, ")\n"));
  
  sched_elt = new_schedule_elt ();
  SCHED_LOOP_NEST (sched_elt) = loop_nest;
  VARRAY_PUSH_TREE (SCHED_SCC (sched_elt), ssa_name);
  VARRAY_PUSH_GENERIC_PTR (imperative_list, sched_elt);
}



/* Analyze the evolution of a SSA_NAME in the LOOP_NEST.  */

static void 
analyze_evolution (tree ssa_name_to_analyze, 
		   struct loop *loop_nest)
{
  tree var_to_analyze = SSA_NAME_VAR (ssa_name_to_analyze);
  tree loop_phi_node;
  
  /* At depth zero, analyze only the initial condition.  */
  if (loop_nest->depth == 0)
    {
      analyze_initial_condition_depth_zero 
	(loop_nest, ssa_name_to_analyze);
      return;
    }
  
  loop_phi_node = get_loop_phi_node_for_variable 
    (loop_nest, var_to_analyze);
  
  DBG_S (fprintf (stderr, "(analyze_evolution \n");
	 fprintf (stderr, "  loop_num = %d\n", loop_nest->num);
	 fprintf (stderr, "  ssa_name_to_analyze = ");
	 debug_generic_expr (ssa_name_to_analyze));
  
  /* The variable has a phi node in this loop: the variable is thus
     "updated" at every iteration of this loop.  */
  if (loop_phi_node)
    {
      analyze_initial_condition (loop_nest, loop_phi_node);
      analyze_evolution_in_loop (loop_phi_node, 0);
    }
  
  /* The variable is not modified in this loop, analyze its evolution
     in the inner loops.  */
  else if (loop_nest->inner)
    analyze_evolution_rec (ssa_name_to_analyze, loop_nest, 
			   loop_nest->inner);
  
  else
    determine_whether_ssa_name_is_symbolic_parameter 
      (ssa_name_to_analyze, loop_nest);
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* Recursive version of the analyzer.  This version is used when the
   loop does not contain a loop phi-node for the analyzed variable.
   In this case we have to search the phi nodes in the inner and
   sibling loops.  */

static void 
analyze_evolution_rec (tree ssa_name_to_analyze, 
		       struct loop *loop_nest,
		       struct loop *loop)
{
  tree var_to_analyze = SSA_NAME_VAR (ssa_name_to_analyze);
  tree loop_phi_node = get_loop_phi_node_for_variable 
    (loop, var_to_analyze);
  
  if (loop_phi_node)
    {
      analyze_initial_condition (loop_nest, loop_phi_node);
      analyze_evolution_in_loop (loop_phi_node, 0);
    }
  else if (loop->inner)
    analyze_evolution_rec (ssa_name_to_analyze, loop_nest, loop->inner);
  else
    determine_whether_ssa_name_is_symbolic_parameter (ssa_name_to_analyze, 
						      loop_nest);
  
  if (loop->next)
    analyze_evolution_rec (ssa_name_to_analyze, loop_nest, loop->next);
}

/* Analyze the evolution of the variables in MIXER.  

   FIXME analysis of mixers:

   At this moment, we have all the information we need to analyze the
   mixer (ie. the evolution of all the other variables on which the
   components of the mixer depends on).
   
   However, for the moment, the analyzer associates to each variable 
   in the MIXER its initial condition, and an evolution of [-oo, +oo].  */

static void 
analyze_evolution_scc (varray_type mixer, 
		       struct loop *loop_nest)
{
  unsigned k;
  
  DBG_S (fprintf (stderr, "(analyze_evolution_scc \n");
	 fprintf (stderr, "  loop_num = %d", loop_nest->num);
	 dump_schedule_scc (stderr, mixer));
  
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
	  set_scev (loop_nest->num, var_to_analyze, chrec_top);
	}
      else
	{
	  /* The variable is a constant in this loop, analyze its
	     evolution in the inner loops.  */
	  if (loop_nest->inner)
	    analyze_evolution_scc_rec 
	      (var_to_analyze, loop_nest->inner);
	}
    }
}

/* Helper function.  */

static void
analyze_evolution_scc_rec (tree var_to_analyze, 
			   struct loop *loop_nest)
{
  tree loop_phi_node = get_loop_phi_node_for_variable 
    (loop_nest, var_to_analyze);
  
  if (loop_phi_node)
    {
      /* FIXME analysis of mixers: For the moment, set the evolution
	 in this loop to chrec_top.  There is no need to analyze the
	 initial condition for the moment.
	 
	 analyze_initial_condition (loop_nest, loop_phi_node);
	 set_scev (loop_nest->num, var_to_analyze, 
	 reset_evolution_in_loop (loop_nest->num, 
	 get_scev (0, var_to_analyze),
	 chrec_top));
      */
      set_scev (loop_nest->num, var_to_analyze, chrec_top);
    }
  else if (loop_nest->inner)
    analyze_evolution_scc_rec (var_to_analyze, loop_nest->inner);
  
  if (loop_nest->next)
    analyze_evolution_scc_rec (var_to_analyze, loop_nest->next);
}

/* Given an SSA_NAME, follow the edge to its definition, and try to
   determine the initial condition for the variable knowing that we
   search this information in the outermost loop, at depth zero.  */

static void 
analyze_initial_condition_depth_zero (struct loop *loop_nest, 
				      tree ssa_name_to_analyze)
{
  tree upper_chain, res;
  
#if defined ENABLE_CHECKING
  if (ssa_name_to_analyze == NULL_TREE)
    abort ();
#endif
  
  DBG_S (fprintf (stderr, "(analyze_initial_condition_depth_zero \n");
	 fprintf (stderr, "  loop_nest.depth = %d \n", loop_nest->depth);
	 fprintf (stderr, "  ssa_name_to_analyze = ");
	 debug_generic_expr (ssa_name_to_analyze));
  
  upper_chain = SSA_NAME_DEF_STMT (ssa_name_to_analyze);
  
  switch (TREE_CODE (upper_chain))
    {
    case ASM_EXPR:
    case NOP_EXPR:
      DBG_S (fprintf (stderr, ")\n"));
      return;
      
    case PHI_NODE:
      merge_branches_of_condition_phi_node (upper_chain);
      DBG_S (fprintf (stderr, ")\n"));
      return;
      
    default:
      break;
    }
  
  res = iccp_determine_evolution_function (loop_nest, upper_chain);
  
  set_scev_inner_value (0, ssa_name_to_analyze, 
			initial_condition (res));
  set_scev (0, SSA_NAME_VAR (ssa_name_to_analyze), res);
  
  DBG_S (fprintf (stderr, ")\n"));
}


/* Given a loop phi-node, this function determines the initial
   conditions of the variable on entry of the loop.  This function is
   just a wrapper around the ICCP, and is specific to the case of loop
   phi-nodes, where we have to determine the edge going out of the
   current loop.  */

static void 
analyze_initial_condition (struct loop *loop_nest, 
			   tree loop_phi_node)
{
  unsigned aphi_num, aphi_depth;
  int i;
  tree res = chrec_not_analyzed_yet;
  tree original_chrec = chrec_not_analyzed_yet;
  varray_type branch_chrecs;
  
#if defined ENABLE_CHECKING
  if (loop_phi_node == NULL_TREE)
    abort ();
#endif
  
  DBG_S (fprintf (stderr, "(analyze_initial_condition \n");
	 fprintf (stderr, "  loop_phi_node = \n");
	 debug_generic_expr (loop_phi_node));

  VARRAY_TREE_INIT (branch_chrecs, 2, "branch_chrecs");
  original_chrec = get_scev (0, SSA_NAME_VAR (PHI_RESULT (loop_phi_node)));
  
  aphi_num = bb_for_stmt (loop_phi_node)->loop_father->num;
  aphi_depth = bb_for_stmt (loop_phi_node)->loop_father->depth;
  
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
	  
	  /* If the phi node has a NOP_EXPR or a NULL_TREE argument,
	     then this argument is the initial condition, and thus the
	     initial condition is not known.  No precise information
	     can be extracted from the program: the initial condition
	     is set by default at [-oo, +oo].  */
	  if (upper_chain == NULL_TREE 
	      || TREE_CODE (upper_chain) == NOP_EXPR)
	    branch_effect = chrec_top;
	  
	  else 
	    {
	      unsigned upper_chain_depth = bb_for_stmt 
		(upper_chain)->loop_father->depth;
	      
	      /* UPPER_CHAIN is a definition outside the current loop
		 nest: determine the initial conditions.  */
	      if (upper_chain_depth < aphi_depth)
		{
		  /* KEEP_IT_SYMBOLIC.  When the definition contains
		     VUSE_OPS, keep the branch's effect under a
		     symbolic expression.  */
		  if (VUSE_OPS (stmt_ann (upper_chain)))
		    branch_effect = branch;
		  
		  else
		    branch_effect = iccp_determine_evolution_function 
		      (bb_for_stmt (upper_chain)->loop_father, upper_chain);
		}
	      
	      else
		continue;
	    }
	  break;
	  
	default:
	  branch_effect = get_scev (loop_nest->num, branch);
	  break;
	}
      
      VARRAY_PUSH_TREE (branch_chrecs, branch_effect);
    }
  
  if (VARRAY_ACTIVE_SIZE (branch_chrecs) == 1)
    res = VARRAY_TREE (branch_chrecs, 0);
  else
    res = merge_evolutions (original_chrec, branch_chrecs);
  varray_clear (branch_chrecs);
  
  set_scev_inner_value (aphi_num, PHI_RESULT (loop_phi_node), 
			initial_condition (res));
  set_scev (0, SSA_NAME_VAR (PHI_RESULT (loop_phi_node)), res);
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* Given a loop phi-node LOOP_PHI_NODE for a variable VAR, and the
   outer loop phi-node HALTING_PHI, this function determines the
   evolution function for VAR in the current loop.  */

static void
analyze_evolution_in_loop (tree loop_phi_node, 
			   unsigned halting_num)
{
  unsigned aphi_depth;
  int i;
  
#if defined ENABLE_CHECKING
  if (loop_phi_node == NULL_TREE)
    abort ();
#endif
  
  DBG_S (fprintf (stderr, "(analyze_evolution_in_loop \n");
	 fprintf (stderr, "  loop_phi_node = \n");
	 debug_generic_expr (loop_phi_node));
  
  aphi_depth = bb_for_stmt (loop_phi_node)->loop_father->depth;
  
  for (i = 0; i < PHI_NUM_ARGS (loop_phi_node); i++)
    {
      tree arg = PHI_ARG_DEF (loop_phi_node, i);
      
      if (TREE_CODE (arg) == SSA_NAME)
	{
	  tree inner_chain = SSA_NAME_DEF_STMT (arg);
	  
	  /* Select the edges that enters the loop body
	     (inner_chain).  */
	  if (inner_chain != NULL_TREE 
	      && TREE_CODE (inner_chain) != NOP_EXPR
	      && (((unsigned) bb_for_stmt (inner_chain)->loop_father->depth) 
		  >= aphi_depth))
	    {
	      tree nb_iter;
	      unsigned loop_phi_node_num;
	      struct loop *loop_phi_node_father;
	      
	      monev_follow_ssa_edge (inner_chain, loop_phi_node);
	      
	      loop_phi_node_father = bb_for_stmt (loop_phi_node)->loop_father;
	      loop_phi_node_num = loop_phi_node_father->num;
	      
	      /* If we have determined that the branch has an undefinite evolution, 
		 then reset the evolution of the main variable consequently.  */
	      if (get_scev (loop_phi_node_num, arg) == chrec_top)
		{
		  tree ssnv = SSA_NAME_VAR (PHI_RESULT (loop_phi_node));
		  set_scev (0, ssnv, 
			    reset_evolution_in_loop (loop_phi_node_num, 
						     get_scev (0, ssnv),
						     chrec_top));
		}
	      
	      nb_iter = number_of_iterations_in_loop (loop_phi_node_father);
	      
	      if (nb_iter == chrec_top)
		{
		  /* If the number of iterations is not known, set the
		     evolution to chrec_top.  As an example, consider
		     the following loop:
		     
		     | i = 5
		     | loop 
		     |   i = i + 1
		     |   loop k = 1, Unknown_N
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
		    {
		      tree ssnv = SSA_NAME_VAR (PHI_RESULT (loop_phi_node));
		      set_scev (0, ssnv, 
				reset_evolution_in_loop (halting_num, 
							 get_scev (0, ssnv),
							 chrec_top));
		    }
		}
	      else
		{
		  tree overall_effect;
		  tree new_evol, exit_value;
		  tree evolution_fn;
		  
		  /* An example: given the evolution 

		     "{{[22,22], +, [4,4]}_1, +, [1,3]}_2", 

		     and the fact that the loop 2 runs exactly 6
		     times, the overall effect is obtained by
		     evaluating:
		     
		     "({{[22,22], +, [4,4]}_1, +, [1,3]}_2 - initial_conditions) (2, [6,6])"
		     "({{[22,22], +, [4,4]}_1, +, [1,3]}_2 - [22,22]) (2, [6,6])"
		     "{{[0,0], +, [4,4]}_1, +, [1,3]}_2 (2, [6,6])"
		     "[6,18]".
		     
		     That means that the effect of the inner loop 2 in
		     the outer loop 1 is that of adding an integer
		     between 6 and 18 every time the loop 1 is
		     executed.  The evolution function is thus:
		     
		     "{{[22,22], +, [10,22]}_1, +, [1,3]}_2".
		  */
		  
		  evolution_fn = get_scev 
		    (0, SSA_NAME_VAR (PHI_RESULT (loop_phi_node)));
		  
		  overall_effect = chrec_apply
		    (loop_phi_node_num,
		     update_initial_condition_to_origin
		     (evolution_function_in_loop_num (evolution_fn, 
						      loop_phi_node_num)),
		     nb_iter);
		  
		  /* On exit of the loop, the value of the variable is
		     equal to the sum of its initial condition with
		     the loop's overall effect.  */
		  exit_value = chrec_fold_plus 
		    (get_scev (loop_phi_node_num, PHI_RESULT (loop_phi_node)), 
		     overall_effect);
		  
		  /* A loop phi node has two values: 
		     
		     - one exposed to the statements in the inner
		     loop,
		     
		     - the other that is seen by the statements after
		     the loop.
		     
		     This updates the exit value of the loop's phi node.  */
		  set_scev_outer_value (PHI_RESULT (loop_phi_node), 
					exit_value);
		  
		  /* We're not interested in the evolution in the
		     outermost scope: loop 0 represents the analyzed
		     function.  Maybe in an interprocedural analysis
		     this information could be useful, but for
		     interprocedural analysis we would need another
		     type of loop indexing.  */
		  if (halting_num != 0)
		    {
		      if (TREE_CODE (overall_effect) == EXPONENTIAL_CHREC)
			/* testsuite/.../ssa-chrec-41.c.  */
			new_evol = multiply_evolution
			  (loop_phi_node_father->outer->num, 
			   get_scev (loop_phi_node_num, 
				     SSA_NAME_VAR 
				     (PHI_RESULT (loop_phi_node))), 
			   overall_effect);
		      else 
			new_evol = add_to_evolution 
			  (loop_phi_node_father->outer->num, 
			   get_scev (loop_phi_node_num, 
				     SSA_NAME_VAR 
				     (PHI_RESULT (loop_phi_node))), 
			   overall_effect);
		      
		      set_scev (0, SSA_NAME_VAR (PHI_RESULT (loop_phi_node)),
				new_evol);
		    }
		}
	    }
	}
    }
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* This recursive function follows an SSA edge from a loop phi node to
   itself, constructing a path that is analyzed on the return
   walk.  */

static void
monev_follow_ssa_edge (tree rdef, 
		       tree halting_phi)
{
  unsigned rdef_num, rdef_depth;
  unsigned halting_num, halting_depth;
  
  if (rdef == NULL_TREE 
      || TREE_CODE (rdef) == NOP_EXPR
      /* End the recursion when the halting_phi node is reached.  */
      || rdef == halting_phi)
    return;
  
  /* DBG_S (debug_generic_expr (rdef)); */
  
  rdef_num = bb_for_stmt (rdef)->loop_father->num;
  rdef_depth = bb_for_stmt (rdef)->loop_father->depth;
  halting_num = bb_for_stmt (halting_phi)->loop_father->num;
  halting_depth = bb_for_stmt (halting_phi)->loop_father->depth;
  
  /* We have to deal with only three cases in an update expression: 
     - an inner loop PHI_NODE, 
     - a conditional PHI_NODE, 
     - a MODIFY_EXPR.
  */
  switch (TREE_CODE (rdef))
    {
    case PHI_NODE:

      /* Avoid recursion when the phi-nodes contain SCCs.  For
	 example, in gcc/gcc/calls.c:expand_call, we have the
	 following phi nodes:
    
	 "normal_call_insns_8 = PHI <0B(380), normal_call_insns_9(782)>;
	 normal_call_insns_9 = PHI <normal_call_insns_8(773), T.1529_2087(777), 
	 T.1529_2087(778)>;"
      */
      if (PHI_MARKED (rdef) == 1)
	return;
      
      else
	PHI_MARKED (rdef) = 1;

      /* Determine whether the PHI_NODE is a loop phi or a conditional
	 phi.  */
      if (rdef_depth > halting_depth)
	{
	  /* This is a LOOP-phi-node: the rdef is in an inner loop.
	     
	     First, follow the upper chain for determining the initial
	     conditions from the analysis of the outer loop.  Then
	     analyse the evolution in the inner loop.  */
	  
	  int i;
	  
	  for (i = 0; i < PHI_NUM_ARGS (rdef); i++)
	    {
	      tree arg = PHI_ARG_DEF (rdef, i);
	      tree upper_branch;
	      unsigned upper_depth, upper_num;
	      
	      if (TREE_CODE (arg) == INTEGER_CST)
		{
		  set_scev (rdef_num, PHI_RESULT (rdef), arg);
		  break;
		}
	      
	      upper_branch = SSA_NAME_DEF_STMT (arg);
	      upper_depth = bb_for_stmt (upper_branch)->loop_father->depth;
	      upper_num = bb_for_stmt (upper_branch)->loop_father->num;
	      
	      if (upper_depth < rdef_depth)
		{
		  /* This is an out of analyzed-loop edge.  */
		  tree res;
		  
		  switch (TREE_CODE (upper_branch))
		    {
		    case MODIFY_EXPR:
		      monev_follow_ssa_edge (upper_branch, halting_phi);
		      res = evolution_at_version 
			(upper_num, TREE_OPERAND (upper_branch, 0));
		      break;
		      
		    case PHI_NODE:
		      res = evolution_at_version 
			(upper_num, PHI_RESULT (upper_branch));
		      break;
		      
		    default:
		      debug_tree (rdef);
		      res = chrec_top;
		    }
		  
		  set_scev (rdef_num, PHI_RESULT (rdef), 
			    initial_condition (res));
		}
	    }
	  
	  analyze_evolution_in_loop (rdef, halting_num);
	}
      
      else
	/* RDEF is a CONDITION-phi-node.
	   
	   Follow the branches, and record their evolutions.  Finally,
	   merge the collected information and set the approximation
	   to the main variable.
	   
	   FIXME: It is possible to improve the analysis speed by not
	   following the whole chain to the loop-phi-node.  A better
	   solution is to analyze the evolution only on the portions
	   of paths that differ, ie. do not analyze n times outside
	   the if-body.  */
	merge_branches_of_condition_phi_node_in_loop (rdef, halting_phi);
      
      /* Reset the marker after the analysis.  */
      PHI_MARKED (rdef) = 0;
      break;
      
    case MODIFY_EXPR:
      {
	/* RDEF is an assignment under the form: "OPND0 = OPND1".  */
	tree opnd0, opnd1, var;
	
	opnd0 = TREE_OPERAND (rdef, 0);
	opnd1 = TREE_OPERAND (rdef, 1);
	
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
	      /* FIXME wrap_around:  
		 This assignment is under the form "a_1 = 7".  */
	      set_scev (rdef_num, opnd0, chrec_top);
	      set_scev (0, var, chrec_top);
	      break;
	    }
	    
	  case SSA_NAME:
	    if (var == SSA_NAME_VAR (opnd1))
	      {
		/* FIXME does this happen?
		   This is a strange case: "a_1 = a_2".  */
		abort ();
	      }
	    else
	      {
		/* FIXME wrap_around:
		   This assignment is under the form: "a_1 = b_2".  */
		set_scev (rdef_num, opnd0, chrec_top);
		set_scev (0, var, chrec_top);
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
		  if (SSA_NAME_VAR (opnd10) == var)
		    {
		      /* The assignment is under the form: 
			 "a_1 = a_2 + ...".  */
		      tree upper_chain = SSA_NAME_DEF_STMT (opnd10);
		      tree chrec10, chrec11, chrec_before;
		      tree new_init_cond, new_evol;
		      tree to_be_added;
		      
		      /* Recursively construct the SSA path.  */
		      monev_follow_ssa_edge (upper_chain, halting_phi);
		      
		      /* Analyze the assignment on the return walk.  */

		      chrec_before = get_scev (0, var);
		      if (chrec_should_remain_symbolic (chrec_before))
			/* KEEP_IT_SYMBOLIC.  */
			chrec_before = opnd10;
		      
		      chrec10 = evolution_at_version (rdef_num, opnd10);
		      chrec11 = evolution_at_version (rdef_num, opnd11);
		      if (chrec_should_remain_symbolic (chrec11))
			/* KEEP_IT_SYMBOLIC.  
			   Don't propagate unknown values, but instead, 
			   keep the evolution function under a symbolic 
			   form.  Example: testsuite/.../ssa-chrec-17.c.  */
			to_be_added = opnd11;
		      else
			to_be_added = chrec11;
		      
		      /* Record the initial condition.  */
		      new_init_cond = chrec_fold_plus 
			(initial_condition (chrec10), 
			 initial_condition (to_be_added));
		      set_scev (rdef_num, opnd0, new_init_cond);
		      
		      /* Update the evolution of the main variable.  */
		      new_evol = add_to_evolution (rdef_num, chrec_before, 
						   to_be_added);
		      set_scev (0, var, new_evol);
		    }
		  else
		    {
		      /* The assignment is under the form: 
			 "a_1 = b_2 + ...".  */
		      if (TREE_CODE (opnd11) == SSA_NAME
			  && SSA_NAME_VAR (opnd11) == var)
			{
			  /* The assignment is under the form: 
			     "a_1 = b_2 + a_3".  */
			  tree upper_chain = SSA_NAME_DEF_STMT (opnd11);
			  tree chrec10, chrec11, chrec_before;
			  tree new_init_cond, new_evol;
			  tree to_be_added;
			  
			  /* Recursively construct the SSA path.  */
			  monev_follow_ssa_edge (upper_chain, halting_phi);
			  
			  /* Analyze the assignment on the return walk.  */
			  
			  chrec_before = get_scev (0, var);
			  if (chrec_should_remain_symbolic (chrec_before))
			    /* KEEP_IT_SYMBOLIC.  */
			    chrec_before = opnd11;
			  
			  chrec10 = evolution_at_version (rdef_num, opnd10);
			  chrec11 = evolution_at_version (rdef_num, opnd11);
			  if (chrec_should_remain_symbolic (chrec10))
			    /* KEEP_IT_SYMBOLIC.  */
			    to_be_added = opnd10;
			  else
			    to_be_added = chrec10;
			  
			  /* Record the initial condition.  */
			  new_init_cond = chrec_fold_plus 
			    (initial_condition (chrec11), 
			     initial_condition (to_be_added));
			  set_scev (rdef_num, opnd0, new_init_cond);
			  
			  /* Update the evolution.  */
			  new_evol = add_to_evolution (rdef_num, chrec_before, 
						       to_be_added);
			  set_scev (0, var, new_evol);
			}
		      else
			{
			  /* FIXME wrap_around:
			     The assignment is under the form: 
			     "a_1 = b_2 + 5".  */
			  set_scev (rdef_num, opnd0, chrec_top);
			  set_scev (0, var, chrec_top);
			}
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
		  else
		    {
		      /* FIXME wrap_around:
			 The assignment is under the form: "a_1 = 5 + b_2.  */
		      set_scev (rdef_num, opnd0, chrec_top);
		      set_scev (0, var, chrec_top);
		    }
		}
	      else
		{
		  /* The arguments do not contain SSA_NAMEs.  For
		     example, this could be the computation of a
		     memory address as in:
		     
		     java/jcf-io.c: "buf_ptr_7 = &buf + 68B".  */
		  set_scev (rdef_num, opnd0, opnd1);
		  set_scev (0, var, opnd1);
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
		  if (SSA_NAME_VAR (opnd10) == var)
		    {
		      /* The assignment is under the form: 
			 "a_1 = a_2 - ...".  */
		      tree upper_chain = SSA_NAME_DEF_STMT (opnd10);
		      tree chrec10, chrec11, chrec_before;
		      tree new_init_cond, new_evol;
		      tree to_be_added;
		      
		      /* Recursively construct the SSA path.  */
		      monev_follow_ssa_edge (upper_chain, halting_phi);
		      
		      /* Analyze the assignment on the return walk.  */
		      
		      chrec_before = get_scev (0, var);
		      if (chrec_should_remain_symbolic (chrec_before))
			/* KEEP_IT_SYMBOLIC.  */
			chrec_before = opnd10;
		      
		      chrec10 = evolution_at_version (rdef_num, opnd10);
		      chrec11 = evolution_at_version (rdef_num, opnd11);
		      if (chrec_should_remain_symbolic (chrec11))
			/* KEEP_IT_SYMBOLIC.  */
			to_be_added = chrec_fold_multiply (opnd11,
							   integer_minus_one_node);
		      else
			to_be_added = chrec_fold_multiply (chrec11,
							   integer_minus_one_node);
		      
		      /* Record the initial condition.  */
		      new_init_cond = chrec_fold_plus 
			(initial_condition (chrec10), 
			 initial_condition (to_be_added));
		      set_scev (rdef_num, opnd0, new_init_cond);
		      
		      /* Update the evolution.  */
		      new_evol = add_to_evolution (rdef_num, chrec_before, 
						   to_be_added);
		      set_scev (0, var, new_evol);
		    }
		  else
		    {
		      /* The assignment is under the form: 
			 "a_1 = b_2 - ...".  */
		      if (TREE_CODE (opnd11) == SSA_NAME
			  && SSA_NAME_VAR (opnd11) == var)
			{
			  /* The assignment is under the form: 
			     "a_1 = b_2 - a_3".  
			     FIXME arithmetic flip-flop.  */
			  set_scev (rdef_num, opnd0, chrec_top);
			  set_scev (0, var, chrec_top);
			}
		      else
			{
			  /* FIXME wrap_around:
			     The assignment is under the form: 
			     "a_1 = b_2 - 5".  */			     
			  set_scev (rdef_num, opnd0, chrec_top);
			  set_scev (0, var, chrec_top);
			}
		    }
		}
	      else if (TREE_CODE (opnd11) == SSA_NAME)
		{
		  if (SSA_NAME_VAR (opnd11) == var)
		    {
		      /* The assignment is under the form: 
			 "a_1 = 5 - a_2".  
			 FIXME arithmetic flip-flop.  */
		      set_scev (rdef_num, opnd0, chrec_top);
		      set_scev (0, var, chrec_top);
		    }
		  else
		    {
		      /* FIXME wrap_around:
			 The assignment is under the form: "a_1 = 5 - b_2.  */
		      set_scev (rdef_num, opnd0, chrec_top);
		      set_scev (0, var, chrec_top);
		    }
		}
	      else
		{
		  /* The arguments do not contain SSA_NAMEs.  */
		  set_scev (rdef_num, opnd0, opnd1);
		  set_scev (0, var, opnd1);
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
		  if (SSA_NAME_VAR (opnd10) == var)
		    {
		      /* The assignment is under the form: 
			 "a_1 = a_2 * ...".  */
		      tree upper_chain = SSA_NAME_DEF_STMT (opnd10);
		      tree chrec10, chrec11, chrec_before;
		      tree new_init_cond, new_evol;
		      tree to_be_multiplied;
		      
		      /* Recursively construct the SSA path.  */
		      monev_follow_ssa_edge (upper_chain, halting_phi);
		      /* Then, analyze the assignment on the return walk.  */
		      
		      chrec_before = get_scev (0, var);
		      if (chrec_should_remain_symbolic (chrec_before))
			/* KEEP_IT_SYMBOLIC.  */
			chrec_before = opnd10;
		      
		      chrec10 = evolution_at_version (rdef_num, opnd10);
		      chrec11 = evolution_at_version (rdef_num, opnd11);
		      if (chrec_should_remain_symbolic (chrec11))
			/* KEEP_IT_SYMBOLIC.  */
			to_be_multiplied = opnd11;
		      else
			to_be_multiplied = chrec11;
		      
		      /* Record the initial condition.  */
		      new_init_cond = chrec_fold_multiply 
			(initial_condition (chrec10), 
			 initial_condition (to_be_multiplied));
		      set_scev (rdef_num, opnd0, new_init_cond);
		      
		      /* Update the evolution of the main variable.  */
		      new_evol = multiply_evolution (rdef_num, chrec_before, 
						     to_be_multiplied);
		      set_scev (0, var, new_evol);
		    }
		  else
		    {
		      /* The assignment is under the form: 
			 "a_1 = b_2 * ...".  */
		      if (TREE_CODE (opnd11) == SSA_NAME
			  && SSA_NAME_VAR (opnd11) == var)
			{
			  /* The assignment is under the form: 
			     "a_1 = b_2 * a_3".  */
			  tree upper_chain = SSA_NAME_DEF_STMT (opnd11);
			  tree chrec10, chrec11, chrec_before;
			  tree new_init_cond, new_evol;
			  tree to_be_multiplied;
			  
			  /* Recursively construct the SSA path.  */
			  monev_follow_ssa_edge (upper_chain, halting_phi);
			  /* Then, analyze the assignment on the return walk.  */

			  chrec_before = get_scev (0, var);
			  if (chrec_should_remain_symbolic (chrec_before))
			    /* KEEP_IT_SYMBOLIC.  */
			    chrec_before = opnd11;
			  
			  chrec10 = evolution_at_version (rdef_num, opnd10);
			  chrec11 = evolution_at_version (rdef_num, opnd11);
			  if (chrec_should_remain_symbolic (chrec10))
			    /* KEEP_IT_SYMBOLIC.  */
			    to_be_multiplied = opnd10;
			  else
			    to_be_multiplied = chrec10;
			  
			  /* Record the initial condition.  */
			  new_init_cond = chrec_fold_multiply 
			    (initial_condition (chrec11), 
			     initial_condition (to_be_multiplied));
			  set_scev (rdef_num, opnd0, new_init_cond);
			  
			  /* Update the evolution.  */
			  new_evol = multiply_evolution (rdef_num, chrec_before, 
							 to_be_multiplied);
			  set_scev (0, var, new_evol);
			}
		      else
			{
			  /* FIXME wrap_around:
			     The assignment is under the form: 
			     "a_1 = b_2 * 5".  */
			  set_scev (rdef_num, opnd0, chrec_top);
			  set_scev (0, var, chrec_top);
			}
		    }
		}
	      else if (TREE_CODE (opnd11) == SSA_NAME)
		{
		  if (SSA_NAME_VAR (opnd11) == var)
		    {
		      /* FIXME does this happen? 
			 The assignment is under the form: 
			 "a_1 = 5 * a_2".  
			 From what I've seen, this case is turned into:
			 "a_1 = a_2 * 5".  */
		      abort ();
		    }
		  else
		    {
		      /* FIXME wrap_around:
			 The assignment is under the form: "a_1 = 5 * b_2.  */
		      set_scev (rdef_num, opnd0, chrec_top);
		      set_scev (0, var, chrec_top);
		    }
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
	    set_scev (rdef_num, opnd0, chrec_top);
	    set_scev (0, var, chrec_top);
	    break;
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

/* In the case where there is no inner loops, the variable can be a
   symbolic parameter.  In this function we try to determine the
   evolution function of the variable, and if it cannot be determined,
   set its evolution to the peculiar symbol
   CHREC_SYMBOLIC_PARAMETER.  */

static void
determine_whether_ssa_name_is_symbolic_parameter (tree ssa_name_to_analyze, 
						  struct loop *loop_nest)
{
  tree var_to_analyze = SSA_NAME_VAR (ssa_name_to_analyze);
  tree def_stmt = SSA_NAME_DEF_STMT (ssa_name_to_analyze);
  
  switch (TREE_CODE (def_stmt))
    {
    case MODIFY_EXPR:
      {
	varray_type already_seen_variables;
	tree res;
	
	VARRAY_TREE_INIT (already_seen_variables, 37, "already_seen_variables");
	res = iccp_follow_ssa_edge_and_compute_scev 
	  (loop_nest->num, ssa_name_to_analyze, already_seen_variables);
	varray_clear (already_seen_variables);
	
	/* KEEP_IT_SYMBOLIC.  */
	if (res == chrec_top)
	  set_scev (loop_nest->num, ssa_name_to_analyze, 
		    TREE_OPERAND (def_stmt, 0));
	
	else
	  set_scev (loop_nest->num, ssa_name_to_analyze, res);
	
	break;
      }
      
    case NOP_EXPR:
    default:
      set_scev (loop_nest->num, var_to_analyze, chrec_symbolic_parameter);
      break;
    }
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
get_loop_phi_node_for_variable (struct loop *loop, tree v)
{
  tree phi;
  basic_block bb = loop->header;
  
  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
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

static struct monev_info_str *
find_var_monev_info (tree var)
{
  unsigned int i;
  struct monev_info_str *res;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (monev_info); i++)
    {
      res = VARRAY_GENERIC_PTR (monev_info, i);
      if (MI_VAR (res) == var)
	return res;
    }
  
  /* The variable is not in the table, create a new entry for it.  */
  res = new_monev_info_str (var);
  VARRAY_PUSH_GENERIC_PTR (monev_info, res);
  
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
set_scev (unsigned loop_num, 
	  tree var, 
	  tree chrec)
{
  struct monev_info_str *var_info;
  
  var_info = find_var_monev_info (var);
  chrec = set_scev_keep_symbolic (var, chrec);
  
  DBG_S (fprintf (stderr, "(set_scev \n");
	 fprintf (stderr, "  sc = ");
	 debug_generic_expr (var);
	 fprintf (stderr, "  scev = ");
	 debug_generic_expr (chrec);
	 fprintf (stderr, ")\n"));
  
  MI_LOOP_NUM (var_info) = loop_num;
  MI_INNER_LOOPS_CHREC (var_info) = chrec;
  MI_OUTER_LOOPS_CHREC (var_info) = chrec;
}

/* Associate CHREC to the inner loops for VAR.  */

static void
set_scev_inner_value (unsigned loop_num, 
		      tree var, 
		      tree chrec)
{
  struct monev_info_str *var_info;
  
  var_info = find_var_monev_info (var);
  chrec = set_scev_keep_symbolic (var, chrec);
  
  DBG_S (fprintf (stderr, "(set_scev_inner_value \n");
	 fprintf (stderr, "  sc = ");
	 debug_generic_expr (var);
	 fprintf (stderr, "  scev = ");
	 debug_generic_expr (chrec);
	 fprintf (stderr, ")\n"));
  
  MI_LOOP_NUM (var_info) = loop_num;
  MI_INNER_LOOPS_CHREC (var_info) = chrec;
}

/* Associate CHREC to the outer loops for VAR.  */

static void
set_scev_outer_value (tree var, 
		      tree chrec)
{
  struct monev_info_str *var_info;
  
  var_info = find_var_monev_info (var);
  chrec = set_scev_keep_symbolic (var, chrec);
  
  DBG_S (fprintf (stderr, "(set_scev_outer_value \n");
	 fprintf (stderr, "  sc = ");
	 debug_generic_expr (var);
	 fprintf (stderr, "  scev = ");
	 debug_generic_expr (chrec);
	 fprintf (stderr, ")\n"));
  
  MI_OUTER_LOOPS_CHREC (var_info) = chrec;
}

/* Retrieve the chrec associated to EXPR.  */

static tree
get_scev (unsigned loop_num, tree var)
{
  struct monev_info_str *var_info;
  tree res = NULL_TREE;
  
  DBG_S (fprintf (stderr, "(get_scev \n"));
  
  switch (TREE_CODE (var))
    {
    case REAL_CST:
    case INTEGER_CST:
      res = var;
      break;
      
    case SSA_NAME:
    case VAR_DECL:
    case PARM_DECL:
      var_info = find_var_monev_info (var);
      if (loop_is_included_in (loop_num, MI_LOOP_NUM (var_info)))
	res = MI_INNER_LOOPS_CHREC (var_info);
      else
	res = MI_OUTER_LOOPS_CHREC (var_info);
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
  
  DBG_S (fprintf (stderr, "  sc = ");
	 debug_generic_expr (var);
	 fprintf (stderr, "  scev = ");
	 debug_generic_expr (res);

	 /*  
	     if (res == chrec_top) {
	     fprintf (stderr, "TTT: \n");
	     debug_tree (var);
	     }
	 */

	 fprintf (stderr, ")\n"));
  return res;
}

/* Determines whether loop A is contained in loop B.  
   
   FIXME: Maybe it would be better to record this information as a bit
   matrix in the loops structure.  */

static bool
loop_is_included_in (unsigned a, unsigned b)
{
  struct loop *loop_b;
  
  if (a == b)
    return true;
  
  /* That's a property of the loops->parray (see the comment in the
     loops structure).  */
  if (a < b)
    return false;
  
  loop_b = loop_from_num (monev_loops, b);
  
  if (loop_b->inner
      && loop_is_included_in_rec (a, loop_b->inner))
    return true;
  
  return false;
}

/* Recursively determine whether A is contained in LOOP_B.  */

static bool
loop_is_included_in_rec (unsigned a, struct loop *loop_b)
{
  if (loop_b->num == (int) a)
    return true;
  
  if (loop_b->inner
      && loop_is_included_in_rec (a, loop_b->inner))
    return true;
  
  if (loop_b->next
      && loop_is_included_in_rec (a, loop_b->next))
    return true;
  
  return false;
}

/* Merge the evolution functions from BRANCH_CHRECS with the
   ORIGINAL_CHREC.  */

static tree 
merge_evolutions (tree original_chrec, varray_type branch_chrecs)
{
  unsigned int i;
  tree res;
  varray_type diff_chrecs;
  
  VARRAY_TREE_INIT (diff_chrecs, 2, "diff_chrecs");
  
  DBG_S (fprintf (stderr, "(merge_evolutions \n"));

  if (original_chrec == chrec_not_analyzed_yet)
    original_chrec = integer_zero_node;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (branch_chrecs); i++)
    {
      tree diff_chrec;
      tree branch_chrec = VARRAY_TREE (branch_chrecs, i);
      
      diff_chrec = chrec_fold_minus (branch_chrec, original_chrec);
      DBG_S (fprintf (stderr, "  branch = ");
	     debug_generic_expr (diff_chrec));
      VARRAY_PUSH_TREE (diff_chrecs, diff_chrec);
    }
  
  res = VARRAY_TREE (diff_chrecs, 0);
  if (res == NULL_TREE)
    res = chrec_top;
  
  for (i = 1; i < VARRAY_ACTIVE_SIZE (diff_chrecs); i++)
    res = chrec_merge (res, VARRAY_TREE (diff_chrecs, i));
  
  DBG_S (fprintf (stderr, "  merged_branches = ");
	 debug_generic_expr (res);
	 fprintf (stderr, ")\n"));
  
  varray_clear (diff_chrecs);
  
  return chrec_fold_plus (original_chrec, res);
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
	  monev_follow_ssa_edge (SSA_NAME_DEF_STMT (branch), 
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
  set_scev (bb_for_stmt (condition_phi)->loop_father->num, 
	    PHI_RESULT (condition_phi), 
	    chrec_eval_next_init_cond (res));
  
  varray_clear (branch_chrecs);
}

/* This function merges the branches of a condition phi node.  The phi
   node is not in a loop: we're analyzing the initial conditions.  */

static void
merge_branches_of_condition_phi_node (tree condition_phi)
{
  int i;
  tree original_chrec, res;
  varray_type branch_chrecs;
  
  VARRAY_TREE_INIT (branch_chrecs, 2, "branch_chrecs");
  original_chrec = get_scev (0, SSA_NAME_VAR (PHI_RESULT (condition_phi)));
  
  for (i = 0; i < PHI_NUM_ARGS (condition_phi); i++)
    {
      tree branch = PHI_ARG_DEF (condition_phi, i);
      
      switch (TREE_CODE (branch))
	{
	case INTEGER_CST:
	  res = get_scev (0, branch);
	  break;
	  
	case SSA_NAME:
	  res = iccp_determine_evolution_function 
	    (bb_for_stmt (condition_phi)->loop_father, branch);
	  break;
	  
	default:
	  res = chrec_top;
	  break;
	}
      
      VARRAY_PUSH_TREE (branch_chrecs, res);
    }
  
  res = merge_evolutions (original_chrec, branch_chrecs);
  
  set_scev (0, SSA_NAME_VAR (PHI_RESULT (condition_phi)), res);
  set_scev (0, PHI_RESULT (condition_phi), initial_condition (res));
  
  varray_clear (branch_chrecs);
}

/* Merge the information of the evolution of a variable with the
   initial condition at the given point/version.  The initial
   condition is an interval, while the evolution of the variable is
   the evolution at the origin.  
   
   When the evolution of a variable is not analyzed yet, this function
   returns the version itself.  */

static tree 
evolution_at_version (unsigned loop_num, 
		      tree version)
{
  struct monev_info_str *var_info;
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
      var_info = find_var_monev_info (version);
      if (loop_is_included_in (loop_num, MI_LOOP_NUM (var_info)))
	{
	  /* Retrieve the initial condition from the version.  */
	  init_cond = get_scev (loop_num, version);
	  if (init_cond == chrec_top
	      || init_cond == chrec_not_analyzed_yet)
	    /* Instead of propagating chrec_top elements incognito, it
	    is more precise to keep a pointer to the version (from
	    which it is still possible to get_scev and say that it is
	    chrec_top).  */
	    init_cond = version;
	  
	  /* The evolution in a loop is stored as an attribute of the
	     main variable.  */
	  evolution = get_scev (0, SSA_NAME_VAR (version));
	  
	  if (evolution == chrec_top 
	      && init_cond == version)
	    /* The main variable has an unknown evolution, but the
	       current version has not been yet analyzed.  The result
	       is unknown.  */
	    res = chrec_top;
	  else
	    {
	      /* The evolution function for a version is equal to the
		 evolution function in the loop instantiated with the
		 initial condition of the version.  */
	      res = replace_initial_condition (evolution, init_cond);
	      
	      if (symbolic_parameter_expr_p (res))
		/* KEEP_IT_SYMBOLIC.  */
		res = version;
	      else if ((TREE_CODE (res) == POLYNOMIAL_CHREC
			|| TREE_CODE (res) == EXPONENTIAL_CHREC)
		       && CHREC_LEFT (res) == chrec_top
		       && CHREC_RIGHT (res) == chrec_top)
		res = chrec_top;
	    }
	}
      else 
	/* Get the overall effect of the loop on the variable.  At
	   this point get_scev returns a NULL_TREE if the condition
	   after the crossing of a loop has not been determined yet.
	   If the outside visible value is chrec_top, then that means
	   that the analyzer has failed to compute the number of
	   iterations in the loop.  */
	res = get_scev (loop_num, version);
      
      break;
      
    default:
      res = chrec_top;
      break;
    }
  
  DBG_S (fprintf (stderr, "  version = ");
	 debug_generic_expr (version);
	 fprintf (stderr, "  loop_num = %d\n", loop_num);
	 fprintf (stderr, "  scev = ");
	 debug_generic_expr (res);
	 
	 /*
	   if (res == chrec_top) {
	   fprintf (stderr, "YYY: \n");
	   debug_tree (version);
	   }
	 */
	 
	 fprintf (stderr, ")\n"));
  return res;
}

/* Add TO_ADD to the evolution part of CHREC_BEFORE in the dimension
   of LOOP_NUM.  */

static tree 
add_to_evolution (unsigned loop_num, 
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
  
  DBG_S (fprintf (stderr, "(add_to_evolution \n"));
  
  switch (TREE_CODE (to_add))
    {
    case POLYNOMIAL_CHREC:
      if (no_evolution_in_loop_p (chrec_before, loop_num))
	/* testsuite/.../ssa-chrec-38.c.  */
	res = build_polynomial_evolution_in_loop (loop_num, 
						  chrec_before, 
						  to_add);
      else
	/* testsuite/.../ssa-chrec-37.c.  */
	res = add_expr_to_loop_evolution (loop_num, chrec_before, to_add);
      break;
      
    case EXPONENTIAL_CHREC:
      if (no_evolution_in_loop_p (chrec_before, loop_num))
	/* testsuite/.../ssa-chrec-40.c  */
	res = build_polynomial_evolution_in_loop (loop_num, 
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
      if (no_evolution_in_loop_p (chrec_before, loop_num))
	/* testsuite/.../ssa-chrec-39.c.  */
	res = build_polynomial_evolution_in_loop (loop_num, chrec_before, 
						  to_add);
      else
	/* testsuite/.../ssa-chrec-20.c.  */
	res = add_expr_to_loop_evolution (loop_num, chrec_before, to_add);
      break;
    }
  
  DBG_S (fprintf (stderr, "  loop_num = %d\n", loop_num);
	 fprintf (stderr, "  chrec_before = ");
	 debug_generic_expr (chrec_before);
	 fprintf (stderr, "  to_add = ");
	 debug_generic_expr (to_add);
	 fprintf (stderr, "  res = ");
	 debug_generic_expr (res);
	 fprintf (stderr, ")\n"));
  return res;
}

/* Add TO_MULT to the evolution part of CHREC_BEFORE in the dimension
   of LOOP_NUM.  */

static tree 
multiply_evolution (unsigned loop_num, 
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
  
  DBG_S (fprintf (stderr, "(multiply_evolution \n"));

  switch (TREE_CODE (to_mult))
    {
    case EXPONENTIAL_CHREC:
      if (no_evolution_in_loop_p (chrec_before, loop_num))
	/* testsuite/.../ssa-chrec-41.c.  */
	res = build_exponential_evolution_in_loop (loop_num, 
						   chrec_before, 
						   to_mult);
      else
	/* testsuite/.../ssa-chrec-22.c.  */
	res = multiply_by_expr_the_loop_evolution (loop_num, 
						   chrec_before, 
						   to_mult);
      break;
      
    case POLYNOMIAL_CHREC:
      if (no_evolution_in_loop_p (chrec_before, loop_num))
	/* testsuite/.../ssa-chrec-23.c.  */
	res = build_exponential_evolution_in_loop (loop_num, 
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
      if (no_evolution_in_loop_p (chrec_before, loop_num))
	/* testsuite/.../ssa-chrec-22.c.  */
	res = build_exponential_evolution_in_loop (loop_num, 
						   chrec_before, 
						   to_mult);
      else
	/* Example: 
	   LOOP_NUM = 1
	   TO_ADD = "3*N"
	   CHREC_BEFORE = "{{0, +, 1}_1, +, 5}_1"
	   --
	   RES = "{{0, +, 3*N + 1}_1, +, 5}_1".  */
	res = multiply_by_expr_the_loop_evolution (loop_num, 
						   chrec_before, 
						   to_mult);
      break;
    }
  
  DBG_S (fprintf (stderr, "  loop_num = %d\n", loop_num);
	 fprintf (stderr, "  chrec_before = ");
	 debug_generic_expr (chrec_before);
	 fprintf (stderr, "  to_mult = ");
	 debug_generic_expr (to_mult);
	 fprintf (stderr, "  res = ");
	 debug_generic_expr (res);
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
  DBG_S (fprintf (stderr, "  nb_iterations cannot be determined\n");
	 fprintf (stderr, ")\n"));
  return chrec_top;
}

/* Helper function.  */

static inline tree
set_nb_iterations_in_loop (struct loop *loop, 
			   tree res)
{
  DBG_S (fprintf (stderr, "  set_nb_iterations_in_loop = ");
	 debug_generic_expr (res);
	 fprintf (stderr, ")\n"));
  
  loop->nb_iterations = res;
  return res;
}

/* This function tries to safely approximate the number of iterations
   the loop will run.  When this property is not decidable at compile
   time, the result is [-oo, +oo].  Otherwise the result is an
   interval.
   
   Example: Suppose that the loop has an exit condition:
   
   "if (b > 49) goto end_loop;"
   
   and that in a previous analysis we have determined that the
   variable 'b' has an evolution function:
   
   "EF = {[23,23], +, [5,5]}_2".  
   
   When we evaluate the function at the point 5, i.e. the value of the
   variable 'b' after 5 iterations in the loop, we have EF (5) = 48,
   and EF (6) = 53.  In this case the value of 'b' on exit is '53' and
   the loop body has been executed 6 times.
   
   FIXME zero-trip-problem:
   Try to determine an approximation of the number of iterations the
   loop LOOP will run.  The result of the function is an interval.  */

static tree 
number_of_iterations_in_loop (struct loop *loop)
{
  int loop_num = loop->num;
  tree res;
  tree test, opnd0, opnd1;
  tree chrec0, chrec1, chrec_cond;
  
  res = loop->nb_iterations;
  if (res)
    /* This information has already been computed.  */
    return res;
  
  DBG_S (fprintf (stderr, "(number_of_iterations_in_loop \n"));
  test = get_loop_exit_condition (loop);
  
  if (test == NULL_TREE)
    return set_nb_iterations_in_loop (loop, chrec_top);
  else
    switch (TREE_CODE (test))
      {
      case SSA_NAME:
	/* "while (opnd0)", or in other terms, 
	   "exit when (chrec0 == 0)".  */
	opnd0 = test;
	chrec0 = evolution_at_version (loop_num, opnd0);
	
	if (chrec0 == chrec_top
	    || chrec0 == chrec_not_analyzed_yet
	    || chrec0 == chrec_symbolic_parameter)
	  /* KEEP_IT_SYMBOLIC.  */
	  chrec0 = opnd0;
	
	DBG_S (fprintf (stderr, "  loop_num = %d\n", loop_num);
	       fprintf (stderr, "  loop_while_expr_is_true: ");
	       debug_generic_expr (test);
	       fprintf (stderr, "  chrec0 = ");
	       debug_generic_expr (chrec0));
	
	if (chrec_contains_undetermined (chrec0))
	  return cannot_analyze_loop_nb_iterations_yet ();
	else
	  return set_nb_iterations_in_loop 
	    (loop, how_far_from_zero (loop_num, chrec0));
	
      case LT_EXPR:
      case LE_EXPR:
      case GT_EXPR:
      case GE_EXPR:
      case EQ_EXPR:
      case NE_EXPR:
	opnd0 = TREE_OPERAND (test, 0);
	opnd1 = TREE_OPERAND (test, 1);
	chrec0 = evolution_at_version (loop_num, opnd0);
	chrec1 = evolution_at_version (loop_num, opnd1);
	
	if (chrec0 == chrec_top
	    || chrec0 == chrec_not_analyzed_yet
	    || chrec0 == chrec_symbolic_parameter)
	  /* KEEP_IT_SYMBOLIC.  */
	  chrec0 = opnd0;
	
	if (chrec1 == chrec_top
	    || chrec1 == chrec_not_analyzed_yet
	    || chrec1 == chrec_symbolic_parameter)
	  /* KEEP_IT_SYMBOLIC.  */
	  chrec1 = opnd1;
		
	DBG_S (fprintf (stderr, "  loop_num = %d\n", loop_num);
	       fprintf (stderr, "  loop_while_expr_is_true: ");
	       debug_generic_expr (test);
	       fprintf (stderr, "  chrec0 = ");
	       debug_generic_expr (chrec0);
	       fprintf (stderr, "  chrec1 = ");
	       debug_generic_expr (chrec1));
	
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
	      (loop, how_far_from_positive (loop_num, chrec_cond));
	    
	  case LE_EXPR:
	    /* "while (opnd0 <= opnd1)", or, 
	       "exit when (chrec0 - chrec1 > 0)".  */
	    chrec_cond = chrec_fold_minus (chrec0, chrec1);
	    return set_nb_iterations_in_loop 
	      (loop, how_far_from_positive (loop_num, chrec_cond));
	    
	  case GT_EXPR:
	    /* "while (opnd0 > opnd1)", or, 
	       "exit when (chrec1 - chrec0 + 1 > 0)".  */
	    chrec_cond = chrec_fold_plus 
	      (chrec_fold_minus (chrec1, chrec0), integer_one_node);
	    return set_nb_iterations_in_loop 
	      (loop, how_far_from_positive (loop_num, chrec_cond));
	    
	  case GE_EXPR:
	    /* "while (opnd0 >= opnd1)", or, 
	       "exit when (chrec1 - chrec0 > 0)".  */
	    chrec_cond = chrec_fold_minus (chrec1, chrec0);
	    return set_nb_iterations_in_loop 
	      (loop, how_far_from_positive (loop_num, chrec_cond));
	    
	  case EQ_EXPR:
	    /* "while (opnd0 == opnd1)", or,
	       "exit when (chrec0 - chrec1) != 0.  */
	    chrec_cond = chrec_fold_minus (chrec0, chrec1);
	    return set_nb_iterations_in_loop 
	      (loop, how_far_from_non_zero (loop_num, chrec_cond));
	    
	  case NE_EXPR:
	    /* "while (opnd0 != opnd1)", or, 
	       "exit when (chrec0 == chrec1)".  */
	    return set_nb_iterations_in_loop 
	      (loop, nb_iterations_in_loop_until_eq (loop_num, 
						     chrec0, chrec1));
	    
	  default:
	    return set_nb_iterations_in_loop (loop, chrec_top);
	  }
	
      default:
	return set_nb_iterations_in_loop (loop, chrec_top);
      }
}



/* The following section contains Scalar Dependence Graph related
   functions.  The SDG is used for directing the analysis of the
   induction variables.  */

/* For each exit condition in EXIT_CONDITIONS, get its operands, and
   determine a list of variables that we have to imperatively
   analyze.  */

static void
schedule_exit_conditions (struct loop *loop_nest, 
			  varray_type exit_conditions, 
			  varray_type ln_imperative_vars)
{
  unsigned int i;
  
  DBG_S (fprintf (stderr, "(schedule_exit_conditions \n"));
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (exit_conditions); i++)
    {
      tree test, opnd0, opnd1;
      
      test = VARRAY_TREE (exit_conditions, i);
      
      switch (TREE_CODE (test))
	{
	case LT_EXPR:
	case LE_EXPR:
	case GT_EXPR:
	case GE_EXPR:
	case EQ_EXPR:
	case NE_EXPR:
	  opnd0 = TREE_OPERAND (test, 0);
	  opnd1 = TREE_OPERAND (test, 1);
	  
	  if (TREE_CODE (opnd0) == SSA_NAME)
	    if (!variable_is_in_imperative_list_p (SSA_NAME_VAR (opnd0), 
						   ln_imperative_vars))
	      insert_ssa_name_in_imperative_list (opnd0, 
						  loop_nest,
						  ln_imperative_vars);
	  
	  if (TREE_CODE (opnd1) == SSA_NAME)
	    if (!variable_is_in_imperative_list_p (SSA_NAME_VAR (opnd1), 
						   ln_imperative_vars))
	      insert_ssa_name_in_imperative_list (opnd1, 
						  loop_nest,
						  ln_imperative_vars);
	  break;
	  
	case SSA_NAME:
	  if (!variable_is_in_imperative_list_p (SSA_NAME_VAR (test), 
					       ln_imperative_vars))
	    insert_ssa_name_in_imperative_list (test, 
						loop_nest,
						ln_imperative_vars);
	  break;
	  
	default:
	  abort ();
	  break;
	}
    }
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* Construct the schedule of the analysis.  The schedule keeps track of:
   
   - the dependences coming from assignments:
     "a = a + b" adds a dependence "a->b", and the evolution of "a"
     will be analyzed after the evolution of "b",
     
   - the dependences coming from loop nestings:
     "while (b < 100) a++;" adds the dependence "a->b" since the
     evolution of "a" depends on the number of iterations of the loop
     on "b".  */

static void
construct_schedule (varray_type imperative_vars, 
		    varray_type schedule)
{
  varray_type sdd_graph, complete_imperative_list;
  unsigned int i;
  
  DBG_S (fprintf (stderr, "(construct_schedule \n"));
  
  VARRAY_TREE_INIT (sdd_graph, 100, "sdd_graph");
  VARRAY_GENERIC_PTR_INIT (complete_imperative_list, 100, 
			   "complete_imperative_list");
  
  /* Construct the SDG = SDD = Scalar Data Dependence Graph.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (imperative_vars); i++)
    {
      struct schedule_elt *sched_elt;
      
      sched_elt = (struct schedule_elt *) 
	VARRAY_GENERIC_PTR (imperative_vars, i);
      DBG_SCHEDULE_S (dump_schedule_elt (stderr, sched_elt));
      
      record_dependences_for_ssa_name (SCHED_LOOP_NEST (sched_elt),
				       VARRAY_TREE (SCHED_SCC (sched_elt), 0),
				       sdd_graph, 
				       complete_imperative_list);
    }
  
  DBG_SCHEDULE_S (debug_schedule (complete_imperative_list));
  
  /* Then establish a schedule for this analysis.  */
  wolfe_topological_sort_SDD_with_SCCs (complete_imperative_list, 
					schedule, sdd_graph);
  DBG_SCHEDULE_S (debug_schedule (schedule));
  
  varray_clear (sdd_graph);
  varray_clear (complete_imperative_list);
  DBG_S (fprintf (stderr, ")\n"));
}

/* Compute the scalar dependences for variable VAR in LOOP_NEST, and
   record these dependence relations into the SDD_GRAPH.  Construct
   the COMPLETE_IMPERATIVE_LIST that contains all the nodes of the
   SDD_GRAPH.  */

static void
record_dependences_for_ssa_name (struct loop *loop_nest,
				 tree ssa_name, 
				 varray_type sdd_graph, 
				 varray_type complete_imperative_list)
{
  tree var = SSA_NAME_VAR (ssa_name);
  
  /* Avoid cycles in the scalar dependence graph.  */
  if (variable_is_in_imperative_list_p (var, complete_imperative_list))
    return;
  
  insert_ssa_name_in_imperative_list (ssa_name, loop_nest, 
				      complete_imperative_list);
  
  follow_ssa_edge_and_record_dependences (loop_nest, 
					  SSA_NAME_DEF_STMT (ssa_name), 
					  sdd_graph, 
					  complete_imperative_list);
  
  if (loop_nest->inner)
    record_dependences_for_var_rec (loop_nest, loop_nest->inner, 
				    var, sdd_graph, 
				    complete_imperative_list);
}

/* Helper function.  */

static bool
iccp_record_dependences_for_ssa_name (struct loop *loop_nest,
				      tree ssa_name, 
				      varray_type sdd_graph, 
				      varray_type imperative_list)
{
  tree chrec, def_stmt;

  if (variable_is_in_imperative_list_p (SSA_NAME_VAR (ssa_name),
					imperative_list))
    return false;
  
  def_stmt = SSA_NAME_DEF_STMT (ssa_name);
  if (TREE_CODE (def_stmt) == NOP_EXPR)
    /* testsuite/.../ssa-chrec-46.c.  */
    chrec = chrec_symbolic_parameter;
  
  /* Keep symbolic the variables defined in function of VUSEs.  */
  else if (VUSE_OPS (stmt_ann (def_stmt)))
    {
      chrec = chrec_symbolic_parameter;
      set_scev (loop_nest->num, TREE_OPERAND (def_stmt, 0), chrec);
    }
  
  else
    chrec = evolution_at_version 
      (bb_for_stmt (def_stmt)->loop_father->num, ssa_name);
  
  /* When the evolution of the variable is not yet analyzed, enqueue
     it in the IMPERATIVE_LIST, and follow the ssa link.  */
  if (chrec == chrec_not_analyzed_yet)
    {
      insert_ssa_name_in_imperative_list (ssa_name, loop_nest, 
					  imperative_list);
      iccp_record_dependences (loop_nest, def_stmt, sdd_graph, 
			       imperative_list);
      
      /* Yes, we have inserted the SSA_NAME in the
	 imperative_list.  */
      return true;
    }
  return false;
}

/* Helper function.  */

static void
record_dependences_for_var_rec (struct loop *loop_nest, 
				struct loop *loop, 
				tree var, 
				varray_type sdd_graph, 
				varray_type complete_imperative_list)
{
  tree halting_phi = get_loop_phi_node_for_variable (loop, var);
  unsigned halting_depth;
  
  if (halting_phi)
    {
      int i;
      
      halting_depth = bb_for_stmt (halting_phi)->loop_father->depth;
      
      for (i = 0; i < PHI_NUM_ARGS (halting_phi); i++)
	{
	  tree arg = PHI_ARG_DEF (halting_phi, i);
	  
	  if (TREE_CODE (arg) == SSA_NAME)
	    {
	      tree rdef = SSA_NAME_DEF_STMT (arg);
	      
	      if (rdef != NULL_TREE 
		  && TREE_CODE (rdef) != NOP_EXPR)
		{
		  if (((unsigned) bb_for_stmt (rdef)->loop_father->depth)
		      >= halting_depth)
		    follow_ssa_edge_and_record_dependences_rec 
		      (loop_nest, rdef, halting_phi, sdd_graph, 
		       complete_imperative_list);
		  else if (TREE_CODE (arg) == SSA_NAME)
		    {
		      /* This corresponds to an outer loop branch of
			 the PHI node that refers to another variable.
			 
			 Example: in testsuite/.../ssa-chrec-01.c the
			 variable "c" depends on the evolution of the
			 variable "a".  "c_5 = PHI <a_13(2),
			 c_16(3)>;"
		      */
		      SDD_insert (PHI_RESULT (halting_phi), arg, sdd_graph);
		      
		      record_dependences_for_ssa_name 
			(loop_nest, arg, sdd_graph, complete_imperative_list);
		    }
		}
	    }
	}
    }
  else if (loop->inner)
    record_dependences_for_var_rec 
      (loop_nest, loop->inner, var, sdd_graph, complete_imperative_list);
  
  if (loop->next)
    record_dependences_for_var_rec 
      (loop_nest, loop->next, var, sdd_graph, complete_imperative_list);
  
}

/* Helper function.  */

static inline void 
record_dependences_for_opnd (struct loop *loop_nest, 
			     tree lhs,
			     tree opnd, 
			     varray_type sdd_graph, 
			     varray_type complete_imperative_list)
{
  if (opnd == NULL_TREE)
    return;
  
  if (TREE_CODE (opnd) == SSA_NAME)
    {
      if (SSA_NAME_VAR (lhs) != SSA_NAME_VAR (opnd))
	{
	  SDD_insert (lhs, opnd, sdd_graph);
	  record_dependences_for_ssa_name 
	    (loop_nest, opnd, sdd_graph, complete_imperative_list);
	}
      else
	follow_ssa_edge_and_record_dependences
	  (loop_nest, SSA_NAME_DEF_STMT (opnd), 
	   sdd_graph, complete_imperative_list);
    }
}

/* Helper function for the recursive version.  */

static inline void 
record_dependences_for_opnd_rec (struct loop *loop_nest, 
				 tree lhs,
				 tree opnd, 
				 tree halting_phi,
				 varray_type sdd_graph, 
				 varray_type complete_imperative_list)
{
  if (opnd == NULL_TREE)
    return;
  
  if (TREE_CODE (opnd) == SSA_NAME)
    {
      if (SSA_NAME_VAR (lhs) != SSA_NAME_VAR (opnd))
	{
	  SDD_insert (lhs, opnd, sdd_graph);
	  record_dependences_for_ssa_name 
	    (loop_nest, opnd, sdd_graph, complete_imperative_list);
	}
      else
	follow_ssa_edge_and_record_dependences_rec
	  (loop_nest, SSA_NAME_DEF_STMT (opnd), halting_phi,
	   sdd_graph, complete_imperative_list);
    }
}

/* Helper function.  */

static inline void
iccp_record_dependences_for_opnd (struct loop *loop_nest, 
				  tree lhs,
				  tree opnd,
				  varray_type sdd_graph,
				  varray_type imperative_list)
{
  if (opnd == NULL_TREE)
    return;

  if (TREE_CODE (opnd) == SSA_NAME
      && SSA_NAME_VAR (lhs) != SSA_NAME_VAR (opnd))
    {
      if (iccp_record_dependences_for_ssa_name (loop_nest, opnd, sdd_graph, 
						imperative_list))
	{
	  /* If we have inserted OPND in the imperative_list, then we
	     have to complete the list with the LHS and insert an edge
	     in the SDD (for Wolfe's algorithm).  */
	  insert_ssa_name_in_imperative_list (lhs, loop_nest, imperative_list);
	  SDD_insert (lhs, opnd, sdd_graph);
	}
    }
}

/* Helper function.  */

static void
record_dependences_for_modify_expr (struct loop *loop_nest, 
				    tree modify_expr, 
				    varray_type sdd_graph, 
				    varray_type complete_imperative_list)
{
  tree lhs, rhs, opnd0, opnd1;
  
  lhs = TREE_OPERAND (modify_expr, 0);
  rhs = TREE_OPERAND (modify_expr, 1);
  
  if (TREE_CODE_LENGTH (TREE_CODE (rhs)) == 2)
    {
      opnd0 = TREE_OPERAND (rhs, 0);
      record_dependences_for_opnd (loop_nest, lhs, opnd0, 
				   sdd_graph, complete_imperative_list);
      
      opnd1 = TREE_OPERAND (rhs, 1);
      record_dependences_for_opnd (loop_nest, lhs, opnd1, 
				   sdd_graph, complete_imperative_list);
    }
  else if (TREE_CODE (rhs) == SSA_NAME)
    {
      if (SSA_NAME_VAR (lhs) != SSA_NAME_VAR (rhs))
	{
	  SDD_insert (lhs, rhs, sdd_graph);
	  record_dependences_for_ssa_name 
	    (loop_nest, rhs, sdd_graph, complete_imperative_list);
	}
    }
  else if (TREE_CODE_LENGTH (TREE_CODE (rhs)) == 1)
    {
      opnd0 = TREE_OPERAND (rhs, 0);
      record_dependences_for_opnd (loop_nest, lhs, opnd0, 
				   sdd_graph, complete_imperative_list);
    }
}

/* Helper function for the recursive version.  */

static void
record_dependences_for_modify_expr_rec (struct loop *loop_nest, 
					tree modify_expr, 
					tree halting_phi,
					varray_type sdd_graph, 
					varray_type complete_imperative_list)
{
  tree lhs, rhs, opnd0, opnd1;
  
  lhs = TREE_OPERAND (modify_expr, 0);
  rhs = TREE_OPERAND (modify_expr, 1);
  
  if (TREE_CODE_LENGTH (TREE_CODE (rhs)) == 2)
    {
      opnd0 = TREE_OPERAND (rhs, 0);
      record_dependences_for_opnd_rec (loop_nest, lhs, opnd0, halting_phi,
				       sdd_graph, complete_imperative_list);
      
      opnd1 = TREE_OPERAND (rhs, 1);
      record_dependences_for_opnd_rec (loop_nest, lhs, opnd1, halting_phi,
				       sdd_graph, complete_imperative_list);
    }
  else if (TREE_CODE (rhs) == SSA_NAME)
    {
      if (SSA_NAME_VAR (lhs) != SSA_NAME_VAR (rhs))
	{
	  SDD_insert (lhs, rhs, sdd_graph);
	  record_dependences_for_ssa_name 
	    (loop_nest, rhs, sdd_graph, complete_imperative_list);
	}
    }
  else if (TREE_CODE_LENGTH (TREE_CODE (rhs)) == 1)
    {
      opnd0 = TREE_OPERAND (rhs, 0);
      record_dependences_for_opnd_rec (loop_nest, lhs, opnd0, halting_phi,
				       sdd_graph, complete_imperative_list);
    }
}

/* Helper function.  */

static void 
iccp_record_dependences_for_modify_expr (struct loop *loop_nest,
					 tree modify_expr,
					 varray_type sdd_graph,
					 varray_type imperative_list)
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
      iccp_record_dependences_for_opnd
	(loop_nest, lhs, TREE_OPERAND (rhs, 1), sdd_graph, imperative_list);
      
    case 1:
      iccp_record_dependences_for_opnd
	(loop_nest, lhs, TREE_OPERAND (rhs, 0), sdd_graph, imperative_list);
      
    case 0:
      return;
      
    default:
      abort ();
    }
}

/* Follow the SSA edges and record all the dependences due to scalar
   assignments.  */

static void
follow_ssa_edge_and_record_dependences (struct loop *loop_nest,
					tree def_stmt, 
					varray_type sdd_graph, 
					varray_type complete_imperative_list)
{
  DBG_SCHEDULE_S (fprintf (stderr, "(follow_ssa_edge_and_record_dependences: \n");
		  debug_generic_expr (def_stmt));
  
  switch (TREE_CODE (def_stmt))
    {
    case PHI_NODE:
      {
	int i;
	unsigned halting_depth;
	tree halting_phi;
	
	halting_phi = def_stmt;
	halting_depth = bb_for_stmt (halting_phi)->loop_father->depth;
	
	for (i = 0; i < PHI_NUM_ARGS (halting_phi); i++)
	  {
	    tree arg = PHI_ARG_DEF (halting_phi, i);
	    
	    if (TREE_CODE (arg) == SSA_NAME)
	      {
		tree rdef = SSA_NAME_DEF_STMT (arg);
		
		if (rdef != NULL_TREE 
		    && TREE_CODE (rdef) != NOP_EXPR
		    && (((unsigned) bb_for_stmt (rdef)->loop_father->depth)
			>= halting_depth))
		  {
		    if (SSA_NAME_VAR (arg) 
			!= SSA_NAME_VAR (PHI_RESULT (halting_phi)))
		      /* This is the case when we have a loop phi node with 
			 another variable name: see testsuite/.../ssa_chrec-36.c
			 "  #   a_1 = PHI <1(0), T.1_11(1)>;".  */
		      record_dependences_for_ssa_name 
			(loop_nest, arg, sdd_graph, complete_imperative_list);
		    else
		      follow_ssa_edge_and_record_dependences_rec 
			(loop_nest, rdef, halting_phi, sdd_graph, 
			 complete_imperative_list);
		  }
	      }
	  }
	break;
      }
      
    case MODIFY_EXPR:
      record_dependences_for_modify_expr (loop_nest, def_stmt, sdd_graph, 
					  complete_imperative_list);
      break;
      
    case ASM_EXPR:
    case NOP_EXPR:
      break;
     
    default:
      abort ();
      break;
    }
  
  DBG_SCHEDULE_S (fprintf (stderr, ")\n"));
}

/* Recursively follow the SSA edges and record all the dependences due
   to scalar assignments, and to loop conditions.  */

static void
follow_ssa_edge_and_record_dependences_rec (struct loop *loop_nest,
					    tree rdef, 
					    tree halting_phi, 
					    varray_type sdd_graph,
					    varray_type complete_imperative_list)
{
  unsigned rdef_depth, halting_depth;
  if (rdef == NULL_TREE 
      || halting_phi == NULL_TREE
      || TREE_CODE (rdef) == NOP_EXPR
      /* End the recursion when the halting_phi node is reached.  */
      || rdef == halting_phi)
    return;
  
  DBG_SDD_S (debug_generic_expr (rdef));
  
  rdef_depth = bb_for_stmt (rdef)->loop_father->depth;
  halting_depth = bb_for_stmt (halting_phi)->loop_father->depth;
  
  switch (TREE_CODE (rdef))
    {
    case PHI_NODE:
      
      /* Avoid recursion when the phi-nodes contain SCCs.  */
      if (PHI_MARKED (rdef) == 1)
	return;
      
      else
	PHI_MARKED (rdef) = 1;
      
      if (rdef_depth > halting_depth)
	{
	  /* This is an inner loop phi.  */
	  int i;
	  
	  for (i = 0; i < PHI_NUM_ARGS (rdef); i++)
	    {
	      tree arg = PHI_ARG_DEF (rdef, i);
	      
	      if (TREE_CODE (arg) == SSA_NAME)
		{
		  tree rdef_edge = SSA_NAME_DEF_STMT (arg);
		  
		  if (rdef_edge != NULL_TREE 
		      && TREE_CODE (rdef_edge) != NOP_EXPR)
		    {
		      if (bb_for_stmt (rdef_edge)->loop_father->depth >=
			  (int) rdef_depth)
			/* This is an edge to the inner loop.  */
			follow_ssa_edge_and_record_dependences_rec 
			  (loop_nest, rdef_edge, rdef, sdd_graph, 
			   complete_imperative_list);
		      else
			/* This is an edge to the outer loop.  */
			follow_ssa_edge_and_record_dependences_rec 
			  (loop_nest, rdef_edge, halting_phi, sdd_graph,
			   complete_imperative_list);
		    }
		}
	    }
	  SDD_insert_deps_on_loop_exit_vars 
	    (loop_nest, PHI_RESULT (rdef), bb_for_stmt (rdef)->loop_father,
	     sdd_graph, complete_imperative_list);
	}
      else
	{
	  /* This is a phi after a multi-branch expression.  */
	  int i;
	  
	  for (i = 0; i < PHI_NUM_ARGS (rdef); i++)
	    {
	      tree arg = PHI_ARG_DEF (rdef, i);
	      
	      if (TREE_CODE (arg) == SSA_NAME)
		{
		  tree rdef_edge = SSA_NAME_DEF_STMT (arg);
		  
		  if (rdef_edge != NULL_TREE 
		      && TREE_CODE (rdef_edge) != NOP_EXPR)
		    follow_ssa_edge_and_record_dependences_rec 
		      (loop_nest, rdef_edge, halting_phi, sdd_graph, 
		       complete_imperative_list);
		}
	    }
	}
      
      /* Reset the marker after the analysis.  */
      PHI_MARKED (rdef) = 0;
      break;
      
    case MODIFY_EXPR:
      record_dependences_for_modify_expr_rec 
	(loop_nest, rdef, halting_phi, sdd_graph, complete_imperative_list);
      break;
      
    case ASM_EXPR:
    case NOP_EXPR:
      break;
      
    default:
      abort ();
      break;
    }
}



/* Inserts dependences on loop exit: we have to compute the number of
   iterations of the inner loop before analysing the evolution of a
   variable in that loop.  */

static void
SDD_insert_deps_on_loop_exit_vars (struct loop *loop_nest,
				   tree var, 
				   struct loop *loop,
				   varray_type sdd_graph, 
				   varray_type complete_imperative_list)
{
  tree test = get_loop_exit_condition (loop);
  tree opnd0, opnd1;
  
  if (test == NULL_TREE)
    return;
  
  switch (TREE_CODE (test))
    {
    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
      opnd0 = TREE_OPERAND (test, 0);
      opnd1 = TREE_OPERAND (test, 1);
      
      if (TREE_CODE (opnd0) == SSA_NAME)
	{
	  /*  (VAR -> OPND0).  */
	  SDD_insert (var, opnd0, sdd_graph);
	  record_dependences_for_ssa_name 
	    (loop_nest, opnd0, sdd_graph, complete_imperative_list);
	}
      
      if (TREE_CODE (opnd1) == SSA_NAME)
	{
	  /*  (VAR -> OPND1).  */
	  SDD_insert (var, opnd1, sdd_graph);
	  record_dependences_for_ssa_name 
	    (loop_nest, opnd1, sdd_graph, complete_imperative_list);
	}
      break;
      
    case SSA_NAME:
      /*  (VAR -> TEST).  */
      SDD_insert (var, test, sdd_graph);
      record_dependences_for_ssa_name 
	(loop_nest, test, sdd_graph, complete_imperative_list);
      break;
      
    default:
      abort ();
      break;
    }
}

/* Insert a tuple (a, b), ie. "a depends on b", in the SDD.  */

static void
SDD_insert (tree a, 
	    tree b,
	    varray_type sdd_graph)
{
  if (TREE_CODE (a) == SSA_NAME 
      && TREE_CODE (b) == SSA_NAME
      && TREE_CODE (SSA_NAME_VAR (a)) != INDIRECT_REF
      && TREE_CODE (SSA_NAME_VAR (b)) != INDIRECT_REF)
    {
      tree name_a, name_b;
      
      name_a = SSA_NAME_VAR (a);
      name_b = SSA_NAME_VAR (b);
      
      /* Insert the tuple (a, b).  */
      
      DBG_SDD_S (fprintf (stderr, "SDD_insert \n");
		 debug_generic_expr (name_a);
		 debug_generic_expr (name_b));
      
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
      SCHED_LOOP_NEST (sched_elt) = loop_nest_at_index_in_imperative_list 
	(VARRAY_TOP_UINT (wolfe_stack), complete_imperative_list);
      
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
	  
#if defined ENABLE_CHECKING
	  {
	    struct loop *loop_nest;
	    
	    loop_nest = loop_nest_at_index_in_imperative_list 
	      (w, complete_imperative_list);
	    
	    if (SCHED_LOOP_NEST (sched_elt) != loop_nest
		&& !loop_is_included_in (SCHED_LOOP_NEST (sched_elt)->num, 
					 loop_nest->num))
	      /* This means that the SCC spans over multiple loop nests: 
		 the Wolfe's analyzer should not detect this as a SCC.  */
	      abort ();
	  }
#endif
	  
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
	analyze_evolution (VARRAY_TREE (SCHED_SCC (sched_elt), 0), 
			   SCHED_LOOP_NEST (sched_elt));
      else
	analyze_evolution_scc (SCHED_SCC (sched_elt), 
			       SCHED_LOOP_NEST (sched_elt));
    }
  
  DBG_S (fprintf (stderr, ")\n"));
}



/* This section selects the loops that will be good candidates for the
   scalar evolution analysis.
   
   Note: This section will be rewritten to expose a better interface
   to other client passes.  For the moment, greedily select all the
   loop nests we could analyze.  */

/* Select the candidate loop nests for the analysis.  This function
   initializes the loop_nests and exit_conditions arrays.  */

void
select_loop_nests_for_scalar_evolutions_analyzer (struct loops *loops, 
						  varray_type loop_nests, 
						  varray_type exit_conditions)
{
  unsigned int i;
  
  for (i = 1; i < loops->num; i++)
    {
      struct loop *loop = loops->parray[i];
      
      /* Loop selection condition.  
	 FIXME: use another condition...  */
      if (loop->depth == 1)
	{
	  varray_type ec;
	  
	  VARRAY_TREE_INIT (ec, 5, "ec");
	  if (get_exit_conditions_for_nest (loop, ec))
	    {
	      VARRAY_PUSH_GENERIC_PTR (exit_conditions, ec);
	      VARRAY_PUSH_GENERIC_PTR (loop_nests, loop);
	    }
	  else
	    {
	      /* Don't bother analyze the loop nest if the exit
		 conditions are too difficult to analyze.

		 FIXME: It is possible though to analyze an inner
		 nest...  Don't discard the whole nest if the outer
		 loop is ill formed.
	      */
	      varray_clear (ec);
	    }
	}
    }
}

/* Retrieve all the exit conditions for a loop nest, without entering
   in the sibling loops.  If one of the conditions is too difficult to
   analyze, then return false.  */

static bool 
get_exit_conditions_for_nest (struct loop *loop_nest, 
			      varray_type exit_conditions)
{
  tree loop_condition;
  
  DBG_S (fprintf (stderr, "Analyzing loop_nest: %d\n", loop_nest->num));
  
  flow_loop_scan (loop_nest, LOOP_EXIT_EDGES);
  if (loop_nest->num_exits != 1)
    return false;
  
  loop_condition = get_loop_exit_condition (loop_nest);
  if (loop_condition)
    {
      VARRAY_PUSH_TREE (exit_conditions, loop_condition);
      return get_exit_conditions_rec (loop_nest->inner, exit_conditions);
    }
  else
    return false;
}

/* Recursively determine and enqueue the exit conditions for a loop.  */

static bool 
get_exit_conditions_rec (struct loop *loop, 
			 varray_type exit_conditions)
{
  tree loop_condition;
  
  if (!loop)
    return true;

  flow_loop_scan (loop, LOOP_EXIT_EDGES);
  if (loop->num_exits != 1)
    return false;
  
  loop_condition = get_loop_exit_condition (loop);
  if (loop_condition)
    VARRAY_PUSH_TREE (exit_conditions, loop_condition);
  else
    return false;
  
  /* Recurse on the next (sibling) loop, then on the inner loops.  */
  return (get_exit_conditions_rec (loop->next, exit_conditions)
	  && get_exit_conditions_rec (loop->inner, exit_conditions));
}

/* For a loop with a single exit edge, determine the COND_EXPR that
   guards the exit edge.  If the expression is too difficult to
   analyze, then give up.  */

static tree 
get_loop_exit_condition (struct loop *loop)
{
  tree res = NULL_TREE;
  
  DBG_S (fprintf (stderr, "(get_loop_exit_condition \n  "));
  
  if (loop->exit_edges)
    {
      edge exit;
      basic_block exit_bb;
      tree expr;
      
      exit = loop->exit_edges[0];
      exit_bb = exit->src;
      expr = last_stmt (exit_bb);
      
      if (analyzable_condition (expr))
	res = TREE_OPERAND (expr, 0);
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



/* ICCP section. 
   ICCP = Interval Copy Constant Propagation.  */

/* Recursive helper.  */

static void
iccp_record_dependences (struct loop *loop_nest,
			 tree expr,
			 varray_type sdd_graph,
			 varray_type imperative_list)
{
  int i;
  
  switch (TREE_CODE (expr))
    {
    case SSA_NAME:
      iccp_record_dependences_for_ssa_name 
	(loop_nest, expr, sdd_graph, imperative_list);
      return;
      
    case PHI_NODE:
      for (i = 0; i < PHI_NUM_ARGS (expr); i++)
	{
	  struct loop *loopf = bb_for_stmt (expr)->loop_father;
	  tree arg_i = PHI_ARG_DEF (expr, i);
	  iccp_record_dependences 
	    (loopf, arg_i, sdd_graph, imperative_list);
	  
	  /* If we have decided that this branch cannot be analyzed,
	     don't analyze the other branches.  */
	  if (get_scev (loopf->num, arg_i) == chrec_symbolic_parameter)
	    break;
	}
      return;
      
    case MODIFY_EXPR:
      iccp_record_dependences_for_modify_expr 
	(bb_for_stmt (expr)->loop_father, expr, sdd_graph, imperative_list);
      return;
      
    default:
      break;
    }
  
  switch (TREE_CODE_LENGTH (TREE_CODE (expr)))
    {
    case 2:
      iccp_record_dependences 
	(loop_nest, TREE_OPERAND (expr, 1), sdd_graph, imperative_list);
      
    case 1:
      iccp_record_dependences
	(loop_nest, TREE_OPERAND (expr, 0), sdd_graph, imperative_list);
      
    case 0:
      return;
      
    default:
      abort ();
    }
}

/* Determines a SCHEDULE of scalar variables that have to be analyzed
   before beeing able to analyze the expression EXPR.  */

static void 
iccp_what_scalars_have_to_be_analyzed_before (struct loop *loop_nest, 
					      tree expr, 
					      varray_type schedule)
{
  varray_type sdd_graph, imperative_list;
  
  DBG_S (fprintf (stderr, "(iccp_what_scalars_have_to_be_analyzed_before: ");
	 debug_generic_expr (expr));
  
  VARRAY_TREE_INIT (sdd_graph, 100, "sdd_graph");
  VARRAY_GENERIC_PTR_INIT (imperative_list, 100, 
			   "imperative_list");
  
  iccp_record_dependences 
    (loop_nest, expr, sdd_graph, imperative_list);
  
  /* When the scalar evolution cannot be determined directly because
     of other not yet analyzed scalar evolutions, establish a schedule
     for the analysis.  */
  if (VARRAY_ACTIVE_SIZE (imperative_list) > 0)
    {
      DBG_SCHEDULE_S (debug_schedule (imperative_list));
      wolfe_topological_sort_SDD_with_SCCs (imperative_list, 
					    schedule, sdd_graph);
      DBG_SCHEDULE_S (debug_schedule (schedule));
    }
  
  varray_clear (sdd_graph);
  varray_clear (imperative_list);
  DBG_S (fprintf (stderr, ")\n"));
}

/* A recursive helper function.  */

static tree 
iccp_follow_ssa_edge_and_compute_scev (int loop_num, 
				       tree expr,
				       varray_type already_seen_variables)
{
  tree res;
  
#if defined ENABLE_CHECKING
  if (expr == NULL_TREE)
    abort ();
#endif
  
  switch (TREE_CODE (expr))
    {
    case SSA_NAME:
      if (tree_is_in_varray_tree_p (expr, already_seen_variables))
	/* Stop an infinite recursion caused by a SCC in the SSA.  */
	res = expr;
      
      else
	{
	  /* LOOP_NUM is used here for determining in which loop the
	     EXPR lives.  */
	  res = evolution_at_version (loop_num, expr);
	  
	  /* When the version has not been analyzed yet, follow the
	     SSA edge, and compute its scalar evolution.  */
	  if (res == expr
	      || res == chrec_not_analyzed_yet
	      || res == chrec_symbolic_parameter)
	    {
	      VARRAY_PUSH_TREE (already_seen_variables, expr);
	      res = iccp_follow_ssa_edge_and_compute_scev 
		(loop_num, SSA_NAME_DEF_STMT (expr), already_seen_variables);
	    }
	}
      break;
      
    case PHI_NODE:
      /* FIXME: There are two kinds of phi nodes: the if-phi-nodes,
	 and the loop-phi-nodes.  The following code solves only the
	 case of the if-phi-nodes.  When seeing a loop-phi-node, the
	 analyzer has to either push the variable into the imperative
	 list and call the IV detector, or to answer "don't know" if
	 the variable has no chrec associated with it on the current
	 loop-phi-node.  */
      res = evolution_at_version 
	(bb_for_stmt (expr)->loop_father->num, PHI_RESULT (expr));
      
      if (res == PHI_RESULT (expr))
	{
	  int i;
	  res = iccp_follow_ssa_edge_and_compute_scev 
	    (loop_num, PHI_ARG_DEF (expr, 0), already_seen_variables);
	  
	  for (i = 1; i < PHI_NUM_ARGS (expr); i++)
	    res = chrec_merge 
	      (res, iccp_follow_ssa_edge_and_compute_scev 
	       (loop_num, PHI_ARG_DEF (expr, i), already_seen_variables));
	}
      break;
      
    case MODIFY_EXPR:
      res = iccp_follow_ssa_edge_and_compute_scev 
	(bb_for_stmt (expr)->loop_father->num,
	 TREE_OPERAND (expr, 1), already_seen_variables);
      break;
      
    case PLUS_EXPR:
      res = chrec_fold_plus 
	(iccp_follow_ssa_edge_and_compute_scev 
	 (loop_num, TREE_OPERAND (expr, 0), already_seen_variables),
	 iccp_follow_ssa_edge_and_compute_scev 
	 (loop_num, TREE_OPERAND (expr, 1), already_seen_variables));
      break;
      
    case MINUS_EXPR:
      res = chrec_fold_minus
	(iccp_follow_ssa_edge_and_compute_scev 
	 (loop_num, TREE_OPERAND (expr, 0), already_seen_variables),
	 iccp_follow_ssa_edge_and_compute_scev 
	 (loop_num, TREE_OPERAND (expr, 1), already_seen_variables));
      break;
      
    case MULT_EXPR:
      res = chrec_fold_multiply
	(iccp_follow_ssa_edge_and_compute_scev 
	 (loop_num, TREE_OPERAND (expr, 0), already_seen_variables),
	 iccp_follow_ssa_edge_and_compute_scev 
	 (loop_num, TREE_OPERAND (expr, 1), already_seen_variables));
      break;
      
    case INTEGER_CST:
    case REAL_CST:
      res = expr;
      break;
      
    case NOP_EXPR:
      res = get_scev (loop_num, expr);
      break;
      
    case VAR_DECL:
    case PARM_DECL:
    default:
      res = get_scev (loop_num, expr);
      
      if (res == chrec_top)
	/* KEEP_IT_SYMBOLIC.  */
	res = expr;
      
      break;
    } 
  
  return res;
}

/* Given an expression EXPR, this function finds an approximation of
   the evolution function.  When the expression is not in a loop, the
   result is equivalent to a classic CCP with interval values for
   approximating conditional expressions.
   
   How it works?  
   
   It follows the ssa links bottom-up until obtaining a close form
   expression.  CHREC_TOP is used as usual in the cases where the
   value cannot be determined statically.  Note that the IV analyzer
   is called */

tree 
iccp_determine_evolution_function (struct loop *loop_nest, 
				   tree expr)
{
  tree res;
  
  DBG_S (fprintf (stderr, "(iccp_determine_evolution_function: ");
	 debug_generic_expr (expr));
  
  switch (TREE_CODE (expr))
    {
    case INTEGER_CST:
    case REAL_CST:
    case VAR_DECL:
    case PARM_DECL:
      res = get_scev (loop_nest->num, expr);
      break;
      
    case PHI_NODE:
    case MODIFY_EXPR:
      loop_nest = bb_for_stmt (expr)->loop_father;

      /* Fall-thru.  */
    case SSA_NAME:
      {
	varray_type schedule;
	varray_type already_seen_variables;
	
	res = evolution_at_version (loop_nest->num, expr);
	
	/* When the evolution is already in the knowledge base, just
	   return it, else compute it.  */
	if (res == expr
	    || res == chrec_top)
	  {
	    
	    /* No need to compute the evolutions in a loop of depth 0.
	       At depth 0, the evolution is constant, and the ICCP is
	       the classic range propagation.  */
	    if (loop_nest->depth > 0)
	      {
		VARRAY_GENERIC_PTR_INIT (schedule, 37, "schedule");
		iccp_what_scalars_have_to_be_analyzed_before (loop_nest, expr, 
							      schedule);
		
		if (VARRAY_ACTIVE_SIZE (schedule) > 0)
		  analyze_scalars_from_schedule (schedule);
		
		varray_clear (schedule);
	      }
	    
	    VARRAY_TREE_INIT (already_seen_variables, 37, 
			      "already_seen_variables");
	    res = iccp_follow_ssa_edge_and_compute_scev
	      (loop_nest->num, expr, already_seen_variables);
	    varray_clear (already_seen_variables);
	  }
	
	break;
      }
      
    default:
      res = chrec_top;
      break;
    }
  
  DBG_S (fprintf (stderr, ")\n"));
  return res;
}



/* Debugging functions section.  */

/* Dump into DUMP_FILE all the evolution functions contained in
   SCHEDULE.  */

static void 
dump_evolution_functions (varray_type schedule)
{
  if (dump_file)
    {
      unsigned int j;
      
      /* print_generic_stmt (dump_file, 
	 DECL_SAVED_TREE (current_function_decl), 0);  */
      fprintf (dump_file, "\n\n  Scalar evolution functions: \n\n");
      
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
	      
	      print_generic_stmt (dump_file, var, 0);
	      fprintf (dump_file, "  ->  ");
	      print_generic_stmt (dump_file, chrec, 0);
	      fprintf (dump_file, "\n");
	    }
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
  fprintf (file, "  loop_num = %d", SCHED_LOOP_NEST (sched_elt)->num);
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

void draw_SDD (varray_type sdd_graph)
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
initialize_scalar_evolutions_analyzer (void)
{
  /* Initialize debugging dumps.  */
  dump_file = dump_begin (TDI_scev, &dump_flags);
    
#if 0
  DBG_S (fprintf (stderr, "Loop and SSA IRs:\n");
	 debug_loop_ir ());
#endif
  
  /* The elements below are unique.  The values contained in these
     intervals are not used.  */
  chrec_not_analyzed_yet = build_interval_chrec (build_int_2 (1111, 0), 
						 build_int_2 (1111, -1));
  chrec_top = build_interval_chrec (build_int_2 (2222, 0), 
				    build_int_2 (2222, -1));
  chrec_bot = build_interval_chrec (build_int_2 (3333, -1), 
				    build_int_2 (3333, 0));
  chrec_symbolic_parameter = build_interval_chrec (build_int_2 (4444, -1),
						   build_int_2 (4444, 0));
}

/* Finalize.  */

void 
finalize_scalar_evolutions_analyzer (void)
{
  if (dump_file)
    {
      fprintf (dump_file, "\n\n");
      dump_end (TDI_scev, dump_file);
    }
}


/* Entry point for this analysis pass.  */

void 
analyze_scalar_evolutions (struct loops *loops, 
			   struct loop *loop_nest,
			   varray_type ev_info, 
			   varray_type exit_conditions)
{
  /* ANALYSIS_SCHEDULE is a list of strongly connected components
     (SCC).  The list contains the order in which the SCCs have to be
     analyzed.  An element of the ANALYSIS_SCHEDULE contains a list of
     variables that constitute an SCC.  */
  varray_type analysis_schedule;
  
  /* In the "light-weight" algorithm, we're interested only in a
     subset of the integer variables of a loop.  This constitutes the
     imperative schedule list, from which the analysis schedule list
     is built by partially sorting the SDG.  */
  varray_type imperative_vars;
  
  timevar_push (TV_SCALAR_EVOLUTIONS);
  
  monev_loops = loops;
  monev_info = ev_info;
  
  VARRAY_GENERIC_PTR_INIT (analysis_schedule, 37, 
			   "analysis_schedule");
  VARRAY_GENERIC_PTR_INIT (imperative_vars, 37, 
			   "imperative_vars");
  
  /* Construct a minimal list of analyzed variables.  It is 
     possible to push_back other integer variables to the 
     imperative schedule list: IMPERATIVE_VARS.  */
  schedule_exit_conditions (loop_nest,
			    exit_conditions, 
			    imperative_vars);
  
  construct_schedule (imperative_vars, analysis_schedule);
  analyze_scalars_from_schedule (analysis_schedule);
  dump_evolution_functions (analysis_schedule);
  
  varray_clear (imperative_vars);
  varray_clear (analysis_schedule);
  
  timevar_pop (TV_SCALAR_EVOLUTIONS);
}
