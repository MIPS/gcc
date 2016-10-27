/* Parser for GIMPLE.
   Copyright (C) 2016 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "target.h"
#include "function.h"
#include "c-tree.h"
#include "timevar.h"
#include "stringpool.h"
#include "cgraph.h"
#include "attribs.h"
#include "stor-layout.h"
#include "varasm.h"
#include "trans-mem.h"
#include "c-family/c-pragma.h"
#include "c-lang.h"
#include "c-family/c-objc.h"
#include "plugin.h"
#include "omp-low.h"
#include "builtins.h"
#include "gomp-constants.h"
#include "c-family/c-indentation.h"
#include "gimple-expr.h"
#include "context.h"
#include "gcc-rich-location.h"
#include "c-parser.h"
#include "tree-vrp.h"
#include "tree-pass.h"
#include "tree-pretty-print.h"
#include "tree.h"
#include "basic-block.h"
#include "gimple.h"
#include "gimple-pretty-print.h"
#include "tree-ssa.h"
#include "pass_manager.h"
#include "tree-ssanames.h"
#include "gimple-ssa.h"
#include "tree-dfa.h"


/* Gimple parsing functions.  */
static bool c_parser_gimple_compound_statement (c_parser *, gimple_seq *);
static void c_parser_gimple_label (c_parser *, gimple_seq *);
static void c_parser_gimple_expression (c_parser *, gimple_seq *);
static struct c_expr c_parser_gimple_binary_expression (c_parser *, enum tree_code *);
static struct c_expr c_parser_gimple_unary_expression (c_parser *);
static struct c_expr c_parser_gimple_postfix_expression (c_parser *);
static struct c_expr c_parser_gimple_postfix_expression_after_primary (c_parser *,
								       location_t,
								       struct c_expr);
static void c_parser_gimple_declaration (c_parser *);
static void c_parser_gimple_goto_stmt (location_t, tree, gimple_seq *);
static void c_parser_gimple_if_stmt (c_parser *, gimple_seq *);
static void c_parser_gimple_switch_stmt (c_parser *, gimple_seq *);
static void c_parser_gimple_return_stmt (c_parser *, gimple_seq *);
static void c_finish_gimple_return (location_t, tree);
static c_expr c_parser_parse_ssa_names (c_parser *);
static tree c_parser_gimple_paren_condition (c_parser *);
static vec<tree, va_gc> *c_parser_gimple_expr_list (c_parser *,
		    vec<tree, va_gc> **, vec<location_t> *);


/* Parse the body of a function declaration marked with "__GIMPLE".  */

void
c_parser_parse_gimple_body (c_parser *parser)
{
  gimple_seq seq = NULL;
  gimple_seq body = NULL;
  tree stmt = push_stmt_list ();
  push_scope ();
  location_t loc1 = c_parser_peek_token (parser)->location;

  init_tree_ssa (cfun);

  if (! c_parser_gimple_compound_statement (parser, &seq))
    {
      gimple *ret = gimple_build_return (NULL);
      gimple_seq_add_stmt (&seq, ret);
    }

  tree block = pop_scope ();
  stmt = pop_stmt_list (stmt);
  stmt = c_build_bind_expr (loc1, block, stmt);

  block = DECL_INITIAL (current_function_decl);
  BLOCK_SUBBLOCKS (block) = NULL_TREE;
  BLOCK_CHAIN (block) = NULL_TREE;
  TREE_ASM_WRITTEN (block) = 1;

  gbind *bind_stmt = gimple_build_bind (BIND_EXPR_VARS (stmt), NULL,
					BIND_EXPR_BLOCK (stmt));
  gimple_bind_set_body (bind_stmt, seq);
  gimple_seq_add_stmt (&body, bind_stmt);
  gimple_set_body (current_function_decl, body);

  /* While we have SSA names in the IL we do not have a CFG built yet
     and PHIs are represented using a PHI internal function.  We do
     have lowered control flow and exception handling (well, we do not
     have parser support for EH yet).  */
  cfun->curr_properties = PROP_gimple_any | PROP_gimple_lcf | PROP_gimple_leh;

  return;
}

/* Parse a compound statement in gimple function body.

   gimple-statement:
     gimple-statement
     gimple-declaration-statement
     gimple-if-statement
     gimple-switch-statement
     gimple-labeled-statement
     gimple-expression-statement
     gimple-goto-statement
     gimple-phi-statement
     gimple-return-statement
*/

