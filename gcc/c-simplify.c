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

static void simplify_for_stmt PARAMS ((tree, tree, tree *, tree *, tree *));
static void simplify_while_stmt PARAMS ((tree, tree, tree *, tree *, tree *));
static void simplify_do_stmt PARAMS ((tree, tree, tree *, tree *, tree *));
static void simplify_if_stmt PARAMS ((tree, tree, tree *, tree *, tree *));
static void simplify_switch_stmt PARAMS ((tree, tree, tree *, tree *, tree *));
static tree simplify_decl_stmt PARAMS ((tree, tree *));
static tree simplify_expr PARAMS ((tree, tree, tree *, tree *, tree *));
static tree create_tmp_var PARAMS ((tree, tree *));
static void tree_build_scope PARAMS ((tree *));
static tree declare_tmp_vars PARAMS ((tree, tree));
static void add_tree PARAMS ((tree, tree *));
static int keep_stmt_p PARAMS ((tree));

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
	  break;

	case FOR_STMT:
	  simplify_for_stmt (stmt, scope, &before, &after, &new_vars);
	  break;

	case WHILE_STMT:
	  simplify_while_stmt (stmt, scope, &before, &after, &new_vars);
	  break;

	case DO_STMT:
	  simplify_do_stmt (stmt, scope, &before, &after, &new_vars);
	  break;

	case IF_STMT:
	  simplify_if_stmt (stmt, scope, &before, &after, &new_vars);
	  break;

	case SWITCH_STMT:
	  simplify_switch_stmt (stmt, scope, &before, &after, &new_vars);
	  break;

	case EXPR_STMT:
	  TREE_OPERAND (stmt, 0) = simplify_expr (TREE_OPERAND (stmt, 0), 
	                                          scope, &before, &after,
						  &new_vars);
	  break;

	case DECL_STMT:
	  /* Simplify all the declarations at once.  */
	  prev = simplify_decl_stmt (stmt, &after);
	  stmt = prev;
	  break;

	case RETURN_STMT:
	  if (RETURN_EXPR (stmt))
	    {
	      tree e = RETURN_EXPR (stmt);
	      
	      TREE_OPERAND (e, 1) = simplify_expr (TREE_OPERAND (e, 1),
						   scope, &before, &after,
						   &new_vars);
	    }
	  break;

	case SCOPE_STMT:
	  if (SCOPE_BEGIN_P (stmt))
	    scope = stmt;
	  break;

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

      /* Include the nodes in the BEFORE list.  Update line number
	 information on each node and re-compute PREV.  */
      if (before)
	{
	  TREE_CHAIN (prev) = before;

	  for (; TREE_CHAIN (before); before = TREE_CHAIN (before))
	    STMT_LINENO (before) = STMT_LINENO (stmt);
	  STMT_LINENO (before) = STMT_LINENO (stmt);

	  prev = before;

	  /* Include STMT, if it's worth keeping (i.e., it has not been
	     nullified by the simplification process) and re-compute PREV.  */
	  if (keep_stmt_p (stmt))
	    {
	      TREE_CHAIN (prev) = stmt;
	      prev = stmt;
	    }
	}
      else
	prev = stmt;

      /* Include the nodes in the AFTER list.  Update line number
	 information on each node and re-compute PREV.  */
      if (after)
	{
	  TREE_CHAIN (prev) = after;

	  for (; TREE_CHAIN (after); after = TREE_CHAIN (after))
	    STMT_LINENO (after) = STMT_LINENO (stmt);
	  STMT_LINENO (after) = STMT_LINENO (stmt);

	  prev = after;
	}

      /* Make sure that PREV is chained to NEXT (just in case it was
	 re-computed above)  */
      TREE_CHAIN (prev) = next;

      stmt = next;
    }
}

/* }}} */

/** {{{ simplify_for_stmt()

   Simplify a FOR_STMT node.  FIXME.  */

static void
simplify_for_stmt (t, scope, before_p, after_p, new_vars_p)
     tree t;
     tree scope;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  /* It's unsafe to decompose the FOR_COND, and the FOR_EXPR 
     (reevaluation of the condition).  Thus for simplifying these nodes 
     apply before this pass the loop decomposition.  */

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&FOR_BODY (t));

  /* Decompose the loop body.  */
  simplify_stmt (FOR_BODY (t), scope);
}

/* }}} */

/** {{{ simplify_while_stmt()

   Simplify a WHILE_STMT node.  FIXME.  */

static void
simplify_while_stmt (t, scope, before_p, after_p, new_vars_p)
     tree t;
     tree scope;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&WHILE_BODY (t));

  /* Decompose the loop body.  */
  simplify_stmt (WHILE_BODY (t), scope);
}

/* }}} */

/** {{{ simplify_do_stmt()

   Simplify a DO_STMT node.  FIXME.  */

static void
simplify_do_stmt (t, scope, before_p, after_p, new_vars_p)
     tree t;
     tree scope;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&DO_BODY (t));

  /* Decompose the loop body.  */
  simplify_stmt (DO_BODY (t), scope);
}

/* }}} */

/** {{{ simplify_if_stmt()

    Simplifies an IF_STMT node.  FIXME.  */

static void
simplify_if_stmt (t, scope, before_p, after_p, new_vars_p)
     tree t;
     tree scope;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  /* FIXME.  BEFORE_P and AFTER_P should be expanded here.  */
  IF_COND (t) = simplify_expr (IF_COND (t), scope, before_p, after_p,
      new_vars_p);

  if (THEN_CLAUSE (t))
    {
      /* Make sure that the branch has a scope.  */
      tree_build_scope (&THEN_CLAUSE (t));

      /* Decompose the THEN branch.  */
      simplify_stmt (THEN_CLAUSE (t), scope);
    }

  if (ELSE_CLAUSE (t))
    {
      /* Make sure that the branch has a scope.  */
      tree_build_scope (&ELSE_CLAUSE (t));

      /* Decompose the ELSE branch.  */
      simplify_stmt (ELSE_CLAUSE (t), scope);
    }
}

