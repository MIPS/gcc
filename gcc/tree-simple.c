/* Functions to analyze and validate SIMPLE trees.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "tree.h"
/* ??? SIMPLE trees use many tree nodes defined in the C front end.  These
       should be moved to tree.def.  */ 
#include "c-tree.h"
#include "tree-simple.h"

/** SIMPLE C Grammar
  
    Original grammar available at:

	      http://www-acaps.cs.mcgill.ca/info/McCAT/McCAT.html


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

      All the possible C declarations. The only difference is that in
      SIMPLE the declarations are not allowed to have initializations in
      them.

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
	      | condexpr		=> Original grammar does not allow
					   conditional expressions.
					   Allowing them here, leads to
					   less code being generated when
					   simplifying IF/WHILE/DO
					   conditionals.

      unary_expr
	      : simp_expr
	      | '*' ID
	      | '&' varname
	      | '&' CONST		=> Needed for constant strings.
					   Not present in the original
					   grammar.
	      | call_expr
	      | unop val
	      | '(' cast ')' varname
	      | '({' all_stmts '})'	=> Original grammar does not
	                                   allow expression-statements.
					   We shouldn't, either.  But
					   for now it is easier if we
					   do.

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
	      : arraybase reflist	=> 'arraybase' is any valid C array
					   name.  Original grammar only
					   allowed 'ID reflist'.  This
					   causes splits like k.d[2][3]
					   into
					   
					   	t1 = &(k.d[2])
  						t2 = *(&t1[3])

					   which is invalid.

      reflist
	      : '[' val ']'
	      | reflist '[' val ']'

      idlist
	      : idlist '.' ID
	      | ID

      compref
	      : '(' '*' ID ')' '.' idlist
	      : '&' ID '.' idlist	=> Original grammar does not allow
					   this.
	      | idlist

     ----------------------------------------------------------------------  */

/* Validation of SIMPLE statements.  */

/** {{{ is_simple_stmt ()

    Return nonzero if T is a statement that complies with the SIMPLE
    grammar.

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

      stop_stmt
	      : BREAK ';'
	      | CONTINUE ';'
	      | RETURN ';'
	      | RETURN val ';'
	      | RETURN '(' val ')' ';'  */

int
is_simple_stmt (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  switch (TREE_CODE (t))
    {
    case COMPOUND_STMT:
      return is_simple_compstmt (COMPOUND_BODY (t));

    case SCOPE_STMT:
      return is_simple_compstmt (t);

    case EXPR_STMT:
      return is_simple_expr (EXPR_STMT_EXPR (t));

    case IF_STMT:
      return (is_simple_condexpr (IF_COND (t))
	      && is_simple_stmt (THEN_CLAUSE (t))
	      && is_simple_stmt (ELSE_CLAUSE (t)));

    case WHILE_STMT:
      return (is_simple_condexpr (WHILE_COND (t))
	      && is_simple_stmt (WHILE_BODY (t)));

    case DO_STMT:
      return (is_simple_condexpr (DO_COND (t))
	      && is_simple_stmt (DO_BODY (t)));

    case FOR_STMT:
      return (is_simple_exprseq (EXPR_STMT_EXPR (FOR_INIT_STMT (t)))
	      && is_simple_condexpr (FOR_COND (t))
	      && is_simple_exprseq (FOR_EXPR (t))
	      && is_simple_stmt (FOR_BODY (t)));

    /* Note that we can assume that we don't need to special case the body
       of the switch() statement.  If we got to this stage, we can assume
       that the switch() is properly formed (i.e., it will be a compound
       statement containing all the case labels).  */
    case SWITCH_STMT:
      return (is_simple_val (SWITCH_COND (t))
	      && is_simple_stmt (SWITCH_BODY (t)));

    case FILE_STMT:
    case LABEL_STMT:
    case GOTO_STMT:
    case ASM_STMT:
    case CASE_LABEL:
    case CONTINUE_STMT:
    case BREAK_STMT:
      return 1;

    case RETURN_STMT:
      {
	tree type = TREE_TYPE (TREE_TYPE (current_function_decl));
	if (TREE_CODE (type) != VOID_TYPE
	    && RETURN_EXPR (t))
	  return is_simple_expr (TREE_OPERAND (RETURN_EXPR (t), 1));
	else
	  return 1;
      }

    default:
      return 0;
    }
}

