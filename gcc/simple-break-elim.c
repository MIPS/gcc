/* Break and continue statement elimination for Simple.
   Copyright (C) 2002 Free Software Foundation, Inc.
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "c-tree.h"
#include "c-common.h"
#include "tree-simple.h"
#include "tree-dchain.h"
#include "diagnostic.h"

/* This pass eliminates all BREAK_STMTs and CONTINUE_STMTs from loops.  
   It performs also dead code elimination.  This pass works on double linked
   statements on the SIMPLE representation.
   After this pass we obtain a subset of the SIMPLE grammar since all 
   SWITCH_STMTs are trasformed into IF_STMTs.  */

/* For normalizing the flow in the loop body, there are two approaches :
   Solution 1: copy statements (explicit control flow).
   Solution 2: use logical variables (data controled flow).

   The first solution raises a difficult problem linked to the visibility of a
   variable in different scopes.  

   The second solution is similar to the one adopted by the McCAT compiler for 
   eliminating GOTO_STMTs.  They describe an algorithm for eliminating goto 
   statements, but they use BREAK_STMTs for exiting loops.  The following 
   implementation is an extension of their algorithm and is based on the same 
   principle : the use of logical variables for controlling the control flow.
   
   References :
   - Taming Control Flow : A structured	Approach to Eliminate Goto Statements,
   Ana M. Erosa and Laurie J. Hendren, in the proceedings of the 1994 
   International Conference on Computer Languages (ICCL'94) pp.229-240. 

   - Erosa's MSc. Thesis, University McGill : 
   A goto-elimination method and its implementation for the McCAT C compiler,
   http://www.sable.mcgill.ca/~hendren/ftp/erosa/thesis.ps.gz  */


enum eval_bool {
  EVAL_FALSE = 0,
  EVAL_TRUE,
  EVAL_DONT_KNOW
};

static void break_continue_elimination_rec PARAMS ((tree));
static void break_elimination              PARAMS ((tree));
static void continue_stmt_solution2        PARAMS ((tree *));
static void break_stmt_solution2           PARAMS ((tree *));
static void outward_motion                 PARAMS ((tree, tree, tree, 
						    void (*) PARAMS ((tree, tree, tree, tree))));
static void update_break_paths             PARAMS ((tree, tree, tree, tree));
static void switch_to_if                   PARAMS ((tree));
static void construct_body_for_case_labels PARAMS ((tree));
static void complete_body_of_case_labels   PARAMS ((tree));
static void reorder_case_labels            PARAMS ((tree));
static tree cases_to_if                    PARAMS ((tree, tree));
static enum eval_bool eval_bool_condition  PARAMS ((tree));

/* Static variables.  */
static PATH_TYPE PATH_TOP;


/* Entry point to the break and continue elimination pass.  FN is the 
   FUNCTION_DECL node for the function we want to simplify.  */

void
break_continue_elimination (fn)
     tree fn;
{
  tree fn_body;

  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return;

  fn_body = COMPOUND_BODY (DECL_SAVED_TREE (fn));
  if (fn_body == NULL)
    return;

  /* Create a new binding level for the temporaries created by the
     simplification process.  */
  pushlevel (0);

  /* Transform breaks and continues from the function's body.  */
  break_continue_elimination_rec (fn_body);

  /* Declare the new temporary variables.  */
  dchain_declare_tmp_vars (getdecls(), fn_body);

  /* Restore the binding level.  */
  poplevel (1, 1, 0);
}

/* A helper function that transforms all [BREAK|CONTINUE]_STMTs from NODE 
   recursively.  */

static void
break_continue_elimination_rec (node)
     tree node;
{
  while (node && node != error_mark_node)
    {
      switch (TREE_CODE (node))
	{
	case COMPOUND_STMT:
	  PATH_PUSH (node);
	  break_continue_elimination_rec (COMPOUND_BODY (node));
	  PATH_POP ();
	  break;

	case FOR_STMT:
	  dchain_build_scope (&FOR_BODY (node));
	  PATH_PUSH (node);
	  break_continue_elimination_rec (FOR_BODY (node));
	  PATH_POP ();
	  break;

	case WHILE_STMT:
	  dchain_build_scope (&WHILE_BODY (node));
	  PATH_PUSH (node);
	  break_continue_elimination_rec (WHILE_BODY (node));
	  PATH_POP ();
	  break;

	case DO_STMT:
	  dchain_build_scope (&DO_BODY (node));
	  PATH_PUSH (node);
	  break_continue_elimination_rec (DO_BODY (node));
	  PATH_POP ();
	  break;

	case SWITCH_STMT:
	  /* Transform this node into an IF_STMT, since a SWITCH_STMT contains 
	     BREAK_STMTs that aren't of the same type as those we'll eliminate.
	     This transformation eliminate BREAK_STMTs that refers to a switch.  */
	  switch_to_if (node);
	  break;
	  
	case IF_STMT:
	  dchain_build_scope (&THEN_CLAUSE (node));
	  PATH_PUSH (node);
	  break_continue_elimination_rec (THEN_CLAUSE (node));
	  PATH_POP ();
	  if (ELSE_CLAUSE (node))
	    {
	      dchain_build_scope (&ELSE_CLAUSE (node));
	      PATH_PUSH (node);
	      break_continue_elimination_rec (ELSE_CLAUSE (node));
	      PATH_POP ();
	    }
	  break;

	case CONTINUE_STMT:
	  continue_stmt_solution2 (&node);
	  break;
	  
	case BREAK_STMT:
	  break_stmt_solution2 (&node);
	  break;
	  
	default:
	  break;
	}
      node = NEXT_STMT (node);
    }
}

/* This function transforms all BREAK_STMTs from NODE and all BREAK_STMTs and 
   CONTINUE_STMTs from its nested loops.  We call this function only in special 
   cases when we have no information about a potential outer loop to which a 
   CONTINUE_STMT could be related.  This is the reason to keep it static.  */