static bool
c_parser_gimple_compound_statement (c_parser *parser, gimple_seq *seq)
{
  bool return_p = false;

  if (! c_parser_require (parser, CPP_OPEN_BRACE, "expected %<{%>"))
    return false;

  if (c_parser_next_token_is (parser, CPP_CLOSE_BRACE))
    {
      c_parser_consume_token (parser);
      return false;
    }

  if (c_parser_next_token_is (parser, CPP_CLOSE_BRACE))
    {
      c_parser_error (parser, "expected declaration or statement");
      c_parser_consume_token (parser);
      return false;
    }

  while (c_parser_next_token_is_not (parser, CPP_CLOSE_BRACE))
    {
      if (parser->error)
	{
	  c_parser_skip_until_found (parser, CPP_CLOSE_BRACE, NULL);
	  return return_p;
	}

      if (c_parser_next_token_is (parser, CPP_NAME)
	  && c_parser_peek_2nd_token (parser)->type == CPP_COLON)
	c_parser_gimple_label (parser, seq);

      else if (c_parser_next_tokens_start_declaration (parser))
	c_parser_gimple_declaration (parser);

      else if (c_parser_next_token_is (parser, CPP_EOF))
	{
	  c_parser_error (parser, "expected declaration or statement");
	  return return_p;
	}

      else
	{
	  switch (c_parser_peek_token (parser)->type)
	    {
	    case CPP_KEYWORD:
	      switch (c_parser_peek_token (parser)->keyword)
		{
		case RID_IF:
		  c_parser_gimple_if_stmt (parser, seq);
		  break;
		case RID_SWITCH:
		  c_parser_gimple_switch_stmt (parser, seq);
		  break;
		case RID_GOTO:
		    {
		      location_t loc = c_parser_peek_token (parser)->location;
		      c_parser_consume_token (parser);
		      if (c_parser_next_token_is (parser, CPP_NAME))
			{
			  c_parser_gimple_goto_stmt (loc,
						     c_parser_peek_token
						       (parser)->value,
						     seq);
			  c_parser_consume_token (parser);
			  if (! c_parser_require (parser, CPP_SEMICOLON,
						  "expected %<;%>"))
			    return return_p;
			}
		    }
		  break;
		case RID_RETURN:
		  return_p = true;
		  c_parser_gimple_return_stmt (parser, seq);
		  if (! c_parser_require (parser, CPP_SEMICOLON,
					  "expected %<;%>"))
		    return return_p;
		  break;
		default:
		  goto expr_stmt;
		}
	      break;
	    case CPP_SEMICOLON:
	      c_parser_consume_token (parser);
	      break;
	    default:
	    expr_stmt:
	      c_parser_gimple_expression (parser, seq);
	      if (! c_parser_require (parser, CPP_SEMICOLON, "expected %<;%>"))
		return return_p;
	    }
	}
    }
  c_parser_consume_token (parser);
  return return_p;
}

/* Parse a gimple expression.

   gimple-expression:
     gimple-unary-expression
     gimple-call-statement
     gimple-binary-expression
     gimple-assign-expression
     gimple-cast-expression

*/

static void
c_parser_gimple_expression (c_parser *parser, gimple_seq *seq)
{
  struct c_expr lhs, rhs;
  gimple *assign = NULL;
  enum tree_code subcode = NOP_EXPR;
  location_t loc;
  tree arg = NULL_TREE;
  auto_vec<tree> vargs;

  lhs = c_parser_gimple_unary_expression (parser);
  rhs.value = error_mark_node;

  if (c_parser_next_token_is (parser, CPP_EQ))
    c_parser_consume_token (parser);

  loc = EXPR_LOCATION (lhs.value);

  /* GIMPLE call expression.  */
  if (c_parser_next_token_is (parser, CPP_SEMICOLON)
      && TREE_CODE (lhs.value) == CALL_EXPR)
    {
      gimple *call;
      call = gimple_build_call_from_tree (lhs.value);
      gimple_seq_add_stmt (seq, call);
      gimple_set_location (call, loc);
      return;
    }

  /* Cast expression.  */
  if (c_parser_next_token_is (parser, CPP_OPEN_PAREN)
      && c_token_starts_typename (c_parser_peek_2nd_token (parser)))
    {
      /* TODO: have a gimple_cast_expr function.  */
      rhs = c_parser_cast_expression (parser, NULL);
      if (lhs.value != error_mark_node &&
	  rhs.value != error_mark_node)
	{
	  assign = gimple_build_assign (lhs.value, rhs.value);
	  gimple_seq_add_stmt (seq, assign);
	  gimple_set_location (assign, loc);
	  return;
	}
    }

  if (POINTER_TYPE_P (TREE_TYPE (lhs.value)))
    {
      STRIP_USELESS_TYPE_CONVERSION (rhs.value);
      if (! useless_type_conversion_p (TREE_TYPE (lhs.value),
				       TREE_TYPE (rhs.value)))
	rhs.value = fold_convert_loc (loc, TREE_TYPE (lhs.value), rhs.value);
    }

  /* Pointer expression.  */
  if (TREE_CODE (lhs.value) == INDIRECT_REF)
    {
      tree save_expr = lhs.value;
      bool volatilep = TREE_THIS_VOLATILE (lhs.value);
      bool notrap = TREE_THIS_NOTRAP (lhs.value);
      tree saved_ptr_type = TREE_TYPE (TREE_OPERAND (lhs.value, 0));

      lhs.value = fold_indirect_ref_loc (loc, lhs.value);
      if (lhs.value == save_expr)
	{
	  lhs.value = fold_build2_loc (input_location, MEM_REF,
				       TREE_TYPE (lhs.value),
				       TREE_OPERAND (lhs.value, 0),
				       build_int_cst (saved_ptr_type, 0));
	  TREE_THIS_VOLATILE (lhs.value) = volatilep;
	  TREE_THIS_NOTRAP (lhs.value) = notrap;
	}
    }

  if (c_parser_next_token_is (parser, CPP_AND)
      || c_parser_next_token_is (parser, CPP_MULT)
      || c_parser_next_token_is (parser, CPP_PLUS)
      || c_parser_next_token_is (parser, CPP_MINUS)
      || c_parser_next_token_is (parser, CPP_COMPL)
      || c_parser_next_token_is (parser, CPP_NOT))
    {
      rhs = c_parser_gimple_unary_expression (parser);
      assign = gimple_build_assign (lhs.value, rhs.value);
      gimple_set_location (assign, loc);
      gimple_seq_add_stmt (seq, assign);
      return;
    }

  /* GIMPLE PHI expression.  */
  if (c_parser_next_token_is_keyword (parser, RID_PHI))
    {
      c_parser_consume_token (parser);

      if (! c_parser_require (parser, CPP_OPEN_PAREN, "expected %<(%>"))
	return;

      if (c_parser_next_token_is (parser, CPP_OPEN_PAREN))
	c_parser_consume_token (parser);

      while (c_parser_next_token_is_not (parser, CPP_CLOSE_PAREN))
	{
	  if (c_parser_next_token_is (parser, CPP_NAME)
	      && c_parser_peek_2nd_token (parser)->type == CPP_COLON)
	    {
	      arg = lookup_label_for_goto (loc,
					   c_parser_peek_token (parser)->value);
	      c_parser_consume_token (parser);

	      if (c_parser_next_token_is (parser, CPP_COLON))
		c_parser_consume_token (parser);
	      vargs.safe_push (arg);
	    }
	  else if (c_parser_next_token_is (parser, CPP_COMMA))
	    c_parser_consume_token (parser);
	  else
	    {
	      arg = c_parser_parse_ssa_names (parser).value;
	      vargs.safe_push (arg);
	    }
	}

      c_parser_skip_until_found (parser, CPP_CLOSE_PAREN,
				 "expected %<)%>");

      /* Build internal function for PHI.  */
      gcall *call_stmt = gimple_build_call_internal_vec (IFN_PHI, vargs);
      gimple_call_set_lhs (call_stmt, lhs.value);
      gimple_set_location (call_stmt, UNKNOWN_LOCATION);
      gimple_seq_add_stmt (seq, call_stmt);
      return;
    }

  /* GIMPLE call with lhs.  */
  if (c_parser_next_token_is (parser, CPP_NAME)
      && c_parser_peek_2nd_token (parser)->type == CPP_OPEN_PAREN
      && lookup_name (c_parser_peek_token (parser)->value))
    {
      rhs = c_parser_gimple_unary_expression (parser);
      gimple *call = gimple_build_call_from_tree (rhs.value);
      gimple_call_set_lhs (call, lhs.value);
      gimple_seq_add_stmt (seq, call);
      gimple_set_location (call, loc);
      return;
    }

  rhs = c_parser_gimple_binary_expression (parser, &subcode);

  if (lhs.value != error_mark_node
      && rhs.value != error_mark_node)
    {
      if (subcode == NOP_EXPR)
	assign = gimple_build_assign (lhs.value, rhs.value);
      else
	assign = gimple_build_assign (lhs.value, subcode,
				      TREE_OPERAND (rhs.value, 0),
				      TREE_OPERAND (rhs.value, 1));
      gimple_seq_add_stmt (seq, assign);
      gimple_set_location (assign, loc);
    }
  return;
}

