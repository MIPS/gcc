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
#include "output.h"

/*  SIMPLE C Grammar
  
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

      NOTE: This is not possible for static variables, so we allow
	    initializers there.

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
	      | call_expr
	      | unop val
	      | '(' cast ')' varname

	      (cast here stands for all valid C typecasts)

      binary_expr
	      : val binop val

      unop
	      : '+'
	      | '-'
	      | '!'
	      | '~'

      binop
	      : relop
	      | '-'
	      | '+'
	      | '/'
	      | '*'
	      | '%'
	      | '&'
	      | '|'
	      | '<<'
	      | '>>'
	      | '^'

      relop
	      : '<'
	      | '<='
	      | '>'
	      | '>='
	      | '=='
	      | '!='

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
	      | '(' '*' ID ')' reflist  => extension because ARRAY_REF
	      				   requires an ARRAY_TYPE argument.

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

/* Validation of SIMPLE statements.  */

/*  Return nonzero if T is a statement that complies with the SIMPLE
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
      {
	int s1, s2, s3, s4;

	if (TREE_CODE (FOR_INIT_STMT (t)) == DECL_STMT)
	  s1 = 0;
	else
	  s1 = is_simple_exprseq (EXPR_STMT_EXPR (FOR_INIT_STMT (t)));

	s2 = is_simple_condexpr (FOR_COND (t));
	s3 = is_simple_exprseq (FOR_EXPR (t));
	s4 = is_simple_stmt (FOR_BODY (t));

	return (s1 && s2 && s3 && s4);
      }

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
	  return is_simple_rhs (TREE_OPERAND (RETURN_EXPR (t), 1));
	else
	  return 1;
      }

    case DECL_STMT:
      return is_simple_decl_stmt (t);

    default:
      return 0;
    }
}

/* Returns nonzero if STMT is a SIMPLE declaration, i.e. one with no
   initializer.

   This is not appropriate for static decls, so we leave them alone.  */

int
is_simple_decl_stmt (stmt)
     tree stmt;
{
  tree decl = DECL_STMT_DECL (stmt);
  tree init = DECL_INITIAL (decl);

  if (!is_simple_val (DECL_SIZE_UNIT (decl)))
    return 0;

  /* Plain decls are simple.  */
  if (init == NULL_TREE || init == error_mark_node)
    return 1;

  /* Don't mess with a compile-time initializer.  */
  if (TREE_STATIC (decl))
    return 1;

  return 0;
}

/* Returns nonzero if T is a simple CONSTRUCTOR:

     aggr_init: '{' vals '}'
     vals: aggr_init_elt | vals ',' aggr_init_elt
     aggr_init_elt: val | aggr_init

   This is an extension to SIMPLE.  Perhaps CONSTRUCTORs should be
   eliminated entirely?  */
   
int
is_simple_constructor (t)
     tree t;
{
  tree elt_list;

  if (TREE_CODE (t) != CONSTRUCTOR)
    return 0;

  if (TREE_STATIC (t))
    return 1;

  for (elt_list = CONSTRUCTOR_ELTS (t); elt_list;
       elt_list = TREE_CHAIN (elt_list))
    if (!is_simple_constructor_elt (TREE_VALUE (elt_list)))
      return 0;

  return 1;
}

/* Returns nonzero if T is a simple aggr_init_elt, as above.  */

int
is_simple_constructor_elt (t)
     tree t;
{
  return (is_simple_val (t)
	  || is_simple_constructor (t));
}

/* Returns nonzero if T is a simple initializer for a decl, for use in the
   INIT_EXPR we will generate.  This is the same as the right side of a
   MODIFY_EXPR, but here we also allow a CONSTRUCTOR.  */

int
is_simple_initializer (t)
     tree t;
{
  return (is_simple_rhs (t)
	  || is_simple_constructor (t));
}

/*  Return nonzero if T is a SIMPLE compound statement.

      compsmt
	      : '{' all_stmts '}'
	      | '{' '}'
	      | '{' decls all_stmts '}'
	      | '{' decls '}'  */