static void
break_elimination (node)
     tree node;
{
  while (node && node != error_mark_node)
    {
      switch (TREE_CODE (node))
	{
	case COMPOUND_STMT:
	  PATH_PUSH (node);
	  break_elimination (COMPOUND_BODY (node));
	  PATH_POP ();
	  break;

	case FOR_STMT:
	  dchain_build_scope (&FOR_BODY (node));
	  PATH_PUSH (node);
	  break_continue_elimination_rec (FOR_BODY (node));
	  PATH_POP ();
	  break;

	case WHILE_STMT:
	  dchain_build_scope (&WHILE_BODY (node));
	  PATH_PUSH (node);
	  break_continue_elimination_rec (WHILE_BODY (node));
	  PATH_POP ();
	  break;

	case DO_STMT:
	  dchain_build_scope (&DO_BODY (node));
	  PATH_PUSH (node);
	  break_continue_elimination_rec (DO_BODY (node));
	  PATH_POP ();
	  break;

	case SWITCH_STMT:
	  /* Transform this node into an IF_STMT, since a SWITCH_STMT contains 
	     BREAK_STMTs that aren't of the same type as those we'll eliminate.
	     This transformation eliminate BREAK_STMTs that refers to a switch.  */
	  switch_to_if (node);
	  break;
	  
	case IF_STMT:
	  dchain_build_scope (&THEN_CLAUSE (node));
	  PATH_PUSH (node);
	  break_elimination (THEN_CLAUSE (node));
	  PATH_POP ();
	  if (ELSE_CLAUSE (node))
	    {
	      dchain_build_scope (&ELSE_CLAUSE (node));
	      PATH_PUSH (node);
	      break_elimination (ELSE_CLAUSE (node));
	      PATH_POP ();
	    }
	  break;

	case BREAK_STMT:
	  break_stmt_solution2 (&node);
	  break;
	  
	default:
	  break;
	}
      node = NEXT_STMT (node);
    }
}

/* Determines wether the chain NODE contains a nested statement CODE.  */

int
chain_contains_stmt_p (node, code)
     tree node;
     enum tree_code code;
{
  while (node)
    {
      if (TREE_CODE (node) == code)
	return 1;

      switch (TREE_CODE (node))
	{
	case LABEL_STMT:
	  return 1;

	case COMPOUND_STMT:
	  if (chain_contains_stmt_p (COMPOUND_BODY (node), code))
	    return 1;
	  break;

	case FOR_STMT:
	  if (chain_contains_stmt_p (FOR_INIT_STMT (node), code))
	    return 1;
	  if (chain_contains_stmt_p (FOR_BODY (node), code))
	    return 1;
	  break;

	case WHILE_STMT:
	  if (chain_contains_stmt_p (WHILE_BODY (node), code))
	    return 1;
	  break;

	case DO_STMT:
	  if (chain_contains_stmt_p (DO_BODY (node), code))
	    return 1;
	  break;

	case IF_STMT:
	  if (chain_contains_stmt_p (THEN_CLAUSE (node), code))
	    return 1;
	  if (chain_contains_stmt_p (ELSE_CLAUSE (node), code))
	    return 1;
	  break;

	case SWITCH_STMT:
	  if (chain_contains_stmt_p (SWITCH_BODY (node), code))
	    return 1;
	  break;

	case EXPR_STMT:
	case DECL_STMT:
	case RETURN_STMT:
	case SCOPE_STMT:
	case FILE_STMT:
	case GOTO_STMT:
	case ASM_STMT:
	case CASE_LABEL:
	case CONTINUE_STMT:
	case BREAK_STMT:
	default:
	  break;

	}
      node = TREE_CHAIN (node);
    }
  return 0;
}

/* If PARENT is a statement with a condition, then insert CONDITION in its 
   condition as the first operand of an TRUTH_ANDIF_EXPR.  That means that the 
   CONDITION is executed before the evaluation of the rest of loop's condition.  */ 

void 
insert_andif_in_condition (condition, parent)
     tree condition, parent;
{
  switch (TREE_CODE (parent))
    {
    case FOR_STMT:
      if (FOR_COND (parent))
	FOR_COND (parent) = fold (build (TRUTH_ANDIF_EXPR, integer_type_node, 
					 condition, FOR_COND (parent)));
      else
	FOR_COND (parent) = condition;
      break;

    case WHILE_STMT:
      if (WHILE_COND (parent))
	WHILE_COND (parent) = fold (build (TRUTH_ANDIF_EXPR, integer_type_node, 
					   condition, WHILE_COND (parent)));
      else
	WHILE_COND (parent) = condition;
      break;

    case DO_STMT:
      if (DO_COND (parent))
	DO_COND (parent) = fold (build (TRUTH_ANDIF_EXPR, integer_type_node, 
					condition, DO_COND (parent)));
      else
	DO_COND (parent) = condition;
      break;
      
    case IF_STMT:
      if (IF_COND (parent))
	IF_COND (parent) = fold (build (TRUTH_ANDIF_EXPR, integer_type_node, 
					condition, IF_COND (parent)));
      else
	IF_COND (parent) = condition;
      break;
      
    default:
      break;
    }
}

/* If PARENT is a statement with a condition, then insert CONDITION in its 
   condition as the first operand of an TRUTH_ORIF_EXPR.  That means that the 
   CONDITION is executed before the evaluation of the rest of loop's condition.  */ 