/* }}} */

/** {{{ simplify_switch_stmt()

    Simplifies a SWITCH_STMT node.  FIXME.  */

static void
simplify_switch_stmt (t, scope, before_p, after_p, new_vars_p)
     tree t;
     tree scope;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  SWITCH_COND (t) = simplify_expr (SWITCH_COND (t), scope, before_p,
      after_p, new_vars_p);
  simplify_stmt (SWITCH_BODY (t), scope);
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

      /* If there is an initializer for the variable, queue it up to be
	 processed after all the declarations.  */
      if (DECL_INITIAL (decl))
	{
	  tree assign = build_modify_expr (decl, NOP_EXPR, DECL_INITIAL (decl));
	  add_tree (build_stmt (EXPR_STMT, assign), after_p);
	  DECL_INITIAL (decl) = NULL;
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
simplify_expr (expr, scope, before_p, after_p, new_vars_p)
     tree expr;
     tree scope;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  enum tree_code code;
  tree t1, t2, lhs, rhs;

  if (expr == NULL)
    abort ();

  code = TREE_CODE (expr);

  switch (TREE_CODE (expr))
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
	                                      scope, before_p, after_p,
					      new_vars_p);
      return expr;


      /* Post-decrement and increment expressions.  Queue the operation in
	 the AFTER list.  */
    case POSTINCREMENT_EXPR:
    case POSTDECREMENT_EXPR:
      lhs = simplify_expr (TREE_OPERAND (expr, 0), scope, before_p,
	  after_p, new_vars_p);
      rhs = simplify_expr (TREE_OPERAND (expr, 1), scope, before_p,
	  after_p, new_vars_p);
      t1 = build ((code == POSTINCREMENT_EXPR) ? PLUS_EXPR : MINUS_EXPR,
	          TREE_TYPE (expr), lhs, rhs);
      t2 = build_modify_expr (lhs, NOP_EXPR, t1);
      add_tree (build_stmt (EXPR_STMT, t2), after_p);
      return lhs;


      /* Pre-decrement and increment expressions.  Queue the operation in
	 the BEFORE list.  */
    case PREDECREMENT_EXPR:
    case PREINCREMENT_EXPR:
      lhs = simplify_expr (TREE_OPERAND (expr, 0), scope, before_p,
	  after_p, new_vars_p);
      rhs = simplify_expr (TREE_OPERAND (expr, 1), scope, before_p,
	  after_p, new_vars_p);
      t1 = build ((code == PREINCREMENT_EXPR) ? PLUS_EXPR : MINUS_EXPR,
	          TREE_TYPE (expr), lhs, rhs);
      t2 = build_modify_expr (lhs, NOP_EXPR, t1);
      add_tree (build_stmt (EXPR_STMT, t2), before_p);
      return lhs;


      /* Binary arithmetic expressions.  Simplify each operand, assign the
	 result of the operation to a new temporary T and return T.  */
    case COMPONENT_REF:
    case ARRAY_REF:
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
    case TRUTH_ANDIF_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_ORIF_EXPR:
    case TRUTH_XOR_EXPR:
    case COMPLEX_EXPR:
    case COMPOUND_EXPR:
    case CONSTRUCTOR:
    case COND_EXPR:
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
      lhs = simplify_expr (TREE_OPERAND (expr, 0), scope, before_p,
	  after_p, new_vars_p);
      rhs = simplify_expr (TREE_OPERAND (expr, 1), scope, before_p,
	  after_p, new_vars_p);
      TREE_OPERAND (expr, 0) = lhs;
      TREE_OPERAND (expr, 1) = rhs;
      t1 = create_tmp_var (TREE_TYPE (expr), new_vars_p);
      t2 = build_modify_expr (t1, NOP_EXPR, expr);
      add_tree (build_stmt (EXPR_STMT, t2), before_p);
      return t1;


      /* Assignment and initialization.  Simplify both sides, queue up the
	 simplified expression and return the LHS.  */
    case INIT_EXPR:
    case MODIFY_EXPR:
      lhs = simplify_expr (TREE_OPERAND (expr, 0), scope, before_p,
	  after_p, new_vars_p);
      rhs = simplify_expr (TREE_OPERAND (expr, 1), scope, before_p,
	  after_p, new_vars_p);
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
					      scope, before_p, after_p,
					      new_vars_p);
      if (TREE_OPERAND (expr, 1))
	TREE_OPERAND (expr, 1) = simplify_expr (TREE_OPERAND (expr, 1),
						scope, before_p, after_p,
						new_vars_p);

      return expr;

    case TREE_LIST:
      {
	tree op;

	for (op = expr; op; op = TREE_CHAIN (op))
	  TREE_VALUE (op) = simplify_expr (TREE_VALUE (op), scope,
	                                   before_p, after_p, new_vars_p);

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
  if (TYPE_READONLY (TREE_TYPE (tmp_var)))
    {
      TREE_TYPE (tmp_var) = copy_node (TREE_TYPE (tmp_var));
      TYPE_READONLY (TREE_TYPE (tmp_var)) = 0;
    }

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
   
   Replaces T by a scope containing T.  */

static void
tree_build_scope (t)
     tree *t;
{
  if (*t == NULL_TREE)
    return;

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
      && TREE_CODE (TREE_OPERAND (t, 0)) == VAR_DECL
      && !TYPE_VOLATILE (TREE_TYPE (TREE_OPERAND (t, 0))))
    return 0;

  return 1;
}

/* }}} */