/* Parse gimple binary expr.

   gimple-multiplicative-expression:
     gimple-unary-expression * gimple-unary-expression
     gimple-unary-expression / gimple-unary-expression
     gimple-unary-expression % gimple-unary-expression

   gimple-additive-expression:
     gimple-unary-expression + gimple-unary-expression
     gimple-unary-expression - gimple-unary-expression

   gimple-shift-expression:
     gimple-unary-expression << gimple-unary-expression
     gimple-unary-expression >> gimple-unary-expression

   gimple-relational-expression:
     gimple-unary-expression < gimple-unary-expression
     gimple-unary-expression > gimple-unary-expression
     gimple-unary-expression <= gimple-unary-expression
     gimple-unary-expression >= gimple-unary-expression

   gimple-equality-expression:
     gimple-unary-expression == gimple-unary-expression
     gimple-unary-expression != gimple-unary-expression

   gimple-AND-expression:
     gimple-unary-expression & gimple-unary-expression

   gimple-exclusive-OR-expression:
     gimple-unary-expression ^ gimple-unary-expression

   gimple-inclusive-OR-expression:
     gimple-unary-expression | gimple-unary-expression

   gimple-logical-AND-expression:
     gimple-unary-expression && gimple-unary-expression

   gimple-logical-OR-expression:
     gimple-unary-expression || gimple-unary-expression

*/

