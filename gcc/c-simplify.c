/* Tree lowering pass.  This pass simplifies the tree representation built
   by the C-based front ends.  The structure of simplified, or
   language-independent, trees is dictated by the grammar described in this
   file.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Lowering of expressions contributed by Sebastian Pop <s.pop@laposte.net>
   Re-written to support lowering of whole function trees, documentation
   and miscellaneous cleanups by Diego Novillo <dnovillo@redhat.com>

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
#include "tree.h"
#include "errors.h"
#include "c-tree.h"
#include "c-common.h"
#include "diagnostic.h"

/* {{{ Grammar for SIMPLE trees.  */

/** The simplification pass converts the language-dependent trees
    (ld-trees) emitted by the parser into language-independent trees
    (li-trees) that are the target of SSA analysis and transformations.  

    Language-independent trees are based on the SIMPLE intermediate
    representation used in the McCAT compiler framework:

    "Designing the McCAT Compiler Based on a Family of Structured
    Intermediate Representations,"
    L. Hendren, C. Donawa, M. Emami, G. Gao, Justiani, and B. Sridharan,
    Proceedings of the 5th International Workshop on Languages and
    Compilers for Parallel Computing, no. 757 in Lecture Notes in
    Computer Science, New Haven, Connecticut, pp. 406-420,
    Springer-Verlag, August 3-5, 1992.

    http://www-acaps.cs.mcgill.ca/info/McCAT/McCAT.html

                                SIMPLE C Grammar

    ----------------------------------------------------------------------

    The BNF rules for SIMPLE C

      Statements

      all_stmts
	      : stmtlist stop_stmt
	      | stmtlist

      stmtlist
	      : stmtlist stmt
	      | stmt

      stmt
	      : compstmt
	      | expr ';'
	      | IF '(' condexpr ')' stmt
	      | IF '(' condexpr ')' stmt ELSE stmt
	      | WHILE '(' condexpr ')' stmt
	      | DO stmt WHILE '(' condexpr ')'
	      | FOR '('exprseq ';' condexpr ';'exprseq ')' stmt
	      | SWITCH '(' val ')' casestmts
	      | ';'

      compsmt
	      : '{' all_stmts '}'
	      | '{' '}'
	      | '{' decls all_stmts '}'
	      | '{' decls '}'

      Declarations

      All the possible C declarations. The only difference is that in SIMPLE the
      declarations are not allowed to have initializations in them.

      Expressions

      exprseq
	      : exprseq ',' expr
	      | expr

      stop_stmt
	      : BREAK ';'
	      | CONTINUE ';'
	      | RETURN ';'
	      | RETURN val ';'
	      | RETURN '(' val ')' ';'

      casestmts
	      : '{' cases default'}'
	      | ';'
	      | '{' '}'

      cases
	      : cases case
	      | case

      case
	      : CASE CONST':' stmtlist stop_stmt

      default
	      : DEFAULT ':' stmtlist stop_stmt

      expr
	      : rhs
	      | modify_expr

      call_expr
	      : ID '(' arglist ')'

      arglist
	      : arglist ',' val
	      | val

      modify_expr
	      : varname '=' rhs
	      | '*' ID '=' rhs

      rhs
	      : binary_expr
	      | unary_expr

      unary_expr
	      : simp_expr
	      | '*' ID
	      | '&' varname
	      |call_expr
	      | unop val
	      | '(' cast ')' varname

	      (cast here stands for all valid C typecasts)

      binary_expr
	      : val binop val

      unop
	      : '+'
	      | '-'

      binop
	      : relop
	      | '-'
	      | '+'
	      ....

      relop
	      : '<'
	      | '<='
	      ...

      condexpr
	      : val
	      | val relop val

      simp_expr
	      : varname
	      | CONST

      val
	      : ID
	      | CONST

      varname
	      : arrayref
	      | compref
	      | ID

      arrayref
	      : ID reflist

      reflist
	      : '[' val ']'
	      | reflist '[' val ']'

      idlist
	      : idlist '.' ID
	      | ID

      compref
	      : '(' '*' ID ')' '.' idlist
	      | idlist

     ----------------------------------------------------------------------  */

/* }}} */

/* {{{ Local declarations.  */

static tree simplify_for_stmt PARAMS ((tree, tree, tree, tree));
static tree simplify_while_stmt PARAMS ((tree, tree, tree, tree));
static tree simplify_do_stmt PARAMS ((tree, tree, tree, tree));
static tree new_simplified_if PARAMS ((tree, tree, tree, tree));
static void simplify_switch_stmt PARAMS ((tree, tree *, tree *, tree *, tree));
static tree simplify_decl_stmt PARAMS ((tree, tree *));
static tree simplify_expr PARAMS ((tree, tree *, tree *, tree *, tree));
static tree create_tmp_var PARAMS ((tree, tree *));
static void make_type_writable PARAMS ((tree));
static void tree_build_scope PARAMS ((tree *));
static tree declare_tmp_vars PARAMS ((tree, tree));
static void add_tree PARAMS ((tree, tree *));
static int keep_stmt_p PARAMS ((tree));
static void insert_before_continue_end PARAMS ((tree, tree, int));
static tree insert_before_continue PARAMS ((tree, tree));
static tree copy_stmt_chain PARAMS ((tree));
static tree copy_stmt PARAMS ((tree));
static void insert_stmt_chain PARAMS ((tree *, tree));
static tree update_line_number PARAMS ((tree, int));
static int simplified_p PARAMS ((tree));
static int simplified_rec_p PARAMS ((tree, int));

/* }}} */

/** {{{ simplify_stmt()
  
    Entry point for the tree lowering pass.  Recursively scan
    STMT and convert it to a SIMPLE tree.

    SCOPE points to the scope enclosing STMT.  */