int
is_simple_compstmt (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  /* Look for '{'.  */
  if (TREE_CODE (t) != SCOPE_STMT
      || !SCOPE_BEGIN_P (t))
    return 0;

  /* Test all the statements in the body.  */
  for (t = TREE_CHAIN (t);
       t && !(TREE_CODE (t) == SCOPE_STMT && SCOPE_END_P (t));
       t = TREE_CHAIN (t))
    if (!is_simple_stmt (t))
      return 0;

  /* Look for '}'.  */
  if (t
      && (TREE_CODE (t) != SCOPE_STMT
	  || !SCOPE_END_P (t)))
    return 0;

  return 1;
}



/* Validation of SIMPLE expressions.  */

/*  Return nonzero if T is an expression that complies with the SIMPLE
    grammar.

      expr
	      : rhs
	      | modify_expr  */

int
is_simple_expr (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  return (is_simple_rhs (t) || is_simple_modify_expr (t));
}


/*  Return nonzero if T is a SIMPLE RHS:

      rhs
	      : binary_expr
	      | unary_expr  */

int
is_simple_rhs (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  return (is_simple_binary_expr (t)
          || is_simple_unary_expr (t));
}


/*  Return nonzero if T is a SIMPLE assignment expression:

      modify_expr
	      : varname '=' rhs
	      | '*' ID '=' rhs  */

int
is_simple_modify_expr (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  /* Additions to the original grammar.  Allow NON_LVALUE_EXPR
     and EXPR_WITH_FILE_LOCATION wrappers.  */
  if (TREE_CODE (t) == EXPR_WITH_FILE_LOCATION
      || TREE_CODE (t) == NON_LVALUE_EXPR)
    return is_simple_modify_expr (TREE_OPERAND (t, 0));

  return ((TREE_CODE (t) == MODIFY_EXPR
	   || TREE_CODE (t) == INIT_EXPR)
	  && is_simple_modify_expr_lhs (TREE_OPERAND (t, 0))
	  && is_simple_rhs (TREE_OPERAND (t, 1)));
}


/*  Return nonzero if T is a valid LHS for a SIMPLE assignment expression.  */

int
is_simple_modify_expr_lhs (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  return (is_simple_varname (t)
	  || (TREE_CODE (t) == INDIRECT_REF
	      && is_simple_id (TREE_OPERAND (t, 0))));
}


/*  Return true if CODE is designates a SIMPLE relop:

      relop
	      : '<'
	      | '<='
	      ...  */
    
bool
is_simple_relop (code)
     enum tree_code code;
{
  return (TREE_CODE_CLASS (code) == '<'
	  || code == TRUTH_AND_EXPR
	  || code == TRUTH_OR_EXPR
	  || code == TRUTH_XOR_EXPR);
}


/*  Return nonzero if T is a SIMPLE binary expression:

      binary_expr
	      : val binop val  */

int
is_simple_binary_expr (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  /* Additions to the original grammar.  Allow NON_LVALUE_EXPR
     and EXPR_WITH_FILE_LOCATION wrappers.  */
  if (TREE_CODE (t) == EXPR_WITH_FILE_LOCATION
      || TREE_CODE (t) == NON_LVALUE_EXPR)
    return is_simple_binary_expr (TREE_OPERAND (t, 0));

  return ((TREE_CODE_CLASS (TREE_CODE (t)) == '2'
	   || is_simple_relop (TREE_CODE (t)))
	  && is_simple_val (TREE_OPERAND (t, 0))
	  && is_simple_val (TREE_OPERAND (t, 1)));
}


/*  Return nonzero if T is a SIMPLE conditional expression:

      condexpr
	      : val
	      | val relop val  */

int
is_simple_condexpr (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  /* Additions to the original grammar.  Allow NON_LVALUE_EXPR
     and EXPR_WITH_FILE_LOCATION wrappers.  */
  if (TREE_CODE (t) == EXPR_WITH_FILE_LOCATION
      || TREE_CODE (t) == NON_LVALUE_EXPR)
    return is_simple_condexpr (TREE_OPERAND (t, 0));

  return (is_simple_val (t)
	  || (is_simple_relop (TREE_CODE (t))
	      && is_simple_val (TREE_OPERAND (t, 0))
	      && is_simple_val (TREE_OPERAND (t, 1))));
}