static c_expr
c_parser_gimple_binary_expression (c_parser *parser, enum tree_code *subcode)
{
  struct {
    /* The expression at this stack level.  */
    struct c_expr expr;
    /* The operation on its left.  */
    enum tree_code op;
    /* The source location of this operation.  */
    location_t loc;
  } stack[2];
  int sp;
  /* Location of the binary operator.  */
  location_t binary_loc = UNKNOWN_LOCATION;  /* Quiet warning.  */
#define POP								      \
  do {									      \
    if (sp == 1								      \
	&& c_parser_peek_token (parser)->type == CPP_SEMICOLON		      \
	&& (((1 << PREC_BITOR) | (1 << PREC_BITXOR) | (1 << PREC_BITAND)      \
	       | (1 << PREC_SHIFT) | (1 << PREC_ADD) | (1 << PREC_MULT)))     \
	&& stack[sp].op != TRUNC_MOD_EXPR				      \
	&& stack[0].expr.value != error_mark_node			      \
	&& stack[1].expr.value != error_mark_node)			      \
      stack[0].expr.value						      \
	= build2 (stack[1].op, TREE_TYPE (stack[0].expr.value),		      \
		  stack[0].expr.value, stack[1].expr.value);		      \
    else								      \
      stack[sp - 1].expr = parser_build_binary_op (stack[sp].loc,	      \
						   stack[sp].op,	      \
						   stack[sp - 1].expr,	      \
						   stack[sp].expr);	      \
    sp--;								      \
  } while (0)
  stack[0].loc = c_parser_peek_token (parser)->location;
  stack[0].expr = c_parser_gimple_unary_expression (parser);
  sp = 0;
  source_range src_range;
  if (parser->error)
    goto out;
  switch (c_parser_peek_token (parser)->type)
    {
    case CPP_MULT:
      *subcode = MULT_EXPR;
      break;
    case CPP_DIV:
      *subcode = TRUNC_DIV_EXPR;
      break;
    case CPP_MOD:
      *subcode = TRUNC_MOD_EXPR;
      break;
    case CPP_PLUS:
      *subcode = PLUS_EXPR;
      break;
    case CPP_MINUS:
      *subcode = MINUS_EXPR;
      break;
    case CPP_LSHIFT:
      *subcode = LSHIFT_EXPR;
      break;
    case CPP_RSHIFT:
      *subcode = RSHIFT_EXPR;
      break;
    case CPP_LESS:
      *subcode = LT_EXPR;
      break;
    case CPP_GREATER:
      *subcode = GT_EXPR;
      break;
    case CPP_LESS_EQ:
      *subcode = LE_EXPR;
      break;
    case CPP_GREATER_EQ:
      *subcode = GE_EXPR;
      break;
    case CPP_EQ_EQ:
      *subcode = EQ_EXPR;
      break;
    case CPP_NOT_EQ:
      *subcode = NE_EXPR;
      break;
    case CPP_AND:
      *subcode = BIT_AND_EXPR;
      break;
    case CPP_XOR:
      *subcode = BIT_XOR_EXPR;
      break;
    case CPP_OR:
      *subcode = BIT_IOR_EXPR;
      break;
    case CPP_AND_AND:
      *subcode = TRUTH_ANDIF_EXPR;
      break;
    case CPP_OR_OR:
      *subcode = TRUTH_ORIF_EXPR;
      break;
    default:
      /* Not a binary operator, so end of the binary expression.  */
      *subcode = NOP_EXPR;
      goto out;
    }
  binary_loc = c_parser_peek_token (parser)->location;
  c_parser_consume_token (parser);
  switch (*subcode)
    {
    case TRUTH_ANDIF_EXPR:
      src_range = stack[sp].expr.src_range;
      stack[sp].expr.value = c_objc_common_truthvalue_conversion
	(stack[sp].loc, default_conversion (stack[sp].expr.value));
      set_c_expr_source_range (&stack[sp].expr, src_range);
      break;
    case TRUTH_ORIF_EXPR:
      src_range = stack[sp].expr.src_range;
      stack[sp].expr.value = c_objc_common_truthvalue_conversion
	(stack[sp].loc, default_conversion (stack[sp].expr.value));
      set_c_expr_source_range (&stack[sp].expr, src_range);
      break;
    default:
      break;
    }
  sp++;
  stack[sp].loc = binary_loc;
  stack[sp].expr = c_parser_gimple_unary_expression (parser);
  stack[sp].op = *subcode;
out:
  while (sp > 0)
    POP;
  return stack[0].expr;
#undef POP
}

/* Parse gimple unary expression.

   gimple-unary-expression:
     gimple-postfix-expression
     unary-operator cast-expression

   unary-operator: one of
     & * + - ~ !
*/

static c_expr
c_parser_gimple_unary_expression (c_parser *parser)
{
  struct c_expr ret, op;
  if (c_parser_peek_token (parser)->value
      && TREE_CODE (c_parser_peek_token (parser)->value) == IDENTIFIER_NODE
      && ! lookup_name (c_parser_peek_token (parser)->value))
    return c_parser_parse_ssa_names (parser);

  location_t op_loc = c_parser_peek_token (parser)->location;
  location_t finish;
  ret.original_code = ERROR_MARK;
  ret.original_type = NULL;
  switch (c_parser_peek_token (parser)->type)
    {
    case CPP_AND:
      c_parser_consume_token (parser);
      op = c_parser_cast_expression (parser, NULL);
      mark_exp_read (op.value);
      return parser_build_unary_op (op_loc, ADDR_EXPR, op);
    case CPP_MULT:
      {
	c_parser_consume_token (parser);
	op = c_parser_cast_expression (parser, NULL);
	finish = op.get_finish ();
	location_t combined_loc = make_location (op_loc, op_loc, finish);
	ret.value = build_indirect_ref (combined_loc, op.value,
					RO_UNARY_STAR);
	ret.src_range.m_start = op_loc;
	ret.src_range.m_finish = finish;
	return ret;
      }
    case CPP_PLUS:
      c_parser_consume_token (parser);
      op = c_parser_cast_expression (parser, NULL);
      return parser_build_unary_op (op_loc, CONVERT_EXPR, op);
    case CPP_MINUS:
      c_parser_consume_token (parser);
      op = c_parser_cast_expression (parser, NULL);
      return parser_build_unary_op (op_loc, NEGATE_EXPR, op);
    case CPP_COMPL:
      c_parser_consume_token (parser);
      op = c_parser_cast_expression (parser, NULL);
      return parser_build_unary_op (op_loc, BIT_NOT_EXPR, op);
    case CPP_NOT:
      c_parser_consume_token (parser);
      op = c_parser_cast_expression (parser, NULL);
      return parser_build_unary_op (op_loc, TRUTH_NOT_EXPR, op);
    default:
      return c_parser_gimple_postfix_expression (parser);
    }
}

/* Parse gimple ssa names.  */