/* }}} */

/** {{{ is_simple_compstmt ()

    Return nonzero if T is a SIMPLE compound statement.

      compsmt
	      : '{' all_stmts '}'
	      | '{' '}'
	      | '{' decls all_stmts '}'
	      | '{' decls '}'  */

int
is_simple_compstmt (t)
     tree t;
{
  /* Look for '{'.  */
  if (TREE_CODE (t) != SCOPE_STMT
      || !SCOPE_BEGIN_P (t))
    return 0;

  /* Test all the declarations.

     ??? This allows initializers for read-only variables.  This is not
         allowed in the original grammar, but I really see no other way
	 around this.  If we simplify these declarations, we get warnings
	 and if we change the read-only bit, we'll change the semantics of
	 the variable.  */
  for (t = TREE_CHAIN (t); t && TREE_CODE (t) == DECL_STMT; t = TREE_CHAIN (t))
    if (DECL_INITIAL (DECL_STMT_DECL (t))
	&& !TREE_READONLY (DECL_STMT_DECL (t)))
      return 0;

  /* Test all the statements in the body.  */
  for (t = TREE_CHAIN (t);
       t && !(TREE_CODE (t) == SCOPE_STMT && SCOPE_END_P (t));
       t = TREE_CHAIN (t))
    if (!is_simple_stmt (t))
      return 0;

  /* Look for '}'.  */
  if (TREE_CODE (t) != SCOPE_STMT
      || !SCOPE_END_P (t))
    return 0;

  return 1;
}

/* }}} */


/* Validation of SIMPLE expressions.  */

/** {{{ is_simple_expr ()

    Return nonzero if T is an expression that complies with the SIMPLE
    grammar.

      expr
	      : rhs
	      | modify_expr  */

int
is_simple_expr (t)
     tree t;
{
  return (is_simple_rhs (t) || is_simple_modify_expr (t));
}

/* }}} */

/** {{{ is_simple_rhs ()

    Return nonzero if T is a SIMPLE RHS:

      rhs
	      : binary_expr
	      | unary_expr
	      | condexpr	=> Original grammar does not allow
				   conditional expressions on RHS.
				   However, allowing them lets us generate
				   fewer code when simplifying IF/WHILE/DO
				   structures.  */

int
is_simple_rhs (t)
     tree t;
{
  return (is_simple_binary_expr (t)
          || is_simple_unary_expr (t)
	  || is_simple_condexpr (t));
}

/* }}} */

/** {{{ is_simple_modify_expr ()

    Return nonzero if T is a SIMPLE assignment expression:

      modify_expr
	      : varname '=' rhs
	      | '*' ID '=' rhs  */

int
is_simple_modify_expr (t)
     tree t;
{
  /* Additions to the original grammar.  Allow SAVE_EXPR, NON_LVALUE_EXPR
     and EXPR_WITH_FILE_LOCATION wrappers.  */
  if (TREE_CODE (t) == SAVE_EXPR
      || TREE_CODE (t) == EXPR_WITH_FILE_LOCATION
      || TREE_CODE (t) == NON_LVALUE_EXPR)
    return is_simple_modify_expr (TREE_OPERAND (t, 0));

  return (TREE_CODE (t) == MODIFY_EXPR
	  && is_simple_modify_expr_lhs (TREE_OPERAND (t, 0))
	  && is_simple_rhs (TREE_OPERAND (t, 1)));
}

/* }}} */