void 
insert_orif_in_condition (condition, parent)
     tree condition, parent;
{
  switch (TREE_CODE (parent))
    {
    case FOR_STMT:
      if (FOR_COND (parent))
	FOR_COND (parent) = fold (build (TRUTH_ORIF_EXPR, integer_type_node, 
					 condition, FOR_COND (parent)));
      else
	FOR_COND (parent) = condition;
      break;

    case WHILE_STMT:
      if (WHILE_COND (parent))
	WHILE_COND (parent) = fold (build (TRUTH_ORIF_EXPR, integer_type_node, 
					   condition, WHILE_COND (parent)));
      else
	WHILE_COND (parent) = condition;
      break;
      
    case DO_STMT:
      if (DO_COND (parent))
	DO_COND (parent) = fold (build (TRUTH_ORIF_EXPR, integer_type_node, 
					condition, DO_COND (parent)));
      else
	DO_COND (parent) = condition;
      break;
      
    case IF_STMT:
      if (IF_COND (parent))
	IF_COND (parent) = fold (build (TRUTH_ORIF_EXPR, integer_type_node, 
					condition, IF_COND (parent)));
      else
	IF_COND (parent) = condition;
      break;
      
    default:
      break;
    }
}


/* Solution 2: DATA_CONTROLED_FLOW
   use logical variables to decide wether the code following 
   the CONTINUE_STMT in the parent block will be executed or not.
   This solution avoids to expand the code, but the control flow 
   is no more explicit since the control depends on the logical 
   variables : we transformed the control flow in a data flow.
   This is the solution adopted by the McCAT compiler.  

   Example : "while (c) {s1; s2; continue; s3;}"
   Calling continue_stmt_solution2 (continue);
   transforms the previous while into the following :
   "while (c) {s1; s2;}"  if "s3;" doesn't contain LABEL_STMTs (entry points),
   "while (c) {s1; s2; if (1 == 0) {s3;}"  otherwise.  
   
   Example : "while (c1) {s1; if (c2) {s2; continue; s3;} s4;}"
   Calling continue_stmt_solution2 (continue);
   transforms the previous while into the following :
   "while (c1) {int t=0; s1; if (c2) {s2; t=1;} if (t==0) {s4;}}" 
   if there are no entry points in s3,
   "while (c1) {int t=0; s1; if (c2) {s2; t=1; if (1==0) {s3;}} if (t==0) {s4;}}" 
   if there's an entry point in s3.  */

static void 
continue_stmt_solution2 (stmt)
     tree *stmt;
{
  int one_if = 0;
  PATH_TYPE related_parent;
  tree related_loop;
  
  /* Search the related loop statement.  */
  {
    related_parent = PATH_TOP;
    do {
      related_parent = PATH_PARENT (related_parent);
      related_loop = PATH_STMT (related_parent);
    } while (TREE_CODE (related_loop) == COMPOUND_STMT
	     || (TREE_CODE (related_loop) == IF_STMT && (one_if = 1)));
  }
  
  /* Move the flow down to the end of the related_loop's body.  */
  if (one_if == 0)
    /* The CONTINUE_STMT is unconditional.  The logical variable is always true.  
       That will allow outward_motion to eliminate dead code in some cases.  */
    {
      tree prev;
      prev = PREV_STMT (*stmt);

      /* Move the flow out up to the end of the related_loop's body. 
	 Stop before we move the flow out of related_parent : that's a CONTINUE_STMT.  */
      flow_out_of (/* Exit the loop's body, don't exit the loop.  */ PATH_CHILD (related_parent), 
		   PATH_TOP, 
		   /* Delete the CONTINUE_STMT.  */ prev,
		   /* Allow code elimination.  */ integer_one_node, 
		   /* Updating function */ update_break_paths);
      *stmt = prev;
    }
  else
    /* The CONTINUE_STMT is conditional.  */
    {
      tree lvar, lvar_one;
      
      /* Initialise a logical variable to false at the beginning of the loop.  */
      lvar = create_tmp_var (integer_type_node);
      DECL_INITIAL (lvar) = integer_zero_node;
      
      /* Replace the CONTINUE_STMT by "lvar=1".  */
      lvar_one = build (MODIFY_EXPR, integer_type_node, lvar, 
			integer_one_node);
      lvar_one = build_stmt (EXPR_STMT, lvar_one);
      dchain_replace_stmt (*stmt, lvar_one);
      *stmt = lvar_one;
      
      /* Move the flow down to the end of the related_loop's body. 
	 Stop before we move the flow out of related_parent : that's a CONTINUE_STMT.  */
      flow_out_of (PATH_CHILD (related_parent), PATH_TOP, lvar_one, lvar, update_break_paths);
    }
}

/* Remove FOR_EXPR of a FOR_STMT.  */
#define REMOVE_FOR_EXPR(NODE) do {                                             \
  if (TREE_CODE (NODE) == FOR_STMT && FOR_EXPR (NODE))                         \
    {                                                                          \
      tree reeval;                                                             \
      reeval = build_stmt (EXPR_STMT, FOR_EXPR (NODE));                        \
      insert_before_continue_end (reeval, FOR_BODY (NODE), STMT_LINENO (NODE));\
      FOR_EXPR (NODE) = NULL_TREE;                                             \
    }                                                                          \
} while (0)

/* Solution 2: DATA_CONTROLED_FLOW
   use logical variables to decide wether the code following 
   the BREAK_STMT in the parent block will be executed or not.
   This solution avoids to expand the code, but the control flow 
   is no more explicit since the control depends on the logical 
   variables : we transformed the control flow in a data flow.
   This is the solution adopted by the McCAT compiler.  

   Example : "while (c1) {s1; if (c2) {s2; break_stmt; s3;} s4;}"
   Calling break_stmt_solution2 (break_stmt);
   transforms the previous while into the following :
   "int t=0; while (t==0 && c1) {s1; if (c2) {s2; t=1;} if (t==0) {s4;}}" 
   if there are no entry points in s3,
   "int t=0; while (t==0 && c1) {s1; if (c2) {s2; t=1; if (1==0) {s3;}} if (t==0) {s4;}}" 
   if there's an entry point in s3.  */