void 
simplify_stmt (stmt, scope)
     tree stmt;
     tree scope;
{
  tree next, prev, before, after, new_vars;
  
  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return;

  /* BEFORE and AFTER are tree chains that contain the
     side-effects of the simplified tree.  For instance, given the
     expression tree:

     		c = a + b++;

     After simplification, the tree will be re-written as:

     		t1 = a + b;	<-- BEFORE
     		c = t1;
     		b = b + 1;	<-- AFTER

     NEW_VARS is a tree chain holding all the artificial variables 
     declared during simplification.  */

  prev = stmt;
  while (stmt && stmt != error_mark_node)
    {
      before = NULL;
      after = NULL;
      new_vars = NULL;
      
      switch (TREE_CODE (stmt))
	{
	case COMPOUND_STMT:
	  simplify_stmt (COMPOUND_BODY (stmt), scope);
	  
	  /* Prepare for the next iteration.  */
	  next = TREE_CHAIN (stmt);
	  prev = stmt;
	  stmt = next;
	  continue;
	  
	case FOR_STMT:
	  prev = simplify_for_stmt (prev, stmt, TREE_CHAIN (stmt), scope);
	  stmt = TREE_CHAIN (prev);
	  continue;
	  
	case WHILE_STMT:
	  prev = simplify_while_stmt (prev, stmt, TREE_CHAIN (stmt), scope);
	  stmt = TREE_CHAIN (prev);
	  continue;

	case DO_STMT:
	  prev = simplify_do_stmt (prev, stmt, TREE_CHAIN (stmt), scope);
	  stmt = TREE_CHAIN (prev);
	  continue;

	case IF_STMT:
	  next = TREE_CHAIN (stmt);
	  tree_build_scope (&THEN_CLAUSE (stmt));
	  tree_build_scope (&ELSE_CLAUSE (stmt));
	  simplify_stmt (THEN_CLAUSE (stmt), scope);
	  simplify_stmt (ELSE_CLAUSE (stmt), scope);
	  /* Construct a simplified version of this IF_STMT.  */
	  stmt = new_simplified_if (IF_COND (stmt), 
				    THEN_CLAUSE (stmt),
				    ELSE_CLAUSE (stmt),
				    scope);
	  TREE_CHAIN (prev) = stmt;
	
	  /* Insert the simplified IF_STMT in the tree.  */
	  { 
	    /* Place an iterator on the last node in the simplified IF_STMT.  */
	    tree it;
	    it = tree_last (stmt);

	    /* Add the rest of the tree after the generated STMT.  */
	    TREE_CHAIN (it) = next;

	    /* Next iteration.  */
	    prev = it; 
	    stmt = TREE_CHAIN (it);
	  }
	  continue;
	
	case SWITCH_STMT:
	  simplify_switch_stmt (stmt, &before, &after, &new_vars, scope);
	  break;

	case EXPR_STMT:
	  TREE_OPERAND (stmt, 0) = simplify_expr (TREE_OPERAND (stmt, 0), 
						  &before, &after,
						  &new_vars, scope);
	  break;

	case DECL_STMT:
	  /* Simplify all the declarations at once.  */
	  stmt = simplify_decl_stmt (stmt, &after);
	  
	  if (after)
	    /* Insert AFTER in the tree.  */
	    {
	      tree end_after;
	      /* Save a pointer to the next stmt.  */
	      next = TREE_CHAIN (stmt);
	      
	      /* Include the nodes from AFTER list.  */
	      TREE_CHAIN (stmt) = after;
	      
	      /* Update the line number and make PREV points to the last stmt in 
		 AFTER list.  */
	      end_after = update_line_number (after, STMT_LINENO (stmt));
	      
	      /* Chain the end of the AFTER block to the next stmt.  */
	      TREE_CHAIN (end_after) = next;
	    }
	  /* else {
	     no initial values, so nothing to be simplified;
	     }.  */

	  /* Go to the next statement to be simplified.  */
	  prev = stmt;
	  stmt = TREE_CHAIN (stmt);
	  continue;

	case RETURN_STMT:
	  if (TREE_CODE (TREE_TYPE (TREE_TYPE (current_function_decl))) != VOID_TYPE
	      && RETURN_EXPR (stmt))
	    {
	      tree e = RETURN_EXPR (stmt);
	      TREE_OPERAND (e, 1) = simplify_expr (TREE_OPERAND (e, 1),
						   &before, &after,
						   &new_vars, scope);
	    }
	  break;
	  
	case SCOPE_STMT:
	  if (SCOPE_BEGIN_P (stmt))
	    /* Save a pointer to the scope.  */
	    scope = stmt; 
	    	  
	  /* Next iteration.  */
	  prev = stmt;
	  stmt = TREE_CHAIN (stmt);
	  continue;

	case FILE_STMT:
	case LABEL_STMT:
	case GOTO_STMT:
	case ASM_STMT:
	case CASE_LABEL:
	case CONTINUE_STMT:
	case BREAK_STMT:
	  /* Nothing to do.  */
	  break;

	default:
	  prep_stmt (stmt);
	  error ("unhandled statement node in simplify_stmt():");
	  fprintf (stderr, "\n");
	  debug_tree (stmt);
	  fprintf (stderr, "\n");
	  abort ();
	  break;
	}

      /* If we created new temporary variables, create declarations for
	 them in the current scope.  */
      if (new_vars)
	{
	  tree last = declare_tmp_vars (new_vars, scope);

	  /* If the previous statement was a declaration or a scope
	     statement, we need to re-set it to point to the last
	     temporary variable declaration.  Otherwise, they will be
	     overwritten by the next iteration.  */
	  if (TREE_CODE (prev) == DECL_STMT
	      || TREE_CODE (prev) == SCOPE_STMT)
	    prev = last;
	}

      /* There are three tree chains that need to be dealt with: BEFORE,
	 STMT and AFTER.  If they all exist, the program needs to be
	 transformed from:

	 	PREV -> STMT -> NEXT
	 to
	 	PREV -> BEFORE -> STMT -> AFTER -> NEXT

	 Since either of the three may be NULL, we need to consider all the
	 possible variations.  */

      next = TREE_CHAIN (stmt);

      if (before)
	{
	  /* Include the nodes from the BEFORE list.  */
	  TREE_CHAIN (prev) = before;

	  /* Update the line number and make PREV points to the last stmt in 
	     BEFORE list  */
	  prev = update_line_number (before, STMT_LINENO (stmt));
	}
      
      if (keep_stmt_p (stmt))
	{
	  /* Include STMT, if it's worth keeping (i.e., it has not been
	     nullified by the simplification process).  */
	  TREE_CHAIN (prev) = stmt;
	  
	  /* Re-compute PREV.  */
	  prev = stmt;
	}
	
      if (after)
	{
	  /* Include the nodes from the AFTER list.  */
	  TREE_CHAIN (prev) = after;

	  /* Update the line number and make PREV points to the last stmt in 
	     AFTER list.  */
	  prev = update_line_number (after, STMT_LINENO (stmt));
	}

      /* Make sure that PREV is chained to NEXT (just in case it was
	 re-computed above)  */
      TREE_CHAIN (prev) = next;

      /* Next step of the iteration.  */
      stmt = next;
    }
}

/* }}} */

/** {{{  simplify_for_stmt()

    Simplifies the given FOR_STMT.
    Return a pointer to the last statment of the simplified region.  
    This becomes the PREV pointer. The next statement to be simplified
    is the next stmt in the chain.  */