/** {{{ is_simple_modify_expr_lhs ()

    Return nonzero if T is a valid LHS for a SIMPLE assignment expression.  */

int
is_simple_modify_expr_lhs (t)
     tree t;
{
  return (is_simple_varname (t)
	  || (TREE_CODE (t) == INDIRECT_REF
	      && is_simple_id (TREE_OPERAND (t, 0))));
}

/* }}} */

/** {{{ is_simple_binary_expr ()

    Return nonzero if T is a SIMPLE binary expression:

      binary_expr
	      : val binop val  */

int
is_simple_binary_expr (t)
     tree t;
{
  /* Additions to the original grammar.  Allow SAVE_EXPR, NON_LVALUE_EXPR
     and EXPR_WITH_FILE_LOCATION wrappers.  */
  if (TREE_CODE (t) == SAVE_EXPR
      || TREE_CODE (t) == EXPR_WITH_FILE_LOCATION
      || TREE_CODE (t) == NON_LVALUE_EXPR)
    return is_simple_binary_expr (TREE_OPERAND (t, 0));

  return (TREE_CODE_CLASS (TREE_CODE (t)) == '2'
	  && is_simple_val (TREE_OPERAND (t, 0))
	  && is_simple_val (TREE_OPERAND (t, 1)));
}

/* }}} */

/** {{{ is_simple_condexpr ()

    Return nonzero if T is a SIMPLE conditional expression:

      condexpr
	      : val
	      | val relop val  */

int
is_simple_condexpr (t)
     tree t;
{
  /* Additions to the original grammar.  Allow SAVE_EXPR, NON_LVALUE_EXPR
     and EXPR_WITH_FILE_LOCATION wrappers.  */
  if (TREE_CODE (t) == SAVE_EXPR
      || TREE_CODE (t) == EXPR_WITH_FILE_LOCATION
      || TREE_CODE (t) == NON_LVALUE_EXPR)
    return is_simple_condexpr (TREE_OPERAND (t, 0));

  return (is_simple_val (t)
	  || ((TREE_CODE_CLASS (TREE_CODE (t)) == '<'
	       || TREE_CODE (t) == TRUTH_AND_EXPR
	       || TREE_CODE (t) == TRUTH_OR_EXPR
	       || TREE_CODE (t) == TRUTH_XOR_EXPR)
	      && is_simple_val (TREE_OPERAND (t, 0))
	      && is_simple_val (TREE_OPERAND (t, 1))));
}

/* }}} */

/** {{{ is_simple_unary_expr ()

    Return nonzero if T is a unary expression as defined by the SIMPLE
    grammar:

      unary_expr
	      : simp_expr
	      | '*' ID
	      | '&' varname
	      | '&' CONST		=> Needed for constant strings.
					   Not present in the original
					   grammar.
	      | call_expr
	      | unop val
	      | '(' cast ')' varname
	      | '({' all_stmts '})'	=> Original grammar does not
	                                   allow expression-statements.
					   We shouldn't, either.  But
					   for now it is easier if we
					   do.

	      (cast here stands for all valid C typecasts)  */