static void 
break_stmt_solution2 (stmt)
     tree *stmt;
{
  int one_if = 0;
  PATH_TYPE related_parent;
  tree related_loop;
  tree lvar, lvar_one;

  /* Search the related loop statement.  */
  {
    related_parent = PATH_TOP;
    do {
      related_parent = PATH_PARENT (related_parent);
      related_loop = PATH_STMT (related_parent);
    } while (TREE_CODE (related_loop) == COMPOUND_STMT
	     || (TREE_CODE (related_loop) == IF_STMT && (one_if = 1)));
  }
    
  /* Replace the BREAK_STMT by "lvar=1;".  */
  {
    /* Initialise a logical variable to false in the function's scope.  */
    lvar = create_tmp_var (integer_type_node);
    DECL_INITIAL (lvar) = integer_zero_node;
    
    /* Replace the BREAK_STMT by "lvar=1".  */
    lvar_one = build (MODIFY_EXPR, integer_type_node, lvar, 
		      integer_one_node);
    lvar_one = build_stmt (EXPR_STMT, lvar_one);
    dchain_replace_stmt (*stmt, lvar_one);
    *stmt = lvar_one;
  }
  
  if (one_if == 0)
    /* The BREAK_STMT is unconditional.  The logical variable is always true.  
       That will allow outward_motion to eliminate dead code in some cases.  */
    {
      flow_out_of (PATH_CHILD (related_parent), PATH_TOP, lvar_one, 
		   integer_one_node, update_break_paths);

      /* Remove the FOR_EXPR, since when we break a loop we don't want to 
	 reevaluate it again.  After the break elimination, we exit the FOR_STMT 
	 after the evaluation of its FOR_COND.  If the loop contains a BREAK_STMT, 
	 then we evaluate the FOR_EXPR before exiting the FOR_STMT, and that is
	 not the exact semantic of a BREAK_STMT.  */
      REMOVE_FOR_EXPR (related_loop);

      /* Move the flow out of related_loop.  */
      outward_motion (related_loop, NULL_TREE, lvar, update_break_paths);
    }
  else
    /* The BREAK_STMT is conditional.  */
    flow_out_of (related_parent, PATH_TOP, lvar_one, lvar, update_break_paths);
    
  /* Reinitialize to false the logical variable once we have left the loop.  */
  if (PREV_STMT (related_loop) && NEXT_STMT (related_loop))
    /* The given related_loop is in a statement chain : it is not a fake loop.  */
    dchain_insert_stmt_after 
      (related_loop, build_stmt (EXPR_STMT, 
				 build_modify_expr (lvar, NOP_EXPR, 
						    integer_zero_node)));
}

/* If the LVAR is true, then LAST_EXECUTED is the last statement to be executed 
   in the INNER structure (that can be an IF_STMT, WHILE_STMT, ... ).  The 
   control is moved out of the OUTER structure under the control of LVAR.  
   PATH_TYPE must be under the same implementation as the PATH defined above :
   a TREE_LIST and the inner element is the first element in the list.  */

void 
flow_out_of (outer, inner, last_to_be_executed, lvar, update_paths_fnct)
     PATH_TYPE outer, inner;
     tree last_to_be_executed, lvar;
     void (*update_paths_fnct) PARAMS ((tree, tree, tree, tree));
{
  do {
    /* Remove the FOR_EXPR, since when we break a loop we don't want to 
       reevaluate its FOR_EXPR again.  After the break elimination, 
       we exit the FOR_STMT after the evaluation of its FOR_COND.  
       If the loop contains a BREAK_STMT, then we evaluate the FOR_EXPR before
       exiting the FOR_STMT, and that is not the exact semantic of a BREAK_STMT.  */
    REMOVE_FOR_EXPR (PATH_STMT (inner));
    
    outward_motion (PATH_STMT (inner), last_to_be_executed, lvar, update_paths_fnct);
    
    last_to_be_executed = PATH_STMT (inner);

  } while (PATH_STMT (inner) != PATH_STMT (outer)
	   && (inner = PATH_PARENT (inner)));
}

#undef REMOVE_FOR_EXPR

/* Make the control flow exit from a statement PARENT, by transforming control
   flow into data controlled flow.  
   LAST_EXECUTED is the last statement that the caller wants to be executed in 
   the current scope when the value of the logical variable LVAR becomes true.  
   If the logical variable is 'integer_one_node', then the caller expects that 
   the code following LAST_EXECUTED should never be executed.  If there are no 
   entry points (LABEL_STMTs) between LAST_EXECUTED and the end of the scope, 
   then we can eliminate dead code, else we insert the code following 
   LAST_EXECUTED in an 'if (0)' statement.  

   Example :  "while (c) {s1; s2; s3; s4; s5;}"
   Calling outward_motion (WHILE_BODY (while), s3, integer_one_node) 
   produces the following statement chain :
   "while (c) {s1; s2; s3;}"  if "s4; s5;" doesn't contain LABEL_STMTs.
   "while (c) {s1; s2; s3; if (1==0) {s4; s5;}}"  otherwise.

   Example :  "while (c) {s1; s2;}"
   Calling outward_motion (while, NULL_TREE, lvar);
   produces the following statement chain :
   "while (lvar == 0 && c) {s1; s2;}".  */