static tree
simplify_for_stmt (prev, stmt, next, scope)
     tree prev, stmt, next, scope;
{
  tree cond, body, loop, last;
  tree new_scope;
  tree init_before = NULL_TREE, init_after = NULL_TREE;
  tree expr_before = NULL_TREE, expr_after = NULL_TREE;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&FOR_BODY (stmt));
  cond = FOR_COND (stmt);
  body = FOR_BODY (stmt);
  new_scope = COMPOUND_BODY (body);
  TREE_CHAIN (prev) = NULL_TREE;

  /* Simplify FOR_INIT_STMT.  */
  if (TREE_OPERAND (FOR_INIT_STMT (stmt), 0))
    {
      tree new_vars = NULL;
      simplify_expr (TREE_OPERAND (FOR_INIT_STMT (stmt), 0), &init_before, 
		     &init_after, &new_vars, scope);
      /* FOR_INIT_STMT (t) = build1 (EXPR_STMT, NULL_TREE, NULL_TREE); */

      /* Insert the simplification chains before the beginning of the loop.  */
      chainon (prev, init_before);
      if (TREE_CODE (TREE_OPERAND (FOR_INIT_STMT (stmt), 0)) == COMPOUND_EXPR)
	chainon (prev, FOR_INIT_STMT (stmt));
      chainon (prev, init_after);
      
      /* If we created new temporary variables, create declarations for
	 them in the current scope.  */
      if (new_vars)
	declare_tmp_vars (new_vars, scope);
      
    }
  
  /* Simplify FOR_EXPR.  */
  if (FOR_EXPR (stmt))
    {
      tree new_scope_vars = NULL;
      simplify_expr (FOR_EXPR (stmt), &expr_before, &expr_after, &new_scope_vars, scope);
      /* FOR_EXPR (t) = NULL_TREE;  */
      
      /* If we created new temporary variables, create declarations for
	 them in the current scope.  */
      if (new_scope_vars)
	declare_tmp_vars (new_scope_vars, new_scope);
    }
    
  if (simplified_p (cond))
    /* The FOR_COND is already simplified, simplify just the FOR_BODY.  */
    {
      tree reeval;

      /* step 1 : Insert a simplified version of 'reeval;' in the loop body.  */
      reeval = chainon (expr_before, expr_after);
      if (reeval)
	insert_before_continue_end (reeval, FOR_BODY (stmt), STMT_LINENO (stmt));
      
      /* Step 2 : Construct a 'while (cond) {body}' statement.  */
      loop = build_stmt (WHILE_STMT, 
			 /* WHILE_COND */ FOR_COND (stmt),
			 /* WHILE_BODY */ NULL_TREE);
      
      /* Step 4 : Initialize the loop body.  */
      simplify_stmt (body, scope);
      WHILE_BODY (loop) = body;
      
      /* Step 5 : Next statement to be processed.  */
      last = chainon (prev, loop);
      last = tree_last (last);
      chainon (loop, next);
      return last;
    }

  /* step 1 : Insert a simplified version of 'reeval; if (!cond) break;' 
     in the loop body.  */
  {
    tree if_stmt, reeval;

    /* Build 'if (cond) {} else break;'.  */
    if_stmt = new_simplified_if (cond, NULL_TREE, build_stmt (BREAK_STMT), 
				 COMPOUND_BODY (FOR_BODY (stmt)));
    
    /* Insert the 'reeval; if_stmt;' before CONTINUE_STMT and at the 
       end of the loop block.  */
    reeval = chainon (expr_before, chainon (expr_after, if_stmt));
    if (reeval)
      insert_before_continue_end (reeval, FOR_BODY (stmt), STMT_LINENO (stmt));
  }
  
  /* Step 2 : Construct a 'while (1!=0) {body}' statement.  For the moment don't 
     initialize the loop body in order to avoid to simplify it twice.  */
  tree_build_scope (&body);
  simplify_stmt (body, COMPOUND_BODY (body));
  loop = build_stmt (WHILE_STMT, 
		     /* WHILE_COND */ build (NE_EXPR, integer_type_node, 
					     integer_one_node, integer_zero_node),
		     /* WHILE_BODY */ body);
  
  /* Step 3 : Include the loop in an IF_STMT : 'if (cond) while (1){}'  */
  stmt = new_simplified_if (copy_node (cond), loop, NULL_TREE, scope);
  
  /* Step 4 : Next statement to be processed.  */
  last = chainon (prev, stmt);
  last = tree_last (last);
  chainon (stmt, next);
  return last;
}

/* }}} */

/** {{{  simplify_while_stmt()

    Simplifies the given WHILE_STMT : 
    while (cond) {X; continue; Y;} 
    into a simplified version :
    if (cond) while (1) {X; if (!cond) break; continue; Y; if (!cond) break;}  
    Return a pointer to the last statement of the simplified region.  */

static tree
simplify_while_stmt (prev, stmt, next, scope)
     tree prev, stmt, next, scope;
{
  tree cond, body, loop, last;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&WHILE_BODY (stmt));
  cond = WHILE_COND (stmt);
  body = WHILE_BODY (stmt);

  if (simplified_p (cond))
    /* The WHILE_COND is already simplified, simplify just the WHILE_BODY.  */
    {
      simplify_stmt (WHILE_BODY (stmt), COMPOUND_BODY (WHILE_BODY (stmt)));
      /* The last statement in the simplified region is the current one.  */
      last = stmt;
      return last;
    }

  /* step 1 : Insert a simplified version of 'if (!cond) break;' in the loop body.  */
  {
    tree if_stmt;

    /* Build 'if (cond) {} else break;'.  */
    if_stmt = new_simplified_if (cond, NULL_TREE, build_stmt (BREAK_STMT), 
				 COMPOUND_BODY (WHILE_BODY (stmt)));
    
    /* Insert the 'if (!cond) break;' before CONTINUE_STMT and at the end 
       of the loop block.  */
    insert_before_continue_end (if_stmt, WHILE_BODY (stmt), STMT_LINENO (stmt));
  }
  
  /* Step 2 : Construct a 'while (1!=0) {}' statement.  For the moment don't 
     initialize the loop body in order to avoid to simplify it twice.  */
  tree_build_scope (&body);
  simplify_stmt (body, COMPOUND_BODY (body));
  loop = build_stmt (WHILE_STMT, 
		     /* WHILE_COND */ build (NE_EXPR, integer_type_node, 
					     integer_one_node, integer_zero_node),
		     /* WHILE_BODY */ body);
  
  /* Step 3 : Include the loop in an IF_STMT : 'if (cond) while (1){}'  */
  stmt = new_simplified_if (copy_node (cond), loop, NULL_TREE, scope);

  /* Step 4 : Next statement to be processed.  */
  TREE_CHAIN (prev) = NULL_TREE;
  last = chainon (prev, stmt);
  last = tree_last (last);
  chainon (stmt, next);
  return last;
}

/* }}} */

/** {{{ simplify_do_stmt()

   Simplify a DO_STMT node.
   Return a pointer to the last statement of the simplified region.  */


