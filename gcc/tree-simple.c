/* Functions to analyze and validate GIMPLE trees.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

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
#include "tree-simple.h"
#include "output.h"
#include "rtl.h"
#include "expr.h"

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
	      | FOR '(' expr ';' condexpr ';' expr ')' stmt
	      		|
			+-> Original SIMPLE grammar allows exprseq here,
			    but this makes life more difficult to some
			    optimizers that need to learn to deal with
			    expression sequences instead of SIMPLE
			    assignments (e.g., CCP when examining
			    variable definitions)
	      	
	      | SWITCH '(' val ')' casestmts
	      | GOTO val		-> Not present in the original
					   grammar.
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
	      | RETURN rhs ';'
	      | RETURN '(' rhs ')' ';'

      casestmts
	      : '{' cases default'}'
	      | ';'
	      | '{' '}'

      cases
	      : cases case
	      | case

      case
	      : CASE CONST ':' stmtlist stop_stmt

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

/* FIXME all of the is_simple_* predicates should be changed to only test
   for appropriate top-level structures; we can safely assume that after
   simplification, a PLUS_EXPR is a simple PLUS_EXPR, so the predicate only
   needs to decide whether or not a PLUS_EXPR is suitable here.  */

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
     wrappers.  */
  if (TREE_CODE (t) == NON_LVALUE_EXPR)
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
     wrappers.  */
  if (TREE_CODE (t) == NON_LVALUE_EXPR)
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
     wrappers.  */
  if (TREE_CODE (t) == NON_LVALUE_EXPR)
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

	      (cast here stands for all valid C typecasts)  */