static void 
outward_motion (parent, last_executed, lvar, update_paths_fnct)
     tree parent, last_executed, lvar;
     void (*update_paths_fnct) PARAMS ((tree, tree, tree, tree));
{
  if (!parent)
    abort ();

  switch (TREE_CODE (parent))
    {
    case COMPOUND_STMT:
      /* Move the control flow at the end of the COMPOUND_BODY under the control of lvar.  */
      {
	if (TREE_CODE (last_executed) == SCOPE_STMT 
	    && SCOPE_END_P (last_executed))
	  /* LAST_EXECUTED is an end scope : nothing to do.  */
	  return;
	
	if (lvar == integer_one_node)
	  /* The caller expects  'if (1 == 0) {rest_of_code;}' to be built.
	     For removing dead code we have to determine wether the rest of 
	     code contains LABEL_STMTs that are potential entry points.
	     These extra checking can be removed if we run this function 
	     after the goto elimination.  */
	  if (!chain_contains_stmt_p (last_executed, LABEL_STMT))
	    /* The COMPOUND_BODY doesn't contain LABEL_STMTs after LAST_EXECUTED, 
	       so we can safely remove this code.  */
	    {
	      /* FIXME : Maybe we delete some GOTO_STMTs without deleting them
		 from the GOTO_LIST... */
	      dchain_delete_stmts (last_executed, tree_last (last_executed));
	      return;
	    }

	/* Transform the chain <LAST_EXECUTED; rest_of_code;>
	   into <LAST_EXECUTED; if (lvar==0) {rest_of_code;}>.  */
	eliminate_with_if (last_executed, tree_last (last_executed), lvar, 
			   update_paths_fnct);
	break;
      }
      
    case FOR_STMT:
    case WHILE_STMT:
    case DO_STMT:
      {
	/* Insert the stop condition in the loop condition.  */
	insert_andif_in_condition 
	  (build (EQ_EXPR, integer_type_node, lvar, integer_zero_node), parent);
	break;
      }
      
    case IF_STMT:
      {
	/* Nothing to do.  */
	break;
      }
	
    default:
      /* Nothing falls down here.  */
      abort ();
      break;
    }
}

/* Update all paths used in the break_elimination pass.  In fact there's nothing 
   to do since we work only on a single path top down, whereas in goto elimination
   we have to keep up to date a set of paths.  */

static void 
update_break_paths (st1, st2, begin_stmt, end_stmt)
     tree st1, st2, begin_stmt, end_stmt;
{
}

/* Transforms a SWITCH_STMT in a nested IF_STMT.  
   STMT is supposed to be a SWITCH_STMT.  */

static void 
switch_to_if (stmt)
     tree stmt;
{
  tree switch_body;
  tree open_scope, close_scope;
  tree first_case_label;
  tree if_stmt;
  
  if (TREE_CODE (stmt) != SWITCH_STMT)
    abort ();

  switch_body = SWITCH_BODY (stmt);
  open_scope = COMPOUND_BODY (switch_body);
  close_scope = tree_last (open_scope);
  first_case_label = open_scope;
  
  /* Search the first CASE_LABEL from the SWITCH_STMT.  */
  while (first_case_label && TREE_CODE (first_case_label) != CASE_LABEL)
    first_case_label = NEXT_STMT (first_case_label);
  
  if (first_case_label == NULL_TREE)
    /* There aren't CASE_LABELs in this SWITCH_STMT.  
       We have to delete the SWITCH_STMT.  */
    {
      dchain_delete_stmts (PREV_STMT (stmt), NEXT_STMT (stmt));
      return;
    }

  /* Step 0: Delete all unreachable statements from the last declaration up to 
     the first case_label.  Maybe we have to warn here that there's dead code 
     in a switch?  */
  {
    tree last_decl;
    
    last_decl = open_scope;
    while (TREE_CHAIN (last_decl) && TREE_CODE (TREE_CHAIN (last_decl)) == DECL_STMT)
      last_decl = TREE_CHAIN (last_decl);
    
    dchain_delete_stmts (last_decl, first_case_label);
  }
  
  /* Step 1: Include all statements of a CASE_LABEL in a COMPOUND_STMT.  */
  construct_body_for_case_labels (first_case_label);
  
  /* Step 2: Copy statements from following CASE_LABELs : that simulates the 
     fall through mechanism.  Eliminate BREAK_STMTs and dead code.  */
  complete_body_of_case_labels (first_case_label);
  
  /* Step 3: Reorder CASE_LABELs in the SWITCH_STMT.  The minimum reorder is to 
     place the default case at the last position.  */
  reorder_case_labels (first_case_label);
  
  /* Replace the cursor on the first_case_label.  */
  first_case_label = open_scope;
  while (TREE_CODE (first_case_label) != CASE_LABEL)
    first_case_label = NEXT_STMT (first_case_label);
  
  /* Step 4: Creates an IF_STMT for each CASE_LABEL : 
     'if (switch_cond == case_number)'.  */
  if_stmt = cases_to_if (SWITCH_COND (stmt), first_case_label);
    
  /* Step 5: Replace the SWITCH_STMT by its SWITCH_BODY in which we find all 
     local definitions and the constructed IF_STMT.  */
  dchain_insert_chain (PREV_STMT (first_case_label), close_scope, if_stmt, if_stmt);
  eliminate_compound_stmts (COMPOUND_BODY (switch_body));
  dchain_replace_stmt (stmt, switch_body);
}

/* NODE is the first CASE_LABEL of a SWITCH_STMT.  This function constructs a 
   COMPOUND_STMT around the body of every CASE_LABEL of a SWITCH_STMT.  */