static tree
simplify_do_stmt (prev, stmt, next, scope)
     tree prev, stmt, next, scope;
{
  tree cond, body, loop, last;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&DO_BODY (stmt));
  cond = DO_COND (stmt);
  body = DO_BODY (stmt);

  if (simplified_p (cond))
    /* The DO_COND is already simplified, simplify just the DO_BODY.  */
    {
      simplify_stmt (DO_BODY (stmt), COMPOUND_BODY (DO_BODY (stmt)));
      /* The last statement in the simplified region is the current one.  */
      last = stmt;
      return last;
    }

  /* step 1 : Insert a simplified version of 'if (!cond) break;' in the loop body.  */
  {
    tree if_stmt;
    
    /* Build 'if (cond) {} else break;'.  */
    if_stmt = new_simplified_if (cond, NULL_TREE, build_stmt (BREAK_STMT), 
				 COMPOUND_BODY (DO_BODY (stmt)));

    /* Insert the 'if (!cond) break;' before CONTINUE_STMT and at the end 
       of the loop block.  */
    insert_before_continue_end (if_stmt, DO_BODY (stmt), STMT_LINENO (stmt));
  }
  
  /* Step 2 : Construct a 'while (1!=0) {}' statement.  For the moment don't 
     initialize the loop body in order to avoid to simplify it twice.  */
  loop = build_stmt (WHILE_STMT, 
		     /* WHILE_COND */ build (NE_EXPR, integer_type_node, 
					     integer_one_node, integer_zero_node),
		     /* WHILE_BODY */ NULL_TREE);
    
  /* Step 4 : Initialize the loop body.  */
  simplify_stmt (body, scope);
  WHILE_BODY (loop) = body;

  /* Step 5 : Next statement to be processed.  */
  TREE_CHAIN (prev) = NULL_TREE;
  last = chainon (prev, loop);
  last = tree_last (last);
  chainon (loop, next);
  return last;
}


/* }}} */

/** {{{ new_simplified_if()
    
    Constructs a new IF_STMT with its IF_COND under a simplified form.  */

static tree 
new_simplified_if (cond, then_clause, else_clause, scope)
     tree cond;
     tree then_clause;
     tree else_clause;
     tree scope;
{
  if (scope == NULL_TREE)
    abort ();
    		
  switch (TREE_CODE (cond))
    /* Simplify IF_COND.  */
    {
    case TRUTH_ANDIF_EXPR:
      {
	tree op0, op1;
	op0 = TREE_OPERAND (cond, 0);
	op1 = TREE_OPERAND (cond, 1);
	return new_simplified_if 
	  (op0, new_simplified_if (op1, then_clause, else_clause, scope),
	   NULL_TREE, scope);
      }

    case TRUTH_ORIF_EXPR:
      {
	tree op0, op1;
	op0 = TREE_OPERAND (cond, 0);
	op1 = TREE_OPERAND (cond, 1);
	return new_simplified_if 
	  (op0, then_clause,
	   new_simplified_if (op1, then_clause, else_clause, scope), scope);
      }

    default:
      {
	tree res;
	tree before, new_vars;
	before = NULL;
	new_vars = NULL;

	/* Build 'if (cond) then_clause; else else_clause;'.  */
	res = build_stmt (IF_STMT, cond, then_clause, else_clause);
	
	/* Simplify if condition.  */
	{
	  tree cond_before = NULL, cond_after = NULL;
	  tree repl;

	  repl = simplify_expr (cond, &cond_before, &cond_after, &new_vars, scope);
	  
	  if (simplified_rec_p (repl, 1))
	    /* The simplification introduced a new temporary variable.  */
	    IF_COND (res) = build (NE_EXPR, integer_type_node, repl, integer_zero_node);
	  
	  /* Expand IF_COND before the beginning of the IF_STMT.  */
	  insert_stmt_chain (&before, cond_before);
	  insert_stmt_chain (&before, cond_after);
	}
	
	if (new_vars)
	  /* If we created new temporary variables, create declarations for them.  */
	  declare_tmp_vars (new_vars, scope);

	if (before)
	  /* Insert the IF_STMT 'res' after the code contained in BEFORE.  */
	  res = chainon (before, res);
	
  
	return res;
      }
    }
}

/* }}} */

/** {{{ simplify_switch_stmt()

    Simplifies a SWITCH_STMT node.  FIXME.  */

static void
simplify_switch_stmt (t, before_p, after_p, new_vars_p, scope)
     tree t;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
     tree scope;
{
  SWITCH_COND (t) = simplify_expr (SWITCH_COND (t), before_p,
				   after_p, new_vars_p, scope);

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&SWITCH_BODY (t));
  simplify_stmt (/* stmt */ SWITCH_BODY (t), 
		 /* scope */ TREE_OPERAND (SWITCH_BODY (t), 0));
}

/* }}} */

/** {{{ simplify_decl_stmt()

    Simplifies a DECL_STMT node T.  This function scans the list of all
    declarations starting with T looking for initializers.
    
    If a declaration V has an initial value I, the expression 'V = I' is
    added to the AFTER_P list.  On return, all the statements in AFTER_P
    will be inserted immediately after all the DECL_STMTs in the same order
    that they appeared originally.

    The function returns the last DECL_STMT found.  */

static tree
simplify_decl_stmt (t, after_p)
     tree t;
     tree *after_p;
{
  tree prev;

  if (TREE_CODE (t) != DECL_STMT)
    abort ();

  prev = t;
  while (t && TREE_CODE (t) == DECL_STMT)
    {
      tree decl = DECL_STMT_DECL (t);

      /* If there is an initializer for the variable,  */
      if (DECL_INITIAL (decl)
	  /* and if the declaration is not declared as a const,  */	  
	  && !TREE_READONLY (decl))
	/* queue it up to be processed after all the declarations.  */
	{
	  tree assign = build_modify_expr (decl, NOP_EXPR, DECL_INITIAL (decl));
	  add_tree (build_stmt (EXPR_STMT, assign), after_p);
	  DECL_INITIAL (decl) = NULL_TREE;
	}

      prev = t;
      t = TREE_CHAIN (t);
    }

  return prev;
}

/* }}} */

/** {{{ simplify_expr()

    Simplifies the expression tree rooted at T.

    SCOPE indicates where new temporary variables should be created.

    BEFORE_P points to the list where side effects that must happen before
	T should be stored.

    AFTER_P points to the list where side effects that must happen after T
	should be stored.

    NEW_VARS_P points to the list where any temporary VAR_DECLs needed to
	model T's side effects should be stored.  */