int
is_simple_unary_expr (t)
     tree t;
{
  /* Additions to the original grammar.  Allow STMT_EXPR, SAVE_EXPR and
     EXPR_WITH_FILE_LOCATION wrappers.  */
  if (TREE_CODE (t) == SAVE_EXPR
      || TREE_CODE (t) == EXPR_WITH_FILE_LOCATION
      || TREE_CODE (t) == NON_LVALUE_EXPR)
    return is_simple_unary_expr (TREE_OPERAND (t, 0));

  if (is_simple_varname (t) || is_simple_const (t))
    return 1;

  if (TREE_CODE (t) == INDIRECT_REF
      && is_simple_id (TREE_OPERAND (t, 0)))
    return 1;

  if (TREE_CODE (t) == ADDR_EXPR
      && (is_simple_varname (TREE_OPERAND (t, 0))
	  || is_simple_const (TREE_OPERAND (t, 0))))
    return 1;

  if (is_simple_call_expr (t))
    return 1;

  if (TREE_CODE_CLASS (TREE_CODE (t)) == '1'
      && is_simple_val (TREE_OPERAND (t, 0)))
    return 1;

  if (is_simple_cast (t))
    return 1;

  /* Additions to the original grammar.  FIXME:  Statement-expressions should
     really be expanded.  */
  if (TREE_CODE (t) == STMT_EXPR
      && is_simple_stmt (STMT_EXPR_STMT (t)))
    return 1;

  /* Addition to the original grammar.  Allow BIT_FIELD_REF nodes where
     operand 0 is a SIMPLE identifier and operands 1 and 2 are SIMPLE
     values.  */
  if (TREE_CODE (t) == BIT_FIELD_REF)
    return (is_simple_id (TREE_OPERAND (t, 0))
	    && is_simple_val (TREE_OPERAND (t, 1))
	    && is_simple_val (TREE_OPERAND (t, 2)));

  /* Addition to the original grammar.  Allow VA_ARG_EXPR nodes.  */
  if (TREE_CODE (t) == VA_ARG_EXPR)
    return 1;

  return 0;
}

/* }}} */

/** {{{ is_simple_call_expr ()

    Return nonzero if T is a SIMPLE call expression:

      call_expr
	      : ID '(' arglist ')'

      arglist
	      : arglist ',' val
	      | val  */

int
is_simple_call_expr (t)
     tree t;
{
  tree decl;

  if (TREE_CODE (t) != CALL_EXPR)
    return 0;

  /* Consider that calls to builtin functions are in SIMPLE form already.

     FIXME: The simplifier will not simplify these calls because some
	    builtins need specific arguments (e.g., __builtin_stdarg_start
	    wants one of the function arguments as its last parameters).  */
  decl = get_callee_fndecl (t);
  if (decl && DECL_BUILT_IN (decl))
    return 1;

  return (is_simple_id (TREE_OPERAND (t, 0))
          && is_simple_arglist (TREE_OPERAND (t, 1)));
}

/* }}} */

/** {{{ is_simple_arglist ()

    Return nonzero if T is a SIMPLE argument list:

      arglist
	      : arglist ',' val
	      | val  */

int
is_simple_arglist (t)
     tree t;
{
  tree op;

  /* Check that each argument is also in SIMPLE form.  */
  for (op = t; op; op = TREE_CHAIN (op))
    if (! is_simple_val (TREE_VALUE (op)))
      return 0;

  return 1;
}

/* }}} */

/** {{{ is_simple_varname ()

    Return nonzero if T is SIMPLE variable name:

      varname
	      : arrayref
	      | compref
	      | ID     */

int
is_simple_varname (t)
     tree t;
{
  return (is_simple_id (t) || is_simple_arrayref (t) || is_simple_compref (t));
}

/* }}} */

/** {{{ is_simple_const ()

    Return nonzero if T is a constant.  */

int
is_simple_const (t)
     tree t;
{
  return (TREE_CODE (t) == INTEGER_CST
	  || TREE_CODE (t) == REAL_CST
	  || TREE_CODE (t) == STRING_CST
	  || TREE_CODE (t) == LABEL_DECL
	  || TREE_CODE (t) == RESULT_DECL
	  || TREE_CODE (t) == COMPLEX_CST);
}

/* }}} */

/** {{{ is_simple_id ()

    Return nonzero if T is a SIMPLE identifier.  */