static void
construct_body_for_case_labels (node)
     tree node;
{
  tree case_label = node;

  /* For each statement in SWITCH_BODY,  */
  while (node != NULL_TREE && NEXT_STMT (node) != NULL_TREE)
    {
      if (TREE_CODE (node) == CASE_LABEL
	  && TREE_CODE (NEXT_STMT (node)) == CASE_LABEL)
	/* The CASE_LABEL is followed by another CASE_LABEL.  
	   Transform it into CASE_LABEL; COMPOUND_STMT; CASE_LABEL; with a 
	   COMPOUND_STMT empty.  It will be filled with code copied from the 
	   body of the next CASE_LABEL later in complete_body_of_case_labels ().  */
	{
	  tree compound_stmt;
	  
	  /* Construct an empty COMPOUND_STMT.  */
	  compound_stmt = NULL_TREE;
	  dchain_build_scope (&compound_stmt);
	  dchain_insert_stmt_after (node, compound_stmt);
	  
	  /* Next iteration.  */
	  node = case_label = NEXT_STMT (compound_stmt);
	  continue;
	}

      if (TREE_CODE (node) == CASE_LABEL
	  && TREE_CODE (NEXT_STMT (node)) == COMPOUND_STMT
	  && (TREE_CODE (NEXT_STMT (NEXT_STMT (node))) == CASE_LABEL
	      || NEXT_STMT (NEXT_STMT (NEXT_STMT (node))) == NULL_TREE))
	/* The case has already a COMPOUND_STMT : don't construct another one.  */
	{
	  node = case_label = NEXT_STMT (NEXT_STMT (node));
	  continue;
	}
      
      if (TREE_CODE (NEXT_STMT (node)) == CASE_LABEL
	  || NEXT_STMT (NEXT_STMT (node)) == NULL_TREE)
	/* The next statement ends the CASE_LABEL's body.  */
	{
	  tree compound_stmt, next;
	  
	  /* Store a pointer to the next statement.  */
	  next = NEXT_STMT (node);
	  
	  /* Construct a COMPOUND_STMT.  */
	  compound_stmt = NULL_TREE;
	  dchain_build_scope (&compound_stmt);
	  
	  /* Rewire.  */
	  {
	    tree open_scope, close_scope;
	    
	    open_scope = COMPOUND_BODY (compound_stmt);
	    close_scope = NEXT_STMT (open_scope);
	    
	    /* Insert statements that belongs to the CASE_LABEL in its body.  */ 
	    dchain_insert_chain (open_scope, close_scope, 
				 NEXT_STMT (case_label), node);
	    
	    dchain_insert_chain (case_label, next, compound_stmt, compound_stmt);
	  }
	  
	  /* Next iteration.  */
	  node = case_label = next;
	  continue;
	}
      else
	/* Next iteration.  */
	node = NEXT_STMT (node);
    }
}

/* The following macros access a regular SWITCH_BODY : each CASE_LABEL has a 
   body composed of a COMPOUND_STMT.  Thus we have a chain under the following
   format: CASE_LABEL; COMPOUND_STMT; CASE_LABEL; ... COMPOUND_STMT;.  */
#define NEXT_CASE(NODE) NEXT_STMT (NEXT_STMT (NODE))
#define PREV_CASE(NODE) PREV_STMT (PREV_STMT (NODE))
#define FOREACH_CASE(NODE)  for (; TREE_CODE (NODE) != SCOPE_STMT; (NODE) = NEXT_CASE (NODE))
#define FOREACH_CASE_BOTTOM_UP(NODE)  for (; NODE && TREE_CODE (NODE) == CASE_LABEL; NODE = PREV_CASE (NODE))
#define LAST_CASE(NODE) for (; TREE_CODE (NEXT_CASE (NODE)) == CASE_LABEL; (NODE) = NEXT_CASE (NODE))
#define CASE_BODY(NODE) NEXT_STMT (NODE)

/* NODE is the first CASE_LABEL of a SWITCH_STMT.  This function suposes that
   SWITCH_BODY is composed as described by the following schema :
   {CASE_LABEL; COMPOUND_STMT; CASE_LABEL; COMPOUND_STMT; ... COMPOUND_STMT;}
   every CASE_LABEL is followed by a COMPOUND_STMT.  
   This function completes the body of a CASE_LABEL by copying statements in 
   order to simulate the fall through mechanism.  

   FIXME: An optimized way to do this job is bottom-up : begin the completion of
   bodies from the last CASE_LABEL and follow the PREV_STMT chain.  
   Apply the break and dead code elimination for each CASE_LABEL body before 
   copying the code for the PREV_CASE.  

   The transformation is done bottom-up.
   Example of the execution of this algorithm :
   switch ()
   {
   case 1: {b1;}
   case 2: {b2;}
   case 3: {b3;}
   }
   0. Nothing to copy : "case 3" is the last in SWITCH_BODY.
   1. Apply break/dead code elimination on {b3;} gives {a3;}.
   switch ()
   {
   case 1: {b1;}
   case 2: {b2;}
   case 3: {a3;}
   }
   2. Copy a3; after b2;.
   switch ()
   {
   case 1: {b1;}
   case 2: {b2; a3;}
   case 3: {a3;}
   }
   3. Apply break/dead code elimination on {b2; a3;} gives {a2;}.
   switch ()
   {
   case 1: {b1;}
   case 2: {a2;}
   case 3: {a3;}
   }
   4. Copy a2; after b1;.
   switch ()
   {
   case 1: {b1; a2;}
   case 2: {a2;}
   case 3: {a3;}
   }
   5. Apply break/dead code elimination on {b1; a2;} gives {a1;}.
   switch ()
   {
   case 1: {a1;}
   case 2: {a2;}
   case 3: {a3;}
   }
   6. End.  */

static void
complete_body_of_case_labels (node)
     tree node;
{
  LAST_CASE (node);

  /* For each CASE_LABEL in SWITCH_BODY,  */
  FOREACH_CASE_BOTTOM_UP (node)
    {
      tree last_stmt, next_case;
      tree close_scope;
      
      close_scope = tree_last (COMPOUND_BODY (TREE_CHAIN (node)));
      last_stmt = PREV_STMT (close_scope);
      next_case = NEXT_CASE (node);
      
      if (TREE_CODE (next_case) == CASE_LABEL)
	/* Copy the CASE_BODY of the next CASE_LABEL after the last 
	   statement of the body of this CASE_LABEL.  */
	{
	  dchain_insert_stmt_after (last_stmt, 
				    dchain_deep_copy_node (CASE_BODY (next_case)));

	  /* Delete all LABEL_STMTs we've copied.  This way we avoid duplicated
	     label definitions.  That is what we could call following the same
	     terminology as McGill : 'avoid a LABEL_STMT capture'.  */
	  dchain_delete_labels (COMPOUND_BODY (NEXT_STMT (last_stmt)));
	}
      /* else
	 This is the last CASE_LABEL of the SWITCH_BODY : nothing to copy.  */
      
      /* Eliminate BREAK_STMTs and dead code.  */
      {
	tree fake_do, fake_body;

	fake_body = CASE_BODY (node);
	
	/* Construct a fake DO_STMT whose body is the COMPOUND_STMT of this 
	   CASE_LABEL.  */
	fake_do = build_stmt (DO_STMT, integer_zero_node, fake_body);
	
	/* 'break_elimination' is called only since we cannot eliminate
	   CONTINUE_STMTs that refers to an outer loop.  CONTINUE_STMTs are 
	   eliminated once the SWITCH_STMT was completely transformed by 
	   reruning the pass once again on the produced IF_STMT.  */
	break_elimination (fake_do);
      }
    }
}