static tree
simplify_expr (expr, before_p, after_p, new_vars_p, scope)
     tree expr;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
     tree scope;
{
  enum tree_code code;
  tree t1, t2, lhs, rhs;

  if (expr == NULL)
    abort ();

  code = TREE_CODE (expr);

  switch (code)
    {
      /* Unary expressions that do not compute a new value.  Nothing to do.  */
    case VAR_DECL:
    case FUNCTION_DECL:
    case PARM_DECL:
    case FIELD_DECL:
    case COMPLEX_CST:
    case INTEGER_CST:
    case LABEL_DECL:
    case REAL_CST:
    case RESULT_DECL:
    case STRING_CST:
    case ADDR_EXPR:
      return expr;
      

      /* Unary expressions that compute a new value.  Re-write the operand
	 and return the modified expression.  */
    case ABS_EXPR:
    case CONJ_EXPR:
    case CONVERT_EXPR:
    case FFS_EXPR:
    case FIX_CEIL_EXPR:
    case FIX_FLOOR_EXPR:
    case FIX_ROUND_EXPR:
    case FIX_TRUNC_EXPR:
    case FLOAT_EXPR:
    case IMAGPART_EXPR:
    case INDIRECT_REF:
    case NEGATE_EXPR:
    case NON_LVALUE_EXPR:
    case NOP_EXPR:
    case REALPART_EXPR:
    case REFERENCE_EXPR:
    case BIT_NOT_EXPR:
    case TRUTH_NOT_EXPR:
    case VA_ARG_EXPR:
      TREE_OPERAND (expr, 0) = simplify_expr (TREE_OPERAND (expr, 0), 
	                                      before_p, after_p,
					      new_vars_p, scope);
      return expr;


      /* Post-decrement and increment expressions.  Queue the operation in
	 the AFTER list.  */
    case POSTINCREMENT_EXPR:
    case POSTDECREMENT_EXPR:
      lhs = simplify_expr (TREE_OPERAND (expr, 0), before_p, after_p, new_vars_p, scope);
      rhs = simplify_expr (TREE_OPERAND (expr, 1), before_p, after_p, new_vars_p, scope);
      t1 = build ((code == POSTINCREMENT_EXPR) ? PLUS_EXPR : MINUS_EXPR,
	          TREE_TYPE (expr), lhs, rhs);
      t2 = build_modify_expr (lhs, NOP_EXPR, t1);
      add_tree (build_stmt (EXPR_STMT, t2), after_p);
      return lhs;


      /* Pre-decrement and increment expressions.  Queue the operation in
	 the BEFORE list.  */
    case PREDECREMENT_EXPR:
    case PREINCREMENT_EXPR:
      lhs = simplify_expr (TREE_OPERAND (expr, 0), before_p, after_p, new_vars_p, scope);
      rhs = simplify_expr (TREE_OPERAND (expr, 1), before_p, after_p, new_vars_p, scope);
      t1 = build ((code == PREINCREMENT_EXPR) ? PLUS_EXPR : MINUS_EXPR,
	          TREE_TYPE (expr), lhs, rhs);
      t2 = build_modify_expr (lhs, NOP_EXPR, t1);
      add_tree (build_stmt (EXPR_STMT, t2), before_p);
      return lhs;


      /* Binary arithmetic expressions.  Simplify each operand, assign the
	 result of the operation to a new temporary T and return T.  */
    case BIT_AND_EXPR:
    case BIT_ANDTC_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
    case CEIL_DIV_EXPR:
    case CEIL_MOD_EXPR:
    case EXACT_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case FLOOR_MOD_EXPR:
    case LROTATE_EXPR:
    case LSHIFT_EXPR:
    case MINUS_EXPR:
    case MULT_EXPR:
    case PLUS_EXPR:
    case RDIV_EXPR:
    case ROUND_DIV_EXPR:
    case ROUND_MOD_EXPR:
    case RROTATE_EXPR:
    case RSHIFT_EXPR:
    case TRUNC_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
    case COMPLEX_EXPR:
    case COMPOUND_EXPR:
    case CONSTRUCTOR:
    case EQ_EXPR:
    case GE_EXPR:
    case GT_EXPR:
    case LE_EXPR:
    case LT_EXPR:
    case NE_EXPR:
    case MAX_EXPR:
    case MIN_EXPR:
    case ORDERED_EXPR:
    case UNEQ_EXPR:
    case UNGE_EXPR:
    case UNGT_EXPR:
    case UNLE_EXPR:
    case UNLT_EXPR:
    case UNORDERED_EXPR:
      {
	int s0, s1;
	s0 = simplified_rec_p (TREE_OPERAND (expr, 0), 1);
	s1 = simplified_rec_p (TREE_OPERAND (expr, 1), 1);
	
	if (s0 && s1)
	  /* Don't simplify an expression that is already simple enough.  */
	  return expr;
	
	if (!s0 && !s1)
	  /* Simplify both sides of the expression.  */
	  {
	    lhs = simplify_expr (TREE_OPERAND (expr, 0), before_p,
				 after_p, new_vars_p, scope);
	    rhs = simplify_expr (TREE_OPERAND (expr, 1), before_p,
				 after_p, new_vars_p, scope);
	    
	    TREE_OPERAND (expr, 0) = lhs;
	    TREE_OPERAND (expr, 1) = rhs;
	    
	    t1 = create_tmp_var (TREE_TYPE (expr), new_vars_p);
	    t2 = build_modify_expr (t1, NOP_EXPR, expr);
	    add_tree (build_stmt (EXPR_STMT, t2), before_p);
	    return t1;
	  }
	
	if (s0)
	  /* Simplify just the rhs, and don't construct any temporary variable.  */
	  {
	    rhs = simplify_expr (TREE_OPERAND (expr, 1), before_p,
				 after_p, new_vars_p, scope);
	    TREE_OPERAND (expr, 1) = rhs;
	    return expr;
	  }

	/* if (s1)  */
	/* Simplify just the lhs, and don't construct any temporary variable.  */
	lhs = simplify_expr (TREE_OPERAND (expr, 0), before_p,
			     after_p, new_vars_p, scope);
	TREE_OPERAND (expr, 0) = lhs;
	return expr;
      }

    case ARRAY_REF:
      {
	lhs = simplify_expr (TREE_OPERAND (expr, 0), before_p,
			     after_p, new_vars_p, scope);
	rhs = simplify_expr (TREE_OPERAND (expr, 1), before_p,
			     after_p, new_vars_p, scope);
	TREE_OPERAND (expr, 0) = lhs;
	TREE_OPERAND (expr, 1) = rhs;
      	
	return expr;
      }

    case COMPONENT_REF:
      /* Simplification of x = a.b.c.d[7] into 
	 int *T1;  T1 = &a.b.c.d;  x = *T1[7];  */
      {
	tree type;
	type = TREE_TYPE (expr);
	if (TREE_CODE (type) == ARRAY_TYPE)
	  {
	    tree type_p; 
	    type_p = build_pointer_type (TREE_TYPE (type));

	    t1 = create_tmp_var (type_p, new_vars_p);
	    t2 = build_modify_expr (t1, NOP_EXPR, build1 (ADDR_EXPR, type_p, expr));
	    
	    add_tree (build_stmt (EXPR_STMT, t2), before_p);
	    // FIXME : work, but incorrect ?  
	    return build1 (INDIRECT_REF, TREE_TYPE (expr), t1); 
	  }
	else
	  return expr;
      }

    case COND_EXPR:
      {
	/* The expression hides an if.
	   (cond) ? then_clause : else_clause;
	   Transform it in an IF_STMT, 
	   if (cond) tmp = then_clause; else tmp = else_clause;
	   and simplify it correctly.  
	   FIXME : case when else is a block...
	*/
	tree if_new;
	tree tmp;
	
	fprintf (stderr, "Got a cond_expr : \n");
	debug_c_node (expr);
	fprintf (stderr, "\n");
	
	/* FIXME: What happens if the type is a pointer ? function pointer ? ... */
	tmp = create_tmp_var (TREE_TYPE (expr), new_vars_p);
	
	/* Build the THEN_CLAUSE.  */
	t1 = build_modify_expr (tmp, NOP_EXPR, TREE_OPERAND (expr, 1));
	t1 = build_stmt (EXPR_STMT, t1);
	tree_build_scope (&t1);
	simplify_stmt (t1, scope); //COMPOUND_BODY (t1));
	
	/* Build the ELSE_CLAUSE.  */
	t2 = build_modify_expr (tmp, NOP_EXPR, TREE_OPERAND (expr, 2));
	t2 = build_stmt (EXPR_STMT, t2);
	tree_build_scope (&t2);
	simplify_stmt (t2, scope); //COMPOUND_BODY (t2));
	
	if_new = new_simplified_if (TREE_OPERAND (expr, 0), t1, t2, scope);
	insert_stmt_chain (before_p, if_new);
	return tmp;
      }

    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
      /* Transform this expression into T, 
	 insert in before_p its definition : the simplified version of 
	 'if (cond) T = 1; else T = 0;'  */
      {
	tree tmp, if_new;
	
	/* Create a temporary variable.  */
	tmp = create_tmp_var (TREE_TYPE (expr), new_vars_p);

	/* Build the THEN_CLAUSE.  */
	t1 = build_modify_expr (tmp, NOP_EXPR, integer_one_node);
	t1 = build_stmt (EXPR_STMT, t1);
	tree_build_scope (&t1);
	simplify_stmt (t1, COMPOUND_BODY (t1));

	/* Build the ELSE_CLAUSE.  */
	t2 = build_modify_expr (tmp, NOP_EXPR, integer_zero_node);
	t2 = build_stmt (EXPR_STMT, t2);
	tree_build_scope (&t2);
	simplify_stmt (t2, COMPOUND_BODY (t2));
	
	/* Construct the simplified IF_STMT.  */
	if_new = new_simplified_if (expr, t1, t2, scope);
	insert_stmt_chain (before_p, if_new);

	return tmp;
      }

      /* Assignment and initialization.  Simplify both sides, queue up the
	 simplified expression and return the LHS.  */
    case INIT_EXPR:
      lhs = simplify_expr (TREE_OPERAND (expr, 0), before_p,
			   after_p, new_vars_p, scope);
      rhs = simplify_expr (TREE_OPERAND (expr, 1), before_p,
			   after_p, new_vars_p, scope);
      TREE_OPERAND (expr, 0) = lhs;
      TREE_OPERAND (expr, 1) = rhs;
      add_tree (build_stmt (EXPR_STMT, expr), before_p);
      return lhs;

    case MODIFY_EXPR:
      lhs = simplify_expr (TREE_OPERAND (expr, 0), before_p,
			   after_p, new_vars_p, scope);
      rhs = simplify_expr (TREE_OPERAND (expr, 1), before_p,
			   after_p, new_vars_p, scope);
      TREE_OPERAND (expr, 0) = lhs;
      TREE_OPERAND (expr, 1) = rhs;
      add_tree (build_stmt (EXPR_STMT, expr), before_p);
      return lhs;


      /* Ternary operations.  FIXME.  */
    case BIT_FIELD_REF:
    case SAVE_EXPR:
      return expr;


      /* Function calls.  Simplify all the arguments, the function
	 reference and re-write the call.  */
    case CALL_EXPR:
      TREE_OPERAND (expr, 0) = simplify_expr (TREE_OPERAND (expr, 0), 
					      before_p, after_p,
					      new_vars_p, scope);
      if (TREE_OPERAND (expr, 1))
	TREE_OPERAND (expr, 1) = simplify_expr (TREE_OPERAND (expr, 1),
						before_p, after_p,
						new_vars_p, scope);

      return expr;

    case TREE_LIST:
      {
	tree op;

	for (op = expr; op; op = TREE_CHAIN (op))
	  TREE_VALUE (op) = simplify_expr (TREE_VALUE (op), before_p, after_p,
					   new_vars_p, scope);

	return expr;
      }


      /* C/C++ statement-expressions.  FIXME.  */
    case STMT_EXPR:
      return expr;


      /* File and line number annotations.  FIXME.  */
    case EXPR_WITH_FILE_LOCATION:
      return expr;

      /* C99 compound literals.  FIXME.  */
    case COMPOUND_LITERAL_EXPR:
      return expr;

    default:
      {
	error ("unhandled expression in simplify_expr():");
	debug_tree (expr);
	fputs ("\n", stderr);
	abort ();
      }
    }
}