static c_expr
c_parser_parse_ssa_names (c_parser *parser)
{
  tree id = NULL_TREE;
  c_expr ret;
  char *var_name = NULL, *var_version = NULL, *token = NULL;
  ret.original_code = ERROR_MARK;
  ret.original_type = NULL;

  /* SSA token string.  */
  const char *ssa_token
    = IDENTIFIER_POINTER (c_parser_peek_token (parser)->value);
  token = new char [strlen (ssa_token) + 1];
  strcpy (token, ssa_token);

  /* Separate var name and version.  */
  var_version = strrchr (token, '_');
  if (var_version)
    {
      var_name = new char[var_version - token + 1];
      memcpy (var_name, token, var_version - token);
      var_name[var_version - token] = '\0';
      id = get_identifier (var_name);

      /* lookup for parent decl.  */
      if (lookup_name (id))
	{
	  var_version++;
	  unsigned int version;
	  version = atoi (var_version);
	  if (var_version && version)
	    {
	      ret.value = NULL_TREE;
	      if (version < num_ssa_names)
		ret.value = ssa_name (version);
	      if (! ret.value)
		ret.value = make_ssa_name_fn (cfun, lookup_name (id),
					      gimple_build_nop (), version);
	      c_parser_consume_token (parser);
	    }
	}
    }

  /* For default definition SSA names.  */
  if (c_parser_next_token_is (parser, CPP_OPEN_PAREN))
    {
      c_parser_consume_token (parser);
      ssa_token = IDENTIFIER_POINTER (c_parser_peek_token (parser)->value);
      if (! strcmp ("D", ssa_token))
	{
	  set_ssa_default_def (cfun, lookup_name (id), ret.value);
	  c_parser_consume_token (parser);
	}
      if (! c_parser_require (parser, CPP_CLOSE_PAREN, "expected %<)%>"))
	goto out;
    }

  out:
  free (var_name);
  free (token);
  return ret;
}

/* Parse gimple postfix expression.

   gimple-postfix-expression:
     gimple-primary-expression
     gimple-primary-xpression [ gimple-primary-expression ]
     gimple-primary-expression ( gimple-argument-expression-list[opt] )

   gimple-argument-expression-list:
     gimple-unary-expression
     gimple-argument-expression-list , gimple-unary-expression

   gimple-primary-expression:
     identifier
     constant
     string-literal

*/

static struct c_expr
c_parser_gimple_postfix_expression (c_parser *parser)
{
  struct c_expr expr;
  location_t loc = c_parser_peek_token (parser)->location;
  source_range tok_range = c_parser_peek_token (parser)->get_range ();
  expr.original_code = ERROR_MARK;
  expr.original_type = NULL;
  switch (c_parser_peek_token (parser)->type)
    {
    case CPP_NUMBER:
      expr.value = c_parser_peek_token (parser)->value;
      set_c_expr_source_range (&expr, tok_range);
      loc = c_parser_peek_token (parser)->location;
      c_parser_consume_token (parser);
      break;
    case CPP_CHAR:
    case CPP_CHAR16:
    case CPP_CHAR32:
    case CPP_WCHAR:
      expr.value = c_parser_peek_token (parser)->value;
      set_c_expr_source_range (&expr, tok_range);
      c_parser_consume_token (parser);
      break;
    case CPP_STRING:
    case CPP_STRING16:
    case CPP_STRING32:
    case CPP_WSTRING:
    case CPP_UTF8STRING:
      expr.value = c_parser_peek_token (parser)->value;
      set_c_expr_source_range (&expr, tok_range);
      expr.original_code = STRING_CST;
      c_parser_consume_token (parser);
      break;
    case CPP_NAME:
      if (c_parser_peek_token (parser)->id_kind == C_ID_ID)
	{
	  tree id = c_parser_peek_token (parser)->value;
	  c_parser_consume_token (parser);
	  expr.value = build_external_ref (loc, id,
					   (c_parser_peek_token (parser)->type
					    == CPP_OPEN_PAREN),
					   &expr.original_type);
	  set_c_expr_source_range (&expr, tok_range);
	  break;
	}
      else
	{
	  c_parser_error (parser, "expected expression");
	  expr.set_error ();
	  break;
	}
      break;
    default:
      c_parser_error (parser, "expected expression");
      expr.set_error ();
      break;
    }
  return c_parser_gimple_postfix_expression_after_primary
    (parser, EXPR_LOC_OR_LOC (expr.value, loc), expr);
}

/* Parse a gimple postfix expression after the initial primary or compound
   literal.  */

static struct c_expr
c_parser_gimple_postfix_expression_after_primary (c_parser *parser,
						  location_t expr_loc,
						  struct c_expr expr)
{
  struct c_expr orig_expr;
  vec<tree, va_gc> *exprlist;
  vec<tree, va_gc> *origtypes = NULL;
  vec<location_t> arg_loc = vNULL;
  location_t start;
  location_t finish;

  location_t op_loc = c_parser_peek_token (parser)->location;