/* NODE is the first CASE_LABEL statement of a SWITCH_BODY.  
   This function suposes that a SWITCH_BODY is composed as described by the 
   following schema :
   {CASE_LABEL; COMPOUND_STMT; CASE_LABEL; COMPOUND_STMT; ... COMPOUND_STMT;}
   every CASE_LABEL is followed by a COMPOUND_STMT.  Moreover it supposes that
   there's no fall through between cases, and that all BREAK_STMTs were deleted.
   This function reorder CASE_LABELs in the SWITCH_BODY. 
   It performs for the moment the minimum : ensure that the DEFAULT case is the
   last case in the SWITCH_BODY.  
   Other optimisations based on the reordering of cases could be implemented 
   or rewritten (from switch_stack)*/

static void 
reorder_case_labels (node)
     tree node;
{
  /* For each CASE_LABEL in SWITCH_BODY, search the DEFAULT case.  */
  FOREACH_CASE (node)
    {
      if (CASE_LOW (node) == NULL_TREE 
	  && CASE_HIGH (node) == NULL_TREE)
	/* That's the DEFAULT case : move it at the end of the SWITCH_BODY.  */
	{
	  tree default_case;
	  default_case = node;

	  if (TREE_CODE (NEXT_CASE (default_case)) == SCOPE_STMT)
	    /* The DEFAULT case is well placed at the end of the SWITCH_BODY.  */
	    break;
	  
	  /* Delete the default_case from its position.  */
	  dchain_delete_stmts (PREV_STMT (default_case), NEXT_CASE (default_case));
	  
	  /* Search the last CASE_LABEL in this SWITCH_STMT.  */
	  LAST_CASE (node);
	  
	  node = CASE_BODY (node);
	  
	  /* Add the default_case's body at the end of the SWITCH_BODY.  */
	  dchain_insert_stmt_after (node, CASE_BODY (default_case));
	  
	  /* Add the default_case label.  */
	  dchain_insert_stmt_after (node, default_case);
	  
	  /* That's all.  */
	  break;
	}
    }
}

/* NODE is a CASE_LABEL of a SWITCH_BODY.  SVAR is the SWITCH_COND. 
   This function transforms all cases of a SWITCH_STMT into a nested IF_STMT.
   This function suposes that a SWITCH_BODY is composed as described by the
   following schema :
   {CASE_LABEL; COMPOUND_STMT; ... default: COMPOUND_STMT;}
   every CASE_LABEL is followed by a COMPOUND_STMT, there's no fall through 
   between cases, all BREAK_STMTs were deleted, and the DEFAULT case is the last
   in the SWITCH_BODY.  */

static tree
cases_to_if (svar, node)
     tree svar, node;
{
  tree then_clause, else_clause;
  tree low, high;
  tree cond;

  if (TREE_CODE (node) == SCOPE_STMT)
    /* The previous CASE_LABEL was the last one.  That means that this 
       SWITCH_STMT doesn't contain a DEFAULT case.  */
    return NULL_TREE;
    
  low = CASE_LOW (node);
  high = CASE_HIGH (node);
  if (low && high)
    /* Range.  */
    {
      tree le, ge;
      
      /* Construct the condition : (low >= svar && svar <= high).  */
      ge = build (GE_EXPR, integer_type_node, svar, low);
      le = build (LE_EXPR, integer_type_node, svar, high);
      cond = build (TRUTH_ANDIF_EXPR, integer_type_node, le, ge);
    }
  else if (low)
    /* A normal case.  Construct the condition : (svar == low).  */
    cond = build (EQ_EXPR, integer_type_node, svar, low);
  else
    /* The default case : we're on the last case of the SWITCH_BODY.  */
    {
      else_clause = CASE_BODY (node);

      /* The else_clause becomes part of the IF_STMT : remove its pointers to 
	 the statemtent chain.  */
      NEXT_STMT (else_clause) = NULL_TREE;
      PREV_STMT (else_clause) = NULL_TREE;

      return else_clause;
    }

  then_clause = CASE_BODY (node);
  else_clause = cases_to_if (svar, NEXT_CASE (node));
  
  /* The then_clause becomes part of the IF_STMT : remove its pointers to 
     the statemtent chain.  */
  NEXT_STMT (then_clause) = NULL_TREE;
  PREV_STMT (then_clause) = NULL_TREE;
  
  /* Be sure that the constructed else_clause has a scope.  */
  dchain_build_scope (&else_clause);
  
  /* Construct an IF_STMT.  */
  return build_stmt (IF_STMT, cond, then_clause, else_clause);
}

/* Eliminate COMPOUND_STMTs from the chain pointed by STMT.  Only COMPOUND_STMTs 
   that doesn't contain variable declarations are opened and their statements 
   are chained at STMT's level.  This function eliminate IF, WHILE, DO and FOR
   statements if the function eval_bool_condition is able to determine 
   condition's valuation.  */