/* }}} */

/** {{{ create_tmp_var()

   Create a new temporary variable declaration of type TYPE.  Returns the
   newly created decl and adds it to the list pointed by NEW_VARS_P.  */

static tree
create_tmp_var (type, new_vars_p)
     tree type;
     tree *new_vars_p;
{
  static unsigned int id_num = 1;
  char *tmp_name;
  tree tmp_var;

  ASM_FORMAT_PRIVATE_NAME (tmp_name, "T", id_num++);
  tmp_var = build_decl (VAR_DECL, get_identifier (tmp_name), type);

  /* The variable was declared by the compiler.  */
  DECL_ARTIFICIAL (tmp_var) = 1;

  /* Make the variable writable.  */
  TREE_READONLY (tmp_var) = 0;

  /* Make the type of the variable writable.  */
  make_type_writable (tmp_var);

  /* Set the declaration context.  */
  DECL_CONTEXT (tmp_var) = current_function_decl;
  DECL_EXTERNAL (tmp_var) = 0;
  TREE_STATIC (tmp_var) = 0;
  TREE_USED (tmp_var) = 1;

  /* Insert the declaration in the list of new variables.  */
  add_tree (tmp_var, new_vars_p);

  return tmp_var;
}

/* }}} */

/** {{{ make_type_writable()

    Change the flags for the type of the node T to make it writtable.  */

static void 
make_type_writable (t)
     tree t;
{
  if (t == NULL_TREE)
    abort();

  if (TYPE_READONLY (TREE_TYPE (t))
      || ((TREE_CODE (TREE_TYPE (t)) == RECORD_TYPE
	   || TREE_CODE (TREE_TYPE (t)) == UNION_TYPE)
	  && C_TYPE_FIELDS_READONLY (TREE_TYPE (t))))
    {
      /* Make a copy of the type declaration.  */
      TREE_TYPE (t) = build_type_copy (TREE_TYPE (t));
      TYPE_READONLY (TREE_TYPE (t)) = 0;
      
      /* If the type is a structure that contains a field readonly.  */
      if ((TREE_CODE (TREE_TYPE (t)) == RECORD_TYPE
	   || TREE_CODE (TREE_TYPE (t)) == UNION_TYPE)
	  && C_TYPE_FIELDS_READONLY (TREE_TYPE (t)))
	{
	  C_TYPE_FIELDS_READONLY (TREE_TYPE (t)) = 0;

	  /* Make the fields of the structure writable.  */
	  {
	    tree it;
	    it = TYPE_FIELDS (TREE_TYPE (t));
	    while (it)
	      {
		/* Make the field writable.  */
		TREE_READONLY (it) = 0;
		
		/* Make the type of the field writable.  */
		make_type_writable (it);
		it = TREE_CHAIN (it);
	      }
	  }
	}
    }
}