  switch (c_parser_peek_token (parser)->type)
    {
    case CPP_OPEN_SQUARE:
      {
	c_parser_consume_token (parser);
	tree idx = c_parser_gimple_unary_expression (parser).value;

	if (! c_parser_require (parser, CPP_CLOSE_SQUARE, "expected %<]%>"))
	  break;

	start = expr.get_start ();
	finish = parser->tokens_buf[0].location;
	expr.value = build_array_ref (op_loc, expr.value, idx);
	set_c_expr_source_range (&expr, start, finish);

	expr.original_code = ERROR_MARK;
	expr.original_type = NULL;
	break;
      }
    case CPP_OPEN_PAREN:
      {
	/* Function call.  */
	c_parser_consume_token (parser);
	if (c_parser_next_token_is (parser, CPP_CLOSE_PAREN))
	  exprlist = NULL;
	else
	  exprlist = c_parser_gimple_expr_list (parser, &origtypes,
						&arg_loc);
	c_parser_skip_until_found (parser, CPP_CLOSE_PAREN,
				   "expected %<)%>");
	orig_expr = expr;
	start = expr.get_start ();
	finish = parser->tokens_buf[0].get_finish ();
	expr.value = c_build_function_call_vec (expr_loc, arg_loc, expr.value,
						exprlist, origtypes);
	set_c_expr_source_range (&expr, start, finish);

	expr.original_code = ERROR_MARK;
	if (TREE_CODE (expr.value) == INTEGER_CST
	    && TREE_CODE (orig_expr.value) == FUNCTION_DECL
	    && DECL_BUILT_IN_CLASS (orig_expr.value) == BUILT_IN_NORMAL
	    && DECL_FUNCTION_CODE (orig_expr.value) == BUILT_IN_CONSTANT_P)
	  expr.original_code = C_MAYBE_CONST_EXPR;
	expr.original_type = NULL;
	if (exprlist)
	  {
	    release_tree_vector (exprlist);
	    release_tree_vector (origtypes);
	  }
	arg_loc.release ();
	break;
      default:
	return expr;
      }
    }
  return expr;
}

/* Parse expression list.

   gimple-expr-list:
     gimple-unary-expression
     gimple-expr-list , gimple-unary-expression

 */

static vec<tree, va_gc> *
c_parser_gimple_expr_list (c_parser *parser, vec<tree, va_gc> **p_orig_types,
			   vec<location_t> *locations)
{
  vec<tree, va_gc> *ret;
  vec<tree, va_gc> *orig_types;
  struct c_expr expr;
  location_t loc = c_parser_peek_token (parser)->location;

  ret = make_tree_vector ();
  if (p_orig_types == NULL)
    orig_types = NULL;
  else
    orig_types = make_tree_vector ();

  expr = c_parser_gimple_unary_expression (parser);
  vec_safe_push (ret, expr.value);
  if (orig_types)
    vec_safe_push (orig_types, expr.original_type);
  if (locations)
    locations->safe_push (loc);
  while (c_parser_next_token_is (parser, CPP_COMMA))
    {
      c_parser_consume_token (parser);
      loc = c_parser_peek_token (parser)->location;
      expr = c_parser_gimple_unary_expression (parser);
      vec_safe_push (ret, expr.value);
      if (orig_types)
	vec_safe_push (orig_types, expr.original_type);
      if (locations)
	locations->safe_push (loc);
    }
  if (orig_types)
    *p_orig_types = orig_types;
  return ret;
}

/* Parse gimple label.

   gimple-label:
     identifier :
     case constant-expression :
     default :

*/

static void
c_parser_gimple_label (c_parser *parser, gimple_seq *seq)
{
  tree name = c_parser_peek_token (parser)->value;
  location_t loc1 = c_parser_peek_token (parser)->location;
  gcc_assert (c_parser_next_token_is (parser, CPP_NAME));
  c_parser_consume_token (parser);
  gcc_assert (c_parser_next_token_is (parser, CPP_COLON));
  c_parser_consume_token (parser);
  tree label = define_label (loc1, name);
  gimple_seq_add_stmt (seq, gimple_build_label (label));
  return;
}

/* Parse gimple pass list.

   gimple-pass-list:
     startwith("pass-name")
 */

void
c_parser_gimple_pass_list (c_parser *parser, char **pass)
{
  if (! c_parser_require (parser, CPP_OPEN_PAREN, "expected %<(%>"))
    return;

  if (c_parser_next_token_is (parser, CPP_CLOSE_PAREN))
    return;

  if (c_parser_next_token_is (parser, CPP_NAME))
    {
      const char *op = IDENTIFIER_POINTER (c_parser_peek_token (parser)->value);
      c_parser_consume_token (parser);
      if (! strcmp (op, "startwith"))
	{
	  if (! c_parser_require (parser, CPP_OPEN_PAREN, "expected %<(%>"))
	    return;
	  if (c_parser_next_token_is_not (parser, CPP_STRING))
	    {
	      error_at (c_parser_peek_token (parser)->location,
			"expected pass name");
	      return;
	    }
	  *pass = xstrdup (TREE_STRING_POINTER
				(c_parser_peek_token (parser)->value));
	  c_parser_consume_token (parser);
	  if (! c_parser_require (parser, CPP_CLOSE_PAREN, "expected %<)%>"))
	    return;
	}
      else
	{
	  error_at (c_parser_peek_token (parser)->location,
		    "invalid operation");
	  return;
	}
    }
  else if (c_parser_next_token_is (parser, CPP_EOF))
    {
      c_parser_error (parser, "expected parameters");
      return;
    }

  return;
}

/* Parse gimple local declaration.

   declaration-specifiers:
     storage-class-specifier declaration-specifiers[opt]
     type-specifier declaration-specifiers[opt]
     type-qualifier declaration-specifiers[opt]
     function-specifier declaration-specifiers[opt]
     alignment-specifier declaration-specifiers[opt]

   storage-class-specifier:
     typedef
     extern
     static
     auto
     register

   type-specifier:
     void
     char
     short
     int
     long
     float
     double
     signed
     unsigned
     _Bool
     _Complex

   type-qualifier:
     const
     restrict
     volatile
     address-space-qualifier
     _Atomic

 */