/*  Return nonzero if T is a unary expression as defined by the SIMPLE
    grammar:

      unary_expr
	      : simp_expr
	      | '*' ID
	      | '&' varname
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
  if (t == NULL_TREE)
    return 1;

  STRIP_NOPS (t);

  /* Additions to the original grammar.  Allow NON_LVALUE_EXPR and
     EXPR_WITH_FILE_LOCATION wrappers.  */
  if (TREE_CODE (t) == EXPR_WITH_FILE_LOCATION
      || TREE_CODE (t) == NON_LVALUE_EXPR)
    return is_simple_unary_expr (TREE_OPERAND (t, 0));

  if (is_simple_varname (t) || is_simple_const (t))
    return 1;

  if (TREE_CODE (t) == INDIRECT_REF
      && is_simple_id (TREE_OPERAND (t, 0)))
    return 1;

  if (TREE_CODE (t) == ADDR_EXPR
      && is_simple_addr_expr_arg (TREE_OPERAND (t, 0)))
    return 1;

  if (is_simple_call_expr (t))
    return 1;

  if (TREE_CODE_CLASS (TREE_CODE (t)) == '1'
      && is_simple_val (TREE_OPERAND (t, 0)))
    return 1;

  if (is_simple_cast (t))
    return 1;

  /* Addition to the original grammar.  Allow BIT_FIELD_REF nodes where
     operand 0 is a SIMPLE identifier and operands 1 and 2 are SIMPLE
     values.  */
  /* FIXME: This breaks stage2.  I still haven't figured out why.  When
	    fixing remember to undo a similar change in simplify_expr.  */
  if (TREE_CODE (t) == BIT_FIELD_REF)
    return 1;
#if 0
    return (is_simple_id (TREE_OPERAND (t, 0))
	    && is_simple_val (TREE_OPERAND (t, 1))
	    && is_simple_val (TREE_OPERAND (t, 2)));
#endif

  /* Addition to the original grammar.  Allow VA_ARG_EXPR nodes.  */
  if (TREE_CODE (t) == VA_ARG_EXPR)
    return 1;

  /* Addition to the original grammar.  Allow simple constructor
     expressions.  */
  if (TREE_CODE (t) == CONSTRUCTOR)
    return is_simple_constructor (t);

  return 0;
}


/*  Return nonzero if T is a SIMPLE call expression:

      call_expr
	      : ID '(' arglist ')'

      arglist
	      : arglist ',' val
	      | val  */

int
is_simple_call_expr (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  if (TREE_CODE (t) != CALL_EXPR)
    return 0;

  /* Some builtins cannot be simplified because the require specific
     arguments.  */
  if (!is_simplifiable_builtin (t))
    return 1;

  return (is_simple_id (TREE_OPERAND (t, 0))
          && is_simple_arglist (TREE_OPERAND (t, 1)));
}


/*  Return nonzero if T is a SIMPLE argument list:

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


/*  Return nonzero if T is SIMPLE variable name:

      varname
	      : arrayref
	      | compref
	      | ID     */

int
is_simple_varname (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  return (is_simple_id (t)
#if 0
	  || is_simple_arrayref (t) || is_simple_compref (t)
#else
	  || is_simple_compound_lval (t)
#endif
	  );
}

/* Returns nonzero if T is an array or member reference of the form:

      compound_lval
      	      : min_lval '[' val ']'
	      | min_lval '.' ID
	      | compound_lval '[' val ']'
	      | compound_lval '.' ID

   This is not part of the original SIMPLE definition, which separates
   array and member references, but it seems reasonable to handle them
   together.  Also, this way we don't run into problems with union
   aliasing; gcc requires that for accesses through a union to alias, the
   union reference must be explicit, which was not always the case when we
   were splitting up array and member refs.  */