/* }}} */

/** {{{ declare_tmp_vars()

    Declares all the variables in VARS in SCOPE.  Returns the last DECL_STMT
    emitted.  */

static tree
declare_tmp_vars (vars, scope)
     tree vars;
     tree scope;
{
  tree t, last;

  /* Add all the symbols to the BLOCK_VARS for the scope.  */
  if (SCOPE_STMT_BLOCK (scope) == NULL)
    SCOPE_STMT_BLOCK (scope) = build_block (vars, NULL, NULL, NULL, NULL);
  else
    chainon (BLOCK_VARS (SCOPE_STMT_BLOCK (scope)), vars);

  /* Find the last declaration statement in the scope.  Add all the new
     declarations after it.  */
  last = scope;
  while (TREE_CHAIN (last) && TREE_CODE (TREE_CHAIN (last)) == DECL_STMT)
    last = TREE_CHAIN (last);

  for (t = vars; t; t = TREE_CHAIN (t))
    {
      tree decl, tmp;

      decl = build_stmt (DECL_STMT, t);
      STMT_LINENO (decl) = STMT_LINENO (scope);

      tmp = TREE_CHAIN (last);
      TREE_CHAIN (last) = decl;
      TREE_CHAIN (decl) = tmp;

      last = decl;
    }

  return last;
}

/* }}} */

/** {{{ tree_build_scope()
   
   Replaces T; by a COMPOUND_STMT containing {T;}.  */

static void
tree_build_scope (t)
     tree *t;
{
  if (*t == NULL_TREE)
    {
      /* Construct a compound statement and a scope.  */
      tree body, start_scope, end_scope;
      body = make_node (COMPOUND_STMT);
      start_scope = make_node (SCOPE_STMT);
      end_scope = make_node (SCOPE_STMT);
      TREE_LANG_FLAG_0 (start_scope) = 1;
      TREE_LANG_FLAG_0 (end_scope) = 0;
      TREE_OPERAND (body, 0) = start_scope;
      /* Construct an empty body.  */
      TREE_CHAIN (start_scope) = end_scope;
      /* Replace the node by the constructed body.  */
      *t = body;
      return;
    }

  if (TREE_CODE (*t) == COMPOUND_STMT)
    {
      if (TREE_OPERAND (*t, 0) == NULL_TREE)
	{
	  /* There's a compound statement, but no scope.  */
	  tree start_scope, end_scope;
	  start_scope = make_node (SCOPE_STMT);
	  end_scope = make_node (SCOPE_STMT);
	  TREE_LANG_FLAG_0 (start_scope) = 1;
	  TREE_LANG_FLAG_0 (end_scope) = 0;
	  TREE_OPERAND (*t, 0) = start_scope;
	  TREE_CHAIN (start_scope) = end_scope;
	}
      else 
	/* The given NODE is actually a scope.  */
	return;
    }
  else
    {
      /* Construct a compound statement and a scope.  */
      tree body, start_scope, end_scope;
      body = make_node (COMPOUND_STMT);
      start_scope = make_node (SCOPE_STMT);
      end_scope = make_node (SCOPE_STMT);
      TREE_LANG_FLAG_0 (start_scope) = 1;
      TREE_LANG_FLAG_0 (end_scope) = 0;
      TREE_OPERAND (body, 0) = start_scope;
      TREE_CHAIN (start_scope) = *t;
      TREE_CHAIN (*t) = end_scope;
      *t = body;
    }
}

/* }}} */

/** {{{ add_tree()

   Add T to the list pointed by LIST_P.  */

static void
add_tree (t, list_p)
     tree t;
     tree *list_p;
{
  if (t == NULL || list_p == NULL)
    abort ();

  if (*list_p)
    chainon (*list_p, t);
  else
    *list_p = t;
}

/* }}} */

/** {{{ keep_stmt_p()

    Return zero if T is a statement that has been nullified by the
    simplification process.  */

static int
keep_stmt_p (t)
     tree t;
{
  if (t == NULL)
    return 0;

  /* Expression statements that only contain a non-volatile variable
     declaration (e.g., 'a;') should be discarded.  */
  if (TREE_CODE (t) == EXPR_STMT
      && TREE_OPERAND (t, 0)
      && (TREE_CODE (TREE_OPERAND (t, 0)) == VAR_DECL ||
	  TREE_CODE (TREE_OPERAND (t, 0)) == PARM_DECL ||
	  TREE_CODE (TREE_OPERAND (t, 0)) == INDIRECT_REF ||
	  TREE_CODE (TREE_OPERAND (t, 0)) == ARRAY_REF ||
	  TREE_CODE (TREE_OPERAND (t, 0)) == COMPONENT_REF)
      && !TYPE_VOLATILE (TREE_TYPE (TREE_OPERAND (t, 0))))
    return 0;

  return 1;
}

/* }}} */

/** {{{ insert_before_continue_end()
    
    Insert the REEVAL list before CONTINUE_STMTs and at the end of the loop body BODY.  
    Set the line number of the REEVAL list to LINE.  */

static void
insert_before_continue_end (reeval, body, line)
     tree reeval;
     tree body;
     int line;
{
  tree last, forelast;

  /* Update the line number information.  */
  {
    tree it;
    for (it = reeval; TREE_CHAIN (it); it = TREE_CHAIN (it))
      update_line_number (it, line);
  }

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&body);

  /* Insert the reevaluation list before every CONTINUE_STMT.  */
  /*  TREE_CHAIN (reeval) = NULL_TREE; */
  forelast = insert_before_continue (body, reeval);
  last = TREE_CHAIN (forelast);
  
  /* If the last statement of the WHILE_BODY is not a CONTINUE_STMT, 
     then insert reeval at the end of the loop block.  */
  if (TREE_CODE (forelast) != CONTINUE_STMT)
    {
      TREE_CHAIN (forelast) = copy_stmt_chain (reeval);
      forelast = tree_last (forelast);
      TREE_CHAIN (forelast) = last;
    }
}

/* }}} */

/** {{{ insert_before_continue()
    
    Insert the list REEVAL of statements before each CONTINUE_STMT in the block 
    pointed to by NODE.  At the end returns a pointer to the forelast node in 
    the block NODE.  The caller can insert then the last loop reevaluation at 
    the end of the loop block.  */