int
is_simple_id (t)
     tree t;
{
  return (TREE_CODE (t) == VAR_DECL
	  || TREE_CODE (t) == FUNCTION_DECL
	  || TREE_CODE (t) == PARM_DECL
	  || TREE_CODE (t) == FIELD_DECL
	  /* Additions to original grammar.  Allow identifiers wrapped
	     in NON_LVALUE_EXPR, EXPR_WITH_FILE_LOCATION and SAVE_EXPR.
	     Allow the address of a function decl.  */
	  || ((TREE_CODE (t) == NON_LVALUE_EXPR
	       || TREE_CODE (t) == EXPR_WITH_FILE_LOCATION
	       || TREE_CODE (t) == SAVE_EXPR)
	      && is_simple_id (TREE_OPERAND (t, 0)))
	  || (TREE_CODE (t) == ADDR_EXPR
	      && TREE_CODE (TREE_OPERAND (t, 0)) == FUNCTION_DECL));
}

/* }}} */

/** {{{ is_simple_val ()

    Return nonzero if T is an identifier or a constant.  */

int
is_simple_val (t)
     tree t;
{
  return (is_simple_id (t) || is_simple_const (t));
}

/* }}} */

/** {{{ is_simple_arrayref ()

    Return nonzero if T is an array reference of the form:

      arrayref
	      : arraybase reflist	=> 'arraybase' is any valid C array
					    name.  The original grammar
					    allowed only ID here, but we
					    cannot simplify array bases
					    because we may get invalid
					    code.

      reflist
	      : '[' val ']'
	      | reflist '[' val ']'  */

int
is_simple_arrayref (t)
     tree t;
{
  return (TREE_CODE (t) == ARRAY_REF
          && is_simple_val (TREE_OPERAND (t, 1)));
}

/* }}} */

/** {{{ is_simple_compref ()

    Return nonzero if T is a component reference of the form:

      compref
	      : '(' '*' ID ')' '.' idlist
	      : '&' ID '.' idlist
	      | idlist

      idlist
	      : idlist '.' ID
	      | ID  */

int
is_simple_compref (t)
     tree t;
{
  return (TREE_CODE (t) == COMPONENT_REF
	  && is_simple_compref_lhs (TREE_OPERAND (t, 0))
	  && is_simple_id (TREE_OPERAND (t, 1)));
}

/* }}} */

/** {{{ is_simple_compref_lhs ()

    Return nonzero if T is allowed on the left side of a component
    reference.  */

int
is_simple_compref_lhs (t)
     tree t;
{
  /* Allow ID, *ID or an idlist on the left side.  */
  return (is_simple_id (t)
	  || (TREE_CODE (t) == INDIRECT_REF
	      && is_simple_id (TREE_OPERAND (t, 0)))
	  || (TREE_CODE (t) == ADDR_EXPR
	      && is_simple_id (TREE_OPERAND (t, 0)))
	  || is_simple_compref (t));
}

/* }}} */

/** {{{ is_simple_cast ()

   Return nonzero if T is a typecast operation of the form
   '(' cast ')' varname.  */

int
is_simple_cast (t)
     tree t;
{
  return (is_simple_cast_op (t) && is_simple_varname (TREE_OPERAND (t, 0)));
}

/* }}} */

/** {{{ is_simple_cast_op ()

    Return nonzero if T is a typecast operator.  */

int
is_simple_cast_op (t)
     tree t;
{
  return (TREE_CODE (t) == NOP_EXPR
	  || TREE_CODE (t) == CONVERT_EXPR
          || TREE_CODE (t) == FIX_TRUNC_EXPR
          || TREE_CODE (t) == FIX_CEIL_EXPR
          || TREE_CODE (t) == FIX_FLOOR_EXPR
          || TREE_CODE (t) == FIX_ROUND_EXPR);
}

/* }}} */

/** {{{ is_simple_exprseq ()

    Return 1 if T is a SIMPLE expression sequence:

      exprseq
	      : exprseq ',' expr
	      | expr  */

int
is_simple_exprseq (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  return (is_simple_expr (t)
          || (TREE_CODE (t) == COMPOUND_EXPR
	      && is_simple_expr (TREE_OPERAND (t, 0))
	      && is_simple_exprseq (TREE_OPERAND (t, 1))));
}

/* }}} */