int
is_simple_unary_expr (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  STRIP_NOPS (t);

  /* Additions to the original grammar.  Allow NON_LVALUE_EXPR
     wrappers.  */
  if (TREE_CODE (t) == NON_LVALUE_EXPR
      || TREE_CODE (t) == VTABLE_REF)
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
  if (TREE_CODE (t) == BIT_FIELD_REF)
    return (is_simple_min_lval (TREE_OPERAND (t, 0))
	    && is_simple_val (TREE_OPERAND (t, 1))
	    && is_simple_val (TREE_OPERAND (t, 2)));

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

int
is_simple_stmt (t)
     tree t ATTRIBUTE_UNUSED;
{
  return 1;
}

/*  Return nonzero if T is a SIMPLE identifier.  */

int
is_simple_id (t)
     tree t;
{
  if (t == NULL_TREE)
    return 1;

  /* Additions to the original grammar.  Allow identifiers wrapped in
     NON_LVALUE_EXPR.  */
  if (TREE_CODE (t) == NON_LVALUE_EXPR)
    return is_simple_id (TREE_OPERAND (t, 0));

  /* Allow real and imaginary parts of a complex variable.  */
  if (TREE_CODE (t) == REALPART_EXPR
      || TREE_CODE (t) == IMAGPART_EXPR)
    return is_simple_id (TREE_OPERAND (t, 0));

  return (TREE_CODE (t) == VAR_DECL
	  || TREE_CODE (t) == FUNCTION_DECL
	  || TREE_CODE (t) == PARM_DECL
	  || TREE_CODE (t) == RESULT_DECL
	  || TREE_CODE (t) == FIELD_DECL
	  || TREE_CODE (t) == LABEL_DECL
	  /* FIXME make this a decl.  */
	  || TREE_CODE (t) == EXC_PTR_EXPR
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
   target-defined builtins that may need specific tree nodes in their
   argument list.  */

int
is_simplifiable_builtin (expr)
     tree expr;
{
  tree decl;

  decl = get_callee_fndecl (expr);

  /* Do not simplify target-defined builtin functions.
     FIXME: Maybe we should add a target hook for allowing this in the
	    future?  */
  if (decl && DECL_BUILT_IN_CLASS (decl) == BUILT_IN_MD)
    return 0;

  return 1;
}

/* Given an _EXPR TOP, reorganize all of the nested _EXPRs with the same
   code so that they only appear as the second operand.  This should only
   be used for tree codes which are truly associative, such as
   COMPOUND_EXPR and TRUTH_ANDIF_EXPR.  Arithmetic is not associative
   enough, due to the limited precision of arithmetic data types.

   This transformation is conservative; the operand 0 of a matching tree
   node will only change if it is also a matching node.  */

tree
right_assocify_expr (top)
     tree top;
{
  tree *p = &top;
  enum tree_code code = TREE_CODE (*p);
  while (TREE_CODE (*p) == code)
    {
      tree cur = *p;
      tree lhs = TREE_OPERAND (cur, 0);
      if (TREE_CODE (lhs) == code)
	{
	  /* There's a left-recursion.  If we have ((a, (b, c)), d), we
	     want to rearrange to (a, (b, (c, d))).  */
	  tree *q;

	  /* Replace cur with the lhs; move (a, *) up.  */
	  *p = lhs;

	  if (code == COMPOUND_EXPR)
	    {
	      /* We need to give (b, c) the type of c; previously lhs had
		 the type of b.  */
	      TREE_TYPE (lhs) = TREE_TYPE (cur);
	      if (TREE_SIDE_EFFECTS (cur))
		TREE_SIDE_EFFECTS (lhs) = 1;
	    }

	  /* Walk through the op1 chain from there until we find something
	     with a different code.  In this case, c.  */
	  for (q = &TREE_OPERAND (lhs, 1); TREE_CODE (*q) == code;
	       q = &TREE_OPERAND (*q, 1))
	    TREE_TYPE (*q) = TREE_TYPE (cur);

	  /* Change (*, d) into (c, d).  */
	  TREE_OPERAND (cur, 0) = *q;

	  /* And plug it in where c used to be.  */
	  *q = cur;
	}
      else
	p = &TREE_OPERAND (cur, 1);
    }
  return top;
}

/* Normalize the statement TOP.  If it is a COMPOUND_EXPR, reorganize it so
   that we can traverse it without recursion.  If it is null, replace it
   with a nop.  */

tree
rationalize_compound_expr (top)
     tree top;
{
  if (top == NULL_TREE)
    top = empty_stmt_node;
  else if (TREE_CODE (top) == COMPOUND_EXPR)
    top = right_assocify_expr (top);

  return top;
}

/* Given a SIMPLE varname (an ID, an arrayref or a compref), return the
   base symbol for the variable.  */

tree
get_base_symbol (t)
     tree t;
{
  STRIP_NOPS (t);

  switch (TREE_CODE (t))
    {
    case VAR_DECL:
    case FUNCTION_DECL:
    case PARM_DECL:
    case FIELD_DECL:
    case LABEL_DECL:
      return t;

    case SSA_NAME:
      return get_base_symbol (SSA_NAME_VAR (t));

    case ARRAY_REF:
    case COMPONENT_REF:
    case INDIRECT_REF:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
      return get_base_symbol (TREE_OPERAND (t, 0));

    default:
      return NULL_TREE;
    }
}

void
recalculate_side_effects (t)
     tree t;
{
  enum tree_code code = TREE_CODE (t);
  int fro = first_rtl_op (code);
  int i;

  switch (TREE_CODE_CLASS (code))
    {
    case 'e':
      switch (code)
	{
	case INIT_EXPR:
	case MODIFY_EXPR:
	case VA_ARG_EXPR:
	case RTL_EXPR:
	case PREDECREMENT_EXPR:
	case PREINCREMENT_EXPR:
	case POSTDECREMENT_EXPR:
	case POSTINCREMENT_EXPR:
	  /* All of these have side-effects, no matter what their
	     operands are.  */
	  return;

	default:
	  break;
	}
      /* Fall through.  */

    case '<':  /* a comparison expression */
    case '1':  /* a unary arithmetic expression */
    case '2':  /* a binary arithmetic expression */
    case 'r':  /* a reference */
      TREE_SIDE_EFFECTS (t) = 0;
      for (i = 0; i < fro; ++i)
	{
	  tree op = TREE_OPERAND (t, i);
	  if (op && TREE_SIDE_EFFECTS (op))
	    TREE_SIDE_EFFECTS (t) = 1;
	}
      break;
   }
}