void
eliminate_compound_stmts (stmt)
     tree stmt;
{
  while (stmt != NULL_TREE)
    {
      switch (TREE_CODE (stmt))
	{
	case COMPOUND_STMT:
	  if (!SCOPE_STMT_BLOCK (COMPOUND_BODY (stmt)))
	    /* This COMPOUND_STMT doesn't contain declarations : open it.  */
	    {
	      tree prev;
	      prev = PREV_STMT (stmt);
	      dchain_insert_chain (prev, NEXT_STMT (stmt),
				   /* First stmt is open_scope : don't copy it.  */
				   NEXT_STMT (COMPOUND_BODY (stmt)),
				   /* Last stmt is close_scope : don't copy it.  */
				   PREV_STMT (tree_last (COMPOUND_BODY (stmt))));
	      stmt = prev;
	    }
	  else 
	    /* Keep the COMPOUND_STMT, but remove unneeded COMPOUND_STMTs from 
	       its body.  */
	    eliminate_compound_stmts (COMPOUND_BODY (stmt));
	  break;

	case FOR_STMT:
	  {
	    if (eval_bool_condition (FOR_COND (stmt)) == EVAL_FALSE 
		&& !chain_contains_stmt_p (FOR_BODY (stmt), LABEL_STMT))
	      /* Eliminate this FOR_STMT.  */
	      dchain_delete_stmts (PREV_STMT (stmt), NEXT_STMT (stmt));
	    else
	      /* Keep the FOR_STMT, but eliminate unneeded COMPOUND_STMTs from
		 its body.  */
	      {
		dchain_build_scope (&FOR_BODY (stmt));
		eliminate_compound_stmts (COMPOUND_BODY (FOR_BODY (stmt)));
	      }
	    break;
	  }

	case WHILE_STMT:
	  {
	    if (eval_bool_condition (WHILE_COND (stmt)) == EVAL_FALSE 
		&& !chain_contains_stmt_p (WHILE_BODY (stmt), LABEL_STMT))
	      /* Eliminate this WHILE_STMT.  */
	      dchain_delete_stmts (PREV_STMT (stmt), NEXT_STMT (stmt));
	    else
	      /* Keep the WHILE_STMT, but eliminate unneeded COMPOUND_STMTs from
		 its body.  */
	      {
		dchain_build_scope (&WHILE_BODY (stmt));
		eliminate_compound_stmts (COMPOUND_BODY (WHILE_BODY (stmt)));
	      }
	    break;
	  }

	case DO_STMT:
	  {
	    if (eval_bool_condition (DO_COND (stmt)) == EVAL_FALSE 
		&& !chain_contains_stmt_p (DO_BODY (stmt), LABEL_STMT))
	      /* Eliminate this DO_STMT : keep only its DO_BODY.  */
	      {
		dchain_replace_stmt (stmt, DO_BODY (stmt));
		stmt = DO_BODY (stmt);
		
		/* Eliminate COMPOUND_STMTs from DO_BODY.  */
		continue;
	      }
	    else
	      /* Keep the DO_STMT, but eliminate unneeded COMPOUND_STMTs from
		 its body.  */
	      {
		dchain_build_scope (&DO_BODY (stmt));
		eliminate_compound_stmts (COMPOUND_BODY (DO_BODY (stmt)));
		break;
	      }
	  }

	case SWITCH_STMT:
	  dchain_build_scope (&SWITCH_BODY (stmt));
	  eliminate_compound_stmts (COMPOUND_BODY (SWITCH_BODY (stmt)));
	  break;

	case IF_STMT:
	  {
	    enum eval_bool evc;
	    evc = eval_bool_condition (IF_COND (stmt));
	    
	    if (evc == EVAL_TRUE 
		&& !chain_contains_stmt_p (ELSE_CLAUSE (stmt), LABEL_STMT))
	      /* Eliminate this IF_STMT : keep only its THEN_CLAUSE.  */
	      {
		dchain_build_scope (&THEN_CLAUSE (stmt));
		dchain_replace_stmt (stmt, THEN_CLAUSE (stmt));
		stmt = THEN_CLAUSE (stmt);
		continue;
	      }
	    else if (evc == EVAL_FALSE 
		     && !chain_contains_stmt_p (THEN_CLAUSE (stmt), LABEL_STMT))
	      /* Eliminate this IF_STMT : keep only its ELSE_CLAUSE.  */
	      {
		dchain_build_scope (&ELSE_CLAUSE (stmt));
		dchain_replace_stmt (stmt, ELSE_CLAUSE (stmt));
		stmt = ELSE_CLAUSE (stmt);
		continue;
	      }
	    else
	      /* Keep both clauses of the IF_STMT.  */
	      {
		dchain_build_scope (&THEN_CLAUSE (stmt));
		dchain_build_scope (&ELSE_CLAUSE (stmt));
		eliminate_compound_stmts (COMPOUND_BODY (THEN_CLAUSE (stmt)));
		eliminate_compound_stmts (COMPOUND_BODY (ELSE_CLAUSE (stmt)));
	      }
	    break;
	  }

	default:
	  /* Nothing to do.  */
	  break;
	}

      /* Next iteration.  */
      stmt = NEXT_STMT (stmt);
    }
}

/* Try to evaluate the boolean value of EXPR.  */

static enum eval_bool 
eval_bool_condition (expr)
     tree expr;
{
  tree f;
  f = fold (expr);
  
  switch (TREE_CODE (f))
    {
    case INTEGER_CST:
      if (TREE_INT_CST_LOW (f) == 0)
	return EVAL_FALSE;
      else
	return EVAL_TRUE;
      
    case NE_EXPR:
    case EQ_EXPR:
      {
	tree op0, op1;
	op0 = TREE_OPERAND (f, 0);
	op1 = TREE_OPERAND (f, 1);
	
	if (TREE_CODE (op0) == INTEGER_CST 
	    && TREE_CODE (op1) == INTEGER_CST)
	  /* Both operands are integers.  */
	  {
	    int i0, i1;
	    i0 = TREE_INT_CST_LOW (op0);
	    i1 = TREE_INT_CST_LOW (op1);
	    
	    if (TREE_CODE (f) == EQ_EXPR)
	      {
		if (i0 == i1)
		  return EVAL_TRUE;
		else 
		  return EVAL_FALSE;
	      }
	    else /* NE_EXPR */
	      {
		if (i0 != i1)
		  return EVAL_TRUE;
		else
		  return EVAL_FALSE;
	      }
	  }
	else 
	  return EVAL_DONT_KNOW;
      }
      
    default:
      return EVAL_DONT_KNOW;
    }
}