static void
c_parser_gimple_declaration (c_parser *parser)
{
  struct c_declarator *declarator;
  struct c_declspecs *specs = build_null_declspecs ();
  c_parser_declspecs (parser, specs, true, true, true,
		      true, true, cla_nonabstract_decl);
  finish_declspecs (specs);

  /* Provide better error recovery.  Note that a type name here is usually
     better diagnosed as a redeclaration.  */
  if (c_parser_next_token_starts_declspecs (parser)
      && ! c_parser_next_token_is (parser, CPP_NAME))
    {
      c_parser_error (parser, "expected %<;%>");
      parser->error = false;
      return;
    }

  bool dummy = false;
  declarator = c_parser_declarator (parser,
				    specs->typespec_kind != ctsk_none,
				    C_DTR_NORMAL, &dummy);

  if (c_parser_next_token_is (parser, CPP_SEMICOLON))
    {
      tree postfix_attrs = NULL_TREE;
      tree all_prefix_attrs = specs->attrs;
      specs->attrs = NULL;
      tree decl = start_decl (declarator, specs, false,
			 chainon (postfix_attrs, all_prefix_attrs));
      if (decl)
	finish_decl (decl, UNKNOWN_LOCATION, NULL_TREE, NULL_TREE, NULL_TREE);
    }
  else
    {
      c_parser_error (parser, "expected %<;%>");
      return;
    }
}

/* Parse gimple goto statement.  */

static void
c_parser_gimple_goto_stmt (location_t loc, tree label, gimple_seq *seq)
{
  tree decl = lookup_label_for_goto (loc, label);
  gimple_seq_add_stmt (seq, gimple_build_goto (decl));
  return;
}

/* Parse a parenthesized condition.
   gimple-condition:
     ( gimple-binary-expression )    */

static tree
c_parser_gimple_paren_condition (c_parser *parser)
{
  enum tree_code subcode = NOP_EXPR;
  location_t loc = c_parser_peek_token (parser)->location;
  if (! c_parser_require (parser, CPP_OPEN_PAREN, "expected %<(%>"))
    return error_mark_node;
  tree cond = c_parser_gimple_binary_expression (parser, &subcode).value;
  cond = c_objc_common_truthvalue_conversion (loc, cond);
  if (! c_parser_require (parser, CPP_CLOSE_PAREN, "expected %<)%>"))
    return error_mark_node;
  return cond;
}

/* Parse gimple if-else statement.

   if-statement:
     if ( gimple-binary-expression ) gimple-goto-statement
     if ( gimple-binary-expression ) gimple-goto-statement \
					else gimple-goto-statement
 */

static void
c_parser_gimple_if_stmt (c_parser *parser, gimple_seq *seq)
{
  tree t_label, f_label, label;
  location_t loc;
  c_parser_consume_token (parser);
  tree cond = c_parser_gimple_paren_condition (parser);

  if (c_parser_next_token_is_keyword (parser, RID_GOTO))
    {
      loc = c_parser_peek_token (parser)->location;
      c_parser_consume_token (parser);
      label = c_parser_peek_token (parser)->value;
      t_label = lookup_label_for_goto (loc, label);
      c_parser_consume_token (parser);
      if (! c_parser_require (parser, CPP_SEMICOLON, "expected %<;%>"))
	return;
    }
  else
    {
      c_parser_error (parser, "expected goto expression");
      return;
    }

  if (c_parser_next_token_is_keyword (parser, RID_ELSE))
    c_parser_consume_token (parser);
  else
    {
      c_parser_error (parser, "expected else statement");
      return;
    }

  if (c_parser_next_token_is_keyword (parser, RID_GOTO))
    {
      loc = c_parser_peek_token (parser)->location;
      c_parser_consume_token (parser);
      label = c_parser_peek_token (parser)->value;
      f_label = lookup_label_for_goto (loc, label);
      c_parser_consume_token (parser);
      if (! c_parser_require (parser, CPP_SEMICOLON, "expected %<;%>"))
	return;
    }
  else
    {
      c_parser_error (parser, "expected goto expression");
      return;
    }

  gimple_seq_add_stmt (seq, gimple_build_cond_from_tree (cond, t_label,
							 f_label));
}

/* Parse gimple switch-statement.

   gimple-switch-statement:
     switch (gimple-unary-expression) gimple-case-statement

   gimple-case-statement:
     gimple-case-statement
     gimple-label-statement : gimple-goto-statment
*/