int
is_simple_compound_lval (t)
     tree t;
{
  /* Allow arrays of complex types.  */
  if (TREE_CODE (t) == REALPART_EXPR
      || TREE_CODE (t) == IMAGPART_EXPR)
    t = TREE_OPERAND (t, 0);

  /* Allow arrays wrapped in NON_LVALUE_EXPR nodes.  */
  if (TREE_CODE (t) == NON_LVALUE_EXPR)
    t = TREE_OPERAND (t, 0);

  if (TREE_CODE (t) != ARRAY_REF && TREE_CODE (t) != COMPONENT_REF)
    return 0;

  for (; TREE_CODE (t) == COMPONENT_REF || TREE_CODE (t) == ARRAY_REF;
       t = TREE_OPERAND (t, 0))
    {
      if (TREE_CODE (t) == ARRAY_REF
	  && !is_simple_val (TREE_OPERAND (t, 1)))
	return 0;
    }

  return is_simple_min_lval (t);
}

/*  Return nonzero if T can be used as the argument for an ADDR_EXPR node.
    This is not part of the original SIMPLE grammar, but in C99 it is
    possible to generate an address expression for a function call:

      struct A_s {
	char a[1];
      } A;

      extern struct A_s foo ();

      main()
      {
	char *t = foo().a;
      }

    When the above is compiled with -std=iso9899:1999, the front end will
    generate 't = (char *)(char[1] *)&foo ();'.  */

int
is_simple_addr_expr_arg (t)
     tree t;
{
  return (is_simple_varname (t) || is_simple_call_expr (t));
}

/*  Return nonzero if T is a constant.  */

int
is_simple_const (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  STRIP_NOPS (t);

  if (TREE_CODE (t) == ADDR_EXPR
      && TREE_CODE (TREE_OPERAND (t, 0)) == STRING_CST)
    return 1;

  return (TREE_CODE (t) == INTEGER_CST
	  || TREE_CODE (t) == REAL_CST
	  || TREE_CODE (t) == STRING_CST
	  || TREE_CODE (t) == LABEL_DECL
	  || TREE_CODE (t) == RESULT_DECL
	  || TREE_CODE (t) == COMPLEX_CST);
}


/*  Return nonzero if T is a SIMPLE identifier.  */

int
is_simple_id (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  /* Additions to the original grammar.  Allow identifiers wrapped in
     NON_LVALUE_EXPR and EXPR_WITH_FILE_LOCATION.  */
  if (TREE_CODE (t) == NON_LVALUE_EXPR
      || TREE_CODE (t) == EXPR_WITH_FILE_LOCATION)
    return is_simple_id (TREE_OPERAND (t, 0));

  /* Allow real and imaginary parts of a complex variable.  */
  if (TREE_CODE (t) == REALPART_EXPR
      || TREE_CODE (t) == IMAGPART_EXPR)
    return is_simple_id (TREE_OPERAND (t, 0));

  return (TREE_CODE (t) == VAR_DECL
	  || TREE_CODE (t) == FUNCTION_DECL
	  || TREE_CODE (t) == PARM_DECL
	  || TREE_CODE (t) == FIELD_DECL
	  || TREE_CODE (t) == LABEL_DECL
	  /* Allow the address of a function decl.  */
	  || (TREE_CODE (t) == ADDR_EXPR
	      && TREE_CODE (TREE_OPERAND (t, 0)) == FUNCTION_DECL)
	  /* Allow string constants.  */
	  || TREE_CODE (t) == STRING_CST);
}


/*  Return nonzero if T is an identifier or a constant.  */

int
is_simple_val (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  return (is_simple_id (t) || is_simple_const (t));
}


/*  Return true if T is a SIMPLE minimal lvalue, of the form

    min_lval: ID | '(' '*' ID ')'

    This never actually appears in the original SIMPLE grammar, but is
    repeated in several places.  */

int
is_simple_min_lval (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  return (is_simple_id (t)
	  || (TREE_CODE (t) == INDIRECT_REF
	      && is_simple_id (TREE_OPERAND (t, 0))));
}

#if 0
/*  Return nonzero if T is an array reference of the form:

      arrayref
	      : ID reflist
	      | '(' '*' ID ')' reflist  => extension because ARRAY_REF
	      				   requires an ARRAY_TYPE argument.

      reflist
	      : '[' val ']'
	      | reflist '[' val ']'  */