static tree
insert_before_continue (node, reeval)
     tree node, reeval;
{
  tree next;

  if (reeval == NULL_TREE || node == NULL_TREE)
    return NULL_TREE;
  
  if (TREE_CODE (node) == COMPOUND_STMT)
    node = COMPOUND_BODY (node);
  
  next = TREE_CHAIN (node);
  if (next == NULL_TREE)
    return NULL_TREE;

  /* Walk through each statement in the given block up to the last one, 
     searching for CONTINUE_STMTs.  */
  for ( ;TREE_CHAIN (next) != NULL_TREE; 
	node = TREE_CHAIN (node), next = TREE_CHAIN (next))
    {
      switch (TREE_CODE (next))
	{

	case CONTINUE_STMT:
	  /* Insert the reeval of statements before continue.  */
	  TREE_CHAIN (node) = copy_stmt_chain (reeval);
	  node = tree_last (node);
	  TREE_CHAIN (node) = next;
	  break;
	  
	case IF_STMT:
	  /* Be sure that the THEN_CLAUSE has a scope.  */
	  tree_build_scope (&THEN_CLAUSE (next));

	  /* Insert the code REEVAL in the block of the THEN_CLAUSE.  */
	  insert_before_continue (COMPOUND_BODY (THEN_CLAUSE (next)), reeval);
	  
	  /* Same thing for the ELSE_CLAUSE.  */
	  if (ELSE_CLAUSE (TREE_CHAIN (node)))
	    {
	      tree_build_scope (&ELSE_CLAUSE (next));
	      insert_before_continue (COMPOUND_BODY (ELSE_CLAUSE (next)), reeval);
	    }
	  break;

	  //§ FIXME ... to be tested ...
	case SWITCH_STMT:
	  /* Be sure that the SWITCH_BODY has a scope.  */
	  tree_build_scope (&SWITCH_BODY (next));

	  /* Insert the code REEVAL in the SWITCH_BODY.  */
	  insert_before_continue (COMPOUND_BODY (SWITCH_BODY (next)), reeval);
	  break;

	case COMPOUND_STMT:
	  /* Insert in the inner block.  */
	  insert_before_continue (COMPOUND_BODY (next), reeval);
	  break;

	default:
	  /* Don't enter in sub loops...  The continue statement has an effect 
	     only at a depth 1.  */
	  break;
	}
    }
  return node;
}

/* }}} */


/** {{{ copy_stmt_chain()
    
    Copy every statement from the chain CHAIN and return the new chain.  */
    
static tree
copy_stmt_chain (chain)
     tree chain;
{
  tree new_chain, res;

  if (chain == NULL_TREE)
    /* Nothing to copy.  */
    return NULL_TREE;
  
  new_chain = copy_stmt (chain);
  res = new_chain;

  while (TREE_CHAIN (chain))
    {
      chain = TREE_CHAIN (chain);
      TREE_CHAIN (new_chain) = copy_stmt (chain);
      new_chain = TREE_CHAIN (new_chain);
    }

  return res;
}

/* }}} */

/** {{{ copy_stmt()

    Copy a statement by using copy_node when needed, but ensuring that we copy 
    enough information in order to have distinct statements.  */

static tree 
copy_stmt (node)
     tree node;
{
  tree res;

  if (node == NULL_TREE)
    return NULL_TREE;

  switch (TREE_CODE (node))
    {
    case COMPOUND_STMT:
      res = build_stmt (COMPOUND_STMT, 
			copy_stmt_chain (COMPOUND_BODY (node)));
      break;

    case FOR_STMT:
      res = build_stmt (FOR_STMT, 
			copy_stmt (FOR_INIT_STMT (node)),
			copy_node (FOR_COND (node)),
			copy_node (FOR_EXPR (node)),
			copy_stmt (FOR_BODY (node)));
      break;

    case WHILE_STMT:
      res = build_stmt (WHILE_STMT, 
			copy_node (WHILE_COND (node)),
			copy_stmt (WHILE_BODY (node)));
      break;

    case DO_STMT:
      res = build_stmt (DO_STMT, 
			copy_node (DO_COND (node)),
			copy_stmt (DO_BODY (node)));
      break;

    case IF_STMT:
      res = build_stmt (IF_STMT, 
			copy_node (IF_COND (node)),
			copy_stmt (THEN_CLAUSE (node)),
			copy_stmt (ELSE_CLAUSE (node)));
      break;

    case SWITCH_STMT:
      res = build_stmt (SWITCH_STMT,
			copy_node (SWITCH_COND (node)),
			copy_stmt (SWITCH_BODY (node)));
      break;

    case EXPR_STMT:
      res = build_stmt (EXPR_STMT,
			copy_node (EXPR_STMT_EXPR (node)));
      break;

    case DECL_STMT:
      res = build_stmt (DECL_STMT,
			copy_node (DECL_STMT_DECL (node)));
      break;

    case RETURN_STMT:
      res = build_stmt (RETURN_STMT,
			copy_node (RETURN_EXPR (node)));
      break;

    case SCOPE_STMT:
    case FILE_STMT:
    case LABEL_STMT:
    case GOTO_STMT:
    case ASM_STMT:
    case CASE_LABEL:
    case CONTINUE_STMT:
    case BREAK_STMT:
    default:
      res = copy_node (node);
      break;
    }
  
  /* Set the line number.  */
  STMT_LINENO (res) = STMT_LINENO (node);
  
  return res;
}

/** {{{ insert_stmt_chain()

    Insert the chain op2 at the end of the chain *op1.  */

static void 
insert_stmt_chain (op1, op2)
     tree *op1, op2;
{
  tree last;
  if (op1 == NULL)
    abort ();

  if (*op1 == NULL_TREE)
    *op1 = op2;
  else
    {
      last = tree_last (*op1);
      TREE_CHAIN (last) = op2;
    }
}

/* }}} */

/** {{{ update_line_number()

    Updates the STMT_LINENO of each stmt in the tree t to the line number LINE. 
    Returns the last stmt in the tree chain.  */

static tree
update_line_number (t, line)
     tree t;
     int line;
{
  if (t == NULL_TREE)
    return NULL_TREE;

  for (; TREE_CHAIN (t); t = TREE_CHAIN (t))
    STMT_LINENO (t) = line;
  STMT_LINENO (t) = line;
  return t;
}

/* }}} */

/** {{{ simplified_p()

    Return one if T is an expression that is under its simplified form.  */

static int
simplified_p (t)
     tree t;
{
  return simplified_rec_p (t, 0);
}

/* }}} */

/** {{{ simplified_p()
    
    Return one if T is an expression that is under its simplified form.
    DEPTH is the nest level of recursion, call this function with DEPTH=0.  */

static int
simplified_rec_p (t, depth)
     tree t;
     int depth;
{
  switch (TREE_CODE (t))
    {
    case INTEGER_CST:
    case REAL_CST:
    case COMPLEX_CST:
    case STRING_CST:
    case CONST_DECL:
    case VAR_DECL:
    case PARM_DECL:
    case FIELD_DECL:
      return 1;

    case MODIFY_EXPR:
    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case UNLT_EXPR:
    case UNLE_EXPR:
    case UNGT_EXPR:
    case UNGE_EXPR:
    case UNEQ_EXPR:
      /* Binary expressions : both sides must be simplified.  */
      if (depth == 0 &&
	  simplified_rec_p (TREE_OPERAND (t, 0), 1) &&
	  simplified_rec_p (TREE_OPERAND (t, 1), 1))
	return 1;
      else 
	return 0;

    default:
      return 0;
    }
}