static void
c_parser_gimple_switch_stmt (c_parser *parser, gimple_seq *seq)
{
  c_expr cond_expr;
  tree case_label, label;
  auto_vec<tree> labels;
  tree default_label = NULL_TREE;
  gimple_seq switch_body = NULL;
  c_parser_consume_token (parser);

  if (c_parser_require (parser, CPP_OPEN_PAREN, "expected %<(%>"))
    {
      cond_expr = c_parser_gimple_unary_expression (parser);
      if (! c_parser_require (parser, CPP_CLOSE_PAREN, "expected %<)%>"))
	return;
    }

  if (c_parser_require (parser, CPP_OPEN_BRACE, "expected %<{%>"))
    {
      while (c_parser_next_token_is_not (parser, CPP_CLOSE_BRACE))
	{
	  if (c_parser_next_token_is (parser, CPP_EOF))
	    {
	      c_parser_error (parser, "expected statement");
	      return;
	    }

	  switch (c_parser_peek_token (parser)->keyword)
	    {
	    case RID_CASE:
	      {
		c_expr exp1;
		location_t loc = c_parser_peek_token (parser)->location;
		c_parser_consume_token (parser);

		if (c_parser_next_token_is (parser, CPP_NAME)
		    || c_parser_peek_token (parser)->type == CPP_NUMBER)
		  exp1 = c_parser_gimple_unary_expression (parser);
		else
		  c_parser_error (parser, "expected expression");

		if (c_parser_next_token_is (parser, CPP_COLON))
		  {
		    c_parser_consume_token (parser);
		    if (c_parser_next_token_is (parser, CPP_NAME))
		      {
			label = c_parser_peek_token (parser)->value;
			c_parser_consume_token (parser);
			tree decl = lookup_label_for_goto (loc, label);
			case_label = build_case_label (exp1.value, NULL_TREE,
						       decl);
			labels.safe_push (case_label);
			if (! c_parser_require (parser, CPP_SEMICOLON,
						"expected %<;%>"))
			  return;
		      }
		    else if (! c_parser_require (parser, CPP_NAME,
						 "expected label"))
		      return;
		  }
		else if (! c_parser_require (parser, CPP_SEMICOLON,
					    "expected %<:%>"))
		  return;
		break;
	      }
	    case RID_DEFAULT:
	      {
		location_t loc = c_parser_peek_token (parser)->location;
		c_parser_consume_token (parser);
		if (c_parser_next_token_is (parser, CPP_COLON))
		  {
		    c_parser_consume_token (parser);
		    if (c_parser_next_token_is (parser, CPP_NAME))
		      {
			label = c_parser_peek_token (parser)->value;
			c_parser_consume_token (parser);
			tree decl = lookup_label_for_goto (loc, label);
			default_label = build_case_label (NULL_TREE, NULL_TREE,
							  decl);
			if (! c_parser_require (parser, CPP_SEMICOLON,
						"expected %<;%>"))
			  return;
		      }
		    else if (! c_parser_require (parser, CPP_NAME,
						 "expected label"))
		      return;
		  }
		else if (! c_parser_require (parser, CPP_SEMICOLON,
					    "expected %<:%>"))
		  return;
		break;
	      }
	    case RID_GOTO:
	      {
		location_t loc = c_parser_peek_token (parser)->location;
		c_parser_consume_token (parser);
		if (c_parser_next_token_is (parser, CPP_NAME))
		  {
		    c_parser_gimple_goto_stmt (loc,
					       c_parser_peek_token
					         (parser)->value,
					       &switch_body);
		    c_parser_consume_token (parser);
		    if (c_parser_next_token_is (parser, CPP_SEMICOLON))
		      c_parser_consume_token (parser);
		    else
		      {
			c_parser_error (parser, "expected semicolon");
			return;
		      }
		  }
		else if (! c_parser_require (parser, CPP_NAME,
					    "expected label"))
		  return;
		break;
	      }
	    default:
	      c_parser_error (parser, "expected case label or goto statement");
	      return;
	    }

	}
    }
  if (! c_parser_require (parser, CPP_CLOSE_BRACE, "expected %<}%>"))
    return;
  gimple_seq_add_stmt (seq, gimple_build_switch (cond_expr.value,
						 default_label, labels));
  gimple_seq_add_seq (seq, switch_body);
  labels.release();
}

/* Parse gimple return statement.  */

static void
c_parser_gimple_return_stmt (c_parser *parser, gimple_seq *seq)
{
  location_t loc = c_parser_peek_token (parser)->location;
  gimple *ret = NULL;
  c_parser_consume_token (parser);
  if (c_parser_next_token_is (parser, CPP_SEMICOLON))
    {
      c_finish_gimple_return (loc, NULL_TREE);
      ret = gimple_build_return (NULL);
      gimple_seq_add_stmt (seq, ret);
    }
  else
    {
      location_t xloc = c_parser_peek_token (parser)->location;
      c_expr expr = c_parser_gimple_unary_expression (parser);
      c_finish_gimple_return (xloc, expr.value);
      ret = gimple_build_return (expr.value);
      gimple_seq_add_stmt (seq, ret);
    }
}

/* Support function for c_parser_gimple_return_stmt.  */

static void
c_finish_gimple_return (location_t loc, tree retval)
{
  tree valtype = TREE_TYPE (TREE_TYPE (current_function_decl));

  /* Use the expansion point to handle cases such as returning NULL
     in a function returning void.  */
  source_location xloc = expansion_point_location_if_in_system_header (loc);

  if (TREE_THIS_VOLATILE (current_function_decl))
    warning_at (xloc, 0,
		"function declared %<noreturn%> has a %<return%> statement");

  if (! retval)
    current_function_returns_null = 1;
  else if (valtype == 0 || TREE_CODE (valtype) == VOID_TYPE)
    {
      current_function_returns_null = 1;
      if (TREE_CODE (TREE_TYPE (retval)) != VOID_TYPE)
	{
	  error_at
	    (xloc, "%<return%> with a value, in function returning void");
	  inform (DECL_SOURCE_LOCATION (current_function_decl),
		  "declared here");
	}
    }
  else if (TREE_CODE (valtype) != TREE_CODE (TREE_TYPE (retval)))
    {
      error_at
	(xloc, "invalid conversion in return statement");
      inform (DECL_SOURCE_LOCATION (current_function_decl),
	      "declared here");
    }
  return;
}
