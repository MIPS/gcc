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
#include "tree-simple.h"

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

/** {{{ is_simple_expr ()

    Return non-zero if T is an expression that complies with the SIMPLE
    grammar.

      expr
	      : rhs
	      | modify_expr  */

int
is_simple_expr (t)
     tree t;
{
  if (t == NULL_TREE)
    abort ();

  return (is_simple_rhs (t) || is_simple_modify_expr (t));
}

/* }}} */

/** {{{ is_simple_rhs ()

    Return non-zero if T is a SIMPLE RHS:

      rhs
	      : binary_expr
	      | unary_expr  */

int
is_simple_rhs (t)
     tree t;
{
  return (is_simple_binary_expr (t) || is_simple_unary_expr (t));
}

/* }}} */

/** {{{ is_simple_modify_expr ()

    Return non-zero if T is a SIMPLE assignment expression:

      modify_expr
	      : varname '=' rhs
	      | '*' ID '=' rhs  */

int
is_simple_modify_expr (t)
     tree t;
{
  return (TREE_CODE (t) == MODIFY_EXPR
          && is_simple_varname (TREE_OPERAND (t, 0))
	  && is_simple_rhs (TREE_OPERAND (t, 1)));
}

/* }}} */

/** {{{ is_simple_binary_expr ()

    Return non-zero if T is a SIMPLE binary expression:

      binary_expr
	      : val binop val  */

int
is_simple_binary_expr (t)
     tree t;
{
  return (is_simple_binop (t)
	  && is_simple_val (TREE_OPERAND (t, 0))
	  && is_simple_val (TREE_OPERAND (t, 1)));
}

/* }}} */

/** {{{ is_simple_binop ()

    Return non-zero if T is a SIMPLE binary operator:

      binop
	      : relop
	      | '-'
	      | '+'
	      ....  */

int
is_simple_binop (t)
     tree t;
{
  return (TREE_CODE_CLASS (TREE_CODE (t)) == '2'
	  || is_simple_relop (t));
}

/* }}} */

/** {{{ is_simple_relop ()

    Return non-zero if T is a SIMPLE relational operator:

      relop
	      : '<'
	      | '<='
	      ...  */

int
is_simple_relop (t)
     tree t;
{
  return (TREE_CODE_CLASS (TREE_CODE (t)) == '<');
}

/* }}} */

/** {{{ is_simple_unary_expr ()

    Return non-zero if T is a unary expression as defined by the SIMPLE
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
  enum tree_code code;

  if (t == NULL_TREE)
    abort ();

  code = TREE_CODE (t);

  /* NOTE: is_simple_varname() will also return true for '*' ID.  */
  if (is_simple_varname (t) || is_simple_const (t))
    return 1;

  if (TREE_CODE (t) == ADDR_EXPR
      && is_simple_varname (TREE_OPERAND (t, 0)))
    return 1;

  if (is_simple_call_expr (t))
    return 1;

  if (TREE_CODE (t) == NEGATE_EXPR
      && is_simple_val (TREE_OPERAND (t, 0)))
    return 1;

 return is_simple_cast (t);
}

/* }}} */

/** {{{ is_simple_call_expr ()

    Return non-zero if T is a SIMPLE call expression:

      call_expr
	      : ID '(' arglist ')'

      arglist
	      : arglist ',' val
	      | val  */

int
is_simple_call_expr (t)
     tree t;
{
  tree addr, callee, op;

  if (TREE_CODE (t) != CALL_EXPR)
    return 0;

  /* Check that the callee is a SIMPLE identifier.  */
  addr = TREE_OPERAND (t, 0);
  callee = (TREE_CODE (addr) == ADDR_EXPR) ? TREE_OPERAND (addr, 0) : addr;
  if (! is_simple_id (callee))
    return 0;

  /* Check that each argument is also in SIMPLE form.  */
  for (op = TREE_OPERAND (t, 1); op; op = TREE_CHAIN (op))
    if (! is_simple_val (TREE_VALUE (op)))
      return 0;

  return 1;
}

/* }}} */

/** {{{ is_simple_varname ()

    Return non-zero if T is SIMPLE variable name:

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
	      | idlist  */
int
is_simple_varname (t)
     tree t;
{
  return (is_simple_id (t) || is_simple_arrayref (t) || is_simple_compref (t));
}

/* }}} */

/** {{{ is_simple_const ()

    Return non-zero if T is a constant.  */

int
is_simple_const (t)
     tree t;
{
  return (TREE_CODE (t) == INTEGER_CST
	  || TREE_CODE (t) == REAL_CST
	  || TREE_CODE (t) == STRING_CST
	  || TREE_CODE (t) == LABEL_DECL
	  || TREE_CODE (t) == RESULT_DECL
	  || TREE_CODE (t) == COMPLEX_CST
	  || (TREE_CODE (t) == ADDR_EXPR
	      && is_simple_const (TREE_OPERAND (t, 0))));
}

/* }}} */

/** {{{ is_simple_id ()

    Return non-zero if T is an identifier or an indirect reference to an
    identifier.  */

int
is_simple_id (t)
     tree t;
{
  return (TREE_CODE (t) == VAR_DECL
	  || TREE_CODE (t) == FUNCTION_DECL
	  || TREE_CODE (t) == PARM_DECL
	  || TREE_CODE (t) == FIELD_DECL
	  || (TREE_CODE (t) == INDIRECT_REF
	      && is_simple_id (TREE_OPERAND (t, 0))));
}

/* }}} */

/** {{{ is_simple_val ()

    Return non-zero if T is an identifier or a constant.  */

int
is_simple_val (t)
     tree t;
{
  return (is_simple_id (t) || is_simple_const (t));
}

/* }}} */

/** {{{ is_simple_arrayref ()

    Return non-zero if T is an array reference of the form:

      arrayref
	      : ID reflist

      reflist
	      : '[' val ']'
	      | reflist '[' val ']'  */

int
is_simple_arrayref (t)
     tree t;
{
  return (TREE_CODE (t) == ARRAY_REF
	  && (is_simple_id (TREE_OPERAND (t, 0))
	      || is_simple_arrayref (TREE_OPERAND (t, 0)))
          && is_simple_val (TREE_OPERAND (t, 1)));
}

/* }}} */

/** {{{ is_simple_compref ()

    Return non-zero if T is a reference of the form:

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
  return (TREE_CODE (t) == COMPONENT_REF
          && is_simple_id (TREE_OPERAND (t, 0))
	  && is_simple_id (TREE_OPERAND (t, 1)));
}

/* }}} */

/** {{{ is_simple_cast ()

   Return non-zero if T is a typecast operation of the form
   '(' cast ')' varname.  */

int
is_simple_cast (t)
     tree t;
{
  return ((TREE_CODE (t) == NOP_EXPR
	   || TREE_CODE (t) == CONVERT_EXPR
           || TREE_CODE (t) == FIX_TRUNC_EXPR
           || TREE_CODE (t) == FIX_CEIL_EXPR
           || TREE_CODE (t) == FIX_FLOOR_EXPR
           || TREE_CODE (t) == FIX_ROUND_EXPR)
          && is_simple_varname (TREE_OPERAND (t, 0)));
}

/* }}} */