int
is_simple_arrayref (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  /* Allow arrays of complex types.  */
  if (TREE_CODE (t) == REALPART_EXPR
      || TREE_CODE (t) == IMAGPART_EXPR)
    t = TREE_OPERAND (t, 0);

  if (TREE_CODE (t) != ARRAY_REF)
    return 0;

  for (; TREE_CODE (t) == ARRAY_REF; t = TREE_OPERAND (t, 0))
    if (! is_simple_val (TREE_OPERAND (t, 1)))
      return 0;

  return is_simple_min_lval (t);
}


/*  Return nonzero if T is a component reference of the form:

      compref
	      : '(' '*' ID ')' '.' idlist
	      | idlist

      idlist
	      : idlist '.' ID
	      | ID  */

int
is_simple_compref (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  if (TREE_CODE (t) != COMPONENT_REF)
    return 0;

  for (; TREE_CODE (t) == COMPONENT_REF; t = TREE_OPERAND (t, 0))
    if (! is_simple_id (TREE_OPERAND (t, 1)))
      abort ();

  return is_simple_min_lval (t);
}
#endif

/*  Return nonzero if T is a typecast operation of the form
    '(' cast ')' varname.  */

int
is_simple_cast (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  return (is_simple_cast_op (t) && is_simple_varname (TREE_OPERAND (t, 0)));
}


/*  Return nonzero if T is a typecast operator.  */

int
is_simple_cast_op (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  return (TREE_CODE (t) == NOP_EXPR
	  || TREE_CODE (t) == CONVERT_EXPR
          || TREE_CODE (t) == FIX_TRUNC_EXPR
          || TREE_CODE (t) == FIX_CEIL_EXPR
          || TREE_CODE (t) == FIX_FLOOR_EXPR
          || TREE_CODE (t) == FIX_ROUND_EXPR);
}


/*  Return 1 if T is a SIMPLE expression sequence:

      exprseq
	      : exprseq ',' expr
	      | expr  */

int
is_simple_exprseq (t)
     tree t;
{
  /* Empty expression sequences are allowed.  */
  if (t == NULL_TREE)
    return 1;

  return (is_simple_expr (t)
          || (TREE_CODE (t) == COMPOUND_EXPR
	      && is_simple_expr (TREE_OPERAND (t, 0))
	      && is_simple_exprseq (TREE_OPERAND (t, 1))));
}

/* Return nonzero if FNDECL can be simplified.  This is needed for
   builtins like __builtin_stdarg_start expects its last parameter to be
   one of the current function's arguments.  */

int
is_simplifiable_builtin (expr)
     tree expr;
{
  enum built_in_function fcode;
  tree decl, t1, t2, t3;

  decl = get_callee_fndecl (expr);

  if (decl == NULL_TREE || !DECL_BUILT_IN (decl))
    return 1;

  fcode = DECL_FUNCTION_CODE (decl);

  switch (fcode)
    {
      /* Many of the string builtins fold certain string patterns into
         constants.  Make sure we don't simplify something which will
         be folded by the builtin later.  */

      /* foo (const char *, const char *, ...).  */
    case BUILT_IN_STRNCMP:
    case BUILT_IN_STRSPN:
    case BUILT_IN_STRSTR:
    case BUILT_IN_STRCSPN:
      t1 = TREE_VALUE (TREE_OPERAND (expr, 1));
      t2 = TREE_VALUE (TREE_CHAIN (TREE_OPERAND (expr, 1)));

      return !(string_constant (t1, &t3) || string_constant (t2, &t3));

      /* foo (const char *, ...).  */
    case BUILT_IN_STRLEN:
    case BUILT_IN_STRRCHR:
    case BUILT_IN_STRCHR:
    case BUILT_IN_INDEX:
    case BUILT_IN_FPUTS:
      t1 = TREE_VALUE (TREE_OPERAND (expr, 1));

      return !string_constant (t1, &t3);

      /* foo (..., const char *, ...).  */
    case BUILT_IN_STRCPY:
    case BUILT_IN_STRNCPY:
    case BUILT_IN_STRCAT:
    case BUILT_IN_STRNCAT:
      t2 = TREE_VALUE (TREE_CHAIN (TREE_OPERAND (expr, 1)));

      return !string_constant (t2, &t3);

    case BUILT_IN_STDARG_START:
    case BUILT_IN_VARARGS_START:
    case BUILT_IN_VA_COPY:
      return 0;

    default:
      return 1;
    }
}
